#pragma once
#include "pch.h"

class InputManager;

class UI
{
public:
	UI() = delete;
	UI(const UID& _uid, const std::string& _name);
	virtual ~UI() = default;

	virtual bool Initialize();
	virtual void Update();
	virtual void Render();
	virtual void Finalize();


	Vector2 GetPosition() const;
	void SetPosition(Vector2 val);
	Vector2 GetSize() const;
	void SetSize(Vector2 val);

private:
	std::string m_fileName;
	Vector2 m_position;
	Vector2 m_size;

};

