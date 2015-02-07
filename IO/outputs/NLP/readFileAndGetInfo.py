import os

outputFile = open('NLPResult.txt', 'w')
outputFile.write('Benchmark\tTotal atoms\tconsequence time\tconsequence size\tGWRS size\tGWRS time\tGSRS size\tGSRS time\n')

rootDirts = ['./EqTest/', './HamiltonianPath/', './RandomNonTight/', './RLP-150/', './RLP-200/', './SchurNumbers/']
for rootDirt in rootDirts : 
    for root, dirt, files in os.walk(rootDirt):
        for filepath in files:
            print filepath
            # fileName = './channelRouting/channelRoute.in1.out'
            fileName = rootDirt + filepath
            inputFile = open(fileName, 'r')

            nums = 0
            lines = [2,4,7,10,11,14,15]
            splits = [' = ', ' : ', ' = ', ' = ', ' = ', ' = ', ' = ']
            sindex = 0
            
            outputFile.write('%s\t' % fileName)
            line = inputFile.readline()
            while line:
                nums += 1
                if nums in lines:
                    # outputFile.write(line)
                    line=line.strip('\n')  # remove '\n' in line
                    figure = line.split(splits[sindex])[1]
                    sindex += 1
                    outputFile.write("%s\t" % figure)
                line = inputFile.readline()

            outputFile.write('\n')
            inputFile.close()

outputFile.close()