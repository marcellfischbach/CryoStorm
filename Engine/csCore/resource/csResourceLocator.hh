#pragma once

#include <csCore/csCoreExport.hh>
#include <string>

namespace cs
{

class CS_CORE_API csResourceLocator
{
public:
	explicit csResourceLocator(const std::string &encoded);
	csResourceLocator(const csResourceLocator &parent, const std::string &encoded);
	csResourceLocator(const csResourceLocator *parent, const std::string &encoded);

  const std::string& GetArchive() const;
	const std::string& GetLocator() const;
	const std::string& GetPath() const;
	const std::string& GetFilename() const;
	const std::string& GetExtension() const;
	const std::string& Encoded() const;

	bool operator<(const csResourceLocator& locator) const;
	bool operator==(const csResourceLocator& locator) const;

private:
	std::string m_locator;

  std::string m_archive;
	std::string m_path;
	std::string m_filename;
	std::string m_extension;
	std::string m_encoded;
};

}
