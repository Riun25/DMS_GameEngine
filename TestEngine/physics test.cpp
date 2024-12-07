#include <physx/PxPhysicsAPI.h>
#include <iostream>
#include <vector>
using namespace physx;
#define DLOG(level, message) std::cout << message << std::endl

/// main test
// 
// using namespace physx;
// 
// // PhysX SDK �ʱ�ȭ
// PxDefaultAllocator      gAllocator;
// PxDefaultErrorCallback  gErrorCallback;
// PxFoundation* gFoundation = nullptr;
// PxPhysics* gPhysics = nullptr;
// PxDefaultCpuDispatcher* gDispatcher = nullptr;
// PxScene* gScene = nullptr;
// PxMaterial* gMaterial = nullptr;
// 
// // ����� ���� ���� ���� Ŭ����
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
// // ���� �߰�
// PxRigidDynamic* createDynamicBox(const PxTransform& t, const PxVec3& dimensions, const PxVec3& velocity = PxVec3(0)) {
// 	PxBoxGeometry geometry(dimensions);
// 	PxRigidDynamic* box = PxCreateDynamic(*gPhysics, t, geometry, *gMaterial, 10.0f);
// 	box->setLinearVelocity(velocity);
// 	box->setName("Box");
// 	gScene->addActor(*box);
// 	return box;
// }
// 
// // ���� �߰�
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
// 	/// �浹 �̺�Ʈ �ݹ� �޼��� 
// 	/// </summary>
// 	/// <param name="_pairHeader">�浹�� �� ��ü�� ���� ������ �����ϴ� ����ü�Դϴ�.</param>
// 	/// <param name="_pairs">�浹�� ��ü �ֵ��� �迭�Դϴ�. �� �迭 ��Ҵ� �浹 �ֿ� ���� ���� ������ �����մϴ�.</param>
// 	/// <param name="_nbPairs">�浹�� ��ü ���� ���� ��Ÿ���ϴ�.</param>
// 	void onContact(const physx::PxContactPairHeader& _pairHeader, const physx::PxContactPair* _pairs, physx::PxU32 _nbPairs) override;
// 
// 	/// <summary>
// 	/// Ʈ���� �̺�Ʈ �ݹ� �޼��� 
// 	/// </summary>
// 	/// <param name="_pairs">Ʈ���ŵ� ��ü �ֵ��� �迭�Դϴ�. �� �迭 ��Ҵ� Ʈ���� �̺�Ʈ�� ���� ������ �����մϴ�.</param>
// 	/// <param name="_count">Ʈ���ŵ� ��ü ���� ���� ��Ÿ���ϴ�.</param>
// 	void onTrigger(physx::PxTriggerPair* _pairs, physx::PxU32 _count) override;
// 
// 	/// <summary>
// 	/// ���� ���� �ı� �̺�Ʈ �ݹ� �޼��� 
// 	/// </summary>
// 	/// <param name="_constraints">�ı��� ������� �迭�Դϴ�. �� �迭 ��Ҵ� �ı��� ���࿡ ���� ������ �����մϴ�.</param>
// 	/// <param name="_count">�ı��� ������ ���� ��Ÿ���ϴ�.</param>
// 	void onConstraintBreak(physx::PxConstraintInfo* _constraints, physx::PxU32 _count) override;
// 
// 	/// <summary>
// 	/// ��ü�� ����� �̺�Ʈ �ݹ� �޼��� 
// 	/// </summary>
// 	/// <param name="_actors">��� ��ü�鿡 ���� ������ �迭�Դϴ�.</param>
// 	/// <param name="_count">��� ��ü�� ���� ��Ÿ���ϴ�.</param>
// 	void onWake(physx::PxActor** _actors, physx::PxU32 _count) override;
// 
// 	/// <summary>
// 	/// ��ü�� ���� �̺�Ʈ �ݹ� �޼��� 
// 	/// </summary>
// 	/// <param name="_actors">��� ��ü�鿡 ���� ������ �迭�Դϴ�.</param>
// 	/// <param name="_count">��� ��ü�� ���� ��Ÿ���ϴ�.</param>
// 	void onSleep(physx::PxActor** _actors, physx::PxU32 _count) override;
// 
// 	/// <summary>
// 	/// �ùķ��̼� ���� ������Ʈ �̺�Ʈ �ݹ� �޼��� 
// 	/// </summary>
// 	/// <param name="_bodyBuffer">������Ʈ�� ��ü ��ü���� ������ �迭�Դϴ�.</param>
// 	/// <param name="_poseBuffer">�� ��ü ��ü�� ���ο� ��ġ�� ȸ���� ��Ÿ���� �迭�Դϴ�.</param>
// 	/// <param name="_count">������Ʈ�� ��ü�� ���� ��Ÿ���ϴ�.</param>
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
// 	// ����ü�� ĳ���Ϳ� �浹���� ���� �̺�Ʈ ó�� ����
// }
// 
// void CustomCallback::HandleMeleeWeaponHit(physx::PxShape* weapon, physx::PxShape* target)
// {
// 	// �ٰŸ� ���Ⱑ ĳ���Ϳ� �浹���� ���� �̺�Ʈ ó�� ����
// }
// 
// void CustomCallback::HandleCoinCollision(physx::PxShape* coin, physx::PxShape* character)
// {
// 	// ������ ĳ���Ϳ� �浹���� ���� �̺�Ʈ ó�� ����
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
//  	box->userData = &filter1; // ���� ������ ����
// 	m_pScene->addActor(*box);
// 
// 	// Add another dynamic actor (box)
// 	PxTransform boxTransform2(PxVec3(0, 0, 0));
// 	PxRigidDynamic* box2 = PxCreateDynamic(*m_pPhysics, boxTransform2, boxGeometry, *gMaterial, 10.0f);
// 	PxFilterData filter2{ 1,0,0,0 };
// 	box2->userData = &filter2; // ���� ������ ����
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

