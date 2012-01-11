#ifndef LEXER_H_INCLUDED
#define LEXER_H_INCLUDED

#include <algorithm>
#include <functional>
#include <utility>
#include <vector>
#include <string>

using namespace std;

struct Trie
{
  	typedef vector<Trie*>::iterator Iter;

	char c;   // this will be a-zA-Z
  	bool t;   // is token
  	vector<Trie*> next;

  	bool operator<(const Trie& other) const
  	{ return c < other.c; }

  	Trie(char ch) : c(ch), t(false) {}

  	~Trie() {
    	for (unsigned int i = 0; i < next.size(); ++i) {
      		delete next[i];
    	}
  	}

  	void addToken(const string& token);
};

struct DereferenceLess
{
	template <class IterPtrType, class PtrType>
	bool operator() (IterPtrType lhs, PtrType rhs) {
		return *lhs < *rhs;
	}
};

void Trie::addToken(const string& token) {
   	if (token.empty()) {
   		this->t = true;
   		return;
   	}

	Trie* current = new Trie(token[0]);
   	pair<Iter, Iter> p = equal_range(next.begin(), next.end(), current, DereferenceLess());

   	if (p.first == p.second) {
   		next.insert(p.first, current);   
		current->addToken(token.substr(1));
	}
   	else {
   		delete current; 
		(*p.first)->addToken(token.substr(1));
	}
}

class Lexer
{
public:
  	Trie* createTrie(const vector<string>& tokens)
  	{
    	// it doesn't matter what root's c is
    	Trie* root = new Trie(' ');

    	for (unsigned int i = 0; i < tokens.size(); ++i) 
      	root->addToken(tokens[i]); 
    
    	return root;
  	}

	vector<string> tokenize(vector<string> tokens, string input)
	{
	    vector<string> ret; 
    	Trie* root = createTrie(tokens);
    	Trie* path = root;
    	int path_len = 0;
    	string longestToken = "";
    	string::iterator curInputIter = input.begin();

    	while (!input.empty() && curInputIter != input.end()) {
      		Trie* current = new Trie(*curInputIter);
      		pair<Trie::Iter, Trie::Iter> p = 
        		equal_range(path->next.begin(), path->next.end(), current, DereferenceLess());

      		if (p.first == p.second) {
	    		if (longestToken.empty()) {
	          		input = input.substr(1);
	        	}
	        	else {
		        	// consume input with the latest token
	          		ret.push_back(longestToken);
          			input = input.substr(longestToken.size());
		        	longestToken = "";
        		}

        		curInputIter = input.begin();
        		path = root;
        		path_len = 0;
      		}
      		else {  // we find it
        		if ((*p.first)->t) {   // we find a token
          			// update the longest token
          			longestToken = input.substr(0, path_len+1);
        		}

        		++curInputIter;
        		path = *p.first;   // update path to proceed
        		++path_len;       // update path_len together with path
      		}
      		delete current;
    	}

		if (!longestToken.empty())
			ret.push_back(longestToken);

    	delete root;
		return ret;
	}
};

#endif		// LEXER_H_INCLUDED