#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <fstream>
#include <iostream>

#ifdef WITHX
#include "xHandle.h"
#endif

#include "displayable.h"
#include "Vector2d.h"
#include "robot.h"
#include "referee.h"
#include "PhysicsEngine.h"
#include "controllerCoop.h"

#include "behaviourRandom.h"
#include "behaviourAvoid.h"
#include "behaviourEvoPattern.h"

using namespace std;

// Function prototypes
bool CollisiionDetected(vector<Robot *> robotVector);
void SaveData(string chromo, float avgX, float avgY, float stdX, float stdY, float avgCollide);
void CalculateStats(vector<Robot *> robotVector, float& avgX, float& avgY, float& stdX, float& stdY, float& avgCollide);
//vector<Robot *> InitializeRobots(string chromo = "0ENESENEW0W0W0000000000000000000");
vector<Robot *> InitializeRobots(string chromo = "00000000000000000000000000000000");


// Configuration global simulations values;
int nRobots = 4;
int maxTimeSteps = 10000;
bool runHeadless = false;
int runID = 0;

float GetRandom(){
    return (float)rand()/(float)RAND_MAX;
 }

/* The almighty MAIN */
int main(int argc, char* argv[]){

  // Random number initialization
  srand (time(NULL));

  nRobots = atoi(argv[1]);
  maxTimeSteps = atoi(argv[2]);
  runHeadless = atoi(argv[3]);
  runID = atoi(argv[4]);

  // This object does all the graphics handling.
#ifdef WITHX
  xHandle * xHandler = new xHandle();
#endif
  
  // Create a PhysicsEngine object 
  PhysicsEngine * physEng = new PhysicsEngine(0.015);

  // Create a referee object which will control the flow of the sim
  Referee * ref = new Referee(physEng);
  
  // Create the robot objects.  
  ifstream inputFile;
  std::stringstream inputFileName;
  inputFileName << "./untested/individual"<< runID <<".dat";
  //printf("%s\n", inputFileName.str().c_str());
  inputFile.open(inputFileName.str().c_str(), std::fstream::in);
  string chromo;
  inputFile >> chromo;
  //printf("%s\n", chromo.c_str());
  vector<Robot *> robotVector = InitializeRobots(chromo);
  inputFile.close();

  // Add the robots to referee.
  for(unsigned int i = 0; i < robotVector.size(); i++){
     ref->AddRobot(robotVector[i]);
  }
 
  // Display initial positions to the stdout
  //if(!runHeadless) ref->Display();


  //====================================================================
  // The main simulation loop!
  //====================================================================

  int currentTimeStep = 0;

  while(currentTimeStep < maxTimeSteps) {	   
   // Have the referee calculate the results of the next time step.
    ref->Update();
    
    if(!runHeadless){
#ifdef WITHX
      xHandler->process();
#endif 

      // Update the graphical display of the simulation
#ifdef WITHX
      if(currentTimeStep%10==0)	
	xHandler->UpdateScene(ref->GetDisplayableRobots(), currentTimeStep);
      
      // Sleep a little bit in order to avoid eating all the cpu. 
      usleep(100);
   
#endif
    }  
    currentTimeStep++;
     
    // Update every so often with some info on progress
    if(currentTimeStep % 1000 == 0 && !runHeadless){
      float avgX, avgY, stdX, stdY, avgCollide;
      CalculateStats(robotVector, avgX, avgY, stdX, stdY, avgCollide);
      printf("%d\t%f\t%f\t%f\t%f\t%f\n", currentTimeStep,avgX-400., avgY-300., stdX, stdY, avgCollide);
    }
    
  }
   
  float avgX, avgY, stdX, stdY, avgCollide ;
  CalculateStats(robotVector, avgX, avgY, stdX, stdY, avgCollide);
  SaveData(chromo, avgX, avgY, stdX, stdY, avgCollide);
  
#ifdef WITHX
  xHandler->End();
#endif
   
  return 0;
    };

//=============================================================================

