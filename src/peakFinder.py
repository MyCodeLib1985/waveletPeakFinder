import numpy as np
from scipy import stats
from scipy import interpolate
from math import factorial
from scipy import signal

"""

A continuous wavelet transform based peak finder. Tested exclusively on Raman spectra, however,
it should work for most datasets.

Parameters
----------

extensionFactor: The number of points to pad the data with to avoid spurious peaks due to
endpoint effects.

lowerBound: The lowest value of the scale factor to use in the wavelet transform
upperBound: The highest value of the scale factor to use in the wavelet transform
steps: The number of scale factors we want between the highest and lowest bounds

rowWindow: The maximum number of rows that a ridge line can be discontinuous before it is
terminated. I.e. the maximum number of scale factors it can deviate.

colWindow: The maximum number of columns that a ridge line can wander before it is terminated.
I.e. the maximum number of wavenumbers (or a similar X value) that the ridge line can deviate.

"""

# CWT Transform parameters
lowerBound = 1
upperBound = 70
steps = 90

# Ridge line filtering parameters
rowWindow = 2
columnWindow = 5

class _spectra:
    def __init__(self,x,y):
        self.x = x
        self.y = y

    def x(self):
        return waveNumbers

    def y(self):
        return intensities

"""

Simple helper function for finding all of the maxima in the 2D array returned by the wavelet
transform. Works on the basis of a simple comparison between neighbouring elements. These
values form the initial basis for the ridge lines.

"""
def _findMaxima1D(CWTArray):

    maximas = np.zeros(CWTArray.size,dtype=(float,3))

    # Populate the maxima array with a tuple of the coordinates and the values of the maxima
    count = 0
    for j,row in enumerate(CWTArray):
        for i,element in enumerate(row):
            try:
                if element > row[i-1] and element > row[i+1]:
                    maximas[count]= ((steps-j,i,element))
                    count += 1
            except IndexError:
                pass

    return np.vstack(maximas[:count])

"""

Filter the ridge lines found from the maxima of the CWT coefficient array based on a set
parameters, namely the maximum deviations in wavenumber and scale space. Any lines which are
found from this criteria are considered to be peaks and further evaluated in the following
steps.

"""
def _filterRidgeLines(maximaArray,rowMax,colMax):

    # Helper to prevent duplicating ridge lines
    def checkValues(value, ridgeLines):

        for lines in ridgeLines:
            for points in lines:
                if value in points:
                    return True
        return False

    ridgeLines = []

    # Maxima array is a n row, 1 column array containing tuples of (scaleFactor, column)
    for i,row in enumerate(maximaArray):
        ridge = [] # For each maxima start a ridge line
        colPos = row[1] # Get the column position of the current maxima
        rowPos = row[0] # Get the row position of the current maxima
        # If this value is already part of another ridge line, move to the next value
        if checkValues(colPos, ridgeLines):
            continue
        for j, nextRows in enumerate(maximaArray[i:,:]): # Look through the subsequent maxima
            if nextRows[0] == rowPos: # If the scale factors are the same, skip
                continue
            if np.abs(colPos - nextRows[1]) <= colMax and \
                    np.abs(rowPos - nextRows[0]) <= rowMax:
                ridge.append((rowPos,colPos,nextRows[2]))
                rowPos = nextRows[0]
                colPos = nextRows[1]

        # If the ridge lines run all the way to the lowest scale factors, add them to the list
        if len(ridge) != 0:
            if ridge[-1][0] <= 2:
                ridgeLines.append(ridge)

    return ridgeLines

"""

For each of the ridge lines found from the filtered CWT array, determine the other
characteristics of the peaks.

The position of the peak is determined from the position of the maxima in the ridge
line.

"""
def getPeakInfo(ridgeLines,data,waveletCoeff):

    # For each of the ridge lines we have found, locate the positions of the maxima. These
    # correspond to the peak centers.
    peakInfo = np.zeros(len(ridgeLines),dtype=[('position','int32'),('scale','int32'),\
                                       ('cwtCoeff','f'),('SNR','f'),('length','uint8'),\
                                               ('intensity','f'),('wavenumber','f')])

    # For each of the ridge lines, add the position of the peak center and the length of the
    # line. These are useful for filtering peaks later.
    for i,lines in enumerate(ridgeLines):
        # Find the index of the maximum CWT coefficient. This is the peak center.
        maximum = np.argmax(zip(*lines)[2])
        peakInfo[i] = lines[maximum][1],lines[maximum][0],lines[maximum][2],0,len(lines),\
                                    data.x[lines[maximum][1]],data.y[lines[maximum][1]]

    # Calculate the local SNR of each peak within a window of 30 pixels of the peak. The SNR is
    # defined as the 95th quantile of the absolute values of the lowest scale factor coefficients.
    for i, peaks in enumerate(peakInfo):
        SNR = np.abs(waveletCoeff[-1,peaks[0]-15:peaks[0]+15])
        if len(SNR) == 0:
            peakInfo['SNR'][i] = 0
        else:
            SNR = stats.scoreatpercentile(SNR, 95)
            peakInfo['SNR'][i] = SNR

    return peakInfo

"""

Processes spectral data and returns a structured array of peak information. Peak can then be
filtered based on ridge line length, signal to noise ratio and scale values.

"""
def getPeaks(waveNumbers,intensities):

    data = _spectra(waveNumbers,intensities)

    # Take the CWT of the spectra. Trim the result to remove padding.
    waveletCoeff = signal.cwt(intensities, signal.ricker, \
                                   np.linspace(lowerBound,upperBound,steps))
    # Flip the matrix so the highest wavelet coefficient is the top row
    waveletCoeff = np.flipud(waveletCoeff)

    # Find the ridge lines connecting the maxima in the wavelet coefficient array. Filter ridge lines
    # takes a (scaleFactor,3) array of positions and values of maxima.
    ridgeLines = _filterRidgeLines(_findMaxima1D(waveletCoeff),columnWindow,rowWindow)

    # Populate a structured array with peak information
    peakInfo = getPeakInfo(ridgeLines,data,waveletCoeff)

    return peakInfo
