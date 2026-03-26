"""
This file will handle running either the simulation and the graphics or JUST the graphics.
This way we can edit settings of the data visualization (handled by data_viz.py) and not
have to rerun the same simulation over and over. Good practice, though c++ is so fast that
it's technically unnecessary.
"""

from typing import final

import data_viz as dv, file_reader as fr

if __name__ == "__main__":
    file_reader = fr.FileReader()
    data_vizualizer = dv.DataVisualizer(file_reader) 

    data_vizualizer.generateGraph(show_applied_force=True, show_pos=True, show_vel=True)
