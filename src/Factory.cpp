#include "Factory.hpp"

namespace FNFE {

Factory *Factory::m_instance = nullptr;

void Factory::DestroyObject(id_t id) {
  m_objects[id]->Destroy();
  m_objects.erase(id);
}

void Factory::DestroyAllObjects() {
  for (auto& [id, obj] : m_objects) {
    obj->Destroy();
    obj.reset();
  }
  m_objects.clear();
}

}
