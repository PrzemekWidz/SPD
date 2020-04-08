#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <functional> 
#include <cstdlib>
#include <assert.h>
#include <chrono>
#include <ctime>


int CMaxArray[501][21]; // +1, dla tablicy uzupelnionej o zera, do obliczania CMax

double fullProgramTime;
double fullNEHTime;
double fullCMAXtime;
int fullJobsTime;
//int** CmaxArray;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct T_Job {
	int entireJobTime{};    // Full job time, sum on all machines
	int Index{};

	int oneJobTime[500];	// Job time for every machine, max 500 jobs in data file

	void calculateEntireJobTime(int size);
	void showJobStats();
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int Cmax(T_Job jobs[], const int jobsNumber, const int machinesNumber);
int AlgorithmNEH(T_Job jobs[], int jobsNumber, int machinesNumber, int dataIndex);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int main()
{
	clock_t beginProgram_time = clock();

	std::string fileName = "NEH_data.txt";
	int dataInstanceNumber = 120;

	std::fstream file;


	int jobsNumber{};
	int machinesNumber{};
	int currentCMax{};


	file.open(fileName.c_str(), std::fstream::in);

	if (!file.is_open()) {
		std::cout << "File open error " << std::endl;
	}

	std::string dataNumberNameFromFile; // data.___

	
	// Read 120 data files
	for (int iG = 0; iG <= dataInstanceNumber; iG++) {
	
		clock_t beginLoop = clock();

		int resultNEH;


		file >> dataNumberNameFromFile;  // data.___
		file >> jobsNumber;
		file >> machinesNumber;
			

		// ZAPIS CZASOW ZADAN Z PLIKU DO STRUKTUR
		T_Job currentJob;
		T_Job jobs[500];

		int currentTimeValue{};
		int currentSumOfTime{};

		assert(jobsNumber > 0 && machinesNumber > 0);

		for (int i = 0; i < jobsNumber; i++) {


			for (int j = 0; j < machinesNumber; j++) {
				file >> currentTimeValue;
				currentJob.oneJobTime[j] = currentTimeValue;
			}
			currentJob.Index = i;
			currentJob.calculateEntireJobTime(machinesNumber);

			jobs[i] = currentJob;

			// Wyswietlenie calosci czasu dla danego zadania
			//std::cout << jobs[i].entireJobTime  << std::endl;
		}

		// SORTOWANIE WEDLUG CALOSCI CZASU ZADANIA
		std::sort(jobs, jobs + jobsNumber, [](const T_Job& first,
			const T_Job& second) {return first.entireJobTime > second.entireJobTime; });

		// Wyswietlenie posortowanej talbicy wedlug czasow:
		//for (int i = 0; i < jobsNumber; i++) {
		//	std::cout << jobs[i].Index  << std::endl;
		//}


		// PO POSORTOWANIU WDLUG CZASU, SORTOWANIE WEDLUG INDEXOW(jesli czas byl taki sam)
		int indexSameTime1{};
		int indexSameTime2{};

		for (int i = 0; i < jobsNumber; i++) {
			if (jobs[i].entireJobTime == jobs[i + 1].entireJobTime) {
				indexSameTime1 = i;

				while (jobs[i].entireJobTime == jobs[i + 1].entireJobTime) {
					i++;
				}
				indexSameTime2 = i;
			}

			std::sort(jobs + indexSameTime1, jobs + indexSameTime2 + 1, [](const T_Job& first, const T_Job& second) {
				return first.Index < second.Index;
				});

		}

		//  Wyswietlenie posortowanej talbicy wedlug czasow z indeksami:
		// for (int i = 0; i < jobsNumber; i++) {
		// 	std::cout << jobs[i].Index  << std::endl;
		// }

		// Wyswietlenie tablicy zadan i czasow: */
		/*
		 for (int i = 0; i < jobsNumber; i++) {
			for (int j = 0; j < machinesNumber; j++) {
				std::cout << jobs[i].oneJobTime[j] << " ";
			}
			std::cout << std::endl;
		}
		*/

		// Obliczenie CMAX dla posortowanej wstepnie tablicy
		int cmaxTimeValue = 0;
		cmaxTimeValue = Cmax(jobs, jobsNumber, machinesNumber);


		resultNEH = AlgorithmNEH(jobs, jobsNumber, machinesNumber, iG);
	}

	clock_t endProgram_time = clock();

	fullProgramTime = double(endProgram_time - beginProgram_time) / CLOCKS_PER_SEC;


	std::cout << "Full program time: " << fullProgramTime << " s" << std::endl
		<< "Full NEH time: " << fullNEHTime << " s" << std::endl
		<< "Full CMax time: " << fullCMAXtime << " s" << std::endl
		<< "% of CMax in entire time: " << double(fullCMAXtime / fullProgramTime) * 100 << " %" << std::endl;

	file.close();

	return 0;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void T_Job::calculateEntireJobTime(int size) {
	entireJobTime = 0;

	for (int i = 0; i < size; i++) {
		entireJobTime += oneJobTime[i];
	}
}

//---------------------------------------------------------------------------------------------------------------------------

void T_Job::showJobStats() {
	std::cout << "===== Jobs stats =====" << std::endl
		<< "Index: " << Index << std::endl
		<< "One job Time: " << oneJobTime[Index] << std::endl
		<< "==========" << std::endl;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int Cmax(T_Job jobs[], const int jobsNumber, const int machinesNumber) {

	// int** CMaxArray;

	/*
	CMaxArray = new int* [jobsNumber+1];
	for (int i = 0; i < jobsNumber+1; i++) {
		CMaxArray[i] = new int[machinesNumber+1];
	}
	*/


	clock_t beginCMax_time = clock();

	int currentSumTimeCMax{};


	for (int i = 0; i < jobsNumber; i++) {
		CMaxArray[i][0] = 0;
	}

	for (int i = 0; i < machinesNumber; i++) {
		CMaxArray[0][i] = 0;
	}


	for (int i = 1; i <= jobsNumber; i++) {
		for (int j = 1; j <= machinesNumber; j++) {
			currentSumTimeCMax = jobs[i - 1].oneJobTime[j - 1];

			CMaxArray[i][j] = std::max(CMaxArray[i - 1][j], CMaxArray[i][j - 1]) + currentSumTimeCMax;
		}
	}


	clock_t endCMax_time = clock();
	double CMax_time = double(endCMax_time - beginCMax_time) / CLOCKS_PER_SEC;
	fullCMAXtime += CMax_time;

	int CMaxResult = CMaxArray[jobsNumber][machinesNumber];


	return CMaxArray[jobsNumber][machinesNumber];
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int AlgorithmNEH(T_Job jobs[], int jobsNumber, int machinesNumber, int dataIndex) {

	// Pomiar czasu start 
	clock_t beginNEH_time = clock();

	int currentCMax{};
	int shortestCMax{};

	int swapNumber{};
	int smallestBestCMaxSwapNumer{};

	for (int currentJobIndex = 0, currentSize = 2; currentJobIndex < jobsNumber - 1; currentJobIndex++, currentSize++) {

		currentCMax = Cmax(jobs, currentSize, machinesNumber);
		shortestCMax = currentCMax;

		// Swap jobs to get best time
		swapNumber = 0;
		smallestBestCMaxSwapNumer = 0;

		for (int i = currentSize - 1; i >= 1; i--) {
			swapNumber++;

			// Zamiana kolejnosci zadan
			std::swap(jobs[i], jobs[i - 1]);
			currentCMax = Cmax(jobs, currentSize, machinesNumber);

			if (currentCMax <= shortestCMax) {
				shortestCMax = currentCMax;
				smallestBestCMaxSwapNumer = swapNumber;
			}
		}


		// Wycofanie sie do najlepszego wyniku
		for (int k = swapNumber, index = 0; k > smallestBestCMaxSwapNumer; k--) {
			std::swap(jobs[index], jobs[index + 1]);
			index++;
		}


	}

	int result{};

	result = Cmax(jobs, jobsNumber, machinesNumber);

	// Pomiar czasu stop
	clock_t endNEH_time = clock();

	double NEH_time = double(endNEH_time - beginNEH_time) / CLOCKS_PER_SEC;
	fullNEHTime += NEH_time;

	std::cout << "Cmax result " << dataIndex << ": " << result << " Time: " << NEH_time << " s" << std::endl;


	return result;
}