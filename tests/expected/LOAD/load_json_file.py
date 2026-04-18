import pandas as pd
from mlscript_runtime.error_handling.safe_execute_load import safe_execute_load
with safe_execute_load(f"loading 'data.json'"):
    my_dataset = pd.read_json(
        path_or_buf='data.json',
        orient=None
    )