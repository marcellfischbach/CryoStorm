
#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/loaders/scene/csEntityStateLoaderBase.hh>

namespace cs
{

class csSpatialState;

class csSpatialStateLoader : public csEntityStateLoaderBase
{
protected:
  explicit csSpatialStateLoader(int priority);

  static void LoadSpatialState (const csCryoFileElement *stateElement, csSpatialState *spatialState) ;
};

} // cs
