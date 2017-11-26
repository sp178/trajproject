#include "testmodel.h"

SPEXPORT_WITH_NAME(testmodel1,testmodel);

SPEXPORT_WITH_NAME(testmodel2,testmodel);


testmodel::testmodel()
{
printf("model\n");
};
testmodel::~testmodel()
{

};
int testmodel::onInitial()
{

    printf("%f %f %f \n",m_params[0],m_params[1],m_params[2]);
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