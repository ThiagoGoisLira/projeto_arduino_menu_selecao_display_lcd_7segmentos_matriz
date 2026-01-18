#include <TM1638lite.h>
#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);


// Teclado Matricial
const byte LINHAS = 4;
const byte COLUNAS = 4;
char TECLAS_MATRIZ[LINHAS][COLUNAS] = {
{'1', '2', '3', '4'}, {'5', '6', '7', '8'},
{'9', '0', '+', '-'}, {'*', '/', 'C', 'R'}
};

const byte PINOS_LINHAS[LINHAS] = {6, 7, 8, 9};
const byte PINOS_COLUNAS[COLUNAS] = {10, 11, 12, 13};

Keypad teclado_personalizado = Keypad(makeKeymap(TECLAS_MATRIZ), PINOS_LINHAS, PINOS_COLUNAS, LINHAS, COLUNAS);


// TM1638
TM1638lite tm(A0, A1, A2); 


// Variáveis para o Ping Pong Independente

unsigned long tempoAnteriorPing = 0;
const int intervaloPing = 100; // Velocidade da animação (ms)
int ledAtual = 0;
int direcao = 1;

//Controle de menu
int controle = 0;


//Variavel para contador
unsigned long tempoAnterior = 0;
long contador = 0;

//Chama funções declaradas no final
void mostraLcd(String x, String y);
void animarPingPong();
void lerTecladoMatriz();
void cronometro();

void setup() {
  tm.reset(); 
  tm.displayText("START"); 
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MENU INICIAL:");
  lcd.setCursor(0, 1);
  lcd.print("SELECIONE -SWx-");
  delay(1000);
}


void loop() {

  // Chama a animação a cada ciclo (ela só age se der o tempo do millis)
  //animarPingPong();
  //lerTecladoMatriz();


// 2. Leitura dos Botões do Módulo TM1638

  uint8_t botoes = tm.readButtons(); 
// 3. Menu de seleção expresso (Corrigido para não travar a animação)

  if (botoes != 0) {
    tm.reset(); // Limpa o display e LEDs do TM1638 ao trocar
    switch (botoes) {
      case 1 << 0: 
        tm.displayText("-1-     "); mostraLcd("Botao SW1:", "Botoes da matriz");
        controle = 1;
        break;
 
      case 1 << 1: 
        tm.displayText("-2-     "); mostraLcd("Botao SW2:", "Ping Pong Led");
        controle = 2;
        break;

      case 1 << 2:
        tm.displayText("-3-     "); mostraLcd("Botao SW3:", "Contador Turbo");
        controle = 3;
        break;

      case 1 << 3:
        tm.displayText("-4-     "); mostraLcd("Botao :", "SW4 selecionado");
        controle = 4;
        break;

      case 1 << 4:
        tm.displayText("-5-     "); mostraLcd("Botao :", " SW5 selecionado");
        controle = 5;
        break;

      case 1 << 5:
        tm.displayText("-6-     "); mostraLcd("Botao :", " SW6 selecionado");
        controle = 6;
        break;

      case 1 << 6:
        tm.displayText("-7-     "); mostraLcd("Botao :", " SW7 selecionado");
        controle = 7;
        break;

      case 1 << 7:
        tm.displayText("-8-     "); mostraLcd("Botao :", " SW8 selecionado");
        controle = 8;
        break;
    }
  }
  if(controle == 1){
      lerTecladoMatriz();
    }
  else if (controle == 2){
      animarPingPong();
    }
  else if (controle == 3){
      cronometro();
    }
}


//*** Função de mostra no LCD ***

void mostraLcd(String x, String y) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(x);
  lcd.setCursor(0, 1);
  lcd.print(y);
}


//*** Função para animação Ping Pong Independente ***
void animarPingPong() {
  if (millis() - tempoAnteriorPing >= intervaloPing) {// Verifica se passou o intervalo de 100ms
    tempoAnteriorPing = millis();
    for(int i = 0; i < 8; i++) {// Apaga o LED anterior (opcional, para efeito de rastro)
        tm.setLED(i, 0);
    }
    tm.setLED(ledAtual, 1);// Acende o LED atual
    ledAtual += direcao;// Incrementa ou decrementa a posição
    if (ledAtual >= 7 || ledAtual <= 0) {// Se chegar no fim ou no início, inverte a direção
      direcao *= -1;
    }
  }
}

//*** Função para leitura do teclado matricial ***
void lerTecladoMatriz(){
 // Leitura do Teclado Matricial
  char leitura_teclas = teclado_personalizado.getKey();
  if (leitura_teclas) {
    mostraLcd("Botao matriz:", String(leitura_teclas));
    return;
  }
} 
void cronometro(){
  if (millis() - tempoAnterior >= 50) {
    tempoAnterior = millis(); 
    for(int i = 0; i < 8; i++) tm.setLED(i, (i == ledAtual));
    ledAtual += direcao;
    if (ledAtual >= 7 || ledAtual <= 0) direcao = -direcao; 
    if (ledAtual > 7) ledAtual = 7;
    if (ledAtual < 0) ledAtual = 0;
    char buffer[16];
    sprintf(buffer, "%8ld", contador); 
    tm.displayText(buffer);
    contador++;
  }
}
