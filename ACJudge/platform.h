#include <string>
#include <tchar.h>
#include <cstdint>

#define WINDOWS
#define DATABASE

#ifdef ANSI
typedef std::string tstring;
typedef std::ifstream tifstream;
typedef std::ofstream tofstream;
#else
typedef std::wstring tstring;
typedef std::wifstream tifstream;
typedef std::wofstream tofstream;
#endif

namespace ACOJ
{
	typedef uint64_t LL;
}