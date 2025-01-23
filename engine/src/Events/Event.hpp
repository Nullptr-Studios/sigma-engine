/**
 * @file event.hpp
 * @author Xein
 * @date 21/11/2024
 *
 * @brief Provides the base classes and macros to define and handle events in the FNF Engine.
 * @note This is literally brought from the Bloom Engine by Nullptr* Studios
 *
 * This file contains the foundational classes and structures for the event system used within
 * the FNF Engine. The event system is designed to facilitate communication between different
 * parts of the engine or game logic. Events are categorized and can be dynamically dispatched
 * using a dispatcher mechanism. The system ensures that events can be handled efficiently and
 * flexibly, while also supporting extension for new event types.
 */

#pragma once
#include <pch.hpp>

namespace FNFE {

/**
 * @enum EventType
 * @brief Represents the various types of events supported by the FNF Engine.
 *
 * Each value in the enum corresponds to a specific type of event that can occur
 * during the execution of the application. This enum is used to identify and
 * differentiate between event types, allowing for specific handling logic to
 * be applied.
 */
enum class EventType {
  NONE = 0,
  MESSAGE
};

/**
 * @macro EVENT_CLASS_TYPE
 * @brief Defines a macro to declare a static type for an event class.
 *
 * This macro simplifies the definition of the static and dynamic type system for events.
 * It declares functions to get the static event type, the dynamic event type, and the name of the event.
 *
 * @param type The event type to associate with the class.
 */
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; } \
                               virtual EventType GetEventType() const override { return GetStaticType(); } \
                               virtual const char* GetName() const override { return #type; }

// Event
/**
 * @class Event
 * @brief Abstract base class for all events in the FNF Engine.
 *
 * The @c Event class serves as the base for all event types. It provides a common interface for
 * retrieving the event type, name, and category flags. Events can also be converted to string
 * representations and checked against specific categories. Derived event classes must override
 * the pure virtual functions to provide specific details for each event type.
 */
class Event {
  friend class EventDispatcher;

protected:
  Event() = default;
  virtual ~Event() = default;

public:
  /**
   * @brief Gets the type of the event.
   *
   * This is a pure virtual function that must be implemented by all derived event classes.
   * It returns the type of the event, allowing it to be identified at runtime.
   *
   * @return The @c EventType of the event.
   */
  [[nodiscard]] virtual EventType GetEventType() const = 0;

  /**
   * @brief Gets the name of the event.
   *
   * This is a pure virtual function that must be implemented by all derived event classes.
   * It returns the name of the event as a string.
   *
   * @return A @code const char* @endcode representing the name of the event.
   */
  [[nodiscard]] virtual const char* GetName() const = 0;

  /**
   * @brief Converts the event to a string representation.
   *
   * This function returns a string representation of the event, primarily for debugging purposes.
   * Derived classes may override this function to provide more detailed representations.
   *
   * @return A @code std::string @endcode containing the string representation of the event.
   */
  [[nodiscard]] virtual std::string ToString() const { return GetName(); }

  /**
   * @brief Indicates whether the event has been handled.
   *
   * This function returns a boolean value indicating whether the event has been handled.
   * If the event has been handled, it should not be processed further.
   *
   * @return @c true if the event has been handled; otherwise, @c false.
   */
  [[nodiscard]]
  bool IsHandled() const { return m_handled; }

protected:
  bool m_handled = false; /**< Indicates whether the event has been handled. */
};

// Dispatcher
/**
 * @class EventDispatcher
 * @brief A utility class for dispatching events to appropriate handlers.
 *
 * The @c EventDispatcher class provides a mechanism for safely and efficiently
 * handling events by invoking user-defined callback functions. It ensures that
 * events are only handled by functions designed for their specific type.
 */
class EventDispatcher {
  template<typename T>
  using EventFn = std::function<bool(T&)>; /**< Type alias for an event handler function. */

public:
  /**
   * @brief Constructs an @c EventDispatcher for a given event.
   *
   * @param event The `Event` to be dispatched.
   */
  explicit EventDispatcher(Event& event) : m_event(event) {}

  /**
   * @brief Dispatches the event to a handler function if the types match.
   *
   * This function compares the type of the event with the static type of the handler's event type.
   * If they match, the handler function is called, and the event is marked as handled.
   *
   * @tparam T The type of event to handle.
   * @param func The handler function to invoke.
   * @return @c true if the event was handled; otherwise, @c false.
   */
  template<typename T>
  bool Dispatch(EventFn<T> func) {
    if (m_event.GetEventType() == T::GetStaticType()) {
      m_event.m_handled = func(*static_cast<T*>(&m_event));
      return true;
    }
    return false;
  }

private:
  Event& m_event;
};

}
