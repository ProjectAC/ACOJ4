#pragma once

#include <string>

struct Data
{
	//输入数据
	std::string input;
	//输出数据
	std::string output;
	//时间限制（毫秒）
	unsigned int time;
	//空间限制（KB）
	unsigned int space;
	//对于OI、提交答案题：测试点分值
	int score;
};