#include "spmodel.h"
class testmodel : public spmodel
{
public:
  testmodel();
  ~testmodel();

protected:
  int onInitial(); //初始化对应SM_INITIAL
  int onClear();   //初始化对应SM_INITIAL
  int onUpdate();  //对应 SM_WRITE
  int onStop();    //对应SM_STOP
  int onDerive();  //对应SM_CONTINUE
};