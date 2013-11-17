#ifndef __COMMON_UTIL_H__
#define __COMMON_UTIL_H__

#include <string>
#include <stdio.h>
#include <time.h>

#include <QThread>

namespace tools {
	class TimeUtil {
	public:
		// Get current date/time, in format: YYYY-MM-DD.HH:mm:ss
		static const std::string getCurrentTime() {
			char			buf[80];
			time_t			now = time(0);
			struct tm		tstruct = *localtime(&now);

			// Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
			// for more information about date/time format
			strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
			return buf;
		}
	};

	class ThreadUtil {
	public:
		static inline const void* getCurrentThreadID() {
			return QThread::currentThreadId();
		}
	};
}

#endif