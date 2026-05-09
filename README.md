# Proyecto Final - Análisis de Algoritmos y Estructuras de Datos

Este proyecto implementa diversas soluciones algorítmicas aplicadas a un problema de planificación de infraestructura de telecomunicaciones, utilizando el dataset **Telco Customer Churn**.

## Integrantes del Grupo
* **Mariana Carrasquilla**
* **Valentina Zapata**
* **Sofía Gallo**

---

## Descripción del Proyecto
El sistema se divide en tres módulos fundamentales que aplican diferentes paradigmas algorítmicos:
1. **Módulo A (Divide y Vencerás):** Ordenamiento de solicitudes mediante *MergeSort* y búsquedas rápidas con *Búsqueda Binaria*.
2. **Módulo B (Codicioso):** Construcción de un Árbol de Expansión Mínima (MST) utilizando el algoritmo de *Kruskal* para optimizar el tendido de red.
3. **Módulo C (Programación Dinámica):** Resolución del problema de la *Mochila 0-1* para la asignación óptima de ancho de banda.

---

## Requisitos Previos
* Tener instalado un compilador de C++ que soporte el estándar **C++17** (como `g++`).
* Contar con el archivo del dataset `WA_Fn-UseC_-Telco-Customer-Churn.csv` ubicado en la carpeta `data/`.

## Cómo Clonar
Para obtener una copia local del proyecto, ejecute:
```bash
git clone https://github.com/vzapatag5/ADA_PF_Gallo_Zapata_Carrasquilla.git
cd ADA_PF_Gallo_Zapata_Carrasquilla
```

## Compilación y Ejecución

### 1. Compilación
Para compilar el proyecto completo, ejecute el siguiente comando en la terminal desde la raíz del repositorio:
```bash
g++ -std=c++17 -O2 -o ada_pf src/*.cpp
```

### 2. Ejecución
Una vez compilado, puede ejecutar el programa pasando la ruta del dataset como argumento:

**En Linux / macOS / Git Bash:**
```bash
./ada_pf data/WA_Fn-UseC_-Telco-Customer-Churn.csv
```

**En Windows (PowerShell):**
```powershell
.\ada_pf data\WA_Fn-UseC_-Telco-Customer-Churn.csv
```

---

## Archivos Generados
Tras la ejecución, el programa generará reportes detallados en la carpeta `results/`:
* `results/solicitudes_ordenadas.csv`: Inventario completo de clientes ordenado por el Módulo A.
* `results/busquedas_A.txt`: Resultados de las consultas de búsqueda binaria y análisis de tiempos.
* `results/mst_red.txt`: Contiene la lista de conexiones del Árbol de Expansión Mínima y el peso total de la red (Módulo B).
* `results/asignacion_bw.txt`: Detalla la selección óptima de solicitudes y el beneficio máximo obtenido mediante programación dinámica (Módulo C).
* Otros reportes estadísticos y de rendimiento según la ejecución.

---
> **Nota:** Los archivos ejecutables (`ada_pf`, `ada_pf.exe`) están excluidos del repositorio mediante el archivo `.gitignore` para mantener la limpieza del proyecto.
