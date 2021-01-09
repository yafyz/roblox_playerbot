#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "account.h"
#include "out.h"

namespace config {
	class cconfig {
	public:
		std::string cookieFile;
		std::vector<std::string> placeIds;
		std::vector<accs::account> accounts;

		void open(const char* config_path) {
			out::debug("Opening config file at \"%s\"\n", config_path); // open config file
			std::ifstream f(config_path);
			if (!f.is_open()) {
				out::error("Can't open config file\n");
				exit(1);
			}

			std::string line; // parse config file
			while (std::getline(f, line)) {
				int spl = line.find("=");
				std::string var = line.substr(0, spl);
				std::string val = line.substr(spl + 1);
				if (var == "cookie_file")
					cookieFile = val;
				else if (var == "place_ids") {
					std::istringstream ss(val);
					while (std::getline(ss, line, ',')) {
						out::debug("PlaceId: %s\n", line);
						placeIds.push_back(line);
					}
				} else if (var == "debug") {
					if (val == "true")
						out::debugOut = true;
				} else
					out::warn("Unkown setting \"%s\" with value \"%s\"\n", var.c_str(), val.c_str());
			}

			if (placeIds.size() < 1) {
				out::error("No place IDs specified\n");
				exit(1);
			}
			if (cookieFile == "") {
				out::error("No cookie file specified\n");
				exit(1);
			}
		}

		char* getPlaceLauncher(const char* placeId) {
			char url[128];
			sprintf_s(url, "https://assetgame.roblox.com/game/PlaceLauncher.ashx?request=RequestGame&placeId=%s&isPlayTogetherGame=false", placeId);
			return url;
		}

		void openCookies() {
			out::debug("Opening cookie file at \"%s\"\n", cookieFile.c_str());
			std::ifstream f(cookieFile);
			if (!f.is_open()) {
				out::error("Can't open cookie file\n");
				exit(1);
			}

			std::string line;
			while (std::getline(f, line))
				accounts.push_back(accs::account(line));
		}
	};
}