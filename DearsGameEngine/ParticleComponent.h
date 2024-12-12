#pragma once
#include "Component.h"
#include "Graphics/BufferData.h"

struct ParticleDesc
{
	Vector3 position;
	float playTime;
	Vector3 color;
	float lifeTime;
	Vector3 direction;
	float velocity;
	Vector3 Rotation;
	float opacity;
	Vector3 Scale;
	float deltaOpacity;
	Vector3 deltaRotation;
	float SclRandomFactor;
	Vector3 deltaScale;
	Vector3 PosRandomFactor;
	Vector3 RotRandomFactor;
	Vector3 dirRandomFactor;
	Vector3 deltaColor;
	float gravity;
	Vector3 colorRandomFactor;
};

/// <summary>
/// ParticleComponent(Vec3 ��ġ, float ����ð�, Vec3 ����, float �� �ð�, Vec3 ����, float �ӵ�
/// , Vec3 ȸ����, float ����, Vec3 ������, float ���� ��ȭ��, Vec3 ȸ���� ��ȭ��, float ������ ������
/// , Vec3 ������ ��ȭ��, Vec3 ��ġ ������, Vec3 ȸ�� ������, Vec3 ���� ������, Vec3 ���� ��ȭ��
/// , float �߷°��, Vec3 ���� ������
/// </summary>
struct ParticleComponent
	: public Component
{
public:
	ParticleComponent(std::shared_ptr<Entity> _owner, const Vector3& _position = Vector3(), float _playTime = 0
		, const Vector3& _color = Vector3(1), float _lifeTime = 4, const Vector3& _direction = Vector3(), float _velocity = 0
		, const Vector3& _rotation = Vector3(), float _opacity = 1, const Vector3& _scale = Vector3(1)
		, float _dOpacity = 0, const Vector3& _dRotation = Vector3(), float _scaleRandomFactor = 0
		, const Vector3& _dScale = Vector3(0.5), const Vector3& _posRandomFactor = Vector3(), const Vector3& _rotRandomFactor = Vector3()
		, const Vector3& _dirRandomFactor = Vector3(), const Vector3& _dColor = Vector3(), float _gravity = 0
		, const Vector3& _colorRandomFactor = Vector3())
		: Component(_owner), mpParticleData(std::make_shared<CSParticleData>())
	{
		mpParticleData->position = _position;
		mpParticleData->playTime = _playTime;
		mpParticleData->color = _color;
		mpParticleData->lifeTime = _lifeTime;
		mpParticleData->direction = _direction;
		mpParticleData->velocity = _velocity;
		mpParticleData->Rotation = _rotation;
		mpParticleData->opacity = _opacity;
		mpParticleData->Scale = _scale;
		mpParticleData->deltaOpacity = _dOpacity;
		mpParticleData->deltaRotation = _dRotation;
		mpParticleData->SclRandomFactor = _scaleRandomFactor;
		mpParticleData->deltaScale = _dScale;
		mpParticleData->PosRandomFactor = _posRandomFactor;
		mpParticleData->RotRandomFactor = _rotRandomFactor;
		mpParticleData->dirRandomFactor = _dirRandomFactor;
		mpParticleData->deltaColor = _dColor;
		mpParticleData->gravity = _gravity;
		mpParticleData->colorRandomFactor = _colorRandomFactor;
	}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<ParticleComponent>(*this);
	}

public:
	std::shared_ptr<CSParticleData> mpParticleData;
};