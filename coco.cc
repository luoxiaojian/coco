#include "graph.h"
#include "func.h"
#include "union_find.h"
#include "pagerank.h"
#include "stats.h"
#include "typed_memory.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include <string>

/*
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

  printf("Before calculating degree\n");
  graph.MapEdges(func1);
  printf("After calculating degree\n");

  int iter = 20;
  while (iter--) {
    printf("Iteration - %d", 20 - iter);
    for (vid_t i = 0; i < vnum; i++) {
      src[i] = alpha * dst[i] / deg[i];
      dst[i] = 1.0f - alpha;
    }
    graph.MapEdges(func2);
  }

  free(src);
  free(dst);
  free(deg);
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
*/

vid_t stats(NEGraph &graph) {
  vid_t max_x = 0;
  vid_t max_y = 0;
  size_t edges = 0;

  /*
  std::function<void(vid_t &, vid_t &, size_t &, vid_t, vid_t)> fc3 = f3;
  std::function<void(vid_t, vid_t)> func3 =
      std::bind(fc3, std::ref(max_x), std::ref(max_y), std::ref(edges),
                std::placeholders::_1, std::placeholders::_2);
  */

  Stats st(max_x, max_y, edges);
  graph.MapEdges(st);
  // graph.MapEdges(func3);

  printf("max x: %u\n", max_x);
  printf("max y: %u\n", max_y);
  printf("edges: %zu\n", edges);

  return max_x > max_y ? (max_x + 1) : (max_y + 1);
}

/*
void f4(vid_t *roots, uint8_t *ranks, vid_t x, vid_t y) {
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

void union_find(NEGraph &graph, vid_t vnum) {
  vid_t *roots = reinterpret_cast<vid_t *>(malloc(sizeof(vid_t) * vnum));
  uint8_t *ranks = reinterpret_cast<uint8_t *>(malloc(sizeof(uint8_t) * vnum));
  for(vid_t i = 0; i < vnum; i++) {
    roots[i] = i;
  }
  memset(ranks, 0, sizeof(uint8_t) * vnum);

  std::function<void(vid_t*, uint8_t*, vid_t, vid_t)> fc4 = f4;
  std::function<void(vid_t, vid_t)> func4 = std::bind(fc4, roots, ranks, std::placeholders::_1, std::placeholders::_2);

  graph.MapEdges(func4);

  vid_t non_roots = 0;
  for(vid_t i = 0; i < vnum; i++) {
    if(i != roots[i]) {
      non_roots++;
    }
  }

  printf("%u non-roots found\n", non_roots);
}

void uf2(NEGraph &graph, vid_t vnum) {
  vid_t *roots = reinterpret_cast<vid_t *>(malloc(sizeof(vid_t) * vnum));
  uint8_t *ranks = reinterpret_cast<uint8_t *>(malloc(sizeof(uint8_t) * vnum));
  for(vid_t i = 0; i < vnum; i++) {
    roots[i] = i;
  }
  memset(ranks, 0, sizeof(uint8_t) * vnum);
  UFFunc uffunc(roots, ranks);
  graph.MapEdgesUF(uffunc);
  vid_t non_roots = 0;
  for(vid_t i = 0; i < vnum; i++) {
    if(i != roots[i]) {
      non_roots++;
    }
  }

  printf("%u non-roots found\n", non_roots);
}

void uf3(NEGraph &graph, vid_t vnum) {
  vid_t *roots = reinterpret_cast<vid_t *>(malloc(sizeof(vid_t) * vnum));
  uint8_t *ranks = reinterpret_cast<uint8_t *>(malloc(sizeof(uint8_t) * vnum));
  for(vid_t i = 0; i < vnum; i++) {
    roots[i] = i;
  }
  memset(ranks, 0, sizeof(uint8_t) * vnum);
  UFFunc uffunc(roots, ranks);
  graph.MapEdgesFunc(uffunc);
  vid_t non_roots = 0;
  for(vid_t i = 0; i < vnum; i++) {
    if(i != roots[i]) {
      non_roots++;
    }
  }

  printf("%u non-roots found\n", non_roots);
}
*/

void union_find(NEGraph &graph, vid_t vnum) {
  vid_t *roots = reinterpret_cast<vid_t *>(malloc(sizeof(vid_t) * vnum));
  uint8_t *ranks = reinterpret_cast<uint8_t *>(malloc(sizeof(uint8_t) * vnum));
  for(vid_t i = 0; i < vnum; i++) {
    roots[i] = i;
  }
  memset(ranks, 0, sizeof(uint8_t) * vnum);

  UnionFind uf(roots, ranks);
  graph.MapEdges(uf);

  vid_t non_roots = 0;
  for(vid_t i = 0; i < vnum; i++) {
    if(i != roots[i]) {
      non_roots++;
    }
  }

  printf("%u non-roots found\n", non_roots);

  free(roots);
  free(ranks);
}

void pagerank(NEGraph &graph, vid_t vnum, float alpha) {
  float *src = reinterpret_cast<float *>(malloc(sizeof(float) * vnum));
  float *dst = reinterpret_cast<float *>(malloc(sizeof(float) * vnum));
  float *deg = reinterpret_cast<float *>(malloc(sizeof(float) * vnum));

  std::fill_n(src, vnum, 0.0f);
  std::fill_n(dst, vnum, 0.0f);
  std::fill_n(deg, vnum, 0.0f);

  CalcDegree cd(deg);
  graph.MapEdges(cd);

  Pagerank pr(src, dst);
  int iter = 20;
  while(iter--) {
    printf("Iteration - %d\n", 20 - iter);
    for(vid_t i = 0; i < vnum; i++) {
      src[i] = alpha * dst[i] / deg[i];
      dst[i] = 1.0f - alpha;
    }
    graph.MapEdges(pr);
  }

  free(src);
  free(dst);
  free(deg);
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
  vid_t vnum = stats(graph);

  printf("graph loaded...\n");

  struct timeval start, end;
  gettimeofday(&start, NULL);
  if (algo_name == "pagerank") {
    pagerank(graph, vnum, 0.8f);
  } else if(algo_name == "union_find") {
    union_find(graph, vnum);
  } else if(algo_name == "uf") {
    graph.UnionFind(vnum);
  } else {
    printf("algorithm not implemented...");
  }
  /*
  if (algo_name == "pagerank") {
    pagerank(graph, vnum, 0.8f);
  } else if(algo_name == "union_find") {
    union_find(graph, vnum);
  } else if(algo_name == "uf") {
    graph.UnionFind(vnum);
  } else if(algo_name == "uf2") {
    uf2(graph, vnum);
  } else if(algo_name == "uf3") {
    uf3(graph, vnum);
  } else {
    printf("algorithm not implemented...");
  }
  */
  gettimeofday(&end, NULL);

  double elapsed_time = (double)(end.tv_sec - start.tv_sec) + ((double)(end.tv_usec - start.tv_usec)) / 1000000.0;
  printf("elapsed time: %lf\n", elapsed_time);

  return 0;
}
