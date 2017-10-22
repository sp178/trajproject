//#include "modelbase.h"
#include <iostream>
#include <string>
#include <tuple>
#include <vector>
using namespace std;
int main()
{
    vector<tuple<string, string, string>> test;
    test.push_back(make_tuple("hehe", "hehe", "hehe"));
    for (auto test1 : test)
        cout << get<0>(test1) << endl;
    return 0;
}