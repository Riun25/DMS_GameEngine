#include "UIManager.h"
#include "CameraComponent.h"
#include "Transform.h"
#include "Texture2D.h"
#include "Text.h"
#include "Box2D.h"
#include "FreeBox2D.h"
#include "HealthBarComponenet.h"
#include "Tile.h"
#include "MessageBox2D.h"
#include "MessageBox3D.h"

bool UIManager::Initialize()
{
	return true;
}

void UIManager::FixedUpdate(float _fixedDTime)
{

}

void UIManager::Update(float _dTime)
{
	auto cameraView = mRegistry.view<CameraComponent>();
	for (auto entity : cameraView)
	{
		auto& camera = mRegistry.get<CameraComponent>(entity);
		if (camera.m_cameraEnum == 0 && camera.m_pCamera)
		{
			UpdateViewNProjection(camera.m_pCamera);
		}

	}

	Vector2 mousePos = mpInputManager->GetMousePos();

	auto textureView = mRegistry.view<Texture2D, Button>();
	for (auto& entity : textureView)
	{
		auto& texture = mRegistry.get<Texture2D>(entity);
		auto& button = mRegistry.get<Button>(entity);
		// 버튼이 사용불가이면 넘긴다.
		if (!button.mIsEnable) continue;

		// 호버링이 된 상태인지 확인한다.
		bool isHovered =
			(mousePos.x >= texture.m_position.x) && (mousePos.x <= texture.m_position.x + texture.m_size.x) &&
			(mousePos.y >= texture.m_position.y) && (mousePos.y <= texture.m_position.y + texture.m_size.y);

		// 버튼의 상태를 체크한다.
		CheckButtonState(&button, isHovered);

		auto text = mRegistry.try_get<Text>(entity);
		UpdateButton(&texture, &button, text);

	}

	/// 모델 버퍼에서 사용하는 경우에 대한 텍스처를 업데이트한다.
	auto texture2DView = mRegistry.view<Texture2D>();
	for (auto& entity : texture2DView)
	{
		auto& texture = mRegistry.get<Texture2D>(entity);
		auto mesh = mRegistry.try_get<MeshRenderer>(entity);

		if (!(texture.m_file == "") && mesh)
		{
			auto newTexture2D = mpGraphicsEngine->Get_Textures(texture.m_file);
			if (mesh->m_pModel->m_diffusetexture != newTexture2D)
			{
				if (auto button = mRegistry.try_get<Button>(entity))
				{
					button->mUIPosition = texture.m_position;
					button->mNormalUIName = texture.m_file;
				}
				mesh->m_pModel->m_diffusetexture = newTexture2D;
			}
		}
	}

}

void UIManager::LateUpdate(float _dTime)
{
}

void UIManager::Render(float _dTime)
{
	for (int i = maxNumLayer; i >= 0; i--)
	{
		/// 타일은 가장 안쪽에 그린다.
		RenderTile(i);

		/// texture 먼저 그림 (순서 변경 금지 -> 텍스처보다 텍스트가 위로 올라와야 함)
		RenderUI(i);

		/// 체력바를 그림
		RenderHealthBar(i);

		/// 직사각형을 이때 그리겠습니다.
		RenderRectFilled(i);

		/// 해당 레이어의 텍스트를 렌더링
		RenderText(i);
	}

	RenderDebugInfo();


}

void UIManager::Finalize()
{
	// 텍스트 삭제
	auto view1 = mRegistry.view<Text>();
	for (auto& entity : view1)
	{
		// Text 컴포넌트가 있는 경우에만 실행
		if (auto text = mRegistry.try_get<Text>(entity))
		{
			// 폰트는 중복이 될 수 있기 때문에 있는지 확인하고 있을 때만 지워준다.
			/*if (mpGraphicsEngine->Get_Font(text->m_font) != nullptr)
			{
				mpGraphicsEngine->Erase_Textures(text->m_font);
			}*/
			RemoveText(text->m_pOwner);
		}
	}

	// 직사각형 삭제
	auto view2 = mRegistry.view<Box2D>();
	for (auto& entity : view2)
	{
		if (auto box2D = mRegistry.try_get<Box2D>(entity))
		{
			RemoveRect(box2D->m_pOwner);
		}
	}

	// 체력바 삭제
	auto view3 = mRegistry.view<HealthBarComponenet>();
	for (auto& entity : view3)
	{
		if (auto HPBar = mRegistry.try_get<HealthBarComponenet>(entity))
		{
			RemoveHPBar(HPBar->m_pOwner);
		}
	}

	// 텍스처 삭제
	auto view4 = mRegistry.view<Texture2D>();
	for (auto& entity : view4)
	{
		// Texture2D 컴포넌트가 있는 경우에만 실행 : 이게 있어야만 버튼이 생성되기 때문에 따로 버튼은 삭제 코드를 넣지 않았다.
		if (auto texture = mRegistry.try_get<Texture2D>(entity))
		{
			RemoveUI(texture->m_pOwner);
		}
	}

	// 메세지 박스 삭제
	auto view5 = mRegistry.view<MessageBox2D>();
	for (auto& entity : view5)
	{
		if (auto texture = mRegistry.try_get<MessageBox2D>(entity))
		{
			RemoveMessageBox2D(texture->m_pOwner);
		}
	}
	auto view6 = mRegistry.view<MessageBox3D>();
	for (auto& entity : view6)
	{
		if (auto texture = mRegistry.try_get<MessageBox3D>(entity))
		{
			RemoveMessageBox3D(texture->m_pOwner);
		}
	}
}

void UIManager::AddFont(std::string _basePath, std::string _fileName, float _size, bool _isKorean)
{
	if (mIsAlreadyFontLoaded == false)
	{
		mpGraphicsEngine->AddFont(_basePath, _fileName, _size, _isKorean);
	}
	else
	{
		std::cout << "이미 폰트 세팅을 호출했습니다. 추가 세팅이 무시됩니다." << std::endl;
	}
}

void UIManager::FontSetFinish()
{
	if (mIsAlreadyFontLoaded == false)
	{
		mpGraphicsEngine->FontSetFinish();
		mIsAlreadyFontLoaded = true;
	}
	else
	{
		std::cout << "이미 폰트 세팅을 호출했습니다. 추가 세팅이 무시됩니다." << std::endl;
	}
}

void UIManager::AddText(std::shared_ptr<Entity> _entity, std::u8string _text, std::string _fontName, Vector2 _pos, int _layer, bool _isVisible, Vector4 _rgba)
{
	_entity->AddComponent<Text>(_text, _fontName, _pos, _layer, _isVisible, _rgba); // 텍스트 컴포넌트 추가
	auto& text = _entity->GetComponent<Text>();
	maxNumLayer = std::max(maxNumLayer, _layer);  // 최대 레이어 번호 갱신
}

