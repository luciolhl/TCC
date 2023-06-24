#include <SPI.h>
#include <MFRC522.h>
#include <Ethernet.h>
#include <Keypad.h>   // Biblioteca teclado
#include <U8glib.h>   //Biblioteca Display LCD
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
 
#define SS_PIN 53
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN); //CRIA INSTANCIA DO RFID.

const byte LINES = 4; // Linhas do teclado
const byte COLUMNS = 4; // Colunas do teclado

const byte PINS_LINES[LINES] = {33, 35, 37, 39};     // Pinos de conexao com as linhas do teclado
const byte PINS_COLUMNS[COLUMNS] = {32, 34, 36, 38}; // Pinos de conexao com as colunas do teclado

const char MATRIX_KEYS[LINES][COLUMNS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

Keypad customKeyboard = Keypad(makeKeymap(MATRIX_KEYS), PINS_LINES, PINS_COLUMNS, LINES, COLUMNS); //Instancia o teclado

U8GLIB_ST7920_128X64_1X display(6, 5, 4 , 7); //Instanciando Display LCD passando Pinos como parametro

byte mac_addr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //DEFINE ENDEREÇO MAC DO ETHERNET

IPAddress server_addr(34, 196, 79, 118); //IP SERVIDOR BANCO DE DADOS
char user[] = "usertcc"; //USUÁRIO BANCO DE DADOS
char password[] = "trabalhotcc"; //SENHA BANCO DE DADOS
char dbName[] = "hospixoval"; //NOME BANCO DE DADOS


EthernetClient client; //DECLARADO O CLIENT PARA ETHERNET

MySQL_Connection conn((Client *)&client); //OBJETO CONEXÃO COM BANCO

void Display_config() {
  display.setFont(u8g_font_6x10);
  display.setFontRefHeightExtendedText();
  display.setDefaultForegroundColor();
  display.setFontPosTop();
}
 
void setup() 
{
  DrawScreenMsg("Serial iniciando...", "");
  Serial.begin(9600); //INICIA SERIAL
  while (!Serial); //AGUAR INICIAR A PORTA SERIAL
  DrawScreenMsg("Serial iniciado", "");

  DrawScreenMsg("SPI iniciando...", "");
  SPI.begin(); //Inicia  SPI bus
  DrawScreenMsg("SPI iniciado", "");

  DrawScreenMsg("RFID iniciando...", "");
  mfrc522.PCD_Init(); //INICIA MFRC522
  DrawScreenMsg("RFID iniciado", "");

  DrawScreenMsg("Rede iniciando...", "");
  Ethernet.begin(mac_addr); //INICIA MODULO ETHERNET
  DrawScreenMsg("Rede iniciado", IpAdressString(Ethernet.localIP()));
  Serial.println(Ethernet.localIP()); //VERIFICA O IP RECEBIDO

  DrawScreenMsg("SQL iniciando...", "");
  Serial.println(F("\nConectando SQL..."));
  ProcessInitSQL(); //INICIAR CONEXÃO COM BANCO
  DrawScreenMsg("SQL Iniciado", "");
  
  Serial.println(F("\nAproxime o seu cartao do leitor...\n"));

  DrawMessageInitial();
}
 
void loop() 
{
  DrawOkProccess();
  //ReadKeyboard();
  ReadRFID();
}

char ReadKeyboard(){
  char readKeys = customKeyboard.getKey(); //Atribui a variavel a leitura do teclado

  if (readKeys) { //Se alguma tecla foi pressionada
    Serial.println(readKeys); //Imprime a tecla pressionada na porta serial
    return readKeys;
  }
}

void ReadRFID(){
  //VERIFICA SE TEM NOVOS MAIS PARA SEREM LIDOS
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  //VERIFICA SELAÇÃO DE TAG
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }

  Serial.print("UID da tag :");
  String conteudo= "";

  //DECODIFICA O UID DA TAG E ARMAZENA NA VARIAVEL conteudo E PRINTA NA SERIAL
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : ""));
     conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  conteudo.toUpperCase(); //CONVERTE O CONTEUDO LIDO PARA MAISCULO
  PrintUUIDProcess(conteudo);
  CallDB(conteudo); //CHAMA A FUNÇÃO DE INSERIR NO BANCO O REGISTRO
}

