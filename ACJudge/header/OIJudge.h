#include "Judge.h"
#include "../header/Type.h"

namespace ACOJ
{
	class OIJudge : public Judge
	{
	private:
		int judge(const Data &d);
		
	protected:
		
	public:
		void judge(const Submission &s);
	};
}