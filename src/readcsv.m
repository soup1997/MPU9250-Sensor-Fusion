T = readtable("imu_data.csv");
p = T{:, 1};
q = T{:, 2};
r = T{:, 3};
mag_yaw = T{:, 4};