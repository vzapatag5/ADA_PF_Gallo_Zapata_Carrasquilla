#ifndef KNAPSACK_HPP
#define KNAPSACK_HPP
 
#include <string>
#include <vector>
#include "parser.hpp"
 
// Guarda los resultados de la mochila
struct KnapsackResult {
    int optimalValue;               // valor total optimo encontrado
    int totalWeight;                // peso total usado
    std::vector<Request> selected; // solicitudes que entraron en la mochila
};
 
// Guarda los datos del contraejemplo codicioso
struct CounterItem {
    std::string id;
    int    weight;
    int    value;
    double ratio;  // ratio v/w
};
 
struct CounterexampleData {
    CounterItem a, b, c;    // los 3 items del contraejemplo
    int         capacity;   // capacidad usada en el contraejemplo
 
    std::vector<std::string> greedySelected;  // ids elegidos por greedy
    int            greedyValue;
 
    std::vector<std::string> dpSelected;      // ids elegidos por DP
    int            dpValue;
};
 
// Capacidad fija de 500
const int KNAPSACK_CAPACITY = 500;
 
// Selecciona las primeras 50 solicitudes activas (churn = No) del arreglo ya ordenado
std::vector<Request> getTop50Active(const std::vector<Request>& sortedRequests);
 
// Resuelve la mochila 0-1 usando tabulacion
KnapsackResult solveKnapsack(const std::vector<Request>& requests, int capacity);
 
// Busca un contraejemplo donde el greedy por ratio v/w falla
// Retorna true si lo encuentra, false si no
bool findCounterexample(const std::vector<Request>& top50, CounterexampleData& ce);
 
// Genera el archivo results/asignacion_bw.txt
void writeKnapsackOutput(const KnapsackResult&       result,
                         const std::vector<Request>& top50,
                         const CounterexampleData&   ce,
                         bool                        ceFound,
                         const std::string&               outputPath);
 
#endif // KNAPSACK_HPP