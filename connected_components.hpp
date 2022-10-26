#ifndef CONNECTED_COMPONENTS_HPP_
#define CONNECTED_COMPONENTS_HPP_

#include <unordered_map>
#include <unordered_set>

#include "graph.hpp"
#include "union_find.hpp"

/// algorithms to find connected components of a undirected graph.

// dfs.
void dfs(const Graph& g, const int v, const int cc_id,
         std::unordered_map<int, std::list<int>>& cc,
         std::unordered_set<int>& visited) {
  visited.insert(v);
  cc[cc_id].push_back(v);

  for (const Edge& e : g.edges(v)) {
    if (visited.count(e.w) == 0) {
      dfs(g, e.w, cc_id, cc, visited);
    }
  }
}

std::unordered_map<int, std::list<int>> dfs_connected_components(
    const Graph& g) {
  int cc_id = 0;                    // the id of the next cc.
  std::unordered_set<int> visited;  // contains visited vertices.
  // key: cc_id, value: vertices in this cc.
  std::unordered_map<int, std::list<int>> cc;
  for (const int& v : g.all_vertices()) {
    if (visited.count(v) == 0) {
      dfs(g, v, cc_id, cc, visited);
      ++cc_id;
    }
  }
  return cc;
}

// union-find.
std::unordered_map<int, std::list<int>> uf_connected_components(
    const Graph& g) {
  UF uf;
  for (const int& v : g.all_vertices()) {
    uf.add_vertex(v);
  }

  // edges provide the connection.
  for (const Edge& e : g.all_edges()) {
    uf.union_vertices(e.v, e.w);
  }

  // key: cc_id, value: vertices in this cc.
  // note, cc ids are chosen from the vertex ids.
  std::unordered_map<int, std::list<int>> cc;

  // after the union, vertices in the same cc will have the same root.
  for (const int& v : g.all_vertices()) {
    const int root = uf.find(v);
    cc[root].push_back(v);
  }

  return cc;
}

#endif  // CONNECTED_COMPONENTS_HPP_