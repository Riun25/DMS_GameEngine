#include "TextUI.h"

TextUI::TextUI(const UID& _uid, const std::string& _name)
	: UI(_uid, _name)
{

}

std::string TextUI::GetText() const
{
	return m_text;
}

void TextUI::SetText(std::string val)
{
	m_text = val;
}