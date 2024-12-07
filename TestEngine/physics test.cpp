#include <physx/PxPhysicsAPI.h>
#include <iostream>
#include <vector>
using namespace physx;
#define DLOG(level, message) std::cout << message << std::endl

/// main test
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

/// callback test
// 
// using namespace physx;
// 
// #define DLOG(level, message) std::cout << message << std::endl
// 
// class CustomCallback
// 	: public physx::PxSimulationEventCallback
// {
// public:
// 	/// <summary>
// 	/// 충돌 이벤트 콜백 메서드 
// 	/// </summary>
// 	/// <param name="_pairHeader">충돌한 두 객체에 대한 정보를 포함하는 구조체입니다.</param>
// 	/// <param name="_pairs">충돌한 객체 쌍들의 배열입니다. 각 배열 요소는 충돌 쌍에 대한 세부 정보를 포함합니다.</param>
// 	/// <param name="_nbPairs">충돌한 객체 쌍의 수를 나타냅니다.</param>
// 	void onContact(const physx::PxContactPairHeader& _pairHeader, const physx::PxContactPair* _pairs, physx::PxU32 _nbPairs) override;
// 
// 	/// <summary>
// 	/// 트리거 이벤트 콜백 메서드 
// 	/// </summary>
// 	/// <param name="_pairs">트리거된 객체 쌍들의 배열입니다. 각 배열 요소는 트리거 이벤트에 대한 정보를 포함합니다.</param>
// 	/// <param name="_count">트리거된 객체 쌍의 수를 나타냅니다.</param>
// 	void onTrigger(physx::PxTriggerPair* _pairs, physx::PxU32 _count) override;
// 
// 	/// <summary>
// 	/// 제약 조건 파괴 이벤트 콜백 메서드 
// 	/// </summary>
// 	/// <param name="_constraints">파괴된 제약들의 배열입니다. 각 배열 요소는 파괴된 제약에 대한 정보를 포함합니다.</param>
// 	/// <param name="_count">파괴된 제약의 수를 나타냅니다.</param>
// 	void onConstraintBreak(physx::PxConstraintInfo* _constraints, physx::PxU32 _count) override;
// 
// 	/// <summary>
// 	/// 객체가 깨어나는 이벤트 콜백 메서드 
// 	/// </summary>
// 	/// <param name="_actors">깨어난 객체들에 대한 포인터 배열입니다.</param>
// 	/// <param name="_count">깨어난 객체의 수를 나타냅니다.</param>
// 	void onWake(physx::PxActor** _actors, physx::PxU32 _count) override;
// 
// 	/// <summary>
// 	/// 객체가 잠드는 이벤트 콜백 메서드 
// 	/// </summary>
// 	/// <param name="_actors">잠든 객체들에 대한 포인터 배열입니다.</param>
// 	/// <param name="_count">잠든 객체의 수를 나타냅니다.</param>
// 	void onSleep(physx::PxActor** _actors, physx::PxU32 _count) override;
// 
// 	/// <summary>
// 	/// 시뮬레이션 상태 업데이트 이벤트 콜백 메서드 
// 	/// </summary>
// 	/// <param name="_bodyBuffer">업데이트된 강체 객체들의 포인터 배열입니다.</param>
// 	/// <param name="_poseBuffer">각 강체 객체의 새로운 위치와 회전을 나타내는 배열입니다.</param>
// 	/// <param name="_count">업데이트된 강체의 수를 나타냅니다.</param>
// 	void onAdvance(const physx::PxRigidBody* const* _bodyBuffer, const physx::PxTransform* _poseBuffer, const physx::PxU32 _count) override;
// 
// private:
// 	void HandleProjectileHit(physx::PxShape* projectile, physx::PxShape* target);
// 	void HandleMeleeWeaponHit(physx::PxShape* weapon, physx::PxShape* target);
// 	void HandleCoinCollision(physx::PxShape* coin, physx::PxShape* character);
// };
// 
// 
// 
// PxFilterFlags CustomFilterShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0,
// 	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
// 	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
// {
// 	pairFlags = PxPairFlag::eCONTACT_DEFAULT | PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_LOST;
// 	return PxFilterFlag::eDEFAULT;
// }
// 
// void CustomCallback::onContact(const physx::PxContactPairHeader& _pairHeader, const physx::PxContactPair* _pairs, physx::PxU32 _nbPairs)
// {
// 	DLOG(LOG_INFO, "OnContact");
// 	for (PxU32 i = 0; i < _nbPairs; i++)
// 	{
// 		const PxContactPair& cp = _pairs[i];
// 
// 		if ((cp.shapes[0]->getSimulationFilterData().word0 == 1 && cp.shapes[1]->getSimulationFilterData().word0 == 2) ||
// 			(cp.shapes[0]->getSimulationFilterData().word0 == 2 && cp.shapes[1]->getSimulationFilterData().word0 == 1))
// 		{
// 			DLOG(LOG_INFO, "Collision detected between objects.");
// 		}
// 	}
// }
// 
// void CustomCallback::onTrigger(physx::PxTriggerPair* _pairs, physx::PxU32 _count)
// {
// 	PX_UNUSED((_pairs));
// 	DLOG(LOG_INFO, "onTrigger");
// 
// }
// 
// void CustomCallback::onConstraintBreak(physx::PxConstraintInfo* _constraints, physx::PxU32 _count)
// {
// 	PX_UNUSED((_constraints));
// 	DLOG(LOG_INFO, "onConstraintBreak");
// }
// 
// void CustomCallback::onWake(physx::PxActor** _actors, physx::PxU32 _count)
// {
// 	PX_UNUSED((_actors));
// 	DLOG(LOG_INFO, "onWake");
// }
// 
// void CustomCallback::onSleep(physx::PxActor** _actors, physx::PxU32 _count)
// {
// 	PX_UNUSED((_actors));
// 	DLOG(LOG_INFO, "onSleep");
// }
// 
// void CustomCallback::onAdvance(const physx::PxRigidBody* const* _bodyBuffer, const physx::PxTransform* _poseBuffer, const physx::PxU32 _count)
// {
// 	DLOG(LOG_INFO, "onAdvance");
// }
// 
// void CustomCallback::HandleProjectileHit(physx::PxShape* projectile, physx::PxShape* target)
// {
// 	// 투사체가 캐릭터에 충돌했을 때의 이벤트 처리 로직
// }
// 
// void CustomCallback::HandleMeleeWeaponHit(physx::PxShape* weapon, physx::PxShape* target)
// {
// 	// 근거리 무기가 캐릭터에 충돌했을 때의 이벤트 처리 로직
// }
// 
// void CustomCallback::HandleCoinCollision(physx::PxShape* coin, physx::PxShape* character)
// {
// 	// 동전이 캐릭터와 충돌했을 때의 이벤트 처리 로직
// }
// 
// 
// int main()
// {
// 	DLOG(LOG_INFO, "Starting PhysX simulation...");
// 
// 	PxDefaultAllocator		gAllocator;
// 	PxDefaultErrorCallback	gErrorCallback;
// 
// 	PxFoundation* gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
// 	PxPhysics* m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale());
// 
// 	PxDefaultCpuDispatcher* m_pDispatcher = PxDefaultCpuDispatcherCreate(2);
// 
// 	auto _callback = new CustomCallback();
// 	physx::PxSimulationFilterShader _filterShader = CustomFilterShader;
// 
// 	physx::PxSceneDesc sceneDesc(m_pPhysics->getTolerancesScale());
// 	sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
// 	sceneDesc.cpuDispatcher = m_pDispatcher;
// 	sceneDesc.filterShader = _filterShader;
// 	sceneDesc.simulationEventCallback = _callback;
// 
// 	PxScene* m_pScene = m_pPhysics->createScene(sceneDesc);
// 
// 	if (!m_pScene) {
// 		DLOG(LOG_INFO, "Scene creation failed!");
// 		return -1;
// 	}
// 
// 	// Add ground plane
// 	PxMaterial* gMaterial = m_pPhysics->createMaterial(0.5f, 0.5f, 0.6f);
// 	PxRigidStatic* groundPlane = PxCreatePlane(*m_pPhysics, PxPlane(0, 1, 0, 0), *gMaterial);
// 	m_pScene->addActor(*groundPlane);
// 
// 	// Add a dynamic actor (box)
// 	PxTransform boxTransform(PxVec3(0, 10, 0));
// 	PxBoxGeometry boxGeometry(PxVec3(1, 1, 1));
// 	PxRigidDynamic* box = PxCreateDynamic(*m_pPhysics, boxTransform, boxGeometry, *gMaterial, 10.0f);
// 	PxFilterData filter1{ 1,0,0,0 };
//  	box->userData = &filter1; // 필터 데이터 설정
// 	m_pScene->addActor(*box);
// 
// 	// Add another dynamic actor (box)
// 	PxTransform boxTransform2(PxVec3(0, 0, 0));
// 	PxRigidDynamic* box2 = PxCreateDynamic(*m_pPhysics, boxTransform2, boxGeometry, *gMaterial, 10.0f);
// 	PxFilterData filter2{ 1,0,0,0 };
// 	box2->userData = &filter2; // 필터 데이터 설정
// 	m_pScene->addActor(*box2);
// 
// 	// Simulation loop
// 	for (int i = 0; i < 300; i++)
// 	{
// // 		std::cout << "pos: " << box->getGlobalPose().p.x << box->getGlobalPose().p.y << box->getGlobalPose().p.z << std::endl;
// // 		std::cout << "pos2: " << box2->getGlobalPose().p.x << box2->getGlobalPose().p.y << box2->getGlobalPose().p.z << std::endl;
// 		m_pScene->simulate(1.0f / 60.0f);
// 		m_pScene->fetchResults(true);
// 	}
// 
// 	// Cleanup
// 	m_pScene->release();
// 	m_pDispatcher->release();
// 	m_pPhysics->release();
// 	gFoundation->release();
// 	delete _callback;
// 
// 	DLOG(LOG_INFO, "Simulation completed.");
// 	return 0;
// }

