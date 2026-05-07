#include "binary_search.hpp"

// Búsqueda binaria recursiva en arreglo ordenado DESCENDENTEMENTE por tenure.
// Retorna el índice de cualquier registro con tenure == k, o -1 si no existe.
// Corrección aplicada per aclaración docente (07/05/2026):
// Si hay múltiples registros con el mismo tenure, es válido retornar cualquiera.
int recursiveBinarySearch(const std::vector<Request>& arr, int low, int high, int k) {
    if (low > high) {
        return -1; // No encontrado
    }

    int mid = low + (high - low) / 2;

    if (arr[mid].tenure == k) {
        // Encontramos un registro con tenure exactamente igual a k.
        // Retornamos este índice directamente (cualquier ocurrencia es válida).
        return mid;
    }
    // Dado que el arreglo está en orden DESCENDENTE:
    // Si arr[mid].tenure < k, entonces k está en la mitad izquierda (valores mayores).
    else if (arr[mid].tenure < k) {
        return recursiveBinarySearch(arr, low, mid - 1, k);
    }
    // Si arr[mid].tenure > k, entonces k está en la mitad derecha (valores menores).
    else {
        return recursiveBinarySearch(arr, mid + 1, high, k);
    }
}
