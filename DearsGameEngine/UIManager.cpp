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
		if (camera.mCameraEnum == 0 && camera.mpCamera)
		{
			UpdateViewNProjection(camera.mpCamera);
		}
	}

	Vector2 mousePos = mpInputManager->GetMousePos();

	auto textureView = mRegistry.view<Texture2D, Button>();
	for (auto& entity : textureView)
	{
		auto& texture = mRegistry.get<Texture2D>(entity);
		auto& button = mRegistry.get<Button>(entity);
		// ��ư�� ���Ұ��̸� �ѱ��.
		if (!button.mIsEnable) continue;

		// ȣ������ �� �������� Ȯ���Ѵ�.
		bool isHovered =
			(mousePos.x >= texture.mPosition.x) && (mousePos.x <= texture.mPosition.x + texture.mSize.x) &&
			(mousePos.y >= texture.mPosition.y) && (mousePos.y <= texture.mPosition.y + texture.mSize.y);

		// ��ư�� ���¸� üũ�Ѵ�.
		CheckButtonState(&button, isHovered);

		auto text = mRegistry.try_get<Text>(entity);
		UpdateButton(&texture, &button, text);
	}

	/// �� ���ۿ��� ����ϴ� ��쿡 ���� �ؽ�ó�� ������Ʈ�Ѵ�.
	auto texture2DView = mRegistry.view<Texture2D>();
	for (auto& entity : texture2DView)
	{
		auto& texture = mRegistry.get<Texture2D>(entity);
		auto mesh = mRegistry.try_get<MeshRenderer>(entity);

		if (!(texture.mFile == "") && mesh)
		{
			auto newTexture2D = mpGraphicsEngine->Get_Textures(texture.mFile);
			if (mesh->mpModel->m_diffusetexture != newTexture2D)
			{
				if (auto button = mRegistry.try_get<Button>(entity))
				{
					button->mUIPosition = texture.mPosition;
					button->mNormalUIName = texture.mFile;
				}
				mesh->mpModel->m_diffusetexture = newTexture2D;
			}
		}
	}
}

void UIManager::LateUpdate(float _dTime)
{
}

void UIManager::Render(float _dTime)
{
	for (int i = mMaxNumLayer; i >= 0; i--)
	{
		/// Ÿ���� ���� ���ʿ� �׸���.
		RenderTile(i);

		/// texture ���� �׸� (���� ���� ���� -> �ؽ�ó���� �ؽ�Ʈ�� ���� �ö�;� ��)
		RenderUI(i);

		/// ü�¹ٸ� �׸�
		RenderHealthBar(i);

		/// ���簢���� �׸�
		RenderRectFilled(i);

		/// �ش� ���̾��� �ؽ�Ʈ�� ������
		RenderText(i);
	}

	RenderDebugInfo();
}

