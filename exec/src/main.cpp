#include <game/game.hpp>
int main() {

  bool exit = true;
  Init();
  InitGame();
  while (exit) {
    exit = Loop();
  }
  Cleanup();
}
