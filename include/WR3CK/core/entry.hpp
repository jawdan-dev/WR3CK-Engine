#pragma once
#include <WR3CK/core/core.hpp>

#include <WR3CK/application/application.hpp>

#define WR3CK_ENTRY()                                                                     \
	int main() {                                                                          \
		WR3CK_LOG(" \n"                                                                   \
				  "I want to take you to the planetarium\n"                               \
				  "I want to show you how ugly the sky is\n");                            \
		try {                                                                             \
			WR3CK::Application app;                                                       \
			WR3CK::Application::initializeApplication(app);                               \
			app.start();                                                                  \
		} catch (const std::exception& exception) {                                       \
			WR3CK_LOG_ERROR("WR3CK Engine : Exception Caught\n\t%s\n", exception.what()); \
			WR3CK_MESSAGEBOX("WR3CK Engine : Exception Caught", exception.what());        \
		}                                                                                 \
		return EXIT_SUCCESS;                                                              \
	}                                                                                     \
	void WR3CK::Application::initializeApplication(Application& app)
