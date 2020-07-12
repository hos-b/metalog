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
    DoubleStreamWriter(_FST* fst, _OST* ost) {
        fst_ = fst;
        ost_ = ost;
    }
public:
    static void Write(const std::string& str) {
        (*fst_) << str;
        (*ost_) << str;
    }

private:
    static _FST* fst_;
    static _OST* ost_;
};

template <>
class DoubleStreamWriter <std::nullptr_t, std::ostream> {
public:
    DoubleStreamWriter(std::nullptr_t fst, std::ostream* ost) {
        ost_ = ost;
    }
    static void Write(const std::string& str) {
        (*ost_) << str;
    }

private:
    static std::ostream* ost_;
};

template <>
class DoubleStreamWriter <std::fstream, std::nullptr_t> {
public:
    DoubleStreamWriter(std::fstream* fst, std::nullptr_t ost) {
        fst_ = fst;
    }
    static void Write(const std::string& str) {
        (*fst_) << str;
    }

private:
    static std::fstream* fst_;
};

template <>
class DoubleStreamWriter <std::nullptr_t, std::nullptr_t> {
public:
    DoubleStreamWriter(std::nullptr_t fst, std::nullptr_t ost) {
        std::cout << ">>>>>> METALOG: cannot log anything with those settings " << std::endl;
    }
    static void Write(const std::string& str) {
    }
};


#endif