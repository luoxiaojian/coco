#ifndef GRAPH_H_
#define GRAPH_H_

#include <functional>
#include <utility>

#include "typed_memory.h"
#include "func.h"

using vid_t = uint32_t;

class NEGraph {
 public:
  NEGraph(const char* nodes_path, const char* edges_path) {
    if(!nodes_.Open(nodes_path)) {
      printf("failed to open nodes file\n");
    }
    if(!edges_.Open(edges_path)) {
      printf("failed to open edges file\n");
    }
    nodes_.Traverse();
    edges_.Traverse();
  }

  void MapEdges(std::function<void(vid_t, vid_t)> action) {
    size_t vnum = nodes_.Size();
    vid_t* e = &edges_[0];
    for (size_t i = 0; i < vnum; i++) {
      vid_t node = nodes_[i].first;
      unsigned count = nodes_[i].second;
      while (count) {
        action(node, *(e++));
        --count;
      }
    }
  }

  void MapEdges(Func &action) {
    size_t vnum = nodes_.Size();
    vid_t* e = &edges_[0];
    for (size_t i = 0; i < vnum; i++) {
      vid_t node = nodes_[i].first;
      unsigned count = nodes_[i].second;
      while (count) {
        action(node, *(e++));
        --count;
      }
    }
  }

  inline void func(vid_t *roots, uint8_t *ranks, vid_t x, vid_t y) {
    x = roots[x];
    y = roots[y];

    while(x != roots[x]) {
      x = roots[x];
    }

    while(y != roots[y]) {
      y = roots[y];
    }

    if(x != y) {
      if(ranks[x] < ranks[y]) {
        roots[x] = y;
      } else if(ranks[x] > ranks[y]) {
        roots[y] = x;
      } else {
        roots[y] = x;
        ranks[x]++;
      }
    }
  }

  void UnionFind(vid_t vnum) {
    vid_t *roots = reinterpret_cast<vid_t *>(malloc(sizeof(vid_t) * vnum));
    uint8_t *ranks = reinterpret_cast<uint8_t *>(malloc(sizeof(uint8_t) * vnum));
    for(vid_t i = 0; i < vnum; i++) {
      roots[i] = i;
    }
    memset(ranks, 0, sizeof(uint8_t) * vnum);

    size_t vn = nodes_.Size();
    vid_t *e = &edges_[0];
    for(size_t i = 0; i < vn; i++) {
      vid_t node = nodes_[i].first;
      unsigned count = nodes_[i].second;
      while(count) {
        func(roots, ranks, node, *(e++));
        count--;
      }
    }

    vid_t non_roots = 0;
    for(vid_t i = 0; i < vnum; i++) {
      if(i != roots[i]) {
        non_roots++;
      }
    }

    printf("%u non-roots found\n", non_roots);
  }

 private:
  TypedMemory<std::pair<vid_t, uint32_t>> nodes_;
  TypedMemory<vid_t> edges_;
};

#endif
