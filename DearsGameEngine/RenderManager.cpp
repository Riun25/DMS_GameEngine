#pragma once
#include "RenderManager.h"
#include "Transform.h"
#include "AnimationComponent.h"
#include "CameraComponent.h"
#include "MeshRenderer.h"
#include "BoneGroupComponent.h"
#include "TargetBoneComponent.h"
#include "LightComponent.h"
#include "CameraComponent.h"
#include "Texture2D.h"
#include "Texture3D.h"
#include "Text.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "Graphics/DearsGraphicsEngine.h"
#include "WorldManager.h"
#include "TimeManager.h"
#include "WeaponComponent.h"
#include "EquipmentComponent.h"
#include "Name.h"
#include "OutlineComponent.h"
#include "CubeMapComponent.h"
#include "AlphaBlendComponent.h"
#include "FlowTextureComponent.h"

RenderManager::RenderManager(entt::registry& _registry, std::shared_ptr<DearsGraphicsEngine> _pGraphicsEngine
	, WorldManager* _pWorldManager, TimeManager* _pTimeManager)
	: mRegistry(_registry)
	, m_pGraphicsEngine(_pGraphicsEngine)
	, mpWorldManager(_pWorldManager)
	, mpTimeManager(_pTimeManager)
	, mRenderModel(true)
	, mRenderGeometry(false)
	, mpWorldCamera(nullptr)
	, mpLightCamera{ nullptr, nullptr, nullptr }
{
	mpAnimationManager = nullptr;
	mpLightObserver = std::make_unique<entt::observer>(mRegistry, entt::collector.group<LightComponent>().update<LightComponent>());
	mpMeshObserver = std::make_unique<entt::observer>(mRegistry, entt::collector.group<MeshRenderer>().update<MeshRenderer>());
	mpTexture3DObserver = std::make_unique<entt::observer>(mRegistry, entt::collector.group<Texture3D>().update<Texture3D>());
	mpTexture2DObserver = std::make_unique<entt::observer>(mRegistry, entt::collector.group<Texture2D>().update<Texture2D>());
}

RenderManager::~RenderManager()
{
	delete mpAnimationManager;
}

bool RenderManager::Initialize()
{
	mpAnimationManager = new AnimationManager(mRegistry, m_pGraphicsEngine);
	return true;
}

