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
 *  Copyright (C) 2011, Justin Davis <tuxdavis@gmail.com>                   *
 ****************************************************************************/

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QFileInfo>
#include "configuregradient.h"
#include "configuremenuimage.h"
#include "configuremenuitemlist.h"
#include "configuremenutext.h"
#include "imagebrowser.h"
#include "menuframe.h"
#include "menuimage.h"
#include "menuitemtable.h"
#include "menuobject.h"
#include "menuobjectdelegate.h"
#include "menuobjectproperties.h"
#include "menuscrollarea.h"
#include "menuselectablearea.h"
#include "menutext.h"
#include "projectdata.h"
#include "projectdata_baseevent.h"
#include "variableselector.h"

MenuObjectProperties::MenuObjectProperties(QWidget *parent) : QTableWidget(parent)
{
	objectDelegate = new MenuObjectDelegate;
	setItemDelegate(objectDelegate);
	QObject::connect(this, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(editMenuObjectItem(QTableWidgetItem*)));
}

MenuObjectProperties::~MenuObjectProperties()
{
}

void MenuObjectProperties::setCurrentObject(MenuObject *menuObject)
{
	QTableWidgetItem *item;
	QColor color1, color2;
	clearContents();
	setRowCount(0);
	if (menuObject != NULL)
	{
		objectType = menuObject->getType();
		if (objectType == MenuObject::TYPE_FRAME)
		{
			object.frame = (MenuFrame*)menuObject;
			setRowCount(5);
			item = new QTableWidgetItem("Object Name");
			item->setFlags(Qt::ItemIsEnabled);
			setItem(0, 0, item);
			item = new QTableWidgetItem(object.frame->getName());
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
			setItem(0, 1, item);
			setRowHeight(0, 16);
			item = new QTableWidgetItem("X");
			item->setFlags(Qt::ItemIsEnabled);
			setItem(1, 0, item);
			item = new QTableWidgetItem(QString::number(object.frame->getX()));
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
			setItem(1, 1, item);
			setRowHeight(1, 16);
			item = new QTableWidgetItem("Y");
			item->setFlags(Qt::ItemIsEnabled);
			setItem(2, 0, item);
			item = new QTableWidgetItem(QString::number(object.frame->getY()));
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
			setItem(2, 1, item);
			setRowHeight(2, 16);
			item = new QTableWidgetItem("Width");
			item->setFlags(Qt::ItemIsEnabled);
			setItem(3, 0, item);
			item = new QTableWidgetItem(QString::number(object.frame->getWidth()));
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
			setItem(3, 1, item);
			setRowHeight(3, 16);
			item = new QTableWidgetItem("Height");
			item->setFlags(Qt::ItemIsEnabled);
			setItem(4, 0, item);
			item = new QTableWidgetItem(QString::number(object.frame->getHeight()));
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
			setItem(4, 1, item);
			setRowHeight(4, 16);
		}
		else if (objectType == MenuObject::TYPE_TEXT)
		{
			object.text = (MenuText*)menuObject;
			int alignment = object.text->getAlignment();
			setRowCount(9);
			item = new QTableWidgetItem("Object Name");
			item->setFlags(Qt::ItemIsEnabled);
			setItem(0, 0, item);
			item = new QTableWidgetItem(object.text->getName());
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
			setItem(0, 1, item);
			setRowHeight(0, 16);
			item = new QTableWidgetItem("X");
			item->setFlags(Qt::ItemIsEnabled);
			setItem(1, 0, item);
			item = new QTableWidgetItem(QString::number(object.text->getX()));
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
			setItem(1, 1, item);
			setRowHeight(1, 16);
			item = new QTableWidgetItem("Y");
			item->setFlags(Qt::ItemIsEnabled);
			setItem(2, 0, item);
			item = new QTableWidgetItem(QString::number(object.text->getY()));
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
			setItem(2, 1, item);
			setRowHeight(2, 16);
			item = new QTableWidgetItem("Text");
			item->setFlags(Qt::ItemIsEnabled);
			setItem(3, 0, item);
			item = new QTableWidgetItem(object.text->getFinalText());
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
			setItem(3, 1, item);
			setRowHeight(3, 16);
			item = new QTableWidgetItem("Font");
			item->setFlags(Qt::ItemIsEnabled);
			setItem(4, 0, item);
			item = new QTableWidgetItem(ProjectData::fontNames[object.text->getFontID()]);
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
			setItem(4, 1, item);
			setRowHeight(4, 16);
			item = new QTableWidgetItem("Enabled Color");
			item->setFlags(Qt::ItemIsEnabled);
			setItem(5, 0, item);
			color1 = object.text->getEnabledTopColor();
			color2 = object.text->getEnabledBottomColor();
			item = new QTableWidgetItem(QString("#%1%2%3-#%4%5%6").arg(color1.red(), 2, 16, QChar('0')).arg(color1.green(), 2, 16, QChar('0')).arg(color1.blue(), 2, 16, QChar('0')).arg(color2.red(), 2, 16, QChar('0')).arg(color2.green(), 2, 16, QChar('0')).arg(color2.blue(), 2, 16, QChar('0')));
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
			setItem(5, 1, item);
			setRowHeight(5, 16);
			item = new QTableWidgetItem("Disabled Color");
			item->setFlags(Qt::ItemIsEnabled);
			setItem(6, 0, item);
			color1 = object.text->getDisabledTopColor();
			color2 = object.text->getDisabledBottomColor();
			item = new QTableWidgetItem(QString("#%1%2%3-#%4%5%6").arg(color1.red(), 2, 16, QChar('0')).arg(color1.green(), 2, 16, QChar('0')).arg(color1.blue(), 2, 16, QChar('0')).arg(color2.red(), 2, 16, QChar('0')).arg(color2.green(), 2, 16, QChar('0')).arg(color2.blue(), 2, 16, QChar('0')));
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
			setItem(6, 1, item);
			setRowHeight(6, 16);
			item = new QTableWidgetItem("Text Width");
			item->setFlags(Qt::ItemIsEnabled);
			setItem(7, 0, item);
			item = new QTableWidgetItem(QString::number(object.text->getTextWidth()));
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
			setItem(7, 1, item);
			setRowHeight(7, 16);
			item = new QTableWidgetItem("Alignment");
			item->setFlags(Qt::ItemIsEnabled);
			setItem(8, 0, item);
			if (alignment == MenuText::ALIGN_LEFT)
				item = new QTableWidgetItem("Left");
			else if (alignment == MenuText::ALIGN_CENTER)
				item = new QTableWidgetItem("Center");
			else
				item = new QTableWidgetItem("Right");
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
			setItem(8, 1, item);
			setRowHeight(8, 16);
		}
		else if (objectType == MenuObject::TYPE_IMAGE)
		{
			object.image = (MenuImage*)menuObject;
			int imageType = object.image->getImageType();
			bool scaled = object.image->isScaled();
			setRowCount(7);
			item = new QTableWidgetItem("Object Name");
			item->setFlags(Qt::ItemIsEnabled);
			setItem(0, 0, item);
			item = new QTableWidgetItem(object.image->getName());
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
			setItem(0, 1, item);
			setRowHeight(0, 16);
			item = new QTableWidgetItem("X");
			item->setFlags(Qt::ItemIsEnabled);
			setItem(1, 0, item);
			item = new QTableWidgetItem(QString::number(object.image->getX()));
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
			setItem(1, 1, item);
			setRowHeight(1, 16);
			item = new QTableWidgetItem("Y");
			item->setFlags(Qt::ItemIsEnabled);
			setItem(2, 0, item);
			item = new QTableWidgetItem(QString::number(object.image->getY()));
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
			setItem(2, 1, item);
			setRowHeight(2, 16);
			item = new QTableWidgetItem("Image");
			item->setFlags(Qt::ItemIsEnabled);
			setItem(3, 0, item);
			if (imageType == MenuImage::TYPE_PARTYMEMBER)
				item = new QTableWidgetItem("Party Member");
			else if (imageType == MenuImage::TYPE_CHARACTER)
				item = new QTableWidgetItem("Character");
			else if (imageType == MenuImage::TYPE_CLASS)
				item = new QTableWidgetItem("Class");
			else if (imageType == MenuImage::TYPE_MONSTER)
				item = new QTableWidgetItem("Monster");
			else if (imageType == MenuImage::TYPE_VEHICLE)
				item = new QTableWidgetItem("Vehicle");
			else if (imageType == MenuImage::TYPE_TILE)
				item = new QTableWidgetItem("Tile");
			else if (imageType == MenuImage::TYPE_FILE)
			{
				QString fileLocation;
				object.image->getImageLocation(&fileLocation);
				item = new QTableWidgetItem(QFileInfo(fileLocation).fileName());
			}
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
			setItem(3, 1, item);
			setRowHeight(3, 16);
			item = new QTableWidgetItem("Scaled");
			item->setFlags(Qt::ItemIsEnabled);
			setItem(4, 0, item);
			item = new QTableWidgetItem((scaled) ? "True":"False");
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
			setItem(4, 1, item);
			setRowHeight(4, 16);
			item = new QTableWidgetItem("Scaled to Width");
			item->setFlags(Qt::ItemIsEnabled);
			setItem(5, 0, item);
			item = new QTableWidgetItem(QString::number(object.image->getScaledToWidth()));
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
			setItem(5, 1, item);
			setRowHeight(5, 16);
			item = new QTableWidgetItem("Scaled to Height");
			item->setFlags(Qt::ItemIsEnabled);
			setItem(6, 0, item);
			item = new QTableWidgetItem(QString::number(object.image->getScaledToHeight()));
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
			setItem(6, 1, item);
			setRowHeight(6, 16);
		}
		else if (objectType == MenuObject::TYPE_SELECTABLEAREA)
		{
			object.selectableArea = (MenuSelectableArea*)menuObject;
			setRowCount(5);
			item = new QTableWidgetItem("Object Name");
			item->setFlags(Qt::ItemIsEnabled);
			setItem(0, 0, item);
			item = new QTableWidgetItem(object.selectableArea->getName());
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
			setItem(0, 1, item);
			setRowHeight(0, 16);
			item = new QTableWidgetItem("X");
			item->setFlags(Qt::ItemIsEnabled);
			setItem(1, 0, item);
			item = new QTableWidgetItem(QString::number(object.selectableArea->getX()));
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
			setItem(1, 1, item);
			setRowHeight(1, 16);
			item = new QTableWidgetItem("Y");
			item->setFlags(Qt::ItemIsEnabled);
			setItem(2, 0, item);
			item = new QTableWidgetItem(QString::number(object.selectableArea->getY()));
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
			setItem(2, 1, item);
			setRowHeight(2, 16);
			item = new QTableWidgetItem("Width");
			item->setFlags(Qt::ItemIsEnabled);
			setItem(3, 0, item);
			item = new QTableWidgetItem(QString::number(object.selectableArea->getWidth()));
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
			setItem(3, 1, item);
			setRowHeight(3, 16);
			item = new QTableWidgetItem("Height");
			item->setFlags(Qt::ItemIsEnabled);
			setItem(4, 0, item);
			item = new QTableWidgetItem(QString::number(object.selectableArea->getHeight()));
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
			setItem(4, 1, item);
			setRowHeight(4, 16);
		}
		else if (objectType == MenuObject::TYPE_SCROLLAREA)
		{
			object.scrollArea = (MenuScrollArea*)menuObject;
			setRowCount(9);
			item = new QTableWidgetItem("Object Name");
			item->setFlags(Qt::ItemIsEnabled);
			setItem(0, 0, item);
			item = new QTableWidgetItem(object.scrollArea->getName());
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
			setItem(0, 1, item);
			setRowHeight(0, 16);
			item = new QTableWidgetItem("X");
			item->setFlags(Qt::ItemIsEnabled);
			setItem(1, 0, item);
			item = new QTableWidgetItem(QString::number(object.scrollArea->getX()));
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
			setItem(1, 1, item);
			setRowHeight(1, 16);
			item = new QTableWidgetItem("Y");
			item->setFlags(Qt::ItemIsEnabled);
			setItem(2, 0, item);
			item = new QTableWidgetItem(QString::number(object.scrollArea->getY()));
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
			setItem(2, 1, item);
			setRowHeight(2, 16);
			item = new QTableWidgetItem("Width");
			item->setFlags(Qt::ItemIsEnabled);
			setItem(3, 0, item);
			item = new QTableWidgetItem(QString::number(object.scrollArea->getWidth()));
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
			setItem(3, 1, item);
			setRowHeight(3, 16);
			item = new QTableWidgetItem("Height");
			item->setFlags(Qt::ItemIsEnabled);
			setItem(4, 0, item);
			item = new QTableWidgetItem(QString::number(object.scrollArea->getHeight()));
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
			setItem(4, 1, item);
			setRowHeight(4, 16);
			item = new QTableWidgetItem("Internal Width");
			item->setFlags(Qt::ItemIsEnabled);
			setItem(5, 0, item);
			item = new QTableWidgetItem(QString::number(object.scrollArea->getInternalWidth()));
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
			setItem(5, 1, item);
			setRowHeight(5, 16);
			item = new QTableWidgetItem("Internal Height");
			item->setFlags(Qt::ItemIsEnabled);
			setItem(6, 0, item);
			item = new QTableWidgetItem(QString::number(object.scrollArea->getInternalHeight()));
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
			setItem(6, 1, item);
			setRowHeight(6, 16);
			item = new QTableWidgetItem("Scroll X");
			item->setFlags(Qt::ItemIsEnabled);
			setItem(7, 0, item);
			item = new QTableWidgetItem(QString::number(object.scrollArea->getCurrentScrollX()));
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
			setItem(7, 1, item);
			setRowHeight(7, 16);
			item = new QTableWidgetItem("Scroll Y");
			item->setFlags(Qt::ItemIsEnabled);
			setItem(8, 0, item);
			item = new QTableWidgetItem(QString::number(object.scrollArea->getCurrentScrollY()));
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
			setItem(8, 1, item);
			setRowHeight(8, 16);
		}
		else if (objectType == MenuObject::TYPE_ITEMLIST)
		{
			object.itemList = (MenuItemTable*)menuObject;
			int tableType = object.itemList->getTableType();
			int alignment = object.itemList->getTextAlignment();
			setRowCount(18);
			item = new QTableWidgetItem("Object Name");
			item->setFlags(Qt::ItemIsEnabled);
			setItem(0, 0, item);
			item = new QTableWidgetItem(object.itemList->getName());
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
			setItem(0, 1, item);
			setRowHeight(0, 16);
			item = new QTableWidgetItem("X");
			item->setFlags(Qt::ItemIsEnabled);
			setItem(1, 0, item);
			item = new QTableWidgetItem(QString::number(object.itemList->getX()));
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
			setItem(1, 1, item);
			setRowHeight(1, 16);
			item = new QTableWidgetItem("Y");
			item->setFlags(Qt::ItemIsEnabled);
			setItem(2, 0, item);
			item = new QTableWidgetItem(QString::number(object.itemList->getY()));
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
			setItem(2, 1, item);
			setRowHeight(2, 16);
			item = new QTableWidgetItem("Width");
			item->setFlags(Qt::ItemIsEnabled);
			setItem(3, 0, item);
			item = new QTableWidgetItem(QString::number(object.itemList->getWidth()));
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
			setItem(3, 1, item);
			setRowHeight(3, 16);
			item = new QTableWidgetItem("Height");
			item->setFlags(Qt::ItemIsEnabled);
			setItem(4, 0, item);
			item = new QTableWidgetItem(QString::number(object.itemList->getHeight()));
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
			setItem(4, 1, item);
			setRowHeight(4, 16);
			item = new QTableWidgetItem("List Type");
			item->setFlags(Qt::ItemIsEnabled);
			setItem(5, 0, item);
			if (tableType == MenuItemTable::TYPE_ITEMS)
				item = new QTableWidgetItem("Items");
			else if (tableType == MenuItemTable::TYPE_SKILLS)
				item = new QTableWidgetItem("Skills");
			else if (tableType == MenuItemTable::TYPE_EQUIPMENT)
				item = new QTableWidgetItem("Available Equipment");
			else if (tableType == MenuItemTable::TYPE_CHARACTERS)
				item = new QTableWidgetItem("Characters");
			else if (tableType == MenuItemTable::TYPE_CLASSES)
				item = new QTableWidgetItem("Classes");
			else if (tableType == MenuItemTable::TYPE_MONSTERS)
				item = new QTableWidgetItem("Monsters");
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
			setItem(5, 1, item);
			setRowHeight(5, 16);
			item = new QTableWidgetItem("Font");
			item->setFlags(Qt::ItemIsEnabled);
			setItem(6, 0, item);
			item = new QTableWidgetItem(ProjectData::fontNames[object.itemList->getFontID()]);
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
			setItem(6, 1, item);
			setRowHeight(6, 16);
			item = new QTableWidgetItem("Enabled Color");
			item->setFlags(Qt::ItemIsEnabled);
			setItem(7, 0, item);
			color1 = object.itemList->getEnabledTopColor();
			color2 = object.itemList->getEnabledBottomColor();
			item = new QTableWidgetItem(QString("#%1%2%3-#%4%5%6").arg(color1.red(), 2, 16, QChar('0')).arg(color1.green(), 2, 16, QChar('0')).arg(color1.blue(), 2, 16, QChar('0')).arg(color2.red(), 2, 16, QChar('0')).arg(color2.green(), 2, 16, QChar('0')).arg(color2.blue(), 2, 16, QChar('0')));
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
			setItem(7, 1, item);
			setRowHeight(7, 16);
			item = new QTableWidgetItem("Disabled Color");
			item->setFlags(Qt::ItemIsEnabled);
			setItem(8, 0, item);
			color1 = object.itemList->getDisabledTopColor();
			color2 = object.itemList->getDisabledBottomColor();
			item = new QTableWidgetItem(QString("#%1%2%3-#%4%5%6").arg(color1.red(), 2, 16, QChar('0')).arg(color1.green(), 2, 16, QChar('0')).arg(color1.blue(), 2, 16, QChar('0')).arg(color2.red(), 2, 16, QChar('0')).arg(color2.green(), 2, 16, QChar('0')).arg(color2.blue(), 2, 16, QChar('0')));
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
			setItem(8, 1, item);
			setRowHeight(8, 16);
			item = new QTableWidgetItem("Text Alignment");
			item->setFlags(Qt::ItemIsEnabled);
			setItem(9, 0, item);
			if (alignment == MenuText::ALIGN_LEFT)
				item = new QTableWidgetItem("Left");
			else if (alignment == MenuText::ALIGN_CENTER)
				item = new QTableWidgetItem("Center");
			else
				item = new QTableWidgetItem("Right");
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
			setItem(9, 1, item);
			setRowHeight(9, 16);
			item = new QTableWidgetItem("Number Of Columns");
			item->setFlags(Qt::ItemIsEnabled);
			setItem(10, 0, item);
			item = new QTableWidgetItem(QString::number(object.itemList->getNumColumns()));
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
			setItem(10, 1, item);
			setRowHeight(10, 16);
			item = new QTableWidgetItem("Row Height");
			item->setFlags(Qt::ItemIsEnabled);
			setItem(11, 0, item);
			item = new QTableWidgetItem(QString::number(object.itemList->getRowHeight()));
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
			setItem(11, 1, item);
			setRowHeight(11, 16);
			item = new QTableWidgetItem("Column Spacing");
			item->setFlags(Qt::ItemIsEnabled);
			setItem(12, 0, item);
			item = new QTableWidgetItem(QString::number(object.itemList->getColumnSpacing()));
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
			setItem(12, 1, item);
			setRowHeight(12, 16);
			item = new QTableWidgetItem("Row Spacing");
			item->setFlags(Qt::ItemIsEnabled);
			setItem(13, 0, item);
			item = new QTableWidgetItem(QString::number(object.itemList->getRowSpacing()));
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
			setItem(13, 1, item);
			setRowHeight(13, 16);
			item = new QTableWidgetItem("Text X-Offset");
			item->setFlags(Qt::ItemIsEnabled);
			setItem(14, 0, item);
			item = new QTableWidgetItem(QString::number(object.itemList->getTextXOffset()));
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
			setItem(14, 1, item);
			setRowHeight(14, 16);
			item = new QTableWidgetItem("Text Y-Offset");
			item->setFlags(Qt::ItemIsEnabled);
			setItem(15, 0, item);
			item = new QTableWidgetItem(QString::number(object.itemList->getTextYOffset()));
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
			setItem(15, 1, item);
			setRowHeight(15, 16);
			item = new QTableWidgetItem("Selection Variable");
			item->setFlags(Qt::ItemIsEnabled);
			setItem(16, 0, item);
			item = new QTableWidgetItem(ProjectData::numberVariableIdToString(object.itemList->getSelectionVariableID()));
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
			setItem(16, 1, item);
			setRowHeight(16, 16);
			item = new QTableWidgetItem("Sorting Enabled");
			item->setFlags(Qt::ItemIsEnabled);
			setItem(17, 0, item);
			item = new QTableWidgetItem((object.itemList->isSortingEnabled()) ? "True":"False");
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
			setItem(17, 1, item);
			setRowHeight(17, 16);
		}
	}
	objectDelegate->setCurrentObject(menuObject);
}

