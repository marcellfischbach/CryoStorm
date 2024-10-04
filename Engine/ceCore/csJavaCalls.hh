//
// Created by MCEL on 08.05.2024.
//

#pragma once

#include <ceCore/csCoreExport.hh>
#include <ceCore/class.hh>
#include <jni.h>


namespace cryo
{

#define JAVA_BOOLEAN "Z"
#define JAVA_BYTE "B"
#define JAVA_SHORT "S"
#define JAVA_INT "I"
#define JAVA_LONG "J"
#define JAVA_FLOAT "F"
#define JAVA_DOUBLE "D"
#define JAVA_STRING "Ljava/lang/String;"

#define JAVA_BOOLEAN_ARRAY "[Z"
#define JAVA_BYTE_ARRAY "[B"
#define JAVA_SHORT_ARRAY "[S"
#define JAVA_INT_ARRAY "[I"
#define JAVA_LONG_ARRAY "[J"
#define JAVA_FLOAT_ARRAY "[F"
#define JAVA_DOUBLE_ARRAY "[D"
#define JAVA_STRING_ARRAY "[Ljava/lang/String;"

#define CS_JOBJECT !jobj ? (jobj = (obj ? obj->GetJObject() : nullptr)) : jobj

class csJavaCallVoid
{
public:
  csJavaCallVoid(JNIEnv *env, jobject jobj, const std::string &className, const std::string &methodName)
  {
    this->obj = nullptr;
    this->jobj = jobj;
    cls      = env->FindClass(className.c_str());
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), "()V") : nullptr;
  }
  csJavaCallVoid(JNIEnv *env, const iObject *obj, const std::string &className, const std::string &methodName)
  {
    this->obj = obj;
    this->jobj = nullptr;
    cls      = env->FindClass(className.c_str());
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), "()V") : nullptr;
  }

  void call(JNIEnv *env)
  {
    if (methodId)
    {
      jobject pJobject = CS_JOBJECT;
      if (pJobject)
      {
        env->CallVoidMethod(pJobject, methodId);
      }
    }
  }

private:
  const iObject *obj;
  jobject       jobj;
  jclass        cls;
  jmethodID     methodId;
};

template<typename T0>
class csJavaCallVoid1
{
public:
  csJavaCallVoid1(JNIEnv *env,
                  jobject jobj,
                  const std::string &className,
                  const std::string &methodName,
                  const std::string &arg0)
  {
    this->obj = nullptr;
    this->jobj = jobj;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + ")V";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }
  csJavaCallVoid1(JNIEnv *env,
                  const iObject *obj,
                  const std::string &className,
                  const std::string &methodName,
                  const std::string &arg0)
  {
    this->obj = obj;
    this->jobj = nullptr;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + ")V";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  void call(JNIEnv *env, T0 arg0)
  {
    if (methodId)
    {
      jobject pJobject = CS_JOBJECT;
      if (pJobject)
      {
        env->CallVoidMethod(pJobject, methodId, arg0);
      }
    }
  }

private:
  const iObject *obj;
  jobject       jobj;
  jclass        cls;
  jmethodID     methodId;
};

template<typename T0, typename T1>
class csJavaCallVoid2
{
public:
  csJavaCallVoid2(JNIEnv
                *env,
                  jobject jobj,
                  const std::string &className,
                  const std::string &methodName,
                  const std::string &arg0,
                  const std::string &arg1
                 )
  {
    this->obj = nullptr;
    this->jobj = jobj;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + arg1 + ")V";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  csJavaCallVoid2(JNIEnv
             *env,
                  const iObject *obj,
                  const std::string &className,
                  const std::string &methodName,
                  const std::string &arg0,
                  const std::string &arg1
                 )
  {
    this->obj = obj;
    this->jobj = nullptr;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + arg1 + ")V";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  void call(JNIEnv *env, T0 arg0, T1 arg1)
  {
    if (methodId)
    {
      jobject pJobject = CS_JOBJECT;
      if (pJobject)
      {
        env->CallVoidMethod(pJobject, methodId, arg0, arg1);
      }
    }
  }

private:
  const iObject *obj;
  jobject       jobj;
  jclass        cls;
  jmethodID     methodId;
};


template<typename T0, typename T1, typename T2>
class csJavaCallVoid3
{
public:
  csJavaCallVoid3(JNIEnv
                *env,
                  jobject jobj,
                  const std::string &className,
                  const std::string &methodName,
                  const std::string &arg0,
                  const std::string &arg1,
                  const std::string &arg2
                 )
  {
    this->obj = nullptr;
    this->jobj = jobj;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + arg1 + arg2 + ")V";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }
  csJavaCallVoid3(JNIEnv
             *env,
                  const iObject *obj,
                  const std::string &className,
                  const std::string &methodName,
                  const std::string &arg0,
                  const std::string &arg1,
                  const std::string &arg2
                 )
  {
    this->obj = obj;
    this->jobj = nullptr;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + arg1 + arg2 + ")V";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  void call(JNIEnv *env, T0 arg0, T1 arg1, T2  arg2)
  {
    if (methodId)
    {
      jobject pJobject = CS_JOBJECT;
      if (pJobject)
      {
        env->CallVoidMethod(pJobject, methodId, arg0, arg1, arg2);
      }
    }
  }

private:
  const iObject *obj;
  jobject       jobj;
  jclass        cls;
  jmethodID     methodId;
};



class csJavaCallBoolean
{
public:
  csJavaCallBoolean(JNIEnv *env, jobject jobj, const std::string &className, const std::string &methodName)
  {
    this->obj = nullptr;
    this->jobj = jobj;
    cls      = env->FindClass(className.c_str());
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), "()Z") : nullptr;
  }
  csJavaCallBoolean(JNIEnv *env, const iObject *obj, const std::string &className, const std::string &methodName)
  {
    this->obj = obj;
    this->jobj = nullptr;
    cls      = env->FindClass(className.c_str());
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), "()Z") : nullptr;
  }

