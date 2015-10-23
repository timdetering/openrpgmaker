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
 *  Copyright (C) 2012, Justin Davis <tuxdavis@gmail.com>                   *
 ****************************************************************************/

#include "menueventeditor.h"
#include "menuitemtable.h"
#include "menuselectablearea.h"
#include "projectdata_baseevent.h"
#include "projectdata_mainmenulayout.h"
#include "projectdata_popupmenulayout.h"
#include "projectdata_titlemenulayout.h"

MenuEventEditor::MenuEventEditor(int id, int type, QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	mainRef = NULL;
	popupRef = NULL;
	titleMenuLayout = NULL;
	menuID = id;
	if (type == MenuObject::MENUTYPE_MAINMENU)
		mainRef = new ProjectData::DataReference<MainMenuLayout>(id, __FILE__, __LINE__);
	else if (type == MenuObject::MENUTYPE_POPUPMENU)
		popupRef = new ProjectData::DataReference<PopupMenuLayout>(id, __FILE__, __LINE__);
	else if (type == MenuObject::MENUTYPE_TITLESCREENMENU)
		titleMenuLayout = ProjectData::titleMenuLayout;
	setupEventTree();
}

MenuEventEditor::~MenuEventEditor()
{
	if (mainRef != NULL)
	{
		delete mainRef;
		mainRef = NULL;
	}
	if (popupRef != NULL)
	{
		delete popupRef;
		popupRef = NULL;
	}
	titleMenuLayout = NULL;
}

