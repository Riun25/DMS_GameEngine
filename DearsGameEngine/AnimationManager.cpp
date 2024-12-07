#include "AnimationManager.h"

AnimationManager::AnimationManager(entt::registry& _registry, std::shared_ptr<DearsGraphicsEngine> _graphicsEngine)
	: m_registry(_registry), mpGraphicsEngine(_graphicsEngine)
{
}

AnimationManager::~AnimationManager()
{
}

void AnimationManager::AnimationUpdate(std::shared_ptr<Entity> _entity, float _dTime)
{
	//엔티티의 모델버퍼를 불러온다.
	auto& modelBuffer = _entity->GetComponent<MeshRenderer>().m_pModel;
	//엔티티의 animationComponent를 불러온다.
	if (_entity->HasComponent<AnimationComponent>())
	{
		auto& animationComp = _entity->GetComponent<AnimationComponent>();

		// 만약에 Animation이 정지 상태면 종료
		//정지 상태인게 뭐가있을까..? 죽었을때? 게임이 마무리 되었을때?
		if (animationComp.mPaused == true)
		{
			return;
		}

		animationComp.mAnimationCoolTime = GetCoolTime(_entity);



		// 애니메이션 플레이타임에 경과 시간을 더해준다.
		animationComp.mAnimationPlaytime += _dTime * animationComp.mPlaySpeed;
		//	std::cout << _dTime << std::endl;
			//여기서 루프처리를 해주는 식으로. 루프 변수가 true일 경우 애니메이션을 연속재생시키고 그게 아닐경우는 한번만 실행되도록..!
		if (animationComp.mIsLoop)
		{
			///어쩌구 저쩌구
			if (animationComp.mAnimationPlaytime * modelBuffer->mpTargetAnimation->mTicksPerSecond >= modelBuffer->mpTargetAnimation->mDuration)
			{
				animationComp.mAnimationPlaytime = 0.0f;
			}

		}

		// 현재 실행중인 다음 애니메이션이 존재한다면, 다음 애니메이션에도 경과시간을 더해준다.
		if (animationComp.mpNextTargetAnimation)
		{
			animationComp.mNextAnimationPlayTime += _dTime * animationComp.mPlaySpeed;
		}
		modelBuffer->mAnimationPlaytime = animationComp.mAnimationPlaytime;
		modelBuffer->mNextAnimationPlaytime = animationComp.mNextAnimationPlayTime;
		modelBuffer->mpTargetAnimation = animationComp.mpTargetAnimation;
		modelBuffer->mpNextTargetAnimation = animationComp.mpNextTargetAnimation;



		// 	// 모델 버퍼에 있는 플레이타임과 애니메이션 플레이타임의 시간을 일치시켜준다.
		// 	modelBuffer->mAnimationPlaytime = animationComp.mAnimationPlaytime;
		// 
		// 
		// 	// 보간을 할 애니메이션이 있다면
		// 	if (animationComp.mpNextTargetAnimation != "")
		// 	{
		// 		Animation* tempAnimation = mpGraphicsEngine->Get_Animation(animationComp.mpNextTargetAnimation);
		// 
		// 		// 타깃 애니메이션과 넥스트 타깃 애니메이션이 같은 경우 보간을 하지 않음
		// 		if (animationComp.mpTargetAnimation != animationComp.mpNextTargetAnimation)
		// 		{
		// 			// 현재 넥스트 애니메이션과 전환하려는 애니메이션이 다를 경우 && 보간 중인 애니메이션이 없음
		// 			if (modelBuffer->mpNextTargetAnimation != tempAnimation && (!modelBuffer->mpNextTargetAnimation))
		// 			{
		// 				modelBuffer->mpNextTargetAnimation = tempAnimation;
		// 				animationComp.isInterpolation = true;
		// 			}
		// 		}
		// 		else
		// 		{
		// 			// 넥스트 타깃 애니메이션과 타깃 애니메이션이 같으므로 보간을 생략하고 바로 루핑 처리
		// 			animationComp.mpNextTargetAnimation = "";
		// 			animationComp.isInterpolation = false;
		// 		}
		// 	}
		// 
		// 	// 쿨타임도 넣어준다.
		// 	animationComp.mAnimationCoolTime = GetCoolTime(_entity);
		// 
		// 	// 애니메이션이 루핑 상태일 경우
		// 	if (animationComp.mIsLoop == true)
		// 	{
		// 		SettingLoopAnimation(_entity);
		// 	}
		// 	else
		// 	{
		// 		// 보간을 할 애니메이션이 없다면
		// 		if (_entity->GetComponent<AnimationComponent>().mpNextTargetAnimation == "")
		// 		{
		// 			animationComp.isInterpolation = false;
		// 			_entity->GetComponent<AnimationComponent>().mAnimationPlaytime = 0.0f;
		// 		}
		// 	}
	}

}

void AnimationManager::SetInitialAnimation(std::shared_ptr<Entity> _entity)
{
	auto& modelBuffer = _entity->GetComponent<MeshRenderer>().m_pModel;
	auto& animationComp = _entity->GetComponent<AnimationComponent>();
	if (animationComp.mpTargetAnimation)
	{
		modelBuffer->mpTargetAnimation = animationComp.mpTargetAnimation;
	}
	//modelBuffer->mpTargetAnimation = mpGraphicsEngine->Get_Animation(_entity->GetComponent<AnimationComponent>().mpTargetAnimation);
}

