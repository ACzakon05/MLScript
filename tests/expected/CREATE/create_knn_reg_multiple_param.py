import pandas as pd
from mlscript_runtime.error_handling.safe_execute_load import safe_execute_load
from sklearn.neighbors import KNeighborsRegressor
knn_reg = KNeighborsRegressor(
	n_jobs=-1,
	algorithm='auto',
	weights='uniform',
	n_neighbors=4,
)