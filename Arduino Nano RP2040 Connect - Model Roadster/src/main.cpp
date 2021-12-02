#include <ArduinoBLE.h>

BLEService MotorService("20B10000-E8F2-537E-4F6C-D104768A1214"); // BLE Motor Service
BLEService SteeringService("19B10000-E8F2-537E-4F6C-D104768A1214"); // BLE Steering Service
// BLE LED Switch Characteristic - custom 128-bit UUID, read and writable by central
BLEByteCharacteristic MotorCharacteristic("20B10000-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify | BLEWrite);
BLEByteCharacteristic SteeringCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify | BLEWrite);


void setup() {
  Serial.begin(9600);
  
  //PINOUT SETUP
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
  }
  BLE.setDeviceName("RP2040");
  BLE.setLocalName("RP2040.loc");
  BLE.setAdvertisedService(MotorService);
  BLE.setAdvertisedService(SteeringService);

  // add the characteristic to the service
  MotorService.addCharacteristic(MotorCharacteristic);
  SteeringService.addCharacteristic(SteeringCharacteristic);

  // add service
  BLE.addService(SteeringService);
  BLE.addService(MotorService);
  // start advertising
  BLE.advertise();
  Serial.println("BLE Peripheral, waiting for connections....");

}
void loop() {
  //Variables
  int previousMotorVal = 0;
  int previousSteeringVal = 0;
  // listen for BLE peripherals to connect:
  BLEDevice central = BLE.central();
  // if a central is connected to peripheral:
  if (central) {
    Serial.println("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());
    // while the central is still connected to peripheral:
    while (central.connected()){
      int motorVal = MotorCharacteristic.value();
      int steeringVal = SteeringCharacteristic.value();
      if(motorVal != previousMotorVal || steeringVal != previousSteeringVal){
        if (motorVal ==1 ){
          digitalWrite(17, HIGH);
        }
        if (motorVal == 0){
          digitalWrite(17, LOW);
        }
        if (steeringVal == 1){
          digitalWrite(12, HIGH);
        }
        if (steeringVal == 0){
          digitalWrite(12, LOW);
        }
        Serial.print("Moror");
        Serial.println(motorVal);
        Serial.print("Steering");
        Serial.println(steeringVal);
        previousMotorVal = motorVal;
        previousSteeringVal = steeringVal;
        
      }
      delay(200);
      //digitalWrite(13, LOW);
    }  
  }else{
    
    Serial.println("Not Connected");
    digitalWrite(LED_BUILTIN, HIGH);
    delay(5000);
    digitalWrite(LED_BUILTIN, LOW);
  }
  delay(2000);
}