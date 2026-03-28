"""
The purpose of this file is to extract the final simulation data
saved to .csv format by src/file_conv.cpp. This file takes and reads
the data to be used by data_viz.py, which handles the actual GUI
"""

import pandas as pd

class FileReader:
    def __init__(self):
        self.FILE_PATH = "../../data/telemetry.csv"
        self.data_file = None
        self.file_line_count : int
        self.BUFFER_LINES_COUNT = 3
        self.POSITION_DATA_HEADER = "Position"
        self.VELOCITY_DATA_HEADER = "Velocity"
        self.FORCE_DATA_HEADER = "Force"
        self.TIME_DATA_HEADER = "Time"

    def openDataFile(self, file_path):
        # find and open the file, use safe catches if file misplaced
        try:
            self.data_file = open(file_path, mode='r')
            # https://stackoverflow.com/questions/845058/how-to-get-the-line-count-of-a-large-file-cheaply-in-python
            self.file_line_count = sum(1 for _ in self.data_file) - self.BUFFER_LINES_COUNT
            self.closeDataFile()

        except FileNotFoundError as e:
            print("\nSIMULATION DATA FILE COULD NOT BE FOUND")
            print("Failed to find", file_path, ", check the path or name")
            print(e)

        except Exception as e:
            print("\nERROR OCURRED WHEN OPENING FILE")
            print(e)

    def closeDataFile(self):
        if self.data_file:
            self.data_file.close()
        else:
            print("\nERROR OCURRED: File was never found or opened.")

    def parseSimData(self) -> pd.DataFrame:
        return pd.read_csv(self.FILE_PATH,
                                   header=0,
                                    names=[self.TIME_DATA_HEADER, self.FORCE_DATA_HEADER, self.POSITION_DATA_HEADER, self.VELOCITY_DATA_HEADER],
                                    dtype=float,
                                    skip_blank_lines=True)

    def collectSimSettings(self):
        # return a python dict of all simulation settings like target position, etc. from
        # the sim_config.json file
        pass
