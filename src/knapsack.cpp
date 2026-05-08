#include "knapsack.hpp"
 
#include <algorithm>
#include <climits>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>
 
using namespace std;
 
 
// -------------------------------------------------------------------
// Selecciona las primeras 50 solicitudes activas del arreglo ordenado
// -------------------------------------------------------------------
vector<Request> getTop50Active(const vector<Request>& sortedRequests) {
    vector<Request> result;
 
    for (const Request& req : sortedRequests) {
        // Solo tomamos las que tienen Churn = false (solicitudes activas)
        if (!req.Churn) {
            result.push_back(req);
        }
 
        // Paramos cuando ya tenemos 50
        if ((int)result.size() == 50) {
            break;
        }
    }
 
    return result;
}
 
 
// -------------------------------------------------------------------
// Resuelve la mochila 0-1 usando programacion dinamica (tabulacion)
// dp[i][w] = maximo valor usando los primeros i items con capacidad w
// -------------------------------------------------------------------
KnapsackResult solveKnapsack(const vector<Request>& requests, int capacity) {
    int n = (int)requests.size();
 
    // Convertimos los datos a enteros (peso y valor)
    vector<int> weight(n);
    vector<int> value(n);
 
    for (int i = 0; i < n; i++) {
        weight[i] = (int)round(requests[i].TotalCharges);
        value[i]  = (int)round(requests[i].MonthlyCharges * 10.0);
    }
 
    // Creamos la tabla dp de (n+1) filas y (capacity+1) columnas
    // Todo empieza en 0
    vector<vector<int>> dp(n + 1, vector<int>(capacity + 1, 0));
 
    // Llenamos la tabla fila por fila
    for (int i = 1; i <= n; i++) {
        for (int w = 0; w <= capacity; w++) {
            // Por defecto no tomamos el item i: copiamos la fila anterior
            dp[i][w] = dp[i-1][w];
 
            // Revisamos si conviene tomar el item i
            if (weight[i-1] <= w) {
                int valorConItem = dp[i-1][w - weight[i-1]] + value[i-1];
                if (valorConItem > dp[i][w]) {
                    dp[i][w] = valorConItem;
                }
            }
        }
    }
 
    // Recuperamos las solicitudes seleccionadas (backtracking sobre la tabla)
    KnapsackResult result;
    result.optimalValue = dp[n][capacity];
    result.totalWeight  = 0;
 
    int w = capacity;
    for (int i = n; i >= 1; i--) {
        // Si el valor cambio respecto a la fila anterior, tomamos el item i
        if (dp[i][w] != dp[i-1][w]) {
            result.selected.push_back(requests[i-1]);
            result.totalWeight += weight[i-1];
            w -= weight[i-1];
        }
    }
 
    return result;
}
 
 
// -------------------------------------------------------------------
// Funciones auxiliares para el contraejemplo codicioso
// -------------------------------------------------------------------
 
// Convierte una Request en CounterItem (ratio v/w)
static CounterItem toCounterItem(const Request& req) {
    CounterItem item;
    item.id     = req.customerID;
    item.weight = (int)round(req.TotalCharges);
    item.value  = (int)round(req.MonthlyCharges * 10.0);
    item.ratio  = (item.weight > 0) ? (double)item.value / item.weight : 0.0;
    return item;
}
 
// Simula el enfoque codicioso: asume que los items ya estan ordenados por ratio v/w
static void runGreedy(CounterItem sorted[3], int cap,
                      vector<string>& selected, int& totalValue) {
    selected.clear();
    totalValue  = 0;
    int usedW   = 0;
 
    for (int t = 0; t < 3; t++) {
        if (usedW + sorted[t].weight <= cap) {
            usedW      += sorted[t].weight;
            totalValue += sorted[t].value;
            selected.push_back(sorted[t].id);
        }
    }
}
 
