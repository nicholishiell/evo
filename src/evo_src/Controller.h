#ifndef CONTROLLER
#define CONTROLLER

#include <stdio.h>
#include <string>


using namespace std;

class Controller{
public:
  Controller(string s, float f, float d, float sx, float sy, float nc){
    
    chromo = s;
    fitness = f;
    dist = d;
    stdX = sx;
    stdY = sy;
    nCollide = nc;
    
    distRank = 0;
    stdXRank = 0;
    stdYRank = 0;
    nCollideRank = 0;
  }

  Controller(Controller* c){
    chromo = c->GetChromo();
    fitness = c->GetFitness();
    dist = c->GetDist();
    stdX = c->GetStdX();
    stdY = c->GetStdY();
    nCollide = c->GetNCollide();
  }

  ~Controller(){};

  int CompareChromo(Controller * a){

    int similarity = 0;
    string otherChromo = a->GetChromo();

    for(int i = 0; i < chromo.size(); i++){

      if(otherChromo[i] == chromo[i]) similarity++;
    
    }

    return similarity;

  }

  float * GetAllFitnessMeasures(){
    float * f = new float[3];
    
    f[0] = dist;
    f[1] = stdX;
    f[2] = nCollide;

    return f;
  }
  
  bool DominatedBy(Controller * c){
    int nLesser = 0;
    int nGreater = 0;
    int nEqual = 0;
    
    float * f1 = GetAllFitnessMeasures();
    float * f2 = c->GetAllFitnessMeasures();

    // Loop over all the objectives and count how many
    // components are greater, equal, or less than
    for(int i = 0; i < 3; i++){
      float f1_i = f1[i];
      float f2_i = f2[i];
      
      if(f1_i == f2_i) nEqual++;
      else if(f1_i > f2_i) nGreater++;
      else nLesser++;
    }
    
    if(nLesser > 0) return false;
    else if(nGreater > 0) return true;    
    else return false;
  } 

 
  string GetChromo(){return chromo;}
  int GetChromoLength(){return chromo.size();}

  float GetFitness(){return fitness;}
  float GetDist(){return dist;}
  float GetStdX(){return stdX;}
  float GetStdY(){return stdY;}
  float GetNCollide(){return nCollide;}

  float GetDistRank(){return distRank;}
  float GetStdXRank(){return stdXRank;}
  float GetStdYRank(){return stdYRank;}
  float GetNCollideRank(){return nCollideRank;}

  void SetFitness(float f){fitness = f;}
  void SetDistRank(float d){distRank = d;}
  void SetStdXRank(float s){stdXRank = s;}
  void SetStdYRank(float s){stdYRank = s;}  
  void SetNCollideRank(float n){nCollideRank = n;}

  void Print(){  
    printf("%s\t%f\t%f\t%f\t%f\t%f\n", chromo.c_str(), fitness, dist, stdX, stdY, nCollide);
  }

private:
  string chromo;
  float fitness;
  float dist;
  float stdX;
  float stdY;
  float nCollide;
  
  float distRank;
  float stdXRank;
  float stdYRank;
  float nCollideRank;
};

#endif
