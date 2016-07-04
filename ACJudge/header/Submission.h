#pragma once

#include <string>
#include <vector>
#include "../language.h"

namespace ACOJ
{
	struct Submission
	{
		//sid�ύ��ţ�uid�û���ţ�tid��Ŀ���
		LL sid, uid, tid;
		//�����OI���ύ���⣬score��ʾ���յ÷֣�
		//�����ACM�⣬score��ʾ״̬��
		int score;
		//�����ѡ�ֵ�����
		Language language;
		//����Ƿ������ô�������
		tstring code;
		//����������ô�����
		std::vector<tstring> answer;
		//detail����ÿһ�����Ե��С�֡�
		std::vector<int> detail;
		//���������Ϣ
		tstring CEmessage;
	};
}