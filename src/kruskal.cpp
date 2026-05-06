#include "kruskal.hpp"
#include "graph.hpp"
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;


// -------------------- UNION-FIND --------------------

vector<int> parent;
vector<int> rankUF;

void makeSet(int n) {
    parent.resize(n);
    rankUF.resize(n);

    for (int i = 0; i < n; i++) {
        parent[i] = i;   // (1) asigna cada nodo como su propio padre
        rankUF[i] = 0;   // (2) inicializa el rango a 0 para cada nodo
    }
}

int findSet(int x) {
    if (parent[x] != x) {
        parent[x] = findSet(parent[x]);   // (3) compresión de ruta, recursión
    }
    return parent[x];
}

void unionSet(int a, int b) { 
    int rootA = findSet(a);
    int rootB = findSet(b);

    if (rootA != rootB) {
        if (rankUF[rootA] < rankUF[rootB]) {
            parent[rootA] = rootB; // (4) el árbol con menor rango se une al de mayor rango  
        } else if (rankUF[rootA] > rankUF[rootB]) {
            parent[rootB] = rootA; // (5) el árbol con menor rango se une al de mayor rango  
        } else {
            parent[rootB] = rootA;
            rankUF[rootA]++; // (6) si ambos árboles tienen el mismo rango, uno se une al otro y el rango del nuevo árbol se incrementa en 1                  
        }
    }
}

// -------------------- KRUSKAL --------------------

vector<Edge> kruskal(Graph& g) {
    vector<Edge> mst;

    // Ordenar aristas por peso
    sort(g.edges.begin(), g.edges.end(), [](Edge a, Edge b) {
        return a.weight < b.weight;   // (7) comparador para ordenar las aristas de menor a mayor peso
    });

    makeSet(g.V);

    for (Edge e : g.edges) {
        if ( findSet(e.u) != findSet(e.v) ) {   // (8) si los nodos de la arista pertenecen a conjuntos diferentes, se agrega la arista al MST y se unen los conjuntos
            mst.push_back(e);
            unionSet(e.u, e.v);
        }
    }

    return mst;
}


