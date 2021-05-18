import numpy as np
from matplotlib import collections  as mc, pyplot as pl
import matplotlib.animation as animation
from numpy.core.fromnumeric import repeat
# from matplotlib import pylab as pl

class HiveVisualization:
    def __init__(self, xmin, xmax, ymin, ymax) -> None:
        self.ORIGINAL_COLOR = np.array([0, 0, 0, 1])
        self.COLORS = np.array([(1, 0, 0, 1), (0, 1, 0, 1), (0, 0, 1, 1), (1, 1, 0, 1), (0, 1, 1, 1), (1, 0, 1, 1)])
        self.index = 0
        self.FIG, self.AX = pl.subplots()
        self.AX.set_xlim([xmin, xmax])
        self.AX.set_ylim([ymin, ymax])

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
            # print(new_line)
            x1, y1 = new_line[0]
            x2, y2 = new_line[1]
            pl.plot((x1, x2), (y1, y2), color = color)

    def make_graphs(self, lines, new_lines):
        graph = self.plot_lines(lines)
        ani = animation.FuncAnimation(self.FIG, self.update_graph, new_lines, interval=1000, blit=False, repeat= False)
        pl.show()

global INF
INF = 2147483647

def find_max_min(a, b, c, maximum=True):
    global INF
    options = {a, b, c}
    if not maximum:
        INF = -INF
    if a == INF:
        options.remove(a)
    if b == INF:
        options.remove(b)
    if c == INF:
        options.remove(c)
    if maximum:
        return max(options)
    INF = -INF
    return min(options)


def segment_str_to_list(str_segments, xmin = float("inf"), xmax = -float("inf"), ymin = float("inf"), ymax = -float("inf")):
    new_list = []
    for segment in str_segments:
        segment = segment[1:-1]
        x1, y1, x2, y2 = segment.split(',')
        x1, y1, x2, y2 = float(x1), float(y1), float(x2), float(y2)
        xmin, ymin = find_max_min(x1, x2, xmin, False), find_max_min(ymin, y1, y2, False)
        xmax, ymax = find_max_min(xmax, x1, x2), find_max_min(ymax, y1, y2)
        new_list.append([[x1, y1], [x2, y2]])
    return (new_list, xmin, xmax, ymin, ymax)

f = open("points.txt", "r")
page_lines = f.readlines()
original_lines = page_lines[0].split()
lines, xmin, xmax, ymin, ymax = segment_str_to_list(original_lines)
new_lines = []
for i in range(1, len(page_lines)):
    new_set_split = page_lines[i].split()
    new_set, xmin, xmax, ymin, ymax = segment_str_to_list(new_set_split, xmin, xmax, ymin, ymax)
    new_lines.append(new_set)
f.close()
for segment in lines:
    for x,y in segment:
        if x == INF:
            segment[0] = xmax
        if y == INF:
            segment[1] = ymax
        if x == -INF:
            segment[0] = xmin
        if y == -INF:
            segment[1] = ymin
for set_of_lines in new_lines:
    for segment in set_of_lines:
        for point in segment:
            x, y = point
            if x == INF:
                point[0] = xmax
            if y == INF:
                point[1] = ymax
            if x == -INF:
                point[0] = xmin
            if y == -INF:
                point[1] = ymin
hv = HiveVisualization(xmin, xmax, ymin, ymax)
hv.make_graphs(lines, new_lines)