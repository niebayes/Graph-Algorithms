#ifndef SHORTEST_PATH_HPP_
#define SHORTEST_PATH_HPP_

#include <list>
#include <queue>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "graph.hpp"

// algorithms for finding the shortest path.
// there're generally three categories of problems:
// (1) single source shortest path: given a source vertex, show the shortest
//     path to each other vertices.
// (2) source-sink shortest path: given a source and a target vertices, show
//     the shortest path from the source to the target.
// (3) all-pairs shortest path: given a graph, show the shortest paths between
//     each pair of vertices.

// you have to first state that the graph is weighted or unweighted.
// a unweighted graph is viewed as a graph wherein all edges are of the same
// weight 1.

// you also have to state that if the edge can has negative weight.

// helper function to print the path from src to dst.
void print_path(const int src, const int dst,
                const std::unordered_map<int, int>& parent) {
  std::stack<int> s;
  for (int x = dst; x != src; x = parent.at(x)) {
    s.push(x);
  }
  s.push(src);

  while (!s.empty()) {
    const int v = s.top();
    s.pop();
    std::cout << v << " -> ";
  }
  std::cout << '\n';
}

// BFS for unweighted graph.
// this algo works as such: do a level order BFS traversal. The first level has
// distance 1, and each subsequent level increments the distance by 1. When the
// traversal hits the destination vertex, terminates. Along with the traversal,
// maintain the parent mapping. And maintain a visited set to remove the effect
// of cycle.
//
// intuition: it's like a radio broadcasting from the source vertex. When the
// radio reaches the destination, the shortest path is found.
/// @return false if no path from src to dst.
bool bfs_sssp(const Graph& g, const int src, const int dst) {
  std::queue<int> q;
  std::unordered_map<int, int> parent;
  std::unordered_set<int> visited;  // used to fight against cycle.s

  q.push(src);

  while (!q.empty()) {
    const int lvl_cnt = q.size();
    for (int i = 0; i < lvl_cnt; ++i) {
      const int v = q.front();
      q.pop();

      visited.insert(v);

      if (v == dst) {
        // print the shortest path.
        print_path(src, dst, parent);
        return true;
      }

      // dive into the next level.
      for (const Edge& e : g.edges(v)) {
        if (visited.count(e.w) == 0) {
          q.push(e.w);
        }
      }
    }
  }

  return false;
}

// Dijkstra for non-negative weighted graph.
// this algo works as below:
// (1) maintain a distance look up table dist_to storing the known smallest
//     distance from the source vertex to each other vertex. Initially,
//     dist_to[src] = 0 and dist_to[other] = INF.
// (2) start from the source vertex, for each connected outgoing vertex w, relax
//     it, i.e. update dist_to[w] = min(dist_to[w], dist_to[v] + e.weight). That
//     is the known shortest path is the shortest path to v plus the edge v ->
//     w. If a relaxation happens, push the newly relaxed vertex and its dist_to
//     to the priority queue.
// (3) on each iteration, pop out the priority queue to get the vertex with the
//     lowest dist_to. Apply verted relaxation on this vertex.
// (4) when the priority queue is empty, all vertices connected to the source
//     vertex are relaxed and the shortest path can be reconstructed from the
//     parent mapping.
//
// intuition: assume there's a shortest path from src -> dst and assume the path
// is src -> n1 -> n2 -> ... -> dst. Starting from the source vertex, apply
// vertex relaxation on each connected outgoing vertex. Since all not-yet
// relaxed vertices will be pushed in to the queue and the algorithm terminates
// when the queue is empty, so eventually all vertices connected with the source
// vertex must be relaxed and hence the shortest path is obtained.
bool dijkstra_sssp(const Graph& g, const int src, const int dst) {
  // {vertex, current distance from src to this vertex}.
  using Pair = std::pair<int, int>;
  // used to construct min-heap: lowest dist at the top, lower vertex id at the
  // top.
  auto cmp = [](const Pair& a, const Pair& b) -> bool {
    if (a.second != b.second) {
      return a.second > b.second;
    }
    return a.first >= b.first;
  };
  std::priority_queue<Pair, std::vector<Pair>, decltype(cmp)> pq(cmp);

  // key: vertex, value: known smallest distance to this vertex from the src
  // vertex.
  std::unordered_map<int, int> dist_to;

  // initially, we have no info about vertices except the src vertex, and hence
  // the distances are positive inf.
  for (const int& v : g.all_vertices()) {
    // do not use INT32_MAX to avoid integer overflow.
    dist_to[v] = INT32_MAX;
  }
  dist_to[src] = 0;

  pq.push({src, dist_to[src]});

  // parent mapping in the shorest path tree.
  // used to reconstruct the shorest path.
  std::unordered_map<int, int> parent;
  parent[src] = -1;

  while (!pq.empty()) {
    const auto [v, dist_to_v] = pq.top();
    pq.pop();

    if (v == dst) {
      print_path(src, dst, parent);
      return true;
    }

    for (const Edge& e : g.edges(v)) {
      // vertex relaxation: for each connected outgoing vertex w of this vertex
      // v, update the smallest distance to w according to the weight of the
      // edge v -> w and the smallest distance to v.
      // from another perspective, the distance to a vertex is like the force in
      // physics, by relaxing a vertex, the force on it is reduced, i.e. the
      // distance is reduced.
      if (dist_to[e.w] > dist_to_v + e.weight) {
        dist_to[e.w] = dist_to_v + e.weight;
        parent[e.w] = v;

        // we can also put this statement outside the if scope, as long as you
        // have ensured that e.w is not relaxed yet.
        // since any relaxed vertices won't be relaxed again (this is because
        // there's no edges with negative weights) and hence the control flow
        // won't reach here, we can safely state that e.w is not relaxed yet.
        pq.push({e.w, dist_to[e.w]});
      }
    }
  }

  return false;
}

