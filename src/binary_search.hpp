#ifndef BINARY_SEARCH_HPP
#define BINARY_SEARCH_HPP

#include "parser.hpp"
#include <vector>

// Busca de forma recursiva la primera solicitud cuyo tenure es igual a k
// Dado que el arreglo está ordenado de forma descendente, esta función
// debe encontrar el primer índice donde tenure == k.
// Retorna el índice, o -1 si no se encuentra.
int recursiveBinarySearch(const std::vector<Request>& arr, int low, int high, int k);

#endif // BINARY_SEARCH_HPP
