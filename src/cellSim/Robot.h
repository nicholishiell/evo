#ifndef ROBOT_H
#define ROBOT_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <vector>
#include <string>

using namespace std;

class Robot{
 public:
  Robot(int posX, int posY, string c, int r, int id = 0){

    x = posX;
    y = posY;

    range = r;

    controller = c;

    zoneA = false;
    zoneB = false;
    zoneC = false;
    zoneD = false;
    zoneE = false;

    number = id;
  }

  ~Robot(){}

  int GetX(){return x;}
  int GetY(){return y;}
  int GetRange(){return range;}
  int GetID(){return number;}

  void Print(){
    printf("%d\n", number);
  }

  void UpdatePosition(int posX, int posY){
    x = posX;
    y = posY;
  }

  void ResetSensors(){
    zoneA = false;
    zoneB = false;
    zoneC = false;
    zoneD = false;
    zoneE = false;
  }

  void UpdateSensors(int contactX, int contactY){
    
    // zoneA
    if(y == contactY and x < contactX) zoneA = true;

    // zoneB
    if(x < contactX and y != contactY) zoneB = true;

    // zoneC
    if(x > contactX and y != contactY) zoneC = true;

    if(y == contactY and x > contactX) zoneD = true;

    // Zone E
    if(x == contactX and y < contactY) zoneE = true;
    if(x == contactX and y > contactY) zoneE = true;

  }

  char GetMove(){
    
    int cmd = 0;
    
    if(zoneA) cmd += 1;
    if(zoneB) cmd += 2;
    if(zoneC) cmd += 4;
    if(zoneD) cmd += 8;
    if(zoneE) cmd += 16;

    return controller[cmd];
  }


 private:

  int x,y;
  int range;
  int number;
  string controller;
  bool zoneA, zoneB, zoneC, zoneD, zoneE;

};
#endif
