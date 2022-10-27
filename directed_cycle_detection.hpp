#ifndef DIRECTED_CYCLE_DETECTION_HPP_
#define DIRECTED_CYCLE_DETECTION_HPP_

#include <cassert>
#include <iostream>
#include <stack>
#include <unordered_map>
#include <vector>

#include "graph.hpp"
#include "union_find.hpp"

// algorithms for detecting a cycle in a directed graph, i.e. check if the given
// graph is an directed acyclic graph (DAG).

// DFS to detect back edge, i.e. an edge points to an ancestor vertex which is
// not the parent.
// assume the current vertex is v and the ancenstor vertex is w.
// such a back edge indicates that there's a path v -> w and also a path w - v
// and hence a cycle.
bool dfs(const Graph& g, const int& v, std::unordered_set<int>& visited,
         std::unordered_set<int>& ancestors,
         std::unordered_map<int, int>& parent) {
  visited.insert(v);
  ancestors.insert(v);  // ancestors of the vertex w.

  for (const Edge& e : g.edges(v)) {
    const int& w = e.w;
    // find a tree edge, i.e. from parent to child.
    if (visited.count(w) == 0) {
      parent[w] = v;
      if (dfs(g, w, visited, ancestors, parent)) {
        return true;
      }
    } else if (ancestors.count(w) > 0) {
      // find a back edge, i.e. from child to ancestor.
      // this means there's a cycle:
      // w -> ... -> v -> w.

      // print the cycle.
      std::stack<int> cycle;
      cycle.push(w);
      for (int x = v; x != w; x = parent.at(x)) {
        cycle.push(x);
      }
      cycle.push(w);
      while (!cycle.empty()) {
        std::cout << cycle.top() << " -> ";
        cycle.pop();
      }
      std::cout << '\n';

      return true;
    }
    // otherwise, the vertex w is a visited vertex and not an ancestor.
    // there're two possibilities:
    // (1) cross edge: w is in another subtree.
    // (2) forward edge: w is in the subtree.
    // but these edges are not in our concern.
  }

  // ancestors form a stack and hence we need to pop the top of the stack per
  // the system stack.
  ancestors.erase(v);
  return false;
}

bool dfs_detect_cycle(const Graph& g) {
  std::unordered_set<int> visited;  // visited vertices.

  for (const int& v : g.all_vertices()) {
    // ancestors of the currently visiting vertex.
    std::unordered_set<int> ancestors;
    // key: current vertex, val: the parent vertex of this vertex.
    std::unordered_map<int, int> parent;

    if (visited.count(v) == 0) {
      if (dfs(g, v, visited, ancestors, parent)) {
        return true;
      }
    }
  }
  return false;
}

#endif  // DIRECTED_CYCLE_DETECTION_HPP_