#pragma once

#include "../platform.h"
#include <string>

namespace ACOJ
{
	struct Data
	{
		LL did;
		//��������
		tstring input;
		//�������
		tstring output;
		//ʱ�����ƣ����룩
		unsigned int time;
		//�ռ����ƣ�KB��
		unsigned int space;
		//����OI���ύ���⣺���Ե��ֵ
		int score;
	};
}