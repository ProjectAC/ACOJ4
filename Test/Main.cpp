#include<windows.h>
#include<iostream>

using namespace std;

typedef unsigned long long LL;

int process(wstring cmd, LL time, LL space)
{
	if (time == -1)
		time = INFINITE;
	if (space == -1)
		space = 0x7fffffff;
	TCHAR str[10000] = L"g++";
	//wcscpy(str, );

	JOBOBJECT_EXTENDED_LIMIT_INFORMATION ex_lim;
	JOBOBJECT_BASIC_UI_RESTRICTIONS bs_ui;
	JOBOBJECTINFOCLASS info;

	HANDLE iocp;
	JOBOBJECT_ASSOCIATE_COMPLETION_PORT acp;

	//清零标记
	ZeroMemory(&ex_lim, sizeof(ex_lim));

	//设置最大进程数
	ex_lim.BasicLimitInformation.LimitFlags |= JOB_OBJECT_LIMIT_ACTIVE_PROCESS;
	ex_lim.BasicLimitInformation.ActiveProcessLimit = 1;

	//设置内存限制
	ex_lim.BasicLimitInformation.LimitFlags |= JOB_OBJECT_LIMIT_JOB_MEMORY;
	ex_lim.JobMemoryLimit = space;

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

	//创建IO接口
	iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	//创建作业
	HANDLE job = CreateJobObject(NULL, NULL);
	SetInformationJobObject(job, JobObjectExtendedLimitInformation, &ex_lim, sizeof(ex_lim));
	SetInformationJobObject(job, JobObjectBasicUIRestrictions, &bs_ui, sizeof(bs_ui));

	//创建进程
	STARTUPINFO s = { sizeof(s) };
	_PROCESS_INFORMATION p;
	DWORD ret =
		CreateProcess(NULL, str,  // Command
			NULL,              // Process handle not inheritable.   
			NULL,              // Thread handle not inheritable.   
			FALSE,             // Set handle inheritance to FALSE.
			CREATE_SUSPENDED,  // No creation flags.
			NULL,              // Use parent's environment block.   
			NULL,              // Use parent's starting directory.   
			&s,                // Pointer to STARTUPINFO structure.  
			&p);               // Pointer to PROCESS_INFORMATION structure.
	if (!ret)
		return 0;

	//将进程绑定到作业，然后搞起！
	AssignProcessToJobObject(job, p.hProcess);
	ResumeThread(p.hThread);
	cout << "开始！" << endl;

	//开始计时
	HANDLE handles[2];
	handles[0] = p.hProcess;
	handles[1] = job;
	ret = WaitForMultipleObjects(2, handles, FALSE, time*10);
	QueryInformationJobObject(job, JobObjectExtendedLimitInformation,
		&ex_lim, sizeof(ex_lim), NULL);
	printf("%d %d %d\n", ex_lim.PeakJobMemoryUsed, ex_lim.JobMemoryLimit, ex_lim.BasicLimitInformation.PerJobUserTimeLimit);

	//ret = WaitForSingleObject(p.hProcess, );
	DWORD ret2;
	LPDWORD code = &ret2;
	switch (ret)
	{
	case WAIT_OBJECT_0:
		//进程退出了
		cout << "正常！返回：";
		ret = GetExitCodeProcess(p.hProcess, code);
		cout << *code << endl;
		cout << "可以" << endl;
		return 0;

	case WAIT_TIMEOUT:
		//进程超时了
		cout << "超时！" << endl;
		return 0;

	case WAIT_FAILED:
		//进程爆炸了
		cout << "爆炸！" << endl;
		return 0;
	}
	return 0;
}

int main()
{
	process(L"tmp3.exe > fuck.txt", 1000, 1000000000);
	system("pause");
	return 0;
}
