#pragma once
#include "TestScene.h"
#include "EventManager.h"
#include "RenderManager.h"
#include "PhysicsManager.h"
#include "UIManager.h"
#include "EntityManager.h"
#include "ResourceManager.h"
#include "MessageBox3D.h"
#include "MessageBox2D.h"

TestScene::TestScene(entt::registry& _registry, const std::string& _name
	, EventManager* _pEventManager, RenderManager* _pRenderManager
	, PhysicsManager* _pPhysicsManager, InputManager* _pInpueManager
	, WorldManager* _pWorldManager, UIManager* _pUIManager
	, EntityManager* _pEntityManager, ResourceManager* _pResourceManager
	, SoundManager* _pSoundManager)
	: Scene(_registry, _name, _pEventManager, _pRenderManager, _pPhysicsManager
		, _pInpueManager, _pWorldManager, _pUIManager, _pEntityManager, _pResourceManager
		, _pSoundManager)
{
}

bool TestScene::Initialize()
{
	REGISTER_IMMEDIATE_EVENT("Character Move Forward", OnCharacterMove);
	REGISTER_IMMEDIATE_EVENT("Character Move Right", OnCharacterMove);
	REGISTER_IMMEDIATE_EVENT("Character Move Left", OnCharacterMove);
	REGISTER_IMMEDIATE_EVENT("Character Move Back", OnCharacterMove);
	REGISTER_IMMEDIATE_EVENT("Character Jump", OnCharacterMove);

	AddResource();
	m_pRenderManager->SetCubeMap("Atrium_diffuseIBL.dds", "Atrium_specularIBL.dds");
	// 	m_pPhysicsManager->SetGroupCollisionFlag(1, 1, false);

	// 카메라 엔티티 생성, 세팅
	cameraEntity = m_pEntityManager->CreateEntity("Camera");
	cameraEntity->AddComponent<CameraComponent>(m_pRenderManager->GetScreenWidth(), m_pRenderManager->GetScreenHeight()
		, 10.f, Vector3(0.f, 5.f, -50.f), Vector3(0.f, 0.f, 1.f), Vector3(0.f, 1.f, 0.f), Vector3(70.f, 0.01f, 10000.f),
		static_cast<unsigned int>(cameraEnum::WorldCamera));
	// 	AddEntity(cameraEntity);

	/// 객체 생성
	// 플레이어 엔티티 추가
	entity1 = m_pEntityManager->CreateEntity("Player");
	entity1->AddComponent<Transform>();
	entity1->AddComponent<CapsuleCollider>();
	entity1->AddComponent<Rigidbody>();
	// 	auto& rigid1 = entity1->GetComponent<Rigidbody>();
	// 	rigid1.m_useGravity = false;
	// 	rigid1.m_mass = 50.f;
	entity1->AddComponent<MeshRenderer>();
	entity1->AddComponent<Texture3D>();
	entity1->AddComponent<AnimationComponent>();
	entity1->AddComponent<BoneGroupComponent>();
	auto& bone = entity1->GetComponent<BoneGroupComponent>();
	auto& trs1 = entity1->GetComponent<Transform>();
	auto& collider1 = entity1->GetComponent<CapsuleCollider>();
	trs1.m_localPosition = Vector3(-20.0f, 0.f, 10.f);
	trs1.m_localRotation = Vector3(0.0f, 90.f, 0.f);
	trs1.m_localScale = Vector3(0.2f, 0.15f, 0.2f);
	//collider1.m_size = Vector3(5.f, 5.f, 5.f);
	collider1.m_radius = 2;
	collider1.m_height = 3;
	entity1->GetComponent<MeshRenderer>().m_file = "Character 01.FBX";
	entity1->GetComponent<MeshRenderer>().m_meshName = "Character 01";
	entity1->GetComponent<MeshRenderer>().m_hasBone = true;
	entity1->GetComponent<Texture3D>().m_diffuse = "M-Cos-16-Blue.png";
	entity1->GetComponent<AnimationComponent>().mpTargetAnimation = m_pResourceManager->Get_Animation("Character@SlashAttack.fbx");
	entity1->GetComponent<AnimationComponent>().mIsLoop = true;
	//  	m_pPhysicsManager->AddPhysicsObject(entity1->GetUID(), 1);
	m_pUIManager->Add3DHPBar(entity1, Vector3(0, 30, 0), Vector2(100, 10), 1, Vector4(1.0f, 0.0f, 0.0f, 1.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), 0.6f);
	auto& equipment = entity1->AddComponent<EquipmentComponent>();
	equipment.AddEquipment("Hat 04.FBX", "Hat 04", "Character 01", "RigHead", "M-Cos-16-Blue.png");
	//entity1->AddComponent<OutlineComponent>();

	entity1->AddComponent<FlowTextureComponent>(0, 0.1f);

	entity1_2 = m_pEntityManager->CreateEntity("Axe");
	entity1_2->AddComponent<Transform>();
	entity1_2->AddComponent<BoxCollider>(true, Vector3(), Vector3(10));
	entity1_2->AddComponent<Rigidbody>(1.f, 0.f, 0.f, false, true, 0.f, 0.f, 0.f, false);
	// 	entity1_2->AddComponent<MeshRenderer>();
	entity1_2->AddComponent<Texture3D>();
	// 	entity1_2->AddComponent<TargetBoneComponent>();
	auto& trs1_2 = entity1_2->GetComponent<Transform>();
	trs1_2.m_pParent = &trs1;
	// 	entity1_2->GetComponent<MeshRenderer>().m_file = "Axe 01.FBX";
	// 	entity1_2->GetComponent<MeshRenderer>().m_meshName = "Axe 01";
	// 	entity1_2->GetComponent<MeshRenderer>().mIsWeapon = true;
		// 	auto& targetBoneComponent1 = entity1_2->GetComponent<TargetBoneComponent>();
		// 	targetBoneComponent1.mTargetBoneAboveMatrix = m_pRenderManager->GetTargetBoneAboveMatrix("Character 01.FBX", "RigRPalm", 1.f);
	entity1_2->GetComponent<Texture3D>().m_diffuse = "M-Cos-16-Blue.png";
	entity1->AddComponent<WeaponComponent>(/*"Character 01.FBX", "RigRPalm", 1.f*/).m_pAttachedEntity = entity1_2.get();
	entity1->GetComponent<WeaponComponent>().m_boneIndex = m_pRenderManager->Get_TargetModelBoneIndex("Character 01", "RigRPalm");
	entity1->GetComponent<WeaponComponent>().m_weaponAABB = m_pRenderManager->Get_AABB("Axe 01.FBX");
	entity1->GetComponent<WeaponComponent>().m_fileName = "Axe 01.FBX";
	entity1->GetComponent<WeaponComponent>().m_meshName = "Axe 01";

	entity1_2->GetComponent<BoxCollider>().m_size = entity1->GetComponent<WeaponComponent>().m_weaponAABB.mMax
		- entity1->GetComponent<WeaponComponent>().m_weaponAABB.mMin;
	m_pPhysicsManager->AddPhysicsObject(entity1_2->GetUID(), 1);

	// 	/// 장비 생성2
	// 	// 플레이어 엔티티 추가
	// 	auto player1_1 = m_pEntityManager->CreateEntity("Hat");
	// 	player1_1->AddComponent<Transform>();
	// 	// 	player1_1->AddComponent<BoxCollider>();
	// 	// 	player1_1->AddComponent<Rigidbody>();
	// 	player1_1->AddComponent<MeshRenderer>();
	// 	player1_1->AddComponent<Texture3D>();
	// 	player1_1->AddComponent<TargetBoneComponent>();
	// 	auto& player_trs1_1 = player1_1->GetComponent<Transform>();
	// 	player_trs1_1.m_pParent = &player_trs1;
	// 	player1_1->GetComponent<MeshRenderer>().m_file = "Hat 04.FBX";
	// 	player1_1->GetComponent<MeshRenderer>().m_meshName = "Hat 04";
	// 	auto& player_targetBoneComponent = player1_1->GetComponent<TargetBoneComponent>();
	// 	player_targetBoneComponent.mTargetBoneIndex = m_pRenderManager->Get_TargetModelBoneIndex("Character 01", "RigHead");
	// 	player_targetBoneComponent.mTarGetBoneConstantBufferData = &(player1->GetComponent<BoneGroupComponent>().m_boneData);
	// 	player_targetBoneComponent.mTargetBoneData.targrtBoneMatrix = player_targetBoneComponent.mTarGetBoneConstantBufferData->bone[player_targetBoneComponent.mTargetBoneIndex];
	// 
	// 
	// 	/// 무기 생성2
	// 	// 플레이어 엔티티 추가
	// 	player1_2 = m_pEntityManager->CreateEntity("Axe");
	// 	player1_2->AddComponent<Transform>();
	// 	player1_2->AddComponent<BoxCollider>(true, Vector3(), Vector3(10));
	// 	// 	player1_2->AddComponent<Rigidbody>();
	// 	player1_2->AddComponent<MeshRenderer>();
	// 	player1_2->AddComponent<Texture3D>();
	// 	player1_2->AddComponent<TargetBoneComponent>();
	// 	auto& player_trs1_2 = player1_2->GetComponent<Transform>();
	// 	player_trs1_2.m_pParent = &player_trs1;
	// 	player1_2->GetComponent<MeshRenderer>().m_file = "Axe 01.FBX";
	// 	player1_2->GetComponent<MeshRenderer>().m_meshName = "Axe 01";
	// 	auto& player1targetBoneComponent1 = player1_2->GetComponent<TargetBoneComponent>();
	// 	player1targetBoneComponent1.mTargetBoneAboveMatrix = m_pRenderManager->GetTargetBoneAboveMatrix("Character 01.FBX", "RigRPalm", 1.f);
	// 	player1_2->GetComponent<Texture3D>().m_diffuse = "M-Cos-16-Blue.png";
	// 	player1->AddComponent<WeaponComponent>("Character 01.FBX", "RigRPalm", 1.f).m_pAttachedEntity = player1_2.get();
	// 	player1->GetComponent<WeaponComponent>().m_boneIndex = m_pRenderManager->Get_TargetModelBoneIndex("Character 01", "RigRPalm");
	// 	player1->GetComponent<WeaponComponent>().m_weaponAABB = m_pRenderManager->Get_AABB("Axe 01.FBX");
	// 	player1_2->GetComponent<BoxCollider>().m_size = player1->GetComponent<WeaponComponent>().m_weaponAABB.mMax
	// 		- player1->GetComponent<WeaponComponent>().m_weaponAABB.mMin;

		// 발판 엔티티 추가
// 	entity2 = m_pEntityManager->CreateEntity("Cube1");
// 	entity2->AddComponent<Transform>();
// 	entity2->AddComponent<BoxCollider>();
// 	entity2->AddComponent<Rigidbody>();
// 	entity2->AddComponent<MeshRenderer>();
// 	entity2->AddComponent<Texture2D>();
// 	auto& trs2 = entity2->GetComponent<Transform>();
// 	auto& collider2 = entity2->GetComponent<BoxCollider>();
// 	auto& rigid2 = entity2->GetComponent<Rigidbody>();
// 	auto& mesh2 = entity2->GetComponent<MeshRenderer>();
// 	trs2.m_localPosition = Vector3(0.f, -3.f, 0.f);
// 	trs2.m_localScale = Vector3(100.5f, 1.f, 100.5f);
// 	collider2.m_size = Vector3(100, 1, 100);
// 	rigid2.m_mass = 100.f;
// 	rigid2.m_isKinematic = true;
// 	entity2->GetComponent<MeshRenderer>().m_file = "box.fbx";
// 	entity2->GetComponent<MeshRenderer>().m_meshName = "Cube.001";
// 	entity2->GetComponent<Texture2D>().m_file = "blue.png";
// 	m_pPhysicsManager->AddPhysicsObject(entity2->GetUID(), 0);

	// 장애물 엔티티 추가
// 	entity3 = m_pEntityManager->CreateEntity("Cube2");
// 	entity3->AddComponent<Transform>();
// 	entity3->AddComponent<BoxCollider>();
// 	entity3->AddComponent<Rigidbody>();
// 	entity3->AddComponent<MeshRenderer>();
// 	entity3->AddComponent<Texture2D>();
// 	auto& trs3 = entity3->GetComponent<Transform>();
// 	auto& collider3 = entity3->GetComponent<BoxCollider>();
// 	auto& mesh3 = entity3->GetComponent<MeshRenderer>();
// 	auto& rigid3 = entity3->GetComponent<Rigidbody>();
// 	trs3.m_localPosition = Vector3(10.f, 5.f, 0.f);
// 	trs3.m_localScale = Vector3(5.f, 5.f, 5.f);
// 	collider3.m_size = Vector3(1, 1, 1);
// 	mesh3.m_file = "Cube";
// 	rigid3.m_useGravity = true;
// 	rigid3.m_mass = 50.f;
// 	entity3->GetComponent<MeshRenderer>().m_file = "box.fbx";
// 	entity3->GetComponent<MeshRenderer>().m_meshName = "Cube.001";
// 	entity3->GetComponent<Texture2D>().m_file = "namu.png";
// 	m_pPhysicsManager->AddPhysicsObject(entity3->GetUID(), 1);
	//AddEntity(entity3);


	// 전역적으로 쓸 라이트 엔티티 생성, 세팅
	// 일단 모든 라이트들의 데이터를 하나의 엔티티가 가지도록 설정, 추후에 그래픽스 파트와 상의 후 고칠예정
	auto lightEntity = m_pEntityManager->CreateEntity("LightGroup");
	lightEntity->AddComponent<LightComponent>();

	// UI 엔티티 생성
	auto uiEntity = m_pEntityManager->CreateEntity("UI_button");
	//m_pUIManager->AddText(uiEntity, u8"Text Test", "L.ttf", Vector2(10, 436));
	m_pUIManager->AddUI(uiEntity, "namu.png", Vector2(0, 500), Vector2(809 / 2, 493 / 2), 0, Vector4(0.8f, 0.8f, 0.8f, 1.0f));
	m_pUIManager->AddButtonAllColor(uiEntity, Vector4(0.5f, 0.5f, 0.5f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	m_pUIManager->AddMessageBox2D(uiEntity, "blue.png", Vector2(105, 380), Vector2(220, 100), Vector4(1.0f), u8"품종 : 보더 콜리\n특징 : 활발함", Vector2(135, 405), 0.2f, "M.ttf", Vector4(0.0f, 0.0f, 0.0f, 1.0f), 0, false);
	//m_pUIManager->AddButtonHoveredUI(uiEntity, "coco.jpg");
	//m_pUIManager->AddButtonHoveredText(uiEntity, "B.ttf", u8"Hovering", Vector2(200, 0));

	//auto uiNumText = m_pEntityManager->CreateEntity("UI2"); //숫자를 넣은 텍스트 테스트

	//m_pUIManager->AddTextwithFloat2(uiNumText, u8"마우스 x좌표: %.2f \n마우스 y좌표: %.2f", "L.ttf", Vector2(0, 120), m_pInputManager->GetMousePos().x, m_pInputManager->GetMousePos().y);

	auto uiEntity21 = m_pEntityManager->CreateEntity("UIMessage");
	m_pUIManager->AddMessageBox3D(uiEntity21, "blue.png", Vector2(0, 0), Vector2(250, 125), Vector4(1.0f), u8"지각하면 5000원이야.", Vector2(10, 50), 0.2f, "M.ttf", Vector4(1.0f), trs1.m_localPosition, Vector2(0, -35));

	auto uiText = m_pEntityManager->CreateEntity("UI");
	m_pUIManager->AddText(uiText, u8"asdf", "M.ttf", Vector2(100, 100), 1, true);

	//m_pUIManager->AddButtonPressUI(uiEntity, "joco.jpg");
	//m_pUIManager->AddButtonPressText(uiEntity, "B.ttf", u8"Click!", Vector2(400, 0));
	m_pUIManager->SetMaxNumLayer(2);

	auto uiBasic = m_pEntityManager->CreateEntity("UI"); // 기본 도형 테스트
	//m_pUIManager->AddRect(uiBasic, Vector2(100, 100), Vector2(100, 100), 0, Vector4(1.0f, 1.0f, 0.0f, 1.0f), 10.0f);

	/// 라이트 세팅
	m_pRenderManager->LightInitialize(&lightEntity->GetComponent<LightComponent>().m_commonConstData, 3);

	m_pRenderManager->SetDirLight(&lightEntity->GetComponent<LightComponent>().m_commonConstData, 0, 1.0f, Vector3(1.0f, -1.0f, 1.0f));
	// m_pRenderManager->m_pGraphicsEngine->SetPointLight(&lightEntity->GetComponent<LightComponent>().m_constData, 1, 1.0f, 1.0f, 10.0f, Vector3(0.0f, 10.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));
	// m_pRenderManager->m_pGraphicsEngine->SetSpotLight(&lightEntity->GetComponent<LightComponent>().m_constData, 2, 1.0f, 10.0f, 50.0f, Vector3(0.0f, -20.0f, 1.0f), Vector3(0.0f, 0.0f, -4.0f), 1.0f, Vector3(1.0f, 0.0f, 0.0f));
	m_pRenderManager->PrintLightInfo(&lightEntity->GetComponent<LightComponent>().m_commonConstData);

	///첫번째 라이트의 짝꿍카메라---------------------------------------------------------------------------------
	Light templight = lightEntity->GetComponent<LightComponent>().m_commonConstData.light[0];
	templight.position = Vector3(-100.f, 100.0f, -100.f);
	std::shared_ptr<Entity> tempEntity = m_pEntityManager->CreateEntity("Camera");
	tempEntity->AddComponent<CameraComponent>(m_pRenderManager->GetScreenWidth(), m_pRenderManager->GetScreenHeight()
		, 0.f, Vector3(-100.f, 100.0f, -100.f), Vector3(1.f, -1.f, 1.f), Vector3(0.f, 1.f, 0.f), Vector3(70.f, 100.f, 1000.f),
		static_cast<unsigned int>(cameraEnum::LightCamera));
	tempEntity->GetComponent<CameraComponent>().setLightCameraIndex(0);	//0번째 라이트기 때문
	tempEntity->GetComponent<CameraComponent>().setViewDir(templight.direction);	//0번째 라이트기 때문
	tempEntity->GetComponent<CameraComponent>().setEyePos(templight.position);	//0번째 라이트기 때문
	//-------------위에서 생성한 라이트 만큼 카메라를 만들어 주는 것이 좋다. -> shadow를 위해서 ----------------------

	std::shared_ptr<Entity> tempCubeMap = m_pEntityManager->CreateEntity("CubeMap");
	tempCubeMap->AddComponent<Transform>();
	// 	tempCubeMap->AddComponent<MeshRenderer>();
	// 	tempCubeMap->AddComponent<Texture3D>();
	// 	auto& tempTrs = tempCubeMap->GetComponent<Transform>();
	// 	auto& tempMesh = tempCubeMap->GetComponent<MeshRenderer>();
	// 	tempTrs.m_localPosition = Vector3(0.f, 0.f, 0.f);
	// 	tempTrs.m_localScale = Vector3(1, 1, 1);
	// 	tempCubeMap->GetComponent<MeshRenderer>().m_file = "box.fbx";
	// 	tempCubeMap->GetComponent<MeshRenderer>().m_meshName = "CubeMap";
	// 	tempCubeMap->GetComponent<MeshRenderer>().mIs_CubeMap = true;
	// 	tempCubeMap->GetComponent<Texture3D>().m_diffuse = "skybox_diffuse.dds";
	// 	tempCubeMap->GetComponent<Texture3D>().m_specular = "skybox_specular.dds";
		//m_pPhysicsManager->AddPhysicsObject(tempCubeMap->GetUID(), 0);
	tempCubeMap->AddComponent<CubeMapComponent>("skybox_diffuse.dds", "skybox_specular.dds");


	auto outline = m_pEntityManager->CreateEntity("outline");
	outline->AddComponent<Transform>(Vector3(1), Vector3(), Vector3(17));
	outline->AddComponent<MeshRenderer>("", "MySquare");
	outline->AddComponent<Texture3D>("White.png");
	outline->AddComponent<OutlineComponent>();
	outline->AddComponent<AlphaBlendComponent>(1);
	auto outline2 = m_pEntityManager->CreateEntity("outline");
	outline2->AddComponent<Transform>(Vector3(), Vector3(), Vector3(17));
	outline2->AddComponent<MeshRenderer>("", "MySquare");
	outline2->AddComponent<Texture3D>("White.png");
	outline2->AddComponent<OutlineComponent>();
	outline2->AddComponent<AlphaBlendComponent>(0.5);

	auto outline3 = m_pEntityManager->CreateEntity("outline");
	outline3->AddComponent<Transform>(Vector3(2), Vector3(), Vector3(17));
	outline3->AddComponent<MeshRenderer>("", "MySquare");
	outline3->AddComponent<Texture3D>("White.png");
	outline3->AddComponent<OutlineComponent>(1.1, Vector3(1, 0, 0));
	outline3->AddComponent<AlphaBlendComponent>(0);


	auto particle = m_pEntityManager->CreateEntity("particle");
	//particle->AddComponent<ParticleComponent>(Vector3(0, 1, 0), 0, Vector3(0.8f), 4.f, Vector3(), 0, Vector3(), 1, Vector3(1.f), 0
	//	, Vector3(), 0, Vector3(0.5f), Vector3(), Vector3(), Vector3(), Vector3(), 0, Vector3());
// 	particle->AddComponent<ParticleComponent>();
// 	particle->GetComponent<ParticleComponent>().m_pParticleData->position = Vector3(5, 10, 0);

	// 	entity2 = m_pEntityManager->CreateEntity("CubeMap");
	// 	entity2->AddComponent<Transform>();
	// 	entity2->AddComponent<BoxCollider>();
	// 	entity2->AddComponent<Rigidbody>();
	// 	entity2->AddComponent<MeshRenderer>();
	// 	entity2->AddComponent<Texture2D>();
	// 	auto& trs2 = entity2->GetComponent<Transform>();
	// 	auto& collider2 = entity2->GetComponent<BoxCollider>();
	// 	auto& rigid2 = entity2->GetComponent<Rigidbody>();
	// 	auto& mesh2 = entity2->GetComponent<MeshRenderer>();
	// 	trs2.m_localPosition = Vector3(0.f, -3.f, 0.f);
	// 	trs2.m_localScale = Vector3(100.f, 100.f, 100.f);
	// 	collider2.m_size = Vector3(100, 100, 100);
	// 	rigid2.m_mass = 100.f;
	// 	rigid2.m_isKinematic = true;
	// 	entity2->GetComponent<MeshRenderer>().m_file = "box.fbx";
	// 	entity2->GetComponent<MeshRenderer>().m_meshName = "CubeMap";
	// 	entity2->GetComponent<Texture2D>().m_file = "blue.png";
	// 	m_pPhysicsManager->AddPhysicsObject(entity2->GetUID(), 0);

	m_pSoundManager->PlayBGM("Snd_bgm_Title");

	/// 이 씬의 렌더링 오브젝트 초기화
	return m_pRenderManager->InitializeScene();
}

void TestScene::Update(float _dTime)
{
	for (auto& [uid, entity] : m_pEntityManager->GetEntityMap())
	{
		if (m_pInputManager->GetKeyDown(KEY::LBUTTON))
		{
			// UI 테스트용, 너무 길고, 테스트용이라서 줄바꿈은 코딩 컨벤션을 지키지 않았습니다.
			if (entity->GetName() == "UIMessage")
			{
				auto& msgCompText = entity->GetComponent<MessageBox3D>().mText;

				if (msgCompText == u8"지각하면 5000원이야.")
				{
					msgCompText = u8"얼른 오지 못할까!";
				}
			}
		}

		if (entity->GetName() == "UI_button")
		{
			if (entity->GetComponent<Button>().mButtonState == ButtonState::HOVERED)
			{
				entity->GetComponent<MessageBox2D>().mIsImgVisible = true;
				entity->GetComponent<MessageBox2D>().mIsTextVisible = true;
			}
			else
			{
				entity->GetComponent<MessageBox2D>().mIsImgVisible = false;
				entity->GetComponent<MessageBox2D>().mIsTextVisible = false;
			}
		}
	}

	if (m_pInputManager->GetKeyDown(KEY::INS))
	{
		static bool sleep = false;
		if (sleep)
		{
			sleep = false;
			m_pPhysicsManager->SetALLObjectWake();
		}
		else
		{
			sleep = true;
			m_pPhysicsManager->SetALLObjectSleep();
		}
	}

	if (m_pInputManager->GetKey(KEY::W))
	{

		//m_pEventManager->TriggerEvent(Event("Character Move Forward", KEY::W));
	}
	if (m_pInputManager->GetKey(KEY::A))
	{
		//m_pEventManager->TriggerEvent(Event("Character Move Left", KEY::A));
	}
	if (m_pInputManager->GetKey(KEY::S))
	{
		//m_pEventManager->TriggerEvent(Event("Character Move Back", KEY::S));
	}
	if (m_pInputManager->GetKey(KEY::D))
	{
		//m_pEventManager->TriggerEvent(Event("Character Move Right", KEY::D));
	}

	if (m_pInputManager->GetKey(KEY::C))
	{
		entity1->GetComponent<MeshRenderer>().m_pModel->mAnimationPlaytime = 0;
		entity2->GetComponent<MeshRenderer>().m_pModel->mAnimationPlaytime = 0;
		entity3->GetComponent<MeshRenderer>().m_pModel->mAnimationPlaytime = 0;
	}
	if (m_pInputManager->GetKeyState(KEY::V) == KEY_STATE::TAP)
	{
		entity1->GetComponent<AnimationComponent>().mpNextTargetAnimation = m_pResourceManager->Get_Animation("Character@SlashAttack.fbx");
		entity1->GetComponent<AnimationComponent>().SetNextTargetAnimation(m_pResourceManager->Get_Animation("Character@SlashAttack.fbx"));
		//player1->GetComponent<AnimationComponent>().mpNextTargetAnimation = m_pResourceManager->Get_Animation("Character@Move.fbx");
	}

	if (m_pInputManager->GetKeyState(KEY::B) == KEY_STATE::TAP)
	{
		//entity1->GetComponent<MeshRenderer>().m_pModel->mAnimationPlaytime = 0;
		//player1->GetComponent<MeshRenderer>().m_pModel->mAnimationPlaytime = 0;
		entity1->GetComponent<AnimationComponent>().mpNextTargetAnimation = m_pResourceManager->Get_Animation("Character@Move.fbx");
		//player1->GetComponent<AnimationComponent>().mpNextTargetAnimation = m_pResourceManager->Get_Animation("Character@SlashAttack.fbx");
	}

	// 	if (m_pInputManager->GetKeyUp(KEY::W) || m_pInputManager->GetKeyUp(KEY::A)
	// 		|| m_pInputManager->GetKeyUp(KEY::S) || m_pInputManager->GetKeyUp(KEY::D))
	// 	{
	// 		m_pPhysicsManager->SetVelocity(entity1, Vector3(0.f, 0.f, 0.f));
	// 	}

		// 	if (!(m_pInputManager->GetKeyState(KEY::W) == KEY_STATE::HOLD)
		// 		&& !(m_pInputManager->GetKeyState(KEY::A) == KEY_STATE::HOLD)
		// 		&& !(m_pInputManager->GetKeyState(KEY::S) == KEY_STATE::HOLD)
		// 		&& !(m_pInputManager->GetKeyState(KEY::D) == KEY_STATE::HOLD))
		// 	{
		// 		entity1->GetComponent<MeshRenderer>().m_pModel->mpTargetAnimation
		// 			= m_pRenderManager->m_pGraphicsEngine->Get_Animation("Character@Run Forward Bare Hands In Place.FBX");
		// 	}
		// 
		// 	if (m_pInputManager->GetKeyState(KEY::SPACE) == KEY_STATE::HOLD)
		// 	{
		// 		entity1->GetComponent<MeshRenderer>().m_pModel->mpTargetAnimation
		// 			= m_pRenderManager->m_pGraphicsEngine->Get_Animation("Character@Run Forward Bare Hands In Place.FBX");
		// 		m_pPhysicsManager->ApplyForce(entity1, Vector3(0, 100, 0));
		// 	}


		// 	auto& targetBoneComponent1 = entity1_2->GetComponent<TargetBoneComponent>();
		// 	targetBoneComponent1.mTargetBoneAboveMatrix = m_pRenderManager->m_pGraphicsEngine->GetTargetBoneAboveMatrix("Character 01.FBX", "RigRPalm", 1.f);
		// 	
		// 	auto& targetBoneComponent2 = player1_2->GetComponent<TargetBoneComponent>();
		// 	targetBoneComponent2.mTargetBoneAboveMatrix = m_pRenderManager->m_pGraphicsEngine->GetTargetBoneAboveMatrix("Character 01.FBX", "RigRPalm", 1.f);

	if (m_pInputManager->GetKeyUp(KEY::L))
	{
		entity1->GetComponent<Transform>().m_localPosition += Vector3(5, 0, 0);
	}

	static float alpha = 0;
	alpha += _dTime;
	if (alpha >= 1)
	{
		alpha = 0;
	}
	m_pRenderManager->SetOpacityFactor(Vector4(alpha));

	//if (m_pInputManager->GetKeyUp(KEY::F9))
	//{
	//	if (alpha < 1)
	//	{
	//		alpha = 1;
	//		m_pRenderManager->SetOpacityFactor(Vector4(alpha));
	//	}
	//	else
	//	{
	//		alpha = 0;
	//		m_pRenderManager->SetOpacityFactor(Vector4(alpha));
	//	}
	//}

	static int mode = 2;
	if (m_pInputManager->GetKeyState(KEY::Q) == KEY_STATE::TAP)
	{
		mode++;
	}

	switch (mode)
	{
	case 0:
		m_pRenderManager->SetRenderMode(true, false, true);
		break;
	case 1:
		m_pRenderManager->SetRenderMode(false, true, false);
		break;
	case 2:
		m_pRenderManager->SetRenderMode(true, true, true);
		break;
	case 3:
		m_pRenderManager->SetRenderMode(false, false, false);
		break;
	default:
		mode = 0;
		break;
	}
	testTimer += 0.3f;

	auto partView = m_registry.view<ParticleComponent>();
	for (auto& entity : partView)
	{
		auto& particle = m_registry.get<ParticleComponent>(entity);
		m_pRenderManager->AddParticle(100, *particle.m_pParticleData);
	}
}

void TestScene::LateUpdate(float _dTime)
{
	auto& camera = cameraEntity->GetComponent<CameraComponent>().m_pCamera;

	for (auto& [uid, entity] : m_pEntityManager->GetEntityMap())
	{
		// UI 테스트용, 너무 길고, 테스트용이라서 줄바꿈은 코딩 컨벤션을 지키지 않았습니다.
		if (entity->GetName() == "UI")
		{
			// 			if (m_pInputManager->GetKeyState(KEY::D0) == KEY_STATE::TAP)
			// 			{
			// 				auto& text = entity->GetComponent<Text>();
			// 				if (text.m_rgba == Vector4(0.0f, 0.0f, 0.0f, 1.0f)) {
			// 					text.m_rgba = Vector4(0.0f, 1.0f, 0.0f, 0.5f);
			// 				}
			// 				else { text.m_rgba = Vector4(0.0f, 0.0f, 0.0f, 1.0f); }
			// 				if (text.m_layer == 0) { text.m_layer = 2; }
			// 				else { text.m_layer = 0; }
			// 			}
			// 			if (m_pInputManager->GetKeyState(KEY::D1) == KEY_STATE::TAP)
			// 			{
			// 				auto& text = entity->GetComponent<Text>();
			// 				if (text.m_isVisible == true) {
			// 					text.m_isVisible = false;
			// 				}
			// 				else { text.m_isVisible = true; }
			// 			}

						/*if (m_pUIManager->GetButtonState(entity) == ButtonState::HOVERED)
						{
							entity->GetComponent<Text>().m_rgba = Vector4(1.0f, 0.0f, 0.0f, 0.8f);
						}
						else
						{
							entity->GetComponent<Text>().m_rgba = Vector4(0.0f, 0.0f, 0.0f, 1.8f);
						}*/
		}

		if (entity->GetName() == "UI2")
		{
			auto& text = entity->GetComponent<Text>();
			text.m_num3 = m_pInputManager->GetMousePos().x;
			text.m_num4 = m_pInputManager->GetMousePos().y;

		}

		// 애니메이션 테스트용, 너무 길고, 테스트용이라서 줄바꿈은 코딩 컨벤션을 지키지 않았습니다.
		if (entity->GetName() == "Player")
		{
			if (m_pInputManager->GetKeyState(KEY::L) == KEY_STATE::TAP)
			{
				auto& loop = entity->GetComponent<AnimationComponent>().mIsLoop;
				if (loop == true) { loop = false; }
				else { loop = true; }
			}

			if (m_pInputManager->GetKeyState(KEY::K) == KEY_STATE::TAP)
			{
				auto& loop = entity->GetComponent<AnimationComponent>().mPaused;
				if (loop == true) { loop = false; }
				else { loop = true; }
			}

			if (m_pInputManager->GetKeyState(KEY::J) == KEY_STATE::TAP)
			{
				auto& loop = entity->GetComponent<AnimationComponent>().mPlaySpeed;
				if (loop == 1.0f) { loop = 0.1f; }
				else { loop = 1.0f; }
			}

			// 			if (m_pInputManager->GetKeyState(KEY::H) == KEY_STATE::TAP)
			// 			{
			// 				auto& isLoop = entity->GetComponent<AnimationComponent>().mIsLoop;
			// 				if (isLoop) { isLoop = false; }
			// 				else { isLoop = true; }
			// 			}

						// 텍스처 변경 테스트용입니다. 너무 길고, 테스트용이라서 줄바꿈은 코딩 컨벤션을 지키지 않았습니다.
			if (testTimer > 50)
			{
				entity->GetComponent<Texture3D>().m_diffuse = "F-Cos-01-Red.png";
				if (testTimer > 100) testTimer = 0.0f;
			}
			else { entity->GetComponent<Texture3D>().m_diffuse = "M-Cos-16-Blue.png"; }
		}
	}

	/// 카메라의 뷰 변경 키는 R입니다.
	if (m_pInputManager->GetKeyState(KEY::R) == KEY_STATE::TAP)
	{
		if (camera->mIsFirstPersonMode)
		{
			camera->mIsFirstPersonMode = false;
		}
		else
		{
			camera->mIsFirstPersonMode = true;
		}
	}

	if (m_pInputManager->GetKeyState(KEY::UP) == KEY_STATE::HOLD)
	{
		camera->MoveForward(_dTime * 10);
	}
	if (m_pInputManager->GetKeyState(KEY::DOWN) == KEY_STATE::HOLD)
	{
		camera->MoveForward(-_dTime * 10);
	}
	if (m_pInputManager->GetKeyState(KEY::LEFT) == KEY_STATE::HOLD)
	{
		camera->MoveRight(-_dTime * 10);
	}
	if (m_pInputManager->GetKeyState(KEY::RIGHT) == KEY_STATE::HOLD)
	{
		camera->MoveRight(_dTime * 10);
	}
	if (m_pInputManager->GetKeyState(KEY::N) == KEY_STATE::HOLD)
	{
		camera->MoveUp(_dTime * 10);
	}
	if (m_pInputManager->GetKeyState(KEY::M) == KEY_STATE::HOLD)
	{
		camera->MoveUp(-_dTime * 10);
	}

	Vector2 dMousePos = m_pInputManager->GetMousePos();
	// DLog(LOG_INFO, std::to_string(dMousePos.x) + std::to_string(dMousePos.y));
	camera->OnMouseMove(static_cast<int>(dMousePos.x), static_cast<int>(dMousePos.y));
	//camera->PrintCamInfo();



	m_pPhysicsManager->PickObejct();
}

void TestScene::Finalize()
{
	// 물리 객체 할당 해제
	/*m_pPhysicsManager->RemovePhysicsObject(entity1);
	m_pPhysicsManager->RemovePhysicsObject(entity2);
	m_pPhysicsManager->RemovePhysicsObject(entity3);*/

	// 카메라 메모리 할당 해제
	auto cameraView = m_registry.view<CameraComponent>();
	for (auto entity : cameraView)
	{
		auto camera = m_registry.try_get<CameraComponent>(entity);
		delete camera->m_pCamera;
	}

	m_registry.clear();

	/// 리소스 할당 해제
	// 모델
// 	m_pRenderManager->m_pGraphicsEngine->Erase_ModelInfo("box.fbx");
// 	m_pRenderManager->m_pGraphicsEngine->Erase_ModelInfo("BigBox.fbx");
// 	m_pRenderManager->m_pGraphicsEngine->Erase_ModelInfo("Character 01.FBX");
// 
// 	// 애니메이션
// 	m_pRenderManager->m_pGraphicsEngine->Erase_Animation("Character@Die.FBX");
// 	m_pRenderManager->m_pGraphicsEngine->Erase_Animation("Character@Run Forward Bare Hands In Place.FBX");
// 	m_pRenderManager->m_pGraphicsEngine->Erase_Animation("Character@Slash Attack.FBX");
// 
// 	// 3D 텍스쳐
// 	m_pRenderManager->m_pGraphicsEngine->Erase_Textures("M-Cos-16-Blue.png.png");
// 	//m_pRenderManager->m_pGraphicsEngine->Erase_Textures("Ch03_1001_Normal.png");
// 
// 	// 2D 텍스쳐
// 	m_pRenderManager->m_pGraphicsEngine->Erase_Textures("bird.png");
// 	m_pRenderManager->m_pGraphicsEngine->Erase_Textures("blue.png");
// 	m_pRenderManager->m_pGraphicsEngine->Erase_Textures("coco.jpg");
// 	m_pRenderManager->m_pGraphicsEngine->Erase_Textures("namu.png");

	UNREGISTER_EVENT("Character Move Forward");
	UNREGISTER_EVENT("Character Move Right");
	UNREGISTER_EVENT("Character Move Left");
	UNREGISTER_EVENT("Character Move Back");
	UNREGISTER_EVENT("Character Jump");

}

void TestScene::AddResource()
{
	/// 리소스 추가
// 모델 추가
	m_pRenderManager->AddModel("../TestAsset/", "box.fbx");
	m_pRenderManager->AddModel("../TestAsset/", "BigBox.fbx");
	m_pRenderManager->AddModel("../TestAsset/Test/", "Character 01.FBX");
	m_pRenderManager->AddModel("../TestAsset/Test/", "Axe 01.FBX");
	m_pRenderManager->AddModel("../TestAsset/Test/", "Hat 04.FBX");

	m_pUIManager->AddTexture2D("../../Resources/Texture/Texture2D/", "White.png");
	m_pRenderManager->Add3DTexture("../../Resources/Texture/Texture3D/", "White.png");

	// 애니메이션 추가
	m_pResourceManager->AddFilesInDirAni("Animation");

	// 3D 텍스쳐 추가
	m_pRenderManager->Add3DTexture("../TestAsset/Test/", "M-Cos-16-Blue.png");
	m_pRenderManager->Add3DTexture("../TestAsset/Test/", "F-Cos-01-Red.png");

	//큐브맵 텍스쳐를 추가한다.
	m_pRenderManager->AddDDSTexture("../../Resources/Texture/CubeMap/", "skybox_diffuse.dds");
	m_pRenderManager->AddDDSTexture("../../Resources/Texture/CubeMap/", "skybox_specular.dds");
	//m_pRenderManager->m_pGraphicsEngine->Add3DTexture("../TestAsset/", "Ch03_1001_Normal.png");

	// 2D 텍스쳐 추가
	m_pUIManager->AddTexture2D("../../Resources/Dummy/", "bird.png");
	m_pUIManager->AddTexture2D("../../Resources/Dummy/", "blue.png");
	m_pUIManager->AddTexture2D("../../Resources/Dummy/", "coco.jpg");
	m_pUIManager->AddTexture2D("../../Resources/Dummy/", "joco.jpg");
	m_pUIManager->AddTexture2D("../../Resources/Dummy/", "namu.png");

	// 폰트 추가
	// m_pUIManager->AddFont("../../Resources/Font/", "B.ttf", 40.f, true);
	// m_pUIManager->AddFont("../../Resources/Font/", "L.ttf", 120.f, true);
	m_pUIManager->AddFont("../../Resources/Font/", "M.ttf", 120.f, true);
	m_pUIManager->FontSetFinish();

	// 사운드 추가
	m_pResourceManager->AddFilesInDirBGMSound("BGM");
	m_pResourceManager->AddFilesInDirBGMSound("SFX");
	m_pSoundManager->LoadBGM("../../Resources/Sound/Snd_bgm_Title.wav", "Snd_bgm_Title");

	// 커서 추가
	m_pInputManager->AddMouseCursor("../../Resources/Texture/Texture2D/ICON_X.ico");
	m_pInputManager->SetMouseCursor(0);
}

void TestScene::OnCharacterMove(const Event& _event)
{
	auto key = _event.GetDataAs<KEY>();

	Vector3 force(0.f, 0.f, 0.f);
	Vector3 velocity(0.f, 0.f, 0.f);
	Vector3 rotation(0.f, 0.f, 0.f);

	if (key == KEY::W) {
		force = Vector3(0.f, 0.f, 50.f);
		velocity = Vector3(0.f, 0.f, 10.f);
		rotation = Vector3(0.f, 2 * std::numbers::phi, 0.f);
		DLOG(LOG_DEBUG, "Move Forward");
	}
	else if (key == KEY::A) {
		force = Vector3(-50.f, 0.f, 0.f);
		velocity = Vector3(-10.f, 0.f, 0.f);
		rotation = Vector3(0.f, std::numbers::phi, 0.f);
		DLOG(LOG_DEBUG, "Move Left");
	}
	else if (key == KEY::S) {
		force = Vector3(0.f, 0.f, -50.f);
		velocity = Vector3(0.f, 0.f, -10.f);
		rotation = Vector3(0.f, 0.f, 0.f);
		DLOG(LOG_DEBUG, "Move Back");
	}
	else if (key == KEY::D) {
		force = Vector3(50.f, 0.f, 0.f);
		velocity = Vector3(10.f, 0.f, 0.f);
		rotation = Vector3(0.f, -std::numbers::phi, 0.f);
		DLOG(LOG_DEBUG, "Move Right");
	}

	//m_pPhysicsManager->ApplyForce(entity1, force);
	m_pPhysicsManager->SetVelocity(entity1, velocity);
	entity1->GetComponent<MeshRenderer>().m_pModel->mpTargetAnimation = m_pResourceManager->Get_Animation("Character@Move.fbx");
	entity1->GetComponent<Transform>().m_localRotation = rotation;
}