void UIManager::AddTextwithInt(std::shared_ptr<Entity> _entity, std::u8string _text, std::string _fontName, Vector2 _pos, int _num, int _layer /*= 0*/, bool _isVisible /*= true*/, Vector4 _rgba /*= Vector4(0.0f, 0.0f, 0.0f, 1.0f)*/)
{
	AddText(_entity, _text, _fontName, _pos, _layer, _isVisible, _rgba);
	auto& text = _entity->GetComponent<Text>();
	text.m_idx = TEXT_IDX::INT1;
	text.m_num1 = _num;
}

void UIManager::AddTextwithInt2(std::shared_ptr<Entity> _entity, std::u8string _text, std::string _fontName, Vector2 _pos, int _num1, int _num2, int _layer /*= 0*/, bool _isVisible /*= true*/, Vector4 _rgba /*= Vector4(0.0f, 0.0f, 0.0f, 1.0f)*/)
{
	AddText(_entity, _text, _fontName, _pos, _layer, _isVisible, _rgba);
	auto& text = _entity->GetComponent<Text>();
	text.m_idx = TEXT_IDX::INT2;
	text.m_num1 = _num1;
	text.m_num2 = _num2;
}

void UIManager::AddTextwithFloat(std::shared_ptr<Entity> _entity, std::u8string _text, std::string _fontName, Vector2 _pos, float _num, int _layer /*= 0*/, bool _isVisible /*= true*/, Vector4 _rgba /*= Vector4(0.0f, 0.0f, 0.0f, 1.0f)*/)
{
	AddText(_entity, _text, _fontName, _pos, _layer, _isVisible, _rgba);
	auto& text = _entity->GetComponent<Text>();
	text.m_idx = TEXT_IDX::FLOAT1;
	text.m_num3 = _num;
}

void UIManager::AddTextwithFloat2(std::shared_ptr<Entity> _entity, std::u8string _text, std::string _fontName, Vector2 _pos, float _num1, float _num2, int _layer /*= 0*/, bool _isVisible /*= true*/, Vector4 _rgba /*= Vector4(0.0f, 0.0f, 0.0f, 1.0f)*/)
{
	AddText(_entity, _text, _fontName, _pos, _layer, _isVisible, _rgba);
	auto& text = _entity->GetComponent<Text>();
	text.m_idx = TEXT_IDX::FLOAT2;
	text.m_num3 = _num1;
	text.m_num4 = _num2;
}

void UIManager::RemoveText(std::shared_ptr<Entity> _entity)
{
	// 방어 코드: 엔티티에 텍스처나 텍스트 컴포넌트가 없으면 함수 실행 안함
	if (!_entity->HasComponent<Text>())
	{
		return;
	}
	// 컴포넌트 제거
	_entity->RemoveComponent<Text>();
}

void UIManager::EraseFont(std::string _fontName)
{
	mpGraphicsEngine->Erase_Font(_fontName);
}

void UIManager::AddTexture2D(std::string _basePath, std::string _fileName)
{
	mpGraphicsEngine->Add2DTexture(_basePath, _fileName);
}

void UIManager::AddUI(std::shared_ptr<Entity> _entity, std::string _fileName, Vector2 _pos, Vector2 _size, int _layer, Vector4 _rgba, bool _isVisible)
{
	// Texture2D 컴포넌트를 엔티티에 추가
	_entity->AddComponent<Texture2D>(_fileName, _pos, _size, _layer, _rgba, _isVisible);
	maxNumLayer = std::max(maxNumLayer, _layer);  // 최대 레이어 번호 갱신
}

void UIManager::RemoveUI(std::shared_ptr<Entity> _entity)
{
	// 방어 코드: 엔티티에 텍스처나 텍스트 컴포넌트가 없으면 함수 실행 안함
	if (!_entity->HasComponent<Texture2D>())
	{
		return;
	}
	// 컴포넌트 제거
	_entity->RemoveComponent<Texture2D>();
}

void UIManager::Erase_Textures(std::string _textureName)
{
	mpGraphicsEngine->Erase_Textures(_textureName);
}

void UIManager::SetMaxNumLayer(int _num)
{
	maxNumLayer = _num;
}

void UIManager::AddButtonPressUI(std::shared_ptr<Entity> _entity, std::string _UIImange, Vector2 _pressedPosition, Vector2 _pressedSize)
{
	if (_entity->HasComponent<Texture2D>())
	{
		auto& originalTexture = _entity->GetComponent<Texture2D>();

		if (!_entity->HasComponent<Button>()) // 버튼이 없으면 추가한다.
		{
			_entity->AddComponent<Button>();
		}
		auto& buttonComp = _entity->GetComponent<Button>();
		buttonComp.mUIPosition = originalTexture.m_position;
		buttonComp.mUISize = originalTexture.m_size;
		buttonComp.mNormalUIName = originalTexture.m_file;
		buttonComp.mPressedUIName = _UIImange;
		buttonComp.mUIPressedSize = _pressedSize;
		buttonComp.mUIPressedPosition = _pressedPosition;
		buttonComp.mPressedRGBA = Vector4(1.0f);
	}
	else
	{
		std::cout << "버튼 추가 실패" << std::endl;
		return;
	}
}

void UIManager::AddButtonPressText(std::shared_ptr<Entity> _entity, std::string _font, std::u8string _text, Vector2 _pressedPosition /*= Vector2()*/)
{
	if (_entity->HasComponent<Text>())
	{
		auto& originalText = _entity->GetComponent<Text>();

		if (!_entity->HasComponent<Button>()) // 버튼이 없으면 추가한다.
		{
			_entity->AddComponent<Button>();
		}
		auto& buttonComp = _entity->GetComponent<Button>();
		buttonComp.mTextPosition = originalText.m_position;
		buttonComp.mNormalTextFont = originalText.m_font;
		buttonComp.mNormalText = originalText.m_text;
		buttonComp.mPressedTextFont = _font;
		buttonComp.mPressedText = _text;
		buttonComp.mTextPressedPosition = _pressedPosition;
	}
}

void UIManager::AddButtonPressColor(std::shared_ptr<Entity> _entity, Vector4 _PressedRGBA)
{
	if (_entity->HasComponent<Texture2D>())
	{
		auto& originalTexture = _entity->GetComponent<Texture2D>();

		if (!_entity->HasComponent<Button>()) // 버튼이 없으면 추가한다.
		{
			_entity->AddComponent<Button>();
		}
		auto& buttonComp = _entity->GetComponent<Button>();
		buttonComp.mUIPosition = originalTexture.m_position;
		buttonComp.mUISize = originalTexture.m_size;
		buttonComp.mNormalRGBA = originalTexture.m_rgba;
		buttonComp.mUIPressedPosition = originalTexture.m_position;
		buttonComp.mUIPressedSize = originalTexture.m_size;
		buttonComp.mPressedRGBA = _PressedRGBA;
	}
	else
	{
		std::cout << "버튼 추가 실패" << std::endl;
		return;
	}
}

