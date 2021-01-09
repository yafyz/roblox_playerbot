#include <iostream>
#include "config.h"
#include "rbx.h"
#include "watcher.h"

int main()
{
    out::backgroundColor = FOREGROUND_WHITE * BACKGROUND | BACKGROUND_INTENSITY;
    out::updateBGColor();

    SetConsoleTitleA("fyz's shitty playerbot");
    out::info("Consider donating some BTC to bc1qpxh9vkx4w2r2vcu3ynz7aukfds6szuaut9553z\n\n");

    config::cconfig conf;
    rbx::roblox roblox;
    std::vector<HANDLE> vInstances;

    roblox.locateExecutable();
    roblox.singleInstanceBypass();
    conf.open("files/conf");
    conf.openCookies();
    
    out::info("Loaded %d cookies\n", conf.accounts.size());
    
    int idx = 0;
    watcher::rbxMainArgs argz;
    for (accs::account acc : conf.accounts) {
        argz = {&conf, &roblox, idx};
        watcher::rbxMainArgs* args = (watcher::rbxMainArgs*)malloc(sizeof(watcher::rbxMainArgs));
        if (args == NULL) {
            out::error("Are you that broke that you can't afford more RAM\n");
            exit(1);
        }
        memcpy(args, &argz, sizeof(watcher::rbxMainArgs));
        vInstances.push_back(CreateThread(NULL, NULL, watcher::rbxMain, args, 0, NULL));
        idx++;
    }

    for (HANDLE hThread : vInstances) {
        if (hThread != NULL)
            WaitForSingleObject(hThread, -1);
    }

    SetConsoleTextAttribute(out::hConsole, FOREGROUND_BLACK | FOREGROUND_WHITE * BACKGROUND | BACKGROUND_INTENSITY);
}
