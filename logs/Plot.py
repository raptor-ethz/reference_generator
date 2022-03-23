import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import sys
import math
import os
import csv
import statistics

# Parameters
Start = 0  # start frame
N = 10000  # how many frames

ref_color = "black"  # reference color
curve_color = "red"  # measurement color
# Plot initialization
fig_xyz = plt.figure()  # xyz plot
z_plt = fig_xyz.add_subplot(111)

# fig_thrust = plt.figure()  # xyz plot
# thrust_plt = fig_thrust.add_subplot(111)
# Data initialization / Read Data

#df = pd.read_csv(sys.argv[2])

colors = ["red", "blue", "orange", "green",
          "orange", "grey", "olive", "cyan", "brown", "purple"]

header_list = [
    "t", "x", "y", "z"
]

ref_pos = np.full(1000, 2)
ref_null = np.full(1000, 0)
ref_gr = np.linspace(-1, 1, num=1000)
ref_gr_pl = np.linspace(0, 2, num=1000)
ref_neg = np.full(1000, 1)

ref = z_plt.scatter(ref_null, ref_gr, marker="o", s=1, c="black")
ref = z_plt.scatter(ref_pos, ref_gr, marker="o", s=1, c="black")
ref = z_plt.scatter(ref_gr_pl, -ref_neg, marker="o", s=1, c="black")
ref = z_plt.scatter(ref_gr_pl, ref_neg, marker="o", s=1, c="black")
ref.set_label("reference")

line = [0]*len(sys.argv)
for i in range(1, len(sys.argv)):
    df = pd.read_csv(sys.argv[i])
    time = pd.read_csv(sys.argv[i], usecols=["t"],
                       names=header_list).to_numpy()
    x = pd.read_csv(sys.argv[i], usecols=["x"],
                    names=header_list).to_numpy()
    y = pd.read_csv(sys.argv[i], usecols=["y"],
                    names=header_list).to_numpy()
    z = pd.read_csv(sys.argv[i], usecols=["z"],
                    names=header_list).to_numpy()

    # Eps = (np.max(x)-1)/2
    # for a in range(len(x)):
    #     if(x[a] > 0.9):
    #         t_90 = time[a] / 1000.0
    #         break

    # print("iteration: "+str(i)+" t_90: "+str(t_90)+" Eps: "+str(Eps))

    # t-z plot
    #z_plt.scatter(time, p_ref[:, 2], marker="o", s=1, c=ref_color)
   # if(i == 1):
    #line[i] = z_plt.scatter(time/1000, p[:, 0], marker="o", s=1, c=colors[i])
    line[i] = z_plt.scatter(x, y, marker="o", s=1, c=colors[i])

    # if(i == 2):
    # high_line = z_plt.scatter(
    #     time/1000, p[:, 2]-1.0, marker="o", s=1, c=curve_color)
    # high_line.set_label('Quadcopter out of ground effect (-1 m)')

    # t-thrust plot
    #thrust_plt.scatter(time, rpy[:, 1], marker="o", s=1, c=curve_color)


line[1].set_label("old gains")
line[2].set_label("new gains")

z_plt.set_xlabel("time [s]")
z_plt.set_ylabel("x [m]")
z_plt.legend()
plt.savefig("plot.svg")

# thrust_plt.set_xlabel("time [s]")
# thrust_plt.set_ylabel("thrust [mm]")

# Display Plots
plt.show()