void UIManager::AddButtonHoveredUI(std::shared_ptr<Entity> _entity, std::string _UIImange, Vector2 _hoveringPosition, Vector2 _hoveringSize)
{
	if (_entity->HasComponent<Texture2D>())
	{
		auto& originalTexture = _entity->GetComponent<Texture2D>();

		if (!_entity->HasComponent<Button>()) // 버튼이 없으면 추가한다.
		{
			_entity->AddComponent<Button>();
		};
		auto& buttonComp = _entity->GetComponent<Button>();
		buttonComp.mUIPosition = originalTexture.m_position;
		buttonComp.mUISize = originalTexture.m_size;
		buttonComp.mNormalUIName = originalTexture.m_file;
		buttonComp.mHoveredUIName = _UIImange;
		buttonComp.mUIHoveringSize = _hoveringSize;
		buttonComp.mUIHoveringPosition = _hoveringPosition;
		buttonComp.mHoveredRGBA = Vector4(1.0f);
	}
	else
	{
		std::cout << "버튼 추가 실패" << std::endl;
		return;
	}
}

void UIManager::AddButtonHoveredText(std::shared_ptr<Entity> _entity, std::string _font, std::u8string _text, Vector2 _hoveringPosition /*= Vector2()*/)
{
	if (_entity->HasComponent<Text>())
	{
		auto& originalText = _entity->GetComponent<Text>();

		if (!_entity->HasComponent<Button>()) // 버튼이 없으면 추가한다.
		{
			_entity->AddComponent<Button>();
		}
		auto& buttonComp = _entity->GetComponent<Button>();
		buttonComp.mTextPosition = originalText.m_position;
		buttonComp.mNormalTextFont = originalText.m_font;
		buttonComp.mNormalText = originalText.m_text;
		buttonComp.mHoveredTextFont = _font;
		buttonComp.mHoveredText = _text;
		buttonComp.mTextHoveringPosition = _hoveringPosition;
	}
}

void UIManager::AddButtonHoveredColor(std::shared_ptr<Entity> _entity, Vector4 _HoveredRGBA)
{
	if (_entity->HasComponent<Texture2D>())
	{
		auto& originalTexture = _entity->GetComponent<Texture2D>();

		if (!_entity->HasComponent<Button>()) // 버튼이 없으면 추가한다.
		{
			_entity->AddComponent<Button>();
		}
		auto& buttonComp = _entity->GetComponent<Button>();
		buttonComp.mUIPosition = originalTexture.m_position;
		buttonComp.mUISize = originalTexture.m_size;
		buttonComp.mNormalRGBA = originalTexture.m_rgba;
		buttonComp.mUIHoveringSize = originalTexture.m_size;
		buttonComp.mUIHoveringPosition = originalTexture.m_position;
		buttonComp.mHoveredRGBA = _HoveredRGBA;
	}
	else
	{
		std::cout << "버튼 추가 실패" << std::endl;
		return;
	}
}

void UIManager::AddButtonAllColor(std::shared_ptr<Entity> _entity, Vector4 _PressedRGBA, Vector4 _HoveredRGBA)
{
	if (_entity->HasComponent<Texture2D>())
	{
		auto& originalTexture = _entity->GetComponent<Texture2D>();

		if (!_entity->HasComponent<Button>()) // 버튼이 없으면 추가한다.
		{
			_entity->AddComponent<Button>();
		}
		auto& buttonComp = _entity->GetComponent<Button>();
		buttonComp.mUIPosition = originalTexture.m_position;
		buttonComp.mUISize = originalTexture.m_size;
		buttonComp.mNormalRGBA = originalTexture.m_rgba;
		buttonComp.mUIPressedPosition = originalTexture.m_position;
		buttonComp.mUIPressedSize = originalTexture.m_size;
		buttonComp.mPressedRGBA = _PressedRGBA;
		buttonComp.mUIHoveringSize = originalTexture.m_size;
		buttonComp.mUIHoveringPosition = originalTexture.m_position;
		buttonComp.mHoveredRGBA = _HoveredRGBA;
	}
	else
	{
		std::cout << "버튼 추가 실패" << std::endl;
		return;
	}
}

void UIManager::AddButtonAllUI(std::shared_ptr<Entity> _entity, std::string _pressedUIImange, std::string _hoveringUIImange, Vector2 _pressedPosition, Vector2 _pressedSize, Vector2 _hoveringPosition, Vector2 _hoveringSize)
{
	if (_entity->HasComponent<Texture2D>())
	{
		auto& originalTexture = _entity->GetComponent<Texture2D>();

		if (!_entity->HasComponent<Button>()) // 버튼이 없으면 추가한다.
		{
			_entity->AddComponent<Button>();
		}
		auto& buttonComp = _entity->GetComponent<Button>();
		buttonComp.mUIPosition = originalTexture.m_position;
		buttonComp.mUISize = originalTexture.m_size;
		buttonComp.mNormalUIName = originalTexture.m_file;
		buttonComp.mPressedUIName = _pressedUIImange;
		buttonComp.mUIPressedSize = _pressedSize;
		buttonComp.mUIPressedPosition = _pressedPosition;
		buttonComp.mPressedRGBA = Vector4(1.0f);
		buttonComp.mHoveredUIName = _hoveringUIImange;
		buttonComp.mUIHoveringSize = _hoveringSize;
		buttonComp.mUIHoveringPosition = _hoveringPosition;
		buttonComp.mHoveredRGBA = Vector4(1.0f);
	}
	else
	{
		std::cout << "버튼 추가 실패" << std::endl;
		return;
	}
}

ButtonState UIManager::GetButtonState(std::shared_ptr<Entity> _entity)
{
	return _entity->GetComponent<Button>().mButtonState;
}

void UIManager::RemoveButton(std::shared_ptr<Entity> _entity)
{
	if (_entity->HasComponent<Button>())
	{
		return;
	}
	// 컴포넌트 제거
	_entity->RemoveComponent<Button>();
}

void UIManager::AddRect(std::shared_ptr<Entity> _entity, Vector2 _posXY, Vector2 _sizeWH, int _layer, Vector4 _rgba, float _thickness /*= 0.0f*/, Vector4 _BorderRgba /*= Vector4()*/)
{
	_entity->AddComponent<Box2D>(_posXY, _sizeWH, _layer, _rgba, _thickness, _BorderRgba, true, false);
	maxNumLayer = std::max(maxNumLayer, _layer);  // 최대 레이어 번호 갱신
}

void UIManager::RemoveRect(std::shared_ptr<Entity> _entity)
{
	// 방어 코드: 엔티티에 텍스처나 텍스트 컴포넌트가 없으면 함수 실행 안함
	if (!_entity->HasComponent<Box2D>())
	{
		return;
	}
	// 컴포넌트 제거
	_entity->RemoveComponent<Box2D>();
}

