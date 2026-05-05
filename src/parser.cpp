#include "parser.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

// Auxiliar para hacer trim de strings
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

static inline void trim(std::string &s) {
    rtrim(s);
    ltrim(s);
}

std::vector<Request> parseCSV(const std::string& filename, int& totalRecords, int& nullTotalChargesCount) {
    std::vector<Request> requests;
    totalRecords = 0;
    nullTotalChargesCount = 0;

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo: " << filename << std::endl;
        return requests;
    }

    std::string line;
    // Ignorar la cabecera
    std::getline(file, line);

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        // Limpiar retornos de carro al final (como \r)
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> tokens;

        while (std::getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        if (tokens.size() < 21) {
            continue; // Evitar registros mal formados
        }

        Request req;
        req.customerID = tokens[0];
        
        try {
            req.tenure = std::stoi(tokens[5]);
        } catch (...) {
            req.tenure = 0;
        }
        
        try {
            req.MonthlyCharges = std::stod(tokens[18]);
        } catch (...) {
            req.MonthlyCharges = 0.0;
        }
        
        std::string tcStr = tokens[19];
        trim(tcStr);
        if (tcStr.empty()) {
            req.TotalCharges = 0.0;
            nullTotalChargesCount++;
        } else {
            try {
                req.TotalCharges = std::stod(tcStr);
            } catch (...) {
                req.TotalCharges = 0.0;
            }
        }
        
        std::string churnStr = tokens[20];
        trim(churnStr);
        req.Churn = (churnStr == "Yes");

        requests.push_back(req);
        totalRecords++;
    }

    file.close();
    return requests;
}
