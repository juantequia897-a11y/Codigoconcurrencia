#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>

using namespace std;

// Estructura simple para los datos del sensor
struct DatoTrafico {
    int idSensor;
    int vehiculos;
};

queue<DatoTrafico> buffer;
const size_t TAMANO_MAX = 5;

mutex mtx;
condition_variable cvLleno, cvVacio;
bool ejecutando = true;

void sensorProductor(int id) {
    int lecturas = 1;
    while (ejecutando) {
        this_thread::sleep_for(chrono::milliseconds(800));
        
        unique_lock<mutex> lock(mtx);
        cvLleno.wait(lock, [] { return buffer.size() < TAMANO_MAX || !ejecutando; });
        
        if (!ejecutando) break;
        
        buffer.push({id, lecturas * 10});
        cout << "[Sensor " << id << "] Envio datos (" << lecturas * 10 
             << " vehiculos). Buffer: " << buffer.size() << "/" << TAMANO_MAX << endl;
        
        lecturas++;
        cvVacio.notify_one();
    }
}

void moduloConsumidor(int id) {
    while (ejecutando || !buffer.empty()) {
        unique_lock<mutex> lock(mtx);
        if (cvVacio.wait_for(lock, chrono::milliseconds(1000), [] { return !buffer.empty(); })) {
            DatoTrafico dato = buffer.front();
            buffer.pop();
            
            cout << "  [Modulo " << id << "] Procesando datos de Sensor " 
                 << dato.idSensor << ": " << dato.vehiculos << " veh/min." << endl;
            
            cvLleno.notify_one();
        }
    }
}

int main() {
    cout << "=== SIMULACION CONCURRENCIA SIGET (Productor-Consumidor) ===\n\n";

    thread s1(sensorProductor, 1);
    thread s2(sensorProductor, 2);
    thread s3(sensorProductor, 3);
    thread m1(moduloConsumidor, 101);
    thread m2(moduloConsumidor, 102);

    this_thread::sleep_for(chrono::seconds(6));
    ejecutando = false;

    cvLleno.notify_all();
    cvVacio.notify_all();

    s1.join(); s2.join(); s3.join();
    m1.join(); m2.join();

    cout << "\n=== Simulacion finalizada correctamente ===" << endl;
    return 0;
}