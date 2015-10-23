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

#include <QMap>
#include <QPoint>
#include <QString>
#include "projectdata.h"
#include "projectdata_menunavigation.h"
#include "storagefile.h"
#include "xmlParser.h"
#include "xmlTools.h"

MenuNavigation::MenuNavigation()
{
	numRows = 0;
	numColumns = 0;
	minRow = 0;
	minColumn = 0;
	wrapHorizontally = true;
	wrapVertically = true;
}

MenuNavigation::MenuNavigation(XMLNode NavigationNode)
{
	XMLNode TempNode;
	QString objectName;
	int row, column;
	int maxRow = 0;
	int maxColumn = 0;
	int numObjects = NavigationNode.nChildNode("object");
	numRows = 0;
	numColumns = 0;
	minRow = 0;
	minColumn = 0;
	wrapHorizontally = (QString(NavigationNode.getAttribute("wrapHorizontally")) == "true") ? true:false;
	wrapVertically = (QString(NavigationNode.getAttribute("wrapVertically")) == "true") ? true:false;
	for (int i = 0; i < numObjects; ++i)
	{
		TempNode = NavigationNode.getChildNode("object", i);
		objectName = TempNode.getAttribute("name");
		row = QString(TempNode.getAttribute("row")).toInt();
		column = QString(TempNode.getAttribute("column")).toInt();
		menuObjects.insert(QPoint(column, row), objectName);
		if (row < minRow)
			minRow = row;
		if (row > maxRow)
			maxRow = row;
		if (column < minColumn)
			minColumn = column;
		if (column > maxColumn)
			maxColumn = column;
	}
	numRows = maxRow - minRow + 1;
	numColumns = maxColumn - minColumn + 1;
	if (menuObjects.size() == 0)
	{
		numRows = 0;
		numColumns = 0;
	}
}

MenuNavigation::MenuNavigation(StorageFile &storageFile)
{
	QString objectName;
	int row, column;
	int maxRow = 0;
	int maxColumn = 0;
	int numObjects;
	char *temp_string;
	numRows = 0;
	numColumns = 0;
	minRow = 0;
	minColumn = 0;
	wrapHorizontally = storageFile.getBool();
	wrapVertically = storageFile.getBool();
	numObjects = storageFile.getSignedInt();
	for (int i = 0; i < numObjects; ++i)
	{
		temp_string = storageFile.getString();
		objectName = temp_string;
		delete[] temp_string;
		row = storageFile.getSignedInt();
		column = storageFile.getSignedInt();
		menuObjects.insert(QPoint(column, row), objectName);
		if (row < minRow)
			minRow = row;
		if (row > maxRow)
			maxRow = row;
		if (column < minColumn)
			minColumn = column;
		if (column > maxColumn)
			maxColumn = column;
	}
	numRows = maxRow - minRow + 1;
	numColumns = maxColumn - minColumn + 1;
	if (menuObjects.size() == 0)
	{
		numRows = 0;
		numColumns = 0;
	}
}

MenuNavigation::~MenuNavigation()
{
}

void MenuNavigation::saveToStorageFile(StorageFile &storageFile)
{
	QMap<QPoint, QString>::iterator iter;
	storageFile.putBool(wrapHorizontally);
	storageFile.putBool(wrapVertically);
	storageFile.putSignedInt(menuObjects.count());
	for (iter = menuObjects.begin(); iter != menuObjects.end(); ++iter)
	{
		storageFile.putString(iter.value().toUtf8().data());
		storageFile.putSignedInt(iter.key().y());
		storageFile.putSignedInt(iter.key().x());
	}
}

XMLNode MenuNavigation::getNavigationNode()
{
	XMLNode TempNode, NavigationNode = XMLNode::createXMLTopNode("navigation");
	QMap<QPoint, QString>::iterator iter;
	NavigationNode.addAttribute("wrapHorizontally", (wrapHorizontally) ? "true":"false");
	NavigationNode.addAttribute("wrapVertically", (wrapVertically) ? "true":"false");
	for (iter = menuObjects.begin(); iter != menuObjects.end(); ++iter)
	{
		TempNode = NavigationNode.addChild("object");
		TempNode.addAttribute("name", iter.value().toUtf8().data());
		TempNode.addAttribute("row", QString::number(iter.key().y()).toUtf8().data());
		TempNode.addAttribute("column", QString::number(iter.key().x()).toUtf8().data());
	}
	return NavigationNode;
}

QString MenuNavigation::objectNameAtLocation(QPoint location)
{
	if (menuObjects.contains(location))
		return menuObjects[location];
	return QString();
}

QStringList MenuNavigation::getObjectNames()
{
	return menuObjects.values();
}

void MenuNavigation::addObject(QString name, QPoint location)
{
	QMap<QPoint, QString>::iterator iter;
	QList<QPoint> points;
	int maxRow = 0;
	int maxColumn = 0;
	int row, column;
	numRows = 0;
	numColumns = 0;
	minRow = 0;
	minColumn = 0;
	menuObjects.insert(location, name);
	points = menuObjects.keys();
	for (int i = 0; i < points.size(); ++i)
	{
		row = points[i].y();
		column = points[i].x();
		if (row < minRow)
			minRow = row;
		if (row > maxRow)
			maxRow = row;
		if (column < minColumn)
			minColumn = column;
		if (column > maxColumn)
			maxColumn = column;
	}
	numRows = maxRow - minRow + 1;
	numColumns = maxColumn - minColumn + 1;
}

