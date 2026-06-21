# PID Simulation — Spring-Damper System

[![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](#getting-started)
[![Python 3.12](https://img.shields.io/badge/Python-3.12-blue.svg)](#getting-started)
[![Build: CMake](https://img.shields.io/badge/Build-CMake-064F8C.svg)](#getting-started)
[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)

A from-scratch PID controller simulation written in C++20, paired with a Python toolkit for running batches of simulations and visualizing the results with matplotlib. Given a target position, gains (`kp`, `ki`, `kd`), and a few physical parameters, the simulator computes how a controlled point mass moves over time and writes the result to CSV for inspection.

## About

This is my first C++ project, built without AI assistance as a way to learn both modern C++ and the fundamentals of PID control. It started as a personal learning exercise and has grown into something I'm also using as a portfolio piece, so it's written to be buildable and usable by someone other than me.

At its core, the simulation answers one question: given a point mass starting at rest at some position, how does a PID controller drive it to a target position over time, and how do different `kp`/`ki`/`kd` gains change that behavior? The C++ side runs the physics and control loop as fast as possible and logs every timestep to CSV; the Python side handles config discovery, batch runs, and plotting, so you can compare many tunings side-by-side without touching C++.

## Demo / Example Output

![Position and velocity data displayed.](docs/images/pos_and_vel.png)
![Example configs available, like gradually improving PID tunings.](docs/images/pos_improving.png)
![Examples of poor and improved tuned PID's.](docs/images/pos_varying.png)
![Compare multiple PID tunings of slight variance.](docs/images/pos_weak.png)

## Features

- **Config-driven simulations** — define physical properties, PID gains, and simulation timing entirely in JSON, with sensible defaults for anything you omit.
- **Batch runs** — point `run.py` at a single config, a list of configs, or an entire folder of configs in one command.
- **CSV telemetry** — every simulation writes a CSV with time, applied force, position, and velocity at each timestep, plus a header comment recording the gains used and the wall-clock run time.
- **Flexible visualization** — plot position, velocity, and/or applied force independently; overlay multiple runs on one graph or split them into a grid of up to 9 subplots.
- **Animated plots** — render a run as an animation over a configurable duration instead of a static figure.
- **Built-in tuning examples** — pre-built config folders demonstrating weak, sharp, and improving PID tunings, plus single-gain comparisons (see [Configuration Reference](#configuration-reference)).
- **Safety caps with overrides** — both the simulation runner and the visualizer cap batch size by default (50 simulations) to avoid accidentally launching/plotting a huge batch; `--exempt` bypasses this.
- **Dual kill switches** — every simulation stops either when it reaches its configured simulated duration or when it exceeds a wall-clock time budget, whichever comes first, so a runaway config can't hang the program.

## How It Works

### Physical model

The simulation models a single point mass, free to move along one axis, acted on by:

- a constant gravitational force, `F_gravity = mass * gravity_accel`
- a PID-controlled applied force, recalculated every timestep

Net acceleration is computed as:

```
a = (F_applied - F_gravity) / mass
```

and integrated forward with simple Euler steps each timestep `dt`:

```
velocity += a * dt
position += velocity * dt
```

(`src/phys_sim.cpp`)

> **Note:** despite the repository name, the current physics model does **not** implement spring or damper forces — there's no position- or velocity-proportional restoring force beyond what the PID controller itself applies. The model today is gravity + point mass + PID force only. Spring-damper dynamics are a planned addition — see [Known Issues / Roadmap](#known-issues--roadmap).

### PID controller

`PIDCalculator` (`include/PID.hpp`, `src/PID.cpp`) computes the applied force each timestep as:

```
error      = target_pos - position
sumError  += error * dt
force      = kp * error + ki * sumError - kd * velocity
```

The derivative term acts directly on velocity (rather than on the derivative of the error) and serves as a braking force. There is currently no anti-windup or output clamping on the integral term.

### Simulation loop

`main.cpp` ties everything together for a single run:

1. Load and parse the JSON config file (path given as `argv[1]`) into a `SimConfig`.
2. Construct a `PIDCalculator` (loads gains + target from the config) and a `PhysicsSim`.
3. Run the simulation loop (`PhysicsSim::beginSimulation`), which on every tick:
   - asks the PID controller for the current applied force,
   - integrates velocity and position,
   - records a data point.
4. The loop stops on whichever happens first:
   - **simulated time** reaches `simulated_duration`, or
   - **wall-clock time** exceeds `max_seconds` (checked roughly every 200 ticks).
5. Write all recorded data points to the CSV path given as `argv[2]`.

### Simulation output format

Each run produces one CSV file shaped like this:

```
# kp: 50 ki: 7 kd: 30 Duration: 12.3456 ms
Time,Force,Position,Velocity
0,0,0,0
0.01,245.5,0.0123,1.227
...
```

- Line 1 is a comment recording the gains used and (if `record_sim_duration` is enabled) the wall-clock duration of the run, in milliseconds.
- Line 2 is the column header.
- Every subsequent line is one simulation timestep.

## Project Structure

```
pid_proj/
├── CMakeLists.txt        # C++20 build config; produces build/new_test
├── LICENSE
├── README.md
├── include/               # C++ headers
│   ├── PID.hpp
│   ├── phys_sim.hpp
│   ├── file_conv.hpp
│   └── sim_config.hpp
├── src/
│   ├── main.cpp           # Entry point: loads config, runs sim, writes CSV
│   ├── PID.cpp
│   ├── phys_sim.cpp
│   ├── file_conv.cpp
│   └── python/             # CLI + visualization tooling
│       ├── run.py          # Runs the C++ simulation from JSON configs
│       ├── viz.py          # Visualizes resulting CSVs with matplotlib
│       ├── data_visualizer.py
│       ├── file_reader.py
│       └── tools.py
├── external/
│   └── json.hpp            # nlohmann/json v3.11.2 (header-only)
├── config/                 # JSON simulation configs
│   ├── min_config.json
│   ├── std_config.json
│   ├── compare_pids/
│   ├── improving_pids/
│   ├── sharp_pids/
│   └── weak_pids/
└── data/                   # Generated CSV output (git-ignored)
```

## Getting Started

### Prerequisites

- CMake ≥ 3.14
- A C++20 compiler (e.g. GCC 13+ or Clang 16+ — the project uses `std::format`)
- Python 3.12+
- `pandas` and `matplotlib`

```bash
python -m venv .venv
source .venv/bin/activate
pip install pandas matplotlib
```

### Build

```bash
cmake -B build -S .
cmake --build build
```

This produces the `new_test` executable in `build/`. You generally won't invoke it directly — `run.py` builds it automatically if `build/` doesn't exist yet, and handles passing it the right paths.

### First run

```bash
python src/python/run.py -c std_config.json
python src/python/viz.py -s std_config.csv -p -ve -af
```

The first command runs the simulation defined in `config/std_config.json` and writes `data/std_config.csv`. The second opens a matplotlib window plotting position, velocity, and applied force from that CSV.

## Configuration Reference

Every config is a JSON file with four optional top-level sections. Any field you omit falls back to the default below.

| Section | Field | Type | Default | Description |
|---|---|---|---|---|
| `simulation` | `dt` | float | `0.01` | Integration timestep, in seconds |
| `simulation` | `gravity_accel` | float | `9.81` | Gravitational acceleration applied to the mass, in m/s² |
| `simulation` | `max_seconds` | float | `0.8` | Wall-clock time budget, in seconds — the run is aborted if it takes longer than this to *compute*, regardless of simulated duration |
| `simulation` | `simulated_duration` | float | `5.0` | Length of simulated time to run, in seconds |
| `physical_props` | `mass_kg` | float | `5.0` | Mass of the simulated point object, in kg |
| `physical_props` | `start_pos` | float | `0.0` | Starting position, in meters |
| `physical_props` | `target_pos` | float | `10.0` | Target position (setpoint) the PID controller drives toward, in meters |
| `pid_gains` | `kp` | float | `0.0` | Proportional gain |
| `pid_gains` | `ki` | float | `0.0` | Integral gain |
| `pid_gains` | `kd` | float | `0.0` | Derivative gain |
| `data_handling` | `record_sim_duration` | bool | `true` | Whether to record the wall-clock run time in the CSV header |

`config/min_config.json` shows the minimum viable config — it only sets PID gains and relies on defaults for everything else:

```json
{
    "simulation": {},
    "physical_props": {},
    "pid_gains": {
        "kp": 50.0,
        "ki": 7.0,
        "kd": 30.0
    },
    "data_handling": {}
}
```

`config/std_config.json` is a fully-specified example:

```json
{
    "simulation": {
        "gravity_accel": 9.81,
        "dt": 0.01,
        "max_seconds": 0.8,
        "simulated_duration": 5.0
    },
    "physical_props": {
        "mass_kg": 5,
        "start_pos": 0.0,
        "target_pos": 10.0
    },
    "pid_gains": {
        "kp": 50.0,
        "ki": 7.0,
        "kd": 30.0
    },
    "data_handling": {
        "record_sim_duration": true
    }
}
```

### Config subfolders

| Folder | Demonstrates |
|---|---|
| `compare_pids/` | Isolates a single gain at a time (`heavy_kp.json`, `heavy_ki.json`, `heavy_kd.json`) to show its individual effect on the response |
| `improving_pids/` | A progressive sequence of five configs showing iterative tuning improvements |
| `sharp_pids/` | High-gain, aggressive tunings that settle quickly but tend toward overshoot/oscillation |
| `weak_pids/` | Low-gain tunings that respond slowly and sluggishly |

## CLI Reference

### `run.py` — run simulations

```
python src/python/run.py [OPTIONS]
```

| Flag | Alias | Type | Default | Description |
|---|---|---|---|---|
| `--config` | `-c` | str (one or more) | *(required)* | Config file name(s) or a single folder name under `config/` (e.g. `weak_pids/`) |
| `--exempt` | `-e` | flag | `False` | Bypass the 50-simulation safety cap |
| `--verbose` | `-v` | flag | `False` | Print debug data from the C++ simulation |
| `--showtime` | `-t` | flag | `False` | Print timing info after all simulations finish |
| `--folder` | `-fo` | str | `''` | Write output CSVs to `data/<name>/` instead of directly into `data/` |

```bash
# Run a single config with debug output
python src/python/run.py -c std_config.json -v

# Run every config in weak_pids/, writing CSVs to data/weak/
python src/python/run.py -c weak_pids/ --folder weak

# Run more than 50 simulations at once
python src/python/run.py -c compare_pids/ improving_pids/ sharp_pids/ weak_pids/ --exempt
```

### `viz.py` — visualize results

```
python src/python/viz.py [OPTIONS]
```

| Flag | Alias | Type | Default | Description |
|---|---|---|---|---|
| `--specify` | `-s` | str (one or more) | all `.csv` in `data/` | CSV file name(s) or a folder (relative to `data/`) to visualize |
| `--divided` | `-d` | flag | `False` | Show each file in its own subplot (up to 9) instead of one combined graph |
| `--showpos` | `-p` | flag | `False` | Plot position |
| `--showvel` | `-ve` | flag | `False` | Plot velocity |
| `--showappliedforce` | `-af` | flag | `False` | Plot applied force |
| `--nolegend` | `-nl` | flag | `False` | Hide the legend |
| `--exempt` | `-e` | flag | `False` | Bypass the 50-simulation combined-graph cap |
| `--animated <ms>` | — | int | `0` | Animate the plot over the given duration, in milliseconds |
| `--verbose` | `-v` | flag | `False` | Print debug info while parsing CSVs |

```bash
# Visualize a single result, showing all three traces
python src/python/viz.py -s std_config.csv -p -ve -af

# Compare every config in two tuning folders, one subplot per file
python src/python/viz.py -s weak_pids/ sharp_pids/ -d -p

# Animate a single run over 4 seconds
python src/python/viz.py -s std_config.csv -p --animated 4000
```

## Example Workflow

1. **Pick or write a config.** Start from `config/min_config.json` or `config/std_config.json` and adjust `kp`/`ki`/`kd`, `target_pos`, etc.
2. **Run it:**
   ```bash
   python src/python/run.py -c my_config.json
   ```
3. **Visualize it:**
   ```bash
   python src/python/viz.py -s my_config.csv -p -ve -af
   ```
4. **Compare tunings.** Run a whole folder of configs and view them together:
   ```bash
   python src/python/run.py -c weak_pids/ sharp_pids/
   python src/python/viz.py -s weak_pids/ sharp_pids/ -d -p
   ```

## Known Issues / Roadmap

- **No spring-damper dynamics yet.** The repository name reflects the original goal of modeling a spring-damper system, but the implemented physics is currently gravity + point mass + PID force only. Adding actual spring (position-proportional) and damper (velocity-proportional) forces to `phys_sim.cpp` is the main planned extension.
- **No integral anti-windup.** `sumError` in `PIDCalculator` accumulates without any clamping, so heavily saturated or long-running configs can experience integral windup.
- **Config key typo (fixed).** Earlier versions of every config file (except `min_config.json`) used the key `target_post` instead of `target_pos`, which `sim_config.hpp` actually reads — meaning the configured target position was silently ignored in favor of the 10.0 default. This has been corrected in all config files in this repository; if you're working from an older clone or a copied config, double-check the key name.
- **`--showtime` prints the wrong line.** `run.py`'s `-t/--showtime` flag intends to show each run's duration, but it currently skips the CSV's comment header (which contains the duration) and prints the column-title line instead. See the `FIXME` in `run.py`.

## License

[MIT](LICENSE)
