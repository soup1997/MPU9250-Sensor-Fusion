T = readtable("imu_data.csv");

p = T{:, 1};
q = T{:, 2};
r = T{:, 3};
mag_yaw = T{:, 4};

time_table = zeros(length(p), 1);

for k=1:length(p)
    if k == 1
        time(1) = 0.00;
    
    else
        time = (k-1) * 0.02;
        time_table(k) = time;
    end

end

p = [time_table, p];
q = [time_table, q];
r = [time_table, r];
mag_yaw = [time_table, mag_yaw];
