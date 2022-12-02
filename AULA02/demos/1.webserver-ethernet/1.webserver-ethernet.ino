#include <SPI.h>
#include <Ethernet.h>

#define LED 2

// Entre com os dados do MAC para o dispositivo.
// Lembre-se que o ip depende de sua rede local
byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
//IPAddress ip(10,10,2,102);
 
// Inicializando a biblioteca Ehternet
// 80 é a porta que será usada. (padrão http)
EthernetServer server(80);

//Varial para mostrar a atualização da pagina
 
void setup() {
 // Abrindo a comunicação serial para monitoramento.
  Serial.begin(115200);

  pinMode(LED, OUTPUT);

  // Inicia a conexão Ethernet e o servidor:
  //Ethernet.begin(mac, ip);
  Ethernet.begin(mac);

  Serial.println("Conectando...");
  server.begin();
  Serial.print("Servidor iniciado em: ");
  Serial.println(Ethernet.localIP());
}
 
void loop() {
  // Aguardando novos clientes;
  EthernetClient client = server.available();
  if (client) {
    Serial.println("Novo Cliente.");
    String currentLine = "";
    
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // Se tiver chegado ao fim da linha (recebeu um novo 
        // Caractere) e a linha estiver em branco, o pedido http terminou,
        // Para que você possa enviar uma resposta
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.print("<a href=\"/ON\">LIGAR</a><br>");
            client.print("<a href=\"/OFF\">DESLIGAR</a><br>");
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }

        if (currentLine.endsWith("GET /ON")) {
          digitalWrite(LED, HIGH);
        }else if (currentLine.endsWith("GET /OFF")) {
          digitalWrite(LED, LOW);
        }
      }
    }

    // Fecha a conexão:
    client.stop();
    Serial.println("Cliente desconectado");
  }
}
