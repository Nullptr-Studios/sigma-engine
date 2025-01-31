/**
 * @file Scene.hpp
 * @author Dario
 * @date 11/01/2025
 *
 * @brief [Brief description of the file's purpose]
 */

#pragma once


namespace sigma {

class Scene {
public:
  /**
   * @brief Construct a new Scene object
   *
   * @param name
   * @param ID
   */
  Scene(const char* name, unsigned ID)
  {
    m_name = name;
    m_ID = ID;
  }

  ~Scene() = default;

  virtual void Load() {}
  virtual void Init() {}
  virtual void Update(double delta) {}
  virtual void Draw() {}
  virtual void Free() {}
  virtual void Unload() {}

  /**
   * @brief Get the name of this scene
   *
   * @return const char*
   */
  const char* GetName() const { return m_name; }

  /**
   * @brief Get the ID of this scene
   *
   * @return unsigned
   */
  unsigned GetID() const { return m_ID; }

private:
  const char* m_name;
  unsigned m_ID;

};

} // sigma

