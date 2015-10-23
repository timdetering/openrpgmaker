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

#include <QPixmap>
#include <QPainter>
#include <QBitmap>
#include "menuitemtable.h"
#include "menuobject.h"
#include "menuscrollarea.h"
#include "projectdata.h"
#include "projectdata_baseevent.h"
#include "projectdata_font.h"
#include "storagefile.h"
#include "xmlParser.h"
#include "xmlTools.h"

MenuItemTable::MenuItemTable(MenuObject *parent) : MenuObject(parent)
{
	objectName = "itemList";
	type = MenuObject::TYPE_ITEMLIST;
	selectionChangedEvent = new BaseEvent(BaseEvent::MENUEVENT);
	acceptEvent = new BaseEvent(BaseEvent::MENUEVENT);
	cancelEvent = new BaseEvent(BaseEvent::MENUEVENT);
	enabledTopColor = ProjectData::systemGraphics.defaultFontEnabledTopColor;
	enabledBottomColor = ProjectData::systemGraphics.defaultFontEnabledBottomColor;
	disabledTopColor = ProjectData::systemGraphics.defaultFontDisabledTopColor;
	disabledBottomColor = ProjectData::systemGraphics.defaultFontDisabledBottomColor;
	x = 0;
	y = 0;
	width = 64;
	height = 64;
	tableType = TYPE_ITEMS;
	numColumns = 1;
	textAlignment = ALIGN_LEFT;
	selectionVariableID = 0;
	rowSpacing = 0;
	columnSpacing = 0;
	rowHeight = 32;
	textXOffset = 8;
	textYOffset = 8;
	fontID = ProjectData::defaultFontID;
	sortingEnabled = true;
	settings.items = new ItemSettings;
	settings.items->showCommonItems = true;
	settings.items->showWeapons = true;
	settings.items->showShields = true;
	settings.items->showArmor = true;
	settings.items->showHelmets = true;
	settings.items->showAccessories = true;
	settings.items->showMedicine = true;
	settings.items->showSkillBooks = true;
	settings.items->showSeeds = true;
	settings.items->showSkillScrolls = true;
	settings.items->showSwitches = true;
	settings.items->showEvents = true;
}

MenuItemTable::MenuItemTable(MainMenuLayout *layout, XMLNode ObjectNode, MenuObject *parent) : MenuObject(layout, ObjectNode, parent)
{
	type = MenuObject::TYPE_ITEMLIST;
	loadXMLData(ObjectNode);
}

MenuItemTable::MenuItemTable(PopupMenuLayout *layout, XMLNode ObjectNode, MenuObject *parent) : MenuObject(layout, ObjectNode, parent)
{
	type = MenuObject::TYPE_ITEMLIST;
	loadXMLData(ObjectNode);
}

MenuItemTable::MenuItemTable(TitleMenuLayout *layout, XMLNode ObjectNode, MenuObject *parent) : MenuObject(layout, ObjectNode, parent)
{
	type = MenuObject::TYPE_ITEMLIST;
	loadXMLData(ObjectNode);
}

MenuItemTable::MenuItemTable(MainMenuLayout *layout, StorageFile &storageFile, MenuObject *parent) : MenuObject(layout, storageFile, parent)
{
	type = MenuObject::TYPE_ITEMLIST;
	loadStorageData(storageFile);
}

MenuItemTable::MenuItemTable(PopupMenuLayout *layout, StorageFile &storageFile, MenuObject *parent) : MenuObject(layout, storageFile, parent)
{
	type = MenuObject::TYPE_ITEMLIST;
	loadStorageData(storageFile);
}

MenuItemTable::MenuItemTable(TitleMenuLayout *layout, StorageFile &storageFile, MenuObject *parent) : MenuObject(layout, storageFile, parent)
{
	type = MenuObject::TYPE_ITEMLIST;
	loadStorageData(storageFile);
}

MenuItemTable::~MenuItemTable()
{
	if (selectionChangedEvent != NULL)
	{
		delete selectionChangedEvent;
		selectionChangedEvent = NULL;
	}
	if (acceptEvent != NULL)
	{
		delete acceptEvent;
		acceptEvent = NULL;
	}
	if (cancelEvent != NULL)
	{
		delete cancelEvent;
		cancelEvent = NULL;
	}
	if (tableType == TYPE_ITEMS)
		delete (settings.items);
	else if (tableType == TYPE_SKILLS)
		delete (settings.skills);
	else if (tableType == TYPE_EQUIPMENT)
		delete (settings.equipment);
	else
		delete (settings.other);
}

void MenuItemTable::getCopy(MenuObject *object)
{
	MenuItemTable *tableObject = (MenuItemTable*)object;
	MenuObject::getCopy(object);
	tableObject->selectionChangedEvent = new BaseEvent(selectionChangedEvent);
	tableObject->acceptEvent = new BaseEvent(acceptEvent);
	tableObject->cancelEvent = new BaseEvent(cancelEvent);
	tableObject->enabledTopColor = enabledTopColor;
	tableObject->enabledBottomColor = enabledBottomColor;
	tableObject->disabledTopColor = disabledTopColor;
	tableObject->disabledBottomColor = disabledBottomColor;
	tableObject->setTableType(tableType);
	if (tableType == TYPE_ITEMS)
		tableObject->setSettings(settings.items);
	else if (tableType == TYPE_SKILLS)
		tableObject->setSettings(settings.skills);
	else if (tableType == TYPE_EQUIPMENT)
		tableObject->setSettings(settings.equipment);
	else
		tableObject->setSettings(settings.other);
	tableObject->numColumns = numColumns;
	tableObject->textAlignment = textAlignment;
	tableObject->selectionVariableID = selectionVariableID;
	tableObject->rowSpacing = rowSpacing;
	tableObject->columnSpacing = columnSpacing;
	tableObject->rowHeight = rowHeight;
	tableObject->textXOffset = textXOffset;
	tableObject->textYOffset = textYOffset;
	tableObject->fontID = fontID;
	tableObject->sortingEnabled = sortingEnabled;
	tableObject->updatePixmap();
}

