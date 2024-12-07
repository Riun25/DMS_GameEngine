#include <physx/PxPhysicsAPI.h>
#include <iostream>
// 
// using namespace physx;
// 
// // PhysX SDK 초기화
// PxDefaultAllocator      gAllocator;
// PxDefaultErrorCallback  gErrorCallback;
// PxFoundation* gFoundation = nullptr;
// PxPhysics* gPhysics = nullptr;
// PxDefaultCpuDispatcher* gDispatcher = nullptr;
// PxScene* gScene = nullptr;
// PxMaterial* gMaterial = nullptr;
// 
// // 사용자 정의 접촉 보고 클래스
// class ContactReportCallback : public PxSimulationEventCallback {
// public:
// 	void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override {
// 		for (PxU32 i = 0; i < nbPairs; i++) {
// 			const PxContactPair& cp = pairs[i];
// 			if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND) {
// 				std::cout << "Contact detected between " << pairHeader.actors[0]->getName() << " and " << pairHeader.actors[1]->getName() << std::endl;
// 			}
// 		}
// 	}
// 
// 	void onTrigger(PxTriggerPair*, PxU32) override {}
// 	void onAdvance(const PxRigidBody* const*, const PxTransform*, const PxU32) override {}
// 	void onConstraintBreak(PxConstraintInfo*, PxU32) override {}
// 	void onWake(PxActor**, PxU32) override {}
// 	void onSleep(PxActor**, PxU32) override {}
// };
// 
// ContactReportCallback gContactReportCallback;
// 
// void initPhysX() {
// 	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
// 	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale());
// 
// 	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
// 	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
// 	gDispatcher = PxDefaultCpuDispatcherCreate(2);
// 	sceneDesc.cpuDispatcher = gDispatcher;
// 	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
// 	sceneDesc.simulationEventCallback = &gContactReportCallback;
// 	gScene = gPhysics->createScene(sceneDesc);
// 
// 	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);
// }
// 
// // 상자 추가
// PxRigidDynamic* createDynamicBox(const PxTransform& t, const PxVec3& dimensions, const PxVec3& velocity = PxVec3(0)) {
// 	PxBoxGeometry geometry(dimensions);
// 	PxRigidDynamic* box = PxCreateDynamic(*gPhysics, t, geometry, *gMaterial, 10.0f);
// 	box->setLinearVelocity(velocity);
// 	box->setName("Box");
// 	gScene->addActor(*box);
// 	return box;
// }
// 
// // 지면 추가
// PxRigidStatic* createGroundPlane() {
// 	PxRigidStatic* groundPlane = PxCreatePlane(*gPhysics, PxPlane(0, 1, 0, 0), *gMaterial);
// 	groundPlane->setName("Ground");
// 	gScene->addActor(*groundPlane);
// 	return groundPlane;
// }
// 
// void stepPhysics(PxRigidDynamic* box) {
// 	gScene->simulate(1.0f / 60.0f);
// 	gScene->fetchResults(true);
// 	PxTransform transform = box->getGlobalPose();
// 	PxVec3 position = transform.p;
// 	std::cout << "Box position: (" << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;
// }
// 
// void cleanupPhysX() {
// 	gScene->release();
// 	gDispatcher->release();
// 	gPhysics->release();
// 	gFoundation->release();
// }
// 
// int main() {
// 	initPhysX();
// 
// 	createGroundPlane();
// 	PxRigidDynamic* box = createDynamicBox(PxTransform(PxVec3(0, 10, 0)), PxVec3(0.5, 0.5, 0.5));
// 
// 	for (int i = 0; i < 300; i++) {
// 		stepPhysics(box);
// 	}
// 
// 	cleanupPhysX();
// 	return 0;
// }

using namespace physx;

#define DLOG(level, message) std::cout << message << std::endl

