#pragma once

#include <string>
#include <vector>
#include "../tasktype.h"
#include "../language.h"

namespace ACOJ
{
	struct Task
	{
		//tid题目编号
		LL tid;
		//Special Judge
		tstring spj;
		//Special Judge编写语言
		Language language;
		//题目类型
		TaskType type;
	};
}