# ifndef STOPWATCH11_H
#	define STOPWATCH11_H
#	include <chrono>
//PS: ticks == ms
	using namespace std::chrono;
	class TimerBaseChrono {
	public:
		//	clears the timer
		TimerBaseChrono() : m_start(system_clock::time_point::min()) { }

		//  clears the timer
		void Clear() { 
			m_start = system_clock::time_point::min(); 
		}

		//	returns true if the timer is running
		bool IsStarted() const {
			return (m_start != system_clock::time_point::min());
		}

		//	start the timer
		void Start()            { m_start = std::chrono::system_clock::now(); }

		//	get the number of milliseconds since the timer was started
		using tick_t=unsigned long;
		tick_t GetTicks() const {
			if (IsStarted()) {
				system_clock::duration diff;
				diff = system_clock::now() - m_start;
				return (tick_t)(duration_cast<milliseconds>(diff).count());
			}
			return 0;
		}
		tick_t GetMs() const {
			return GetTicks();
		}
		static tick_t GetMs(tick_t ticks)  {
			return ticks;
		}
	private:
		std::chrono::system_clock::time_point m_start;
	}; 
#   include "stopwatch.h"
	typedef basic_stopwatch<TimerBaseChrono> Stopwatch;
# endif