class CustomCallback
	: public physx::PxSimulationEventCallback
{
public:
	/// <summary>
	/// 충돌 이벤트 콜백 메서드 
	/// </summary>
	/// <param name="_pairHeader">충돌한 두 객체에 대한 정보를 포함하는 구조체입니다.</param>
	/// <param name="_pairs">충돌한 객체 쌍들의 배열입니다. 각 배열 요소는 충돌 쌍에 대한 세부 정보를 포함합니다.</param>
	/// <param name="_nbPairs">충돌한 객체 쌍의 수를 나타냅니다.</param>
	void onContact(const physx::PxContactPairHeader& _pairHeader, const physx::PxContactPair* _pairs, physx::PxU32 _nbPairs) override;

	/// <summary>
	/// 트리거 이벤트 콜백 메서드 
	/// </summary>
	/// <param name="_pairs">트리거된 객체 쌍들의 배열입니다. 각 배열 요소는 트리거 이벤트에 대한 정보를 포함합니다.</param>
	/// <param name="_count">트리거된 객체 쌍의 수를 나타냅니다.</param>
	void onTrigger(physx::PxTriggerPair* _pairs, physx::PxU32 _count) override;

	/// <summary>
	/// 제약 조건 파괴 이벤트 콜백 메서드 
	/// </summary>
	/// <param name="_constraints">파괴된 제약들의 배열입니다. 각 배열 요소는 파괴된 제약에 대한 정보를 포함합니다.</param>
	/// <param name="_count">파괴된 제약의 수를 나타냅니다.</param>
	void onConstraintBreak(physx::PxConstraintInfo* _constraints, physx::PxU32 _count) override;

	/// <summary>
	/// 객체가 깨어나는 이벤트 콜백 메서드 
	/// </summary>
	/// <param name="_actors">깨어난 객체들에 대한 포인터 배열입니다.</param>
	/// <param name="_count">깨어난 객체의 수를 나타냅니다.</param>
	void onWake(physx::PxActor** _actors, physx::PxU32 _count) override;

	/// <summary>
	/// 객체가 잠드는 이벤트 콜백 메서드 
	/// </summary>
	/// <param name="_actors">잠든 객체들에 대한 포인터 배열입니다.</param>
	/// <param name="_count">잠든 객체의 수를 나타냅니다.</param>
	void onSleep(physx::PxActor** _actors, physx::PxU32 _count) override;

	/// <summary>
	/// 시뮬레이션 상태 업데이트 이벤트 콜백 메서드 
	/// </summary>
	/// <param name="_bodyBuffer">업데이트된 강체 객체들의 포인터 배열입니다.</param>
	/// <param name="_poseBuffer">각 강체 객체의 새로운 위치와 회전을 나타내는 배열입니다.</param>
	/// <param name="_count">업데이트된 강체의 수를 나타냅니다.</param>
	void onAdvance(const physx::PxRigidBody* const* _bodyBuffer, const physx::PxTransform* _poseBuffer, const physx::PxU32 _count) override;

private:
	void HandleProjectileHit(physx::PxShape* projectile, physx::PxShape* target);
	void HandleMeleeWeaponHit(physx::PxShape* weapon, physx::PxShape* target);
	void HandleCoinCollision(physx::PxShape* coin, physx::PxShape* character);
};



PxFilterFlags CustomFilterShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0,
	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
	pairFlags = PxPairFlag::eCONTACT_DEFAULT | PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_LOST;
	return PxFilterFlag::eDEFAULT;
}

void CustomCallback::onContact(const physx::PxContactPairHeader& _pairHeader, const physx::PxContactPair* _pairs, physx::PxU32 _nbPairs)
{
	DLOG(LOG_INFO, "OnContact");
	for (PxU32 i = 0; i < _nbPairs; i++)
	{
		const PxContactPair& cp = _pairs[i];

		if ((cp.shapes[0]->getSimulationFilterData().word0 == 1 && cp.shapes[1]->getSimulationFilterData().word0 == 2) ||
			(cp.shapes[0]->getSimulationFilterData().word0 == 2 && cp.shapes[1]->getSimulationFilterData().word0 == 1))
		{
			DLOG(LOG_INFO, "Collision detected between objects.");
		}
	}
}

void CustomCallback::onTrigger(physx::PxTriggerPair* _pairs, physx::PxU32 _count)
{
	PX_UNUSED((_pairs));
	DLOG(LOG_INFO, "onTrigger");

}

void CustomCallback::onConstraintBreak(physx::PxConstraintInfo* _constraints, physx::PxU32 _count)
{
	PX_UNUSED((_constraints));
	DLOG(LOG_INFO, "onConstraintBreak");
}

