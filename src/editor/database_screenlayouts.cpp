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

#include <QBrush>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QMenu>
#include <QInputDialog>
#include <QMessageBox>
#include "database_screenlayouts.h"
#include "projectdata.h"
#include "projectdata_mainmenulayout.h"
#include "projectdata_popupmenulayout.h"

Database_ScreenLayouts::Database_ScreenLayouts(QWidget *parent) : QWidget(parent)
{
	setupUi(this);
	wMenuName->setEnabled(false);
	ignoreEvents = false;
}

Database_ScreenLayouts::~Database_ScreenLayouts()
{
}

void Database_ScreenLayouts::setupScreenLayouts()
{
	QTreeWidgetItem *topLevelItem;
	QTreeWidgetItem *item;
	QList<int> mainMenuIDs = ProjectData::mainMenuLayoutNames.keys();
	QList<int> popupMenuIDs = ProjectData::popupMenuLayoutNames.keys();
	topLevelItem = new QTreeWidgetItem(twLayouts);
	topLevelItem->setText(0, "Battle Layout");
	topLevelItem->setText(1, "-1");
	topLevelItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
	topLevelItem = new QTreeWidgetItem(twLayouts);
	topLevelItem->setText(0, "Title Screen");
	topLevelItem->setText(1, "-1");
	topLevelItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
	topLevelItem = new QTreeWidgetItem(twLayouts);
	topLevelItem->setText(0, "Main Menus");
	topLevelItem->setText(1, "-1");
	topLevelItem->setFlags(Qt::ItemIsEnabled);
	for (int i = 0; i < mainMenuIDs.size(); ++i)
	{
		item = new QTreeWidgetItem(topLevelItem);
		item->setText(0, ProjectData::mainMenuLayoutNames[mainMenuIDs[i]]);
		item->setText(1, QString::number(mainMenuIDs[i]));
		item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
	}
	topLevelItem = new QTreeWidgetItem(twLayouts);
	topLevelItem->setText(0, "Popup Menus");
	topLevelItem->setText(1, "-1");
	topLevelItem->setFlags(Qt::ItemIsEnabled);
	for (int i = 0; i < popupMenuIDs.size(); ++i)
	{
		item = new QTreeWidgetItem(topLevelItem);
		item->setText(0, ProjectData::popupMenuLayoutNames[popupMenuIDs[i]]);
		item->setText(1, QString::number(popupMenuIDs[i]));
		item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
	}
	wBattleLayout->setupLayout();
	wTitleMenuLayout->setupLayout();
	wPopupMenuLayout->setupLayout();
	wMainMenuLayout->setupLayout();
}

void Database_ScreenLayouts::shutdownScreenLayouts()
{
	ignoreEvents = true;
	twLayouts->clear();
	wBattleLayout->shutdownLayout();
	wTitleMenuLayout->shutdownLayout();
	wPopupMenuLayout->shutdownLayout();
	wMainMenuLayout->shutdownLayout();
	stackedWidget->setCurrentWidget(wBlankPage);
	ignoreEvents = false;
}

