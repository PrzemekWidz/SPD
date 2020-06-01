#pragma once

// Przechowuje dane o kolejnosci zadan, ich dlugosci (zgodnie z klasa T_job)
// oraz dane o dlugosci wyliczonego CMAX.
#include <vector>

#include "T_Job.h"

class T_Data
{
public:
	std::vector<T_Job> order;
	int C_MAX;
};

