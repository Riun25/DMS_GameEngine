#define  _CRTDBG_MAP_ALLOC
#include "framework.h"

/*
/// 1번
class EventManager
{
public:
	EventManager() {}
	EventManager(const EventManager&) = delete;
	EventManager& operator=(const EventManager&) = delete;
	~EventManager() {}

	// 이벤트 이름과 콜백 함수를 받아서 해당 이벤트의 콜백 리스트에 추가합니다.
	// 매서드 체이닝을 위해 this 포인터를 반환합니다.
	EventManager* on(const std::string& _eventName, std::function<void(int)> _callback)
	{
		auto& listeners = m_events[_eventName];

		// 이미 존재하는 리스너를 중복 등록하지 않습니다.
		if (std::find(listeners.begin(), listeners.end(), _callback) == listeners.end())
		{
			listeners.push_back(_callback);
		}

		return this;
	}

	// 이벤트 이름에 등록된 모든 콜백 함수를 호출합니다.
	// 이벤트에 콜백 함수가 등록되어 있지않다면 false, 콜백 함수를 실행하면 true를 반환합니다.
	bool emit(const std::string& _eventName, int _arg)
	{
		auto it = m_events.find(_eventName);
		if (it == m_events.end() || it->second.empty())
		{
			return false;
		}

		for (auto& listener : it->second)
		{
			listener(_arg);
		}

		return true;
	}

private:
	std::map<std::string, std::vector<std::function<void(int)>>> m_events;

};

void callback1(int _num)
{
	std::cout << "callback1-" << _num * 2 << std::endl;
}

void callback2(int _num)
{
	std::cout << "callback2-" << _num / 2 << std::endl;
}


int main()
{
	std::cout << "Event Manager Test" << std::endl;
	auto event_manager = std::make_unique<EventManager>();
	event_manager->on("event", callback1)->on("event", callback2);

	bool eventA = event_manager->emit("event", 100);
	bool eventB = event_manager->emit("bad_event", 200);

	std::cout << eventA << " " << eventB << std::endl;

	return 0;
}
*/
/*
/// 2번

struct Component;

struct GameObject
{
	std::vector<Component*> m_components;
};

struct Component
{
	int iValue;
	float fValue;
};

using EventData = std::variant<std::monostate, int, float, std::string, std::shared_ptr<Component>>;

// 이벤트를 나타내는 열거형
enum class EVENT_TYPE
{
	ButtonClicked,
	KeyPressed,
	Collision,
	Quit
};

// 이벤트를 저장하는 클래스
class Event
{
public:
	EVENT_TYPE type;
	EventData data;
	Event(EVENT_TYPE t) : type(t), data() {}
	Event(EVENT_TYPE t, EventData d) : type(t), data(d) {}
};

// 이벤트 핸들러를 나타내는 함수 시그니처
using EventHandler = std::function<void(const Event&)>;

// 이벤트 매니저 클래스
class EventManager
{
public:
	// 이벤트를 등록하는 함수
	void RegisterHandler(EVENT_TYPE type, const EventHandler& handler)
	{
		auto& handlerList = handlers[type];
		if (std::find(handlerList.begin(), handlerList.end(), handler) == handlerList.end())
		{
			handlerList.push_back(handler);
		}
	}

	// 이벤트 핸들러를 등록 취소하는 함수
	void UnregisterHandler(EVENT_TYPE type, const EventHandler& handler)
	{
		auto& handlerList = handlers[type];
		handlerList.erase(std::remove(handlerList.begin(), handlerList.end(), handler), handlerList.end());
	}

	// 등록된 핸들러에 이벤트를 전파하는 함수
	void HandleEvent(const Event& event)
	{
		auto it = handlers.find(event.type);
		if (it != handlers.end())
		{
			for (auto& handler : it->second)
			{
				try {
					handler(event);
				}
				catch (const std::exception& e)
				{
					std::cerr << "Exception caught in handler: " << e.what() << std::endl;
				}
			}
		}
	}

private:
	// 이벤트 타입과 핸들러를 저장하는 맵
	std::unordered_map<EVENT_TYPE, std::vector<EventHandler>> handlers;
};

// 예제로 사용될 이벤트 핸들러 함수
void OnButtonClick(const Event& event)
{
	std::cout << "Button Clicked Event Handled\n";
}

void OnKeyPress(const Event& event)
{
	std::cout << "Key Pressed Event Handled\n";
}

void OnCollision(const Event& event)
{
	std::cout << "Collision Event Handled\n";
	// 여기에 충돌에 대한 추가적인 처리를 추가할 수 있습니다.
}

int main()
{
	EventManager eventManager;

	// 이벤트 핸들러 등록
	eventManager.RegisterHandler(EVENT_TYPE::ButtonClicked, OnButtonClick);
	eventManager.RegisterHandler(EVENT_TYPE::KeyPressed, OnKeyPress);
	eventManager.RegisterHandler(EVENT_TYPE::Collision, OnCollision);

	// 이벤트 발생 및 처리
	Event buttonEvent(EVENT_TYPE::ButtonClicked);
	eventManager.HandleEvent(buttonEvent);

	Event keyEvent(EVENT_TYPE::KeyPressed);
	eventManager.HandleEvent(keyEvent);

	// 충돌 이벤트가 발생했다고 가정
	Event collisionEvent(EVENT_TYPE::Collision);
	eventManager.HandleEvent(collisionEvent);

	// 핸들러 등록 취소
	eventManager.UnregisterHandler(EVENT_TYPE::ButtonClicked, OnButtonClick);

	return 0;
}



*/

