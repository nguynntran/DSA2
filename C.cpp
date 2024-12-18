#include <algorithm>
#include <iostream>
#include <vector>

const int MAX_NODES = 53;
int graph[MAX_NODES][MAX_NODES];

// Depth-First Search to explore connected components
void DepthFirstSearch(int node, std::vector<bool>& visited, std::vector<int>& current_group) {
  visited[node] = true;
  current_group.push_back(node);

  for (int neighbor = 0; neighbor < MAX_NODES; ++neighbor) {
    if (graph[node][neighbor] == 1 && !visited[neighbor]) {
      DepthFirstSearch(neighbor, visited, current_group);
    }
  }
}

// Find the largest group of fully connected nodes using a bitmask-based approach
std::vector<int> FindLargestFullyConnectedGroup(int n) {
  std::vector<bool> is_valid(1 << n, false);  // Tracks valid subsets
  std::vector<int> neighbors(n, 0);          // Adjacency masks for each node
  std::vector<int> largest_group;            // Stores the largest group

  // Build the adjacency masks
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if (graph[i][j] == 1) {
        neighbors[i] |= (1 << j);
      }
    }
  }

  // Determine valid subsets using dynamic programming
  for (int subset = 1; subset < (1 << n); ++subset) {
    if ((subset & (subset - 1)) == 0) {  // Single-node subset
      is_valid[subset] = true;
    } else {
      int oldest_node = __builtin_ctz(subset);  // Rightmost set bit
      int subset_without_oldest = subset ^ (1 << oldest_node);

      if (is_valid[subset_without_oldest] &&
          (subset_without_oldest & neighbors[oldest_node]) == subset_without_oldest) {
        is_valid[subset] = true;
      }
    }
  }

  // Find the largest valid subset
  for (int subset = 1; subset < (1 << n); ++subset) {
    if (is_valid[subset] && __builtin_popcount(subset) > largest_group.size()) {
      largest_group.clear();
      for (int i = 0; i < n; ++i) {
        if (subset & (1 << i)) {
          largest_group.push_back(i);
        }
      }
    }
  }

  return largest_group;
}

// Wrapper to handle special cases or fallback to general solution
std::vector<int> FindLargestGroupWrapper(int n) {
  bool is_complete_graph = true;

  // Check if the graph is a complete graph
  for (int i = 0; i < n && is_complete_graph; ++i) {
    for (int j = 0; j < n; ++j) {
      if (i != j && graph[i][j] != 1) {
        is_complete_graph = false;
        break;
      }
    }
  }

  if (is_complete_graph) {
    // Use DFS to find the largest connected component in the complete graph
    std::vector<bool> visited(MAX_NODES, false);
    std::vector<int> largest_group;

    for (int i = 0; i < n; ++i) {
      if (!visited[i]) {
        std::vector<int> current_group;
        DepthFirstSearch(i, visited, current_group);
        if (current_group.size() > largest_group.size()) {
          largest_group = std::move(current_group);
        }
      }
    }
    return largest_group;
  } else {
    // Use the general solution for other cases
    return FindLargestFullyConnectedGroup(n);
  }
}

int main() {
  int n;
  std::cin >> n;

  // Input the adjacency matrix
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      std::cin >> graph[i][j];
    }
  }

  // Find the largest group and output the result
  std::vector<int> largest_group = FindLargestGroupWrapper(n);

  std::sort(largest_group.begin(), largest_group.end());
  for (size_t i = 0; i < largest_group.size(); ++i) {
    std::cout << largest_group[i];
    if (i < largest_group.size() - 1) {
      std::cout << " ";
    }
  }

  return 0;
}
