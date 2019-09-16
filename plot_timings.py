import sys
import seaborn as sns
import matplotlib.pyplot as plt
import pandas as pd

for i in range(len(sys.argv)-1):
    df = pd.read_csv("checkpoint_{:d}/timings.csv".format(checkpoint1))
    reduced_df = df[["checkpoint","reduce","transform_reduce"]]
    reduced_df['checkpoint'] = [sys.argv[i+1]]*df.shape[0]

    if(i>0):
        fulldf = pd.concat(full_df, reduced_df)
    else:
        fulldf = reduced_df
        
print(fulldf)
# melted_df = pd.melt(reduced_df, id_vars="checkpoint", var_name="operation", value_name="time")

# ax = sns.barplot(x="operation", y="time", hue="checkpoint", data=melted_df)

# plt.show()
