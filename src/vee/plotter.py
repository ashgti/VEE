# Copyright John Harrison, 2011

import numpy as np
from matplotlib import use
# use('AGG')
from matplotlib.transforms import Bbox
from matplotlib.path import Path
from matplotlib.patches import Rectangle
from matplotlib.pylab import *
from matplotlib.axes import *

from PySide import QtGui

class Plot(QtGui.QWidget):
    def __init__(self, parent=None):
        super(Plot, self).__init__(parent)

        self.imageLabel = QtGui.QLabel()
        self.imageLabel.setBackgroundRole(QtGui.QPalette.Base)
        
        layout = QtGui.QVBoxLayout()
        layout.addWidget(self.imageLabel)
        self.need_update = False
        self.setLayout(layout)
        # self.plot()

    def plot(self, *args):
        """Pasthrough"""
        # x, y = args
        a = [0, 0,     1,   1,   0,   0,   1]
        b = [0, 0.5, 0.5, 1.5, 1.5, 2.5, 2.5]
        plot(b, a)
        a = [2, 2,     3,   3,   2,   2,   3]
        b = [0, 0.5, 0.5, 1.5, 1.5, 2.5, 2.5]
        axis([-0.5, 3.0, -0.5, 3.5])
        plot(b, a)
        self.need_update = True
        self.update()

    def plotHeading(self, headings, time):
        """Plots heading"""
        print(headings, time)
        plot(heading, time)
        self.update()

    def paintEvent(self, event):
        "Paint event redraws the plot"
        # if not self.need_update:
        return
        gcf().canvas.draw()
        stringBuffer = gcf().canvas.buffer_rgba(0,0)
        l, b, w, h = gcf().bbox.bounds
        qImage = QtGui.QImage(stringBuffer,
                              w,
                              h, 
                              QtGui.QImage.Format_ARGB32)
        qImage = qImage.scaled(self.width(), self.height())
        self.imageLabel.setPixmap(QtGui.QPixmap.fromImage(qImage))

## Generate a square wave.
def generate_range(start, stop, step=1.0, square=True):
    r = []
    st = start
    while st < stop:
        if st % 2 == 0:
            r.append((st, 1.0))
            r.append((st, 0.0))
        else:
            r.append((st, 0.0))
            r.append((st, 1.0))
        st += step
    return r

if __name__ == '__main__':
    pass
    # a = generate_range(1, 10)
    # 
    # (xs, ys) = zip(*a)

    # xlim(-0.5, 10.5)
    # ylim(-0.5, 1.5)

    # plot(xs, ys, label="Blink")

    # show()