/// 투사체 테스트
//// 필터 그룹 및 플래그 정의
//#define FILTER_GROUP_PROJECTILE 1
//#define FILTER_FLAG_ON_GROUND (1 << 0) // onGround 플래그
//
//class CustomCallback : public PxSimulationEventCallback
//{
//public:
//	void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override;
//	void onTrigger(PxTriggerPair* pairs, PxU32 count) override;
//	void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) override;
//	void onWake(PxActor** actors, PxU32 count) override;
//	void onSleep(PxActor** actors, PxU32 count) override;
//	void onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count) override;
//};
//
//PxFilterFlags CustomFilterShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0,
//	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
//	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
//{
//	pairFlags = PxPairFlag::eCONTACT_DEFAULT | PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_LOST;
//
//	// 충돌 시 onGround 플래그 설정
//	if (filterData0.word0 == FILTER_GROUP_PROJECTILE || filterData1.word0 == FILTER_GROUP_PROJECTILE)
//	{
//		if (filterData0.word0 == FILTER_GROUP_PROJECTILE)
//		{
//			filterData0.word1 |= FILTER_FLAG_ON_GROUND;
//		}
//		if (filterData1.word0 == FILTER_GROUP_PROJECTILE)
//		{
//			filterData1.word1 |= FILTER_FLAG_ON_GROUND;
//		}
//	}
//	return PxFilterFlag::eDEFAULT;
//}
//
//void CustomCallback::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
//{
//	DLOG(LOG_INFO, "OnContact");
//
//	// 충돌한 물체를 멈추게 하기
//	for (PxU32 i = 0; i < nbPairs; i++)
//	{
//		const PxContactPair& cp = pairs[i];
//		for (PxU32 j = 0; j < 2; j++)
//		{
//			PxShape* shape = cp.shapes[j];
//			PxRigidDynamic* dynamicActor = shape->getActor()->is<PxRigidDynamic>();
//			if (dynamicActor)
//			{
//				PxFilterData filterData = shape->getSimulationFilterData();
//				if (filterData.word1 & FILTER_FLAG_ON_GROUND) // onGround 플래그 확인
//				{
//					dynamicActor->setLinearVelocity(PxVec3(0, 0, 0));
//					dynamicActor->setAngularVelocity(PxVec3(0, 0, 0));
//					DLOG(LOG_INFO, "velocity 0");
//				}
//			}
//		}
//	}
//}
//
//void CustomCallback::onTrigger(PxTriggerPair* pairs, PxU32 count)
//{
//	DLOG(LOG_INFO, "onTrigger");
//}
//
//void CustomCallback::onConstraintBreak(PxConstraintInfo* constraints, PxU32 count)
//{
//	DLOG(LOG_INFO, "onConstraintBreak");
//}
//
//void CustomCallback::onWake(PxActor** actors, PxU32 count)
//{
//	DLOG(LOG_INFO, "onWake");
//}
//
//void CustomCallback::onSleep(PxActor** actors, PxU32 count)
//{
//	DLOG(LOG_INFO, "onSleep");
//}
//
//void CustomCallback::onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count)
//{
//	DLOG(LOG_INFO, "onAdvance");
//}
//
//int main()
//{
//	DLOG(LOG_INFO, "Starting PhysX simulation...");
//
//	PxDefaultAllocator gAllocator;
//	PxDefaultErrorCallback gErrorCallback;
//
//	PxFoundation* gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
//	PxPhysics* m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale());
//
//	PxDefaultCpuDispatcher* m_pDispatcher = PxDefaultCpuDispatcherCreate(2);
//
//	auto _callback = new CustomCallback();
//	physx::PxSimulationFilterShader _filterShader = CustomFilterShader;
//
//	physx::PxSceneDesc sceneDesc(m_pPhysics->getTolerancesScale());
//	sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f); // 중력 설정
//	sceneDesc.cpuDispatcher = m_pDispatcher;
//	sceneDesc.filterShader = _filterShader;
//	sceneDesc.simulationEventCallback = _callback;
//
//	PxScene* m_pScene = m_pPhysics->createScene(sceneDesc);
//
//	if (!m_pScene) {
//		DLOG(LOG_INFO, "Scene creation failed!");
//		return -1;
//	}
//
//	// Add ground plane
//	PxMaterial* gMaterial = m_pPhysics->createMaterial(0.5f, 0.5f, 0.0f); // 탄성 계수를 0으로 설정
//	PxRigidStatic* groundPlane = PxCreatePlane(*m_pPhysics, PxPlane(0, 1, 0, 0), *gMaterial);
//	m_pScene->addActor(*groundPlane);
//
//	// Add a dynamic actor (box)
//	PxTransform boxTransform(PxVec3(0, 10, 0));
//	PxBoxGeometry boxGeometry(PxVec3(1, 1, 1));
//	PxRigidDynamic* box = PxCreateDynamic(*m_pPhysics, boxTransform, boxGeometry, *gMaterial, 10.0f);
//	box->setAngularDamping(0.5f); // 회전 감속 설정
//
//	// 초기 속도 설정 (예: x 방향으로 10, y 방향으로 10)
//	PxVec3 initialVelocity(10.0f, 10.0f, 0.0f);
//	box->setLinearVelocity(initialVelocity);
//
//	// 필터 데이터 설정
//	PxFilterData filterData;
//	filterData.word0 = FILTER_GROUP_PROJECTILE; // 투사체 그룹 설정
//	filterData.word1 = 0; // 초기 상태에서 onGround 플래그 비활성화
//	box->userData = &filterData;
//
//	m_pScene->addActor(*box);
//
//	// Simulation loop
//	for (int i = 0; i < 300; i++)
//	{
//		std::cout << "pos: " 
//			<< box->getGlobalPose().p.x <<"/"
//			<< box->getGlobalPose().p.y <<"/"
//			<< box->getGlobalPose().p.z 
//			<< std::endl;
//		m_pScene->simulate(1.0f / 60.0f);
//		m_pScene->fetchResults(true);
//	}
//
//	// Cleanup
//	m_pScene->release();
//	m_pDispatcher->release();
//	m_pPhysics->release();
//	gFoundation->release();
//	delete _callback;
//
//	DLOG(LOG_INFO, "Simulation completed.");
//	return 0;
//}

