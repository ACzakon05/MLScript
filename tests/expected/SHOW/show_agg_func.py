import pandas as pd
from mlscript_runtime.error_handling.safe_execute_load import safe_execute_load
with safe_execute_load(f"loading 'data.csv'"):
	my_dataset = pd.read_csv(
		filepath_or_buffer='data.csv',
		delimiter=",",
		header="infer"
	)
print(my_dataset[["price"]].agg(['mean']))
print(my_dataset[["price"]].agg(['max']))
print(my_dataset[["price"]].agg(['min']))
print(my_dataset[["price"]].agg(['sum']))
print(my_dataset[["price"]].agg(['median']))
print(my_dataset[["price"]].agg(['prod']))
print(my_dataset[["price"]].agg(['std']))
print(my_dataset[["price"]].agg(['var']))
print(my_dataset[["price"]].agg(['unique']))
print(my_dataset[["price"]].agg(['nunique']))