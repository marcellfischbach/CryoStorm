spc::iRenderPipelineClass *spc::iRenderPipelineClass::Get()
{
  static spc::iRenderPipelineClass static_class;
  return &static_class;
};

spc::iRenderPipelineClass::iRenderPipelineClass()
  : spc::Class("spc::iRenderPipeline")
{
  AddSuperClass(iObject::GetStaticClass());
}

spc::iObject *spc::iRenderPipelineClass::CreateInstance() const
{
  throw spc::InstanciationException("spc::iRenderPipeline");
}

const spc::Class *spc::iRenderPipeline::GetClass() const
{
  return spc::iRenderPipelineClass::Get();
}

const spc::Class *spc::iRenderPipeline::GetStaticClass()
{
  return spc::iRenderPipelineClass::Get();
}

void *spc::iRenderPipeline::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::iRenderPipelineClass::Get())
  {
    return static_cast<spc::iRenderPipeline*>(this);
  }
  void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::iRenderPipeline::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::iRenderPipelineClass::Get())
  {
    return static_cast<const spc::iRenderPipeline*>(this);
  }
  const void *super = nullptr;
  super = iObject::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

