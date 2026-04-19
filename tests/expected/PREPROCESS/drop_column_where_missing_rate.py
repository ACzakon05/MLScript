import pandas as pd
from mlscript_runtime.error_handling.safe_execute_load import safe_execute_load
with safe_execute_load(f"loading 'data.csv'"):
	my_dataset = pd.read_csv(
		filepath_or_buffer='data.csv',
		delimiter=",",
		header="infer"
	)
# Drop columns from my_dataset
if 'my_dataset' not in globals() and 'my_dataset' not in locals():
    raise NameError("Dataset my_dataset not found")
columns_to_drop = [col for col in my_dataset.columns if  (my_dataset[col].isna().sum() / len(my_dataset) > 0.5) ]
columns_to_drop = list(columns_to_drop)
missing = [c for c in columns_to_drop if c not in my_dataset.columns]
if missing:
    raise KeyError(f"Columns not found: {missing}")
my_dataset = my_dataset.drop(columns=columns_to_drop)
