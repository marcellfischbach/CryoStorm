//
// Created by MCEL on 08.05.2024.
//

#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>
#include <jni.h>


namespace ce
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


class JavaCallVoid
{
public:
  JavaCallVoid(JNIEnv *env, const iObject *obj, const std::string &className, const std::string &methodName)
  {
    this->obj = obj;
    cls      = env->FindClass(className.c_str());
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), "()V") : nullptr;
  }

  void call(JNIEnv *env)
  {
    if (methodId)
    {
      jobject pJobject = obj->GetJObject();
      if (pJobject)
      {
        env->CallVoidMethod(pJobject, methodId);
      }
    }
  }

private:
  const iObject *obj;
  jclass        cls;
  jmethodID     methodId;
};

template<typename T0>
class JavaCallVoid1
{
public:
  JavaCallVoid1(JNIEnv *env,
                const iObject *obj,
                const std::string &className,
                const std::string &methodName,
                const std::string &arg0)
  {
    this->obj = obj;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + ")V";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  void call(JNIEnv *env, T0 arg0)
  {
    if (methodId)
    {
      jobject pJobject = obj->GetJObject();
      if (pJobject)
      {
        env->CallVoidMethod(pJobject, methodId, arg0);
      }
    }
  }

private:
  const iObject *obj;
  jclass        cls;
  jmethodID     methodId;
};

template<typename T0, typename T1>
class JavaCallVoid2
{
public:
  JavaCallVoid2(JNIEnv
             *env,
                const iObject *obj,
                const std::string &className,
                const std::string &methodName,
                const std::string &arg0,
                const std::string &arg1
               )
  {
    this->obj = obj;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + arg1 + ")V";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  void call(JNIEnv *env, T0 arg0, T1 arg1)
  {
    if (methodId)
    {
      jobject pJobject = obj->GetJObject();
      if (pJobject)
      {
        env->CallVoidMethod(pJobject, methodId, arg0, arg1);
      }
    }
  }

private:
  const iObject *obj;
  jclass        cls;
  jmethodID     methodId;
};


template<typename T0, typename T1, typename T2>
class JavaCallVoid3
{
public:
  JavaCallVoid3(JNIEnv
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
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + arg1 + arg2 + ")V";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  jlong call(JNIEnv *env, T0 arg0, T1 arg1, T2  arg2)
  {
    if (methodId)
    {
      jobject pJobject = obj->GetJObject();
      if (pJobject)
      {
        env->CallVoidMethod(pJobject, methodId, arg0, arg1, arg2);
      }
    }
  }

private:
  const iObject *obj;
  jclass        cls;
  jmethodID     methodId;
};



class JavaCallBoolean
{
public:
  JavaCallBoolean(JNIEnv *env, const iObject *obj, const std::string &className, const std::string &methodName)
  {
    this->obj = obj;
    cls      = env->FindClass(className.c_str());
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), "()Z") : nullptr;
  }

