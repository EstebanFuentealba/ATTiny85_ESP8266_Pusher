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
# Actualizar firmware del ESP8266
Lo primero es actualizar el firmware, lo actualizaremos a la versión [AI-v0.9.5.0 AT Firmware.bin](https://raw.githubusercontent.com/EstebanFuentealba/ATTiny85_ESP8266_Pusher/firmware%20esp8266/AI-v0.9.5.0%20AT%20Firmware.bin). Después de probar muchos firmwares es el unico que me funcionó para el ATTiny85 configurado a 4800 bauds.
Para subir el archivos necesitamos conectar el ESP8266 al Arduino UNO como se ve en el siguiente diagrama

![alt text](https://raw.githubusercontent.com/EstebanFuentealba/ATTiny85_ESP8266_Pusher/master/firmware%20esp8266/ESP8266_Pines.jpg "ESP8266")
![alt text](https://raw.githubusercontent.com/EstebanFuentealba/ATTiny85_ESP8266_Pusher/master/firmware%20esp8266/ESP8266-Connections-Arduino-UNO.png "Conexión")

Después de conectar utilizaremos el programa [esp8266_flasher.exe](https://raw.githubusercontent.com/EstebanFuentealba/ATTiny85_ESP8266_Pusher/firmware%20esp8266/esp8266_flasher.exe) Para windows o [esptool.py](https://github.com/themadinventor/esptool) para otros sistemas operativos.

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