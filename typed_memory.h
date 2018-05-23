#ifndef TYPED_MEMORY_H_
#define TYPED_MEMORY_H_

#include <dirent.h>
#include <errno.h>
#include <sys/errno.h>
#include <sys/fcntl.h>
#include <sys/file.h>
#include <sys/mman.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/unistd.h>
#include <unistd.h>
#include <string.h>

template <typename T>
class TypedMemory {
 public:
  TypedMemory() : inited_(false), buffer_(NULL), size_(0) {}
  ~TypedMemory() {
    if (inited_) {
      munmap(buffer_, size_in_byte_);
    }
  }

  bool Open(const char* path) {
    if (inited_) {
      Close();
    }
    if (!CheckPath(path)) {
      printf("failed check path\n");
      return false;
    }
    int fd = open(path_, O_RDONLY);
    if (fd == -1) {
      printf("failed open file\n");
      return false;
    }
    struct stat st;
    fstat(fd, &st);
    size_in_byte_ = st.st_size;
    size_ = size_in_byte_ / sizeof(T);
    size_in_byte_ = size_ * sizeof(T);

    buffer_ = reinterpret_cast<T*>(
        mmap(NULL, size_in_byte_, PROT_READ, MAP_PRIVATE, fd, 0));
    inited_ = true;
    close(fd);
    return true;
  }

  void Create(const char* path, size_t size) {
    if (inited_) {
      Close();
    }
    if (!CheckPath(path)) {
      return false;
    }
    int fd = open(path_, O_RDONLY);
    if (fd != -1) {
      close(fd);
      return false;
    }
    fd = open(path_, O_CREAT | O_RDWR,
              S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    if (fd == -1) {
      return false;
    }
    size_ = size;
    size_in_byte_ = size * sizeof(T);
    if (ftruncate(fd, size_in_byte_)) {
      return false;
    }
    buffer_ = reinterpret_cast<T*>(
        mmap(NULL, size_in_byte_, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0));
    close(fd);
    inited_ = true;
    return true;
  }

  void Close() {
    if (inited_) {
      munmap(buffer_, size_in_byte_);
    }
    buffer_ = NULL;
    inited_ = false;
  }

  inline size_t Size() { return size_; }

  inline T& operator[](size_t loc) { return buffer_[loc]; }

 private:
  bool CheckPath(const char* path) {
    if (strlen(path) > sizeof(path_) - 1) {
      return false;
    } else {
      strncpy(&path_[0], path, sizeof(path_) - 1);
      return true;
    }
  }

  bool inited_;
  T* buffer_;
  size_t size_;
  size_t size_in_byte_;
  char path_[256];
};

#endif
