#include <SPI.h>
#include <Ethernet2.h> // Knjižnica za W5500

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 177);
EthernetServer server(80);

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Ethernet.begin(mac, ip);
  delay(1000); // kratka pauza

  Serial.print("Server pokrenut na IP adresi: ");
  Serial.println(Ethernet.localIP());

  server.begin();
}

void loop() {
  EthernetClient client = server.available(); // provjera je li se netko spojio

  if (client) {
    Serial.println("Klijent povezan");
    boolean currentLineIsBlank = true;
    String request = "";

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        request += c;
        Serial.write(c);

        if (c == '\n' && currentLineIsBlank) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<h1>Pozdrav s ATmega328 + W5500!</h1>");
          client.println("</html>");
          break;
        }

        if (c == '\n') {
          currentLineIsBlank = true;
        } else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }

    delay(1);
    client.stop();
    Serial.println("Klijent odspojen.");
  }
}
