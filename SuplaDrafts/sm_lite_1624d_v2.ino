
#include <SuplaDevice.h>
#include <supla/control/rgbw_leds.h>
#include <supla/control/dimmer_leds.h>
//#include <supla/network/esp_wifi.h>
#include <supla/control/button.h>
#include <supla/sensor/binary.h>
#include <supla/sensor/DHT.h>
#include <supla/control/relay.h>
#include <supla/control/virtual_relay.h>
#include <supla/control/action_trigger.h>
#include <supla/device/status_led.h>
#include <supla/storage/littlefs_config.h>
#include <supla/network/esp_web_server.h>
#include <supla/network/html/device_info.h>
#include <supla/network/html/protocol_parameters.h>
#include <supla/network/html/status_led_parameters.h>
#include <supla/device/supla_ca_cert.h>
//#include <supla/network/html/wifi_parameters.h>
#include <supla/device/supla_ca_cert.h>
#include <supla/events.h>


#include <supla/control/EXT_PCA9685.h>
#include <supla/control/ExpanderMCP23017.h>
#include <supla/network/esp32eth.h>
 Supla::ESPETH Eth(1);  // uint_t ETH_ADDR = I²C-address of Ethernet PHY (0 or 1)
#define STATUS_LED_GPIO 12
#define I2C_SDA 5
#define I2C_SCL 15
#define ETH_MDC_PIN 23 
#define ETH_MDIO_PIN 18      
#define ETH_POWER_PIN 16
#define DHTPIN 32 
#define DHTTYPE DHT22      
Supla::Control::ExtPCA9685 *pca1 = nullptr;
Supla::Control::ExtPCA9685 *pca2 = nullptr;
Supla::Control::ExpanderMCP23017 *mcp1 = nullptr;
uint32_t lastMillis= 0; //Save the last millis()

//OLD
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


//OLDF
#include <supla/storage/eeprom.h>
Supla::Eeprom eeprom;
 //#include <supla/storage/fram_spi.h>
 //Supla::FramSpi fram(STORAGE_OFFSET);
//WLACZENIE WIFI
//Supla::ESPWifi wifi;
Supla::LittleFsConfig configSupla;

Supla::Device::StatusLed statusLed(STATUS_LED_GPIO, false); // inverted state
Supla::EspWebServer suplaServer;


// HTML www component (they appear in sections according to creation
// sequence)
Supla::Html::DeviceInfo htmlDeviceInfo(&SuplaDevice);
//Supla::Html::WifiParameters htmlWifi;
Supla::Html::ProtocolParameters htmlProto;
Supla::Html::StatusLedParameters htmlStatusLed;


