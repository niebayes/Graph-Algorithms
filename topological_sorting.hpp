#ifndef TOPOLOGICAL_SORTING_HPP_
#define TOPOLOGICAL_SORTING_HPP_

#include <algorithm>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "directed_cycle_detection.hpp"
#include "graph.hpp"

// algorithms for topogical sorting on a directed acyclic graph (DAG).

// DFS topological sorting algorithm.
// this algo works as below:
// (1) at first, ensure there's no cycle in the graph.
// (2) DFS on the graph and record the DFS postorder.
// (3) reverse the DFS postorder to get the topological order.
// note, there're two DFS orders for a graph:
// (a) preorder: push the node in a queue the first time visits it.
//     this order reveals the start of a dfs call.
// (2) postorder: push the node in a queue in the end of a dfs call.
//     this order reveals the finish of a dfs call.
// a finish of a dfs call reveals a finish of the item.
// after the finish of the dfs call on a child, should the finish of the dfs
// call on the parent.
// the reverse of such a finish order exactly reveals the topological order.
void dfs(const Graph& g, const int& v, std::vector<int>& postorder,
         std::unordered_set<int>& visited) {
  visited.insert(v);

  for (const Edge& e : g.edges(v)) {
    if (visited.count(e.w) == 0) {
      dfs(g, e.w, postorder, visited);
    }
  }

  postorder.push_back(v);
}

std::vector<int> dfs_topological_sorting(const Graph& g) {
  if (dfs_detect_cycle(g)) {
    return {};
  }

  std::vector<int> postorder;

  std::unordered_set<int> visited;
  for (const int& v : g.all_vertices()) {
    if (visited.count(v) == 0) {
      dfs(g, v, postorder, visited);
    }
  }

  std::reverse(postorder.begin(), postorder.end());
  return postorder;
}

// BFS topological sorting algorithm, aka. kahn algorithm.
// this algo works as below:
// (1) construct an indegree table for the graph where keys are the vertices
//     and the values are the indegree count of a vertex.
//     each positive indegree count for a vertex means that we have to finish
//     another thing first until we're prepared to finish this thing.
//     if a vertex's indegree is 0, we can finish it diectly.
//     after finish a thing, check all things dependend on it. Check if their
//     indegree becomes 0 after finishes this thing.
// (2) push all vertices with 0 indegree to a queue (order does not matter).
// (3) repeated pop a vertex in the queue, and update the degrees of the
// connected nodes. Push all connected nodes with 0 indegree.
// (4) eventually, all edges will be removed and hence all vertices will be of 0
// indegree and hence this algo terminates.
// the topological order is exactly the order vertices pushed out of the queue.
std::vector<int> bfs_topological_sorting(const Graph& g) {
  // note, we can also record how many vertices visited during bfs.
  // if this number is not equal to the #vertices, then there's a cycle.
  if (dfs_detect_cycle(g)) {
    return {};
  }

  std::vector<int> order;
  std::queue<int> q;
  std::unordered_map<int, int> indegree;
  for (const int& v : g.all_vertices()) {
    indegree[v] = 0;
  }
  for (const Edge& e : g.all_edges()) {
    ++indegree[e.w];
  }

  for (const auto& p : indegree) {
    if (p.second == 0) {
      q.push(p.first);
    }
  }

  while (!q.empty()) {
    const int v = q.front();
    q.pop();
    order.push_back(v);

    for (const Edge& e : g.edges(v)) {
      --indegree[e.w];
      if (indegree[e.w] == 0) {
        q.push(e.w);
      }
    }
  }

  return order;
}

#endif  // TOPOLOGICAL_SORTING_HPP_