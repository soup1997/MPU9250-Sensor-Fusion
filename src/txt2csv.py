import pandas as pd

data_path = 'C:/Users/soup1997/Desktop/soup1997/MPU9250-Sensor-Fusion/data/imu_data.txt'
df = pd.read_table(data_path, sep="\t")
df.to_csv('C:/Users/soup1997/Desktop/soup1997/MPU9250-Sensor-Fusion/data/imu_data.csv', index=False)