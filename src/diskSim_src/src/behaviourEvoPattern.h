#ifndef EVO_H
#define EVO_H

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "behaviour.h"
#include "Vector2d.h"
#include "sensorContact.h"

class BehaviourEvoPattern: public Behaviour{

 public:

  BehaviourEvoPattern(float normAng, float angWidth1, float angWidth2, string s){
  
    angWidthCenter = angWidth1;
    angWidthSide = angWidth2;

    formationNormal = new Vector2d(normAng);
    formationPerp = new Vector2d(normAng + M_PI/2.);

    chromosome = s;
        
  }
  ~BehaviourEvoPattern(){};
 
  int CalculateCommand(){
    int cmd = 0;

    if(zoneA) cmd += 1;
    if(zoneB) cmd += 2;
    if(zoneC) cmd += 4;
    if(zoneD) cmd += 8;
    if(zoneE) cmd += 16;

    return cmd;
  }


  // Returns a velocity vector
  Vector2d * GetResponse(){
    //response = ScalarMultiplyVector(0., formationNormal);
    
    char cmd = chromosome[ CalculateCommand() ];
    //printf("%c\n",cmd);
    //printf("%d\n",CalculateCommand());

    if(cmd == 'N') response = ScalarMultiplyVector(1., formationNormal);
    else if(cmd == 'E') response = ScalarMultiplyVector(1., formationPerp);
    else if(cmd == 'S') response = ScalarMultiplyVector(-1., formationNormal);
    else if(cmd == 'W') response = ScalarMultiplyVector(-1., formationPerp);
    else response = ScalarMultiplyVector(0., formationPerp);

    if(response->GetNorm() > 0.){
      //response = AddVectors(response, new Vector2d( 2.*M_PI*Random() ));
    }
      
    return response;
  }

  // Takes in a list of vectors giving the local position of neighbours
  void UpdateStimulus( std::vector<SensorContact*> contacts){
    // Reset Stimuli
    zoneA = false;
    zoneB = false;
    zoneC = false;
    zoneD = false;
    zoneE = false;

    // Now evaluate the contacts
    for(unsigned int i = 0; i < contacts.size(); i++){
      Vector2d * v = contacts[i]->GetVector();
      
      // Determine the bearing of the contact;
      float contactBearing =  fabs(v->GetAngle());      

      if(contactBearing < angWidthCenter) 
	zoneA = true;
      else if(contactBearing < M_PI/2. - angWidthSide/2.)
	zoneB = true;
      else if(contactBearing < M_PI/2. + angWidthSide/2.)
	zoneE = true;
      else if(contactBearing < M_PI - angWidthCenter)
	zoneC = true;
      else
	zoneD = true;

    } // End of contact loop.  
    
  }

  // Display information to the screen.
  void Print(){
    printf("EvoPattern\t%f\n",activationLevel);
  }

 private:
  float Random(){
    return rand()/(float)(RAND_MAX);
  }
  // Stimuli variables
  bool zoneA, zoneB, zoneC, zoneD, zoneE;

  // Variables set during initialization
  float angWidthCenter, angWidthSide;
  
  string chromosome;

  Vector2d * formationPerp; 
  Vector2d * formationNormal;
};
#endif
