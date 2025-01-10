#include <iostream>
#include <aecore/AEEngine.h>

int main() {
  AESysInit("Bloom engine but worse");
  AESysShowConsole();

  AESysGameLoop();

  return 0;
}