/// 투사체 테스트 2
//// PhysX 전역 변수
//PxDefaultAllocator gAllocator;
//PxDefaultErrorCallback gErrorCallback;
//PxFoundation* gFoundation = nullptr;
//PxPhysics* gPhysics = nullptr;
//PxDefaultCpuDispatcher* gDispatcher = nullptr;
//PxScene* gScene = nullptr;
//PxMaterial* gMaterial = nullptr;
//
//// 중력 설정
//PxVec3 gGravity(0.0f, -9.81f, 0.0f);
//
//// 초기화 함수
//void initPhysX() {
//	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
//	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale());
//	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
//	sceneDesc.gravity = gGravity;
//	gDispatcher = PxDefaultCpuDispatcherCreate(2);
//	sceneDesc.cpuDispatcher = gDispatcher;
//	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
//	gScene = gPhysics->createScene(sceneDesc);
//	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f); // 마찰 및 반발 계수
//}
//
//// 종료 함수
//void cleanupPhysX() {
//	gScene->release();
//	gDispatcher->release();
//	gPhysics->release();
//	gFoundation->release();
//}
//
//// 시뮬레이션 함수
//void simulateProjectile() {
//	// 물체 생성
//	PxTransform startTransform(PxVec3(0.0f, 0.0f, 0.0f));
//	PxRigidDynamic* sphere = PxCreateDynamic(*gPhysics, startTransform, PxSphereGeometry(0.5f), *gMaterial, 1.0f);
//	sphere->setName("sphere");
//	gScene->addActor(*sphere);
//
//	// 초기 속도 설정 (포물선 운동)
//	PxVec3 initialVelocity(5.0f, 10.0f, 5.0f); // 수직 속도를 높여 포물선 운동
//	sphere->setLinearVelocity(initialVelocity);
//
//	// 시뮬레이션 루프
//	for (int i = 0; i < 500; ++i) { // 5초 동안 시뮬레이션 (0.01초 간격)
//		gScene->simulate(0.01f); // 시뮬레이션 스텝
//		gScene->fetchResults(true);
//
//		// 물체 위치 업데이트
//		PxTransform currentPose = sphere->getGlobalPose();
//		PxVec3 position = currentPose.p;
//		std::cout << sphere->getName() << " / " << "Time: " << i * 0.01f << "s, Position: ("
//			<< position.x << ", " << position.y << ", " << position.z << ")" << std::endl;
//	}
//
//	sphere->release();
//}
//
//int main() {
//	initPhysX();
//	simulateProjectile();
//	cleanupPhysX();
//	return 0;
//}

