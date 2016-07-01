#include "../platform.h"
#include "Database.h"
#include <thread>

namespace ACOJ
{
	class Judge
	{
	protected:
		Database &db;
	
		virtual void prepare(int sid);
		
	int count_down(int ms, std::thread &t);
		
		void load_data(int tid, int did);
		
	public:
		virtual void judge(int sid);
	};
}