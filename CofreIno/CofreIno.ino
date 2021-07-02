/**************************************************************************************************************************

  Aluno: Pedro Augusto Barbosa Aparecido                                        Matrícula: 170
  Curso: Engenharia de Software

  Projeto: CofrIno

  Atualização: 06/18/2021

**************************************************************************************************************************/

// ========================================================================================================================
// Inclusão de bibliotecas
#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Servo.h>
// ========================================================================================================================

// ========================================================================================================================
// Configuração das bibliotecas e criação de variáveis para mesmas

// numero de linhas e colunas do teclado matricial
const byte numLinhas = 4; 
const byte numColunas = 4;

// mapeamento dos pinos que estão sendo usados
byte pinosColunas[numColunas] = { A3, A2, A1, A0 };
byte pinosLinhas[numLinhas] = { 6, 7, 8, 9 };

// mapeamento das teclas
char teclas[numLinhas][numColunas] = {
                    {'1','2','3','A'},
                    {'4','5','6','B'},
                    {'7','8','9','C'},
                    {'*','0','#','D'}
                };

// instanciação de objetos das bibliotecas
Keypad teclado = Keypad(makeKeymap(teclas), pinosLinhas, pinosColunas, numLinhas, numColunas);
LiquidCrystal displayLCD(12, 11, 5, 4, 3, 2);
Servo mtServo;

// ========================================================================================================================

// ========================================================================================================================
// Criação funções

void cleanDisplay() {
  displayLCD.setCursor(0, 0);
  displayLCD.print("                ");
  displayLCD.setCursor(0, 1);
  displayLCD.print("                ");
}

void changePassword(String& password) {
  bool changed = false;
  int posDisplay = 0;
  String newPassword = "";
  char charKey = ' ';
  
  cleanDisplay();
  
  displayLCD.setCursor(0, 0);
  displayLCD.print("Digite a senha:");
  displayLCD.setCursor(0, 1);

  do {
    charKey = teclado.getKey();

    if (charKey != NO_KEY) {
      switch(charKey) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
          newPassword += charKey;
          displayLCD.print("*");
          displayLCD.setCursor(posDisplay, 1);
          posDisplay++;
          break;
        default:
          break;
      }
      
      if ((charKey == 'A') || (newPassword.length() == 8)) {
        password = newPassword;
        changed = true;
      }
      
    }
    
  } while(!changed);
  
}

void Init(String& password) {
  char key = ' ';
  bool True = true;
  
  displayLCD.setCursor(0,0);
  displayLCD.print("Trocar senha:");
  displayLCD.setCursor(0,1);
  displayLCD.print("[A]-SIM [B]-NAO");

  do {
    key = teclado.getKey();

    if (key != NO_KEY) {
      if (key == 'A') {
        changePassword(password);
        True = false;
      } else if (key == 'B') {
        True = false;
      }
    }
    
  } while (True);
  
}

void loopContentDisplay(float temp) {
  cleanDisplay();
  displayLCD.setCursor(0, 0);
  displayLCD.write("CofreIno");
  displayLCD.setCursor(11, 0);
  displayLCD.print(temp);
  displayLCD.setCursor(0, 1);
  displayLCD.print("(D)-Op(C)-C(#)V");
}

char getKeyValue(String show, int &posDisplay) {
  bool changed = false;

  char charKey = ' ';
  
  cleanDisplay();
  
  displayLCD.setCursor(0, 0);
  displayLCD.print("Digite a senha:");
  displayLCD.setCursor(0, 1);

  do {
    charKey = teclado.getKey();

    if (charKey != NO_KEY) {
      switch(charKey) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case 'A':
          displayLCD.setCursor(posDisplay, 1);
          displayLCD.print(show);
          posDisplay++;
          return charKey;
        default:
          break;
      }
      
    }
    
  } while(!changed);

}

bool validateKey (char key) {
  switch (key) {
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case '*':
    case '#':
      return false;
    default:
      return true;
  }
}

