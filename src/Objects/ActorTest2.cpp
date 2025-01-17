#include "ActorTest2.hpp"

namespace FNFE {

bool ActorTest2::OnMessage(Object *sender) {
  std::cout << "Message received! (from: " << sender->GetName() << ")\n";
  return false;
}

} // namespace FNFE
