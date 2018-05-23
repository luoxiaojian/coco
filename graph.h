#ifndef GRAPH_H_
#define GRAPH_H_

#include <functional>
#include <utility>

#include "typed_memory.h"

using vid_t = uint32_t;

class NEGraph {
 public:
  NEGraph(const char* nodes_path, const char* edges_path) {
    nodes_.Open(nodes_path);
    edges_.Open(edges_path);
  }

  void MapEdges(std::function<void(vid_t, vid_t)> action) {
    size_t vnum = nodes_.Size();
    vid_t* e = &edges_[0];
    for (size_t i = 0; i < vnum; i++) {
      vid_t node = nodes_[i].first;
      unsigned count = nodes_[i].second;
      while (count) {
        action(node, *(e++));
        count--;
      }
    }
  }

 private:
  TypedMemory<std::pair<vid_t, uint32_t>> nodes_;
  TypedMemory<vid_t> edges_;
};

#endif
