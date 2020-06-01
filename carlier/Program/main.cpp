#include <fstream>
#include <string>
#include <algorithm>
#include <numeric>
#include <vector>

#include "T_Job.h"
#include "T_Data.h"


// CMAX
int getCMAX(std::vector<T_Job>& t_jobs);	// CMAX by jobs in vector
int getCMAX_index(std::vector<T_Job>& t_jobs, int t_index);	// CMAX to number index


// Free jobs
std::vector<int> getFreeJobsIndex(std::vector<T_Job>& t_jobs, int t_time);

// Shrage algorithm 
T_Data ShrageAlgorithm(std::vector<T_Job> jobs);
int shrageWithInterrupt(std::vector<T_Job> jobs);

//---------------------------------------------------------------------------------------

int getJA(const std::vector<T_Job>& jobs, const int jobIndex)
{

	int leftIndex;
	int resA;

	for (int i = jobIndex; i >= 0; i--)
	{
		if (jobs[i].leftStick == false)
		{
			leftIndex = i;
			break;
		}
	}

	for (int j = leftIndex; j <= jobIndex; j++)
	{
		if ((jobs[j].timeWhenJobBegin - jobs[j].R) == 0)
		{
			resA = j;
			break;
		}
	}


	return resA;
}

int getJB(std::vector<T_Job>& jobs)
{
	int startCmax = getCMAX(jobs);
	int resIdx;
	int jobsSize;


	jobsSize = jobs.size();
	for (int i = jobsSize - 1; i >= 0; i--)
	{
		if (getCMAX_index(jobs, i) != startCmax)
		{
			resIdx = i + 1;
			break;
		}
	}
	return resIdx;
}

int getJC(const std::vector<T_Job>& jobs, const int Aindex, const int Bindex)
{
	int resC;

	for (int i = (Bindex - 1); i >= Aindex; i--)
	{
		if (jobs[i].Q < jobs[Bindex].Q)
		{
			resC= i;
			break;
		}
	}
	return resC; 
}

//---------------------------------------------------------------------------------------

// Calier algorithm
T_Data CarlierAlgorithm(T_Data initDto);

///////////////////////////////////////////////////////////////////////////////////////

int main() {

	std::vector<T_Job> jobs;

	T_Job thisJob;
	T_Data fData;

	int dataNumber;
	int currentBestCmax = std::numeric_limits<int>::max();
	std::vector<T_Job> currentBestOrder;

	std::ifstream myfile("carl.data.txt");

	std::string line;


	if (!myfile.is_open()) throw std::runtime_error("Blad odczytu");

	std::string currentDataSet;

	for (int i = 0; i <= 8; i++) {

		currentDataSet = "data.00" + std::to_string(i) + ":";

		while (getline(myfile, line) && (line != currentDataSet));
		myfile >> dataNumber;

		
		for (int i = 0; i < dataNumber; i++) {
			myfile >> thisJob.R >> thisJob.P >> thisJob.Q;
			thisJob.initialIndex = i;
			thisJob.P_Shrage = thisJob.P;

			jobs.push_back(thisJob);
		}
		std::sort(jobs.begin(), jobs.end(), [](const T_Job& first, const T_Job& second) { return first.R < second.R; });
		fData.order = jobs;
		fData = CarlierAlgorithm(fData);


		// Show final solution
		std::cout << "CMAX: " << fData.C_MAX << "\n\n";
		std::cout << "Kolejnosc: ";
		for (auto& obj : fData.order) {
			obj.showInitIndex();
		}
		std::cout << " " << std::endl;
	}


	return 0;
}


///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
// CMAX by vector jobs
int getCMAX(std::vector<T_Job>& t_jobs)
{
	int x{ 0 };
	int res{ 0 };

	for (int i = 0; i < t_jobs.size(); i++) {
		x = std::max(t_jobs[i].R, x) + t_jobs[i].P;
		res = std::max(res, x + t_jobs[i].Q);
	}

	return res;
}

//-------------------------------------------------------------------------------------

int getCMAX_index(std::vector<T_Job>& t_jobs, int t_index)
{
	int x{ 0 };
	int res{ 0 };

	for (int i = 0; i <= t_index; i++) {
		x = std::max(t_jobs[i].R, x) + t_jobs[i].P;
		res = std::max(res, x + t_jobs[i].Q);
	}

	return res;
}

//-------------------------------------------------------------------------------------

std::vector<int> getFreeJobsIndex(std::vector<T_Job>& t_jobs, int t_time) 
{
	std::vector<int> res;

	for (int i = 0; i < t_jobs.size(); i++) {
		if (t_jobs[i].R <= t_time) {
			res.push_back(i);
		}
	}

	return res;
}

///////////////////////////////////////////////////////////////////////////////////////
// Shrage algorithms

