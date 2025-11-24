#include <WR3CK/core/error.hpp>

#include <cstdarg>

#ifdef __WIN32
#	include <windows.h>
#endif

namespace WR3CK::Internal
{
Error::Error(const char* format, va_list args) {
	vsnprintf(m_msg, sizeof(m_msg), format, args);
}
Error::~Error() {}

const char* Error::what() const throw() {
	return m_msg;
}

void throwError(const char* format...) {
	va_list args;
	va_start(args, format);
	throw Error(format, args);
}
void messageBox(const char* caption, const char* format...) {
	char message[512];
	va_list args;
	va_start(args, format);
	vsnprintf(message, sizeof(message), format, args);
	va_end(args);

#ifdef __WIN32
	MessageBox(NULL, message, caption, MB_OK);
#endif
}
}
