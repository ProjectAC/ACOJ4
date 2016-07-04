#pragma once
#include "Type.h"

class Database
{


public:
	auto sql(tstring cmd);

	Task get_task(const unsigned long long &tid);
	vector<Data> get_data(const Task &s);
	Submission get_submission(const unsigned long long &sid);

	void set_submission(Submission t);
};