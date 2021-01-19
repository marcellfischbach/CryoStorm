
#include <spcCore/entity/transform.hh>
#include <spcCore/entity/spatialstate.hh>

namespace spc
{

Transform::Transform(SpatialState *spatialState)
: m_spatialState(spatialState)
, m_translation(0.0f, 0.0f, 0.0f)
, m_rotation(0.0f, 0.0f, 0.0f, 1.0f)
, m_scale(1.0f, 1.0f, 1.0f)
, m_dirty(false)
{
}

Transform &Transform::SetTranslation(const Vector3f &translation)
{
	m_translation = translation;
	m_dirty = true;
	return *this;
}

const Vector3f &Transform::GetTranslation() const
{
	return m_translation;
}


Transform &Transform::SetRotation(const Quaternion &rotation)
{
	m_rotation = rotation;
	m_dirty = true;
	return *this;
}

const Quaternion &Transform::GetRotation() const
{
	return m_rotation;
}


Transform &Transform::SetScale(const Vector3f &scale)
{
	m_scale = scale;
	m_dirty = true;
	return *this;
}

void Transform::Finish()
{
	if (m_spatialState)
	{
		m_spatialState->FinishTransformation();
	}
}

const Vector3f &Transform::GetScale() const
{
	return m_scale;
}


const Matrix4f &Transform::GetMatrix() const
{
	if (m_dirty)
	{
		m_rotation.ToMatrix4(m_matrix);
		m_matrix.SetTranslation(m_translation);
		m_matrix.Scale(m_scale);
		m_dirty = false;
	}

	return m_matrix;
}

}
