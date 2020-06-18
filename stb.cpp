#include "stb.h"


bool saveSTB(const char *szFile, sstb *s) {
	FILE *f = fopen(szFile, "wb");
	if(!f)
		return false;

	//write STB code
	fwrite(s->m_szCode, 1, 4, f);
	//SKIP offset
	fseek(f, 4, SEEK_CUR);
	//write rowcnt
	writeDWORD(f, s->m_dwRowCNT);
	//write colcnt
	writeDWORD(f, s->m_dwColCNT);
	//write rowheight
	writeDWORD(f, s->m_dwRowHeight);
	
	std::list<WORD>::iterator itword;
	std::list<stbdatum>::iterator itdatum;
	std::list<std::list<stbdatum>>::iterator itrow;
	//write colwidths
	for(itword = s->m_pColWidths->begin(); itword != s->m_pColWidths->end(); itword++)
		writeWORD(f, *itword);
	//write coltitles
	for(itdatum = s->m_pColTitles->begin(); itdatum != s->m_pColTitles->end(); itdatum++)
		writeDATUM(f, *itdatum);
	//write idcolname
	writeDATUM(f, *(s->m_IDColName));
	//write rowtitles
	for(itdatum = s->m_pRowTitles->begin(); itdatum != s->m_pRowTitles->end(); itdatum++)
		writeDATUM(f, *itdatum);
	
	//FTELL pos
	DWORD dwOffset = ftell(f);
	//seek to SET+4
	fseek(f, 4, SEEK_SET);
	//WRITE offset
	writeDWORD(f, dwOffset);
	//SEEK back to offset
	fseek(f, dwOffset, SEEK_SET);
	//write celldata...
	for(itrow = s->m_pCellData->begin(); itrow != s->m_pCellData->end(); itrow++)
		for(itdatum = itrow->begin(); itdatum != itrow->end(); itdatum++)
			writeDATUM(f, *itdatum);

	fclose(f);
	s->m_btEdited = 0;
	return true;
}

void writeDWORD(FILE *f, DWORD d) {
	fwrite(&d, sizeof(DWORD), 1, f);
}

void writeWORD(FILE *f, WORD w) {
	fwrite(&w, sizeof(WORD), 1, f);
}

void writeDATUM(FILE *f, stbdatum &s) {
	writeWORD(f, s.m_nLen);
	fwrite(s.m_pDATA, 1, s.m_nLen, f);
}
