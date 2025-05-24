'''
AMMM P2 Instance Generator v2.0
Config attributes validator.
Copyright 2020 Luis Velasco

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

from InstanceGeneratorProject.AMMMGlobals import AMMMException


class ValidateConfig(object):
    # Validate config attributes read from a DAT file.

    @staticmethod
    def validate(data):
        # Validate that mandatory input parameters were found
        paramList = ['instancesDirectory', 'fileNamePrefix', 'fileNameExtension', 'numInstances',
                      'numMembers', 'minBet', 'maxBet']
        for paramName in paramList:
            if paramName not in data.__dict__:
                raise AMMMException('Parameter(%s) has not been not specified in Configuration' % str(paramName))

        instancesDirectory = data.instancesDirectory
        if len(instancesDirectory) == 0: raise AMMMException('Value for instancesDirectory is empty')

        fileNamePrefix = data.fileNamePrefix
        if len(fileNamePrefix) == 0: raise AMMMException('Value for fileNamePrefix is empty')

        fileNameExtension = data.fileNameExtension
        if len(fileNameExtension) == 0: raise AMMMException('Value for fileNameExtension is empty')

        numInstances = data.numInstances
        if not isinstance(numInstances, int) or (numInstances <= 0):
            raise AMMMException('numInstances(%s) has to be a positive integer value.' % str(numInstances))

        numMembers = data.numMembers
        if not isinstance(numMembers, int) or (numMembers <= 0):
            raise AMMMException('numMembers(%s) has to be a positive integer value.' % str(numMembers))

        minBet = data.minBet
        if not isinstance(minBet, (int, float)) or (minBet < 0):
            raise AMMMException('minCapacityPerCore(%s) has to be a positive float value.' % str(minBet))

        maxBet = data.maxBet
        if not isinstance(maxBet, (int, float)) or (maxBet <= 0):
            raise AMMMException('maxBet(%s) has to be a positive float value.' % str(maxBet))

        if maxBet < minBet:
            raise AMMMException('maxBet(%s) has to be >= minBet(%s).' % (str(maxBet), str(minBet)))
