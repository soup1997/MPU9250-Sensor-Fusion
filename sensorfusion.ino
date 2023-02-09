#include "MPU9250.h"
#include "string.h"

typedef struct {
  float accel[3]; // raw accel value
  float gyro[3]; // raw gyro value
  float mag[3]; // raw mag value

  float prev_accel[3]; // previous accel value
  float prev_gyro[3]; // raw gyro value
  float prev_mag[3]; // raw mag value

  float filtered_accel[3]; // filtered accel value
  float filtered_gyro[3]; // filtered gyro value
  float filtered_mag[3]; // filtered mag value
}imu_data;

// an MPU9250 object with the MPU-9250 sensor on I2C bus 0 with address 0x68
MPU9250 IMU(Wire, 0x68);
imu_data mpu9250;
int status;
char* print_string[3] = {"Accel, Gyro, Mag X", "Accel, Gyro, Mag Y", "Accel, Gyro, Mag Z"};
static float alpha = 0.8;

void get_raw_data(imu_data *imu)
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

void lowpass_filter(imu_data *imu)
{
  for(int i = 0; i < 3; i++)
  {
    imu->filtered_accel[i] = (alpha * imu->prev_accel[i]) + ((1.0 - alpha) * imu->accel[i]); 
    imu->filtered_gyro[i] = (alpha * imu->prev_gyro[i]) + ((1.0 - alpha) * imu->gyro[i]); 
    imu->filtered_mag[i] = (alpha * imu->prev_mag[i]) + ((1.0 - alpha) * imu->mag[i]); 
  }

  for(int j = 0; j < 3; j++)
  {
    imu->prev_accel[j] = imu->filtered_accel[j]; 
    imu->prev_gyro[j] = imu->filtered_gyro[j]; 
    imu->prev_mag[j] = imu->filtered_mag[j]; 
  }
}

void serial_print(imu_data *imu)
{
  for(int i = 0; i < 3; i++)
  {
    Serial.println(print_string[i]);

    Serial.print(imu->filtered_accel[i], 6);
    Serial.print("\t");
    Serial.print(imu->filtered_gyro[i], 6);
    Serial.print("\t");
    Serial.print(imu->filtered_mag[i], 6);
    Serial.print("\t");
    Serial.println("\n");
  }
}

void set_memory(imu_data *imu)
{
  memset(imu->accel, 0, 3*sizeof(float));
  memset(imu->gyro, 0, 3*sizeof(float));
  memset(imu->mag, 0, 3*sizeof(float));
  memset(imu->prev_accel, 0, 3*sizeof(float));
  memset(imu->prev_gyro, 0, 3*sizeof(float));
  memset(imu->prev_mag, 0, 3*sizeof(float));
  memset(imu->filtered_gyro, 0, 3*sizeof(float));
  memset(imu->filtered_accel, 0, 3*sizeof(float));
  memset(imu->filtered_mag, 0, 3*sizeof(float));
}

void setup() {
  Serial.begin(115200);
  while(!Serial);
  status = IMU.begin(); // set I2C Register

  if (status < 0) { // detect error
    Serial.println("IMU initialization unsuccessful");
    Serial.println("Check IMU wiring or try cycling power");
    Serial.print("Status: ");
    Serial.println(status);
  }

  else
  {
    Serial.println("IMU initialization successful....please wait");
    
    // setting the accelerometer full scale range to +/-8G 
    IMU.setAccelRange(MPU9250::ACCEL_RANGE_8G);

    // setting the gyroscope full scale range to +/-500 deg/s
    IMU.setGyroRange(MPU9250::GYRO_RANGE_500DPS);

    // setting DLPF bandwidth to 20 Hz
    IMU.setDlpfBandwidth(MPU9250::DLPF_BANDWIDTH_20HZ);

    // setting SRD to 19 for a 50 Hz update rate
    IMU.setSrd(19);
    
    // set struct variables to 0
    set_memory(&mpu9250);
  }
}

void loop() {
  IMU.readSensor(); // read raw data
  get_raw_data(&mpu9250); // allocate raw data to struct
  lowpass_filter(&mpu9250); // filtered data
  serial_print(&mpu9250); // print filtered data
}