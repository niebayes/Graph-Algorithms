#ifndef UNION_FIND_HPP_
#define UNION_FIND_HPP_

#include <unordered_map>

// union-find implementation with path-compressiond and union by rank
// optimizations.
// the logical representation of a union-find or disjoint set is a forest.
// each tree in the forest corresponds to one connected component of the graph.
// initially, each tree has only one vertex. Trees are merged by union roots of
// trees. to find the root of a tree, we trace up the tree using the parent
// mapping.
// by path compression, all leaf nodes are linked with the root node
// directly.
// by union by rank, prefers merging shorter trees to higher trees such that the
// height of the merge tree won't increase by more than one.
// these two optimizations make the tracing up more efficiently in that less
// nodes in the path
class UF {
 public:
  void add_vertex(const int id) {
    p[id] = id;
    rank[id] = 1;
    ++cc_cnt;
  }

  // find the root vertex of the given vertex.
  // a root vertex's parent is itself.
  int find(int id) { return p[id] == id ? id : (p[id] = find(p[id])); }

  // union two vertices by rank.
  void union_vertices(const int v, const int w) {
    const int root_v = find(v);
    const int root_w = find(w);
    if (root_v == root_w) {
      return;
    }
    if (rank[root_v] <= rank[root_w]) {
      p[root_v] = root_w;
      // if the two trees have the same height, then merging them
      // will increment the height by one.
      // otherwise, the height is unchanged.
      if (rank[root_v] == rank[root_w]) {
        ++rank[root_w];
      }
    } else {
      p[root_w] = root_v;
    }
    --cc_cnt;
  }

  bool is_connected(const int v, const int w) { return find(v) == find(w); }

  int get_cc_cnt() const { return cc_cnt; }

 private:
  // key: vertex id, value: parent's vertex id.
  std::unordered_map<int, int> p;
  // key: vertex id, value: height of the tree.
  std::unordered_map<int, int> rank;
  // #connected components.
  int cc_cnt{0};
};

#endif  // UNION_FIND_HPP_