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

#include <QList>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QMouseEvent>
#include "configureevent_teleport.h"
#include "projectdata.h"
#include "projectdata_map.h"
#include "variableselector.h"

ConfigureEvent_Teleport::ConfigureEvent_Teleport(QWidget *parent) : QDialog(parent)
{
	QTreeWidgetItem *topLevelItem;
	QString varText = ProjectData::numberVariableIdToString(0);
	setupUi(this);
	resize(sizeHint().width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
	lblMapVariable->setText(varText);
	lblXVariable->setText(varText);
	lblYVariable->setText(varText);
	lblLayerVariable->setText(varText);
	topLevelItem = new QTreeWidgetItem;
	topLevelItem->setText(0, ProjectData::gameName);
	topLevelItem->setText(1, "top");
	topLevelItem->setIcon(0, ProjectData::mapItems->icon(0));
	buildMapTree(topLevelItem, ProjectData::mapItems);
	twMapList->addTopLevelItem(topLevelItem);
	topLevelItem->setExpanded(true);
	wLocation->setupMapLocation(ProjectData::mapNames.keys().first());
	setLocation(ProjectData::mapNames.keys().first(), 0, 0, 0);
}

ConfigureEvent_Teleport::~ConfigureEvent_Teleport()
{
}

void ConfigureEvent_Teleport::setLocation(int mapID, int x, int y, int layer)
{
	if (rbSpecificLocation->isChecked())
	{
		selectMap(twMapList->topLevelItem(0), mapID);
		wLocation->changeMap(mapID);
		wLocation->setLocation(x, y, layer);
	}
	else
	{
		lblMapVariable->setText(ProjectData::numberVariableIdToString(mapID));
		lblXVariable->setText(ProjectData::numberVariableIdToString(x));
		lblYVariable->setText(ProjectData::numberVariableIdToString(y));
		lblLayerVariable->setText(ProjectData::numberVariableIdToString(layer));
	}
}

void ConfigureEvent_Teleport::setIsStoredLocation(bool stored)
{
	if (stored)
		rbStoredLocation->setChecked(true);
	else
		rbSpecificLocation->setChecked(true);
}

void ConfigureEvent_Teleport::setFacing(int facing)
{
	if (facing == RETAIN_CURRENT_FACING)
		rbRetainCurrentFacing->setChecked(true);
	else if (facing == FACE_UP)
		rbFaceUp->setChecked(true);
	else if (facing == FACE_DOWN)
		rbFaceDown->setChecked(true);
	else if (facing == FACE_LEFT)
		rbFaceLeft->setChecked(true);
	else
		rbFaceRight->setChecked(true);
}

int ConfigureEvent_Teleport::getMapID()
{
	if (rbSpecificLocation->isChecked())
		return twMapList->currentItem()->text(1).toInt(NULL, 16);
	return ProjectData::numberVariableIdFromString(lblMapVariable->text());
}

int ConfigureEvent_Teleport::getX()
{
	if (rbSpecificLocation->isChecked())
		return wLocation->getXLocation();
	return ProjectData::numberVariableIdFromString(lblXVariable->text());
}

int ConfigureEvent_Teleport::getY()
{
	if (rbSpecificLocation->isChecked())
		return wLocation->getYLocation();
	return ProjectData::numberVariableIdFromString(lblYVariable->text());
}

int ConfigureEvent_Teleport::getLayer()
{
	if (rbSpecificLocation->isChecked())
		return wLocation->getLayer();
	return ProjectData::numberVariableIdFromString(lblLayerVariable->text());
}

bool ConfigureEvent_Teleport::isStoredLocation()
{
	return rbStoredLocation->isChecked();
}

int ConfigureEvent_Teleport::getFacing()
{
	if (rbRetainCurrentFacing->isChecked())
		return RETAIN_CURRENT_FACING;
	else if (rbFaceUp->isChecked())
		return FACE_UP;
	else if (rbFaceDown->isChecked())
		return FACE_DOWN;
	else if (rbFaceLeft->isChecked())
		return FACE_LEFT;
	return FACE_RIGHT;
}

void ConfigureEvent_Teleport::on_bBrowseMapVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblMapVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblMapVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_Teleport::on_bBrowseXVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblXVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblXVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_Teleport::on_bBrowseYVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblYVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblYVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_Teleport::on_bBrowseLayerVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblLayerVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblLayerVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_Teleport::on_rbSpecificLocation_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wMap);
	else
		stackedWidget->setCurrentWidget(wVariables);
}

void ConfigureEvent_Teleport::on_twMapList_itemCollapsed(QTreeWidgetItem *item)
{
	if (item->text(1) == "top")
		item->setExpanded(true);
}

void ConfigureEvent_Teleport::on_twMapList_itemSelectionChanged()
{
	QTreeWidgetItem *currentItem = twMapList->currentItem();
	if (currentItem->text(1) != "top")
	{
		wLocation->changeMap(currentItem->text(1).toInt(NULL, 16));
		wLocation->setEnabled(true);
		buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
		wLocation->setLocation(0, 0, 0);
	}
	else
	{
		wLocation->setEnabled(false);
		buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	}
}

void ConfigureEvent_Teleport::buildMapTree(QTreeWidgetItem *parentItem, QTreeWidgetItem *parentMapItem)
{
	QTreeWidgetItem *item;
	for (int i = 0; i < parentMapItem->childCount(); ++i)
	{
		QTreeWidgetItem *childItem = parentMapItem->child(i);
		if (childItem->text(3) == "map")
		{
			item = new QTreeWidgetItem();
			item->setText(0, childItem->text(0));
			item->setText(1, childItem->text(1));
			item->setIcon(0, childItem->icon(0));
			parentItem->addChild(item);
			buildMapTree(item, childItem);
		}
	}
}

bool ConfigureEvent_Teleport::selectMap(QTreeWidgetItem *parentItem, int mapID)
{
	QTreeWidgetItem *childItem;
	bool foundOne = false;
	for (int i = 0; i < parentItem->childCount() && !foundOne; ++i)
	{
		childItem = parentItem->child(i);
		if (childItem->text(1).toInt(NULL, 16) == mapID)
		{
			twMapList->setCurrentItem(childItem);
			foundOne = true;
		}
		else
			foundOne = selectMap(childItem, mapID);
	}
	return foundOne;
}
