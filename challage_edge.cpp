#include <LiquidCrystal_I2C.h>


//Define os pinos ao LED associado ao sensor PIR,
//pino de leitura do LDR (sensor de luminosidade) e ao LED associado ao LDR,
const int pinoLED_PIR = 13;
const int pinoPIR = 2;
const int LDR_PIN = A0;
const int pinoLED_LDR = 12;

const int enderecoLCD = 0x27;
LiquidCrystal_I2C lcd(enderecoLCD, 16, 2);


// Declaração das variáveis para controle do programa,
int contagemPisca = 0;
int caloriasGastas = 0;
unsigned long ultimoMovimento = 0;
unsigned long tempoMensagemPIR = 0;
unsigned long tempoMensagemLDR = 0;
bool exibindoMensagemPIR = false;
bool exibindoMensagemLDR = false;
bool exibindoMensagemInicial = true;

void setup() {
  pinMode(pinoLED_PIR, OUTPUT);
  pinMode(pinoPIR, INPUT);
  pinMode(LDR_PIN, INPUT);
  pinMode(pinoLED_LDR, OUTPUT);

  digitalWrite(pinoLED_PIR, LOW);
  digitalWrite(pinoLED_LDR, LOW);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("     Bike-E");
  lcd.setCursor(0, 1);
  lcd.print("    Mahindra");
}

void loop() {
  int estadoPIR = digitalRead(pinoPIR);

  
// Lê o estado do sensor PIR
  if (estadoPIR == HIGH && millis() - ultimoMovimento > 2000) {
    digitalWrite(pinoLED_PIR, HIGH);
    delay(500);
    digitalWrite(pinoLED_PIR, LOW);

    // Acende e apaga o LED para indicar 1km rodado
    contagemPisca++;
    caloriasGastas += 23;
    ultimoMovimento = millis();
    

    // Incrementa a contagem, km rodado e as calorias gastas,
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Voce andou ");
    lcd.print(contagemPisca);
    lcd.print(" km");

    lcd.setCursor(0, 1);
    lcd.print("Gastou ");
    lcd.print(caloriasGastas);
    lcd.print(" kcal");

    exibindoMensagemPIR = true;
    tempoMensagemPIR = millis();
    

    // Verifica se certa distância foi percorrida e certa quantidade de calorias foi queimada, e exibe uma mensagem de parabéns.
    if (contagemPisca == 5 && caloriasGastas == 115) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Parabens! ganhou");
      lcd.setCursor(0, 1);
      lcd.print("    50 moedas");
      delay(5000);
    }
  }

  if (exibindoMensagemPIR && (millis() - tempoMensagemPIR > 2000)) {
    exibindoMensagemPIR = false;
  }

  if (!exibindoMensagemPIR) {
    int ldrValue = analogRead(LDR_PIN);

// Acende o LED ao detectar que esta escurecendo

    if (ldrValue < 500) {
      digitalWrite(pinoLED_LDR, HIGH);

      if (!exibindoMensagemLDR) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("   Anoiteceu");
        lcd.setCursor(0, 1);
        lcd.print("  Farol ligado");
        exibindoMensagemLDR = true;
        tempoMensagemLDR = millis() + 3000;
      }

      
// Apaga o LED ao detectar que o dia está iluminado
    } else {
      digitalWrite(pinoLED_LDR, LOW);
      if (exibindoMensagemLDR) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("    Dia claro");
        lcd.setCursor(0, 1);
        lcd.print(" Farol desligado");
        exibindoMensagemLDR = false;
        exibindoMensagemInicial = true;
        delay(3000);
      }
    }
  }

  if (exibindoMensagemInicial) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("     Bike-E");
    lcd.setCursor(0, 1);
    lcd.print("    Mahindra");
    exibindoMensagemInicial = false;
  }

  delay(100);
}

