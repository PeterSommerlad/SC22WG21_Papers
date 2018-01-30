# include "stopwatch11.h"
# include <thread>

void Sleep(unsigned ms)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

int test_stopwatch11(int test_no, unsigned long)
{
	bool rc = true;
	switch (test_no)
	{
	default:	return -1;
	case 0:		return 4;
	case 1: {
				TimerBaseChrono tb;
				tb.Start();
				Sleep(5);
				unsigned dt1 = tb.GetMs();
				Sleep(20);
				unsigned dt2 = tb.GetMs();
				Sleep(35);
				unsigned dt3 = tb.GetMs();
				break;
			}

	case 2: {
				Stopwatch sw;
				Sleep(15);
				unsigned dt1 = sw.Show();
				rc &= (dt1 == sw.LapGet());
				rc &= (15 <= dt1 && dt1 <= 32);
				Sleep(25);
				unsigned dt2 = sw.Show();
				rc &= (dt2 == sw.LapGet());
				rc &= (dt1 + 25 <= dt2 && dt2 <= dt1 + 32);
				Sleep(15);
				unsigned dt3 = sw.Stop();
				rc &= (dt3 == sw.LapGet());
				rc &= (dt2 + 15 <= dt3 && dt3 <= dt2 + 32);
				break;
			}

	case 3:	{
				Stopwatch sw("timing run", false);
				Sleep(10);
				unsigned dt1 = sw.Start();
				rc &= (dt1 == 0);
				Sleep(15);
				unsigned dt2 = sw.Start("event1");
				rc &= (15 <= dt2 && dt2 <= 32);
				Sleep(25);
				unsigned dt3 = sw.Start("event2");
				rc &= (25 <= dt3 && dt3 <= 32);
				Sleep(5);
				unsigned dt4 = sw.Stop("event3");
				rc &= (5 <= dt4 && dt4 <= 16);
				break;
			}

	case 4:	{
				Stopwatch sw("");
				Sleep(15);
				unsigned dt1 = sw.Show("");
				rc &= (15 <= dt1 && dt1 <= 32);
				unsigned dt2 = sw.Start("");
				rc &= (15 <= dt2 && dt2 <= 32);
				break;
			}
	}//	end switch
	return rc ? 1 : 0;
}
