#include <SPI.h>
#include <Ethernet.h>
#include <ArduinoRS485.h> 
#include <ArduinoModbus.h>

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 0, 88);
EthernetServer ethServer(502);
ModbusTCPServer modbusTCPServer;


bool estadoButton = 0;
bool estadobotao1 = 0;
bool estadobotao2 = 0;
bool estadobotao3 = 0;
bool estadobotao4 = 0;
bool estadobotao5 = 0;

bool conmuta = true;
bool conmuta2 = true;
bool conmuta3 = true;
bool conmuta4 = true;


int contador3 = 0;
int contador4 = 0;



// For variable sensor
int sensor1 = 0;
int sensor3 = 0;
int bit_val2 = 0;
int bit_val3 = 0;
int detectado = 0;
int sensor2 = 0;
int sensor4 = 0;
int contagem1 = 0;
int contagem2 = 0;
int bit_val9 = 0;
int bit_val10 = 0;
int bit_val11 = 0;
int bit_val12 = 0;
int bit_val13 = 0;
int moveX = 0;
int moveZ = 0;

//For stacker
bool status_storage = false;
bool status_retrieving = false;
int storage_pos = 1;
int retrieving_pos = 1;

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500); 
  
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  pinMode(A6, INPUT);
  pinMode(A7, INPUT);


  
  
  Serial.println("Ethernet Modbus TCP Example");

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);

  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    if (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }

  // start the server
  ethServer.begin();

  // start the Modbus TCP server
  if (!modbusTCPServer.begin()) {
    Serial.println("Failed to start Modbus TCP Server!");
    if (1);
  }

  // configure 16 coils at address 0x00
  // Coil 0-7 for Sensor
  // Coil 8-15 for actuator
  modbusTCPServer.configureCoils(0, 20);

  // configure Holding Register at address 0x00 for target position
  modbusTCPServer.configureHoldingRegisters(0, 1);
  
}

void loop() {

    // listen for incoming clients
  EthernetClient client = ethServer.available();

  if (client) {
    // a new client connected
    Serial.println("new client");

    // let the Modbus TCP accept the connection
    modbusTCPServer.accept(client);

    while (client.connected()) {
      // poll for Modbus TCP requests, while client connected
      modbusTCPServer.poll();

      // update sensor
      read_coils();

      //check button
      check_button();
    }

    Serial.println("client disconnected");
  }
  
  
}


void read_coils() {
  // read the current value of the coil
  sensor1 = modbusTCPServer.coilRead(0);
  sensor3 = modbusTCPServer.coilRead(1);
  bit_val2 = modbusTCPServer.coilRead(2);
  bit_val3 = modbusTCPServer.coilRead(3);
  detectado = modbusTCPServer.coilRead(4);
  sensor2 = modbusTCPServer.coilRead(5);
  sensor4 = modbusTCPServer.coilRead(6);
  contagem1 = modbusTCPServer.coilRead(7);
  contagem2 = modbusTCPServer.coilRead(8);
  bit_val9 = modbusTCPServer.coilRead(9);
  bit_val10 = modbusTCPServer.coilRead(10);
  bit_val11 = modbusTCPServer.coilRead(11);
  bit_val12 = modbusTCPServer.coilRead(12);
  bit_val13 = modbusTCPServer.coilRead(13);
  moveX = modbusTCPServer.coilRead(14);
  moveZ = modbusTCPServer.coilRead(15);

}

void print_status() {
  //Print serial
  Serial.print("Status Sensors: ");
  Serial.print(sensor1);
  Serial.print(", ");
  Serial.print(sensor3);
  Serial.print(", ");
  Serial.print(bit_val2);
  Serial.print(", ");
  Serial.print(bit_val3);
  Serial.print(", ");
  Serial.print(detectado);
  Serial.print(", ");
  Serial.print(sensor2);
  Serial.print(", ");
  Serial.print(sensor4);
  Serial.print(", ");
  Serial.println(contagem1);
  Serial.print("Storage Position: ");
  Serial.print(storage_pos);
  Serial.print(", Retrieving Position: ");
  Serial.println(retrieving_pos);
}



