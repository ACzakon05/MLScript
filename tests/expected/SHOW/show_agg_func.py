import pandas as pd
from mlscript_runtime.error_handling.safe_execute_load import safe_execute_load
from mlscript_runtime.error_handling.safe_execute_show import safe_execute_show
with safe_execute_load(f"loading 'data.csv'"):
	my_dataset = pd.read_csv(
		filepath_or_buffer='data.csv',
		delimiter=",",
		header="infer"
	)
with safe_execute_show('displaying data'):
	print(my_dataset[["price"]].agg(['mean']))
with safe_execute_show('displaying data'):
	print(my_dataset[["price"]].agg(['max']))
with safe_execute_show('displaying data'):
	print(my_dataset[["price"]].agg(['min']))
with safe_execute_show('displaying data'):
	print(my_dataset[["price"]].agg(['sum']))
with safe_execute_show('displaying data'):
	print(my_dataset[["price"]].agg(['median']))
with safe_execute_show('displaying data'):
	print(my_dataset[["price"]].agg(['prod']))
with safe_execute_show('displaying data'):
	print(my_dataset[["price"]].agg(['std']))
with safe_execute_show('displaying data'):
	print(my_dataset[["price"]].agg(['var']))
with safe_execute_show('displaying data'):
	print(my_dataset[["price"]].agg(['unique']))
with safe_execute_show('displaying data'):
	print(my_dataset[["price"]].agg(['nunique']))