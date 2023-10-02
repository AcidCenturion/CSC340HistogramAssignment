#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

/** FIRST PROBLEM
  * desc: Generate a histogram detailing the frequencies of the random numbers that fall within a certain range
  * 	  Bins denoted with  1) center value,  2) frequency. bins are equidistant (mean +/- x*stdDev) and equal in width (range)
  * notes: The canvas instructions mention rounding into the nearest bin, however she said that if the widths of the bins dont  
  * 	   touch the other bins, a number generated in between the bins will be discarded. so presumably that means if the value 
  * 	   is not exactly the center but is within the width, it is counted towards the bin of the centerred value.
  * in: mean
  *	stdDev
  *	numBins
  *	numSamples
  *	binWidth (no bigger than stdDev)
  * defaults: numSamples : 20000
  * 	      binWidth : 1 (catch all numbers)
  * out: a collection of elements, each containing a designation (binNum) and a frequency
  */

std::vector<std::pair<double, int>> normalDistribution(double mean, double stdDev, int numBins, int numSamples, double binWidth){
    int i;
    int j;
    
    //check for defaults
    if (numBins < 9){
        numBins = 9;
    }
    if (numSamples == -1){
        numSamples = 20000;
    }
    if (binWidth == -1 || binWidth > stdDev){
        binWidth = stdDev;
    }

    /* Generate numbers into a historgram that follow normal distribution (using mean and stdDev) */
    //declare histogram as a vector of pairs(bin center, frequency) of size numBin
    std::vector<std::pair<double, int>> histo;
    histo.resize(numBins);
    //initialize pairs in histogram
    //start from center (mean) to end
    j = 0;
    for(i = numBins/2; i < numBins; i++){
        histo.at(i).first = mean+j*stdDev;
        histo.at(i).second = 0;
        j++;
    }
    //start from left of center to beginning
    j = -1;
    for(i = numBins/2 -1; i >= 0; i--){
        histo.at(i).first = mean+j*stdDev;
        histo.at(i).second = 0;
        j--;
    }

    //declare random number engine
    std::random_device rando{};
    std::mt19937 engine{rando()};
    //declare distribution
    std::normal_distribution<double> distr{mean, stdDev};
    //declare a temp variables before loop
    //to store random number
    //the nearest bin to random number
    double randNum;
    double nearBin;

    //loop numSample times
    for(i = 0; i < numSamples; i++){
        //generate a random number
        randNum = distr(engine);

        //if number is within bin width of the nearest bin
        //nearest bin center
        nearBin = roundToBin(histo, randNum);

        //if number is within the upperbound of the lower bin
        if(randNum <= nearBin + binWidth/2){
            //find the bin in the histo
            for(j = 0; j < numBins; j++){
                if(histo.at(j).first == nearBin){
                    //add to the bin frequency
                    histo.at(j).second++;
                }
            }
        }

        //if number is within the lowerbound of the upper bin
        else if(randNum >= nearBin - binWidth/2){
            //find the bin in the histo
            for(j = 0; j < numBins; j++){
                if(histo.at(j).first == nearBin){
                    //add to the bin frequency
                    histo.at(j).second++;
                }
            }
        }
        
        //otherwise, ignore the random number because it does not land within the widths of the nearest bin
    }//end loop

    return histo;
}




/** MAIN METHOD */
int main(){
    normalDistribution(5, 5, 9, 25, 5);
    return 0;
}

/* Helper Function: roundToBin(num) */
double roundToBin(std::vector<std::pair<double, int>> histo, double num){
    //declarations
    double prevBin = histo.at(0).first; //bin at left
    double nextBin;
    //iterate through bins
    for(int i = 1; i < histo.size(); i++){
        nextBin = histo.at(i).first; //bin at right
        //num is between two bins
        if(num < nextBin){
            //check which of the two bins
            if(nextBin - num <= num - prevBin){ //close to nextBin
                return nextBin;
            }
            else{ //closer to prevBin
                return prevBin;
            }
        }
        prevBin = nextBin;
    }
}