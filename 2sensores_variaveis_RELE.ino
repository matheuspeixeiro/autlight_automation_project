//////////////////////////////////////////////////
////                 AUTLIGHT                 ////
//////////////////////////////////////////////////

#include "config.h"

#define pinSensorFora   D1
#define pinSensorDentro D0
#define RELE_LAMPADA    D2

// constantes Contador
#define ESPERA 0
#define ENTRANDO 1
#define SAINDO 2

// variáveis Contador
int pessoas = 0;
int modo = ESPERA;
int modoAnterior;

/* variáveis Lâmpada
int sentido;
int sentidoAnterior;  */

//  instanciando objetos do MQTT-Broker ()
AdafruitIO_Feed *feedCONTAGEM = io.feed("CONTAGEM");
    //AdafruitIO_Feed *feedSENTIDO  = io.feed("SENTIDO");
AdafruitIO_Feed *feedLAMPADA  = io.feed("LAMPADA");

void setup(){
  pinMode(pinSensorFora, INPUT);
  pinMode(pinSensorDentro, INPUT);
  pinMode(RELE_LAMPADA, OUTPUT);
  
  Serial.begin(9600);
  while(!Serial);
  
  conectaBroker();
}

void loop(){
 
  // processa as mensagens e mantem a conexão ativa
  byte state = io.run();

  // verifica se está conectado
  if(state == AIO_NET_DISCONNECTED | state == AIO_DISCONNECTED){
    conectaBroker(); //função para conectar ao broker
  }

  //chama a função contadorPessoas
  contadorPessoas();
}

void conectaBroker(){

  //conectando à nuvem adafruit io
  Serial.print("Conectando ao Adafruit IO");
  io.connect();
  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println(io.statusText()); // mostrar que foi conectado
  Serial.println("Setup concluído");
  
  // instancia um handler para recepção da mensagem do feed L1 
  feedLAMPADA->onMessage(handleLAMPADA);

  //obtendo valor atual:
  feedCONTAGEM->get();
  feedLAMPADA->get();
    //feedSENTIDO->get();
  
}

void handleLAMPADA(AdafruitIO_Data *data) {
  // Mensagem 
  Serial.print("Recebido  <- ");
  Serial.print(data->feedName());
  Serial.print(" : ");
  Serial.println(data->value());

  //Aciona saída conforme dado recebido
  if(data->isTrue())
    digitalWrite(RELE_LAMPADA, LOW);   //LIGA A LÂMPADA
  else
    digitalWrite(RELE_LAMPADA, HIGH);  //DESLIGA A LÂMPADA
}

void contadorPessoas(){
  modoAnterior = modo;
  
  bool estadoSensorFora   = !digitalRead( pinSensorFora );
  bool estadoSensorDentro = !digitalRead(pinSensorDentro);
  
  if(modo == ESPERA){
    if(estadoSensorFora==1) {
      modo = ENTRANDO;
    }
  }
   if(modo == ESPERA){
    if(estadoSensorDentro==1) {
      modo = SAINDO;
     }
    }
  
  if(modo == ENTRANDO){
    if(estadoSensorDentro==0 && estadoSensorFora==0) {
        pessoas++;
       // sentidoAnterior = sentido;
       // sentido=1;
        modo = ESPERA;
        delay(150);
    } 
  }
  
  if(modo == SAINDO){
    if(estadoSensorDentro==0 && estadoSensorFora==0) {
      if (pessoas > 0)
          pessoas--;
         // sentidoAnterior = sentido;
         // sentido=0;
          modo = ESPERA;
          delay(150);
    } 
  }

  /*if(sentido != sentidoAnterior) //salvando SENTIDO
    feedSENTIDO->save(sentido); */

  if(modo != modoAnterior){      //salvando CONTAGEM
    Serial.println(pessoas);
    
    //publica (->) a variável pessoas no tópico (feed) "CONTAGEM"
    feedCONTAGEM->save(pessoas);
    delay(150);
  }
  
}
