#include "PhysicsManager.h"
#include "CollisionCallback.h"
#include "Entity.h"
#include "Transform.h"
#include "Collider.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "CharacterController.h"
#include "Rigidbody.h" 
#include "CharacterController.h" 
#include "PlaneComponent.h" 
#include "CameraComponent.h"
#include "GameEngine.h"
#include "WorldManager.h"
#include "Scene.h"
#include "RaycastQueryFilterCallback.h"
#include "Name.h"

physx::PxFilterFlags CustomFilterShader(physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
	physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
	physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize)
{
	pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT | physx::PxPairFlag::eNOTIFY_TOUCH_FOUND | physx::PxPairFlag::eNOTIFY_TOUCH_LOST;
	return physx::PxFilterFlag::eDEFAULT;
}

PhysicsManager::PhysicsManager(entt::registry& _registry)
	: mRegistry(_registry)
	, mpFoundation(nullptr)
	, mpPhysics(nullptr)
	, mpDispatcher(nullptr)
	, mpScene(nullptr)
	, mpMaterial(nullptr)
	, mpControllerManager(nullptr)
	, mpEngine(nullptr)
	// 	, m_pCollisionCallback(nullptr)
{
}

PhysicsManager::PhysicsManager(entt::registry& _registry, GameEngine* _pEngine)
	: mRegistry(_registry)
	, mpFoundation(nullptr)
	, mpPhysics(nullptr)
	, mpDispatcher(nullptr)
	, mpScene(nullptr)
	, mpMaterial(nullptr)
	, mpControllerManager(nullptr)
	, mpEngine(_pEngine)
{

}

bool PhysicsManager::Initialize()
{
	std::string strVersion = std::to_string(PX_PHYSICS_VERSION_MAJOR) + "."
		+ std::to_string(PX_PHYSICS_VERSION_MINOR) + "."
		+ std::to_string(PX_PHYSICS_VERSION_BUGFIX);
	DLOG(LOG_INFO, "PhysX version: " + strVersion);

	// Foundation 생성 : 메모리 할당 및 에러 콜백 관리
	mpFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, mAllocator, mErrorCallback);
	if (!mpFoundation)
	{
		DLOG(LOG_ERROR, "PxCreateFoundation Failed.");
		return false;
	}

#ifdef _DEBUG
	// pvd 생성
	m_pPvd = physx::PxCreatePvd(*mpFoundation);
	m_pTransport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);

	// PVD를 네트워크 소켓으로 연결 (IP: 127.0.0.1, 포트: 5425)
	m_pPvd->connect(*m_pTransport, physx::PxPvdInstrumentationFlag::eALL);
#endif

	// Physics 객체 생성
#ifdef _DEBUG
	mpPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mpFoundation, physx::PxTolerancesScale(), true, m_pPvd);
#else 
	mpPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mpFoundation, physx::PxTolerancesScale(), true);
#endif

	if (!mpPhysics)
	{
		DLOG(LOG_ERROR, "PxCreatePhysics Failed.");
		return false;
	}

	// Scene 생성 : 물리 시뮬레이션 환경 설정
	physx::PxSceneDesc sceneDesc(mpPhysics->getTolerancesScale());
	sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);

	// CPU Dispatcher 생성 : 시뮬레이션 작업을 여러 스레드에 분산
	mpDispatcher = physx::PxDefaultCpuDispatcherCreate(4);
	if (!mpDispatcher)
	{
		DLOG(LOG_ERROR, "PxDefaultCpuDispatcherCreate Failed.");
		return false;
	}
	sceneDesc.cpuDispatcher = mpDispatcher;
	sceneDesc.filterShader = CustomFilterShader;

	// Collision Callback 설정
	auto m_pCollisionCallback = new CollisionCallback();
	sceneDesc.simulationEventCallback = m_pCollisionCallback;

	// 	sceneDesc.

	mpScene = mpPhysics->createScene(sceneDesc);
	if (!mpScene)
	{
		DLOG(LOG_ERROR, "createScene Failed.");
		return false;
	}

	// Material 생성 : 마찰, 반발 계수 등 물리 객체의 물리적 특성 정의 // (정적 마찰 계수, 동적 마찰 계수, 반발 계수)
	mpMaterial = mpPhysics->createMaterial(1.f, 1.f, 0.f);
	mpMaterial->setFrictionCombineMode(physx::PxCombineMode::eMAX);
	if (!mpMaterial)
	{
		DLOG(LOG_ERROR, "createMaterial Failed.");
		return false;
	}

	// controller manager 생성
	mpControllerManager = PxCreateControllerManager(*mpScene);
	if (!mpControllerManager)
	{
		DLOG(LOG_ERROR, "PxCreateControllerManager Failed.");
		return false;
	}

	return true;
}

void PhysicsManager::CreatePhysicsScene(physx::PxSceneDesc _sceneDesc)
{
	/// 씬 초기화
	if (mpScene)
	{
		mpScene->release();
		mpScene = nullptr;
	}

	/// Scene 재생성
	mpScene = mpPhysics->createScene(_sceneDesc);
	if (!mpScene)
	{
		DLOG(LOG_ERROR, "createScene Failed.");
	}
}

physx::PxSceneDesc PhysicsManager::CreateSceneDesc(physx::PxSimulationEventCallback* _callback, physx::PxSimulationFilterShader _filterShader)
{
	// Scene 생성 : 물리 시뮬레이션 환경 설정
	physx::PxSceneDesc sceneDesc(mpPhysics->getTolerancesScale());
	sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);

	sceneDesc.cpuDispatcher = mpDispatcher;
	sceneDesc.filterShader = _filterShader;

	// Collision Callback 설정
	sceneDesc.simulationEventCallback = _callback;

	return sceneDesc;
}

