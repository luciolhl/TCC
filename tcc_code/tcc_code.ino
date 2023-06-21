#include <SPI.h>
#include <MFRC522.h>
#include <Ethernet.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
 
#define SS_PIN 8
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN); //CRIA INSTANCIA DO RFID.

byte mac_addr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //DEFINE ENDEREÇO MAC DO ETHERNET

IPAddress server_addr(85, 10, 205, 173); //IP SERVIDOR BANCO DE DADOS
char user[] = "usertcc"; //USUÁRIO BANCO DE DADOS
char password[] = "trabalhotcc"; //SENHA BANCO DE DADOS
char dbName[] = "hospixoval"; //NOME BANCO DE DADOS

EthernetClient client; //DECLARADO O CLIENT PARA ETHERNET

MySQL_Connection conn((Client *)&client); //OBJETO CONEXÃO COM BANCO
 
void setup() 
{
  Serial.begin(9600); //INICIA SERIAL
  SPI.begin(); //Inicia  SPI bus
  mfrc522.PCD_Init(); //INICIA MFRC522

  while (!Serial); //AGUAR INICIAR A PORTA SERIAL
  Ethernet.begin(mac_addr); //INICIA MODULO ETHERNET
  Serial.println(Ethernet.localIP()); //VERIFICA O IP RECEBIDO

  Serial.println(F("\nConectando SQL..."));
  ProcessInitSQL(); //INICIAR CONEXÃO COM BANCO
  
  Serial.println(F("\nAproxime o seu cartao do leitor...\n"));
}
 
void loop() 
{
  ReadRFID();
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
    Serial.println(F("Nenhum registro retornado.\n"));
    insert = false;
  }else{
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

  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn); //CRIA UM PONTEIRO PARA EXECUÇÃO DAS QUERY
  
  tag.toCharArray(tagChar, 10);

  sprintf(query, INSERT_SQL, tagChar, 2, 1); //SUBSTITUI AS VARIAVEIS DO SQL       
  cur_mem->execute(query);//EXECUTA A QUERY NO BANCO
  
  if(cur_mem->get_rows_affected() > 0){
    Serial.print(F("Tag: "));
    Serial.print(tag);
    Serial.println(F(" inserida com sucesso."));
  }else{
    Serial.print(F("Falha ao inserir Tag: "));
    Serial.println(tag);
  }
  delete cur_mem;
}

void ProcessInitSQL(){
  while(!conn.connect(server_addr, 3306, user, password, dbName)){
    Serial.println(F("Falha na conexão com DB."));
    Serial.println(F("Tentando novamente..."));
    conn.close();
  };
}