XMLNode MenuItemTable::getObjectNode()
{
	XMLNode TempNode, ObjectNode = XMLNode::createXMLTopNode("object");
	QString tempString;
	ObjectNode.addAttribute("type", "item list");
	ObjectNode.addAttribute("objectName", objectName.toUtf8().data());
	ObjectNode.addAttribute("x", QString::number(x).toUtf8().data());
	ObjectNode.addAttribute("y", QString::number(y).toUtf8().data());
	ObjectNode.addAttribute("width", QString::number(width).toUtf8().data());
	ObjectNode.addAttribute("height", QString::number(height).toUtf8().data());
	TempNode = selectionChangedEvent->getEventNode();
	TempNode.updateName("onSelectionChangedEvent");
	ObjectNode.addChild(TempNode);
	TempNode = acceptEvent->getEventNode();
	TempNode.updateName("onAcceptEvent");
	ObjectNode.addChild(TempNode);
	TempNode = cancelEvent->getEventNode();
	TempNode.updateName("onCancelEvent");
	ObjectNode.addChild(TempNode);
	tempString = QString("#%1%2%3").arg(enabledTopColor.red(), 2, 16, QChar('0')).arg(enabledTopColor.green(), 2, 16, QChar('0')).arg(enabledTopColor.blue(), 2, 16, QChar('0'));
	tempString += QString("-#%1%2%3").arg(enabledBottomColor.red(), 2, 16, QChar('0')).arg(enabledBottomColor.green(), 2, 16, QChar('0')).arg(enabledBottomColor.blue(), 2, 16, QChar('0'));
	ObjectNode.addAttribute("enabledColor", tempString.toUtf8().data());
	tempString = QString("#%1%2%3").arg(disabledTopColor.red(), 2, 16, QChar('0')).arg(disabledTopColor.green(), 2, 16, QChar('0')).arg(disabledTopColor.blue(), 2, 16, QChar('0'));
	tempString += QString("-#%1%2%3").arg(disabledBottomColor.red(), 2, 16, QChar('0')).arg(disabledBottomColor.green(), 2, 16, QChar('0')).arg(disabledBottomColor.blue(), 2, 16, QChar('0'));
	ObjectNode.addAttribute("disabledColor", tempString.toUtf8().data());
	ObjectNode.addAttribute("numColumns", QString::number(numColumns).toUtf8().data());
	if (textAlignment == ALIGN_LEFT)
		ObjectNode.addAttribute("textAlignment", "left");
	else if (textAlignment == ALIGN_CENTER)
		ObjectNode.addAttribute("textAlignment", "center");
	else
		ObjectNode.addAttribute("textAlignment", "right");
	ObjectNode.addAttribute("selectionVariableID", QString::number(selectionVariableID).toUtf8().data());
	ObjectNode.addAttribute("rowSpacing", QString::number(rowSpacing).toUtf8().data());
	ObjectNode.addAttribute("columnSpacing", QString::number(columnSpacing).toUtf8().data());
	ObjectNode.addAttribute("rowHeight", QString::number(rowHeight).toUtf8().data());
	ObjectNode.addAttribute("textXOffset", QString::number(textXOffset).toUtf8().data());
	ObjectNode.addAttribute("textYOffset", QString::number(textYOffset).toUtf8().data());
	ObjectNode.addAttribute("fontID", QString::number(fontID).toUtf8().data());
	ObjectNode.addAttribute("sortingEnabled", (sortingEnabled) ? "true":"false");
	TempNode = ObjectNode.addChild("settings");
	if (tableType == TYPE_ITEMS)
		TempNode.addAttribute("tableType", "items");
	else if (tableType == TYPE_SKILLS)
		TempNode.addAttribute("tableType", "skills");
	else if (tableType == TYPE_EQUIPMENT)
		TempNode.addAttribute("tableType", "equipment");
	else if (tableType == TYPE_CHARACTERS)
		TempNode.addAttribute("tableType", "characters");
	else if (tableType == TYPE_CLASSES)
		TempNode.addAttribute("tableType", "classes");
	else if (tableType == TYPE_MONSTERS)
		TempNode.addAttribute("tableType", "monsters");
	else if (tableType == TYPE_VEHICLES)
		TempNode.addAttribute("tableType", "vehicles");
	if (tableType == TYPE_ITEMS)
	{
		TempNode.addAttribute("showCommonItems", (settings.items->showCommonItems) ? "true":"false");
		TempNode.addAttribute("showWeapons", (settings.items->showWeapons) ? "true":"false");
		TempNode.addAttribute("showShields", (settings.items->showShields) ? "true":"false");
		TempNode.addAttribute("showArmor", (settings.items->showArmor) ? "true":"false");
		TempNode.addAttribute("showHelmets", (settings.items->showHelmets) ? "true":"false");
		TempNode.addAttribute("showAccessories", (settings.items->showAccessories) ? "true":"false");
		TempNode.addAttribute("showMedicine", (settings.items->showMedicine) ? "true":"false");
		TempNode.addAttribute("showSkillBooks", (settings.items->showSkillBooks) ? "true":"false");
		TempNode.addAttribute("showSeeds", (settings.items->showSeeds) ? "true":"false");
		TempNode.addAttribute("showSkillScrolls", (settings.items->showSkillScrolls) ? "true":"false");
		TempNode.addAttribute("showSwitches", (settings.items->showSwitches) ? "true":"false");
		TempNode.addAttribute("showEvents", (settings.items->showEvents) ? "true":"false");
	}
	else if (tableType == TYPE_SKILLS)
	{
		QString tempString = "";
		for (int i = 0; i < settings.skills->skillTypesToShow.size(); ++i)
			tempString += (settings.skills->skillTypesToShow[i]) ? "1":"0";
		TempNode.addAttribute("partyMemberID", QString::number(settings.skills->memberID).toUtf8().data());
		TempNode.addAttribute("skillTypesToShow", tempString.toUtf8().data());
	}
	else if (tableType == TYPE_EQUIPMENT)
	{
		TempNode.addAttribute("partyMemberID", QString::number(settings.equipment->memberID).toUtf8().data());
		if (settings.equipment->equipmentType == EquipmentSettings::TYPE_WEAPONS)
			TempNode.addAttribute("equipmentType", "weapons");
		else if (settings.equipment->equipmentType == EquipmentSettings::TYPE_SHIELDS)
			TempNode.addAttribute("equipmentType", "shields");
		else if (settings.equipment->equipmentType == EquipmentSettings::TYPE_ARMOR)
			TempNode.addAttribute("equipmentType", "armor");
		else if (settings.equipment->equipmentType == EquipmentSettings::TYPE_HELMETS)
			TempNode.addAttribute("equipmentType", "helmets");
		else if (settings.equipment->equipmentType == EquipmentSettings::TYPE_ACCESSORIES)
			TempNode.addAttribute("equipmentType", "accessories");
	}
	else
	{
		TempNode.addAttribute("useVariablesToSetState", (settings.other->useVariablesToSetState) ? "true":"false");
		if (settings.other->useVariablesToSetState)
			TempNode.addAttribute("startVariableID", QString::number(settings.other->startVariableID).toUtf8().data());
	}
	return ObjectNode;
}

