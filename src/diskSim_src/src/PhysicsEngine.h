#ifndef PHYSENG
#define PHYSENG

#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string>

#include "robot.h"
#include "Vector2d.h"

class PhysicsEngine{
 public:
  PhysicsEngine(float tStep){
    timeStep = tStep;

    nSubSteps = 10;
    miniStep = timeStep/(float)nSubSteps;
    
  }
  
  void Step(vector<Robot *> robotVector){
    
    // The step will actually be carried out in 10 smaller steps.
    // This is being done to increase accuracy of collision detection.
    
    for(int iStep = 0; iStep < nSubSteps; iStep++)
      SubStep(miniStep,robotVector);
   
  }
 
  bool CheckForCollision(int currentIndex, Vector2d * newPos, vector<Robot *> robotVector){
   
    Robot * r1 = robotVector[currentIndex];

    for(unsigned i = 0; i < robotVector.size(); i++){
      if(i == currentIndex) continue;

      Robot * r2 = robotVector[i];

      Vector2d* disp = 
	  SubtractVectors(r2->GetPositionVector(), newPos);
      
      float minSeperation = r1->GetRadius() + r2->GetRadius()+2.;
	
      if( minSeperation > disp->GetNorm() ){
	// collision has occured
	delete disp;
	r1->AddCollision();
	return true;
      }
      delete disp;
    }

    // No collision
    return false;
  }
 
  void SubStep(float miniStep,vector<Robot *> robotVector){
    //printf("~~~~~~~~~~~~~~~~~~~~~~~\n");
    // First check for collisions 
    /*for(unsigned i = 0; i < robotVector.size(); i++){
      Robot * r1 = robotVector[i];
      
      for(unsigned j = i+1; j < robotVector.size(); j++){
	Robot * r2 = robotVector[j];
	
	Vector2d* disp = 
	  SubtractVectors(r2->GetPositionVector(), r1->GetPositionVector());
	
	float minSeperation = r1->GetRadius() + r2->GetRadius()+1.;
	
	if( minSeperation > disp->GetNorm() ){
	  //printf("%d\t%d\n",i,j);
	  HandleCollision(r1,r2);
	}

	delete disp;
      }
      }*/
    
    // Now update all the robot positions
    for(unsigned int i = 0; i < robotVector.size(); i++){
      Robot * aRobot = robotVector[i];
      
      Vector2d* displacementVector = 
	ScalarMultiplyVector(miniStep, aRobot->GetVelocityVector());
      
      Vector2d* newPos = AddVectors(aRobot->GetPositionVector(),
				    displacementVector);

      if( !CheckForCollision(i, newPos, robotVector) )
	aRobot->UpdatePosition(newPos);

      delete displacementVector;
    } 
    
  }

  void SlideApart(Robot * r1, Robot * r2){
    // Determine the distance between robot centers (dist) and how much
    // they overlap (delta)
   
    Vector2d* collisionNormal = SubtractVectors(r2->GetPositionVector(),
						r1->GetPositionVector());

    float dist = collisionNormal->GetNorm();
    float minDist = r1->GetRadius() + r2->GetRadius();
    float delta = fabs(minDist - dist);
    
    collisionNormal->Normalize();
    
    // Now slide each robot back a distance delta/2 along the collision normal
    Vector2d * r1_slide = 
      ScalarMultiplyVector(-0.5*delta, collisionNormal);
    Vector2d * r2_slide = 
      ScalarMultiplyVector(0.5*delta, collisionNormal);

     r1->UpdatePosition( AddVectors(r1->GetPositionVector(), r1_slide) );
     r2->UpdatePosition( AddVectors(r2->GetPositionVector(), r2_slide) );
    
     delete r1_slide;
     delete r2_slide;
     delete collisionNormal;
  }

  // collisionNormal points from r1 to r2
  void HandleCollision(Robot * r1, Robot * r2){

    // Slide the 
    SlideApart(r1,r2);
    // Make sure the vector is indeed a normal and is in the right direction.
    Vector2d* collisionNormal = SubtractVectors(r2->GetPositionVector(),
						r1->GetPositionVector());
    collisionNormal->Normalize();
     

    Vector2d* v1_c = ProjectAonB(r1->GetVelocityVector(), collisionNormal); 
    Vector2d* v2_c = ProjectAonB(r2->GetVelocityVector(), collisionNormal); 

    Vector2d* v1_p = SubtractVectors(r1->GetVelocityVector(), v1_c);
    Vector2d* v2_p = SubtractVectors(r2->GetVelocityVector(), v2_c);
  
    // Now just use momentum conservation to change in v1_c after the collision
    float m1 = r1->GetMass();
    float m2 = r2->GetMass();

    Vector2d * a1 = ScalarMultiplyVector( (m1-m2) / (m1+m2), v1_c );
    Vector2d * b1 = ScalarMultiplyVector( 2.*m2 / (m1+m2), v2_c );
    Vector2d * a2 = ScalarMultiplyVector( (m2-m1) / (m1+m2), v2_c );
    Vector2d * b2 = ScalarMultiplyVector( 2.*m1 / (m1+m2), v1_c );
  
    v1_c = AddVectors(a1,b1);
    v2_c = AddVectors(a2,b2);

    // Add the vector adjusted for the collision back to the perp component
   
    Vector2d * v1 = AddVectors(v1_c, v1_p);
    Vector2d * v2 = AddVectors(v2_c, v2_p);

    // v1 and v2 are the robots new velocities
    r1->UpdateVelocity(v1);
    r2->UpdateVelocity(v2);

    // Move the robots off each other
    //r1->RevertToPreviousPosition();
    //r2->RevertToPreviousPosition();

    delete v1_c;
    delete v2_c;
    delete v1_p;
    delete v2_p;
    delete a1;
    delete b1;
    delete a2;
    delete b2;
  }

 private:
  float timeStep;
  int nSubSteps;
  float miniStep;

};

#endif
