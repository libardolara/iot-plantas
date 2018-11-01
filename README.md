# IoT Platform en monitoreo de plantas

Este tutorial muestra como poder unir un dispositivo de IoT como Node MCU, Arduino, Raspberry Pi, etc. con los servicios en IBM Cloud. El tutorial tiene como objetivo de realizar un sistema de monitoreo de plantas, con sensores de humedad y de luminicidad, que permita visualizar el estado actual y configurar reglas y alertas tempranas.

Después de terminar este tutorial usted entenderá como: 

* Configurar y conectar un dispositivo
* Crear tableros de control
* Crear interfaces fisicas y logicas
* Integrar el flujo de datos con una aplicación en Node-RED
* Crear reglas

El servicio de Internet of Things Platform permite 


## Componentes Incluidos

* [Internet of Things Platform](https://console.bluemix.net/catalog/services/internet-of-things-platform): Permite comunicarse y consumir datos de dispositivos conectados y gateways.
* [Node-RED](https://nodered.org/): Es un framework Open Source que permite construir aplicaciones basdas en flujos y eventos.
* [Cloudant](https://console.ng.bluemix.net/catalog/services/cloudant-nosql-db): Una base de datos completamente manejada diseñada para aplicaciones web y mobile modernas que usan documentos como JSON.
* [Arduino IDE](https://www.arduino.cc/en/Main/Software): Software Open Source que facilita escribir código y cargarlo en la board


## Materiales

* Tarjeta Node MCU with ESP8266
* Sensor de humedad (HL-69) + transductor
* Sensor de luminicidad (BH1750)
* Cables para conexiones
* 2 Resistencias 12K Ohms

# Paso a Paso

### 1. Clonar el repo

Clona el repositorio `iot-plantas` localmente. En una terminal, ejecuta:

```
$ git clone https://github.com/libardolara/iot-plantas
```

### 2. Configuración del Arduino IDE

Se debe configurar el Arduino IDE para trabajar con la tarjeta Node MCU ESP8266 y con el sensor de luz. Para esto debes tener instalado [Arduino IDE](https://www.arduino.cc/en/Main/Software).

#### Configurar Administrador de la Tarjeta

* Abre **Arduino IDE**
* Ve al menú _Archivo -> Preferencias_ y en el campo para adicionar una URL para administrar tarjetas agrega la url para la tarjeta ESP8266 http://arduino.esp8266.com/stable/package_esp8266com_index.json
* En el menú _Herramientas -> Board... -> Administrador de Tarjetas..._
* En el buscador de la ventana emergente escribe **esp**
* Selecciona el administrador para la tarjeta ESP8266, antes de hacer click en instalar asegurate que la version sea la ultima.
* Espera mientras instala el administrador de la tarjeta.
* En el menú _Herramientas -> Board..._ selecciona la tarjeta NodeMCU (ESP-12E Module)

#### Agregar librerias

* Ve al menú _Sketch -> Include Library -> Add .ZIP Library..._.
* Selecciona el archivo [](pubsubclient-2.3.zip) para instalar la libreria de mensajeria MQTT.
* Ve al menú _Sketch -> Include Library -> Add .ZIP Library..._.
* Selecciona el archivo [](BH1750-master.zip) para instalar la libreria del sensor de luz.

### 3. Conectar y probar el sensor de humedad

Conecta la tarjeta ESP8266 con el sensor de humedad, el transductor y las resistencias. Sigue el diagrama:

![](img/diagram_humidity.png)

* Conecta la tarjeta a tu puerto USB
* Abre el archivo [](codes/humidity_sensor.ccp), copia el contenido y pegalo en el editor del Arduino IDE
* Ve al menú _Herramientas -> Port... ->_ y selecciona un puerto USB (COM*)

> Si utilizas MacOS instala primero el driver para usar el puerto USB de https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers

* Haz click en el boto de ejecutar el código.
* Espera la compilación del programa, si falla vuelve a intentarlo cambiando de puerto en el IDE.
* Cuando la compilación termine exitosamente al 100% ve al menú _Herramientas -> Monitor Serial_ para revisar las lecturas del sensor de humedad

Ejemplo:
```
Funciona !!
Soil Moisture:
123
```

### 4. Conectar y probar el sensor de luz

Conecta el dispositivo creado en el punto anterior con el sensor de luz. (Antes desconecta el puerto USB) 
Sigue el diagrama:

![](img/diagram_light.png)

* Conecta la tarjeta a tu puerto USB
* Abre el archivo [](codes/light_sensor.ccp), copia el contenido y pegalo reemplazando el código del editod el Arduino IDE
* Haz click en el boto de ejecutar el código.
* Espera la compilación del programa.
* Cuando la compilación termine exitosamente al 100% ve al menú _Herramientas -> Monitor Serial_ para revisar las lecturas del sensor de humedad

Ejemplo:
```
Nivel de iluminacion: 243 lx
```


### 5. Crear el servicio IoT Platform

* En el catalogo de IBM Cloud, haz click en la categoria _Kits de Iniciador_ (Starter Kits)
* Haz click en el kit **Internet of Things Platform Starter** el cual aparte de crear el servicio de IoT Platform, crea y asocia una aplicación desarrollada en Node-RED con una base de datos Cloudant.
* Dale un nombre a tu aplicación que sea unico y pueda ser usado como subdominio.
* Haz click en _Crear_
* Mientras tu aplicación se inicializa, haz click en el menú de la izquierda, Conexiones (Connections)
* Haz click en el servicio de tipo **Internet of Things Platform**

### 6. Registro del dispositivo

Usaremos el servicio de IoT Platform para crear un dispositivo dentro de nuestro sistema, para poder gestionarlo y recibir la información que este envie a la nube.

* Haz click en el botón _Launch_ 
* En la esquina superior derecha, haz click en el boton _Add Device_
* En el campo _Device Type_ escribe `ESP8266-MONITOR`
* En el campo _Device ID_ escribe un ID especifico, por ejemplo `M-4371`

> Estos campos son de identificación del dispositivo, por lo cual los usaremos para autenticarnos a la plataforma desde las tarjetas ESP8266

* Haz click en _Next_
* La plataforma nos da la opción de especificar mas información del dispositivo como; numero serial, modelo, descripcion, version del hardaware, etc. Esta es información que generalmenta la indrustria usa para administrar y certificar sus dispositivos.

Ejemplo:
```
Serial Number: 3186229960
Model: ESP-12E
Description: Monitor de Plantas
Hardware Version: 00001
Manufacturer: Node MCU
Device Class: Tarjeta MCU
Firmware Version: 00001
Descriptive Location: Indoors
```

* Haz click en _Next_
* En la pestaña de seguridad usaremos el defecto, que genera automaticamente un token. Haz click en _Next_
* Termina el registro de tu dispositivo haciendo click en _Done_
* El sistema solo te dejará ver token de seguridad una vez. Anota el `Organization ID, Device Type, Device ID y Authentication Token` que seran nuestra informacion de autenticación desde el dispositivo.
* Antes de continuar a configurar la tarjeta debemos configurar la seguridad para no usar certificados de seguridad en esta aplicación. Haz click en el icono de un candado en el menú de la izquierda.
* Haz click en el icono de editar (Forma de un Lapíz)
* Selecciona el nivel de seguridad a **TLS Optional** y haz click en el boton de refrescar las politicas (Refresh compliance)
* Vamos a probar todo el dispositivo con los sensores. Abre el archivo [](codes/total_snippet.ccp) copia el contenido y pegalo reemplazando todo el codigo en el Arduino IDE.
* Modifica las variables customizables para poder conectarte al Wifi, como el **ssid** y el **password**
> El ssid es el nombre de la red, es sensible a mayusculas.

Ejemplo:
```c++
const char* ssid = "RedWifi";
const char* password = "miclave";
```

* Modifica las variables customizables para poder conectarte al IoT Platform, como el **ORG**, **DEVICE_TYPE**, **DEVICE_ID** y **TOKEN**

Ejemplo
```c++
#define ORG "1wmrzl"
#define DEVICE_TYPE "ESP8266-MONITOR"
#define DEVICE_ID "M-4371"
#define TOKEN "xQWE123!asd&00"
```

* Haz click en el boto de ejecutar el código.
* Espera la compilación del programa.
* Cuando la compilación termine exitosamente al 100% ve al menú _Herramientas -> Monitor Serial_ para revisar las lecturas de los sensores.


### 7. Crear tableros


### 8. Crear interfaces


