#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include "parser.hpp"  // Para usar Request

// -------------------- ESTRUCTURA DE ARISTA --------------------

struct Edge {
    int u;
    int v;
    double weight;
};

// -------------------- CLASE GRAFO --------------------

class Graph {
public:
    int V;  // número de nodos
    std::vector<Edge> edges;

    Graph(int vertices);

    void addEdge(int u, int v, double weight);
};

// -------------------- CONSTRUCCIÓN DEL GRAFO --------------------

// Construye el grafo a partir de los datos parseados
std::vector<Edge> construirGrafo(const std::vector<Request>& requests);

#endif