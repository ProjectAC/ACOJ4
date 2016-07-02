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

	//׼�������ļ������루����ǽű���������Ϊ����ֱ��ͨ����
	prepare_submission(s);
	if ((ret = compile(L"sol", s.language)) != Result::AC)
	{
		t.score = ret;
		db.set_submission(t);
		return;
	}

	//ʹ�����ݲ���
	vector<Data> data = db.get_data(task);
	t.detail.clear();
	for (auto &i : data)
	{
		ret = judge(i);
		if (ret >= 0)  //������ص��Ƿ���
		{
			t.score += ret;
			t.detail.push_back(t.score);
		}
		else  //���򷵻ص��Ǵ���ԭ��
			t.detail.push_back(ret);
	}
	db.set_submission(t);
}