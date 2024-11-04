#if !defined(_PCF8574_H)
#define _PCF8574_H

#include <stdint.h>
#include <driver/i2c_master.h>

class Pcf8574
{
private:
  gpio_num_t _sda_pin;
  gpio_num_t _scl_pin;
  gpio_num_t _intr_pin;

  i2c_master_bus_config_t _bus_cfg;
  i2c_master_bus_handle_t _bus_handle;
  i2c_device_config_t _dev_cfg;
  i2c_master_dev_handle_t _dev_handle;

  bool _exclusive_bus;

public:
  struct Address
  {
    bool a0;
    bool a1;
    bool a2;
  };

  enum DataDirection
  {
    DataWrite = 0,
    DataRead = 1
  };

  static const uint8_t BASE_ADDRESS_7 = 0b0100000;
  static const uint32_t MAX_RATE = 100000; // 100 kHz

  static const i2c_master_bus_config_t DEFAULT_BUS_CONFIG;

  // Creates new I2C master bus
  Pcf8574(gpio_num_t sda_pin, gpio_num_t scl_pin, Address addr, gpio_num_t intr_pin = GPIO_NUM_NC, i2c_master_bus_config_t bus_cfg = DEFAULT_BUS_CONFIG);
  // Use existing I2C master bus
  Pcf8574(i2c_master_bus_handle_t handle, Address addr, gpio_num_t intr_pin = GPIO_NUM_NC);
  ~Pcf8574();

  void write(uint8_t *data, int timeout = -1);
  void read(uint8_t *data, int timeout = -1);
};

#endif // _PCF8574_H
