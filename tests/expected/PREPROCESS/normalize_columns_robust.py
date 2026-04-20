import pandas as pd
from mlscript_runtime.error_handling.safe_execute_load import safe_execute_load
# Normalize dataset
if 'my_dataset' not in globals() and 'my_dataset' not in locals():
    raise NameError("Dataset my_dataset not found")
import numpy as np
my_dataset = my_dataset.replace([np.inf, -np.inf], np.nan)
columns_to_normalize = list(columns_to_normalize)
missing = [c for c in columns_to_normalize if c not in my_dataset.columns]
if missing:
    raise KeyError(f"Columns not found: {missing}")
cols_to_normalize = ["age", "salary"]
from sklearn.preprocessing import RobustScaler
scaler = RobustScaler()
my_dataset[cols_to_normalize] = scaler.fit_transform(my_dataset[cols_to_normalize])
