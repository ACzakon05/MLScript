import pandas as pd
from mlscript_runtime.error_handling.safe_execute_load import safe_execute_load
svm_clf = SVC(
	probability=True,
	coef0=0.0,
	gamma=1.3,
	kernel='sigmoid',
	degree=3,
	C=1.4,
)
