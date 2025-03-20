//Proyecto motor
//botones
#define LED1 7
#define LED2 8
#define LED3 9
#define BUTTON1 2
#define BUTTON2 3
#define BUTTON3 4
#define BUTTON4 5
#define BUTTON5 6
#define MOTOR1 10  // Control del motor (sentido 1, usando el transistor N2222)
#define MOTOR2 11  // Control del motor (sentido 2, usando el transistor N2222)

unsigned long previousMillis = 0;
const long automationInterval = 10000;  // Tiempo de espera para activar el giro automatizado (10 segundos)
const long motorRunTime = 5000;  // Tiempo en que el motor corre en un sentido (5 segundos)
bool motorRunning = false;
bool automationActive = true;  // La automatización está activa por defecto
bool motorDirection = true;  // true = sentido horario, false = antihorario
bool systemActive = true;  // Controla si el sistema está activo o apagado

void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(MOTOR1, OUTPUT);
  pinMode(MOTOR2, OUTPUT);
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(BUTTON3, INPUT_PULLUP);
  pinMode(BUTTON4, INPUT_PULLUP);
  pinMode(BUTTON5, INPUT_PULLUP);  
}

void loop() {
  unsigned long currentMillis = millis();

  // Si no hay interacción durante 10 segundos, inicia el ciclo de automatización
  if (automationActive && (currentMillis - previousMillis >= automationInterval)) {
    previousMillis = currentMillis;  // Reinicia el temporizador
    automatedMotorCycle();  // Llama al ciclo de motor automatizado
  }

  // Lógica para los botones 1, 2 y 3
  if (digitalRead(BUTTON1) == LOW || digitalRead(BUTTON2) == LOW) {
    if (systemActive) {
      systemActive = false;  // Desactiva el sistema para evitar la automatización
      automationActive = false;  // Desactiva la automatización

      digitalWrite(LED1, HIGH);  // Enciende LED1
      delay(3000);  // Espera 3 segundos
      blinkLED(LED1, 5000);  // Parpadeo de LED1 por 5 segundos
      controlMotorAndLED(LED2, MOTOR1, motorDirection);  // Control del motor y LED2

      automationActive = true;  // Reactiva el ciclo automático después de la acción
    }
  }
  // Lógica para el botón 3
  else if (digitalRead(BUTTON3) == LOW) {
    if (systemActive) {
      systemActive = false;  // Desactiva el sistema para evitar la automatización
      automationActive = false;  // Desactiva la automatización

      digitalWrite(LED2, HIGH); // Enciende LED2
      delay(3000);  // Espera 3 segundos
      blinkLED(LED2, 5000);  // Parpadeo de LED2 por 5 segundos
      controlMotorAndLED(LED3, MOTOR2, !motorDirection);  // Control del motor y LED3, con dirección invertida

      automationActive = true;  // Reactiva el ciclo automático después de la acción
    }
  }

  // Si el botón 4 o 5 es presionado, apaga todo (motor y LEDs) y desactiva la automatización
  if (digitalRead(BUTTON4) == LOW || digitalRead(BUTTON5) == LOW) {
    systemActive = false;  // Desactiva el sistema
    automationActive = false;  // Desactiva la automatización
    motorRunning = false;
    digitalWrite(MOTOR1, LOW);
    digitalWrite(MOTOR2, LOW);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);

    // Se asegura que después de detener el sistema, pueda volver a activarse
    previousMillis = millis();  // Reinicia el temporizador para que el ciclo automático pueda reiniciarse
  }

  // Si el sistema está apagado, esperar hasta que se presione un botón diferente para activarlo
  if (!systemActive && (digitalRead(BUTTON1) == LOW || digitalRead(BUTTON2) == LOW || digitalRead(BUTTON3) == LOW)) {
    systemActive = true;  // Reactiva el sistema
    previousMillis = millis();  // Reinicia el temporizador
    automationActive = true;  // Reactiva la automatización
  }
}

void blinkLED(int led, int duration) {
  unsigned long startTime = millis();
  while (millis() - startTime < duration) {
    digitalWrite(led, HIGH);
    delay(500);
    digitalWrite(led, LOW);
    delay(500);
  }
}

void controlMotorAndLED(int led, int motorPin, bool direction) {
  digitalWrite(led, HIGH);  // Enciende el LED correspondiente
  motorRunning = true;  // El motor está corriendo
  if (motorPin == MOTOR1) {
    digitalWrite(MOTOR1, HIGH);  // Motor en sentido horario
    digitalWrite(MOTOR2, LOW);  // Apaga MOTOR2
  } else {
    digitalWrite(MOTOR2, HIGH);  // Motor en sentido antihorario
    digitalWrite(MOTOR1, LOW);  // Apaga MOTOR1
  }
  delay(motorRunTime);  // El motor corre durante 5 segundos
  digitalWrite(MOTOR1, LOW);  // Apaga MOTOR1
  digitalWrite(MOTOR2, LOW);  // Apaga MOTOR2
  digitalWrite(led, LOW);  // Apaga el LED
  motorDirection = !direction;  // Cambia la dirección del motor
  previousMillis = millis();  // Actualiza el tiempo de referencia
}

void automatedMotorCycle() {
  // Primer ciclo de giro automático: Parpadeo simultáneo de LED1 y LED2
  unsigned long startTime = millis();
  while (millis() - startTime < 2500) { // Parpadeo durante 2.5 segundos
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    delay(500);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    delay(500);
  }
  digitalWrite(LED3, HIGH);    // Enciende LED3
  digitalWrite(MOTOR1, HIGH);  // Motor en sentido horario
  digitalWrite(MOTOR2, LOW);   // Apaga MOTOR2
  delay(motorRunTime);         // El motor corre durante 5 segundos
  digitalWrite(MOTOR1, LOW);   // Apaga MOTOR1
  digitalWrite(LED3, LOW);     // Apaga LED3

  delay(10000);  // Espera 10 segundos

  // Segundo ciclo de giro automático: Parpadeo simultáneo de LED3 y LED2
  startTime = millis();
  while (millis() - startTime < 2500) { // Parpadeo durante 2.5 segundos
    digitalWrite(LED3, HIGH);
    digitalWrite(LED2, HIGH);
    delay(500);
    digitalWrite(LED3, LOW);
    digitalWrite(LED2, LOW);
    delay(500);
  }
  digitalWrite(LED1, HIGH);    // Enciende LED1
  digitalWrite(MOTOR1, LOW);   // Apaga MOTOR1
  digitalWrite(MOTOR2, HIGH);  // Motor en sentido antihorario
  delay(motorRunTime);         // El motor corre durante 5 segundos
  digitalWrite(MOTOR2, LOW);   // Apaga MOTOR2
  digitalWrite(LED1, LOW);     // Apaga LED1

  previousMillis = millis();  // Actualiza el tiempo de referencia
}