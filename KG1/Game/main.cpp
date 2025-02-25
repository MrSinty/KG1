#include "Game.h"
#include "GameComponent.h"

#include "../Debug/DXDebug.h"


int main()
{
    if (Game::Get().Init(L"3DAPP", 800, 800))
    {
        Game::Get().Run();
        while (!Game::Get().GetShouldClose())
        {
            Game::Get().MessageHandler();
            Game::Get().Update();
            Game::Get().Draw();
        }

        Game::Get().Shutdown();
    }

}