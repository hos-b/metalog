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

namespace token_types
{
	static constexpr unsigned int int_token = 0b0001;
	static constexpr unsigned int float_token = 0b0010;
	static constexpr unsigned int double_token = 0b0011;
	static constexpr unsigned int string_token = 0b0100;
	static constexpr unsigned int id_token = 0b0101;
	static constexpr unsigned int char_token = 0b0110;
}

namespace {
void PlaceHolder() {
	(void)output::err;
	(void)output::out;
	(void)output::logfile;
	(void)token_types::int_token;
	(void)token_types::float_token;
	(void)token_types::double_token;
	(void)token_types::string_token;
	(void)token_types::id_token;
	(void)token_types::char_token;
}

}
#endif