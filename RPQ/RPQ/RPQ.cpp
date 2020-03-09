#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>

struct element {

	int index;
	int r;
	int p;
	int q;
};

void loadData(int size, std::string fileName, std::vector <element>& task) {

	std::fstream file;
	file.open(fileName.c_str(), std::fstream::in);

	int tmpInt;
	element tmpElement;

	file >> tmpInt;

	for (int i = 0; i < size; i++)
	{
		tmpElement.index = i + 1;

		file >> tmpInt;
		tmpElement.r = tmpInt;

		file >> tmpInt;
		tmpElement.p = tmpInt;

		file >> tmpInt;
		tmpElement.q = tmpInt;

		task.push_back(tmpElement);
	}

	file.close();
}

void showData(int size, std::vector <element> task) {

	for (int i = 0; i < size; i++)
	{
		std::cout << task[i].index << ") ";
		std::cout << task[i].r << " ";
		std::cout << task[i].p << " ";
		std::cout << task[i].q << " ";
		std::cout << std::endl;

	}
}

void showArray(int size, element* tablica) {

	for (int i = 0; i < size; i++)
	{
		std::cout << tablica[i].index << " "; /*") ";*/
		//std::cout << tablica[i].r << " ";
		//std::cout << tablica[i].p << " ";
		//std::cout << tablica[i].q << " ";
		//std::cout << std::endl;

	}
}

int check(std::vector <element> task, int size)
{
	int c = 0;
	int m = 0;

	for (int i = 0; i < size; i++)
	{
		m = std::max(m, task[i].r) + task[i].p;
		c = std::max(m + task[i].q, c);
	}

	return c;
}

bool compareByR(const element& a, const element& b)
{
	return a.r > b.r;
}

bool compareByQ(const element& a, const element& b)
{
	return a.q > b.q;
}

void algorithm(std::vector <element>& task, element* tablica, int size)
{
	int tmp1 = 0;
	int tmp2 = 1;

	for (int i = 0; i < size; i++)
	{
		if (i % 2 == 0)
		{
			std::sort(task.begin(), task.end(), compareByR);
			tablica[tmp1] = task[task.size() - 1];
			task.pop_back();
			tmp1 += 1;
		}

		if (i % 2 != 0)
		{
			std::sort(task.begin(), task.end(), compareByQ);
			tablica[size - tmp2] = task[task.size() - 1];
			task.pop_back();
			tmp2 += 1;
		}
	}
}

int check(element* tablica, int size)
{
	int c = 0;
	int m = 0;

	for (int i = 0; i < size; i++)
	{
		m = std::max(m, tablica[i].r) + tablica[i].p;
		c = std::max(m + tablica[i].q, c);
	}

	return c;
}

int main() {

	int size;
	std::vector <element> task;
	std::string fileName;
	std::fstream file;


	std::cin >> fileName;

	file.open(fileName.c_str(), std::fstream::in);
	file >> size;
	file.close();

	loadData(size, fileName, task);
	showData(size, task);

	element* tablica = new element[size];

	algorithm(task, tablica, size);

	std::cout << std::endl;

	showArray(size, tablica);

	int timeResult = check(tablica, size);
	std::cout << std::endl;
	std::cout << timeResult;

	delete[] tablica;

	return 0;
}
