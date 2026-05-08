"""
This short python file handles CLI input to run a c++ PID simulation so
that c++ doesn't have to deal with command line args and relative file
finding. This is handled by python's pathlib library instead.

Note: This file does NOT run the python vizulatization, just the simulation.
"""

import subprocess
from pathlib import Path
import argparse as ap

from tools import change_extension, get_data_files

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

def _get_target_paths_from_configs(configs:list, head_dir:Path) -> list:
    """Create a list of the target folder paths using the file names in configs"""
    targets:Path = []
    curr_filename:str = ""
    for c in configs: targets.append(Path(head_dir / change_extension(c, ".csv")))
    
    return targets

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
    parser.add_argument('-fo', '--folder', type=str, default='',              help="Store all simulation data in a folder of given name (--folder <new_name>).")

    return parser

if (__name__) == "__main__":
    _check_and_handle_builds()
    parser = _create_custom_parser()
    args = parser.parse_args()

    if not CSV_FOLDER_PATH.is_dir():
        print("WARNING: Data/ folder not found in project root, creating a new one...")
        CSV_FOLDER_PATH.mkdir(parents=True, exist_ok=True)

    # if user specified for a target folder to be make for final .csv data, make it (ie --folder my_sims)
    if args.folder != '':
        new_target_folder = CSV_FOLDER_PATH / str(args.folder)
        if not new_target_folder.is_dir():
            new_target_folder.mkdir()
        CSV_FOLDER_PATH = new_target_folder

    # find configs and csv target locations
    configs:Path = get_data_files(args.config, ".json", CONFIG_FOLDER_PATH)
    csv_targets:Path = _get_target_paths_from_configs(configs, CSV_FOLDER_PATH)

    # limit max sims
    if not args.exempt and len(configs) > MAX_SIMULATIONS:
        print(f"ERROR: {MAX_SIMULATIONS} simulations maximum. {len(args.config)} simulations requested. Use --exempt to override.")
        exit(0)

    # run each simulation
    for i, config in enumerate(configs):
        subprocess.run([str(EXE_PATH),
                            str(CONFIG_FOLDER_PATH / config),
                            str(CSV_FOLDER_PATH / csv_targets[i]),
                            str(args.verbose)], check=True)
    
    # display final times after all have finished (in case of verbose mode)
    if args.showtime:
        for target in enumerate(csv_targets):
            with open(target[1]) as f:
                f.readline()            # FIXME: read headers properly, this just skips first line (titles)
                print(f.readline())     # (first line of data, fix later to display time)