bool RenderManager::InitializeScene()
{
	// #sung 현재 씬의 카메라 초기화, 설정 // 현재는 씬당 하나의 월드카메라만 설정하고있음
	//또한 라이트 카메라를 세팅해준다 -> 그림자를 위해
	auto cameraView = mRegistry.view<CameraComponent>();
	for (auto entity : cameraView)
	{
		auto& cameraComponent = mRegistry.get<CameraComponent>(entity);
		if (cameraComponent.mCameraEnum == static_cast<unsigned int>(cameraEnum::WorldCamera))		//월드카메라일 경우 -> 월드카메라는 하나만 존재하여야한다.
		{
			mpWorldCamera = cameraComponent.mpCamera;
			m_pGraphicsEngine->SetCamera(mpWorldCamera);
		}
		if (cameraComponent.mCameraEnum == static_cast<unsigned int>(cameraEnum::LightCamera))		//라이트 카메라일 경우 ->라이트카메라 역시 최대 MaxLight수 만큼만 존재하여야한다. 
		{
			mpLightCamera[cameraComponent.mLightIndex] = cameraComponent.mpCamera;
		}
	}

	if (!mpWorldCamera)
	{
		DLOG(LOG_ERROR, "There are No WorldCamera in the Scene.");
		return false;
	}

	// 현재 씬의 라이트 초기화, 설정														   
	auto lightView = mRegistry.view<LightComponent>();										   //현재 이 구조가 결코 좋은 구조는 아니다
	PSConstantBufferData lightData;														   //만약 라이트컴포넌트를 가진 객체가 여러개일 경우 하나만 전체에 적용되기 때문이다.
	for (auto entity : lightView)															   //디퍼드 렌더링을 만들면서 구조를 수정하는게 좋아보인다.
	{
		auto& light = mRegistry.get<LightComponent>(entity);
		m_pGraphicsEngine->UpdateCommonConstantBuffer(light.mCommonConstData);
	}

	// 모델 버퍼 설정
	auto modelView = mRegistry.view<Transform, MeshRenderer>();
	for (auto entity : modelView)
	{
		auto& transform = mRegistry.get<Transform>(entity);	//특정 모델의 Transform*
		auto& model = mRegistry.get<MeshRenderer>(entity);	//특정 모델의 MeshRenderer*
		auto& modelBuffer = model.mpModel;					//특정 MeshRenderer의 modelbuffer
		//auto& name = mRegistry.get<Name>(entity).mName;
		auto& modelConst = model.mVSConstantBufferData;
		auto bone = mRegistry.try_get<BoneGroupComponent>(entity);
		auto targetBone = mRegistry.try_get<TargetBoneComponent>(entity);
		auto flowTexture = mRegistry.try_get<FlowTextureComponent>(entity);

		///#sung 모델의 월드변환과 역행렬을 업데이트해준다.
		modelConst.world = transform.GetTransformMatrix();
		modelConst.invWorld = modelConst.world.Invert();

		// 모델 버텍스, 인텍스 버퍼 바인딩
		modelBuffer->m_pVertexBuffer = m_pGraphicsEngine->Get_VertexBuffer(model.mMeshName);
		modelBuffer->m_pIndexBuffer = m_pGraphicsEngine->Get_IndexBuffer(model.mMeshName);

		// 모델 상수 버퍼 생성 및 바인딩
		modelBuffer->m_pVSConstantBuffer = m_pGraphicsEngine->CreateConstantBuffer(modelConst);
		model.mIs_VSconstant = true;
		modelBuffer->m_pPSConstantBuffer = m_pGraphicsEngine->CreateConstantBuffer(lightData);
		model.mIs_PSconstant = true;
		modelBuffer->mNumIndices = m_pGraphicsEngine->Get_NumIndex(model.mMeshName);
		if (!model.mFile.empty())
		{
			modelBuffer->mpTargetModel = m_pGraphicsEngine->Get_ModelInfo(model.mFile);
		}

		// 현재 엔티티에 BoneGroupComponent가 있는가? 없을 경우 nullptr반환
		if (bone)
		{
			//버퍼 생성 및 바인딩
			modelBuffer->m_BoneConstantBuffer = m_pGraphicsEngine->CreateConstantBuffer(bone->mBoneData);
			model.mIs_VSBoneConstant = true;
		}

		// 3D 텍스쳐 설정
		if (auto texture3d = mRegistry.try_get<Texture3D>(entity))
		{
			if (!(texture3d->mDiffuse == ""))
			{
				modelBuffer->m_diffusetexture = m_pGraphicsEngine->Get_Textures(texture3d->mDiffuse);
			}
			if (!(texture3d->mNormal == ""))
			{
				modelBuffer->m_normaltexture = m_pGraphicsEngine->Get_Textures(texture3d->mNormal);
			}
		}

		if (flowTexture)
		{
			modelBuffer->m_pVSWaterConstantBuffer = m_pGraphicsEngine->CreateConstantBuffer(flowTexture->mVSWCSD);
			flowTexture->mVSWCSD.time = flowTexture->mTime;
			flowTexture->mVSWCSD.speed = flowTexture->mSpeed;
		}

		// 애니메이션 설정
		if (auto animation = mRegistry.try_get<AnimationComponent>(entity))
		{
			mpAnimationManager->SetInitialAnimation(animation->mpOwner);
			//modelBuffer->mpTargetAnimation = m_pGraphicsEngine->Get_Animation(animation->mTargetAnimation);
		}

		// 무기 설정
		if (auto weapon = mRegistry.try_get<WeaponComponent>(entity))
		{
			// 무기의 월드변환과 역행렬을 업데이트해준다
			auto& weaponModel = weapon->mpModel;
			auto& weaponConst = weapon->mWeaponConstantBufferData;
			weaponConst.world = transform.GetTransformMatrix();
			weaponConst.invWorld = transform.GetTransformMatrix().Invert();

			// 무기 버텍스, 인텍스 버퍼 바인딩
			weaponModel->m_pVertexBuffer = m_pGraphicsEngine->Get_VertexBuffer(weapon->mMeshName);
			weaponModel->m_pIndexBuffer = m_pGraphicsEngine->Get_IndexBuffer(weapon->mMeshName);

			// 무기 상수 버퍼 생성 및 바인딩
			weaponModel->m_pVSConstantBuffer = m_pGraphicsEngine->CreateConstantBuffer(weaponConst);
			weaponModel->m_pPSConstantBuffer = m_pGraphicsEngine->CreateConstantBuffer(lightData);
			weaponModel->mNumIndices = m_pGraphicsEngine->Get_NumIndex(weapon->mMeshName);
			weaponModel->mpTargetModel = m_pGraphicsEngine->Get_ModelInfo(weapon->mFileName);

			auto& weaponEntity = weapon->mpAttachedEntity;

			weaponEntity->GetComponent<Transform>().mLocalPosition = transform.mLocalPosition;
			weaponEntity->GetComponent<Transform>().mLocalRotation = transform.mLocalRotation;
			weaponEntity->GetComponent<Transform>().mLocalScale = transform.mLocalScale;

			if (weaponEntity->HasComponent<Texture3D>())
			{
				auto& weaponTexture = weaponEntity->GetComponent<Texture3D>();
				if (!(weaponTexture.mDiffuse == ""))
				{
					weaponModel->m_diffusetexture = m_pGraphicsEngine->Get_Textures(weaponTexture.mDiffuse);
				}
				if (!(weaponTexture.mNormal == ""))
				{
					weaponModel->m_normaltexture = m_pGraphicsEngine->Get_Textures(weaponTexture.mNormal);
				}
			}

			weaponConst.world = model.mVSConstantBufferData.world * m_pGraphicsEngine->GetTargetBoneAboveMatrix(model.mFile, weapon->mBoneIndex, weapon->mScale).Transpose();
			weaponConst.invWorld = weaponConst.world.Invert();

			weapon->mWeaponBoneMatrix = weaponConst.world.Transpose();

			Matrix matrix = Matrix::CreateScale(weapon->mpAttachedEntity->GetComponent<BoxCollider>().mSize)
				* Matrix::CreateTranslation(weapon->mpAttachedEntity->GetComponent<BoxCollider>().mCenter)
				* weapon->mWeaponBoneMatrix;

			auto& weaponTrs = weapon->mpAttachedEntity->GetComponent<Transform>();

			// 1.Translation 저장
			weaponTrs.mLocalPosition = Vector3(matrix._41, matrix._42, matrix._43);
			Vector3 scale;
			// 2. Scale 추출 
			scale.x = Vector3(matrix._11, matrix._12, matrix._13).Length();
			scale.y = Vector3(matrix._21, matrix._22, matrix._23).Length();
			scale.z = Vector3(matrix._31, matrix._32, matrix._33).Length();

			weaponTrs.mLocalScale = scale;
			// 3. Rotation 추출
			Matrix rotationMatrix = matrix;

			if (scale.x != 0) {
				rotationMatrix._11 /= scale.x;
				rotationMatrix._12 /= scale.x;
				rotationMatrix._13 /= scale.x;
			}
			if (scale.y != 0) {
				rotationMatrix._21 /= scale.y;
				rotationMatrix._22 /= scale.y;
				rotationMatrix._23 /= scale.y;
			}
			if (scale.z != 0) {
				rotationMatrix._31 /= scale.z;
				rotationMatrix._32 /= scale.z;
				rotationMatrix._33 /= scale.z;
			}

			// 4. Rotation 추출, 저장
			weaponTrs.mLocalRotation.x = atan2(rotationMatrix._32, rotationMatrix._33);  // Pitch (X축 회전)
			weaponTrs.mLocalRotation.y = atan2(-rotationMatrix._31, sqrt(rotationMatrix._32 * rotationMatrix._32 + rotationMatrix._33 * rotationMatrix._33));  // Yaw (Y축 회전)
			weaponTrs.mLocalRotation.z = atan2(rotationMatrix._21, rotationMatrix._11);  // Roll (Z축 회전)

			weaponTrs.mLocalMatrix = matrix;
			//parent.m_weaponBoneMatrix = matrix;

		}

		// 장비 설정
		if (auto equips = mRegistry.try_get<EquipmentComponent>(entity))
		{
			for (auto& equip : equips->mpEquipments)
			{
				if (!equip->mMeshName.empty())
				{
					auto& buffer = equip->mpModelBuffer;
					// 장비 모델 버텍스, 인텍스 버퍼 바인딩
					buffer->m_pVertexBuffer = m_pGraphicsEngine->Get_VertexBuffer(equip->mMeshName);
					buffer->m_pIndexBuffer = m_pGraphicsEngine->Get_IndexBuffer(equip->mMeshName);

					// 장비의 모델 상수 버퍼 생성, 바인딩
					buffer->m_pVSConstantBuffer = m_pGraphicsEngine->CreateConstantBuffer(modelConst);
					buffer->m_pPSConstantBuffer = m_pGraphicsEngine->CreateConstantBuffer(lightData);
					buffer->mNumIndices = m_pGraphicsEngine->Get_NumIndex(equip->mMeshName);
					buffer->mpTargetModel = m_pGraphicsEngine->Get_ModelInfo(equip->mModelFileName);

					equip->mTargetBoneIndex = m_pGraphicsEngine->Get_TargetModelBoneIndex(equip->mTargetModelName, equip->mBoneName);

					//버퍼 생성 및 바인딩
					buffer->m_TargetBoneConstantBuffer = m_pGraphicsEngine->CreateConstantBuffer(equip->mTargetBoneConstantBufferData);

					// 장비의 텍스쳐 설정
					if (!(equip->mDiffuseTexture == ""))
					{
						buffer->m_diffusetexture = m_pGraphicsEngine->Get_Textures(equip->mDiffuseTexture);
					}
					if (!(equip->mNormalTexture == ""))
					{
						buffer->m_normaltexture = m_pGraphicsEngine->Get_Textures(equip->mNormalTexture);
					}

				}
			}
		}

		// 외곽선 설정
		if (auto outline = mRegistry.try_get<OutlineComponent>(entity))
		{
			modelBuffer->m_pVSEdgeConstantBuffer = m_pGraphicsEngine->CreateConstantBuffer(modelConst);
			outline->mVSEdgeCBD.EdgeScaleMatrix = Matrix::CreateScale(outline->mThickness);
			outline->mPSEdgeCBD.color = outline->mColor;
			modelBuffer->m_pPSEdgeConstantBuffer = m_pGraphicsEngine->CreateConstantBuffer(outline->mPSEdgeCBD);
		}
	}
	SetCubeMap("skybox_diffuse.dds", "skybox_specular.dds");
	// 큐브맵 설정
	auto cubemapView = mRegistry.view<CubeMapComponent>();
	for (auto& entity : cubemapView)
	{
		auto& cubemap = mRegistry.get<CubeMapComponent>(entity);
		auto& cubeModel = cubemap.mpModel;
		cubeModel->m_pVSConstantBuffer = m_pGraphicsEngine->CreateConstantBuffer(cubemap.mVSCBD);
		cubeModel->m_pPSConstantBuffer = m_pGraphicsEngine->CreateConstantBuffer(lightData);
		cubeModel->m_pVertexBuffer = m_pGraphicsEngine->Get_VertexBuffer("CubeMap");
		cubeModel->m_pIndexBuffer = m_pGraphicsEngine->Get_IndexBuffer("CubeMap");
		cubeModel->mNumIndices = m_pGraphicsEngine->Get_NumIndex("CubeMap");
		SetCubeMap(cubemap.mDiffuse, cubemap.mSpecular);
	}

	return true;
}

