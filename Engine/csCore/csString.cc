
#include <csCore/csString.hh>


namespace cs
{

std::string to_lower(const std::string& str)
{
  std::string res(str);
  for (size_t i = 0, in = res.length(); i < in; i++)
  {
    res[i] = std::tolower(res[i]);
  }
  return res;
}

std::string to_upper(const std::string& str)
{
  std::string res(str);
  for (size_t i = 0, in = res.length(); i < in; i++)
  {
    res[i] = std::toupper(res[i]);
  }


  return res;

}

}