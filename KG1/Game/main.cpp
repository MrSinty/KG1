#include "Game.h"
#include "Components/TriangleComponent.h"
#include "Platform.h"
#include "Ball.h"
#include "../DXDebug/DXDebug.h"


int main()
{

    if (Game::Get().Init(L"3DAPP", 800, 800))
    {
        Game::Get().Run(); // { 0,1,2, 1,0,3 }

        //std::vector<Vector2> vec2{
        //    Vector2 { -1.0f, 1.0f, },
        //    Vector2 { -0.9f, 0.5f, },
        //    Vector2 { -1.0f, 0.5f, },
        //    Vector2 { -0.9f, 1.0f, }
        //};

        while (!Game::Get().GetShouldClose())
        {
            Game::Get().MessageHandler();
            Game::Get().Update();
            Game::Get().Draw();
        }

        Game::Get().Shutdown();

    }

}