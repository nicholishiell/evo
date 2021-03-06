#!/bin/bash

# Update average
g++ src/average_src/average.cpp -o average


# Update simulation
cd src/diskSim_src
./compileX
cp diskSim ../../

cd ../..

# Update initialize
g++ src/initialize_src/initialize.cpp -o initialize

# Update evolve
g++ src/evo_src/evolve.cpp -o evolve

#Update wordChecker
g++ src/wordChecker/main.cpp -o wordChecker

#Update cellSim
g++ src/cellSim/CellSim.cpp -o cellSim
