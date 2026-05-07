#ifndef BINARY_SEARCH_HPP
#define BINARY_SEARCH_HPP

#include "parser.hpp"
#include <vector>

// Busca de forma recursiva un registro cuyo tenure sea exactamente igual a k.
// El arreglo está ordenado de forma DESCENDENTE por tenure.
// Si existen varios registros con el mismo tenure, se puede retornar cualquiera.
// Retorna el índice del registro encontrado, o -1 si no existe ninguno con tenure == k.
int recursiveBinarySearch(const std::vector<Request>& arr, int low, int high, int k);

#endif // BINARY_SEARCH_HPP
