//***** Definição dos Pinos *********
#define pinIN1 10    // Motor 1
#define pinIN2 9     // Motor 1
#define pinIN3 6     // Motor 2
#define pinIN4 5     // Motor 2 
#define pinPot1  A1  // Potenciômetro Frente e Reverso
#define pinPot2  A2  // Potenciômetro Direito e Esquerdo
#define pinBotao A3  // Botão

//***** Variáveis para controle do botão  ****
boolean botaoAnt = HIGH;
boolean botao    = HIGH;
boolean botao1 = false;

//***** Variáveis para registro do estado do Joystick e motor  ****
int pot1 = 512;
int pot2 = 512;
int threshold = 50;

void setup() {
  pinMode(pinIN1, OUTPUT);
  pinMode(pinIN2, OUTPUT);
  pinMode(pinIN3, OUTPUT);
  pinMode(pinIN4, OUTPUT);
  pinMode(pinBotao, INPUT_PULLUP);  
  Serial.begin(9600); // Inicializa o Monitor Serial
}

void loop() {
  // Leitura do Joystick
  pot1 = analogRead(pinPot1);
  pot2 = analogRead(pinPot2);

  // Leitura do Botão do Joystick
  botao = digitalRead(pinBotao);
  if (botao == LOW && (botao != botaoAnt)) {
     botao1 = !botao1; // Alterna o estado do botão
  }
  botaoAnt = botao;

  // Freio de emergência
  if (botao1) {
    digitalWrite(pinIN1, HIGH);
    digitalWrite(pinIN2, HIGH);
    digitalWrite(pinIN3, HIGH);
    digitalWrite(pinIN4, HIGH);
    Serial.println("Freio acionado");
    delay(100);
    return;
  }

  // Controle de Movimento
  int velocidadeMotor1 = 0;
  int velocidadeMotor2 = 0;

  // Frente e trás
  if (abs(pot1 - 512) > threshold) {
    if (pot1 < 512) { 
      // Reverso
      velocidadeMotor1 = map(pot1, 511, 0, 0, 255);
      velocidadeMotor2 = velocidadeMotor1;
      digitalWrite(pinIN1, LOW);
      analogWrite(pinIN2, velocidadeMotor1);
      digitalWrite(pinIN3, LOW);
      analogWrite(pinIN4, velocidadeMotor2);
    } else {
      // Para frente
      velocidadeMotor1 = map(pot1, 512, 1023, 0, 255);
      velocidadeMotor2 = velocidadeMotor1;
      analogWrite(pinIN1, velocidadeMotor1);
      digitalWrite(pinIN2, LOW);
      analogWrite(pinIN3, velocidadeMotor2);
      digitalWrite(pinIN4, LOW);
    }
  }

  // Direita e esquerda (Ajustar velocidades para curvas)
  if (abs(pot2 - 512) > threshold) {
    if (pot2 < 512) { 
      // Gira para a esquerda
      int curva = map(pot2, 511, 0, 0, 255);
      analogWrite(pinIN1, 0);
      analogWrite(pinIN2, curva);
    } else {
      // Gira para a direita
      int curva = map(pot2, 512, 1023, 0, 255);
      analogWrite(pinIN3, curva);
      analogWrite(pinIN4, 0);
    }
  }

  // Se o joystick está centralizado, pare os motores
  if (abs(pot1 - 512) <= threshold && abs(pot2 - 512) <= threshold) {
    digitalWrite(pinIN1, LOW);
    digitalWrite(pinIN2, LOW);
    digitalWrite(pinIN3, LOW);
    digitalWrite(pinIN4, LOW);
  }

  delay(100); // Delay para estabilidade
}
