//
// Created by MCEL on 29.11.2024.
//

#pragma once

#include <string>

class Project
{
public:
  static Project* Get ();

  bool Open(const std::string &projectFolder);


private:

};
