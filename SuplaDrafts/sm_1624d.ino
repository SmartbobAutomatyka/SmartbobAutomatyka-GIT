/*
  Copyright (C) AC SOFTWARE SP. Z O.O.

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
//
#include <SuplaDevice.h>
#include <supla/control/rgbw_leds.h>
#include <supla/control/dimmer_leds.h>
//#include <supla/network/esp_wifi.h>
#include <supla/control/button.h>
#include <supla/sensor/binary.h>
#include <supla/sensor/DHT.h>
#include <supla/control/relay.h>
#include <supla/control/action_trigger.h>
#include <supla/device/status_led.h>
#include <supla/storage/littlefs_config.h>
#include <supla/network/esp_web_server.h>
#include <supla/network/html/device_info.h>
#include <supla/network/html/protocol_parameters.h>
#include <supla/network/html/status_led_parameters.h>
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



unsigned long lastMillis;
#include "DEV_OLED.h"
#include <supla/storage/eeprom.h>
Supla::Eeprom eeprom;
 #include <supla/storage/fram_spi.h>
 Supla::FramSpi fram(STORAGE_OFFSET);
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
  auto sensor7 = new Supla::Sensor::Binary(mcp1, 15, false,true); // PIN 15 kontaktron - czujnik otwarcia drzwi
  auto sensor1 = new Supla::Sensor::DHT(DHTPIN, DHTTYPE);//DHT czujnik temperatury
 //relays 0-1 właczanie 8 kanałów
 auto relay0 = new Supla::Control::Relay(pca1, 0, true);
 auto relay1 = new Supla::Control::Relay(pca1, 1, true);
 auto relay2 = new Supla::Control::Relay(pca1, 2, true);
 auto relay3 = new Supla::Control::Relay(pca1, 3, true);
 auto relay4 = new Supla::Control::Relay(pca1, 4, true);
 auto relay5 = new Supla::Control::Relay(pca1, 5, true);
 auto relay6 = new Supla::Control::Relay(pca1, 6, true);
 auto relay7 = new Supla::Control::Relay(pca1, 7, true);
 //butons 15 szt
 auto button0 = new Supla::Control::Button(mcp1, 0, false,true);
 auto button1 = new Supla::Control::Button(mcp1, 1, false,true);
 auto button2 = new Supla::Control::Button(mcp1, 2, false,true);
 auto button3 = new Supla::Control::Button(mcp1, 3, false,true);
 auto button4 = new Supla::Control::Button(mcp1, 4, false,true);
 auto button5 = new Supla::Control::Button(mcp1, 5, false,true);
 auto button6 = new Supla::Control::Button(mcp1, 6, false,true);
 auto button7 = new Supla::Control::Button(mcp1, 7, false,true);
 auto button8 = new Supla::Control::Button(mcp1, 8, false,true);
 auto button9 = new Supla::Control::Button(mcp1, 9, false,true);
 auto button10 = new Supla::Control::Button(mcp1, 10, false,true);
 auto button11 = new Supla::Control::Button(mcp1, 11, false,true);
 auto button12 = new Supla::Control::Button(mcp1, 12, false,true);
 auto button13 = new Supla::Control::Button(mcp1, 13, false,true);
 auto button14 = new Supla::Control::Button(mcp1, 14, false,true);
 // bez ostatniego który jest kontaktronem 


  // CHANNEL - dimmer 8 kanałów mono.
  auto led0 = new Supla::Control::DimmerLeds(pca1,8);
  auto led1 = new Supla::Control::DimmerLeds(pca1,9);
  auto led2 = new Supla::Control::DimmerLeds(pca1,10);
  auto led3 = new Supla::Control::DimmerLeds(pca1,11);
  auto led4 = new Supla::Control::DimmerLeds(pca1,12);
  auto led5 = new Supla::Control::DimmerLeds(pca1,13);
  auto led6 = new Supla::Control::DimmerLeds(pca1,14);
  auto led7 = new Supla::Control::DimmerLeds(pca1,15);
  //RGBW - na drugim PCA
  auto rgbww = new Supla::Control::RGBWLeds(pca2,0,1,2,3);
  //auto rgbww1 = new Supla::Control::RGBWLeds(pca2,4,5,6,7);

//Akce do przycisków do dowolnej rozbudowy. 



  button0->setMulticlickTime(200);
  button0->setHoldTime(400);
  button0->repeatOnHoldEvery(200);
  button0->addAction(Supla::TOGGLE, led0, Supla::ON_PRESS);

  button1->setMulticlickTime(200);
  button1->setHoldTime(400);
  button1->repeatOnHoldEvery(200);
  button1->addAction(Supla::TOGGLE, led1, Supla::ON_PRESS);
  
  button2->setMulticlickTime(200);
  button2->setHoldTime(400);
  button2->repeatOnHoldEvery(200);
  button2->addAction(Supla::TOGGLE, led2, Supla::ON_PRESS);
  
  button3->setMulticlickTime(200);
  button3->setHoldTime(400);
  button3->repeatOnHoldEvery(200);
  button3->addAction(Supla::TOGGLE, led3, Supla::ON_PRESS);
  
  button4->setMulticlickTime(200);
  button4->setHoldTime(400);
  button4->repeatOnHoldEvery(200);
  button4->addAction(Supla::TOGGLE, relay0, Supla::ON_PRESS);

  button5->setMulticlickTime(200);
  button5->setHoldTime(400);
  button5->repeatOnHoldEvery(200);
  button5->addAction(Supla::TOGGLE, relay1, Supla::ON_PRESS);

  button6->setMulticlickTime(200);
  button6->setHoldTime(400);
  button6->repeatOnHoldEvery(200);
  button6->addAction(Supla::TOGGLE, relay2, Supla::ON_PRESS);

  button7->setMulticlickTime(200);
  button7->setHoldTime(400);
  button7->repeatOnHoldEvery(200);
  button7->addAction(Supla::TOGGLE, relay3, Supla::ON_PRESS);

  
  /*
     SuplaDevice Initialization.
     Server address is available at https://cloud.supla.org
     If you do not have an account, you can create it at
     https://cloud.supla.org/account/create SUPLA and SUPLA CLOUD are free of
     charge

  */

  // configure defualt Supla CA certificate
  SuplaDevice.setSuplaCACert(suplaCACert);
  SuplaDevice.setSupla3rdPartyCACert(supla3rdCACert);
  SuplaDevice.setName("SM-LITE-1624D");
  SuplaDevice.begin();
}

void loop() {

SuplaDevice.iterate();
}
