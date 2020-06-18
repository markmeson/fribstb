#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "jpkoconv.h"

FILE *ext;
FILE *utf8;
FILE *nonutf8;

void Convert(char *dest, unsigned short *size, FILE *ref) {
  fread(dest, 1, *size, ref);
  nonutf8 = fopen("tmp.nonutf8", "w");
  fwrite(dest, 1, *size, nonutf8);
  fclose(nonutf8);

	// does the buffer contain an EUC-KR string?
	ext = popen("file -ib tmp.nonutf8 | grep -o 8859", "r");
	char tmpb[10];
	fgets(tmpb, 10, ext);
	*size = strlen(tmpb);
	pclose(ext);

	if(*size == 5)
		utf8 = popen("iconv -c -f EUC-KR -t UTF8 tmp.nonutf8", "r"); //buffer = EUC-KR
	else
		utf8 = popen("nkf -Sw tmp.nonutf8", "r"); //buffer = Shift_JIS

	*size = fread(dest, 1, 2048, utf8);
	pclose(utf8);
}
