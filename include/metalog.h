
#ifndef __META_LOG_H__
#define __META_LOG_H__

#include <ctime>
#include <chrono>
#include <vector>
#include <unordered_map>
#include <type_traits>
#include <functional>

#include "types.h"
#include "defaults.h"
#include "double_writer.h"

#define CAST(x) static_cast<DoubleStreamWriter<decltype(fout_), decltype(dout_)>*>(x)
// #define CAST(x) x

template <typename T>
class MetaLog
{
public:
	// default constuctor
	constexpr MetaLog() {
		dout_ = &std::cout;
		fout_ = nullptr;
		dwriter_ = new DoubleStreamWriter<decltype(nullptr), decltype(dout_)>(nullptr, dout_);
		static_cast<DoubleStreamWriter<decltype(fout_), decltype(dout_)>*>(dwriter_)->Write("hi");
		max_log_level_ = static_cast<typename T::type>(0);
		static_assert(std::is_convertible<typename T::type, int>::value, "selected log type enum should be convertible to int");
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
			(*fout_) << "\n>>> new log started: " << ctime(&now) << "\n";
		} else {
			fout_ = nullptr;
		}
		delete dwriter_;
		dwriter_ = new DoubleStreamWriter<decltype(fout_), decltype(dout_)>(fout_, dout_);
	}
	void SetMaximumLogLevel(typename T::type max_log_level) {
		max_log_level_ = max_log_level;
	}
private:
	// base printer
	void printf(const char *s) {
		while (*s) {
			if (*s == '%') {
				if (*(s + 1) != '%')
					++s;
				else
					throw std::runtime_error("invalid format string: missing arguments");
			}
			CAST(dwriter_)->Write(*s++);
		}
	}
	// recursive printer
	template<typename _PT, typename... _PArgs>
	void printf(const char *s, _PT value, _PArgs... args) {
		while (*s) {
			if (*s == '%') {
				if (*(s + 1) != '%') {
					CAST(dwriter_)->Write(value);
					s += 2; // only works on 2-character format strings ( %d, %f, etc ); fails with %5.4f
					printf(s, args...); // called even when *s is 0 but does nothing in that case (and ignores extra arguments)
					return;
				}
				++s;
			}
			std::cout << *s++;
		}    
	}
	// recursive hashing
	template <typename _HT>
	size_t GetVariadicHash(_HT arg) {
		return std::hash<_HT>()(arg);
	}
	template <typename _HT, typename... VH>
	size_t GetVariadicHash(_HT arg, VH... args) {
		return std::hash<_HT>()(arg) ^ GetVariadicHash(args...);
	}

public:
	template<typename... VA>
	void Log(typename T::type type, const std::string_view& format, VA... args) {
		if (type >= max_log_level_)
			printf(format.data(), args...);
	}
	template<typename... VA>
	void TimedLog(typename T::type type, size_t id, long msec, const std::string& format, VA... args) {
		static std::vector<std::chrono::steady_clock::time_point> latest;
		if (id + 1 > latest.size()) {
			latest.resize(id + 1);
			latest[id] = std::chrono::steady_clock::now();
			if (type >= max_log_level_)
				printf(format.data(), args...);
		} else {
			auto now = std::chrono::steady_clock::now();
			if (std::chrono::duration_cast<std::chrono::microseconds>(now - latest[id]).count() >= msec) {
				if (type >= max_log_level_) {
					latest[id] = now;
					printf(format.data(), args...);
				}
			}
		}	
	}

	template<typename... ID, typename... VA>
	void UniqueLog(ID... keys, typename T::type type, const std::string_view& format, VA... variables) {
		static std::unordered_map<std::size_t, std::size_t> hashmap;
		auto collective_hash_k = GetVariadicHash(keys..., std::string(format.data()));
		auto collective_hash_v = GetVariadicHash(variables..., std::string(format.data()));
		auto params = std::make_tuple(keys..., variables...);
		auto iter = hashmap.find(collective_hash_k);
		if (iter == hashmap.end()) {
			if (type >= max_log_level_) {
				hashmap.insert({collective_hash_k, collective_hash_v});
				std::invoke(&MetaLog<T>::printf, *this, format, params);
			}
		} else {
			if (iter->second != collective_hash_v) {
				if (type >= max_log_level_) {
					iter->second = collective_hash_v;
					std::invoke(&MetaLog<T>::printf, *this, format, params);
				}
			}
		}
	}

private:
	typename T::type max_log_level_;
	std::ostream* dout_;
	std::ofstream* fout_;
	DoubleStreamWriter<std::ofstream*, std::ostream*>* dwriter_;
};

using Log = MetaLog<deflog>;
#endif
