"""
The purpose of this file is to extract the final simulation data
saved to .csv format by src/file_conv.cpp. This file takes and reads
the data to be used by data_viz.py, which handles the actual GUI
"""

import numpy as np

class FileReader:
    def __init__(self):
        self.FILE_PATH = "../../data/test.csv"
        self.data_file = None
        self.file_line_count : int
        self.openFile
        self.BUFFER_LINES_COUNT = 3

    def openFile(self, file_path):
        # find and open the file, use safe catches if file misplaced
        try:
            self.data_file = open(file_path, mode='r')
            # https://stackoverflow.com/questions/845058/how-to-get-the-line-count-of-a-large-file-cheaply-in-python
            self.file_line_count = sum(1 for _ in self.data_file) - self.BUFFER_LINES_COUNT

        except FileNotFoundError as e:
            print("\nSIMULATION DATA FILE COULD NOT BE FOUND")
            print("Failed to find", file_path, ", check the path or name")

        except Exception as e:
            print("\nERROR OCURRED WHEN OPENING FILE")
            print(e)

    def closeFile(self):
        if self.data_file:
            self.data_file.close()
        else:
            print("\nERROR OCURRED: File was never found or opened.")

    def parsePositionData(self) -> list:
        return np.ndarray(shape=(2,), dtype=int)
    # def parsePositionData(self) -> np.array:
    #     # pos versus delta time
    #     posData = np.array([[], []])

    #     for i in range(self.file_line_count):
    #         new_data_pt = self._readLine(self.data_file, i)
    #         # TODO: add data to posData and return

    def parseVelocityData(self) -> np.array:
        # vel versus dt
        return [.1, .2, .3, .4], [.4, .3, .2, .1]   
        pass

    def parseAppliedForceData(self) -> np.array:
        # applied F versus dt
        pass

    def _closeFile(self, open_file):
        # handle closing the file (little logic)
        pass

    def _readLine(self, open_file, line_num) -> np.array:
        # return formatted data from current line ('line_num')
        pass

    def _collectFileHeaderData(self, open_file):
        # return the section of the file that holds the time the simulation took, the
        # initial paramaters, and other useful information that is stored by file_conv.cpp
        pass
