#pragma once

#include <cstdint>
#include <cstdarg>
#include <string>

namespace StringUtils {

std::string vaFormatString(const char* format, va_list argumentList);

std::basic_string<wchar_t> vaFormatString(const wchar_t* const format, va_list argumentList);

std::string createFormatted(const char* const format, ...);

std::basic_string<wchar_t> createFormatted(const wchar_t* const format, ...);

std::string& toLower(std::string& str);

std::string toLower(const std::string& str);

} // namespace StringUtils