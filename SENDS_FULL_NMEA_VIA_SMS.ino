#include <SoftwareSerial.h>

SoftwareSerial ss(5, 6); // RX, TX

String rawNMEA; // Variable to store raw NMEA data

void setup()
{
  Serial.begin(115200);
  ss.begin(9600);

  Serial.println("Initializing A9G...");

  sendATCommand("AT");
  sendATCommand("AT+CREG?");
  sendATCommand("AT+CPIN?");
  sendATCommand("AT+GPS=1");
  sendATCommand("AT+GPS?");
  Serial.println("A9G initialized!");
}

void loop()
{
  readGPSData();
  delay(1000);
}

void readGPSData()
{ String rd = ("AT+GPSRD=1\r");
  Serial.println("Sending AT command: " + rd);
  ss.println(rd);

  while (ss.available())
  {
    char c = ss.read();
    Serial.write(c);
    
  
  Serial.println();
  rawNMEA += c;

  // Check for the end of a NMEA sentence
  if (c == '\n')
    {
      // Process the entire NMEA sentence or do additional handling
  sendRawNMEAAsSMS(rawNMEA);

      // Clear the raw NMEA data for the next sentence
      rawNMEA = "";
    }
  }
}

void sendRawNMEAAsSMS(const String &rawNMEASentence)
{
  Serial.println("Sending raw NMEA as SMS...");
  sendATCommand("AT+CMGF=1"); // Set SMS mode to text mode
  delay(1000);

  // Replace "+918493915549" with your desired phone number
  sendATCommand("AT+CMGS=\"+918493915549\"");
  delay(1000);
  ss.print(rawNMEASentence);
  ss.write(0x1A); // Send Ctrl+Z to indicate the end of the SMS message
  delay(1000);

  Serial.println("SMS sent!");
}

void sendATCommand(String command)
{
  Serial.println("Sending AT command: " + command);
  ss.println(command);
  delay(1000);
  while (ss.available())
  {
    char response = ss.read();
    Serial.write(response);
  }
  Serial.println();
}
