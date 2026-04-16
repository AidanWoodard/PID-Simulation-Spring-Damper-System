"""
This program holds basic tools used primarily by files like viz.py and run.py for tasks
like getting a list of paths for files of a specific extension (.csv/.json). 
"""

from pathlib import Path

def change_extension(filepath:Path, extension:str) -> Path:
    return Path((str(filepath.stem) + extension))

def get_data_files(user_args, file_extension:str, head_dir:Path) -> list:
    """Get a list of all data files to run or display simulations on. Accepts from user both file names and folders"""
    data_file_paths = []
    extension_char_len = len(file_extension)

    # loop through all user arguments, either file names or folder names to search through
    for i, data_file in enumerate(user_args):
        if file_extension == data_file[-extension_char_len:]:       # everything but extension, just name
            data_file_paths.append(Path(head_dir / data_file))
        elif data_file[-1] == '/' or data_file == '.':              # add everything in folder
            data_folder_to_search = Path(head_dir / data_file)
            new_data_files = list(file.name for file in data_folder_to_search.glob("*" + file_extension))
            for path in new_data_files: data_file_paths.append(Path(head_dir / data_file / path))
        else:
            print("ERROR: all data files must be of type .csv/.json or a directory with a backslash.")
            exit(1)
        print(f"Preparing to display simulation for data file {data_file}...")

    return data_file_paths
