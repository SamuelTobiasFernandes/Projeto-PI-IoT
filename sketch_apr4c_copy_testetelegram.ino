#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>

//  Conexao na rede WiFi
const char* ssid = "Iphone Samuel";
const char* password = "12345678";

//  Conexao com o Telegram
String token = "8611669899:AAGKEFMXjXK145PC8R9_mnavMe-2BLRgQs4";
String chat_id = "2072753021";

//  Conexao com a Web
ESP8266WebServer server(80);

//  Ligando o sensor
#define ONE_WIRE_BUS D2

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

//  Variavel
float temperaturaAtual = 0;

//  Controle de envio de temperatura, intervalo de envio da mensagem no telegram
unsigned long ultimoEnvio = 0;
unsigned long intervaloEnvio = 1800000;

//  Visualização da pagina na Web
void handleRoot() {
  String html = "<html><head>";
  html += "<meta http-equiv='refresh' content='2'>";
  html += "<style>";
  html += "body { font-family: Arial; text-align: center; margin-top: 50px; }";
  html += "h1 { font-size: 40px; }";
  html += ".temp { font-size: 60px; color: red; margin-top: 40px; }";
  html += "</style>";
  html += "</head><body>";

  html += "<h1>TEMPERATURA</h1>";
  html += "<br><br>";
  html += "<div class='temp'>" + String(temperaturaAtual) + " &deg;C</div>";

  html += "</body></html>";

  server.send(200, "text/html", html);
}


//  Mensagem no Telegram
void enviarTelegram(float temp) {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClientSecure client;
    client.setInsecure();

    HTTPClient http;

    String url = "https://api.telegram.org/bot" + token +
                 "/sendMessage?chat_id=" + chat_id +
                 "&text=Temperatura%20atual:%20" + String(temp) + "C";

        http.begin(client, url);
        int httpCode = http.GET();

        Serial.print("Telegram HTTP: ");
        Serial.println(httpCode);

        http.end();
  }
}

//  Setup para o NodeMCU
void setup() {
  Serial.begin(115200);

  sensors.begin();

  WiFi.begin(ssid, password);
  Serial.print("Conectando");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConectado");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.begin();
}

//  Codigo em Loop em casos de erros e pra envio da temperatura
void loop() {
  server.handleClient();

//  Ler a temperatura do sensor
  sensors.requestTemperatures();
  float temp = sensors.getTempCByIndex(0);

//  -127 possível erro de leitura nesse tipo de sensor
  if (temp != -127.00) {
    temperaturaAtual = temp;
    } else {
      Serial.println("Erro no sensor");
    }

    //  Envia a informação para o telegram a cada intervalo
    if (millis() - ultimoEnvio > intervaloEnvio) {
      enviarTelegram(temperaturaAtual);
      ultimoEnvio = millis();
    }
  delay(30000);
}