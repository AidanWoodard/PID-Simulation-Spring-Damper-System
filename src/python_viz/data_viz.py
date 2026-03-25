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
        self.PLOT_TITLE = "1 Dimension PID Simulated Control"
        self.ANIMATED = animated
        if animated: 
            self.animation_length_ms : int
            self.MAX_ANIM_LENGTH_MS = 10*1000   # 10 seconds
            self.MIN_ANIM_LENGTH_MS = 0

    def generateGraph(self, show_pos=False, show_vel=False, show_applied_force=False):
        # create a graph and show data
        self.file_reader.openFile(self.file_reader.FILE_PATH)
        # TODO: parse header data and display (ie PID params, dt value)
        fig, ax = self._createWindow(show_pos, show_vel, show_applied_force)
        if show_pos: ax.plot(self.file_reader.parsePositionData(), label="Position")
        if show_vel: ax.plot(self.file_reader.parseVelocityData(), label="Velocity")
        if show_applied_force: ax.plot(self.file_reader.parseAppliedForceData(), label="Force")
        plt.show()

    def _createWindow(self, show_pos=False, show_vel=False, show_applied_force=False):
        if not(show_pos or show_vel or show_applied_force):
            print("ERROR ENCOUNTERED WHEN GENERATING GRAPH: No data instructed to display.\nDid you forget to set show_pos, etc. to True?")
            raise Exception
        
        fig, ax = plt.subplots()
        ax.xaxis.set_label_text("Time dt (Seconds)")
        ax.set_title(self.PLOT_TITLE)

        return fig, ax

    def setAnimationTime(self, duration_ms:int):
        # make sure plot is supposed to be animated (encapsulation, good practice)
        if self.ANIMATED and duration_ms > self.MIN_ANIM_LENGTH_MS and duration_ms < self.MAX_ANIM_LENGTH_MS:
            self.animation_length_ms = duration_ms
        elif not self.ANIMATED:
            print("ERROR ENCOUNTERED WHILE UPDATING ANIMATION TIME:\nGraph not supposed to be animated.")
            raise IOError
        else:
            print(f"ERROR ENCOUNTERED WHILE UPDATING ANIMATION TIME.\nInputed time: {duration_ms}\n")
            raise IOError