void UIManager::AddTile(std::shared_ptr<Entity> _entity, Vector2 _posXY, Vector2 _sizeWH, int _layer, Vector4 _rgba, float _thickness /*= 0.0f*/, Vector4 _BorderRgba /*= Vector4()*/)
{
	_entity->AddComponent<Tile>(_posXY, _sizeWH, _layer, _rgba, _thickness, _BorderRgba, true, false);
	maxNumLayer = std::max(maxNumLayer, _layer);  // 최대 레이어 번호 갱신
}

void UIManager::AddTileMap(std::shared_ptr<Entity> _entity, Vector2 _center, int _col, int _row)
{
	if (_entity->HasComponent<Tile>())
	{
		auto& tile = _entity->GetComponent<Tile>();
	}
	else
	{
		std::cout << "There is no Tile." << std::endl;
		return;
	}
}

void UIManager::AddHPBar(std::shared_ptr<Entity> _entity, Vector2 _posXY, Vector2 _sizeWH, int _layer, Vector4 _backgroundRgba, Vector4 _foregroundColor, float _healthPercentage /*= 1.0f*/)
{
	_entity->AddComponent<HealthBarComponenet>(_posXY, Vector3(), _sizeWH, _layer, _backgroundRgba, _foregroundColor, _healthPercentage, true, false);
	maxNumLayer = std::max(maxNumLayer, _layer);  // 최대 레이어 번호 갱신
}

void UIManager::Add3DHPBar(std::shared_ptr<Entity> _entity, Vector3 _offset, Vector2 _sizeWH,
	int _layer, Vector4 _backgroundRgba, Vector4 _foregroundColor,
	float _healthPercentage /*= 1.0f*/)
{
	_entity->AddComponent<HealthBarComponenet>(Vector2(), _offset, _sizeWH, _layer, _backgroundRgba, _foregroundColor, _healthPercentage, true, true);
	maxNumLayer = std::max(maxNumLayer, _layer);  // 최대 레이어 번호 갱신
}

void UIManager::RemoveHPBar(std::shared_ptr<Entity> _entity)
{
	// 방어 코드: 엔티티에 텍스처나 텍스트 컴포넌트가 없으면 함수 실행 안함
	if (!_entity->HasComponent<HealthBarComponenet>())
	{
		return;
	}
	// 컴포넌트 제거
	_entity->RemoveComponent<HealthBarComponenet>();
}

void UIManager::AddMessageBox2D(std::shared_ptr<Entity> _entity, std::string _imgFileName, Vector2 _imgPos, Vector2 _size, Vector4 _imgRgba
	, std::u8string _text, Vector2 _textPos, float _textScale, std::string _fontName, Vector4 _textRgba, int _layer /*= 0*/, bool _isVisible /*= true*/)
{
	auto& message2DComp = _entity->AddComponent<MessageBox2D>();
	message2DComp.mFile = _imgFileName;
	message2DComp.mTexturePosition = _imgPos;
	message2DComp.mSize = _size;
	message2DComp.mImgRgba = _imgRgba;

	message2DComp.mText = _text;
	message2DComp.mTextPosition = _textPos;
	message2DComp.mScale = _textScale;
	message2DComp.mFont = _fontName;
	message2DComp.mTextRgba = _textRgba;

	message2DComp.mLayer = _layer;
	message2DComp.mIsImgVisible = _isVisible;
	message2DComp.mIsTextVisible = _isVisible;

	maxNumLayer = std::max(maxNumLayer, _layer);  // 최대 레이어 번호 갱신
}

void UIManager::AddMessageBox3D(std::shared_ptr<Entity> _entity, std::string _imgFileName, Vector2 _imgOffset, Vector2 _size, Vector4 _imgRgba
	, std::u8string _text, Vector2 _textOffset, float _textScale, std::string _fontName, Vector4 _textRgba, Vector3 _worldPos, Vector2 _worldPosOffset, int _layer, bool _isVisible)
{
	auto& message3DComp = _entity->AddComponent<MessageBox3D>();
	message3DComp.mFile = _imgFileName;
	message3DComp.mImgOffset = _imgOffset;
	message3DComp.mSize = _size;
	message3DComp.mImgRgba = _imgRgba;

	message3DComp.mText = _text;
	message3DComp.mTextOffset = _textOffset;
	message3DComp.mScale = _textScale;
	message3DComp.mFont = _fontName;
	message3DComp.mTextRgba = _textRgba;

	// NDC로 바꿀 거기 때문에 x는 그대로지만, y는 역방향이어야 한다.
	message3DComp.mWorldPosition = _worldPos + Vector2(_worldPosOffset.x, -_worldPosOffset.y);
	message3DComp.mLayer = _layer;
	message3DComp.mIsImgVisible = _isVisible;
	message3DComp.mIsTextVisible = _isVisible;

	maxNumLayer = std::max(maxNumLayer, _layer);  // 최대 레이어 번호 갱신
}

void UIManager::RemoveMessageBox2D(std::shared_ptr<Entity> _entity)
{
	// 방어 코드: 엔티티에 텍스처나 텍스트 컴포넌트가 없으면 함수 실행 안함
	if (!_entity->HasComponent<MessageBox2D>())
	{
		return;
	}
	// 컴포넌트 제거
	_entity->RemoveComponent<MessageBox2D>();
}

void UIManager::RemoveMessageBox3D(std::shared_ptr<Entity> _entity)
{
	// 방어 코드: 엔티티에 텍스처나 텍스트 컴포넌트가 없으면 함수 실행 안함
	if (!_entity->HasComponent<MessageBox3D>())
	{
		return;
	}
	// 컴포넌트 제거
	_entity->RemoveComponent<MessageBox3D>();
}

void UIManager::SetRenderDebugInfo(bool _fps, bool _camera)
{
	m_rendFPS = _fps;
	m_rendCameraInfo = _camera;
}

/// UI 매니저 내에서만 사용--------------------------------------------------------------------------------------------------------

void UIManager::UpdateViewNProjection(Camera* camera)
{
	/// 카메라가 위치 바뀔 경우에 대해 새롭게 연산
	if (camera->GetViewRow() != mViewMatrix || camera->GetProjRow() != mProjMatrix)
	{
		mViewMatrix = camera->GetViewRow();
		mProjMatrix = camera->GetProjRow();
	}
}

Vector2 UIManager::ConvertWorldtoNDC(Vector3 _worldPos, const Matrix& viewMatrix, const Matrix& projMatrix)
{
	// 월드 좌표를 4D 벡터로 변환 (homogeneous 좌표계에서 w = 1.0)
	Vector4 position = Vector4(_worldPos.x, _worldPos.y, _worldPos.z, 1.0f);

	// 뷰 행렬과 프로젝션 행렬을 곱한 행렬 계산
	Matrix viewProj = viewMatrix * projMatrix;

	// 월드 좌표 -> 클립 공간으로 변환
	Vector4 clipSpacePosition = Vector4::Transform(position, viewProj);

	// Homogeneous Divide (w로 나누기)
	if (clipSpacePosition.w != 0.0f)
	{
		clipSpacePosition.x /= clipSpacePosition.w;
		clipSpacePosition.y /= clipSpacePosition.w;
		clipSpacePosition.z /= clipSpacePosition.w;
	}

	Vector2 NDCpos = { clipSpacePosition.x , clipSpacePosition.y };
	return NDCpos;
}

