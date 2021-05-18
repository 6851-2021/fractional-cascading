import numpy as np
from matplotlib import collections  as mc, pyplot as pl
import matplotlib.animation as animation
from numpy.core.fromnumeric import repeat
# from matplotlib import pylab as pl

class HiveVisualization:
    def __init__(self) -> None:
        self.ORIGINAL_COLOR = np.array([0, 0, 0, 1])
        self.COLORS = np.array([(1, 0, 0, 1), (0, 1, 0, 1), (0, 0, 1, 1), (1, 1, 0, 1), (0, 1, 1, 1), (1, 0, 1, 1)])
        self.index = 0
        self.FIG, self.AX = pl.subplots()

    def plot_lines(self, lines):
        lc = mc.LineCollection(lines, colors= self.ORIGINAL_COLOR, linewidths=2)
        self.AX.add_collection(lc)
        self.AX.autoscale()
        self.AX.margins(0.1)
        graph_plots = self.AX.plot()
        return graph_plots

    def update_graph(self, new_set):
        color = self.COLORS[self.index % len(self.COLORS)]
        self.index += 1
        for new_line in new_set:
            x1, y1 = new_line[0]
            x2, y2 = new_line[1]
            pl.plot((x1, x2), (y1, y2), color = color)

    def make_graphs(self, lines, new_lines):
        graph = self.plot_lines(lines)
        ani = animation.FuncAnimation(self.FIG, self.update_graph, new_lines, interval=1000, blit=False, repeat= False)
        pl.show()

lines = [[(0, 1), (1, 1)], [(2, 3), (3, 3)], [(1, 2), (1, 3)]]
new_lines = [([(2, 1), (2, 2)], [(3, 1), (1, 1)], [(6, 3), (7, 2)]),
            ([(3, 5), (1, 2)], [(1, 4), (2, 6)])]
hv = HiveVisualization()
hv.make_graphs(lines, new_lines)