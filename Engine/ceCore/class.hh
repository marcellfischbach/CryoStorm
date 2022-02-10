
#pragma once

#include <cstdint>
#include <ceCore/defs.hh>

//
// Meta information for CE_CLASS definitions
//  Virtual - Prevent the ceMOC from implementing CreateInstance 

#define CE_CLASS(...)
#define CE_SUPER(Cls) Cls
#define CE_PROPERTY(...)
#define CE_FUNCTION(...)
#define CE_CLASS_GEN public: \
    const ce::Class *GetClass () const override;\
    static const ce::Class *GetStaticClass (); \
    void *QueryClass(const ce::Class *clazz) override; \
    const void *QueryClass(const ce::Class *clazz) const override

#define CE_CLASS_GEN_OBJECT \
    CE_CLASS_GEN; \
    void AddRef() override \
    { \
      m_refCount++; \
    } \
    void Release() override  \
    { \
      --m_refCount; \
      if (m_refCount <= 0) \
      { \
          delete this;\
      } \
    } \
    CE_NODISCARD int64_t RefCount () const override \
    { \
      return m_refCount; \
    } \
    private: \
      int64_t m_refCount

#define CE_CLASS_GEN_CONSTR m_refCount = 1

#define CE_WEAK_OBJECT(Super) \
  public: void AddRef () { Super::AddRef (); }\
          void Release () { Super::Release (); }


#define CE_SET(oo, no) if (no) no->AddRef(); if (oo) oo->Release(); oo = no
#define CE_ADDREF(o) if (o) o->AddRef()
#define CE_RELEASE(o) if (o) o->Release()

#include <map>
#include <set>
#include <string>
#include <vector>
#include <stdarg.h>
#include <ceCore/coreexport.hh>
#include <exception>

namespace ce
{


class BadMethodInvokation : public std::exception
{
public:
  BadMethodInvokation(const std::string& msg)
    : m_msg(msg)
  {
  }
  virtual ~BadMethodInvokation()
  {

  }

  const char* getMessage() const
  {
    return m_msg.c_str();
  }
private:
  std::string m_msg;
};


class NoSuchMethodException : public std::exception
{
public:
  NoSuchMethodException(const std::string& methodName)
    : m_methodName(methodName)
  {
  }
  virtual ~NoSuchMethodException()
  {

  }

  const char* getMethodName() const
  {
    return m_methodName.c_str();
  }
private:
  std::string m_methodName;
};


class InstanciationException : public std::exception
{
public:
  InstanciationException(const std::string& className)
    : m_className(className)
  {
  }
  virtual ~InstanciationException()
  {

  }

  const char* GetClassName() const
  {
    return m_className.c_str();
  }
private:
  std::string m_className;
};


class Class;

struct CE_CORE_API iObject
{
  iObject();
  virtual ~iObject();
  virtual const Class* GetClass() const;
  static const Class* GetStaticClass();

  virtual void AddRef() = 0;

  virtual void Release() = 0;

  CE_NODISCARD virtual int64_t RefCount() const = 0;

  template<typename T>
  CE_NODISCARD T* Query()
  {
    return reinterpret_cast<T*>(QueryClass(T::GetStaticClass()));
  }

  CE_NODISCARD virtual void* QueryClass(const Class* clazz);

  template<typename T>
  CE_NODISCARD const T* Query() const
  {
    return reinterpret_cast<const T*>(QueryClass(T::GetStaticClass()));
  }
  CE_NODISCARD virtual const void* QueryClass(const Class* clazz) const;
};


class AutoRelease
{
private:
  iObject* obj;
public:
  AutoRelease(iObject* obj) : obj(obj) { }
  ~AutoRelease() { CE_RELEASE(obj); }

};


template<typename T>
T* QueryClass(ce::iObject* object)
{
  if (!object)
  {
    return 0;
  }
  return reinterpret_cast<T*>(object->QueryClass(T::GetStaticClass()));
}

template<typename T>
const T* QueryClass(const ce::iObject* object)
{
  if (!object)
  {
    return 0;
  }
  return reinterpret_cast<const T*>(object->QueryClass(T::GetStaticClass()));
}

enum eConstness
{
  eC_NonConst,
  eC_Const,
  e_FORCE32 = 0xffffffff
};

enum eValueMemoryMode
{
  eVMM_Value,
  eVMM_Reference,
  eVMM_Pointer,
  eVMM_PointerToPointer,
};

enum eFunctionVirtuality
{
  eFV_NonVirtual,
  eFV_Virtual,
};


class CE_CORE_API ValueDeclaration
{
public:
  ValueDeclaration(eConstness constness = eC_NonConst, const std::string& type = "void", eValueMemoryMode mode = eVMM_Value);

