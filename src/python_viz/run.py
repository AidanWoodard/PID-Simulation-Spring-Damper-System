"""
This file will handle running either the simulation and the graphics or JUST the graphics.
This way we can edit settings of the data visualization (handled by data_viz.py) and not
have to rerun the same simulation over and over. Good practice, though c++ is so fast that
it's technically unnecessary.
"""

from typing import final

import data_viz as dv, file_reader as fr

WINDOW_HEIGHT_PX: final = 800
WINDOW_WIDTH_PX: final = 1200

if __name__ == "__main__":
    data_vizualizer = dv.DataVisualizer(WINDOW_WIDTH_PX, WINDOW_HEIGHT_PX) 
    file_reader = fr.FileReader()

    data_vizualizer.generateGraph(file_reader)