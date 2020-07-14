
#ifndef __META_LOG_H__
#define __META_LOG_H__

#include <ctime>
#include <chrono>
#include <unordered_set>
#include <type_traits>
#include <fstream>

#include "types.h"
#include "defaults.h"
#include "multi_writer.h"
#include "helper.h"

template <typename T>
class MetaLog
{
public:
	// default constuctor
	constexpr MetaLog() {
		dout_ = &std::cout;
		fout_ = nullptr;
		mwriter_ = new MultiStreamWriter<decltype(dout_), decltype(fout_)>(dout_, fout_);
		max_log_level_ = static_cast<typename T::type>(0);
		static_assert(std::is_convertible<typename T::type, int>::value, "selected log type enum should be convertible to int");
	}
	~MetaLog() {
		if (fout_) {
			fout_->flush();
			fout_->close();
		}
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
	void Init(typename T::type max_log_level, outstream log_output, const std::string& output_file = "") {
		max_log_level_ = max_log_level;
		if (log_output & output::out) dout_ = &std::cout;
		else if (log_output & output::err) dout_ = &std::cerr;
		else dout_ = nullptr;
		if (log_output & output::logfile) {
			fout_ = new std::ofstream;
			fout_->open((output_file == "") ? DEFAULT_LOGFILE : output_file,
					   std::ios::out | std::ios::app);
			time_t now = time(0);
			(*fout_) << "\n>>> new log started: " << ctime(&now) << ", max log level = " << T::ToString(max_log_level) << "\n";
		} else {
			fout_ = nullptr;
		}
		delete mwriter_;
		mwriter_ = new MultiStreamWriter<decltype(dout_), decltype(fout_)>(dout_, fout_);
	}
	void SetMaximumLogLevel(typename T::type max_log_level) {
		max_log_level_ = max_log_level;
	}
	void FlushStreams() {
		mwriter_->Flush();
	}
private:
	// base printer
	void printf(const char *s) {
		while (*s) {
			if (*s == '%') {
				if (*(s + 1) != '%')
					throw std::runtime_error("invalid format string: missing arguments");
				else
					++s;
			}
			mwriter_->Write(*s++);
		}
	}
	// recursive printer
	template<typename _PT, typename... _PArgs>
	void printf(const char *s, _PT value, _PArgs... args) {
		while (*s) {
			if (*s == '%') {
				if (*(s + 1) != '%') {
					mwriter_->Write(value);
					++s;
					printf(s, args...); // called even when *s is 0 but does nothing in that case (and ignores extra arguments)
					return;
				} else {
					++s;
				}
			}
			mwriter_->Write(*s++);
		}    
	}


public:
	template<typename... VA>
	void Log(typename T::type type, const std::string_view& format, VA... args) {
		if (type >= max_log_level_)
			printf(format.data(), args...);
	}
	template<typename... VA>
	void TimedLog(typename T::type type, size_t id, long msec, const std::string& format, VA... args) {
		if (id + 1 > latest_.size()) {
			latest_.resize(id + 1);
			latest_[id] = std::chrono::steady_clock::now();
			if (type >= max_log_level_)
				printf(format.data(), args...);
		} else {
			auto now = std::chrono::steady_clock::now();
			if (std::chrono::duration_cast<std::chrono::milliseconds>(now - latest_[id]).count() >= msec) {
				if (type >= max_log_level_) {
					latest_[id] = now;
					printf(format.data(), args...);
				}
			}
		}	
	}

	template<typename... VA>
	void UniqueLog(typename T::type type, const std::string_view& format, VA... args) {
		size_t collective_hash = GetVariadicHash(format.size(), args...);
		if (hash_set_.find(collective_hash) == hash_set_.end()) {
			if (type >= max_log_level_) {
				hash_set_.insert(collective_hash);
				printf(format.data(), args...);
			}
		}
	}

private:
	typename T::type max_log_level_;
	std::ostream* dout_;
	std::ofstream* fout_;
	MultiStreamWriter<std::ostream*, std::ofstream*>* mwriter_;
	std::vector<std::chrono::steady_clock::time_point> latest_;
	std::unordered_set<std::size_t> hash_set_;
};

using Log = MetaLog<deflog>;
#endif