Vector2 UIManager::ConvertNDCtoScreeen(Vector2 _NDCPos)
{
	Vector2 screeenPosition = {};
	screeenPosition.x = (_NDCPos.x + 1.0f) * 0.5f * mpGraphicsEngine->GetScreenWidth();
	screeenPosition.y = (1.0f - _NDCPos.y) * 0.5f * mpGraphicsEngine->GetScreenHeight();

	return screeenPosition;
}

void UIManager::UpdateButton(Texture2D* _pTexture2D, Button* _pButton, Text* _pText)
{
	// 버튼의 상태에 따라 텍스처와 텍스트를 업데이트 : 텍스트는 없을 수도 있기에 있는지도 반드시 확인한다.
	switch (_pButton->mButtonState)
	{
	case ButtonState::NORMAL:
		if (_pButton->mNormalUIName != "") // 노멀 텍스처가 없다면 하지 않는다.
		{
			// if문은 복사 방지임 : file과 위치가 현재와 같지 않다면 업데이트 해준다.
			if (_pTexture2D->m_file != _pButton->mNormalUIName) _pTexture2D->m_file = _pButton->mNormalUIName;
			if (_pTexture2D->m_position != _pButton->mUIPosition) _pTexture2D->m_position = _pButton->mUIPosition;
			if (_pTexture2D->m_size != _pButton->mUISize) _pTexture2D->m_size = _pButton->mUISize;
		}

		if (_pTexture2D->m_rgba != _pButton->mNormalRGBA) // 색상이 현재와 같지 않다면 업데이트 해준다.
		{
			_pTexture2D->m_rgba = _pButton->mNormalRGBA;
		}

		if (_pText && _pText->m_font != "" && _pText->m_text != u8""
			&& _pButton->mNormalTextFont != "" && _pButton->mNormalText != u8"") // 폰트나 텍스트가 없으면 진행하지 않는다.
		{
			// if문은 복사 방지임 : 폰트, 텍스트, 위치가 현재와 같지 않다면 업데이트 해준다.
			if (_pText->m_font != _pButton->mNormalTextFont) _pText->m_font = _pButton->mNormalTextFont;
			if (_pText->m_text != _pButton->mNormalText) _pText->m_text = _pButton->mNormalText;
			if (_pText->m_position != _pButton->mTextPosition) _pText->m_position = _pButton->mTextPosition;
		}
		break;
	case ButtonState::HOVERED:
		if (_pButton->mHoveredUIName != "") // 호버된 텍스처가 없다면 하지 않는다.
		{
			// if문은 복사 방지임 : file과 위치가 현재와 같지 않다면 업데이트 해준다.
			if (_pTexture2D->m_file != _pButton->mHoveredUIName) _pTexture2D->m_file = _pButton->mHoveredUIName;
			if (_pTexture2D->m_position != _pButton->mUIHoveringPosition) _pTexture2D->m_position = _pButton->mUIHoveringPosition;
			if (_pTexture2D->m_size != _pButton->mUIHoveringSize) _pTexture2D->m_size = _pButton->mUIHoveringSize;
		}

		if (_pTexture2D->m_rgba != _pButton->mHoveredRGBA) // 색상이 현재와 같지 않다면 업데이트 해준다.
		{
			_pTexture2D->m_rgba = _pButton->mHoveredRGBA;
		}

		if (_pText && _pText->m_font != "" && _pText->m_text != u8""
			&& _pButton->mHoveredTextFont != "" && _pButton->mHoveredText != u8"")
		{
			// if문은 복사 방지임 : 폰트, 텍스트, 위치가 현재와 같지 않다면 업데이트 해준다.
			if (_pText->m_font != _pButton->mHoveredTextFont) _pText->m_font = _pButton->mHoveredTextFont;
			if (_pText->m_text != _pButton->mHoveredText) _pText->m_text = _pButton->mHoveredText;
			if (_pText->m_position != _pButton->mTextHoveringPosition) _pText->m_position = _pButton->mTextHoveringPosition;
		}
		break;
	case ButtonState::PRESSED:

		if (_pButton->mPressedUIName != "") // 프레스 상태의 텍스처가 없다면 하지 않는다.
		{
			// if문은 복사 방지임 : file과 위치가 현재와 같지 않다면 업데이트 해준다.
			if (_pTexture2D->m_file != _pButton->mPressedUIName) _pTexture2D->m_file = _pButton->mPressedUIName;
		}

		// 색만 바뀌어도 움직여야 하기 때문에 텍스처가 없어도 되어야 한다.
		if (_pButton->mCanDrag && _pButton->mIsDrag)
		{
			// 드래그 상태일 때는 델타 포스를 더해준 후 리셋해야 다음 프레임의 델타 포스에 영향을 미치지 않는다.
			Vector2 deltaPos = mpInputManager->GetDeltaMousePos();
			_pButton->mUIPosition += deltaPos;
			deltaPos = Vector2();
		}
		if (_pTexture2D->m_position != _pButton->mUIPressedPosition) _pTexture2D->m_position = _pButton->mUIPressedPosition;
		if (_pTexture2D->m_size != _pButton->mUIPressedSize) _pTexture2D->m_size = _pButton->mUIPressedSize;

		if (_pTexture2D->m_rgba != _pButton->mPressedRGBA) // 색상이 현재와 같지 않다면 업데이트 해준다.
		{
			_pTexture2D->m_rgba = _pButton->mPressedRGBA;
		}

		if (_pText && _pText->m_font != "" && _pText->m_text != u8""
			&& _pButton->mPressedTextFont != "" && _pButton->mPressedText != u8"")
		{
			// if문은 복사 방지임 : 폰트, 텍스트, 위치가 현재와 같지 않다면 업데이트 해준다.
			if (_pText->m_font != _pButton->mPressedTextFont) _pText->m_font = _pButton->mPressedTextFont;
			if (_pText->m_text != _pButton->mPressedText) _pText->m_text = _pButton->mPressedText;
			// 드래그 상태일 때는 델타 포스를 더해준 후 리셋해야 다음 프레임의 델타 포스에 영향을 미치지 않는다.
			if (_pButton->mCanDrag && _pButton->mIsDrag)
			{
				Vector2 deltaPos = mpInputManager->GetDeltaMousePos();
				_pButton->mTextPosition += deltaPos;
				deltaPos = Vector2();
			}
			if (_pText->m_position != _pButton->mTextPressedPosition) _pText->m_position = _pButton->mTextPressedPosition;
		}
		break;
	}
}