  eConstness GetConstness() const;
  const std::string& GetType() const;
  eValueMemoryMode GetMode() const;

  bool operator==(const ValueDeclaration& other) const;
private:
  eConstness m_constness;
  std::string m_type;
  eValueMemoryMode m_mode;
};

class CE_CORE_API Property
{
public:
  virtual ~Property();

  const ValueDeclaration& GetContainerDecl() const;
  const ValueDeclaration& GetDecl() const;
  bool IsContainer() const;
  const std::string& GetName() const;

  template<typename T>
  void Set(ce::iObject* object, T& t) const
  {
    const void* const_value = reinterpret_cast<const void*>(&t);
    SetValue(object, const_cast<void*>(const_value));
  }


  template<typename T>
  T Get(ce::iObject* object) const
  {
    void* data = GetValue(object);
    return *reinterpret_cast<T*>(data);
  }

  template<typename T>
  T Get(const ce::iObject* object) const
  {
    const void* data = GetValue(object);
    return *reinterpret_cast<const T*>(data);
  }


  std::set<std::string> GetProperties() const;
  bool HasProperty(const std::string& property) const;
  std::string GetProperty(const std::string& property) const;


protected:
  Property(const ValueDeclaration& containerDecl, const std::string& name, const ValueDeclaration& decl);

  void SetProperty(const std::string& key, const std::string& value = "");
  virtual void SetValue(ce::iObject* object, void* data) const = 0;
  virtual const void* GetValue(const ce::iObject* object) const = 0;
  virtual void* GetValue(ce::iObject* object) const = 0;


private:
  std::string m_name;
  ValueDeclaration m_containerDecl;
  ValueDeclaration m_decl;
  std::map<std::string, std::string> m_properties;

};


class CE_CORE_API FunctionAttribute
{
public:
  FunctionAttribute(const ValueDeclaration& type = ValueDeclaration(), const std::string& name = "");

  const ValueDeclaration& GetType() const;
  const std::string& GetName() const;

private:
  ValueDeclaration m_type;
  std::string m_name;
};

class CE_CORE_API Function
{
public:
  virtual ~Function() { }
  const std::string& GetName() const;
  const ValueDeclaration& GetReturnType() const;
  eFunctionVirtuality GetVirtuality() const;
  eConstness GetConstness() const;

  size_t GetNumberOfAttributes() const;
  const FunctionAttribute& GetAttribute(size_t idx) const;


  //
  // Invoke void 
  template<typename ... Type>
  void InvokeVoid(ce::iObject* obj, Type ... args) const { InvokeVoidImpl(obj, &args...); }
  template<typename ... Type>
  void InvokeVoid(const ce::iObject* obj, Type ... args) const { InvokeVoidImpl(obj, &args...); }



  //
  // Invoke value
  template<typename R, typename ... Type>
  R InvokeValue(ce::iObject* obj, Type ... args) const { R r; InvokeValueImpl(obj, &args...); return r; }
  template<typename R, typename ... Type>
  R InvokeValue(const ce::iObject* obj, Type ... args) const { R r; InvokeValueImpl(obj, &args...); return r; }


  //
  // Invoke reference 
  template<typename R, typename ... Types>
  R& InvokeReference(ce::iObject* obj, Types ... args) const { return *reinterpret_cast<R*>(InvokeReferenceImpl(obj, &args...)); }
  template<typename R, typename ... Types>
  R& InvokeReference(const ce::iObject* obj, Types ... args) const { return *reinterpret_cast<R*>(InvokeReferenceImpl(obj, &args...)); }

  //
  // Invoke const-reference 
  template<typename R, typename ... Types>
  const R& InvokeConstReference(ce::iObject* obj, Types ... args) const { return *reinterpret_cast<const R*>(InvokeConstReferenceImpl(obj, &args...)); }
  template<typename R, typename ... Types>
  const R& InvokeConstReference(const ce::iObject* obj, Types ... args) const { return *reinterpret_cast<const R*>(InvokeConstReferenceImpl(obj, &args...)); }



  //
  // Invoke pointer 
  template<typename R, typename ... Types>
  R* InvokePointer(ce::iObject* obj, Types ... args) const { return reinterpret_cast<R*>(InvokePointerImpl(obj, &args...)); }
  template<typename R, typename ... Types>
  R* InvokePointer(const ce::iObject* obj, Types ... args) const { return reinterpret_cast<R*>(InvokePointerImpl(obj, &args...)); }