void RenderManager::SetCubeMap(std::string diffuseTextureName, std::string specularTextureName)
{
	m_pGraphicsEngine->Set_CubeMap(diffuseTextureName, specularTextureName);
}

bool RenderManager::InitailizeEntity(std::shared_ptr<Entity> _entity)
{
	PSConstantBufferData lightData;																//머테리얼때문에 필요하다.

	if (_entity->HasComponent<Transform>() && _entity->HasComponent<MeshRenderer>())
	{
		auto& transform = _entity->GetComponent<Transform>();	//특정 모델의 Transform*
		auto& model = _entity->GetComponent<MeshRenderer>();	//특정 모델의 MeshRenderer*
		auto& modelBuffer = model.mpModel;					//특정 MeshRenderer의 modelbuffer


		auto& modelConst = model.mVSConstantBufferData;

		///모델의 월드변환과 역행렬을 업데이트해준다.
		modelConst.world = transform.GetTransformMatrix();
		modelConst.invWorld = transform.GetTransformMatrix().Invert();


		// 모델 버텍스, 인텍스 버퍼 바인딩
		modelBuffer->m_pVertexBuffer = m_pGraphicsEngine->Get_VertexBuffer(model.mMeshName);
		modelBuffer->m_pIndexBuffer = m_pGraphicsEngine->Get_IndexBuffer(model.mMeshName);

		// 모델 상수 버퍼 생성 및 바인딩
		modelBuffer->m_pVSConstantBuffer = m_pGraphicsEngine->CreateConstantBuffer(modelConst);
		model.mIs_VSconstant = true;
		modelBuffer->m_pPSConstantBuffer = m_pGraphicsEngine->CreateConstantBuffer(lightData);
		model.mIs_PSconstant = true;
		modelBuffer->mNumIndices = m_pGraphicsEngine->Get_NumIndex(model.mMeshName);
		modelBuffer->mpTargetModel = m_pGraphicsEngine->Get_ModelInfo(model.mFile);

		// 현재 엔티티에 BoneGroupComponent가 있는가? 없을 경우 nullptr반환
		if (_entity->HasComponent<BoneGroupComponent>())
		{
			//버퍼 생성 및 바인딩
			modelBuffer->m_BoneConstantBuffer = m_pGraphicsEngine->CreateConstantBuffer(_entity->GetComponent<BoneGroupComponent>().mBoneData);
			model.mIs_VSBoneConstant = true;
		}

		// 3D 텍스쳐 설정
		if (_entity->HasComponent<Texture3D>())
		{
			if (!(_entity->GetComponent<Texture3D>().mDiffuse == ""))
			{
				modelBuffer->m_diffusetexture = m_pGraphicsEngine->Get_Textures(_entity->GetComponent<Texture3D>().mDiffuse);
			}
			if (!(_entity->GetComponent<Texture3D>().mNormal == ""))
			{
				modelBuffer->m_normaltexture = m_pGraphicsEngine->Get_Textures(_entity->GetComponent<Texture3D>().mNormal);
			}
		}

		// 애니메이션 설정
		if (_entity->HasComponent<AnimationComponent>())
		{
			mpAnimationManager->SetInitialAnimation(_entity);
		}

		// 무기 설정
		if (_entity->HasComponent<WeaponComponent>())
		{
			auto& weapon = _entity->GetComponent<WeaponComponent>();
			// 무기의 월드변환과 역행렬을 업데이트해준다
			auto& weaponModel = weapon.mpModel;
			auto& weaponConst = weapon.mWeaponConstantBufferData;
			weaponConst.world = transform.GetTransformMatrix();
			weaponConst.invWorld = transform.GetTransformMatrix().Invert();

			// 무기 버텍스, 인텍스 버퍼 바인딩
			weaponModel->m_pVertexBuffer = m_pGraphicsEngine->Get_VertexBuffer(weapon.mMeshName);
			weaponModel->m_pIndexBuffer = m_pGraphicsEngine->Get_IndexBuffer(weapon.mMeshName);

			// 무기 상수 버퍼 생성 및 바인딩
			weaponModel->m_pVSConstantBuffer = m_pGraphicsEngine->CreateConstantBuffer(weaponConst);
			weaponModel->m_pPSConstantBuffer = m_pGraphicsEngine->CreateConstantBuffer(lightData);
			weaponModel->mNumIndices = m_pGraphicsEngine->Get_NumIndex(weapon.mMeshName);
			weaponModel->mpTargetModel = m_pGraphicsEngine->Get_ModelInfo(weapon.mFileName);

			auto& weaponEntity = weapon.mpAttachedEntity;

			weaponEntity->GetComponent<Transform>().mLocalPosition = transform.mLocalPosition;
			weaponEntity->GetComponent<Transform>().mLocalRotation = transform.mLocalRotation;
			weaponEntity->GetComponent<Transform>().mLocalScale = transform.mLocalScale;

			if (weaponEntity->HasComponent<Texture3D>())
			{
				auto& weaponTexture = weaponEntity->GetComponent<Texture3D>();
				if (!(weaponTexture.mDiffuse == ""))
				{
					weaponModel->m_diffusetexture = m_pGraphicsEngine->Get_Textures(weaponTexture.mDiffuse);
				}
				if (!(weaponTexture.mNormal == ""))
				{
					weaponModel->m_normaltexture = m_pGraphicsEngine->Get_Textures(weaponTexture.mNormal);
				}
			}

			weaponConst.world = model.mVSConstantBufferData.world * m_pGraphicsEngine->GetTargetBoneAboveMatrix(model.mFile, weapon.mBoneIndex, weapon.mScale).Transpose();
			weaponConst.invWorld = weaponConst.world.Invert();

			weapon.mWeaponBoneMatrix = weaponConst.world.Transpose();

			Matrix matrix = Matrix::CreateScale(weapon.mpAttachedEntity->GetComponent<BoxCollider>().mSize)
				* Matrix::CreateTranslation(weapon.mpAttachedEntity->GetComponent<BoxCollider>().mCenter)
				* weapon.mWeaponBoneMatrix;

			auto& weaponTrs = weapon.mpAttachedEntity->GetComponent<Transform>();

			// 1.Translation 저장
			weaponTrs.mLocalPosition = Vector3(matrix._41, matrix._42, matrix._43);
			Vector3 scale;
			// 2. Scale 추출 
			scale.x = Vector3(matrix._11, matrix._12, matrix._13).Length();
			scale.y = Vector3(matrix._21, matrix._22, matrix._23).Length();
			scale.z = Vector3(matrix._31, matrix._32, matrix._33).Length();

			weaponTrs.mLocalScale = scale;
			// 3. Rotation 추출
			Matrix rotationMatrix = matrix;

			if (scale.x != 0) {
				rotationMatrix._11 /= scale.x;
				rotationMatrix._12 /= scale.x;
				rotationMatrix._13 /= scale.x;
			}
			if (scale.y != 0) {
				rotationMatrix._21 /= scale.y;
				rotationMatrix._22 /= scale.y;
				rotationMatrix._23 /= scale.y;
			}
			if (scale.z != 0) {
				rotationMatrix._31 /= scale.z;
				rotationMatrix._32 /= scale.z;
				rotationMatrix._33 /= scale.z;
			}

			// 4. Rotation 추출, 저장
			weaponTrs.mLocalRotation.x = atan2(rotationMatrix._32, rotationMatrix._33);  // Pitch (X축 회전)
			weaponTrs.mLocalRotation.y = atan2(-rotationMatrix._31, sqrt(rotationMatrix._32 * rotationMatrix._32 + rotationMatrix._33 * rotationMatrix._33));  // Yaw (Y축 회전)
			weaponTrs.mLocalRotation.z = atan2(rotationMatrix._21, rotationMatrix._11);  // Roll (Z축 회전)

			weaponTrs.mLocalMatrix = matrix;
			//parent.m_weaponBoneMatrix = matrix;


		}
		// 장비 설정
		if (_entity->HasComponent<EquipmentComponent>())
		{
			auto& equips = _entity->GetComponent<EquipmentComponent>();
			for (auto& equip : equips.mpEquipments)
			{
				if (!equip->mMeshName.empty())
				{
					auto& buffer = equip->mpModelBuffer;
					// 장비 모델 버텍스, 인텍스 버퍼 바인딩
					buffer->m_pVertexBuffer = m_pGraphicsEngine->Get_VertexBuffer(equip->mMeshName);
					buffer->m_pIndexBuffer = m_pGraphicsEngine->Get_IndexBuffer(equip->mMeshName);

					// 장비의 모델 상수 버퍼 생성, 바인딩
					buffer->m_pVSConstantBuffer = m_pGraphicsEngine->CreateConstantBuffer(modelConst);
					buffer->m_pPSConstantBuffer = m_pGraphicsEngine->CreateConstantBuffer(lightData);
					buffer->mNumIndices = m_pGraphicsEngine->Get_NumIndex(equip->mMeshName);
					buffer->mpTargetModel = m_pGraphicsEngine->Get_ModelInfo(equip->mModelFileName);

					equip->mTargetBoneIndex = m_pGraphicsEngine->Get_TargetModelBoneIndex(equip->mTargetModelName, equip->mBoneName);

					//버퍼 생성 및 바인딩
					buffer->m_TargetBoneConstantBuffer = m_pGraphicsEngine->CreateConstantBuffer(equip->mTargetBoneConstantBufferData);

					// 장비의 텍스쳐 설정
					if (!(equip->mDiffuseTexture.empty()))
					{
						buffer->m_diffusetexture = m_pGraphicsEngine->Get_Textures(equip->mDiffuseTexture);
					}
					if (!(equip->mNormalTexture.empty()))
					{
						buffer->m_normaltexture = m_pGraphicsEngine->Get_Textures(equip->mNormalTexture);
					}
				}

			}
		}

		// 외곽선 설정
		if (_entity->HasComponent<OutlineComponent>())
		{
			auto& outline = _entity->GetComponent<OutlineComponent>();
			modelBuffer->m_pVSEdgeConstantBuffer = m_pGraphicsEngine->CreateConstantBuffer(modelConst);
			outline.mVSEdgeCBD.EdgeScaleMatrix = Matrix::CreateScale(outline.mThickness);
			outline.mPSEdgeCBD.color = outline.mColor;
			modelBuffer->m_pPSEdgeConstantBuffer = m_pGraphicsEngine->CreateConstantBuffer(outline.mPSEdgeCBD);
		}
	}
	return true;
}

