#ifndef DISP
#define DISP

#ifdef WITHX
class xHandle;
#endif

class Displayable{ 
public:

#ifdef WITHX
  virtual void paint(xHandle* xHandler) = 0;
#endif

};    

#endif
