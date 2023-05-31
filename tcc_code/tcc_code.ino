#include <SPI.h>      //Inclusão Biblioteca
#include <MFRC522.h>  //Biblioteca RFID
#include <U8glib.h>   //Biblioteca Display LCD
#include <Keypad.h>   // Biblioteca teclado
#include <Ethernet.h> // Biblioteca internet

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

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //Endereço MAC para Ethernet Shield

char server[] = "www.google.com"; //Utilizado para verificar se tem internet

EthernetClient client; //Declarado o client para Ethernet Shield

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

  //Iniciando Rede
  DrawScreenRede("Iniciando Rede");

  if (Ethernet.begin(mac) == 0) {
    Serial.println("Falha ao configurar Ethernet usando DHCP");
    //Verifica se o modulo de Ethernet esta conectado
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("O modulo Ethernet Shild não foi encontrada.");
      DrawScreenRede("Placa não conectada");
      while (true) {
        delay(1); //Não faz nada, e a aplicação tem que ser reiniciada conectando o modulo Ethernet
      }
    }
    /*if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Cabo de rede desconectado.");
      //ADD MENSAGEM NA TELA "Cabo de rede desconectado"
    }*/
    //Tentando configurar utilizando o endereço de IP ao invés do DHCP:
    //Ethernet.begin(mac, ip, myDns);
  } else {
    Serial.print("IP atribuido pelo DHCP");
    DrawScreenRede("IP atribuido pelo DHCP ");
    Serial.println(Ethernet.localIP());

    char IP[30];
    String IPs = (String)Ethernet.localIP();
    IPs.toCharArray(IP, 30);
    DrawScreenRede(IP);
  }
  //Tempo de esperar para que a conexão seja realizada
  delay(1000);
  Serial.print("Realizando conexão...");
  DrawScreenRede("Realizando conexão...");
  Serial.print(server);
  Serial.println("...");

  //Testando a conexão
  if (client.connect(server, 80)) {
    Serial.print("Conectando em:");
    Serial.println(client.remoteIP());
    
    client.println("GET /search?q=arduino HTTP/1.1");
    client.println("Host: www.google.com");
    client.println("Connection: close");
    client.println();
    Serial.println("Conexão Sucesso");
    DrawScreenRede("Conexão Sucesso");
  } else {
    //Se não conseguir realizar a comunicação de teste com o google
    Serial.println("Falha na conexão");
    DrawScreenRede("Falha na conexão");
    delay(2000);
  }

  DrawScreenRede("Rede Iniciado");

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

void ScreenRede(String msg)  //Tela 7
{
  display.setFont(u8g_font_unifont);  
  display.setPrintPos(13, 35); 
  display.print(msg);
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

void DrawScreenRede(String msg){
  display.firstPage();
  do{
    Display_config();
    ScreenRede(msg);
  }
  while(display.nextPage());
  delay(2000);
}