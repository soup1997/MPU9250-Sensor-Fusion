# MPU9250 Sensor Fusion
This is the repository for the method to fuse IMU(Gyrosensor, Accelerometer, Magnetometer) by using extended kalman filter.
## Experiment environment

|Equiment|Name|
|---|---|
|IMU|MPU-9250|
|Main Controller|Arduino-UNO|

the wiring for the sensor is arraged as shown in the image below.
![image](https://user-images.githubusercontent.com/86957779/217463645-35ba5197-f327-4761-a887-9932343e454b.png)

## System Diagram
<div align = center>


![image](https://user-images.githubusercontent.com/86957779/220029536-03696811-007e-41d9-b849-2770a3458d28.png)
![image](https://user-images.githubusercontent.com/86957779/220029402-62ce5fb9-b6ca-4dc5-a16c-997808d29fbc.png)


</div>

* **Euler Gyro**: Moudle that receives angular velocity and calculates the euler angle.
* **Euler Accel**: Module that receives acceleration from accelerometer and calculates euler angle rates.
* **Extended Kalman Filter**: Module that calculates euler angle by combining data of a gyro sensor and an accelerometer.
* **Tilt Compensation**: Module that calculates heading angle(yaw) by receiving output of extended kalman filter(roll, pitch) and magnetometer sensing data.


## Result
the final result of this repositoy is arraged as shown in the figure below. 

![image](https://user-images.githubusercontent.com/86957779/220033927-482216c8-0798-420e-9506-a9324535dd9f.png)

## Reference
* https://electronics.stackexchange.com/questions/525266/tilt-compensation-for-yaw-calculation-from-magnetometer-and-accelerometer
* https://www.diva-portal.org/smash/get/diva2:1127455/FULLTEXT02.pdf
* https://link.springer.com/article/10.1007/s42401-022-00131-3
