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

#ifndef PROJECTDATA_MAINMENULAYOUT_H
#define PROJECTDATA_MAINMENULAYOUT_H

class BaseEvent;
class MenuNavigation;

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "menuobject.h"
#include "storagefile.h"
#include "xmlParser.h"

class MainMenuLayout
{
	public:
		friend class ProjectData;
		friend class MenuObject;
		MainMenuLayout();
		MainMenuLayout(XMLNode MenuNode);
		MainMenuLayout(StorageFile &storageFile);
		~MainMenuLayout();
		void saveToStorageFile(StorageFile &storageFile);
		QStringList getResources();
		XMLNode getMenuNode();
		MenuObject *getObjectByName(QString name);
		MenuObject *getTopLevelObject(int objectID);
		void changeObjectName(QString oldName, QString newName);
		void addTopLevelObject(MenuObject *object);
		void addObject(MenuObject *object);
		void deleteTopLevelObject(MenuObject *object);
		void removeTopLevelObject(MenuObject *object);
		void removeObjectFromList(QString objectName);
		int numObjectsByType(int type);
		int numTopLevelObjects();
		QList<MenuObject*> getObjectsByType(int type);
		QList<MenuObject*> getTopLevelObjectsByType(int type);
		QList<MenuObject*> getTopLevelObjects();
		QStringList getObjectNamesByType(int type);
		QStringList getTopLevelObjectNamesByType(int type);
		QStringList getTopLevelObjectNames();
		BaseEvent *getOnAcceptEvent();
		BaseEvent *getOnCancelEvent();
		BaseEvent *getOnLoadEvent();
		BaseEvent *getOnLeaveEvent();
		BaseEvent *getOnUpArrowEvent();
		BaseEvent *getOnDownArrowEvent();
		BaseEvent *getOnLeftArrowEvent();
		BaseEvent *getOnRightArrowEvent();
		MenuNavigation *getNavigation();
		QString getName();
		void setName(QString value);
		void setGraphicsScene(QGraphicsScene *value);
		void unsetGraphicsScene();
		void updateMenuObjects();
		void updateZValues();
		void applyMenuNavigation();
		static bool tryLoad(XMLNode MenuNode);
	private:
		BaseEvent *createArrowEvent(QString objectName);
		QString getObjectNameAbovePoint(int row, int column);
		QString getObjectNameBelowPoint(int row, int column);
		QString getObjectNameLeftOfPoint(int row, int column);
		QString getObjectNameRightOfPoint(int row, int column);
		void checkObjectList();
		QList<MenuObject*> topLevelObjects; // Objects are freed from this list
		QMap<QString, MenuObject*> objectList; // This list is for quick object searching purposes only
		BaseEvent *onAcceptEvent;
		BaseEvent *onCancelEvent;
		BaseEvent *onLoadEvent;
		BaseEvent *onLeaveEvent;
		BaseEvent *onUpArrowEvent;
		BaseEvent *onDownArrowEvent;
		BaseEvent *onLeftArrowEvent;
		BaseEvent *onRightArrowEvent;
		QString menuName;
		QGraphicsScene *graphicsScene;
		MenuNavigation *navigation;
};

#endif // PROJECTDATA_MAINMENULAYOUT_H
