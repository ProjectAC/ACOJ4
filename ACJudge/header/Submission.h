#pragma once

#include <string>
#include <vector>
#include "../language.h"

namespace ACOJ
{
	struct Submission
	{
		//sid�ύ��ţ�uid�û���ţ�tid��Ŀ���
		unsigned long long sid, uid, tid;
		//�����OI���ύ���⣬score��ʾ���յ÷֣�
		//�����ACM�⣬score��ʾ״̬��
		int score;
		//�����ѡ�ֵ�����
		Language language;
		//����Ƿ������ô�������
		std::wstring code;
		//����������ô�����
		std::vector<std::wstring> answer;
		//detail����ÿһ�����Ե��С�֡�
		std::vector<int> detail;
	};
}