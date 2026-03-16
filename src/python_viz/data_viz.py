"""
This python file will handle the data visulization of the simulation .csv data using the 
MatPlotLib python library. The data, formatted with file_reader.py, will be displayed as
a Position vs. Time graph, and initial paramaters of the simulation will be shown, too.
"""

import file_reader

class DataVisualizer:
    def __init__(self, window_width_pixels:int, window_height_pixels:int, animated:bool=False):
        self.WINDOW_WIDTH = window_width_pixels
        self.WINDOW_HEIGHT = window_height_pixels

    def generateGraph(self):
        # create a graph and show data
        pass

    def _createWindow(self):
        # create a new window, look at consts
        pass

    def _drawDataPoint(self):
        # plot a pt
        pass

    def setAnimationTime(self, duration_ms:int):
        # make sure plot is supposed to be animated (encapsulation, good practice)
        pass