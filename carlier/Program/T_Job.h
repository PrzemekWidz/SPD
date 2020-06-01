#pragma once
#include <iostream>

class T_Job
{
public:
	int R, P, Q;

	int P_Shrage;

	int initialIndex;

	bool leftStick = false;

	int timeWhenJobBegin = 0;

	T_Job job() {};

	
	void showInitIndex();
};

