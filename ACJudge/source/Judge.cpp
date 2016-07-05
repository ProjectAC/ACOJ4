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

	//׼�������ļ������루����ǽű���������Ϊ����ֱ��ͨ����
	if (t.type != TaskType::ANSWER)
	{
		prepare_submission(s);
		if ((ret = compile(_T("sol"), s.language)) != Result::AC)
		{
			//��ȡ���������Ϣ
			wifstream fin("errlog");
			TCHAR *x = new TCHAR[100000];
			fin.getline(x, sizeof(TCHAR) * 100000);

			res.score = ret;
			res.CEmessage = x;
			db.set_submission(res);
			return;
		}
	}

	//׼��SPJ�ļ�������
	prepare_spj(t);
	if ((ret = compile(_T("spj"), task.language)) != Result::AC)
	{
		res.score = ret;
		res.CEmessage = _T("Special Judge�������");
		db.set_submission(res);
		return;
	}

	//ʹ�����ݲ���
	vector<Data> data = db.get_data(t);
	res.detail.clear();
	for (size_t i = 0; i < data.size(); i++)
	{
		//�ύ����׼��ѡ�ֵĴ��
		if (task.type == TaskType::ANSWER)
			prepare_answer(s.answer[i]);
		//ִ�е����������
		ret = judge(data[i]);
		if (ret >= 0)  //������ص��Ƿ���
		{
			res.score += ret;
			res.detail.push_back(res.score);
		}
		else  //���򷵻ص��Ǵ���ԭ��
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

	//������
	ZeroMemory(&ex_lim, sizeof(ex_lim));

	//������������
	ex_lim.BasicLimitInformation.LimitFlags |= JOB_OBJECT_LIMIT_ACTIVE_PROCESS;
	ex_lim.BasicLimitInformation.ActiveProcessLimit = 1;

	//�����ڴ�����
	ex_lim.BasicLimitInformation.LimitFlags |= JOB_OBJECT_LIMIT_JOB_MEMORY;
	ex_lim.JobMemoryLimit = SIZE_T(space * 1000);

	//����ʱ������
	ex_lim.BasicLimitInformation.LimitFlags |= JOB_OBJECT_LIMIT_JOB_TIME;
	ex_lim.BasicLimitInformation.PerJobUserTimeLimit.QuadPart = time * 10000;

	//����ϵͳ��������
	bs_ui.UIRestrictionsClass =
		JOB_OBJECT_UILIMIT_EXITWINDOWS |
		JOB_OBJECT_UILIMIT_READCLIPBOARD |
		JOB_OBJECT_UILIMIT_WRITECLIPBOARD |
		JOB_OBJECT_UILIMIT_SYSTEMPARAMETERS |
		JOB_OBJECT_UILIMIT_DISPLAYSETTINGS |
		JOB_OBJECT_UILIMIT_GLOBALATOMS |
		JOB_OBJECT_UILIMIT_DESKTOP |
		JOB_OBJECT_UILIMIT_HANDLES;

	//�����ļ�
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
	//����IO�ӿ�
	STARTUPINFO s = { sizeof(s) };
	ZeroMemory(&s, sizeof(s));
	s.cb = sizeof(STARTUPINFO);
	s.hStdOutput = fin;
	s.hStdError = ferr;
	s.hStdInput = fout;
	s.dwFlags = STARTF_USESTDHANDLES;

	//������ҵ
	HANDLE job = CreateJobObject(NULL, NULL);
	SetInformationJobObject(job, JobObjectExtendedLimitInformation, &ex_lim, sizeof(ex_lim));
	SetInformationJobObject(job, JobObjectBasicUIRestrictions, &bs_ui, sizeof(bs_ui));

	//��������
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

	//�����̰󶨵���ҵ��Ȼ�����
	AssignProcessToJobObject(job, p.hProcess);
	ResumeThread(p.hThread);

	//��ʼ��ʱ
	HANDLE handles[2];
	handles[0] = p.hProcess;
	handles[1] = job;
	ret = WaitForMultipleObjects(2, handles, FALSE, INFINITE);
	switch (ret)
	{
	case WAIT_OBJECT_0:
		//�����˳���
		GetExitCodeProcess(p.hProcess, &ret);
		if (ret == 1816)
			return Result::TLE;
		else if (ret)
			return Result::RTE;
		else
			return Result::AC;

	case WAIT_TIMEOUT:
		//���̳�ʱ��
		return Result::TLE;

	case WAIT_FAILED:
		//���̱�ը��
		return Result::RTE;
	}

	//δ֪����
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