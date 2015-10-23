/****************************************************************************
 *  Open RPG Maker is free software: you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by    *
 *  the Free Software Foundation, either version 3 of the License, or       *
 *  (at your option) any later version.                                     *
 *                                                                          *
 *  Open RPG Maker is distributed in the hope that it will be useful,       *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *  GNU General Public License for more details.                            *
 *                                                                          *
 *  You should have received a copy of the GNU General Public License       *
 *  along with Open RPG Maker. If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                          *
 *  ---                                                                     *
 *  Copyright (C) 2010, Justin Davis <tuxdavis@gmail.com>                   *
 ****************************************************************************/

#ifndef MAPEVENTEDITOR_H
#define MAPEVENTEDITOR_H

class MapEvent;

#include <QDialog>
#include "mapeventpageeditor.h"
#include "projectdata.h"
#include "ui_mapeventeditor.h"

class MapEventEditor : public QDialog, public Ui::MapEventEditor
{
	Q_OBJECT
	public:
		MapEventEditor(int mapID, int eventID, QWidget *parent=0);
		~MapEventEditor();
	protected slots:
		void on_leName_textChanged(QString text);
		void on_bNewPage_clicked();
		void on_bDeletePage_clicked();
		void on_bCopyPage_clicked();
		void on_bPastePage_clicked();
		void on_buttonBox_clicked(QAbstractButton *button);
		void eventCopied();
	private:
		QList<MapEventPageEditor*> pageWidgets;
		ProjectData::DataReference<Map> *mapRef;
		MapEvent *event;
};

#endif // MAPEVENTEDITOR_H
