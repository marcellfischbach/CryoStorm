
#pragma once

#include <cstdint>


#define SPC_CLASS(...)
#define SPC_CLASS(...)
#define SPC_SUPER(Cls) Cls
#define SPC_PROPERTY(...)
#define SPC_FUNCTION(...)
#define SPC_CLASS_GEN public: \
    virtual const spc::Class *GetClass () const;\
    static const spc::Class *GetStaticClass (); \
    virtual void *QueryClass(const spc::Class *clazz); \
    virtual const void *QueryClass(const spc::Class *clazz) const

#define SPC_CLASS_GEN_OBJECT \
    SPC_CLASS_GEN; \
    virtual void AddRef() \
    { \
      m_refCount++; \
    } \
    virtual void Release() \
    { \
      --m_refCount; \
      if (m_refCount <= 0) \
      { \
          delete this;\
      } \
    } \
    virtual int64_t RefCount () const\
    { \
      return m_refCount; \
    } \
    private: \
      int64_t m_refCount
    
#define SPC_CLASS_GEN_CONSTR m_refCount = 1

#define SPC_WEAK_OBJECT(Super) \
  public: void AddRef () { Super::AddRef (); }\
          void Release () { Super::Release (); }


#define SPC_SET(oo, no) if (no) no->AddRef(); if (oo) oo->Release(); oo = no
#define SPC_ADDREF(o) if (o) o->AddRef()
#define SPC_RELEASE(o) if (o) o->Release()

#include <map>
#include <set>
#include <string>
#include <vector>
#include <stdarg.h>
#include <spcCore/coreexport.hh>
#include <exception>

namespace spc
{


class BadMethodInvokation : public std::exception
{
public:
  BadMethodInvokation(const std::string &msg)
    : m_msg(msg)
  {
  }
  virtual ~BadMethodInvokation()
  {

  }

  const char *getMessage () const
  {
    return m_msg.c_str();
  }
private:
  std::string m_msg;
};


class NoSuchMethodException : public std::exception
{
public:
  NoSuchMethodException(const std::string &methodName)
    : m_methodName(methodName)
  {
  }
  virtual ~NoSuchMethodException()
  {

  }

  const char *getMethodName () const
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

struct SPC_CORE_API iObject
{
  iObject();
  virtual ~iObject();
  virtual const Class* GetClass() const;
  static const Class* GetStaticClass();

  virtual void AddRef() = 0;

  virtual void Release() = 0;

  virtual int64_t RefCount() const = 0;

  template<typename T> T* Query()
  {
    return reinterpret_cast<T*>(QueryClass(T::GetStaticClass()));
  }

  virtual void* QueryClass(const Class* clazz);

  template<typename T> const T* Query() const
  {
    return reinterpret_cast<T*>(QueryClass(T::GetStaticClass()));
  }
  virtual const void* QueryClass(const Class* clazz) const;
};


template<typename T>
T* QueryClass(spc::iObject* object)
{
  if (!object)
  {
    return 0;
  }
  return reinterpret_cast<T*>(object->QueryClass(T::GetStaticClass()));
}

template<typename T>
const T* QueryClass(const spc::iObject* object)
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


class SPC_CORE_API ValueDeclaration
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

class SPC_CORE_API Property
{
public:
  virtual ~Property();

  const ValueDeclaration& GetContainerDecl() const;
  const ValueDeclaration& GetDecl() const;
  bool IsContainer() const;
  const std::string& GetName() const;

  template<typename T>
  void Set(spc::iObject* object, T& t) const
  {
    const void* const_value = reinterpret_cast<const void*>(&t);
    SetValue(object, const_cast<void*>(const_value));
  }


  template<typename T>
  T Get(spc::iObject* object) const
  {
    void* data = GetValue(object);
    return *reinterpret_cast<T*>(data);
  }

  template<typename T>
  T Get(const spc::iObject* object) const
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
  virtual void SetValue(spc::iObject* object, void* data) const = 0;
  virtual const void* GetValue(const spc::iObject* object) const = 0;
  virtual void* GetValue(spc::iObject* object) const = 0;


private:
  std::string m_name;
  ValueDeclaration m_containerDecl;
  ValueDeclaration m_decl;
  std::map<std::string, std::string> m_properties;

};


class SPC_CORE_API FunctionAttribute
{
public:
  FunctionAttribute(const ValueDeclaration& type = ValueDeclaration(), const std::string& name = "");

  const ValueDeclaration& GetType() const;
  const std::string& GetName() const;

private:
  ValueDeclaration m_type;
  std::string m_name;
};

class SPC_CORE_API Function
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
  void InvokeVoid(spc::iObject* obj, Type ... args) const { InvokeVoidImpl(obj, &args...); }
  template<typename ... Type>
  void InvokeVoid(const spc::iObject* obj, Type ... args) const { InvokeVoidImpl(obj, &args...); }



