#ifndef STBWIDGET_H
#define STBWIDGET_H

#include <QtWidgets/QTableWidget>
#include <QtWidgets/QHeaderView>
#include "stb.h"

class STBWidget : public QTableWidget {
Q_OBJECT
	public:
		STBWidget();
		char *stitle;
		const char *sfilename;
		int rowcount;
		int colcount;
		int dataoffset;
		QHeaderView *hHeader;
		QHeaderView *vHeader;
		sstb *m_psstb;
		std::list<std::list<stbdatum>> *m_pCellList;
	public slots:
		void updateTitle();
		void cellChange(QTableWidgetItem *item);
		void colHdrChange(int col);
		void rowHdrChange(int row);
		void save(bool checked=false);
		void saveAs(bool checked=false);
	protected:
		virtual void keyPressEvent(QKeyEvent *event);
	private:
		unsigned char hhurowcol;
		int icurrow;
		int icurcol;
		void updateCurrentCellSelection(int i);
		char searchstr[100];
		void getSearchString();
		void findNext();
};

#endif
