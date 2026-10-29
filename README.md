# Philosophers

A 42 School project exploring concurrency in C through the classic Dining Philosophers problem. The goal is to simulate philosophers sharing forks while managing threads, mutexes, and timing to prevent data races and detect starvation.

## Table of Contents

- [About the Project](#about-the-project)
- [Concepts](#concepts)
- [How It Works](#how-it-works)
- [Requirements](#requirements)
- [Installation](#installation)
- [Usage](#usage)
- [Parameters](#parameters)
- [Examples](#examples)
- [Thread Sanitizer](#thread-sanitizer)
- [Output](#output)
- [Makefile Commands](#makefile-commands)

## About the Project

The simulation consists of philosophers sitting around a circular table with one fork between each pair. Each philosopher is represented by a separate thread and alternates between eating, sleeping, and thinking.

To eat, a philosopher must acquire both adjacent forks. Since forks are shared resources, mutexes are used to control access and prevent multiple philosophers from using the same fork simultaneously.

The simulation ends when a philosopher dies from starvation or when all philosophers have reached the specified meal limit, if one is provided.

## Concepts

This project focuses on the following topics:

- **Threads:** Running each philosopher concurrently using POSIX threads.
- **Mutexes:** Protecting shared resources and synchronizing access to forks.
- **Synchronization:** Coordinating the start and termination of the simulation.
- **Time management:** Tracking elapsed time and monitoring each philosopher's last meal.
- **Race conditions:** Protecting shared data accessed by multiple threads.
- **Resource management:** Initializing, using, and destroying mutexes and allocating and freeing memory.

## How It Works

1. **Initialization:** The program validates the arguments and initializes the simulation data, philosophers, and mutexes.
2. **Simulation start:** A separate thread is created for each philosopher.
3. **Eating:** A philosopher acquires both adjacent forks before eating and releases them afterward.
4. **Sleeping and thinking:** After eating, the philosopher sleeps and then returns to thinking.
5. **Monitoring:** The simulation tracks elapsed time and checks whether a philosopher has exceeded the allowed time without eating.
6. **Termination:** The simulation stops when a philosopher dies or the meal limit is reached by every philosopher.

Each fork is represented by a mutex. Additional mutexes protect shared simulation state, meal information, and output.

## Requirements

- A Unix-like operating system.
- A C compiler with POSIX threads support.
- GNU Make.

## Installation

Clone the repository and navigate to the project directory:

    git clone https://github.com/MoriaProfa/philosophers.git
    cd philosophers

Compile the project:

    make

This creates the `philo` executable.

## Usage

Run the program with the following arguments:

    ./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]

## Parameters

| Parameter | Description |
|---|---|
| `number_of_philosophers` | Number of philosophers and forks. |
| `time_to_die` | Maximum time in milliseconds a philosopher can go without starting a meal before dying. |
| `time_to_eat` | Time in milliseconds required for a philosopher to eat. |
| `time_to_sleep` | Time in milliseconds a philosopher spends sleeping. |
| `number_of_times_each_philosopher_must_eat` | Optional number of meals each philosopher must complete before the simulation stops successfully. |

All time values are expressed in milliseconds.

If the optional meal limit is omitted, the simulation continues until a philosopher dies.

## Examples

Run a simulation with four philosophers:

    ./philo 4 800 200 200

Run five philosophers, each required to eat at least seven times:

    ./philo 5 800 200 200 7

Run a simulation with a single philosopher:

    ./philo 1 800 200 200

A single philosopher can access only one fork, so they cannot eat and will eventually die.

## Thread Sanitizer

The project includes an `fsan` Makefile target to compile the program with Thread Sanitizer enabled.

Thread Sanitizer helps detect data races and other concurrency-related issues during execution.

Build the project with Thread Sanitizer:

    make fsan

Run the instrumented executable using the same arguments as the regular program:

    ./philo 5 800 200 200

The sanitizer uses the following compiler flags:

    -fsanitize=thread -g

- `-fsanitize=thread` enables Thread Sanitizer instrumentation.
- `-g` includes debugging information to help identify the source of reported issues.

Thread Sanitizer is a debugging tool and may affect execution speed and timing. Use a normal build to evaluate the simulation's timing behavior.

## Output

The program logs philosopher state changes using the following format:

    timestamp_in_ms X has taken a fork
    timestamp_in_ms X is eating
    timestamp_in_ms X is sleeping
    timestamp_in_ms X is thinking
    timestamp_in_ms X died

- `timestamp_in_ms` represents the elapsed time in milliseconds since the simulation started.
- `X` represents the philosopher's identifier.

Example output:

    0 1 has taken a fork
    0 1 has taken a fork
    0 1 is eating
    200 1 is sleeping
    200 2 is thinking

The actual output and timing depend on the simulation parameters and thread scheduling.

## Makefile Commands

| Command | Description |
|---|---|
| `make` or `make all` | Compile the project. |
| `make fsan` | Compile the project with Thread Sanitizer enabled. |
| `make clean` | Remove object files and dependency files. |
| `make fclean` | Remove object files, dependency files and the executable. |
| `make re` | Clean and rebuild the project (without Thread Sanitizer enabled). |