void CustomCallback::onWake(physx::PxActor** _actors, physx::PxU32 _count)
{
	PX_UNUSED((_actors));
	DLOG(LOG_INFO, "onWake");
}

void CustomCallback::onSleep(physx::PxActor** _actors, physx::PxU32 _count)
{
	PX_UNUSED((_actors));
	DLOG(LOG_INFO, "onSleep");
}

void CustomCallback::onAdvance(const physx::PxRigidBody* const* _bodyBuffer, const physx::PxTransform* _poseBuffer, const physx::PxU32 _count)
{
	DLOG(LOG_INFO, "onAdvance");
}

void CustomCallback::HandleProjectileHit(physx::PxShape* projectile, physx::PxShape* target)
{
	// 투사체가 캐릭터에 충돌했을 때의 이벤트 처리 로직
}

void CustomCallback::HandleMeleeWeaponHit(physx::PxShape* weapon, physx::PxShape* target)
{
	// 근거리 무기가 캐릭터에 충돌했을 때의 이벤트 처리 로직
}

void CustomCallback::HandleCoinCollision(physx::PxShape* coin, physx::PxShape* character)
{
	// 동전이 캐릭터와 충돌했을 때의 이벤트 처리 로직
}


int main()
{
	DLOG(LOG_INFO, "Starting PhysX simulation...");

	PxDefaultAllocator		gAllocator;
	PxDefaultErrorCallback	gErrorCallback;

	PxFoundation* gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
	PxPhysics* m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale());

	PxDefaultCpuDispatcher* m_pDispatcher = PxDefaultCpuDispatcherCreate(2);

	auto _callback = new CustomCallback();
	physx::PxSimulationFilterShader _filterShader = CustomFilterShader;

	physx::PxSceneDesc sceneDesc(m_pPhysics->getTolerancesScale());
	sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
	sceneDesc.cpuDispatcher = m_pDispatcher;
	sceneDesc.filterShader = _filterShader;
	sceneDesc.simulationEventCallback = _callback;

	PxScene* m_pScene = m_pPhysics->createScene(sceneDesc);

	if (!m_pScene) {
		DLOG(LOG_INFO, "Scene creation failed!");
		return -1;
	}

	// Add ground plane
	PxMaterial* gMaterial = m_pPhysics->createMaterial(0.5f, 0.5f, 0.6f);
	PxRigidStatic* groundPlane = PxCreatePlane(*m_pPhysics, PxPlane(0, 1, 0, 0), *gMaterial);
	m_pScene->addActor(*groundPlane);

	// Add a dynamic actor (box)
	PxTransform boxTransform(PxVec3(0, 10, 0));
	PxBoxGeometry boxGeometry(PxVec3(1, 1, 1));
	PxRigidDynamic* box = PxCreateDynamic(*m_pPhysics, boxTransform, boxGeometry, *gMaterial, 10.0f);
	PxFilterData filter1{ 1,0,0,0 };
 	box->userData = &filter1; // 필터 데이터 설정
	m_pScene->addActor(*box);

	// Add another dynamic actor (box)
	PxTransform boxTransform2(PxVec3(0, 0, 0));
	PxRigidDynamic* box2 = PxCreateDynamic(*m_pPhysics, boxTransform2, boxGeometry, *gMaterial, 10.0f);
	PxFilterData filter2{ 1,0,0,0 };
	box2->userData = &filter2; // 필터 데이터 설정
	m_pScene->addActor(*box2);

	// Simulation loop
	for (int i = 0; i < 300; i++)
	{
// 		std::cout << "pos: " << box->getGlobalPose().p.x << box->getGlobalPose().p.y << box->getGlobalPose().p.z << std::endl;
// 		std::cout << "pos2: " << box2->getGlobalPose().p.x << box2->getGlobalPose().p.y << box2->getGlobalPose().p.z << std::endl;
		m_pScene->simulate(1.0f / 60.0f);
		m_pScene->fetchResults(true);
	}

	// Cleanup
	m_pScene->release();
	m_pDispatcher->release();
	m_pPhysics->release();
	gFoundation->release();
	delete _callback;

	DLOG(LOG_INFO, "Simulation completed.");
	return 0;
}