void UIManager::CheckButtonState(Button* _pButton, bool _isHovered)
{
	if (_isHovered) // 호버링이 됐다면 상태를 각 상황에 맞게 바꿔준다.
	{
		if (mpInputManager->GetKeyDown(KEY::LBUTTON)) // TAP
		{
			_pButton->mButtonState = ButtonState::PRESSED;
			_pButton->mCanDrag = false;
		}
		else if (mpInputManager->GetKey(KEY::LBUTTON)) // HOLD
		{
			_pButton->mCanDrag = true;// 드래그는 눌린 상태 중에서도 Hold에서만 활성화된다.
		}
		else if (mpInputManager->GetKeyUp(KEY::LBUTTON) && _pButton->mButtonState == ButtonState::PRESSED) // AWAY
		{
			if (_isHovered)
			{
				_pButton->mButtonState = ButtonState::HOVERED;
			}
			else
			{
				_pButton->mButtonState = ButtonState::NORMAL;
			}
			_pButton->mCanDrag = false;
			/*if (_pButton->mOnClick)
			{
				_pButton->mOnClick();
				int nextOrder = _pButton->mOrder + 1;
				for (auto& nextEntity : textureView)
				{
					Button* newButton = mRegistry.try_get<Button>(nextEntity);
					if (newButton && newButton->mOrder == nextOrder)
					{
						newButton->mIsEnable = true;
						break;
					}
				}
			}*/
		}
		else // 호버링 상태
		{
			_pButton->mButtonState = ButtonState::HOVERED;
			_pButton->mCanDrag = false;
		}
	}
	else // 노멀 상태
	{
		_pButton->mButtonState = ButtonState::NORMAL;
		_pButton->mCanDrag = false;
	}
}

void UIManager::RenderUI(int _layerOrder)
{
	auto imgView = mRegistry.view<Texture2D>();
	auto mgs2DView = mRegistry.view<MessageBox2D>();
	auto mgs3DView = mRegistry.view<MessageBox3D>();
	/// texture 먼저 그림 (순서 변경 금지 -> 텍스처보다 텍스트가 위로 올라와야 함)
	// 이미지를 그리기 시작할 것임을 선언
	mpGraphicsEngine->UIDrawImageStart();

	for (auto& entity : mgs3DView)
	{
		auto& msg3DComp = mRegistry.get<MessageBox3D>(entity);
		// 컴포넌트의 레이어 값이 현재의 i 값과 일치하고, 그릴 리소스인지 확인하고 그린다.

		if (msg3DComp.mLayer == _layerOrder && msg3DComp.mIsImgVisible == true)
		{
			Vector2 imgRenderPos = -Vector2(msg3DComp.mSize.x / 2, 0) + ConvertNDCtoScreeen(ConvertWorldtoNDC(msg3DComp.mWorldPosition, mViewMatrix, mProjMatrix)) + msg3DComp.mImgOffset;
			mpGraphicsEngine->UIDrawImage(imgRenderPos, msg3DComp.mSize, msg3DComp.mFile, msg3DComp.mImgRgba);
		}
	}

	for (auto& entity : mgs2DView)
	{
		auto& msgComp = mRegistry.get<MessageBox2D>(entity);
		// 컴포넌트의 레이어 값이 현재의 i 값과 일치하고, 그릴 리소스인지 확인하고 그린다.

		if (msgComp.mLayer == _layerOrder && msgComp.mIsImgVisible == true)
		{
			mpGraphicsEngine->UIDrawImage(msgComp.mTexturePosition, msgComp.mSize, msgComp.mFile, msgComp.mImgRgba);
		}
	}


	// 해당 레이어의 텍스처를 렌더링
	for (auto& entity : imgView)
	{
		auto& texture = mRegistry.get<Texture2D>(entity);
		// 컴포넌트의 레이어 값이 현재의 i 값과 일치하고, 그릴 리소스인지 확인하고 그린다.

		if (texture.m_layer == _layerOrder && texture.m_isVisible == true)
		{
			mpGraphicsEngine->UIDrawImage(texture.m_position, texture.m_size, texture.m_file, texture.m_rgba);
		}
	}

	// 이미지를 더 이상 그리지 않음
	mpGraphicsEngine->UIDrawImageFin();
}

