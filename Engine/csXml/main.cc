
#include <csXml/csXml.hh>


using namespace cryo;


int main(int argc, char **argv)
{
  if (argc != 2)
  {
    return -1;
  }

  xml::csDocument doc = xml::csParser::ParseFilename(std::string(argv[1]));


}