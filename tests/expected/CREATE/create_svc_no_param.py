import pandas as pd
from mlscript_runtime.error_handling.safe_execute_load import safe_execute_load
svm_clf = SVC(
	probability=False,
	coef0=0.0,
	gamma='scale',
	kernel='rbf',
	degree=3,
	C=1.0,
)
