#pragma once
#include "Component.h"
#include "physx/PxRigidActor.h"
#include "Entity.h"

/// <summary>
/// ������Ʈ�� ���� ��Ģ�� �����Ͽ� ����ó�� �����̰ų� �����ϰ� �ϴ� ������Ʈ�Դϴ�.
/// <para>
/// Rigidbody(float ����, float ���� ����(0~1), float �� ����, bool �߷�, bool Ű�׸�ƽ, float �ݹ� ���(0~1), float ���� ���� ���(0~1)
/// , float ���� ���� ���(0~1), ������ ��ü����, Vector3 ��ġ ���� �÷���, Quaternion ȸ�� ���� �÷���)
/// </para>
/// </summary>
struct Rigidbody
	: public Component
{
public:
	Rigidbody(std::shared_ptr<Entity> _owner, float _mass = 1.f, float _drag = 0.f, float _angularDrag = 0.f, bool _useGravity = true, bool _isKinematic = false
		, float _restitution = 0.f, float _staticFriction = 1.f, float _dynamicFriction = 1.f, bool _static = false
		, const Vector3& _freezePosition = Vector3(1000.f), const Quaternion& _freezeRotation = Quaternion(360.f, 360.f, 360.f, 1.f))
		: Component(_owner), mMass(_mass), mDrag(_drag), mAngularDrag(_angularDrag), mUseGravity(_useGravity), mIsKinematic(_isKinematic)
		, mRestitution(_restitution), mStaticFriction(_staticFriction), mDynamicFriction(_dynamicFriction), mIsStatic(_static)
		, mFreezePosition(_freezePosition), mFreezeRotation(_freezeRotation), mpRigidActor(nullptr)
	{
		const std::string& ownerName = _owner->GetName();
		mName = new char[ownerName.size() + 1];  // ���ڿ� ���� + null terminator
		strcpy_s(mName, ownerName.size() + 1, ownerName.c_str());  // ������ ���ڿ� ����
	}
	
	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<Rigidbody>(*this);
	}

public:

	Vector3 mFreezePosition;	// ��ġ ���� �÷���
	Quaternion mFreezeRotation;	// ȸ�� ���� �÷���
	float mMass;				// ����
	float mDrag;				// ���� ����
	float mAngularDrag;		// �� ����
	float mRestitution;		// �ݹ� ���			// 0 ~ 1
	float mStaticFriction;		// ���� ���� ���	// 0 ~ 1
	float mDynamicFriction;	// ���� ���� ���	// 0 ~ 1
	bool mUseGravity;			// �߷� ��� ����
	bool mIsKinematic;			// Ű�׸�ƽ ����
	bool mIsStatic;			// ���� ����
	physx::PxRigidActor* mpRigidActor;
	char* mName;
};