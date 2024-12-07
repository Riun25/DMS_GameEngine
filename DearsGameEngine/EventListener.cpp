#include "EventListener.h"
#include "EventManager.h"

// 객체의 생성자에서 이벤트 매니저를 의존성 주입
EventListener::EventListener(EventManager* _eventManager)
	: m_pEventManager(_eventManager)
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
	HandlerID id = m_pEventManager->RegisterImmediateEvent(_eventName, std::move(_handler));
	m_registeredImmediateHandlers[_eventName] = id;
	return id;
}

// 지연된 이벤트를 이벤트 매니저에 등록합니다.
HandlerID EventListener::RegisterDelayedEvent(const std::string& _eventName, EventHandler _handler)
{
	HandlerID id = m_pEventManager->RegisterDelayedEvent(_eventName, std::move(_handler));
	m_registeredDelayedHandlers[_eventName] = id;
	return id;
}

// 이벤트 매니저에 등록되어있는 이벤트를 해제합니다.
void EventListener::UnregisterEvent(const std::string& _eventName)
{
	auto it = m_registeredImmediateHandlers.find(_eventName);
	if (it != m_registeredImmediateHandlers.end())
	{
		m_pEventManager->UnregisterEvent(_eventName, it->second);
		m_registeredImmediateHandlers.erase(it);
	}
	it = m_registeredDelayedHandlers.find(_eventName);
	if (it != m_registeredDelayedHandlers.end())
	{
		m_pEventManager->UnregisterEvent(_eventName, it->second);
		m_registeredDelayedHandlers.erase(it);
	}
}
