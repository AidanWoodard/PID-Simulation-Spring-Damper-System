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

def _create_custom_parser() -> ap.ArgumentParser:
    parser = ap.ArgumentParser(
        prog="PID_Visualizer",
        description="Simple PID simulation with visualization in Python.",
        epilog="Use the above flags and args to visualize and graph simulation data. ,\n" \
            "The --specify <name.csv> is not required but the visualization will default to all data in\n" \
                "'data/' folder. It is highly recommended to specify which .csv files or which folder\n" \
                "should be displayed. Use --help when running simulations to learn how to create a data folder.")

    parser.add_argument('-d', '--divided', action='store_true',             help=f"Use this flag to display up to {MAX_SIMULATIONS_DIVIDED_DISPLAY} graphs in separate graphs rather than one combined graph.")
    parser.add_argument('-s', '--specify', nargs='+',                       help="List specific .csv files in data/ to display. They have same names as their config.json files.")
    parser.add_argument('-p', '--showpos', action='store_true',             help="Display position data")
    parser.add_argument('-ve', '--showvel', action='store_true',             help="Display velocity data.")
    parser.add_argument('-af', '--showappliedforce', action='store_true',   help="Display applied force data.")
    parser.add_argument('-nl', '--nolegend', action='store_true',           help="Display graph without legend.")
    parser.add_argument('-e', '--exempt', action='store_true',              help=f"Use this flag if you want to ignore max of {MAX_SIMULATIONS_SUGGESTED} simulations displayed in one graph.")
    parser.add_argument('--animated', type=int, default=0,                  help="Use this flag to display the graph animated over an inputted millisecond duration.")
    parser.add_argument('-v', "--verbose", action="store_true",                   help="Use this flag for basic debug data when parsing .csv data files.")

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
    
    # TODO: add checker and handler if data folder at DATA_DIR doesn't exist yet

    MAX_SIMULATIONS = 200                       # allow this many if --exempt flag used
    MAX_SIMULATIONS_SUGGESTED = 50              # allow up to 50 different simulations combined
    MAX_SIMULATIONS_DIVIDED_DISPLAY = 9         # max simulations when diplayed in separate graphs

    # handle arguments and create a format dictionary to pass to the visualizer
    parser = _create_custom_parser()
    args = parser.parse_args()
    data_file_paths:list = [DATA_DIR / "sharp_pid_config.csv", DATA_DIR / "weak_pid_config.csv"]    # FIXME: temporary
    graph_format = _parse_graph_format(args)
    
    file_reader_obj = fr.FileReader()
    data_vizualizer = dv.DataVisualizer(file_reader_obj, animated=graph_format["animated"], show_legend=graph_format["show_legend"])

    fig, ax = data_vizualizer.createWindow(graph_format)
    fig, ax = data_vizualizer.generateGraphs(data_file_paths, fig, ax, format=graph_format)
    plt.show()
