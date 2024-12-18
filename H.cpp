#include <iostream>
#include <queue>
#include <vector>
#include <limits>

// Define a constant representing a very large value
const int kMax = 1e6;

// Struct representing an edge in the graph
struct Edge {
    int to;    // Destination vertex
    int cost;  // Cost of traveling along this edge
};

// Class representing a graph
class Graph {
private:
    int num_vertices_; // Number of vertices in the graph
    std::vector<std::vector<Edge>> adj_list_; // Adjacency list representation

public:
    // Constructor initializes the graph with the given number of vertices
    explicit Graph(int n) : num_vertices_(n), adj_list_(n + 1) {}

    // Adds a directed edge to the graph
    void AddEdge(int from, int to, int cost) {
        adj_list_[from].push_back({to, cost});
    }

    // Returns the adjacency list of the graph
    const std::vector<std::vector<Edge>>& GetAdjList() const {
        return adj_list_;
    }

    // Returns the number of vertices in the graph
    int GetNumVertices() const {
        return num_vertices_;
    }
};

// Function implementing Dijkstra's algorithm with a constraint on the number of flights
int Dijkstra(int n, const Graph& graph, int k, int start, int end) {
    // Create a 2D vector to track the shortest distances with up to k flights
    std::vector<std::vector<int>> distances(n + 1, std::vector<int>(k + 1, kMax));
    distances[start][0] = 0;

    // Priority queue to store {cost, {current vertex, flights taken}}
    std::priority_queue<std::pair<int, std::pair<int, int>>,
                        std::vector<std::pair<int, std::pair<int, int>>>,
                        std::greater<>> pq;

    // Start with the source vertex
    pq.push({0, {start, 0}});

    while (!pq.empty()) {
        int cost = pq.top().first;             // Current cost
        int current = pq.top().second.first;  // Current vertex
        int flights_taken = pq.top().second.second; // Flights used so far
        pq.pop();

        // If the destination vertex is reached, return the cost
        if (current == end) {
            return cost;
        }

        // If more flights can be used
        if (flights_taken < k) {
            // Explore all adjacent edges
            for (const Edge& edge : graph.GetAdjList()[current]) {
                int new_cost = cost + edge.cost;
                if (new_cost < distances[edge.to][flights_taken + 1]) {
                    distances[edge.to][flights_taken + 1] = new_cost;
                    pq.push({new_cost, {edge.to, flights_taken + 1}});
                }
            }
        }
    }

    // If no valid path is found, return -1
    return -1;
}

int main() {
    int n;     // Number of vertices
    int m;     // Number of edges
    int k;     // Maximum number of flights
    int start; // Start vertex
    int end;   // End vertex

    // Input the number of vertices, edges, max flights, start, and end vertices
    std::cin >> n >> m >> k >> start >> end;

    // Initialize the graph
    Graph graph(n);

    // Input the edges of the graph
    for (int i = 0; i < m; ++i) {
        int from, to, cost;
        std::cin >> from >> to >> cost;
        graph.AddEdge(from, to, cost);
    }

    // Run the Dijkstra algorithm and print the result
    int result = Dijkstra(n, graph, k, start, end);
    std::cout << result;

    return 0;
}