void RenderManager::Update(float _dTime)
{
	// 변경된 텍스쳐 업데이트
	UpdateTexture();

	//라이트 컴포넌트를 업데이트해준다. -> 전역적으로 쓰는 컨스턴트 버퍼의 데이터
	UpdateLight();

	// 상수 버퍼 업데이트
	UpdateModel(_dTime);

	// 큐브맵 업데이트
	UpdateCubeMap();

	// 픽셀 셰이더 상수 버퍼 업데이트
	UpdatePSCB();

	m_pGraphicsEngine->Update();

}

void RenderManager::UpdateEntityTexture(std::shared_ptr<Entity> _entity)
{

	if (!_entity->HasComponent<MeshRenderer>() || !_entity->HasComponent<Texture3D>())
	{
		return;
	}
	auto& texture3d = _entity->GetComponent<Texture3D>();
	auto& mesh = _entity->GetComponent<MeshRenderer>();

	//디퓨즈 텍스쳐를 가져온다.
	if (!(texture3d.mDiffuse.empty()))
	{
		auto newTexture3D = m_pGraphicsEngine->Get_Textures(texture3d.mDiffuse);
		mesh.mpModel->m_diffusetexture = newTexture3D;
	}
	//노말 텍스쳐를 가져온다.
	if (!(texture3d.mNormal.empty()))
	{
		auto newTextureNomal3D = m_pGraphicsEngine->Get_Textures(texture3d.mNormal);
		mesh.mpModel->m_normaltexture = newTextureNomal3D;
	}
}