void PhysicsManager::FixedUpdate(float _fixedDTime)
{
	// 3D 오브젝트 피킹
	//PickObejct();

	// 이전에 큐에 저장된 필터 데이터 수정 사항 처리
	while (!mFilterDataUpdates.empty())
	{
		auto& update = mFilterDataUpdates.front();
		if (update.first)
		{
			update.first->setSimulationFilterData(update.second);
		}
		mFilterDataUpdates.pop();
	}

	// 변경된 리지드바디 정보를 엑터에 적용
	auto rigidbodyView = mRegistry.view<Rigidbody>();
	for (auto entity : rigidbodyView)
	{
		auto rigidbody = mRegistry.try_get<Rigidbody>(entity);

		if (rigidbody->mpRigidActor)
		{
			// 리지드 엑터를 리지드 다이나믹으로 캐스팅
			if (physx::PxRigidDynamic* dynamic = rigidbody->mpRigidActor->is<physx::PxRigidDynamic>())
			{
				// 리지드 다이나믹의 물리적 속성 업데이트
				dynamic->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, rigidbody->mIsKinematic);			// 키네마틱 설정	// 물리 시뮬레이션에서 힘을 받지 않으며, 직접 위치를 제어할 수 있음.
				dynamic->setMass(rigidbody->mMass);																// 질량 설정
				dynamic->setLinearDamping(rigidbody->mDrag);														// 저항 계수 설정
				dynamic->setAngularDamping(rigidbody->mAngularDrag);												// 회전 저항 계수 설정
				dynamic->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !rigidbody->mUseGravity);				// 중력 적용

				// 엑터에 연결된 셰이프 가져오기
				physx::PxShape* shape = nullptr;
				dynamic->getShapes(&shape, 1);

				// 셰이프에 연결된 마테리얼 가져오기
				if (shape)
				{
					physx::PxMaterial* material = nullptr;
					shape->getMaterials(&material, 1);

					// 마테리얼의 물리적 속성 업데이트
					if (material)
					{
						material->setDynamicFriction(rigidbody->mDynamicFriction);   // 동적 마찰 계수
						material->setStaticFriction(rigidbody->mStaticFriction);     // 정적 마찰 계수
						material->setRestitution(rigidbody->mRestitution);           // 반발 계수
					}
				}

			}
		}
	}

	// 씬 내부 객체 수 확인
	physx::PxU32 nbActors = mpScene->getNbActors(physx::PxActorTypeFlag::eRIGID_DYNAMIC);

	// 동적 객체 수가 1개 이상일 경우, physx 시뮬레이션 수행
	if (nbActors < 1)
	{
		// DLOG(LOG_INFO, "Physics Scene Is Empty. No Simulation Performed.");
	}
	else
	{
		// 씬의 물리 시뮬레이션
		mpScene->simulate(_fixedDTime);
		mpScene->fetchResults(true);

		// 각 엔티티들의 Transform 업데이트
		auto view = mRegistry.view<Transform, Rigidbody>();
		for (auto entity : view)
		{
			auto& transform = mRegistry.get<Transform>(entity);
			auto& rigidbody = mRegistry.get<Rigidbody>(entity);
			auto& actor = rigidbody.mpRigidActor;
			bool trigger = false;

			if (auto box = mRegistry.try_get<BoxCollider>(entity))
			{
				trigger = box->mIsTrigger;
			}
			else if (auto sphere = mRegistry.try_get<SphereCollider>(entity))
			{
				trigger = sphere->mIsTrigger;
			}
			else if (auto capsule = mRegistry.try_get<CapsuleCollider>(entity))
			{
				trigger = capsule->mIsTrigger;
			}

			if (actor)
			{
				// 콜라이더가 trigger on일 경우
				if (trigger)
				{
					physx::PxShape* shape = nullptr;
					actor->getShapes(&shape, 1);
					physx::PxShapeFlags currentFlags = shape->getFlags();
					// 
					if (shape->getFlags() & ~physx::PxShapeFlag::eTRIGGER_SHAPE)
					{
						currentFlags &= ~physx::PxShapeFlag::eSIMULATION_SHAPE;
						currentFlags |= physx::PxShapeFlag::eTRIGGER_SHAPE;
						shape->setFlags(currentFlags);
					}
				}
				// 콜라이더가 trigger off 일 경우
				else
				{
					physx::PxShape* shape = nullptr;
					actor->getShapes(&shape, 1);
					physx::PxShapeFlags currentFlags = shape->getFlags();
					if (shape->getFlags() & physx::PxShapeFlag::eTRIGGER_SHAPE)
					{
						currentFlags |= physx::PxShapeFlag::eSIMULATION_SHAPE;
						currentFlags &= ~physx::PxShapeFlag::eTRIGGER_SHAPE;
						shape->setFlags(currentFlags & ~physx::PxShapeFlag::eTRIGGER_SHAPE);
					}
				}

				if (!rigidbody.mIsStatic)
				{

					/// 스케일이 달라졌을 경우, 셰이프 수정
					if (transform.mpParent)
					{
						physx::PxShape* shape;
						actor->getShapes(&shape, 1); // 첫 번째 shape 가져오기

						// Shape의 타입을 확인
						physx::PxGeometryType::Enum geoType = shape->getGeometry().getType();

						switch (geoType)
						{
						case physx::PxGeometryType::eBOX:
						{
							// BoxGeometry 처리
							physx::PxBoxGeometry boxGeometry = static_cast<const physx::PxBoxGeometry&>(shape->getGeometry());
							// Transform의 스케일과 BoxCollider의 크기 비교
							auto& name = mRegistry.get<Name>(entity).mName;
							constexpr float epsilon = std::numeric_limits<float>::epsilon();

							if (fabsf(boxGeometry.halfExtents.x - transform.mLocalScale.x / 2.0f) > epsilon ||
								fabsf(boxGeometry.halfExtents.y - transform.mLocalScale.y / 2.0f) > epsilon ||
								fabsf(boxGeometry.halfExtents.z - transform.mLocalScale.z / 2.0f) > epsilon)
							{
								// Transform과 스케일이 다르다면 BoxGeometry 업데이트
								boxGeometry.halfExtents = physx::PxVec3(transform.mLocalScale.x / 2.0f,
									transform.mLocalScale.y / 2.0f,
									transform.mLocalScale.z / 2.0f);
								shape->setGeometry(boxGeometry); // Shape의 geometry 업데이트
							}
							break;
						}
						// 	case physx::PxGeometryType::eCAPSULE:
						// 	{
						// 		// CapsuleGeometry 처리
						// 		physx::PxCapsuleGeometry capsuleGeometry = static_cast<const physx::PxCapsuleGeometry&>(shape->getGeometry());
						// 
						// 		if (capsuleGeometry.radius != transform->mLocalScale.x / 2.0f || // x축으로 가정
						// 			capsuleGeometry.halfHeight != transform->mLocalScale.y / 2.0f) // y축으로 가정
						// 		{
						// 			// Transform에 맞게 CapsuleGeometry 업데이트
						// 			capsuleGeometry.radius = transform->mLocalScale.x / 2.0f;
						// 			capsuleGeometry.halfHeight = transform->mLocalScale.y / 2.0f;
						// 			shape->setGeometry(capsuleGeometry); // Shape의 geometry 업데이트
						// 		}
						// 		break;
						// 	}
						// 	case physx::PxGeometryType::eSPHERE:
						// 	{
						// 		// SphereGeometry 처리
						// 		physx::PxSphereGeometry sphereGeometry = static_cast<const physx::PxSphereGeometry&>(shape->getGeometry());
						// 		if (sphereGeometry.radius != transform->mLocalScale.x / 2.0f) // 반지름을 x축 스케일로 가정
						// 		{
						// 			// Transform에 맞게 SphereGeometry 업데이트
						// 			sphereGeometry.radius = transform->mLocalScale.x / 2.0f;
						// 			shape->setGeometry(sphereGeometry); // Shape의 geometry 업데이트
						// 		}
						// 		break;
						// 	}
						default:
							// 다른 geometry 타입은 처리하지 않음
							break;
						}
					}

					/// 엑터의 위치 변환
					if (!rigidbody.mIsKinematic)
					{
						physx::PxTransform pxTransform = actor->getGlobalPose();
						Quaternion qut = Quaternion(pxTransform.q.x, pxTransform.q.y, pxTransform.q.z, pxTransform.q.w);
						Vector3 vec = qut.ToEuler();
						if (auto box = mRegistry.try_get<BoxCollider>(entity))
						{
							transform.mLocalPosition = Vector3(pxTransform.p.x, pxTransform.p.y, pxTransform.p.z) - box->mCenter;
						}
						else if (auto sphere = mRegistry.try_get<SphereCollider>(entity))
						{
							transform.mLocalPosition = Vector3(pxTransform.p.x, pxTransform.p.y, pxTransform.p.z) - sphere->mCenter;
						}
						else if (auto capsule = mRegistry.try_get<CapsuleCollider>(entity))
						{
							transform.mLocalPosition = Vector3(pxTransform.p.x, pxTransform.p.y, pxTransform.p.z) - capsule->mCenter;
						}
					}
					// 키네마틱 객체의 경우, 객체의 트랜스폼 컴포넌트로부터 물리 객체의 위치를 변경시킴
					else
					{
						// 컴포넌트로부터 트랜스폼 값들을 가져옴
						Vector3 rot = transform.mLocalRotation /** (std::numbers::pi / 180.f)*/;
						Quaternion qut = Quaternion::CreateFromYawPitchRoll(rot);

						physx::PxVec3 pxPos;

						Vector3 center;
						if (auto box = mRegistry.try_get<BoxCollider>(entity))
						{
							center = box->mCenter;
						}
						else if (auto sphere = mRegistry.try_get<SphereCollider>(entity))
						{
							center = sphere->mCenter;
						}
						else if (auto capsule = mRegistry.try_get<CapsuleCollider>(entity))
						{
							center = capsule->mCenter;
						}
						pxPos = physx::PxVec3(transform.mLocalPosition.x + center.x
							, transform.mLocalPosition.y + center.y
							, transform.mLocalPosition.z + center.z);

						physx::PxQuat pxQut = physx::PxQuat(qut.x, qut.y, qut.z, qut.w);
						physx::PxTransform pxTransform = physx::PxTransform(pxPos, pxQut);

						// physx::PxTransform pxTransform = physx::PxTransform(pxPos, pxQut);
						// 엑터에 트랜스폼 대입
						actor->setGlobalPose(pxTransform);

					}
				}
			}

		}

		auto charView = mRegistry.view<Transform, CharacterController>();
		for (auto entity : charView)
		{
			auto transform = mRegistry.try_get<Transform>(entity);
			auto controller = mRegistry.try_get<CharacterController>(entity);

			if (transform && controller)
			{
				// PxExtendedVec3로부터 위치 정보를 가져옴
				physx::PxExtendedVec3 pxTransform = controller->mpController->getPosition();

				// transform의 mLocalPosition에 값을 할당
				transform->mLocalPosition = Vector3(pxTransform.x, pxTransform.y, pxTransform.z);
			}
		}
	}
}

