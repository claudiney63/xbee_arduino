#include <XBee.h>

XBee xbee = XBee();
ZBRxResponse rx = ZBRxResponse();

void setup() {
  Serial.begin(9600);
  xbee.setSerial(Serial);
}

void loop() {

  // Envio de uma mensagem
  char message[] = "Ola, Coord";
  XBeeAddress64 destAddress = XBeeAddress64(0x0013A200, 0x417373F4);  // Endereço do XBee de destino
  ZBTxRequest zbTx = ZBTxRequest(destAddress, message, strlen(message));
  xbee.send(zbTx);
  Serial.println("Mensagem enviada para o XBee!\n");

  // Lê o pacote recebido
  xbee.readPacket();
  if (xbee.getResponse().isAvailable()) {
    // Verifica se é uma resposta de recebimento de dados ZigBee
    if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
      // Processa a mensagem recebida
      xbee.getResponse().getZBRxResponse(rx);
      XBeeAddress64 senderAddress = rx.getRemoteAddress64();
      uint8_t payloadLength = rx.getDataLength();
      uint8_t payload[payloadLength];
      rx.getData(payload);
      Serial.print("\nMensagem recebida do XBee: ");
      for (int i = 0; i < payloadLength; i++) {
        Serial.print((char)payload[i]);
      }
      Serial.println();

      // Envio de uma resposta
      char responseMessage[] = "Recebido!";
      ZBTxRequest zbTx = ZBTxRequest(senderAddress, (uint8_t *)responseMessage, strlen(responseMessage));
      xbee.send(zbTx);
    }
  }

  delay(100);  // Atraso para evitar leituras rápidas na porta serial
}
