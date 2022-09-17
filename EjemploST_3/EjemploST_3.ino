#include <WiFi.h>                     //Incluye la librería ESP8266WiFi
#include "config.h"
#include "ESP32_Utils.hpp"
WiFiServer server(80);                    //Definimos el puerto de comunicaciones
// Variable to store the HTTP request
String header;
// Variables auxiliares del estado de las salidase
String AZULState = "off";
String ROJOState = "off";
String VERDEState = "off";
// Asignando los piones GPIO
const int AZUL = 21;
const int ROJO = 22;
const int VERDE = 23;
int pulsador = 25;                        // Estado del pulsador
int contador = 0;
//Potenciometro
int potPin = 32;
int potValue = 0;
double valorPorcentaje=0;
void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(AZUL, OUTPUT);  
  pinMode(VERDE, OUTPUT);
  pinMode(ROJO, OUTPUT);
  pinMode(pulsador, INPUT);  
  // Set outputs to LOW
  digitalWrite(AZUL, LOW);
  digitalWrite(VERDE, LOW);
  digitalWrite(ROJO, LOW);             
  ConnectWiFi_STA();
  server.begin();
 
  Serial.printf("IP local del ESP32: %s\n", WiFi.localIP().toString().c_str());
  Serial.printf("DNS #1, #2 IP: %s, %s\n", WiFi.dnsIP().toString().c_str(), WiFi.dnsIP(1).toString().c_str());
  Serial.printf("Submascara de red -subnetMask-: %s\n", WiFi.subnetMask().toString().c_str());
  //Serial.printf("Direccion MAC en modo Server -macAddress-: %s\n", WiFi.macAddress().c_str());
  Serial.printf("IP privada del punto de acceso -dnsIP-: %s\n", WiFi.gatewayIP().toString().c_str());
  //Serial.printf("Nombre del NodeMCU como cliente -hostname-: %s\n", WiFi.hostname().c_str());
  Serial.printf("Connection status: %d\n0=WL_IDLE_STATUS, 1=WL_NO_SSID_AVAIL, 3=WL_CONNECTED, 4=WL_CONNECTED-FAILED y 6=WL_DISCONNECTED\n", WiFi.status());
  Serial.printf("Nombre de red -SSID-: %s\n", WiFi.SSID().c_str());
  //Serial.printf("Clave de red -Password-: %s\n",  WiFi.psk().c_str());
  //Serial.printf("Direccion MAC del punto de acceso -BSSID-: %s\n", WiFi.BSSIDstr().c_str());
  Serial.printf("Intensidad de la conexion con el punto de acceso -RSSI-: %d dB\n", WiFi.RSSI());
  Serial.printf("\n\nUtiliza esta URL para conectar: http://%s/\n", WiFi.localIP().toString().c_str());
} 
 