void PhysicsManager::Update(float _dTime)
{
}

void PhysicsManager::LateUpdate(float _dTime)
{

}

void PhysicsManager::Finalize()
{
	if (mpScene)
	{
		mpScene->release();
		mpScene = nullptr;
	}

	if (mpDispatcher)
	{
		mpDispatcher->release();
		mpDispatcher = nullptr;
	}

	if (mpPhysics)
	{
		mpPhysics->release();
		mpPhysics = nullptr;
	}

#ifdef _DEBUG
	if (m_pTransport)
	{
		m_pTransport->release();
		m_pTransport = nullptr;
	}

	if (m_pPvd)
	{
		m_pPvd->release();
		m_pPvd = nullptr;
	}
#endif

	if (mpFoundation)
	{
		mpFoundation->release();
		mpFoundation = nullptr;
	}

	if (mpControllerManager)
	{
		mpControllerManager->release();
		mpControllerManager = nullptr;
	}
}

void PhysicsManager::AddPhysicsObject(const UID& _uid, physx::PxU32 _groupID /*= 0*/, physx::PxU32 _data /*= 0*/)
{
	//auto view = mRegistry.view<Transform, Rigidbody>();
	entt::entity entity = static_cast<entt::entity>(_uid);
	auto& transform = mRegistry.get<Transform>(entity);
	if (Rigidbody* rigidbody = mRegistry.try_get<Rigidbody>(entity))
	{
		if (rigidbody->mpRigidActor)
		{
			DLOG(LOG_INFO, "Actor already create.");
		}

		if (auto plane = mRegistry.try_get<PlaneComponent>(entity))
		{
			physx::PxPlane pxPlane = physx::PxPlane(plane->mAxes.x, plane->mAxes.y, plane->mAxes.z, plane->mDistance);

			physx::PxMaterial* material = mpPhysics->createMaterial(rigidbody->mStaticFriction, rigidbody->mDynamicFriction, rigidbody->mRestitution);
			material->setFrictionCombineMode(physx::PxCombineMode::eMAX);

			rigidbody->mpRigidActor = PxCreatePlane(*mpPhysics, pxPlane, *material);

			if (rigidbody->mpRigidActor)
			{
				// filterdata 설정
				physx::PxFilterData filterData;
				filterData.word0 = _groupID;
				filterData.word1 = _data;
				physx::PxShape* shape = nullptr;
				auto num = rigidbody->mpRigidActor->getNbShapes();
				rigidbody->mpRigidActor->getShapes(&shape, 1);
				shape->setSimulationFilterData(filterData);

				// name 설정
				rigidbody->mpRigidActor->setName(rigidbody->mName);

				// 씬에 엑터 추가
				mpScene->addActor(*rigidbody->mpRigidActor);

				// 마테리얼 할당 해제
				material->release();
			}
		}
		else
		{
			Vector3 center{};
			if (auto collider = mRegistry.try_get<BoxCollider>(entity))
			{
				center = collider->mCenter;
			}
			else if (auto collider = mRegistry.try_get<SphereCollider>(entity))
			{
				center = collider->mCenter;
			}
			else if (auto collider = mRegistry.try_get<CapsuleCollider>(entity))
			{
				center = collider->mCenter;
			}

			// transform 생성
			Quaternion qut = Quaternion::CreateFromYawPitchRoll(transform.mLocalRotation);
			physx::PxTransform pxTransform(
				physx::PxVec3(
					transform.mLocalPosition.x + center.x,
					transform.mLocalPosition.y + center.y,
					transform.mLocalPosition.z + center.z),
				physx::PxQuat(qut.x, qut.y, qut.z, qut.w)
			);

			// getmetry 생성
			std::shared_ptr<physx::PxGeometry> geometry;
			if (auto collider = mRegistry.try_get<BoxCollider>(entity))
			{
				if (!transform.mpParent)
				{
					geometry = std::make_shared<physx::PxBoxGeometry>(collider->mSize.x / 2, collider->mSize.y / 2, collider->mSize.z / 2);
				}
				else
				{
					geometry = std::make_shared<physx::PxBoxGeometry>(transform.mLocalScale.x / 2, transform.mLocalScale.y / 2, transform.mLocalScale.z / 2);
				}
			}
			else if (auto collider = mRegistry.try_get<SphereCollider>(entity))
			{
				geometry = std::make_shared<physx::PxSphereGeometry>(collider->mRadius);
			}
			else if (auto collider = mRegistry.try_get<CapsuleCollider>(entity))
			{
				geometry = std::make_shared<physx::PxCapsuleGeometry>(collider->mRadius, collider->mHeight / 2);
				pxTransform = physx::PxTransform(pxTransform.p, physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0.f, 0.f, 1.f))); // 90도 회전
			}

			// dynamic actor 생성
			physx::PxMaterial* material = mpPhysics->createMaterial(rigidbody->mStaticFriction, rigidbody->mDynamicFriction, rigidbody->mRestitution);
			material->setFrictionCombineMode(physx::PxCombineMode::eMAX);

			if (!rigidbody->mIsStatic)
			{
				rigidbody->mpRigidActor = PxCreateDynamic(*mpPhysics, pxTransform, *geometry, *material, 1.f);
				physx::PxRigidDynamic* rigidDynamic = rigidbody->mpRigidActor->is<physx::PxRigidDynamic>();
				rigidDynamic->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, rigidbody->mIsKinematic);			// 키네마틱 설정	// 물리 시뮬레이션에서 힘을 받지 않으며, 직접 위치를 제어할 수 있음.
				rigidDynamic->setMass(rigidbody->mMass);																// 질량 설정
				rigidDynamic->setLinearDamping(rigidbody->mDrag);														// 저항 계수 설정
				rigidDynamic->setAngularDamping(rigidbody->mAngularDrag);												// 회전 저항 계수 설정
				rigidDynamic->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !rigidbody->mUseGravity);				// 중력 적용
				rigidDynamic->setSleepThreshold(0.5f);																	// 객체가 휴면 상태로 전환되는 임계값 설정
				rigidDynamic->setMaxLinearVelocity(100.0f);																// 최고 속도 설정
				rigidDynamic->setMaxAngularVelocity(100.0f);															// 최고 각속도 설정

				// 객체의 X축 및 Z축 회전을 고정하여 자빠지지 않게 한다.
				if (mRegistry.try_get<CapsuleCollider>(entity))
				{
					rigidDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, true);
					rigidDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, true);
				}

			}
			else
			{
				rigidbody->mpRigidActor = PxCreateStatic(*mpPhysics, pxTransform, *geometry, *material);
				physx::PxRigidStatic* rigidStatic = rigidbody->mpRigidActor->is<physx::PxRigidStatic>();
			}

			// name 설정
			rigidbody->mpRigidActor->setName(rigidbody->mName);

			// filterdata 설정
			physx::PxFilterData filterData;
			filterData.word0 = _groupID;
			filterData.word1 = _data;
			physx::PxShape* shape = nullptr;
			auto num = rigidbody->mpRigidActor->getNbShapes();
			rigidbody->mpRigidActor->getShapes(&shape, 1);
			shape->setSimulationFilterData(filterData);

			// 씬에 엑터 추가
			mpScene->addActor(*rigidbody->mpRigidActor);

			// 마테리얼 할당 해제
			material->release();
		}
	}
}

