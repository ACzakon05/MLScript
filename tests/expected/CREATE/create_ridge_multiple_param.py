import pandas as pd
from mlscript_runtime.error_handling.safe_execute_load import safe_execute_load
from sklearn.linear_model import Ridge
ridge_reg = Ridge(
	positive=False,
	solver='svd',
	max_iter=1000,
	tol=0.0001,
	fit_intercept=False,
	alpha=1.0,
)
