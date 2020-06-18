#ifndef STB_H
#define STB_H

#include <list>
#include <cstdio>

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;

typedef struct  {
	BYTE				m_btUntouched; //0 if char pointer is from original malloc, 1 if realloc'd
	BYTE				m_btEdited; //0 if unedited since last save, 1 otherwise
	WORD				m_nLen;
	char				*m_pDATA;
} stbdatum;

typedef struct {
	char				m_szCode[4];
	DWORD				m_dwOffset;
	DWORD				m_dwRowCNT;
	DWORD				m_dwColCNT;

	DWORD									m_dwRowHeight;
	std::list<WORD>				*m_pColWidths;
	std::list<stbdatum>		*m_pColTitles;
	std::list<stbdatum>		*m_pRowTitles;
	stbdatum							*m_IDColName;
	std::list<std::list< stbdatum >>		*m_pCellData;
	BYTE				m_btEdited;

} sstb;

bool saveSTB(const char *szFile, sstb *s);

void writeDWORD(FILE *f, DWORD d);
void writeWORD(FILE *f, WORD w);
void writeDATUM(FILE *f, stbdatum &s);

#endif
