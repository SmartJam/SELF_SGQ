#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <iostream>
#include <fstream>

#include <QString>

#include "CommonUtil.h"

namespace tools {

#define CURRENT_THREAD_TIME TimeUtil::getCurrentTime() << " thread(" << ThreadUtil::getCurrentThreadID() << ")"

	class Logger {
	public:
		enum LogType { LOG_ERROR, LOG_WARNING, LOG_INFO };

		explicit Logger (const char *fname = "tools_log.txt")
			: _numErrors(0), _numWarnings(0), _numInfos(0)
		{
			std::ofstream* outLogFile = new std::ofstream(fname, std::ofstream::out | std::ofstream::app);
			if (outLogFile == NULL) {
				_isFileOstream = false;
				_outLogFile = &std::cout;
			}
			
			if (outLogFile->is_open()) {
				_isFileOstream = true;
				_outLogFile = outLogFile;
				*_outLogFile << CURRENT_THREAD_TIME << " Log file [" << fname << "] opened.";
			} 
			else {
				delete outLogFile;
				outLogFile = NULL;
				_isFileOstream = false;
				_outLogFile = &std::cout;
				*_outLogFile <<  CURRENT_THREAD_TIME 
					         << " create Log file [" << fname << "] error. use stdout instead.";
			}
		}

		~Logger () {
			*_outLogFile << std::endl << std::endl << CURRENT_THREAD_TIME << " Logger close." 
						 << " INFOs:[" << _numInfos << "] WARNs:[" << _numWarnings << "] ERRORs:[" << _numErrors << "]"
						 << "\n\n";

			if (_isFileOstream) {
				std::ofstream* outLogFile = (std::ofstream*) _outLogFile;
				if (outLogFile->is_open()) {
					outLogFile->close();
					delete _outLogFile;
					_outLogFile = NULL;
				}
			}
		}

		// Overload << operator using log type
		friend Logger &operator << (Logger &logger, const LogType l_type) {

			//*(logger._outLogFile) << "\n" << CURRENT_THREAD_TIME;
			*(logger._outLogFile) << std::endl << CURRENT_THREAD_TIME;

			switch (l_type) {
			case Logger::LOG_ERROR:
				*(logger._outLogFile) << "   [ERROR] ";
				++logger._numErrors;
				break;

			case Logger::LOG_WARNING:
				*(logger._outLogFile) << " [WARNING] ";
				++logger._numWarnings;
				break;

			default:
				++logger._numInfos;
				*(logger._outLogFile) << "    [INFO] ";
				break;
			} 

			return logger;
		}

		// Overload << operator using C style strings
		// No need for std::string objects here
		template<typename T>
		friend Logger &operator << (Logger &logger, const T& msg) {
			*(logger._outLogFile) << msg;
			return logger;
		}

		friend Logger &operator << (Logger &logger, const QString& msg) {
			*(logger._outLogFile) << msg.toLocal8Bit().data();
			return logger;
		}

	private:
		int				_numErrors;
		int				_numWarnings;
		int				_numInfos;
		bool			_isFileOstream;
		std::ostream*    _outLogFile;
	}; // class end

}  // namespace

static tools::Logger LoggerInstance("./log.txt");
#define LOG_INFO LoggerInstance << tools::Logger::LOG_INFO << __FILE__ << "(" << __LINE__ << "):"
#define LOG_WARN LoggerInstance << tools::Logger::LOG_WARNING << __FILE__ << "(" << __LINE__ << "):"
#define LOG_ERROR LoggerInstance << tools::Logger::LOG_ERROR << __FILE__ << "(" << __LINE__ << "):"

#endif 