#! /bin/bash

# Quick script to run the plot-making macro for the HBHE commissioning channel timing plots

# Directory where your input root files are stored
INPUTDIR=/afs/cern.ch/work/s/sabrandt/public/HCAL/data_timing_m2/CMSSW_7_5_0_pre6/src

# function inputs: drawPlots(inputFileName, outputFolderName, datasetDescriptionForPlots, getTextOutput of timing)
root -l drawTimingMaps.C+\(\"${INPUTDIR}/runTime_254532_test2.root\",\"test\",\"test\",false\)
# root -l drawPlots.C+\(\"${INPUTDIR}/run254532.root\",\"run254532\",\"run254532\"\)

rm *.so *.d *.pcm