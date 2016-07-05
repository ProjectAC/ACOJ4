#include <thread>
#include <iostream>
#include <fstream>
#include "../result.h"
#include "../header/Judge.h"
#include "../platform.h"

#if defined WINDOWS
#include <Windows.h>
#elif defined LINUX

#endif

using namespace std;
using namespace ACOJ;

void Judge::write(tstring filename, tstring text)
{
	tofstream fout(name + _T("/") + filename);
	fout << text << endl;
	fout.close();
}

void Judge::prepare_answer(const tstring &s)
{
	write(_T("sol.out"), s);
}

void Judge::prepare_spj(const Task &t)
{
	write(_T("spj") + get_suffix(t.language), t.spj);
}

void Judge::prepare_submission(const Submission &s)
{
	write(_T("sol") + get_suffix(s.language), s.code);
}

void Judge::prepare_data(const Data &d)
{
	write(_T("data.in"), d.input);
	write(_T("data.out"), d.output);
}

int Judge::compile(tstring s, Language l)
{
	tstring cmd;

#if defined WINDOWS
	if (l == Language::CPP)
		cmd = _T("g++ ") + s + _T(".cpp -o ") + s + _T(".exe -O2 -Wall -lm --static -DONLINE_JUDGE");
	else if (l == Language::CPP11)
		cmd = _T("g++ ") + s + _T(".cpp -o ") + s + _T(".exe -O2 -Wall -lm --static -std=c++11 -DONLINE_JUDGE");
	else if (l == Language::C)
		cmd = _T("gcc ") + s + _T(".c -o ") + s + _T(".exe -Wall -lm --static -std=c99 -DONLINE_JUDGE");
	else if (l == Language::PYTHON)
		return Result::AC;
#elif defined LINUX
	
#endif

	int ret = process(cmd, 5000, -1, _T(""), _T(""), _T("errlog"));
	if (ret == 1)
		return Result::CE;
	else
		return ret;
}

void Judge::start(const Submission &s, const Task &t)
{
	Submission res;
	int ret;

	//准备代码文件并编译（如果是脚本语言则视为编译直接通过）
	if (t.type != TaskType::ANSWER)
	{
		prepare_submission(s);
		if ((ret = compile(_T("sol"), s.language)) != Result::AC)
		{
			//读取编译错误信息
			wifstream fin("errlog");
			TCHAR *x = new TCHAR[100000];
			fin.getline(x, sizeof(TCHAR) * 100000);

			res.score = ret;
			res.CEmessage = x;
			db.set_submission(res);
			return;
		}
	}

	//准备SPJ文件并编译
	prepare_spj(t);
	if ((ret = compile(_T("spj"), task.language)) != Result::AC)
	{
		res.score = ret;
		res.CEmessage = _T("Special Judge编译错误！");
		db.set_submission(res);
		return;
	}

	//使用数据测试
	vector<Data> data = db.get_data(t);
	res.detail.clear();
	for (size_t i = 0; i < data.size(); i++)
	{
		//提交答案题准备选手的答卷
		if (task.type == TaskType::ANSWER)
			prepare_answer(s.answer[i]);
		//执行单点评测过程
		ret = judge(data[i]);
		if (ret >= 0)  //如果返回的是分数
		{
			res.score += ret;
			res.detail.push_back(res.score);
		}
		else  //否则返回的是错误原因
			res.detail.push_back(ret);
	}
	db.set_submission(res);
}

int Judge::run(const Data &d)
{
#if defined WINDOWS
	return process(_T("sol.exe"), d.time, d.space, _T("data.in"), _T("data.out"));
#elif defined LINUX

#endif
}

