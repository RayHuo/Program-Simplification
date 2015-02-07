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

# Running IO/inputs/DLP/Hampath/
# cd IO/inputs/NLP/EqTest/

# eqtfFiles=(`ls`)
# num=${#eqtfFiles[@]}
# pre="IO/inputs/NLP/EqTest/"
# type=0  # NLP


# cd ../../../../
# for ((i=0; i<num; i++));
# do
#     echo ${pre}${eqtfFiles[i]}, ${type}
#     ./programsimplification ${pre}${eqtfFiles[i]} ${type}
# done


############################################################

# Running IO/inputs/DLP/Factoring/
# cd IO/inputs/NLP/Factoring/

# ifFiles=(`ls`)
# num=${#ifFiles[@]}
# pre="IO/inputs/NLP/Factoring/"
# type=0  # NLP


# cd ../../../../
# for ((i=0; i<num; i++));
# do
#     echo ${pre}${ifFiles[i]}, ${type}
#     ./programsimplification ${pre}${ifFiles[i]} ${type}
# done


############################################################

# Running IO/inputs/DLP/HamiltonianPath/
# cd IO/inputs/NLP/HamiltonianPath/

# htpFiles=(`ls`)
# num=${#htpFiles[@]}
# pre="IO/inputs/NLP/HamiltonianPath/"
# type=0  # NLP


# cd ../../../../
# for ((i=0; i<num; i++));
# do
#     echo ${pre}${htpFiles[i]}, ${type}
#     ./programsimplification ${pre}${htpFiles[i]} ${type}
# done


############################################################

# Running IO/inputs/DLP/RandomNonTight/
# cd IO/inputs/NLP/RandomNonTight/

# rntFiles=(`ls`)
# num=${#rntFiles[@]}
# pre="IO/inputs/NLP/RandomNonTight/"
# type=0  # NLP


# cd ../../../../
# for ((i=0; i<num; i++));
# do
#     echo ${pre}${rntFiles[i]}, ${type}
#     ./programsimplification ${pre}${rntFiles[i]} ${type}
# done


############################################################

# Running IO/inputs/DLP/RLP-150/
# cd IO/inputs/NLP/RLP-150/

# r15Files=(`ls`)
# num=${#r15Files[@]}
# pre="IO/inputs/NLP/RLP-150/"
# type=0  # NLP


# cd ../../../../
# for ((i=0; i<num; i++));
# do
#     echo ${pre}${r15Files[i]}, ${type}
#     ./programsimplification ${pre}${r15Files[i]} ${type}
# done


############################################################

# Running IO/inputs/DLP/RLP-200/
# cd IO/inputs/NLP/RLP-200/

# r20Files=(`ls`)
# num=${#r20Files[@]}
# pre="IO/inputs/NLP/RLP-200/"
# type=0  # NLP


# cd ../../../../
# for ((i=0; i<num; i++));
# do
#     echo ${pre}${r20Files[i]}, ${type}
#     ./programsimplification ${pre}${r20Files[i]} ${type}
# done


############################################################

# Running IO/inputs/DLP/SchurNumbers/
# cd IO/inputs/NLP/SchurNumbers/

# snFiles=(`ls`)
# num=${#snFiles[@]}
# pre="IO/inputs/NLP/SchurNumbers/"
# type=0  # NLP


# cd ../../../../
# for ((i=0; i<num; i++));
# do
#     echo ${pre}${snFiles[i]}, ${type}
#     ./programsimplification ${pre}${snFiles[i]} ${type}
# done


############################################################

# Running IO/inputs/DLP/15-Puzzle/
cd IO/inputs/NLP/15-Puzzle/

p15Files=(`ls`)
num=${#p15Files[@]}
pre="IO/inputs/NLP/15-Puzzle/"
type=0  # NLP


cd ../../../../
for ((i=0; i<num; i++));
do
    echo ${pre}${p15Files[i]}, ${type}
    ./programsimplification ${pre}${p15Files[i]} ${type}
done
