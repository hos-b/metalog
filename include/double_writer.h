#ifndef __METALOG_DOUBLE_WRITER_H__
#define __METALOG_DOUBLE_WRITER_H__

#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>

/* 
	templated writer to simultaneously write to an output stream (cout or cerr)
	and a file stream. all of this just to avoid an if statement
*/

template <typename _FST, typename _OST>
class DoubleStreamWriter {
public:
	DoubleStreamWriter(_FST fst, _OST ost) {
		fst_ = fst;
		ost_ = ost;
	}
	template <typename WT>
	void Write(WT obj) {
		(*fst_) << obj;
		(*ost_) << obj;
	}
	void Flush() {
		fst_->flush();
		ost_->flush();
	}

private:
	_FST fst_;
	_OST ost_;
};

template <>
class DoubleStreamWriter <std::nullptr_t, std::ostream*> {
public:
	DoubleStreamWriter(std::nullptr_t fst, std::ostream* ost) {
		(void)fst;
		ost_ = ost;
	}
	template <typename WT>
	void Write(WT obj) {
		(*ost_) << obj;
	}
	void Flush() {
		ost_->flush();
	}

private:
	std::ostream* ost_;
};

template <>
class DoubleStreamWriter <std::ofstream*, std::nullptr_t> {
public:
	DoubleStreamWriter(std::ofstream* fst, std::nullptr_t ost) {
		(void)ost;
		fst_ = fst;
	}
	template <typename WT>
	void Write(WT obj) {
		(*fst_) << obj;
	}
	void Flush() {
		fst_->flush();	
	}

private:
	static std::ofstream* fst_;
};

template <>
class DoubleStreamWriter <std::nullptr_t, std::nullptr_t> {
public:
	DoubleStreamWriter(std::nullptr_t fst, std::nullptr_t ost) {
		(void)fst;
		(void)ost;
		std::cout << ">>> cannot log anything with those settings" << std::endl;
	}
	template <typename WT>
	void Write(WT obj) {
		(void)obj;
	}
	void Flush() {

	}
};


#endif