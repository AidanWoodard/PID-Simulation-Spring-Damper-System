"""
This file will handle running either the simulation and the graphics or JUST the graphics.
This way we can edit settings of the data visualization (handled by data_viz.py) and not
have to rerun the same simulation over and over. Good practice, though c++ is so fast that
it's technically unnecessary.
"""

from pathlib import Path
import argparse as ap
import matplotlib.pyplot as plt

import data_viz as dv, file_reader as fr

def _get_graph_row_col(num_graphs:int) -> list:
    """Return the format of graphs in the form [row, col]. Currently limited to 3x3 graph layout."""
    dimensions = []
    if num_graphs <= 0:
        print("ERROR: Fewer than 1 graphs attempted to display.\nUse --specify <name.csv> or --specify <folder>/ to ensure you're displaying data.\nOmiting --specify will default to all .csv files in data/ folder, excluding subfolders.")
        exit(1)
    if num_graphs <= 2: return [1, 2]       # 2x1 two graphs side by side
    elif num_graphs <= 4: return [2, 2]     # 2x2 four graph layout
    elif num_graphs <= 6: return [2, 3]     # 3x2 six graphs layout
    elif num_graphs <= 9: return [3, 3]
    elif num_graphs >= MAX_SIMULATIONS_DIVIDED_DISPLAY:
        print(f"ERROR ENCOUNTERED WHEN DISPLAYING DIVIDED GRAPHS: Maximum number of graphs to display divided is {MAX_SIMULATIONS_DIVIDED_DISPLAY} graphs.\nOmit the --divided flag to display more than {MAX_SIMULATIONS_DIVIDED_DISPLAY} graphs in one combined graph.")
        exit(1)
    else:
        print(f"ERROR ENCOUNTERED WHEN DISPLAYING DIVIDED GRAPHS: {num_graphs} graphs requested to display in divided format.\nUse --help to learn more about how to display in divided graphs format.")
        exit(1)

def _create_custom_parser() -> ap.ArgumentParser:
    parser = ap.ArgumentParser(
        prog="PID_Visualizer",
        description="Simple PID simulation with visualization in Python.",
        epilog="Use the above flags and args to visualize and graph simulation data. ,\n" \
            "The --specify <name.csv> is not required but the visualization will default to all data in\n" \
                "'data/' folder. It is highly recommended to specify which .csv files or which folder\n" \
                "should be displayed. Use --help when running simulations to learn how to create a data folder.")

    parser.add_argument('-d', '--divided', action='store_true',             help=f"Use this flag to display up to {MAX_SIMULATIONS_DIVIDED_DISPLAY} graphs in separate graphs rather than one combined graph.")
    parser.add_argument('-s', '--specify', nargs='+', default=".",          help="List specific .csv files in data/ to display. They have same names as their config.json files.")
    parser.add_argument('-p', '--showpos', action='store_true',             help="Display position data")
    parser.add_argument('-ve', '--showvel', action='store_true',            help="Display velocity data.")
    parser.add_argument('-af', '--showappliedforce', action='store_true',   help="Display applied force data.")
    parser.add_argument('-nl', '--nolegend', action='store_true',           help="Display graph without legend.")
    parser.add_argument('-e', '--exempt', action='store_true',              help=f"Use this flag if you want to ignore max of {MAX_SIMULATIONS_SUGGESTED} simulations displayed in one graph.")
    parser.add_argument('--animated', type=int, default=0,                  help="Use this flag to display the graph animated over an inputted millisecond duration.")
    parser.add_argument('-v', "--verbose", action="store_true",             help="Use this flag for basic debug data when parsing .csv data files.")

    return parser

def _get_data_files(requested_files:list=None, requested_folder:str=None) -> list:
    """Get a list of all .csv data files to run simulations on. Find their paths and return as a list"""
    if not requested_files and not requested_folder:
        print("ERROR ENCOUNTERED WHEN FINDING .CSV FILES: No .csv files or folders requested to search for.\nDid you forget to pass argparser data?")
        raise Exception
    # TODO: implement checking for .csv files/opening folders and adding all to return list in Path form
    pass

def _parse_graph_format(args) -> dict:
    new_format = {}
    new_format["show_pos"] = args.showpos
    new_format["show_vel"] = args.showvel
    new_format["show_applied_force"] = args.showappliedforce
    new_format["show_legend"] = not(args.nolegend)
    new_format["animated"] = (args.animated > 0)
    new_format["verbose"] = args.verbose
    return new_format

if __name__ == "__main__":
    SCRIPT_DIR = Path(__file__).resolve().parent
    DATA_DIR = SCRIPT_DIR.parent.parent / "data"

    MAX_SIMULATIONS = 200                       # allow this many if --exempt flag used
    MAX_SIMULATIONS_SUGGESTED = 50              # allow up to 50 different simulations combined
    MAX_SIMULATIONS_DIVIDED_DISPLAY = 9         # max simulations when diplayed in separate graphs

    # handle arguments and create a format dictionary to pass to the visualizer
    parser = _create_custom_parser()
    args = parser.parse_args()
    data_file_paths = []
    graph_format = _parse_graph_format(args)

    # collect all of our data files and folders into data_file_paths
    for i, data_file in enumerate(args.specify):
        if ".csv" == data_file[-4:]:
            data_file_paths.append(Path(DATA_DIR / data_file))
        elif data_file[-1] == '/' or data_file == '.':
            data_folder_to_search = Path(DATA_DIR / data_file)
            new_data_files = list(file.name for file in data_folder_to_search.glob("*.csv"))
            for path in new_data_files: data_file_paths.append(Path(DATA_DIR / data_file / path))
        else:
            print("ERROR: all data files must be of type .csv or a directory with a backslash.")
            exit(1)
        print(f"Preparing to display simulation for data file {data_file}...")

    file_reader_obj = fr.FileReader()
    data_vizualizer = dv.DataVisualizer(file_reader_obj, animated=graph_format["animated"], show_legend=graph_format["show_legend"])

    fig, ax = data_vizualizer.createWindow(graph_format)
    fig, ax = data_vizualizer.generateGraphs(data_file_paths, fig, ax, format=graph_format)
    plt.show()
