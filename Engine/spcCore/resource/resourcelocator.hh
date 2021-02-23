#pragma once

#include <spcCore/coreexport.hh>
#include <string>

namespace spc
{

class SPC_CORE_API ResourceLocator
{
public:
	ResourceLocator(const std::string &encoded);
	ResourceLocator(const ResourceLocator &parent, const std::string &encoded);
	ResourceLocator(const ResourceLocator *parent, const std::string &encoded);

	const std::string& GetLocator() const;

	const std::string& GetProto() const;
	const std::string& GetPath() const;
	const std::string& GetFilename() const;
	const std::string& GetExtension() const;
	const std::string& Encoded() const;

	bool operator<(const ResourceLocator& locator) const;
	bool operator==(const ResourceLocator& locator) const;

private:
	std::string m_locator;


	std::string m_proto;
	std::string m_path;
	std::string m_filename;
	std::string m_extension;
	std::string m_encoded;
};

}
