#pragma once

#include <string>
#include <vector>
#include "../language.h"

namespace ACOJ
{
	struct Submission
	{
		//sid提交编号，uid用户编号，tid题目编号
		unsigned long long sid, uid, tid;
		//如果是OI或提交答案题，score表示最终得分；
		//如果是ACM题，score表示状态。
		int score;
		//这个是选手的语言
		Language language;
		//如果是非提答，那么保存代码
		std::wstring code;
		//如果是提答，那么保存答案
		std::vector<std::wstring> answer;
		//detail保存每一个测试点的小分。
		std::vector<int> detail;
	};
}