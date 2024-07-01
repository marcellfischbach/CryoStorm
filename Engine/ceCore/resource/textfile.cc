//
// Created by Marcell on 01.07.2024.
//

#include <ceCore/resource/textfile.hh>

namespace ce
{


void TextFile::SetContent(const std::string &content)
{
  m_content = content;
}

const std::string &TextFile::GetContent() const
{
  return m_content;
}


} // ce