/// 캐릭터 컨트롤러 테스트
// 전역 객체 선언
//PxDefaultAllocator		gAllocator;
//PxDefaultErrorCallback	gErrorCallback;
//
//PxFoundation* gFoundation = nullptr;
//PxPhysics* gPhysics = nullptr;
//PxControllerManager* gControllerManager = nullptr;
//PxScene* gScene = nullptr;
//PxMaterial* gMaterial = nullptr;
//
//// PhysX 초기화 함수
//void initPhysX()
//{
//	// PhysX 기초 객체 생성
//	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
//	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale());
//
//	// 씬 설정
//	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
//	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
//
//	// 디스패치와 필터 셰이더 설정
//	sceneDesc.cpuDispatcher = PxDefaultCpuDispatcherCreate(2);
//	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
//
//	gScene = gPhysics->createScene(sceneDesc);
//
//	// 컨트롤러 관리자 생성
//	gControllerManager = PxCreateControllerManager(*gScene);
//
//	// 기본 재질 생성
//	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.5f);
//}
//
//// 캡슐 컨트롤러 생성 함수
//PxCapsuleController* createCapsuleController()
//{
//	// 캡슐 컨트롤러 설정
//	PxCapsuleControllerDesc desc;
//	desc.height = 2.0f; // 캡슐 높이
//	desc.radius = 0.5f; // 캡슐 반지름
//	desc.position = PxExtendedVec3(0, 10, 0); // 초기 위치
//	desc.material = gMaterial; // 재질 설정
//	desc.contactOffset = 0.1f;
//	desc.stepOffset = 0.5f;
//	desc.slopeLimit = 0.707f; // 45도
//
//	// 캡슐 컨트롤러 생성
//	PxCapsuleController* capsuleController = static_cast<PxCapsuleController*>(gControllerManager->createController(desc));
//	return capsuleController;
//}
//
//// 캐릭터 이동 함수
//void moveCharacter(PxCapsuleController* capsuleController, const PxVec3& direction, float elapsedTime)
//{
//	PxControllerFilters filters;
//	PxVec3 displacement = direction * elapsedTime;
//	PxControllerState state;
//
//	capsuleController->move(displacement, 0.001f, elapsedTime, filters);
//
//	// 상태 확인 후 시각화
//	auto position = capsuleController->getPosition();
//	std::cout << "Character Position: ("
//		<< position.x << ", "
//		<< position.y << ", "
//		<< position.z << ")\n";
//}
//
//// PhysX 정리 함수
//void cleanupPhysX()
//{
//	if (gControllerManager) gControllerManager->release();
//	if (gScene) gScene->release();
//	if (gPhysics) gPhysics->release();
//	if (gFoundation) gFoundation->release();
//}
//
//int main()
//{
//	// PhysX 초기화
//	initPhysX();
//
//	// 캡슐 컨트롤러 생성
//	PxCapsuleController* capsuleController = createCapsuleController();
//
//	// 게임 루프
//	bool running = true;
//	while (running)
//	{
//		// 시간 경과
//		float elapsedTime = 1.0f / 60.0f; // 60 FPS
//
//		// 사용자 입력에 따라 이동 (임시로 상수 값 사용)
//		PxVec3 direction(1.0f, 0.0f, 0.0f); // 오른쪽으로 이동
//
//		// 캐릭터 이동
//		moveCharacter(capsuleController, direction, elapsedTime);
//
//		// 씬 스텝
//		gScene->simulate(elapsedTime);
//		gScene->fetchResults(true);
//
//		// 종료 조건 (여기서는 예제로 임의의 루프 종료)
//		static int frameCount = 0;
//		if (++frameCount > 300) // 약 5초 후 종료
//			running = false;
//	}
//
//	// PhysX 정리
//	cleanupPhysX();
//
//	return 0;
//}

