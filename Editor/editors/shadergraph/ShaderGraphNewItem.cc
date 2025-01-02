//
// Created by Marcell on 25.12.2024.
//

#include <editors/shadergraph/ShaderGraphNewItem.hh>
#include <fstream>


std::string &ShaderGraphNewItem::GetName() const
{
  static std::string name = "Shader Graph";
  return name;
}


void ShaderGraphNewItem::Create(const std::fs::path &path, const std::string &itemName)
{

  // Create the shader graph file
  std::fs::path fileName = std::fs::path(path) / std::fs::path(itemName + ".shadergraph");
  std::ofstream file(fileName);

  file << R"(

shaderGraph {

    receiveShadow 1,
    queue Default,
    lighting Default,
    blending Off,

    nodes {

        node "cs::csSGConstColor3" colorDiffuse x: -200 y: 0 {
            value 0 1.0,
            value 1 1.0,
            value 2 1.0,
        },



        shaderGraph x: 0 y: 0 {
            binding "Diffuse" colorDiffuse,
        }
    },

    attributes {
    },

},
  )";
  file.close();
}