// Bellman-Ford for weighted graph.
// this algo works as below:
// (1) assume the #vertices is V. Apply V - 1 passes of vertex relaxation. In
// each pass, iterate all vertices, and then apply vertex relaxtion to all
// connected vertices with this vertex.
// (2) do another pass of vertex relaxation, check if the dist_to table has
// changes. if it has, then the graph has a cycle containing negative-weight
// edge and hence it's nonesense to find the shortest path.
// another solution is to iterate all edges and check if the dist_to[w] >
// dist_to[v] + e.weight. since after V - 1 passes of vertex relaxation, this
// can only happen when there's a negative edge, we can state that there's a
// negative edge.
// (3) the shortest path is also revealed by the parent mapping.
//
// inituition: assume there's a shortest path from src -> dst and assume the
// path is src -> n1 -> n2 -> ... -> dst. After the i-th pass of vertex
// relaxation, the shortest path from src -> ni must have been found. Since the
// shortest can at most have V - 1 edges and hence V - 1 passes of vertex
// relaxation is enough to find the shortest path if there is one.
bool bellman_ford_sssp(const Graph& g, const int src, const int dst) {
  std::unordered_map<int, int> dist_to;
  for (const int& v : g.all_vertices()) {
    // do not use INT32_MAX to avoid integer overflow.
    dist_to[v] = INT32_MAX / 2;
  }
  dist_to[src] = 0;

  std::unordered_map<int, int> parent;
  parent[src] = -1;

  // V - 1 passes of vertex relaxation.
  // note, if you want to find the shortest path from src to dst with at most k
  // #edges, you can only do k passes of vertex relaxation.
  for (int i = 0; i < g.all_vertices().size() - 1; ++i) {
    for (const int& v : g.all_vertices()) {
      for (const Edge& e : g.edges(v)) {
        if (dist_to[e.w] > dist_to[v] + e.weight) {
          dist_to[e.w] = dist_to[v] + e.weight;
          parent[e.w] = v;
        }
      }
    }
  }

  // check if we can reach dst from src.
  if (dist_to[dst] == INT32_MAX / 2) {
    return false;
  }

  // check if there's negative-weight edge.
  for (const Edge& e : g.all_edges()) {
    if (dist_to[e.w] > dist_to[e.v] + e.weight) {
      return false;
    }
  }

  print_path(src, dst, parent);

  return true;
}

