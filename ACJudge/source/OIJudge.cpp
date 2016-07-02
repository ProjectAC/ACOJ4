#include "../header/OIJudge.h"
#include "../result.h"
#include <vector>

using namespace std;
using namespace ACOJ;

int OIJudge::judge(const Data &d)
{
	prepare_data(d);
	run(d);
	return score();
}

void OIJudge::judge(const Submission &s)
{
	Submission t = s;
	int ret;

	//准备代码文件并编译（如果是脚本语言则视为编译直接通过）
	prepare_submission(s);
	if ((ret = compile(L"sol", s.language)) != Result::AC)
	{
		t.score = ret;
		db.set_submission(t);
		return;
	}

	//使用数据测试
	vector<Data> data = db.get_data(task);
	t.detail.clear();
	for (auto &i : data)
	{
		ret = judge(i);
		if (ret >= 0)  //如果返回的是分数
		{
			t.score += ret;
			t.detail.push_back(t.score);
		}
		else  //否则返回的是错误原因
			t.detail.push_back(ret);
	}
	db.set_submission(t);
}