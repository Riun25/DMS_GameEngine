#include "EventListener.h"
#include "EventManager.h"
#include "Event.h"

/// <summary>
/// 이벤트 리스너의 사용 예시 클래스입니다.
/// </summary>
class TestObject : public EventListener
{
public:
	TestObject(EventManager* _pEventManager)
		:EventListener(_pEventManager)
	{
		// 즉각적인 이벤트 등록
		REGISTER_IMMEDIATE_EVENT("TestImmediateEventHandler", TestImmediateEventHandler);
		// 지연된 이벤트 등록
		REGISTER_DELAYED_EVENT("TestDelayedEventHandler", TestDelayedEventHandler);
	}

	void Update()
	{
		// 즉각적인 이벤트 트리거
		mpEventManager->TriggerEvent(Event("TestImmediateEventHandler"));
		// 지연된 이벤트를 이벤트 큐에 푸쉬
		mpEventManager->PushDelayedEvent(Event("TestImmediateEventHandler"), 1);
	}

	// 즉각적으로 처리할 이벤트 핸들러 함수입니다.
	void TestImmediateEventHandler(const Event& _event)
	{
		DLOG(LOG_INFO, "TestImmediateEventHandler");
	}

	// 지연되게 처리할 이벤트 핸들러 함수입니다.
	void TestDelayedEventHandler(const Event& _event)
	{
		DLOG(LOG_INFO, "TestDelayedEventHandler");
	}

};