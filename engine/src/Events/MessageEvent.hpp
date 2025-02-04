/**
 * @file MessageEvent.h
 * @author Xein
 * @date 1/17/2025
 *
 * @brief [Brief description of the file's purpose]
 */

#pragma once
#include "Event.hpp"

namespace Sigma {

class Object;

/**
 * @class MessageEvent
 * @brief Basic event for messaging between Objects
 */
class MessageEvent : public Event {
public:
  /**
   * @param sender Reference to the object it's sending the message
   * @param receiver Name of the object that has to receive the message
   */
  explicit MessageEvent(Object* sender, const char* receiver = "FORCE_EVERYONE")
      : m_sender(sender), m_receiver(receiver) {}

  [[nodiscard]] Object* GetSender() const { return m_sender; } ///< @brief Returns the sender object
  const char* GetReceiver() const { return m_receiver; } ///< @brief Returns the receiver name

  /**
   * @brief Overrides the @c ToString function to provide a detailed string representation of the event.
   * @return A string containing the event details.
   */
  [[nodiscard]] std::stringstream ToString() const override {
    std::stringstream ss;
    ss << "Message Event to Object \"" << m_receiver << "\"";
    return ss;
  }

  EVENT_CLASS_TYPE(MESSAGE);

private:
  Object* m_sender;
  const char* m_receiver;
};

}