  //
  // Invoke value
  template<typename R, typename ... Type>
  R InvokeValue(spc::iObject* obj, Type ... args) const { R r; InvokeValueImpl(obj, &args...); return r; }
  template<typename R, typename ... Type>
  R InvokeValue(const spc::iObject* obj, Type ... args) const { R r; InvokeValueImpl(obj, &args...); return r; }


  //
  // Invoke reference 
  template<typename R, typename ... Types>
  R& InvokeReference(spc::iObject* obj, Types ... args) const { return *reinterpret_cast<R*>(InvokeReferenceImpl(obj, &args...)); }
  template<typename R, typename ... Types>
  R& InvokeReference(const spc::iObject* obj, Types ... args) const { return *reinterpret_cast<R*>(InvokeReferenceImpl(obj, &args...)); }

  //
  // Invoke const-reference 
  template<typename R, typename ... Types>
  const R& InvokeConstReference(spc::iObject* obj, Types ... args) const { return *reinterpret_cast<const R*>(InvokeConstReferenceImpl(obj, &args...)); }
  template<typename R, typename ... Types>
  const R& InvokeConstReference(const spc::iObject* obj, Types ... args) const { return *reinterpret_cast<const R*>(InvokeConstReferenceImpl(obj, &args...)); }



  //
  // Invoke pointer 
  template<typename R, typename ... Types>
  R* InvokePointer(spc::iObject* obj, Types ... args) const { return reinterpret_cast<R*>(InvokePointerImpl(obj, &args...)); }
  template<typename R, typename ... Types>
  R* InvokePointer(const spc::iObject* obj, Types ... args) const { return reinterpret_cast<R*>(InvokePointerImpl(obj, &args...)); }

  //
  // Invoke const-pointer 
  template<typename R, typename ... Types>
  const R* InvokeConstPointer(spc::iObject* obj, Types ... args) const { return reinterpret_cast<const R*>(InvokeConstPointerImpl(obj, &args...)); }
  template<typename R, typename ... Types>
  const R* InvokeConstPointer(const spc::iObject* obj, Types ... args) const { return reinterpret_cast<const R*>(InvokeConstPointerImpl(obj, &args...)); }


protected:
  Function(eFunctionVirtuality virtuality, const ValueDeclaration& returnType, const std::string& name, eConstness constness);

  void AddAttribute(const FunctionAttribute& attribute);

  virtual void InvokeVoidImpl(spc::iObject* obj, ...) const = 0;
  virtual void InvokeVoidImpl(const spc::iObject* obj, ...) const = 0;
  virtual void InvokeValueImpl(spc::iObject* obj, ...) const = 0;
  virtual void InvokeValueImpl(const spc::iObject* obj, ...) const = 0;
  virtual void* InvokeReferenceImpl(spc::iObject* obj, ...) const = 0;
  virtual void* InvokeReferenceImpl(const spc::iObject* obj, ...) const = 0;
  virtual const void* InvokeConstReferenceImpl(spc::iObject* obj, ...) const = 0;
  virtual const void* InvokeConstReferenceImpl(const spc::iObject* obj, ...) const = 0;
  virtual void* InvokePointerImpl(spc::iObject* obj, ...) const = 0;
  virtual void* InvokePointerImpl(const spc::iObject* obj, ...) const = 0;
  virtual const void* InvokeConstPointerImpl(spc::iObject* obj, ...) const = 0;
  virtual const void* InvokeConstPointerImpl(const spc::iObject* obj, ...) const = 0;

private:
  eFunctionVirtuality m_virtuality;
  eConstness m_constness;
  std::string m_name;
  ValueDeclaration m_returnType;

  std::vector<FunctionAttribute> m_attributes;
  FunctionAttribute m_invalid;
};



class SPC_CORE_API Class
{
public:
  virtual ~Class();
  virtual spc::iObject* CreateInstance() const = 0;
  template<typename T>
  T* CreateInstance() const
  {
    spc::iObject* obj = CreateInstance();
    if (!obj)
    {
      return 0;
    }

    T* t = spc::QueryClass<T>(obj);
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


class SPC_CORE_API iObjectClass : public Class
{
public:
  iObjectClass();

  static iObjectClass* Get();

  virtual spc::iObject* CreateInstance() const;

};



SPC_CLASS()
class SPC_CORE_API Object : public SPC_SUPER(spc::iObject)
{
  SPC_CLASS_GEN_OBJECT;

public:
  Object();
  virtual ~Object();



};


}