  jboolean call(JNIEnv *env, jboolean def)
  {
    jboolean res = def;
    if (methodId)
    {
      jobject pJobject = obj->GetJObject();
      if (pJobject)
      {
        res = (jboolean) env->CallBooleanMethod(pJobject, methodId);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jclass        cls;
  jmethodID     methodId;
};

template<typename T0>
class JavaCallBoolean1
{
public:
  JavaCallBoolean1(JNIEnv *env,
               const iObject *obj,
               const std::string &className,
               const std::string &methodName,
               const std::string &arg0)
  {
    this->obj = obj;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + ")Z";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  jboolean call(JNIEnv *env, T0 arg0, jboolean def)
  {
    jboolean res = def;
    if (methodId)
    {
      jobject pJobject = obj->GetJObject();
      if (pJobject)
      {
        res = (jboolean) env->CallBooleanMethod(pJobject, methodId, arg0);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jclass        cls;
  jmethodID     methodId;
};

template<typename T0, typename T1>
class JavaCallBoolean2
{
public:
  JavaCallBoolean2(JNIEnv
               *env,
               const iObject *obj,
               const std::string &className,
               const std::string &methodName,
               const std::string &arg0,
               const std::string &arg1
              )
  {
    this->obj = obj;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + arg1 + ")Z";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  jboolean call(JNIEnv *env, T0 arg0, T1 arg1, jboolean def)
  {
    jboolean res = def;
    if (methodId)
    {
      jobject pJobject = obj->GetJObject();
      if (pJobject)
      {
        res = (jboolean) env->CallBooleanMethod(pJobject, methodId, arg0, arg1);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jclass        cls;
  jmethodID     methodId;
};


template<typename T0, typename T1, typename T2>
class JavaCallBoolean3
{
public:
  JavaCallBoolean3(JNIEnv
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
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + arg1 + arg2 + ")Z";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  jboolean call(JNIEnv *env, T0 arg0, T1 arg1, T2  arg2, jboolean def)
  {
    jboolean res = def;
    if (methodId)
    {
      jobject pJobject = obj->GetJObject();
      if (pJobject)
      {
        res = (jboolean) env->CallBooleanMethod(pJobject, methodId, arg0, arg1, arg2);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jclass        cls;
  jmethodID     methodId;
};



class JavaCallByte
{
public:
  JavaCallByte(JNIEnv *env, const iObject *obj, const std::string &className, const std::string &methodName)
  {
    this->obj = obj;
    cls      = env->FindClass(className.c_str());
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), "()B") : nullptr;
  }

  jbyte call(JNIEnv *env, jbyte def)
  {
    jbyte res = def;
    if (methodId)
    {
      jobject pJobject = obj->GetJObject();
      if (pJobject)
      {
        res = (jbyte) env->CallByteMethod(pJobject, methodId);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jclass        cls;
  jmethodID     methodId;
};

template<typename T0>
class JavaCallByte1
{
public:
  JavaCallByte1(JNIEnv *env,
               const iObject *obj,
               const std::string &className,
               const std::string &methodName,
               const std::string &arg0)
  {
    this->obj = obj;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + ")B";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  jbyte call(JNIEnv *env, T0 arg0, jbyte def)
  {
    jbyte res = def;
    if (methodId)
    {
      jobject pJobject = obj->GetJObject();
      if (pJobject)
      {
        res = (jbyte) env->CallByteMethod(pJobject, methodId, arg0);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jclass        cls;
  jmethodID     methodId;
};

template<typename T0, typename T1>
class JavaCallByte2
{
public:
  JavaCallByte2(JNIEnv
               *env,
               const iObject *obj,
               const std::string &className,
               const std::string &methodName,
               const std::string &arg0,
               const std::string &arg1
              )
  {
    this->obj = obj;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + arg1 + ")B";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  jbyte call(JNIEnv *env, T0 arg0, T1 arg1, jbyte def)
  {
    jbyte res = def;
    if (methodId)
    {
      jobject pJobject = obj->GetJObject();
      if (pJobject)
      {
        res = (jbyte) env->CallByteMethod(pJobject, methodId, arg0, arg1);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jclass        cls;
  jmethodID     methodId;
};


template<typename T0, typename T1, typename T2>
class JavaCallByte3
{
public:
  JavaCallByte3(JNIEnv
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
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + arg1 + arg2 + ")B";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  jbyte call(JNIEnv *env, T0 arg0, T1 arg1, T2  arg2, jbyte def)
  {
    jbyte res = def;
    if (methodId)
    {
      jobject pJobject = obj->GetJObject();
      if (pJobject)
      {
        res = (jbyte) env->CallByteMethod(pJobject, methodId, arg0, arg1, arg2);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jclass        cls;
  jmethodID     methodId;
};




class JavaCallShort
{
public:
  JavaCallShort(JNIEnv *env, const iObject *obj, const std::string &className, const std::string &methodName)
  {
    this->obj = obj;
    cls      = env->FindClass(className.c_str());
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), "()S") : nullptr;
  }

  jshort call(JNIEnv *env, jshort def)
  {
    jshort res = def;
    if (methodId)
    {
      jobject pJobject = obj->GetJObject();
      if (pJobject)
      {
        res = (jshort) env->CallShortMethod(pJobject, methodId);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jclass        cls;
  jmethodID     methodId;
};

template<typename T0>
class JavaCallShort1
{
public:
  JavaCallShort1(JNIEnv *env,
               const iObject *obj,
               const std::string &className,
               const std::string &methodName,
               const std::string &arg0)
  {
    this->obj = obj;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + ")S";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  jshort call(JNIEnv *env, T0 arg0, jshort def)
  {
    jshort res = def;
    if (methodId)
    {
      jobject pJobject = obj->GetJObject();
      if (pJobject)
      {
        res = (jshort) env->CallShortMethod(pJobject, methodId, arg0);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jclass        cls;
  jmethodID     methodId;
};

template<typename T0, typename T1>
class JavaCallShort2
{
public:
  JavaCallShort2(JNIEnv
               *env,
               const iObject *obj,
               const std::string &className,
               const std::string &methodName,
               const std::string &arg0,
               const std::string &arg1
              )
  {
    this->obj = obj;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + arg1 + ")S";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  jshort call(JNIEnv *env, T0 arg0, T1 arg1, jshort def)
  {
    jshort res = def;
    if (methodId)
    {
      jobject pJobject = obj->GetJObject();
      if (pJobject)
      {
        res = (jshort) env->CallShortMethod(pJobject, methodId, arg0, arg1);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jclass        cls;
  jmethodID     methodId;
};


template<typename T0, typename T1, typename T2>
class JavaCallShort3
{
public:
  JavaCallShort3(JNIEnv
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
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + arg1 + arg2 + ")S";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  jshort call(JNIEnv *env, T0 arg0, T1 arg1, T2  arg2, jshort def)
  {
    jshort res = def;
    if (methodId)
    {
      jobject pJobject = obj->GetJObject();
      if (pJobject)
      {
        res = (jshort) env->CallShortMethod(pJobject, methodId, arg0, arg1, arg2);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jclass        cls;
  jmethodID     methodId;
};







class JavaCallInt
{
public:
  JavaCallInt(JNIEnv *env, const iObject *obj, const std::string &className, const std::string &methodName)
  {
    this->obj = obj;
    cls      = env->FindClass(className.c_str());
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), "()I") : nullptr;
  }

  jint call(JNIEnv *env, jint def)
  {
    jint res = def;
    if (methodId)
    {
      jobject pJobject = obj->GetJObject();
      if (pJobject)
      {
        res = (jint) env->CallIntMethod(pJobject, methodId);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jclass        cls;
  jmethodID     methodId;
};

template<typename T0>
class JavaCallInt1
{
public:
  JavaCallInt1(JNIEnv *env,
                const iObject *obj,
                const std::string &className,
                const std::string &methodName,
                const std::string &arg0)
  {
    this->obj = obj;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + ")I";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  jint call(JNIEnv *env, T0 arg0, jint def)
  {
    jint res = def;
    if (methodId)
    {
      jobject pJobject = obj->GetJObject();
      if (pJobject)
      {
        res = (jint) env->CallIntMethod(pJobject, methodId, arg0);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jclass        cls;
  jmethodID     methodId;
};

template<typename T0, typename T1>
class JavaCallInt2
{
public:
  JavaCallInt2(JNIEnv
                *env,
                const iObject *obj,
                const std::string &className,
                const std::string &methodName,
                const std::string &arg0,
                const std::string &arg1
               )
  {
    this->obj = obj;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + arg1 + ")I";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  jint call(JNIEnv *env, T0 arg0, T1 arg1, jint def)
  {
    jint res = def;
    if (methodId)
    {
      jobject pJobject = obj->GetJObject();
      if (pJobject)
      {
        res = (jint) env->CallIntMethod(pJobject, methodId, arg0, arg1);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jclass        cls;
  jmethodID     methodId;
};


template<typename T0, typename T1, typename T2>
class JavaCallInt3
{
public:
  JavaCallInt3(JNIEnv
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
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + arg1 + arg2 + ")I";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  jint call(JNIEnv *env, T0 arg0, T1 arg1, T2  arg2, jint def)
  {
    jint res = def;
    if (methodId)
    {
      jobject pJobject = obj->GetJObject();
      if (pJobject)
      {
        res = (jint) env->CallIntMethod(pJobject, methodId, arg0, arg1, arg2);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jclass        cls;
  jmethodID     methodId;
};



class JavaCallLong
{
public:
  JavaCallLong(JNIEnv *env, const iObject *obj, const std::string &className, const std::string &methodName)
  {
    this->obj = obj;
    cls      = env->FindClass(className.c_str());
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), "()J") : nullptr;
  }

  jlong call(JNIEnv *env, jlong def)
  {
    jlong res = def;
    if (methodId)
    {
      jobject pJobject = obj->GetJObject();
      if (pJobject)
      {
        res = (jlong) env->CallLongMethod(pJobject, methodId);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jclass        cls;
  jmethodID     methodId;
};

template<typename T0>
class JavaCallLong1
{
public:
  JavaCallLong1(JNIEnv *env,
                const iObject *obj,
                const std::string &className,
                const std::string &methodName,
                const std::string &arg0)
  {
    this->obj = obj;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + ")J";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  jlong call(JNIEnv *env, T0 arg0, jlong def)
  {
    jlong res = def;
    if (methodId)
    {
      jobject pJobject = obj->GetJObject();
      if (pJobject)
      {
        res = (jlong) env->CallLongMethod(pJobject, methodId, arg0);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jclass        cls;
  jmethodID     methodId;
};

template<typename T0, typename T1>
class JavaCallLong2
{
public:
  JavaCallLong2(JNIEnv
             *env,
                const iObject *obj,
                const std::string &className,
                const std::string &methodName,
                const std::string &arg0,
                const std::string &arg1
               )
  {
    this->obj = obj;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + arg1 + ")J";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  jlong call(JNIEnv *env, T0 arg0, T1 arg1, jlong def)
  {
    jlong res = def;
    if (methodId)
    {
      jobject pJobject = obj->GetJObject();
      if (pJobject)
      {
        res = (jlong) env->CallLongMethod(pJobject, methodId, arg0, arg1);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jclass        cls;
  jmethodID     methodId;
};


template<typename T0, typename T1, typename T2>
class JavaCallLong3
{
public:
  JavaCallLong3(JNIEnv
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
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + arg1 + arg2 + ")J";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  jlong call(JNIEnv *env, T0 arg0, T1 arg1, T2  arg2, jlong def)
  {
    jlong res = def;
    if (methodId)
    {
      jobject pJobject = obj->GetJObject();
      if (pJobject)
      {
        res = (jlong) env->CallLongMethod(pJobject, methodId, arg0, arg1, arg2);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jclass        cls;
  jmethodID     methodId;
};





class JavaCallFloat
{
public:
  JavaCallFloat(JNIEnv *env, const iObject *obj, const std::string &className, const std::string &methodName)
  {
    this->obj = obj;
    cls      = env->FindClass(className.c_str());
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), "()F") : nullptr;
  }

  jfloat call(JNIEnv *env, jfloat def)
  {
    jfloat res = def;
    if (methodId)
    {
      jobject pJobject = obj->GetJObject();
      if (pJobject)
      {
        res = (jfloat) env->CallFloatMethod(pJobject, methodId);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jclass        cls;
  jmethodID     methodId;
};

template<typename T0>
class JavaCallFloat1
{
public:
  JavaCallFloat1(JNIEnv *env,
                const iObject *obj,
                const std::string &className,
                const std::string &methodName,
                const std::string &arg0)
  {
    this->obj = obj;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + ")F";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  jfloat call(JNIEnv *env, T0 arg0, jfloat def)
  {
    jfloat res = def;
    if (methodId)
    {
      jobject pJobject = obj->GetJObject();
      if (pJobject)
      {
        res = (jfloat) env->CallFloatMethod(pJobject, methodId, arg0);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jclass        cls;
  jmethodID     methodId;
};

template<typename T0, typename T1>
class JavaCallFloat2
{
public:
  JavaCallFloat2(JNIEnv
                *env,
                const iObject *obj,
                const std::string &className,
                const std::string &methodName,
                const std::string &arg0,
                const std::string &arg1
               )
  {
    this->obj = obj;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + arg1 + ")F";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  jfloat call(JNIEnv *env, T0 arg0, T1 arg1, jfloat def)
  {
    jfloat res = def;
    if (methodId)
    {
      jobject pJobject = obj->GetJObject();
      if (pJobject)
      {
        res = (jfloat) env->CallFloatMethod(pJobject, methodId, arg0, arg1);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jclass        cls;
  jmethodID     methodId;
};


template<typename T0, typename T1, typename T2>
class JavaCallFloat3
{
public:
  JavaCallFloat3(JNIEnv
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
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + arg1 + arg2 + ")F";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  jfloat call(JNIEnv *env, T0 arg0, T1 arg1, T2  arg2, jfloat def)
  {
    jfloat res = def;
    if (methodId)
    {
      jobject pJobject = obj->GetJObject();
      if (pJobject)
      {
        res = (jfloat) env->CallFloatMethod(pJobject, methodId, arg0, arg1, arg2);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jclass        cls;
  jmethodID     methodId;
};


class JavaCallDouble
{
public:
  JavaCallDouble(JNIEnv *env, const iObject *obj, const std::string &className, const std::string &methodName)
  {
    this->obj = obj;
    cls      = env->FindClass(className.c_str());
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), "()D") : nullptr;
  }

  jdouble call(JNIEnv *env, jdouble def)
  {
    jdouble res = def;
    if (methodId)
    {
      jobject pJobject = obj->GetJObject();
      if (pJobject)
      {
        res = (jdouble) env->CallDoubleMethod(pJobject, methodId);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jclass        cls;
  jmethodID     methodId;
};

template<typename T0>
class JavaCallDouble1
{
public:
  JavaCallDouble1(JNIEnv *env,
                 const iObject *obj,
                 const std::string &className,
                 const std::string &methodName,
                 const std::string &arg0)
  {
    this->obj = obj;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + ")D";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  jdouble call(JNIEnv *env, T0 arg0, jdouble def)
  {
    jdouble res = def;
    if (methodId)
    {
      jobject pJobject = obj->GetJObject();
      if (pJobject)
      {
        res = (jdouble) env->CallDoubleMethod(pJobject, methodId, arg0);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jclass        cls;
  jmethodID     methodId;
};

template<typename T0, typename T1>
class JavaCallDouble2
{
public:
  JavaCallDouble2(JNIEnv
                 *env,
                 const iObject *obj,
                 const std::string &className,
                 const std::string &methodName,
                 const std::string &arg0,
                 const std::string &arg1
                )
  {
    this->obj = obj;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + arg1 + ")D";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  jdouble call(JNIEnv *env, T0 arg0, T1 arg1, jdouble def)
  {
    jdouble res = def;
    if (methodId)
    {
      jobject pJobject = obj->GetJObject();
      if (pJobject)
      {
        res = (jdouble) env->CallDoubleMethod(pJobject, methodId, arg0, arg1);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jclass        cls;
  jmethodID     methodId;
};


template<typename T0, typename T1, typename T2>
class JavaCallDouble3
{
public:
  JavaCallDouble3(JNIEnv
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
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + arg1 + arg2 + ")D";
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  jdouble call(JNIEnv *env, T0 arg0, T1 arg1, T2  arg2, jdouble def)
  {
    jdouble res = def;
    if (methodId)
    {
      jobject pJobject = obj->GetJObject();
      if (pJobject)
      {
        res = (jdouble) env->CallDoubleMethod(pJobject, methodId, arg0, arg1, arg2);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jclass        cls;
  jmethodID     methodId;
};




template<typename R>
class JavaCallObject
{
public:
  JavaCallObject(JNIEnv *env, const iObject *obj, const std::string &className, const std::string &methodName, const std::string &returnType)
  {
    this->obj = obj;
    cls      = env->FindClass(className.c_str());
    std::string sig = "()" + returnType;
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  R call(JNIEnv *env, R def)
  {
    R res = def;
    if (methodId)
    {
      jobject pJobject = obj->GetJObject();
      if (pJobject)
      {
        res = (R) env->CallObjectMethod(pJobject, methodId);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jclass        cls;
  jmethodID     methodId;
};

template<typename R, typename T0>
class JavaCallObject1
{
public:
  JavaCallObject1(JNIEnv *env,
                  const iObject *obj,
                  const std::string &className,
                  const std::string &methodName,
                  const std::string &returnType,
                  const std::string &arg0)
  {
    this->obj = obj;
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + ")" + returnType;
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  R call(JNIEnv *env, T0 arg0, R def)
  {
    R res = def;
    if (methodId)
    {
      R pJobject = obj->GetJObject();
      if (pJobject)
      {
        res = (R) env->CallObjectMethod(pJobject, methodId, arg0);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jclass        cls;
  jmethodID     methodId;
};

template<typename R, typename T0, typename T1>
class JavaCallObject2
{
public:
  JavaCallObject2(JNIEnv
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
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + arg1 + ")" + returnType;
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  R call(JNIEnv *env, T0 arg0, T1 arg1, R def)
  {
    R res = def;
    if (methodId)
    {
      jobject pJobject = obj->GetJObject();
      if (pJobject)
      {
        res = (R) env->CallObjectMethod(pJobject, methodId, arg0, arg1);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jclass        cls;
  jmethodID     methodId;
};


template<typename R, typename T0, typename T1, typename T2>
class JavaCallObject3
{
public:
  JavaCallObject3(JNIEnv
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
    cls             = env->FindClass(className.c_str());
    std::string sig = std::string("(") + arg0 + arg1 + arg2 + ")" + returnType;
    methodId = cls ? env->GetMethodID(cls, methodName.c_str(), sig.c_str()) : nullptr;
  }

  R call(JNIEnv *env, T0 arg0, T1 arg1, T2  arg2, R def)
  {
    R res = def;
    if (methodId)
    {
      R pJobject = obj->GetJObject();
      if (pJobject)
      {
        res = (R) env->CallObjectMethod(pJobject, methodId, arg0, arg1, arg2);
      }
    }
    return res;
  }

private:
  const iObject *obj;
  jclass        cls;
  jmethodID     methodId;
};


}

