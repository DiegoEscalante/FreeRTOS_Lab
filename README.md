# Laboratorio 2: FreeRTOS
**Universidad de La Sabana — Internet de las Cosas**  
Autores: Angie Camila Cano Gómez · Carmen Celeste Durán Báez · Diego Eduardo Escalante García  
Profesor: Fabián Páez

---

## Descripción
Sistema de lectura de dos sensores capacitivos/touch en ESP32 usando FreeRTOS.
Los datos se envían por puerto serial en formato JSON con control de concurrencia
mediante mutex.

## Estructura del repositorio

```text
FreeRTOS_Lab/
├── practice/
│   └── practice.ino
├── Laboratorio2_FreeRTOS.pdf
├── README.md
├── diagrama_deadlock.png
├── diagrama_practica.png
└── diagrama_proyecto_final.png
```

## Cómo importar en Arduino IDE
1. Descargar o clonar el repositorio
2. Abrir Arduino IDE
3. Ir a **Archivo → Abrir** y seleccionar `practice/practice.ino`
4. Seleccionar la placa **ESP32 Dev Module**
5. Cargar el código

## Elementos FreeRTOS implementados
- **Tareas:** `Sensor1Task`, `Sensor2Task`, `Serial1Task`, `Serial2Task`
- **Colas:** `queue1`, `queue2` (capacidad 10 elementos)
- **Mutex:** `serialMutex` (acceso al puerto serial), `touchMutex` (lectura de sensores)
- **Reutilización:** función `sensorTask()` y `serialTask()` con parámetros `void*`