/// ����ü �׽�Ʈ
//// ���� �׷� �� �÷��� ����
//#define FILTER_GROUP_PROJECTILE 1
//#define FILTER_FLAG_ON_GROUND (1 << 0) // onGround �÷���
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
//	// �浹 �� onGround �÷��� ����
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
//	// �浹�� ��ü�� ���߰� �ϱ�
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
//				if (filterData.word1 & FILTER_FLAG_ON_GROUND) // onGround �÷��� Ȯ��
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
//	sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f); // �߷� ����
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
//	PxMaterial* gMaterial = m_pPhysics->createMaterial(0.5f, 0.5f, 0.0f); // ź�� ����� 0���� ����
//	PxRigidStatic* groundPlane = PxCreatePlane(*m_pPhysics, PxPlane(0, 1, 0, 0), *gMaterial);
//	m_pScene->addActor(*groundPlane);
//
//	// Add a dynamic actor (box)
//	PxTransform boxTransform(PxVec3(0, 10, 0));
//	PxBoxGeometry boxGeometry(PxVec3(1, 1, 1));
//	PxRigidDynamic* box = PxCreateDynamic(*m_pPhysics, boxTransform, boxGeometry, *gMaterial, 10.0f);
//	box->setAngularDamping(0.5f); // ȸ�� ���� ����
//
//	// �ʱ� �ӵ� ���� (��: x �������� 10, y �������� 10)
//	PxVec3 initialVelocity(10.0f, 10.0f, 0.0f);
//	box->setLinearVelocity(initialVelocity);
//
//	// ���� ������ ����
//	PxFilterData filterData;
//	filterData.word0 = FILTER_GROUP_PROJECTILE; // ����ü �׷� ����
//	filterData.word1 = 0; // �ʱ� ���¿��� onGround �÷��� ��Ȱ��ȭ
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