void PhysicsManager::ApplyForce(std::shared_ptr<Entity>& _pEntity, const Vector3& _force, int _mode)
{
	if (_pEntity->HasComponent<Rigidbody>())
	{
		auto& rigidbody = _pEntity->GetComponent<Rigidbody>();

		auto actor = rigidbody.mpRigidActor->is<physx::PxRigidDynamic>();
		if (actor)
		{
			// 힘 적용
			actor->addForce(physx::PxVec3(_force.x, _force.y, _force.z), physx::PxForceMode::eFORCE);
			// 			DLog(LOG_INFO, "addforce");
			// 			DLog(LOG_INFO, std::to_string(actor->getLinearVelocity().x)
			// 				+ std::to_string(actor->getLinearVelocity().y)
			// 				+ std::to_string(actor->getLinearVelocity().z));
		}
	}
}

void PhysicsManager::SetVelocity(std::shared_ptr<Entity>& _pEntity, const Vector3& _velocity)
{
	auto& rigidbody = _pEntity->GetComponent<Rigidbody>();
	if (_pEntity->HasComponent<Rigidbody>())
	{
		auto actor = rigidbody.mpRigidActor->is<physx::PxRigidDynamic>();
		if (actor)
		{
			actor->setLinearVelocity(physx::PxVec3(_velocity.x, _velocity.y, _velocity.z));
		}
	}
}

