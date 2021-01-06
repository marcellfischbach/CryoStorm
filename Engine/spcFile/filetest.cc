

#include <spcFile/file.hh>

int main(int argc, char** argv)
{
  spc::file::File file;
  if (file.Parse("d:\\Dev\\SpiceEngine\\data\\test.spc"))
  {
    file.Debug();
    if (file.GetDataSize())
    {
      char* data = new char[file.GetDataSize() + 1];
      memcpy(data, file.GetData(), file.GetDataSize());
      data[file.GetDataSize()] = '\0';
      printf("Data:\n-----\n%s-----\n", data);


      FILE* out = fopen("d:\\Dev\\SpiceEngine\\data\\test_out_2.spc", "wb");
      std::string v = file.Print(true, 4);
      fwrite(v.c_str(), v.length(), 1, out);
      fclose(out);
    }

  }

  return 0;
}