void Database_ScreenLayouts::on_aDeleteMenu_triggered()
{
	QString currentWidget = stackedWidget->currentWidget()->objectName();
	int menuID = contextMenuItem->text(1).toInt();
	int confirm = QMessageBox::question(this, "Confirm Delete", "Are you sure you want to delete the selected menu?", QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
	if (confirm == QMessageBox::Yes)
	{
		if (currentWidget == "wPopupMenuLayout")
		{
			wPopupMenuLayout->unloadCurrentLayout();
			ProjectData::deletePopupMenuLayout(menuID);
		}
		else if (currentWidget == "wMainMenuLayout")
		{
			wMainMenuLayout->unloadCurrentLayout();
			ProjectData::deleteMainMenuLayout(menuID);
		}
		delete contextMenuItem;
	}
}

void Database_ScreenLayouts::on_aNewMainMenu_triggered()
{
	MainMenuLayout *mainMenu;
	QTreeWidgetItem *item;
	QString menuName;
	int menuID;
	bool ok;
	menuName = QInputDialog::getText(window(), "Menu Name", "Name:", QLineEdit::Normal, "Menu", &ok);
	if (ok)
	{
		mainMenu = new MainMenuLayout();
		mainMenu->setName(menuName);
		menuID = ProjectData::addMainMenuLayout(mainMenu);
		item = new QTreeWidgetItem(contextMenuItem);
		item->setText(0, menuName);
		item->setText(1, QString::number(menuID));
		item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
		twLayouts->setCurrentItem(item);
		item->setSelected(true);
	}
}

void Database_ScreenLayouts::on_aNewPopupMenu_triggered()
{
	PopupMenuLayout *popupMenu;
	QTreeWidgetItem *item;
	QString menuName;
	int menuID;
	bool ok;
	menuName = QInputDialog::getText(window(), "Menu Name", "Name:", QLineEdit::Normal, "Menu", &ok);
	if (ok)
	{
		popupMenu = new PopupMenuLayout();
		popupMenu->setName(menuName);
		menuID = ProjectData::addPopupMenuLayout(popupMenu);
		item = new QTreeWidgetItem(contextMenuItem);
		item->setText(0, menuName);
		item->setText(1, QString::number(menuID));
		item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
		twLayouts->setCurrentItem(item);
		item->setSelected(true);
	}
}

void Database_ScreenLayouts::on_leMenuName_textEdited(const QString &text)
{
	QTreeWidgetItem *item = twLayouts->currentItem();
	if (item != NULL)
	{
		if (item->text(1) != "-1")
		{
			if (item->parent()->text(0) == "Main Menus")
			{
				item->text(0) = text;
				wMainMenuLayout->changeCurrentLayoutsName(text);
			}
			else if (item->parent()->text(0) == "Popup Menus")
			{
				item->text(0) = text;
				wPopupMenuLayout->changeCurrentLayoutsName(text);
			}
		}
	}
}

void Database_ScreenLayouts::on_twLayouts_customContextMenuRequested(const QPoint &pos)
{
	QTreeWidgetItem *item = twLayouts->itemAt(pos);
	if (item != NULL)
	{
		QMenu *popupMenu = new QMenu(this);
		bool displayMenu = false;
		if (item->text(1) == "-1")
		{
			displayMenu = true;
			if (item->text(0) == "Main Menus")
				popupMenu->addAction(aNewMainMenu);
			else if (item->text(0) == "Popup Menus")
				popupMenu->addAction(aNewPopupMenu);
			else
				displayMenu = false;
		}
		else if (item->text(1) != "-1")
		{
			displayMenu = true;
			if (item->parent()->text(0) == "Main Menus" || item->parent()->text(0) == "Popup Menus")
				popupMenu->addAction(aDeleteMenu);
			else
				displayMenu = false;
		}
		if (displayMenu)
		{
			contextMenuItem = item;
			popupMenu->exec(twLayouts->mapToGlobal(pos));
		}
		delete popupMenu;
	}
}

void Database_ScreenLayouts::on_twLayouts_itemSelectionChanged()
{
	if (!ignoreEvents)
	{
		QTreeWidgetItem *item = twLayouts->currentItem();
		wMenuName->setEnabled(false);
		leMenuName->setText("");
		if (item != NULL)
		{
			if (item->text(1) == "-1")
			{
				if (item->text(0) == "Battle Layout")
					stackedWidget->setCurrentWidget(wBattleLayout);
				else if (item->text(0) == "Title Screen")
					stackedWidget->setCurrentWidget(wTitleMenuLayout);
				else
					stackedWidget->setCurrentWidget(wBlankPage);
			}
			else
			{
				if (item->parent()->text(0) == "Main Menus")
				{
					stackedWidget->setCurrentWidget(wMainMenuLayout);
					wMainMenuLayout->setCurrentLayout(item->text(1).toInt());
					wMenuName->setEnabled(true);
					leMenuName->setText(item->text(0));
				}
				else if (item->parent()->text(0) == "Popup Menus")
				{
					stackedWidget->setCurrentWidget(wPopupMenuLayout);
					wPopupMenuLayout->setCurrentLayout(item->text(1).toInt());
					wMenuName->setEnabled(true);
					leMenuName->setText(item->text(0));
				}
				else
					stackedWidget->setCurrentWidget(wBlankPage);
			}
		}
		else
			stackedWidget->setCurrentWidget(wBlankPage);
	}
}
