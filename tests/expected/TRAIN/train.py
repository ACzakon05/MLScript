import pandas as pd
from mlscript_runtime.error_handling.safe_execute_load import safe_execute_load
from mlscript_runtime.error_handling.safe_execute_show import safe_execute_show
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LinearRegression
with safe_execute_load(f"loading 'examples/data/data.csv'"):
	df = pd.read_csv(
		filepath_or_buffer='examples/data/data.csv',
		delimiter=",",
		header="infer"
	)
# Marked 'first_name' as target for dataset 'df'
# Split whole dataset into train and test
train_set, test_set = train_test_split(
    df,
    test_size=0.2,
    random_state=None,
    shuffle=True
)
lin_reg = LinearRegression(
	positive=False,
	n_jobs=None,
	tol=1e-6,
	fit_intercept=True,
)
X_train = train_set.drop(columns=['first_name'])
y_train = train_set['first_name']
lin_reg.fit(X_train, y_train)