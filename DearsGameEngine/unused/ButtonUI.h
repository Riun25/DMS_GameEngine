#pragma once
#include "pch.h"
#include "UI.h"

class ButtonUI
	: public UI
{
public:
	ButtonUI() = delete;
	ButtonUI(const UID& _uid, const std::string& _name);
	~ButtonUI() = default;

	bool IsClicked() const;
	void IsClicked(bool val);

private:
	bool m_isClicked;
};

