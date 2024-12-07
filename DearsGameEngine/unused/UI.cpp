#include "UI.h"

UI::UI(const UID& _uid, const std::string& _name)
{

}

bool UI::Initialize()
{
	return true;
}

void UI::Update()
{

}

void UI::Render()
{

}

void UI::Finalize()
{

}

Vector2 UI::GetPosition() const
{
	return m_position;
}

void UI::SetPosition(Vector2 val)
{
	m_position = val;
}

Vector2 UI::GetSize() const
{
	return m_size;
}

void UI::SetSize(Vector2 val)
{
	m_size = val;
}