void PhysicsManager::SetKinematic(std::shared_ptr<Entity>& _pEntity, bool _isTrue)
{

}

void PhysicsManager::AddCharacterController(std::shared_ptr<Entity>& _pEntity)
{
	auto& transform = _pEntity->GetComponent<Transform>();
	auto& collider = _pEntity->GetComponent<CapsuleCollider>();

	if (_pEntity->HasComponent<Transform>() && _pEntity->HasComponent<CapsuleCollider>())
	{
		physx::PxCapsuleControllerDesc desc;
		desc.setToDefault();
		desc.height = collider.mHeight;
		desc.radius = collider.mRadius;
		desc.position = physx::PxExtendedVec3(transform.mLocalPosition.x, transform.mLocalPosition.y, transform.mLocalPosition.z);
		desc.material = mpMaterial;
		desc.behaviorCallback = nullptr;
		desc.reportCallback = nullptr;
		DLOG(LOG_INFO, "Creating Capsule Controller");

		switch (collider.mDirection)
		{
		case 0: // X축
			desc.upDirection = physx::PxVec3(1.0f, 0.0f, 0.0f);
			break;
		case 1: // Y축
			desc.upDirection = physx::PxVec3(0.0f, 1.0f, 0.0f);
			break;
		case 2: // Z축
			desc.upDirection = physx::PxVec3(0.0f, 0.0f, 1.0f);
			break;
		default:
			desc.upDirection = physx::PxVec3(0.0f, 1.0f, 0.0f);
			break;
		}

		desc.contactOffset = 0.1f;
		desc.stepOffset = 0.5f;
		desc.slopeLimit = 0.f;
		desc.climbingMode = physx::PxCapsuleClimbingMode::eEASY;
		desc.density = 10.f;

		physx::PxCapsuleController* controller = static_cast<physx::PxCapsuleController*>(mpControllerManager->createController(desc));
		if (!controller)
		{
			DLOG(LOG_ERROR, "Failed to create controller.");
			return;
		}

		_pEntity->GetComponent<CharacterController>().mpController = controller;
		DLOG(LOG_INFO, "Controller created successfully.");
	}
	else
	{
		DLOG(LOG_ERROR, "Entity does not have required components.");
	}
}

void PhysicsManager::MoveCharacterController(std::shared_ptr<Entity>& _pEntity, const Vector3& _displacement, float _dTime)
{
	physx::PxCapsuleController* controller = _pEntity->GetComponent<CharacterController>().mpController;
	controller->move(physx::PxVec3(_displacement.x, _displacement.y, _displacement.z), 0.001f, _dTime, physx::PxControllerFilters());
}

void PhysicsManager::RemovePhysicsObject(std::shared_ptr<Entity>& _pEntity)
{
	auto actor = _pEntity->GetComponent<Rigidbody>().mpRigidActor;
	mpScene->removeActor(*actor);
	actor->release();
	DLOG(LOG_INFO, "Physics object removed: " + _pEntity->GetName());
}

