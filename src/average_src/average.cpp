#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <sstream> 
#include <fstream> 
#include <math.h>
#include <time.h>
#include <stdlib.h>

using namespace std;


int main(int argc, char* argv[]){

  int nSamples = atoi(argv[1]);
  int id = atoi(argv[2]);

  ifstream inputFile;
  std::stringstream inputFileName;
  inputFileName << "./results/outputData"<< id;
 
  inputFile.open(inputFileName.str().c_str(), std::fstream::in);
  string chromo, trash;
  float avgFitness = 0.;
  float avgDist = 0.;
  float avgStdX = 0.;
  float avgStdY = 0.;
  float avgCollide = 0.; 

  for(int i = 0; i < nSamples; i++){
    float fitness, dist, stdX, stdY, nCollide;

    if(i==0) inputFile >> chromo;
    else inputFile >> trash;
    inputFile >> fitness;
    inputFile >> dist;
    inputFile >> stdX;
    inputFile >> stdY;
    inputFile >> nCollide;

    avgFitness += fitness;
    avgDist += dist;
    avgStdX += stdX;
    avgStdY += stdY;
    avgCollide += nCollide;
  }

  avgFitness /= (float)nSamples;
  avgDist /= (float)nSamples;  
  avgStdX /= (float)nSamples;
  avgStdY /= (float)nSamples;
  avgCollide /= (float)nSamples;

  ofstream outputFile;
  std::stringstream ss;
  ss << "./tested/individual"<< id <<".dat";
  outputFile.open(ss.str().c_str(), std::fstream::trunc);
  outputFile << chromo <<" "<< avgFitness 
	     <<" "<<avgDist <<" "<<avgStdX<<" "<<avgStdY<<" " <<avgCollide;
  outputFile.close();    

  return 0;
}
