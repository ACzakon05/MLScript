import pandas as pd
from mlscript_runtime.error_handling.safe_execute_load import safe_execute_load
# Normalize dataset
if 'my_dataset' not in globals() and 'my_dataset' not in locals():
    raise NameError("Dataset my_dataset not found")
import numpy as np
my_dataset = my_dataset.replace([np.inf, -np.inf], np.nan)
numeric_cols = my_dataset.select_dtypes(include=[np.number]).columns
my_dataset[numeric_cols] = (my_dataset[numeric_cols] - my_dataset[numeric_cols].min()) / (my_dataset[numeric_cols].max() - my_dataset[numeric_cols].min() + 1e-9)
