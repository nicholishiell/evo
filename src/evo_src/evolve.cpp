#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>
#include <sstream> 
#include <fstream> 
#include <math.h>
#include <time.h>  

#include "Population.h"
#include "Controller.h"

using namespace std;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void LoadPopulation(Population * p, int nControllers);
void SavePopulation(Population * p, int nControllers);
void RankPopulation(Population * p);
float CalculateDiversity(Population * p);
Controller * WeightedSelect(Population * p);
Controller * TournamentSelect(Population * p);
Controller * RandomSelect(Population * p);
Controller * DensitySelect(Population * p);

Controller * OnePointCrossOver(Controller* a, Controller* b);
Controller * TwoPointCrossOver(Controller* a, Controller* b);

Controller * Mutate(Controller * a);

Controller * GenerateRandom(Population * p);

bool CheckOriginality(Controller * c, Population *p);
float GetRandom();
Population * ParetoFilter(Population * p);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int main(int argc, char* argv[]){

  srand (time(NULL));

  int populationSize = atoi(argv[1]);
  float mutationRate = 0.05;

  Population * popn = new Population(); 
  
  // Load the evaluated controllers into the population
  LoadPopulation(popn, populationSize);
  float diversityFull =  CalculateDiversity(popn);
  
  /*cout << "Diversity = " << diversityFull <<"\n";
  cout << "FULL POPULATION\n=====================\n";
  for(unsigned int i = 0; i < populationSize; i++){
    Controller * c = popn->GetController(i);
    cout << c->GetChromo() << "\t"  
	 << c->GetFitness() << "\t"
	 << c->GetStdX() <<"\t"
	 << c->GetStdY() <<"\t"
	 << c->GetNCollide() <<"\n";
  }
  cout << "\n=====================\n";
 
  // Filter out the dominated individuals.
  /*popn = ParetoFilter(popn);
  int paretoPopulationSize = popn->GetNumberOfControllers();

  popn->Rank();
  float diversity =  CalculateDiversity(popn);
  
  cout << "Diversity = " << diversity <<"\n";
  cout << "PARETO POPULATION\n=====================\n";
 
  for(unsigned int i = 0; i < paretoPopulationSize; i++){
    Controller * c = popn->GetController(i);
    cout << c->GetChromo() << "\t"  
	 << c->GetDist() << "\t"
	 << c->GetStdX() <<"\t"
	 << c->GetStdY() <<"\t"
	 << c->GetNCollide() <<"\n";
  }
  cout << "\n=====================\n";*/
  Controller * bestSoFar = popn->GetFittest();
  cout << bestSoFar->GetChromo() << "\t" <<bestSoFar->GetFitness() 
  << "\t"<<diversityFull << "\n";
  //getchar();

  // Create a new population based on the old one.
  Population *newPopn = new Population();

  // Use elitism
  newPopn->AddController(popn->GetFittest()); // Elitism.
  /*for(int i = 0; i < paretoPopulationSize; i++){
    Controller * c = popn->GetController(i);

    //if(c->GetStdX() < 50. and c->GetNCollide() < 2000.)
      newPopn->AddController(popn->GetController(i));
    

      }*/
  
  //paretoPopulationSize = newPopn->GetNumberOfControllers();

  //int nNewControllers = populationSize - paretoPopulationSize;
  //Xprintf("%d - %d = %d\n", populationSize, paretoPopulationSize, nNewControllers);

  //for(int i = 0; i < nNewControllers; i++){
  for(int i = 0; i < populationSize -1; i++){
    
    // Selection (Roulette).
    //Controller * a = WeightedSelect(popn);
    //Controller * b = WeightedSelect(popn);
    
    // Selection (Tournament k = 2).
    Controller * a = TournamentSelect(popn);
    Controller * b = a;
    while(a==b) 
      b = TournamentSelect(popn);
    
    // Random select from Pareto Set
    // Controller * a = RandomSelect(popn);
    //Controller * b = a;
    //while(a==b) 
    //b = RandomSelect(popn);

    // Select based on neighourhood density.
    //Controller * a = DensitySelect(popn);
    //Controller * b = a;
    //while(a==b) 
    //b = DensitySelect(popn);

    // Crossover.
    //Controller * c = OnePointCrossOver(a,b);
    Controller * c = TwoPointCrossOver(a,b);
    
    // Mutation.
    if(GetRandom() <= mutationRate){
      c = Mutate(c);
    }    

    while(!CheckOriginality(c,newPopn)){
      c = Mutate(c);
    }

    newPopn->AddController(c); 
  }

  // Save the new population.
  SavePopulation(newPopn, populationSize);
  //getchar();
  return 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool CheckOriginality(Controller * c, Population *p) {

  int nControllers = p->GetNumberOfControllers();
  string testChromo = c->GetChromo();

  for(int i = 0; i < nControllers; i++){
    Controller * a = p->GetController(i);
    string chromo = a->GetChromo();
    
    if(chromo.compare(testChromo) == 0){
      return false;
    }
  }
  return true;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Controller * GenerateRandom(Population * p){
  int chromoLength = 32;
  string alphabet = "NEWS0";

  string newChromo;

  for(int d = 0; d < chromoLength; d++){
    int index = (int)alphabet.size()*GetRandom();
    if(d == 16) newChromo += '0';
    else newChromo += alphabet[index];
  }

  return new Controller(newChromo, 0., 0., 0., 0., 0.);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Population * ParetoFilter(Population * p){
  
  Population * paretoPopulation = new Population();
  
  int populationSize = p->GetNumberOfControllers();

  // Check it solution against all the others to find out if it is dominate
  for(int i = 0; i < populationSize; i++){
  
    Controller * testController = p->GetController(i);
    bool isDominate = true;
    for(int j = 0; j < populationSize; j++){
        
      if(i == j) continue;
      
      Controller * checkController = p->GetController(j);
      if(testController->DominatedBy(checkController)){
	isDominate = false;
	break;
      }
    }

    if(isDominate){
      paretoPopulation->AddController(testController);
    }
  }

  return paretoPopulation;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
float CalculateDiversity(Population * p){
  float populationDiversity = 0.;
  
  // Create an array of strings that will store the chromos 
  int nControllers = p->GetNumberOfControllers();
  string * chromoArray = new string[nControllers];
 
  for(int i = 0; i < nControllers; i++){
    chromoArray[i] = p->GetController(i)->GetChromo();
  }  

  int chromoLength = chromoArray[0].size();

  for(int i = 0; i < chromoLength; i++){
    // Reset count array
    float letterCounter[5] = { 0., 0., 0., 0., 0. }; 
    float geneDiversity = 0.;    
    // Count how frequently each letter appears
    for(int d = 0; d < nControllers; d++){
      char position_i = chromoArray[d][i];
      if(position_i == 'N') letterCounter[0] += 1.;
      if(position_i == 'E') letterCounter[1] += 1.;
      if(position_i == 'W') letterCounter[2] += 1.;
      if(position_i == 'S') letterCounter[3] += 1.;
      if(position_i == '0') letterCounter[4] += 1.;
    }
    
    for(int d = 0; d < 5; d++){
      float val = letterCounter[d];
      
      // skip zeros
      if(val <= 0.) continue;

      val = val / (float) nControllers;
      geneDiversity += val*log10(val);
    }
    
    //printf("%f %f %f %f %f %f\n", letterCounter[0],letterCounter[1], 
    //	   letterCounter[2], letterCounter[3], letterCounter[4], geneDiversity);

    populationDiversity += geneDiversity;
  }
 
  populationDiversity = -1.*populationDiversity;
 
  float maxDiversityValue = (float)chromoLength*log10(5);

  //printf("maxDiv = %f\ndiversity = %f\n", maxDiversityValue, populationDiversity);

  return populationDiversity / maxDiversityValue;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void LoadPopulation(Population * p, int nControllers){
  ifstream initFile;
 
  // Open all the files in "tested" directory  
  for(int i = 0; i < nControllers; i++){
    std::stringstream ss;
    ss << "./tested/individual"<< i+1 <<".dat";
  
    initFile.open (ss.str().c_str());

    string chrome;
    float fitness, dist, stdX, stdY, nCollide;
    
    initFile >> chrome;
    initFile >> fitness;
    initFile >> dist;
    initFile >> stdX;
    initFile >> stdY;
    initFile >> nCollide;

    p->AddController(new Controller(chrome, fitness, dist, stdX, stdY, nCollide));
    
    initFile.close();   
  }

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void SavePopulation(Population * p, int nControllers){
  ofstream outputFile;
  
  //cout << "NEW POPULATION\n=====================\n";
  for(int i = 0; i < nControllers; i++){
    std::stringstream ss;
    ss << "./untested/individual"<< i+1 <<".dat";
    outputFile.open(ss.str().c_str(), std::fstream::trunc);
        
    Controller * c = p->GetController(i);
    outputFile << c->GetChromo() <<" 0. 0. 0. 0. 0.";
    /*  cout << c->GetChromo() << "\t" << c->GetFitness() << "\t\t" 
	 << c->GetDistRank() <<"("<<c->GetDist()<<")" << "\t"
	 << c->GetStdXRank() <<"("<<c->GetStdX()<<")" <<"\t"
	 << c->GetNCollideRank() <<"("<<c->GetNCollide()<<")" <<"\n";*/
    outputFile.close();   
  }

  // cout << "\n=====================\n";

};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Controller * RandomSelect(Population * p){
  int nControllers = p->GetNumberOfControllers();

  int index = (int)nControllers*GetRandom();
  
  return p->GetController(index);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Controller * DensitySelect(Population * p){

  int nControllers = p->GetNumberOfControllers();

  int index1 = (int)nControllers*GetRandom();
  int index2 = index1;
  
  while(index2 == index1)
    index2 = (int)nControllers*GetRandom();

  Controller* a = p->GetController(index1);
  Controller* b = p->GetController(index2);

  float aDensity = p->GetDensity(a);
  float bDensity = p->GetDensity(b);

  if(aDensity < bDensity){
    return a;
  }
  else{ 
    return b;  
  }
  
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Controller * TournamentSelect(Population * p){

  int nControllers = p->GetNumberOfControllers();

  int index1 = (int)nControllers*GetRandom();
  int index2 = index1;
  
  while(index2 == index1)
    index2 = (int)nControllers*GetRandom();

  Controller* a = p->GetController(index1);
  Controller* b = p->GetController(index2);

  //printf("Tourn. Sel. %d\t%d\n", index1, index2);

  if(a->GetFitness() > b->GetFitness()){
    //printf("%d\n", index1);
    return a;
  }
  else{ 
    //printf("%d\n", index2);
    return b;  
  }
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Controller * WeightedSelect(Population * p){
  
 Controller * c;
 int nControllers = p->GetNumberOfControllers();
 float wheel[nControllers];

 // Normalize Weight
 float W = 0.;
 for(int i = 0; i < nControllers; i++)
   W += (p->GetController(i))->GetFitness();
 for(int i = 0; i < nControllers; i++){
   wheel[i] = p->GetController(i)->GetFitness()/W;
 }

 // Build the wheel
 for(int i = 0; i < nControllers; i++){
   if(i == 0) wheel[i] = wheel[i];
   else wheel[i] = wheel[i-1]+wheel[i];
  
 }

 // Roulette Sampling 
 float value = GetRandom();
 
 for(int i = 0; i < nControllers; i++){
   if( value <= wheel[i]){ 
     c = new Controller(p->GetController(i));
     break;
   } 
 }
 return c;

};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 1 point cross over. 
Controller * OnePointCrossOver(Controller* a, Controller* b){
  string chromoA = a->GetChromo();
  string chromoB = b->GetChromo();
  string newChromo;

  int length = chromoA.size();
  int index1 = (int)length*GetRandom();
  
  // Append chromoA to newChromo up to index1
  for(int i = 0; i < index1; i++){
    newChromo += chromoA[i];
  }

  for(int i = index1; i < length; i++){
    newChromo += chromoB[i];
  }
  
  return new Controller(newChromo, 0., 0., 0. ,0.,0.);
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Controller * Mutate(Controller * a){
  string oldChromo = a->GetChromo();
  int length = oldChromo.size();
  
  string newChromo;
  string alphabet("NESW0");

  int geneIndex = (int)length*GetRandom();
  
  int alphabetIndex = (int)5*GetRandom();
  
  for(int i = 0; i < length; i++){
    if(i == geneIndex){
      newChromo += alphabet[alphabetIndex];
    }
    else
      newChromo += oldChromo[i];
  }
 
  return new Controller(newChromo, 0., 0., 0. ,0.,0.);
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Controller * TwoPointCrossOver(Controller* a, Controller* b){
  string chromoA = a->GetChromo();
  string chromoB = b->GetChromo();
  string newChromo;

  int length = chromoA.size();
  int index1 = (int)length*GetRandom();
  int index2 = index1;
  while(index1 == index2)
    index2 = (int)length*GetRandom();

  if(index2 < index1){
    int temp = index2;
    index2 = index1;
    index1 = temp;
  }
  
  // Append chromoA to newChromo up to index1
  for(int i = 0; i < chromoA.size(); i++){
    if(i < index1) newChromo += chromoA[i];
    else if(i < index2) newChromo += chromoB[i];
    else newChromo += chromoA[i];
  }

  //printf("%d\t%d\n", index1, index2);
  //printf("%s\n%s\n", chromoA.c_str(), chromoB.c_str());
  //printf("---------------------------------\n");
  //printf("%s\n", newChromo.c_str());
    
  return new Controller(newChromo, 0.,0.,0.,0.,0.);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

float GetRandom(){
  return (float)rand()/RAND_MAX;
};
