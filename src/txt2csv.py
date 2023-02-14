import pandas as pd

data_path = 'C:\\Users\\soup1997\\Desktop\\test\\yaw_data.txt'
save_path = 'C:\\Users\\soup1997\\Desktop\\test\\yaw_data.csv'

#read_file = pd.read_csv(data_path)
#read_file.to_csv(save_path)

df = pd.read_table('./yaw_data.txt', sep="\t")
df.to_csv('./yaw_data.csv', index=False)