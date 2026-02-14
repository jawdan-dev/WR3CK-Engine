#include <cstdlib>
#include <exception>

#include <packager.hpp>

using namespace WR3CK;

int main(const int argc, const char** argv) {
	try {
		// Package Assets.
		Packager packager;
		for (int i = 2; i < argc; i++) {
			packager.addPath(argv[i]);
		}

		// Create package file.
		WR3CK_ASSERT(argc > 1, "No output file path supplied.");
		const char* packageFile = argv[1];
		packager.saveToFile(packageFile);

		// Compression information.
		size_t totalSizeUncompressed = 0, totalSizeCompressed = 0;
		for (const auto& it : packager.fileDataMap()) {
			totalSizeUncompressed += it.second.dataSizeUncompressed();
			totalSizeCompressed += it.second.dataSizeCompressed();
		}
		const float compressionRatio = static_cast<float>(totalSizeCompressed) / static_cast<float>(totalSizeUncompressed);
		WR3CK_LOG("- Successfully packed %zu asset files to %2.2f%% of original packaged size.\n", packager.fileDataMap().size(), compressionRatio * 100.0f);
	} catch (const std::exception& exception) {
		WR3CK_LOG_ERROR("WR3CK Asset Packager encountered an error: %s\n", exception.what());
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}