void AnimationManager::InterPolationAnimation(std::shared_ptr<Entity> _entity)
{
	// 	auto& modelBuffer = _entity->GetComponent<MeshRenderer>().m_pModel;
	// 
	// 	// 현재 애니메이션을 보간된 애니메이션으로 교체
	// 	modelBuffer->mAnimationPlaytime = modelBuffer->mNextAnimationPlaytime;
	// 	modelBuffer->mpTargetAnimation = modelBuffer->mpNextTargetAnimation;
	// 	_entity->GetComponent<AnimationComponent>().mpTargetAnimation = _entity->GetComponent<AnimationComponent>().mpNextTargetAnimation;
	// 
	// 	// 보간 상태 초기화
	// 	modelBuffer->mNextAnimationPlaytime = 0;
	// 	modelBuffer->mpNextTargetAnimation = nullptr;
	// 	_entity->GetComponent<AnimationComponent>().mpNextTargetAnimation = "";
	auto& animationComp = _entity->GetComponent<AnimationComponent>();
	animationComp.mAnimationPlaytime = animationComp.mNextAnimationPlayTime;
	animationComp.mpTargetAnimation = animationComp.mpNextTargetAnimation;
	animationComp.mNextAnimationPlayTime = 0;
	animationComp.mpNextTargetAnimation = nullptr;


}

void AnimationManager::SettingLoopAnimation(std::shared_ptr<Entity> _entity)
{
	// 	auto& animationComp = _entity->GetComponent<AnimationComponent>();
	// 
	// 	if (animationComp.mAnimationPlaytime > animationComp.mAnimationCoolTime)
	// 	{
	// 		if (_entity->GetComponent<AnimationComponent>().mpNextTargetAnimation == "")
	// 		{
	// 			_entity->GetComponent<AnimationComponent>().mAnimationPlaytime = 0.0f;
	// 		}
	// 		else
	// 		{
	// 			animationComp.isInterpolation = false;
	// 			mpGraphicsEngine->Get_Animation(_entity->GetComponent<AnimationComponent>().mpTargetAnimation); // 보간되는 애니메이션이 반복되지 않게 방지하기 위한 코드다. 
	// 			animationComp.mAnimationPlaytime = 0.0f;
	// 		}
	// 	}
}

float AnimationManager::GetCoolTime(std::shared_ptr<Entity> _entity)
{
	// 	auto& modelBuffer = _entity->GetComponent<MeshRenderer>().m_pModel;
	// 	auto& animationComp = _entity->GetComponent<AnimationComponent>();
	// 	Animation* animation = mpGraphicsEngine->Get_Animation(animationComp.mpTargetAnimation);
	// 
	// 	// 보간 중인 애니메이션이 있음
	// 	if (animationComp.mpNextTargetAnimation != "" && animationComp.isInterpolation == true)
	// 	{
	// 		Animation* nextAnimation = mpGraphicsEngine->Get_Animation(animationComp.mpNextTargetAnimation);
	// 
	// 		double targetDuration = animation->mDuration / animation->mTicksPerSecond;
	// 		double nextTargetDuration = nextAnimation->mDuration / nextAnimation->mTicksPerSecond;
	// 		double interpolationTime = modelBuffer->mNextAnimationPlaytime;
	// 		float playtime = modelBuffer->mAnimationPlaytime;
	// 
	// 		//int a = 1; // 디버깅용
	// 
	// 		if (playtime >= targetDuration)
	// 		{
	// 			// 첫 번째 애니메이션이 끝나면 남은 시간에서 두 번째 애니메이션의 시간을 사용
	// 			double remainingTime = playtime - targetDuration;
	// 			//std::cout << remainingTime << "/" << nextTargetDuration << std::endl;
	// 
	// 			if (remainingTime < nextTargetDuration)
	// 			{
	// 				return static_cast<float>(remainingTime);
	// 			}
	// 			else
	// 			{
	// 				// 두 번째 애니메이션이 종료된 경우
	// 				return static_cast<float>(nextTargetDuration);
	// 			}
	// 		}
	// 		else
	// 		{
	// 			// 보간 중이므로, 전체 시간을 합산하여 반환
	// 			//std::cout << playtime << "/" << static_cast<float>(targetDuration + nextTargetDuration) << std::endl;
	// 			return static_cast<float>(targetDuration + nextTargetDuration - playtime);
	// 		}
	// 
	// 	}
	// 	else
	// 	{
	// 		//std::cout << modelBuffer->mAnimationPlaytime << "/" << static_cast<float>(animation->mDuration / animation->mTicksPerSecond + (animationComp.mHoldTimeAtEnd / animationComp.mPlaySpeed)) << std::endl;
	// 		return static_cast<float>(animation->mDuration / animation->mTicksPerSecond + (animationComp.mHoldTimeAtEnd / animationComp.mPlaySpeed));
	// 	}
	return 0;
}