  jboolean call(JNIEnv *env, jboolean def)
  {
    jboolean res = def;
    if (methodId)
    {
      jobject pJobject = CS_JOBJECT;
      if (pJobject)
      {
        res = (jboolean) env->CallBooleanMethod(pJobject, methodId);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jobject       jobj;
  jclass        cls;
  jmethodID     methodId;
};

template<typename T0>
class csJavaCallBoolean1
{
public:
  csJavaCallBoolean1(JNIEnv *env,
                     jobject jobj,
                     const std::string &className,
                     const std::string &methodName,
                     const std::string &arg0)
  {
    this->obj = nullptr;
    this->jobj = jobj;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + ")Z";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }
  csJavaCallBoolean1(JNIEnv *env,
                     const iObject *obj,
                     const std::string &className,
                     const std::string &methodName,
                     const std::string &arg0)
  {
    this->obj = obj;
    this->jobj = nullptr;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + ")Z";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  jboolean call(JNIEnv *env, T0 arg0, jboolean def)
  {
    jboolean res = def;
    if (methodId)
    {
      jobject pJobject = CS_JOBJECT;
      if (pJobject)
      {
        res = (jboolean) env->CallBooleanMethod(pJobject, methodId, arg0);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jobject       jobj;
  jclass        cls;
  jmethodID     methodId;
};

template<typename T0, typename T1>
class csJavaCallBoolean2
{
public:
  csJavaCallBoolean2(JNIEnv
                   *env,
                     jobject jobj,
                     const std::string &className,
                     const std::string &methodName,
                     const std::string &arg0,
                     const std::string &arg1
                    )
  {
    this->obj = nullptr;
    this->jobj = jobj;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + arg1 + ")Z";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }
  csJavaCallBoolean2(JNIEnv
               *env,
                     const iObject *obj,
                     const std::string &className,
                     const std::string &methodName,
                     const std::string &arg0,
                     const std::string &arg1
                    )
  {
    this->obj = obj;
    this->jobj = nullptr;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + arg1 + ")Z";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  jboolean call(JNIEnv *env, T0 arg0, T1 arg1, jboolean def)
  {
    jboolean res = def;
    if (methodId)
    {
      jobject pJobject = CS_JOBJECT;
      if (pJobject)
      {
        res = (jboolean) env->CallBooleanMethod(pJobject, methodId, arg0, arg1);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jobject       jobj;
  jclass        cls;
  jmethodID     methodId;
};


template<typename T0, typename T1, typename T2>
class csJavaCallBoolean3
{
public:
  csJavaCallBoolean3(JNIEnv
                   *env,
                     jobject jobj,
                     const std::string &className,
                     const std::string &methodName,
                     const std::string &arg0,
                     const std::string &arg1,
                     const std::string &arg2
                    )
  {
    this->obj = nullptr;
    this->jobj = jobj;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + arg1 + arg2 + ")Z";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }
  csJavaCallBoolean3(JNIEnv
               *env,
                     const iObject *obj,
                     const std::string &className,
                     const std::string &methodName,
                     const std::string &arg0,
                     const std::string &arg1,
                     const std::string &arg2
                    )
  {
    this->obj = obj;
    this->jobj = nullptr;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + arg1 + arg2 + ")Z";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  jboolean call(JNIEnv *env, T0 arg0, T1 arg1, T2  arg2, jboolean def)
  {
    jboolean res = def;
    if (methodId)
    {
      jobject pJobject = CS_JOBJECT;
      if (pJobject)
      {
        res = (jboolean) env->CallBooleanMethod(pJobject, methodId, arg0, arg1, arg2);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jobject       jobj;
  jclass        cls;
  jmethodID     methodId;
};



class csJavaCallByte
{
public:
  csJavaCallByte(JNIEnv *env, jobject jobj, const std::string &className, const std::string &methodName)
  {
    this->obj = nullptr;
    this->jobj = jobj;
    cls      = env->FindClass(className.c_str());
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), "()B") : nullptr;
  }
  csJavaCallByte(JNIEnv *env, const iObject *obj, const std::string &className, const std::string &methodName)
  {
    this->obj = obj;
    this->jobj = nullptr;
    cls      = env->FindClass(className.c_str());
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), "()B") : nullptr;
  }

  jbyte call(JNIEnv *env, jbyte def)
  {
    jbyte res = def;
    if (methodId)
    {
      jobject pJobject = CS_JOBJECT;
      if (pJobject)
      {
        res = (jbyte) env->CallByteMethod(pJobject, methodId);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jobject       jobj;
  jclass        cls;
  jmethodID     methodId;
};

template<typename T0>
class csJavaCallByte1
{
public:
  csJavaCallByte1(JNIEnv *env,
                  jobject jobj,
                  const std::string &className,
                  const std::string &methodName,
                  const std::string &arg0)
  {
    this->obj = nullptr;
    this->jobj = jobj;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + ")B";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }
  csJavaCallByte1(JNIEnv *env,
                  const iObject *obj,
                  const std::string &className,
                  const std::string &methodName,
                  const std::string &arg0)
  {
    this->obj = obj;
    this->jobj = nullptr;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + ")B";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  jbyte call(JNIEnv *env, T0 arg0, jbyte def)
  {
    jbyte res = def;
    if (methodId)
    {
      jobject pJobject = CS_JOBJECT;
      if (pJobject)
      {
        res = (jbyte) env->CallByteMethod(pJobject, methodId, arg0);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jobject       jobj;
  jclass        cls;
  jmethodID     methodId;
};

template<typename T0, typename T1>
class csJavaCallByte2
{
public:
  csJavaCallByte2(JNIEnv
                *env,
                  jobject jobj,
                  const std::string &className,
                  const std::string &methodName,
                  const std::string &arg0,
                  const std::string &arg1
                 )
  {
    this->obj = nullptr;
    this->jobj = jobj;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + arg1 + ")B";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }
  csJavaCallByte2(JNIEnv
               *env,
                  const iObject *obj,
                  const std::string &className,
                  const std::string &methodName,
                  const std::string &arg0,
                  const std::string &arg1
                 )
  {
    this->obj = obj;
    this->jobj = nullptr;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + arg1 + ")B";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  jbyte call(JNIEnv *env, T0 arg0, T1 arg1, jbyte def)
  {
    jbyte res = def;
    if (methodId)
    {
      jobject pJobject = CS_JOBJECT;
      if (pJobject)
      {
        res = (jbyte) env->CallByteMethod(pJobject, methodId, arg0, arg1);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jobject       jobj;
  jclass        cls;
  jmethodID     methodId;
};


template<typename T0, typename T1, typename T2>
class csJavaCallByte3
{
public:
  csJavaCallByte3(JNIEnv
                *env,
                  jobject jobj,
                  const std::string &className,
                  const std::string &methodName,
                  const std::string &arg0,
                  const std::string &arg1,
                  const std::string &arg2
                 )
  {
    this->obj = nullptr;
    this->jobj = jobj;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + arg1 + arg2 + ")B";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }
  csJavaCallByte3(JNIEnv
               *env,
                  const iObject *obj,
                  const std::string &className,
                  const std::string &methodName,
                  const std::string &arg0,
                  const std::string &arg1,
                  const std::string &arg2
                 )
  {
    this->obj = obj;
    this->jobj = nullptr;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + arg1 + arg2 + ")B";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  jbyte call(JNIEnv *env, T0 arg0, T1 arg1, T2  arg2, jbyte def)
  {
    jbyte res = def;
    if (methodId)
    {
      jobject pJobject = CS_JOBJECT;
      if (pJobject)
      {
        res = (jbyte) env->CallByteMethod(pJobject, methodId, arg0, arg1, arg2);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jobject       jobj;
  jclass        cls;
  jmethodID     methodId;
};




class csJavaCallShort
{
public:
  csJavaCallShort(JNIEnv *env, jobject jobj, const std::string &className, const std::string &methodName)
  {
    this->obj = nullptr;
    this->jobj = jobj;
    cls      = env->FindClass(className.c_str());
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), "()S") : nullptr;
  }
  csJavaCallShort(JNIEnv *env, const iObject *obj, const std::string &className, const std::string &methodName)
  {
    this->obj = obj;
    this->jobj = nullptr;
    cls      = env->FindClass(className.c_str());
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), "()S") : nullptr;
  }

  jshort call(JNIEnv *env, jshort def)
  {
    jshort res = def;
    if (methodId)
    {
      jobject pJobject = CS_JOBJECT;
      if (pJobject)
      {
        res = (jshort) env->CallShortMethod(pJobject, methodId);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jobject       jobj;
  jclass        cls;
  jmethodID     methodId;
};

template<typename T0>
class csJavaCallShort1
{
public:
  csJavaCallShort1(JNIEnv *env,
                   jobject jobj,
                   const std::string &className,
                   const std::string &methodName,
                   const std::string &arg0)
  {
    this->obj = nullptr;
    this->jobj = jobj;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + ")S";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }
  csJavaCallShort1(JNIEnv *env,
                   const iObject *obj,
                   const std::string &className,
                   const std::string &methodName,
                   const std::string &arg0)
  {
    this->obj = obj;
    this->jobj = nullptr;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + ")S";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  jshort call(JNIEnv *env, T0 arg0, jshort def)
  {
    jshort res = def;
    if (methodId)
    {
      jobject pJobject = CS_JOBJECT;
      if (pJobject)
      {
        res = (jshort) env->CallShortMethod(pJobject, methodId, arg0);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jobject       jobj;
  jclass        cls;
  jmethodID     methodId;
};

template<typename T0, typename T1>
class csJavaCallShort2
{
public:
  csJavaCallShort2(JNIEnv
                 *env,
                   jobject jobj,
                   const std::string &className,
                   const std::string &methodName,
                   const std::string &arg0,
                   const std::string &arg1
                  )
  {
    this->obj = nullptr;
    this->jobj = jobj;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + arg1 + ")S";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }
  csJavaCallShort2(JNIEnv
               *env,
                   const iObject *obj,
                   const std::string &className,
                   const std::string &methodName,
                   const std::string &arg0,
                   const std::string &arg1
                  )
  {
    this->obj = obj;
    this->jobj = nullptr;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + arg1 + ")S";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  jshort call(JNIEnv *env, T0 arg0, T1 arg1, jshort def)
  {
    jshort res = def;
    if (methodId)
    {
      jobject pJobject = CS_JOBJECT;
      if (pJobject)
      {
        res = (jshort) env->CallShortMethod(pJobject, methodId, arg0, arg1);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jobject       jobj;
  jclass        cls;
  jmethodID     methodId;
};


template<typename T0, typename T1, typename T2>
class csJavaCallShort3
{
public:
  csJavaCallShort3(JNIEnv
                 *env,
                   jobject jobj,
                   const std::string &className,
                   const std::string &methodName,
                   const std::string &arg0,
                   const std::string &arg1,
                   const std::string &arg2
                  )
  {
    this->obj = nullptr;
    this->jobj = jobj;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + arg1 + arg2 + ")S";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }
  csJavaCallShort3(JNIEnv
               *env,
                   const iObject *obj,
                   const std::string &className,
                   const std::string &methodName,
                   const std::string &arg0,
                   const std::string &arg1,
                   const std::string &arg2
                  )
  {
    this->obj = obj;
    this->jobj = nullptr;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + arg1 + arg2 + ")S";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  jshort call(JNIEnv *env, T0 arg0, T1 arg1, T2  arg2, jshort def)
  {
    jshort res = def;
    if (methodId)
    {
      jobject pJobject = CS_JOBJECT;
      if (pJobject)
      {
        res = (jshort) env->CallShortMethod(pJobject, methodId, arg0, arg1, arg2);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jobject       jobj;
  jclass        cls;
  jmethodID     methodId;
};







class csJavaCallInt
{
public:
  csJavaCallInt(JNIEnv *env, jobject jobj, const std::string &className, const std::string &methodName)
  {
    this->obj = nullptr;
    this->jobj = jobj;
    cls      = env->FindClass(className.c_str());
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), "()I") : nullptr;
  }
  csJavaCallInt(JNIEnv *env, const iObject *obj, const std::string &className, const std::string &methodName)
  {
    this->obj = obj;
    this->jobj = nullptr;
    cls      = env->FindClass(className.c_str());
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), "()I") : nullptr;
  }

  jint call(JNIEnv *env, jint def)
  {
    jint res = def;
    if (methodId)
    {
      jobject pJobject = CS_JOBJECT;
      if (pJobject)
      {
        res = (jint) env->CallIntMethod(pJobject, methodId);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jobject       jobj;
  jclass        cls;
  jmethodID     methodId;
};

template<typename T0>
class csJavaCallInt1
{
public:
  csJavaCallInt1(JNIEnv *env,
                 jobject jobj,
                 const std::string &className,
                 const std::string &methodName,
                 const std::string &arg0)
  {
    this->obj = nullptr;
    this->jobj = jobj;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + ")I";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }
  csJavaCallInt1(JNIEnv *env,
                 const iObject *obj,
                 const std::string &className,
                 const std::string &methodName,
                 const std::string &arg0)
  {
    this->obj = obj;
    this->jobj = nullptr;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + ")I";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  jint call(JNIEnv *env, T0 arg0, jint def)
  {
    jint res = def;
    if (methodId)
    {
      jobject pJobject = CS_JOBJECT;
      if (pJobject)
      {
        res = (jint) env->CallIntMethod(pJobject, methodId, arg0);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jobject       jobj;
  jclass        cls;
  jmethodID     methodId;
};

template<typename T0, typename T1>
class csJavaCallInt2
{
public:
  csJavaCallInt2(JNIEnv
               *env,
                 jobject jobj,
                 const std::string &className,
                 const std::string &methodName,
                 const std::string &arg0,
                 const std::string &arg1
                )
  {
    this->obj = nullptr;
    this->jobj = jobj;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + arg1 + ")I";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }
  csJavaCallInt2(JNIEnv
                *env,
                 const iObject *obj,
                 const std::string &className,
                 const std::string &methodName,
                 const std::string &arg0,
                 const std::string &arg1
                )
  {
    this->obj = obj;
    this->jobj = nullptr;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + arg1 + ")I";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  jint call(JNIEnv *env, T0 arg0, T1 arg1, jint def)
  {
    jint res = def;
    if (methodId)
    {
      jobject pJobject = CS_JOBJECT;
      if (pJobject)
      {
        res = (jint) env->CallIntMethod(pJobject, methodId, arg0, arg1);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jobject       jobj;
  jclass        cls;
  jmethodID     methodId;
};


template<typename T0, typename T1, typename T2>
class csJavaCallInt3
{
public:
  csJavaCallInt3(JNIEnv
               *env,
                 jobject jobj,
                 const std::string &className,
                 const std::string &methodName,
                 const std::string &arg0,
                 const std::string &arg1,
                 const std::string &arg2
                )
  {
    this->obj = nullptr;
    this->jobj = jobj;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + arg1 + arg2 + ")I";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }
  csJavaCallInt3(JNIEnv
                *env,
                 const iObject *obj,
                 const std::string &className,
                 const std::string &methodName,
                 const std::string &arg0,
                 const std::string &arg1,
                 const std::string &arg2
                )
  {
    this->obj = obj;
    this->jobj = nullptr;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + arg1 + arg2 + ")I";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  jint call(JNIEnv *env, T0 arg0, T1 arg1, T2  arg2, jint def)
  {
    jint res = def;
    if (methodId)
    {
      jobject pJobject = CS_JOBJECT;
      if (pJobject)
      {
        res = (jint) env->CallIntMethod(pJobject, methodId, arg0, arg1, arg2);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jobject       jobj;
  jclass        cls;
  jmethodID     methodId;
};



class csJavaCallLong
{
public:
  csJavaCallLong(JNIEnv *env, jobject jobj, const std::string &className, const std::string &methodName)
  {
    this->obj = nullptr;
    this->jobj = jobj;
    cls      = env->FindClass(className.c_str());
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), "()J") : nullptr;
  }
  csJavaCallLong(JNIEnv *env, const iObject *obj, const std::string &className, const std::string &methodName)
  {
    this->obj = obj;
    this->jobj = nullptr;
    cls      = env->FindClass(className.c_str());
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), "()J") : nullptr;
  }

  jlong call(JNIEnv *env, jlong def)
  {
    jlong res = def;
    if (methodId)
    {
      jobject pJobject = CS_JOBJECT;
      if (pJobject)
      {
        res = (jlong) env->CallLongMethod(pJobject, methodId);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jobject       jobj;
  jclass        cls;
  jmethodID     methodId;
};

template<typename T0>
class csJavaCallLong1
{
public:
  csJavaCallLong1(JNIEnv *env,
                  jobject jobj,
                  const std::string &className,
                  const std::string &methodName,
                  const std::string &arg0)
  {
    this->obj = nullptr;
    this->jobj = jobj;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + ")J";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }
  csJavaCallLong1(JNIEnv *env,
                  const iObject *obj,
                  const std::string &className,
                  const std::string &methodName,
                  const std::string &arg0)
  {
    this->obj = obj;
    this->jobj = nullptr;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + ")J";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  jlong call(JNIEnv *env, T0 arg0, jlong def)
  {
    jlong res = def;
    if (methodId)
    {
      jobject pJobject = CS_JOBJECT;
      if (pJobject)
      {
        res = (jlong) env->CallLongMethod(pJobject, methodId, arg0);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jobject       jobj;
  jclass        cls;
  jmethodID     methodId;
};

template<typename T0, typename T1>
class csJavaCallLong2
{
public:
  csJavaCallLong2(JNIEnv
                *env,
                  jobject jobj,
                  const std::string &className,
                  const std::string &methodName,
                  const std::string &arg0,
                  const std::string &arg1
                 )
  {
    this->obj = nullptr;
    this->jobj = jobj;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + arg1 + ")J";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  csJavaCallLong2(JNIEnv
             *env,
                  const iObject *obj,
                  const std::string &className,
                  const std::string &methodName,
                  const std::string &arg0,
                  const std::string &arg1
                 )
  {
    this->obj = obj;
    this->jobj = nullptr;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + arg1 + ")J";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  jlong call(JNIEnv *env, T0 arg0, T1 arg1, jlong def)
  {
    jlong res = def;
    if (methodId)
    {
      jobject pJobject = CS_JOBJECT;
      if (pJobject)
      {
        res = (jlong) env->CallLongMethod(pJobject, methodId, arg0, arg1);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jobject       jobj;
  jclass        cls;
  jmethodID     methodId;
};


template<typename T0, typename T1, typename T2>
class csJavaCallLong3
{
public:
  csJavaCallLong3(JNIEnv
                *env,
                  jobject jobj,
                  const std::string &className,
                  const std::string &methodName,
                  const std::string &arg0,
                  const std::string &arg1,
                  const std::string &arg2
                 )
  {
    this->obj = nullptr;
    this->jobj = jobj;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + arg1 + arg2 + ")J";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }
  csJavaCallLong3(JNIEnv
             *env,
                  const iObject *obj,
                  const std::string &className,
                  const std::string &methodName,
                  const std::string &arg0,
                  const std::string &arg1,
                  const std::string &arg2
                 )
  {
    this->obj = obj;
    this->jobj = nullptr;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + arg1 + arg2 + ")J";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  jlong call(JNIEnv *env, T0 arg0, T1 arg1, T2  arg2, jlong def)
  {
    jlong res = def;
    if (methodId)
    {
      jobject pJobject = CS_JOBJECT;
      if (pJobject)
      {
        res = (jlong) env->CallLongMethod(pJobject, methodId, arg0, arg1, arg2);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jobject       jobj;
  jclass        cls;
  jmethodID     methodId;
};





class csJavaCallFloat
{
public:
  csJavaCallFloat(JNIEnv *env, jobject jobj, const std::string &className, const std::string &methodName)
  {
    this->obj = nullptr;
    this->jobj = jobj;
    cls      = env->FindClass(className.c_str());
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), "()F") : nullptr;
  }
  csJavaCallFloat(JNIEnv *env, const iObject *obj, const std::string &className, const std::string &methodName)
  {
    this->obj = obj;
    this->jobj = nullptr;
    cls      = env->FindClass(className.c_str());
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), "()F") : nullptr;
  }

  jfloat call(JNIEnv *env, jfloat def)
  {
    jfloat res = def;
    if (methodId)
    {
      jobject pJobject = CS_JOBJECT;
      if (pJobject)
      {
        res = (jfloat) env->CallFloatMethod(pJobject, methodId);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jobject       jobj;
  jclass        cls;
  jmethodID     methodId;
};

template<typename T0>
class csJavaCallFloat1
{
public:
  csJavaCallFloat1(JNIEnv *env,
                   jobject jobj,
                   const std::string &className,
                   const std::string &methodName,
                   const std::string &arg0)
  {
    this->obj = nullptr;
    this->jobj = jobj;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + ")F";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }
  csJavaCallFloat1(JNIEnv *env,
                   const iObject *obj,
                   const std::string &className,
                   const std::string &methodName,
                   const std::string &arg0)
  {
    this->obj = obj;
    this->jobj = nullptr;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + ")F";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  jfloat call(JNIEnv *env, T0 arg0, jfloat def)
  {
    jfloat res = def;
    if (methodId)
    {
      jobject pJobject = CS_JOBJECT;
      if (pJobject)
      {
        res = (jfloat) env->CallFloatMethod(pJobject, methodId, arg0);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jobject       jobj;
  jclass        cls;
  jmethodID     methodId;
};

template<typename T0, typename T1>
class csJavaCallFloat2
{
public:
  csJavaCallFloat2(JNIEnv
                 *env,
                   jobject jobj,
                   const std::string &className,
                   const std::string &methodName,
                   const std::string &arg0,
                   const std::string &arg1
                  )
  {
    this->obj = nullptr;
    this->jobj = jobj;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + arg1 + ")F";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }
  csJavaCallFloat2(JNIEnv
                *env,
                   const iObject *obj,
                   const std::string &className,
                   const std::string &methodName,
                   const std::string &arg0,
                   const std::string &arg1
                  )
  {
    this->obj = obj;
    this->jobj = nullptr;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + arg1 + ")F";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  jfloat call(JNIEnv *env, T0 arg0, T1 arg1, jfloat def)
  {
    jfloat res = def;
    if (methodId)
    {
      jobject pJobject = CS_JOBJECT;
      if (pJobject)
      {
        res = (jfloat) env->CallFloatMethod(pJobject, methodId, arg0, arg1);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jobject       jobj;
  jclass        cls;
  jmethodID     methodId;
};


template<typename T0, typename T1, typename T2>
class csJavaCallFloat3
{
public:
  csJavaCallFloat3(JNIEnv
                 *env,
                   jobject jobj,
                   const std::string &className,
                   const std::string &methodName,
                   const std::string &arg0,
                   const std::string &arg1,
                   const std::string &arg2
                  )
  {
    this->obj = nullptr;
    this->jobj = jobj;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + arg1 + arg2 + ")F";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }
  csJavaCallFloat3(JNIEnv
                *env,
                   const iObject *obj,
                   const std::string &className,
                   const std::string &methodName,
                   const std::string &arg0,
                   const std::string &arg1,
                   const std::string &arg2
                  )
  {
    this->obj = obj;
    this->jobj = nullptr;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + arg1 + arg2 + ")F";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  jfloat call(JNIEnv *env, T0 arg0, T1 arg1, T2  arg2, jfloat def)
  {
    jfloat res = def;
    if (methodId)
    {
      jobject pJobject = CS_JOBJECT;
      if (pJobject)
      {
        res = (jfloat) env->CallFloatMethod(pJobject, methodId, arg0, arg1, arg2);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jobject       jobj;
  jclass        cls;
  jmethodID     methodId;
};


class csJavaCallDouble
{
public:
  csJavaCallDouble(JNIEnv *env, jobject jobj, const std::string &className, const std::string &methodName)
  {
    this->obj = nullptr;
    this->jobj = jobj;
    cls      = env->FindClass(className.c_str());
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), "()D") : nullptr;
  }
  csJavaCallDouble(JNIEnv *env, const iObject *obj, const std::string &className, const std::string &methodName)
  {
    this->obj = obj;
    this->jobj = nullptr;
    cls      = env->FindClass(className.c_str());
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), "()D") : nullptr;
  }

  jdouble call(JNIEnv *env, jdouble def)
  {
    jdouble res = def;
    if (methodId)
    {
      jobject pJobject = CS_JOBJECT;
      if (pJobject)
      {
        res = (jdouble) env->CallDoubleMethod(pJobject, methodId);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jobject       jobj;
  jclass        cls;
  jmethodID     methodId;
};

template<typename T0>
class csJavaCallDouble1
{
public:
  csJavaCallDouble1(JNIEnv *env,
                    jobject jobj,
                    const std::string &className,
                    const std::string &methodName,
                    const std::string &arg0)
  {
    this->obj = nullptr;
    this->jobj = jobj;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + ")D";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }
  csJavaCallDouble1(JNIEnv *env,
                    const iObject *obj,
                    const std::string &className,
                    const std::string &methodName,
                    const std::string &arg0)
  {
    this->obj = obj;
    this->jobj = nullptr;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + ")D";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  jdouble call(JNIEnv *env, T0 arg0, jdouble def)
  {
    jdouble res = def;
    if (methodId)
    {
      jobject pJobject = CS_JOBJECT;
      if (pJobject)
      {
        res = (jdouble) env->CallDoubleMethod(pJobject, methodId, arg0);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jobject       jobj;
  jclass        cls;
  jmethodID     methodId;
};

template<typename T0, typename T1>
class csJavaCallDouble2
{
public:
  csJavaCallDouble2(JNIEnv
                  *env,
                    jobject jobj,
                    const std::string &className,
                    const std::string &methodName,
                    const std::string &arg0,
                    const std::string &arg1
                   )
  {
    this->obj = nullptr;
    this->jobj = jobj;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + arg1 + ")D";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  csJavaCallDouble2(JNIEnv
                 *env,
                    const iObject *obj,
                    const std::string &className,
                    const std::string &methodName,
                    const std::string &arg0,
                    const std::string &arg1
                   )
  {
    this->obj = obj;
    this->jobj = nullptr;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + arg1 + ")D";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  jdouble call(JNIEnv *env, T0 arg0, T1 arg1, jdouble def)
  {
    jdouble res = def;
    if (methodId)
    {
      jobject pJobject = CS_JOBJECT;
      if (pJobject)
      {
        res = (jdouble) env->CallDoubleMethod(pJobject, methodId, arg0, arg1);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jobject       jobj;
  jclass        cls;
  jmethodID     methodId;
};


template<typename T0, typename T1, typename T2>
class csJavaCallDouble3
{
public:
  csJavaCallDouble3(JNIEnv
                  *env,
                    jobject jobj,
                    const std::string &className,
                    const std::string &methodName,
                    const std::string &arg0,
                    const std::string &arg1,
                    const std::string &arg2
                   )
  {
    this->obj = nullptr;
    this->jobj = jobj;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + arg1 + arg2 + ")D";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }
  csJavaCallDouble3(JNIEnv
                 *env,
                    const iObject *obj,
                    const std::string &className,
                    const std::string &methodName,
                    const std::string &arg0,
                    const std::string &arg1,
                    const std::string &arg2
                   )
  {
    this->obj = obj;
    this->jobj = nullptr;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + arg1 + arg2 + ")D";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  jdouble call(JNIEnv *env, T0 arg0, T1 arg1, T2  arg2, jdouble def)
  {
    jdouble res = def;
    if (methodId)
    {
      jobject pJobject = CS_JOBJECT;
      if (pJobject)
      {
        res = (jdouble) env->CallDoubleMethod(pJobject, methodId, arg0, arg1, arg2);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jobject       jobj;
  jclass        cls;
  jmethodID     methodId;
};




template<typename R>
class csJavaCallObject
{
public:
  csJavaCallObject(JNIEnv *env, jobject jobj, const std::string &className, const std::string &methodName, const std::string &returnType)
  {
    this->obj = nullptr;
    this->jobj = jobj;
    cls      = env->FindClass(className.c_str());
    std::string sig = "()" + returnType;
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }
  csJavaCallObject(JNIEnv *env, const iObject *obj, const std::string &className, const std::string &methodName, const std::string &returnType)
  {
    this->obj = obj;
    this->jobj = nullptr;
    cls      = env->FindClass(className.c_str());
    std::string sig = "()" + returnType;
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  R call(JNIEnv *env, R def)
  {
    R res = def;
    if (methodId)
    {
      jobject pJobject = CS_JOBJECT;
      if (pJobject)
      {
        res = (R) env->CallObjectMethod(pJobject, methodId);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jobject       jobj;
  jclass        cls;
  jmethodID     methodId;
};

template<typename R, typename T0>
class csJavaCallObject1
{
public:
  csJavaCallObject1(JNIEnv *env,
                    jobject jobj,
                    const std::string &className,
                    const std::string &methodName,
                    const std::string &returnType,
                    const std::string &arg0)
  {
    this->obj = nullptr;
    this->jobj = jobj;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + ")" + returnType;
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }
  csJavaCallObject1(JNIEnv *env,
                    const iObject *obj,
                    const std::string &className,
                    const std::string &methodName,
                    const std::string &returnType,
                    const std::string &arg0)
  {
    this->obj = obj;
    this->jobj = nullptr;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + ")" + returnType;
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  R call(JNIEnv *env, T0 arg0, R def)
  {
    R res = def;
    if (methodId)
    {
      jobject pJobject = CS_JOBJECT;
      if (pJobject)
      {
        res = (R) env->CallObjectMethod(pJobject, methodId, arg0);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jobject       jobj;
  jclass        cls;
  jmethodID     methodId;
};

template<typename R, typename T0, typename T1>
class csJavaCallObject2
{
public:
  csJavaCallObject2(JNIEnv
                  *env,
                    jobject jobj,
                    const std::string &className,
                    const std::string &methodName,
                    const std::string &returnType,
                    const std::string &arg0,
                    const std::string &arg1
                   )
  {
    this->obj = nullptr;
    this->jobj = jobj;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + arg1 + ")" + returnType;
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }
  csJavaCallObject2(JNIEnv
                  *env,
                    const iObject *obj,
                    const std::string &className,
                    const std::string &methodName,
                    const std::string &returnType,
                    const std::string &arg0,
                    const std::string &arg1
                   )
  {
    this->obj = obj;
    this->jobj = nullptr;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + arg1 + ")" + returnType;
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  R call(JNIEnv *env, T0 arg0, T1 arg1, R def)
  {
    R res = def;
    if (methodId)
    {
      jobject pJobject = CS_JOBJECT;
      if (pJobject)
      {
        res = (R) env->CallObjectMethod(pJobject, methodId, arg0, arg1);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jobject       jobj;
  jclass        cls;
  jmethodID     methodId;
};


template<typename R, typename T0, typename T1, typename T2>
class csJavaCallObject3
{
public:
  csJavaCallObject3(JNIEnv
                  *env,
                    jobject jobj,
                    const std::string &className,
                    const std::string &methodName,
                    const std::string &returnType,
                    const std::string &arg0,
                    const std::string &arg1,
                    const std::string &arg2
                   )
  {
    this->obj = nullptr;
    this->jobj = jobj;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + arg1 + arg2 + ")" + returnType;
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }
  csJavaCallObject3(JNIEnv
                  *env,
                    const iObject *obj,
                    const std::string &className,
                    const std::string &methodName,
                    const std::string &returnType,
                    const std::string &arg0,
                    const std::string &arg1,
                    const std::string &arg2
                   )
  {
    this->obj = obj;
    this->jobj = nullptr;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + arg1 + arg2 + ")" + returnType;
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  R call(JNIEnv *env, T0 arg0, T1 arg1, T2  arg2, R def)
  {
    R res = def;
    if (methodId)
    {
      jobject pJobject = CS_JOBJECT;
      if (pJobject)
      {
        res = (R) env->CallObjectMethod(pJobject, methodId, arg0, arg1, arg2);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jobject       jobj;
  jclass        cls;
  jmethodID     methodId;
};


#undef CS_JOBJECT

}

