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

void setup() {
  //Iniciando Serial
  DrawScreen(1);
  Serial.begin(9600);//INICIALIZA A SERIAL
  delay(1000);
  DrawScreen(2);
  delay(1000);
  //Fim Serial

  //Iniciando SPI
  DrawScreen(3);
  SPI.begin();//INICIALIZA O BARRAMENTO SPI
  delay(1000);
  DrawScreen(4);
  delay(1000);
  //Fim SPI

  //Inicinando RFID
  DrawScreen(5);
  rfid.PCD_Init();//INICIALIZA MFRC522
  delay(1000);
  DrawScreen(6);
  delay(1000);
  //Fim RFID

  if ( display.getMode() == U8G_MODE_R3G3B2 )
    display.setColorIndex(20);
  else if ( display.getMode() == U8G_MODE_GRAY2BIT )
    display.setColorIndex(1);
  else if ( display.getMode() == U8G_MODE_BW )
    display.setColorIndex(1);
}

void loop() {
  ReadKeyboard();
  ReadRFID();
}

void ReadKeyboard(){
  char readKeys = customKeyboard.getKey(); //Atribui a variavel a leitura do teclado

  if (readKeys) { //Se alguma tecla foi pressionada
    Serial.println(readKeys); //Imprime a tecla pressionada na porta serial
  }
}

void ReadRFID(){
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

void InitSerial()  //Tela 1
{
  display.setFont(u8g_font_unifont);  
  display.drawStr( 1, 35, "Iniciando Serial");  
  display.drawFrame(0,0,128,64);  
  display.drawFrame(2,2,124,60);   
}

void FinishSerial()  //Tela 2
{
  display.setFont(u8g_font_unifont);  
  display.drawStr( 4, 35, "Serial Iniciado");  
  display.drawFrame(0,0,128,64);  
  display.drawFrame(2,2,124,60);   
}  

void InitSPI()  //Tela 3
{
  display.setFont(u8g_font_unifont);  
  display.drawStr( 11, 35, "Iniciando SPI");  
  display.drawFrame(0,0,128,64);  
  display.drawFrame(2,2,124,60);   
}

void FinishSPI()  //Tela 4
{
  display.setFont(u8g_font_unifont);  
  display.drawStr( 16, 35, "SPI Iniciado");  
  display.drawFrame(0,0,128,64);  
  display.drawFrame(2,2,124,60);   
}  

void InitRFID()  //Tela 5
{
  display.setFont(u8g_font_unifont);  
  display.drawStr( 10, 35, "Iniciando RFID");  
  display.drawFrame(0,0,128,64);  
  display.drawFrame(2,2,124,60);   
}

void FinishRFID()  //Tela 6
{
  display.setFont(u8g_font_unifont);  
  display.drawStr( 13, 35, "RFID Iniciado");  
  display.drawFrame(0,0,128,64);  
  display.drawFrame(2,2,124,60);   
}  

void DrawScreen(int page){
  display.firstPage();
    do {
      Display_config();
      switch(page) //Carrega a tela correspondente  
      {
        case 1:  
          InitSerial(); //Tela 1
          break;  
        case 2:  
          FinishSerial(); //Tela 2
          break; 
        case 3:  
          InitSPI(); //Tela 3  
          break;  
        case 4:  
          FinishSPI(); //Tela 4  
          break; 
        case 5:  
          InitRFID(); //Tela 5  
          break;  
        case 6:  
          FinishRFID(); //Tela 6  
          break; 
      }
    }
    while (display.nextPage());
}