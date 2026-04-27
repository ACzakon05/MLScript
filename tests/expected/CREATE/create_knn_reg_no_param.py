import pandas as pd
from mlscript_runtime.error_handling.safe_execute_load import safe_execute_load
from mlscript_runtime.error_handling.safe_execute_show import safe_execute_show
from sklearn.neighbors import KNeighborsRegressor
knn_reg = KNeighborsRegressor(
	n_jobs=None,
	algorithm='auto',
	weights='uniform',
	n_neighbors=5,
)
