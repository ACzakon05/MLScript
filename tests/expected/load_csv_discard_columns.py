import pandas as pd
my_dataset = pd.read_csv("data/data.csv")
my_dataset = my_dataset.drop(columns=["surname"])