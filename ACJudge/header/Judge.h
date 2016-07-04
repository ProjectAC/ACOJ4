#include "../platform.h"
#include "Type.h"
#include "Database.h"
#include <thread>

namespace ACOJ
{
	class Judge
	{
	protected:
		tstring name;
		Database &db;
		Task &task;

		virtual int judge(const Data &d) {}
		virtual int score() {}

		int compile(tstring s, Language l);
		int run(const Data &d);

		void prepare_submission(const Submission &s);
		void prepare_data(const Data &d);
		void prepare_spj(const Task &t);
		
	public:

		void judge(const Submission &s);
		void write(tstring filename, tstring text);

		static int process(tstring cmd, LL time, LL space, tstring input = L"input", tstring output = L"output", tstring error = L"errlog");
		static tstring get_suffix(Language l);
	};
}