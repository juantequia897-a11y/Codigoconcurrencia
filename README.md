# SIGET - Módulo de Concurrencia (Productor-Consumidor)

Simulación en C++11 de la gestión concurrente de tráfico urbano mediante el patrón Productor-Consumidor.

##  Descripción
El sistema simula sensores de tráfico (productores) que generan lecturas en tiempo real y módulos de procesamiento (consumidores) que analizan los datos recibidos mediante un búfer compartido.

## 🛠️ Tecnologías y Primitivas de Concurrencia
* **Lenguaje:** C++11
* **Manejo de hilos:** `std::thread`
* **Exclusión mutua:** `std::mutex` y `std::unique_lock`
* **Sincronización:** `std::condition_variable`

##  Compilación y Ejecución
Para compilar y ejecutar desde la terminal (usando MinGW / GCC):

```bash
g++ -std=c++11 main.cpp -o concurrencia
.\concurrencia.exe
