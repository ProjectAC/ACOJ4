#include <string>
#include <tchar.h>

#define WINDOWS
#define UNICODE

#ifdef ANSI
typedef std::string tstring;
typedef std::ifstream tifstream;
typedef std::ofstream tofstream;
#else
typedef std::wstring tstring;
typedef std::wifstream tifstream;
typedef std::wofstream tofstream;
#endif