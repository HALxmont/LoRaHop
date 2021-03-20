
//ping pong prototype
#include <SoftwareSerial.h>

SoftwareSerial lora(4,3);


int ID = 1;         //node ID. 0 will be the first speaker.
char r_message = ' '; //buffer for reception message
char s_message = ' '; //buffer for message to send 
char confirmed = "CONFIRMED!";  //confirmation message
bool ping = true;

void setup() {
Serial.begin(9600); //initial serial communication
lora.begin(9600); //initial serial comunication for Lora Module
}

void loop() {
  
 // if (lora.available()) { // only when you receive data:   
    r_message = lora.read(); // read the incoming message from Lora module
    Serial.print("I received: "); // say what you got:
    Serial.print(r_message); // say what you got:
    Serial.print("\n");
//  }

lora.print("Pong"); //send pong from Lora Module
delay(200);

}
