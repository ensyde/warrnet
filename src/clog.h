#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include "types.h"

class CLog {
	FILE *File;
public:
	CLog(std::string FilePath, std::string Mode);
   ~CLog(void);

	void Write(const char *Fmt, ...);

	void HexDump(u32 Size, const u8 *Data);
	void HexDump(u32 Size, const char *Data);
};

#endif
