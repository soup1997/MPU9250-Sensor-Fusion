T = readtable("imu_total.csv");

ax = T{:, 1};
ay = T{:, 2};
az = T{:, 3};
p = T{:, 4};
q = T{:, 5};
r = T{:, 6};
mx = T{:, 7};
my = T{:, 8};
mz = T{:, 9};

time_table = zeros(length(p), 1);

for k=1:length(p)
    if k == 1
        time(1) = 0.00;
    
    else
        time = (k-1) * 0.02;
        time_table(k) = time;
    end

end

ax = [time_table, ax];
ay = [time_table, ay];
az = [time_table, az];

p = [time_table, p];
q = [time_table, q];
r = [time_table, r];

mx = [time_table, mx];
my = [time_table, my];
mz = [time_talbe, mz];

