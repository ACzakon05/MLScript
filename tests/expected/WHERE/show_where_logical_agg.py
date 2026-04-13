import pandas as pd
my_dataset = pd.read_csv(
	filepath_or_buffer='data.csv',
	delimiter=",",
	header="infer"
)
print(my_dataset[["price"]].where( ( (my_dataset["category"] == 'Electronics')  &  (my_dataset["stock"] >= 10) ) ).agg(['mean']))