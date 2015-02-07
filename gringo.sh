#! /bin/bash

# gringo benchmarks in 2009/channelRouting
# cd IO/inputs/2009/channelRouting/ChannelRouting/

# crFiles=(`ls`)
# num=${#crFiles[@]}
# post=".in"
# pre="./ChannelRouting/"


# cd ../
# for ((i=0; i<num; i++));
# do
#     output=${crFiles[i]}${post}
#     gringo channelRouting.aspx ${pre}${crFiles[i]} -t > ${output}
# done


# gringo benchmarks in 2009/labyrinth
cd IO/inputs/2009/labyrinth/17-Labyrinth/allInstances/

crFiles=(`ls`)
num=${#crFiles[@]}
post=".in"
pre="./allInstances/"
outputpre="../"

cd ../
for ((i=0; i<num; i++));
do
    output=${outputpre}${crFiles[i]}${post}
    gringo labyrinth.enc.asp ${pre}${crFiles[i]} -t > ${output}
done
