#pragma once
#include "Component.h"

/// <summary>
/// 2D �ؽ��� ������Ʈ�Դϴ�.
/// UI ��½ÿ� ���˴ϴ�.
/// �⺻ ������ ���� �������� �ʰڽ��ϴ�.
/// <para>
/// Texture2D(string "���ϸ�", Vector2 ��ġ, Vector2 ������, int ���̾�, bool �׸��� ����, Vector4 RGBA);
/// </para>
/// </summary>
struct Texture2D
	:public Component
{
public:
	Texture2D(std::shared_ptr<Entity> _owner, const std::string& _file = "", const Vector2& _position = Vector2()
		, const Vector2& _size = Vector2(1.f), const int& _layer = 0, const Vector4& _rgba = Vector4(1.0f, 1.0f, 1.0f, 1.0f)
		, const bool& _isVisible = true, const bool& _hasDepth = false)
		: Component(_owner), mFile(_file), mPosition(_position), mSize(_size), mLayer(_layer), 
		mIsVisible(_isVisible), mRgba(_rgba), mHasDepth(_hasDepth)
	{}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<Texture2D>(*this);
	}

public:
	std::string mFile; // ���ϸ�
	Vector2 mPosition; // ��ġ���� ���� ��� �����̴�.
	Vector2 mSize;		// ����, ����
	int mLayer;		// ���ڰ� Ŭ���� �ڿ� �׷����ϴ�.
	Vector4 mRgba;		// RGBA ��, �̹����� RGB�� �⺻ 1�̾�� �ϰ�, A�� ���İ��̴�.
	bool mIsVisible;	// ������������ ����� ������, true�� ���� �׸���.
	bool mHasDepth;	// 3D �������� z������ ���̰��� ��� �ϸ� ����Ѵ�.
};