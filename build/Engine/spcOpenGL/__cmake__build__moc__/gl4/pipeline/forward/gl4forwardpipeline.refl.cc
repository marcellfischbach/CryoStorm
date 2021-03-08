spc::opengl::GL4ForwardPipelineClass *spc::opengl::GL4ForwardPipelineClass::Get()
{
  static spc::opengl::GL4ForwardPipelineClass static_class;
  return &static_class;
};

spc::opengl::GL4ForwardPipelineClass::GL4ForwardPipelineClass()
  : spc::Class("spc::opengl::GL4ForwardPipeline")
{
  AddSuperClass(iRenderPipeline::GetStaticClass());
}

spc::iObject *spc::opengl::GL4ForwardPipelineClass::CreateInstance() const
{
  return static_cast<spc::iObject*>(new spc::opengl::GL4ForwardPipeline());
}

const spc::Class *spc::opengl::GL4ForwardPipeline::GetClass() const
{
  return spc::opengl::GL4ForwardPipelineClass::Get();
}

const spc::Class *spc::opengl::GL4ForwardPipeline::GetStaticClass()
{
  return spc::opengl::GL4ForwardPipelineClass::Get();
}

void *spc::opengl::GL4ForwardPipeline::QueryClass(const spc::Class* clazz) 
{
  if (clazz == spc::opengl::GL4ForwardPipelineClass::Get())
  {
    return static_cast<spc::opengl::GL4ForwardPipeline*>(this);
  }
  void *super = nullptr;
  super = iRenderPipeline::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

const void *spc::opengl::GL4ForwardPipeline::QueryClass(const spc::Class* clazz) const 
{
  if (clazz == spc::opengl::GL4ForwardPipelineClass::Get())
  {
    return static_cast<const spc::opengl::GL4ForwardPipeline*>(this);
  }
  const void *super = nullptr;
  super = iRenderPipeline::QueryClass(clazz);
  if (super)
  {
    return super;
  }
  return nullptr;
}

