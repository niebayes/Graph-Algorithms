#ifndef KRUSKAL_MST_HPP_
#define KRUSKAL_MST_HPP_

#include "graph.hpp"
#include "union_find.hpp"

// kruskal minimum spanning tree algorithm.
// this algorithm works as below:
// (1) sort all edges by non-decreasing weight.
// (2) examine the next edge with the lowest weight.
// (2.a) if adding this edge won't form a loop in the mst, add it.
// (2.b) otherwise, discard it.
// (3) after examined all edges, the mst is constructed.
/// @param g connected graph, i.e. all vertices must be connected.
/// @return the minimum spanning tree of the graph g.
Graph kruskal_min_span_tree(const Graph& g) {
  std::list<Edge> all_edges = dedup_edges(g.all_edges());
  all_edges.sort(Edge::less);

  // helper union-find data structure to detect loops.
  UF uf;
  Graph mst;
  for (const int& v : g.all_vertices()) {
    uf.add_vertex(v);
    mst.add_vertex(v);
  }

  for (const Edge& e : all_edges) {
    // if v and w is already connected which says there's a path linking v and
    // w, then adding this edge e will introduce another path linking v and w
    // and hence violates tree properties.
    if (!uf.is_connected(e.v, e.w)) {
      mst.add_edge(e);
      mst.add_edge(e.reversed());
      uf.union_vertices(e.v, e.w);
    }
  }

  return mst;
}

// kruskal maximum spanning tree algorithm.
Graph kruskal_max_span_tree(const Graph& g) {
  std::list<Edge> all_edges = dedup_edges(g.all_edges());
  all_edges.sort(Edge::greater());

  // helper union-find data structure to detect loops.
  UF uf;
  Graph mst;
  for (const int& v : g.all_vertices()) {
    uf.add_vertex(v);
    mst.add_vertex(v);
  }

  for (const Edge& e : all_edges) {
    // if v and w is already connected which says there's a path linking v and
    // w, then adding this edge e will introduce another path linking v and w
    // and hence violates tree properties.
    if (!uf.is_connected(e.v, e.w)) {
      mst.add_edge(e);
      mst.add_edge(e.reversed());
      uf.union_vertices(e.v, e.w);
    }
  }

  return mst;
}

#endif  // KRUSKAL_MST_HPP_