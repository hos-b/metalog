
#ifndef __METALOG_TOKENS_H__
#define __METALOG_TOKENS_H__

#include <string>
#include <string_view>
#include <vector>
#include <iostream>
#include <array>

namespace {
enum _tokenizer_state {
    TNORMAL,
    TPERC
};
}


std::vector<std::string_view> GetStringTokens(const std::string_view& signature) {
    std::vector<std::string_view> token_vector;
    _tokenizer_state state = TNORMAL;
    size_t idx_start = 0, idx = 0;
    while (true) {
        if (state == TNORMAL) {
            if (signature[idx] == '%') {
                std::cout << "% found at " << idx << std::endl;
                state = TPERC;
            } else if (signature[idx] == '\0') {
                std::cout << "str ends at " << idx;
                std::cout << ", making substr(" << idx_start << ", " << idx - idx_start << ")" << std::endl;
                std::cout << "substr = " << signature.substr(idx_start, idx - idx_start) << std::endl;
                token_vector.emplace_back(signature.substr(idx_start, idx - idx_start));
                break;
            }
            ++idx;
        } else {
            state = TNORMAL;
            switch (signature[idx]) {
            case 's':
            case 'c':
            case 'i':
            case 'd':
            case 'f':
            case '\0':
                std::cout << signature[idx] << " found after % at " << idx;
                std::cout << ", making substr(" << idx_start << ", " << idx - idx_start - 1 << ")" << std::endl;
                std::cout << "substr = " << signature.substr(idx_start, idx - idx_start - 1) << std::endl;
                token_vector.emplace_back(signature.substr(idx_start, idx - idx_start - 1));
                idx_start = idx + 1;
                break;
            default:
                break;
            }
            ++idx;
        }
    }

    return token_vector;
}

#endif