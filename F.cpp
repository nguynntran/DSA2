#include <algorithm>
#include <iostream>
#include <stack>
#include <vector>

template <typename T>
class Graph {
 public:
  explicit Graph(int vertices)
      : total_vertices(vertices),
        adjacency_list(vertices),
        reverse_adjacency_list(vertices) {}

  void AddEdge(T source, T destination) {
    adjacency_list[source].emplace_back(destination);
    reverse_adjacency_list[destination].emplace_back(source);
  }

  std::vector<std::vector<T>> FindStronglyConnectedComponents() {
    std::stack<T> finish_order;
    std::vector<bool> visited(total_vertices, false);

    // Perform a DFS to record the finish order of vertices
    for (T i = 0; i < total_vertices; ++i) {
      if (!visited[i]) {
        PerformDFS(i, visited, finish_order);
      }
    }

    // Reset visited for the second pass
    visited.assign(total_vertices, false);

    std::vector<std::vector<T>> scc_list;

    // Process vertices in reverse finish order to find SCCs
    while (!finish_order.empty()) {
      T current = finish_order.top();
      finish_order.pop();

      if (!visited[current]) {
        std::vector<T> scc;
        ExtractSCC(current, visited, scc);
        scc_list.emplace_back(scc);
      }
    }

    return scc_list;
  }

  Graph<T> CondenseGraph(const std::vector<std::vector<T>>& scc_list) {
    int new_vertex_count = scc_list.size();
    Graph<T> condensed_graph(new_vertex_count);

    for (size_t i = 0; i < scc_list.size(); ++i) {
      for (T vertex : scc_list[i]) {
        for (T neighbor : adjacency_list[vertex]) {
          int source_index = GetComponentIndex(scc_list, vertex);
          int dest_index = GetComponentIndex(scc_list, neighbor);
          if (source_index != dest_index) {
            condensed_graph.AddEdge(source_index, dest_index);
          }
        }
      }
    }

    return condensed_graph;
  }

  std::pair<int, int> CalculateDegrees() {
    std::vector<int> in_degree(total_vertices, 0);
    std::vector<int> out_degree(total_vertices, 0);

    for (T u = 0; u < total_vertices; ++u) {
      for (T v : adjacency_list[u]) {
        out_degree[u]++;
        in_degree[v]++;
      }
    }

    int zero_in_degree = std::count(in_degree.begin(), in_degree.end(), 0);
    int zero_out_degree = std::count(out_degree.begin(), out_degree.end(), 0);

    return {zero_in_degree, zero_out_degree};
  }

 private:
  int total_vertices;
  std::vector<std::vector<T>> adjacency_list;
  std::vector<std::vector<T>> reverse_adjacency_list;

  void PerformDFS(T vertex, std::vector<bool>& visited, std::stack<T>& finish_order) {
    visited[vertex] = true;

    for (T neighbor : adjacency_list[vertex]) {
      if (!visited[neighbor]) {
        PerformDFS(neighbor, visited, finish_order);
      }
    }

    finish_order.push(vertex);
  }

  void ExtractSCC(T vertex, std::vector<bool>& visited, std::vector<T>& scc) {
    visited[vertex] = true;
    scc.emplace_back(vertex);

    for (T neighbor : reverse_adjacency_list[vertex]) {
      if (!visited[neighbor]) {
        ExtractSCC(neighbor, visited, scc);
      }
    }
  }

  int GetComponentIndex(const std::vector<std::vector<T>>& scc_list, T vertex) const {
    for (size_t i = 0; i < scc_list.size(); ++i) {
      if (std::find(scc_list[i].begin(), scc_list[i].end(), vertex) != scc_list[i].end()) {
        return static_cast<int>(i);
      }
    }
    return -1;
  }
};

int main() {
  int vertices, edges;
  std::cin >> vertices >> edges;

  Graph<int> city_graph(vertices);

  for (int i = 0; i < edges; ++i) {
    int from, to;
    std::cin >> from >> to;
    city_graph.AddEdge(from - 1, to - 1);  // Convert to 0-based indexing
  }

  // Find all strongly connected components (SCCs)
  auto sccs = city_graph.FindStronglyConnectedComponents();

  // If there's only one SCC or no edges in the graph
  if (sccs.size() <= 1) {
    std::cout << 0 << std::endl;
    return 0;
  }

  // Create the condensed graph
  Graph<int> condensed_graph = city_graph.CondenseGraph(sccs);

  // Calculate the in-degrees and out-degrees for the condensed graph
  auto degrees = condensed_graph.CalculateDegrees();

  // Output the maximum of zero in-degrees or zero out-degrees
  std::cout << std::max(degrees.first, degrees.second) << std::endl;

  return 0;
}
