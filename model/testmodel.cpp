#include "testmodel.h"
SPEXPORT_WITH_NAME(testmodel, testmodel);
int testmodel::onInitial()
{
    return 0;
}; //初始化对应SM_INITIAL
int testmodel::onClear()
{
    return 0;
}; //初始化对应SM_INITIAL
int testmodel::onUpdate()
{
    return 0;
}; //对应 SM_WRITE
int testmodel::onStop()
{
    return 0;
}; //对应SM_STOP
int testmodel::onDerive()
{
    return 0;
}; //对应SM_CONTINUE