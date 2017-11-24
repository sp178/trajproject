#pragma once
#include"..\utility\spmath.h"
struct Traj {
	double r, miu, lambda, V, gamma, chi, rengeS, targetmiu, targetlambda, lanuchangle, stogo, begLongt, begLat;
};		//弹道积分初始值
struct interPlatheData
{
	spfloat h, alpha, Ma;
};