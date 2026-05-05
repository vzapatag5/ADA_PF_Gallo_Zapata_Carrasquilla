#include "parser.hpp"
#include "mergesort.hpp"
#include "binary_search.hpp"
#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <iomanip>
#include <filesystem>

// Para crear el directorio results/ si no existe
namespace fs = std::filesystem;

void writeSortedRequests(const std::vector<Request>& arr, const std::string& filename) {
    std::ofstream out(filename);
    if (!out.is_open()) {
        std::cerr << "Error al abrir el archivo de salida: " << filename << std::endl;
        return;
    }
    // Escribir cabecera
    out << "customerID,tenure,MonthlyCharges,TotalCharges,Churn\n";
    for (const auto& req : arr) {
        out << req.customerID << ","
            << req.tenure << ","
            << std::fixed << std::setprecision(2) << req.MonthlyCharges << ","
            << req.TotalCharges << ","
            << (req.Churn ? "Yes" : "No") << "\n";
    }
    out.close();
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <ruta_al_csv>" << std::endl;
        return 1;
    }

    std::string csvPath = argv[1];
    
    // Crear la carpeta results/ si no existe
    if (!fs::exists("results")) {
        fs::create_directory("results");
    }

    // 1. Parseo del CSV
    int totalRecords = 0;
    int nullTotalChargesCount = 0;
    std::vector<Request> allRequests = parseCSV(csvPath, totalRecords, nullTotalChargesCount);
    
    if (allRequests.empty()) {
        std::cerr << "No se pudieron cargar datos del CSV." << std::endl;
        return 1;
    }
    
    std::cout << "--- Modulo A: Parseo ---" << std::endl;
    std::cout << "Total de registros cargados: " << totalRecords << std::endl;
    std::cout << "Registros con TotalCharges nulo (tenure=0): " << nullTotalChargesCount << std::endl;

    // Medición empírica para submuestras y dataset completo
    std::vector<int> sampleSizes = {1000, 3500, totalRecords};
    std::cout << "\n--- Modulo A: Analisis Empirico MergeSort ---" << std::endl;
    std::cout << std::left << std::setw(15) << "Tamano (n)" << "Tiempo (ms)" << std::endl;
    std::cout << "-----------------------------------" << std::endl;

    for (int n : sampleSizes) {
        if (n > totalRecords) continue;

        // Crear submuestra
        std::vector<Request> sample(allRequests.begin(), allRequests.begin() + n);

        auto start = std::chrono::high_resolution_clock::now();
        mergeSort(sample);
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::milli> duration = end - start;
        std::cout << std::left << std::setw(15) << n 
                  << std::fixed << std::setprecision(2) << duration.count() << " ms" << std::endl;
    }

    // Ordenar el arreglo completo con MergeSort
    mergeSort(allRequests);

    // Escribir salida de solicitudes ordenadas
    writeSortedRequests(allRequests, "results/solicitudes_ordenadas.csv");

    // 3. Búsqueda Binaria Recursiva
    std::vector<int> queries = {72, 60, 45, 30, 12};
    std::vector<std::string> queryNames = {"Q_A01", "Q_A02", "Q_A03", "Q_A04", "Q_A05"};
    
    std::ofstream busquedasOut("results/busquedas_A.txt");
    if (!busquedasOut.is_open()) {
        std::cerr << "Error al abrir results/busquedas_A.txt" << std::endl;
        return 1;
    }
    
    busquedasOut << "--- Resultados de Busqueda Binaria Recursiva ---\n";
    busquedasOut << "Consulta\tValor k\t\tcustomerID\n";
    busquedasOut << "------------------------------------------------\n";
    
    std::cout << "\n--- Modulo A: Consultas Busqueda Binaria ---" << std::endl;
    
    for (size_t i = 0; i < queries.size(); ++i) {
        int k = queries[i];
        int index = recursiveBinarySearch(allRequests, 0, allRequests.size() - 1, k);
        std::string resultID = "No encontrado";
        if (index != -1) {
            resultID = allRequests[index].customerID;
        }
        
        busquedasOut << queryNames[i] << "\t\t" << k << "\t\t" << resultID << "\n";
        std::cout << queryNames[i] << " (k=" << k << ") -> " << resultID << std::endl;
    }
    busquedasOut.close();
    
    std::cout << "\nArchivos generados:" << std::endl;
    std::cout << "- results/solicitudes_ordenadas.csv" << std::endl;
    std::cout << "- results/busquedas_A.txt" << std::endl;

    return 0;
}