void RenderManager::LateUpdate(float _dTime)
{
	auto boxGeometryView = mRegistry.view<BoxCollider, Transform>();
	for (auto entity : boxGeometryView)
	{
		auto& transform = mRegistry.get<Transform>(entity);
		auto& collider = mRegistry.get<BoxCollider>(entity);
		// 		auto& name = mRegistry.get<Name>(entity).mName;
		// 		auto uid = static_cast<int>(entity);
				// 장비 콜라이더
		if (transform.mpParent)
		{
			auto parentTrs = transform.mpParent;
			// 			auto name2 = parentTrs->mpOwner->GetName();
			if (!parentTrs->mpOwner.get())
			{
				continue;
			}
			if (parentTrs->mpOwner->HasComponent<WeaponComponent>())
			{
				auto& parent = parentTrs->mpOwner->GetComponent<WeaponComponent>();
				auto& parentMesh = parentTrs->mpOwner->GetComponent<MeshRenderer>();
				Matrix matrix
					= Matrix::CreateScale(collider.mSize)
					* Matrix::CreateTranslation(Vector3(collider.mCenter))
					* parent.mWeaponBoneMatrix;
				// 1.Translation 저장
				transform.mLocalPosition = Vector3(matrix._41, matrix._42, matrix._43);
				Vector3 scale;
				// 2. Scale 추출 
				scale.x = Vector3(matrix._11, matrix._12, matrix._13).Length();
				scale.y = Vector3(matrix._21, matrix._22, matrix._23).Length();
				scale.z = Vector3(matrix._31, matrix._32, matrix._33).Length();

				transform.mLocalScale = scale;
				// 3. Rotation 추출
				Matrix rotationMatrix = matrix;

				if (scale.x != 0) {
					rotationMatrix._11 /= scale.x;
					rotationMatrix._12 /= scale.x;
					rotationMatrix._13 /= scale.x;
				}
				if (scale.y != 0) {
					rotationMatrix._21 /= scale.y;
					rotationMatrix._22 /= scale.y;
					rotationMatrix._23 /= scale.y;
				}
				if (scale.z != 0) {
					rotationMatrix._31 /= scale.z;
					rotationMatrix._32 /= scale.z;
					rotationMatrix._33 /= scale.z;
				}

				// 4. Rotation 추출, 저장
				transform.mLocalRotation.x = atan2(rotationMatrix._32, rotationMatrix._33);  // Pitch (X축 회전)
				transform.mLocalRotation.y = atan2(-rotationMatrix._31, sqrt(rotationMatrix._32 * rotationMatrix._32 + rotationMatrix._33 * rotationMatrix._33));  // Yaw (Y축 회전)
				transform.mLocalRotation.z = atan2(rotationMatrix._21, rotationMatrix._11);  // Roll (Z축 회전)

				transform.mLocalMatrix = matrix;
				//parent.m_weaponBoneMatrix = matrix;
			}

		}
	}

	// 카메라 업데이트
	auto cameraView = mRegistry.view<CameraComponent>();
	for (auto entity : cameraView)
	{
		auto& camera = cameraView.get<CameraComponent>(entity);
	}
}

void RenderManager::Render(float _dTime)
{
	LightComponent* light = nullptr;
	//라이트 컴포넌트를 업데이트해준다. -> 전역적으로 쓰는 컨스턴트 버퍼의 데이터
	auto lightComponent = mRegistry.view<LightComponent>();
	for (auto entity : lightComponent)
	{
		if (light = mRegistry.try_get<LightComponent>(entity))
		{
			// 			m_pGraphicsEngine->UpdateCommonConstantBuffer(light->m_commonConstData);
			// 			light->m_commonConstData.light[0].viewProj = (m_pLightCamera[0]->GetViewRow() * m_pLightCamera[0]->GetProjRow()).Transpose();
		}
	}

	// 그림자 렌더링
	if (mRenderShadow && mRenderModel && light)
	{
		RenderShadow(light);
	}

	// 모델 렌더링
	if (mRenderModel)
	{
		RenderModel(light);
	}

	// 기하 정보 및 디버깅 정보 렌더링
	if (mRenderGeometry)
	{
		RenderGeometry(light);
	}

	m_pGraphicsEngine->RendParticle();

	/// 디버깅 정보 출력
	if (mRenderGeometry)
	{
		// 		m_pGraphicsEngine->UIStartFontID("B.ttf");
		// 		std::string fpsStr = std::format("{:.2f}", m_pTimeManager->FPS());
		// 		std::u8string fps(fpsStr.begin(), fpsStr.end());
		// 		std::string dTStr = std::format("{:.6f}", m_pTimeManager->DeltaTime());
		// 		std::u8string dTime(dTStr.begin(), dTStr.end());
		// 		m_pGraphicsEngine->UIDrawText(Vector2(0, 0), fps);
		// 		m_pGraphicsEngine->UIDrawText(Vector2(0, 40), dTime + std::u8string(u8"ms"));
		// 		m_pGraphicsEngine->UIFinFontID();
		// 		DLOG(LOG_INFO, fpsStr);
		// 		DLOG(LOG_INFO, dTStr);
	}
}

