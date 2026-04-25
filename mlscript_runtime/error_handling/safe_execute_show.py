import sys
import os
import pandas as pd
from contextlib import contextmanager

@contextmanager
def safe_execute_show(action_description):
    try:
        yield
    except IndexError:
        print(f"[MLScript Error] Row index out of range while {action_description}.", file=sys.stderr)
        sys.exit(1)
    except KeyError as e:
        print(f"[MLScript Error] Column {e} not found while {action_description}.", file=sys.stderr)
        sys.exit(1)
    except Exception as e:
        print(f"[MLScript Error] Unexpected error when {action_description} ", file=sys.stderr)
        sys.exit(1)