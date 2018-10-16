import numpy as np
import pandas as pd

from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
from matplotlib import cm
from matplotlib.ticker import LinearLocator, FormatStrFormatter

#https://stackoverflow.com/questions/36589521/how-to-surface-plot-3d-plot-from-dataframe

folder = 'C:/work/Data/'

# load trades
filename = 'results.1233.csv'
trades = pd.read_csv(folder + filename, sep=';')

fig = plt.figure()
ax = fig.gca(projection='3d')

# Make data.
x = trades.loc[:, 'fix_at']
y = trades.loc[:, 'stop_loss_at']
z = trades.loc[:, 'p&l']

# Plot the surface.
surf = ax.plot_trisurf(x, y, z, cmap=cm.RdYlGn, linewidth=0.2)

# Add a color bar which maps values to colors.
fig.colorbar(surf, shrink=0.5, aspect=5)

plt.show()
