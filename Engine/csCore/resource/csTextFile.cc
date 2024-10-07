//
// Created by Marcell on 01.07.2024.
//

#include <csCore/resource/csTextFile.hh>

namespace cs
{


void csTextFile::SetContent(const std::string &content)
{
  m_content = content;
}

const std::string &csTextFile::GetContent() const
{
  return m_content;
}


} // ce