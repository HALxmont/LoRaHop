
//ping pong prototype

#include <SoftwareSerial.h>
#include <string.h>

SoftwareSerial lora(4,3);
/*
int ID = 0;         //node ID. 0 will be the first speaker.
char r_message = ' '; //buffer for reception message
char s_message = ' ';//buffer for message to send 
//char confirmed = "CONFIRMED!";  //confirmation message
bool ping = true;


String cadena0 = "012345";
String cadena1 = "6789ab";
String cadena2 = "cdefgh";
String cadena3 = "ijklmn";
String cadena4 = "opqrst";
String cadena5 = "vwxyz0";
String cadena6 = "_01234";
String cadena7 = "_56789";
String cadena8 = "_abcde";
String cadena9 = "_fghij";
String cadena10 = "_klmno";


String concat0 = cadena0 + cadena1;
String concat1 = concat0 + cadena2;
String concat2 = concat1 + cadena3;
String concat3 = concat2 + cadena4;
String concat4 = concat3 + cadena5;
String concat5 = concat4 + cadena6;
String concat6 = concat5 + cadena7;
String concat7 = concat6 + cadena8;
String concat8 = concat7 + cadena9;
String concat9 = concat8 + cadena10;
*/
                                    //particular node1 node 0x00 0x01 0x0f   (x  direccion  canal)
int objetive[] = {0xFF,0xFF,0x0F}; //BRODACAST 0XFF 0XFF 0X0F

// STRUCTS DEFFINITIONS
struct ME{
  int dir;
  int channel;
};

struct Node1{
  int dir;
  int channel;
};
  
struct Node2{
  int dir;
  int channel;
};
  
struct Node3{
  int dir;
  int channel; 
};

struct Node4{
  int dir;
  int channel;
};

struct Node5{
  int dir;
  int channel;
};


void setup() {
Serial.begin(9600); //initial serial communication
lora.begin(9600); //initial serial comunication for Lora Module

}

void loop() {  

Node5.dir = 15;
/*
  for(int i = 0; i<11; i++){
    //if(i == 0) lora.print("--- New Frame ---\r\n");
    if(i == 0) lora.print(cadena0);
    if(i == 1) lora.print(concat0);
    if(i == 2) lora.print(concat1);
    if(i == 3) lora.print(concat2);
    if(i == 4) lora.print(concat3);
    if(i == 5) lora.print(concat4);
    if(i == 6) lora.print(concat5);
    if(i == 7) lora.print(concat6);
    if(i == 8) lora.print(concat7);
    if(i == 9) lora.print(concat8);
    if(i == 10) lora.print(concat9);
    //if(i == 10) lora.print("Scince Bitch!");  
    lora.print("\r\n"); //new line baby!
    delay(500);
  }
lora.print(a); //send ping from lora module
*/
  


//lora.write(objetive[0]);
//lora.write(objetive[1]);
//lora.write(objetive[2]);

//lora.write("Ping\n");
//lora.write(0x70);
//lora.write(0x69);
//lora.write(0x6E);
//lora.write(0x67);

delay(100);
 // if(lora.available() > 1){
 //   String input = lora.readString();
 //   Serial.println(input);    
 // }
  
}