void RenderManager::BeginRender()
{
	m_pGraphicsEngine->BeginRender();
}

void RenderManager::EndRender()
{
	m_pGraphicsEngine->EndRender();
}

void RenderManager::Finalize()
{
	m_pGraphicsEngine->Finalize(); // 깡통임
}

void RenderManager::SetRenderMode(bool _model, bool _geometry, bool _shadow)
{
	mRenderModel = _model;
	mRenderGeometry = _geometry;
	mRenderShadow = _shadow;
}

void RenderManager::ChangeCamera(const uint8_t _index)
{
	auto cameraView = mRegistry.view<CameraComponent>();
	for (auto entity : cameraView)
	{
		auto cameraComponent = mRegistry.try_get<CameraComponent>(entity);
		// 		if (cameraComponent->m_index == _index)
		// 		{
		// 			m_pCurrentCamera = cameraComponent->m_pCamera;
		// 
		// 			m_pGraphicsEngine->SetCamera(m_pCurrentCamera);
		// 		}
	}
}

int RenderManager::GetScreenWidth()
{
	return m_pGraphicsEngine->GetScreenWidth();
}

int RenderManager::GetScreenHeight()
{
	return m_pGraphicsEngine->GetScreenHeight();
}

void RenderManager::AddModel(std::string _basePath, std::string _fileName)
{
	m_pGraphicsEngine->AddModel(_basePath, _fileName);
}

void RenderManager::AddAnimation(std::string _basePath, std::string _fileName)
{
	m_pGraphicsEngine->AddAnimation(_basePath, _fileName);
}

Animation* RenderManager::Get_Animation(std::string _animeName)
{
	return m_pGraphicsEngine->Get_Animation(_animeName);
}

void RenderManager::Erase_Animation(std::string _animName)
{
	m_pGraphicsEngine->Erase_Animation(_animName);
}

void RenderManager::Add3DTexture(std::string _basePath, std::string _fileName)
{
	m_pGraphicsEngine->Add3DTexture(_basePath, _fileName);
}

void RenderManager::AddDDSTexture(std::string _basePath, std::string _fileName)
{
	m_pGraphicsEngine->AddDDSTexture(_basePath, _fileName);
}

void RenderManager::Erase_Textures(std::string _textureName)
{
	m_pGraphicsEngine->Erase_Textures(_textureName);
}

int RenderManager::Get_TargetModelBoneIndex(std::string _modelName, std::string _boneName)
{
	return m_pGraphicsEngine->Get_TargetModelBoneIndex(_modelName, _boneName);
}

Matrix RenderManager::GetTargetBoneAboveMatrix(std::string _targetModel, std::string _targetBoneName, float _scale /*= 1.0f*/)
{
	return m_pGraphicsEngine->GetTargetBoneAboveMatrix(_targetModel, _targetBoneName, _scale);
}

AABB RenderManager::Get_AABB(std::string __targetModel)
{
	return m_pGraphicsEngine->Get_AABB(__targetModel);
}

void RenderManager::LightInitialize(CommonConstantBufferData* _ccBufferData, UINT _num)
{
	m_pGraphicsEngine->LightInitialize(_ccBufferData, _num);
}

void RenderManager::SetDirLight(CommonConstantBufferData* _ccBufferData, UINT _index, float _strength, Vector3 _dir, Vector3 _color /*= Vector3(1.0f, 1.0f, 1.0f)*/)
{
	m_pGraphicsEngine->SetDirLight(_ccBufferData, _index, _strength, _dir, _color);
}

void RenderManager::PrintLightInfo(CommonConstantBufferData* _ccBufferData)
{
	m_pGraphicsEngine->PrintLightInfo(_ccBufferData);
}

void RenderManager::CameraSetPerspective()
{
	auto cameraView = mRegistry.view<CameraComponent>();
	for (auto entity : cameraView)
	{
		auto& cameraComponent = mRegistry.get<CameraComponent>(entity);
		if (cameraComponent.mCameraEnum == 0)
		{
			cameraComponent.mpCamera->SetPerspective();
			cameraComponent.mIsPerspective = true;
		}
	}

}

void RenderManager::CameraSetOrthographic(float _scale)
{
	auto cameraView = mRegistry.view<CameraComponent>();
	for (auto entity : cameraView)
	{
		auto& cameraComponent = mRegistry.get<CameraComponent>(entity);
		if (cameraComponent.mCameraEnum == 0)
		{
			cameraComponent.mpCamera->SetOrthgraphic(_scale);
			cameraComponent.mIsPerspective = false;
		}
	}
}

void RenderManager::SetOpacityFactor(const Vector4& _value)
{
	m_opacityFactor[0] = _value.x;
	m_opacityFactor[1] = _value.y;
	m_opacityFactor[2] = _value.z;
	m_opacityFactor[3] = _value.w;
	m_pGraphicsEngine->SetOpacityFactor(m_opacityFactor);
}

void RenderManager::AddParticle(uint16_t _num, CSParticleData& _particleData) const
{
	m_pGraphicsEngine->m_pParticleManager->AddParticle(_num, _particleData);
}

void RenderManager::RenderShadow(LightComponent* _light)
{
	m_pGraphicsEngine->SetCamera(mpLightCamera[0]);
	m_pGraphicsEngine->UpdateCommonConstantBuffer(_light->mCommonConstData);

	auto view = mRegistry.view<MeshRenderer>();
	for (auto entity : view)
	{
		auto& mesh = mRegistry.get<MeshRenderer>(entity);
		auto equips = mRegistry.try_get<EquipmentComponent>(entity);
		auto weapon = mRegistry.try_get<WeaponComponent>(entity);
		auto buffer = mesh.mpModel.get();

		if (auto alpha = mRegistry.try_get<AlphaBlendComponent>(entity))
		{
			continue;
		}

		if (mesh.mpModel == nullptr)
		{
			DLOG(LOG_ERROR, "Model Pointer is Nullptr");
		}
		else
		{
			if (mesh.mIs_VSBoneConstant)
			{
				m_pGraphicsEngine->mpRenderer->RenderAniDepthMap(buffer);
			}
			else
			{
				if (mesh.mIs_VSTargetBoneconstant)
				{
					if (!mesh.mMeshName.empty())
					{
						//m_pGraphicsEngine->mpRenderer->RenderEquipDepthMap(buffer);
					}
				}
				else
				{
					m_pGraphicsEngine->mpRenderer->RenderDepthMap(buffer);
				}
			}
		}
		if (weapon)
		{
			m_pGraphicsEngine->mpRenderer->RenderDepthMap(weapon->mpModel.get());
		}
		if (equips)
		{
			for (auto& equip : equips->mpEquipments)
			{
				if (!mesh.mMeshName.empty())
				{
					//m_pGraphicsEngine->mpRenderer->RenderEquipDepthMap(equip->mpModelBuffer.get());
				}
			}
		}
	}
}

