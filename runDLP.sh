#! /bin/bash


###############################################################

# not finish
# Running IO/inputs/DLP/DisjunctiveLoops/
# cd IO/inputs/DLP/DisjunctiveLoops/

# dlFiles=(`ls`)
# num=${#dlFiles[@]}
# pre="IO/inputs/DLP/DisjunctiveLoops/"
# type=1  # DLP


# cd ../../../../
# for ((i=0; i<num; i++));
# do
#     echo ${pre}${dlFiles[i]}, ${type}
#     ./programsimplification ${pre}${dlFiles[i]} ${type}
# done


###############################################################

# not finish
# Running IO/inputs/DLP/Mutex/
# cd IO/inputs/DLP/Mutex/

# dlFiles=(`ls`)
# num=${#dlFiles[@]}
# pre="IO/inputs/DLP/Mutex/"
# type=1  # DLP


# cd ../../../../
# for ((i=0; i<num; i++));
# do
#     echo ${pre}${dlFiles[i]}, ${type}
#     ./programsimplification ${pre}${dlFiles[i]} ${type}
# done


###############################################################


# Running IO/inputs/DLP/StrategicCompanies/
cd IO/inputs/DLP/StrategicCompanies/

dlFiles=(`ls`)
num=${#dlFiles[@]}
pre="IO/inputs/DLP/StrategicCompanies/"
type=1  # DLP


cd ../../../../
for ((i=0; i<num; i++));
do
    echo ${pre}${dlFiles[i]}, ${type}
    ./programsimplification ${pre}${dlFiles[i]} ${type}
done