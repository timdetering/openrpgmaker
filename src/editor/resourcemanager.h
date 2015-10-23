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
 *  Copyright (C) 2013, Justin Davis <tuxdavis@gmail.com>                   *
 ****************************************************************************/

#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "ui_resourcemanager.h"

class ResourceManager : public QDialog, public Ui::ResourceManager
{
	Q_OBJECT
	public:
		ResourceManager(QWidget *parent=0);
		~ResourceManager();
	protected slots:
		void on_bExport_clicked();
		void on_bImportExternal_clicked();
		void on_bImportORPGM_clicked();
		void on_buttonBox_helpRequested();
		void on_twResources_customContextMenuRequested(const QPoint &pos);
	private:
		void buildResourceTree();
		QTreeWidgetItem *getFolderTree(QString folderLocation);
		void writeXMLData(QString xmlData, QString fileLocation);
		QString getXMLData(QString fileLocation);
		void setCheckState(QTreeWidgetItem *item, Qt::CheckState state);
		void setCheckStateRecursively(QTreeWidgetItem *item, Qt::CheckState state);
		QList<QTreeWidgetItem*> attributeItems;
		QList<QTreeWidgetItem*> battleAnimationItems;
		QList<QTreeWidgetItem*> characterItems;
		QList<QTreeWidgetItem*> characterSpriteItems;
		QList<QTreeWidgetItem*> classItems;
		QList<QTreeWidgetItem*> commonEventItems;
		QList<QTreeWidgetItem*> conditionItems;
		QList<QTreeWidgetItem*> fontItems;
		QList<QTreeWidgetItem*> globalAnimationItems;
		QList<QTreeWidgetItem*> itemItems;
		QList<QTreeWidgetItem*> mainMenuItems;
		QList<QTreeWidgetItem*> monsterAnimationItems;
		QList<QTreeWidgetItem*> monsterItems;
		QList<QTreeWidgetItem*> monsterGroupItems;
		QList<QTreeWidgetItem*> popupMenuItems;
		QList<QTreeWidgetItem*> skillItems;
		QList<QTreeWidgetItem*> terrainItems;
		QList<QTreeWidgetItem*> tilesetItems;
		QList<QTreeWidgetItem*> vehicleItems;
		QList<QTreeWidgetItem*> fileItems;
		QList<int> mainMenuIDs;
		QList<int> popupMenuIDs;
		QStringList fileLocations;
};

#endif // RESOURCEMANAGER_H
