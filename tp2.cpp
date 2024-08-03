#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <algorithm>
#include <climits>
#include <numeric>
#include <functional>

using namespace std;

struct Edge {
    int u, v, year, length, cost;
};

// Union-Find data structure
struct UnionFind {
    vector<int> parent, rank;

    UnionFind(int size) : parent(size), rank(size, 0) {
        iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);  // Path compression
        }
        return parent[x];
    }

    void unite(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);
        if (rootX != rootY) {
            if (rank[rootX] > rank[rootY]) {
                parent[rootY] = rootX;
            } else if (rank[rootX] < rank[rootY]) {
                parent[rootX] = rootY;
            } else {
                parent[rootY] = rootX;
                rank[rootX]++;
            }
        }
    }

    bool connected(int x, int y) {
        return find(x) == find(y);
    }
};

void processEdgesUntilYear(const vector<Edge>& edges, int N, int year, UnionFind& uf) {
    for (const Edge& edge : edges) {
        if (edge.year <= year) {
            uf.unite(edge.u, edge.v);
        }
    }
}

int findFirstYearAllDistancesRealizable(const vector<Edge>& edges, int N) {
    UnionFind uf(N);
    int largestYear = -1;

    for (const Edge& edge : edges) {
        uf.unite(edge.u, edge.v);

        bool allConnected = true;
        int root = uf.find(0);
        for (int i = 1; i < N; i++) {
            if (uf.find(i) != root) {
                allConnected = false;
                break;
            }
        }

        if (allConnected) {
            largestYear = edge.year;
        }
    }

    return largestYear;
}

int main() {
    int N, M;
    cin >> N >> M;

    vector<Edge> edges(M);
    for (int i = 0; i < M; i++) {
        cin >> edges[i].u >> edges[i].v >> edges[i].year >> edges[i].length >> edges[i].cost;
        edges[i].u--; // Adjusting to 0-based index
        edges[i].v--; // Adjusting to 0-based index
    }

    // Sort edges by year of completion
    sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
        return a.year < b.year;
    });

    vector<vector<pair<int, int>>> graph(N);

    // Question 1: Minimum distances after all projects are complete
    for (const Edge& edge : edges) {
        graph[edge.u].emplace_back(edge.v, edge.length);
        graph[edge.v].emplace_back(edge.u, edge.length);
    }

    // Perform Dijkstra's algorithm for Question 1
    vector<int> dist(N, INT_MAX);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push(make_pair(0, 0));
    dist[0] = 0;

    while (!pq.empty()) {
        pair<int, int> top = pq.top();
        pq.pop();
        int currentDist = top.first;
        int u = top.second;

        if (currentDist > dist[u]) continue;

        for (const auto& neighbor : graph[u]) {
            int v = neighbor.first;
            int length = neighbor.second;
            if (dist[u] + length < dist[v]) {
                dist[v] = dist[u] + length;
                pq.push(make_pair(dist[v], v));
            }
        }
    }

    for (int i = 0; i < N; i++) {
        cout << dist[i] << endl;
    }

    // Question 2: First year when all distances are mutually realizable
    int firstYearAllDistancesRealizable = findFirstYearAllDistancesRealizable(edges, N);
    cout << firstYearAllDistancesRealizable << endl;

    // Question 3: First year when the entire kingdom is reachable from the palace
    int firstYearKingdomReachable = -1;
    UnionFind ufForReachability(N);
    for (const Edge& edge : edges) {
        ufForReachability.unite(edge.u, edge.v);

        bool allConnected = true;
        int root = ufForReachability.find(0);  // Find the root of the palace
        for (int i = 1; i < N; i++) {
            if (ufForReachability.find(i) != root) {
                allConnected = false;
                break;
            }
        }

        if (allConnected) {
            firstYearKingdomReachable = edge.year;
            break;
        }
    }

    cout << firstYearKingdomReachable << endl;

    // Question 4: Minimum cost to connect all villages
    sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
        return a.cost < b.cost;
    });

    UnionFind ufForCost(N);
    int minCost = 0;
    for (const Edge& edge : edges) {
        if (!ufForCost.connected(edge.u, edge.v)) {
            ufForCost.unite(edge.u, edge.v);
            minCost += edge.cost;
        }
    }

    cout << minCost << endl;

    return 0;
}
