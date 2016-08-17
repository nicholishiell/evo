#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

/* The almighty MAIN */
int main(int argc, char* argv[]){

  int runID = atoi(argv[1]);
  //  string answer = argv[2];
  string answer1 = "NNNNNNNNNNNNNNNN0NNNNNNNNNNNNNNN";
  string answer2 = "EEEEEEEEEEEEEEEE0EEEEEEEEEEEEEEE";
  string answer3 = "NENENENENENENENE0ENENENENENENENE";

  //string answer1 = "NEWS0ENS0EWNS0EN0WNW0ENS0WNE0SN";
  //string answer2 = "WSW0ESNWSNE0SNWE0ENSNWEW0W0EWWW";
  //string answer3 = "ES00WSNS0WNE0SNW0ESNW0ENS0WNS0E";

  //string answer1 = "00000000000000000000000000000000";
  //string answer2 = "00000000000000000000000000000000";
  //string answer3 = "00000000000000000000000000000000";


  // Create the robot objects.
  ifstream inputFile;
  std::stringstream inputFileName;
  inputFileName << "./untested/individual"<< runID <<".dat";
  inputFile.open(inputFileName.str().c_str(), std::fstream::in);
  string chromo;
  inputFile >> chromo;
  inputFile.close();

  float fitness1 = 32.;
  float fitness2 = 32.;
  float fitness3 = 32.;

  for(unsigned int i = 0; i < chromo.size(); i++){
    if(chromo[i] == answer1[i]) fitness1 -= 1.;
    if(chromo[i] == answer2[i]) fitness2 -= 1.;
    if(chromo[i] == answer3[i]) fitness3 -= 1.;
  }
  
  ofstream myfile;
  std::stringstream outputFileName;
  outputFileName << "./results/outputData" << runID;
  myfile.open(outputFileName.str().c_str(), std::fstream::app);
  myfile << chromo <<"\t" << 0. << "\t" <<fitness1
	 <<"\t"<< fitness2 <<"\t"<< 0. <<"\t"<<fitness3<<"\n"; 
  myfile.close();
  
   
  return 0;
}
