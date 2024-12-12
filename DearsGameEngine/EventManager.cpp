#include "EventManager.h"
#include "Event.h"

bool EventManager::Initialize()
{
	mCurrentFrame = 0;
	return true;
}

void EventManager::Update(float _dTime)
{
	mCurrentFrame++;

	// �ﰢ���� �̺�Ʈ ó��
	ProcessImmediateEvents();

	// ������ �̺�Ʈ ó��
// 	std::lock_guard<std::mutex> lock(m_eventMutex);
	while (!mDelayedEventQueue.empty() && mDelayedEventQueue.top().mExecuteFrame <= mCurrentFrame)
	{
		auto delayedEvent = mDelayedEventQueue.top().mEvent;
		mDelayedEventQueue.pop();
		TriggerDelayedEvent(*delayedEvent);
	}
}

void EventManager::LateUpdate(float _dTime)
{
// 	// �ﰢ���� �̺�Ʈ ó��
// 	ProcessImmediateEvents();
// 
// 	// ������ �̺�Ʈ ó��
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

// �ﰢ������ ó���� �̺�Ʈ �ڵ鷯�� ����ϴ� �Լ��Դϴ�..
HandlerID EventManager::RegisterImmediateEvent(const std::string& _name, EventHandler _handler)
{
	HandlerID newId = ++m_lastHandlerId;
	mImmediateHandlers[_name][newId] = std::move(_handler);
	return newId;
}

// ����ó���� �̺�Ʈ �ڵ鷯�� ����ϴ� �Լ��Դϴ�
HandlerID EventManager::RegisterDelayedEvent(const std::string& _name, EventHandler _handler)
{
	HandlerID newId = ++m_lastHandlerId;
	mDelayedHandlers[_name][newId] = std::move(_handler);
	return newId;
}

// ��ϵǾ��ִ� �̺�Ʈ �ڵ鷯�� ��������ϴ� �Լ��Դϴ�
void EventManager::UnregisterEvent(const std::string& _name, HandlerID _handlerId)
{
	// �ﰢ���� �ڵ鷯���� ����
	if (auto it = mImmediateHandlers.find(_name); it != mImmediateHandlers.end())
	{
		it->second.erase(_handlerId);
		if (it->second.empty())
		{
			mImmediateHandlers.erase(it);
		}
	}
	// ������ �ڵ鷯���� ����
	if (auto it = mDelayedHandlers.find(_name); it != mDelayedHandlers.end())
	{
		it->second.erase(_handlerId);
		if (it->second.empty())
		{
			mDelayedHandlers.erase(it);
		}
	}
}

// �ﰢ���� �̺�Ʈ Ʈ���� �Լ��Դϴ�
void EventManager::TriggerEvent(const Event& _event)
{
// 	std::lock_guard<std::mutex> lock(m_eventMutex);
	mImmediateEventQueue.push(std::make_shared<Event>(_event));
}

// �ﰢ���� �̺�Ʈ ó�� �Լ��Դϴ�
void EventManager::ProcessImmediateEvents()
{
// 	std::lock_guard<std::mutex> lock(m_eventMutex);
	while (!mImmediateEventQueue.empty())
	{
		auto event = std::move(mImmediateEventQueue.front());
		mImmediateEventQueue.pop();

		if (auto it = mImmediateHandlers.find(event->mName); it != mImmediateHandlers.end())
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

// �̺�Ʈ ť�� ������ �̺�Ʈ�� �߰��մϴ�
void EventManager::PushDelayedEvent(const Event& _event, int _delayFrames)
{
// 	std::lock_guard<std::mutex> lock(m_eventMutex);
	int executeFrame = mCurrentFrame + _delayFrames;
	mDelayedEventQueue.push(DelayedEvent{ std::make_unique<Event>(_event), executeFrame });
}

//  ������ �̺�Ʈ Ʈ���� �Լ��Դϴ�.
void EventManager::TriggerDelayedEvent(const Event& _event)
{
	if (auto it = mDelayedHandlers.find(_event.mName); it != mDelayedHandlers.end())
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
	: mEvent(std::move(e)), mExecuteFrame(frame)
{
}

bool EventManager::DelayedEvent::operator<(const DelayedEvent& _other) const
{
	return mExecuteFrame > _other.mExecuteFrame;
}