// Shrage
T_Data ShrageAlgorithm(std::vector<T_Job> jobs) 
{
	int maxQIdx;
	int maxQ;

	int Time = jobs[0].R;
	int newTime{ 0 };
	int nextTime;
	int Cmax = 0;
	int JobsSize;

	int HVar = 0;

	T_Data data;

	std::vector<T_Job> res;
	std::vector<int> freeIndexJobs;

	std::sort(jobs.begin(), jobs.end(), [](const T_Job& first, const T_Job& second) { return first.R < second.R; });

	while (jobs.size() != 0) {
		freeIndexJobs = getFreeJobsIndex(jobs, Time);

		if (freeIndexJobs.size() != 0)
		{
			maxQIdx = freeIndexJobs[0];
			maxQ = jobs[maxQIdx].Q;

			for (int index : freeIndexJobs) {
				if (maxQ < jobs[index].Q) {
					maxQ = jobs[index].Q;
					maxQIdx = index;

				}
				else if (maxQ == jobs[index].Q) {

					if (jobs[maxQIdx].initialIndex > jobs[index].initialIndex) {
						maxQ = jobs[index].Q;
						maxQIdx = index;
					}
				}
			}


			nextTime = std::numeric_limits<int>::max();
			for (int i = 0; i < jobs.size(); i++) {
				if (jobs[i].R > Time) {
					nextTime = jobs[i].R;
					break;
				}
			}


			HVar++;
			if (HVar > 1)
			{
				jobs[maxQIdx].leftStick = true;
			}

			jobs[maxQIdx].timeWhenJobBegin = Time;

			Time += jobs[maxQIdx].P;



			Cmax = std::max(Time + jobs[maxQ].Q, Cmax);

			res.push_back(jobs[maxQ]);

			jobs.erase(jobs.begin() + maxQ);

		}
		else {
			Time = nextTime;
			HVar = 0;
		}
	}


	data.C_MAX = Cmax;
	data.order = res;

	return data;
}

// Shrage interrupts
int shrageWithInterrupt(std::vector<T_Job> jobs) {

	int Time = jobs[0].R;
	int nextTime;
	int newTime;
	int Cmax = 0;
	int maxQIdx;
	int maxQ;

	std::vector<int> freeJobsIdx;
	std::sort(jobs.begin(), jobs.end(), [](const T_Job& first, const T_Job& second) { return first.R < second.R; });


	while (jobs.size() != 0) {
		freeJobsIdx = getFreeJobsIndex(jobs, Time);

		if (freeJobsIdx.size() != 0) {
			maxQIdx = freeJobsIdx[0];
			maxQ = jobs[maxQIdx].Q;

			for (int index : freeJobsIdx) {
				if (maxQ < jobs[index].Q) {
					maxQ = jobs[index].Q;
					maxQIdx = index;

				}
				else if (maxQ == jobs[index].Q) {

					if (jobs[maxQIdx].initialIndex > jobs[index].initialIndex) {
						maxQ = jobs[index].Q;
						maxQIdx = index;
					}
				}
			}


			newTime = std::numeric_limits<int>::max();
			for (int i = 0; i < jobs.size(); i++) {
				if (jobs[i].R > Time) {
					newTime = jobs[i].R;
					break;
				}
			}


			nextTime = std::min(Time + jobs[maxQIdx].P_Shrage, newTime);
			jobs[maxQIdx].P_Shrage = jobs[maxQIdx].P_Shrage - (nextTime - Time);
			Time = nextTime;


			if (jobs[maxQIdx].P_Shrage == 0) {
				Cmax = std::max(Time + jobs[maxQIdx].Q, Cmax);


				jobs.erase(jobs.begin() + maxQIdx);
			}
		}
		else {
			Time = newTime;
		}
	}

	return Cmax;
}


///////////////////////////////////////////////////////////////////////////////////////
// Carlier algorithm

T_Data CarlierAlgorithm(T_Data startData)
{

	T_Data data = ShrageAlgorithm(startData.order);


	int BidxData = getJB(data.order);
	int AidxData = getJA(data.order, BidxData);
	int CidxData = getJC(data.order, AidxData, BidxData);

	if (CidxData == -1)
	{
		return data;
	}

	std::vector<T_Job> K;
	for (int index = (CidxData + 1); index <= BidxData; index++)
	{
		K.push_back(data.order[index]);
	}


	int minRinK = (*std::min_element(K.begin(), K.end(),
		[](const T_Job& lhs, const T_Job& rhs)
		{
			return lhs.R < rhs.R;
		})).R;

	int minQinK = (*std::min_element(K.begin(), K.end(),
		[](const T_Job& lhs, const T_Job& rhs)
		{
			return lhs.Q < rhs.Q;
		})).Q;

	int fullK = std::accumulate(K.begin(), K.end(), 0,
		[](int i, const T_Job& job)
		{
			return job.P + i;
		});




	int CopyVal = data.order[CidxData].R;

	data.order[CidxData].R = std::max(CopyVal, (minRinK + fullK));



	int CmaxShrageInterrupt = shrageWithInterrupt(data.order);



	std::vector<T_Job> Kcopy = K;

	Kcopy.insert(Kcopy.begin(), data.order[CidxData]);

	int H_OD_K = minRinK + fullK + minQinK;


	CmaxShrageInterrupt = std::max(H_OD_K, CmaxShrageInterrupt);


	if (CmaxShrageInterrupt < data.C_MAX)
	{
		data = CarlierAlgorithm(data);
	}

	data.order[CidxData].R = CopyVal;

	int cpyOrderValue = data.order[CidxData].Q;

	data.order[CidxData].Q = std::max(cpyOrderValue, (minQinK + fullK));

	CmaxShrageInterrupt = shrageWithInterrupt(data.order);


	H_OD_K = minRinK + fullK + minQinK;

	CmaxShrageInterrupt = std::max(H_OD_K, CmaxShrageInterrupt);

	if (CmaxShrageInterrupt < data.C_MAX)
	{
		data = CarlierAlgorithm(data); // rec
	}
	data.order[CidxData].Q = cpyOrderValue;


	
	return data;
}