// Calcula el optimo exacto para 3 items por enumeracion de subconjuntos
static void runDP3(CounterItem items[3], int cap,
                   vector<string>& selected, int& totalValue) {
    selected.clear();
    totalValue = 0;
 
    // Hay 7 subconjuntos no vacios (2^3 - 1)
    for (int mask = 1; mask < 8; mask++) {
        int wSum = 0, vSum = 0;
        vector<string> sel;
 
        for (int t = 0; t < 3; t++) {
            if (mask & (1 << t)) {
                wSum += items[t].weight;
                vSum += items[t].value;
                sel.push_back(items[t].id);
            }
        }
 
        // Guardamos si cabe y es mejor que lo actual
        if (wSum <= cap && vSum > totalValue) {
            totalValue = vSum;
            selected   = sel;
        }
    }
}
 
 
// -------------------------------------------------------------------
// Busca un contraejemplo donde greedy no es optimo
// Prueba ternas del top50 con diferentes capacidades
// -------------------------------------------------------------------
bool findCounterexample(const vector<Request>& top50, CounterexampleData& ce) {
    int n = (int)top50.size();
 
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            for (int k = j + 1; k < n; k++) {
                CounterItem items[3];
                items[0] = toCounterItem(top50[i]);
                items[1] = toCounterItem(top50[j]);
                items[2] = toCounterItem(top50[k]);
 
                // Saltamos items con peso 0 (no tienen sentido en la mochila)
                if (items[0].weight == 0 || items[1].weight == 0 || items[2].weight == 0) {
                    continue;
                }
 
                // Ordenamos por ratio descendente una sola vez para estos 3 items
                sort(items, items + 3, [](const CounterItem& x, const CounterItem& y) {
                    return x.ratio > y.ratio;
                });

                int totalW = items[0].weight + items[1].weight + items[2].weight;
 
                // Probamos capacidades entre el minimo y el total menos uno
                int minW = min({items[0].weight, items[1].weight, items[2].weight});
                
                // Declaramos los vectores fuera del bucle interno para reusar la memoria reservada
                vector<string> gSel, dpSel;
                gSel.reserve(3);
                dpSel.reserve(3);

                for (int cap = minW; cap < totalW; cap++) {
                    int gVal, dpVal;
 
                    runGreedy(items, cap, gSel, gVal);
                    runDP3(items, cap, dpSel, dpVal);
 
                    // Si greedy no es optimo, encontramos el contraejemplo
                    if (gVal < dpVal) {
                        ce.a        = items[0];
                        ce.b        = items[1];
                        ce.c        = items[2];
                        ce.capacity = cap;
 
                        ce.greedySelected = gSel;
                        ce.greedyValue    = gVal;
 
                        ce.dpSelected = dpSel;
                        ce.dpValue    = dpVal;
 
                        return true;
                    }
                }
            }
        }
    }
 
    return false;
}
 
 
// -------------------------------------------------------------------
// Genera el archivo de salida results/asignacion_bw.txt
// -------------------------------------------------------------------
void writeKnapsackOutput(const KnapsackResult&        result,
                         const vector<Request>& top50,
                         const CounterexampleData&    ce,
                         bool                         ceFound,
                         const string&                outputPath) {
    ofstream file(outputPath);
    if (!file.is_open()) {
        cerr << "Error: no se pudo crear el archivo " << outputPath << endl;
        return;
    }
 
    int n = (int)top50.size();
    int W = KNAPSACK_CAPACITY;
 
    
    file << "============================================================\n";
    file << "  MODULO C: ASIGNACION DE ANCHO DE BANDA\n";
    file << "  Programacion Dinamica - Mochila 0-1\n";
    file << "============================================================\n\n";
 
    file << "Capacidad total del nodo concentrador : " << W << " unidades\n";
    file << "Solicitudes activas evaluadas         : " << n << "\n\n";
 
    file << "------------------------------------------------------------\n";
    file << "  SOLUCION OPTIMA\n";
    file << "------------------------------------------------------------\n\n";
 
    file << "Valor optimo total   : " << result.optimalValue << " centavos\n";
    file << "Peso total utilizado : " << result.totalWeight << " / " << W << " unidades\n";
    file << "Solicitudes elegidas : " << result.selected.size() << "\n\n";
 
    // Calculamos el peso minimo de los items disponibles (para diagnostico)
    int minWeight = INT_MAX;
    for (const Request& r : top50) {
        int w = (int)round(r.TotalCharges);
        if (w < minWeight) minWeight = w;
    }
 
    if (result.selected.empty()) {
        file << "\nNinguna solicitud fue seleccionada.\n";
        file << "Razon: el peso minimo entre las 50 solicitudes es " << minWeight
             << " unidades,\n";
        file << "       lo cual supera la capacidad de " << W << " unidades.\n\n";
 
        file << "Esto ocurre porque las solicitudes con mayor tenure acumulan\n";
        file << "muchos cargos totales (TotalCharges), superando ampliamente\n";
        file << "la capacidad del nodo. El algoritmo es correcto:\n";
        file << "dp[n][500] = 0 porque ningun item tiene peso <= 500.\n\n";
 
        // Mostramos los 5 items con menor peso para ilustrar
        vector<pair<int, Request>> byWeight;
        for (const Request& r : top50) {
            byWeight.push_back({ (int)round(r.TotalCharges), r });
        }
        sort(byWeight.begin(), byWeight.end(),
             [](const pair<int, Request>& x, const pair<int, Request>& y) {
                 return x.first < y.first;
             });
 
        file << "Los 5 items de menor peso del conjunto (todos > " << W << "):\n";
        file << left << setw(20) << "  CustomerID"
             << setw(10) << "Peso"
             << setw(10) << "Valor"
             << "Tenure\n";
        file << "  " << string(48, '-') << "\n";
        for (int t = 0; t < 5 && t < (int)byWeight.size(); t++) {
            const Request& r = byWeight[t].second;
            int w = (int)round(r.TotalCharges);
            int v = (int)round(r.MonthlyCharges * 10.0);
            file << "  " << left << setw(18) << r.customerID
                 << setw(10) << w
                 << setw(10) << v
                 << r.tenure << "\n";
        }
    } else {
        file << "CustomerIDs seleccionados:\n";
        file << left << setw(20) << "  CustomerID"
             << setw(10) << "Peso"
             << setw(10) << "Valor"
             << "Ratio v/w\n";
        file << "  " << string(50, '-') << "\n";
 
        for (const Request& r : result.selected) {
            int w = (int)round(r.TotalCharges);
            int v = (int)round(r.MonthlyCharges * 10.0);
            double ratio = (w > 0) ? (double)v / w : 0.0;
 
            file << "  " << left  << setw(18) << r.customerID
                 << setw(10) << w
                 << setw(10) << v
                 << fixed << setprecision(4) << ratio << "\n";
        }
    }
 
    file << "\n";
 
    file << "------------------------------------------------------------\n";
    file << "  CONTRAEJEMPLO: FALLO DEL ENFOQUE CODICIOSO\n";
    file << "------------------------------------------------------------\n\n";
 
    if (ceFound) {
        file << "Tomamos 3 solicitudes reales del conjunto anterior:\n\n";
 
        file << left << setw(20) << "  CustomerID"
             << setw(10) << "Peso"
             << setw(10) << "Valor"
             << "Ratio v/w\n";
        file << "  " << string(50, '-') << "\n";
 
        CounterItem ceItems[3] = {ce.a, ce.b, ce.c};
        for (int t = 0; t < 3; t++) {
            file << "  " << left  << setw(18) << ceItems[t].id
                 << setw(10) << ceItems[t].weight
                 << setw(10) << ceItems[t].value
                 << fixed << setprecision(4) << ceItems[t].ratio << "\n";
        }
 
        file << "\nCapacidad del contraejemplo: " << ce.capacity << " unidades\n\n";
 
        // Tabla comparativa
        file << "Enfoque              | Solicitudes seleccionadas          | Valor | Optimo?\n";
        file << "---------------------|---------------------------------------|-------|--------\n";
 
        // Fila greedy
        string greedyIds;
        for (const string& id : ce.greedySelected) greedyIds += id + " ";
        if (greedyIds.empty()) greedyIds = "(ninguna)";
        file << left << setw(21) << "Codicioso (ratio v/w)"
             << "| " << setw(38) << greedyIds
             << "| " << setw(6) << ce.greedyValue
             << "| No\n";
 
        // Fila DP
        string dpIds;
        for (const string& id : ce.dpSelected) dpIds += id + " ";
        if (dpIds.empty()) dpIds = "(ninguna)";
        file << left << setw(21) << "PD (Mochila 0-1)"
             << "| " << setw(38) << dpIds
             << "| " << setw(6) << ce.dpValue
             << "| Si\n";
 
        file << "\nExplicacion:\n";
        file << "  El codicioso selecciona primero el item con mayor ratio v/w.\n";
        file << "  Pero eso no garantiza el optimo: puede haber otro item (o conjunto)\n";
        file << "  que quepa en la mochila y tenga mayor valor absoluto.\n";
        file << "  La PD evalua todas las combinaciones posibles y siempre\n";
        file << "  encuentra el valor maximo real.\n";
 
    } else {
        file << "No se encontro un contraejemplo con los items del top50.\n";
        file << "Esto puede ocurrir si los datos tienen una estructura particular.\n";
    }
 
    file << "\n";
 
    file << "------------------------------------------------------------\n";
    file << "  ANALISIS DE COMPLEJIDAD\n";
    file << "------------------------------------------------------------\n\n";
 
    long long ops = (long long)n * W;
 
    file << "Parametros:\n";
    file << "  n = " << n << "  (numero de solicitudes)\n";
    file << "  W = " << W << "  (capacidad de la mochila)\n\n";
 
    file << "Complejidad temporal: Theta(n * W)\n";
    file << "  = Theta(" << n << " * " << W << ")\n";
    file << "  = Theta(" << ops << ") operaciones\n\n";
 
    file << "  Por que? Tenemos dos ciclos anidados:\n";
    file << "    - El ciclo externo recorre los n items\n";
    file << "    - El ciclo interno recorre las W+1 capacidades\n";
    file << "  En cada celda hacemos O(1) trabajo -> total Theta(n * W)\n\n";
 
    file << "Complejidad espacial: Theta(n * W)\n";
    file << "  La tabla dp tiene (n+1) x (W+1) celdas\n";
    file << "  = (" << n+1 << ") x (" << W+1 << ") = " << (long long)(n+1)*(W+1) << " enteros\n\n";
 
    file << "Pseudopolinomialidad:\n";
    file << "  El algoritmo es Theta(n * W), pero W no depende de n.\n";
    file << "  Si W fuera exponencial en n (por ejemplo W = 2^n),\n";
    file << "  el algoritmo seria exponencial tambien.\n";
    file << "  Por eso no es polinomial en sentido estricto:\n";
    file << "  es polinomial en el VALOR de W, pero no en su representacion\n";
    file << "  en bits (log W bits). A esto se le llama pseudopolinomial.\n\n";
 
    file << "  En la practica, con n = " << n << " y W = " << W << ", el algoritmo\n";
    file << "  es muy eficiente y facil de implementar.\n\n";
 
    file << "============================================================\n";
    file << "  Fin del reporte - Modulo C\n";
    file << "============================================================\n";
 
    file.close();
    cout << "[Modulo C] Archivo generado: " << outputPath << endl;
}