void MenuItemTable::saveToStorageFile(StorageFile &storageFile)
{
	unsigned long color;
	storageFile.putSignedInt(type);
	storageFile.putString(objectName.toUtf8().data());
	storageFile.putSignedInt(x);
	storageFile.putSignedInt(y);
	storageFile.putSignedInt(width);
	storageFile.putSignedInt(height);
	selectionChangedEvent->saveToStorageFile(storageFile);
	acceptEvent->saveToStorageFile(storageFile);
	cancelEvent->saveToStorageFile(storageFile);
	color = (((unsigned long)enabledTopColor.red()) << 16) + (((unsigned long)enabledTopColor.green()) << 8) + (unsigned long)enabledTopColor.blue();
	storageFile.putUnsignedLong(color);
	color = (((unsigned long)enabledBottomColor.red()) << 16) + (((unsigned long)enabledBottomColor.green()) << 8) + (unsigned long)enabledBottomColor.blue();
	storageFile.putUnsignedLong(color);
	color = (((unsigned long)disabledTopColor.red()) << 16) + (((unsigned long)disabledTopColor.green()) << 8) + (unsigned long)disabledTopColor.blue();
	storageFile.putUnsignedLong(color);
	color = (((unsigned long)disabledBottomColor.red()) << 16) + (((unsigned long)disabledBottomColor.green()) << 8) + (unsigned long)disabledBottomColor.blue();
	storageFile.putUnsignedLong(color);
	storageFile.putSignedInt(tableType);
	storageFile.putSignedInt(numColumns);
	storageFile.putSignedInt(textAlignment);
	storageFile.putSignedInt(selectionVariableID);
	storageFile.putSignedInt(rowSpacing);
	storageFile.putSignedInt(columnSpacing);
	storageFile.putSignedInt(rowHeight);
	storageFile.putSignedInt(textXOffset);
	storageFile.putSignedInt(textYOffset);
	storageFile.putSignedInt(fontID);
	storageFile.putBool(sortingEnabled);
	if (tableType == TYPE_ITEMS)
	{
		storageFile.putBool(settings.items->showCommonItems);
		storageFile.putBool(settings.items->showWeapons);
		storageFile.putBool(settings.items->showShields);
		storageFile.putBool(settings.items->showArmor);
		storageFile.putBool(settings.items->showHelmets);
		storageFile.putBool(settings.items->showAccessories);
		storageFile.putBool(settings.items->showMedicine);
		storageFile.putBool(settings.items->showSkillBooks);
		storageFile.putBool(settings.items->showSeeds);
		storageFile.putBool(settings.items->showSkillScrolls);
		storageFile.putBool(settings.items->showSwitches);
		storageFile.putBool(settings.items->showEvents);
	}
	else if (tableType == TYPE_SKILLS)
	{
		storageFile.putSignedInt(settings.skills->memberID);
		storageFile.putSignedInt(settings.skills->skillTypesToShow.size());
		for (int i = 0; i < settings.skills->skillTypesToShow.size(); ++i)
			storageFile.putBool(settings.skills->skillTypesToShow[i]);
	}
	else if (tableType == TYPE_EQUIPMENT)
	{
		storageFile.putSignedInt(settings.equipment->memberID);
		storageFile.putSignedInt((int)settings.equipment->equipmentType);
	}
	else
	{
		storageFile.putBool(settings.other->useVariablesToSetState);
		if (settings.other->useVariablesToSetState)
			storageFile.putSignedInt(settings.other->startVariableID);
	}
}

QStringList MenuItemTable::getResources()
{
	QStringList resources;
	resources.append(selectionChangedEvent->getResources());
	resources.append(acceptEvent->getResources());
	resources.append(cancelEvent->getResources());
	return resources;
}

