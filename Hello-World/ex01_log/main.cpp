#include <iostream>


class Log
{
public:
	enum LogLevel {
		WARNING,
		ERROR,
		INFO,
	};

private:
	enum LogLevel m_LogLevel;

private:
	void (Log::*_log)(const char *);

	void warn(const char *message)
	{
		std::cout << "\033[32m[WARNING]: " << message << "\033[m" << std::endl;
	}

	void error(const char *message)
	{
		std::cout << "\033[31m[ERROR]: " << message << "\033[m" << std::endl;
	}

	void info(const char *message)
	{
		std::cout << "\033[35m[INFO]: " << message << "\033[m" << std::endl;
	}

public:
	void log(const char *message)
	{
		(*this.*_log)(message);
	}

	void setLevel(LogLevel level)
	{
		m_LogLevel = level;
		switch (level) {
			case WARNING: _log = &Log::warn; break;
			case ERROR: _log = &Log::error; break;
			case INFO: _log = &Log::info; break;
		}
	}
	LogLevel getLevel()
	{
		return m_LogLevel;
	}
};

int main()
{
	Log log;
	log.setLevel(Log::WARNING);
	log.log("Hello!");
}
