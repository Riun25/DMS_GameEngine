#include "EventListener.h"
#include "EventManager.h"

// ��ü�� �����ڿ��� �̺�Ʈ �Ŵ����� ������ ����
EventListener::EventListener(EventManager* _eventManager)
	: mpEventManager(_eventManager)
{
}

// ��ü�� �Ҹ�ɶ�, �ش� ��ü�� ����� ��� �̺�Ʈ���� ��������˴ϴ�.
EventListener::~EventListener()
{
// 	for (const auto& [name, id] : m_registeredImmediateHandlers)
// 	{
// 		m_pEventManager->UnregisterEvent(name, id);
// 	}
// 	for (const auto& [name, id] : m_registeredDelayedHandlers)
// 	{
// 		m_pEventManager->UnregisterEvent(name, id);
// 	}
}


// �ﰢ���� �̺�Ʈ�� �̺�Ʈ �Ŵ����� ����մϴ�.
HandlerID EventListener::RegisterImmediateEvent(const std::string& _eventName, EventHandler _handler)
{
	HandlerID id = mpEventManager->RegisterImmediateEvent(_eventName, std::move(_handler));
	mRegisteredImmediateHandlers[_eventName] = id;
	return id;
}

// ������ �̺�Ʈ�� �̺�Ʈ �Ŵ����� ����մϴ�.
HandlerID EventListener::RegisterDelayedEvent(const std::string& _eventName, EventHandler _handler)
{
	HandlerID id = mpEventManager->RegisterDelayedEvent(_eventName, std::move(_handler));
	mRegisteredDelayedHandlers[_eventName] = id;
	return id;
}

// �̺�Ʈ �Ŵ����� ��ϵǾ��ִ� �̺�Ʈ�� �����մϴ�.
void EventListener::UnregisterEvent(const std::string& _eventName)
{
	auto it = mRegisteredImmediateHandlers.find(_eventName);
	if (it != mRegisteredImmediateHandlers.end())
	{
		mpEventManager->UnregisterEvent(_eventName, it->second);
		mRegisteredImmediateHandlers.erase(it);
	}
	it = mRegisteredDelayedHandlers.find(_eventName);
	if (it != mRegisteredDelayedHandlers.end())
	{
		mpEventManager->UnregisterEvent(_eventName, it->second);
		mRegisteredDelayedHandlers.erase(it);
	}
}
