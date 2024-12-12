#include "pch.h"
#include "Component.h"
#include "Entity.h"

std::string Component::GetOwnerName() const
{
	return mpOwner->GetName();
}
