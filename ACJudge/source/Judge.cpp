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

void Judge::write(wstring filename, wstring text)
{
	wofstream fout(name + L"/" + filename);
	fout << text << endl;
	fout.close();
}

void Judge::prepare_spj(const Task &t)
{
	write(L"spj" + get_suffix(t.language), t.spj);
}

void Judge::prepare_submission(const Submission &s)
{
	write(L"sol" + get_suffix(s.language), s.code);
}

void Judge::prepare_data(const Data &d)
{
	write(L"data.in", d.input);
	write(L"data.out", d.output);
}

int Judge::compile(wstring s, Language l)
{
#if defined WINDOWS
	wstring cmd;
	if (l == Language::CPP)
		cmd = L"g++ " + s + L".cpp -o " + s + L".exe -O2 -Wall -lm --static -DONLINE_JUDGE";
	else if (l == Language::CPP11)
		cmd = L"g++ " + s + L".cpp -o " + s + L".exe -O2 -Wall -lm --static -std=c++11 -DONLINE_JUDGE";
	else if (l == Language::C)
		cmd = L"gcc " + s + L".c -o " + s + L".exe -Wall -lm --static -std=c99 -DONLINE_JUDGE ";
	else if (l == Language::PYTHON)
		return Result::AC;
#elif defined LINUX

#endif

	return process(cmd, 5000);
}

int Judge::run(const Data &d)
{
#if defined WINDOWS
	return process(L"sol.exe < data.in > sol.out", d.time);
#elif defined LINUX

#endif
}

int Judge::process(wstring cmd, LL ms)
{
	TCHAR str[100];
	wcscpy(str, cmd.c_str);

#if defined WINDOWS

	STARTUPINFO s;
	_PROCESS_INFORMATION p;

	DWORD ret =
		CreateProcess(NULL, str,  // Command
		NULL,             // Process handle not inheritable.   
		NULL,             // Thread handle not inheritable.   
		FALSE,            // Set handle inheritance to FALSE.   
		0,                // No creation flags.   
		NULL,             // Use parent's environment block.   
		NULL,             // Use parent's starting directory.   
		&s,               // Pointer to STARTUPINFO structure.  
		&p);              // Pointer to PROCESS_INFORMATION structure.
	if (!ret)
		return Result::RTE;

	ret = WaitForSingleObject(p.hProcess, ms);
	LPDWORD code;
	switch (ret)
	{
	case WAIT_OBJECT_0:
		//进程退出了
		ret = GetExitCodeProcess(p.hProcess, code);
		return *code;

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

wstring get_suffix(Language l)
{
	wstring suf;
	switch (l)
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
	return suf;
}