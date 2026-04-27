import pandas as pd
from mlscript_runtime.error_handling.safe_execute_load import safe_execute_load
from mlscript_runtime.error_handling.safe_execute_show import safe_execute_show
from sklearn.neighbors import KNeighborsRegressor
knn_reg = KNeighborsRegressor(
	n_jobs=-1,
	algorithm='ball_tree',
	weights='distance',
	n_neighbors=3,
)
