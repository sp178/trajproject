#include"spinterplation.hpp"
int main()
{
    BLOCK<2>* data =  makeCuBlock<2>("te","test");
    InterRecod<2> m_recorder;
    double test[2];
    double data1 = interplate(data,test,&m_recorder);
    return 0;
}