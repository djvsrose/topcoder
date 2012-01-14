#ifndef RULE_ENGINE_H_INCLUDED
#define RULE_ENGINE_H_INCLUDED
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <string>
#include <vector>
#include <utility>
#include <cassert>
#include <sstream>
#include <regex>
#include <algorithm>
using namespace std;

struct Data
{
	Data(char c) : name(c)
	{ for (int i=0; i<21; ++i)	value[i] = true; }

	int GetCount() const 
	{
		int count = 0;
		for (int i = 0; i < 21; ++i) 
			count += (value[i] ? 1 : 0);
		return count;
	}

	char name;
	bool value[21];
};

typedef vector<Data> DataSet;

class Rule 
{
protected:
	char c_;
	int n_;		// this is actually the index in the array, so 0 will be in data.value[10]

	Data& FindData(DataSet& dataSet, char c)
	{
		for (unsigned int i = 0; i < dataSet.size(); ++i) {
			if (dataSet[i].name == c)
				return dataSet[i];
		}
		assert(false);	// this can't happen
	}

public:
	Rule(char c, int x) : c_(c), n_(x+10) {}
	virtual ~Rule() {}
	virtual DataSet& Apply(DataSet& dataSet) = 0;
};

int Str2Int(const string& s)
{
	int ret;
	istringstream iss(s);
	iss >> ret;
	return ret;
}

class EGTRule : public Rule	// >=
{
public:
	EGTRule(char c, int x) : Rule(c, x) {}

	virtual DataSet& Apply(DataSet& dataSet)
	{
		Data& data = FindData(dataSet, c_);
		for (int i = 0; i < n_; ++i) 
			data.value[i] = false;
		return dataSet;	
	}
};

class ELTRule : public Rule	// <=
{
public:
	ELTRule(char c, int x) : Rule(c, x) {}

	virtual DataSet& Apply(DataSet& dataSet)
	{
		Data& data = FindData(dataSet, c_);
		for (int i = n_ + 1; i < 21; ++i) 
			data.value[i] = false;
		return dataSet;	
	}
};

class EQRule : public Rule	// ==
{
public:
	EQRule(char c, int x) : Rule(c, x) {}

	virtual DataSet& Apply(DataSet& dataSet)
	{
		Data& data = FindData(dataSet, c_);
		for (int i = 0; i < 21; ++i) {
			if (i != n_)	data.value[i] = false;
		}
		return dataSet;
	}
};

class NEQRule : public Rule	// ==
{
public:
	NEQRule(char c, int x) : Rule(c, x) {}

	virtual DataSet& Apply(DataSet& dataSet)
	{
		Data& data = FindData(dataSet, c_);
		for (int i = 0; i < 21; ++i) {
			if (i == n_)	data.value[i] = false;
		}
		return dataSet;
	}
};

class GTRule : public Rule	// >
{
public:
	GTRule(char c, int x) : Rule(c, x) {}

	virtual DataSet& Apply(DataSet& dataSet)
	{
		Data& data = FindData(dataSet, c_);
		for (int i = 0; i <= n_; ++i) 
			data.value[i] = false;
		return dataSet;	
	}
};

class LTRule : public Rule	// <
{
public:
	LTRule(char c, int x) : Rule(c, x) {}

	virtual DataSet& Apply(DataSet& dataSet)
	{
		Data& data = FindData(dataSet, c_);
		for (int i = n_; i < 21; ++i) 
			data.value[i] = false;
		return dataSet;	
	}
};

class BetweenRule : public Rule		// B
{
	int ubidx_, lbidx_;
public:
	BetweenRule(char c, int lb, int ub) : Rule(c, 0), ubidx_(ub+10), lbidx_(lb+10) {}

	virtual DataSet& Apply(DataSet& dataSet)
	{
		Data& data = FindData(dataSet, c_);
		for (int i = 0; i < lbidx_; ++i)		data.value[i] = false;
		for (int i = ubidx_ + 1; i < 21; ++i)	data.value[i] = false;	
		return dataSet;
	}
};

class RuleEngine
{
	vector<Rule*> rules_;
	DataSet dataSet_;

private:
	void UpdateDataSet(char c)
	{
		bool already_exist = false;
		for (int i=0; i<dataSet_.size(); ++i) {
			if (dataSet_[i].name == c)
				already_exist = true;
		}
		if (!already_exist)
			dataSet_.push_back(Data(c));
	}

	void CreateRules(const vector<string>& rules)
	{
		regex rx1("(\\w)([><=!][=]?)(-?0?\\d)");
		regex rx2("(\\w)B(-?0?\\d),(-?0?\\d)");

		for_each (rules.begin(), rules.end(), [this, &rx1, &rx2] (const string& rule) {
			smatch mr1, mr2;
			if (regex_match(rule.begin(), rule.end(), mr1, rx1)) {
				string op = mr1[2];
				char var = string(mr1[1])[0];
				int value = Str2Int(mr1[3]);
			
				UpdateDataSet(var);

				// create new Rule
				if (op == ">=")			rules_.push_back(new EGTRule(var, value));
				else if (op == "<=")	rules_.push_back(new ELTRule(var, value));
				else if (op == ">")		rules_.push_back(new GTRule(var, value));
				else if (op == "<")		rules_.push_back(new LTRule(var, value));
				else if (op == "==")	rules_.push_back(new EQRule(var, value));
				else if (op == "!=")	rules_.push_back(new NEQRule(var, value));
				else					assert(false);
			}
			else if (regex_match(rule.begin(), rule.end(), mr2, rx2)) {
				char var = string(mr2[1])[0];
				int lb = Str2Int(mr2[2]);
				int ub = Str2Int(mr2[3]);

				UpdateDataSet(var);
				rules_.push_back(new BetweenRule(var, lb, ub));
			}
		});
	}

public:
	~RuleEngine() 
	{
		for_each(rules_.begin(), rules_.end(), [] (Rule* r) {
			delete r;
		});
	}

	string countSets(vector<string> rule1, vector<string> rule2)
	{
		CreateRules(rule1);	// this will ready rules_, dataSet_
		CreateRules(rule2);

		for (unsigned int i = 0; i < rules_.size(); ++i) {
			dataSet_ = rules_[i]->Apply(dataSet_);
		}

		// count the combination
		unsigned long ret = 1;
		for (unsigned int i = 0; i < dataSet_.size(); ++i) {
			unsigned int count = dataSet_[i].GetCount();
			ret *= count;
		}

		char buf[30];
		sprintf(buf, "%l\0", ret);
		return buf;
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////
#endif