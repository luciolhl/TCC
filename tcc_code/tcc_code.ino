#include <SPI.h> //Inclusão Biblioteca
#include <MFRC522.h> //Biblioteca RFID
#include <U8glib.h>  //Biblioteca Display LCD
#include <Keypad.h>  // Biblioteca teclado

#define SS_PIN 23  //Pino SDA RFID
#define RST_PIN 22 //Pino de RESET RFID

U8GLIB_ST7920_128X64_1X display(8, 7, 6 , 9); //Instanciando Display LCD passando Pinos como parametro

MFRC522 rfid(SS_PIN, RST_PIN); //Instanciando RFID

const byte LINES = 4; // Linhas do teclado
const byte COLUMNS = 4; // Colunas do teclado

const char MATRIX_KEYS[LINES][COLUMNS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

const byte PINS_LINES[LINES] = {33, 35, 37, 39};     // Pinos de conexao com as linhas do teclado
const byte PINS_COLUMNS[COLUMNS] = {32, 34, 36, 38}; // Pinos de conexao com as colunas do teclado

Keypad customKeyboard = Keypad(makeKeymap(MATRIX_KEYS), PINS_LINES, PINS_COLUMNS, LINES, COLUMNS); //Instancia o teclado

void Display_config() {
  display.setFont(u8g_font_6x10);
  display.setFontRefHeightExtendedText();
  display.setDefaultForegroundColor();
  display.setFontPosTop();
}

void Tela1() {
  display.setFont(u8g_font_unifont);
  display.drawStr(45, 15, "Vida");
  display.drawStr(53, 40, "de");
  display.drawStr(35, 60, "Silicio");
}
void Tela2() {
  display.setFont(u8g_font_unifont);
  display.drawBox(0, 0, 128, 64);
  display.setColorIndex(0);
  display.drawStr(4, 35, "Vida de Silicio");
  display.drawStr(5, 35, "Vida de Silicio");
  display.drawFrame(1, 1, 126, 62);
}
void Tela3() {
  display.setFont(u8g_font_helvB08);
  display.drawStr(45, 31, "Arduino");
  display.drawStr90(105, 15, "Display");
  display.drawStr270(20, 55, "Eletronica");
}
void Tela4() {
  display.setFont(u8g_font_robot_de_niro);
  display.drawStr(0, 13, "Vida de Silicio");
  display.setFont(u8g_font_helvB08);
  display.drawStr(0, 25, "Display Grafico");
  display.setFont(u8g_font_8x13);
  display.drawBox(0, 31, 96, 13);
  display.setColorIndex(0);
  display.drawStr(0, 41, "Arduino Mega");
  display.setFont(u8g_font_ncenB10);
  display.setColorIndex(1);
  display.drawStr(0, 60, "0123456789");
}
void Tela5() {
  char ASCII[2] = " ";
  int dec = 32;
  display.setFont(u8g_font_robot_de_niro);
  for (int linha = 10; linha < 70; linha += 10) {
    for (int coluna = 2; coluna < 128; coluna += 8) {
      ASCII[0] = dec;
      display.drawStr(coluna, linha, ASCII);
      dec ++;
    }
  }
  
}
void Tela6() {
  display.setFont(u8g_font_8x13);
  display.drawLine(10, 2, 110, 2);
  display.drawBox(20, 10, 15, 15);
  display.drawCircle(85, 20, 15);
  display.drawEllipse(30, 50, 10, 10);
  display.drawDisc(90, 50, 10);
}

void setup() {
  Serial.begin(9600); //INICIALIZA A SERIAL
  SPI.begin();        //INICIALIZA O BARRAMENTO SPI
  rfid.PCD_Init();    //INICIALIZA MFRC522
  Serial.println("RFID reading UID");

  if ( display.getMode() == U8G_MODE_R3G3B2 )
    display.setColorIndex(20);
  else if ( display.getMode() == U8G_MODE_GRAY2BIT )
    display.setColorIndex(1);
  else if ( display.getMode() == U8G_MODE_BW )
    display.setColorIndex(1);
}

void loop() {
  char readKeys = customKeyboard.getKey(); //Atribui a variavel a leitura do teclado

  if (readKeys) { //Se alguma tecla foi pressionada
    Serial.println(readKeys); //Imprime a tecla pressionada na porta serial
  }
  //ReadRFID();
}

void ReadRFID(){
  //Tela 1
  display.firstPage();
  do {
    Display_config();
    Tela1();
  }
  while (display.nextPage());
  delay(1000);
  //Tela 2
  display.firstPage();
  do {
    Display_config();
    Tela2();
  }
  while (display.nextPage());
  delay(1000);
  //Tela 3
  display.firstPage();
  do {
    Display_config();
    Tela3();
  }
  while (display.nextPage());
  delay(1000);
  //Tela 4
  display.firstPage();
  do {
    Display_config();
    Tela4();
  }
  while (display.nextPage());
  delay(1000);
  //Tela 5
  display.firstPage();
  do {
    Display_config();
    Tela5();
  }
  while (display.nextPage());
  delay(1000);
  //Tela 6
  display.firstPage();
  do {
    Display_config();
    Tela6();
  }
  while (display.nextPage());
  delay(1000);

  if(rfid.PICC_IsNewCardPresent()){
    if(rfid.PICC_ReadCardSerial()){
      Serial.print("Tag UID:");
      for(byte i=0; i<rfid.uid.size; i++){
        Serial.print(rfid.uid.uidByte[i] < 0x10 ? "0" : " ");
        Serial.print(rfid.uid.uidByte[i], HEX);
      }

      Serial.println();
      rfid.PICC_HaltA();
    }
  }
}