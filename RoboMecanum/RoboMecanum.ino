#include <ESP32_Servo.h>
#include <BluetoothSerial.h>
#include <math.h>


#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

Servo motor1;  //Motor Frente Direta
Servo motor2;  //Motor Frente Esquerda
Servo motor3;  //Motor Tras Direita
Servo motor4;  //Motor Tras Esquerda

int speedH = 2000;   // Velocidade no sentido horario
int speedAH = 1000;  // Velocidade no sentido anti-horario

BluetoothSerial SerialBT;

void setup() {
  motor1.attach(12,1000,2000);
  motor2.attach(14,1000,2000);
  motor3.attach(27,1000,2000);
  motor4.attach(26,1000,2000);

  //Nesse caso deve se configurar o tempo de caractere por segundo do bluetooth para 0,008680 ms
  //1 / 115200 * 0,001
  Serial.begin(115200); 
  if (!SerialBT.begin("ESP32test")) {
    Serial.println("Falhou a conexão bluetooth");
    abort();
  }else{
    Serial.println("Bluetooth device address: " + SerialBT.getBtAddressString());
  }
}

bool botaoPressionado = false;
unsigned long tempoInicial = 0;
unsigned long duracao = 200;

void loop() {
  if (SerialBT.available()) {
    String data = SerialBT.readStringUntil('\n');

    if (!data.isEmpty()) {
      char command = data[0];

      verificaBotao();

      switch (command) {
        case 'F':
          Serial.println("Andando Frente");
          Frente();
          botaoPressionado = true;
          break;

        case 'T':
          Serial.println("Andando Tras");
          Tras();
          botaoPressionado = true;
          break;

        case 'D':
          Serial.println("Andando Direita");
          Direita();
          botaoPressionado = true;
          break;

        case 'E':
          Serial.println("Andando Esquerda");
          Esquerda();
          botaoPressionado = true;
          break;

        case 'S':
          Serial.println("Parando");
          Parar();
          botaoPressionado = false;
          break;
      }
    }
  }
}

void verificaBotao() {
  unsigned long tempoAtual = millis();  // Obtém o tempo atual em milissegundos

  if (botaoPressionado && tempoAtual - tempoInicial >= duracao) {
    botaoPressionado = false;
  }
}


void Parar() {
  motor1.writeMicroseconds(1500);
  motor2.writeMicroseconds(1500);
  motor3.writeMicroseconds(1500);
  motor4.writeMicroseconds(1500);
}

void Frente() {
  motor1.writeMicroseconds(speedH);
  motor2.writeMicroseconds(speedAH);
  motor3.writeMicroseconds(speedH);
  motor4.writeMicroseconds(speedAH);
}

void Tras() {
  motor1.writeMicroseconds(speedAH);
  motor2.writeMicroseconds(speedH);
  motor3.writeMicroseconds(speedAH);
  motor4.writeMicroseconds(speedH);
}

void Direita() {
  motor1.writeMicroseconds(speedH);
  motor2.writeMicroseconds(speedH);
  motor3.writeMicroseconds(speedH);
  motor4.writeMicroseconds(speedH);
}

void Esquerda() {
  motor1.writeMicroseconds(speedAH);
  motor2.writeMicroseconds(speedAH);
  motor3.writeMicroseconds(speedAH);
  motor4.writeMicroseconds(speedAH);
}
