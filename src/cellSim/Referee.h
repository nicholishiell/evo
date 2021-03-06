#ifndef REFEREE_H
#define REFEREE_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

#include "Robot.h"

class Referee{

 public:
  Referee(vector<Robot*> r, int mX, int mY);

  void Step();

  void ResetSensors();
  void UpdateSensors();
  void UpdateGrid();

  bool CheckRange(Robot *r1, Robot *r2);

  bool CollisionDetected(Robot*& r1, Robot*& r2);

  void MoveBack(Robot *r);

  void Display();

  void GetStats(float& dist, float& stdX, float& stdY);

 private:
  
  float GetRandom(){
    return (float)rand()/(float)RAND_MAX;
  }
  
  vector<Robot*> robotVector;
  int maxX;
  int maxY;
  char** grid;
};

//============================================================================

Referee::Referee(vector<Robot*> r, int mX, int mY){
  robotVector = r;

  maxX = mX;
  maxY = mY;

  grid= new char*[maxX];
  for(int i = 0 ; i < maxX; i++)
    grid[i] = new char[maxY];
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void Referee::GetStats(float& dist, float& stdX, float& stdY){

  unsigned int nRobots =  robotVector.size();

  float avgX, avgY;

  avgX = 0.;
  avgY = 0.;

  for(unsigned int i = 0; i < nRobots; i++){
    Robot* aRobot = robotVector[i];
    
    avgX += aRobot->GetX();
    avgY += aRobot->GetY();
    
  }

  avgX = avgX / nRobots;
  avgY = avgY / nRobots;

  dist = sqrt(avgX*avgX + avgY*avgY);  

  stdX = 0.;
  stdY = 0.;

  for(unsigned int i = 0; i < nRobots; i++){
    Robot* aRobot = robotVector[i];

    stdX += pow(aRobot->GetX() - avgX, 2);
    stdY += pow(aRobot->GetY() - avgY, 2);
    
  }

  stdX = sqrt(stdX/nRobots);
  stdY = sqrt(stdY/nRobots);

  return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void Referee::Step(){
  ResetSensors();
  
  UpdateSensors();
  
  UpdateGrid();
  
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void Referee::ResetSensors(){
  for(unsigned int i =0; i < robotVector.size(); i++)
    robotVector[i]->ResetSensors();
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

bool Referee::CheckRange(Robot *r1, Robot *r2){
  float range = (float)r1->GetRange();
  int x1 = r1->GetX();
  int y1 = r1->GetY();
  int x2 = r2->GetX();
  int y2 = r2->GetY();
  
  float dist = sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) );
  
  if(dist <= range) return true;
  else return false;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void Referee::UpdateSensors(){
  
  for(unsigned int i =0; i < robotVector.size(); i++){
    Robot * r1 = robotVector[i];
    
    for(unsigned int j = 0; j < robotVector.size(); j++){
      if(i == j) continue;
      Robot * r2 = robotVector[j];
      
      if(CheckRange(r1, r2)) r1->UpdateSensors(r2->GetX(), r2->GetY());
    }
    
  }
  
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

bool Referee::CollisionDetected(Robot*& r1, Robot*& r2){

  for(unsigned int i = 0; i < robotVector.size(); i++){
    r1 = robotVector[i];
    for(unsigned int j = 0; j < robotVector.size(); j++){

      if(i == j ) continue;

      r2 = robotVector[j];
      
      if(r2->GetX() == r1->GetX() and r2->GetY() == r1->GetY() ){
	return true;
      }
      
    } 
  }

  return false;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Referee::MoveBack(Robot *r){

  char move = r->GetMove();
  int pX = r->GetX();
  int pY = r->GetY();

  if(move == 'N') pX--;
  else if(move == 'S') pX++;
  else if(move == 'E') pY--;
  else if(move == 'W') pY++;
  else pX=pX;
  
  r->UpdatePosition(pX, pY);

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void Referee::UpdateGrid(){

  // Update all the robot positions.
  for(unsigned int i =0; i < robotVector.size(); i++){
    Robot * r1 = robotVector[i];
    int pX = r1->GetX();
    int pY = r1->GetY();
    char move = r1->GetMove();

    if(move == 'N') pX++;
    else if(move == 'S') pX--;
    else if(move == 'E') pY++;
    else if(move == 'W') pY--;
    else pX=pX;

    r1->UpdatePosition(pX, pY);
    Robot *r2;
    if( CollisionDetected(r1, r2) ){
      MoveBack(r1);
      printf("collision\n");
      r1->Print();
      r2->Print();
    }
  }
  // If a collision is detected these variables will hold the colliding robots.
  /*Robot *r1;
  Robot *r2;

  while( CollisionDetected(r1, r2) ){
    // If one of the robots didnt move then the other one should be moved back
    if(r1->GetMove() == '0') MoveBack(r2);
    else if(r2->GetMove() == '0') MoveBack(r1);      
    
    //MoveBack(r1);
    //MoveBack(r2);
    r1->Print();
    r2->Print();


    // If both robots moved them randomly pick one move back
    if(GetRandom() > 0.5) MoveBack(r1);
    else MoveBack(r2);
    
    printf("collision\n");
    getchar();
    }*/

  return;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void Referee::Display(){

 for(int i = 0; i < maxX; i++)
    for(int j = 0; j < maxY; j++)
      grid[i][j] = '-';

 for(unsigned int i =0; i < robotVector.size(); i++){
    Robot * r1 = robotVector[i];
    int pX = r1->GetX();
    int pY = r1->GetY();

    if( pX >= maxX || pX < 0 ) continue;
    if( pY >= maxY || pY < 0 ) continue;
      
    //grid[pX][pY] = 'O';
    char * a;
    grid[pX][pY] = r1->GetID() + '0';
 }
  
  for(int i = 0; i < maxX; i++){
    for(int j = 0; j < maxY; j++){
      printf("%c",grid[j][i]);
    }
    printf("\n");
  }
  
  return;
}
#endif
