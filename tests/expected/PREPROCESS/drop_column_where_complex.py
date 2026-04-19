import pandas as pd
from mlscript_runtime.error_handling.safe_execute_load import safe_execute_load
with safe_execute_load(f"loading 'data.csv'"):
	my_dataset = pd.read_csv(
		filepath_or_buffer='data.csv',
		delimiter=",",
		header="infer"
	)
# Drop columns from my_dataset
my_dataset = my_dataset.drop(columns=[col for col in my_dataset.columns if  ( (my_dataset[col].dtype == 'object')  &  (my_dataset[col].isna().sum() / len(my_dataset) > 0.1) ) ])
