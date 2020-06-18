#include "stbwidget.h"
#include "stbglobal.h"
#include "stbfunctions.h"
#include <QtGui/QKeyEvent>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QInputDialog>


STBWidget::STBWidget() {
	hhurowcol = 0;
	icurrow = 0;
	icurcol = 0;
	searchstr[0] = 0x00;
	hHeader = this->horizontalHeader();
	vHeader = this->verticalHeader();
}

void STBWidget::findNext() {
		int row = currentRow();
		int col = currentColumn();
		if(search(sfilename, row, col, rowcount, colcount, dataoffset, searchstr, true) > 0)
			setCurrentCell(row, col);
}

void STBWidget::getSearchString() {
	bool ok = false;
	const char *ss = QInputDialog::getText(this, "Enter Search String", "", QLineEdit::Normal, "", &ok).toStdString().c_str();
	if(!ok || !strlen(ss)) return;
	strcpy(searchstr, ss);
	printf("%s\n", searchstr);
	findNext();
}

void STBWidget::updateTitle() {
	sprintf(buffer, "%s (%d:%d) (0x%hXx0x%hX)", stitle, currentRow(), currentColumn() + 1, currentRow(), currentColumn() + 1);
	setWindowTitle(buffer);
}

void STBWidget::colHdrChange(int col) {
	bool ok = false;
	std::list<stbdatum>::iterator ittitle = std::next(m_psstb->m_pColTitles->begin(), col+1);
	const char *newcolhdr = QInputDialog::getText(this, "Rename Column Header", "New Column Header:                                    ", QLineEdit::Normal, ittitle->m_pDATA, &ok).toLatin1();
	if(ok) {
		ittitle->m_nLen = strlen(newcolhdr);
		if(ittitle->m_btUntouched) { //Was still in the shared initial malloc block
			ittitle->m_btUntouched = 0;
		} else {
			free(ittitle->m_pDATA); //Was reallocated already, so free it
		}
		ittitle->m_pDATA = (char *)malloc(ittitle->m_nLen + 1);
		strcpy(ittitle->m_pDATA, newcolhdr);
		setHorizontalHeaderItem(col, new QTableWidgetItem(ittitle->m_pDATA));
		ittitle->m_btEdited = 1;
		m_psstb->m_btEdited = 1;
	}
}

void STBWidget::rowHdrChange(int row) {
	bool ok = false;
	std::list<stbdatum>::iterator ittitle = std::next(m_psstb->m_pRowTitles->begin(), row);
	const char *newrowhdr = QInputDialog::getText(this, "Rename Row Header", "New Row Header:                                     ", QLineEdit::Normal, ittitle->m_pDATA, &ok).toLatin1();
	if(ok) {
		ittitle->m_nLen = strlen(newrowhdr);
		if(ittitle->m_btUntouched) { //Was still in the shared initial malloc block
			ittitle->m_btUntouched = 0;
		} else {
			free(ittitle->m_pDATA); //Was reallocated already, so free it
		}
		ittitle->m_pDATA = (char *)malloc(ittitle->m_nLen + 1);
		strcpy(ittitle->m_pDATA, newrowhdr);
		setVerticalHeaderItem(row, new QTableWidgetItem(ittitle->m_pDATA));
		ittitle->m_btEdited = 1;
		m_psstb->m_btEdited = 1;
	}
}

void STBWidget::cellChange(QTableWidgetItem *item) {
	std::list<stbdatum>::iterator itcell;
	itcell = std::next((std::next( m_pCellList->begin(), item->row()))->begin(), item->column());
	const char *szNew = item->data(0).toString().toLatin1();
	itcell->m_nLen = strlen(szNew);
	if(itcell->m_btUntouched) { //Was still in the shared initial malloc block
		itcell->m_btUntouched = 0;
	} else {
		free(itcell->m_pDATA); //Was reallocated already, so free it
	}
	itcell->m_pDATA = (char *)malloc(itcell->m_nLen + 1);
	strcpy(itcell->m_pDATA, szNew);
	itcell->m_btEdited = 1;
	m_psstb->m_btEdited = 1;
}

void STBWidget::saveAs(bool checked) {
	bool ok = false;
	const char *saveasname = QInputDialog::getText(this, "Save STB File As...", "Save STB File As:                                                            ", QLineEdit::Normal, sfilename, &ok).toLatin1();;
	if(ok) {
		if(saveSTB(saveasname, m_psstb)) {
			sprintf(buffer, "STB successfully saved as \"%s\"", saveasname);
			QMessageBox::information(this, "File Saved!", buffer);
		} else {
			sprintf(buffer, "Error saving STB file \"%s\"!", saveasname);
			QMessageBox::critical(this, "Error!", buffer);
		}
	}
}

void STBWidget::save(bool checked) {
	FILE *foo;
	if(saveSTB(sfilename, m_psstb)) {
		sprintf(buffer, "STB successfully saved as \"%s\"", sfilename);
		QMessageBox::information(this, "File Saved!", buffer);
	} else {
		sprintf(buffer, "Error saving STB file \"%s\"!", sfilename);
		QMessageBox::critical(this, "Error!", buffer);
	}
}

void STBWidget::updateCurrentCellSelection(int i) {
	if (hhurowcol == ROW) { icurrow *= 10; icurrow += i; }
	if (hhurowcol == COL) { icurcol *= 10; icurcol += i; }
}

void STBWidget::keyPressEvent(QKeyEvent *event) {
	/* NEED TO REIMPLEMENT USING MODIFIER KEY */
	/*
	int ikeycode = event->nativeScanCode();
	switch(ikeycode) {
		case 27: //r (go to row)
			icurrow = 0; icurcol = currentColumn() + 1;
			hhurowcol = ROW; return;
		case 54: //c (go to column)
			icurcol = 0; icurrow = currentRow() + 1;
			hhurowcol = COL; return;
		case 39: //s (enter search string)
			getSearchString();
			return;
		case 10: //1
		case 87: updateCurrentCellSelection(1); return;
		case 11: //2
		case 88: updateCurrentCellSelection(2); return;
		case 12: //3
		case 89: updateCurrentCellSelection(3); return;
		case 13: //4
		case 83: updateCurrentCellSelection(4); return;
		case 14: //5
		case 84: updateCurrentCellSelection(5); return;
		case 15: //6
		case 85: updateCurrentCellSelection(6); return;
		case 16: //7
		case 79: updateCurrentCellSelection(7); return;
		case 17: //8
		case 80: updateCurrentCellSelection(8); return;
		case 18: //9
		case 81: updateCurrentCellSelection(9); return;
		case 19: //0
		case 90: updateCurrentCellSelection(0); return;
		case 36: //ENTER / RETURN
		case 104:
			if (hhurowcol != NONE) {
				if(icurrow && icurcol && icurrow < rowCount() + 1 && icurcol < columnCount() + 1) {
					setCurrentCell(icurrow - 1, icurcol - 1); 
					hhurowcol = NONE;
					return;
				} else {
					sprintf(buffer, "Out of bounds.");
					QMessageBox info; info.setText(buffer);
					info.exec();
					return;
				}
			}
			break;
		// keys we don't want to catch or get code for
		case 108: //ALTR
		case 64: //ALTL
		case 35: // ]
		case 37: //CTLL
		case 111: //arrows
		case 113:
		case 114:
		case 116:
			break;
			*/
		// keys we want to get code for
		/*default:		//uncomment to get key values
			sprintf(buffer, "%d", ikeycode); 
			QMessageBox info;
			info.setText(buffer);
			info.exec();
			return;
		*/
	//}
	QTableView::keyPressEvent(event);
}