/// 필터, 콜백
//// 충돌 그룹 비트 정의
//enum CollisionType
//{
//	TYPE_GROUND = 1 << 0, // 땅
//	TYPE_UNIT = 1 << 1, // 유닛
//	TYPE_PROJECTILE = 1 << 2, // 투사체
//	TYPE_WEAPON = 1 << 3, // 무기
//	TYPE_GOLD = 1 << 4, // 금화
//	TYPE_OBSTACLE = 1 << 5  // 장애물
//};
//
//enum CollisionAttribute
//{
//	ATTR_ALLY = 1 << 0, // 아군
//	ATTR_ENEMY = 1 << 1, // 적군
//	ATTR_ON_GROUND = 1 << 2, // 지면에 붙어 있는지 여부
//	ATTR_IN_AIR = 1 << 3  // 공중에 있는지 여부
//};
//
//// 충돌 필터 셰이더 정의
//PxFilterFlags FilterShader(
//	PxFilterObjectAttributes attributes0, PxFilterData filterData0,
//	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
//	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
//{
//	// 금화는 다른 금화와 충돌하지 않음
//	if ((filterData0.word0 & TYPE_GOLD) &&
//		(filterData1.word0 & TYPE_GOLD))
//	{
//		return PxFilterFlag::eSUPPRESS;
//	}
//
//	// 유닛과 장애물은 중첩될 수 없음
//	if ((filterData0.word0 & TYPE_UNIT) &&
//		(filterData1.word0 & TYPE_UNIT))
//	{
//		pairFlags = PxPairFlag::eCONTACT_DEFAULT;
//		return PxFilterFlag::eDEFAULT;
//	}
//
//	// 금화는 땅과만 상호작용 가능
//	if ((filterData0.word0 & TYPE_GOLD) &&
//		(filterData1.word0 & TYPE_GROUND))
//	{
//		pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
//		return PxFilterFlag::eDEFAULT;
//	}
//
//	// 투사체는 장애물을 통과할 수 없음
//	if (((filterData0.word0 & TYPE_PROJECTILE) &&
//		(filterData1.word0 & TYPE_OBSTACLE)) ||
//		((filterData1.word0 & TYPE_PROJECTILE) &&
//			(filterData0.word0 & TYPE_OBSTACLE)))
//	{
//		pairFlags = PxPairFlag::eCONTACT_DEFAULT;
//		return PxFilterFlag::eDEFAULT;
//	}
//
//	// 무기는 모든 오브젝트를 통과할 수 있음
//	if ((filterData0.word0 & TYPE_WEAPON) ||
//		(filterData1.word0 & TYPE_WEAPON))
//	{
//		return PxFilterFlag::eSUPPRESS;
//	}
//
//	// 기본 페어 플래그 정의
//	pairFlags = PxPairFlag::eCONTACT_DEFAULT;
//	return PxFilterFlag::eDEFAULT;
//}
//
//// 충돌 콜백 핸들러 구현
//class CollisionCallback : public PxSimulationEventCallback
//{
//public:
//	void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override
//	{
//		for (PxU32 i = 0; i < nbPairs; i++)
//		{
//			const PxContactPair& cp = pairs[i];
//
//			// 아군 및 적군 투사체와 유닛 간의 접촉 처리
//			if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
//			{
//				// 액터에서 첫 번째 셰이프 가져오기
//				PxRigidActor* actor0 = pairHeader.actors[0]->is<PxRigidActor>();
//				PxRigidActor* actor1 = pairHeader.actors[1]->is<PxRigidActor>();
//
//				if (actor0 && actor1) {
//					PxShape* shapes0[1];
//					PxShape* shapes1[1];
//
//					actor0->getShapes(shapes0, 1);
//					actor1->getShapes(shapes1, 1);
//
//					PxFilterData filterData0 = shapes0[0]->getSimulationFilterData();
//					PxFilterData filterData1 = shapes1[0]->getSimulationFilterData();
//
//					// 아군 투사체인지 적군 투사체인지 확인
//					if ((filterData0.word0 & TYPE_PROJECTILE) && (filterData0.word1 & ATTR_ALLY))
//					{
//						// 아군 투사체 데미지 처리 로직 구현
//						std::cout << "Ally projectile hit something!" << std::endl;
//					}
//					else if ((filterData1.word0 & TYPE_PROJECTILE) && (filterData1.word1 & ATTR_ALLY))
//					{
//						// 아군 투사체 데미지 처리 로직 구현
//						std::cout << "Ally projectile hit something!" << std::endl;
//					}
//
//					if ((filterData0.word0 & TYPE_PROJECTILE) && (filterData0.word1 & ATTR_ENEMY))
//					{
//						// 적군 투사체 데미지 처리 로직 구현
//						std::cout << "Enemy projectile hit something!" << std::endl;
//					}
//					else if ((filterData1.word0 & TYPE_PROJECTILE) && (filterData1.word1 & ATTR_ENEMY))
//					{
//						// 적군 투사체 데미지 처리 로직 구현
//						std::cout << "Enemy projectile hit something!" << std::endl;
//					}
//				}
//			}
//		}
//	}
//
//	void onTrigger(PxTriggerPair* pairs, PxU32 count) override
//	{
//		for (PxU32 i = 0; i < count; i++)
//		{
//			const PxTriggerPair& tp = pairs[i];
//
//			// 금화가 땅에 닿았을 때의 루팅 로직 처리
//			if (tp.status & PxPairFlag::eNOTIFY_TOUCH_FOUND)
//			{
//				// 액터에서 첫 번째 셰이프 가져오기
//				PxRigidActor* actor0 = tp.triggerActor->is<PxRigidActor>();
//				PxRigidActor* actor1 = tp.otherActor->is<PxRigidActor>();
//
//				if (actor0 && actor1) {
//					PxShape* shapes0[1];
//					PxShape* shapes1[1];
//
//					actor0->getShapes(shapes0, 1);
//					actor1->getShapes(shapes1, 1);
//
//					PxFilterData filterData0 = shapes0[0]->getSimulationFilterData();
//					PxFilterData filterData1 = shapes1[0]->getSimulationFilterData();
//
//					if ((filterData0.word0 & TYPE_GOLD) && (filterData1.word0 & TYPE_GROUND))
//					{
//						// 루팅 로직 구현
//						std::cout << "Gold hit the ground!" << std::endl;
//					}
//				}
//			}
//		}
//	}
//
//	void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) override {}
//	void onWake(PxActor** actors, PxU32 count) override {}
//	void onSleep(PxActor** actors, PxU32 count) override {}
//	void onAdvance(const PxRigidBody* const*, const PxTransform*, const PxU32) override {}
//};
//
//int main()
//{
//	// PhysX 초기화
//	PxDefaultErrorCallback gDefaultErrorCallback;
//	PxDefaultAllocator gDefaultAllocatorCallback;
//
//	PxFoundation* foundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
//	PxPhysics* physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, PxTolerancesScale(), true, nullptr);
//
//	PxSceneDesc sceneDesc(physics->getTolerancesScale());
//	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
//
//	PxDefaultCpuDispatcher* dispatcher = PxDefaultCpuDispatcherCreate(2);
//	sceneDesc.cpuDispatcher = dispatcher;
//	sceneDesc.filterShader = FilterShader;
//
//	PxScene* scene = physics->createScene(sceneDesc);
//
//	// 물리 소재 생성
//	PxMaterial* material = physics->createMaterial(1.f, 1.f, 0.f);
//
//	// 지면 생성
//	PxRigidStatic* ground = PxCreatePlane(*physics, PxPlane(0, 1, 0, 0), *material);
//	scene->addActor(*ground);
//
//	// 금화 1 생성
//	PxRigidDynamic* gold1 = PxCreateDynamic(*physics, PxTransform(PxVec3(0, 5, 0)), PxSphereGeometry(5.f), *material, 1.0f);
//	gold1->setAngularDamping(0.f);
//	PxShape* shape1;
//	gold1->getShapes(&shape1, 1);
//	PxFilterData filterData1;
//	filterData1.word0 = TYPE_GOLD;
//	shape1->setSimulationFilterData(filterData1);
//	scene->addActor(*gold1);
//
//	// 금화 2 생성
//	PxRigidDynamic* gold2 = PxCreateDynamic(*physics, PxTransform(PxVec3(0, 10, 1)), PxSphereGeometry(5.f), *material, 1.0f);
//	gold2->setAngularDamping(0.f);
//	PxShape* shape2;
//	gold2->getShapes(&shape2, 1);
//	PxFilterData filterData2;
//	filterData2.word0 = TYPE_GOLD;
//	shape2->setSimulationFilterData(filterData2);
//	scene->addActor(*gold2);
//
//	// 물리 엔진 스텝 수행
//	for (int i = 0; i < 300; i++)
//	{
//		scene->simulate(1.0f / 60.0f);
//		scene->fetchResults(true);
//
//		// 금화 위치 출력
//		PxTransform transform1 = gold1->getGlobalPose();
//		PxTransform transform2 = gold2->getGlobalPose();
//		std::cout << "Gold1 position: (" << transform1.p.x << ", " << transform1.p.y << ", " << transform1.p.z << ")" << std::endl;
//		std::cout << "Gold2 position: (" << transform2.p.x << ", " << transform2.p.y << ", " << transform2.p.z << ")" << std::endl;
//	}
//
//	// 종료
//	scene->release();
//	dispatcher->release();
//	physics->release();
//	foundation->release();
//
//	return 0;
//}

