// Used libraries 
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>

// A representation of our task using structure 

struct task {

	int index;

	int deliveryTime;
	int doingTime;
	int coolingTime;

};

// Sort vector type task by Q (coolingTime)

bool compareByQ(const task& s1, const task& s2)
{
	return s1.coolingTime < s2.coolingTime;
}

// Function that loads data

void loadData(int size, std::string fileName, std::vector <task>& tasks)
{
	std::fstream file;
	file.open(fileName.c_str(), std::fstream::in);

	int tmpInt;
	task tmpElement;

	file >> tmpInt;

	for (int i = 0; i < size; i++)
	{
		tmpElement.index = i + 1;

		file >> tmpInt;
		tmpElement.deliveryTime = tmpInt;

		file >> tmpInt;
		tmpElement.doingTime = tmpInt;

		file >> tmpInt;
		tmpElement.coolingTime = tmpInt;

		tasks.push_back(tmpElement);
	}

	file.close();
}

// Function that shows data (1)

void showData(int size, std::vector <task> tasks) {

	for (int i = 0; i < size; i++)
	{
		std::cout << tasks[i].index << ") ";
		std::cout << tasks[i].deliveryTime << " ";
		std::cout << tasks[i].doingTime << " ";
		std::cout << tasks[i].coolingTime << " ";
		std::cout << std::endl;

	}
}

// Function that shows data (2)

void showData2(int size, std::vector <task> tasks)
{
	for (int i = 0; i < size; i++)
	{

		std::cout << tasks[i].index << " ";
	}

	std::cout << std::endl;
}

// Function that checks cMax 

int check(std::vector <task>& tasks, int size)
{
	int c = 0;
	int m = 0;

	for (int i = 0; i < size; i++)
	{
		m = std::max(m, tasks[i].deliveryTime) + tasks[i].doingTime;
		c = std::max(m + tasks[i].coolingTime, c);
	}

	return c;
}

// Schrage algorithm function 

void schrage(int size, std::vector <task>& tasks, std::string currentFileName)
{
	std::vector <task> readyTasks;
	std::vector <task> results;

	int cMax = 0;
	int t = 0;
	int i = 0;

	while (results.size() != size)
	{
		std::vector <int> deleteVector;

		for (int i = 0; i < tasks.size(); i++)
		{
			if (tasks[i].deliveryTime <= t)
			{
				readyTasks.push_back(tasks[i]);
				deleteVector.push_back(tasks[i].index);
			}

		}

		while (deleteVector.size() >= 1)
		{
			int tmp = deleteVector[deleteVector.size() - 1];

			tasks[tmp - 1].deliveryTime = 10000;

			deleteVector.pop_back();
		}

		if (readyTasks.size() >= 1)
		{
			std::sort(readyTasks.begin(), readyTasks.end(), compareByQ);
			task Tmp = readyTasks[readyTasks.size() - 1];
			results.push_back(Tmp);
			readyTasks.pop_back();

			t = t + Tmp.doingTime;
		}

		else
		{
			t = t + 1;
		}
	}

	cMax = check(results, results.size());

	std::cout << std::endl;
	std::cout << currentFileName << std::endl;
	std::cout << std::endl;

	std::cout << "schr:" << std::endl;
	std::cout << cMax << std::endl;
	showData2(results.size(), results);
	std::cout << std::endl;


}

// Function that loads all examples from indicated file

void doTasks(std::string currentFileName) {

	int taskAmount;
	std::vector<task> tasks;
	std::fstream file;

	file.open(currentFileName.c_str(), std::fstream::in);
	file >> taskAmount;
	file.close();

	loadData(taskAmount, currentFileName, tasks);

	schrage(taskAmount, tasks, currentFileName);

}

// Main 

int main()
{

	std::string fileNames[9] = { "data.000.txt","data.001.txt","data.002.txt","data.003.txt" ,
								 "data.004.txt","data.005.txt","data.006.txt",
								 "data.007.txt","data.008.txt" };

	for (int i = 0; i < 9; i++)
	{
		std::string currentFileName = fileNames[i];
		doTasks(currentFileName);
	}

	std::getchar();
}