void loop(){
  WiFiClient client = server.available();   // Escuchar a las clientes entrantes
  if (client) {                             // Si una nueva cliente se conecta,
    Serial.println("New Client.");          // imprime un mensaje en el puerto serieport
    String currentLine = "";                // crea una cadena para contener los datos entrantes del cliente
    while (client.connected()) {  // Loop mientras la cliente está conectada
      if (client.available()) {             // si hay bytes para leer del cliente,
        char c = client.read();             // lee un byte, luego
        Serial.write(c);                    // imprimirlo en el monitor serial
        header += c;
        if (c == '\n') {                    // si el byte es un carácter de nueva línea
          // si la línea actual está en blanco, tienes dos caracteres de nueva línea seguidos.
          // ese es el final de la solicitud HTTP del cliente, así que envíe una respuesta:
          if (currentLine.length() == 0) {
          // Los encabezados HTTP siempre comienzan con un código de respuesta (por ejemplo, HTTP/1.1 200 OK)
          // y un tipo de contenido para que el cliente sepa lo que viene, luego una línea en blanco:
 
  client.println("HTTP/1.1 200 OK");      //Envía la página HTML de respuesta al cliente
  client.println("");                     //No olvidar esta línea de separación
  client.print("<style>");
  client.print(".ampliacion:hover{");
  client.print("transform: scale(1.1);");
  client.print("}");
  client.print(".changecolor:hover{");
  client.print("background-color:#298095;");
  client.print("color: white;");
  client.print("border: solid 3px #164275;");
  client.print("}");
  client.print("</style>");  
  client.print("<div style='width: 100%; height: 10px; background-color: #164275;'></div>");
  client.print("<div style='width: 100%; height: 200px; display:inline-flex; margin-top: 10px; margin-bottom: 10px;'>");
  client.print("<div style='margin-top: 15px; margin-left: 30px; margin-bottom: 15px;'><img src='https://ucblpz.com/wp-content/uploads/2018/12/UCB-logo-lapaz.png'></div>");
  client.print("<div style='text-align: right; margin-left: 150px; margin-top: 50px;'><h1 style='color: #164275; font-size: 40px;'>ESP32 como Station</h1></div>");
  client.print("<div style='text-align: right; margin-left: 50px; margin-top: 50px;'><h1 style='color: #164275; font-size: 40px;'>Internet de las Cosas</h1></div>");
  client.print("</div>");
  client.print("<div style='width: 100%; height: 10px; background-color: #164275;'></div>");
  client.print("<center>");
  //client.print("<div style='margin-top: 80px; width: 100%; display: inline-block;'>");
           // Cambiando estado de los GPIO
            if (header.indexOf("GET /26/on") >= 0) {
              Serial.println("GPIO 26 on");  AZULState = "on"; digitalWrite(AZUL, HIGH);
            } else if (header.indexOf("GET /26/off") >= 0) {
              Serial.println("GPIO 26 off");  AZULState = "off";  digitalWrite(AZUL, LOW);
            } else if (header.indexOf("GET /27/on") >= 0) {
              Serial.println("GPIO 27 on");   VERDEState = "on"; digitalWrite(VERDE, HIGH);
            } else if (header.indexOf("GET /27/off") >= 0) {
              Serial.println("GPIO 27 off");  VERDEState = "off"; digitalWrite(VERDE, LOW);
            }  else if (header.indexOf("GET /21/on") >= 0) {
              Serial.println("GPIO 21 on");   ROJOState = "on"; digitalWrite(ROJO, HIGH);
            } else if (header.indexOf("GET /21/off") >= 0) {
              Serial.println("GPIO 21 off");  ROJOState = "off"; digitalWrite(ROJO, LOW);
            }
           // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link href='https://fonts.googleapis.com/css2?family=Yusei+Magic&display=swap' rel='stylesheet'>");
            //client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons Feel free to change the background-color and font-size attributes to fit your preferences
            // client.println("<img src='https://internacional.ucb.edu.bo/wp-content/uploads/2020/10/ucb-logo.png' alt='Bombilla apagada' style='display:block; margin:auto' width='40%'>");
            client.print("<div style='margin-top: 15px; margin-left: 30px; margin-bottom: 15px;'><img src='https://lpz.ucb.edu.bo/wp-content/uploads/2022/05/Logo-aniversario-22.jpg' style='display:block; margin:auto' width='35%'></div>");
            client.print("<style>@import url('https://fonts.googleapis.com/css2?family=Quicksand:wght@400;500&display=swap');html { font-family: 'Quicksand'; display: inline-block; margin: 0px auto; text-align: center;}.styled-table {border-radius: 20px;border-collapse: collapse;margin: 25px 0;font-size: 0.9em;min-width: 400px;box-shadow: 0 0 20px rgba(0, 0, 0, 0.15);}");  
            client.print(".styled-table thead tr {background-color: #009879;color: #ffffff;text-align: left;}.styled-table th,.styled-table td {padding: 12px 15px;}.styled-table tbody tr {border-bottom: 1px solid #dddddd;}.styled-table tbody tr:nth-of-type(even){background-color: #f3f3f3;}.styled-table tbody tr:last-of-type {border-bottom: 2px solid #009879;}.styled-table tbody tr.active-row {font-weight: bold;color: #009879;}</style></head>");
            client.print("<div style='width: 100%; height: 10px; background-color: #164275;'></div>");
            client.print("<center>");
            // Encabezado de la página web
            client.println("<body>");//<h1>ESP32 Web Server</h1>");
               
            contador++;  
            client.println("");
            client.println("<div style='display: grid; grid-template-columns: repeat(3, 1fr);'><div style='display: grid; grid-template-rows: (3, fr);'><div>"); 
            // Muestra el estado actual y los botones ON/OFF para GPIO 19 
            client.println("<p>AZUL - State " + AZULState + "</p>");
            // Si AZULState está apagado, muestra el botón ON      
            if (AZULState=="off") {
               client.print("<div style='display: inline-flex; margin: 0% 10%;'><img src='http://www.clker.com/cliparts/1/3/e/6/1194989228908147779led_circle_grey.svg.med.png'  width='100px'><input type='image' src='http://esploradores.com/Practicas_html/encender_300x88.png' style='display:block; margin:auto' width='40%' onClick=location.href='/26/on'></div>");         
            } else {
              client.print("<div style='display: inline-flex; margin: 0% 10%;'><img src='http://www.clker.com/cliparts/6/4/9/f/1194989227957039545led_circle_blue.svg.med.png'  width='100px'><input type='image' src='http://esploradores.com/Practicas_html/apagar_300x88.png' style='display:block; margin:auto' width='40%' onClick=location.href='/26/off'></div>");   
            } 
            // Muestra el estado actual y los botones ON/OFF para GPIO 22  
            client.println("</div><div><p>VERDE - State " + VERDEState + "</p>");
            // Si el VERDEState está apagado, muestra el botón ON      
            if (VERDEState=="off") {
              client.print("<div style='display: inline-flex; margin: 0% 10%;'><img src='http://www.clker.com/cliparts/1/3/e/6/1194989228908147779led_circle_grey.svg.med.png'  width='100px'><input type='image' src='http://esploradores.com/Practicas_html/encender_300x88.png' style='display:block; margin:auto' width='40%' onClick=location.href='/27/on'></div>");
            } else {
              client.print("<div style='display: inline-flex; margin: 0% 10%;'><img src='http://www.clker.com/cliparts/1/5/4/b/11949892282132520602led_circle_green.svg.med.png'  width='100px'><input type='image' src='http://esploradores.com/Practicas_html/apagar_300x88.png' style='display:block; margin:auto' width='40%' onClick=location.href='/27/off'></div>");
            }  
            // Muestra el estado actual y los botones ON/OFF para GPIO 21
            client.println("</div><div><p>ROJO - State " + ROJOState + "</p>");
            // Si el ROJOState está apagado, muestra el botón ON      
            if (ROJOState=="off") {
              client.print("<div style='display: inline-flex; margin: 0% 10%;'><img src='http://www.clker.com/cliparts/1/3/e/6/1194989228908147779led_circle_grey.svg.med.png'  width='100px'><input type='image' src='http://esploradores.com/Practicas_html/encender_300x88.png' style='display:block; margin:auto' width='40%' onClick=location.href='/21/on'></div>");             
            } else {
              client.print("<div style='display: inline-flex; margin: 0% 10%;'><img src='http://www.clker.com/cliparts/5/7/b/5/1194989231691813435led_circle_red.svg.med.png'  width='100px'><input type='image' src='http://esploradores.com/Practicas_html/apagar_300x88.png' style='display:block; margin:auto' width='40%' onClick=location.href='/21/off'></div>");   
            }  
            client.println("</div></div><div style='display: grid; grid-template-rows: repeat(2, 1fr);'><div style='display:flex; align-items: center; justify-content: center;'><div><p>Pulsador</p>");
            //Imprime la lectura de un pulsador
            //client.print("<h1>El estado del pulsador: ");                 
            //client.print(String(digitalRead(pulsador))+"</h1>");
            if(digitalRead(pulsador) == 0){
              client.print("<div style='display: flex; justify-content: center;'><img src='https://www.clker.com/cliparts/9/1/5/2/119498475589498995button-red_benji_park_01.svg.med.png'  width='100px'></div>");
            } else {
              client.print("<div style='display: flex; justify-content: center;'><img src='https://www.clker.com/cliparts/4/8/d/d/11949847561597560220button-seagreen_benji_pa_01.svg.med.png'  width='100px'></div>");
            }
            valorPorcentaje = (potValue*100)/4893;
            if(potValue==0){
              client.println("</div></div><div><h3>Valor del adc: </h3><img style='width: 50%;margin-top: 5px;' src='https://raw.githubusercontent.com/MicaelaGordillo/IOT_2/main/EjemploST_3/images/bateria0.jpg' alt='bateria'><p>"+String(valorPorcentaje)+"%</p></div></div>");
            }else if(potValue<978){
              client.println("</div></div><div><h3>Valor del adc: </h3><img style='width: 50%;margin-top: 5px;' src='https://raw.githubusercontent.com/MicaelaGordillo/IOT_2/main/EjemploST_3/images/bateria20.jpg' alt='bateria'><p>"+String(valorPorcentaje)+"%</p></div></div>");
            }else if(potValue>=979&&potValue<1957){
              client.println("</div></div><div><h3>Valor del adc: </h3><img style='width: 50%;margin-top: 5px;' src='https://raw.githubusercontent.com/MicaelaGordillo/IOT_2/main/EjemploST_3/images/bateria40.jpg' alt='bateria'><p>"+String(valorPorcentaje)+"%</p></div></div>");
            }else if(potValue>=1958&&potValue<2935){
              client.println("</div></div><div><h3>Valor del adc: </h3><img style='width: 50%;margin-top: 5px;' src='https://raw.githubusercontent.com/MicaelaGordillo/IOT_2/main/EjemploST_3/images/bateria60.jpg' alt='bateria'><p>"+String(valorPorcentaje)+"%</p></div></div>");
            }else if(potValue>=2936&&potValue<3914){
              client.println("</div></div><div><h3>Valor del adc: </h3><img style='width: 50%;margin-top: 5px;' src='https://raw.githubusercontent.com/MicaelaGordillo/IOT_2/main/EjemploST_3/images/bateria80.jpg' alt='bateria'><p>"+String(valorPorcentaje)+"%</p></div></div>");
            }else{
              client.println("</div></div><div><h3>Valor del adc: </h3><img style='width: 50%;margin-top: 5px;' src='https://raw.githubusercontent.com/MicaelaGordillo/IOT_2/main/EjemploST_3/images/bateria100.jpg' alt='bateria'><p>"+String(valorPorcentaje)+"%</p></div></div>");
            }
            
            

            client.print("<div style='display: grid; grid-template-rows:80px 1fr 1fr 1fr; background-color: rgb(160, 18, 18);'><h2>Integrantes: </h2><div style='background-color: #658b62;'><p>Yamil Barrientos</p></div><div style='background-color: #8b6281;'><p>Micaela Gordillo</p></div><div style='background-color: #626f8b;'><p>Naomi Tacachira</p></div></div></div></body>");       
            client.print("<h1>Informacion de la conexion </h1>"); 
            client.print("</div>");
            client.print("<center>");
            client.print("<div style='width: 90%; margin-top: 120px;transition: 1s;' class='ampliacion'>");
            client.print("<table class='styled-table'>");
            client.print("<thead>");
            client.print("<tr><th style=' padding: 20px;'>IP Local</th><th style=' padding: 20px; '>Host Name</th><th style=' padding: 20px; '>Status</th><th style=' padding: 20px; '>SSID</th><th style=' padding: 20px; '>PSK</th><th style=' padding: 20px; '>BSSID</th><th style=' padding: 20px; '>RSSI</th></tr>");
            client.print("</thead>");
            client.print("<tr>");
            client.print("<td style='padding: 20px;text-align: center;'class='changecolor'>");
            client.print(WiFi.localIP().toString().c_str());   
            client.print("</td>");
            client.print("<td style='padding: 20px;text-align: center;'class='changecolor'>");
            //client.print(WiFi.hostname().c_str());   
            client.print("</td>");
            client.print("<td style='padding: 20px;text-align: center;'class='changecolor'>");
            client.print(WiFi.status());   
            client.print("</td>");
            client.print("<td style='padding: 20px;text-align: center;'class='changecolor'>");
            client.print(WiFi.SSID().c_str());   
            client.print("</td>");
            client.print("<td style='padding: 20px;text-align: center;'class='changecolor'>");
            client.print(WiFi.psk().c_str());   
            client.print("</td>");
            client.print("<td style='padding: 20px;text-align: center;'class='changecolor'>");
            client.print(WiFi.BSSIDstr().c_str());   
            client.print("</td>");
            client.print("<td style='padding: 20px;text-align: center;'class='changecolor'>");
            client.print(WiFi.RSSI());   
            client.print("</td>"); 
            client.print("</tr>");
            client.print("</table>");
            client.print("</div>");
            client.print("</center>");
            client.print("</center>");
            client.println("</html>");  
            // La respuesta HTTP termina con otra línea en blanco
            client.println();
            // Salir del bucle while
            break;
          } else { // si obtuvo una nueva línea, borre la línea actual
            currentLine = "";
             }
        } else if (c != '\r') {  // si obtuvo algo más que un carácter de retorno de carro,
          currentLine += c;      // agregarlo al final de la línea actual
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
  //ADC
  potValue = analogRead(potPin);
  delay(50);
}
