// ConsoleApplication1.cpp: îïðåäåëÿåò òî÷êó âõîäà äëÿ êîíñîëüíîãî ïðèëîæåíèÿ.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include <set>
#include <cctype>

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
	const int WORDS_COUNT = 50;
	setlocale(LC_ALL, "rus");
	std::map < std::string, int> words;
	std::string  buff;
	std::vector<list> a;
	const std::set<char> punctuationMarks = { '.', ',', ':', '!', '?',')',']'};
	int i = 0;

	std::ifstream fin("War and Word.txt");
	if (!fin.is_open()) 
		std::cout << "File isn't open\n";
	else
	{
		while (1)
		{
			if (fin.eof())
				break;

			fin >> buff;
				
			if(punctuationMarks.find(buff[buff.length()- 1] ) != punctuationMarks.end())
				buff.resize(buff.length() - 1);

				std::transform(buff.begin(), buff.end(), buff.begin(), tolower);
				words[buff]++;
		}

	};

	a.resize(words.size());

	for (auto it = words.begin(); it != words.end(); ++it)
	{
		a[i].s = it->first;
		a[i].x = it->second;
		i++;
	}
	
	sort(a.begin(), a.end(), comp);
	for (size_t i = 0; i < WORDS_COUNT; ++i)
		std::cout << i + 1 << ". " << a[i].s << " - was used " << a[i].x << " times.\n";
	
	fin.close();
	system("pause");
    return 0;
}

