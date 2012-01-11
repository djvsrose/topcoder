#ifndef LETTER_RANGE_H_INCLUDED
#define LETTER_RANGE_H_INCLUDED 

#include <algorithm>
#include <string>
#include <vector>
#include <utility>

using namespace std;

bool IsContinuous(const pair<int,bool>& lhs, const pair<int,bool>& rhs)
{
	return lhs.second == rhs.second;
}

class LetterRange
{
	string makeRange(char from, char to)
	{
		string rangeStr = "";
		rangeStr += from;
		rangeStr += ":";
		rangeStr += to;
		return rangeStr;
	}

public:
	vector<string> ranges(string input)
	{
		vector<string> result;
		pair<int, bool> rec[26];

		for (int i = 0; i < 26; ++i)
			rec[i] = make_pair(i, false);

		for (unsigned int i = 0; i < input.length(); ++i) {
			if (input[i] != ' ') {
				rec[input[i] - 'a'].second = true;
			}
		}

		pair<int, bool>* last = rec + 26;
		char startLetter = ' ';
		char closeLetter = ' ';
		for (pair<int, bool>* first = rec; first != last; ++first) {
			if ((*first).second == false && startLetter != ' ') {
				result.push_back(makeRange(startLetter, closeLetter));
				startLetter = closeLetter = ' ';
			}
			else if ((*first).second == true && startLetter != ' ') {
				closeLetter = (*first).first + 'a';
			}
			else if ((*first).second == true && startLetter == ' ') {
				startLetter =  closeLetter = (*first).first + 'a';
			}
		}

		if (startLetter != ' ')
			result.push_back(makeRange(startLetter, closeLetter));

		return result;
	}
};

#endif