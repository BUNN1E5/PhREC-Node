#include "sensors.h"
#include <Wire.h>

class MT6701_I2C : public Sensor{
public:
    MT6701_I2C(int sensor_id, int group_id);
    SensorDataResponse getData() override;
    int id;
    int group_id;
};


const int slave_id = 0b0000110;
const int angle_data_registers[] = {
    0x03,
    0x04
};

const PinName mt6701_i2c_pins[] = {
    PB_6,  // SCL
    PB_7   // SDA
};

MT6701_I2C::MT6701_I2C(int sensor_id, int group_id) {
    this->id = sensor_id;
    this->group_id = group_id;
    Wire.begin();
}

SensorDataResponse MT6701_I2C::getData() {
    Wire.beginTransmission(slave_id);
    Wire.endTransmission(false);

    Wire.requestFrom(slave_id, angle_data_registers[0]);
    byte a = Wire.read();
    Wire.requestFrom(slave_id, angle_data_registers[1]);
    byte b = Wire.read();

    Message msg;
    msg.sensorDataResponse.message_id = -13; // Defined in messages.md
    msg.sensorDataResponse.sensor_data = (a << 6) | (b >> 2); // Combine the two bytes of angle data
    msg.sensorDataResponse.crc8 = crc8(msg.body.data, sizeof(msg.body));
    return msg.sensorDataResponse;
}