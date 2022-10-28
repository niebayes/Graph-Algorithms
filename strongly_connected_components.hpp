#ifndef STRONGLY_CONNECTED_COMPONENTS_HPP_
#define STRONGLY_CONNECTED_COMPONENTS_HPP_

#include <list>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "connected_components.hpp"
#include "graph.hpp"
#include "topological_sorting.hpp"

// algorithms to find strongly connected components in a directed graph.

// kosaraju strongly connected components algorithm.
// this algo works as below:
// (1) do a DFS traversal of the original graph, get the DFS reversed postorder,
// i.e. the topological order.
// (2) transpose the graph, i.e. reverse every edge of the graph. Then do a DFS
// traversal of the transposed graph according to the topological order.
// (3) each pass of the second DFS traversal reveals a strongly connected
// component.
//
// intuition:
// 对原图进行 graph contraction，每个强连通分量会被 contract 成一个
// node，连接不同强连通
// 分量的边保留，则整个图被转化为一个有向无环图。肯定是没有环的，因为环都在强连通分量内部。
// 假设从这个图的某个 node 开始 DFS，那么这个 node 肯定是在一个 DFS pass
// 中最后结束的，即 DFS 逆后序的第一个节点。 当把原图 transpose 后，contracted
// 的这个图会变成这样：假设原来有一条边从一个强连通分量 A 指向 另一个强连通分量
// B，那么这条边会被反向。 假设第一次 DFS 从 A 中的某个节点开始，那么在
// tranposed 的图中再从 A 中的这个节点开始 DFS 时， 必定不会再 traverse
// 到强连通分量 B，因为连接两个强连通分量的边反向了，从出边转为进边了。
// 因此第二次 DFS 根据第一次 DFS 的逆后序进行，就保证了在第二次 DFS 时，每个 DFS
// pass 只会访问同一个
// 强连通分量，而不会访问其他强连通分量。当一个强连通分量被访问完后，才会开始访问下一个强连通分量。
std::unordered_map<int, std::list<int>> kosaraju_scc(const Graph& g) {
  std::vector<int> postorder;
  std::unordered_set<int> visited;
  for (const int& v : g.all_vertices()) {
    if (visited.count(v) == 0) {
      dfs(g, v, postorder, visited);
    }
  }

  const std::vector<int> reversed_postorder(postorder.crbegin(),
                                            postorder.crend());

  // transpose the graph.
  Graph rg = g.reversed();
  visited.clear();
  std::unordered_map<int, std::list<int>> cc;
  int cc_id = 0;
  for (const int& v : reversed_postorder) {
    if (visited.count(v) == 0) {
      dfs(g, v, cc_id, cc, visited);
    }
  }

  return cc;
}

#endif  // STRONGLY_CONNECTED_COMPONENTS_HPP_