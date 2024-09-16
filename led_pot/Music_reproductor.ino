#define BTN_PLAY 2  // Botón 1 para Play/Pausa
#define BTN_STOP 3        // Botón 2 para Stop
#define BAURENT 9600
#define DEBOUNCE_DELAY 50

unsigned long previousMillis = 0;  // Tiempo anterior 
unsigned long lastDebounceTime = 0;  // Tiempo del último cambio para evitar rebote

volatile bool playState = false;  // false = Pausado, true = Reproduciendo
volatile bool stopFlag = false;   // Flag para saber si se ha detenido

unsigned long elapsedTime = 0;  // Tiempo total transcurrido en milisegundos

volatile bool playPausePressed = false;
volatile bool stopPressed = false;

void setup() {
  Serial.begin(BAURENT);
  
  pinMode(BTN_PLAY, INPUT);
  pinMode(BTN_STOP, INPUT);

  // Interrupciones para botones
  attachInterrupt(digitalPinToInterrupt(BTN_PLAY), playPauseInterrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(BTN_STOP), stopInterrupt, FALLING);
}

void loop() {
  if (playPausePressed) {
    playPause();
    playPausePressed = false;
  }
  
  if (stopPressed) {
    stopMusic();
    stopPressed = false;
  }

  if (playState == 1) {
    unsigned long currentMillis = millis();
    
    if (currentMillis - previousMillis >= 1000) {  // Actualizar cada segundo
      elapsedTime += 1000;  // Sumar 1 segundo al tiempo transcurrido
      previousMillis = currentMillis;

      displayTime();
    }
  }
}

void playPauseInterrupt() {
  unsigned long currentTime = millis();
  if (currentTime - lastDebounceTime > DEBOUNCE_DELAY) {
    playPausePressed = true;  // Se marca para ser manejado en el loop
    lastDebounceTime = currentTime;
  }
}

void stopInterrupt() {
  unsigned long currentTime = millis();
  if (currentTime - lastDebounceTime > DEBOUNCE_DELAY) {
    stopPressed = true;  // Se marca para ser manejado en el loop
    lastDebounceTime = currentTime;
  }
}

void playPause() {
  if (stopFlag == 1) {
    // Si el botón de Stop se había presionado, resetear el flag de stop
    stopFlag = 0;
    elapsedTime = 0;  // Reiniciar el tiempo a 00:00
    previousMillis = millis();  // Reiniciar el contador de tiempo
  }

  if (playState == 0) {
    // Si estaba pausado, retomar la reproducción
    playState = 1;
    Serial.println("Reproduciendo...");
  } else if (playState == 1) {
    // Si estaba reproduciendo, pausar
    playState = 0;
    Serial.println("Pausa.");
  }
}

void stopMusic() {
  // Reiniciar el tiempo y detener la reproducción
  playState = 0;
  stopFlag = 1;
  elapsedTime = 0;
  previousMillis = millis();  // Reiniciar el conteo de tiempo
  Serial.println("Stop. Tiempo reiniciado.");
  displayTime();  // Mostrar 00:00 en el monitor serial
}

void displayTime() {
  unsigned int seconds = (elapsedTime / 1000) % 60;  // Calcular los segundos
  unsigned int minutes = (elapsedTime / 1000) / 60;  // Calcular los minutos

  // Mostrar el tiempo en formato MM:SS
  Serial.print(minutes < 10 ? "0" : "");
  Serial.print(minutes);
  Serial.print(":");
  Serial.print(seconds < 10 ? "0" : "");
  Serial.println(seconds);
}
