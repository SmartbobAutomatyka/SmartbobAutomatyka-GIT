#ifndef SRC_SUPLA_CONTROL_EXT_MCP23017_H_
#define SRC_SUPLA_CONTROL_EXT_MCP23017_H_
/*
Dependency: https://github.com/RobTillaart/MCP23017_RT
use library manager to install it
*/

#include <Wire.h> // set Wire.begin(SDA, SCL)
#include <Adafruit_MCP23X17.h>
#include <supla/io.h>
#include <supla/log_wrapper.h>

namespace Supla {
namespace Control {
	
class ExtMCP23017 : public Supla::Io {
 public:
  ExtMCP23017(uint8_t address = 0x20)
      : ioExpander(address), address(address), Supla::Io(false) {
    if (!ioExpander.begin_I2C()) {
      SUPLA_LOG_DEBUG("Unable to find MCP23017");
    } else {
      Serial.print("MCP23017 is connected at address: 0x");
      Serial.println(address, HEX);
      isConnected = true;
    }
  }

  void customPinMode(int channelNumber, uint8_t pin, uint8_t mode) override {
    if (isConnected) {
	  ioExpander.pinMode(pin, mode);
       
    }
  }
  void customDigitalWrite(int channelNumber, uint8_t pin,
                                                        uint8_t val) override {
    if (isConnected) {
	  ioExpander.digitalWrite(pin, val);
    }
  }
  int customDigitalRead(int channelNumber, uint8_t pin) override {
      return isConnected ? ioExpander.digitalRead(pin) : 0;
  }
  unsigned int customPulseIn(int channelNumber, uint8_t pin, uint8_t value,
                                              uint64_t timeoutMicro) override {
    // add implementation or leave empty
    return 0;
  }
  void customAnalogWrite(int channelNumber, uint8_t pin, int val) override {
    // add implementation
  }

  int customAnalogRead(int channelNumber, uint8_t pin) override {
    // add implementation
    return 0;
  }
 
 protected:
  ::Adafruit_MCP23X17 ioExpander;
  uint8_t address;
  bool isConnected = false;
};

};  // namespace Control
};  // namespace Supla

#endif  // SRC_SUPLA_CONTROL_EXT_MCP23017_H_