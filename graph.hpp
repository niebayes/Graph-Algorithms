#ifndef GRAPH_HPP_
#define GRAPH_HPP_

#include <iostream>
#include <list>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// note, a vertex may also has a corresponding key.
// if it's such a case, just create a vertex type to wrap
// the vertex id the vertex key.

// undirected/directed weighted edge.
// although undirected edge has no concept of source and
// destination vertices, it's more intuitive to set source
// and destination vertices.
struct Edge {
  int v;  // source vertex id.
  int w;  // destination vertex id.
  int weight;

  // construct an edge with the source and destination vertices reversed.
  Edge reversed() const { return Edge{.v = w, .w = v, .weight = weight}; }

  // return true if this edge is identical to the other edge.
  bool equal(const Edge& other) const {
    return (v == other.v && w == other.w) || (w == other.v && v == other.w);
  }

  // less comparator. Used to sort edges by non-decreasing weight.
  static bool less(const Edge& a, const Edge& b) {
    return a.weight <= b.weight;
  }

  // greater comparator. Used to construct a min-heap of edges.
  // note, the c++ priority queue lib requires you to feed into a greater comp
  // to construct a min-heap.
  struct greater {
    bool operator()(const Edge& a, const Edge& b) {
      return a.weight >= b.weight;
    }
  };
};

std::list<Edge> dedup_edges(const std::list<Edge>& edges) {
  std::list<Edge> es;
  for (const Edge& e : edges) {
    if (std::find_if(es.cbegin(), es.cend(), [=](const Edge& other) {
          return e.equal(other);
        }) != es.cend()) {
      continue;
    }
    es.push_back(e);
  }
  return es;
}

// undirected weighted graph represented as adjacency list.
// note, for simplicity, there's no error handling.
class Graph {
 public:
  void add_vertex(const int& v) { maybe_add_vertex(v); }

  void add_edge(const Edge& e) {
    adj_list[e.v].push_back(e);
    maybe_add_vertex(e.v);
    maybe_add_vertex(e.w);
  }

  std::list<int> all_vertices() const { return vertices; }

  std::list<Edge> edges(const int v) const {
    if (adj_list.count(v) == 0) {
      return {};
    }
    return adj_list.at(v);
  }

  // return deduplicated edges.
  std::list<Edge> all_edges() const {
    std::list<Edge> edges;
    for (const auto& p : adj_list) {
      edges.insert(edges.end(), p.second.cbegin(), p.second.cend());
    }
    return edges;
  }

  // print the graph using adjacency list representation.
  void print() const {
    std::vector<int> vs(vertices.cbegin(), vertices.cend());
    std::sort(vs.begin(), vs.end());
    for (const int& v : vs) {
      std::cout << v << " -> ";
      for (const Edge& e : edges(v)) {
        std::cout << e.w << ' ';
      }
      std::cout << '\n';
    }
  }

 private:
  // add the vertex v if it does exit.
  void maybe_add_vertex(const int v) {
    if (std::find(vertices.cbegin(), vertices.cend(), v) == vertices.cend()) {
      vertices.push_back(v);
    }
  }

  std::list<int> vertices;
  // key: vertex, value: outgoing edges of this vertex.
  std::map<int, std::list<Edge>> adj_list;
};

#endif  //  GRAPH_HPP_