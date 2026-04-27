import pandas as pd
from mlscript_runtime.error_handling.safe_execute_load import safe_execute_load
from mlscript_runtime.error_handling.safe_execute_show import safe_execute_show
from sklearn.linear_model import LinearRegression
lin_reg = LinearRegression(
	positive=False,
	n_jobs=None,
	tol=1e-7,
	fit_intercept=True,
)