void MenuEventEditor::on_twObjects_itemSelectionChanged()
{
	QList<QTreeWidgetItem*> selectedItems = twObjects->selectedItems();
	MenuObject *object;
	QString objectName;
	int eventType, objectType;
	int menuType;
	if (mainRef != NULL)
		menuType = MenuObject::MENUTYPE_MAINMENU;
	else if (popupRef != NULL)
		menuType = MenuObject::MENUTYPE_POPUPMENU;
	else if (titleMenuLayout != NULL)
		menuType = MenuObject::MENUTYPE_TITLESCREENMENU;
	else
		menuType = -1;
	if (selectedItems.size() > 0)
	{
		eventType = selectedItems[0]->text(1).toInt();
		if (eventType != -1)
		{
			objectName = selectedItems[0]->parent()->text(0).toUtf8().data();
			if (objectName == "Menu's Global Events")
			{
				if (mainRef != NULL)
				{
					if (eventType == MENUEVENT_ONACCEPT)
						lwEvents->setupView(menuID, menuType, (*mainRef)->getOnAcceptEvent());
					else if (eventType == MENUEVENT_ONCANCEL)
						lwEvents->setupView(menuID, menuType, (*mainRef)->getOnCancelEvent());
					else if (eventType == MENUEVENT_ONLOAD)
						lwEvents->setupView(menuID, menuType, (*mainRef)->getOnLoadEvent());
					else if (eventType == MENUEVENT_ONLEAVE)
						lwEvents->setupView(menuID, menuType, (*mainRef)->getOnLeaveEvent());
					else if (eventType == MENUEVENT_UPARROW)
						lwEvents->setupView(menuID, menuType, (*mainRef)->getOnUpArrowEvent());
					else if (eventType == MENUEVENT_DOWNARROW)
						lwEvents->setupView(menuID, menuType, (*mainRef)->getOnDownArrowEvent());
					else if (eventType == MENUEVENT_LEFTARROW)
						lwEvents->setupView(menuID, menuType, (*mainRef)->getOnLeftArrowEvent());
					else if (eventType == MENUEVENT_RIGHTARROW)
						lwEvents->setupView(menuID, menuType, (*mainRef)->getOnRightArrowEvent());
				}
				else if (popupRef != NULL)
				{
					if (eventType == MENUEVENT_ONACCEPT)
						lwEvents->setupView(menuID, menuType, (*popupRef)->getOnAcceptEvent());
					else if (eventType == MENUEVENT_ONCANCEL)
						lwEvents->setupView(menuID, menuType, (*popupRef)->getOnCancelEvent());
					else if (eventType == MENUEVENT_ONLOAD)
						lwEvents->setupView(menuID, menuType, (*popupRef)->getOnLoadEvent());
					else if (eventType == MENUEVENT_ONLEAVE)
						lwEvents->setupView(menuID, menuType, (*popupRef)->getOnLeaveEvent());
					else if (eventType == MENUEVENT_UPARROW)
						lwEvents->setupView(menuID, menuType, (*popupRef)->getOnUpArrowEvent());
					else if (eventType == MENUEVENT_DOWNARROW)
						lwEvents->setupView(menuID, menuType, (*popupRef)->getOnDownArrowEvent());
					else if (eventType == MENUEVENT_LEFTARROW)
						lwEvents->setupView(menuID, menuType, (*popupRef)->getOnLeftArrowEvent());
					else if (eventType == MENUEVENT_RIGHTARROW)
						lwEvents->setupView(menuID, menuType, (*popupRef)->getOnRightArrowEvent());
				}
				else if (titleMenuLayout != NULL)
				{
					if (eventType == MENUEVENT_ONACCEPT)
						lwEvents->setupView(menuID, menuType, titleMenuLayout->getOnAcceptEvent());
					else if (eventType == MENUEVENT_ONCANCEL)
						lwEvents->setupView(menuID, menuType, titleMenuLayout->getOnCancelEvent());
					else if (eventType == MENUEVENT_ONLOAD)
						lwEvents->setupView(menuID, menuType, titleMenuLayout->getOnLoadEvent());
					else if (eventType == MENUEVENT_ONLEAVE)
						lwEvents->setupView(menuID, menuType, titleMenuLayout->getOnLeaveEvent());
					else if (eventType == MENUEVENT_UPARROW)
						lwEvents->setupView(menuID, menuType, titleMenuLayout->getOnUpArrowEvent());
					else if (eventType == MENUEVENT_DOWNARROW)
						lwEvents->setupView(menuID, menuType, titleMenuLayout->getOnDownArrowEvent());
					else if (eventType == MENUEVENT_LEFTARROW)
						lwEvents->setupView(menuID, menuType, titleMenuLayout->getOnLeftArrowEvent());
					else if (eventType == MENUEVENT_RIGHTARROW)
						lwEvents->setupView(menuID, menuType, titleMenuLayout->getOnRightArrowEvent());
				}
			}
			else
			{
				if (mainRef != NULL)
					object = (*mainRef)->getObjectByName(objectName);
				else if (popupRef != NULL)
					object = (*popupRef)->getObjectByName(objectName);
				else if (titleMenuLayout != NULL)
					object = titleMenuLayout->getObjectByName(objectName);
				else
					object = NULL;
				objectType = object->getType();
				if (objectType == MenuObject::TYPE_SELECTABLEAREA)
				{
					if (eventType == SELECTABLEAREA_ONHIGHLIGHT)
						lwEvents->setupView(menuID, menuType, ((MenuSelectableArea*)object)->getHighlightEvent());
					else if (eventType == SELECTABLEAREA_ONLEFTARROW)
						lwEvents->setupView(menuID, menuType, ((MenuSelectableArea*)object)->getLeftArrowEvent());
					else if (eventType == SELECTABLEAREA_ONRIGHTARROW)
						lwEvents->setupView(menuID, menuType, ((MenuSelectableArea*)object)->getRightArrowEvent());
					else if (eventType == SELECTABLEAREA_ONUPARROW)
						lwEvents->setupView(menuID, menuType, ((MenuSelectableArea*)object)->getUpArrowEvent());
					else if (eventType == SELECTABLEAREA_ONDOWNARROW)
						lwEvents->setupView(menuID, menuType, ((MenuSelectableArea*)object)->getDownArrowEvent());
					else if (eventType == SELECTABLEAREA_ONSELECTED)
						lwEvents->setupView(menuID, menuType, ((MenuSelectableArea*)object)->getSelectedEvent());
					else if (eventType == SELECTABLEAREA_ONCANCEL)
						lwEvents->setupView(menuID, menuType, ((MenuSelectableArea*)object)->getCancelEvent());
				}
				else if (objectType == MenuObject::TYPE_ITEMLIST)
				{
					if (eventType == ITEMLIST_SELECTIONCHANGED)
						lwEvents->setupView(menuID, menuType, ((MenuItemTable*)object)->getSelectionChangedEvent());
					else if (eventType == ITEMLIST_ONACCEPT)
						lwEvents->setupView(menuID, menuType, ((MenuItemTable*)object)->getAcceptEvent());
					else if (eventType == ITEMLIST_ONCANCEL)
						lwEvents->setupView(menuID, menuType, ((MenuItemTable*)object)->getCancelEvent());
				}
			}
		}
	}
}

