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

#ifndef HELPMANUAL_H
#define HELPMANUAL_H

struct XMLNode;
class QTreeWidgetItem;

#include "ui_helpmanual.h"

class HelpManual : public QDialog, public Ui::HelpManual
{
	Q_OBJECT
	public:
		HelpManual(QString folderLocation, QWidget *parent=0);
		~HelpManual();
		void loadPage(QString file);
		void loadIndex(QString index);
	protected slots:
		void on_twContents_itemClicked(QTreeWidgetItem *item);
		void on_leIndexSearch_textChanged(QString text);
		void on_lwIndexList_itemClicked(QListWidgetItem *item);
		void on_webView_urlChanged();
		void on_twContents_customContextMenuRequested();
		void on_lwIndexList_customContextMenuRequested();
	private:
		void buildContentTree(QTreeWidgetItem *parent, XMLNode TreeNode);
		QMap<QString, QString> indexList;
		QString helpManualFolder;
};

#endif // HELPMANUAL_H
