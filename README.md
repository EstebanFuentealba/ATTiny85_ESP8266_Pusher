# ATTiny85_ESP8266_Pusher
ATTiny85 + ESP8266 + Pusher.com WebSocket Client


Materiales
===================
- ATTiny85
- 2 Led
- Protoboard
- ESP8266
- Arduino UNO
- USBTinyISP (OPCIONAL Para subir código al ATTiny85)
- Cuenta en Pusher.com
- Cables


Instrucciones
===================
## Crear Cuenta en Pusher.com
Pusher es un servicio que provee de servidores websocket para utilizarlo como uno quiera. Esto funciona con tres actores:
- servicio (pusher)
- servidor (envía eventos)
- cliente (se conecta al servicio escucha y recibe eventos)

![alt text](https://raw.githubusercontent.com/EstebanFuentealba/ATTiny85_ESP8266_Pusher/master/images/hero_howitworks.png "cómo funciona")

De la imagen anterior debemos entender que ATTiny85 funcionará como cliente reemplazando a *Browsers*

Conociendo eso entramos a [https://pusher.com/signup](https://pusher.com/signup) ingresan su email - contraseña o loguearse con alguna cuenta de github o google y confirmar cuenta. Después de eso ya podemos crear apps. Luego de eso los redirecciona a un dashboard y solo deben presionar el botón:

![alt text](https://raw.githubusercontent.com/EstebanFuentealba/ATTiny85_ESP8266_Pusher/master/images/newapp.png "new app")

se abre un modal y le damos un nombre y checkeamos *Enable client event* y presionamos *Create App*

![alt text](https://raw.githubusercontent.com/EstebanFuentealba/ATTiny85_ESP8266_Pusher/master/images/newapp_conf.png "configure new app")

ya tenemos una app y podemos comenzar a utilizar el servicio. Nos entrega unos ejemplos de Cliente servidor, pero a nosotros solo nos importará la APP KEY por ejemplo
```
key = '856c493d6d461226XXX'
```

## Actualizar firmware del ESP8266
Lo primero es actualizar el firmware, lo actualizaremos a la versión [AI-v0.9.5.0 AT Firmware.bin](https://github.com/EstebanFuentealba/ATTiny85_ESP8266_Pusher/raw/master/firmware%20esp8266/AI-v0.9.5.0%20AT%20Firmware.bin). Después de probar muchos firmwares es el unico que me funcionó para el ATTiny85 configurado a 4800 bauds.
Primero se sube un sketch vacio al Arduino UNO

```arduino
void setup() {
  // put your setup code here, to run once:
}
void loop() {
  // put your main code here, to run repeatedly:
}
```

Después necesitamos conectar el ESP8266 al Arduino UNO como se ve en el siguiente diagrama

![alt text](https://raw.githubusercontent.com/EstebanFuentealba/ATTiny85_ESP8266_Pusher/master/firmware%20esp8266/ESP8266_Pines.jpg "ESP8266")
![alt text](https://raw.githubusercontent.com/EstebanFuentealba/ATTiny85_ESP8266_Pusher/master/firmware%20esp8266/ESP8266-Connections-Arduino-UNO.png "Conexión")

Después de conectar utilizaremos el programa [esp8266_flasher.exe](https://github.com/EstebanFuentealba/ATTiny85_ESP8266_Pusher/raw/master/firmware%20esp8266/esp8266_flasher.exe) Para windows o [esptool.py](https://github.com/themadinventor/esptool) para otros sistemas operativos.

![alt text](https://raw.githubusercontent.com/EstebanFuentealba/ATTiny85_ESP8266_Pusher/master/firmware%20esp8266/FO8YOLOIATH773M.LARGE.jpg "Programa esp8266_flasher")

Se debe seleccionar el archivo presionando el botón "Bin", escribir el puerto donde está conectado el Arduino UNO (COM1) y presionar el botón "Download" , y comenzará a escribirse el firmware en el 8266 imprimiendo algo como lo siguiente

```
connecting ....
Erasing flash...
Writing at 0x00000000... (0 %)
Writing at 0x00000400... (0 %)
Writing at 0x00000800... (0 %)
Writing at 0x0007e800... (99 %)
Writing at 0x0007ec00... (99 %)
Leaving... Failed to leave Flash mode
```

Al terminar de actualizar el firmware deben desconectar del 8266 el pin GPIO0 

## Configurar ESP8266
Despues de tener el nuevo firmware vamos a cambiar un parametro del modulo wifi, debemos dejar el baudaje de comunicación a 4800, por defecto viene con 9600. Para ello nuevamente usaremos Arduino UNO, necesitamos subir el siguiente sketch:

```arduino
#include <SoftwareSerial.h>
SoftwareSerial BT1(3, 2); // RX | TX
void setup() {  
	Serial.begin(9600);
    BT1.begin(9600);
}

void loop() {
	String B= "." ;
	if (BT1.available()) { 
		char c = BT1.read() ;
		Serial.print(c);
	}
	if (Serial.available()){
		char c = Serial.read();
		BT1.print(c);
	}
}
```

y conectaremos el modulo ESP8266 de la siguiente forma:

![alt text](https://raw.githubusercontent.com/EstebanFuentealba/ATTiny85_ESP8266_Pusher/master/firmware%20esp8266/ESP8266-Configure-AT.png "Configurando con comandos AT")

Ahora en el programa Arduino vamos a *Herramientas* -> *Monitor Serie* , Seleccionar *Ambos NL & CR* y *9600 baudio*

![alt text](https://raw.githubusercontent.com/EstebanFuentealba/ATTiny85_ESP8266_Pusher/master/firmware%20esp8266/monitor_cfg.png "Configurando Monitor Serie")


y podremos ver algo como lo siguiente 

![alt text](https://raw.githubusercontent.com/EstebanFuentealba/ATTiny85_ESP8266_Pusher/master/firmware%20esp8266/monitor.png "Monitor Serie")


Ahora ya podemos comenzar a escribir comandos **AT**, lo primero que podemos escribir es el Comando **AT** que debería responder **OK** 

![alt text](https://raw.githubusercontent.com/EstebanFuentealba/ATTiny85_ESP8266_Pusher/master/firmware%20esp8266/monitor_AT.png "AT")

para configurar a 4800 bauds debemos escribir **AT+CIOBAUD=4800** que debería responder **OK** aunque ya no se nos verá el texto porque ahora ya no funciona a 9600 bauds. Para volver a ver texto deben subir nuevamente el sketch anterior pero pasando 4800 al método begin
```arduino
BT1.begin(4800);
```
Ahora ya hemos terminado con la configuración del modulo wifi, aunque si quieren investigar mas [acá dejo un pdf](https://github.com/EstebanFuentealba/ATTiny85_ESP8266_Pusher/raw/master/firmware%20esp8266/ESP8266%20AT%20Command%20Set.pdf) con todos los comandos disponibles que usaremos en los siguientes pasos.


## Subir código al ATTiny85
Para subir el código al integrado Se puede usar Arduino UNO o un ISP yo utilizaré USBTinyISP donde debemos conectar el Attiny85 al ISP de la siguiente forma:

![alt text](https://raw.githubusercontent.com/EstebanFuentealba/ATTiny85_ESP8266_Pusher/master/images/6-pin-isp-cable.png "USBTinyISP") ![alt text](https://raw.githubusercontent.com/EstebanFuentealba/ATTiny85_ESP8266_Pusher/master/images/attiny85.png "ATTiny85")

Ya conectado debemos agregar ATTiny a las placas del programa Arduino, yo estoy usando la version 1.6.5 que es sencillo, sólo deben ir a *Archivo* -> *Preferencias* donde encontraremos el siguiente campo

![alt text](https://raw.githubusercontent.com/EstebanFuentealba/ATTiny85_ESP8266_Pusher/master/images/additional-boards-manager-urls-blank.png "Agregar placa")

Ahí agregamos éste link que contiene la configuracion del package que contiene las placas ATTiny
```
https://raw.githubusercontent.com/damellis/attiny/ide-1.6.x-boards-manager/package_damellis_attiny_index.json
```
click en OK para guardar la configuración. Ir a *Herramientas* -> *Placa* -> *Boards Manager...* buscar ATtiny y presionar el botón *Install*

![alt text](https://raw.githubusercontent.com/EstebanFuentealba/ATTiny85_ESP8266_Pusher/master/images/boards-manager-install.png "Install board ATTiny")

Ahora ya puedes utilizar ATTiny en Arduino. Ahora configuraremos Arduino Anda a *Herramientas* 
- **Placa** y selecciona *ATtiny*
- **Procesador** ATTiny85
- **Clock** 8 MHz (internal)
- **Programador** USBtinyISP

![alt text](https://raw.githubusercontent.com/EstebanFuentealba/ATTiny85_ESP8266_Pusher/master/images/attiny_cfg.png "board ATTiny")


Importante es Presionar **Quemar Bootloader**, cuando no hice eso el programa andaba lento


Ahora solo deben configurar [ATTiny85WebSocketClient.ino](https://github.com/EstebanFuentealba/ATTiny85_ESP8266_Pusher/blob/master/attiny85/ATTiny85WebSocketClient.ino) agregando la información de su red wifi y la Key de pusher que obtuvimos en el Paso 1

```arduino
#define SSID        "WifiAP"
#define PASS        "password"
#define PUSHER_KEY  "xxxxxxxxxxxxxxx"
```
El codigo se conecta como Cliente a Pusher esperando eventos en el channel **test_channel**, los eventos que definí son los siguientes:

- **ledAOn** 
- **ledAOff**
- **ledBOn**
- **ledBOff**

esos eventos prenderán o apagarán 2 leds. A continuación dejo el diagrama de como deben ir conectadas las leds y el módulo ESP8266 al ATTiny85

![alt text](https://raw.githubusercontent.com/EstebanFuentealba/ATTiny85_ESP8266_Pusher/master/images/ATtiny85-ESP8266-2Led.png "cableado")

Ahora solo debemos subir el código al ATTint85 y esperar que conecte.


