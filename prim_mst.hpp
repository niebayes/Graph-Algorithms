#ifndef PRIM_MST_HPP_
#define PRIM_MST_HPP_

#include <cassert>
#include <queue>
#include <unordered_set>

#include "def.h"
#include "graph.hpp"

// Prim minimum spanning tree algorithm.
// it works as below:
// (1) select a source vertex.
// (2) init a priority queue with all connected edges of the source vertex.
//     the queue is a min-heap, i.e. edges with smaller weightes are placed at
//     the top.
// (3) repeat until the queue is empty: pop and examine the top edge in the
//     queue. If the outgoing vertex of the edge is not in the mst currently,
//     add all connected edges of the outgoing vertex into the queue. This edge
//     is added into the mst.
/// @param g connected graph, i.e. all vertices must be connected.
/// @return the minimum spanning tree of the graph g.
Graph prim_min_span_tree(const Graph& g) {
  const std::list<int> vertices = g.all_vertices();
  assert(!vertices.empty());
  const int src = vertices.front();

  std::priority_queue<Edge, std::vector<Edge>, Edge::greater> pq;
  for (const Edge& e : g.edges(src)) {
    pq.push(e);
  }

  // helper hash set to contain visited vertices.
  std::unordered_set<int> visited;
  visited.insert(src);

  Graph mst;
  for (const int& v : g.all_vertices()) {
    mst.add_vertex(v);
  }

  while (!pq.empty()) {
    const Edge e = pq.top();
    pq.pop();

    // if the w vertex is already visited which says there's a path linking e.v
    // and e.w, so adding this edge e will introduce another path linking e.v
    // and e.w and hence violates tree properties.
    // from the perspective of a DFS generating tree, such an edge is a back
    // edge which introduces a loop and hence cannot be added.
    if (visited.count(e.w) == 0) {
      mst.add_edge(e);
      mst.add_edge(e.reversed());
      visited.insert(e.w);
      total_cost += e.weight;
      // push new edges into the queue.
      for (const Edge& ne : g.edges(e.w)) {
        pq.push(ne);
      }
    }
  }

  return mst;
}

#endif  // PRIM_MST_HPP_