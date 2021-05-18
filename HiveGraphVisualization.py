import numpy as np
from matplotlib import collections  as mc, pyplot as pl
import matplotlib.animation as animation
from numpy.core.fromnumeric import repeat
# from matplotlib import pylab as pl

COLORS = np.array([(1, 0, 0, 1), (0, 1, 0, 1), (0, 0, 1, 1)])
FIG, AX = pl.subplots()

def plot_lines(lines):
    lc = mc.LineCollection(lines, colors=COLORS, linewidths=2)
    AX.add_collection(lc)
    AX.autoscale()
    AX.margins(0.1)
    graph_plots = AX.plot()
    return graph_plots

def update_graph(new_line):
    x1, y1 = new_line[0]
    x2, y2 = new_line[1]
    pl.plot((x1, x2), (y1, y2))

def create_plot(lines, new_lines):
    graph = plot_lines(lines)
    ani = animation.FuncAnimation(FIG, update_graph, new_lines, interval=1000, blit=False, repeat= False)
    pl.show()

lines = [[(0, 1), (1, 1)], [(2, 3), (3, 3)], [(1, 2), (1, 3)]]
new_lines = [[(2, 1), (2, 2)], [(3, 1), (1, 1)], [(6, 3), (7, 2)]]

create_plot(lines, new_lines)