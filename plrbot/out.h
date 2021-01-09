#pragma once

#define BACKGROUND 0x10
#define FOREGROUND_WHITE 0x7
#define FOREGROUND_YELLOW 0x6
#define FOREGROUND_BLACK 0

#include <windows.h>

namespace out {
	bool debugOut = false;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD backgroundColor;

	#define INTERNAL_OUT(color) { \
		SetConsoleTextAttribute(hConsole, color); \
		va_list ap; \
		va_start(ap, format); \
		vprintf(format, ap); \
		va_end(ap); \
	}
	static inline void info(const char* format, ...) INTERNAL_OUT(FOREGROUND_BLACK | backgroundColor);
	static inline void warn(const char* format, ...) INTERNAL_OUT(FOREGROUND_YELLOW | backgroundColor);
	static inline void error(const char* format, ...) INTERNAL_OUT(FOREGROUND_RED | backgroundColor);
	static inline void debug(const char* format, ...) {
		if (debugOut)
			INTERNAL_OUT(FOREGROUND_BLUE | FOREGROUND_INTENSITY | backgroundColor);
	}

	void updateBGColor() {
		char buf[16];
		sprintf_s(buf, "color %x", backgroundColor);
		system(buf);
	}
}