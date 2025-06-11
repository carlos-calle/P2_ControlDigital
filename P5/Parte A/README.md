# Parte A: Patrones de Espiral en Matriz 3×3

## Descripción
En esta parte se implementan tres patrones de encendido de una matriz de LEDs 3×3 conectada a un Controllino Mega, controlados por tres pulsadores:

1. **Espiral normal**: Recorrido de LEDs en sentido horario.
2. **Espiral inversa**: Recorrido de LEDs en sentido antihorario.
3. **Apagar/Reiniciar**: Apaga todos los LEDs.

## Hardware y Pines

- **Salidas digitales** (matriz 3×3):
  ```cpp
  int salidas[9] = {
    CONTROLLINO_D0, CONTROLLINO_D6, CONTROLLINO_D12,
    CONTROLLINO_D13, CONTROLLINO_D14, CONTROLLINO_D8,
    CONTROLLINO_D2, CONTROLLINO_D1, CONTROLLINO_D7
  };
  ```
- **Entradas digitales** (pulsadores):
  ```cpp
  int entradas[3] = {
    CONTROLLINO_I16,  // Botón Espiral normal
    CONTROLLINO_I17,  // Botón Espiral inversa
    CONTROLLINO_I18   // Botón Apagar/Reiniciar
  };
  ```

## Lógica de Control

1. Se define una **FSM** con tres estados:
   ```cpp
   enum Estado { OFF, NORMAL_SPIRAL, INVERTED_SPIRAL };
   Estado estado_actual = OFF;
   ```
2. Se utiliza `millis()` para controlar el retardo no bloqueante entre pasos:
   ```cpp
   const unsigned long ts = 500;  // Tiempo entre pasos (ms)
   unsigned long tp = 0;          // Marca de tiempo previa
   ```
3. **Funciones clave**:
   - `leerEntradas()`: Actualiza señales de los pulsadores.
   - `apagarTodo()`: Apaga todos los LEDs.
   - `espiralNormal()` y `espiralInvertida()`: Encienden el siguiente LED según el patrón y apagan el anterior.

## Uso

1. Carga el sketch `ParteA/Practica2_ParteA.ino` en Arduino IDE.
2. Conecta la matriz de LEDs y los pulsadores a los pines configurados.
3. Al presionar cada botón:
   - **Espiral normal** inicia el recorrido horario desde la posición 0.
   - **Espiral inversa** inicia el recorrido antihorario desde la posición 8.
   - **Apagar/Reiniciar** detiene y apaga la matriz.

