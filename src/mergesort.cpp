#include "mergesort.hpp"

// Función auxiliar para mezclar dos mitades
static void merge(std::vector<Request>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Arreglos temporales
    std::vector<Request> L(n1);
    std::vector<Request> R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;

    // Merge garantizando estabilidad y orden descendente por 'tenure'.
    // Si L[i].tenure == R[j].tenure, preferimos el de la izquierda (L) para ser estables.
    while (i < n1 && j < n2) {
        if (L[i].tenure >= R[j].tenure) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copiar elementos restantes de L[]
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copiar elementos restantes de R[]
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

static void mergeSortHelper(std::vector<Request>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSortHelper(arr, left, mid);
        mergeSortHelper(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }
}

void mergeSort(std::vector<Request>& arr) {
    if (arr.empty()) return;
    mergeSortHelper(arr, 0, arr.size() - 1);
}
