#include <Arduino.h>
#include <Controllino.h>

// Definición de arrays
int salidas[9] = {CONTROLLINO_D0, CONTROLLINO_D6, CONTROLLINO_D12, CONTROLLINO_D13, CONTROLLINO_D14, CONTROLLINO_D8, CONTROLLINO_D2, CONTROLLINO_D1, CONTROLLINO_D7};
int entradas[3] = {CONTROLLINO_I16, CONTROLLINO_I17, CONTROLLINO_I18};
int* punteroSal = salidas;
int* punteroEnt = entradas;

// FSM
enum Estado { 
  OFF, 
  NORMAL_SPIRAL, 
  INVERTED_SPIRAL 
};
Estado estado_actual = OFF;

// Control de tiempo
unsigned long tp = 0;
const unsigned long ts = 500; 

// Posición de LED
int posicion = 0;

// Lectura de botones
bool botonNormal = false;
bool botonInvertido = false;
bool botonApagar = false;

void leerEntradas() {
  botonNormal    = digitalRead(*(punteroEnt + 0)) == HIGH;
  botonInvertido = digitalRead(*(punteroEnt + 1)) == HIGH;
  botonApagar    = digitalRead(*(punteroEnt + 2)) == HIGH;
}

void apagarTodo() {
  for (int i = 0; i < 9; i++) {
    digitalWrite(*(punteroSal + i), LOW);
  }
}

void espiralNormal() {
  if (millis() - tp >= ts) {
    tp = millis();
    int anterior = (posicion == 0) ? 8 : posicion - 1;

    digitalWrite(*(punteroSal + anterior), LOW);
    digitalWrite(*(punteroSal + posicion), HIGH);

    posicion = (posicion + 1) % 9;
  }
}

void espiralInvertida() {
  if (millis() - tp >= ts) {
    tp = millis();
    int siguiente = (posicion == 8) ? 0 : posicion + 1;

    digitalWrite(*(punteroSal + siguiente), LOW);
    digitalWrite(*(punteroSal + posicion), HIGH);

    posicion = (posicion == 0) ? 8 : posicion - 1;
  }
}

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 9; i++) {
    pinMode(*(punteroSal + i), OUTPUT);
  }
  for (int i = 0; i < 3; i++) {
    pinMode(*(punteroEnt + i), INPUT);
  }

  apagarTodo();  // Estado inicial
}

void loop() {
  leerEntradas();

  switch (estado_actual) {
    case NORMAL_SPIRAL:
      espiralNormal();
      if (botonInvertido) {
        estado_actual = INVERTED_SPIRAL;
        posicion = 8;
        apagarTodo();
        Serial.println("Estado: INVERTED_SPIRAL");
      } else if (botonApagar) {
        estado_actual = OFF;
        apagarTodo();
        Serial.println("Estado: OFF");
      }
      break;

    case INVERTED_SPIRAL:
      espiralInvertida();
      if (botonNormal) {
        estado_actual = NORMAL_SPIRAL;
        posicion = 0;
        apagarTodo();
        Serial.println("Estado: NORMAL_SPIRAL");
      } else if (botonApagar) {
        estado_actual = OFF;
        apagarTodo();
        Serial.println("Estado: OFF");
      }
      break;

    case OFF:
      apagarTodo();
      if (botonNormal) {
        estado_actual = NORMAL_SPIRAL;
        posicion = 0;
        Serial.println("Estado: NORMAL_SPIRAL");
      } else if (botonInvertido) {
        estado_actual = INVERTED_SPIRAL;
        posicion = 8;
        Serial.println("Estado: INVERTED_SPIRAL");
      }
      break;
  }
}