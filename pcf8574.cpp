#include <pcf8574.h>

const i2c_master_bus_config_t Pcf8574::DEFAULT_BUS_CONFIG = {
    .i2c_port = I2C_NUM_0,
    .sda_io_num = GPIO_NUM_NC,
    .scl_io_num = GPIO_NUM_NC,
    .clk_source = I2C_CLK_SRC_DEFAULT,
    .glitch_ignore_cnt = 7,
    .flags = {
        .enable_internal_pullup = true,
    }};

i2c_device_config_t mkDeviceConfig(Pcf8574::Address addr)
{
  uint16_t address = mkAddress(addr);
  i2c_device_config_t dev_cfg = {
      .dev_addr_length = I2C_ADDR_BIT_LEN_7,
      .device_address = address,
      .scl_speed_hz = Pcf8574::MAX_RATE,
  };
}

uint16_t mkAddress(Pcf8574::Address addr)
{
  return Pcf8574::BASE_ADDRESS_7 | (addr.a2 << 2) | (addr.a1 << 1) | addr.a0;
}

Pcf8574::Pcf8574(gpio_num_t sda_pin, gpio_num_t scl_pin, Pcf8574::Address addr, gpio_num_t intr_pin, i2c_master_bus_config_t bus_cfg)
{
  bus_cfg.sda_io_num = sda_pin;
  bus_cfg.scl_io_num = scl_pin;
  ESP_ERROR_CHECK(i2c_new_master_bus(&bus_cfg, &_bus_handle));

  _sda_pin = sda_pin;
  _scl_pin = scl_pin;
  _bus_cfg = bus_cfg;
  _intr_pin = intr_pin;
  _dev_cfg = mkDeviceConfig(addr);

  ESP_ERROR_CHECK(i2c_master_bus_add_device(_bus_handle, &_dev_cfg, &_dev_handle));

  _exclusive_bus = true;
}

Pcf8574::Pcf8574(i2c_master_bus_handle_t handle, Address addr, gpio_num_t intr_pin)
{
  _bus_handle = handle;
  _dev_cfg = mkDeviceConfig(addr);
  ESP_ERROR_CHECK(i2c_master_bus_add_device(_bus_handle, &_dev_cfg, &_dev_handle));

  _exclusive_bus = false;
};

Pcf8574::~Pcf8574()
{
  ESP_ERROR_CHECK(i2c_master_bus_rm_device(_dev_handle));

  if (_exclusive_bus)
  {
    ESP_ERROR_CHECK(i2c_del_master_bus(_bus_handle));
  }
}

void Pcf8574::write(uint8_t *data, int timeout)
{
  ESP_ERROR_CHECK(i2c_master_transmit(_dev_handle, data, 1, timeout));
}

void Pcf8574::read(uint8_t *data, int timeout)
{
  ESP_ERROR_CHECK(i2c_master_receive(_dev_handle, data, 1, timeout));
}
