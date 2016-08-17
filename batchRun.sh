#!/bin/bash 
# arg1 nSamples
# arg2 nControllers
# arg3 chromoLength
# arg4 alphabet
# arg5 nRobots
# arg6 maxTimeSteps
# arg7 runHeadless
# arg8 nGenerations

clear

N_SAMPLES=$1
N_CONTROLLERS=$2
CHROMO_LENGTH=$3
ALPHABET=$4
N_ROBOTS=$5
MAX_TIME_STEPS=$6
RUNHEADLESS=$7
N_GENERATIONS=$8

# Clean up old files
rm ./tested/*.dat
rm ./untested/*.dat
rm ./results/*

# start with a random population
./initialize $N_CONTROLLERS $CHROMO_LENGTH $ALPHABET

GEN_COUNTER=0
while [  $GEN_COUNTER -lt $N_GENERATIONS ]; do
   
    #echo Generation "$GEN_COUNTER"
    # Evaluate each controller
    CONTROLLER_COUNTER=0
    while [  $CONTROLLER_COUNTER -lt $N_CONTROLLERS ]; do
	#echo "Evaluating Controller: " $CONTROLLER_COUNTER
	# Prepare the input and output files
	let id=CONTROLLER_COUNTER+1

        # Run diskSim on the current controller
	SAMPLE_COUNTER=0
	while [  $SAMPLE_COUNTER -lt $N_SAMPLES ]; do
	    #echo Generation "$GEN_COUNTER" Individual "$CONTROLLER_COUNTER" Sample "$SAMPLE_COUNTER"
	    #clear
	    #./diskSim $N_ROBOTS $MAX_TIME_STEPS $RUNHEADLESS $id
	    ./cellSim $N_ROBOTS $MAX_TIME_STEPS $id $RUNHEADLESS
	    #./wordChecker $id
	    #sleep 1
	    let SAMPLE_COUNTER=SAMPLE_COUNTER+1
	done

	# Now average the fitness over all the samples taken
	./average $N_SAMPLES $id
	
	let CONTROLLER_COUNTER=CONTROLLER_COUNTER+1
    done
    #clear
    # Evolve the tested controllers
    ./evolve $N_CONTROLLERS

    rm results/*
    
   let GEN_COUNTER=GEN_COUNTER+1 
done

