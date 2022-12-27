/*************************************************************
  WARNING!
    It's very tricky to get it working. Please read this article:
    http://help.blynk.cc/hardware-and-libraries/arduino/esp8266-with-at-firmware

  You’ll need:
   - Blynk IoT app (download from App Store or Google Play)
   - Arduino Uno board
   - Decide how to connect to Blynk
     (USB, Ethernet, Wi-Fi, Bluetooth, ...)

  There is a bunch of great example sketches included to show you how to get
  started. Think of them as LEGO bricks  and combine them as you wish.
  For example, take the Ethernet Shield sketch and combine it with the
  Servo example, or choose a USB sketch and add a code from SendData
  example.
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID "TMPLQIvgFMQ4"
#define BLYNK_DEVICE_NAME "Smart Parking System"
#define BLYNK_AUTH_TOKEN "rp_8oOsq1vL3LvhrkaakajnSGRPTrhXM"


// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial
#define SS_PIN 10// sda pin rfid
#define RST_PIN 9


#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

#include<LiquidCrystal_I2C.h> //panggil library I2C
#include <RFID.h>
#include <SPI.h>


const int Ir_1 = 7;
const int Ir_2 = 4;
RFID rfid(SS_PIN,RST_PIN);

LiquidCrystal_I2C lcd(0x27, 16, 2); //inisialisasi library I2C ke variabel lcd
int Li          = 16;
int Lii         = 0; 
int serNum[5];
int i = 0;
bool access = true;
int Slot = 0;

int card[5] = {77, 57, 37, 217, 136};
int temp = 0;



char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Redmi Note 10";
char pass[] = "testing3542";

// Hardware Serial on Mega, Leonardo, Micro...
//#define EspSerial Serial1

// or Software Serial on Uno, Nano...
#include <SoftwareSerial.h>
SoftwareSerial EspSerial(2, 3); // RX, TX

// Your ESP8266 baud rate:
#define ESP8266_BAUD 115200

ESP8266 wifi(&EspSerial);

void setup()
{
  // Debug console
  Serial.begin(115200);

  // Set ESP8266 baud rate
  EspSerial.begin(ESP8266_BAUD);
  delay(10);

  //Blynk.begin(auth, wifi, ssid, pass);
  // You can also specify server:
  Blynk.begin(auth, wifi, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, wifi, ssid, pass, IPAddress(192,168,1,100), 8080);
  
  lcd.init(); //aktifkan LCD I2C
  lcd.begin(16,2);
  lcd.backlight();  //aktifkan lampu background LCD I2C
  
  Serial.begin(9600);
  pinMode(Ir_1, INPUT);
  pinMode(Ir_2, INPUT);
  SPI.begin();
  rfid.init();
}

void loop()
{
  Blynk.run();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
  CekSlot1();  
  readRFID();
  
}

void CekSlot1(){
  temp = 0;
  int Slot1 = digitalRead(Ir_1);
  int Slot2 = digitalRead(Ir_2);
  sensorinfra1(Slot1);
  sensorinfra1(Slot2);
  viewSlot();
}


void sensorinfra1(int read){
  
  switch(read){
    case 1 :
      temp+=1;
      break;
    default:
      break;
  }  
}

void viewSlot() {
  lcd.setCursor(0, 0);
  lcd.print("Sisa Slot : ");
  Blynk.virtualWrite(V1,"Sisa Slot: ", temp);
  lcd.print(temp);
  //lcd.printLeft()  
}

void readRFID() {
  CekSlot1();
  if(rfid.isCard())
  {
    if(rfid.readCardSerial())
    {
      Serial.print(rfid.serNum[0]);
      Serial.print(" ");
      Serial.print(rfid.serNum[1]);
      Serial.print(" ");
      Serial.print(rfid.serNum[2]);
      Serial.print(" ");
      Serial.print(rfid.serNum[3]);
      Serial.print(" ");
      Serial.print(rfid.serNum[4]);
      Serial.println(" ");
    } 

    if(rfid.serNum[i] == card[i])
      {
    
        //lcd.setCursor(0, 0);  
        //lcd.print(a);
       
        lcd.setCursor(0, 1);                                                
        lcd.print("Kartu Terdaftar"); 
        delay(800);
        //lcd.clear();
        access = true;
        if (access) {
         
          for (int i = 0 ; i <= 17; i++) 
          {
            //delay();
            lcd.setCursor(0, 1);
            lcd.print(Scroll_LCD_Left("Silahkan masuk"));
            delay(350);
          }
          lcd.clear ();
        }
      }
      else{
        lcd.setCursor(0, 0);
        lcd.print("Kartu Tidak Terdaftar");
        lcd.clear();
        delay(1000);
      }      
  }
  rfid.halt(); 
  delay(500);
}

String Scroll_LCD_Left(String StrDisplay){
  String result;
  String StrProcess = "                " + StrDisplay + "                ";
  result = StrProcess.substring(Li,Lii);
  Li++;
  Lii++;
  if (Li>StrProcess.length()){
    Li=16;
    Lii=0;
  }
  return result;
}