  //
  // Invoke const-pointer 
  template<typename R, typename ... Types>
  const R* InvokeConstPointer(ce::iObject* obj, Types ... args) const { return reinterpret_cast<const R*>(InvokeConstPointerImpl(obj, &args...)); }
  template<typename R, typename ... Types>
  const R* InvokeConstPointer(const ce::iObject* obj, Types ... args) const { return reinterpret_cast<const R*>(InvokeConstPointerImpl(obj, &args...)); }


protected:
  Function(eFunctionVirtuality virtuality, const ValueDeclaration& returnType, const std::string& name, eConstness constness);

  void AddAttribute(const FunctionAttribute& attribute);

  virtual void InvokeVoidImpl(ce::iObject* obj, ...) const = 0;
  virtual void InvokeVoidImpl(const ce::iObject* obj, ...) const = 0;
  virtual void InvokeValueImpl(ce::iObject* obj, ...) const = 0;
  virtual void InvokeValueImpl(const ce::iObject* obj, ...) const = 0;
  virtual void* InvokeReferenceImpl(ce::iObject* obj, ...) const = 0;
  virtual void* InvokeReferenceImpl(const ce::iObject* obj, ...) const = 0;
  virtual const void* InvokeConstReferenceImpl(ce::iObject* obj, ...) const = 0;
  virtual const void* InvokeConstReferenceImpl(const ce::iObject* obj, ...) const = 0;
  virtual void* InvokePointerImpl(ce::iObject* obj, ...) const = 0;
  virtual void* InvokePointerImpl(const ce::iObject* obj, ...) const = 0;
  virtual const void* InvokeConstPointerImpl(ce::iObject* obj, ...) const = 0;
  virtual const void* InvokeConstPointerImpl(const ce::iObject* obj, ...) const = 0;

private:
  eFunctionVirtuality m_virtuality;
  eConstness m_constness;
  std::string m_name;
  ValueDeclaration m_returnType;

  std::vector<FunctionAttribute> m_attributes;
  FunctionAttribute m_invalid;
};



class CE_CORE_API Class
{
public:
  virtual ~Class();
  virtual ce::iObject* CreateInstance() const = 0;
  template<typename T>
  T* CreateInstance() const
  {
    ce::iObject* obj = CreateInstance();
    if (!obj)
    {
      return 0;
    }

    T* t = ce::QueryClass<T>(obj);
    if (!t)
    {
      obj->Release();
      obj = nullptr;
    }
    return t;
  }

  size_t GetNumberOfProperties() const;
  const Property* GetProperty(size_t idx) const;
  const Property* GetProperty(const std::string& propName) const;

  size_t GetNumberOfFunctions() const;
  const Function* GetFunction(size_t idx) const;
  std::vector<const Function*> GetFunction(const std::string& functionName) const;
  std::vector<const Function*> GetFunction(const std::string& functionName, eConstness constness) const;
  const Function* GetFirstFunction(const std::string& functionName) const;
  const Function* GetFirstFunction(const std::string& functionName, eConstness constness) const;

  size_t GetNumberOfSuperClasses() const;
  const Class* GetSuperClass(size_t idx) const;

  bool HasMeta(const std::string& meta) const;
  const std::string GetMeta(const std::string& meta) const;

  const std::string& GetName() const;

  template<typename T>
  bool IsInstanceOf() const
  {
    return IsInstanceOf(T::GetStaticClass());
  }
  bool IsInstanceOf(const Class* clazz) const;

  template<typename T>
  bool IsAssignableFrom() const
  {
    return IsAssignableFrom(T::GetStaticClass());
  }
  bool IsAssignableFrom(const Class* clazz) const;


protected:
  Class(const std::string& name);

  void AddSuperClass(const Class* parentClass);
  void AddProperty(const Property* prop);
  void AddFunction(const Function* function);
  void AddMeta(const std::string& key, const std::string& value);
private:
  std::string m_name;
  std::vector<const Class*> m_superClasses;
  std::vector<const Property*> m_properties;
  std::vector<const Function*> m_functions;
  std::map<std::string, std::string> m_meta;
};

template<typename T>
T* csNewClassInstance(const Class* clazz)
{
  return reinterpret_cast<T*>(clazz->CreateInstance());
}


class CE_CORE_API iObjectClass : public Class
{
public:
  iObjectClass();

  static iObjectClass* Get();

  virtual ce::iObject* CreateInstance() const;

};



CE_CLASS()
class CE_CORE_API Object : public CE_SUPER(ce::iObject)
{
  CE_CLASS_GEN_OBJECT;

public:
  Object();
  virtual ~Object();



};


}

