#include <cstdlib>

#include <WR3CK/WR3CK.hpp>

int main() {
	WR3CK_LOG(" \n"
			  "I want to take you to the planetarium\n"
			  "I want to show you how ugly the sky is\n");

	try {
		WR3CK::Application application;
		WR3CK::Application::initializeApplication(application);
		application.start();
	} catch (const std::exception& exception) {
		WR3CK_LOG_ERROR("WR3CK Engine : Exception Caught\n\t%s\n", exception.what());
		WR3CK_MESSAGEBOX("WR3CK Engine : Exception Caught", exception.what());
	}

	return EXIT_SUCCESS;
}