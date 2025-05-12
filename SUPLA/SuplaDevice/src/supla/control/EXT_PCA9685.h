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

#ifndef SRC_SUPLA_CONTROL_EXT_PCA9685_H_
#define SRC_SUPLA_CONTROL_EXT_PCA9685_H_

/*
Dependency: https://github.com/NachtRaveVL/PCA9685-Arduino/
use library manager to install it
*/

#include <PCA9685.h>
#include <supla/io.h>
#include <supla/log_wrapper.h>

namespace Supla {
namespace Control {

class ExtPCA9685 : public Supla::Io {
 public:
  explicit ExtPCA9685(uint8_t address = 0x40)
      : pca(address), Supla::Io(false) {
    //if (!pca.init()) {
//      SUPLA_LOG_DEBUG("Unable to find PCA9685");
  //  } else {
      //SUPLA_LOG_DEBUG("PCA9685 is connected at address: 0x%x, "                        "with PWM freq: %d Hz", address );
    //  
    //}

    pca.init();
    isConnected = true;
  }

  void customPinMode(int channelNumber, uint8_t pin, uint8_t mode) override {
  }
  void customDigitalWrite(int channelNumber, uint8_t pin,
                                                      uint8_t val) override {
    if (isConnected) {
      if(val == 0) pca.setChannelOff(pin);
      if(val == 1) pca.setChannelOn(pin);

    }
  }
  int customDigitalRead(int channelNumber, uint8_t pin) override {
  if(pca.getChannelPWM(pin) > 0){
return 1;
   }else{
return 0;
   }
   
  
  }
  unsigned int customPulseIn(int channelNumber, uint8_t pin, uint8_t value,
                                            uint64_t timeoutMicro) override {
    return 0;
  }
  void customAnalogWrite(int channelNumber, uint8_t pin, int val) override {
    if (isConnected) {
      val = map(val, 0, 1023, 0, 4095);
      pca.setChannelPWM(pin, val);
    }
  }

  int customAnalogRead(int channelNumber, uint8_t pin) override {
    return 0;
  }

  // Default frequency: 200 Hz
  void setPWMFrequency(uint16_t frequency_) {
    if (isConnected) {
      pca.setPWMFrequency(frequency_);
      SUPLA_LOG_DEBUG("PCA9685 - setting PWM frequency to: %d Hz",
                                                  frequency_);
    }
  }

 protected:
  ::PCA9685 pca;
  bool isConnected = false;
};

};  // namespace Control
};  // namespace Supla

#endif  // SRC_SUPLA_CONTROL_EXT_PCA9685_H_
