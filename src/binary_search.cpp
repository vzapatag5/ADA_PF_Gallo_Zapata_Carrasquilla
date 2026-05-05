#include "binary_search.hpp"

int recursiveBinarySearch(const std::vector<Request>& arr, int low, int high, int k) {
    if (low > high) {
        return -1; // No encontrado
    }

    int mid = low + (high - low) / 2;

    if (arr[mid].tenure == k) {
        // Encontramos una solicitud con tenure = k.
        // Como nos piden localizar "la primera solicitud" y puede haber duplicados,
        // debemos buscar a la izquierda (menor índice) para ver si hay una aparición anterior.
        int first_occurrence = recursiveBinarySearch(arr, low, mid - 1, k);
        if (first_occurrence != -1) {
            return first_occurrence;
        } else {
            return mid;
        }
    }
    // Dado que el arreglo está en orden DESCENDENTE:
    // Si el valor en mid es menor que k, k debe estar en la mitad izquierda (índices menores)
    else if (arr[mid].tenure < k) {
        return recursiveBinarySearch(arr, low, mid - 1, k);
    }
    // Si el valor en mid es mayor que k, k debe estar en la mitad derecha (índices mayores)
    else {
        return recursiveBinarySearch(arr, mid + 1, high, k);
    }
}