void UIManager::RenderText(int _layerOrder)
{
	auto textView = mRegistry.view<Text>();
	auto mgs2DView = mRegistry.view<MessageBox2D>();
	auto mgs3DView = mRegistry.view<MessageBox3D>();

	for (auto& entity : mgs3DView)
	{
		auto& msg3DComp = mRegistry.get<MessageBox3D>(entity);
		// 새로운 폰트를 사용할 것임을 선언
		mpGraphicsEngine->UIStartFontID(msg3DComp.mFont);
		// 폰트 크기 재설정
		ImGui::SetWindowFontScale(msg3DComp.mScale);
		// 컴포넌트의 레이어 값이 현재의 i 값과 일치하고, 그릴 리소스인지 확인하고 그린다.
		if (msg3DComp.mLayer == _layerOrder && msg3DComp.mIsTextVisible == true)
		{
			Vector2 textRenderPos = -Vector2(msg3DComp.mSize.x / 2, 0) + ConvertNDCtoScreeen(ConvertWorldtoNDC(msg3DComp.mWorldPosition, mViewMatrix, mProjMatrix)) + msg3DComp.mTextOffset;
			switch (msg3DComp.mIdx)
			{
			case TEXT_IDX::NONE:
				mpGraphicsEngine->UIDrawText(textRenderPos, msg3DComp.mText, msg3DComp.mTextRgba);
				break;
			case TEXT_IDX::INT1:
				mpGraphicsEngine->UIDrawTextWithNum(textRenderPos, msg3DComp.mText, msg3DComp.mTextRgba, msg3DComp.mNum1);
				break;
			default:
				break;
			}
		}
		// 이전 폰트를 더 사용하지 않을 것임을 선언
		mpGraphicsEngine->UIFinFontID();
	}

	for (auto& entity : mgs2DView)
	{
		auto& msgComp = mRegistry.get<MessageBox2D>(entity);
		// 새로운 폰트를 사용할 것임을 선언
		mpGraphicsEngine->UIStartFontID(msgComp.mFont);
		// 폰트 크기 재설정
		ImGui::SetWindowFontScale(msgComp.mScale);
		// 컴포넌트의 레이어 값이 현재의 i 값과 일치하고, 그릴 리소스인지 확인하고 그린다.
		if (msgComp.mLayer == _layerOrder && msgComp.mIsTextVisible == true)
		{
			switch (msgComp.mIdx)
			{
			case TEXT_IDX::NONE:
				mpGraphicsEngine->UIDrawText(msgComp.mTextPosition, msgComp.mText, msgComp.mTextRgba);
				break;
			case TEXT_IDX::INT1:
				mpGraphicsEngine->UIDrawTextWithNum(msgComp.mTextPosition, msgComp.mText, msgComp.mTextRgba, msgComp.mNum1);
				break;
			default:
				break;
			}
		}
		// 이전 폰트를 더 사용하지 않을 것임을 선언
		mpGraphicsEngine->UIFinFontID();
	}


	/// 해당 레이어의 텍스트를 렌더링
	for (auto& entity : textView)
	{
		auto& text = mRegistry.get<Text>(entity);
		// 새로운 폰트를 사용할 것임을 선언
		mpGraphicsEngine->UIStartFontID(text.m_font);
		// 폰트 크기 재설정
		ImGui::SetWindowFontScale(text.m_scale);
		if (text.m_layer == _layerOrder && text.m_isVisible == true)
		{
			if (text.m_hasDepth == false) // 3D가 아님
			{
				switch (text.m_idx)
				{
				case TEXT_IDX::NONE:
					mpGraphicsEngine->UIDrawText(text.m_position, text.m_text, text.m_rgba);
					break;
				case TEXT_IDX::INT1:
					mpGraphicsEngine->UIDrawTextWithNum(text.m_position, text.m_text, text.m_rgba, text.m_num1);
					break;
				case TEXT_IDX::INT2:
					mpGraphicsEngine->UIDrawTextWithNum(text.m_position, text.m_text, text.m_rgba, text.m_num1, text.m_num2);
					break;
				case TEXT_IDX::FLOAT1:
					mpGraphicsEngine->UIDrawTextWithNum(text.m_position, text.m_text, text.m_rgba, text.m_num3);
					break;
				case TEXT_IDX::FLOAT2:
					mpGraphicsEngine->UIDrawTextWithNum(text.m_position, text.m_text, text.m_rgba, text.m_num3, text.m_num4);
					break;
				default:
					break;
				}
			}
			else // 3D임
			{
				Vector2 renderPosition = ConvertNDCtoScreeen(ConvertWorldtoNDC(text.m_worldPosition + text.m_offset, mViewMatrix, mProjMatrix));
				switch (text.m_idx)
				{
				case TEXT_IDX::NONE:
					mpGraphicsEngine->UIDrawText(renderPosition, text.m_text, text.m_rgba);
					break;
				case TEXT_IDX::INT1:
					mpGraphicsEngine->UIDrawTextWithNum(renderPosition, text.m_text, text.m_rgba, text.m_num1);
					break;
				case TEXT_IDX::INT2:
					mpGraphicsEngine->UIDrawTextWithNum(renderPosition, text.m_text, text.m_rgba, text.m_num1, text.m_num2);
					break;
				case TEXT_IDX::FLOAT1:
					mpGraphicsEngine->UIDrawTextWithNum(renderPosition, text.m_text, text.m_rgba, text.m_num3);
					break;
				case TEXT_IDX::FLOAT2:
					mpGraphicsEngine->UIDrawTextWithNum(renderPosition, text.m_text, text.m_rgba, text.m_num3, text.m_num4);
					break;
				default:
					break;
				}

			}
		}
		// 이전 폰트를 더 사용하지 않을 것임을 선언
		mpGraphicsEngine->UIFinFontID();
	}
}

void UIManager::RenderRectFilled(int _layerOrder)
{
	auto view = mRegistry.view<Box2D>();
	/// 해당 레이어의 텍스트를 렌더링
	for (auto& entity : view)
	{
		auto& box2D = mRegistry.get<Box2D>(entity);
		// 새로운 폰트를 사용할 것임을 선언
		if (box2D.mLayer == _layerOrder && box2D.mIsVisible == true)
		{
			if (box2D.mThickness == 0.0f) // 두께가 없다.
			{
				mpGraphicsEngine->UIDrawRectFilled(box2D.mPosition, box2D.mSize, box2D.mRgba);
			}
			else if (box2D.mRgba.z == 0.0f) // 두께가 있고 색상이 없다.
			{
				mpGraphicsEngine->UIDrawRect(box2D.mPosition, box2D.mSize, box2D.mBorderRgba, 0.0f, box2D.mThickness);
			}
			else  // 두께가 있고, 색상이 있고, 테두리 색이 있다.
			{
				mpGraphicsEngine->UIDrawRectwithBorder(box2D.mPosition, box2D.mSize, box2D.mRgba, 0.0f, box2D.mThickness/*, box2D->mBorderRgba*/);
			}
		}
	}
}

void UIManager::RenderHealthBar(int _layerOrder)
{
	auto view = mRegistry.view<HealthBarComponenet>();
	/// 해당 레이어의 텍스트를 렌더링
	for (auto& entity : view)
	{
		auto& healthBar = mRegistry.get<HealthBarComponenet>(entity);
		if (healthBar.mLayer == _layerOrder && healthBar.mIsVisible == true)
		{
			Vector2 renderPosition = healthBar.mPosition;

			if (healthBar.mHasDepth == true)
			{
				auto& playerTransform = mRegistry.get<Transform>(entity);
				renderPosition = -(healthBar.mSize / 2) + ConvertNDCtoScreeen(ConvertWorldtoNDC(playerTransform.m_localPosition + healthBar.mOffSet, mViewMatrix, mProjMatrix));
			}
			// 배경 그리기
			mpGraphicsEngine->UIDrawRectFilled(renderPosition, healthBar.mSize, healthBar.mBackgroundColor);
			// 체력 바 그리기
			healthBar.mHealthPercentage = std::clamp(healthBar.mHealthPercentage, 0.0f, 1.0f);
			mpGraphicsEngine->UIDrawRectFilled(renderPosition, { healthBar.mSize.x * healthBar.mHealthPercentage, healthBar.mSize.y }, healthBar.mForegroundColor);

		}
	}
}

