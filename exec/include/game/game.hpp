#ifndef BANE_GAME
#define BANE_GAME
#ifdef BANE_GAME_EXPORTS
#define BANE_GAME_API __declspec(dllexport)
#else
#define BANE_GAME_API __declspec(dllimport)
#endif
struct SDL_Window;
extern "C" BANE_GAME_API void Init();
// Most likely required for use in-editor
extern "C" BANE_GAME_API void SetSDLWindow(SDL_Window *window);
extern "C" BANE_GAME_API void InitGame();
extern "C" BANE_GAME_API bool Loop();
extern "C" BANE_GAME_API void Cleanup();
void ResetGame();

#endif