void CalculateStats(vector<Robot *> robotVector, 
		    float& avgX, float& avgY, float& stdX, float& stdY, float& avgCollide ){
  
  Vector2d* avgVector = new Vector2d(0.,0.);
  avgCollide = 0.;

  unsigned int nRobots =  robotVector.size();

  for(unsigned int i = 0; i < nRobots; i++){
    Robot* aRobot = robotVector[i];
    
    avgVector = AddVectors(aRobot->GetPositionVector(), avgVector);
  
    avgCollide += aRobot->GetCollisionCounter();
  }

  avgCollide /= nRobots;
  
  avgX = avgVector->GetX() / nRobots;
  avgY = avgVector->GetY() / nRobots;

  stdX = 0.;
  stdY = 0.;

  for(unsigned int i = 0; i < nRobots; i++){
    Robot* aRobot = robotVector[i];

    stdX += pow(aRobot->GetPositionVector()->GetX() - avgX, 2);
    stdY += pow(aRobot->GetPositionVector()->GetY() - avgY, 2);
    
  }

  stdX = sqrt(stdX/nRobots);
  stdY = sqrt(stdY/nRobots);

  return;
}

//=============================================================================

void SaveData(string chromo, float avgX, float avgY, float stdX, float stdY, float avgCollide){
  ofstream myfile;
 
  float distTravelled = sqrt(avgX*avgX + avgY* avgY);
  float fitness = 1. / (distTravelled + stdX + stdY);
  //float fitness = stdX + stdY;

  std::stringstream outputFileName;
  outputFileName << "./results/outputData" << runID;
  myfile.open(outputFileName.str().c_str(), std::fstream::app);
  myfile << chromo <<"\t" << fitness << "\t" << distTravelled
	 <<"\t"<< stdX <<"\t"<< stdY <<"\t"<<avgCollide<<"\n"; 
  myfile.close();

  //printf("%s\t%f\n", chromo.c_str(), fitness);
  ///printf("%s\t%f\t%f\t%f\t%f\n", chromo.c_str(), fitness, distTravelled, stdX, stdY);
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

bool CollisiionDetected(vector<Robot *> robotVector){
  Robot * r1 = robotVector.back();

  for(unsigned int j =0; j < robotVector.size()-1; j++){
    Robot * r2 = robotVector[j];
    
    Vector2d* disp_ij = 
      SubtractVectors(r1->GetPositionVector(), r2->GetPositionVector());
    
    float minSeperation = r1->GetRadius() + r2->GetRadius() + 3.;
       
    if(disp_ij->GetNorm() < minSeperation){
      delete disp_ij;
      return true;
    }
    delete disp_ij;
  } 

  return false;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
vector<Robot *> InitializeRobots(string chromo){

  vector<Robot *> robotVector;
  
  for(int i = 0; i < nRobots; i++){
    // Create a new controller    
    ControllerCoop* aController = new ControllerCoop();
    //aController->AddBehaviour( new BehaviourAvoid(15.) ); 
    aController->AddBehaviour( new BehaviourRandom(1.0) ); 
    aController->AddBehaviour(new BehaviourEvoPattern(0., 45.*M_PI/180., 5.*M_PI/180., chromo) );
    
    // Initialize position and velocity of robot
    Vector2d * velocityVector = new Vector2d(0.,0.);
    Vector2d * positionVector = new Vector2d(2.*M_PI*GetRandom());
    positionVector = ScalarMultiplyVector(50.*GetRandom(), positionVector);
    positionVector = AddVectors(positionVector, new Vector2d(400., 300.) );
    
    // Create robot
    Robot* aRobot = new Robot (positionVector, velocityVector, i, aController);
   
    // Add the robot to the robot vector
    robotVector.push_back(aRobot);
 
    // After adding the last robot check to see if it is on top of another robot
    // if so reposition it!
    while(CollisiionDetected(robotVector)){
      Vector2d * positionVector = new Vector2d(2.*M_PI*GetRandom());
      positionVector = ScalarMultiplyVector(50.* GetRandom(), positionVector);
      positionVector = AddVectors(positionVector, new Vector2d(400., 300.) );
      robotVector.back()->UpdatePosition(positionVector);
    }
  }

  // Now we are ready to simulate!
  return robotVector;
};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

