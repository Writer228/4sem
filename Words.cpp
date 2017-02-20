// ConsoleApplication1.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>
#include <string>

struct list
{
	std::string s;
	int x;
};

bool comp(list const&  a, list const& b)
{
	return a.x > b.x;
};

int main()
{
	setlocale(LC_ALL, "rus");
	std::map < std::string, int> words;
	std::string  buff;
	std::vector<list> a;
	int i = 0;

	std::ifstream fin("infa.txt");
	if (!fin.is_open()) // если файл не открыт
		std::cout << "File isn't open\n";
	else
	{
		while (1)
		{
			if (fin.eof())
				break;

			fin >> buff;
			
			if ((buff[buff.length() - 1] == '.') || (buff[buff.length() - 1] == ',') || (buff[buff.length() - 1] == '!') || (buff[buff.length() - 1] == '?') || (buff[buff.length() - 1] == '...') || (buff[buff.length() - 1] == ')') || (buff[buff.length() - 1] == ':') || (buff[buff.length() - 1] == ';'))
			{
				buff.resize(buff.length() - 1);
				std::transform(buff.begin(), buff.end(), buff.begin(), tolower);
				words[buff]++;
			}
			else
				std::transform(buff.begin(), buff.end(), buff.begin(), tolower);
				words[buff]++;
		}

	};

	a.resize(words.size());

	for (std::map<std::string, int>::iterator it = words.begin(); it != words.end(); ++it)
	{
		a[i].s = it->first;
		a[i].x = it->second;
		i++;
	}
	
	sort(a.begin(), a.end(), comp);
	for (size_t i = 0; i < 50; ++i)
		std::cout << i + 1 << ". " << a[i].s << " - was used " << a[i].x << " times.\n";
	
	fin.close();
	system("pause");
    return 0;
}

