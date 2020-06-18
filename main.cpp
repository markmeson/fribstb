#include <QtWidgets/QApplication>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMenu>
#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include "jpkoconv.h"
#include "stbwidget.h"
#include "stbfunctions.h"
#include "stb.h"

using namespace std;

static FILE *fstbout;

char *buffer;
FILE *stbfile;

void usage() {
		cout << "Usage: fribstb STBFILENAME [-i (info) | -J (non-jp) | -w (write to file) | -v ROW COL [n] | -r ROWHEADER# | -c COLHEADER# | -s [SEARCHSTRING]  | -x ]" << endl;
		cout << "Exiting..." << endl;
}

int main(int argc, char *argv[])
{
	buffer = (char *)malloc(2048);
	int stbSize;
	int getRow, getCol;

	if(argc < 2) {
		usage();
		return -1;
	}

	bool readonly = true;
	bool singlevalue = false;
	bool bconvert = false;
	bool nonjp = false;
	bool fileinfo = false;
	char get_type = 0x00;
	bool newline = true;
	bool strsearch = false;

	if(argc > 2) {
		if(argv[2][0] == '-') {
			//program options supplied
			switch (argv[2][1]) {
				case 'J':
					nonjp = true;
					break;
				case 'w':
					readonly = false;
					break;
				case 'v': { // get cell data
					if (argc < 5) { usage(); return -1; }
					singlevalue = true;
					get_type = 'v';
					getRow = atoi(argv[3]);
					getCol = atoi(argv[4]);
					if (argc > 5)
					if (argv[5][0] != 'n') { usage(); return -1; } 
					else newline = false;
					break;
				}
				case 'r': { // get row header
					if (argc < 4) { usage(); return -1; }
					singlevalue = true;
					get_type = 'r';
					getRow = atoi(argv[3]);
					if (argc > 4)
					if (argv[4][0] != 'n') { usage(); return -1; }
					else newline = false;
					break;
				}
				case 'c': { // get col header
					if (argc < 4) { usage(); return -1; }
					singlevalue = true;
					get_type = 'c';
					getCol = atoi(argv[3]);
					if (argc > 4)
					if (argv[4][0] != 'n') { usage(); return -1; }
					else newline = false;
					break;
				}
				case 'i': fileinfo = true; break; // get file info
				case 'x': {
					printf("Converting %s...\n", argv[1]);
					bconvert = true; // convert file from original to NON-SJIS/EUC-KR	
					sprintf(buffer, "./out/%s", argv[1]);
					fstbout = fopen(buffer, "wb");
					break; 
				}
				case 's': {
					if (argc > 4) { usage(); return -1; }
					strsearch = true;
					break;
				}
				default:
					cout << "Unknown Argument: " << argv[2] << endl;
					return -1;
			}
		} else {
			cout << "Unknown Argument: " << argv[2] << endl;
			return -1;
		}
	}

	if(!(stbfile = fopen(argv[1], "rb"))) {
		cout << "Unable to load \"" << argv[1] << "\". Exiting..." << endl;
		return -1;
	}

	fseek(stbfile, 0, SEEK_END);
	stbSize = ftell(stbfile);
	fseek(stbfile, 0, SEEK_SET);


	std::list<WORD>										lsColWidths;
	std::list<stbdatum>								lsColTitles;
	std::list<stbdatum>								lsRowTitles;
	std::list<std::list<stbdatum>>		lsCellData;
	stbdatum													IDColName;

	sstb mainstb;
	mainstb.m_pColWidths = &lsColWidths;
	mainstb.m_pColTitles = &lsColTitles;
	mainstb.m_pRowTitles = &lsRowTitles;
	mainstb.m_pCellData = &lsCellData;
	mainstb.m_IDColName = &IDColName;

	fread(buffer, 1, 4, stbfile); if(bconvert) fwrite(buffer, 1, 4, fstbout);
	buffer[4] = '\0';
	if(strcmp("STB1", buffer)) {
		cout << "File \"" << argv[1] << "\" is not an STB file!" << endl;
		cout << "Exiting..." << endl;
		fclose(stbfile);
		return -1;
	}

	memcpy(&mainstb.m_szCode, buffer, 4);

	int data_offset;
	int rowcount;
	int colcount;
	int rowheight;

	int loaded = 0;

	int odata_offset = ftell(stbfile);
	fread(&data_offset, 4, 1, stbfile);	 // offset from file start where cell data begins
	if(bconvert) fwrite(&data_offset, 4, 1, fstbout);

	fread(&rowcount, 4, 1, stbfile); if(bconvert) fwrite(&rowcount, 4, 1, fstbout);
	fread(&colcount, 4, 1, stbfile); if(bconvert) fwrite(&colcount, 4, 1, fstbout);

	if(strsearch) {
		fclose(stbfile);
		int r = 0; int c = 0;
		search(argv[1], r, c, rowcount, colcount, data_offset, argv[3], false);
		fclose(stbfile);
		return 0;
	}

	if (singlevalue)  {
		switch (get_type) {
			case 'v': 
				if (getRow < 1 || getRow > rowcount || getCol < 1 || getCol > colcount) return -1;
				else return getSTBValue(getRow, getCol, rowcount, colcount, data_offset, newline);
			case 'r':
				if (getRow < 1 || getRow > rowcount) return -1;
				else return getSTBRowHeader(getRow, rowcount, colcount, newline);
			case 'c':
				if (getCol < 0 || getCol > colcount) return -1;
				else return getSTBColHeader(getCol, rowcount, colcount, newline);
		}
	}

// VVVVVV --- LOAD WHOLE FILE / CONVERT --- VVVVVVV

	fread(&rowheight, 4, 1, stbfile);	if(bconvert) fwrite(&rowheight, 4, 1, fstbout);

	mainstb.m_dwOffset = data_offset;
	mainstb.m_dwRowCNT = rowcount;
	mainstb.m_dwColCNT = colcount;
	mainstb.m_dwRowHeight = rowheight;

	short colwidth;
	for (int i = 0; i < colcount + 1; i++) {
		fread(&colwidth, 2, 1, stbfile);//jrose version: if(bconvert) fwrite(&colwidths[i], 2, 1, fstbout);
		mainstb.m_pColWidths->push_back(colwidth);
	}

	unsigned short len = 0;
	//	char *coltitles[colcount];
	for (int i = 0; i < colcount; i++) {
		stbdatum newcoltitle;
		fread(&newcoltitle.m_nLen, 2, 1, stbfile);
		/* old jrose version: 
		if(bconvert) { 
			if (nonjp) fread(buffer, len, 1, stbfile);
			else Convert(buffer, &len, stbfile);
			fwrite(&len, 2, 1, fstbout);
			fwrite(buffer, 1, len, fstbout);
		} else { */
			newcoltitle.m_pDATA = (char *)malloc(newcoltitle.m_nLen+1);
			fread(newcoltitle.m_pDATA, 1, newcoltitle.m_nLen, stbfile);
			newcoltitle.m_pDATA[newcoltitle.m_nLen] = '\0';
			newcoltitle.m_btUntouched = 0; //Not part of shared mem block
			newcoltitle.m_btEdited = 0;
			mainstb.m_pColTitles->push_back(newcoltitle);
//		}
	}
	cout << endl;

	len = 0;
	//char *idcolumnname; // no idea what this is
	fread(&mainstb.m_IDColName->m_nLen, 2, 1, stbfile);
	/* jrose version:
	if(bconvert) { 
		if (nonjp) fread(buffer, len, 1, stbfile);
		else Convert(buffer, &len, stbfile);
		fwrite(&len, 2, 1, fstbout);
		fwrite(buffer, 1, len, fstbout);
	} else { */
		mainstb.m_IDColName->m_pDATA = (char *)malloc(mainstb.m_IDColName->m_nLen+1);	
		fread(mainstb.m_IDColName->m_pDATA, 1, mainstb.m_IDColName->m_nLen, stbfile);
		mainstb.m_IDColName->m_pDATA[mainstb.m_IDColName->m_nLen] = '\0';
//	}

	if(fileinfo) {
		printf("%s:\nRows: %d\nCols: %d\nData: 0x%.8X\nID Column Name: %s\n", argv[1], rowcount, colcount, data_offset, mainstb.m_IDColName->m_pDATA);
		fclose(stbfile);
		return 0;
	}

	len = 0;
//	char *rowtitles[rowcount - 1];
	loaded = 0;
	for (int i = 0; i < rowcount - 1; i++) {
		stbdatum newrowtitle;
		fread(&newrowtitle.m_nLen, 2, 1, stbfile);
		/* old jrose version:
		if(bconvert) { 
			if (nonjp) fread(buffer, len, 1, stbfile);
			else Convert(buffer, &len, stbfile);
			fwrite(&len, 2, 1, fstbout);
			fwrite(buffer, 1, len, fstbout);
		} else { */
			newrowtitle.m_pDATA = (char *)malloc(newrowtitle.m_nLen+1);	
			fread(newrowtitle.m_pDATA, 1, newrowtitle.m_nLen, stbfile);
			newrowtitle.m_pDATA[newrowtitle.m_nLen] = '\0';
			newrowtitle.m_btUntouched = 0; //Not part of shared mem block
			newrowtitle.m_btEdited = 0;
			mainstb.m_pRowTitles->push_back(newrowtitle);
//		}
	}

	len = 0;
	int done = rowcount * colcount;
//	char *celldata[rowcount - 1][colcount - 1];
	char *stbdata = (char *)malloc((stbSize - data_offset) * 2); //more than enough room for all data + null terminators
	char *pnext = stbdata;

	/* old jrose version:
	if(bconvert) {
		//must update new data offset (headers etc changed size)
		data_offset = ftell(fstbout);
		fseek(fstbout, odata_offset, SEEK_SET);
		fwrite(&data_offset, 4, 1, fstbout);
		fseek(fstbout, data_offset, SEEK_SET);
	} */
	loaded = 0;
	printf("Loading (0%)");
	std::list<stbdatum> lsNextRow;
	for (int row = 0; row < rowcount - 1; row++) {
		lsNextRow.clear();
		for (int col = 0; col < colcount - 1; col++) {
			stbdatum nextdatum;
			nextdatum.m_btUntouched = 1;
			nextdatum.m_btEdited = 0;
			nextdatum.m_pDATA = pnext;
			fread(&nextdatum.m_nLen, 2, 1, stbfile);
		/*old jrose version:
		 * if(bconvert) { 
				if (nonjp) fread(buffer, len, 1, stbfile);
				else Convert(buffer, &len, stbfile);
				fwrite(&len, 2, 1, fstbout);
				fwrite(buffer, 1, len, fstbout);
			} else { */
				fread(pnext, 1, nextdatum.m_nLen, stbfile);
				pnext[nextdatum.m_nLen] = '\0'; //STB data is not null-terminated!
				lsNextRow.push_back( nextdatum );
				pnext += nextdatum.m_nLen+1;
			//}
		}
		mainstb.m_pCellData->push_back( lsNextRow );
		if (loaded < 10) printf("\b\b\b"); else printf("\b\b\b\b");
		loaded = ((row + 1) * 100) / (rowcount - 1);
		printf("%d%%)", loaded);
	}
	printf("\nDone.\n");
  fclose(stbfile);
	if(bconvert) { fclose(fstbout); return 0; }

/* ************* BEGIN QT CODE ********************** */

	int colw = 300;
	int rowh = 30;
	int winw = (colcount * colw - 150);
	int winh = (rowcount) * rowh;
	if (winw > 1700) winw = 1700;
	if (winh > 900) winh = 900;

	QApplication app (argc, argv);
	STBWidget *table = new STBWidget;
	mainstb.m_btEdited = 0;
	table->m_psstb = &mainstb;
	table->m_pCellList = &lsCellData;
	table->setRowCount(rowcount - 1);
	table->setColumnCount(colcount - 1);
	table->sfilename = argv[1];
//	if(nonjp)
		sprintf(buffer, "Frib's STB Editor v1.0 - %s (%dx%d)", table->sfilename, rowcount - 1, colcount - 1);
//	else
//		sprintf(buffer, "JRose STB Editor v1.0 - %s (%dx%d)", table->sfilename, rowcount - 1, colcount - 1);
	table->stitle = (char *)malloc(strlen(buffer)+1);
	strcpy(table->stitle, buffer);
	table->rowcount = rowcount;
	table->colcount = colcount;
	table->dataoffset = data_offset;
	table->setGeometry(100,100, winw, winh);
	table->setEditTriggers(QAbstractItemView::AnyKeyPressed | QAbstractItemView::DoubleClicked );

	QTableWidgetItem *none = new QTableWidgetItem("");

	int colnum = 0;
	std::list<WORD>::iterator itwidth;
	for (itwidth = mainstb.m_pColWidths->begin(); itwidth != mainstb.m_pColWidths->end(); itwidth++) {
		table->setColumnWidth(colnum, *itwidth);
		colnum++;
	}
	std::list<stbdatum>::iterator ittitle;
	colnum = 0;
	for (ittitle = mainstb.m_pColTitles->begin(); ittitle != mainstb.m_pColTitles->end(); ittitle++) {
		table->setHorizontalHeaderItem(colnum-1, new QTableWidgetItem(ittitle->m_pDATA));
		colnum++;
	}
	table->setRowHeight(0, rowh);
	int rownum = 0;
	for (ittitle = mainstb.m_pRowTitles->begin(); ittitle != mainstb.m_pRowTitles->end(); ittitle++) {
		table->setRowHeight(rownum, rowh);
		table->setVerticalHeaderItem(rownum, new QTableWidgetItem(ittitle->m_pDATA));
		rownum++;
	}

	rownum = 0;
//	for (int i = 0; i < rowcount - 1; i++) {
//		for (int j = 0; j < colcount - 1; j++) {
	std::list<std::list<stbdatum>>::iterator itrow;
	std::list<stbdatum>::iterator itcell;
	for( itrow = mainstb.m_pCellData->begin(); itrow != mainstb.m_pCellData->end(); itrow++ ) {
		colnum = 0;
		for( itcell = itrow->begin(); itcell != itrow->end(); itcell++) {
			table->setItem(rownum, colnum, new QTableWidgetItem(itcell->m_pDATA));
			colnum++;
		}
		rownum++;
	}

	QObject::connect(table, SIGNAL(itemSelectionChanged()), table, SLOT(updateTitle()));
	//QObject::connect(table, SIGNAL(currentItemChanged(QTableWidgetItem *, QTableWidgetItem *)), table, SLOT(test(QTableWidgetItem *, QTableWidgetItem *)));
	QObject::connect(table, SIGNAL(itemChanged(QTableWidgetItem *)), table, SLOT(cellChange(QTableWidgetItem *)));
	QObject::connect(table->hHeader, SIGNAL(sectionClicked(int)), table, SLOT(colHdrChange(int)));
	QObject::connect(table->vHeader, SIGNAL(sectionClicked(int)), table, SLOT(rowHdrChange(int)));

	QMenuBar *mb = new QMenuBar(table);
	QMenu *fileMenu = mb->addMenu("File");
	QAction saveAction("Save", table);
	QAction saveAsAction("Save As", table);
	QObject::connect(&saveAction, SIGNAL(triggered(bool)), table, SLOT(save(bool)));
	QObject::connect(&saveAsAction, SIGNAL(triggered(bool)), table, SLOT(saveAs(bool)));
	fileMenu->addAction(&saveAction);
	fileMenu->addAction(&saveAsAction);

	table->setSelectionMode(QAbstractItemView::SingleSelection);
	table->setCurrentCell(0,0);
	table->updateTitle();
	table->show();

	app.exec();
	return 0;
}