void CallDB(String tag){
  Serial.print(F("\nTag recebida: "));
  Serial.println(tag);

  if(RecordExists(tag) == false){
    InsertRecord(tag);
  }
}

bool RecordExists(String tag){
  // QUERY VERIFICAR SE TAG EXISTE NO BANCO DE DADOS
  char CONSULT_SQL[] = "SELECT id, local FROM Tags where id='%s'";
  char UPDATE_SQL[] = "UPDATE Tags set local= %d where id='%s'";
  char query[128] = "";
  char tagChar[10] = "";
  bool insert;
  row_values *row = NULL;
  String head_count;
  int local = 0;

  tag.toCharArray(tagChar, 10);

  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn); //CRIA UM PONTEIRO PARA EXECUÇÃO DAS QUERY

  sprintf(query, CONSULT_SQL, tagChar);
  cur_mem->execute(query);

  column_names *columns = cur_mem->get_columns();

  do {
    row = cur_mem->get_next_row();
    if (row != NULL) {
      head_count = atol(row->values[0]);
      if(row->values[1] != NULL){
        local = atoi(row->values[1]);
      }
    }
  } while (row != NULL);

  Serial.print(F("local: "));
  Serial.println(local);
  if(head_count == ""){
    DrawScreenMsg("Tag: "+ tag, "Nao cadastrada");
    Serial.println(F("Nenhum registro retornado.\n"));
    insert = false;
  }else{
    DrawScreenTagFound("Tag encontrada:", tag, "Local: " + (String)local);
    Serial.println(F("Registro encontrado.\n"));
    if(local == 4){
      local = 1;
    }else{
      local = local+1;
    }
    sprintf(query, UPDATE_SQL, local, tagChar);
    cur_mem->execute(query);
    insert = true;
  }

  delete cur_mem;
  return insert;
}

void InsertRecord(String tag){
  char INSERT_SQL[] = "INSERT INTO Tags(id,local, categoria_id, createdAt, updatedAt,deletedAt)VALUES('%s',%d,%d,NOW(),NOW(),NULL)";//QUERY PARA INSERIR AS INFORMAÇÕES NO BANCO
  char query[128] = "";
  char tagChar[10] = "";
  bool stopWhile = false;
  int category;
  String categoryString;

  DrawScreenRegisterTag("Informe categoria:", "", "C para confirmar");
  Serial.println(F("Seleciona qual categoria é esta TAG. Aperte C para confirmar."));
  Serial.print(F("Categoria selecionada: "));
  while(stopWhile == false){
    char character = customKeyboard.getKey();
    if(character){
      if(character == 'C'){
        DrawScreenRegisterTag("Categoria:", categoryString, "Confirmada");
        stopWhile = true;
      }else{
        //category = ConvertCharToInt(character);
        categoryString.concat(character);
        Serial.println(categoryString);
        DrawScreenRegisterTag("Informe categoria:", categoryString, "C para confirmar");
      }
    }
  }

  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn); //CRIA UM PONTEIRO PARA EXECUÇÃO DAS QUERY
  
  tag.toCharArray(tagChar, 10);

  sprintf(query, INSERT_SQL, tagChar, 2, ConvertStringToInt(categoryString)); //SUBSTITUI AS VARIAVEIS DO SQL       
  cur_mem->execute(query);//EXECUTA A QUERY NO BANCO
  
  if(cur_mem->get_rows_affected() > 0){
    Serial.print(F("Tag: "));
    Serial.print(tag);
    Serial.println(F(" inserida com sucesso."));
    DrawScreenMsg("Tag: "+ tag, "Cadastrada");
  }else{
    Serial.print(F("Falha ao inserir Tag: "));
    Serial.println(tag);
    DrawScreenMsg("Tag: "+ tag, "Falha cadastro");
  }
  delete cur_mem;
}

