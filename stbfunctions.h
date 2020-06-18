#ifndef STBFUNCTIONS_H
#define STBFUNCTIONS_H

#define MAXHITS 100

int getSTBRowHeader(int getRow, int rowcount, int colcount, bool newline);
int getSTBColHeader(int getCol, int rowcount, int colcount, bool newline);
int getSTBValue(int getRow, int getCol, int rowcount, int colcount, int data_offset, bool newline);
int search(const char *filename, int &row, int &col, int rowcount, int colcount, int dataoffset, const char *str, bool gui);

#endif
