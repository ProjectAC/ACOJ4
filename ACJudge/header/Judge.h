#include "../platform.h"
#include "Type.h"
#include "Database.h"
#include <thread>

namespace ACOJ
{
	class Judge
	{
	protected:
		std::wstring name;
		Database &db;
		Task &task;

		int compile(std::wstring s, Language l);
		int run(const Data &d);
		int score();

		void prepare_submission(const Submission &s);
		void prepare_data(const Data &d);
		void prepare_spj(const Task &t);
		
	public:

		virtual void judge(int sid);
		void write(std::wstring filename, std::wstring text);

		static int process(std::wstring cmd, LL ms);
		static std::wstring get_suffix(Language l);
	};
}