void UIManager::Finalize()
{
	// �ؽ�Ʈ ����
	auto view1 = mRegistry.view<Text>();
	for (auto& entity : view1)
	{
		// Text ������Ʈ�� �ִ� ��쿡�� ����
		if (auto text = mRegistry.try_get<Text>(entity))
		{
			// ��Ʈ�� �ߺ��� �� �� �ֱ� ������ �ִ��� Ȯ���ϰ� ���� ���� �����ش�.
			/*if (mpGraphicsEngine->Get_Font(text->mFont) != nullptr)
			{
				mpGraphicsEngine->Erase_Textures(text->mFont);
			}*/
			RemoveText(text->mpOwner);
		}
	}

	// ���簢�� ����
	auto view2 = mRegistry.view<Box2D>();
	for (auto& entity : view2)
	{
		if (auto box2D = mRegistry.try_get<Box2D>(entity))
		{
			RemoveRect(box2D->mpOwner);
		}
	}

	// ü�¹� ����
	auto view3 = mRegistry.view<HealthBarComponenet>();
	for (auto& entity : view3)
	{
		if (auto HPBar = mRegistry.try_get<HealthBarComponenet>(entity))
		{
			RemoveHPBar(HPBar->mpOwner);
		}
	}

	// �ؽ�ó ����
	auto view4 = mRegistry.view<Texture2D>();
	for (auto& entity : view4)
	{
		// Texture2D ������Ʈ�� �ִ� ��쿡�� ���� : �̰� �־�߸� ��ư�� �����Ǳ� ������ ���� ��ư�� ���� �ڵ带 ���� �ʾҴ�.
		if (auto texture = mRegistry.try_get<Texture2D>(entity))
		{
			RemoveUI(texture->mpOwner);
		}
	}

	// �޼��� �ڽ� ����
	auto view5 = mRegistry.view<MessageBox2D>();
	for (auto& entity : view5)
	{
		if (auto texture = mRegistry.try_get<MessageBox2D>(entity))
		{
			RemoveMessageBox2D(texture->mpOwner);
		}
	}
	auto view6 = mRegistry.view<MessageBox3D>();
	for (auto& entity : view6)
	{
		if (auto texture = mRegistry.try_get<MessageBox3D>(entity))
		{
			RemoveMessageBox3D(texture->mpOwner);
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
		std::cout << "�̹� ��Ʈ ������ ȣ���߽��ϴ�. �߰� ������ ���õ˴ϴ�." << std::endl;
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
		std::cout << "�̹� ��Ʈ ������ ȣ���߽��ϴ�. �߰� ������ ���õ˴ϴ�." << std::endl;
	}
}

void UIManager::AddText(std::shared_ptr<Entity> _entity, std::u8string _text, std::string _fontName, Vector2 _pos, int _layer, bool _isVisible, Vector4 _rgba)
{
	_entity->AddComponent<Text>(_text, _fontName, _pos, _layer, _isVisible, _rgba); // �ؽ�Ʈ ������Ʈ �߰�
	auto& text = _entity->GetComponent<Text>();
	mMaxNumLayer = std::max(mMaxNumLayer, _layer);  // �ִ� ���̾� ��ȣ ����
}

void UIManager::AddTextwithInt(std::shared_ptr<Entity> _entity, std::u8string _text, std::string _fontName, Vector2 _pos, int _num, int _layer /*= 0*/, bool _isVisible /*= true*/, Vector4 _rgba /*= Vector4(0.0f, 0.0f, 0.0f, 1.0f)*/)
{
	AddText(_entity, _text, _fontName, _pos, _layer, _isVisible, _rgba);
	auto& text = _entity->GetComponent<Text>();
	text.mIdx = TEXT_IDX::INT1;
	text.mNum1 = _num;
}

void UIManager::AddTextwithInt2(std::shared_ptr<Entity> _entity, std::u8string _text, std::string _fontName, Vector2 _pos, int _num1, int _num2, int _layer /*= 0*/, bool _isVisible /*= true*/, Vector4 _rgba /*= Vector4(0.0f, 0.0f, 0.0f, 1.0f)*/)
{
	AddText(_entity, _text, _fontName, _pos, _layer, _isVisible, _rgba);
	auto& text = _entity->GetComponent<Text>();
	text.mIdx = TEXT_IDX::INT2;
	text.mNum1 = _num1;
	text.mNum2 = _num2;
}

void UIManager::AddTextwithFloat(std::shared_ptr<Entity> _entity, std::u8string _text, std::string _fontName, Vector2 _pos, float _num, int _layer /*= 0*/, bool _isVisible /*= true*/, Vector4 _rgba /*= Vector4(0.0f, 0.0f, 0.0f, 1.0f)*/)
{
	AddText(_entity, _text, _fontName, _pos, _layer, _isVisible, _rgba);
	auto& text = _entity->GetComponent<Text>();
	text.mIdx = TEXT_IDX::FLOAT1;
	text.mNum3 = _num;
}

void UIManager::AddTextwithFloat2(std::shared_ptr<Entity> _entity, std::u8string _text, std::string _fontName, Vector2 _pos, float _num1, float _num2, int _layer /*= 0*/, bool _isVisible /*= true*/, Vector4 _rgba /*= Vector4(0.0f, 0.0f, 0.0f, 1.0f)*/)
{
	AddText(_entity, _text, _fontName, _pos, _layer, _isVisible, _rgba);
	auto& text = _entity->GetComponent<Text>();
	text.mIdx = TEXT_IDX::FLOAT2;
	text.mNum3 = _num1;
	text.mNum4 = _num2;
}

void UIManager::RemoveText(std::shared_ptr<Entity> _entity)
{
	// ��� �ڵ�: ��ƼƼ�� �ؽ�ó�� �ؽ�Ʈ ������Ʈ�� ������ �Լ� ���� ����
	if (!_entity->HasComponent<Text>())
	{
		return;
	}
	// ������Ʈ ����
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
	// Texture2D ������Ʈ�� ��ƼƼ�� �߰�
	_entity->AddComponent<Texture2D>(_fileName, _pos, _size, _layer, _rgba, _isVisible);
	mMaxNumLayer = std::max(mMaxNumLayer, _layer);  // �ִ� ���̾� ��ȣ ����
}

void UIManager::RemoveUI(std::shared_ptr<Entity> _entity)
{
	// ��� �ڵ�: ��ƼƼ�� �ؽ�ó�� �ؽ�Ʈ ������Ʈ�� ������ �Լ� ���� ����
	if (!_entity->HasComponent<Texture2D>())
	{
		return;
	}
	// ������Ʈ ����
	_entity->RemoveComponent<Texture2D>();
}

void UIManager::Erase_Textures(std::string _textureName)
{
	mpGraphicsEngine->Erase_Textures(_textureName);
}

void UIManager::SetMaxNumLayer(int _num)
{
	mMaxNumLayer = _num;
}

void UIManager::AddButtonPressUI(std::shared_ptr<Entity> _entity, std::string _UIImange, Vector2 _pressedPosition, Vector2 _pressedSize)
{
	if (_entity->HasComponent<Texture2D>())
	{
		auto& originalTexture = _entity->GetComponent<Texture2D>();

		if (!_entity->HasComponent<Button>()) // ��ư�� ������ �߰��Ѵ�.
		{
			_entity->AddComponent<Button>();
		}
		auto& buttonComp = _entity->GetComponent<Button>();
		buttonComp.mUIPosition = originalTexture.mPosition;
		buttonComp.mUISize = originalTexture.mSize;
		buttonComp.mNormalUIName = originalTexture.mFile;
		buttonComp.mPressedUIName = _UIImange;
		buttonComp.mUIPressedSize = _pressedSize;
		buttonComp.mUIPressedPosition = _pressedPosition;
		buttonComp.mPressedRGBA = Vector4(1.0f);
	}
	else
	{
		std::cout << "��ư �߰� ����" << std::endl;
		return;
	}
}

void UIManager::AddButtonPressText(std::shared_ptr<Entity> _entity, std::string _font, std::u8string _text, Vector2 _pressedPosition /*= Vector2()*/)
{
	if (_entity->HasComponent<Text>())
	{
		auto& originalText = _entity->GetComponent<Text>();

		if (!_entity->HasComponent<Button>()) // ��ư�� ������ �߰��Ѵ�.
		{
			_entity->AddComponent<Button>();
		}
		auto& buttonComp = _entity->GetComponent<Button>();
		buttonComp.mTextPosition = originalText.mPosition;
		buttonComp.mNormalTextFont = originalText.mFont;
		buttonComp.mNormalText = originalText.mText;
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

		if (!_entity->HasComponent<Button>()) // ��ư�� ������ �߰��Ѵ�.
		{
			_entity->AddComponent<Button>();
		}
		auto& buttonComp = _entity->GetComponent<Button>();
		buttonComp.mUIPosition = originalTexture.mPosition;
		buttonComp.mUISize = originalTexture.mSize;
		buttonComp.mNormalRGBA = originalTexture.mRgba;
		buttonComp.mUIPressedPosition = originalTexture.mPosition;
		buttonComp.mUIPressedSize = originalTexture.mSize;
		buttonComp.mPressedRGBA = _PressedRGBA;
	}
	else
	{
		std::cout << "��ư �߰� ����" << std::endl;
		return;
	}
}

void UIManager::AddButtonHoveredUI(std::shared_ptr<Entity> _entity, std::string _UIImange, Vector2 _hoveringPosition, Vector2 _hoveringSize)
{
	if (_entity->HasComponent<Texture2D>())
	{
		auto& originalTexture = _entity->GetComponent<Texture2D>();

		if (!_entity->HasComponent<Button>()) // ��ư�� ������ �߰��Ѵ�.
		{
			_entity->AddComponent<Button>();
		};
		auto& buttonComp = _entity->GetComponent<Button>();
		buttonComp.mUIPosition = originalTexture.mPosition;
		buttonComp.mUISize = originalTexture.mSize;
		buttonComp.mNormalUIName = originalTexture.mFile;
		buttonComp.mHoveredUIName = _UIImange;
		buttonComp.mUIHoveringSize = _hoveringSize;
		buttonComp.mUIHoveringPosition = _hoveringPosition;
		buttonComp.mHoveredRGBA = Vector4(1.0f);
	}
	else
	{
		std::cout << "��ư �߰� ����" << std::endl;
		return;
	}
}

void UIManager::AddButtonHoveredText(std::shared_ptr<Entity> _entity, std::string _font, std::u8string _text, Vector2 _hoveringPosition /*= Vector2()*/)
{
	if (_entity->HasComponent<Text>())
	{
		auto& originalText = _entity->GetComponent<Text>();

		if (!_entity->HasComponent<Button>()) // ��ư�� ������ �߰��Ѵ�.
		{
			_entity->AddComponent<Button>();
		}
		auto& buttonComp = _entity->GetComponent<Button>();
		buttonComp.mTextPosition = originalText.mPosition;
		buttonComp.mNormalTextFont = originalText.mFont;
		buttonComp.mNormalText = originalText.mText;
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

		if (!_entity->HasComponent<Button>()) // ��ư�� ������ �߰��Ѵ�.
		{
			_entity->AddComponent<Button>();
		}
		auto& buttonComp = _entity->GetComponent<Button>();
		buttonComp.mUIPosition = originalTexture.mPosition;
		buttonComp.mUISize = originalTexture.mSize;
		buttonComp.mNormalRGBA = originalTexture.mRgba;
		buttonComp.mUIHoveringSize = originalTexture.mSize;
		buttonComp.mUIHoveringPosition = originalTexture.mPosition;
		buttonComp.mHoveredRGBA = _HoveredRGBA;
	}
	else
	{
		std::cout << "��ư �߰� ����" << std::endl;
		return;
	}
}

void UIManager::AddButtonAllColor(std::shared_ptr<Entity> _entity, Vector4 _PressedRGBA, Vector4 _HoveredRGBA)
{
	if (_entity->HasComponent<Texture2D>())
	{
		auto& originalTexture = _entity->GetComponent<Texture2D>();

		if (!_entity->HasComponent<Button>()) // ��ư�� ������ �߰��Ѵ�.
		{
			_entity->AddComponent<Button>();
		}
		auto& buttonComp = _entity->GetComponent<Button>();
		buttonComp.mUIPosition = originalTexture.mPosition;
		buttonComp.mUISize = originalTexture.mSize;
		buttonComp.mNormalRGBA = originalTexture.mRgba;
		buttonComp.mUIPressedPosition = originalTexture.mPosition;
		buttonComp.mUIPressedSize = originalTexture.mSize;
		buttonComp.mPressedRGBA = _PressedRGBA;
		buttonComp.mUIHoveringSize = originalTexture.mSize;
		buttonComp.mUIHoveringPosition = originalTexture.mPosition;
		buttonComp.mHoveredRGBA = _HoveredRGBA;
	}
	else
	{
		std::cout << "��ư �߰� ����" << std::endl;
		return;
	}
}

void UIManager::AddButtonAllUI(std::shared_ptr<Entity> _entity, std::string _pressedUIImange, std::string _hoveringUIImange, Vector2 _pressedPosition, Vector2 _pressedSize, Vector2 _hoveringPosition, Vector2 _hoveringSize)
{
	if (_entity->HasComponent<Texture2D>())
	{
		auto& originalTexture = _entity->GetComponent<Texture2D>();

		if (!_entity->HasComponent<Button>()) // ��ư�� ������ �߰��Ѵ�.
		{
			_entity->AddComponent<Button>();
		}
		auto& buttonComp = _entity->GetComponent<Button>();
		buttonComp.mUIPosition = originalTexture.mPosition;
		buttonComp.mUISize = originalTexture.mSize;
		buttonComp.mNormalUIName = originalTexture.mFile;
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
		std::cout << "��ư �߰� ����" << std::endl;
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
	// ������Ʈ ����
	_entity->RemoveComponent<Button>();
}

void UIManager::AddRect(std::shared_ptr<Entity> _entity, Vector2 _posXY, Vector2 _sizeWH, int _layer, Vector4 _rgba, float _thickness /*= 0.0f*/, Vector4 _BorderRgba /*= Vector4()*/)
{
	_entity->AddComponent<Box2D>(_posXY, _sizeWH, _layer, _rgba, _thickness, _BorderRgba, true, false);
	mMaxNumLayer = std::max(mMaxNumLayer, _layer);  // �ִ� ���̾� ��ȣ ����
}

void UIManager::RemoveRect(std::shared_ptr<Entity> _entity)
{
	// ��� �ڵ�: ��ƼƼ�� �ؽ�ó�� �ؽ�Ʈ ������Ʈ�� ������ �Լ� ���� ����
	if (!_entity->HasComponent<Box2D>())
	{
		return;
	}
	// ������Ʈ ����
	_entity->RemoveComponent<Box2D>();
}

void UIManager::AddTile(std::shared_ptr<Entity> _entity, Vector2 _posXY, Vector2 _sizeWH, int _layer, Vector4 _rgba, float _thickness /*= 0.0f*/, Vector4 _BorderRgba /*= Vector4()*/)
{
	_entity->AddComponent<Tile>(_posXY, _sizeWH, _layer, _rgba, _thickness, _BorderRgba, true, false);
	mMaxNumLayer = std::max(mMaxNumLayer, _layer);  // �ִ� ���̾� ��ȣ ����
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
	mMaxNumLayer = std::max(mMaxNumLayer, _layer);  // �ִ� ���̾� ��ȣ ����
}

void UIManager::Add3DHPBar(std::shared_ptr<Entity> _entity, Vector3 _offset, Vector2 _sizeWH,
	int _layer, Vector4 _backgroundRgba, Vector4 _foregroundColor,
	float _healthPercentage /*= 1.0f*/)
{
	_entity->AddComponent<HealthBarComponenet>(Vector2(), _offset, _sizeWH, _layer, _backgroundRgba, _foregroundColor, _healthPercentage, true, true);
	mMaxNumLayer = std::max(mMaxNumLayer, _layer);  // �ִ� ���̾� ��ȣ ����
}

void UIManager::RemoveHPBar(std::shared_ptr<Entity> _entity)
{
	// ��� �ڵ�: ��ƼƼ�� �ؽ�ó�� �ؽ�Ʈ ������Ʈ�� ������ �Լ� ���� ����
	if (!_entity->HasComponent<HealthBarComponenet>())
	{
		return;
	}
	// ������Ʈ ����
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

	mMaxNumLayer = std::max(mMaxNumLayer, _layer);  // �ִ� ���̾� ��ȣ ����
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

	// NDC�� �ٲ� �ű� ������ x�� �״������, y�� �������̾�� �Ѵ�.
	message3DComp.mWorldPosition = _worldPos + Vector2(_worldPosOffset.x, -_worldPosOffset.y);
	message3DComp.mLayer = _layer;
	message3DComp.mIsImgVisible = _isVisible;
	message3DComp.mIsTextVisible = _isVisible;

	mMaxNumLayer = std::max(mMaxNumLayer, _layer);  // �ִ� ���̾� ��ȣ ����
}

void UIManager::RemoveMessageBox2D(std::shared_ptr<Entity> _entity)
{
	// ��� �ڵ�: ��ƼƼ�� �ؽ�ó�� �ؽ�Ʈ ������Ʈ�� ������ �Լ� ���� ����
	if (!_entity->HasComponent<MessageBox2D>())
	{
		return;
	}
	// ������Ʈ ����
	_entity->RemoveComponent<MessageBox2D>();
}

void UIManager::RemoveMessageBox3D(std::shared_ptr<Entity> _entity)
{
	// ��� �ڵ�: ��ƼƼ�� �ؽ�ó�� �ؽ�Ʈ ������Ʈ�� ������ �Լ� ���� ����
	if (!_entity->HasComponent<MessageBox3D>())
	{
		return;
	}
	// ������Ʈ ����
	_entity->RemoveComponent<MessageBox3D>();
}

void UIManager::SetRenderDebugInfo(bool _fps, bool _camera)
{
	mRendFPS = _fps;
	mRendCameraInfo = _camera;
}

/// UI �Ŵ��� �������� ���--------------------------------------------------------------------------------------------------------

void UIManager::UpdateViewNProjection(Camera* camera)
{
	/// ī�޶� ��ġ �ٲ� ��쿡 ���� ���Ӱ� ����
	if (camera->GetViewRow() != mViewMatrix || camera->GetProjRow() != mProjMatrix)
	{
		mViewMatrix = camera->GetViewRow();
		mProjMatrix = camera->GetProjRow();
	}
}

Vector2 UIManager::ConvertWorldtoNDC(Vector3 _worldPos, const Matrix& viewMatrix, const Matrix& projMatrix)
{
	// ���� ��ǥ�� 4D ���ͷ� ��ȯ (homogeneous ��ǥ�迡�� w = 1.0)
	Vector4 position = Vector4(_worldPos.x, _worldPos.y, _worldPos.z, 1.0f);

	// �� ��İ� �������� ����� ���� ��� ���
	Matrix viewProj = viewMatrix * projMatrix;

	// ���� ��ǥ -> Ŭ�� �������� ��ȯ
	Vector4 clipSpacePosition = Vector4::Transform(position, viewProj);

	// Homogeneous Divide (w�� ������)
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
	// ��ư�� ���¿� ���� �ؽ�ó�� �ؽ�Ʈ�� ������Ʈ : �ؽ�Ʈ�� ���� ���� �ֱ⿡ �ִ����� �ݵ�� Ȯ���Ѵ�.
	switch (_pButton->mButtonState)
	{
	case ButtonState::NORMAL:
		if (_pButton->mNormalUIName != "") // ��� �ؽ�ó�� ���ٸ� ���� �ʴ´�.
		{
			// if���� ���� ������ : file�� ��ġ�� ����� ���� �ʴٸ� ������Ʈ ���ش�.
			if (_pTexture2D->mFile != _pButton->mNormalUIName) _pTexture2D->mFile = _pButton->mNormalUIName;
			if (_pTexture2D->mPosition != _pButton->mUIPosition) _pTexture2D->mPosition = _pButton->mUIPosition;
			if (_pTexture2D->mSize != _pButton->mUISize) _pTexture2D->mSize = _pButton->mUISize;
		}

		if (_pTexture2D->mRgba != _pButton->mNormalRGBA) // ������ ����� ���� �ʴٸ� ������Ʈ ���ش�.
		{
			_pTexture2D->mRgba = _pButton->mNormalRGBA;
		}

		if (_pText && _pText->mFont != "" && _pText->mText != u8""
			&& _pButton->mNormalTextFont != "" && _pButton->mNormalText != u8"") // ��Ʈ�� �ؽ�Ʈ�� ������ �������� �ʴ´�.
		{
			// if���� ���� ������ : ��Ʈ, �ؽ�Ʈ, ��ġ�� ����� ���� �ʴٸ� ������Ʈ ���ش�.
			if (_pText->mFont != _pButton->mNormalTextFont) _pText->mFont = _pButton->mNormalTextFont;
			if (_pText->mText != _pButton->mNormalText) _pText->mText = _pButton->mNormalText;
			if (_pText->mPosition != _pButton->mTextPosition) _pText->mPosition = _pButton->mTextPosition;
		}
		break;
	case ButtonState::HOVERED:
		if (_pButton->mHoveredUIName != "") // ȣ���� �ؽ�ó�� ���ٸ� ���� �ʴ´�.
		{
			// if���� ���� ������ : file�� ��ġ�� ����� ���� �ʴٸ� ������Ʈ ���ش�.
			if (_pTexture2D->mFile != _pButton->mHoveredUIName) _pTexture2D->mFile = _pButton->mHoveredUIName;
			if (_pTexture2D->mPosition != _pButton->mUIHoveringPosition) _pTexture2D->mPosition = _pButton->mUIHoveringPosition;
			if (_pTexture2D->mSize != _pButton->mUIHoveringSize) _pTexture2D->mSize = _pButton->mUIHoveringSize;
		}

		if (_pTexture2D->mRgba != _pButton->mHoveredRGBA) // ������ ����� ���� �ʴٸ� ������Ʈ ���ش�.
		{
			_pTexture2D->mRgba = _pButton->mHoveredRGBA;
		}

		if (_pText && _pText->mFont != "" && _pText->mText != u8""
			&& _pButton->mHoveredTextFont != "" && _pButton->mHoveredText != u8"")
		{
			// if���� ���� ������ : ��Ʈ, �ؽ�Ʈ, ��ġ�� ����� ���� �ʴٸ� ������Ʈ ���ش�.
			if (_pText->mFont != _pButton->mHoveredTextFont) _pText->mFont = _pButton->mHoveredTextFont;
			if (_pText->mText != _pButton->mHoveredText) _pText->mText = _pButton->mHoveredText;
			if (_pText->mPosition != _pButton->mTextHoveringPosition) _pText->mPosition = _pButton->mTextHoveringPosition;
		}
		break;
	case ButtonState::PRESSED:

		if (_pButton->mPressedUIName != "") // ������ ������ �ؽ�ó�� ���ٸ� ���� �ʴ´�.
		{
			// if���� ���� ������ : file�� ��ġ�� ����� ���� �ʴٸ� ������Ʈ ���ش�.
			if (_pTexture2D->mFile != _pButton->mPressedUIName) _pTexture2D->mFile = _pButton->mPressedUIName;
		}

		// ���� �ٲ� �������� �ϱ� ������ �ؽ�ó�� ��� �Ǿ�� �Ѵ�.
		if (_pButton->mCanDrag && _pButton->mIsDrag)
		{
			// �巡�� ������ ���� ��Ÿ ������ ������ �� �����ؾ� ���� �������� ��Ÿ ������ ������ ��ġ�� �ʴ´�.
			Vector2 deltaPos = mpInputManager->GetDeltaMousePos();
			_pButton->mUIPosition += deltaPos;
			deltaPos = Vector2();
		}
		if (_pTexture2D->mPosition != _pButton->mUIPressedPosition) _pTexture2D->mPosition = _pButton->mUIPressedPosition;
		if (_pTexture2D->mSize != _pButton->mUIPressedSize) _pTexture2D->mSize = _pButton->mUIPressedSize;

		if (_pTexture2D->mRgba != _pButton->mPressedRGBA) // ������ ����� ���� �ʴٸ� ������Ʈ ���ش�.
		{
			_pTexture2D->mRgba = _pButton->mPressedRGBA;
		}

		if (_pText && _pText->mFont != "" && _pText->mText != u8""
			&& _pButton->mPressedTextFont != "" && _pButton->mPressedText != u8"")
		{
			// if���� ���� ������ : ��Ʈ, �ؽ�Ʈ, ��ġ�� ����� ���� �ʴٸ� ������Ʈ ���ش�.
			if (_pText->mFont != _pButton->mPressedTextFont) _pText->mFont = _pButton->mPressedTextFont;
			if (_pText->mText != _pButton->mPressedText) _pText->mText = _pButton->mPressedText;
			// �巡�� ������ ���� ��Ÿ ������ ������ �� �����ؾ� ���� �������� ��Ÿ ������ ������ ��ġ�� �ʴ´�.
			if (_pButton->mCanDrag && _pButton->mIsDrag)
			{
				Vector2 deltaPos = mpInputManager->GetDeltaMousePos();
				_pButton->mTextPosition += deltaPos;
				deltaPos = Vector2();
			}
			if (_pText->mPosition != _pButton->mTextPressedPosition) _pText->mPosition = _pButton->mTextPressedPosition;
		}
		break;
	}
}

void UIManager::CheckButtonState(Button* _pButton, bool _isHovered)
{
	if (_isHovered) // ȣ������ �ƴٸ� ���¸� �� ��Ȳ�� �°� �ٲ��ش�.
	{
		if (mpInputManager->GetKeyDown(KEY::LBUTTON)) // TAP
		{
			_pButton->mButtonState = ButtonState::PRESSED;
			_pButton->mCanDrag = false;
		}
		else if (mpInputManager->GetKey(KEY::LBUTTON)) // HOLD
		{
			_pButton->mCanDrag = true;// �巡�״� ���� ���� �߿����� Hold������ Ȱ��ȭ�ȴ�.
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
		else // ȣ���� ����
		{
			_pButton->mButtonState = ButtonState::HOVERED;
			_pButton->mCanDrag = false;
		}
	}
	else // ��� ����
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
	/// texture ���� �׸� (���� ���� ���� -> �ؽ�ó���� �ؽ�Ʈ�� ���� �ö�;� ��)
	// �̹����� �׸��� ������ ������ ����
	mpGraphicsEngine->UIDrawImageStart();

	for (auto& entity : mgs3DView)
	{
		auto& msg3DComp = mRegistry.get<MessageBox3D>(entity);
		// ������Ʈ�� ���̾� ���� ������ i ���� ��ġ�ϰ�, �׸� ���ҽ����� Ȯ���ϰ� �׸���.

		if (msg3DComp.mLayer == _layerOrder && msg3DComp.mIsImgVisible == true)
		{
			Vector2 imgRenderPos = -Vector2(msg3DComp.mSize.x / 2, 0) + ConvertNDCtoScreeen(ConvertWorldtoNDC(msg3DComp.mWorldPosition, mViewMatrix, mProjMatrix)) + msg3DComp.mImgOffset;
			mpGraphicsEngine->UIDrawImage(imgRenderPos, msg3DComp.mSize, msg3DComp.mFile, msg3DComp.mImgRgba);
		}
	}

	for (auto& entity : mgs2DView)
	{
		auto& msgComp = mRegistry.get<MessageBox2D>(entity);
		// ������Ʈ�� ���̾� ���� ������ i ���� ��ġ�ϰ�, �׸� ���ҽ����� Ȯ���ϰ� �׸���.

		if (msgComp.mLayer == _layerOrder && msgComp.mIsImgVisible == true)
		{
			mpGraphicsEngine->UIDrawImage(msgComp.mTexturePosition, msgComp.mSize, msgComp.mFile, msgComp.mImgRgba);
		}
	}


	// �ش� ���̾��� �ؽ�ó�� ������
	for (auto& entity : imgView)
	{
		auto& texture = mRegistry.get<Texture2D>(entity);
		// ������Ʈ�� ���̾� ���� ������ i ���� ��ġ�ϰ�, �׸� ���ҽ����� Ȯ���ϰ� �׸���.

		if (texture.mLayer == _layerOrder && texture.mIsVisible == true)
		{
			mpGraphicsEngine->UIDrawImage(texture.mPosition, texture.mSize, texture.mFile, texture.mRgba);
		}
	}

	// �̹����� �� �̻� �׸��� ����
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
		// ���ο� ��Ʈ�� ����� ������ ����
		mpGraphicsEngine->UIStartFontID(msg3DComp.mFont);
		// ��Ʈ ũ�� �缳��
		ImGui::SetWindowFontScale(msg3DComp.mScale);
		// ������Ʈ�� ���̾� ���� ������ i ���� ��ġ�ϰ�, �׸� ���ҽ����� Ȯ���ϰ� �׸���.
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
		// ���� ��Ʈ�� �� ������� ���� ������ ����
		mpGraphicsEngine->UIFinFontID();
	}

	for (auto& entity : mgs2DView)
	{
		auto& msgComp = mRegistry.get<MessageBox2D>(entity);
		// ���ο� ��Ʈ�� ����� ������ ����
		mpGraphicsEngine->UIStartFontID(msgComp.mFont);
		// ��Ʈ ũ�� �缳��
		ImGui::SetWindowFontScale(msgComp.mScale);
		// ������Ʈ�� ���̾� ���� ������ i ���� ��ġ�ϰ�, �׸� ���ҽ����� Ȯ���ϰ� �׸���.
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
		// ���� ��Ʈ�� �� ������� ���� ������ ����
		mpGraphicsEngine->UIFinFontID();
	}


	/// �ش� ���̾��� �ؽ�Ʈ�� ������
	for (auto& entity : textView)
	{
		auto& text = mRegistry.get<Text>(entity);
		// ���ο� ��Ʈ�� ����� ������ ����
		mpGraphicsEngine->UIStartFontID(text.mFont);
		// ��Ʈ ũ�� �缳��
		ImGui::SetWindowFontScale(text.mScale);
		if (text.mLayer == _layerOrder && text.mIsVisible == true)
		{
			if (text.mHasDepth == false) // 3D�� �ƴ�
			{
				switch (text.mIdx)
				{
				case TEXT_IDX::NONE:
					mpGraphicsEngine->UIDrawText(text.mPosition, text.mText, text.mRgba);
					break;
				case TEXT_IDX::INT1:
					mpGraphicsEngine->UIDrawTextWithNum(text.mPosition, text.mText, text.mRgba, text.mNum1);
					break;
				case TEXT_IDX::INT2:
					mpGraphicsEngine->UIDrawTextWithNum(text.mPosition, text.mText, text.mRgba, text.mNum1, text.mNum2);
					break;
				case TEXT_IDX::FLOAT1:
					mpGraphicsEngine->UIDrawTextWithNum(text.mPosition, text.mText, text.mRgba, text.mNum3);
					break;
				case TEXT_IDX::FLOAT2:
					mpGraphicsEngine->UIDrawTextWithNum(text.mPosition, text.mText, text.mRgba, text.mNum3, text.mNum4);
					break;
				default:
					break;
				}
			}
			else // 3D��
			{
				Vector2 renderPosition = ConvertNDCtoScreeen(ConvertWorldtoNDC(text.mWorldPosition + text.mOffset, mViewMatrix, mProjMatrix));
				switch (text.mIdx)
				{
				case TEXT_IDX::NONE:
					mpGraphicsEngine->UIDrawText(renderPosition, text.mText, text.mRgba);
					break;
				case TEXT_IDX::INT1:
					mpGraphicsEngine->UIDrawTextWithNum(renderPosition, text.mText, text.mRgba, text.mNum1);
					break;
				case TEXT_IDX::INT2:
					mpGraphicsEngine->UIDrawTextWithNum(renderPosition, text.mText, text.mRgba, text.mNum1, text.mNum2);
					break;
				case TEXT_IDX::FLOAT1:
					mpGraphicsEngine->UIDrawTextWithNum(renderPosition, text.mText, text.mRgba, text.mNum3);
					break;
				case TEXT_IDX::FLOAT2:
					mpGraphicsEngine->UIDrawTextWithNum(renderPosition, text.mText, text.mRgba, text.mNum3, text.mNum4);
					break;
				default:
					break;
				}

			}
		}
		// ���� ��Ʈ�� �� ������� ���� ������ ����
		mpGraphicsEngine->UIFinFontID();
	}
}

void UIManager::RenderRectFilled(int _layerOrder)
{
	auto view = mRegistry.view<Box2D>();
	/// �ش� ���̾��� �ؽ�Ʈ�� ������
	for (auto& entity : view)
	{
		auto& box2D = mRegistry.get<Box2D>(entity);
		// ���ο� ��Ʈ�� ����� ������ ����
		if (box2D.mLayer == _layerOrder && box2D.mIsVisible == true)
		{
			if (box2D.mThickness == 0.0f) // �β��� ����.
			{
				mpGraphicsEngine->UIDrawRectFilled(box2D.mPosition, box2D.mSize, box2D.mRgba);
			}
			else if (box2D.mRgba.z == 0.0f) // �β��� �ְ� ������ ����.
			{
				mpGraphicsEngine->UIDrawRect(box2D.mPosition, box2D.mSize, box2D.mBorderRgba, 0.0f, box2D.mThickness);
			}
			else  // �β��� �ְ�, ������ �ְ�, �׵θ� ���� �ִ�.
			{
				mpGraphicsEngine->UIDrawRectwithBorder(box2D.mPosition, box2D.mSize, box2D.mRgba, 0.0f, box2D.mThickness/*, box2D->mBorderRgba*/);
			}
		}
	}
}

void UIManager::RenderHealthBar(int _layerOrder)
{
	auto view = mRegistry.view<HealthBarComponenet>();
	/// �ش� ���̾��� �ؽ�Ʈ�� ������
	for (auto& entity : view)
	{
		auto& healthBar = mRegistry.get<HealthBarComponenet>(entity);
		if (healthBar.mLayer == _layerOrder && healthBar.mIsVisible == true)
		{
			Vector2 renderPosition = healthBar.mPosition;

			if (healthBar.mHasDepth == true)
			{
				auto& playerTransform = mRegistry.get<Transform>(entity);
				renderPosition = -(healthBar.mSize / 2) + ConvertNDCtoScreeen(ConvertWorldtoNDC(playerTransform.mLocalPosition + healthBar.mOffSet, mViewMatrix, mProjMatrix));
			}
			// ��� �׸���
			mpGraphicsEngine->UIDrawRectFilled(renderPosition, healthBar.mSize, healthBar.mBackgroundColor);
			// ü�� �� �׸���
			healthBar.mHealthPercentage = std::clamp(healthBar.mHealthPercentage, 0.0f, 1.0f);
			mpGraphicsEngine->UIDrawRectFilled(renderPosition, { healthBar.mSize.x * healthBar.mHealthPercentage, healthBar.mSize.y }, healthBar.mForegroundColor);

		}
	}
}

void UIManager::RenderTile(int _layerOrder)
{
	auto view = mRegistry.view<Tile>();
	/// �ش� ���̾��� �ؽ�Ʈ�� ������
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

					if (tile.mThickness == 0.0f) // �β��� ����.
					{
						mpGraphicsEngine->UIFreeRectFilled(pos1, pos2, pos3, pos4, tempRgBa);
					}
					else if (tile.mRgba.z == 0.0f) // �β��� �ְ� ������ ����.
					{
						mpGraphicsEngine->UIFreeRect(pos1, pos2, pos3, pos4, tile.mBorderRgba, tile.mThickness);
					}
					else  // �β��� �ְ�, ������ �ְ�, �׵θ� ���� �ִ�.
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
	///// �ش� ���̾��� �ؽ�Ʈ�� ������
	//for (auto& entity : view)
	//{
	//	auto& messageBox3D = mRegistry.get<MessageBox3D>(entity);
	//	if (messageBox3D.mLayer == _layerOrder && (messageBox3D.mIsImgVisible == true || messageBox3D.mIsTextVisible == true))
	//	{
	//		Vector2 imgRenderPos = ConvertNDCtoScreeen(ConvertWorldtoNDC(messageBox3D.mWorldPosition + messageBox3D.mImgOffset, mViewMatrix, mProjMatrix));
	//		Vector2 textRenderPos = ConvertNDCtoScreeen(ConvertWorldtoNDC(messageBox3D.mWorldPosition + messageBox3D.mTextOffset, mViewMatrix, mProjMatrix));

	//		// ��� �׸���
	//		mpGraphicsEngine->UIDrawRectFilled(renderPosition, messageBox3D.mSize, messageBox3D.mBackgroundColor);
	//		// �ؽ�Ʈ �׸���
	//		messageBox3D.mHealthPercentage = std::clamp(messageBox3D.mHealthPercentage, 0.0f, 1.0f);
	//		mpGraphicsEngine->UIDrawRectFilled(renderPosition, { messageBox3D.mSize.x * messageBox3D.mHealthPercentage, messageBox3D.mSize.y }, messageBox3D.mForegroundColor);

	//	}
	//}
}

void UIManager::RenderDebugInfo()
{
	if (mRendFPS)
	{
		std::string fps = std::format("{:.2f}", mpInfo->GetFPS());
		std::u8string u8fps = std::u8string(fps.begin(), fps.end());
		mpGraphicsEngine->UIDrawText(Vector2(), u8fps);
		std::string dT = std::format("{:.4f}", 1.f / mpInfo->GetFPS());
		std::u8string u8dT = std::u8string(dT.begin(), dT.end());
		mpGraphicsEngine->UIDrawText(Vector2(0, 20), u8dT);

	}
	if (mRendCameraInfo)
	{
		auto view = mRegistry.view<CameraComponent>();
		std::string camPos{};
		std::string camDir{};
		std::string lightCamPos{};
		std::string lightCamDir{};
		for (auto& entity : view)
		{
			auto& cameraComp = mRegistry.get<CameraComponent>(entity);
			if (cameraComp.mCameraEnum == 0)
			{
				std::string camPosX = std::format("{:.2f}", cameraComp.mpCamera->mViewPos.x);
				std::string camPosY = std::format("{:.2f}", cameraComp.mpCamera->mViewPos.y);
				std::string camPosZ = std::format("{:.2f}", cameraComp.mpCamera->mViewPos.z);
				std::string camDirX = std::format("{:.2f}", cameraComp.mpCamera->mViewDir.x);
				std::string camDirY = std::format("{:.2f}", cameraComp.mpCamera->mViewDir.y);
				std::string camDirZ = std::format("{:.2f}", cameraComp.mpCamera->mViewDir.z);
				camPos = "WCameraPos: " + camPosX + "/" + camPosY + "/" + camPosZ;
				camDir = "WCameraDir: " + camDirX + "/" + camDirY + "/" + camDirZ;
			}

			if (cameraComp.mCameraEnum == static_cast<int>(cameraEnum::LightCamera))
			{
				std::string lightCamPosX = std::format("{:.2f}", cameraComp.mpCamera->mViewPos.x);
				std::string lightCamPosY = std::format("{:.2f}", cameraComp.mpCamera->mViewPos.y);
				std::string lightCamPosZ = std::format("{:.2f}", cameraComp.mpCamera->mViewPos.z);
				std::string lightCamDirX = std::format("{:.2f}", cameraComp.mpCamera->mViewDir.x);
				std::string lightCamDirY = std::format("{:.2f}", cameraComp.mpCamera->mViewDir.y);
				std::string lightCamDirZ = std::format("{:.2f}", cameraComp.mpCamera->mViewDir.z);
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
