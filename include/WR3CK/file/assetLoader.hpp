#pragma once
#include <WR3CK/core/core.hpp>

#include <WR3CK/file/assetPackage.hpp>
#include <WR3CK/asset/plaintext.hpp>
#include <WR3CK/asset/texture.hpp>
#include <WR3CK/asset/shader.hpp>

namespace WR3CK
{
class AssetLoader {
public:
	static const PlaintextHandle getPlaintext(const std::string& path);
	static const ShaderHandle getShader(const std::string& path);
	static const TextureHandle getTexture(const std::string& path);

private:
	AssetLoader();
	AssetLoader(const AssetLoader& other) = delete;
	~AssetLoader() = default;

	void* const loadFile(const std::string& path, Internal::AssetPackage::DataType type) const;

	static AssetLoader s_instance;

	struct FileInformation {
		const Internal::AssetPackage::Info* m_info;
		const void* m_data;
	};
	std::map<std::string, FileInformation> m_fileInformation;

	std::map<std::string, PlaintextHandle> m_loadedPlaintext;
	std::map<std::string, TextureHandle> m_loadedTexture;
	std::map<std::string, ShaderHandle> m_loadedShader;
};
}