void setup() {
Serial.begin(115200);
Wire.begin(I2C_SDA, I2C_SCL);
Wire.setClock(400000);

/*
 if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
*/
  // Replace the falowing GUID with value that you can retrieve from https://www.supla.org/arduino/get-guid
 // char GUID[SUPLA_GUID_SIZE] = {0x62,0xAA,0xDB,0x5B,0x7C,0xC7,0x6D,0x03,0x86,0x7E,0xD8,0x2E,0x04,0xEE,0x24,0xE8};

  // Replace the following AUTHKEY with value that you can retrieve from: https://www.supla.org/arduino/get-authkey
  //char AUTHKEY[SUPLA_AUTHKEY_SIZE] = {0x72,0x17,0x5F,0x52,0x3E,0x53,0xB2,0x69,0x16,0x3C,0xFA,0x35,0x0F,0xD6,0xCA,0x31};

  /*
     Having your device already registered at cloud.supla.org,
     you want to change CHANNEL sequence or remove any of them,
     then you must also remove the device itself from cloud.supla.org.
     Otherwise you will get "Channel conflict!" error.
  */

  pca1 = new Supla::Control::ExtPCA9685(0x40);
  pca2 = new Supla::Control::ExtPCA9685(0x43);
  mcp1 = new Supla::Control::ExpanderMCP23017(&Wire,0x21);
  pca1->setPWMFrequency(500);
  pca2->setPWMFrequency(500);
 
 
//buttons
 auto btn0 = new Supla::Control::Button(mcp1, 0, false,true);
 auto btn1 = new Supla::Control::Button(mcp1, 1, false,true);
 auto btn2 = new Supla::Control::Button(mcp1, 2, false,true);
 auto btn3 = new Supla::Control::Button(mcp1, 3, false,true);
 auto btn4 = new Supla::Control::Button(mcp1, 4, false,true);
 auto btn5 = new Supla::Control::Button(mcp1, 5, false,true);
 auto btn6 = new Supla::Control::Button(mcp1, 6, false,true);
 auto btn7 = new Supla::Control::Button(mcp1, 7, false,true);
 auto btn8 = new Supla::Control::Button(mcp1, 8, false,true);
 auto btn9 = new Supla::Control::Button(mcp1, 9, false,true);
 auto btn10 = new Supla::Control::Button(mcp1, 10, false,true);
 auto btn11 = new Supla::Control::Button(mcp1, 11, false,true);
 auto btn12 = new Supla::Control::Button(mcp1, 12, false,true);
 auto btn13 = new Supla::Control::Button(mcp1, 13, false,true);
 auto btn14 = new Supla::Control::Button(mcp1, 14, false,true);
 auto btn15 = new Supla::Control::Button(mcp1, 15, false,true);

  // CHANNEL0 - RGB controller and dimmer (RGBW)
 auto led0 = new Supla::Control::DimmerLeds(pca1,4); //1
 auto led1 = new Supla::Control::DimmerLeds(pca1,5);//2
 auto led2 = new Supla::Control::DimmerLeds(pca1,6);//3
 auto led3 = new Supla::Control::DimmerLeds(pca1,7);//4
 auto led4 = new Supla::Control::DimmerLeds(pca1,8); //5
 auto led5 = new Supla::Control::DimmerLeds(pca1,9);//6
 auto led6 = new Supla::Control::DimmerLeds(pca1,10);//7
 auto led7 = new Supla::Control::DimmerLeds(pca1,11);//8
 auto led8 = new Supla::Control::DimmerLeds(pca1,12);//9
 auto led9 = new Supla::Control::DimmerLeds(pca1,13);//10
 auto led10 = new Supla::Control::DimmerLeds(pca1,14);//11
 auto led11 = new Supla::Control::DimmerLeds(pca1,15);//12
 auto led12 = new Supla::Control::DimmerLeds(pca2,4);//13
 auto led13 = new Supla::Control::DimmerLeds(pca2,5);//14
 auto led14 = new Supla::Control::DimmerLeds(pca2,6);//15
 auto led15 = new Supla::Control::DimmerLeds(pca2,7);//16
 //auto led16 = new Supla::Control::DimmerLeds(pca2,0);//17
 //auto led17 = new Supla::Control::DimmerLeds(pca2,1);//18
 //auto led18 = new Supla::Control::DimmerLeds(pca2,2);//19
 //auto led19 = new Supla::Control::DimmerLeds(pca2,3);//20
 //auto led20 = new Supla::Control::DimmerLeds(pca1,0);//21
 //auto led21 = new Supla::Control::DimmerLeds(pca1,1);//22
 //auto led22 = new Supla::Control::DimmerLeds(pca1,2);//23
 //auto led23 = new Supla::Control::DimmerLeds(pca1,3);//24

//nadmiarowe
 auto led24 = new Supla::Control::DimmerLeds(pca2,8);//13
 auto led25 = new Supla::Control::DimmerLeds(pca2,9);//14
 auto led26 = new Supla::Control::DimmerLeds(pca2,10);//15
 auto led27 = new Supla::Control::DimmerLeds(pca2,11);//16
 auto led28 = new Supla::Control::DimmerLeds(pca2,12);//17
 auto led29 = new Supla::Control::DimmerLeds(pca2,13);//18
 auto led30 = new Supla::Control::DimmerLeds(pca2,14);//19
 auto led31 = new Supla::Control::DimmerLeds(pca2,15);//20



//Virtual Relays
 auto relay0 = new Supla::Control::VirtualRelay();
 auto relay1 = new Supla::Control::VirtualRelay();
 auto relay2 = new Supla::Control::VirtualRelay();
 auto relay3 = new Supla::Control::VirtualRelay();
 auto relay4 = new Supla::Control::VirtualRelay();
 auto relay5 = new Supla::Control::VirtualRelay();
 auto relay6 = new Supla::Control::VirtualRelay();
 auto relay7 = new Supla::Control::VirtualRelay();
 auto relay8 =new Supla::Control::VirtualRelay();
 auto relay9 = new Supla::Control::VirtualRelay();
 auto relay10 = new Supla::Control::VirtualRelay();
 auto relay11 = new Supla::Control::VirtualRelay();
 auto relay12 = new Supla::Control::VirtualRelay();
 auto relay13 = new Supla::Control::VirtualRelay();
 auto relay14 = new Supla::Control::VirtualRelay();
 auto relay15 = new Supla::Control::VirtualRelay();
//

  btn0->setMulticlickTime(200);
  btn0->setHoldTime(400);
  btn0->repeatOnHoldEvery(200);
  btn0->addAction(Supla::TOGGLE, relay0, Supla::ON_PRESS);

  btn1->setMulticlickTime(200);
  btn1->setHoldTime(400);
  btn1->repeatOnHoldEvery(200);
  btn1->addAction(Supla::TOGGLE, relay1, Supla::ON_PRESS);
  
  btn2->setMulticlickTime(200);
  btn2->setHoldTime(400);
  btn2->repeatOnHoldEvery(200);
  btn2->addAction(Supla::TOGGLE, relay2, Supla::ON_PRESS);
  
  btn3->setMulticlickTime(200);
  btn3->setHoldTime(400);
  btn3->repeatOnHoldEvery(200);
  btn3->addAction(Supla::TOGGLE, relay3, Supla::ON_PRESS);
  
  btn4->setMulticlickTime(200);
  btn4->setHoldTime(400);
  btn4->repeatOnHoldEvery(200);
  btn4->addAction(Supla::TOGGLE, relay4, Supla::ON_PRESS);

  btn5->setMulticlickTime(200);
  btn5->setHoldTime(400);
  btn5->repeatOnHoldEvery(200);
  btn5->addAction(Supla::TOGGLE, relay5, Supla::ON_PRESS);

  btn6->setMulticlickTime(200);
  btn6->setHoldTime(400);
  btn6->repeatOnHoldEvery(200);
  btn6->addAction(Supla::TOGGLE, relay6, Supla::ON_PRESS);

  btn7->setMulticlickTime(200);
  btn7->setHoldTime(400);
  btn7->repeatOnHoldEvery(200);
  btn7->addAction(Supla::TOGGLE, relay7, Supla::ON_PRESS);
btn8->setMulticlickTime(200);
btn8->setHoldTime(400);
btn8->repeatOnHoldEvery(200);
btn8->addAction(Supla::TOGGLE, relay8, Supla::ON_PRESS);

btn9->setMulticlickTime(200);
btn9->setHoldTime(400);
btn9->repeatOnHoldEvery(200);
btn9->addAction(Supla::TOGGLE, relay9, Supla::ON_PRESS);

btn10->setMulticlickTime(200);
btn10->setHoldTime(400);
btn10->repeatOnHoldEvery(200);
btn10->addAction(Supla::TOGGLE, relay10, Supla::ON_PRESS);

btn11->setMulticlickTime(200);
btn11->setHoldTime(400);
btn11->repeatOnHoldEvery(200);
btn11->addAction(Supla::TOGGLE, relay11, Supla::ON_PRESS);

btn12->setMulticlickTime(200);
btn12->setHoldTime(400);
btn12->repeatOnHoldEvery(200);
btn12->addAction(Supla::TOGGLE, relay12, Supla::ON_PRESS);

btn13->setMulticlickTime(200);
btn13->setHoldTime(400);
btn13->repeatOnHoldEvery(200);
btn13->addAction(Supla::TOGGLE, relay13, Supla::ON_PRESS);

btn14->setMulticlickTime(200);
btn14->setHoldTime(400);
btn14->repeatOnHoldEvery(200);
btn14->addAction(Supla::TOGGLE, relay14, Supla::ON_PRESS);

btn15->setMulticlickTime(200);
btn15->setHoldTime(400);
btn15->repeatOnHoldEvery(200);
btn15->addAction(Supla::TOGGLE, relay15, Supla::ON_PRESS);


  // configure defualt Supla CA certificate
  SuplaDevice.setSuplaCACert(suplaCACert);
  SuplaDevice.setSupla3rdPartyCACert(supla3rdCACert);
  SuplaDevice.setName("SM-LITE-1624D");
  SuplaDevice.begin();
//OLED
if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

display.clearDisplay();
display.setTextSize(1);
display.setTextColor(WHITE); 
display.setCursor(4,4);
display.println("SM-LITE-1624D");
display.println("Uruchamianie.");
display.display();
//OLED
}

void loop() {
 if (millis() - lastMillis >= 2*10*1000UL) 
  {
   lastMillis = millis();  //get ready for the next iteration
//DisplayOled(ETH.localIP().toString(),ETH.gatewayIP().toString(),ETH.macAddress());  
display.clearDisplay();
display.setTextSize(1);
display.setTextColor(WHITE); 
display.setCursor(0,0);
display.println("SM-LITE-1624D");
display.println("Connected");
display.print("IP:");
display.println(ETH.localIP());
display.display();
 }

SuplaDevice.iterate();
}