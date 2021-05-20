import numpy as np
from matplotlib import collections  as mc, pyplot as pl
import matplotlib.animation as animation
from numpy.core.fromnumeric import repeat

class PolygonVisualization:
    def __init__(self, xmin, xmax, ymin, ymax) -> None:
        self.COLORS = np.array([(0, 0, 0, 1), (1, 0, 0, 1)])
        self.FIG, self.AX = pl.subplots()
        self.AX.set_xlim([xmin-2, xmax+2])
        self.AX.set_ylim([ymin-2, ymax+2])
        self.last_line = 0
        self.index = 0

    def plot_lines(self, lines):
        lc = mc.LineCollection(lines, colors= self.COLORS[0], linewidths=2)
        self.AX.add_collection(lc)
        #self.AX.autoscale()
        self.AX.margins(0.1)
        graph_plots = self.AX.plot()
        return graph_plots

    def update_graph(self, new_set):        
        for i, new_line in enumerate(new_set):
            if(self.index == self.last_line):
                color = np.array([0, 1, 0, 1])
            else:
                color = self.COLORS[i%2]
            for line in new_line:
                x1, y1 = line[0]
                x2, y2 = line[1]
                pl.plot((x1, x2), (y1, y2), color = color)
        self.index += 1


    def make_graphs(self, lines, new_lines):
        self.last_line = len(new_lines)
        graph = self.plot_lines(lines)
        ani = animation.FuncAnimation(self.FIG, self.update_graph, new_lines, interval=1000, blit=False, repeat= False)
        pl.show()

def find_max_min(a, b, maximum=True):
    return max(a, b)
    return min(a, b)

def segment_str_to_list(str_segments):
    new_list = []
    for segment in str_segments:
        segment = segment[1:-1]
        x1, y1, x2, y2 = segment.split(',')
        x1, y1, x2, y2 = float(x1), float(y1), float(x2), float(y2)
        xmin, ymin = find_max_min(x1, x2, False), find_max_min(y1, y2, False)
        xmax, ymax = find_max_min(x1, x2), find_max_min(y1, y2)
        new_list.append([[x1, y1], [x2, y2]])
    return (new_list, xmin, xmax, ymin, ymax)

f = open("visual.txt", "r")
page_lines = f.readlines()
original_lines = [page_lines[0][:-1]]
original_lines.extend(page_lines[1].split())
lines, xmin, xmax, ymin, ymax = segment_str_to_list(original_lines)
new_lines = []
for i in range(2, len(page_lines)):
    new_set_split = page_lines[i].split()
    new_set, txmin, txmax, tymin, tymax = segment_str_to_list(new_set_split)
    if i == 2:
        new_lines.append([[], new_set])
    else:
        new_lines.append([new_lines[-1][1], new_set])
    xmin = min(xmin, txmin)
    ymin = min(ymin, tymin)
    xmax = max(xmax, txmax)
    ymax = max(ymax, tymax)
f.close()

pv = PolygonVisualization(xmin, xmax, ymin, ymax)
pv.make_graphs(lines, new_lines)