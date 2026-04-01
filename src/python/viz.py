"""
This file will handle running either the simulation and the graphics or JUST the graphics.
This way we can edit settings of the data visualization (handled by data_viz.py) and not
have to rerun the same simulation over and over. Good practice, though c++ is so fast that
it's technically unnecessary.
"""

from pathlib import Path
import argparse as ap

import data_viz as dv, file_reader as fr

if __name__ == "__main__":
    SCRIPT_DIR = Path(__file__).resolve().parent
    DATA_DIR = SCRIPT_DIR.parent.parent / "data"

    parser = ap.ArgumentParser(
        prog="PID_Visualizer",
        description="Simple PID simulation with visualization in Python.",
        epilog="Use the above flags and args to visualize and graph simulation data. ,\n" \
            " \n" \
                " \n" \
                "")

    parser.add_argument('-d', '--divided', action='store_true',             help="Use this flag to display up to 4 graphs in separate graphs rather than one combined graph.")
    parser.add_argument('-s', '--specify', nargs='+',                       help="List specific .csv files in data/ to display. They have same names as their config.json files.")
    parser.add_argument('-p', '--showpos', action='store_true',             help="Display position data")
    parser.add_argument('-v', '--showvel', action='store_true',             help="Display velocity data.")
    parser.add_argument('-af', '--showappliedforce', action='store_true',   help="Display applied force data.")
    parser.add_argument('-nl', '--nolegend', action='store_true',           help="Display graph without legend.")

    args = parser.parse_args()

    # TODO: impolement above args
    to_display = ["weak_pid_config.csv", "sharp_pid_config.csv"]
    for file in to_display:
        file_reader = fr.FileReader(DATA_DIR / file)
        data_vizualizer = dv.DataVisualizer(file_reader) 
        data_vizualizer.generateGraph(show_vel=True, show_pos=True)
