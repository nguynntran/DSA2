#include <cstdint>
#include <iostream>
#include <queue>
#include <vector>

// Define a large constant to represent "infinity"
const int kMax = 1e8;

// Class representing a directed graph
class Graph {
private:
    uint64_t num_vertices_; // Number of vertices in the graph
    std::vector<std::vector<std::pair<uint64_t, uint64_t>>> adj_list_; // Adjacency list

public:
    // Constructor to initialize the graph with the given number of vertices
    explicit Graph(uint64_t vertices)
        : num_vertices_(vertices), adj_list_(vertices) {}

    // Adds a directed edge from vertex `u` to vertex `v` with the given weight
    void AddEdge(uint64_t u, uint64_t v, uint64_t weight) {
        adj_list_[u].emplace_back(v, weight);
    }

    // Returns the adjacency list of the graph
    const std::vector<std::vector<std::pair<uint64_t, uint64_t>>>& GetAdjList() const {
        return adj_list_;
    }

    // Returns the number of vertices in the graph
    uint64_t GetNumVertices() const {
        return num_vertices_;
    }
};

// Function to calculate the minimum bottles of lemonade needed to travel
// from universe `x` to universe `y`
uint64_t MinBottlesLemonade(const Graph& graph, uint64_t x, uint64_t y) {
    uint64_t num_vertices = graph.GetNumVertices();

    // If the source and destination are the same, no bottles are needed
    if (x == y) {
        return 0;
    }

    // Initialize the minimum bottles array with a large value
    std::vector<uint64_t> min_bottles(num_vertices, kMax);
    min_bottles[x] = 0;

    // Breadth-first search (BFS) queue
    std::queue<uint64_t> queue;
    queue.push(x);

    // BFS traversal
    while (!queue.empty()) {
        uint64_t current_vertex = queue.front();
        queue.pop();

        // Explore all adjacent vertices
        for (const auto& edge : graph.GetAdjList()[current_vertex]) {
            uint64_t next_vertex = edge.first;
            uint64_t weight = edge.second;

            // Relaxation step: update the minimum bottles if a better path is found
            if (min_bottles[next_vertex] > min_bottles[current_vertex] + weight) {
                min_bottles[next_vertex] = min_bottles[current_vertex] + weight;
                queue.push(next_vertex);
            }
        }
    }

    // Return the minimum bottles required to reach universe `y`
    return min_bottles[y];
}

int main() {
    uint64_t a; // Cost of the first type of edge
    uint64_t b; // Cost of the second type of edge
    uint64_t m; // Number of universes
    uint64_t x; // Starting universe
    uint64_t y; // Target universe

    // Input the parameters
    std::cin >> a >> b >> m >> x >> y;

    // Initialize the graph with `m` universes
    Graph graph(m);

    // Add edges to the graph
    for (uint64_t i = 0; i < m; ++i) {
        graph.AddEdge(i, (i + 1) % m, a);         // First type of edge
        graph.AddEdge(i, (i * i + 1) % m, b);     // Second type of edge
    }

    // Compute the minimum bottles required and output the result
    uint64_t result = MinBottlesLemonade(graph, x, y);
    std::cout << result << std::endl;

    return 0;
}
