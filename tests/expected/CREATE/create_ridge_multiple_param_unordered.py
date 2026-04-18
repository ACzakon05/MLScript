import pandas as pd
from mlscript_runtime.error_handling.safe_execute_load import safe_execute_load
from sklearn.linear_model import Ridge
ridge_reg = Ridge(
	positive=True,
	solver='saga',
	max_iter=500,
	tol=1e-3,
	fit_intercept=False,
	alpha=0.1,
)
