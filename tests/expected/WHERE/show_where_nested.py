import pandas as pd
from mlscript_runtime.error_handling.safe_execute_load import safe_execute_load
with safe_execute_load(f"loading 'data.csv'"):
	my_dataset = pd.read_csv(
		filepath_or_buffer='data.csv',
		delimiter=",",
		header="infer"
	)
print(my_dataset.where( ~( ( ( (my_dataset["status"] == 'inactive')  |  (my_dataset["price"] < 100) ) ) ) ).iloc[0:6])