#pragma once
#include "pch.h"
#include "UI.h"

class TextUI
	: public UI
{
public:
	TextUI() = delete;
	TextUI(const UID& _uid, const std::string& _name);
	~TextUI() = default;

	std::string GetText() const;
	void SetText(std::string val);

private:
	std::string m_text;
};