/// ����ü �׽�Ʈ 2
//// PhysX ���� ����
//PxDefaultAllocator gAllocator;
//PxDefaultErrorCallback gErrorCallback;
//PxFoundation* gFoundation = nullptr;
//PxPhysics* gPhysics = nullptr;
//PxDefaultCpuDispatcher* gDispatcher = nullptr;
//PxScene* gScene = nullptr;
//PxMaterial* gMaterial = nullptr;
//
//// �߷� ����
//PxVec3 gGravity(0.0f, -9.81f, 0.0f);
//
//// �ʱ�ȭ �Լ�
//void initPhysX() {
//	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
//	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale());
//	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
//	sceneDesc.gravity = gGravity;
//	gDispatcher = PxDefaultCpuDispatcherCreate(2);
//	sceneDesc.cpuDispatcher = gDispatcher;
//	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
//	gScene = gPhysics->createScene(sceneDesc);
//	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f); // ���� �� �ݹ� ���
//}
//
//// ���� �Լ�
//void cleanupPhysX() {
//	gScene->release();
//	gDispatcher->release();
//	gPhysics->release();
//	gFoundation->release();
//}
//
//// �ùķ��̼� �Լ�
//void simulateProjectile() {
//	// ��ü ����
//	PxTransform startTransform(PxVec3(0.0f, 0.0f, 0.0f));
//	PxRigidDynamic* sphere = PxCreateDynamic(*gPhysics, startTransform, PxSphereGeometry(0.5f), *gMaterial, 1.0f);
//	sphere->setName("sphere");
//	gScene->addActor(*sphere);
//
//	// �ʱ� �ӵ� ���� (������ �)
//	PxVec3 initialVelocity(5.0f, 10.0f, 5.0f); // ���� �ӵ��� ���� ������ �
//	sphere->setLinearVelocity(initialVelocity);
//
//	// �ùķ��̼� ����
//	for (int i = 0; i < 500; ++i) { // 5�� ���� �ùķ��̼� (0.01�� ����)
//		gScene->simulate(0.01f); // �ùķ��̼� ����
//		gScene->fetchResults(true);
//
//		// ��ü ��ġ ������Ʈ
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

/// ĳ���� ��Ʈ�ѷ� �׽�Ʈ
// ���� ��ü ����
//PxDefaultAllocator		gAllocator;
//PxDefaultErrorCallback	gErrorCallback;
//
//PxFoundation* gFoundation = nullptr;
//PxPhysics* gPhysics = nullptr;
//PxControllerManager* gControllerManager = nullptr;
//PxScene* gScene = nullptr;
//PxMaterial* gMaterial = nullptr;
//
//// PhysX �ʱ�ȭ �Լ�
//void initPhysX()
//{
//	// PhysX ���� ��ü ����
//	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
//	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale());
//
//	// �� ����
//	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
//	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
//
//	// ����ġ�� ���� ���̴� ����
//	sceneDesc.cpuDispatcher = PxDefaultCpuDispatcherCreate(2);
//	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
//
//	gScene = gPhysics->createScene(sceneDesc);
//
//	// ��Ʈ�ѷ� ������ ����
//	gControllerManager = PxCreateControllerManager(*gScene);
//
//	// �⺻ ���� ����
//	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.5f);
//}
//
//// ĸ�� ��Ʈ�ѷ� ���� �Լ�
//PxCapsuleController* createCapsuleController()
//{
//	// ĸ�� ��Ʈ�ѷ� ����
//	PxCapsuleControllerDesc desc;
//	desc.height = 2.0f; // ĸ�� ����
//	desc.radius = 0.5f; // ĸ�� ������
//	desc.position = PxExtendedVec3(0, 10, 0); // �ʱ� ��ġ
//	desc.material = gMaterial; // ���� ����
//	desc.contactOffset = 0.1f;
//	desc.stepOffset = 0.5f;
//	desc.slopeLimit = 0.707f; // 45��
//
//	// ĸ�� ��Ʈ�ѷ� ����
//	PxCapsuleController* capsuleController = static_cast<PxCapsuleController*>(gControllerManager->createController(desc));
//	return capsuleController;
//}
//
//// ĳ���� �̵� �Լ�
//void moveCharacter(PxCapsuleController* capsuleController, const PxVec3& direction, float elapsedTime)
//{
//	PxControllerFilters filters;
//	PxVec3 displacement = direction * elapsedTime;
//	PxControllerState state;
//
//	capsuleController->move(displacement, 0.001f, elapsedTime, filters);
//
//	// ���� Ȯ�� �� �ð�ȭ
//	auto position = capsuleController->getPosition();
//	std::cout << "Character Position: ("
//		<< position.x << ", "
//		<< position.y << ", "
//		<< position.z << ")\n";
//}
//
//// PhysX ���� �Լ�
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
//	// PhysX �ʱ�ȭ
//	initPhysX();
//
//	// ĸ�� ��Ʈ�ѷ� ����
//	PxCapsuleController* capsuleController = createCapsuleController();
//
//	// ���� ����
//	bool running = true;
//	while (running)
//	{
//		// �ð� ���
//		float elapsedTime = 1.0f / 60.0f; // 60 FPS
//
//		// ����� �Է¿� ���� �̵� (�ӽ÷� ��� �� ���)
//		PxVec3 direction(1.0f, 0.0f, 0.0f); // ���������� �̵�
//
//		// ĳ���� �̵�
//		moveCharacter(capsuleController, direction, elapsedTime);
//
//		// �� ����
//		gScene->simulate(elapsedTime);
//		gScene->fetchResults(true);
//
//		// ���� ���� (���⼭�� ������ ������ ���� ����)
//		static int frameCount = 0;
//		if (++frameCount > 300) // �� 5�� �� ����
//			running = false;
//	}
//
//	// PhysX ����
//	cleanupPhysX();
//
//	return 0;
//}

