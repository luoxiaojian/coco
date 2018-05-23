#include <stdio.h>

#include <string>

#include "graph.h"

int main(int argc, char **argv) {
  if (argc < 3) {
    printf("usage: ./parser <source> <prefix>\n");
  }

  std::string prefix = argv[2];
  std::string vfile = prefix + ".nodes";
  std::string efile = prefix + ".edges";

  FILE *fin = fopen(argv[1], "r");
  FILE *foutv = fopen(vfile.c_str(), "wb");
  FILE *foute = fopen(efile.c_str(), "wb");

  vid_t src, dst;
  vid_t cur_src = std::numeric_limits<vid_t>::max();
  unsigned count = 0;
  while (fscanf(fin, "%u%u", &src, &dst) != EOF) {
    if (cur_src != src) {
      if (count > 0) {
        fwrite(&cur_src, sizeof(vid_t), 1, foutv);
        fwrite(&count, sizeof(unsigned), 1, foutv);
      }
      cur_src = src;
    }
    fwrite(&dst, sizeof(vid_t), 1, foute);
    count++;
  }

  if (count > 0) {
    fwrite(&cur_src, sizeof(vid_t), 1, foutv);
  }

  fflush(foutv);
  fflush(foute);

  fclose(foutv);
  fclose(foute);

  return 0;
}
