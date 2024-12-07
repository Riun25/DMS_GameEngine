#include "ButtonUI.h"

ButtonUI::ButtonUI(const UID& _uid, const std::string& _name)
	: UI(_uid, _name)
	,m_isClicked(false)
{

}

bool ButtonUI::IsClicked() const
{
	return m_isClicked;
}

void ButtonUI::IsClicked(bool val)
{
	m_isClicked = val;
}