/// ����, �ݹ�
//// �浹 �׷� ��Ʈ ����
//enum CollisionType
//{
//	TYPE_GROUND = 1 << 0, // ��
//	TYPE_UNIT = 1 << 1, // ����
//	TYPE_PROJECTILE = 1 << 2, // ����ü
//	TYPE_WEAPON = 1 << 3, // ����
//	TYPE_GOLD = 1 << 4, // ��ȭ
//	TYPE_OBSTACLE = 1 << 5  // ��ֹ�
//};
//
//enum CollisionAttribute
//{
//	ATTR_ALLY = 1 << 0, // �Ʊ�
//	ATTR_ENEMY = 1 << 1, // ����
//	ATTR_ON_GROUND = 1 << 2, // ���鿡 �پ� �ִ��� ����
//	ATTR_IN_AIR = 1 << 3  // ���߿� �ִ��� ����
//};
//
//// �浹 ���� ���̴� ����
//PxFilterFlags FilterShader(
//	PxFilterObjectAttributes attributes0, PxFilterData filterData0,
//	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
//	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
//{
//	// ��ȭ�� �ٸ� ��ȭ�� �浹���� ����
//	if ((filterData0.word0 & TYPE_GOLD) &&
//		(filterData1.word0 & TYPE_GOLD))
//	{
//		return PxFilterFlag::eSUPPRESS;
//	}
//
//	// ���ְ� ��ֹ��� ��ø�� �� ����
//	if ((filterData0.word0 & TYPE_UNIT) &&
//		(filterData1.word0 & TYPE_UNIT))
//	{
//		pairFlags = PxPairFlag::eCONTACT_DEFAULT;
//		return PxFilterFlag::eDEFAULT;
//	}
//
//	// ��ȭ�� ������ ��ȣ�ۿ� ����
//	if ((filterData0.word0 & TYPE_GOLD) &&
//		(filterData1.word0 & TYPE_GROUND))
//	{
//		pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
//		return PxFilterFlag::eDEFAULT;
//	}
//
//	// ����ü�� ��ֹ��� ����� �� ����
//	if (((filterData0.word0 & TYPE_PROJECTILE) &&
//		(filterData1.word0 & TYPE_OBSTACLE)) ||
//		((filterData1.word0 & TYPE_PROJECTILE) &&
//			(filterData0.word0 & TYPE_OBSTACLE)))
//	{
//		pairFlags = PxPairFlag::eCONTACT_DEFAULT;
//		return PxFilterFlag::eDEFAULT;
//	}
//
//	// ����� ��� ������Ʈ�� ����� �� ����
//	if ((filterData0.word0 & TYPE_WEAPON) ||
//		(filterData1.word0 & TYPE_WEAPON))
//	{
//		return PxFilterFlag::eSUPPRESS;
//	}
//
//	// �⺻ ��� �÷��� ����
//	pairFlags = PxPairFlag::eCONTACT_DEFAULT;
//	return PxFilterFlag::eDEFAULT;
//}
//
//// �浹 �ݹ� �ڵ鷯 ����
//class CollisionCallback : public PxSimulationEventCallback
//{
//public:
//	void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override
//	{
//		for (PxU32 i = 0; i < nbPairs; i++)
//		{
//			const PxContactPair& cp = pairs[i];
//
//			// �Ʊ� �� ���� ����ü�� ���� ���� ���� ó��
//			if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
//			{
//				// ���Ϳ��� ù ��° ������ ��������
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
//					// �Ʊ� ����ü���� ���� ����ü���� Ȯ��
//					if ((filterData0.word0 & TYPE_PROJECTILE) && (filterData0.word1 & ATTR_ALLY))
//					{
//						// �Ʊ� ����ü ������ ó�� ���� ����
//						std::cout << "Ally projectile hit something!" << std::endl;
//					}
//					else if ((filterData1.word0 & TYPE_PROJECTILE) && (filterData1.word1 & ATTR_ALLY))
//					{
//						// �Ʊ� ����ü ������ ó�� ���� ����
//						std::cout << "Ally projectile hit something!" << std::endl;
//					}
//
//					if ((filterData0.word0 & TYPE_PROJECTILE) && (filterData0.word1 & ATTR_ENEMY))
//					{
//						// ���� ����ü ������ ó�� ���� ����
//						std::cout << "Enemy projectile hit something!" << std::endl;
//					}
//					else if ((filterData1.word0 & TYPE_PROJECTILE) && (filterData1.word1 & ATTR_ENEMY))
//					{
//						// ���� ����ü ������ ó�� ���� ����
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
//			// ��ȭ�� ���� ����� ���� ���� ���� ó��
//			if (tp.status & PxPairFlag::eNOTIFY_TOUCH_FOUND)
//			{
//				// ���Ϳ��� ù ��° ������ ��������
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
//						// ���� ���� ����
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
//	// PhysX �ʱ�ȭ
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
//	// ���� ���� ����
//	PxMaterial* material = physics->createMaterial(1.f, 1.f, 0.f);
//
//	// ���� ����
//	PxRigidStatic* ground = PxCreatePlane(*physics, PxPlane(0, 1, 0, 0), *material);
//	scene->addActor(*ground);
//
//	// ��ȭ 1 ����
//	PxRigidDynamic* gold1 = PxCreateDynamic(*physics, PxTransform(PxVec3(0, 5, 0)), PxSphereGeometry(5.f), *material, 1.0f);
//	gold1->setAngularDamping(0.f);
//	PxShape* shape1;
//	gold1->getShapes(&shape1, 1);
//	PxFilterData filterData1;
//	filterData1.word0 = TYPE_GOLD;
//	shape1->setSimulationFilterData(filterData1);
//	scene->addActor(*gold1);
//
//	// ��ȭ 2 ����
//	PxRigidDynamic* gold2 = PxCreateDynamic(*physics, PxTransform(PxVec3(0, 10, 1)), PxSphereGeometry(5.f), *material, 1.0f);
//	gold2->setAngularDamping(0.f);
//	PxShape* shape2;
//	gold2->getShapes(&shape2, 1);
//	PxFilterData filterData2;
//	filterData2.word0 = TYPE_GOLD;
//	shape2->setSimulationFilterData(filterData2);
//	scene->addActor(*gold2);
//
//	// ���� ���� ���� ����
//	for (int i = 0; i < 300; i++)
//	{
//		scene->simulate(1.0f / 60.0f);
//		scene->fetchResults(true);
//
//		// ��ȭ ��ġ ���
//		PxTransform transform1 = gold1->getGlobalPose();
//		PxTransform transform2 = gold2->getGlobalPose();
//		std::cout << "Gold1 position: (" << transform1.p.x << ", " << transform1.p.y << ", " << transform1.p.z << ")" << std::endl;
//		std::cout << "Gold2 position: (" << transform2.p.x << ", " << transform2.p.y << ", " << transform2.p.z << ")" << std::endl;
//	}
//
//	// ����
//	scene->release();
//	dispatcher->release();
//	physics->release();
//	foundation->release();
//
//	return 0;
//}