int Judge::process(tstring cmd, LL time, LL space, tstring input, tstring output, tstring error)
{
#if defined WINDOWS

	if (time == -1)
		time = INFINITE;
	if (space == -1)
		space = 0x7fffffff;
	TCHAR str[1000];
	wcscpy_s(str, cmd.c_str());
	
	JOBOBJECT_EXTENDED_LIMIT_INFORMATION ex_lim;
	JOBOBJECT_BASIC_UI_RESTRICTIONS bs_ui;

	//清零标记
	ZeroMemory(&ex_lim, sizeof(ex_lim));

	//设置最大进程数
	ex_lim.BasicLimitInformation.LimitFlags |= JOB_OBJECT_LIMIT_ACTIVE_PROCESS;
	ex_lim.BasicLimitInformation.ActiveProcessLimit = 1;

	//设置内存限制
	ex_lim.BasicLimitInformation.LimitFlags |= JOB_OBJECT_LIMIT_JOB_MEMORY;
	ex_lim.JobMemoryLimit = SIZE_T(space * 1000);

	//设置时间限制
	ex_lim.BasicLimitInformation.LimitFlags |= JOB_OBJECT_LIMIT_JOB_TIME;
	ex_lim.BasicLimitInformation.PerJobUserTimeLimit.QuadPart = time * 10000;

	//设置系统调用限制
	bs_ui.UIRestrictionsClass =
		JOB_OBJECT_UILIMIT_EXITWINDOWS |
		JOB_OBJECT_UILIMIT_READCLIPBOARD |
		JOB_OBJECT_UILIMIT_WRITECLIPBOARD |
		JOB_OBJECT_UILIMIT_SYSTEMPARAMETERS |
		JOB_OBJECT_UILIMIT_DISPLAYSETTINGS |
		JOB_OBJECT_UILIMIT_GLOBALATOMS |
		JOB_OBJECT_UILIMIT_DESKTOP |
		JOB_OBJECT_UILIMIT_HANDLES;

	//创建文件
	HANDLE fin = CreateFile(input.c_str(),
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ,
		0,
		CREATE_ALWAYS,
		FILE_FLAG_OVERLAPPED,
		0);
	HANDLE fout = CreateFile(output.c_str(),
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ,
			0,
			CREATE_ALWAYS,
			FILE_FLAG_OVERLAPPED,
			0);
	HANDLE ferr = CreateFile(error.c_str(),
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ,
		0,
		CREATE_ALWAYS,
		FILE_FLAG_OVERLAPPED,
		0);
	//创建IO接口
	STARTUPINFO s = { sizeof(s) };
	ZeroMemory(&s, sizeof(s));
	s.cb = sizeof(STARTUPINFO);
	s.hStdOutput = fin;
	s.hStdError = ferr;
	s.hStdInput = fout;
	s.dwFlags = STARTF_USESTDHANDLES;

	//创建作业
	HANDLE job = CreateJobObject(NULL, NULL);
	SetInformationJobObject(job, JobObjectExtendedLimitInformation, &ex_lim, sizeof(ex_lim));
	SetInformationJobObject(job, JobObjectBasicUIRestrictions, &bs_ui, sizeof(bs_ui));

	//创建进程
	_PROCESS_INFORMATION p;
	DWORD ret =
		CreateProcess(NULL, str,
		NULL,
		NULL,
		FALSE,
		CREATE_SUSPENDED,
		NULL,
		NULL, 
		&s,
		&p);
	if (!ret)
		return Result::RTE;

	//将进程绑定到作业，然后搞起！
	AssignProcessToJobObject(job, p.hProcess);
	ResumeThread(p.hThread);

	//开始计时
	HANDLE handles[2];
	handles[0] = p.hProcess;
	handles[1] = job;
	ret = WaitForMultipleObjects(2, handles, FALSE, INFINITE);
	switch (ret)
	{
	case WAIT_OBJECT_0:
		//进程退出了
		GetExitCodeProcess(p.hProcess, &ret);
		if (ret == 1816)
			return Result::TLE;
		else if (ret)
			return Result::RTE;
		else
			return Result::AC;

	case WAIT_TIMEOUT:
		//进程超时了
		return Result::TLE;

	case WAIT_FAILED:
		//进程爆炸了
		return Result::RTE;
	}

	//未知错误
	return Result::RTE;

#elif defined LINUX

#endif

	return Result::RTE;
}

tstring Judge::get_suffix(Language l)
{
	tstring suf;
	switch (l)
	{
	case Language::CPP:
	case Language::CPP11:
		suf = _T(".cpp");
		break;
	case Language::C:
		suf = _T(".c");
		break;
	case Language::PYTHON:
		suf = _T(".py");
		break;
	default:
		suf = _T("");
		break;
	}
	return suf;
}