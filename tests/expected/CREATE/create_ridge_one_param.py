import pandas as pd
from mlscript_runtime.error_handling.safe_execute_load import safe_execute_load
from mlscript_runtime.error_handling.safe_execute_show import safe_execute_show
from sklearn.linear_model import Ridge
ridge_reg = Ridge(
	positive=False,
	solver='auto',
	max_iter=None,
	tol=0.0001,
	fit_intercept=True,
	alpha=0.5,
)
