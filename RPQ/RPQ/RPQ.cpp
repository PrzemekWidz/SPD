#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>


/* Struktura posiadajaca informacje o konkretnym elemencie zadania */

struct element {

	int index;
	int r;
	int p;
	int q;
};

/* Funkcja, ktora wczytuje dane do wektora ( wektora typu element )  */

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

/* Funkcja wyswietlajaca elementy wektora ( wektora typu element ) */

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

/* Funkcja wyswietlajaca uporzadkowane wedlug algorytmu elementy z tablicy, czyli wynik zadania */

void showArray(int size, element* tablica) {

	for (int i = 0; i < size; i++)
	{
		std::cout << tablica[i].index << " ";
	}
}

/* Sortowanie elementow wektora po R*/

bool compareByR(const element& a, const element& b)
{
	return a.r > b.r;
}

/* Sortowanie elementow wektora po Q*/

bool compareByQ(const element& a, const element& b)
{
	return a.q > b.q;
}

/* Funkcja znajdujaca kolejnosc wybierania elementow zadania */

void algorithm(std::vector <element>& task, element* table, int size)
{
	int tmp1 = 0;
	int tmp2 = 1;

	for (int i = 0; i < size; i++)
	{
		if (i % 2 == 0)
		{
			std::sort(task.begin(), task.end(), compareByR);
			table[tmp1] = task[task.size() - 1];
			task.pop_back();
			tmp1 += 1;
		}

		if (i % 2 != 0)
		{
			std::sort(task.begin(), task.end(), compareByQ);
			table[size - tmp2] = task[task.size() - 1];
			task.pop_back();
			tmp2 += 1;
		}
	}
}

/* Funkcja sprawdzajaca dlugosc wykonywania zadania dla elementow z tablicy */

int check(element* table, int size)
{
	int c = 0;
	int m = 0;

	for (int i = 0; i < size; i++)
	{
		m = std::max(m, table[i].r) + table[i].p;
		c = std::max(m + table[i].q, c);
	}

	return c;
}

/* Funkcja glowna */

int main() {

	int size;
	int sum=0;
	std::string fileNames[4] = {"data.1.txt","data.2.txt","data.3.txt","data.4.txt" };
	std::vector <element> task;
	std::string fileName;
	std::fstream file;
	

	for (int i = 0; i < 4; i++)
	{
		std::cout << std::endl;
		//std::cin >> fileName;
		fileName = fileNames[i];

		file.open(fileName.c_str(), std::fstream::in);
		file >> size;
		file.close();

		loadData(size, fileName, task);
		//showData(size, task);
		std::cout << std::endl;

		element* solvedTask = new element[size];
		algorithm(task, solvedTask, size);

		std::cout << "Rozwiazanie dla " << fileNames[i] << " wyglada nastepujaco : ";
		showArray(size, solvedTask);
		std::cout << std::endl;

		int timeResult = check(solvedTask, size);
		std::cout << "Wynik czasowy dla tego przypadku wynosi : ";
		std::cout << timeResult;
		std::cout << std::endl;

		sum += timeResult;

		delete[] solvedTask;
	}

	std::cout << std::endl;
	std::cout << "Suma czasowa dla wszystkich przykladow wynosi: " << sum ;
	std::cout << std::endl;

	std::getchar();

	return 0;
}
