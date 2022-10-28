# A repo for common graph algorithms

### Graph Rrepresentation
- adjacency list

### Graph Traversal
- DFS
- BFS

### Cycle Detection for Directed Graph
- DFS to detect back edge
- Topological Sorting (Kahn) to check if #visited vertices == #vertices.

### Cycle Detection for Undirected Graph
- DFS to detect back edge 
  - compared to DFS in directed graph: utilize the parent mapping
- Union-Find to iterate all edges
  - compared to Union-Find in directed graph: remove duplicated edges
  - limits compared to DFS: cannot print the cycle
  - but easier to implement
  - note, cannot use Union-Find to detect cycle in directed graph

### Connected Components of Undirected Graph (CC)
- DFS
- Union-Find

### Strongly Connected Components of Directed Graph (SCC)
- Kosaraju

### Topological Sorting 
- DFS 
- BFS (Kahn)

### Minimum/Maximum Spanning Tree (MST)
- Kruskal
- Prim

Applications: 
- remove redundant edges from a graph to form a tree

### Single Source Shortest Path (SSSP)
- BFS for unweighted graph or graph with uniform-weighted edges
- Dijkstra for non-negative weighted graph
- Bellman-Ford for weighted graph

### Source Sink Shortest Path (SSSP)
almost identical to the single source shortest path problem, but with slight twist, i.e. terminate 
the searching when reaching the sink, aka. the target, vertex.

### All Pairs Shortest Path (APSP)
- Floyd-Warshall

### Bipartile Graph Check
- Two-Coloring