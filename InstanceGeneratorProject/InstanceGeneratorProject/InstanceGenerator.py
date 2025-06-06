'''
AMMM P2 Instance Generator v2.0
Instance Generator class.
Copyright 2020 Luis Velasco.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
'''

import os, random
from InstanceGeneratorProject.AMMMGlobals import AMMMException


class InstanceGenerator(object):
    # Generate instances based on read configuration.

    def __init__(self, config):
        self.config = config

    def generate(self):
        instancesDirectory = self.config.instancesDirectory
        fileNamePrefix = self.config.fileNamePrefix
        fileNameExtension = self.config.fileNameExtension
        numInstances = self.config.numInstances

        numMembers = self.config.numMembers
        minBet = self.config.minBet
        maxBet = self.config.maxBet

        if not os.path.isdir(instancesDirectory):
            raise AMMMException('Directory(%s) does not exist' % instancesDirectory)

        for i in range(numInstances):
            instancePath = os.path.join(instancesDirectory, '%s_%d.%s' % (fileNamePrefix, i, fileNameExtension))
            fInstance = open(instancePath, 'w')

            fInstance.write('N=%d;\n' % numMembers)
    
            fInstance.write('m=[\n')
            betPlayer =  [[0 for _ in range(numMembers)] for _ in range(numMembers)]

            for i in range(1,numMembers):
                for j in range(i):
                    bet = random.randint(minBet, maxBet)
                    if bet != 0 :       # if bet is null, then mij = mji = 0
                        betPlayer[i][j] = bet
                        betPlayer[j][i] = random.randint(1,maxBet)    

            for n in range(numMembers):
                #betPlayer = [0] * numMembers # create a vector of 0's with numMembers elements
                #for t in range(numMembers):
                #   betPlayer[t] = random.randint(minBet, maxBet)

                #betPlayer[n]=0 # diagonal of 0
                # translate vector of integers into vector of strings and concatenate that strings separating them by a single space character
                fInstance.write('\t[%s]\n' % (' '.join(map(str, betPlayer[n]))))
            fInstance.write('];\n')

            fInstance.close()
