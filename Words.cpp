// ConsoleApplication1.cpp: îïðåäåëÿåò òî÷êó âõîäà äëÿ êîíñîëüíîãî ïðèëîæåíèÿ.
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
	/*
		Чтобы для русского текста не отображались каракули в гитхабе, нужно файл в кодировке utf-8 сохранять
	*/
	if (!fin.is_open()) // åñëè ôàéë íå îòêðûò
		std::cout << "File isn't open\n";
	else
	{
		while (1)
		{
			if (fin.eof())
				break;

			fin >> buff;
			
			// FIXIT: попробуйте переписать это условие через множество или через ф-и isalpha или ispunct
			if ((buff[buff.length() - 1] == '.') || (buff[buff.length() - 1] == ',') || (buff[buff.length() - 1] == '!') || (buff[buff.length() - 1] == '?') || (buff[buff.length() - 1] == '...') || (buff[buff.length() - 1] == ')') || (buff[buff.length() - 1] == ':') || (buff[buff.length() - 1] == ';'))
			{
				buff.resize(buff.length() - 1);
				/*
					FIXIT: думаю, лучше после if эти две строки написать, чтобы не копипастить
				*/
				std::transform(buff.begin(), buff.end(), buff.begin(), tolower);
				words[buff]++;
			}
			else
				std::transform(buff.begin(), buff.end(), buff.begin(), tolower);
				words[buff]++;
		}

	};

	a.resize(words.size());

	// for (auto it = words.begin() ...  Так короче ведь
	for (std::map<std::string, int>::iterator it = words.begin(); it != words.end(); ++it)
	{
		a[i].s = it->first;
		a[i].x = it->second;
		i++;
	}
	
	sort(a.begin(), a.end(), comp);
	// 50 нужно вынести в отдельную константу
	for (size_t i = 0; i < 50; ++i)
		std::cout << i + 1 << ". " << a[i].s << " - was used " << a[i].x << " times.\n";
	
	fin.close();
	system("pause");
    return 0;
}

