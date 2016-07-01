#pragma once
#include "Type.h"

class Database
{


public:
	auto sql(wstring cmd);

	vector<Data> get_data(const Task &s);
	Submission get_submission(const unsigned long long &sid);

	void set_submission(Submission t);
};