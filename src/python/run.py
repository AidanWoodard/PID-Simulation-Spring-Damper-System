"""
This short python file handles CLI input to run a c++ PID simulation so
that c++ doesn't have to deal with command line args and relative file
finding. This is handled by python's pathlib library instead.

Note: This file does NOT run the python vizulatization, just the simulation.
"""

import subprocess
from pathlib import Path
import argparse as ap

# arbitrary, but a good precaution. Don't want to accidentally let the user run a massive folder
MAX_SIMULATIONS = 50

# current python script dir and project dir
SCRIPT_DIR = Path(__file__).resolve().parent
PROJECT_ROOT = SCRIPT_DIR.parent.parent

# TODO: check for ./new_test build and create one if necessary with cmake commands
######### here

# other paths we need
CONFIG_PATH = PROJECT_ROOT / "config"
EXE_PATH = PROJECT_ROOT / "build" / "./new_test"

parser = ap.ArgumentParser(
    prog="PID_Simulation",
    description="Simple PID simulation with visualization in Python.",
    epilog="Use the above flags and args to run the simulation. For the required --config command,\n" \
        "input the names of the .json configurations for the simulations you would like to run. The \n" \
            "maximum number of simulations you can run at one time is 50, but use --exempt to override.\n" \
            "The -t flag will show the wall time needed to run all configurations simulated.")

parser.add_argument('--config', required=True, nargs='+',           help="Run the simulation using a specific config (REQUIRED). Either file names (standard_example.json) or a single directory (weak_pid_examples)")
parser.add_argument('--exempt', action='store_true',                help="Use this flag if you want to ignore max of 50 simulations at one time.")
parser.add_argument('-v', '--verbose', action='store_true',         help="Run with debug data shown.")
parser.add_argument('-t', '--showtime', action='store_true',        help="Show final duration of simulation in wall time.")

args = parser.parse_args()
configs:str = []

if len(args.config) < MAX_SIMULATIONS or args.exempt:
    if len(args.config) == 1 and ".json" not in args.config[0]:
        print(f"Running all simulations in folder {args.config[0]}.")
        configs = list(Path(CONFIG_PATH / args.config[0]).glob("*.json"))
    else:
        for c in args.config:
            if ".json" not in c:
                print("ERROR: all configs must be of type .json.")
                exit(1)
            print(f"Preparing to start simulation for config {c}...")
            configs.append(Path(CONFIG_PATH / c))
    if args.verbose: print("verbose mode")
    if args.showtime: print("show wall time")
else:
    print(f"ERROR: {MAX_SIMULATIONS} simulations maximum. {len(args.config)} simulations requested. Use --exempt to override.")
    exit(1)
print(configs)