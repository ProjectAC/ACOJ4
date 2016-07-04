#include "../header/OIJudge.h"
#include "../result.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
using namespace ACOJ;

int OIJudge::judge(const Data &d)
{
	prepare_data(d);
	run(d);
	return score();
}