#ifndef BINARY_CODE_H_INCLUDED
#define BINARY_CODE_H_INCLUDED

#include <vector>
#include <string>

using namespace std;

class BinaryCode
{
public:
	vector<string> decode(string message)
	{
		string encoded = "0" + message + "0";

		string decoded0('0', message.length());
		for (int i = 1; i < message.length(); ++i) {
		}

		string decoded1('1', message.length());
		for (int i = 1; i < message.length(); ++i) {
		}



	}
};

#endif