void MenuEventEditor::setupEventTree()
{
	QTreeWidgetItem *objectItem;
	QTreeWidgetItem *eventItem;
	QStringList objectNames;
	if (mainRef != NULL)
		objectNames = (*mainRef)->getObjectNamesByType(MenuObject::TYPE_SELECTABLEAREA) + (*mainRef)->getObjectNamesByType(MenuObject::TYPE_ITEMLIST);
	else if (popupRef != NULL)
		objectNames = (*popupRef)->getObjectNamesByType(MenuObject::TYPE_SELECTABLEAREA) + (*popupRef)->getObjectNamesByType(MenuObject::TYPE_ITEMLIST);
	else if (titleMenuLayout != NULL)
		objectNames = titleMenuLayout->getObjectNamesByType(MenuObject::TYPE_SELECTABLEAREA) + titleMenuLayout->getObjectNamesByType(MenuObject::TYPE_ITEMLIST);
	objectNames.sort();
	objectItem = new QTreeWidgetItem(QStringList() << "Menu's Global Events" << "-1");
	objectItem->setFlags(Qt::ItemIsEnabled);
	twObjects->addTopLevelItem(objectItem);
	eventItem = new QTreeWidgetItem(QStringList() << "On Accept" << "0");
	eventItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
	objectItem->addChild(eventItem);
	eventItem->setSelected(true);
	eventItem = new QTreeWidgetItem(QStringList() << "On Cancel" << "1");
	eventItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
	objectItem->addChild(eventItem);
	eventItem = new QTreeWidgetItem(QStringList() << "On Load" << "2");
	eventItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
	objectItem->addChild(eventItem);
	eventItem = new QTreeWidgetItem(QStringList() << "On Leave" << "3");
	eventItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
	objectItem->addChild(eventItem);
	eventItem = new QTreeWidgetItem(QStringList() << "Up Arrow" << "4");
	eventItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
	objectItem->addChild(eventItem);
	eventItem = new QTreeWidgetItem(QStringList() << "Down Arrow" << "5");
	eventItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
	objectItem->addChild(eventItem);
	eventItem = new QTreeWidgetItem(QStringList() << "Left Arrow" << "6");
	eventItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
	objectItem->addChild(eventItem);
	eventItem = new QTreeWidgetItem(QStringList() << "Right Arrow" << "7");
	eventItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
	objectItem->addChild(eventItem);
	objectItem->setExpanded(true);
	for (int i = 0; i < objectNames.size(); ++i)
	{
		MenuObject *object;
		int objectType;
		if (mainRef != NULL)
			object = (*mainRef)->getObjectByName(objectNames[i]);
		else if (popupRef != NULL)
			object = (*popupRef)->getObjectByName(objectNames[i]);
		else if (titleMenuLayout != NULL)
			object = titleMenuLayout->getObjectByName(objectNames[i]);
		else
			object = NULL;
		objectType = object->getType();
		objectItem = new QTreeWidgetItem(QStringList() << objectNames[i] << "-1");
		objectItem->setFlags(Qt::ItemIsEnabled);
		twObjects->addTopLevelItem(objectItem);
		if (objectType == MenuObject::TYPE_SELECTABLEAREA)
		{
			eventItem = new QTreeWidgetItem(QStringList() << "On highlight" << "0");
			eventItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
			objectItem->addChild(eventItem);
			eventItem = new QTreeWidgetItem(QStringList() << "Left Arrow" << "1");
			eventItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
			objectItem->addChild(eventItem);
			eventItem = new QTreeWidgetItem(QStringList() << "Right Arrow" << "2");
			eventItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
			objectItem->addChild(eventItem);
			eventItem = new QTreeWidgetItem(QStringList() << "Up Arrow" << "3");
			eventItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
			objectItem->addChild(eventItem);
			eventItem = new QTreeWidgetItem(QStringList() << "Down Arrow" << "4");
			eventItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
			objectItem->addChild(eventItem);
			eventItem = new QTreeWidgetItem(QStringList() << "On Selected" << "5");
			eventItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
			objectItem->addChild(eventItem);
			eventItem = new QTreeWidgetItem(QStringList() << "On Cancel" << "6");
			eventItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
			objectItem->addChild(eventItem);
		}
		else if (objectType == MenuObject::TYPE_ITEMLIST)
		{
			eventItem = new QTreeWidgetItem(QStringList() << "Selection Changed" << "0");
			eventItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
			objectItem->addChild(eventItem);
			eventItem = new QTreeWidgetItem(QStringList() << "On Accept" << "1");
			eventItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
			objectItem->addChild(eventItem);
			eventItem = new QTreeWidgetItem(QStringList() << "On Cancel" << "2");
			eventItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
			objectItem->addChild(eventItem);
		}
	}
}
