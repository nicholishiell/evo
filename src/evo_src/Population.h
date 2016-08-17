#ifndef POPN
#define POPN 

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>  

#include "Controller.h"

class Population{
 public:

  Population(){
    numberOfControllers = 0;
    controllerArray = new Controller*[numberOfControllers];
  }
  
  ~Population(){
    delete[] controllerArray;
  }

  Controller * GetController(int i){
    if(i >= numberOfControllers){
      printf("i > # of Controllers\n");
      return NULL;
    }
    else
      return controllerArray[i];
  }

  void AddController(Controller * c){
    IncreaseSize();
    controllerArray[numberOfControllers-1] = c;
  }

  int GetNumberOfControllers(){
    return numberOfControllers;
  }

  Controller * GetFittest(){
    double highest = 0.;
    int indexOfHighest;

    for(int i = 0; i < numberOfControllers;i++){
      if(controllerArray[i]->GetFitness() > highest){
	indexOfHighest = i;
	highest = controllerArray[i]->GetFitness();
      }
    }

    return controllerArray[indexOfHighest];
  }


  float GetDensity(Controller *c){

    float density = 0.;

    for(int i = 0; i < numberOfControllers; i ++){
      Controller * a = controllerArray[i];

      if(c == a) continue;
      
      density += c->CompareChromo(a);

    }
    
    density = density / numberOfControllers;

    return density;
  }

  void Print(){
    for(int i = 0; i < numberOfControllers;i++)
      controllerArray[i]->Print();

    printf("~~~~~~~~~~~~~~\n");
  }


  void Copy(Population * p){
    numberOfControllers = 0;
    
    for(int i = 0; i < p->GetNumberOfControllers(); i++)
      AddController(p->GetController(i));
  }
  
  // Returns array sorted in ascending order
  float * SortArray(float* array){
    
    for(int i=0; i<numberOfControllers-1; i++){
      for(int j=i+1; j<numberOfControllers; j++){
	if(array[i] < array[j]) {
	  // These three lines swap the elements
	  // list[i] and list[j].
	  float temp = array[i];
	  array[i] = array[j];
	  array[j] = temp;
	}
      }
    }
    
    return array;
  }

  void Rank(){

    float * distArray = new float[numberOfControllers];
    float * stdXArray = new float[numberOfControllers];
    float * collideArray = new float[numberOfControllers];

    // Fill arrays with data from controllers
    for(int i = 0; i <numberOfControllers; i++){
      distArray[i] = controllerArray[i]->GetDist();
      stdXArray[i] = controllerArray[i]->GetStdX();
      collideArray[i] = controllerArray[i]->GetNCollide();
    }

    // Sort the data arrays in ascending order
    distArray = SortArray(distArray); 
    stdXArray = SortArray(stdXArray); 
    collideArray = SortArray(collideArray);

    // Now set the data values in the controllers
    // equal to their rank in the sorted array
    for(int i = 0; i < numberOfControllers; i++){
      for(int j = 0; j < numberOfControllers; j++){
	Controller * c = controllerArray[j];
	//printf("%f\t%f\n", distArray[i], stdXArray[i]);

	if(c->GetDist() == distArray[i]){
	  c->SetDistRank( (float) i );
	}
	if(c->GetStdX() == stdXArray[i]){
	  c->SetStdXRank( (float) i );
	}
	if(c->GetNCollide() == collideArray[i]){
	  c->SetNCollideRank( (float) i );
	}
      }
    }

    for(int i = 0; i <numberOfControllers; i++){
      Controller * c = controllerArray[i];
      
      float fitness = 0.;
      
      fitness += controllerArray[i]->GetDistRank();
      fitness += controllerArray[i]->GetStdXRank();
      fitness += controllerArray[i]->GetNCollideRank();

      fitness /= 3.;

      c->SetFitness(fitness); 
    }

  }


 private:

  int numberOfControllers;
  Controller ** controllerArray;

  void IncreaseSize(){

    // Create temp array to store old array
    Controller* tempArray[numberOfControllers];
    for(int i = 0; i < numberOfControllers; i++)
      tempArray[i] = controllerArray[i];

    // Create a new longer node
    controllerArray = new Controller*[numberOfControllers+1];
    
    // Copy from temp back to nodeArray
    for(int i = 0; i < numberOfControllers; i++)
      controllerArray[i] = tempArray[i];
   
    // Increase node size by one
    numberOfControllers++;    
    
    return;
  }

};

#endif
