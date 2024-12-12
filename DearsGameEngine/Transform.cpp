#include "Transform.h"


Matrix Transform::GetTransformMatrix()
{
	if (mpParent)
	{
		return  mpParent->GetTransformMatrix();
	}
	Quaternion qut = Quaternion::CreateFromYawPitchRoll(mLocalRotation);

	Matrix matrix = (Matrix::CreateScale(mLocalScale)
		* Matrix::CreateFromQuaternion(qut)
		* Matrix::CreateTranslation(mLocalPosition)).Transpose();


	return matrix;
}

