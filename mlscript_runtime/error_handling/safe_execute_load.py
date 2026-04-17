import sys
import pandas as pd
from contextlib import contextmanager

@contextmanager
def safe_execute_load(action_description):
    try:
        yield

    # == System & I/O errors ==
    except FileNotFoundError:
        print(f"[MLScript Error] Could not find file when {action_description}.", file=sys.stderr)
        sys.exit(1)
    except PermissionError:
        print(f"[MLScript Error] Permission denied when {action_description}. Check file permissions.", file=sys.stderr)
        sys.exit(1)
    except IsADirectoryError:
        print(f"[MLScript Error] The path provided is a folder, not a file, when {action_description}.", file=sys.stderr)
        sys.exit(1)

    ## == Pandas parsing errors ==
    except pd.errors.EmptyDataError:
        print(f"[MLScript Error] The file is completely empty when {action_description}.", file=sys.stderr)
        sys.exit(1)
    except pd.errors.ParserError:
        print(f"[MLScript Error] Malformed data found when {action_description}. Check delimiters or unequal row lengths.", file=sys.stderr)
        sys.exit(1)
    except UnicodeDecodeError:
        print(f"[MLScript Error] Encoding issue when {action_description}. The file contains non-UTF-8 characters.", file=sys.stderr)
        sys.exit(1)

    ## == Resource & value errors
    except ValueError as e:
        print(f"[MLScript Error] Invalid format or argument when {action_description}: {e}", file=sys.stderr)
        sys.exit(1)
    except MemoryError:
        print(f"[MLScript Error] Out of memory when {action_description}. The dataset is too large to fit in RAM.", file=sys.stderr)
        sys.exit(1)

    # == Data manipulation errors ==
    except KeyError as e:
        missing_cols = e.args[0] if len(e.args) > 0 else "unknown"
        print(f"[MLScript Error] Column(s) not found when {action_description}.", file=sys.stderr)
        print(f" -> Missing column(s): {missing_cols}", file=sys.stderr)
        sys.exit(1)
    except (TypeError, ValueError) as e:
        print(f"[MLScript Error] Invalid row limit or data type when {action_description}: {e}", file=sys.stderr)
        sys.exit(1)

    # == Fallback ==
    except Exception as e:
        print(f"[MLScript Error] Unexpected error when {action_description} ", file=sys.stderr)
        sys.exit(1)