/// 필터, 콜백2
// PhysX 관련 전역 변수들
PxDefaultAllocator gAllocator;
PxDefaultErrorCallback gErrorCallback;
PxFoundation* gFoundation = nullptr;
PxPhysics* gPhysics = nullptr;
PxScene* gScene = nullptr;
PxMaterial* gMaterial = nullptr;
PxDefaultCpuDispatcher* gDispatcher = nullptr;

// PhysX 초기화 함수
bool InitializePhysX()
{
	// Foundation 생성
	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
	if (!gFoundation)
	{
		std::cerr << "PxCreateFoundation failed!" << std::endl;
		return false;
	}

	// Physics 생성
	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale());
	if (!gPhysics)
	{
		std::cerr << "PxCreatePhysics failed!" << std::endl;
		return false;
	}

	// Scene 생성
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);

	// Dispatcher 생성
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	if (!gDispatcher)
	{
		std::cerr << "PxDefaultCpuDispatcherCreate failed!" << std::endl;
		return false;
	}
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;

	gScene = gPhysics->createScene(sceneDesc);
	if (!gScene)
	{
		std::cerr << "createScene failed!" << std::endl;
		return false;
	}

	// Material 생성
	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);
	if (!gMaterial)
	{
		std::cerr << "createMaterial failed!" << std::endl;
		return false;
	}

	return true;
}

