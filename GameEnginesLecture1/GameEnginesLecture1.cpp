#include "framework.h"
#include "GameEnginesLecture1.h"

#include "OgreGraphicsFramework.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    OgreGraphicsFramework ogreGraphicsFramework;

    while (!ogreGraphicsFramework.GetQuit())
        ogreGraphicsFramework.update();

    return 0;
}