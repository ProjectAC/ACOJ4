#include "Judge.h"
#include "../header/Type.h"

namespace ACOJ
{
	class OIJudge : public Judge
	{
	private:
		wstring name;

		int compile(const Submission &s);
		int judge(const Data &d);
		
	protected:
		void prepare(const Submission &s);
		
	public:
		void judge(const Submission &s);
	};
}