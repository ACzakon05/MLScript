import pandas as pd
from mlscript_runtime.error_handling.safe_execute_load import safe_execute_load
# Normalize dataset
if 'my_dataset' not in globals() and 'my_dataset' not in locals():
    raise NameError("Dataset my_dataset not found")
import numpy as np
my_dataset = my_dataset.replace([np.inf, -np.inf], np.nan)
from sklearn.preprocessing import RobustScaler
scaler = RobustScaler()
numeric_cols = my_dataset.select_dtypes(include=[np.number]).columns
my_dataset[numeric_cols] = scaler.fit_transform(my_dataset[numeric_cols])
