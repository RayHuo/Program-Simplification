#! /bin/bash

############################################################

# Running IO/inputs/2009/channelRouting/
# cd IO/inputs/2009/channelRouting/

# crFiles=(`ls`)
# num=${#crFiles[@]}
# pre="IO/inputs/2009/channelRouting/"
# type=0    # NLP


# cd ../../../../
# for ((i=0; i<num; i++));
# do
#     echo ${pre}${crFiles[i]}, ${type}
#     ./programsimplification ${pre}${crFiles[i]} ${type}
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

# Running IO/inputs/DLP/Hampath/
cd IO/inputs/DLP/RandomQuantifiedBooleanFormulas/

rqbfFiles=(`ls`)
num=${#rqbfFiles[@]}
pre="IO/inputs/DLP/RandomQuantifiedBooleanFormulas/"
type=1  # DLP


cd ../../../../
for ((i=0; i<num; i++));
do
    echo ${pre}${rqbfFiles[i]}, ${type}
    ./programsimplification ${pre}${rqbfFiles[i]} ${type}
done