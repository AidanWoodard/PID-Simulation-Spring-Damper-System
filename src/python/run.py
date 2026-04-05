"""
This short python file handles CLI input to run a c++ PID simulation so
that c++ doesn't have to deal with command line args and relative file
finding. This is handled by python's pathlib library instead.

Note: This file does NOT run the python vizulatization, just the simulation.
"""

import subprocess
from pathlib import Path
import argparse as ap

# arbitrary, but a good precaution
# don't want to accidentally let the user run a massive folder
MAX_SIMULATIONS = 50

# current python script dir and project dir
SCRIPT_DIR = Path(__file__).resolve().parent
PROJECT_ROOT = SCRIPT_DIR.parent.parent

# other paths to important folders and files
EXE_FILE_NAME = "new_test"
CONFIG_FOLDER_PATH = PROJECT_ROOT / "config"
EXE_FOLDER_PATH = PROJECT_ROOT / "build"
EXE_PATH = EXE_FOLDER_PATH / EXE_FILE_NAME
CSV_FOLDER_PATH = PROJECT_ROOT / "data"

def json_to_csv(files) -> list:
    converted = []
    for f in files:
        converted.append(f[:-5] + ".csv")
    return converted

def _check_and_handle_builds():
    """Make sure our build file exists for when we call it later, and create it if it doesn't exist."""
    builds = list(Path(EXE_FOLDER_PATH).glob("./" + EXE_FILE_NAME))
    if not EXE_FOLDER_PATH.is_dir():
        print("WARNING: No build/ folder found. Running CMake commands...")
        subprocess.run(["cmake", 
                        "-B", str(EXE_FOLDER_PATH), 
                        "-S", str(PROJECT_ROOT)], 
                        check=True)
        subprocess.run(["make"], 
                       cwd=str(EXE_FOLDER_PATH), 
                       check=True)  
    elif len(builds) > 1:
        print(f"ERROR: More than one build found named {EXE_FILE_NAME} in build/. Remaking build...")
        subprocess.run(["cmake", 
                        "-B", str(EXE_FOLDER_PATH), 
                        "--target", "clean"], 
                        check=True)
        subprocess.run(["make"], 
                       cwd=str(EXE_FOLDER_PATH), 
                       check=True)
    elif len(builds) == 0: 
        print(f"Build file {EXE_FILE_NAME} not found. Running make command...")
        subprocess.run(["make"], 
                       cwd=str(EXE_FOLDER_PATH), 
                       check=True)

def _create_custom_parser() -> ap.ArgumentParser:
    # Read our arguments from the CLI
    parser = ap.ArgumentParser(
        prog="PID_Simulation",
        description="Simple PID simulation with visualization in Python.",
        epilog="Use the above flags and args to run the simulation. For the required --config command,\n" \
            "input the names of the .json configurations for the simulations you would like to run. The \n" \
                "maximum number of simulations you can run at one time is 50, but use --exempt to override.\n" \
                "The -t flag will show the wall time needed to run all configurations simulated.")

    parser.add_argument('-c', '--config', required=True, nargs='+',           help="Run the simulation using a specific config (REQUIRED). Either file names (standard_example.json) or a single directory (weak_pid_examples/)")
    parser.add_argument('-e', '--exempt', action='store_true',                help="Use this flag if you want to ignore max of 50 simulations at one time.")
    parser.add_argument('-v', '--verbose', action='store_true',               help="Run with debug data shown.")
    parser.add_argument('-t', '--showtime', action='store_true',              help="Show final duration of simulation in wall time.")
    # TODO: add ability to create a folder to store all final .csv data with the --folder <name>
    # This way, the use will be able to enter pidviz --specify <name>/ and run all .csv

    return parser

if (__name__) == "__main__":
    _check_and_handle_builds()
    parser = _create_custom_parser()
    args = parser.parse_args()
    configs:str = []
    targets:str = []    # where to write simulation data

    # Parse all of our command line arguments
    if len(args.config) < MAX_SIMULATIONS or args.exempt:
        if len(args.config) == 1 and ".json" not in args.config[0]:
            if args.config[0][-1] == '/':
                print(f"Running all simulations in folder {args.config[0]}.")
                config_folder = Path(CONFIG_FOLDER_PATH / args.config[0])
                configs = list(config_folder.glob("*.json"))
                targets = list(c.name for c in config_folder.glob("*.json"))
            else:
                print("Please use '<folder_name>/' format to denote a folder of configs to simulate. Use --help for more.")
                exit(1)
        else:
            for c in args.config:
                if ".json" not in c:
                    print("ERROR: all configs must be of type .json or a directory")
                    exit(1)
                print(f"Preparing to start simulation for config {c}...")
                configs.append(Path(CONFIG_FOLDER_PATH / c))
                targets.append(c)
        if args.verbose: print("verbose mode")
        if args.showtime: print("show wall time")
    else:
        print(f"ERROR: {MAX_SIMULATIONS} simulations maximum. {len(args.config)} simulations requested. Use --exempt to override.")

    # 'convert' filename to .csv
    targets = json_to_csv(targets)

    # run each simulation
    for i, config in enumerate(configs):
        subprocess.run([str(EXE_PATH),
                        "-config", str(CONFIG_FOLDER_PATH / config),
                        "-target", str(CSV_FOLDER_PATH / targets[i])], check=True)
        