// floyd warshall for weighted graph.
// this algo works as below:
// (1) create a adjacency matrix like two-dim array dist in which dist[i][j]
// tells the known smallest distance from vertex i to vertex j. Initially,
// dist[i][i] = 0 and dist[i][j] = INF for i != j, and dist[i][j] = weight if
// vertices i and j are directly connected with an edge.
// (2) do a dynamic programming scan. This scan from the top left of the matrix
// and proceeds to the the bottom right. I.e., it first gets the smallest
// distances between lower i and j and then gets the smallest distances between
// higher i and j based on the distances calculated before.
// (3) when the dynamic programming scan terminates, all pairs smallest
// distances are obtained.
//
// intuition:
// For a given pair of verties i and j, assume the shortest path exists, then
// the shortest must come from the path i -> j or i -> .. -> k -> .. -> j. for
// the latter case, we do not know which k consititutes the path, so we try each
// vertices, i.e. check if dist[i][k] + dist[k][j] < dist[i][j]. If yes, update
// dist[i][j] accordingly.
// since the path i -> .. -> k -> .. -> j may contain many middle vertices k, we
// have to examine the shorter paths first. So this algo first computes the top
// left portion of the matrix and then proceeds to the bottom right portion.
void print_path(
    const int v, const int w,
    const std::unordered_map<int, std::unordered_map<int, int>>& last) {
  // if v == w or v -> w, then.
  const int k = last.at(v).at(w);
  if (k == v) {
    return;
  }
  print_path(v, k, last);
  std::cout << k << " -> ";
}

void print_all_paths(
    const Graph& g,
    const std::unordered_map<int, std::unordered_map<int, int>>& last) {
  for (const int& v : g.all_vertices()) {
    for (const int& w : g.all_vertices()) {
      std::cout << v << " -> ";
      print_path(v, w, last);
      std::cout << w << '\n';
    }
  }
}

bool floyd_warshall_apsp(const Graph& g) {
  // dist[i][j] = known smallest distance from i to j.
  std::unordered_map<int, std::unordered_map<int, int>> dist;
  // last[i][j] = the last middle vertex k in the path i -> .. -> k -> j.
  // if the path is simply i -> j, i.e. no middle vertices, then k = i.
  std::unordered_map<int, std::unordered_map<int, int>> last;

  // do not use INT32_MAX to avoid integer overflow.
  constexpr int MAX_DIST = INT32_MAX / 2;

  for (const int& v : g.all_vertices()) {
    for (const int& w : g.all_vertices()) {
      if (v == w) {
        dist[v][w] = 0;
        last[v][w] = v;
      } else {
        dist[v][w] = MAX_DIST;
      }
    }
  }

  for (const Edge& e : g.all_edges()) {
    dist[e.v][e.w] = e.weight;
    last[e.v][e.w] = e.v;
  }

  // iterate all middle vertices.
  for (const int& k : g.all_vertices()) {
    // iterate all source vertices.
    for (const int& i : g.all_vertices()) {
      // iterate all destination vertices.
      for (const int& j : g.all_vertices()) {
        // if using the vertex k as the middle vertex would construct a path
        // with less distance from i to j, then use it.
        if (dist[i][j] > dist[i][k] + dist[k][j]) {
          dist[i][j] = dist[i][k] + dist[k][j];
          // the last middle vertex in the path i -> .. -> k -> .. -> last -> j
          // is the last middle vertex in the path k -> .. -> last -> j.
          // by tracing back the last vertices in the path, we can reconstruct
          // the shortest path from i to j.
          last[i][j] = last[k][j];
        }

        // FIXME: is this explanation correct?
        // since initially dist[i][i] == 0 and the above if condition must be
        // false. the exception is that there exists a negative-weight edge in
        // the path i -> .. -> k -> .. -> i and hence the above if condition
        // evaluates to true and the dist[i][i] becomes negative.
        if (dist[i][i] < 0) {
          // found a negative-weight edge.
          return false;
        }
      }
    }
  }

  // print the shortest paths between each pair of vertices.
  print_all_paths(g, last);

  return true;
}

#endif  // SHORTEST_PATH_HPP_