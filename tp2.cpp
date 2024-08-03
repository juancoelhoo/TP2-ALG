#include <cstdio>
#include <climits>
#include <cfloat>
#include <list>
#include <vector>
#include <algorithm>

class Path {
    public:
        Path(double origin, double destination, double year, double time, double cost) 
            : Origin(origin), Destination(destination), Year(year), Time(time), Cost(cost) {};
        double Origin;
        double Destination;
        double Year;
        double Time;
        double Cost;
};

class DisjointSets { 

    private:
        int* parent; 
        double* rank;

    public: 
        DisjointSets(int n) { 
            parent = new int[n]; 
            rank = new double[n]; 
    
            for (int i = 0; i < n; i++) { 
                parent[i] = -1; 
                rank[i] = 1; 
            } 
        }

        ~DisjointSets(){
            delete[] parent;
            delete[] rank;
        }
  
        int find(int i) { 
            if (parent[i] == -1) 
                return i; 
    
            return parent[i] = find(parent[i]); 
        }

        void unite(int x, int y) { 
            int rootX = find(x); 
            int rootY = find(y); 
  
            if (rootX != rootY) { 
                if (rank[rootX] < rank[rootY]) { 
                    parent[rootX] = rootY; 
                } 
                else if (rank[rootX] > rank[rootY]) { 
                    parent[rootY] = rootX; 
                } 
                else { 
                    parent[rootY] = rootX; 
                    rank[rootX] += 1; 
                } 
            } 
        } 
};

int findMin(double distances[], bool visited[], int numNodes) {
    double min = DBL_MAX;
    int node = 0;
 
    for (std::size_t i = 0; i < numNodes; i++){
        if (!visited[i] && distances[i] <= min)
            min = distances[i], node = i;
    }
    return node;
}

bool compareCost(Path i, Path j) { return (i.Cost < j.Cost); }

bool compareYear(Path i, Path j) { return (i.Year < j.Year); }

int main(){
    int numVillages = 0, numConnections = 0;

    if(scanf("%d", &numVillages)){};
    if(scanf("%d", &numConnections)){};

    std::list<Path> adjacencyList;
    std::vector<std::list<Path>> graph(numVillages, adjacencyList);
    std::vector<Path> edges;

    double origin = 0;
    double destination = 0;
    double year = 0;
    double time = 0;
    double cost = 0;
    Path pathAux(origin, destination, year, time, cost);

    for(std::size_t i = 0; i < numConnections; i++){
        if(scanf("%lf", &origin)){};
        pathAux.Origin = origin;
        if(scanf("%lf", &destination)){};
        pathAux.Destination = destination;
        if(scanf("%lf", &year)){};
        pathAux.Year = year;
        if(scanf("%lf", &time)){};
        pathAux.Time = time;
        if(scanf("%lf", &cost)){};
        pathAux.Cost = cost;
        
        graph[origin-1].emplace_back(pathAux);
        edges.push_back(pathAux);

        pathAux.Destination = origin;
        graph[destination-1].emplace_back(pathAux);
    }

    //-- Find Minimum Time --//

    double distances[numVillages];
    bool visited[numVillages];
    int parents[numVillages];

    for (std::size_t i = 0; i < numVillages; i++){
        distances[i] = DBL_MAX;
        visited[i] = false;
        parents[i] = -1;
    }

    distances[0] = 0;

    for (std::size_t k = 0; k < numVillages - 1; k++) {

        int minNode = findMin(distances, visited, numVillages);
        visited[minNode] = true;
 
        for (std::list<Path>::iterator it = graph[minNode].begin(); it != graph[minNode].end(); ++it){
            int num = it->Destination - 1;
            if (!visited[num] && distances[minNode] + it->Time < distances[num]){
                distances[num] = distances[minNode] + it->Time;
                parents[num] = minNode;
            }
        }
    }

    for(std::size_t p = 0; p < numVillages; p++){
        printf("%.0lf\n", distances[p]);
    }

    //-- Find the earliest year on the shortest path --//

    double earliestYear = 0;
    for(std::size_t b = 0; b < numVillages; b++){
        
        if(parents[b] == -1)
            continue;
        for (std::list<Path>::iterator it = graph[b].begin(); it != graph[b].end(); ++it){
            if(it->Destination == parents[b] + 1){
                if(earliestYear < it->Year)
                    earliestYear = it->Year;
            }
        }
    }

    printf("%.0lf\n", earliestYear);

    //-- Find the smallest year to connect the royal palace to any other village --//

    std::sort(edges.begin(), edges.end(), compareYear);

    DisjointSets disjointSets(numVillages);
    earliestYear = 0;

    for (auto edge : edges) {
        int x = edge.Origin - 1;
        int y = edge.Destination - 1;
        double year = edge.Year;

        if (disjointSets.find(x) != disjointSets.find(y)) {
            disjointSets.unite(x, y); 
            if(earliestYear < year)
                earliestYear = year;
        }
    }
    
    printf("%.0lf\n", earliestYear);

    //-- Find the minimum possible cost --//

    std::sort(edges.begin(), edges.end(), compareCost);

    DisjointSets disjointSets2(numVillages);
    double minCost = 0;

    for (auto edge : edges) {
        int x = edge.Origin - 1;
        int y = edge.Destination - 1;
        double cost = edge.Cost;

        if (disjointSets2.find(x) != disjointSets2.find(y)) {
            disjointSets2.unite(x, y); 
            minCost += cost;
        }
    }

    printf("%.0lf", minCost);

    return 0;
}