BaseEvent *MenuItemTable::getSelectionChangedEvent()
{
	return selectionChangedEvent;
}

BaseEvent *MenuItemTable::getAcceptEvent()
{
	return acceptEvent;
}

BaseEvent *MenuItemTable::getCancelEvent()
{
	return cancelEvent;
}

QColor MenuItemTable::getEnabledTopColor()
{
	return enabledTopColor;
}

QColor MenuItemTable::getEnabledBottomColor()
{
	return enabledBottomColor;
}

void MenuItemTable::setEnabledColor(QColor top, QColor bottom)
{
	enabledTopColor = top;
	enabledBottomColor = bottom;
	updatePixmap();
}

QColor MenuItemTable::getDisabledTopColor()
{
	return disabledTopColor;
}

QColor MenuItemTable::getDisabledBottomColor()
{
	return disabledBottomColor;
}

void MenuItemTable::setDisabledColor(QColor top, QColor bottom)
{
	disabledTopColor = top;
	disabledBottomColor = bottom;
	updatePixmap();
}

int MenuItemTable::getTableType()
{
	return tableType;
}

void MenuItemTable::setTableType(int value)
{
	if (tableType == TYPE_ITEMS)
		delete (settings.items);
	else if (tableType == TYPE_SKILLS)
		delete (settings.skills);
	else if (tableType == TYPE_EQUIPMENT)
		delete (settings.equipment);
	else
		delete (settings.other);
	tableType = value;
	if (tableType == TYPE_ITEMS)
		settings.items = new ItemSettings;
	else if (tableType == TYPE_SKILLS)
		settings.skills = new SkillSettings;
	else if (tableType == TYPE_EQUIPMENT)
		settings.equipment = new EquipmentSettings;
	else
		settings.other = new OtherSettings;
	updatePixmap();
}

int MenuItemTable::getNumColumns()
{
	return numColumns;
}

void MenuItemTable::setNumColumns(int value)
{
	numColumns = value;
	updatePixmap();
}

int MenuItemTable::getTextAlignment()
{
	return textAlignment;
}

void MenuItemTable::setTextAlignment(int value)
{
	textAlignment = value;
	updatePixmap();
}

int MenuItemTable::getSelectionVariableID()
{
	return selectionVariableID;
}

void MenuItemTable::setSelectionVariableID(int value)
{
	selectionVariableID = value;
}

int MenuItemTable::getRowSpacing()
{
	return rowSpacing;
}

void MenuItemTable::setRowSpacing(int value)
{
	rowSpacing = value;
	updatePixmap();
}

int MenuItemTable::getColumnSpacing()
{
	return columnSpacing;
}

void MenuItemTable::setColumnSpacing(int value)
{
	columnSpacing = value;
	updatePixmap();
}

int MenuItemTable::getRowHeight()
{
	return rowHeight;
}

void MenuItemTable::setRowHeight(int value)
{
	rowHeight = value;
	updatePixmap();
}

int MenuItemTable::getTextXOffset()
{
	return textXOffset;
}

void MenuItemTable::setTextXOffset(int value)
{
	textXOffset = value;
	updatePixmap();
}

int MenuItemTable::getTextYOffset()
{
	return textYOffset;
}

void MenuItemTable::setTextYOffset(int value)
{
	textYOffset = value;
	updatePixmap();
}

int MenuItemTable::getFontID()
{
	return fontID;
}

void MenuItemTable::setFontID(int value)
{
	fontID = value;
	updatePixmap();
}

bool MenuItemTable::isSortingEnabled()
{
	return sortingEnabled;
}

void MenuItemTable::setSortingEnabled(bool value)
{
	sortingEnabled = value;
}

void MenuItemTable::getSettings(ItemSettings *value)
{
	value->showCommonItems = settings.items->showCommonItems;
	value->showWeapons = settings.items->showWeapons;
	value->showShields = settings.items->showShields;
	value->showArmor = settings.items->showArmor;
	value->showHelmets = settings.items->showHelmets;
	value->showAccessories = settings.items->showAccessories;
	value->showMedicine = settings.items->showMedicine;
	value->showSkillBooks = settings.items->showSkillBooks;
	value->showSeeds = settings.items->showSeeds;
	value->showSkillScrolls = settings.items->showSkillScrolls;
	value->showSwitches = settings.items->showSwitches;
	value->showEvents = settings.items->showEvents;
}

void MenuItemTable::getSettings(SkillSettings *value)
{
	for (int i = 0; i < settings.skills->skillTypesToShow.size(); ++i)
		value->skillTypesToShow.append(settings.skills->skillTypesToShow[i]);
	value->memberID = settings.skills->memberID;
}

void MenuItemTable::getSettings(EquipmentSettings *value)
{
	value->equipmentType = settings.equipment->equipmentType;
	value->memberID = settings.equipment->memberID;
}

void MenuItemTable::getSettings(OtherSettings *value)
{
	value->startVariableID = settings.other->startVariableID;
	value->useVariablesToSetState = settings.other->useVariablesToSetState;
}

void MenuItemTable::setSettings(ItemSettings *value)
{
	settings.items->showCommonItems = value->showCommonItems;
	settings.items->showWeapons = value->showWeapons;
	settings.items->showShields = value->showShields;
	settings.items->showArmor = value->showArmor;
	settings.items->showHelmets = value->showHelmets;
	settings.items->showAccessories = value->showAccessories;
	settings.items->showMedicine = value->showMedicine;
	settings.items->showSkillBooks = value->showSkillBooks;
	settings.items->showSeeds = value->showSeeds;
	settings.items->showSkillScrolls = value->showSkillScrolls;
	settings.items->showSwitches = value->showSwitches;
	settings.items->showEvents = value->showEvents;
}

