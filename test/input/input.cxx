#include <catch2/catch_test_macros.hpp>
#include "framework/event_manager.hxx"
#include "input/input.hxx"
#include "input/input_event.hxx"

TEST_CASE("Input::mouse_position returns current mouse position", "[mouse][unit]") {
    EventManager events;
    Input input(&events);

    SECTION("Returns (0, 0) by default") {
        REQUIRE(input.mouse_position() == Vec2<u32> { 0, 0 });
    }

    SECTION("Returns the last set mouse position") {
        input.set_mouse_position({ 100, 200 });
        REQUIRE(input.mouse_position() == Vec2<u32> { 100, 200 });
    }
}

TEST_CASE("Input::mouse_position triggers MouseMoveEvent", "[mouse][unit][event]") {
    EventManager events;
    std::optional<MouseMoveEvent> event = std::nullopt;
    events.on<MouseMoveEvent>([&event](const MouseMoveEvent& e) -> bool { event = e; return true; });
    Input input(&events);

    SECTION("Returns the last set mouse position") {
        input.set_mouse_position({ 100, 200 });
        REQUIRE(event.has_value());
        REQUIRE(event->position.x == 100);
        REQUIRE(event->position.y == 200);
    }
}

TEST_CASE("Input::mouse_x returns current x coordinate of the mouse cursor", "[mouse][unit]") {
    EventManager events;
    Input input(&events);

    SECTION("Returns 0 by default") {
        REQUIRE(input.mouse_x() == 0);
    }

    SECTION("Returns the last set x coordinate") {
        input.set_mouse_position({ 100, 200 });
        REQUIRE(input.mouse_x() == 100);
    }
}

TEST_CASE("Input::mouse_y returns current y coordinate of the mouse cursor", "[mouse][unit]") {
    EventManager events;
    Input input(&events);

    SECTION("Returns 0 by default") {
        REQUIRE(input.mouse_y() == 0);
    }

    SECTION("Returns the last set y coordinate") {
        input.set_mouse_position({ 100, 200 });
        REQUIRE(input.mouse_y() == 200);
    }
}

TEST_CASE("Input::is_mouse_pressed returns true if the given mouse button is currently pressed", "[mouse][unit]") {
    EventManager events;
    Input input(&events);

    SECTION("Returns false by default") {
        REQUIRE_FALSE(input.is_mouse_pressed(MouseButton::Left));
    }

    SECTION("Returns true if the given button is pressed") {
        input.set_mouse_button_pressed(MouseButton::Left, true);
        REQUIRE(input.is_mouse_pressed(MouseButton::Left));
        input.set_mouse_button_pressed(MouseButton::Left, false);
        REQUIRE_FALSE(input.is_mouse_pressed(MouseButton::Left));
    }

    SECTION("Returns false if the given button is not pressed") {
        input.set_mouse_button_pressed(MouseButton::Left, true);
        REQUIRE_FALSE(input.is_mouse_pressed(MouseButton::Right));
    }
}

TEST_CASE("Input::is_key_pressed returns true if the given keyboard key is currently pressed", "[keyboard][unit]") {
    EventManager events;
    Input input(&events);

    SECTION("Returns false by default") {
        REQUIRE_FALSE(input.is_key_pressed(Key::A));
    }

    SECTION("Returns true if the given key is pressed") {
        input.set_key_pressed(Key::A, true);
        REQUIRE(input.is_key_pressed(Key::A));
        input.set_key_pressed(Key::A, false);
        REQUIRE_FALSE(input.is_key_pressed(Key::A));
    }

    SECTION("Returns false if the given key is not pressed") {
        input.set_key_pressed(Key::A, true);
        REQUIRE_FALSE(input.is_key_pressed(Key::B));
    }
}

TEST_CASE("Input::set_mouse_position updates mouse coordinates", "[mouse][unit]") {
    EventManager events;
    Input input(&events);

    SECTION("Updates mouse_x and mouse_y") {
        input.set_mouse_position({ 100, 200 });
        REQUIRE(input.mouse_x() == 100);
        REQUIRE(input.mouse_y() == 200);
    }
}

TEST_CASE("Input::set_mouse_position triggers MouseMoveEvent", "[mouse][unit][event]") {
    EventManager events;
    std::optional<MouseMoveEvent> event = std::nullopt;
    events.on<MouseMoveEvent>([&event](const MouseMoveEvent& e) -> bool { event = e; return true; });
    Input input(&events);


    SECTION("Updates mouse_x and mouse_y") {
        input.set_mouse_position({ 100, 200 });
        REQUIRE(event.has_value());
        REQUIRE(event->position.x == 100);
        REQUIRE(event->position.y == 200);
    }
}

TEST_CASE("Input::set_mouse_button_pressed updates mouse button state", "[mouse][unit]") {
    EventManager events;
    Input input(&events);

    SECTION("Updates the state of the given button") {
        input.set_mouse_button_pressed(MouseButton::Left, true);
        REQUIRE(input.is_mouse_pressed(MouseButton::Left));
        input.set_mouse_button_pressed(MouseButton::Left, false);
        REQUIRE_FALSE(input.is_mouse_pressed(MouseButton::Left));
    }
}