void RenderManager::RenderModel(LightComponent* _light)
{
	m_pGraphicsEngine->SetCamera(mpWorldCamera);
	if (_light)
	{
		m_pGraphicsEngine->UpdateCommonConstantBuffer(_light->mCommonConstData);
	}

	auto cubeView = mRegistry.view<CubeMapComponent>();
	for (auto& entity : cubeView)
	{
		auto& cube = mRegistry.get<CubeMapComponent>(entity);
		m_pGraphicsEngine->Rend_CubeMap(cube.mpModel.get());
	}

	auto view = mRegistry.view<MeshRenderer>();
	for (auto& entity : view)
	{
		auto& mesh = mRegistry.get<MeshRenderer>(entity);
		auto equips = mRegistry.try_get<EquipmentComponent>(entity);
		auto weapon = mRegistry.try_get<WeaponComponent>(entity);
		auto buffer = mesh.mpModel.get();
		auto& name = mRegistry.get<Name>(entity).mName;
		auto outline = mRegistry.try_get<OutlineComponent>(entity);

		if (auto alpha = mRegistry.try_get<AlphaBlendComponent>(entity))
		{
			if (outline)
			{
				m_pGraphicsEngine->Rend_EdgeModel(buffer);
			}
			SetOpacityFactor(Vector4(alpha->mAlpha));
			m_pGraphicsEngine->Rend_OpacitiyModel(buffer);
			continue;
		}

		if (auto flowTex = mRegistry.try_get<FlowTextureComponent>(entity))
		{
			m_pGraphicsEngine->Rend_Water(buffer);
			continue;
		}

		if (mesh.mpModel == nullptr)
		{
			DLOG(LOG_ERROR, "Model Pointer is Nullptr");
		}
		else
		{
			if (mesh.mIs_CubeMap)
			{
				m_pGraphicsEngine->Rend_CubeMap(buffer);
			}
			else
			{
				if (mesh.mIs_VSBoneConstant)
				{
					m_pGraphicsEngine->Rend_AnimateModel(buffer);
				}
				else
				{
					if (mesh.mIs_VSTargetBoneconstant)
					{
						if (mesh.mMeshName != "")
						{
							m_pGraphicsEngine->Rend_EquipmentModel(buffer);
						}
					}
					else
					{
						m_pGraphicsEngine->Rend_Model(buffer);
					}
				}
			}
		}

		if (weapon)
		{
			m_pGraphicsEngine->Rend_Model(weapon->mpModel.get());
		}

		if (equips)
		{
			for (auto& equip : equips->mpEquipments)
			{
				if (!equip->mMeshName.empty())
				{
					m_pGraphicsEngine->Rend_EquipmentModel(equip->mpModelBuffer.get());
				}
			}
		}

		if (outline)
		{
			m_pGraphicsEngine->Rend_EdgeModel(buffer);
		}
	}

}

void RenderManager::RenderGeometry(LightComponent* _light)
{
	// 라이트의 위치를 본다.
	if (_light)
	{
		m_pGraphicsEngine->Rend_DebugSphere({ 1,1,1 }, { 0,0,0 }, mpLightCamera[0]->GetmViewPos());
	}

	auto boxGeometryView = mRegistry.view<BoxCollider, Transform>();
	for (auto entity : boxGeometryView)
	{
		auto& transform = mRegistry.get<Transform>(entity);
		auto& collider = mRegistry.get<BoxCollider>(entity);

		// 장비 콜라이더
		if (transform.mpParent)
		{
			// 콜라이더 렌더링
			m_pGraphicsEngine->Rend_DebugBox(
				Matrix()
				, Matrix()
				, transform.mLocalMatrix
			);
		}
		else
		{
			m_pGraphicsEngine->Rend_DebugBox(collider.mSize
				, transform.mLocalRotation
				, transform.mLocalPosition + collider.mCenter);
		}
	}
	auto sphereGeometryView = mRegistry.view<SphereCollider, Transform>();
	for (auto entity : sphereGeometryView)
	{
		auto& transform = mRegistry.get<Transform>(entity);
		auto& collider = mRegistry.get<SphereCollider>(entity);

		m_pGraphicsEngine->Rend_DebugSphere(Vector3(collider.mRadius), transform.mLocalRotation
			, transform.mLocalPosition + collider.mCenter);
	}
	auto capsuleGeometryView = mRegistry.view<CapsuleCollider, Transform>();
	for (auto entity : capsuleGeometryView)
	{
		auto& transform = mRegistry.get<Transform>(entity);
		auto& collider = mRegistry.get<CapsuleCollider>(entity);
		m_pGraphicsEngine->Rend_DebugCapsule(Vector3(collider.mRadius, collider.mHeight, collider.mRadius)
			, transform.mLocalRotation
			, transform.mLocalPosition + collider.mCenter);
	}
}

void RenderManager::UpdateLight()
{
	auto lightComponent = mRegistry.view<LightComponent>();
	for (auto entity : lightComponent)
	{
		auto& light = mRegistry.get<LightComponent>(entity);
		m_pGraphicsEngine->UpdateCommonConstantBuffer(light.mCommonConstData);
		light.mCommonConstData.light[0].viewProj = (mpLightCamera[0]->GetViewRow() * mpLightCamera[0]->GetProjRow()).Transpose();
	}
}

