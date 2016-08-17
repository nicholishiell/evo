#include <fstream>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <vector>

using namespace std;

class Solution{
public:
  Solution(float * f, int n, string c){
    fitness = new float[n];
    
    for(int i = 0; i < n; i++)
      fitness[i] = f[i];
    
    nObjectives = n;
    
    chromo = c;      
  }

  float GetFitness(int i){return fitness[i];}

  // Checks if the current solution dominated by solution s.
  // We are looking for minima so smaller is better.
  bool DominatedBy(Solution * s){
    int nLesser = 0;
    int nGreater = 0;
    int nEqual = 0;
    
    // Loop over all the objectives and count how many
    // components are greater, equal, or less than
    for(int i = 0; i < 4; i++){
      float f_i = s->GetFitness(i);
      if(fitness[i] == f_i) nEqual++;
      else if(fitness[i] > f_i) nGreater++;
      else nLesser++;
    }
    
    if(nLesser > 0) return false;
    else if(nGreater > 0) return true;    
    else return false;
  }  

  void Print(){
    printf("%s\t%f\t%f\t%f\t%f\n", chromo.c_str(), fitness[0], fitness[1], fitness[2], fitness[3]);
  }

private:

  string chromo;
  float *fitness;
  int nObjectives;

};

vector<Solution*> LoadSolutions();

//=====================================================================================================

int main(){
  
  vector<Solution*> solutionVector;
  vector<Solution*> paretoVector;

  solutionVector = LoadSolutions();

  // Check it solution against all the others to find out if it is dominate
  for(unsigned int i = 0; i < solutionVector.size(); i++){
    Solution * testingSolution = solutionVector[i];
    bool isDominate = true;

    for(unsigned int j = 0; j < solutionVector.size(); j++){
      
      if(i == j) continue;
      
      Solution * checkSolution = solutionVector[j];
      if(testingSolution->DominatedBy(checkSolution)){
	isDominate = false;
	break;
      }
    }

    if(isDominate)
      paretoVector.push_back(testingSolution);
  }

  for(unsigned int i = 0; i < paretoVector.size(); i++){
    paretoVector[i]->Print();
  }

  printf("Size of Pareto set:\t%lu\n", paretoVector.size());

  return 0;
}

//=====================================================================================================

vector<Solution*> LoadSolutions(){
  vector<Solution*> solnVector;

  ifstream inputFile;
  inputFile.open("data", std::fstream::in);


  for(int i = 0; i < 100; i++){
    string chromo;
    float fitness[4];

    inputFile >> chromo;
    for(int i = 0; i < 4; i++)
      inputFile >> fitness[i];
    printf("%s\t%f\t%f\t%f\t%f\n", chromo.c_str(), fitness[0], fitness[1], fitness[2], fitness[3]);
    solnVector.push_back(new Solution(fitness, 4, chromo));
  }
  printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
  return solnVector;
}

