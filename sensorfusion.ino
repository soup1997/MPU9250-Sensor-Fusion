#include "MPU9250.h"
#include "string.h"

// an MPU9250 object with the MPU-9250 sensor on I2C bus 0 with address 0x68
MPU9250 IMU(Wire, 0x68);

int status = 0;
int max_k = 100;
static int k = 1;

typedef struct total_data {
  float accel[3]; // raw value
  float gyro[3]; // raw value
  float mag[3]; // raw value

  float prev_accel[3]; // raw value
  float prev_gyro[3]; // raw value
  float prev_mag[3]; // raw value
}imu_data;

imu_data *imu_main;

void get_data(imu_data *imu)
{
  imu->accel[0] = IMU.getAccelX_mss();
  imu->accel[1] = IMU.getAccelY_mss();
  imu->accel[2] = IMU.getAccelZ_mss();

  imu->gyro[0] = IMU.getGyroX_rads();
  imu->gyro[1] = IMU.getGyroY_rads();
  imu->gyro[2] = IMU.getGyroZ_rads();

  imu->mag[0] = IMU.getMagX_uT();
  imu->mag[1] = IMU.getMagY_uT();
  imu->mag[2] = IMU.getMagZ_uT();
}

void average_filter(imu_data *imu)
{
  float alpha = (k-1)/k;
  float accel_avg = 0;
  float gyro_avg = 0;
  float mag_avg = 0;

  for(int i=0; i<3; i++)
  {
    accel_avg = alpha * imu->prev_accel[i] * (1-alpha) * imu->accel[i];
    gyro_avg = alpha * imu->prev_gyro[i] * (1-alpha) * imu->gyro[i];
    mag_avg = alpha * imu->prev_mag[i] * (1-alpha) * imu->mag[i];

    imu->prev_accel[i] = accel_avg;
    imu->prev_gyro[i] = gyro_avg;
    imu->prev_mag[i] = mag_avg;
  }
}

void lowpass_filter()
{
}

void serial_print(imu_data *imu)
{
  for(int i=0; i<3; i++)
  {
    Serial.print(imu->prev_accel[i], 6);
    Serial.print(imu->prev_gyro[i], 6);
    Serial.print(imu->prev_mag[i], 6);
  }
}

void setup() {
  Serial.begin(115200);
  status = IMU.begin(); // set I2C Register

  if (status < 0) { // detect error
    Serial.println("IMU initialization unsuccessful");
    Serial.println("Check IMU wiring or try cycling power");
    Serial.print("Status: ");
    Serial.println(status);
  }

  else
  {
    memset(imu_main->accel, 0, sizeof(float) * 3);
    memset(imu_main->gyro, 0, sizeof(float) * 3);
    memset(imu_main->mag, 0, sizeof(float) * 3);
    memset(imu_main->prev_accel, 0, sizeof(float) * 3);
    memset(imu_main->prev_gyro, 0, sizeof(float) * 3);
    memset(imu_main->prev_mag, 0, sizeof(float) * 3);
  }
}

void loop() {
  IMU.readSensor(); // read raw data
  get_data(imu_main);

  while(k <= max_k) // initialization
  {
    average_filter(imu_main);
    k += 1;
  }
  serial_print(imu_main);
  delay(10);
}