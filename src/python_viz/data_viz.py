"""
This python file will handle the data visulization of the simulation .csv data using the 
MatPlotLib python library. The data, formatted with file_reader.py, will be displayed as
a Position vs. Time graph, and initial paramaters of the simulation will be shown, too.
"""

import file_reader
import matplotlib.pyplot as plt
import numpy as np

class DataVisualizer:
    def __init__(self, file_reader:file_reader, animated:bool=False):
        self.file_reader = file_reader

    def generateGraph(self):
        # create a graph and show data
        self.file_reader.openFile(self.file_reader.FILE_PATH)
        fig, ax = self._createWindow(show_pos=True)
        ax.plot(self.file_reader.parsePositionData(), label="Position")
        plt.show()

    def _createWindow(self, show_pos=False, show_vel=False, show_applied_force=False) -> list:
        # create a new window, look at consts
        fig, ax = plt.subplots()
        ax.xaxis.set_label_text("Time dt (Seconds)")
        ax.yaxis.set_label_text("Simulated Object Transform")
        return fig, ax

    def _drawDataPoint(self):
        # plot a pt
        pass

    def setAnimationTime(self, duration_ms:int):
        # make sure plot is supposed to be animated (encapsulation, good practice)
        pass