#include "graph.h"
#include "typed_memory.h"

#include <stdio.h>
#include <stdlib.h>

#include <string>

void f1(float *deg, vid_t a, vid_t b) { deg[a] += 1.0f; }

void f2(float *src, float *dst, vid_t a, vid_t b) { dst[b] += src[a]; }

void pagerank(NEGraph &graph, vid_t vnum, float alpha) {
  float *src = reinterpret_cast<float *>(malloc(sizeof(float) * vnum));
  float *dst = reinterpret_cast<float *>(malloc(sizeof(float) * vnum));
  float *deg = reinterpret_cast<float *>(malloc(sizeof(float) * vnum));

  std::fill_n(src, vnum, 0.0f);
  std::fill_n(dst, vnum, 0.0f);
  std::fill_n(deg, vnum, 0.0f);

  std::function<void(float *, vid_t, vid_t)> fc1 = f1;
  std::function<void(vid_t, vid_t)> func1 =
      std::bind(fc1, deg, std::placeholders::_1, std::placeholders::_2);

  std::function<void(float *, float *, vid_t, vid_t)> fc2 = f2;
  std::function<void(vid_t, vid_t)> func2 =
      std::bind(fc2, src, dst, std::placeholders::_1, std::placeholders::_2);

  graph.MapEdges(func1);

  int iter = 20;
  while (iter--) {
    printf("Iteration - %d", 20 - iter);
    for (vid_t i = 0; i < vnum; i++) {
      src[i] = alpha * dst[i] / deg[i];
      dst[i] = 1f - alpha;
    }
    graph.MapEdges(func2);
  }
}

void f3(vid_t &max_x, vid_t &max_y, size_t &edges, vid_t x, vid_t y) {
  if (max_x < x) {
    max_x = x;
  }
  if (max_y < y) {
    max_y = y;
  }
  edges++;
}

vid_t stats(NEGraph &graph) {
  vid_t max_x = 0;
  vid_t max_y = 0;
  size_t edges = 0;

  std::function<void(vid_t &, vid_t &, size_t &, vid_t, vid_t)> fc3 = f3;
  std::function<void(vid_t, vid_t)> func3 =
      std::bind(fc3, std::ref(max_x), std::ref(max_y), std::ref(edges),
                std::placeholders::_1, std::placeholders::_2);

  graph.MapEdges(func3);

  printf("max x: %u\n", max_x);
  printf("max y: %u\n", max_y);
  printf("edges: %zu\n", edges);
}

int main(int argc, char **argv) {
  if (argc < 3) {
    printf("usage: ./coco <algo> <prefix>\n");
  }

  std::string algo_name = argv[1];
  std::string prefix = argv[2];

  std::string efile = prefix + ".edges";
  std::string vfile = prefix + ".nodes";

  NEGraph graph(vfile.c_str(), efile.c_str());

  if (algo_name == "pagerank") {
    pagerank(graph, stats(graph), 0.8f);
  } else {
    printf("algorithm not implemented...");
  }

  return 0;
}