// PhysX 리소스 해제 함수
void CleanupPhysX()
{
	if (gScene) gScene->release();
	if (gDispatcher) gDispatcher->release();
	if (gPhysics) gPhysics->release();
	if (gFoundation) gFoundation->release();
}

// 기존의 `PxShape`를 복제하여 새로운 `PxShape`를 생성하는 함수
PxShape* CreateUniqueShape(PxRigidActor* actor, PxShape* originalShape)
{
	// 먼저, originalShape의 Material 배열을 가져옴
	PxMaterial* materials[1]; // 최대 8개의 Material이 있다고 가정
	PxU32 materialCount = originalShape->getMaterials(materials, 1); // Material 배열과 개수를 가져옴

	// 복제된 새로운 shape 생성
	PxShape* newShape = gPhysics->createShape(
		originalShape->getGeometry(), materials, materialCount, true, originalShape->getFlags()
	);

	// 새로운 shape를 엑터에 첨부
	actor->attachShape(*newShape);

	return newShape;
}

// 엑터의 필터 데이터를 수정하는 함수 (이제 복제된 shape에만 적용)
void UpdateActorFilterData(PxRigidActor* actor, PxU32 word0, PxU32 word1)
{
	PxShape* shapes[1];
	PxU32 shapeCount = actor->getShapes(shapes, 1);

	for (PxU32 i = 0; i < shapeCount; ++i)
	{
		// 기존 셰이프 복제 후 수정
		PxShape* uniqueShape = CreateUniqueShape(actor, shapes[i]);

		// 복제된 shape의 필터 데이터 수정
		PxFilterData filterData = uniqueShape->getSimulationFilterData();
		filterData.word0 = word0;
		filterData.word1 = word1;
		uniqueShape->setSimulationFilterData(filterData);
	}
}

