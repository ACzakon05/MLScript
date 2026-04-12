import pandas as pd
my_dataset = pd.read_csv(
	filepath_or_buffer='data.csv',
	delimiter=",",
	header="infer"
)
print(my_dataset[["price"]].agg(['mean']))
print(my_dataset[["price"]].agg(['max']))
print(my_dataset[["price"]].agg(['min']))