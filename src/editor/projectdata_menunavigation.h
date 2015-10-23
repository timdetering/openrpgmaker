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

#ifndef PROJECTDATA_MENUNAVIGATION_H
#define PROJECTDATA_MENUNAVIGATION_H

#include <QMap>
#include <QPoint>
#include <QString>
#include "storagefile.h"
#include "xmlParser.h"

class MenuNavigation
{
	public:
		MenuNavigation();
		MenuNavigation(XMLNode NavigationNode);
		MenuNavigation(StorageFile &storageFile);
		~MenuNavigation();
		void saveToStorageFile(StorageFile &storageFile);
		XMLNode getNavigationNode();
		QString objectNameAtLocation(QPoint location);
		QStringList getObjectNames();
		void addObject(QString name, QPoint location);
		void removeObject(QPoint location);
		bool canObjectBeRemoved(QPoint location);
		int getNumRows();
		int getNumColumns();
		int getMinRow();
		int getMinColumn();
		void changeObjectName(QString oldName, QString newName);
		void setWrapHorizontally(bool value);
		bool wrapsHorizontally();
		void setWrapVertically(bool value);
		bool wrapsVertically();
		bool hasNeighborAtLocation(QPoint location);
		static bool tryLoad(XMLNode NavigationNode);
	private:
		QMap<QPoint, QString> menuObjects;
		int numRows;
		int numColumns;
		int minRow;
		int minColumn;
		bool wrapHorizontally;
		bool wrapVertically;
};

#endif // PROJECTDATA_MENUNAVIGATION_H
