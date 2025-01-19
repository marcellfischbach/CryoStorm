
#include <csCore/csClass.hh>

// ---------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------


namespace cs
{

const cs::csClass* iObject::GetClass() const
{
  return cs::iObjectClass::Get();
}

const cs::csClass* iObject::GetStaticClass()
{
  return cs::iObjectClass::Get();
}

void* iObject::QueryClass(const csClass* clazz)
{
  if (clazz == iObjectClass::Get())
  {
    return static_cast<iObject*>(this);
  }
  return nullptr;
}


const void* iObject::QueryClass(const csClass* clazz) const
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
  : csClass("cs::iObject")
{
}

cs::iObject* iObjectClass::CreateInstance() const
{
  throw cs::csInstantiationException("cs::iObject");
}



// ---------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------

csClass::csClass(const std::string& name)
  : m_name(name)
{

}

const std::string& csClass::GetName() const
{
  return m_name;
}

bool csClass::IsInstanceOf(const csClass* clazz) const
{
  if (clazz == this)
  {
    return true;
  }

  for (const csClass * parent : m_superClasses)
  {
    if (parent->IsInstanceOf(clazz))
    {
      return true;
    }
  }
  return false;
}

bool csClass::IsAssignableFrom(const csClass* clazz) const
{
  return clazz->IsInstanceOf(this);
}

void csClass::AddSuperClass(const csClass* parentClass)
{
  m_superClasses.push_back(parentClass);
}

void csClass::AddProperty(const csProperty* prop)
{
  m_properties.push_back(prop);
}

void csClass::AddFunction(const csFunction* function)
{
  m_functions.push_back(function);
}

size_t csClass::GetNumberOfProperties() const
{
  return m_properties.size();
}

const csProperty* csClass::GetProperty(size_t idx) const
{
  return m_properties[idx];
}

const csProperty* csClass::GetProperty(const std::string& propName) const
{
  for (const csProperty * prop : m_properties)
  {
    if (prop->GetName() == propName)
    {
      return prop;
    }
  }
  return nullptr;
}

size_t csClass::GetNumberOfFunctions() const
{
  return m_functions.size();
}

const csFunction* csClass::GetFunction(size_t idx) const
{
  if (idx >= m_functions.size())
  {
    return nullptr;
  }
  return m_functions[idx];
}

std::vector<const csFunction*> csClass::GetFunction(const std::string& functionName) const
{
  std::vector<const csFunction*> result;
  for (const csFunction * function : m_functions)
  {
    if (function->GetName() == functionName)
    {
      result.push_back(function);
    }
  }
  return result;
}


std::vector<const csFunction*> csClass::GetFunction(const std::string& functionName, eConstness constness) const
{
  std::vector<const csFunction*> result;
  for (const csFunction * function : m_functions)
  {
    if (function->GetName() == functionName && function->GetConstness() == constness)
    {
      result.push_back(function);
    }
  }
  return result;
}



const csFunction* csClass::GetFirstFunction(const std::string& functionName) const
{
  for (const csFunction * function : m_functions)
  {
    if (function->GetName() == functionName)
    {
      return function;
    }
  }
  throw csNoSuchMethodException(functionName);
}


const csFunction* csClass::GetFirstFunction(const std::string& functionName, eConstness constness) const
{
  for (const csFunction * function : m_functions)
  {
    if (function->GetName() == functionName && function->GetConstness() == constness)
    {
      return function;
    }
  }
  throw csNoSuchMethodException(functionName);
}

size_t csClass::GetNumberOfSuperClasses() const
{
  return m_superClasses.size();
}

const csClass* csClass::GetSuperClass(size_t idx) const
{
  return m_superClasses[idx];
}

void csClass::AddMeta(const std::string& key, const std::string& value)
{
  m_meta[key] = value;
}

bool csClass::HasMeta(const std::string& meta) const
{
  return m_meta.find(meta) != m_meta.end();
}

const std::string csClass::GetMeta(const std::string& meta) const
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

csProperty::csProperty(const csValueDeclaration& containerDecl, const std::string& name, const csValueDeclaration& decl)
  : m_name(name)
  , m_containerDecl(containerDecl)
  , m_decl(decl)
{

}

csProperty::~csProperty()
{

}

bool csProperty::IsContainer() const
{
  return !m_containerDecl.GetType().empty();
}

const csValueDeclaration& csProperty::GetDecl() const
{
  return m_decl;
}

const csValueDeclaration& csProperty::GetContainerDecl() const
{
  return m_containerDecl;
}

const std::string& csProperty::GetName() const
{
  return m_name;
}

void csProperty::SetProperty(const std::string& key, const std::string& value)
{
  m_properties[key] = value;
}

std::set<std::string> csProperty::GetProperties() const
{
  std::set<std::string> res;
  for (auto const& it : m_properties)
  {
    res.insert(it.first);
  }
  return res;
}

bool csProperty::HasProperty(const std::string& property) const
{
  return m_properties.find(property) != m_properties.end();
}

std::string csProperty::GetProperty(const std::string& property) const
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

csValueDeclaration::csValueDeclaration(eConstness constness, const std::string& type, eValueMemoryMode mode)
  : m_constness(constness)
  , m_type(type)
  , m_mode(mode)
{

}

eConstness  csValueDeclaration::GetConstness() const
{
  return m_constness;
}

const std::string& csValueDeclaration::GetType() const
{
  return m_type;
}

eValueMemoryMode csValueDeclaration::GetMode() const
{
  return m_mode;
}

bool csValueDeclaration::operator==(const csValueDeclaration& other) const
{
  return m_constness == other.m_constness &&
    m_type == other.m_type &&
    m_mode == other.m_mode;

}

// ---------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------
csFunctionAttribute::csFunctionAttribute(const csValueDeclaration& type, const std::string& name)
  : m_type(type)
  , m_name(name)
{
}

const csValueDeclaration& csFunctionAttribute::GetType() const
{
  return m_type;
}

const std::string& csFunctionAttribute::GetName() const
{
  return m_name;
}

// ---------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------

csFunction::csFunction(eFunctionVirtuality virtuality, const csValueDeclaration& returnType, const std::string& name, eConstness constness)
  : m_virtuality(virtuality)
  , m_constness(constness)
  , m_name(name)
  , m_returnType(returnType)
{

}

void csFunction::AddAttribute(const csFunctionAttribute& attribute)
{
  m_attributes.push_back(attribute);
}

size_t csFunction::GetNumberOfAttributes() const
{
  return m_attributes.size();
}

const csFunctionAttribute& csFunction::GetAttribute(size_t idx) const
{
  if (idx >= m_attributes.size())
  {
    return m_invalid;
  }

  return m_attributes[idx];
}

const std::string& csFunction::GetName() const
{
  return m_name;
}

const csValueDeclaration& csFunction::GetReturnType() const
{
  return m_returnType;
}

eFunctionVirtuality csFunction::GetVirtuality() const
{
  return m_virtuality;
}

eConstness csFunction::GetConstness() const
{
  return m_constness;
}


csClass::~csClass()
{
}

}