int ConvertStringToInt(String str) {
  int i, x;
  int tam = str.length() - 1;
  int numero = 0;

  for(i = tam; i >= 0; i--) {
    x = (int) str.charAt(i) - 48;
    numero = x;
  }

  return numero; 
}

void ProcessInitSQL(){
  while(!conn.connect(server_addr, 3306, user, password, dbName)){
    Serial.println(F("Falha na conexão com DB."));
    Serial.println(F("Tentando novamente..."));
    conn.close();
  };
}

String IpAdressString(IPAddress address)
{
 return String(address[0]) + "." + 
        String(address[1]) + "." + 
        String(address[2]) + "." + 
        String(address[3]);
}

void AllMsg(String msg, String msg2)  //Tela 1
{
  display.setFont(u8g_font_fur11);  
  display.setPrintPos(8, 27); 
  display.println(msg);
  display.setPrintPos(8, 45); 
  display.print(msg2);
  display.drawFrame(0,0,128,64);  
  display.drawFrame(2,2,124,60); 
}  

void DrawScreenMsg(String msg, String msg2){
  display.firstPage();
  do{
    Display_config();
    AllMsg(msg, msg2);
  }
  while(display.nextPage());
  delay(2000);
}

void PrintUUIDProcess(String msg)  //Tela 2
{
  display.setFont(u8g_font_fur11);  
  display.setPrintPos(8, 27); 
  display.println("Tag informada:");
  display.setPrintPos(8, 45); 
  display.print(msg);
  display.drawFrame(0,0,128,64);  
  display.drawFrame(2,2,124,60); 
}  

void DrawtUUIDProcess(String msg){
  display.firstPage();
  do{
    Display_config();
    PrintUUIDProcess(msg);
  }
  while(display.nextPage());
  delay(2000);
}

void MessageInitial()  //Tela 3
{
  display.setFont(u8g_font_fur11);  
  display.setPrintPos(8, 27); 
  display.println("Sistema Iniciado");
  display.setPrintPos(28, 42); 
  display.print("Bem Vindo");
  display.drawFrame(0,0,128,64);  
  display.drawFrame(2,2,124,60);   
}  

void DrawMessageInitial(){
  display.firstPage();
  do{
    Display_config();
    MessageInitial();
  }
  while(display.nextPage());
  delay(2000);
}

void OkProccess()  //Tela 4
{
  display.setFont(u8g_font_fur11);  
  display.setPrintPos(8, 27); 
  display.println("Leitor pronto:");
  display.setPrintPos(8, 45); 
  display.print("Aguardando tag");
  display.drawFrame(0,0,128,64);  
  display.drawFrame(2,2,124,60);     
}  

void DrawOkProccess(){
  display.firstPage();
  do{
    Display_config();
    OkProccess();
  }
  while(display.nextPage());
}

void TagFound(String msg, String tag, String local)  //Tela 5
{
  display.setFont(u8g_font_fur11r);  
  display.setPrintPos(8, 15); 
  display.println(msg);
  display.setPrintPos(8, 35); 
  display.print(tag);
  display.setPrintPos(8, 55); 
  display.print(local);
  display.drawFrame(0,0,128,64);  
  display.drawFrame(2,2,124,60); 
}  

void DrawScreenTagFound(String msg, String tag, String local){
  display.firstPage();
  do{
    Display_config();
    TagFound(msg, tag, local);
  }
  while(display.nextPage());
  delay(2000);
}

void RegisterTag(String msg, String msg2, String msg3)  //Tela 1
{
  display.setFont(u8g_font_gdr9);  
  display.setPrintPos(8, 15); 
  display.println(msg);
  display.setPrintPos(8, 35); 
  display.print(msg2);
  display.setPrintPos(8, 55); 
  display.print(msg3);
  display.drawFrame(0,0,128,64);  
  display.drawFrame(2,2,124,60); 
}  

void DrawScreenRegisterTag(String msg, String msg2, String msg3){
  display.firstPage();
  do{
    Display_config();
    RegisterTag(msg, msg2, msg3);
  }
  while(display.nextPage());
  delay(2000);
}