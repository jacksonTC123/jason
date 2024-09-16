
#include <EEPROM.h>
#define PUL2 2
#define PUL1 3
#define LED1 0
#define BAURENT 9600
#define REINICIO 0
#define DEBOUNCE_DELAY 50 
#define MIN 0
#define MAX 100
           // Tiempo de retardo para evitar rebote
unsigned long lastDebounceTime = 0;  // Tiempo en que se detectó el último cambio
int direccion = 0;
unsigned int CONT = 0;               // Inicializamos CONT, pero lo cargaremos desde la EEPROM
unsigned int TIME = 0;
unsigned int PASO = 1;
volatile bool flag = false;

void setup() {
  Serial.begin(BAURENT);
  
  // Leer el valor almacenado en la EEPROM al iniciar
  EEPROM.get(direccion, CONT);
  
  // Si el valor leído no está en el rango esperado (por ejemplo, valores iniciales incorrectos), reiniciar CONT
  if (CONT > MAX) {
    CONT = 0; // Resetear si el valor es inválido
  }

  Serial.print("Contador inicial desde EEPROM: ");
  Serial.println(CONT);

  pinMode(PUL1, INPUT);
  pinMode(PUL2, INPUT);
  
  // Configurar interrupciones para sumar y restar
  attachInterrupt(digitalPinToInterrupt(PUL1), sumar, FALLING);
  attachInterrupt(digitalPinToInterrupt(PUL2), restar, FALLING);
}

void loop() {
  delay(100);  // Pequeño retardo para no saturar el loop principal
}

void sumar() {
  if (CONT >= MAX) {
    Serial.println("El contador ha alcanzado el valor máximo.");
  } else {
    unsigned long currentTime = millis();  // Obtener el tiempo actual

    // Verificar si ha pasado el tiempo suficiente desde la última pulsación (antirrebote)
    if (currentTime - lastDebounceTime > DEBOUNCE_DELAY) {
      flag = !flag;
      if (flag) {
        CONT = CONT + PASO;  // Incrementar el contador
        flag = false;
        
        // Guardar el nuevo valor de CONT en la EEPROM
        EEPROM.put(direccion, CONT);
        
        // Mostrar el contador en el monitor serial
        Serial.print("Pulsaciones (sumar): ");
        Serial.println(CONT);
      }
      lastDebounceTime = currentTime;  // Actualizar el tiempo del último cambio
    }
  }
}

void restar() {
  if (CONT <= MIN) {
    Serial.println("El contador ya está en 0.");
  } else {
    unsigned long currentTime = millis();  // Obtener el tiempo actual

    // Verificar si ha pasado el tiempo suficiente desde la última pulsación (antirrebote)
    if (currentTime - lastDebounceTime > DEBOUNCE_DELAY) {
      flag = !flag;
      if (flag) {
        CONT = CONT - PASO;  // Decrementar el contador
        flag = false;
        
        // Guardar el nuevo valor de CONT en la EEPROM
        EEPROM.put(direccion, CONT);
        
        // Mostrar el contador en el monitor serial
        Serial.print("Pulsaciones (restar): ");
        Serial.println(CONT);
      }
      lastDebounceTime = currentTime;  // Actualizar el tiempo del último cambio
    }
  }
}