TEST_CASE("Input::set_mouse_button_pressed alloed multiple buttons to be pressed", "[mouse][unit]") {
    EventManager events;
    Input input(&events);

    input.set_mouse_button_pressed(MouseButton::Left, true);
    REQUIRE(input.is_mouse_pressed(MouseButton::Left));
    REQUIRE_FALSE(input.is_mouse_pressed(MouseButton::Right));
    REQUIRE_FALSE(input.is_mouse_pressed(MouseButton::Middle));

    input.set_mouse_button_pressed(MouseButton::Right, true);
    REQUIRE(input.is_mouse_pressed(MouseButton::Left));
    REQUIRE(input.is_mouse_pressed(MouseButton::Right));
    REQUIRE_FALSE(input.is_mouse_pressed(MouseButton::Middle));

    input.set_mouse_button_pressed(MouseButton::Middle, true);
    REQUIRE(input.is_mouse_pressed(MouseButton::Left));
    REQUIRE(input.is_mouse_pressed(MouseButton::Right));
    REQUIRE(input.is_mouse_pressed(MouseButton::Middle));

    input.set_mouse_button_pressed(MouseButton::Middle, false);
    REQUIRE(input.is_mouse_pressed(MouseButton::Left));
    REQUIRE(input.is_mouse_pressed(MouseButton::Right));
    REQUIRE_FALSE(input.is_mouse_pressed(MouseButton::Middle));

    input.set_mouse_button_pressed(MouseButton::Right, false);
    REQUIRE(input.is_mouse_pressed(MouseButton::Left));
    REQUIRE_FALSE(input.is_mouse_pressed(MouseButton::Right));
    REQUIRE_FALSE(input.is_mouse_pressed(MouseButton::Middle));

    input.set_mouse_button_pressed(MouseButton::Left, false);
    REQUIRE_FALSE(input.is_mouse_pressed(MouseButton::Left));
    REQUIRE_FALSE(input.is_mouse_pressed(MouseButton::Right));
    REQUIRE_FALSE(input.is_mouse_pressed(MouseButton::Middle));
}

TEST_CASE("Input::set_mouse_button_pressed triggers MouseDownEvent", "[mouse][unit][event]") {
    EventManager events;
    std::optional<MouseDownEvent> event = std::nullopt;
    events.on<MouseDownEvent>([&event](const MouseDownEvent& e) -> bool { event = e; return true; });
    Input input(&events);

    SECTION("Updates the state of the left button") {
        input.set_mouse_button_pressed(MouseButton::Left, true);
        REQUIRE(event.has_value());
        REQUIRE(event->button == MouseButton::Left);
    }

    SECTION("Updates the state of the right button") {
        input.set_mouse_button_pressed(MouseButton::Right, true);
        REQUIRE(event.has_value());
        REQUIRE(event->button == MouseButton::Right);
    }

    SECTION("Updates the state of the middle button") {
        input.set_mouse_button_pressed(MouseButton::Middle, true);
        REQUIRE(event.has_value());
        REQUIRE(event->button == MouseButton::Middle);
    }
}

TEST_CASE("Input::set_key_pressed allows multiple keys to be pressed", "[keyboard][unit]") {
    EventManager events;
    Input input(&events);

    input.set_key_pressed(Key::A, true);
    REQUIRE(input.is_key_pressed(Key::A));
    REQUIRE_FALSE(input.is_key_pressed(Key::B));
    REQUIRE_FALSE(input.is_key_pressed(Key::C));

    input.set_key_pressed(Key::B, true);
    REQUIRE(input.is_key_pressed(Key::A));
    REQUIRE(input.is_key_pressed(Key::B));
    REQUIRE_FALSE(input.is_key_pressed(Key::C));

    input.set_key_pressed(Key::C, true);
    REQUIRE(input.is_key_pressed(Key::A));
    REQUIRE(input.is_key_pressed(Key::B));
    REQUIRE(input.is_key_pressed(Key::C));

    input.set_key_pressed(Key::C, false);
    REQUIRE(input.is_key_pressed(Key::A));
    REQUIRE(input.is_key_pressed(Key::B));
    REQUIRE_FALSE(input.is_key_pressed(Key::C));

    input.set_key_pressed(Key::B, false);
    REQUIRE(input.is_key_pressed(Key::A));
    REQUIRE_FALSE(input.is_key_pressed(Key::B));
    REQUIRE_FALSE(input.is_key_pressed(Key::C));

    input.set_key_pressed(Key::A, false);
    REQUIRE_FALSE(input.is_key_pressed(Key::A));
    REQUIRE_FALSE(input.is_key_pressed(Key::B));
    REQUIRE_FALSE(input.is_key_pressed(Key::C));
}