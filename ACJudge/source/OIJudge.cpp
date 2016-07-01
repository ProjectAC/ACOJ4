#include "../header/OIJudge.h"
#include "../result.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
using namespace ACOJ;

int OIJudge::compile(const Submission &s)
{
	wstring cmd;
	if (s.language == Language::CPP)
		cmd = L"g++ sol.cpp -o sol -O2 -Wall -lm --static -DONLINE_JUDGE";
	else if (s.language == Language::CPP11)
		cmd = L"g++ sol.cpp -o sol -O2 -Wall -lm --static -std=c++11 -DONLINE_JUDGE";
	else if (s.language == Language::C)
		cmd = L"gcc sol.c -o sol -Wall -lm --static -std=c99 -DONLINE_JUDGE ";
	else if (s.language == Language::PYTHON)
		return Result::AC;

	thread run(_wsystem, cmd.c_str);
	return count_down(5000, run);
}

int OIJudge::judge(const Data &d)
{

}

void OIJudge::prepare(const Submission &s)
{
	wstring suf;
	switch (s.language)
	{
	case Language::CPP:
	case Language::CPP11:
		suf = L".cpp";
		break;
	case Language::C:
		suf = L".c";
		break;
	case Language::PYTHON:
		suf = L".py";
		break;
	default:
		suf = L"";
		break;
	}

	wofstream fout(name + L"sol" + suf);
	fout << s.code << endl;
	fout.close();
}

void OIJudge::judge(const Submission &s)
{
	Submission t = s;
	int ret;

	//准备代码文件并编译（如果是脚本语言则视为编译直接通过）
	prepare(s);
	if ((ret = compile(s)) != Result::AC)
	{
		t.score = ret;
		db.set_submission(t);
		return;
	}


	vector<Data> data = db.get_data(s);
	for (auto &i : data)
		judge(i);
}