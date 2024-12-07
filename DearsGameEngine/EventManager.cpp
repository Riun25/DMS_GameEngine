#include "EventManager.h"
#include "Event.h"

bool EventManager::Initialize()
{
	m_currentFrame = 0;
	return true;
}

void EventManager::Update(float _dTime)
{
	m_currentFrame++;

	// 즉각적인 이벤트 처리
	ProcessImmediateEvents();

	// 지연된 이벤트 처리
// 	std::lock_guard<std::mutex> lock(m_eventMutex);
	while (!m_delayedEventQueue.empty() && m_delayedEventQueue.top().m_executeFrame <= m_currentFrame)
	{
		auto delayedEvent = m_delayedEventQueue.top().m_event;
		m_delayedEventQueue.pop();
		TriggerDelayedEvent(*delayedEvent);
	}
}

void EventManager::LateUpdate(float _dTime)
{
// 	// 즉각적인 이벤트 처리
// 	ProcessImmediateEvents();
// 
// 	// 지연된 이벤트 처리
// 	while (!m_delayedEventQueue.empty() && m_delayedEventQueue.top().m_executeFrame <= m_currentFrame)
// 	{
// 		auto delayedEvent = m_delayedEventQueue.top().m_event;
// 		m_delayedEventQueue.pop();
// 		TriggerDelayedEvent(*delayedEvent);
// 	}
}

void EventManager::Finalize()
{
}

// 즉각적으로 처리할 이벤트 핸들러를 등록하는 함수입니다..
HandlerID EventManager::RegisterImmediateEvent(const std::string& _name, EventHandler _handler)
{
	HandlerID newId = ++m_lastHandlerId;
	m_immediateHandlers[_name][newId] = std::move(_handler);
	return newId;
}

// 지연처리할 이벤트 핸들러를 등록하는 함수입니다
HandlerID EventManager::RegisterDelayedEvent(const std::string& _name, EventHandler _handler)
{
	HandlerID newId = ++m_lastHandlerId;
	m_delayedHandlers[_name][newId] = std::move(_handler);
	return newId;
}

// 등록되어있는 이벤트 핸들러를 등록해제하는 함수입니다
void EventManager::UnregisterEvent(const std::string& _name, HandlerID _handlerId)
{
	// 즉각적인 핸들러에서 제거
	if (auto it = m_immediateHandlers.find(_name); it != m_immediateHandlers.end())
	{
		it->second.erase(_handlerId);
		if (it->second.empty())
		{
			m_immediateHandlers.erase(it);
		}
	}
	// 지연된 핸들러에서 제거
	if (auto it = m_delayedHandlers.find(_name); it != m_delayedHandlers.end())
	{
		it->second.erase(_handlerId);
		if (it->second.empty())
		{
			m_delayedHandlers.erase(it);
		}
	}
}

// 즉각적인 이벤트 트리거 함수입니다
void EventManager::TriggerEvent(const Event& _event)
{
// 	std::lock_guard<std::mutex> lock(m_eventMutex);
	m_immediateEventQueue.push(std::make_shared<Event>(_event));
}

// 즉각적인 이벤트 처리 함수입니다
void EventManager::ProcessImmediateEvents()
{
// 	std::lock_guard<std::mutex> lock(m_eventMutex);
	while (!m_immediateEventQueue.empty())
	{
		auto event = std::move(m_immediateEventQueue.front());
		m_immediateEventQueue.pop();

		if (auto it = m_immediateHandlers.find(event->m_name); it != m_immediateHandlers.end())
		{
			for (auto& [id, handler] : it->second)
			{
				try
				{
					handler(*event);
				}
				catch (const std::exception& e)
				{
					DLOG(LOG_WARN, e.what());
				}
			}
		}
	}
}

// 이벤트 큐에 지연된 이벤트를 추가합니다
void EventManager::PushDelayedEvent(const Event& _event, int _delayFrames)
{
// 	std::lock_guard<std::mutex> lock(m_eventMutex);
	int executeFrame = m_currentFrame + _delayFrames;
	m_delayedEventQueue.push(DelayedEvent{ std::make_unique<Event>(_event), executeFrame });
}

//  지연된 이벤트 트리거 함수입니다.
void EventManager::TriggerDelayedEvent(const Event& _event)
{
	if (auto it = m_delayedHandlers.find(_event.m_name); it != m_delayedHandlers.end())
	{
		for (auto& [id, handler] : it->second)
		{
			try
			{
				handler(_event);
			}
			catch (const std::exception& e)
			{
				DLOG(LOG_WARN, e.what());
				// 				std::cerr << "Exception caught in handler: " << e.what() << std::endl;
			}
		}
	}
}

EventManager::DelayedEvent::DelayedEvent(std::shared_ptr<Event> e, int frame)
	: m_event(std::move(e)), m_executeFrame(frame)
{
}

bool EventManager::DelayedEvent::operator<(const DelayedEvent& _other) const
{
	return m_executeFrame > _other.m_executeFrame;
}
