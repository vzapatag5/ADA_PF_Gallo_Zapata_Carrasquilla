#include "graph.hpp"
#include <cmath>    // para floor
#include <iostream>

// -------------------- GRAPH --------------------

Graph::Graph(int vertices) {
    V = vertices;
}

void Graph::addEdge(int u, int v, double weight) {
    edges.push_back({u, v, weight});
}

// -------------------- CONSTRUCCIÓN DEL GRAFO --------------------

std::vector<Edge> construirGrafo(const std::vector<Request>& requests) {

    const int N = 20; // (1) número de nodos (grupos) 

    std::vector<double> suma(N, 0.0); // (2) para acumular la suma de MonthlyCharges por grupo
    std::vector<int> conteo(N, 0); // (3) para contar el número de solicitudes por grupo

    // -------------------- AGRUPAMIENTO --------------------
    for (int i = 0; i < requests.size(); i++) {

        int grupo = i % N;

        suma[grupo] += requests[i].MonthlyCharges;
        conteo[grupo]++;
    }
    // -------------------- PROMEDIOS --------------------

    std::vector<double> promedio(N);

    for (int i = 0; i < N; i++) {
        if (conteo[i] > 0)
            promedio[i] = suma[i] / conteo[i]; // (5) calcular el promedio de MonthlyCharges para cada grupo
        else
            promedio[i] = 0.0; // (6) si no hay solicitudes en el grupo, el promedio se establece en 0 para evitar división por cero
    }

    for (int i = 0; i < 5; i++) {
    std::cout << "Grupo " << i
              << " promedio = "
              << promedio[i]
              << std::endl;
    }

    // -------------------- ARISTAS --------------------

    std::vector<Edge> edges;

    for (int u = 0; u < N; u++) {
        for (int v = u + 1; v < N; v++) { // (7) solo iterar sobre pares únicos (u,v) con u < v para evitar duplicados y auto-conexiones

            double costo = std::floor(promedio[u] + promedio[v]); // (8) calcular el costo de la arista como la suma de los promedios de los dos grupos, redondeada hacia abajo con floor

            edges.push_back({u, v, costo});
        }
    }

    return edges;
}