/// ����, �ݹ�2
// PhysX ���� ���� ������
PxDefaultAllocator gAllocator;
PxDefaultErrorCallback gErrorCallback;
PxFoundation* gFoundation = nullptr;
PxPhysics* gPhysics = nullptr;
PxScene* gScene = nullptr;
PxMaterial* gMaterial = nullptr;
PxDefaultCpuDispatcher* gDispatcher = nullptr;

// PhysX �ʱ�ȭ �Լ�
bool InitializePhysX()
{
	// Foundation ����
	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
	if (!gFoundation)
	{
		std::cerr << "PxCreateFoundation failed!" << std::endl;
		return false;
	}

	// Physics ����
	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale());
	if (!gPhysics)
	{
		std::cerr << "PxCreatePhysics failed!" << std::endl;
		return false;
	}

	// Scene ����
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);

	// Dispatcher ����
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

	// Material ����
	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);
	if (!gMaterial)
	{
		std::cerr << "createMaterial failed!" << std::endl;
		return false;
	}

	return true;
}

// PhysX ���ҽ� ���� �Լ�
void CleanupPhysX()
{
	if (gScene) gScene->release();
	if (gDispatcher) gDispatcher->release();
	if (gPhysics) gPhysics->release();
	if (gFoundation) gFoundation->release();
}