void MenuNavigation::removeObject(QPoint location)
{
	QMap<QPoint, QString>::iterator iter;
	QList<QPoint> points;
	int maxRow = 0;
	int maxColumn = 0;
	int row, column;
	menuObjects.remove(location);
	numRows = 0;
	numColumns = 0;
	minRow = 0;
	minColumn = 0;
	points = menuObjects.keys();
	for (int i = 0; i < points.size(); ++i)
	{
		row = points[i].y();
		column = points[i].x();
		if (row < minRow)
			minRow = row;
		if (row > maxRow)
			maxRow = row;
		if (column < minColumn)
			minColumn = column;
		if (column > maxColumn)
			maxColumn = column;
	}
	numRows = maxRow - minRow + 1;
	numColumns = maxColumn - minColumn + 1;
	if (menuObjects.size() == 0)
	{
		numRows = 0;
		numColumns = 0;
	}
}

bool MenuNavigation::canObjectBeRemoved(QPoint location)
{
	QList<QPoint> locations = menuObjects.keys();
	QList<QPoint> checkedLocations;
	locations.removeAll(location);
	if (locations.size() == 0)
		return true;
	checkedLocations.append(locations.first());
	for (int i = 0; i < checkedLocations.size(); ++i)
	{
		QPoint loc = QPoint(checkedLocations[i]);
		QPoint loc2;
		loc2 = QPoint(loc.x(), loc.y() - 1);
		if (locations.contains(loc2) && !checkedLocations.contains(loc2))
			checkedLocations.append(loc2);
		loc2 = QPoint(loc.x() - 1, loc.y());
		if (locations.contains(loc2) && !checkedLocations.contains(loc2))
			checkedLocations.append(loc2);
		loc2 = QPoint(loc.x() + 1, loc.y());
		if (locations.contains(loc2) && !checkedLocations.contains(loc2))
			checkedLocations.append(loc2);
		loc2 = QPoint(loc.x(), loc.y() + 1);
		if (locations.contains(loc2) && !checkedLocations.contains(loc2))
			checkedLocations.append(loc2);
	}
	if (checkedLocations.size() != locations.size())
		return false;
	return true;
}

int MenuNavigation::getNumRows()
{
	return numRows;
}

int MenuNavigation::getNumColumns()
{
	return numColumns;
}

int MenuNavigation::getMinRow()
{
	return minRow;
}

int MenuNavigation::getMinColumn()
{
	return minColumn;
}

void MenuNavigation::changeObjectName(QString oldName, QString newName)
{
	QMap<QPoint, QString>::iterator iter;
	for (iter = menuObjects.begin(); iter != menuObjects.end(); ++iter)
	{
		if (iter.value() == oldName)
			menuObjects[iter.key()] = newName;
	}
}

void MenuNavigation::setWrapHorizontally(bool value)
{
	wrapHorizontally = value;
}

bool MenuNavigation::wrapsHorizontally()
{
	return wrapHorizontally;
}

void MenuNavigation::setWrapVertically(bool value)
{
	wrapVertically = value;
}

bool MenuNavigation::wrapsVertically()
{
	return wrapVertically;
}

bool MenuNavigation::hasNeighborAtLocation(QPoint location)
{
	QPoint loc2;
	loc2 = QPoint(location.x(), location.y() - 1);
	if (menuObjects.contains(loc2))
		return true;
	loc2 = QPoint(location.x() - 1, location.y());
	if (menuObjects.contains(loc2))
		return true;
	loc2 = QPoint(location.x() + 1, location.y());
	if (menuObjects.contains(loc2))
		return true;
	loc2 = QPoint(location.x(), location.y() + 1);
	if (menuObjects.contains(loc2))
		return true;
	return false;
}

bool MenuNavigation::tryLoad(XMLNode NavigationNode)
{
	XMLNode TempNode;
	int numObjects = NavigationNode.nChildNode("object");
	if (!XMLTools::attributeExists(NavigationNode, "wrapHorizontally"))
		return false;
	if (!XMLTools::attributeStringValid(NavigationNode, "wrapHorizontally", QStringList() << "true" << "false", 1))
		return false;
	if (!XMLTools::attributeExists(NavigationNode, "wrapVertically"))
		return false;
	if (!XMLTools::attributeStringValid(NavigationNode, "wrapVertically", QStringList() << "true" << "false", 1))
		return false;
	for (int i = 0; i < numObjects; ++i)
	{
		TempNode = NavigationNode.getChildNode("object", i);
		if (!XMLTools::attributeExists(TempNode, "name"))
			return false;
		if (!XMLTools::attributeExists(TempNode, "row"))
			return false;
		if (!XMLTools::attributeExists(TempNode, "column"))
			return false;
	}
	return true;
}