void MenuItemTable::setSettings(SkillSettings *value)
{
	for (int i = 0; i < value->skillTypesToShow.size(); ++i)
		settings.skills->skillTypesToShow.append(value->skillTypesToShow[i]);
	settings.skills->memberID = value->memberID;
}

void MenuItemTable::setSettings(EquipmentSettings *value)
{
	settings.equipment->equipmentType = value->equipmentType;
	settings.equipment->memberID = value->memberID;
}

void MenuItemTable::setSettings(OtherSettings *value)
{
	settings.other->startVariableID =value->startVariableID;
	settings.other->useVariablesToSetState = value->useVariablesToSetState;
}

bool MenuItemTable::tryLoad(XMLNode ObjectNode)
{
	XMLNode TempNode;
	QStringList trueFalseList;
	QStringList tempStringList;
	QString tempString;
	trueFalseList << "true";
	trueFalseList << "false";
	if (!XMLTools::attributeExists(ObjectNode, "x"))
		return false;
	if (!XMLTools::attributeExists(ObjectNode, "y"))
		return false;
	if (!XMLTools::attributeExists(ObjectNode, "width"))
		return false;
	if (!XMLTools::attributeExists(ObjectNode, "height"))
		return false;
	if (!XMLTools::nodeExists(ObjectNode, "onSelectionChangedEvent"))
		return false;
	if (!XMLTools::nodeExists(ObjectNode, "onAcceptEvent"))
		return false;
	if (!XMLTools::nodeExists(ObjectNode, "onCancelEvent"))
		return false;
	if (!BaseEvent::tryLoad(ObjectNode.getChildNode("onSelectionChangedEvent"), BaseEvent::MENUEVENT))
		return false;
	if (!BaseEvent::tryLoad(ObjectNode.getChildNode("onAcceptEvent"), BaseEvent::MENUEVENT))
		return false;
	if (!BaseEvent::tryLoad(ObjectNode.getChildNode("onCancelEvent"), BaseEvent::MENUEVENT))
		return false;
	if (!XMLTools::attributeExists(ObjectNode, "enabledColor"))
		return false;
	if (!XMLTools::attributeExists(ObjectNode, "disabledColor"))
		return false;
	if (!XMLTools::attributeExists(ObjectNode, "numColumns"))
		return false;
	if (!XMLTools::attributeExists(ObjectNode, "textAlignment"))
		return false;
	tempStringList.clear();
	tempStringList << "left" << "center" << "right";
	if (!XMLTools::attributeStringValid(ObjectNode, "textAlignment", tempStringList))
		return false;
	if (!XMLTools::attributeExists(ObjectNode, "selectionVariableID"))
		return false;
	if (!XMLTools::attributeExists(ObjectNode, "rowSpacing"))
		return false;
	if (!XMLTools::attributeExists(ObjectNode, "columnSpacing"))
		return false;
	if (!XMLTools::attributeExists(ObjectNode, "rowHeight"))
		return false;
	if (!XMLTools::attributeExists(ObjectNode, "textXOffset"))
		return false;
	if (!XMLTools::attributeExists(ObjectNode, "textYOffset"))
		return false;
	if (!XMLTools::attributeExists(ObjectNode, "fontID"))
		return false;
	if (!XMLTools::attributeExists(ObjectNode, "sortingEnabled"))
		return false;
	if (!XMLTools::attributeStringValid(ObjectNode, "sortingEnabled", trueFalseList, 1))
		return false;
	if (!XMLTools::nodeExists(ObjectNode, "settings"))
		return false;
	return tryLoadSettings(ObjectNode.getChildNode("settings"));
}