// ������ `PxShape`�� �����Ͽ� ���ο� `PxShape`�� �����ϴ� �Լ�
PxShape* CreateUniqueShape(PxRigidActor* actor, PxShape* originalShape)
{
	// ����, originalShape�� Material �迭�� ������
	PxMaterial* materials[1]; // �ִ� 8���� Material�� �ִٰ� ����
	PxU32 materialCount = originalShape->getMaterials(materials, 1); // Material �迭�� ������ ������

	// ������ ���ο� shape ����
	PxShape* newShape = gPhysics->createShape(
		originalShape->getGeometry(), materials, materialCount, true, originalShape->getFlags()
	);

	// ���ο� shape�� ���Ϳ� ÷��
	actor->attachShape(*newShape);

	return newShape;
}

// ������ ���� �����͸� �����ϴ� �Լ� (���� ������ shape���� ����)
void UpdateActorFilterData(PxRigidActor* actor, PxU32 word0, PxU32 word1)
{
	PxShape* shapes[1];
	PxU32 shapeCount = actor->getShapes(shapes, 1);

	for (PxU32 i = 0; i < shapeCount; ++i)
	{
		// ���� ������ ���� �� ����
		PxShape* uniqueShape = CreateUniqueShape(actor, shapes[i]);

		// ������ shape�� ���� ������ ����
		PxFilterData filterData = uniqueShape->getSimulationFilterData();
		filterData.word0 = word0;
		filterData.word1 = word1;
		uniqueShape->setSimulationFilterData(filterData);
	}
}

// ���� �ùķ��̼� ����
void SimulationLoop(std::vector<PxRigidActor*> actors)
{
	static int a = 0;
	while (true) // ���� ����
	{
		a++;
		// ���� �ܺο��� Ư�� ���ǿ� ���� ���� ������ ����
		for (auto actor : actors)
		{
			if (a == 100)
			{
				UpdateActorFilterData(actor, a, a+1); // ���� ���� ������ ����
			}
			if (a == 200)
			{
				UpdateActorFilterData(actor, a, a+1); // ���� ���� ������ ����
			}
		}

		// ���� �ùķ��̼��� �� ���� ����
		gScene->simulate(1.0f / 60.0f); // 60FPS �������� �ùķ��̼�
		gScene->fetchResults(true);

		// ���� ���� ���ο��� �� ���� ó��
		for (auto actor : actors)
		{
			// ���Ϳ� ����� �������� ���� �����͸� ���
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

		// ������ �ߴ��ϴ� ���� Ȯ�� (��: ESC Ű �Է�, Ư�� ���� ���� ��)
 		if (a==105 || a==205)
 		{
 			break;
 		}
	}
}

// ����: ���� ���� ���� �Լ�
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
	// PhysX �ʱ�ȭ
	if (!InitializePhysX())
	{
		std::cerr << "PhysX initialization failed!" << std::endl;
		return -1;
	}

	// ����: ���� ���� ���� �� ���� �ùķ��̼ǿ� �߰�
	std::vector<PxRigidActor*> actors;
	PxRigidDynamic* actor = CreateDynamicActor(PxTransform(PxVec3(0, 10, 0)));
	actors.push_back(actor);

	// ���� �ùķ��̼� ���� ����
	SimulationLoop(actors);

	// ���� �ùķ��̼� ���� ����
	SimulationLoop(actors);

	// PhysX ���ҽ� ����
	CleanupPhysX();

	return 0;
}

