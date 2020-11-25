
#include <spcCore/class.hh>

// ---------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------


namespace spc
{


iObject::iObject()
{

}

iObject::~iObject()
{

}

const spc::Class* iObject::GetClass() const
{
  return spc::iObjectClass::Get();
}

const spc::Class* iObject::GetStaticClass()
{
  return spc::iObjectClass::Get();
}

void* iObject::QueryClass(const Class* clazz)
{
  if (clazz == iObjectClass::Get())
  {
    return static_cast<iObject*>(this);
  }
  return nullptr;
}


const void* iObject::QueryClass(const Class* clazz) const
{
  if (clazz == iObjectClass::Get())
  {
    return static_cast<const iObject*>(this);
  }
  return nullptr;
}


iObjectClass* iObjectClass::Get()
{
  static iObjectClass static_class;
  return &static_class;
}

iObjectClass::iObjectClass()
  : Class("spc::iObject")
{
}

spc::iObject* iObjectClass::CreateInstance() const
{
  throw spc::InstanciationException("spc::iObject");
}

Object::Object()
  : spc::iObject()
{
  SPC_CLASS_GEN_CONSTR;
}

Object::~Object()
{

}


// ---------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------

Class::Class(const std::string& name)
  : m_name(name)
{

}

const std::string& Class::GetName() const
{
  return m_name;
}

bool Class::IsInstanceOf(const Class* clazz) const
{
  if (clazz == this)
  {
    return true;
  }

  for (const Class* parent : m_superClasses)
  {
    if (parent->IsInstanceOf(clazz))
    {
      return true;
    }
  }
  return false;
}

bool Class::IsAssignableFrom(const Class* clazz) const
{
  return clazz->IsInstanceOf(this);
}

void Class::AddSuperClass(const Class* parentClass)
{
  m_superClasses.push_back(parentClass);
}

void Class::AddProperty(const Property* prop)
{
  m_properties.push_back(prop);
}

void Class::AddFunction(const Function* function)
{
  m_functions.push_back(function);
}

size_t Class::GetNumberOfProperties() const
{
  return m_properties.size();
}

const Property* Class::GetProperty(size_t idx) const
{
  return m_properties[idx];
}

const Property* Class::GetProperty(const std::string& propName) const
{
  for (const Property* prop : m_properties)
  {
    if (prop->GetName() == propName)
    {
      return prop;
    }
  }
  return nullptr;
}

size_t Class::GetNumberOfFunctions() const
{
  return m_functions.size();
}

const Function* Class::GetFunction(size_t idx) const
{
  if (idx >= m_functions.size())
  {
    return nullptr;
  }
  return m_functions[idx];
}

std::vector<const Function*> Class::GetFunction(const std::string& functionName) const
{
  std::vector<const Function*> result;
  for (const Function* function : m_functions)
  {
    if (function->GetName() == functionName)
    {
      result.push_back(function);
    }
  }
  return result;
}


std::vector<const Function*> Class::GetFunction(const std::string& functionName, eConstness constness) const
{
  std::vector<const Function*> result;
  for (const Function* function : m_functions)
  {
    if (function->GetName() == functionName && function->GetConstness() == constness)
    {
      result.push_back(function);
    }
  }
  return result;
}



const Function* Class::GetFirstFunction(const std::string& functionName) const
{
  for (const Function* function : m_functions)
  {
    if (function->GetName() == functionName)
    {
      return function;
    }
  }
  throw NoSuchMethodException(functionName);
}


const Function* Class::GetFirstFunction(const std::string& functionName, eConstness constness) const
{
  for (const Function* function : m_functions)
  {
    if (function->GetName() == functionName && function->GetConstness() == constness)
    {
      return function;
    }
  }
  throw NoSuchMethodException(functionName);
}

size_t Class::GetNumberOfSuperClasses() const
{
  return m_superClasses.size();
}

const Class* Class::GetSuperClass(size_t idx) const
{
  return m_superClasses[idx];
}

void Class::AddMeta(const std::string& key, const std::string& value)
{
  m_meta[key] = value;
}

bool Class::HasMeta(const std::string& meta) const
{
  return m_meta.find(meta) != m_meta.end();
}

const std::string Class::GetMeta(const std::string& meta) const
{
  auto it = m_meta.find(meta);
  if (it != m_meta.end())
  {
    return it->second;
  }
  return std::string();
}


// ---------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------

Property::Property(const ValueDeclaration& containerDecl, const std::string& name, const ValueDeclaration& decl)
  : m_name(name)
  , m_containerDecl(containerDecl)
  , m_decl(decl)
{

}

Property::~Property()
{

}

bool Property::IsContainer() const
{
  return !m_containerDecl.GetType().empty();
}

const ValueDeclaration& Property::GetDecl() const
{
  return m_decl;
}

const ValueDeclaration& Property::GetContainerDecl() const
{
  return m_containerDecl;
}

const std::string& Property::GetName() const
{
  return m_name;
}

void Property::SetProperty(const std::string& key, const std::string& value)
{
  m_properties[key] = value;
}

std::set<std::string> Property::GetProperties() const
{
  std::set<std::string> res;
  for (auto const& it : m_properties)
  {
    res.insert(it.first);
  }
  return res;
}

bool Property::HasProperty(const std::string& property) const
{
  return m_properties.find(property) != m_properties.end();
}

std::string Property::GetProperty(const std::string& property) const
{
  std::map<std::string, std::string>::const_iterator it = m_properties.find(property);
  if (it != m_properties.end())
  {
    return it->second;
  }
  return "";
}

// ---------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------

ValueDeclaration::ValueDeclaration(eConstness constness, const std::string& type, eValueMemoryMode mode)
  : m_constness(constness)
  , m_type(type)
  , m_mode(mode)
{

}

eConstness  ValueDeclaration::GetConstness() const
{
  return m_constness;
}

const std::string& ValueDeclaration::GetType() const
{
  return m_type;
}

eValueMemoryMode ValueDeclaration::GetMode() const
{
  return m_mode;
}

bool ValueDeclaration::operator==(const ValueDeclaration& other) const
{
  return m_constness == other.m_constness &&
    m_type == other.m_type &&
    m_mode == other.m_mode;

}

// ---------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------
FunctionAttribute::FunctionAttribute(const ValueDeclaration& type, const std::string& name)
  : m_type(type)
  , m_name(name)
{
}

const ValueDeclaration& FunctionAttribute::GetType() const
{
  return m_type;
}

const std::string& FunctionAttribute::GetName() const
{
  return m_name;
}

// ---------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------

Function::Function(eFunctionVirtuality virtuality, const ValueDeclaration& returnType, const std::string& name, eConstness constness)
  : m_virtuality(virtuality)
  , m_constness(constness)
  , m_name(name)
  , m_returnType(returnType)
{

}

void Function::AddAttribute(const FunctionAttribute& attribute)
{
  m_attributes.push_back(attribute);
}

size_t Function::GetNumberOfAttributes() const
{
  return m_attributes.size();
}

const FunctionAttribute& Function::GetAttribute(size_t idx) const
{
  if (idx >= m_attributes.size())
  {
    return m_invalid;
  }

  return m_attributes[idx];
}

const std::string& Function::GetName() const
{
  return m_name;
}

const ValueDeclaration& Function::GetReturnType() const
{
  return m_returnType;
}

eFunctionVirtuality Function::GetVirtuality() const
{
  return m_virtuality;
}

eConstness Function::GetConstness() const
{
  return m_constness;
}


Class::~Class()
{

}

}
