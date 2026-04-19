import pandas as pd
from mlscript_runtime.error_handling.safe_execute_load import safe_execute_load
with safe_execute_load(f"loading 'data.csv'"):
	my_dataset = pd.read_csv(
		filepath_or_buffer='data.csv',
		delimiter=",",
		header="infer"
	)
#Set target column for my_dataset: target_col
my_dataset_y = my_dataset["target_col"]
my_dataset_X = my_dataset.drop(["target_col"])
# Drop columns from my_dataset
# Safe mode: excluding target column 'target_col'
my_dataset = my_dataset.drop(columns=[col for col in my_dataset.columns if col != 'target_col' and  (my_dataset[col].dtype == 'object') ])
