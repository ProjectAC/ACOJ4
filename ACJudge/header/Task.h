#pragma once

#include <string>
#include <vector>
#include "../tasktype.h"
#include "../language.h"

namespace ACOJ
{
	struct Task
	{
		//tid��Ŀ���
		LL tid;
		//Special Judge
		tstring spj;
		//Special Judge��д����
		Language language;
		//��Ŀ����
		TaskType type;
	};
}