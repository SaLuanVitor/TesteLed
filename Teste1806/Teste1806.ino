
// Bibliotecas

#include <WiFi.h>
#include <WebServer.h>

// WiFi
const char *WIFISSID = "Rabanete";
const char *senha = "thorzuL1";

// Mapa do Hardware

#define LED 27
#define Touch 4
String estadoLED = "Desacionado";
int mediaTouch;

// Definições de rede

IPAddress local_IP(192, 168, 1, 50);  // Defina o IP de acesso
IPAddress gateway(192, 168, 1, 1);    // Defina o IP do roteador de internet
IPAddress subnet(255, 255, 255, 0);   // Defina a máscara de sub-rede
IPAddress primaryDNS(192, 168, 1, 1); // opcional - DNS primário
IPAddress secondaryDNS(8, 8, 8, 8);   // opcional - DNS secundário

WebServer server(80);

void setup()
{
  Serial.begin(115200);
  delay(400);

  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS))
  {
    Serial.println("STA Failed to configure");
  }

  Serial.println("Conectando a rede");
  Serial.println(WIFISSID);
  WiFi.begin(WIFISSID, senha);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Sucesso ao conectar-se a rede WiFi");
  Serial.print("endereço de IP para o web server: ");
  Serial.println(WiFi.localIP());
  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("HTTP servidor está funcionando");
  pinMode(LED, OUTPUT);
}

void loop()
{
  server.handleClient();

  for (int i = 0; i < 100; i++)
  {
    mediaTouch += touchRead(Touch);
  }
  mediaTouch = mediaTouch / 100;

  if (mediaTouch < 40)
  {
    digitalWrite(LED, HIGH);
    estadoLED = "Acionado";
    Serial.println("Botão Acionado");
  }
  else
  {
    digitalWrite(LED, LOW);
    estadoLED = "Desacionado";
    Serial.println("Desacionado");
  }
}

void handle_OnConnect()
{
  server.send(200, "text/html", SendHTML(estadoLED));
}

void handle_NotFound()
{

  server.send(404, "text/plain", "Pagina não existente");
}
String SendHTML(String estadoLED)
{
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta http-equiv=\"refresh\" content=\"0.5\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<link href=\"https://fonts.googleapis.com/css?family=Open+Sans:300,400,600\" rel=\"stylesheet\">\n";
  ptr += "<title>MEDIDOR NÍVEL E AUTOMATIZADOR DO RESERVATÓRIO</title>\n";
  ptr += "<style>html { font-family: 'Open Sans', sans-serif; display: block; margin: 0px auto; text-align: center;color: #333333;}\n";
  ptr += "body{margin-top: 50px;}\n";
  ptr += "h1 {margin: 50px auto 30px;}\n";
  ptr += ".side-by-side{display: inline-block;vertical-align: middle;position: relative;}\n";
  ptr += ".humidity-icon{background-color: #3498db;width: 30px;height: 30px;border-radius: 50%;line-height: 36px;}\n";
  ptr += ".humidity-text{font-weight: 600;padding-left: 15px;font-size: 19px;width: 160px;text-align: left;}\n";
  ptr += ".humidity{font-weight: 300;font-size: 60px;color: #3498db;}\n";
  ptr += ".superscript{font-size: 17px;font-weight: 600;position: absolute;right: -20px;top: 15px;}\n";
  ptr += ".data{padding: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<div id=\"webpage\">\n";
  ptr += "<h1>ESTADO DO LED</h1>\n";
  ptr += "</div>\n";
  ptr += "<div class=\"data\">\n";
  ptr += "<div class=\"side-by-side humidity-icon\">\n";
  ptr += "<svg version=\"1.1\" id=\"Layer_2\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" x=\"0px\" y=\"0px\"\n\"; width=\"12px\" height=\"17.955px\" viewBox=\"0 0 13 17.955\" enable-background=\"new 0 0 13 17.955\" xml:space=\"preserve\">\n";
  ptr += "<path fill=\"#FFFFFF\" d=\"M1.819,6.217C3.139,4.064,6.5,0,6.5,0s3.363,4.064,4.681,6.217c1.793,2.926,2.133,5.05,1.571,7.057\n";
  ptr += "c-0.438,1.574-2.264,4.681-6.252,4.681c-3.988,0-5.813-3.107-6.252-4.681C-0.313,11.267,0.026,9.143,1.819,6.217\"></path>\n";
  ptr += "</svg>\n";
  ptr += "</div>\n";
  ptr += "<div class=\"side-by-side humidity-text\">Estado do LED</div>\n";
  ptr += "<div class=\"side-by-side humidity\">";
  ptr += (String)estadoLED;
  ptr += "<span class=\"superscript\">%</span></div>\n";
  ptr += "</div>\n";
  ptr += "</div>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}
