# Projeto-PI-IoT
O projeto consiste em um sensor de temperatura em tempo real, enviando as informações sobre a temperatura para uma página da Web e também por mensagem via Telegram. 

Para o funcionamento do sistema será necessário os seguintes componentes: NodeMCU ESP8266 12-E, Sensor digital de temperatura Ds18b20, resistor de 4,7kΩ, jumpers e protoboard.
Também será preciso baixar e instalar o programa ARDUINO IDE
Com o circuito montado realize as seguintes alterações no Sketch dentro do ARDUINO IDE

//  Para identificar e ligar a conexão com o sensor, altere o "D2" para a entrada que escolher na controladora NodeMCU
#define ONE_WIRE_BUS D2

//  Conexao com o Telegram
String token = "Token_do_seu_bot_telegram";
String chat_id = "ID_do_chat_que_enviara_as_mensagens";

//  Conexao na rede WiFi
const char* ssid = "SSID_DA_REDE";
const char* password = "SENHA_DA_REDE";

//  Controle de envio de temperatura, intervalo de envio da mensagem no telegram
unsigned long ultimoEnvio = 0;
unsigned long intervaloEnvio = 1800000; // Em milissegundos, esta configurado para 30 minutos

// Na ultima linha do código pode ser alterado o delay da temperatura que irá aparecer na pagina web, também em milissegundos
delay(30000);

No primeiro momento, ira retornar no serial o IP para abrir a pagina Web.
Em caso de retorno no serial, mensagens com -200 do telegram, significa que esta funcionando normalmente.
