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

#include <QPushButton>
#include "configureevent_previousselectedareabranch.h"
#include "projectdata.h"
#include "projectdata_mainmenulayout.h"
#include "projectdata_popupmenulayout.h"
#include "projectdata_titlemenulayout.h"

ConfigureEvent_PreviousSelectedAreaBranch::ConfigureEvent_PreviousSelectedAreaBranch(int menuID, int menuType, QWidget *parent) : QDialog(parent)
{
	QListWidgetItem *item;
	QStringList availableAreas;
	setupUi(this);
	if (menuType == MenuObject::MENUTYPE_MAINMENU)
	{
		ProjectData::DataReference<MainMenuLayout> *menuRef = new ProjectData::DataReference<MainMenuLayout>(menuID, __FILE__, __LINE__);
		availableAreas = (*menuRef)->getObjectNamesByType(MenuObject::TYPE_SELECTABLEAREA);
		delete menuRef;
	}
	else if (menuType == MenuObject::MENUTYPE_POPUPMENU)
	{
		ProjectData::DataReference<PopupMenuLayout> *menuRef = new ProjectData::DataReference<PopupMenuLayout>(menuID, __FILE__, __LINE__);
		availableAreas = (*menuRef)->getObjectNamesByType(MenuObject::TYPE_SELECTABLEAREA);
		delete menuRef;
	}
	else if (menuType == MenuObject::MENUTYPE_TITLESCREENMENU)
	{
		TitleMenuLayout *layout = ProjectData::titleMenuLayout;
		availableAreas = layout->getObjectNamesByType(MenuObject::TYPE_SELECTABLEAREA);
		layout = NULL;
	}
	for (int i = 0; i < availableAreas.size(); ++i)
	{
		item = new QListWidgetItem(availableAreas[i]);
		item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
		item->setCheckState(Qt::Unchecked);
		lwSelectableAreas->addItem(item);
	}
	buttonBox->button(QDialogButtonBox::Ok)->setEnabled((selectedAreas.size() != 0));
}

ConfigureEvent_PreviousSelectedAreaBranch::~ConfigureEvent_PreviousSelectedAreaBranch()
{
}

QStringList ConfigureEvent_PreviousSelectedAreaBranch::getSelectedAreas()
{
	return selectedAreas;
}

void ConfigureEvent_PreviousSelectedAreaBranch::setSelectedAreas(QStringList areas)
{
	QListWidgetItem *item;
	selectedAreas = areas;
	for (int i = 0; i < lwSelectableAreas->count(); ++i)
	{
		item = lwSelectableAreas->item(i);
		if (areas.contains(item->text()))
			item->setCheckState(Qt::Checked);
		else
			item->setCheckState(Qt::Unchecked);
	}
	buttonBox->button(QDialogButtonBox::Ok)->setEnabled((selectedAreas.size() != 0));
}

void ConfigureEvent_PreviousSelectedAreaBranch::on_lwSelectableAreas_itemChanged(QListWidgetItem *item)
{
	if (item->checkState() == Qt::Checked && !selectedAreas.contains(item->text()))
		selectedAreas.append(item->text());
	else if (item->checkState() == Qt::Unchecked && selectedAreas.contains(item->text()))
		selectedAreas.removeAll(item->text());
	buttonBox->button(QDialogButtonBox::Ok)->setEnabled((selectedAreas.size() != 0));
}
