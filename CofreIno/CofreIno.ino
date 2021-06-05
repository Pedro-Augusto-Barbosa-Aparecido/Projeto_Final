#include <Keypad.h>

const byte numLinhas = 4; 
const byte numColunas = 4;

byte pinosColunas[numColunas] = {2,3,4,5};
byte pinosLinhas[numLinhas] = {6,7,8,9};

char teclas[numLinhas][numColunas] = {
                    {'1','2','3','A'},
                    {'4','5','6','B'},
                    {'7','8','9','C'},
                    {'*','0','#','D'}
                };

Keypad teclado = Keypad(makeKeymap(teclas), pinosLinhas, pinosColunas, numLinhas, numColunas);

char teclaApertada;
String senha = "123";
String s;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  teclaApertada = teclado.getKey();
  
  if (teclaApertada != NO_KEY) {
    Serial.println(teclaApertada);
    s = s + teclaApertada;
    Serial.println(s);
  }
  
  if ((senha.compareTo(s)) == 0){
    Serial.println("senha acertada");
  }
  
}

/*

REFERENCIA

arduino.cc/referencies
https://playground.arduino.cc/Code/Keypad/
https://www.arduino.cc/reference/pt/language/variables/data-types/string/functions/compareto/

*/
