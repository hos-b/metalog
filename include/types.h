#ifndef __METALOG_TYPES_H__
#define __METALOG_TYPES_H__

using outstream = unsigned char;
// the output of the log
namespace output
{
	static constexpr outstream err = 0b001;
	static constexpr outstream out = 0b010;
	static constexpr outstream logfile = 0b100;
};


#endif