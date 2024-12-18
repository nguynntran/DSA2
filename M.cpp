#include <iostream>
#include <vector>

const int kMax = 1e5;       // Maximum number of nodes
const int kLogMax = 20;     // Maximum depth for binary lifting

class Tree {
private:
    std::vector<std::vector<int>> adj_;     // Adjacency list to store the tree
    std::vector<int> entry_, exit_;         // Entry and exit times for nodes
    std::vector<std::vector<int>> up_;      // Binary lifting table
    int timer_;                             // Timer for DFS

    // Perform DFS to populate the binary lifting table and track entry/exit times
    void Dfs(int node, int parent = 0) {
        entry_[node] = timer_++;

        up_[node][0] = parent; // Immediate parent
        for (int i = 1; i < kLogMax; i++) {
            up_[node][i] = up_[up_[node][i - 1]][i - 1]; // Populate ancestors
        }

        for (int child : adj_[node]) {
            if (child != parent) {
                Dfs(child, node);
            }
        }

        exit_[node] = timer_++;
    }

    // Check if node `a` is an ancestor of node `b`
    bool IsAncestor(int a, int b) {
        return (entry_[a] <= entry_[b]) && (exit_[a] >= exit_[b]);
    }

public:
    explicit Tree(int n) {
        adj_.resize(n);
        entry_.resize(n);
        exit_.resize(n);
        up_.resize(n, std::vector<int>(kLogMax));
        timer_ = 0;
    }

    // Add a directed edge from `parent` to `child`
    void AddEdge(int parent, int child) {
        adj_[parent].push_back(child);
    }

    // Build the binary lifting table and calculate entry/exit times
    void BuildTree() {
        Dfs(0); // Start DFS from the root (node 0)
    }

    // Find the Lowest Common Ancestor (LCA) of nodes `a` and `b`
    int LCA(int a, int b) {
        if (IsAncestor(a, b)) {
            return a;
        }
        if (IsAncestor(b, a)) {
            return b;
        }

        for (int i = kLogMax - 1; i >= 0; i--) {
            if (!IsAncestor(up_[a][i], b)) {
                a = up_[a][i];
            }
        }
        return up_[a][0]; // Return the parent of the last valid ancestor
    }
};

int main() {
    int n, m; // Number of nodes and number of queries
    std::cin >> n >> m;

    Tree tree(n);

    // Read the tree edges
    for (int i = 1; i < n; i++) {
        int parent;
        std::cin >> parent;
        tree.AddEdge(parent, i);
    }

    tree.BuildTree(); // Preprocess the tree for LCA queries

    int a1, a2, x, y, z;
    std::cin >> a1 >> a2 >> x >> y >> z;

    long long sum = 0; // Sum of LCA values
    int v = 0;         // Current node for LCA calculation

    // Process each query
    for (int i = 0; i < m; i++) {
        v = tree.LCA((a1 + v) % n, a2); // Calculate LCA of the given nodes
        sum += v;

        // Update a1 and a2 for the next query
        a1 = (1LL * x * a1 + 1LL * y * a2 + z) % n;
        a2 = (1LL * x * a2 + 1LL * y * a1 + z) % n;
    }

    std::cout << sum << std::endl; // Output the total sum of LCAs
    return 0;
}
