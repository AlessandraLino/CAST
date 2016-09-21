#include <Ultrasonic.h>

#define TRIGGER 3
#define ECHO 4

#define FORWARD 'F'
#define BACK    'B'
#define LEFT    'L'
#define RIGHT   'R'

// Motor 1 (Esquerda)
#define PIN_O1_MOTOR_ESQ 6
#define PIN_O2_MOTOR_ESQ 9

// Motor 2 (Direita)
#define PIN_O1_MOTOR_DIR 10
#define PIN_O2_MOTOR_DIR 11

#define POTENCIA_SECUNDARIO 60
#define POTENCIA_PRINCIPAL 120
#define POTENCIA_FORWARD 100

#define LIMITE_BRANCO 20
#define MIN_DCM 20.0


Ultrasonic ultrasonic(TRIGGER, ECHO);
long ms = 0;
float dcm = 0;

void setup() {
  // Inicializa os pinos dos motores como saida
  pinMode(PIN_O1_MOTOR_ESQ, OUTPUT);
  pinMode(PIN_O2_MOTOR_ESQ, OUTPUT);
  pinMode(PIN_O1_MOTOR_DIR, OUTPUT);
  pinMode(PIN_O2_MOTOR_DIR, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  // Adquire os valores do sensor esquerdo e direito (portas A0 e A1)
  int sen1=analogRead(A0);
  int sen2=analogRead(A1);

  ms  = ultrasonic.timing();
  dcm = ultrasonic.convert(ms, Ultrasonic::CM);

  if (dcm > MIN_DCM) {
    if(sen1 < LIMITE_BRANCO  &&  sen2 < LIMITE_BRANCO){
       Serial.println("GOING FORWARD");
       control(FORWARD);
    }
    else if(sen1 > LIMITE_BRANCO && sen2 < LIMITE_BRANCO){
      Serial.println("GOING LEFT");
      control(LEFT);
    }
    else if(sen2 > LIMITE_BRANCO && sen1 < LIMITE_BRANCO){
      Serial.println("GOING RIGHT");
      control(RIGHT);
    }
    else if(sen1 > LIMITE_BRANCO && sen2 > LIMITE_BRANCO){
      Serial.println("GOING BACK");
      control(BACK);
    }
  }
  else {
    stop();
  }

  delay(200);              
}

void stop() {
  Serial.println("STOPPING...");
  analogWrite(PIN_O1_MOTOR_ESQ, 0);
  analogWrite(PIN_O2_MOTOR_ESQ, 0);
  analogWrite(PIN_O1_MOTOR_DIR, 0);
  analogWrite(PIN_O2_MOTOR_DIR, 0);
}

//Funcao que controla os motores
void control(char dir){
    switch(dir){
       //Anda para frente   
       case FORWARD:
           analogWrite(PIN_O1_MOTOR_ESQ, 0);
           analogWrite(PIN_O2_MOTOR_ESQ, POTENCIA_FORWARD);
           analogWrite(PIN_O1_MOTOR_DIR, 0);
           analogWrite(PIN_O2_MOTOR_DIR, POTENCIA_FORWARD);
           break;
              
       //Anda para tras   
       case BACK:
           analogWrite(PIN_O1_MOTOR_ESQ, POTENCIA_FORWARD);
           analogWrite(PIN_O2_MOTOR_ESQ, 0);
           analogWrite(PIN_O1_MOTOR_DIR, POTENCIA_FORWARD);
           analogWrite(PIN_O2_MOTOR_DIR, 0);
           break;
              
       //Anda para direita   
       case RIGHT:
           analogWrite(PIN_O1_MOTOR_ESQ, POTENCIA_SECUNDARIO);
           analogWrite(PIN_O2_MOTOR_ESQ, 0);
           analogWrite(PIN_O1_MOTOR_DIR, 0);
           analogWrite(PIN_O2_MOTOR_DIR, POTENCIA_PRINCIPAL);
           break;
              
       //Anda para esquerda
       case LEFT:
           analogWrite(PIN_O1_MOTOR_ESQ, 0);
           analogWrite(PIN_O2_MOTOR_ESQ, POTENCIA_PRINCIPAL);
           analogWrite(PIN_O1_MOTOR_DIR, POTENCIA_SECUNDARIO);
           analogWrite(PIN_O2_MOTOR_DIR, 0);
           break;        
      }       
}
