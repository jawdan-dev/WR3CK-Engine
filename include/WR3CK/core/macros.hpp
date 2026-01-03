#pragma once

#define ARG(...)	  __VA_ARGS__
#define _STRINGIFY(x) #x
#define STRINGIFY(x)  _STRINGIFY(x)

#define WR3CK_GETTER(type, name, variable) \
	inline const type name() { return variable; }
#define WR3CK_GETTER_CONST(type, name, variable) \
	inline const type name() const { return variable; }
#define WR3CK_GETTER_MUT(type, name, variable) \
	inline type name() { return variable; }    \
	WR3CK_GETTER_CONST(type, name, variable)
#define WR3CK_GETTER_CONST_MUT(type, name, variable) \
	inline type name() const { return variable; }

#define WR3CK_CLEANUP(ptr, freeCode) \
	if (ptr != nullptr) {            \
		freeCode;                    \
		ptr = nullptr;               \
	}
#define WR3CK_CLEANUP_GL(glObject, freeCode) \
	if (glObject > 0) {                      \
		freeCode;                            \
		glObject = 0;                        \
	}

// TODO: Colored print functions.
#define WR3CK_DEBUG			   printf("DEBUG @ " __FILE__ ":" STRINGIFY(__LINE__) "\n");
#define WR3CK_PRINT(...)	   printf(__VA_ARGS__)
#define WR3CK_LOG(...)		   WR3CK_PRINT(__VA_ARGS__)
#define WR3CK_LOG_WARNING(...) WR3CK_PRINT(__VA_ARGS__)
#define WR3CK_LOG_ERROR(...)   WR3CK_PRINT(__VA_ARGS__)

#include <WR3CK/core/error.hpp>
#define WR3CK_ERROR(...) \
	::WR3CK::Internal::throwError(__FILE__ ":" STRINGIFY(__LINE__) ": " __VA_ARGS__)
#define WR3CK_ASSERT(condition, ...) \
	if (!(condition)) [[unlikely]] { \
		WR3CK_ERROR(__VA_ARGS__);    \
	}
#define WR3CK_MESSAGEBOX(caption, msg) \
	::WR3CK::Internal::messageBox(caption, msg);
