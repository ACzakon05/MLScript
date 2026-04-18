import pandas as pd
from mlscript_runtime.error_handling.safe_execute_load import safe_execute_load
from sklearn.linear_model import LinearRegression
lin_reg = LinearRegression(
	positive=False,
	n_jobs=None,
	tol=1e-5,
	fit_intercept=False,
)
