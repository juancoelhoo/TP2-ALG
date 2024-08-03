#include <cstdio>
#include <climits>
#include <cfloat>
#include <list>
#include <vector>
#include <algorithm>

class Caminho{
    public:
        Caminho(double n, double a, double t, double c) : NumeroDaVila(n), Ano(a), Tempo(t), Custo(c){};
        double NumeroDaVila;
        double Ano;
        double Tempo;
        double Custo;
        double VilaOrigem;
};

class ConjuntosDisjuntos { 

    private:
        int* parent; 
        double* rank;

    public: 
        ConjuntosDisjuntos(int n) { 
            parent = new int[n]; 
            rank = new double[n]; 
    
            for (int i = 0; i < n; i++) { 
                parent[i] = -1; 
                rank[i] = 1; 
            } 
        }

        ~ConjuntosDisjuntos(){
            delete[] parent;
            delete[] rank;
        }
  
        int find(int i) { 
            if (parent[i] == -1) 
                return i; 
    
            return parent[i] = find(parent[i]); 
        }

        void unite(int x, int y) { 
            int s1 = find(x); 
            int s2 = find(y); 
  
            if (s1 != s2) { 
                if (rank[s1] < rank[s2]) { 
                    parent[s1] = s2; 
                } 
                else if (rank[s1] > rank[s2]) { 
                    parent[s2] = s1; 
                } 
                else { 
                    parent[s2] = s1; 
                    rank[s1] += 1; 
                } 
            } 
        } 
};

int Minimo(double vetor[], bool vilas[], int numVilas) {
    double min = DBL_MAX;
    int vila = 0;
 
    for (std::size_t v = 0; v < numVilas; v++){
        if (vilas[v] == false && vetor[v] <= min)
            min = vetor[v], vila = v;
    }
    return vila;
}

bool ComparaCusto (Caminho i, Caminho j) { return (i.Custo < j.Custo); }

bool ComparaAno (Caminho i, Caminho j) { return (i.Ano < j.Ano); }

int main(){
    int NumeroDeVilas = 0, NumeroDeConexoes = 0;

    if(scanf("%d", &NumeroDeVilas)){};
    if(scanf("%d", &NumeroDeConexoes)){};

    std::list<Caminho> lista;
    std::vector<std::list<Caminho>> Baiconia(NumeroDeVilas, lista);
    std::vector<Caminho> Arestas;

    double vila1 = 0;
    double vila2 = 0;
    double ano = 0;
    double tempo = 0;
    double custo = 0;
    Caminho caminho_aux(vila2, ano, tempo, custo);

    for(std::size_t i=0; i<NumeroDeConexoes; i++){
        if(scanf("%lf", &vila1)){};
        caminho_aux.VilaOrigem = vila1;
        if(scanf("%lf", &vila2)){};
        caminho_aux.NumeroDaVila = vila2;
        if(scanf("%lf", &ano)){};
        caminho_aux.Ano = ano;
        if(scanf("%lf", &tempo)){};
        caminho_aux.Tempo = tempo;
        if(scanf("%lf", &custo)){};
        caminho_aux.Custo = custo;
        
        Baiconia[vila1-1].emplace_back(caminho_aux);
        Arestas.push_back(caminho_aux);

        caminho_aux.NumeroDaVila = vila1;
        Baiconia[vila2-1].emplace_back(caminho_aux);
    }

    //-- Obter Menor Tempo --//

    double vetorMin[NumeroDeVilas];
    bool vilas[NumeroDeVilas];
    int vetorPai[NumeroDeVilas];

    for (std::size_t i = 0; i < NumeroDeVilas; i++){
        vetorMin[i] = DBL_MAX;
        vilas[i] = false;
        vetorPai[i] = -1;
    }

    vetorMin[0] = 0;

    for (std::size_t k = 0; k < NumeroDeVilas - 1; k++) {

        int menor = Minimo(vetorMin, vilas, NumeroDeVilas);
        vilas[menor] = true;
 
        for (std::list<Caminho>::iterator it=Baiconia[menor].begin(); it != Baiconia[menor].end(); ++it){
            int num = it->NumeroDaVila - 1;
            if (!vilas[num] && vetorMin[menor] + it->Tempo < vetorMin[num]){
                vetorMin[num] = vetorMin[menor] + it->Tempo;
                vetorPai[num] = menor;
            }
        }
    }

    for(std::size_t p=0; p<NumeroDeVilas; p++){
        printf("%.0lf\n", vetorMin[p]);
    }

    //-- Obter o primeiro ano do caminho com menor tempo

    double menorAno = 0;
    for(std::size_t b=0; b < NumeroDeVilas; b++){
        
        if(vetorPai[b] == -1)
            continue;
        for (std::list<Caminho>::iterator it=Baiconia[b].begin(); it != Baiconia[b].end(); ++it){
            if(it->NumeroDaVila == vetorPai[b] + 1){
                if(menorAno < it->Ano)
                    menorAno = it->Ano;
            }
        }
    }

    printf("%.0lf\n", menorAno);

    //-- Obter menor ano pra conectar o palacio real a qualquer outra vila

    std::sort (Arestas.begin(), Arestas.end(), ComparaAno);

    ConjuntosDisjuntos conjunto(NumeroDeVilas);
    menorAno = 0;

    for (auto aresta : Arestas) {
        int x = aresta.VilaOrigem - 1;
        int y = aresta.NumeroDaVila - 1;
        double w = aresta.Ano;

        if (conjunto.find(x) != conjunto.find(y)) {
            conjunto.unite(x, y); 
            if(menorAno < w)
                menorAno = w;
        }
    }
    
    printf("%.0lf\n", menorAno);

    //-- Obter menor custo possível --//

    std::sort (Arestas.begin(), Arestas.end(), ComparaCusto);

    ConjuntosDisjuntos conjunto2(NumeroDeVilas);
    double menorCusto = 0;

    for (auto aresta : Arestas) {
        int x = aresta.VilaOrigem - 1;
        int y = aresta.NumeroDaVila - 1;
        double w = aresta.Custo;

        if (conjunto2.find(x) != conjunto2.find(y)) {
            conjunto2.unite(x, y); 
            menorCusto += w;
        }
    }

    printf("%.0lf", menorCusto);

    return 0;
}