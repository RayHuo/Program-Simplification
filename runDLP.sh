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
# cd IO/inputs/DLP/StrategicCompanies/

# dlFiles=(`ls`)
# num=${#dlFiles[@]}
# pre="IO/inputs/DLP/StrategicCompanies/"
# type=1  # DLP


# cd ../../../../
# for ((i=0; i<num; i++));
# do
#     echo ${pre}${dlFiles[i]}, ${type}
#     ./programsimplification ${pre}${dlFiles[i]} ${type}
# done


############################################################

# Running IO/inputs/DLP/Hampath/
# cd IO/inputs/DLP/Hampath/

# hpFiles=(`ls`)
# num=${#hpFiles[@]}
# pre="IO/inputs/DLP/Hampath/"
# type=1  # DLP


# cd ../../../../
# for ((i=0; i<num; i++));
# do
#     echo ${pre}${hpFiles[i]}, ${type}
#     ./programsimplification ${pre}${hpFiles[i]} ${type}
# done


############################################################

# Running IO/inputs/DLP/RandomQuantifiedBooleanFormulas/
# cd IO/inputs/DLP/RandomQuantifiedBooleanFormulas/

# rqbfFiles=(`ls`)
# num=${#rqbfFiles[@]}
# pre="IO/inputs/DLP/RandomQuantifiedBooleanFormulas/"
# type=1  # DLP


# cd ../../../../
# for ((i=0; i<num; i++));
# do
#     echo ${pre}${rqbfFiles[i]}, ${type}
#     ./programsimplification ${pre}${rqbfFiles[i]} ${type}
# done


############################################################
# # Running IO/inputs/DLP/StrategicCompanies/
# cd IO/inputs/DLP/StrategicCompanies/

# rqbfFiles=(`ls`)
# num=${#rqbfFiles[@]}
# pre="IO/inputs/DLP/StrategicCompanies/"
# type=1  # DLP


# cd ../../../../
# for ((i=0; i<num; i++));
# do
#     echo ${pre}${rqbfFiles[i]}, ${type}
#     ./programsimplification ${pre}${rqbfFiles[i]} ${type}
# done



cd IO/inputs/NLP/channelRouting/

crFiles=(`ls`)
num=${#crFiles[@]}
pre="IO/inputs/NLP/channelRouting/"
type=0    # NLP


cd ../../../../
for ((i=0; i<num; i++));
do
    echo ${pre}${crFiles[i]}, ${type}
    ./programsimplification ${pre}${crFiles[i]} ${type}
done