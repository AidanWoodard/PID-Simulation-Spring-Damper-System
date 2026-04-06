"""
This python file will handle the data visulization of the simulation .csv data using the 
MatPlotLib python library. The data, formatted with file_reader.py, will be displayed as
a Position vs. Time graph, and initial paramaters of the simulation will be shown, too.
"""

import file_reader as fr
import matplotlib.pyplot as plt
import pandas as pd # type: ignore
from pathlib import Path

class DataVisualizer:
    def __init__(self, file_reader:fr, animated=False, show_legend=True):
        self.file_reader = file_reader
        self.PLOT_TITLE = "1-Dimension PID Simulated Control"
        self.ANIMATED = animated
        if animated: 
            self.animation_length_ms : int
            self.MAX_ANIM_LENGTH_MS = 10*1000   # 10 seconds
            self.MIN_ANIM_LENGTH_MS = 0

    """Create all graphs using helper function _generateNewGraph to display new data. Return those graphs"""
    def generateGraphs(self, sim_data_files:list, fig, ax, format:dict) -> tuple:
        for i, curr_path in enumerate(sim_data_files):
            parsed_sim_data = self.file_reader.parseSimData(curr_path, debug=format["verbose"])
            fix, ax = self._generateNewGraph(parsed_sim_data, fig, ax, format)

        if format["show_legend"]: ax.legend()
        return fig, ax

    """Generate a new graph on the given axis and figure. Return that new graph"""
    def _generateNewGraph(self, parsed_sim_data, fig, ax, format:dict) -> tuple:
        # create a graph and show data
        if format["show_pos"]:
            ax.plot(
                    parsed_sim_data[self.file_reader.TIME_DATA_HEADER], 
                    parsed_sim_data[self.file_reader.POSITION_DATA_HEADER], 
                    label=self.file_reader.POSITION_DATA_HEADER)
            ax.plot(
                    parsed_sim_data[self.file_reader.TIME_DATA_HEADER],
                    [10.0] * len(parsed_sim_data[self.file_reader.TIME_DATA_HEADER]),                                 # FIXME: read from .csv header data (to implement)
                    label="Target Position")            
        if format["show_vel"]: 
            ax.plot(
                    parsed_sim_data[self.file_reader.TIME_DATA_HEADER],
                    parsed_sim_data[self.file_reader.VELOCITY_DATA_HEADER], 
                    label=self.file_reader.VELOCITY_DATA_HEADER)
            ax.plot(
                    parsed_sim_data[self.file_reader.TIME_DATA_HEADER],
                    [0.0] * len(parsed_sim_data[self.file_reader.TIME_DATA_HEADER]),                                 # FIXME: read from .csv header data (to implement)
                    label="Target Velocity")
        if format["show_applied_force"]:
            ax.plot(
                parsed_sim_data[self.file_reader.TIME_DATA_HEADER], 
                parsed_sim_data[self.file_reader.FORCE_DATA_HEADER], 
                label=self.file_reader.FORCE_DATA_HEADER)
        return fig, ax

    """Create initial window with subplots inside of it. Only called once"""
    def createWindow(self, format:dict, rows=1, columns=1) -> tuple:
        if not(format["show_pos"] or format["show_vel"] or format["show_applied_force"]):
            print("ERROR ENCOUNTERED WHEN GENERATING GRAPH: No data instructed to display. \
                  \nDid you forget to set show_pos, etc. to True?")
            raise Exception
        
        fig, ax = plt.subplots(nrows=rows, ncols=columns, layout="constrained")
        ax.xaxis.set_label_text("Time dt (Seconds)")
        ax.set_title(self.PLOT_TITLE)
        return fig, ax
        
    def setAnimationTime(self, duration_ms:int):
        # make sure plot is supposed to be animated (encapsulation, good practice)
        if self.ANIMATED and duration_ms > self.MIN_ANIM_LENGTH_MS and duration_ms < self.MAX_ANIM_LENGTH_MS:
            self.animation_length_ms = duration_ms
        elif not self.ANIMATED:
            print("ERROR ENCOUNTERED WHILE UPDATING ANIMATION TIME:\nGraph not supposed to be animated. \
                  Make sure to use the animated flag when running simulation.")
            raise IOError
        else:
            print(f"ERROR ENCOUNTERED WHILE UPDATING ANIMATION TIME. \
                  \nInputed time {duration_ms} must be within {self.MIN_ANIM_LENGTH_MS} and \
                    {self.MAX_ANIM_LENGTH_MS} milliseconds.\n")
            raise IOError
