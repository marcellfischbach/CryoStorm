
#include <csXml/csXml.hh>
#include <Windows.h>


using namespace cryo;

int64_t tick()
{
  LARGE_INTEGER c, f;
  if (QueryPerformanceCounter(&c) && QueryPerformanceFrequency(&f))
  {
    c.QuadPart *= 1000000;
    c.QuadPart /= f.QuadPart;
    c.QuadPart /= 1000;
    return c.QuadPart;
  }

  return GetTickCount64();
}



int main(int argc, char **argv)
{
  if (argc != 2)
  {
    return -1;
  }

  try
  {
    int64_t start = tick();
    xml::csDocument *doc = xml::csParser::ParseFilename(std::string(argv[1]));
    int64_t end = tick();

    printf ("Read in %lldms", end - start);

    delete doc;
  }
  catch (const xml::csParseException &e)
  {
    printf ("exc: %s @ %zu %zu\n", e.what(), e.GetLine() +1 , e.GetColumn() +1);
  }

}