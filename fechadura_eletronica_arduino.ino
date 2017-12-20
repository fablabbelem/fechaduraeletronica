#include <SPI.h> // Biblioteca responsável 
#include <MFRC522.h>
#include <Password.h> // Biblioteca utilizada para controle de senha.
#include <Keypad.h> // Biblioteca para controle do teclado de matrizes.

      // Senha utilizada para acionamento do rele.
      Password felipe = Password( "1234" );
      Password mayko = Password( "4321" ); 
      Password tiago = Password( "1707" ); 
  
#define SS_PIN 53
#define RST_PIN 5
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

      const byte ROWS = 4;  // Quatro linhas por...
      const byte COLS = 4;  // Quatro colunas.

      // Define o Keypad (mapa do circuito do teclado).
      char keys[ROWS][COLS] = {
      {'1','2','3','A'},
      {'4','5','6','B'},
      {'7','8','9','C'},
      {'*','0','#','D'}
      };

      // Conecta o teclado matricial em linha 0, linha 1, linha 2, linha 3 e linha 4... 
      //dos pinos do arduino.
      byte rowPins[ROWS] = {30,31,32,33};

      // Conecta o teclado matricial em coluna 0, coluna 1, coluna 2, coluna 3 e...  
      //coluna 4 do arduino.
      byte colPins[COLS] = {34,35,36,37};

      // Cria um objeto Keypad.
      Keypad keypad=Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
      
      int rele = 8;
      int buzzer = 2;

void setup() {
Serial.begin(9600);   // Inicia a serial
  SPI.begin();      // Inicia  SPI bus
  mfrc522.PCD_Init();   // Inicia MFRC522
  
  keypad.addEventListener(keypadEvent);
  keypad.setDebounceTime(250);
  
  Serial.println("Porta Travada!");
  Serial.println();
  
  pinMode(rele, OUTPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, HIGH);
  
  

}

void loop() {

  keypad.getKey();
  
  // Procura por cartao RFID
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Seleciona o cartao RFID
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Mostra UID na serial
  Serial.print("UID da tag :");
  String conteudo= "";
  byte letra;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
     
  }
  Serial.println();
  Serial.print("Mensagem : ");
  conteudo.toUpperCase();

  //Verifica qual tag RFID foi lida
  if (conteudo.substring(1) == "A3 79 59 02" //Teste 
  || conteudo.substring(1) == "DE B8 27 00" // Teste
  || conteudo.substring(1) == "BA 6E 3C D9" // Felipe Casseb
  
  ) 
  {
    //Caso seja verdadeiro...
  Serial.println("Acesso Liberado!");
  //rotina do buzzer
  digitalWrite(buzzer, LOW);
  delay(50);
  digitalWrite(buzzer, HIGH);
  delay(50);
  digitalWrite(buzzer, LOW);
  delay(50);
  digitalWrite(buzzer, HIGH);
  //aciona o rele da fechadura
  digitalWrite(rele, HIGH);
  delay(2000);//por 2 segundos
  digitalWrite(rele, LOW);//depois fecha

}
//senão...
else{
  //Só emeite a mensagem e nao faz nada, mantendo a porta fechada
    Serial.println("Acesso Negado!");
  delay(2000);
}
}

//Leitura da senha no teclado

void keypadEvent(KeypadEvent eKey){
  switch (keypad.getState()){  // Condição switch...
    case PRESSED:  // Se precionado algum botão...
   digitalWrite(buzzer, LOW);
   delay(50);
   digitalWrite(buzzer, HIGH);
   Serial.print("Digitado: ");  // Aparecera no Serial Monitor, Digitado: "X".
   Serial.println(eKey);  // Armazena numero digitado na variável eKey.
   
   
   
   switch (eKey){
   // Pressionando "*" confirma se a senha foi digitado corretamente.
     case '*': guessPassword();  
     break; 
      default:
         felipe.append(eKey);
         mayko.append(eKey);   
         tiago.append(eKey);      
        
  }
}
}

void guessPassword(){
// Informa no Serial Monitor "Verificando, aguarde...".
     Serial.println("Verificando, aguarde... ");  
     if (felipe.evaluate() ||mayko.evaluate()||tiago.evaluate()){ 
        
        
        digitalWrite(rele, HIGH);   // Ativa o rele.
        delay(2000);  // Rele fica acionado por 2 segundos e depois...
        digitalWrite(rele, LOW);   // Desativa o rele.
        // Informa no Serial Monitor "Acionando rele...".
        Serial.println("Porta Aberta! "); 

        resetSenha();
             }
            
             else{
                  digitalWrite(rele, LOW);
             Serial.println("Senha Invalida !");
                  resetSenha();

                  for(int i = 0; i<=3; i++){
                   digitalWrite(buzzer, HIGH);
                   delay(50);
                   digitalWrite(buzzer, LOW);
                   delay(50); 
                  }
                  digitalWrite(buzzer, HIGH);
                   
     }
}
void resetSenha(){
  //
                   felipe.reset(); // Apaga a senha.
                   mayko.reset(); // Apaga a senha.
                   tiago.reset(); // Apaga a senha.
  
}

