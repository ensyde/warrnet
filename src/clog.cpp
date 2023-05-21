#include <ctype.h>
#include "clog.h"

CLog::CLog(std::string FilePath, std::string Mode) {
	// If the mode is "s", output to stdout
	if ((Mode == "s") || (FilePath == "-")) {
		File = stdout;
		return;
	}

	File = fopen(FilePath.c_str(), Mode.c_str());
	if (!File) {
		fprintf(stderr, "Fatal Error: Cannot open file %s for logging.\n", FilePath.c_str());
		exit (1);
   }
}

CLog::~CLog(void) {
	fclose (File);
}

void CLog::Write(const char *Fmt, ...) {
	char Out[1024];
	va_list argptr;

	Out[sizeof(Out) - 1] = '\0';

	va_start(argptr, Fmt);
	vsnprintf(Out, sizeof(Out) - 1, Fmt, argptr);
	va_end(argptr);

	fputs(Out, File);
	fflush(File);
}

void CLog::HexDump(u32 Size, const u8 *Data) {
	FILE *Logfile = File;
	int i, offset, stop;
	u8 *p;
	for (offset = 0; offset < Size; offset += 16) {
		fprintf(Logfile, "%04X   ", offset);
		fflush(Logfile);
		p = (u8 *)&Data[offset];
		stop = ((offset + 16) > Size) ? Size - offset : 16;
		for (i = 0; i < stop; i++) {
			if (i == 8)
				fputs("- ", Logfile);

			fprintf(Logfile, "%02X ", p[i]);
			fflush(Logfile);
		}

		while (i < 16) {
			if (i++ == 8)
				fputs("- ", Logfile);
			
			fputs("   ", Logfile);
			fflush(Logfile);
		}

		fputs("  ", Logfile);
		fflush(Logfile);
		for (i = 0; i < stop; i++)
			fputc(isgraph(p[i]) ? p[i] : p[i] == ' ' ? ' ' : '.', Logfile);

		fputc('\n', Logfile);
		fflush(Logfile);
	}
}

void CLog::HexDump(u32 Size, const char *Data) {
	HexDump( Size, (const u8 *)Data);
}
