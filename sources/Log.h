#pragma once

namespace tinygles {

class Logger final {
public:
	enum Severity {
		Debug = 0,
		Verbose,
		Information,
		Warning,
		Error,
		Critical,
	};
public:
	Logger();
	~Logger();

	void operator()(Severity severity, const char* const formatString, ...);

	void operator()(const char* const formatString, ...);
};

} // namespace tinygles

extern tinygles::Logger Log;