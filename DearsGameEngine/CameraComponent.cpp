#include "CameraComponent.h"

uint8_t CameraComponent::mNumCamera = 0;

CameraComponent::CameraComponent(std::shared_ptr<Entity> _owner, int _width, int _height, float _speed
	, const Vector3& _eyePos, const Vector3& _viewDir, const Vector3& _viewUp, const Vector3& _proj, unsigned int _cameraEnum)
	: Component(_owner)
{
	mNumCamera++;
	mCameraEnum = _cameraEnum;
	mpCamera = new Camera(_width, _height);
	mpCamera->SetSpeed(_speed);
	mpCamera->SetEyePos(_eyePos);
	mpCamera->SetDirection(_viewDir);
	mpCamera->SetUpVec(_viewUp);
	//m_pCamera->SetAircraftAxes(_axes.x, _axes.y, _axes.z); -> #Sung 이건 카메라 내부적으로 처리할꺼라 쓰지않는게 좋다.
	mpCamera->ProjectionSettings(_proj.x, _proj.y, _proj.z);
	
}

CameraComponent::~CameraComponent()
{
	mNumCamera--;
}

void CameraComponent::setViewDir(Vector3 _viewDir)
{
	mpCamera->SetDirection(_viewDir);
}

void CameraComponent::setEyePos(Vector3 _eyePos)
{
	mpCamera->SetEyePos(_eyePos);
}

void CameraComponent::setLightCameraIndex(unsigned int _lightIndex)
{
	mLightIndex = _lightIndex;
}
