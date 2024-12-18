#include <iostream>
#include <vector>

template <typename T>
class TopologicalSort {
 public:
  explicit TopologicalSort(int vertices)
      : num_vertices(vertices), adjacency_list(vertices), in_degree(vertices, 0) {}

  void AddEdge(T from, T to) {
    adjacency_list[from].push_back(to);
    ++in_degree[to];
  }

  bool PerformSort(std::vector<int>& sorted_order) {
    std::vector<T> zero_in_degree_nodes;

    // Collect all vertices with zero in-degree
    for (T i = 0; i < num_vertices; ++i) {
      if (in_degree[i] == 0) {
        zero_in_degree_nodes.push_back(i);
      }
    }

    int processed_count = 0;

    // Process vertices with zero in-degree
    while (!zero_in_degree_nodes.empty()) {
      T current = zero_in_degree_nodes.back();
      zero_in_degree_nodes.pop_back();
      sorted_order.push_back(current);

      for (T neighbor : adjacency_list[current]) {
        if (--in_degree[neighbor] == 0) {
          zero_in_degree_nodes.push_back(neighbor);
        }
      }

      ++processed_count;
    }

    // Return true if all vertices are processed, indicating a valid topological order
    return processed_count == num_vertices;
  }

 private:
  T num_vertices;
  std::vector<std::vector<T>> adjacency_list;
  std::vector<int> in_degree;
};

int main() {
  int vertices, edges;
  std::cin >> vertices >> edges;

  TopologicalSort<int> sorter(vertices);
  std::vector<int> sorted_order;

  // Input edges
  for (int i = 0; i < edges; ++i) {
    int from, to;
    std::cin >> from >> to;
    sorter.AddEdge(from, to);
  }

  // Perform topological sort
  if (sorter.PerformSort(sorted_order)) {
    std::cout << "YES" << std::endl;
    for (int node : sorted_order) {
      std::cout << node << " ";
    }
  } else {
    std::cout << "NO";
  }

  return 0;
}
