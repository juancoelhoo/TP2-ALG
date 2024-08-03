#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <algorithm>
#include <climits>
#include <numeric>  // Para std::iota
#include <functional>  // Para std::function

using namespace std;

struct Edge {
    int u, v, year, length, cost;
};

struct Node {
    int vertex, dist;
    bool operator<(const Node& other) const {
        return dist > other.dist;
    }
};

void dijkstra(int start, const vector<vector<pair<int, int>>>& graph, vector<int>& dist) {
    priority_queue<Node> pq;
    pq.push({start, 0});
    dist[start] = 0;

    while (!pq.empty()) {
        Node current = pq.top();
        pq.pop();

        for (auto& neighbor : graph[current.vertex]) {
            int next = neighbor.first;
            int weight = neighbor.second;

            if (dist[current.vertex] + weight < dist[next]) {
                dist[next] = dist[current.vertex] + weight;
                pq.push({next, dist[next]});
            }
        }
    }
}

int findFirstYearAllDistancesRealizable(const vector<Edge>& edges, int N) {
    vector<vector<pair<int, int>>> graph(N);
    vector<int> dist(N, INT_MAX);

    int firstYearAllDistancesRealizable = -1;
    for (const Edge& edge : edges) {
        graph[edge.u].emplace_back(edge.v, edge.length);
        graph[edge.v].emplace_back(edge.u, edge.length);

        bool allRealizable = true;
        for (int start = 0; start < N; start++) {
            fill(dist.begin(), dist.end(), INT_MAX);
            dijkstra(start, graph, dist);
            for (int i = 0; i < N; i++) {
                if (dist[i] == INT_MAX) {
                    allRealizable = false;
                    break;
                }
            }
            if (!allRealizable) {
                break;
            }
        }

        if (allRealizable) {
            firstYearAllDistancesRealizable = edge.year;
        }
    }

    return firstYearAllDistancesRealizable;
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
    vector<int> dist(N, INT_MAX);

    // Question 1: Minimum distances after all projects are complete
    for (const Edge& edge : edges) {
        graph[edge.u].emplace_back(edge.v, edge.length);
        graph[edge.v].emplace_back(edge.u, edge.length);
    }

    dijkstra(0, graph, dist);

    for (int i = 0; i < N; i++) {
        cout << dist[i] << endl;
    }

    // Question 2: First year when all distances are mutually realizable
    int firstYearAllDistancesRealizable = findFirstYearAllDistancesRealizable(edges, N);
    cout << firstYearAllDistancesRealizable << endl;

    // Question 3: First year when entire kingdom is reachable from the palace
    int firstYearKingdomReachable = -1;
    for (int year = 1; year <= 1e8; year++) {
        fill(dist.begin(), dist.end(), INT_MAX);
        graph.assign(N, {});

        for (const Edge& edge : edges) {
            if (edge.year <= year) {
                graph[edge.u].emplace_back(edge.v, edge.length);
                graph[edge.v].emplace_back(edge.u, edge.length);
            }
        }

        dijkstra(0, graph, dist);

        bool allReachable = true;
        for (int i = 1; i < N; i++) {
            if (dist[i] == INT_MAX) {
                allReachable = false;
                break;
            }
        }

        if (allReachable) {
            firstYearKingdomReachable = year;
            break;
        }
    }

    cout << firstYearKingdomReachable << endl;

    // Question 4: Minimum cost to connect all villages
    sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
        return a.cost < b.cost;
    });

    vector<int> parent(N);
    iota(parent.begin(), parent.end(), 0);

    function<int(int)> find = [&](int x) {
        return parent[x] == x ? x : parent[x] = find(parent[x]);
    };

    auto unite = [&](int x, int y) {
        parent[find(x)] = find(y);
    };

    int minCost = 0;
    for (const Edge& edge : edges) {
        if (find(edge.u) != find(edge.v)) {
            unite(edge.u, edge.v);
            minCost += edge.cost;
        }
    }

    cout << minCost << endl;

    return 0;
}
