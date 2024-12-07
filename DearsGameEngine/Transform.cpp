#include "Transform.h"


Matrix Transform::GetTransformMatrix()
{
	if (m_pParent)
	{
		return  m_pParent->GetTransformMatrix();
	}
	Quaternion qut = Quaternion::CreateFromYawPitchRoll(m_localRotation);

	Matrix matrix = (Matrix::CreateScale(m_localScale)
		* Matrix::CreateFromQuaternion(qut)
		* Matrix::CreateTranslation(m_localPosition)).Transpose();


	return matrix;
}

