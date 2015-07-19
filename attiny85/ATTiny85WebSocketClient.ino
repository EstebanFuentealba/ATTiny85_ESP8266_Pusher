#include <SoftwareSerial.h>
/*
*   efuentealba@json.cl
*/
//CONFIGURE
#define SSID        "WifiAP"
#define PASS        "password"
#define PUSHER_KEY  "xxxxxxxxxxxxxxx"


#define SOFT_RX_PIN     1
#define SOFT_TX_PIN     0
#define ESP_ENABLE_PIN  2
#define LED_A 4
#define LED_B 3
#define TIMEOUT     5000
#define CONTINUE    false
#define HALT        true

char str_buffer[64];

const char STR_OK[] PROGMEM         = "OK";
const char WS_OK[] PROGMEM          = "HTTP/1.1 101";
const char WS_PING[] PROGMEM        = "pusher:ping";
const char STR_READY[] PROGMEM      = "ready";
const char STR_SENDMODE[] PROGMEM   = ">";
const char STR_APNAME[] PROGMEM     = "+CWJAP:\"";
const char SUBSCRIBE_JSON[] PROGMEM = "{\"event\":\"pusher:subscribe\",\"data\":{\"channel\":\"test_channel\"}}";
const char PONG_JSON[] PROGMEM      = "{\"event\":\"pusher:pong\",\"data\":{}}";
const char ping[] PROGMEM           = "ping";

/*
*   EVENTOS PUSHER.COM
*/
const char ledAOn[] PROGMEM         = "ledAOn";
const char ledAOff[] PROGMEM        = "ledAOff";
const char ledBOn[] PROGMEM         = "ledBOn";
const char ledBOff[] PROGMEM        = "ledBOff";

bool connected = false;


byte ledAonC   = 0;
byte ledAoffC  = 0;
byte ledBonC   = 0;
byte ledBoffC  = 0;
byte pingC     = 0;


SoftwareSerial SoftSerial(SOFT_RX_PIN, SOFT_TX_PIN); // RX, TX

void setup() {
  pinMode(SOFT_TX_PIN, OUTPUT);
  pinMode(LED_A, OUTPUT);
  pinMode(ESP_ENABLE_PIN, OUTPUT);
  pinMode(LED_B, OUTPUT);
  digitalWrite(SOFT_TX_PIN, LOW);
  digitalWrite(LED_A, LOW);
  digitalWrite(LED_B, LOW);
  digitalWrite(ESP_ENABLE_PIN, LOW);
  SoftSerial.begin(4800);
  //SoftSerial.setTimeout(TIMEOUT);
  delay(300);
  digitalWrite(ESP_ENABLE_PIN, HIGH);
  SoftSerial.println(F("AT+RST"));
  SoftSerial.flush();
  if (!waitForString(getString(STR_READY), 5, 2000)) {
    return;
  }
  digitalWrite(LED_A, HIGH);
  delay(500);
  digitalWrite(LED_A, LOW);
  SoftSerial.println(F("ATE0"));
  SoftSerial.flush();
  if (waitForString(getString(STR_OK), 2, 2000)) {
    digitalWrite(LED_A, HIGH);
    delay(500);
    digitalWrite(LED_A, LOW);
    boolean connection_established = false;
    for (int i = 0; i < 5; i++) {
      if (connectWiFi()) {
        connection_established = true;
        break;
      }
    }
    if (!connection_established) {
      return;
    }
    SoftSerial.println(F("AT+CIPMODE=1"));
    SoftSerial.flush();
    if (!waitForString(getString(STR_OK), 2, 1000)) {
      return;
    }
    SoftSerial.println(F("AT+CIPMUX=0"));
    SoftSerial.flush();
    if (!waitForString(getString(STR_OK), 2, 1000)) {
      return;
    }
  } else {
    //digitalWrite(LED_B, HIGH);
  }
}

