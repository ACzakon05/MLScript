import pandas as pd
my_dataset = pd.read_csv("data/data.csv")
print(my_dataset[["price"]].agg(['mean']))
print(my_dataset[["price"]].agg(['max']))
print(my_dataset[["price"]].agg(['min']))