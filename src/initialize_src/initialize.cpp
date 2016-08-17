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

float GetRandom(){
  return (float)rand()/RAND_MAX;
};


int main(int argc, char* argv[]){

  srand (time(NULL));

  int populationSize = atoi(argv[1]);
  int chromoLength = atoi(argv[2]);
  string alphabet(argv[3]);

  for(int i = 0; i < populationSize; i++){
    string newChromo;
  
    // Create new chromosome
    if(i == 0){
      //newChromo = "0ENESENEWENEWENE00NE00N000N0W000";
      newChromo = "0ENESENEWENEWENE00NE00N000N0W000";
    }
    else{
      for(int d = 0; d < chromoLength; d++){
	int index = (int)alphabet.size()*GetRandom();
	
	newChromo += alphabet[index];
      }
      }
    /*for(int d = 0; d < chromoLength; d++){
      int index = (int)alphabet.size()*GetRandom();
      if(d == 16) newChromo += '0';
      else newChromo += alphabet[index];
      }*/
    
    // Now save the new controller to a file
    ofstream outputFile;
    std::stringstream ss;
    ss << "./untested/individual"<< i+1 <<".dat";
    outputFile.open(ss.str().c_str(), std::fstream::trunc);
    outputFile << newChromo <<" "<< 0. <<" "<< 0. <<" "<< 0. <<" " <<0. <<" " << 0.;
    outputFile.close();   
  }


  return 0;
}
