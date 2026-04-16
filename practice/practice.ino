#include <Arduino.h>

// --------- ESTRUCTURAS ---------

typedef struct {
  int value;
  uint32_t timestamp;
} SensorData;

typedef struct {
  int pin;
  QueueHandle_t queue;
} SensorParams;

// --------- RECURSOS GLOBALES ---------

QueueHandle_t queue1;
QueueHandle_t queue2;

SemaphoreHandle_t serialMutex;
SemaphoreHandle_t touchMutex; 

// --------- FUNCIÓN A (LECTURA SENSOR) ---------

void sensorTask(void *pvParameters) {
  SensorParams *params = (SensorParams*) pvParameters;
  SensorData data;

  while (1) {


    if (xSemaphoreTake(touchMutex, portMAX_DELAY)) {
      data.value = touchRead(params->pin);
      xSemaphoreGive(touchMutex);
    }

    data.timestamp = millis();

    xQueueSend(params->queue, &data, portMAX_DELAY);

    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

// --------- FUNCIÓN B (ENVÍO SERIAL JSON) ---------

void serialTask(void *pvParameters) {
  QueueHandle_t queue = (QueueHandle_t) pvParameters;
  SensorData data;

  while (1) {
    if (xQueueReceive(queue, &data, portMAX_DELAY)) {

      if (xSemaphoreTake(serialMutex, portMAX_DELAY)) {

        Serial.print("{\"value\":");
        Serial.print(data.value);
        Serial.print(",\"time\":");
        Serial.print(data.timestamp);
        Serial.println("}");

        xSemaphoreGive(serialMutex);
      }
    }
  }
}

// --------- SETUP ---------

void setup() {
  Serial.begin(115200);

  // Crear colas
  queue1 = xQueueCreate(10, sizeof(SensorData));
  queue2 = xQueueCreate(10, sizeof(SensorData));

  // Crear mutex
  serialMutex = xSemaphoreCreateMutex();
  touchMutex  = xSemaphoreCreateMutex();

  // Parámetros de tareas
  static SensorParams sensor1Params;
  static SensorParams sensor2Params;

  sensor1Params.pin = T0;  
  sensor1Params.queue = queue1;

  sensor2Params.pin = T7;  
  sensor2Params.queue = queue2;

  // Inicializar touch 
  touchAttachInterrupt(T0, NULL, 40);
  touchAttachInterrupt(T7, NULL, 40);

  // Crear tareas de sensores
  xTaskCreate(sensorTask, "Sensor1", 4096, &sensor1Params, 1, NULL);
  xTaskCreate(sensorTask, "Sensor2", 4096, &sensor2Params, 1, NULL);

  // Crear tareas de envío serial
  xTaskCreate(serialTask, "Serial1", 4096, queue1, 1, NULL);
  xTaskCreate(serialTask, "Serial2", 4096, queue2, 1, NULL);
}

// --------- LOOP ---------

void loop() {
  // vacío
}