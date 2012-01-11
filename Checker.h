#ifndef CHECKER_H_INCLUDED
#define CHECKER_H_INCLUDED

// {{{ Boilerplate Code <--------------------------------------------------

//

// vim:filetype=cpp foldmethod=marker foldmarker={{{,}}}



#include <algorithm>
#include <set>
#include <utility>
#include <vector>
#include <string>

using namespace std;
// }}}


class Checker
{
public:
	bool CheckData(vector<string> lovers)
	{
        string validChars("ABCDEFGHIJKLMNOPQRSTUVWXYZ-");


        set<string> lover1s;
        set<string> lover2s;

        if (lovers.size() > 20 || lovers.size() < 2)
            return false;

        for (unsigned int i=0; i<lovers.size(); ++i) {
            string loveTri = lovers[i];

            if (loveTri.length() > 40)
                return false;

			int triLen = loveTri.length();

            // check leading/trailing space
            if (loveTri[0] == ' ' || loveTri[triLen-1] == ' ')
                return false;

            // extract LOVER1
            int lover1EndPos = loveTri.find_first_of(' ');
            string lover1 = loveTri.substr(0, lover1EndPos);

            // extract LOVER2
            int lover2StartPos = loveTri.find_last_of(' ') + 1;
            string lover2 = loveTri.substr(lover2StartPos);

            // extract LOVES
            string LOVES = loveTri.substr(lover1EndPos+1, lover2StartPos-lover1EndPos-2);

            if (lover1.empty() || lover2.empty() || 
                lover1 == lover2 || LOVES != "LOVES")
                return false;

			if (lover1.find_first_not_of(validChars) != string::npos ||
				lover2.find_first_not_of(validChars) != string::npos)
                return false;

            lover1s.insert(lover1);
            lover2s.insert(lover2);
        }

        set<string>::iterator iend = lover2s.end();
        for (set<string>::iterator it = lover2s.begin(); it != iend; ++it) {
            if (lover1s.find(*it) == lover1s.end())
                return false;
        }

        return true;
	}
};

#endif
