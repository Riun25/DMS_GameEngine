#include "EventListener.h"
#include "EventManager.h"

// 객체의 생성자에서 이벤트 매니저를 의존성 주입
EventListener::EventListener(EventManager* _eventManager)
	: mpEventManager(_eventManager)
{
}

// 객체가 소멸될때, 해당 객체가 등록한 모든 이벤트들이 등록해제됩니다.
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


// 즉각적인 이벤트를 이벤트 매니저에 등록합니다.
HandlerID EventListener::RegisterImmediateEvent(const std::string& _eventName, EventHandler _handler)
{
	HandlerID id = mpEventManager->RegisterImmediateEvent(_eventName, std::move(_handler));
	mRegisteredImmediateHandlers[_eventName] = id;
	return id;
}

// 지연된 이벤트를 이벤트 매니저에 등록합니다.
HandlerID EventListener::RegisterDelayedEvent(const std::string& _eventName, EventHandler _handler)
{
	HandlerID id = mpEventManager->RegisterDelayedEvent(_eventName, std::move(_handler));
	mRegisteredDelayedHandlers[_eventName] = id;
	return id;
}

// 이벤트 매니저에 등록되어있는 이벤트를 해제합니다.
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
