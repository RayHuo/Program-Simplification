#! /bin/bash


cd IO/inputs/2009/channelRouting/

crFiles=(`ls`)
num=${#crFiles[@]}
pre="IO/inputs/2009/channelRouting/"
type=0


cd ../../../../
for ((i=0; i<num; i++));
do
    echo ${pre}${crFiles[i]}, ${type}
    ./programsimplification ${pre}${crFiles[i]} ${type}
done