bool MenuItemTable::tryLoadSettings(XMLNode SettingsNode)
{
	QStringList trueFalseList;
	QStringList tempStringList;
	QString tempString;
	trueFalseList << "true";
	trueFalseList << "false";
	tempString = SettingsNode.getAttribute("tableType");
	if (tempString == "items")
	{
		if (!XMLTools::attributeExists(SettingsNode, "showCommonItems"))
			return false;
		if (!XMLTools::attributeStringValid(SettingsNode, "showCommonItems", trueFalseList, 1))
			return false;
		if (!XMLTools::attributeExists(SettingsNode, "showWeapons"))
			return false;
		if (!XMLTools::attributeStringValid(SettingsNode, "showWeapons", trueFalseList, 1))
			return false;
		if (!XMLTools::attributeExists(SettingsNode, "showShields"))
			return false;
		if (!XMLTools::attributeStringValid(SettingsNode, "showShields", trueFalseList, 1))
			return false;
		if (!XMLTools::attributeExists(SettingsNode, "showArmor"))
			return false;
		if (!XMLTools::attributeStringValid(SettingsNode, "showArmor", trueFalseList, 1))
			return false;
		if (!XMLTools::attributeExists(SettingsNode, "showHelmets"))
			return false;
		if (!XMLTools::attributeStringValid(SettingsNode, "showHelmets", trueFalseList, 1))
			return false;
		if (!XMLTools::attributeExists(SettingsNode, "showAccessories"))
			return false;
		if (!XMLTools::attributeStringValid(SettingsNode, "showAccessories", trueFalseList, 1))
			return false;
		if (!XMLTools::attributeExists(SettingsNode, "showMedicine"))
			return false;
		if (!XMLTools::attributeStringValid(SettingsNode, "showMedicine", trueFalseList, 1))
			return false;
		if (!XMLTools::attributeExists(SettingsNode, "showSkillBooks"))
			return false;
		if (!XMLTools::attributeStringValid(SettingsNode, "showSkillBooks", trueFalseList, 1))
			return false;
		if (!XMLTools::attributeExists(SettingsNode, "showSeeds"))
			return false;
		if (!XMLTools::attributeStringValid(SettingsNode, "showSeeds", trueFalseList, 1))
			return false;
		if (!XMLTools::attributeExists(SettingsNode, "showSkillScrolls"))
			return false;
		if (!XMLTools::attributeStringValid(SettingsNode, "showSkillScrolls", trueFalseList, 1))
			return false;
		if (!XMLTools::attributeExists(SettingsNode, "showSwitches"))
			return false;
		if (!XMLTools::attributeStringValid(SettingsNode, "showSwitches", trueFalseList, 1))
			return false;
		if (!XMLTools::attributeExists(SettingsNode, "showEvents"))
			return false;
		if (!XMLTools::attributeStringValid(SettingsNode, "showEvents", trueFalseList, 1))
			return false;
	}
	else if (tempString == "skills")
	{
		if (!XMLTools::attributeExists(SettingsNode, "partyMemberID"))
			return false;
		if (!XMLTools::attributeExists(SettingsNode, "skillTypesToShow"))
			return false;
	}
	else if (tempString == "equipment")
	{
		if (!XMLTools::attributeExists(SettingsNode, "partyMemberID"))
			return false;
		if (!XMLTools::attributeExists(SettingsNode, "equipmentType"))
			return false;
		tempStringList.clear();
		tempStringList << "weapons" << "shields" << "armor" << "helmets" << "accessories";
		if (!XMLTools::attributeStringValid(SettingsNode, "equipmentType", tempStringList))
			return false;
	}
	else
	{
		if (!XMLTools::attributeExists(SettingsNode, "useVariablesToSetState"))
			return false;
		if (!XMLTools::attributeStringValid(SettingsNode, "useVariablesToSetString", trueFalseList))
			return false;
		if (QString(SettingsNode.getAttribute("useVariablesToSetString")) == "true")
		{
			if (!XMLTools::attributeExists(SettingsNode, "startVariableID"))
				return false;
		}
	}
	return true;
}

void MenuItemTable::resized()
{
	updatePixmap();
}

void MenuItemTable::updatePixmap()
{
	QPixmap selectionBorder = QPixmap(ProjectData::systemGraphics.selectionBorderLocation);
	QPixmap selectionArea;
	QPainter painter;
	QString text = "";
	int columnWidth = (width - (columnSpacing * (numColumns - 1))) / numColumns;
	if (tableType == TYPE_ITEMS)
		text = "Item List";
	else if (tableType == TYPE_SKILLS)
		text = "Skill List";
	else if (tableType == TYPE_EQUIPMENT)
		text = "Equipment List";
	else if (tableType == TYPE_CHARACTERS)
		text = "Character List";
	else if (tableType == TYPE_CLASSES)
		text = "Class List";
	else if (tableType == TYPE_MONSTERS)
		text = "Monster List";
	else if (tableType == TYPE_VEHICLES)
		text = "Vehicle List";
	renderedPixmap = QPixmap(width, height);
	if (columnWidth < 16)
		columnWidth = 16;
	selectionArea = QPixmap(columnWidth, rowHeight);
	painter.begin(&selectionArea);
	painter.fillRect(0, 0, columnWidth, rowHeight, QColor(255, 0, 255));
	for (int tempY = 16; tempY < rowHeight - 16; tempY += 32)
	{
		for (int tempX = 16; tempX < columnWidth - 16; tempX += 32)
			painter.drawPixmap(tempX, tempY, selectionBorder, 16, 16, 32, 32);
	}
	for (int i = 16; i < columnWidth - 16; i += 32)
	{
		painter.drawPixmap(i, 0, selectionBorder, 16, 0, 32, 16);
		painter.drawPixmap(i, rowHeight - 16, selectionBorder, 16, 48, 32, 16);
	}
	for (int i = 16; i < rowHeight - 16; i += 32)
	{
		painter.drawPixmap(0, i, selectionBorder, 0, 16, 16, 32);
		painter.drawPixmap(columnWidth - 16, i, selectionBorder, 48, 16, 16, 32);
	}
	painter.drawPixmap(0, 0, selectionBorder, 0, 0, 16, 16);
	painter.drawPixmap(columnWidth - 16, 0, selectionBorder, 48, 0, 16, 16);
	painter.drawPixmap(0, rowHeight - 16, selectionBorder, 0, 48, 16, 16);
	painter.drawPixmap(columnWidth - 16, rowHeight - 16, selectionBorder, 48, 48, 16, 16);
	painter.end();
	painter.begin(&renderedPixmap);
	painter.fillRect(0, 0, width, height, QColor(255, 0, 255));
	for (int i = 0; i < numColumns; ++i)
	{
		int x = (i * columnWidth) + (i * columnSpacing);
		painter.drawPixmap(x, 0, selectionArea);
		painter.drawPixmap(x, rowHeight + rowSpacing, selectionArea);
	}
	if (!text.isEmpty())
	{
		ProjectData::DataReference<Font> *fontRef = new ProjectData::DataReference<Font>(fontID, __FILE__, __LINE__);
		QPixmap pixmap = QPixmap::fromImage((*fontRef)->getRenderedText(text, enabledTopColor, enabledBottomColor));
		int x, y = textYOffset;
		if (textAlignment == ALIGN_LEFT)
			x = textXOffset;
		else if (textAlignment == ALIGN_CENTER)
			x = (columnWidth - pixmap.width()) / 2;
		else
			x = columnWidth - pixmap.width() - textXOffset;
		if (x + pixmap.width() > columnWidth)
			pixmap = pixmap.copy(0, 0, columnWidth - x, pixmap.height());
		if (y + pixmap.height() > rowHeight)
			pixmap = pixmap.copy(0, 0, pixmap.width(), rowHeight - y);
		painter.drawPixmap(x, y, pixmap);
		delete fontRef;
	}
	painter.end();
	if (parentObject == NULL)
		pixmapItem->setPos(x, y);
	else if (parentObject->getType() == MenuObject::TYPE_SCROLLAREA)
	{
		MenuScrollArea *scrollArea = (MenuScrollArea*)parentObject;
		pixmapItem->setPos(x - scrollArea->getCurrentScrollX(), y - scrollArea->getCurrentScrollY());
	}
	else
		pixmapItem->setPos(x, y);
	pixmapItem->setPixmap(renderedPixmap);
}

