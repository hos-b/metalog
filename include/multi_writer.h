#ifndef __METALOG_DOUBLE_WRITER_H__
#define __METALOG_DOUBLE_WRITER_H__

#include <cstddef>
#include <iostream>
#include <vector>
/* 
	templated writer to simultaneously write to an output stream (cout or cerr)
	and a file stream. all of this just to avoid an if statement
*/
void VariadicPushBack(std::vector<std::ostream*>& vec, std::ostream* stream) {
	if (stream != nullptr)
		vec.emplace_back(stream);
}
template <typename... F>
void VariadicPushBack(std::vector<std::ostream*>& vec, std::ostream* stream, F... args) {
	if (stream != nullptr)
		vec.emplace_back(stream);
	VariadicPushBack(vec, args...);
}

template <typename... STO>
class MultiStreamWriter {
public:
	MultiStreamWriter(STO ...args) {
		VariadicPushBack(streams_, args...);
	}
	template <typename WT>
	void Write(WT obj) {
		for (auto& writer : streams_) {
			(*writer) << obj;
		}
	}
	void Flush() {
		for (auto& writer : streams_) {
			writer->flush();
		}
	}

private:
	std::vector<std::ostream*> streams_;
};

#endif