# Final Project for the Algorithm and Data Structures course @ Polimi (2022-2023)

## Overview

This C program manages a set of highway service stations and computes a route between two stations under reachability constraints. Each station is identified by its position (`key`) and contains a “car park” of vehicles, represented by their autonomy values. For routing, each station maintains `range`, the maximum autonomy currently available at that station.

## Data model and structures

Stations are stored in a binary search tree (BST) with parent pointers. The code supports:

* lookup (`find_node`)
* in-order predecessor/successor (`prev_node`, `next_node`)
* insert (`insert_node`)
* delete (`remove_node`)
* full cleanup (`delete_tree`)

Each station owns a second BST (`cars`) containing autonomy values. Multiple cars with the same autonomy are tracked via `copies` rather than duplicating nodes.

## Supported operations (stdin commands)

The program reads commands from standard input and prints the required acknowledgments:

* add station (with an initial list of cars, updating `range`)
* add car to a station (updating `range` if the new autonomy is larger)
* remove station (also frees its `cars` tree)
* remove car (and updates `range` if the removed car was providing the maximum)
* plan route (`p start stop`): prints the sequence of station keys or `nessun percorso`

## Route planning logic

For `p start stop`, the program:

1. finds the start/stop nodes
2. builds a linear list of the stations between them using successor/predecessor traversal
3. runs `find_path`, which propagates the minimum number of hops (`clicks`) and stores back-pointers (`link`) to reconstruct the final path from destination to start.

## Notes on complexity

The station/car structures are plain BSTs (no balancing logic is present), so operation cost is `O(h)` where `h` is tree height and may degrade in worst case. Route planning is performed over the stations in the requested interval and may be quadratic in the number of stations in that segment in the worst case.
