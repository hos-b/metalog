
#ifndef __META_LOG_H__
#define __META_LOG_H__

#include <ctime>

#include <vector>
#include <unordered_map>

#include "types.h"
#include "defaults.h"
#include "double_writer.h"


template <typename _T>
class MetaLog
{
public:
	// default constuctor
	MetaLog() {
		dout = nullptr;
		fout = nullptr;
		dwriter_ = new DoubleStreamWriter<decltype(fout), decltype(dout)>(fout, dout);
	}
	~MetaLog() {
		
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
	/* args: 
		1) output stream (err,out | file)
		2) output file path
	*/
	void Init(outstream log_output, const std::string& output_file = "") {
		if (log_output & output::out) dout = &std::cout;
		else if (log_output & output::err) dout = &std::cerr;
		else dout = nullptr;
		if (log_output & output::logfile) {
			fout = new std::ofstream;
			fout->open((output_file == "") ? DEFAULT_LOGFILE : output_file,
					   std::ios::out | std::ios::app);
			time_t now = time(0);
			(*fout) << "\n>>> new log started: " << ctime(&now) << "\n";
		} else {
			fout = nullptr;
		}
		dwriter_ = new DoubleStreamWriter<decltype(fout), decltype(dout)>(fout, dout);
	}
	void Log(typename _T::type type, const std::string& msg) {

	}
	void TimedLog(typename _T::type type, const std::string& signature, const std::string& variadics, unsigned int id, long msec) {

	}
	void DifferentialLog(typename _T::type type, const std::string& signature, const std::string& variadics) {

	}

private:
	std::ostream* dout;
	std::ofstream* fout;
	DoubleStreamWriter<std::nullptr_t, std::nullptr_t>* dwriter_;
};

using Log = MetaLog<deflog>;
#endif
