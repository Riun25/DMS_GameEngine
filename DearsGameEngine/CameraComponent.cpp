#include "CameraComponent.h"

uint8_t CameraComponent::m_numCamera = 0;

CameraComponent::CameraComponent(std::shared_ptr<Entity> _owner, int _width, int _height, float _speed
	, const Vector3& _eyePos, const Vector3& _viewDir, const Vector3& _viewUp, const Vector3& _proj, unsigned int _cameraEnum)
	: Component(_owner)
{
	m_numCamera++;
	m_cameraEnum = _cameraEnum;
	m_pCamera = new Camera(_width, _height);
	m_pCamera->SetSpeed(_speed);
	m_pCamera->SetEyePos(_eyePos);
	m_pCamera->SetDirection(_viewDir);
	m_pCamera->SetUpVec(_viewUp);
	//m_pCamera->SetAircraftAxes(_axes.x, _axes.y, _axes.z); -> #Sung 이건 카메라 내부적으로 처리할꺼라 쓰지않는게 좋다.
	m_pCamera->ProjectionSettings(_proj.x, _proj.y, _proj.z);
	
}

CameraComponent::~CameraComponent()
{
	m_numCamera--;
}

void CameraComponent::setViewDir(Vector3 _viewDir)
{
	m_pCamera->SetDirection(_viewDir);
}

void CameraComponent::setEyePos(Vector3 _eyePos)
{
	m_pCamera->SetEyePos(_eyePos);
}

void CameraComponent::setLightCameraIndex(unsigned int _lightIndex)
{
	m_lightIndex = _lightIndex;
}
