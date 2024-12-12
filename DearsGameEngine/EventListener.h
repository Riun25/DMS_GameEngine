#pragma once
#include "pch.h"

class Event;
class EventManager;
using EventHandler = std::function<void(const Event&)>;
using HandlerID = uint16_t;

/// <summary>
/// �̺�Ʈ ������ Ŭ�����Դϴ�.
/// �� Ŭ������ ��ӹ����� �̺�Ʈ�� ����� �� �ֽ��ϴ�.
/// �� ������Ʈ, ���� ������Ʈ ���� ��ӹ޾Ƽ� ����մϴ�.
/// </summary>
class EventListener
{
public:
	EventListener() = delete;
	EventListener(EventManager* _eventManager);
	virtual ~EventListener();

protected:
	HandlerID RegisterImmediateEvent(const std::string& _eventName, EventHandler _handler);
	HandlerID RegisterDelayedEvent(const std::string& _eventName, EventHandler _handler);
	void UnregisterEvent(const std::string& _eventName);

protected:
	EventManager* mpEventManager;	// �̺�Ʈ �Ŵ���
	std::unordered_map<std::string, HandlerID> mRegisteredImmediateHandlers;	// �ﰢ���� �̺�Ʈ���� id ��
	std::unordered_map<std::string, HandlerID> mRegisteredDelayedHandlers;		// ������ �̺�Ʈ���� id ��

	friend class EventManager;
};

// �ﰢ���� �̺�Ʈ ��� ��ũ��
#define REGISTER_IMMEDIATE_EVENT(EVENT_NAME, HANDLER) \
    mRegisteredImmediateHandlers[EVENT_NAME] = RegisterImmediateEvent(EVENT_NAME, [this](const Event& _event) { HANDLER(_event); })

// ������ �̺�Ʈ ��� ��ũ��
#define REGISTER_DELAYED_EVENT(EVENT_NAME, HANDLER) \
    mRegisteredDelayedHandlers[EVENT_NAME] = RegisterDelayedEvent(EVENT_NAME, [this](const Event& _event) { HANDLER(_event); })

// �̺�Ʈ ��� ���� ��ũ��
#define UNREGISTER_EVENT(EVENT_NAME) \
    UnregisterEvent(EVENT_NAME)