void UIManager::RenderTile(int _layerOrder)
{
	auto view = mRegistry.view<Tile>();
	/// 해당 레이어의 텍스트를 렌더링
	for (auto& entity : view)
	{
		auto& tile = mRegistry.get<Tile>(entity);
		if (tile.mLayer == _layerOrder && tile.mIsVisible == true)
		{
			for (int i = -30; i <= 30; i++)
			{
				for (int j = -30; j <= 30; j++)
				{
					Vector2 halfSize = tile.mSize / 2;

					Vector2 pos1 = { tile.mPosition.x - halfSize.x, tile.mPosition.y + halfSize.y };
					Vector2 pos2 = tile.mPosition + halfSize;
					Vector2 pos3 = { tile.mPosition.x + halfSize.x, tile.mPosition.y - halfSize.y };
					Vector2 pos4 = tile.mPosition - halfSize;
					pos1 = ConvertNDCtoScreeen(ConvertWorldtoNDC(Vector3(pos1.x + i, 0, pos1.y + j), mViewMatrix, mProjMatrix));
					pos2 = ConvertNDCtoScreeen(ConvertWorldtoNDC(Vector3(pos2.x + i, 0, pos2.y + j), mViewMatrix, mProjMatrix));
					pos3 = ConvertNDCtoScreeen(ConvertWorldtoNDC(Vector3(pos3.x + i, 0, pos3.y + j), mViewMatrix, mProjMatrix));
					pos4 = ConvertNDCtoScreeen(ConvertWorldtoNDC(Vector3(pos4.x + i, 0, pos4.y + j), mViewMatrix, mProjMatrix));

					Vector4 tempRgBa = Vector4(1, 0, 0, 0.2f);
					if (i % 2 == 0)
					{
						if (j % 2 == 0)
						{
							tempRgBa = tile.mRgba; // green
						}
						else
						{
							tempRgBa = Vector4(1, 0, 0, 0.2f); //red
						}
					}
					else
					{
						if (j % 2 == 0)
						{
							tempRgBa = Vector4(1, 0, 0, 0.2f); //red
						}
						else
						{
							tempRgBa = tile.mRgba; // green
						}
					}

					if (tile.mThickness == 0.0f) // 두께가 없다.
					{
						mpGraphicsEngine->UIFreeRectFilled(pos1, pos2, pos3, pos4, tempRgBa);
					}
					else if (tile.mRgba.z == 0.0f) // 두께가 있고 색상이 없다.
					{
						mpGraphicsEngine->UIFreeRect(pos1, pos2, pos3, pos4, tile.mBorderRgba, tile.mThickness);
					}
					else  // 두께가 있고, 색상이 있고, 테두리 색이 있다.
					{
						mpGraphicsEngine->UIFreeRectwithBorder(pos1, pos2, pos3, pos4, tile.mRgba, tile.mThickness, tile.mBorderRgba);
					}
				}
			}
		}
	}
}

void UIManager::UpdateMessageBox(int _layerOrder)
{
	//auto view = mRegistry.view<MessageBox3D>();
	///// 해당 레이어의 텍스트를 렌더링
	//for (auto& entity : view)
	//{
	//	auto& messageBox3D = mRegistry.get<MessageBox3D>(entity);
	//	if (messageBox3D.mLayer == _layerOrder && (messageBox3D.mIsImgVisible == true || messageBox3D.mIsTextVisible == true))
	//	{
	//		Vector2 imgRenderPos = ConvertNDCtoScreeen(ConvertWorldtoNDC(messageBox3D.mWorldPosition + messageBox3D.mImgOffset, mViewMatrix, mProjMatrix));
	//		Vector2 textRenderPos = ConvertNDCtoScreeen(ConvertWorldtoNDC(messageBox3D.mWorldPosition + messageBox3D.mTextOffset, mViewMatrix, mProjMatrix));

	//		// 배경 그리기
	//		mpGraphicsEngine->UIDrawRectFilled(renderPosition, messageBox3D.mSize, messageBox3D.mBackgroundColor);
	//		// 텍스트 그리기
	//		messageBox3D.mHealthPercentage = std::clamp(messageBox3D.mHealthPercentage, 0.0f, 1.0f);
	//		mpGraphicsEngine->UIDrawRectFilled(renderPosition, { messageBox3D.mSize.x * messageBox3D.mHealthPercentage, messageBox3D.mSize.y }, messageBox3D.mForegroundColor);

	//	}
	//}
}

void UIManager::RenderDebugInfo()
{
	if (m_rendFPS)
	{
		std::string fps = std::format("{:.2f}", mpInfo->GetFPS());
		std::u8string u8fps = std::u8string(fps.begin(), fps.end());
		mpGraphicsEngine->UIDrawText(Vector2(), u8fps);
		std::string dT = std::format("{:.4f}", 1.f / mpInfo->GetFPS());
		std::u8string u8dT = std::u8string(dT.begin(), dT.end());
		mpGraphicsEngine->UIDrawText(Vector2(0, 20), u8dT);

	}
	if (m_rendCameraInfo)
	{
		auto view = mRegistry.view<CameraComponent>();
		std::string camPos{};
		std::string camDir{};
		std::string lightCamPos{};
		std::string lightCamDir{};
		for (auto& entity : view)
		{
			auto& cameraComp = mRegistry.get<CameraComponent>(entity);
			if (cameraComp.m_cameraEnum == 0)
			{
				std::string camPosX = std::format("{:.2f}", cameraComp.m_pCamera->mViewPos.x);
				std::string camPosY = std::format("{:.2f}", cameraComp.m_pCamera->mViewPos.y);
				std::string camPosZ = std::format("{:.2f}", cameraComp.m_pCamera->mViewPos.z);
				std::string camDirX = std::format("{:.2f}", cameraComp.m_pCamera->mViewDir.x);
				std::string camDirY = std::format("{:.2f}", cameraComp.m_pCamera->mViewDir.y);
				std::string camDirZ = std::format("{:.2f}", cameraComp.m_pCamera->mViewDir.z);
				camPos = "WCameraPos: " + camPosX + "/" + camPosY + "/" + camPosZ;
				camDir = "WCameraDir: " + camDirX + "/" + camDirY + "/" + camDirZ;
			}

			if (cameraComp.m_cameraEnum == static_cast<int>(cameraEnum::LightCamera))
			{
				std::string lightCamPosX = std::format("{:.2f}", cameraComp.m_pCamera->mViewPos.x);
				std::string lightCamPosY = std::format("{:.2f}", cameraComp.m_pCamera->mViewPos.y);
				std::string lightCamPosZ = std::format("{:.2f}", cameraComp.m_pCamera->mViewPos.z);
				std::string lightCamDirX = std::format("{:.2f}", cameraComp.m_pCamera->mViewDir.x);
				std::string lightCamDirY = std::format("{:.2f}", cameraComp.m_pCamera->mViewDir.y);
				std::string lightCamDirZ = std::format("{:.2f}", cameraComp.m_pCamera->mViewDir.z);
				lightCamPos = "LCameraPos: " + lightCamPosX + "/" + lightCamPosY + "/" + lightCamPosZ;
				lightCamDir = "LCameraDir: " + lightCamDirX + "/" + lightCamDirY + "/" + lightCamDirZ;
			}
		}

		std::u8string u8camPos = std::u8string(camPos.begin(), camPos.end());
		std::u8string u8camDir = std::u8string(camDir.begin(), camDir.end());
		std::u8string u8lightCamPos = std::u8string(lightCamPos.begin(), lightCamPos.end());
		std::u8string u8lightCamDir = std::u8string(lightCamDir.begin(), lightCamDir.end());
		mpGraphicsEngine->UIDrawText(Vector2(0, 100), u8camPos);
		mpGraphicsEngine->UIDrawText(Vector2(0, 120), u8camDir);
		mpGraphicsEngine->UIDrawText(Vector2(0, 140), u8lightCamPos);
		mpGraphicsEngine->UIDrawText(Vector2(0, 160), u8lightCamDir);
	}
}
