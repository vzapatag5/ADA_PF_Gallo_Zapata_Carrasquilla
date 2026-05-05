#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>

struct Request {
    std::string customerID;
    int tenure;
    double MonthlyCharges;
    double TotalCharges;
    bool Churn;
};

// Parsea el CSV y retorna el vector de Request.
// Retorna también por referencia el número total de registros cargados y los nulos.
std::vector<Request> parseCSV(const std::string& filename, int& totalRecords, int& nullTotalChargesCount);

#endif // PARSER_HPP