void MenuObjectProperties::objectsNameChanged(QString newName)
{
	item(0, 1)->setText(newName);
}

void MenuObjectProperties::editMenuObjectItem(QTableWidgetItem *item)
{
	int row = item->row();
	int column = item->column();
	bool edited = false;
	if (objectType == MenuObject::TYPE_TEXT)
	{
		if (row == 3 && column == 1) // Edit Text
		{
			ConfigureMenuText *configText = new ConfigureMenuText(object.text, window());
			configText->exec();
			delete configText;
			item->setText(object.text->getFinalText());
			edited = true;
		}
		else if ((row == 5 || row == 6) && column == 1)
		{
			ConfigureGradient *configGradient = new ConfigureGradient();
			if (row == 5)
				configGradient->setColors(object.text->getEnabledTopColor(), object.text->getEnabledBottomColor());
			else if (row == 6)
				configGradient->setColors(object.text->getDisabledTopColor(), object.text->getDisabledBottomColor());
			if (configGradient->exec())
			{
				QColor topColor = configGradient->getTopColor();
				QColor bottomColor = configGradient->getBottomColor();
				if (row == 5)
					object.text->setEnabledColor(topColor, bottomColor);
				else if (row == 6)
					object.text->setDisabledColor(topColor, bottomColor);
				item->setText(QString("#%1%2%3-#%4%5%6").arg(topColor.red(), 2, 16, QChar('0')).arg(topColor.green(), 2, 16, QChar('0')).arg(topColor.blue(), 2, 16, QChar('0')).arg(bottomColor.red(), 2, 16, QChar('0')).arg(bottomColor.green(), 2, 16, QChar('0')).arg(bottomColor.blue(), 2, 16, QChar('0')));
			}
			delete configGradient;
			edited = true;
		}
	}
	else if (objectType == MenuObject::TYPE_IMAGE)
	{
		if (row == 3 && column == 1) // Edit Image
		{
			ConfigureMenuImage *configImage = new ConfigureMenuImage(object.image, window());
			int imageType;
			configImage->exec();
			delete configImage;
			imageType = object.image->getImageType();
			item->setText("");
			if (imageType == MenuImage::TYPE_PARTYMEMBER)
				item->setText("Party Member");
			else if (imageType == MenuImage::TYPE_CHARACTER)
				item->setText("Character");
			else if (imageType == MenuImage::TYPE_CLASS)
				item->setText("Class");
			else if (imageType == MenuImage::TYPE_MONSTER)
				item->setText("Monster");
			else if (imageType == MenuImage::TYPE_VEHICLE)
				item->setText("Vehicle");
			else if (imageType == MenuImage::TYPE_TILE)
				item->setText("Tile");
			else if (imageType == MenuImage::TYPE_FILE)
			{
				QString fileLocation;
				object.image->getImageLocation(&fileLocation);
				item->setText(QFileInfo(fileLocation).fileName());
			}
			edited = true;
		}
	}
	else if (objectType == MenuObject::TYPE_ITEMLIST)
	{
		if (column == 1 && row == 5) // Edit List Type
		{
			ConfigureMenuItemList *configList = new ConfigureMenuItemList(object.itemList, window());
			int tableType;
			configList->exec();
			delete configList;
			tableType = object.itemList->getTableType();
			if (tableType == MenuItemTable::TYPE_ITEMS)
				item->setText("Items");
			else if (tableType == MenuItemTable::TYPE_SKILLS)
				item->setText("Skills");
			else if (tableType == MenuItemTable::TYPE_EQUIPMENT)
				item->setText("Available Equipment");
			else if (tableType == MenuItemTable::TYPE_CHARACTERS)
				item->setText("Characters");
			else if (tableType == MenuItemTable::TYPE_CLASSES)
				item->setText("Classes");
			else if (tableType == MenuItemTable::TYPE_MONSTERS)
				item->setText("Monsters");
			edited = true;
		}
		else if (column == 1 && (row == 7 || row == 8)) // Edit Enabled/Disabled Color
		{
			ConfigureGradient *configGradient = new ConfigureGradient();
			if (row == 7)
				configGradient->setColors(object.itemList->getEnabledTopColor(), object.itemList->getEnabledBottomColor());
			else if (row == 8)
				configGradient->setColors(object.itemList->getDisabledTopColor(), object.itemList->getDisabledBottomColor());
			if (configGradient->exec())
			{
				QColor topColor = configGradient->getTopColor();
				QColor bottomColor = configGradient->getBottomColor();
				if (row == 7)
					object.itemList->setEnabledColor(topColor, bottomColor);
				else if (row == 8)
					object.itemList->setDisabledColor(topColor, bottomColor);
				item->setText(QString("#%1%2%3-#%4%5%6").arg(topColor.red(), 2, 16, QChar('0')).arg(topColor.green(), 2, 16, QChar('0')).arg(topColor.blue(), 2, 16, QChar('0')).arg(bottomColor.red(), 2, 16, QChar('0')).arg(bottomColor.green(), 2, 16, QChar('0')).arg(bottomColor.blue(), 2, 16, QChar('0')));
			}
			delete configGradient;
			edited = true;
		}
		else if (column == 1 && row == 16) // Selection Variable
		{
			VariableSelector *variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, object.itemList->getSelectionVariableID(), window());
			if (variableSelector->exec())
			{
				int varID = variableSelector->getVariableID();
				object.itemList->setSelectionVariableID(varID);
				item->setText(ProjectData::numberVariableIdToString(varID));
			}
			delete variableSelector;
			edited = true;
		}
	}
	if (!edited && (item->flags() & Qt::ItemIsEditable) != 0)
		editItem(item);
}
