#include "header/OIJudge.h"
#include "header/ACMJudge.h"
#include "header/SubmitJudge.h"
#include "tasktype.h"
#include <thread>

using namespace std;
using namespace ACOJ;

int wmain()
{
	Database db("DatabaseName", "username", "password");
	OIJudge oij(db);
	ACMJudge acmj(db);
	SubmitJudge aj(db);
	unsigned int sid;
	
	//Main loop
	while(true)
	{
		if(sid = db.get_submission())
		{
			switch(db.get_task_type())
			{
			case TaskType::OI:
				oij.judge(sid);
				break;
			case TaskType::ACM:
				acmj.judge(sid);
				break;
			case TaskType::ANSWER:
				aj.judge(sid);
				break;
			}
		}
		else
			this_thread::sleep_for(chrono::milliseconds(100));
	}
	
	return 0;
}