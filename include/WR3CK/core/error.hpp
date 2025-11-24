#pragma once
#include <stdexcept>

namespace WR3CK::Internal
{
class Error : public std::exception {
public:
	Error(const char* format, va_list args);
	Error(const Error& other) = delete;
	~Error();

	const char* what() const throw() override;

private:
	char m_msg[512];
};

__attribute__((cold, noreturn)) void throwError(const char* format...);
void messageBox(const char* caption, const char* format...);
}