#include <iostream>
#include <string>
#include <variant>
#include <memory>
#include <functional>
#include <unordered_map>
#include <queue>


// 이벤트 데이터 타입 정의
using EventData = std::any;

struct Event
{
	std::string name;
	EventData data;

	Event(const std::string& _name, const EventData& _data) : name(_name), data(_data) {}
};

using EventCallback = std::function<void(const Event&)>;
using HandlerID = uint16_t;

class EventManager
{
public:
	bool Initialize() { return true; }
	void Update()
	{
		// 지연된 이벤트 처리
		while (!m_delayedEventQueue.empty())
		{
			auto event = std::move(m_delayedEventQueue.front());
			m_delayedEventQueue.pop();
			TriggerDelayedEvent(*event);
		}
	}
	void Finalize() {}

	HandlerID RegisterImmediateEvent(const std::string& _name, EventCallback _handler)
	{
		HandlerID newId = ++m_lastHandlerId;
		m_immediateHandlers[_name][newId] = std::move(_handler);
		return newId;
	}

	HandlerID RegisterDelayedEvent(const std::string& _name, EventCallback _handler)
	{
		HandlerID newId = ++m_lastHandlerId;
		m_delayedHandlers[_name][newId] = std::move(_handler);
		return newId;
	}

	void UnregisterEvent(const std::string& _name, HandlerID _handlerId)
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

	void TriggerEvent(const Event& _event)
	{
		if (auto it = m_immediateHandlers.find(_event.name); it != m_immediateHandlers.end())
		{
			for (auto& [id, handler] : it->second)
			{
				try
				{
					handler(_event);
				}
				catch (const std::exception& e)
				{
					std::cerr << "Exception caught in handler: " << e.what() << std::endl;
				}
			}
		}
	}

	void TriggerDelayedEvent(const Event& _event)
	{
		if (auto it = m_delayedHandlers.find(_event.name); it != m_delayedHandlers.end())
		{
			for (auto& [id, handler] : it->second)
			{
				try
				{
					handler(_event);
				}
				catch (const std::exception& e)
				{
					std::cerr << "Exception caught in handler: " << e.what() << std::endl;
				}
			}
		}
	}

	void PushDelayedEvent(const Event& _event, int _delayFrames)
	{
		m_delayedEventQueue.push(std::make_unique<Event>(_event, _delayFrames));
	}

private:
	std::unordered_map<std::string, std::unordered_map<HandlerID, EventCallback>> m_immediateHandlers;
	std::unordered_map<std::string, std::unordered_map<HandlerID, EventCallback>> m_delayedHandlers;
	HandlerID m_lastHandlerId = 0;
	std::queue<std::unique_ptr<Event>> m_delayedEventQueue;
};

