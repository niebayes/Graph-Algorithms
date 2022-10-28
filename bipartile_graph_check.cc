#include <unordered_map>
#include <unordered_set>

#include "graph.hpp"

// algorithms for checking if a undirected graph is a bipartile graph.
// a bipartile graph is such a graph: vertices can be grouped
// into two disjoint sets and the two vertices in each edge belong
// to different set.

// alternating two coloring algorithm.
// this algo works as such: do a DFS or BFS traversal, for each unvisited
// outgoing vertex, color it a different color against the color of the ingoing
// vertex. If the outgoing vertex is visited and has the same color with
// the ingoing vertex, then this violates the bipartile graph property, i.e.
// the two vertices in each edge belong to different set.
bool dfs(const Graph& g, const int v, std::unordered_map<int, int>& color,
         const int my_color, std::unordered_set<int>& visited) {
  visited.insert(v);
  color[v] = my_color;

  for (const Edge& e : g.edges(v)) {
    if (visited.count(e.w) == 0) {
      // assign opposite color.
      if (!dfs(g, e.w, color, -my_color, visited)) {
        return false;
      }
    } else if (color[v] == color[e.w]) {
      return false;
    }
  }

  return true;
}

bool alter_two_color_bipartile_graph_check(const Graph& g) {
  std::unordered_set<int> visited;
  std::unordered_map<int, int> color;
  const int my_color = 1;
  for (const int& v : g.all_vertices()) {
    if (visited.count(v) == 0) {
      if (!dfs(g, v, color, my_color, visited)) {
        return false;
      }
    }
  }
  return true;
}