void check_button() {

  if(contador3 >= 20){
    digitalWrite(D2, HIGH);
    digitalWrite(LED_D2, HIGH);
  }
  
  if(contador4 >= 20) {
    digitalWrite(D3, HIGH);
    digitalWrite(LED_D3, HIGH);
  }
  
  if (moveX == HIGH){
    if (conmuta3) {
        contador3++;
        conmuta3 = false;
      }
  
    } else {
      conmuta3 = true;
    }
  
  if (moveZ == HIGH){
    if (conmuta4) {
        contador4++;
        conmuta4 = false;
      }
  
    } else {
      conmuta4 = true;
    }




/**************************** AUTOMATICO *********************************/

if(estadoButton == HIGH && estadobotao5 == HIGH){

  if(sensor1 == HIGH && sensor2 == LOW){
    modbusTCPServer.coilWrite(12, 0);
    digitalWrite(D0, LOW);
    digitalWrite(LED_D0, LOW);
    modbusTCPServer.coilWrite(15, 1);
    modbusTCPServer.coilWrite(16, 1);
  }else{
    modbusTCPServer.coilWrite(16, 0);
    modbusTCPServer.coilWrite(12, 1);
    digitalWrite(D0, HIGH);
    digitalWrite(LED_D0, HIGH);
    modbusTCPServer.coilWrite(15, 0);
  }

  if(detectado == HIGH && sensor2 == LOW && estadobotao5 == HIGH){
    modbusTCPServer.coilWrite(15, 0);
    modbusTCPServer.coilWrite(14, 1);
    modbusTCPServer.coilWrite(16, 1);
  }else{
    modbusTCPServer.coilWrite(16, 0);
  }

  if(sensor2 == HIGH && estadobotao5 == HIGH) {
    modbusTCPServer.coilWrite(15, 1);
    modbusTCPServer.coilWrite(16, 1);
    modbusTCPServer.coilWrite(13, 0);
    digitalWrite(D1, LOW);
    digitalWrite(LED_D1, LOW);
  }else{
    modbusTCPServer.coilWrite(13, 1);
    digitalWrite(D1, HIGH);
    digitalWrite(LED_D1, HIGH);
  }

  if(sensor2 == HIGH && sensor3 == HIGH && estadobotao5 == HIGH){
    modbusTCPServer.coilWrite(16, 0);
    modbusTCPServer.coilWrite(15, 0);
    modbusTCPServer.coilWrite(14, 0);
  }


}else{
  modbusTCPServer.coilWrite(12, 0);
  digitalWrite(D0, LOW);
  digitalWrite(LED_D0, LOW);
  modbusTCPServer.coilWrite(13, 0);
  digitalWrite(D1, LOW);
  digitalWrite(LED_D1, LOW);
}







/**************************** MANUAL *********************************/


  estadoButton = digitalRead(A0);
  estadobotao1 = digitalRead(A1);
  estadobotao2 = digitalRead(A2);
  estadobotao3 = digitalRead(A3);
  estadobotao4 = digitalRead(A4);
  estadobotao5 = digitalRead(A5);
  
  if(estadoButton == LOW && estadobotao5 == HIGH){
      if (estadobotao1 == HIGH){
        modbusTCPServer.coilWrite(12, 1);
        Serial.println("ESTEIRA 1 LIGADA");
        print_status();
        digitalWrite(D0,HIGH);
        digitalWrite(LED_D0,HIGH);
      }else{
        modbusTCPServer.coilWrite(12, 0);
        Serial.println("ESTEIRA 2 DESLIGADA");
        print_status();
        digitalWrite(D0,LOW);
        digitalWrite(LED_D0,LOW);
      }
  
      if(estadobotao2 == HIGH && moveX == LOW && estadobotao5 == HIGH){
        modbusTCPServer.coilWrite(15, 1);
        modbusTCPServer.coilWrite(16, 1);
        print_status();
      }
  
      if(detectado == HIGH && moveX == LOW && estadobotao5 == HIGH){
        modbusTCPServer.coilWrite(15, 0);
      }
  
      if (detectado == HIGH && estadobotao3 == HIGH && estadobotao5 == HIGH){
        modbusTCPServer.coilWrite(14, 1);
      }
  
      if(moveX == HIGH && sensor2 == HIGH && sensor4 == HIGH && estadobotao2 != LOW && estadobotao5 == HIGH){
        modbusTCPServer.coilWrite(15, 1);
      }
  
      if(sensor3 == HIGH && estadobotao5 == HIGH){
        modbusTCPServer.coilWrite(14, 0);
        modbusTCPServer.coilWrite(15, 0);
        modbusTCPServer.coilWrite(16, 0);
      }
  
      if(estadobotao4 == HIGH && estadobotao5 == HIGH){
        modbusTCPServer.coilWrite(13, 1);
        digitalWrite(D1, HIGH);
        digitalWrite(LED_D1, HIGH);
      }else{
        modbusTCPServer.coilWrite(13, 0);
        digitalWrite(D1, LOW);
      }

    }
  

}




