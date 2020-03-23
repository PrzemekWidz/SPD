#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <iterator>
#include <cstdio>
#include <cstdlib>
#include <cmath>


struct element {

	int id;
	int taskTime;
	int punishmentRatio;
	int wantedTime;

};

void loadData(int size, std::string fileName, std::vector <element>& task)
{
	std::fstream file;
	file.open(fileName.c_str(), std::fstream::in);

	int tmpInt;
	element tmpElement;

	file >> tmpInt;

	for (int i = 0; i < size; i++)
	{
		tmpElement.id = i + 1;

		file >> tmpInt;
		tmpElement.taskTime = tmpInt;

		file >> tmpInt;
		tmpElement.punishmentRatio = tmpInt;

		file >> tmpInt;
		tmpElement.wantedTime = tmpInt;

		task.push_back(tmpElement);
	}

	file.close();
}

void showData(int size, std::vector <element> task) 
{

	for (int i = 0; i < size; i++)
	{
		std::cout << task[i].id << ") ";
		std::cout << task[i].taskTime << " ";
		std::cout << task[i].punishmentRatio << " ";
		std::cout << task[i].wantedTime << " ";
		std::cout << std::endl;

	}
}

void showPermutation(int size, std::vector <element> answer) {

	for (int i = 0; i < size; i++)
	{
		std::cout << answer[i].id << " ";
	}

	std::cout << std::endl;
}

int endTime(int size, std::vector <element>& task)
{
	int tmp = 0;

	for (int i = 0; i < size; i++)
	{
		tmp += task[i].taskTime;
	}

	return tmp;
}

int witi(int size, std::vector <element>& answer)
{
	int opt=0;
	int actualTime=0;

	std::reverse(answer.begin(), answer.end());

	for (int i = 0; i < size; i++)
	{
		actualTime = actualTime + answer[i].taskTime;

		if (actualTime - answer[i].wantedTime > 0)
		{
			opt = opt + answer[i].punishmentRatio * (actualTime - answer[i].wantedTime);
		}
	}

	return opt;
}

void algorithm(int size, std::vector <element>& task, std::vector <element>& answer)
{
	int K;
	element min;

	for (int i = 0; i < size; i++)
	{
		int bigNumber = 999999;

		int c = endTime(task.size(), task);

		for (int j = 0; j < task.size(); j++)
		{
			K = task[j].punishmentRatio * (c - task[j].wantedTime);
			if (K < bigNumber)
			{
				bigNumber = K;
				min = task[j];
			}
		}

		answer.push_back(min);

		for (int l = 0; l < task.size(); l++)
		{
			int tmp = 0;

			if ((task[l].id == min.id) && (tmp == 0))
			{
				std::swap(task[l], task[task.size() - 1]);
				tmp = 1;
			}
		}

		task.pop_back();
	}
}


int main()
{
	int size;
	std::string fileName;
	std::fstream file;
	
	
	std::string fileNames[11] = { "data.10.txt","data.11.txt","data.12.txt","data.13.txt",
								   "data.14.txt", "data.15.txt", "data.16.txt", "data.17.txt",
								   "data.18.txt", "data.19.txt", "data.20.txt" };

	//std::cin >> fileName;

	for (int i = 0; i < 11; i++)
	{
		std::vector <element> task;
		std::vector <element> answer;

		fileName = fileNames[i];

		file.open(fileName.c_str(), std::fstream::in);
		file >> size;
		file.close();

		loadData(size, fileName, task);
		
		std::cout << fileNames[i] << std::endl;
		std::cout << size << std::endl;
		showData(size, task);

		algorithm(size, task, answer);

		std::cout << std::endl;

		int sum = witi(size, answer);

		std::cout << "Opt = " << sum;
		std::cout << std::endl;
		
		showPermutation(answer.size(), answer);
		std::cout << std::endl;

	}
	std::getchar();
}

