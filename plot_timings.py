import sys
import seaborn as sns
import matplotlib.pyplot as plt
import pandas as pd

if len(sys.argv)<2:
    sys.exit("plot_timings.py: missing operand\n"
             "Usage: python plot_timings.py checkpoint1 [checkpoint2 ...]")
for i in range(len(sys.argv)-1):
    checkpoint = int(sys.argv[i+1])
    try:
        full_df = pd.read_csv("timings_{:d}_release.csv".format(checkpoint))
    except:
        sys.exit("Error: Could not find file timings_{:d}.csv".format(checkpoint))
        
    full_df["checkpoint"] = [checkpoint]*full_df.shape[0]
    if(i>0):
        df = pd.concat([df, full_df], ignore_index=True)
    else:
        df = full_df


fig_ax = []
fig = plt.figure(figsize=(20,10))
gs = fig.add_gridspec(2,3)
fig_ax1 = fig.add_subplot(gs[0,:])
fig_ax1.set_title("any_of, reduce and transform_reduce")
fig_ax2 = fig.add_subplot(gs[1,0])
fig_ax2.set_title("write_data")
fig_ax3 = fig.add_subplot(gs[1,1])
fig_ax3.set_title("nth_element")
fig_ax4 = fig.add_subplot(gs[1,2])
fig_ax4.set_title("mismatch")
allaxes = fig.get_axes()

things2plot = [["checkpoint","any_of","reduce","transform_reduce"],
               ["checkpoint", "write_data"],
               ["checkpoint", "nth_element"],
               ["checkpoint", "mismatch"]]

for i in range(len(things2plot)):
    reduced_df = df[things2plot[i]]        
    melted_df = pd.melt(reduced_df, id_vars="checkpoint", var_name="operation", value_name="time")

    allaxes[i].set_title("RELEASE")
    sns.barplot(x="operation", y="time", hue="checkpoint", data=melted_df, ax=allaxes[i])

plt.savefig("release.png")
plt.show()