class EventListener
{
public:
	EventListener(EventManager* _eventManager)
		: m_pEventManager(_eventManager)
	{
	}

	virtual ~EventListener()
	{
		for (const auto& [name, id] : m_registeredImmediateHandlers)
		{
			m_pEventManager->UnregisterEvent(name, id);
		}
		for (const auto& [name, id] : m_registeredDelayedHandlers)
		{
			m_pEventManager->UnregisterEvent(name, id);
		}
	}

protected:
	HandlerID RegisterImmediateEvent(const std::string& _eventName, EventCallback _handler)
	{
		HandlerID id = m_pEventManager->RegisterImmediateEvent(_eventName, std::move(_handler));
		m_registeredImmediateHandlers[_eventName] = id;
		return id;
	}

	HandlerID RegisterDelayedEvent(const std::string& _eventName, EventCallback _handler)
	{
		HandlerID id = m_pEventManager->RegisterDelayedEvent(_eventName, std::move(_handler));
		m_registeredDelayedHandlers[_eventName] = id;
		return id;
	}

	void UnregisterEvent(const std::string& _eventName)
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

protected:
	EventManager* m_pEventManager;
	std::unordered_map<std::string, HandlerID> m_registeredImmediateHandlers;
	std::unordered_map<std::string, HandlerID> m_registeredDelayedHandlers;
};

#define REGISTER_IMMEDIATE_EVENT(EVENT_NAME, HANDLER) \
    m_registeredImmediateHandlers[EVENT_NAME] = RegisterImmediateEvent(EVENT_NAME, [this](const Event& _event) { HANDLER(_event); })

#define REGISTER_DELAYED_EVENT(EVENT_NAME, HANDLER) \
    m_registeredDelayedHandlers[EVENT_NAME] = RegisterDelayedEvent(EVENT_NAME, [this](const Event& _event) { HANDLER(_event); })

#define UNREGISTER_EVENT(EVENT_NAME) \
    UnregisterEvent(EVENT_NAME)


class Object : public EventListener
{
public:
	Object(EventManager* eventManager) : EventListener(eventManager) {}

	virtual void Update() {};
};

class Test3
{
public:
	Test3()
	{
		static int i = 0;
		str = "test3_" + std::to_string(i++);
	}

	std::string str;
};

class Test1 : public Object
{
public:
	Test1(EventManager* eventManager)
		: Object(eventManager)
		, test()
	{
		REGISTER_IMMEDIATE_EVENT("LButtonClicked", Test1::LButtonClicked);
	}

	~Test1()
	{
		UNREGISTER_EVENT("LButtonClicked");
	}

	void Update() override
	{
		// 즉각적인 이벤트를 트리거합니다.
		m_pEventManager->TriggerEvent(Event("LButtonClicked", std::make_shared<Test3>(test)));
	}

	void LButtonClicked(const Event& event)
	{
		if (auto data = std::any_cast<std::shared_ptr<Test3>>(event.data))
		{
			std::cout << "LButtonClicked test1: " << data->str << std::endl;
		}
	}

	Test3 test;
};

class Test2 : public Object
{
public:
	Test2(EventManager* eventManager)
		: Object(eventManager)
		, test()
	{
		REGISTER_DELAYED_EVENT("LButtonClicked", Test2::LButtonClicked);
	}

	~Test2()
	{
		UNREGISTER_EVENT("LButtonClicked");
	}

	void Update() override
	{
		// 지연된 이벤트를 큐에 추가합니다.
		m_pEventManager->PushDelayedEvent("LButtonClicked", std::make_shared<Test3>(test));
	}

	void LButtonClicked(const Event& event)
	{
		if (auto data = std::any_cast<std::shared_ptr<Test3>>(event.data))
		{
			std::cout << "LButtonClicked test2: " << data->str << std::endl;
		}
	}

	Test3 test;
};

int main()
{
	EventManager manager;
	Test1 t1(&manager);
	Test2 t2(&manager);

	t1.Update();
	t2.Update();

	// 이벤트 처리
	manager.Update();

	return 0;
}