#pragma once
#include "Component.h"
#include "TextIdxEnum.h"

/// <summary>
/// �ؽ�Ʈ ui�� ���� �ؽ�Ʈ ������Ʈ Ŭ����
/// <para>
/// Text(u8string "����� ���ڿ�", string "����� ��Ʈ", Vector2 ��ġ, int ���̾�, bool ���� ����, Vector4 RGBA);
/// </para>
/// </summary>
struct Text
	: public Component
{
public:
	Text(std::shared_ptr<Entity> _owner, std::u8string _text = u8"", std::string _font = ""
		, Vector2 _position = Vector2(), int _layer = 0, bool _isVisible = true, 
		Vector4 _rgba = Vector4(0.0f, 0.0f, 0.0f, 1.0f))
		: Component(_owner), mText(_text), mFont(_font), mPosition(_position), mLayer(_layer),
		mIsVisible(_isVisible), mRgba(_rgba), mHasDepth(false)
	{}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<Text>(*this);
	}

public:

	std::u8string mText;
	std::string mFont;
	Vector2 mPosition;
	int mLayer; // ���ڰ� Ŭ���� �ڿ� �׷����ϴ�.
	bool mIsVisible; // ������������ ����� ������, true�� ���� �׸���.
	Vector4 mRgba;
	float mScale = 1.f;	// ��Ʈ�� ũ��

	Vector3 mWorldPosition = Vector3(); // 3D ������ ��ġ
	Vector3 mOffset = Vector3(); // 3D ���������� ������
	bool mHasDepth;	// 3D �������� z������ ���̰��� ��� �ϸ� ����Ѵ�.

	/// <summary>
	///  �ӽ÷� �̷��� ����ϰڽ��ϴ�. ���� �����ϰڽ��ϴ�.
	/// </summary>
	TEXT_IDX mIdx = TEXT_IDX::NONE;
	int mNum1 = 0; //AddTextwithInt() �Լ��� �μ��Դϴ�.
	int mNum2 = 0; //AddTextwithInt2()�� �� ��° �μ��̸�, ù��° �μ��� m_num1�Դϴ�.
	float mNum3 = 0; // AddTextwithFloat �Լ��� �μ��Դϴ�.
	float mNum4 = 0; //AddTextwithFloat2�� �� ��° �μ��̸�, ù��° �μ��� m_num3�Դϴ�.
};