void PhysicsManager::ClearScene()
{
	physx::PxU32 numActors = mpScene->getNbActors(physx::PxActorTypeFlag::eRIGID_DYNAMIC | physx::PxActorTypeFlag::eRIGID_STATIC);
	physx::PxActor** actors = new physx::PxActor * [numActors];
	mpScene->getActors(physx::PxActorTypeFlag::eRIGID_DYNAMIC | physx::PxActorTypeFlag::eRIGID_STATIC, actors, numActors);

	for (physx::PxU32 i = 0; i < numActors; i++)
	{
		mpScene->removeActor(*actors[i]);
		actors[i]->release();
	}

	delete[] actors;
}

void PhysicsManager::RemoveCharacterController(std::shared_ptr<Entity>& _pEntity)
{
	// 	if (m_controllers.find(_pEntity) != m_controllers.end())
	// 	{
	// 		auto controller = m_controllers[_pEntity];
	// 		controller->release();
	// 		m_controllers.erase(_pEntity);
	// 		DLOG(LOG_INFO, "Character controller removed: " + _pEntity->GetName());
	// 	}
}

void PhysicsManager::SetGroupCollisionFlag(uint16_t _gourpID1, uint16_t _gourpID2, bool _enableCollision/* = true*/)
{
	physx::PxSetGroupCollisionFlag(_gourpID1, _gourpID2, _enableCollision);
}

void PhysicsManager::SetKinematicTarget(std::shared_ptr<Entity> _entity, Vector3 _position, Quaternion _rotation)
{
	// 화살의 위치 및 회전 설정
	physx::PxTransform arrowPose(physx::PxVec3(_position.x, _position.y, _position.z), physx::PxQuat(0, 0, 0, 1));

	// 키네마틱 오브젝트의 위치 및 회전 설정
	if (_entity->GetComponent<Rigidbody>().mpRigidActor->is<physx::PxRigidDynamic>())
	{
		physx::PxRigidDynamic* entity = _entity->GetComponent<Rigidbody>().mpRigidActor->is<physx::PxRigidDynamic>();
		entity->setKinematicTarget(arrowPose);
	}

}

void PhysicsManager::SetFixPosition(std::shared_ptr<Entity> _entity, bool _isTrue)
{
	auto& rigidbody = _entity->GetComponent<Rigidbody>();
	if (_entity->HasComponent<Rigidbody>())
	{
		auto actor = rigidbody.mpRigidActor->is<physx::PxRigidDynamic>();
		if (actor)
		{
			if (_isTrue)
			{
				// 현재 위치를 저장하여 고정
				physx::PxTransform currentPose = actor->getGlobalPose();
				actor->setGlobalPose(currentPose); // 객체의 위치를 강제로 고정
				actor->setLinearVelocity(physx::PxVec3(0, 0, 0)); // 속도를 0으로 설정하여 멈추게 함
				actor->setAngularVelocity(physx::PxVec3(0, 0, 0)); // 회전 속도도 0으로 설정
				//pMaterial->setStaticFriction(0.0f);
				//pMaterial->setDynamicFriction(0.0f);
				//pMaterial->setRestitution(0.0f);
			}
			else
			{
				actor->wakeUp();
			}
		}
	}
}

void PhysicsManager::UpdatePosition(std::shared_ptr<Entity> _entity, Vector3 _newPos)
{
	auto& rigidbody = _entity->GetComponent<Rigidbody>();
	if (_entity->HasComponent<Rigidbody>())
	{
		auto actor = rigidbody.mpRigidActor->is<physx::PxRigidDynamic>();
		if (actor)
		{
			Vector3 center;
			if (_entity->HasComponent<BoxCollider>())
			{
				center = _entity->GetComponent<BoxCollider>().mCenter;
			}
			else if (_entity->HasComponent<SphereCollider>())
			{
				center = _entity->GetComponent<SphereCollider>().mCenter;
			}
			else if (_entity->HasComponent<CapsuleCollider>())
			{
				center = _entity->GetComponent<CapsuleCollider>().mCenter;
			}

			// 새로운 위치로 설정
			physx::PxTransform newTransform(physx::PxVec3(_newPos.x + center.x, _newPos.y + center.y, _newPos.z + center.z)
				, actor->getGlobalPose().q);
			actor->setGlobalPose(newTransform);

			// 물리적 속도 및 회전 설정 (필요시)
			actor->setLinearVelocity(physx::PxVec3(0, 0, 0));
			actor->setAngularVelocity(physx::PxVec3(0, 0, 0));
		}
	}
}

