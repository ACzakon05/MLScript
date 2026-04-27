import pandas as pd
from mlscript_runtime.error_handling.safe_execute_load import safe_execute_load
from mlscript_runtime.error_handling.safe_execute_show import safe_execute_show
from sklearn.svm import SVC
svm_clf = SVC(
	probability=False,
	coef0=0.0,
	gamma='scale',
	kernel='rbf',
	degree=3,
	C=1.0,
)
