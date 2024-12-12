#pragma once
#include "IManager.h"

class Event;

// �̺�Ʈ�ڵ鷯 �Լ���ü�Դϴ�.
using EventHandler = std::function<void(const Event&)>;
// ���� �̸��� �̺�Ʈ�� �����ϱ� ���� uid�Դϴ�.
using HandlerID = uint16_t;

/// <summary>
/// �̺�Ʈ ���, ����, ���� �� �̺�Ʈ ������ �����ϴ� �Ŵ��� Ŭ�����Դϴ�.
/// �ﰢ���� �̺�Ʈ�� �ٷ� ó���ϰ�,
/// ������ �̺�Ʈ�� �̺�Ʈ ť�� ���� �� Update()�Լ����� ���������� �����մϴ�.
/// 2024.05.16 _Doyo;
/// </summary>
class EventManager
	: public IManager
{
public:
	EventManager() = default;
	~EventManager() = default;

	virtual bool Initialize() override;
	virtual void FixedUpdate(float _fixedDTime) override {}
	virtual void Update(float _dTime) override;
	virtual void LateUpdate(float _dTime) override;
	virtual void Finalize() override;

	HandlerID RegisterImmediateEvent(const std::string& _name, EventHandler _handler);
	HandlerID RegisterDelayedEvent(const std::string& _name, EventHandler _handler);
	void UnregisterEvent(const std::string& _name, HandlerID _handlerId);
	void TriggerEvent(const Event& _event);
	void PushDelayedEvent(const Event& _event, int _delayFrames);

private:
	void TriggerDelayedEvent(const Event& _event);
	void ProcessImmediateEvents();

private:
	std::unordered_map<std::string, std::unordered_map<HandlerID, EventHandler>> mImmediateHandlers;	// �ﰢ���� �̺�Ʈ �ڵ鷯 ��
	std::unordered_map<std::string, std::unordered_map<HandlerID, EventHandler>> mDelayedHandlers;		// ������ �̺�Ʈ �ڵ鷯 ��
	HandlerID m_lastHandlerId = 0;	// ������ �ڵ鷯 ID

	struct DelayedEvent
	{
		DelayedEvent(std::shared_ptr<Event> e, int frame);

		bool operator<(const DelayedEvent& _other) const;

		std::shared_ptr<Event> mEvent;
		int mExecuteFrame;
	};

	std::priority_queue<DelayedEvent> mDelayedEventQueue;		// ������ �̺�Ʈ �켱���� ť
	std::queue<std::shared_ptr<Event>> mImmediateEventQueue;	// �ﰢ���� �̺�Ʈ ť

// 	std::mutex mEventMutex;

	int mCurrentFrame = 0; // ���� ������ ��ȣ
};