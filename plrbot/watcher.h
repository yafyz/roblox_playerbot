#pragma once

#include <windows.h>
#include <string>
#include "config.h"
#include "rbx.h"

namespace watcher {
    struct rbxMainArgs {
        config::cconfig* conf;
        rbx::roblox* roblox;
        int accIdx;
    };

    static DWORD WINAPI rbxMain(LPVOID pArgs_) {
        rbxMainArgs* args = (rbxMainArgs*)pArgs_;

        PROCESS_INFORMATION procInfo;
        config::cconfig conf = *(args->conf);
        rbx::roblox roblox = *(args->roblox);
        std::string placeId;
        int accIdx = args->accIdx;
        unsigned int placeIdx = 0;

        free(args);

        out::info("Watcher thread started, idx: %d\n", accIdx);

        while (true) {
            if (conf.placeIds.size()-1 < placeIdx)
                placeIdx = 0;
            placeId = conf.placeIds[placeIdx];
            {
                accs::account acc = conf.accounts[accIdx];
                acc.updateAuthTicket(placeId);
                if (!acc.isValid) {
                    out::warn("Watcher thread of idx %d quitting, Auth denied (cookie invalid)\n", accIdx);
                    return NULL;
                };
                out::info("Lanching roblox of idx %d, placeIdx: %d, place id: %s\n", accIdx, placeIdx, placeId.c_str());
                procInfo = roblox.launchClient(conf.getPlaceLauncher(placeId.c_str()), acc.authTicket.c_str());
            }
            if (WaitForSingleObject(procInfo.hProcess, 600000) == WAIT_TIMEOUT) // might do proper anti-afk later
                TerminateProcess(procInfo.hProcess, 0);
            CloseHandle(procInfo.hProcess);
            CloseHandle(procInfo.hThread);
            out::info("Roblox of idx %d exitted\n", accIdx);
            placeIdx++;
            Sleep(10000);
        }
        return NULL;
    }
}