void RenderManager::UpdateModel(float _dTime)
{
	auto viewAnim = mRegistry.view<MeshRenderer>();
	for (auto& entity : viewAnim)
	{
		auto& mesh = mRegistry.get<MeshRenderer>(entity);
		auto ani = mRegistry.try_get<AnimationComponent>(entity);
		auto bone = mRegistry.try_get<BoneGroupComponent>(entity);
		//auto& name = mRegistry.get<Name>(entity).mName;
		auto& transform = mRegistry.get<Transform>(entity);
		auto equips = mRegistry.try_get<EquipmentComponent>(entity);
		auto outline = mRegistry.try_get<OutlineComponent>(entity);
		auto flowTexture = mRegistry.try_get<FlowTextureComponent>(entity);

		ModelBuffer* modelBuffer = mesh.mpModel.get();
		if (ani)
		{
			mpAnimationManager->AnimationUpdate(mesh.mpOwner, _dTime);
		}

		mesh.mVSConstantBufferData.world = transform.GetTransformMatrix();
		mesh.mVSConstantBufferData.invWorld = mesh.mVSConstantBufferData.world.Invert();

		if (mesh.mIs_VSconstant == true)
		{
			m_pGraphicsEngine->UpdateConstantBuffer(modelBuffer, mesh.mVSConstantBufferData);
		}

		// 애니메이션 업데이트를 먼저 수행한다.
		if (ani)
		{

			if (ani->mpNextTargetAnimation) //if (modelBuffer->mpNextTargetAnimation)
			{
				if (!m_pGraphicsEngine->UpdateTransitionBoneConstantBuffer(modelBuffer, bone->mBoneData))			  ///false가 반환되었는가? -> 다음 애니메이션을 현재의 애니메이션으로 교체한다.
				{
					mpAnimationManager->InterPolationAnimation(mesh.mpOwner);
				}
			}
			else
			{
				m_pGraphicsEngine->UpdateBoneConstantBuffer(modelBuffer, bone->mBoneData);
			}
		}

		// 무기 업데이트
		if (auto weapon = mRegistry.try_get<WeaponComponent>(entity))
		{
			if (weapon->mpAttachedEntity)
			{
				auto& temp = weapon->mWeaponConstantBufferData;
				// temp.world = weapon->m_pAttachedEntity->GetComponent<Transform>().m_localMatrix.Transpose();
				// temp.invWorld = weapon->m_pAttachedEntity->GetComponent<Transform>().m_localMatrix.Transpose().Invert();

				temp.world = mesh.mVSConstantBufferData.world * m_pGraphicsEngine->GetTargetBoneAboveMatrix(mesh.mFile, weapon->mBoneIndex, weapon->mScale).Transpose();
				temp.invWorld = temp.world.Invert();

				weapon->mWeaponBoneMatrix = temp.world.Transpose();
				m_pGraphicsEngine->UpdateConstantBuffer(weapon->mpModel.get(),
					temp);
			}

		}

		// 장비 업데이트
		if (equips)
		{
			auto boneGroup = mRegistry.try_get<BoneGroupComponent>(entity);

			for (auto& equip : equips->mpEquipments)
			{
				if (!equip->mMeshName.empty())
				{
					m_pGraphicsEngine->UpdateConstantBuffer(equip->mpModelBuffer.get(), mesh.mVSConstantBufferData);

					equip->mTargetBoneConstantBufferData.targrtBoneMatrix = boneGroup->mBoneData.bone[equip->mTargetBoneIndex];
					m_pGraphicsEngine->UpdateTargetBoneConstantBuffer(equip->mpModelBuffer.get(), equip->mTargetBoneConstantBufferData);
				}
			}
		}

		// 외곽선 업데이트
		if (outline)
		{
			outline->mPSEdgeCBD.color = outline->mColor;
			m_pGraphicsEngine->UpdateVSEdgeConstantBuffer(modelBuffer, outline->mVSEdgeCBD);
			m_pGraphicsEngine->UpdatePSEdgeConstantBuffer(modelBuffer, outline->mPSEdgeCBD);
		}

		if (flowTexture)
		{
			flowTexture->mVSWCSD.time += _dTime;
			m_pGraphicsEngine->UpdateVSWaterConstantBuffer(modelBuffer, flowTexture->mVSWCSD);
		}
	}
}

void RenderManager::UpdateCubeMap()
{
	auto cubeView = mRegistry.view<CubeMapComponent>();
	for (auto& entity : cubeView)
	{
		auto& cube = mRegistry.get<CubeMapComponent>(entity);
		auto& transform = mRegistry.get<Transform>(entity);
		cube.mVSCBD.world = transform.GetTransformMatrix();
		cube.mVSCBD.invWorld = cube.mVSCBD.world.Invert();
		m_pGraphicsEngine->UpdateConstantBuffer(cube.mpModel.get(), cube.mVSCBD);
		PSConstantBufferData lightData;
		m_pGraphicsEngine->UpdateConstantBuffer(cube.mpModel.get(), lightData);
	}
}

void RenderManager::UpdatePSCB()
{
	auto view = mRegistry.view<MeshRenderer, Transform>();
	for (auto& entity : view)
	{
		auto& transform = mRegistry.get<Transform>(entity);
		auto& mesh = mRegistry.get<MeshRenderer>(entity);
		ModelBuffer* modelBuffer = mesh.mpModel.get();

		//원래는 렌더링되는 객체마다 가지고 있어야하나 현재는 편의상 하나로 퉁친다.
		PSConstantBufferData lightData;
		if (mesh.mIs_PSconstant)
		{
			m_pGraphicsEngine->UpdateConstantBuffer(modelBuffer, lightData);
		}
	}
}

void RenderManager::UpdateTexture()
{
	// 	// 변경된 3D 텍스쳐 업데이트
	// 	m_pTexture3DObserver->each([this](entt::entity entity)
	// 		{
	// 			if (mRegistry.any_of<MeshRenderer, Texture3D>(entity))
	// 			{
	// 				auto& name = mRegistry.get<Name>(entity).mName;
	// 				auto mesh = mRegistry.try_get<MeshRenderer>(entity);
	// 				auto texture = mRegistry.try_get<Texture3D>(entity);
	// 				if (mesh && texture)
	// 				{
	// 					if (!texture->m_diffuse.empty())
	// 					{
	// 						mesh->m_pModel->m_diffusetexture = m_pGraphicsEngine->Get_Textures(texture->m_diffuse);
	// 					}
	// 					if (!texture->m_normal.empty())
	// 					{
	// 						mesh->m_pModel->m_normaltexture = m_pGraphicsEngine->Get_Textures(texture->m_normal);
	// 					}
	// 					if (!texture->m_specular.empty())
	// 					{
	// 						mesh->m_pModel->m_cubeMapSpecularResourceView = m_pGraphicsEngine->Get_Textures(texture->m_specular);
	// 					}
	// 				}
	// 			}
	// 		});
	// 	// 옵저버 초기화
	// 	m_pTexture3DObserver->clear();
}     