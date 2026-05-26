"""
The purpose of this file is to extract the final simulation data
saved to .csv format by src/file_conv.cpp. This file takes and reads
the data to be used by data_viz.py, which handles the actual GUI
"""

import pandas as pd # pyright: ignore[reportMissingModuleSource]

class FileReader:
    def __init__(self):
        self.file_line_count : int
        self.BUFFER_LINES_COUNT = 3
        self.POSITION_DATA_HEADER = "Position"
        self.VELOCITY_DATA_HEADER = "Velocity"
        self.FORCE_DATA_HEADER = "Force"
        self.TIME_DATA_HEADER = "Time"

    def parseSimData(self, sim_data_path, debug=False) -> pd.DataFrame:
        try:
            parsed_data = pd.read_csv(sim_data_path,
                                        header=0,
                                        comment="#",
                                        names=[self.TIME_DATA_HEADER, self.FORCE_DATA_HEADER, self.POSITION_DATA_HEADER, self.VELOCITY_DATA_HEADER],
                                        dtype=float,
                                        skip_blank_lines=True)
            
            if debug:
                header_data:str = "(No header data found)"
                with open(sim_data_path) as file:
                    header_data = file.readline(1000)    # 1000 is arbitrary, there will never be this many chars on first line

                print("\n" + "="*15 + "DEBUG DATA" + "="*15)
                print(f"Parsing data in {str(sim_data_path.name)}:")
                print(f"PARAMS --{header_data[1:]}")
                print(f"Columns loaded: {parsed_data.columns.tolist()}")
                print("\nFirst 10 rows of data:")
                print(parsed_data.head(10))
                print("="*40)

            return parsed_data  
        
        except FileNotFoundError as e:
            print("\nERROR ENCOUNTERED WHEN PARSING FILE: simulation data file could not be found.")
            print("Failed to find", sim_data_path, ", check the path or name. Is is in the data/ folder?\n")
            print(e)
            exit(1)

        except Exception as e:
            print("\nERROR OCURRED WHEN OPENING FILE FOR DATA PARSING:\n")
            print(e)
            exit(1)

    def collectSimSettings(self):
        # return a python dict of all simulation settings like target position, etc. from
        # the sim_config.json file
        pass
