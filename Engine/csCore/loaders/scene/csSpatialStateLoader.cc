//
// Created by Marcell on 13.03.2025.
//

#include <csCore/loaders/scene/csSpatialStateLoader.hh>
#include <csCore/entity/csSpatialState.hh>
#include <csCore/math/csMatrix4f.hh>

namespace cs
{

csSpatialStateLoader::csSpatialStateLoader(int priority)
    : csEntityStateLoaderBase(priority)
{

}

void csSpatialStateLoader::LoadSpatialState(const csCryoFileElement *stateElement, csSpatialState *spatialState)
{
  if (!stateElement)
  {
    return;
  }

  const csCryoFileElement *transformElement = stateElement->GetChild("transform");
  if (!transformElement)
  {
    return;
  }

  const csCryoFileElement *matrixElement = transformElement->GetChild("matrix4");
  if (matrixElement)
  {
    csMatrix4f mat(matrixElement->GetAttribute(0, 1.0f),
                   matrixElement->GetAttribute(1, 0.0f),
                   matrixElement->GetAttribute(2, 0.0f),
                   matrixElement->GetAttribute(3, 0.0f),
                   matrixElement->GetAttribute(4, 0.0f),
                   matrixElement->GetAttribute(5, 1.0f),
                   matrixElement->GetAttribute(6, 0.0f),
                   matrixElement->GetAttribute(7, 0.0f),
                   matrixElement->GetAttribute(8, 0.0f),
                   matrixElement->GetAttribute(9, 0.0f),
                   matrixElement->GetAttribute(10, 1.0f),
                   matrixElement->GetAttribute(11, 0.0f),
                   matrixElement->GetAttribute(12, 0.0f),
                   matrixElement->GetAttribute(13, 0.0f),
                   matrixElement->GetAttribute(14, 0.0f),
                   matrixElement->GetAttribute(15, 1.0f));
    spatialState->SetLocalMatrix(mat);
  }


}

} // cs