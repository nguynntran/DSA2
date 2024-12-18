#include <climits>
#include <iostream>
#include <queue>
#include <vector>

template <typename T>
class Graph {
 public:
  explicit Graph(int vertices) { adjacency_list.resize(vertices); }

  void AddEdge(T source, T destination) {
    adjacency_list[source].push_back(destination);
    adjacency_list[destination].push_back(source);
  }

  T FindShortestPath(T src1, T src2, T target, int total_nodes) {
    std::vector<T> dist_from_src1(total_nodes, INT_MAX);
    std::vector<T> dist_from_src2(total_nodes, INT_MAX);
    std::vector<T> dist_from_target(total_nodes, INT_MAX);

    BFS(src1, total_nodes, dist_from_src1);
    BFS(src2, total_nodes, dist_from_src2);
    BFS(target, total_nodes, dist_from_target);

    return CalculateMinimumDistance(total_nodes, dist_from_src1, dist_from_src2, dist_from_target);
  }

 private:
  std::vector<std::vector<T>> adjacency_list;

  void BFS(T start, int total_nodes, std::vector<T>& distances) {
    std::priority_queue<std::pair<T, T>, std::vector<std::pair<T, T>>, std::greater<>> pq;
    std::vector<bool> visited(total_nodes, false);

    distances[start] = 0;
    pq.emplace(0, start);

    while (!pq.empty()) {
      T current_node = pq.top().second;
      pq.pop();

      if (visited[current_node]) continue;
      visited[current_node] = true;

      for (const T& neighbor : adjacency_list[current_node]) {
        if (distances[current_node] + 1 < distances[neighbor]) {
          distances[neighbor] = distances[current_node] + 1;
          pq.emplace(distances[neighbor], neighbor);
        }
      }
    }
  }

  static T CalculateMinimumDistance(int total_nodes, const std::vector<T>& dist1,
                                     const std::vector<T>& dist2, const std::vector<T>& dist3) {
    T minimum_distance = INT_MAX;
    for (int i = 0; i < total_nodes; ++i) {
      minimum_distance = std::min(minimum_distance, dist1[i] + dist2[i] + dist3[i]);
    }
    return minimum_distance;
  }
};

int main() {
  int nodes, edges, leon, matilda, destination;
  std::cin >> nodes >> edges >> leon >> matilda >> destination;

  Graph<int> city_graph(nodes + 1);

  for (int i = 0; i < edges; ++i) {
    int u, v;
    std::cin >> u >> v;
    city_graph.AddEdge(u, v);
  }

  std::cout << city_graph.FindShortestPath(leon, matilda, destination, nodes + 1);
  return 0;
}
