import pandas as pd
from mlscript_runtime.error_handling.safe_execute_load import safe_execute_load
with safe_execute_load(f"loading 'data.pkl'"):
    my_dataset = pd.read_pickle(
        filepath_or_buffer='data.pkl'
    )