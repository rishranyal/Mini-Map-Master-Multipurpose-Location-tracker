#include <SoftwareSerial.h>
#include <TinyGPSPlus.h>

SoftwareSerial ss(5, 6); // RX, TX

TinyGPSPlus gps;
float f_buf[2];

void setup()
{
  Serial.begin(115200);
  ss.begin(9600);

  Serial.println("Initializing A9G...");

  sendATCommand("AT");
  sendATCommand("AT+CREG?");
  sendATCommand("AT+CPIN?");
  sendATCommand("AT+GPS=1");

  delay(10000); // Allow more time for GPS fix
  sendATCommand("AT+GPS?");

  Serial.println("A9G initialized!");
}

void loop()
{
  sendATcCommand("AT+GPSRD=1\r");
  readGPSData();

  delay(30000); // Delay before next data upload (adjust as needed)
}

void readGPSData()
{

    String c = ss.read()+"\r";
   
    
  


    f_buf[0] = gps.location.lat();
    f_buf[1] = gps.location.lng();

    // Print the latitude and longitude to Serial monitor
    Serial.print("Latitude (deg): ");
    Serial.println(f_buf[0], 6);

    Serial.print("Longitude (deg): ");
    Serial.println(f_buf[1], 6);

    // Send data to ThingSpeak
    sendToThingSpeak();
  
}

void sendToThingSpeak()
{
  String apiKey = "EOPHC8L3ACH5KPIL"; // Your ThingSpeak API key
  String data = "field1=" + String(f_buf[0], 6) + "&field2=" + String(f_buf[1], 6) + "&api_key=" + apiKey;

  Serial.println("Sending HTTP request to ThingSpeak...");
  sendATCommand("AT+CGATT=1");
  sendATCommand("AT+CGDCONT=1,\"IP\",\"bsnlnet\"");
  delay(800);
  sendATCommand("AT+CGACT=1,1");
  delay(800);
  sendATCommand("AT+HTTPGET=\"http://api.thingspeak.com/update?" + data + "\"");
  delay(4000);

  Serial.println("HTTP request sent!");
}

void sendATCommand(String command)
{
  Serial.println("Sending AT command: " + command);
  ss.println(command);
  delay(1000);
  //while (ss.available())
  {
   // Serial.write(ss.read());
  }
  Serial.println();
}
void sendATcCommand(String command)
{
  Serial.println("Sending AT command: " + command);
  ss.println(command);
  delay(1000);
  while (ss.available())
  {
   Serial.write(ss.read());
  }
  Serial.println();
}