void MenuItemTable::loadXMLData(XMLNode ObjectNode)
{
	QString tempString;
	XMLNode TempNode;
	int red, green, blue;
	x = QString(ObjectNode.getAttribute("x")).toInt();
	y = QString(ObjectNode.getAttribute("y")).toInt();
	width = QString(ObjectNode.getAttribute("width")).toInt();
	height = QString(ObjectNode.getAttribute("height")).toInt();
	selectionChangedEvent = new BaseEvent(BaseEvent::MENUEVENT, ObjectNode.getChildNode("onSelectionChangedEvent"));
	acceptEvent = new BaseEvent(BaseEvent::MENUEVENT, ObjectNode.getChildNode("onAcceptEvent"));
	cancelEvent = new BaseEvent(BaseEvent::MENUEVENT, ObjectNode.getChildNode("onCancelEvent"));
	tempString = ObjectNode.getAttribute("enabledColor");
	red = tempString.mid(1, 2).toInt(0, 16);
	green = tempString.mid(3, 2).toInt(0, 16);
	blue = tempString.mid(5, 2).toInt(0, 16);
	enabledTopColor = QColor(red, green, blue);
	red = tempString.mid(9, 2).toInt(0, 16);
	green = tempString.mid(11, 2).toInt(0, 16);
	blue = tempString.mid(13, 2).toInt(0, 16);
	enabledBottomColor = QColor(red, green, blue);
	tempString = ObjectNode.getAttribute("disabledColor");
	red = tempString.mid(1, 2).toInt(0, 16);
	green = tempString.mid(3, 2).toInt(0, 16);
	blue = tempString.mid(5, 2).toInt(0, 16);
	disabledTopColor = QColor(red, green, blue);
	red = tempString.mid(9, 2).toInt(0, 16);
	green = tempString.mid(11, 2).toInt(0, 16);
	blue = tempString.mid(13, 2).toInt(0, 16);
	disabledBottomColor = QColor(red, green, blue);
	numColumns = QString(ObjectNode.getAttribute("numColumns")).toInt();
	tempString = ObjectNode.getAttribute("textAlignment");
	if (tempString == "left")
		textAlignment = ALIGN_LEFT;
	else if (tempString == "center")
		textAlignment = ALIGN_CENTER;
	else if (tempString == "right")
		textAlignment = ALIGN_RIGHT;
	selectionVariableID = QString(ObjectNode.getAttribute("selectionVariableID")).toInt();
	rowSpacing = QString(ObjectNode.getAttribute("rowSpacing")).toInt();
	columnSpacing = QString(ObjectNode.getAttribute("columnSpacing")).toInt();
	rowHeight = QString(ObjectNode.getAttribute("rowHeight")).toInt();
	textXOffset = QString(ObjectNode.getAttribute("textXOffset")).toInt();
	textYOffset = QString(ObjectNode.getAttribute("textYOffset")).toInt();
	fontID = QString(ObjectNode.getAttribute("fontID")).toInt();
	sortingEnabled = (QString(ObjectNode.getAttribute("sortingEnabled")) == "true") ? true:false;
	TempNode = ObjectNode.getChildNode("settings");
	tempString = TempNode.getAttribute("tableType");
	if (tempString == "items")
		tableType = TYPE_ITEMS;
	else if (tempString == "skills")
		tableType = TYPE_SKILLS;
	else if (tempString == "equipment")
		tableType = TYPE_EQUIPMENT;
	else if (tempString == "characters")
		tableType = TYPE_CHARACTERS;
	else if (tempString == "classes")
		tableType = TYPE_CLASSES;
	else if (tempString == "monsters")
		tableType = TYPE_MONSTERS;
	else if (tempString == "vehicles")
		tableType = TYPE_VEHICLES;
	if (tableType == TYPE_ITEMS)
	{
		settings.items = new ItemSettings;
		settings.items->showCommonItems = (QString(TempNode.getAttribute("showCommonItems")) == "true") ? true:false;
		settings.items->showWeapons = (QString(TempNode.getAttribute("showWeapons")) == "true") ? true:false;
		settings.items->showShields = (QString(TempNode.getAttribute("showShields")) == "true") ? true:false;
		settings.items->showArmor = (QString(TempNode.getAttribute("showArmor")) == "true") ? true:false;
		settings.items->showHelmets = (QString(TempNode.getAttribute("showHelmets")) == "true") ? true:false;
		settings.items->showAccessories = (QString(TempNode.getAttribute("showAccessories")) == "true") ? true:false;
		settings.items->showMedicine = (QString(TempNode.getAttribute("showMedicine")) == "true") ? true:false;
		settings.items->showSkillBooks = (QString(TempNode.getAttribute("showSkillBooks")) == "true") ? true:false;
		settings.items->showSeeds = (QString(TempNode.getAttribute("showSeeds")) == "true") ? true:false;
		settings.items->showSkillScrolls = (QString(TempNode.getAttribute("showSkillScrolls")) == "true") ? true:false;
		settings.items->showSwitches = (QString(TempNode.getAttribute("showSwitches")) == "true") ? true:false;
		settings.items->showEvents = (QString(TempNode.getAttribute("showEvents")) == "true") ? true:false;
	}
	else if (tableType == TYPE_SKILLS)
	{
		settings.skills = new SkillSettings;
		settings.skills->memberID = QString(TempNode.getAttribute("partyMemberID")).toInt();
		tempString = TempNode.getAttribute("skillTypesToShow");
		for (int i = 0; i < tempString.size(); ++i)
			settings.skills->skillTypesToShow.append((tempString[i] == QChar('1')) ? true:false);
	}
	else if (tableType == TYPE_EQUIPMENT)
	{
		settings.equipment = new EquipmentSettings;
		settings.equipment->memberID = QString(TempNode.getAttribute("partyMemberID")).toInt();
		tempString = TempNode.getAttribute("equipmentType");
		if (tempString == "weapons")
			settings.equipment->equipmentType = EquipmentSettings::TYPE_WEAPONS;
		else if (tempString == "shields")
			settings.equipment->equipmentType = EquipmentSettings::TYPE_SHIELDS;
		else if (tempString == "armor")
			settings.equipment->equipmentType = EquipmentSettings::TYPE_ARMOR;
		else if (tempString == "helmets")
			settings.equipment->equipmentType = EquipmentSettings::TYPE_HELMETS;
		else if (tempString == "accessories")
			settings.equipment->equipmentType = EquipmentSettings::TYPE_ACCESSORIES;
	}
	else
	{
		settings.other = new OtherSettings;
		settings.other->useVariablesToSetState = (QString(TempNode.getAttribute("useVariablesToSetState")) == "true") ? true:false;
		if (settings.other->useVariablesToSetState)
			settings.other->startVariableID = QString(TempNode.getAttribute("startVariableID")).toInt();
		else
			settings.other->startVariableID = 0;
	}
}