void wssend(char *data) {
  SoftSerial.print((char)0);
  SoftSerial.print(data);
  SoftSerial.print((char)255);
  SoftSerial.flush();
}
void loop() {
  if (!connected) {
    SoftSerial.println("AT+CIPSTART=\"TCP\",\"ws.pusherapp.com\",80");
    SoftSerial.flush();
    if (!waitForString(getString(STR_OK), 2, 1000)) {
      return;
    }

    SoftSerial.println(F("AT+CIPSEND"));
    SoftSerial.flush();
    if (waitForString(getString(STR_SENDMODE), 1, 1000)) {

      SoftSerial.print("GET /app/");
      SoftSerial.print(PUSHER_KEY);
      SoftSerial.print("?client=js&version=2.2&protocol=5");
      SoftSerial.print("HTTP/1.1\r\n");
      SoftSerial.print("Upgrade: WebSocket\r\n");
      SoftSerial.print("Connection: Upgrade\r\n");
      SoftSerial.print("Host: ws.pusherapp.com\r\n");
      SoftSerial.print("Origin: ArduinoWebSocketClient\r\n");
      SoftSerial.print("\r\n");
      SoftSerial.flush();
      if (waitForString(getString(WS_OK), 2, 1000)) {

        wssend(getString(SUBSCRIBE_JSON));
        connected = true;

      } else {
        //Serial.println("ERROR http");
      }

    } else {
      SoftSerial.println(F("AT+CIPCLOSE"));
      SoftSerial.flush();
    }
  }

  if (connected) {
    if (SoftSerial.available()) {
      char ch = SoftSerial.read();
      if (ch == getString(ledAOff)[ledAoffC]) {
        if (++ledAoffC == 7) {
          digitalWrite(LED_A, LOW);
        }
      } else {
        ledAoffC = 0;
      }
      if (ch == getString(ledAOn)[ledAonC]) {
        if (++ledAonC == 6) {
          digitalWrite(LED_A, HIGH);
        }
      } else {
        ledAonC = 0;
      }
      if (ch == getString(ledBOff)[ledBoffC]) {
        if (++ledBoffC == 7) {
          digitalWrite(LED_B, LOW);
        }
      } else {
        ledBoffC = 0;
      }
      if (ch == getString(ledBOn)[ledBonC]) {
        if (++ledBonC == 6) {
          digitalWrite(LED_B, HIGH);
        }
      } else {
        ledBonC = 0;
      }
      if (ch == getString(ping)[pingC]) {
        if (++pingC == 4) {
          wssend(getString(PONG_JSON));
        }
      } else {
        pingC = 0;
      }
    }
  }
}

boolean connectWiFi() {
  SoftSerial.println(F("AT+CWJAP?"));
  SoftSerial.flush();
  if (waitForString(getString(STR_APNAME), 8, 500)) {
    return true;
  }
  SoftSerial.println(F("AT+CWMODE=1"));
  SoftSerial.flush();
  waitForString(getString(STR_OK), 2, 500);

  SoftSerial.print("AT+CWJAP=\"");
  SoftSerial.print(SSID);
  SoftSerial.print("\",\"");
  SoftSerial.print(PASS);
  SoftSerial.println("\"");
  SoftSerial.flush();
  if (waitForString(getString(STR_OK), 2, 10000)) {
    return true;
  } else {
    return false;
  }
}

bool waitForString(char* input, uint8_t length, unsigned int timeout) {

  unsigned long end_time = millis() + timeout;
  int current_byte = 0;
  uint8_t index = 0;

  while (end_time >= millis()) {

    if (SoftSerial.available()) {
      current_byte = SoftSerial.read();
      if (current_byte != -1) {
        if (current_byte == input[index]) {
          index++;
          if (index == length) {
            return true;
          }
        } else {
          index = 0;
        }
      }
    }
  }
  return false;
}
char* getString(const char* str) {
  strcpy_P(str_buffer, (char*)str);
  return str_buffer;
}