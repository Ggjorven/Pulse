#include "Events.hpp"

#include <Pulse/Events/Event.hpp>
#include <Pulse/Events/EventDispatcher.hpp>

struct MouseEvent : public Pulse::Events::Event
{
public:
	float X = 0.0f;
	float Y = 0.0f;
};

struct KeyEvent : public Pulse::Events::Event
{
public:
	std::string KeyCode = {};
};

EventsTest::EventsTest()
{
}

EventsTest::~EventsTest()
{
}

TestResult EventsTest::Execute()
{
	using namespace Pulse;
	using namespace Pulse::Events;

	TestResult result;

	EventDispatcher dispatcher = {};
	dispatcher.AddCallback<MouseEvent>([this](const MouseEvent& e)
	{
		m_MouseReceived = true;
	});
	dispatcher.AddCallback<KeyEvent>([this](const KeyEvent& e)
	{
		m_KeyReceived = true;
	});


	dispatcher.Dispatch<MouseEvent>(MouseEvent());
	dispatcher.Dispatch<KeyEvent>(KeyEvent());


	PULSE_TEST((!m_MouseReceived));
	PULSE_TEST((!m_KeyReceived));

	return result;
}