void MenuItemTable::loadStorageData(StorageFile &storageFile)
{
	unsigned long color;
	x = storageFile.getSignedInt();
	y = storageFile.getSignedInt();
	width = storageFile.getSignedInt();
	height = storageFile.getSignedInt();
	selectionChangedEvent = new BaseEvent(BaseEvent::MENUEVENT, storageFile);
	acceptEvent = new BaseEvent(BaseEvent::MENUEVENT, storageFile);
	cancelEvent = new BaseEvent(BaseEvent::MENUEVENT, storageFile);
	color = storageFile.getUnsignedLong();
	enabledTopColor = QColor((int)((color >> 16) % 256), (int)((color >> 8) % 256), (int)(color % 256));
	color = storageFile.getUnsignedLong();
	enabledBottomColor = QColor((int)((color >> 16) % 256), (int)((color >> 8) % 256), (int)(color % 256));
	color = storageFile.getUnsignedLong();
	disabledTopColor = QColor((int)((color >> 16) % 256), (int)((color >> 8) % 256), (int)(color % 256));
	color = storageFile.getUnsignedLong();
	disabledBottomColor = QColor((int)((color >> 16) % 256), (int)((color >> 8) % 256), (int)(color % 256));
	tableType = storageFile.getSignedInt();
	numColumns = storageFile.getSignedInt();
	textAlignment = storageFile.getSignedInt();
	selectionVariableID = storageFile.getSignedInt();
	rowSpacing = storageFile.getSignedInt();
	columnSpacing = storageFile.getSignedInt();
	rowHeight = storageFile.getSignedInt();
	textXOffset = storageFile.getSignedInt();
	textYOffset = storageFile.getSignedInt();
	fontID = storageFile.getSignedInt();
	sortingEnabled = storageFile.getBool();
	if (tableType == TYPE_ITEMS)
	{
		settings.items = new ItemSettings;
		settings.items->showCommonItems = storageFile.getBool();
		settings.items->showWeapons = storageFile.getBool();
		settings.items->showShields = storageFile.getBool();
		settings.items->showArmor = storageFile.getBool();
		settings.items->showHelmets = storageFile.getBool();
		settings.items->showAccessories = storageFile.getBool();
		settings.items->showMedicine = storageFile.getBool();
		settings.items->showSkillBooks = storageFile.getBool();
		settings.items->showSeeds = storageFile.getBool();
		settings.items->showSkillScrolls = storageFile.getBool();
		settings.items->showSwitches = storageFile.getBool();
		settings.items->showEvents = storageFile.getBool();
	}
	else if (tableType == TYPE_SKILLS)
	{
		settings.skills = new SkillSettings;
		settings.skills->memberID = storageFile.getSignedInt();
		for (int i = 0, count = storageFile.getSignedInt(); i < count; ++i)
			settings.skills->skillTypesToShow.append(storageFile.getBool());
	}
	else if (tableType == TYPE_EQUIPMENT)
	{
		settings.equipment = new EquipmentSettings;
		settings.equipment->memberID = storageFile.getSignedInt();
		settings.equipment->equipmentType = (EquipmentSettings::EquipmentType)storageFile.getSignedInt();
	}
	else
	{
		settings.other = new OtherSettings;
		settings.other->useVariablesToSetState = storageFile.getBool();
		if (settings.other->useVariablesToSetState)
			settings.other->startVariableID = storageFile.getSignedInt();
		else
			settings.other->startVariableID = 0;
	}
}
