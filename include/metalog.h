
#ifndef __META_LOG_H__
#define __META_LOG_H__

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

// hidden status namespace
namespace {
	enum class _STS {
		UNINITIALIZED,
		INITIALIZED
	};
}
template <typename _T>
class MetaLog
{
public:
	// default constuctor
	MetaLog() {
		#ifdef __unix__
		logfile_ = "~/metalog";
		#elif defined(_WIN32)
		logfile_ = "c:\\metalog";
		#endif
		status_ = _STS::UNINITIALIZED;
	}
	// singleton stuff
	MetaLog(const MetaLog&) = delete;
    MetaLog& operator=(const MetaLog &) = delete;
    MetaLog(MetaLog &&) = delete;
    MetaLog & operator=(MetaLog &&) = delete;
	static MetaLog& GetInstance() {
		static MetaLog instance;
		return instance;
	}

	static void Init()

private:
	static _T type_;
	std::string logfile_;
	_STS status_;
};

#endif
