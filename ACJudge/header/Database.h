#pragma once
#include "Type.h"

class Database
{


public:
	auto sql(tstring cmd);

	Task get_task(const unsigned long long &tid);
	vector<Data> get_data(const Task &s);
	Submission get_submission(const unsigned long long &sid);
	Submission get_submission();

	void set_submission(Submission t);

	Database(tstring name, tstring username, tstring password);
};