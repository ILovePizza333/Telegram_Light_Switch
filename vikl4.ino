#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FastBot.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>


const char* ssid = "UPVEL";   const char* password = "12345678";//логин и пароль роутера


//101- подвал дата
//102- вход
//103- студия !!! кнопка подключена к D5
//104- лаб
#define DEVICE 103
String client_ip = "192.168.10.";//последние 3 цифры добавляются в setup-е на основе номера устройства 
IPAddress ip (192, 168, 10, DEVICE);//локальный ip //не забывай про обновление
IPAddress gateway (192, 168, 10, 1);//адрес роутера в сети
IPAddress subnet (255, 255, 255, 0);//макска подсети
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional


#if(DEVICE == 102)
const char* OTAPSWD = "Vhod";//имя устройства для ОТА и пароль
#elif(DEVICE == 103)
const char* OTAPSWD = "Stud";//имя устройства для ОТА и пароль
#elif(DEVICE == 104)
const char* OTAPSWD = "Lab";//имя устройства для ОТА и пароль
#else
const char* OTAPSWD = "???";//имя устройства для ОТА и пароль
#endif

ESP8266WebServer server(80);

String webPage = "";
String SVET = ""; // background: #FFD700;

const char BotToken[] = "905399471:AAFXwqGGWDI4GcXkaU2eVMmVytU1wf9dxwU";// инициализация Telegram бота
FastBot bot(BotToken);

bool sost = 0;
bool prev_sost = 0;

int but_prev_time = 0;
int sost_prev_time = 0;
int cur_time = 0;
#define btn D8
#define out D7

String sendmsg="Свет ";
String sendmsg2=" ";
void setup() {
  client_ip += DEVICE;
  if(DEVICE == 102){
    sendmsg+="на входе ";
  }else if(DEVICE == 103){
    sendmsg+="в студии ";
  }else if(DEVICE == 104){
    sendmsg+="в лаборатории ";
  }else {
    sendmsg+="??? ";
  }
  pinMode(btn, INPUT);
  pinMode(out, OUTPUT);
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.config(ip, gateway, subnet, primaryDNS, secondaryDNS);
  WiFi.begin(ssid, password);
  Serial.println(" "); Serial.print("Connecting Wifi: ");  Serial.println(ssid);

  // ждем соединения:
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected, IP address: ");  Serial.println(WiFi.localIP());

  bot.attach(newMsg);

  ArduinoOTA.setPort(8266);
  ArduinoOTA.setHostname(OTAPSWD);
  ArduinoOTA.setPassword(OTAPSWD);
  ArduinoOTA.begin();
}

void loop() {

  while (digitalRead(btn) == LOW) {
    cur_time = millis();
    //Serial.print(cur_time - sost_prev_time); Serial.print("       "); Serial.println(cur_time - but_prev_time);

    // Serial.print(" btn:");  Serial.print(digitalRead( btn));
    // Serial.print("sost:");  Serial.println(sost);

  }
  if (cur_time - sost_prev_time >= 500 &&  cur_time - but_prev_time >= 100) {
    prev_sost = sost;
    sost = !sost;
    sost_prev_time = millis();
  }

  if (prev_sost != sost) {
    digitalWrite(out, sost);
    if (sost == 0) {
      sendmsg2="выключен с кнопки";
    } else {
      sendmsg2="включен с кнопки";
    }
    bot.sendMessage(sendmsg+sendmsg2, "122190844");
    prev_sost = sost;
  }
  but_prev_time = millis();


  bot.tick();
  server.handleClient();
  ArduinoOTA.handle();
}
