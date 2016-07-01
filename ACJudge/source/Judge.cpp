#include <thread>
#include "../header/Judge.h"

using namespace std;
using namespace ACOJ;

void Judge::count_down(int ms)
{
	this_thread::sleep_for(chrono::milliseconds(ms));
}