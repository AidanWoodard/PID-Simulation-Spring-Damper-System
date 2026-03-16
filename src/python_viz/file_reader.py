"""
The purpose of this file is to extract the final simulation data
saved to .csv format by src/file_conv.cpp. This file takes and reads
the data to be used by data_viz.py, which handles the actual GUI
"""

class FileReader:
    def __init__(self):
        self.FILE_PATH = "~/data/telemetry.csv"

    def readLine(self, open_file, line_num):
        # return formatted data from current line ('line_num')
        pass

    def openFile(self, file_path):
        # find and open the file, use safe catches if file misplaced
        pass

    def closeFile(self, open_file):
        # handle closing the file (little logic)
        pass

    def collectFileHeaderData(self, open_file):
        # return the section of the file that holds the time the simulation took, the
        # initial paramaters, and other useful information that is stored by file_conv.cpp
        pass
