import pandas as pd
my_dataset = pd.read_csv(
	filepath_or_buffer='data.csv',
	delimiter=",",
	header="infer"
)
print(my_dataset.where((my_dataset["age"] > 25)))