#include "Game.h"
#include "GameComponent.h"
#include "TriangleComponent.h"

#include "../Debug/DXDebug.h"


int main()
{

    if (Game::Get().Init(L"3DAPP", 800, 800))
    {
        Game::Get().Run();
        auto triangle1 = new TriangleComponent(Game::Get().GetDevice(), Game::Get().GetDeviceContext());
        triangle1->Init(Game::Get().GetDevice(), Game::Get().GetDeviceContext());
        triangle1->SetPosition(-0.5f, 0.5f);

        auto triangle2 = new TriangleComponent(Game::Get().GetDevice(), Game::Get().GetDeviceContext());
        triangle2->Init(Game::Get().GetDevice(), Game::Get().GetDeviceContext());
        triangle2->SetPosition(0.5f, -0.5f);

        Game::Get().AddGameComponent(triangle1);
        Game::Get().AddGameComponent(triangle2);

        while (!Game::Get().GetShouldClose())
        {
            Game::Get().MessageHandler();
            Game::Get().Update();
            Game::Get().Draw();
        }

        Game::Get().Shutdown();
    }

}