Vector3 PhysicsManager::PickObejct()
{
	POINT mousePos;
	GetCursorPos(&mousePos);
	ScreenToClient(mpEngine->mHWnd, &mousePos);

	Vector3 rayNDC;
	rayNDC.x = (2.f * mousePos.x) / mpEngine->mWidth - 1.f;
	rayNDC.y = 1.f - (2.f * mousePos.y) / mpEngine->mHeight;
	rayNDC.z = 1.f;
	Vector4 clipCoords = Vector4(rayNDC.x, rayNDC.y, -1.f, 1.f);

	auto view = mRegistry.view<CameraComponent>();
	for (auto entity : view)
	{
		auto& cameraComp = mRegistry.get<CameraComponent>(entity);
		if (cameraComp.mCameraEnum == 0)
		{
			auto& camera = cameraComp.mpCamera;

			Matrix invProj = camera->GetProjRow().Invert();
			Vector4 eyeCoords = Vector4::Transform(clipCoords, invProj);

			if (cameraComp.mIsPerspective)
			{
				eyeCoords.z = 1.f;
				eyeCoords.w = 0.f;
			}

			Matrix invView = camera->GetViewRow().Invert();
			Vector4 rayWorld = Vector4::Transform(eyeCoords, invView);

			physx::PxVec3 rayDir;
			physx::PxVec3 origin;

			if (cameraComp.mIsPerspective)
			{
				rayDir = physx::PxVec3(rayWorld.x, rayWorld.y, rayWorld.z);
				rayDir.normalize();
				origin = physx::PxVec3(camera->mViewPos.x, camera->mViewPos.y, camera->mViewPos.z);
			}
			else
			{
				origin = physx::PxVec3(rayWorld.x, rayWorld.y, rayWorld.z);
				Vector3 forward = camera->mViewDir;
				//Vector3 forward = Vector3(0, 0, 1);
				rayDir = physx::PxVec3(forward.x, forward.y, forward.z);
			}
			physx::PxRaycastBuffer hit;
			if (mpScene->raycast(origin, rayDir, 100000.f, hit))
			{
				if (hit.hasBlock)
				{
					// 충돌한 객체
					physx::PxRigidActor* hitActor = hit.block.actor;
					// 충돌 지점
					physx::PxVec3 hitPoint = hit.block.position;

					static float eTime = 1;
					eTime += 1.f / 60.f;

// 					if (eTime >= 1)
// 					{
// 						const auto& name = hitActor->getName();
// 						DLOG(LOG_INFO, "Actor Name: " + std::string(name));
// 						//DLOG(LOG_INFO, "view dir: (" + std::to_string(camera->mViewDir.x) + ", " + std::to_string(camera->mViewDir.y) + ", " + std::to_string(camera->mViewDir.z) + ")");
// 						//DLOG(LOG_INFO, "Origin: (" + std::to_string(origin.x) + ", " + std::to_string(origin.y) + ", " + std::to_string(origin.z) + ")");
// 						//DLOG(LOG_INFO, "Direction: (" + std::to_string(rayDir.x) + ", " + std::to_string(rayDir.y) + ", " + std::to_string(rayDir.z) + ")");
// 						DLOG(LOG_INFO, "Hit Position: (" + std::to_string(hitPoint.x) + ", " + std::to_string(hitPoint.y) + ", " + std::to_string(hitPoint.z) + ")");
// 						//DLOG(LOG_INFO, "Hit distance: " + std::to_string(hit.block.distance));
// 						eTime = 0;
// 					}

					return Vector3(hitPoint.x, hitPoint.y, hitPoint.z);
				}
			}

		}
	}
	return Vector3();
}

Vector3 PhysicsManager::PickObejct(const std::string& _name)
{
	POINT mousePos;
	GetCursorPos(&mousePos);
	ScreenToClient(mpEngine->mHWnd, &mousePos);

	Vector3 rayNDC;
	rayNDC.x = (2.f * mousePos.x) / mpEngine->mWidth - 1.f;
	rayNDC.y = 1.f - (2.f * mousePos.y) / mpEngine->mHeight;
	rayNDC.z = 1.f;
	Vector4 clipCoords = Vector4(rayNDC.x, rayNDC.y, -1.f, 1.f);

	auto view = mRegistry.view<CameraComponent>();
	for (auto entity : view)
	{
		auto& cameraComp = mRegistry.get<CameraComponent>(entity);
		if (mRegistry.get<CameraComponent>(entity).mCameraEnum == 0)
		{
			auto& camera = cameraComp.mpCamera;
			Matrix invProj = camera->GetProjRow().Invert();
			Vector4 eyeCoords = Vector4::Transform(clipCoords, invProj);

			if (cameraComp.mIsPerspective)
			{
				eyeCoords.z = 1.f;
				eyeCoords.w = 0.f;
			}

			Matrix invView = camera->GetViewRow().Invert();
			Vector4 rayWorld = Vector4::Transform(eyeCoords, invView);

			physx::PxVec3 rayDir;
			physx::PxVec3 origin;

			if (cameraComp.mIsPerspective)
			{
				rayDir = physx::PxVec3(rayWorld.x, rayWorld.y, rayWorld.z);
				rayDir.normalize();
				origin = physx::PxVec3(camera->mViewPos.x, camera->mViewPos.y, camera->mViewPos.z);
			}
			else
			{
				origin = physx::PxVec3(rayWorld.x, rayWorld.y, rayWorld.z);
				Vector3 forward = camera->mViewDir;
				//Vector3 forward = Vector3(0,0,1);
				rayDir = physx::PxVec3(forward.x, forward.y, forward.z);
			}

			// 다중 충돌을 처리하기 위한 최대 256개의 히트 설정
			const int maxHits = 256;
			physx::PxRaycastHit hitBuffer[maxHits];

			// 히트 정보와 쿼리 데이터를 설정
			physx::PxRaycastBuffer hitResults(hitBuffer, maxHits);

			// 사용자 정의 필터 콜백을 사용해 객체 필터링
			RaycastQueryFilterCallback filterCallback(_name);
			physx::PxQueryFilterData filterData(physx::PxQueryFlag::eSTATIC | physx::PxQueryFlag::eDYNAMIC);
			filterData.flags |= physx::PxQueryFlag::ePREFILTER;

			// 레이캐스트 실행 (필터 콜백 포함)
			if (mpScene->raycast(origin, rayDir, 100000.f, hitResults, physx::PxHitFlag::eDEFAULT, filterData, &filterCallback))
			{
				// 이름이 일치하는 첫 번째 객체의 충돌 지점 반환
				if (hitResults.hasBlock)
				{
					// 충돌한 객체
					physx::PxRigidActor* hitActor = hitResults.block.actor;
					// 충돌 지점
					physx::PxVec3 hitPoint = hitResults.block.position;

					static float eTime = 1;
					eTime += 1.f / 60.f;

// 					if (eTime >= 1)
// 					{
// 						const auto& name = hitActor->getName();
// 						//DLOG(LOG_INFO, "Actor Name: " + std::string(name));
// 						//DLOG(LOG_INFO, "view dir: (" + std::to_string(camera->mViewDir.x) + ", " + std::to_string(camera->mViewDir.y) + ", " + std::to_string(camera->mViewDir.z) + ")");
// 						//DLOG(LOG_INFO, "Origin: (" + std::to_string(origin.x) + ", " + std::to_string(origin.y) + ", " + std::to_string(origin.z) + ")");
// 						//DLOG(LOG_INFO, "Direction: (" + std::to_string(rayDir.x) + ", " + std::to_string(rayDir.y) + ", " + std::to_string(rayDir.z) + ")");
// 						//DLOG(LOG_INFO, "Hit Position: (" + std::to_string(hitPoint.x) + ", " + std::to_string(hitPoint.y) + ", " + std::to_string(hitPoint.z) + ")");
// 						//DLOG(LOG_INFO, "Hit distance: " + std::to_string(hit.block.distance));
// 						eTime = 0;
// 					}

					return Vector3(hitPoint.x, hitPoint.y, hitPoint.z);
				}
			}
		}
	}

	// 찾지 못한 경우 기본 값 반환
	return Vector3();
}