void indicateCofreOpen(bool &b, int vel) {
  cleanDisplay();
  pinMode(13, OUTPUT);

  displayLCD.setCursor(0, 0);
  displayLCD.write("Cofre Abrindo");

  for (int i = 1; i <= 90; i += vel) {
    digitalWrite(13, HIGH);
    delay(200);
    mtServo.write(i);
    digitalWrite(13, LOW);
    delay(200);
  }

  cleanDisplay();
  displayLCD.setCursor(0, 0);
  displayLCD.write("Cofre Aberto");

  b = true;

  delay(2000);

}

void openCofre (const String &senha, bool &b, int vel) {
    
    String senhaTentativa = "";
    String showDisplay = "";

    char key = ' ';

    bool tried = true;

    int posDisplay = 0;

    do {
      key = getKeyValue(showDisplay, posDisplay);
      if (validateKey(key)) {
        showDisplay += "*";
        senhaTentativa += key;
      }
      else if (key == 'A') {
        if (senhaTentativa.compareTo(senha) == 0) {
           tried = false;
           indicateCofreOpen(b, vel);
        }
        else {
          cleanDisplay();
          displayLCD.setCursor(0, 0);
          displayLCD.write("Senha Errada!");
          tried = false;
        }
      }
    } while (tried);
}

void closeCofre(int vel) {
  cleanDisplay();
  pinMode(13, OUTPUT);

  displayLCD.setCursor(0, 0);
  displayLCD.write("Cofre Fechando");

  for (int i = 90; i > -1; i -= vel) {
    digitalWrite(13, HIGH);
    delay(200);
    mtServo.write(i);
    digitalWrite(13, LOW);
    delay(200);
  }

  cleanDisplay();
  displayLCD.setCursor(0, 0);
  displayLCD.write("Cofre Fechado");

}

int configVel() {

  int velocidade = 0;
  int time = 0;
  char tecla = ' ';

  do {
    tecla = teclado.getKey();
    velocidade = map(analogRead(A4), 0, 1023, 1, 3);

    if (tecla != NO_KEY) {
      if (tecla == '*') {
        break;
      }
    }

    if ((millis() - time) > 300) {
      cleanDisplay();
      
      displayLCD.setCursor(0, 0);
      displayLCD.write("Vel: ");
      displayLCD.setCursor(5, 0);
      displayLCD.print(velocidade);

      time = millis();
    }

  } while (true);

  return velocidade;
}

// ========================================================================================================================

// ========================================================================================================================
// Criação de variáveis

String senha = "0000";
String senhaTentativa = "";

char keyPressed = ' ';

bool opened = false;
bool action = false;

float temperatura = 0.0f;

unsigned long timeMillis = 0;

int velMotor = 1;

// ========================================================================================================================

// ========================================================================================================================
// Setup do projeto
void setup() {

  pinMode(A5, INPUT); // pino sensor de temperatura
  pinMode(A4, INPUT); // pino potenciometro
  
  Serial.begin(9600); // iniciando o monitor Serial para testes do projeto
  displayLCD.begin(16, 2); // iniciando o display
  mtServo.attach(10); // setando o servo 

  mtServo.write(0);

  Init(senha);
  cleanDisplay();
  
}

// ========================================================================================================================

// ========================================================================================================================
// Loop do projeto
void loop() {
  action = false;

  do {
    
    if ((millis() - timeMillis) > 1000) {
      loopContentDisplay(temperatura);
      temperatura = ((5.0f * analogRead(A5) / 1023.0f) - 0.5f) * 100;
      timeMillis = millis();
    }

    keyPressed = teclado.getKey();

    if (keyPressed != NO_KEY) {
      if (keyPressed == 'D') {
        if (!opened) {
          openCofre(senha, opened, velMotor);
          delay(400);
          action = true;
        }
      } 

      else if (keyPressed == 'C') {
        if (opened) {
          opened = false;
          closeCofre(velMotor);
          action = true;
          delay(400);
        }
      }

      else if (keyPressed == '#') {
        velMotor = configVel();
      }

    }
    
  } while(!action);
  
}

// ========================================================================================================================


/**************************************************************************************************************************

REFERENCIA

https://arduino.cc/referencies
https://playground.arduino.cc/Code/Keypad/
https://www.arduino.cc/reference/pt/language/variables/data-types/string/functions/compareto/
https://www.youtube.com/watch?v=OXKK42TMR8E
https://www.youtube.com/watch?v=VcpHP2u7X-8

***************************************************************************************************************************/
