#include "header/OIJudge.h"
//#include "header/ACMJudge.h"
//#include "header/SubmitJudge.h"
#include "tasktype.h"
#include <thread>

using namespace std;
using namespace ACOJ;

void wait_for_submission(Submission &s, Database &db)
{
	s.sid = 0;
	while (s = db.get_submission(), !s.sid)
		this_thread::sleep_for(chrono::milliseconds(500));
}

int wmain()
{
	Database db(_T("DatabaseName"), _T("username"), _T("password"));
	OIJudge oij(db);
	//ACMJudge acmj(db);
	//AnswerJudge aj(db);
	Submission s;
	Task t;
	
	//Main loop
	thread postman(wait_for_submission, s, db);
	while(true)
	{
		postman.join();
		t = db.get_task(s.tid);
		switch(t.type)
		{
		case TaskType::OI:
			oij.start(s, t);
			break;
		/*case TaskType::ACM:
			acmj.judge(s, t);
			break;
		case TaskType::ANSWER:
			aj.judge(s, t);
			break;
		*/
		}
	}

	return 0;
}