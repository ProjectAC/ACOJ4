#pragma once
#include "Type.h"
#include "../platform.h"
#include <vector>

namespace ACOJ
{
	class Database
	{


	public:
		auto sql(tstring cmd);

		Task get_task(const LL &tid);
		std::vector<Data> get_data(const Task &s);
		Submission get_submission(const LL &sid);
		Submission get_submission();

		void set_submission(Submission t);

		Database(tstring name, tstring username, tstring password);
	};
}