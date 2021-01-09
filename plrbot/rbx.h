#pragma once

#include <string>
#include <filesystem>
#include <windows.h>
#include <shlobj.h>
#include <sstream>

#include "out.h"

namespace rbx {
	class roblox {
	public:
		std::string executablePath;
		std::string workingDirectory;

		void locateExecutable() {
			LPWSTR wstrPath;
			HRESULT hr = SHGetKnownFolderPath(FOLDERID_LocalAppData, NULL, NULL, &wstrPath);
			char path[256];

			if (!SUCCEEDED(hr)) {
				out::error("Failed to get localappdata path\n");
				exit(1);
			}

			sprintf_s(path, "%ls\\Roblox\\Versions", wstrPath);

			if (!std::filesystem::is_directory(std::filesystem::status(path))) {
				out::error("%s is not a directory\n", path);
				exit(1);
			};

			for (std::filesystem::directory_entry entry : std::filesystem::directory_iterator(path))
				if (std::filesystem::is_directory(entry))
					for (std::filesystem::directory_entry file : std::filesystem::directory_iterator(entry))
						if (std::filesystem::is_regular_file(file) && file.path().filename().string() == "RobloxPlayerBeta.exe") {
							executablePath = file.path().string();
							workingDirectory = entry.path().string();
						}

			out::debug("Roblox Executable path: %s\nRoblox working directory: %s\n", executablePath.c_str(), workingDirectory.c_str());
		}

		PROCESS_INFORMATION launchClient(const char* placeLauncher, const char* authTicket) {
			STARTUPINFOA startupinfo;
			PROCESS_INFORMATION processinfo;
			char args[1024];

			sprintf_s(args, "--play -a https://www.roblox.com/Login/Negotiate.ashx -t %s -j %s", authTicket, placeLauncher);
			out::debug("Launch args: %s\n", args);

			memset(&startupinfo, 0, sizeof(STARTUPINFOA));
			memset(&processinfo, 0, sizeof(PROCESS_INFORMATION));

			CreateProcessA( // shut up please its not leaking, it gets closed later
				executablePath.c_str(),
				args,
				NULL,
				NULL,
				TRUE,
				NORMAL_PRIORITY_CLASS,
				NULL,
				workingDirectory.c_str(),
				&startupinfo,
				&processinfo
			);

			return processinfo;
		}

		void singleInstanceBypass() {
			CreateMutexA(NULL, TRUE, "ROBLOX_singletonMutex");
		}
	};
}