#ifndef MERGESORT_HPP
#define MERGESORT_HPP

#include "parser.hpp"
#include <vector>

// Ordena el arreglo de requests in-place usando MergeSort
// El orden debe ser descendente respecto al campo 'tenure'
// Garantiza un ordenamiento estable
void mergeSort(std::vector<Request>& arr);

#endif // MERGESORT_HPP
