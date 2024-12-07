#include "RaycastQueryFilterCallback.h"

RaycastQueryFilterCallback::RaycastQueryFilterCallback(const std::string& targetName)
	: m_targetName(targetName)
{
}

physx::PxQueryHitType::Enum RaycastQueryFilterCallback::preFilter(const physx::PxFilterData& filterData
	, const physx::PxShape* shape, const physx::PxRigidActor* actor, physx::PxHitFlags& queryFlags)
{
	const char* actorName = actor->getName();
	if (actorName && m_targetName == actorName)
	{
		// 이름이 일치하면 계속 충돌을 보고
		return physx::PxQueryHitType::eBLOCK;
	}
	else
	{
		// 이름이 일치하지 않으면 레이를 계속 진행
		return physx::PxQueryHitType::eTOUCH;
	}
}

physx::PxQueryHitType::Enum RaycastQueryFilterCallback::postFilter(const physx::PxFilterData& filterData
	, const physx::PxQueryHit& hit, const physx::PxShape* shape, const physx::PxRigidActor* actor)
{
	return physx::PxQueryHitType::eNONE;
}