// 물리 시뮬레이션 루프
void SimulationLoop(std::vector<PxRigidActor*> actors)
{
	static int a = 0;
	while (true) // 물리 루프
	{
		a++;
		// 루프 외부에서 특정 조건에 따라 필터 데이터 수정
		for (auto actor : actors)
		{
			if (a == 100)
			{
				UpdateActorFilterData(actor, a, a+1); // 예시 필터 데이터 설정
			}
			if (a == 200)
			{
				UpdateActorFilterData(actor, a, a+1); // 예시 필터 데이터 설정
			}
		}

		// 물리 시뮬레이션을 한 스텝 진행
		gScene->simulate(1.0f / 60.0f); // 60FPS 기준으로 시뮬레이션
		gScene->fetchResults(true);

		// 물리 루프 내부에서 각 엑터 처리
		for (auto actor : actors)
		{
			// 엑터에 연결된 셰이프의 필터 데이터를 출력
			PxShape* shapes[8];
			PxU32 shapeCount = actor->getShapes(shapes, 8);

			for (PxU32 i = 0; i < shapeCount; ++i)
			{
				PxFilterData filterData = shapes[i]->getSimulationFilterData();
				std::cout << "Actor Filter Data (word0: " << filterData.word0
					<< ", word1: " << filterData.word1
					<< ", word2: " << filterData.word2
					<< ", word3: " << filterData.word3
					<< ")" << std::endl;
			}
		}

		// 루프를 중단하는 조건 확인 (예: ESC 키 입력, 특정 조건 충족 등)
 		if (a==105 || a==205)
 		{
 			break;
 		}
	}
}

// 예시: 동적 엑터 생성 함수
PxRigidDynamic* CreateDynamicActor(const PxTransform& transform)
{
	PxRigidDynamic* dynamicActor = gPhysics->createRigidDynamic(transform);
	PxShape* shape = gPhysics->createShape(PxBoxGeometry(2.0f, 2.0f, 2.0f), *gMaterial);
	dynamicActor->attachShape(*shape);
	gScene->addActor(*dynamicActor);
	return dynamicActor;
}

int main()
{
	// PhysX 초기화
	if (!InitializePhysX())
	{
		std::cerr << "PhysX initialization failed!" << std::endl;
		return -1;
	}

	// 예시: 동적 엑터 생성 및 물리 시뮬레이션에 추가
	std::vector<PxRigidActor*> actors;
	PxRigidDynamic* actor = CreateDynamicActor(PxTransform(PxVec3(0, 10, 0)));
	actors.push_back(actor);

	// 물리 시뮬레이션 루프 실행
	SimulationLoop(actors);

	// 물리 시뮬레이션 루프 실행
	SimulationLoop(actors);

	// PhysX 리소스 해제
	CleanupPhysX();

	return 0;
}

