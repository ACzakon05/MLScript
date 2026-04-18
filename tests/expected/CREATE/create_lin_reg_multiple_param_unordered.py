import pandas as pd
from mlscript_runtime.error_handling.safe_execute_load import safe_execute_load
from sklearn.linear_model import LinearRegression
lin_reg = LinearRegression(
	positive=True,
	n_jobs=-1,
	tol=1e-4,
	fit_intercept=False,
)