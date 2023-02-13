#include "MPU9250.h"
#include "string.h"
#include "math.h"

#define PI 3.141592

// raw data
typedef struct {
  float accel[3]; // raw accel value
  float gyro[3]; // raw gyro value
  float mag[3]; // raw mag value

  float prev_accel[3]; // previous accel value
  float prev_gyro[3]; // previous gyro value
  float prev_mag[3]; // previous mag value

  float filtered_accel[3]; // filtered accel value
  float filtered_gyro[3]; // filtered gyro value
  float filtered_mag[3]; // filtered mag value
}imu_data;

// yaw calculation parameters(Gyroscope)
float prevPhi = 0.0;
float prevTheta = 0.0;
float prevPsi = 0.0;
float phi = 0.0;
float theta = 0.0;
float psi = 0.0;
float gyro_yaw = 0.0;
float dt = (1.0 / 50.0); // 50Hz

// yaw calculation parameters(Magnetometer)
double mag_yaw = 0.0;

// lowpass filter parameter
static float alpha = 0.8;

// init
MPU9250 IMU(Wire, 0x68);
int status;
imu_data mpu9250;

void get_raw_data(imu_data *imu) // rad/sec
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

void lpf(imu_data *imu) // rad/sec
{
  for(int i = 0; i < 3; i++)
  {
    imu->filtered_accel[i] = (alpha * imu->prev_accel[i]) + ((1.0 - alpha) * imu->accel[i]); 
    imu->filtered_gyro[i] = ((alpha * imu->prev_gyro[i]) + ((1.0 - alpha) * imu->gyro[i]));
    imu->filtered_mag[i] = (alpha * imu->prev_mag[i]) + ((1.0 - alpha) * imu->mag[i]); 
  }

  for(int j = 0; j < 3; j++)
  {
    imu->prev_accel[j] = imu->filtered_accel[j]; 
    imu->prev_gyro[j] = imu->filtered_gyro[j]; 
    imu->prev_mag[j] = imu->filtered_mag[j]; 
  }
}

void euler_gyro(imu_data *imu)
{
  double sinPhi = sin(prevPhi);
  double cosPhi = cos(prevPhi);
  double cosTheta = cos(prevTheta);
  double tanTheta = tan(prevTheta);

  // Gyro to Euler Angle
  phi = prevPhi + dt*(imu->filtered_gyro[0] + imu->filtered_gyro[1]*sinPhi*tanTheta + imu->filtered_gyro[2]*cosPhi*tanTheta);
  theta = prevTheta + dt*(imu->filtered_gyro[1]*cosPhi - imu->filtered_gyro[2]*sinPhi);
  psi = prevPsi + dt*(imu->filtered_gyro[1]*sinPhi/cosTheta + imu->filtered_gyro[2]*cosPhi/cosTheta);
  
  prevPhi = phi;
  prevTheta = theta;
  prevPsi = psi;

  gyro_yaw = psi * (180.0 / PI); // radian to degree
}

void euler_mag(imu_data *imu)
{
  mag_yaw = atan2(imu->filtered_mag[1], imu->filtered_mag[0]) * (180.0 / PI); // radian to degree
}

void serial_print(imu_data *imu)
{
  Serial.print("Gyro yaw: ");
  Serial.print(gyro_yaw);

  Serial.print("Mag yaw: ");
  Serial.println(mag_yaw);
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
    IMU.setGyroRange(MPU9250::GYRO_RANGE_250DPS);

    // setting DLPF bandwidth to 20 Hz
    IMU.setDlpfBandwidth(MPU9250::DLPF_BANDWIDTH_20HZ);

    // setting SRD to 19 for a 50 Hz update rate
    IMU.setSrd(19);

    /* finds bias and scale factor calibration for the magnetometer,
    The sensor should be rotated in a figure 8 motion until complete */
    IMU.calibrateGyro();
    IMU.calibrateMag();
    
    // set struct variables to 0
    set_memory(&mpu9250);
  }
}

void loop() {
  IMU.readSensor(); // read raw data
  get_raw_data(&mpu9250); // allocate raw data to struct
  lpf(&mpu9250); // filtered data

  euler_gyro(&mpu9250);
  euler_mag(&mpu9250);

  serial_print(&mpu9250); // print yaw_data
}