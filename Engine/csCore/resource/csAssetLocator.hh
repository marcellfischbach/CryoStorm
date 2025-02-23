#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csDefs.hh>
#include <string>

namespace cs
{

class CS_CORE_API csAssetLocator
{
public:
	csAssetLocator(const std::string &locator);
	csAssetLocator(const csAssetLocator &parent, const std::string &locator);

  CS_NODISCARD const std::string& GetArchive() const;
  CS_NODISCARD const std::string& GetPath() const;
  CS_NODISCARD const std::string& GetFilename() const;
  CS_NODISCARD const std::string& GetExtension() const;
  CS_NODISCARD const std::string& Encoded() const;
  CS_NODISCARD const std::string& Canonical() const;
  CS_NODISCARD const std::string& GetSubAssetName() const;

  csAssetLocator AsAnonymous() const;
  csAssetLocator AsRootLocator() const;

	bool operator<(const csAssetLocator& locator) const;
	bool operator==(const csAssetLocator& locator) const;

private:



  /**
   * \brief Select a specific archive for opening the file.
   *
   * This is optional an may be empty. If this is set, the csVFS will open the file in this archive. If there is no
   * such file in the archive, than nothing will be opened. No other archive is checked.
   */

  std::string m_archive;

  /**
   * \brief The path of the file. Does not include the filename or the archive
   */
	std::string m_path;

  /**
   * \brief Just the filename with the extension. No path
   */
	std::string m_filename;

  /**
   * \brief The extension of the file in UPPER CASE.
   */
	std::string m_extension;

  /**
   * \brief An archive descriptor that will hold all information
   *
   * This contains an optional archive, that path, the filename and the extension. If this locator was constructed
   * with a parent locator that this encoded locator will also contain the complete path of the parent.
   */
	std::string m_encoded;

  /**
   * \brief A clean up representation of m_encoded.
   *
   * The normalized locator will contain all information of the m_encoded locator but it will be cleaned up. That means
   * there is no folder '.' or '..' in the normalized locator.
   */
  std::string m_canonical;

  /**
   * \brief Request just a single sub part of the asset.
   *
   * The real asset (e.g. the file in an archive) is still the one defined by m_encoded or m_canonical. This
   * subAssetName is queried from the iAsset object once the object is loaded.
   */
  std::string m_subAssetName;
};

}