physx::PxFilterData PhysicsManager::GetFilterData(std::shared_ptr<Entity> _pEntity)
{
	physx::PxShape* shape[1];
	_pEntity->GetComponent<Rigidbody>().mpRigidActor->getShapes(shape, 1);
	return shape[0]->getSimulationFilterData();
}

physx::PxFilterData PhysicsManager::GetFilterData(const entt::entity& _entity)
{
	physx::PxShape* shape[1];
	mRegistry.get<Rigidbody>(_entity).mpRigidActor->getShapes(shape, 1);
	return shape[0]->getSimulationFilterData();
}

void PhysicsManager::SetFilterData(std::shared_ptr<Entity>& _pEntity, const physx::PxFilterData& newFilterData)
{
	physx::PxShape* shape[1];
	_pEntity->GetComponent<Rigidbody>().mpRigidActor->getShapes(shape, 1);
	mFilterDataUpdates.push(std::make_pair(shape[0], newFilterData));
}

void PhysicsManager::SetFilterData(physx::PxShape* _pShape, const physx::PxFilterData& newFilterData)
{
	mFilterDataUpdates.push(std::make_pair(_pShape, newFilterData));
}

void PhysicsManager::SetFilterData(const entt::entity& _entity, const physx::PxFilterData& newFilterData)
{
	physx::PxShape* shape[1];
	mRegistry.get<Rigidbody>(_entity).mpRigidActor->getShapes(shape, 1);
	mFilterDataUpdates.push(std::make_pair(shape[0], newFilterData));
}

std::shared_ptr<Entity> PhysicsManager::GetEntityFromActor(physx::PxRigidActor* actor)
{
	auto view = mRegistry.view<Rigidbody>();

	for (auto entity : view)
	{
		auto rigidbody = mRegistry.try_get<Rigidbody>(entity);
		if (rigidbody->mpRigidActor == actor)
		{
			return rigidbody->mpOwner;
		}
	}

	return nullptr;  // 해당하는 엔티티를 찾지 못한 경우
}

void PhysicsManager::SetObjectSleep(std::shared_ptr<Entity> _pEntity)
{
	// RigidDynamic 오브젝트를 sleep 상태로 전환
	physx::PxRigidDynamic* dynamic = _pEntity->GetComponent<Rigidbody>().mpRigidActor->is<physx::PxRigidDynamic>();
	if (dynamic)
	{
		dynamic->putToSleep();
	}
}

void PhysicsManager::SetObjectWake(std::shared_ptr<Entity> _pEntity)
{
	// RigidDynamic 오브젝트를 wake 상태로 전환
	physx::PxRigidDynamic* dynamic = _pEntity->GetComponent<Rigidbody>().mpRigidActor->is<physx::PxRigidDynamic>();
	if (dynamic)
	{
		dynamic->wakeUp();
	}
}

void PhysicsManager::SetALLObjectSleep()
{
	// 모든 RigidDynamic 오브젝트를 sleep 상태로 전환
	auto view = mRegistry.view<Rigidbody>();
	for (auto entity : view)
	{
		auto rigidbody = mRegistry.try_get<Rigidbody>(entity);
		if (rigidbody && rigidbody->mpRigidActor && !rigidbody->mIsKinematic)
		{
			physx::PxRigidDynamic* dynamicActor = rigidbody->mpRigidActor->is<physx::PxRigidDynamic>();
			if (dynamicActor)
			{
				dynamicActor->putToSleep();  // Sleep 상태로 전환
			}
		}
	}
}

void PhysicsManager::SetALLObjectWake()
{
	// 모든 RigidDynamic 오브젝트를 wake 상태로 전환
	auto view = mRegistry.view<Rigidbody>();
	for (auto entity : view)
	{
		auto rigidbody = mRegistry.try_get<Rigidbody>(entity);
		if (rigidbody && rigidbody->mpRigidActor && !rigidbody->mIsKinematic)
		{
			physx::PxRigidDynamic* dynamicActor = rigidbody->mpRigidActor->is<physx::PxRigidDynamic>();
			if (dynamicActor)
			{
				dynamicActor->wakeUp();  // Wake 상태로 전환
			}
		}
	}
}

void PhysicsManager::ClearFilterUpdate()
{
	while (!mFilterDataUpdates.empty())
	{
		mFilterDataUpdates.pop();
	}
}
