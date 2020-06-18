#include "stbglobal.h"
#include "stbfunctions.h"
#include <cstring>
#include <cstdlib>

//THIS IS SO BROKEN
int search(const char *filename, int &row, int &col, int rowcount, int colcount, int dataoffset, const char *str, bool gui) {
	if (row < 0 || col > rowcount - 1) return -1;
	if (row < 0 || col > colcount - 1) return -1;
	
	stbfile = fopen(filename, "rb");

	short unsigned len;
	int hitcount = 0;
	int searchlen = strlen(str);
	fseek(stbfile, dataoffset, 0);
	for(int currow = row; currow < rowcount-1; currow++) {
		for(int curcol = col; curcol < colcount-1; curcol++) {
			fread(&len, 2, 1, stbfile);
			fread(buffer, 1, len, stbfile);
			for(int k = 0; k < searchlen; k++) {
				if(str[k] != buffer[k]) {
					break;
				} else {
					if(k == searchlen - 1) {
						if(gui) {
							row = currow+1;
							col = curcol;
							fclose(stbfile);
							return 1;
						} else {
							printf("Hit %d: %d,%d\n", ++hitcount, currow+1, curcol+1);
							if(hitcount == MAXHITS) { fclose(stbfile); return hitcount; }
						}
					}
				}
			}
		}
	}
}

int getSTBValue(int getRow, int getCol, int rowcount, int colcount, int data_offset, bool newline) {
	if (getRow < 1 || getRow > rowcount - 2) return -1;
	if (getCol < 1 || getCol > colcount - 2) return -1;

	short unsigned len = 0;
	fseek(stbfile, data_offset, 0);
	for (int row = 0; row < getRow - 1; row++) {
		for (int col = 0; col < colcount - 1; col++) {
			fread(&len, 2, 1, stbfile);
			fseek(stbfile, len, SEEK_CUR);
		}
	}
	for (int col = 0; col < getCol - 1; col++) {
		fread(&len, 2, 1, stbfile);
		fseek(stbfile, len, SEEK_CUR);
	}
	fread(&len, 2, 1, stbfile);
	fread(buffer, 1, len, stbfile);
	char *value = (char *)malloc(len+1);	
	memcpy(value, buffer, len);
	value[len] = '\0';
	printf("%s", value);
	if (newline) printf("\n");
	return 0;
}

int getSTBRowHeader(int getRow, int rowcount, int colcount, bool newline) {
	short unsigned len;
	fseek(stbfile, (4+(colcount+1)*2), SEEK_CUR);
	for (int i = 0; i < colcount + 1; i++) {
		fread(&len, 2, 1, stbfile);
		fseek(stbfile, len, SEEK_CUR);
	}
	for (int i = 0; i < getRow - 1; i++) {
		fread(&len, 2, 1, stbfile);
		fseek(stbfile, len, SEEK_CUR);
	}
	fread(&len, 2, 1, stbfile);
	fread(buffer, 1, len, stbfile);
	char *rheader = (char *)malloc(len + 1);
	memcpy(rheader, buffer, len);
	rheader[len] = 0x00;
	printf("%s", rheader);
	if (newline) printf("\n");
	return 0;
}

int getSTBColHeader(int getCol, int rowcount, int colcount, bool newline) {
	short unsigned len;
	fseek(stbfile, (4+(colcount+1)*2), SEEK_CUR);
	for (int i = 0; i < getCol; i++) {
		fread(&len, 2, 1, stbfile);
		fseek(stbfile, len, SEEK_CUR);
	}
	fread(&len, 2, 1, stbfile);
	fread(buffer, 1, len, stbfile);
	char *cheader = (char *)malloc(len + 1);
	memcpy(cheader, buffer, len);
	cheader[len] = 0x00;
	printf("%s", cheader);
	if (newline) printf("\n");
	return 0;
}


