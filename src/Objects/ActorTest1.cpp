#include "ActorTest1.hpp"
#include "Events/MessageEvent.hpp"

namespace FNFE {

void ActorTest1::Update(double delta) {
  Actor::Update(delta);

  MessageEvent event(this, "Test2");
  SendEvent(event);
}

} // namespace FNFE
