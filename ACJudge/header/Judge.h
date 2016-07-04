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
		virtual void prepare() {}

		int compile(tstring s, Language l);
		int run(const Data &d);

		void prepare_submission(const Submission &s);
		void prepare_answer(const tstring &d);
		void prepare_spj(const Task &t);
		void prepare_data(const Data &d);
		
	public:

		void start(const Submission &s, const Task &t);
		void write(tstring filename, tstring text);

		static int process(tstring cmd, LL time, LL space, tstring input = L"input", tstring output = L"output", tstring error = L"errlog");
		static tstring get_suffix(Language l);
	};
}