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
#include "menutext.h"
#include "menuobject.h"
#include "menuscrollarea.h"
#include "projectdata.h"
#include "projectdata_character.h"
#include "projectdata_font.h"
#include "projectdata_item.h"
#include "projectdata_monster.h"
#include "storagefile.h"
#include "xmlParser.h"
#include "xmlTools.h"

MenuText::MenuText(MenuObject *parent) : MenuObject(parent)
{
	objectName = "text";
	type = MenuObject::TYPE_TEXT;
	text.plainText = new QString("Text");
	enabledTopColor = ProjectData::systemGraphics.defaultFontEnabledTopColor;
	enabledBottomColor = ProjectData::systemGraphics.defaultFontEnabledBottomColor;
	disabledTopColor = ProjectData::systemGraphics.defaultFontDisabledTopColor;
	disabledBottomColor = ProjectData::systemGraphics.defaultFontDisabledBottomColor;
	textType = TYPE_PLAINTEXT;
	textWidth = 0;
	alignment = ALIGN_LEFT;
	fontID = ProjectData::defaultFontID;
	saveSlotID = 0;
}

MenuText::MenuText(MainMenuLayout *layout, XMLNode ObjectNode, MenuObject *parent) : MenuObject(layout, ObjectNode, parent)
{
	type = MenuObject::TYPE_TEXT;
	loadXMLData(ObjectNode);
}

MenuText::MenuText(PopupMenuLayout *layout, XMLNode ObjectNode, MenuObject *parent) : MenuObject(layout, ObjectNode, parent)
{
	type = MenuObject::TYPE_TEXT;
	loadXMLData(ObjectNode);
}

MenuText::MenuText(TitleMenuLayout *layout, XMLNode ObjectNode, MenuObject *parent) : MenuObject(layout, ObjectNode, parent)
{
	type = MenuObject::TYPE_TEXT;
	loadXMLData(ObjectNode);
}

MenuText::MenuText(MainMenuLayout *layout, StorageFile &storageFile, MenuObject *parent) : MenuObject(layout, storageFile, parent)
{
	type = MenuObject::TYPE_TEXT;
	loadStorageData(storageFile);
}

MenuText::MenuText(PopupMenuLayout *layout, StorageFile &storageFile, MenuObject *parent) : MenuObject(layout, storageFile, parent)
{
	type = MenuObject::TYPE_TEXT;
	loadStorageData(storageFile);
}

MenuText::MenuText(TitleMenuLayout *layout, StorageFile &storageFile, MenuObject *parent) : MenuObject(layout, storageFile, parent)
{
	type = MenuObject::TYPE_TEXT;
	loadStorageData(storageFile);
}

MenuText::~MenuText()
{
	if (textType == TYPE_PLAINTEXT)
		delete text.plainText;
	if (textType == TYPE_PARTYMEMBER)
		delete text.partyMemberText;
	if (textType == TYPE_MONSTER)
		delete text.monsterText;
	if (textType == TYPE_ITEM)
		delete text.itemText;
	if (textType == TYPE_CHARACTER)
		delete text.characterText;
	if (textType == TYPE_VARIABLE)
		delete text.variableText;
	if (textType == TYPE_SWITCH)
		delete text.switchText;
	if (textType == TYPE_STRING)
		delete text.stringText;
}

void MenuText::getCopy(MenuObject *object)
{
	MenuText *textObject = (MenuText*)object;
	MenuObject::getCopy(object);
	textObject->enabledTopColor = enabledTopColor;
	textObject->enabledBottomColor = enabledBottomColor;
	textObject->disabledTopColor = disabledTopColor;
	textObject->disabledBottomColor = disabledBottomColor;
	textObject->finalText = "";
	textObject->setTextType(textType);
	if (textType == TYPE_PLAINTEXT)
		textObject->setText(*(text.plainText));
	if (textType == TYPE_PARTYMEMBER)
		textObject->setText(text.partyMemberText);
	if (textType == TYPE_MONSTER)
		textObject->setText(text.monsterText);
	if (textType == TYPE_ITEM)
		textObject->setText(text.itemText);
	if (textType == TYPE_CHARACTER)
		textObject->setText(text.characterText);
	if (textType == TYPE_VARIABLE)
		textObject->setText(text.variableText);
	if (textType == TYPE_SWITCH)
		textObject->setText(text.switchText);
	if (textType == TYPE_STRING)
		textObject->setText(text.stringText);
	textObject->textWidth = textWidth;
	textObject->alignment = alignment;
	textObject->fontID = fontID;
	textObject->saveSlotID = saveSlotID;
	textObject->updatePixmap();
}

XMLNode MenuText::getObjectNode()
{
	XMLNode ObjectNode = XMLNode::createXMLTopNode("object");
	QString tempString;
	ObjectNode.addAttribute("type", "text");
	ObjectNode.addAttribute("objectName", objectName.toUtf8().data());
	ObjectNode.addAttribute("x", QString::number(x).toUtf8().data());
	ObjectNode.addAttribute("y", QString::number(y).toUtf8().data());
	if (textType == TYPE_PLAINTEXT)
		ObjectNode.addAttribute("textType", "plain text");
	else if (textType == TYPE_PARTYMEMBER)
		ObjectNode.addAttribute("textType", "party member");
	else if (textType == TYPE_MONSTER)
		ObjectNode.addAttribute("textType", "monster");
	else if (textType == TYPE_ITEM)
		ObjectNode.addAttribute("textType", "item");
	else if (textType == TYPE_CHARACTER)
		ObjectNode.addAttribute("textType", "character");
	else if (textType == TYPE_VARIABLE)
		ObjectNode.addAttribute("textType", "variable");
	else if (textType == TYPE_SWITCH)
		ObjectNode.addAttribute("textType", "switch");
	else if (textType == TYPE_STRING)
		ObjectNode.addAttribute("textType", "string");
	ObjectNode.addAttribute("textWidth", QString::number(textWidth).toUtf8().data());
	if (alignment == ALIGN_LEFT)
		ObjectNode.addAttribute("alignment", "left");
	else if (alignment == ALIGN_CENTER)
		ObjectNode.addAttribute("alignment", "center");
	else
		ObjectNode.addAttribute("alignment", "right");
	ObjectNode.addAttribute("fontID", QString::number(fontID).toUtf8().data());
	ObjectNode.addAttribute("saveSlotID", QString::number(saveSlotID).toUtf8().data());
	tempString = QString("#%1%2%3").arg(enabledTopColor.red(), 2, 16, QChar('0')).arg(enabledTopColor.green(), 2, 16, QChar('0')).arg(enabledTopColor.blue(), 2, 16, QChar('0'));
	tempString += QString("-#%1%2%3").arg(enabledBottomColor.red(), 2, 16, QChar('0')).arg(enabledBottomColor.green(), 2, 16, QChar('0')).arg(enabledBottomColor.blue(), 2, 16, QChar('0'));
	ObjectNode.addAttribute("enabledColor", tempString.toUtf8().data());
	tempString = QString("#%1%2%3").arg(disabledTopColor.red(), 2, 16, QChar('0')).arg(disabledTopColor.green(), 2, 16, QChar('0')).arg(disabledTopColor.blue(), 2, 16, QChar('0'));
	tempString += QString("-#%1%2%3").arg(disabledBottomColor.red(), 2, 16, QChar('0')).arg(disabledBottomColor.green(), 2, 16, QChar('0')).arg(disabledBottomColor.blue(), 2, 16, QChar('0'));
	ObjectNode.addAttribute("disabledColor", tempString.toUtf8().data());
	if (textType == TYPE_PLAINTEXT)
		ObjectNode.addAttribute("plainText", text.plainText->toUtf8().data());
	else if (textType == TYPE_PARTYMEMBER)
	{
		ObjectNode.addAttribute("partyMemberID", QString::number(text.partyMemberText->memberID).toUtf8().data());
		if (text.partyMemberText->textType == PARTYMEMBER_NAME)
			ObjectNode.addAttribute("partyMemberTextType", "name");
		else if (text.partyMemberText->textType == PARTYMEMBER_CLASS)
			ObjectNode.addAttribute("partyMemberTextType", "class");
		else if (text.partyMemberText->textType == PARTYMEMBER_CONDITION)
			ObjectNode.addAttribute("partyMemberTextType", "condition");
		else if (text.partyMemberText->textType == PARTYMEMBER_LEVEL)
			ObjectNode.addAttribute("partyMemberTextType", "level");
		else if (text.partyMemberText->textType == PARTYMEMBER_HP)
			ObjectNode.addAttribute("partyMemberTextType", "hp");
		else if (text.partyMemberText->textType == PARTYMEMBER_MAXHP)
			ObjectNode.addAttribute("partyMemberTextType", "max hp");
		else if (text.partyMemberText->textType == PARTYMEMBER_MP)
			ObjectNode.addAttribute("partyMemberTextType", "mp");
		else if (text.partyMemberText->textType == PARTYMEMBER_MAXMP)
			ObjectNode.addAttribute("partyMemberTextType", "max mp");
		else if (text.partyMemberText->textType == PARTYMEMBER_EXPERIENCE)
			ObjectNode.addAttribute("partyMemberTextType", "experience");
		else if (text.partyMemberText->textType == PARTYMEMBER_EXPERIENCETONEXTLEVEL)
			ObjectNode.addAttribute("partyMemberTextType", "experience to next level");
		else if (text.partyMemberText->textType == PARTYMEMBER_BASEATTACK)
			ObjectNode.addAttribute("partyMemberTextType", "base attack");
		else if (text.partyMemberText->textType == PARTYMEMBER_TOTALATTACK)
			ObjectNode.addAttribute("partyMemberTextType", "total attack");
		else if (text.partyMemberText->textType == PARTYMEMBER_BASEDEFENSE)
			ObjectNode.addAttribute("partyMemberTextType", "base defense");
		else if (text.partyMemberText->textType == PARTYMEMBER_TOTALDEFENSE)
			ObjectNode.addAttribute("partyMemberTextType", "total defense");
		else if (text.partyMemberText->textType == PARTYMEMBER_BASEWISDOM)
			ObjectNode.addAttribute("partyMemberTextType", "base wisdom");
		else if (text.partyMemberText->textType == PARTYMEMBER_TOTALWISDOM)
			ObjectNode.addAttribute("partyMemberTextType", "total wisdom");
		else if (text.partyMemberText->textType == PARTYMEMBER_BASEAGILITY)
			ObjectNode.addAttribute("partyMemberTextType", "base agility");
		else if (text.partyMemberText->textType == PARTYMEMBER_TOTALAGILITY)
			ObjectNode.addAttribute("partyMemberTextType", "total agility");
		else if (text.partyMemberText->textType == PARTYMEMBER_EQUIPPEDWEAPON)
			ObjectNode.addAttribute("partyMemberTextType", "equipped weapon");
		else if (text.partyMemberText->textType == PARTYMEMBER_EQUIPPEDSHIELD)
			ObjectNode.addAttribute("partyMemberTextType", "equipped shield");
		else if (text.partyMemberText->textType == PARTYMEMBER_EQUIPPEDARMOR)
			ObjectNode.addAttribute("partyMemberTextType", "equipped armor");
		else if (text.partyMemberText->textType == PARTYMEMBER_EQUIPPEDHELMET)
			ObjectNode.addAttribute("partyMemberTextType", "equipped helmet");
		else if (text.partyMemberText->textType == PARTYMEMBER_EQUIPPEDACCESSORY)
			ObjectNode.addAttribute("partyMemberTextType", "equipped accessory");
		if (text.partyMemberText->textType >= PARTYMEMBER_EQUIPPEDWEAPON && text.partyMemberText->textType <= PARTYMEMBER_EQUIPPEDACCESSORY)
		{
			if (text.partyMemberText->equipmentTextType == EQUIPMENT_NAME)
				ObjectNode.addAttribute("partyMemberEquipmentTextType", "name");
			else if (text.partyMemberText->equipmentTextType == EQUIPMENT_DESCRIPTION)
				ObjectNode.addAttribute("partyMemberEquipmentTextType", "description");
			else if (text.partyMemberText->equipmentTextType == EQUIPMENT_ATTACK)
				ObjectNode.addAttribute("partyMemberEquipmentTextType", "attack");
			else if (text.partyMemberText->equipmentTextType == EQUIPMENT_DEFENSE)
				ObjectNode.addAttribute("partyMemberEquipmentTextType", "defense");
			else if (text.partyMemberText->equipmentTextType == EQUIPMENT_WISDOM)
				ObjectNode.addAttribute("partyMemberEquipmentTextType", "wisdom");
			else if (text.partyMemberText->equipmentTextType == EQUIPMENT_AGILITY)
				ObjectNode.addAttribute("partyMemberEquipmentTextType", "agility");
		}
	}
	else if (textType == TYPE_MONSTER)
	{
		ObjectNode.addAttribute("monsterID", QString::number(text.monsterText->monsterID).toUtf8().data());
		if (text.monsterText->textType == MONSTER_NAME)
			ObjectNode.addAttribute("monsterTextType", "name");
		else if (text.monsterText->textType == MONSTER_MONEY)
			ObjectNode.addAttribute("monsterTextType", "money");
		else if (text.monsterText->textType == MONSTER_HP)
			ObjectNode.addAttribute("monsterTextType", "hp");
		else if (text.monsterText->textType == MONSTER_MP)
			ObjectNode.addAttribute("monsterTextType", "mp");
		else if (text.monsterText->textType == MONSTER_ATTACK)
			ObjectNode.addAttribute("monsterTextType", "attack");
		else if (text.monsterText->textType == MONSTER_DEFENSE)
			ObjectNode.addAttribute("monsterTextType", "defense");
		else if (text.monsterText->textType == MONSTER_WISDOM)
			ObjectNode.addAttribute("monsterTextType", "wisdom");
		else if (text.monsterText->textType == MONSTER_AGILITY)
			ObjectNode.addAttribute("monsterTextType", "agility");
		else if (text.monsterText->textType == MONSTER_STRONGAGAINST)
			ObjectNode.addAttribute("monsterTextType", "strong against");
		else if (text.monsterText->textType == MONSTER_WEAKAGAINST)
			ObjectNode.addAttribute("monsterTextType", "weak against");
		else if (text.monsterText->textType == MONSTER_EXPERIENCE)
			ObjectNode.addAttribute("monsterTextType", "experience");
		ObjectNode.addAttribute("monsterIDIsReferencedInVariable", (text.monsterText->idIsReferencedInVariable) ? "true":"false");
	}
	else if (textType == TYPE_ITEM)
	{
		ObjectNode.addAttribute("itemID", QString::number(text.itemText->itemID).toUtf8().data());
		if (text.itemText->textType == ITEM_NAME)
			ObjectNode.addAttribute("itemTextType", "name");
		else if (text.itemText->textType == ITEM_DESCRIPTION)
			ObjectNode.addAttribute("itemTextType", "description");
		else if (text.itemText->textType == ITEM_PRICE)
			ObjectNode.addAttribute("itemTextType", "price");
		ObjectNode.addAttribute("itemIDIsReferencedInVariable", (text.itemText->idIsReferencedInVariable) ? "true":"false");
	}
	else if (textType == TYPE_CHARACTER)
	{
		ObjectNode.addAttribute("characterID", QString::number(text.characterText->characterID).toUtf8().data());
		if (text.characterText->textType == CHARACTER_NAME)
			ObjectNode.addAttribute("characterTextType", "name");
		else if (text.characterText->textType == CHARACTER_CLASS)
			ObjectNode.addAttribute("characterTextType", "class");
		else if (text.characterText->textType == CHARACTER_CONDITION)
			ObjectNode.addAttribute("characterTextType", "condition");
		else if (text.characterText->textType == CHARACTER_LEVEL)
			ObjectNode.addAttribute("characterTextType", "level");
		else if (text.characterText->textType == CHARACTER_HP)
			ObjectNode.addAttribute("characterTextType", "hp");
		else if (text.characterText->textType == CHARACTER_MAXHP)
			ObjectNode.addAttribute("characterTextType", "max hp");
		else if (text.characterText->textType == CHARACTER_MP)
			ObjectNode.addAttribute("characterTextType", "mp");
		else if (text.characterText->textType == CHARACTER_MAXMP)
			ObjectNode.addAttribute("characterTextType", "max mp");
		else if (text.characterText->textType == CHARACTER_EXPERIENCE)
			ObjectNode.addAttribute("characterTextType", "experience");
		else if (text.characterText->textType == CHARACTER_EXPERIENCETONEXTLEVEL)
			ObjectNode.addAttribute("characterTextType", "experience to next level");
		else if (text.characterText->textType == CHARACTER_BASEATTACK)
			ObjectNode.addAttribute("characterTextType", "base attack");
		else if (text.characterText->textType == CHARACTER_TOTALATTACK)
			ObjectNode.addAttribute("characterTextType", "total attack");
		else if (text.characterText->textType == CHARACTER_BASEDEFENSE)
			ObjectNode.addAttribute("characterTextType", "base defense");
		else if (text.characterText->textType == CHARACTER_TOTALDEFENSE)
			ObjectNode.addAttribute("characterTextType", "total defense");
		else if (text.characterText->textType == CHARACTER_BASEWISDOM)
			ObjectNode.addAttribute("characterTextType", "base wisdom");
		else if (text.characterText->textType == CHARACTER_TOTALWISDOM)
			ObjectNode.addAttribute("characterTextType", "total wisdom");
		else if (text.characterText->textType == CHARACTER_BASEAGILITY)
			ObjectNode.addAttribute("characterTextType", "base agility");
		else if (text.characterText->textType == CHARACTER_TOTALAGILITY)
			ObjectNode.addAttribute("characterTextType", "total agility");
		else if (text.characterText->textType == CHARACTER_EQUIPPEDWEAPON)
			ObjectNode.addAttribute("characterTextType", "equipped weapon");
		else if (text.characterText->textType == CHARACTER_EQUIPPEDSHIELD)
			ObjectNode.addAttribute("characterTextType", "equipped shield");
		else if (text.characterText->textType == CHARACTER_EQUIPPEDARMOR)
			ObjectNode.addAttribute("characterTextType", "equipped armor");
		else if (text.characterText->textType == CHARACTER_EQUIPPEDHELMET)
			ObjectNode.addAttribute("characterTextType", "equipped helmet");
		else if (text.characterText->textType == CHARACTER_EQUIPPEDACCESSORY)
			ObjectNode.addAttribute("characterTextType", "equipped accessory");
		if (text.characterText->textType >= CHARACTER_EQUIPPEDWEAPON && text.characterText->textType <= CHARACTER_EQUIPPEDACCESSORY)
		{
			if (text.characterText->equipmentTextType == EQUIPMENT_NAME)
				ObjectNode.addAttribute("characterEquipmentTextType", "name");
			else if (text.characterText->equipmentTextType == EQUIPMENT_DESCRIPTION)
				ObjectNode.addAttribute("characterEquipmentTextType", "description");
			else if (text.characterText->equipmentTextType == EQUIPMENT_ATTACK)
				ObjectNode.addAttribute("characterEquipmentTextType", "attack");
			else if (text.characterText->equipmentTextType == EQUIPMENT_DEFENSE)
				ObjectNode.addAttribute("characterEquipmentTextType", "defense");
			else if (text.characterText->equipmentTextType == EQUIPMENT_WISDOM)
				ObjectNode.addAttribute("characterEquipmentTextType", "wisdom");
			else if (text.characterText->equipmentTextType == EQUIPMENT_AGILITY)
				ObjectNode.addAttribute("characterEquipmentTextType", "agility");
		}
		ObjectNode.addAttribute("characterIDIsReferencedInVariable", (text.characterText->idIsReferencedInVariable) ? "true":"false");
	}
	else if (textType == TYPE_VARIABLE)
	{
		ObjectNode.addAttribute("variableID", QString::number(text.variableText->variableID).toUtf8().data());
		ObjectNode.addAttribute("variableAlwaysShowsPlusSign", (text.variableText->alwaysShowPlusSign) ? "true":"false");
	}
	else if (textType == TYPE_SWITCH)
	{
		ObjectNode.addAttribute("switchID", QString::number(text.switchText->switchID).toUtf8().data());
		ObjectNode.addAttribute("switchOnText", text.switchText->onText.toUtf8().data());
		ObjectNode.addAttribute("switchOffText", text.switchText->offText.toUtf8().data());
	}
	else if (textType == TYPE_STRING)
	{
		ObjectNode.addAttribute("stringID", QString::number(text.stringText->stringID).toUtf8().data());
		ObjectNode.addAttribute("systemString", (text.stringText->systemString) ? "true":"false");
	}
	return ObjectNode;
}

void MenuText::saveToStorageFile(StorageFile &storageFile)
{
	unsigned long color;
	storageFile.putSignedInt(type);
	storageFile.putString(objectName.toUtf8().data());
	storageFile.putSignedInt(x);
	storageFile.putSignedInt(y);
	color = (((unsigned long)enabledTopColor.red()) << 16) + (((unsigned long)enabledTopColor.green()) << 8) + (unsigned long)enabledTopColor.blue();
	storageFile.putUnsignedLong(color);
	color = (((unsigned long)enabledBottomColor.red()) << 16) + (((unsigned long)enabledBottomColor.green()) << 8) + (unsigned long)enabledBottomColor.blue();
	storageFile.putUnsignedLong(color);
	color = (((unsigned long)disabledTopColor.red()) << 16) + (((unsigned long)disabledTopColor.green()) << 8) + (unsigned long)disabledTopColor.blue();
	storageFile.putUnsignedLong(color);
	color = (((unsigned long)disabledBottomColor.red()) << 16) + (((unsigned long)disabledBottomColor.green()) << 8) + (unsigned long)disabledBottomColor.blue();
	storageFile.putUnsignedLong(color);
	storageFile.putSignedInt(textType);
	storageFile.putSignedInt(textWidth);
	storageFile.putSignedInt(alignment);
	storageFile.putSignedInt(fontID);
	storageFile.putSignedInt(saveSlotID);
	if (textType == TYPE_PLAINTEXT)
		storageFile.putString(text.plainText->toUtf8().data());
	else if (textType == TYPE_PARTYMEMBER)
	{
		storageFile.putSignedInt(text.partyMemberText->memberID);
		storageFile.putSignedInt(text.partyMemberText->textType);
		storageFile.putSignedInt(text.partyMemberText->equipmentTextType);
	}
	else if (textType == TYPE_MONSTER)
	{
		storageFile.putSignedInt(text.monsterText->monsterID);
		storageFile.putSignedInt(text.monsterText->textType);
		storageFile.putBool(text.monsterText->idIsReferencedInVariable);
	}
	else if (textType == TYPE_ITEM)
	{
		storageFile.putSignedInt(text.itemText->itemID);
		storageFile.putSignedInt(text.itemText->textType);
		storageFile.putBool(text.itemText->idIsReferencedInVariable);
	}
	else if (textType == TYPE_CHARACTER)
	{
		storageFile.putSignedInt(text.characterText->characterID);
		storageFile.putSignedInt(text.characterText->textType);
		storageFile.putSignedInt(text.characterText->equipmentTextType);
		storageFile.putBool(text.characterText->idIsReferencedInVariable);
	}
	else if (textType == TYPE_VARIABLE)
	{
		storageFile.putSignedInt(text.variableText->variableID);
		storageFile.putBool(text.variableText->alwaysShowPlusSign);
	}
	else if (textType == TYPE_SWITCH)
	{
		storageFile.putSignedInt(text.switchText->switchID);
		storageFile.putString(text.switchText->onText.toUtf8().data());
		storageFile.putString(text.switchText->offText.toUtf8().data());
	}
	else if (textType == TYPE_STRING)
	{
		storageFile.putSignedInt(text.stringText->stringID);
		storageFile.putBool(text.stringText->systemString);
	}
}

QStringList MenuText::getResources()
{
	return QStringList();
}

int MenuText::getTextType()
{
	return textType;
}

void MenuText::setTextType(int value)
{
	if (textType == TYPE_PLAINTEXT)
		delete text.plainText;
	if (textType == TYPE_PARTYMEMBER)
		delete text.partyMemberText;
	if (textType == TYPE_MONSTER)
		delete text.monsterText;
	if (textType == TYPE_ITEM)
		delete text.itemText;
	if (textType == TYPE_CHARACTER)
		delete text.characterText;
	if (textType == TYPE_VARIABLE)
		delete text.variableText;
	if (textType == TYPE_SWITCH)
		delete text.switchText;
	if (textType == TYPE_STRING)
		delete text.stringText;
	textType = value;
	if (textType == TYPE_PLAINTEXT)
		text.plainText = new QString("");
	if (textType == TYPE_PARTYMEMBER)
		text.partyMemberText = new PartyMemberText;
	if (textType == TYPE_MONSTER)
		text.monsterText = new MonsterText;
	if (textType == TYPE_ITEM)
		text.itemText = new ItemText;
	if (textType == TYPE_CHARACTER)
		text.characterText = new CharacterText;
	if (textType == TYPE_VARIABLE)
		text.variableText = new VariableText;
	if (textType == TYPE_SWITCH)
		text.switchText = new SwitchText;
	if (textType == TYPE_STRING)
		text.stringText = new StringText;
}

void MenuText::getText(QString *value)
{
	value->clear();
	value->append(*(text.plainText));
}

void MenuText::getText(PartyMemberText *value)
{
	value->memberID = text.partyMemberText->memberID;
	value->textType = text.partyMemberText->textType;
	value->equipmentTextType = text.partyMemberText->equipmentTextType;
}

void MenuText::getText(MonsterText *value)
{
	value->monsterID = text.monsterText->monsterID;
	value->textType = text.monsterText->textType;
	value->idIsReferencedInVariable = text.monsterText->idIsReferencedInVariable;
}

void MenuText::getText(ItemText *value)
{
	value->itemID = text.itemText->itemID;
	value->textType = text.itemText->textType;
	value->idIsReferencedInVariable = text.itemText->idIsReferencedInVariable;
}

void MenuText::getText(CharacterText *value)
{
	value->characterID = text.characterText->characterID;
	value->textType = text.characterText->textType;
	value->equipmentTextType = text.characterText->equipmentTextType;
	value->idIsReferencedInVariable = text.characterText->idIsReferencedInVariable;
}

void MenuText::getText(VariableText *value)
{
	value->variableID = text.variableText->variableID;
	value->alwaysShowPlusSign = text.variableText->alwaysShowPlusSign;
}

void MenuText::getText(SwitchText *value)
{
	value->switchID = text.switchText->switchID;
	value->onText = text.switchText->onText;
	value->offText = text.switchText->offText;
}

void MenuText::getText(StringText *value)
{
	value->stringID = text.stringText->stringID;
	value->systemString = text.stringText->systemString;
}

void MenuText::setText(QString value)
{
	text.plainText->clear();
	text.plainText->append(value);
	updatePixmap();
}

void MenuText::setText(PartyMemberText *value)
{
	text.partyMemberText->memberID = value->memberID;
	text.partyMemberText->textType = value->textType;
	text.partyMemberText->equipmentTextType = value->equipmentTextType;
	updatePixmap();
}

void MenuText::setText(MonsterText *value)
{
	text.monsterText->monsterID = value->monsterID;
	text.monsterText->textType = value->textType;
	text.monsterText->idIsReferencedInVariable = value->idIsReferencedInVariable;
	updatePixmap();
}

void MenuText::setText(ItemText *value)
{
	text.itemText->itemID = value->itemID;
	text.itemText->textType = value->textType;
	text.itemText->idIsReferencedInVariable = value->idIsReferencedInVariable;
	updatePixmap();
}

void MenuText::setText(CharacterText *value)
{
	text.characterText->characterID = value->characterID;
	text.characterText->textType = value->textType;
	text.characterText->equipmentTextType = value->equipmentTextType;
	text.characterText->idIsReferencedInVariable = value->idIsReferencedInVariable;
	updatePixmap();
}

void MenuText::setText(VariableText *value)
{
	text.variableText->variableID = value->variableID;
	text.variableText->alwaysShowPlusSign = value->alwaysShowPlusSign;
	updatePixmap();
}

void MenuText::setText(SwitchText *value)
{
	text.switchText->switchID = value->switchID;
	text.switchText->onText = value->onText;
	text.switchText->offText = value->offText;
	updatePixmap();
}

void MenuText::setText(StringText *value)
{
	text.stringText->stringID = value->stringID;
	text.stringText->systemString = value->systemString;
	updatePixmap();
}

QColor MenuText::getEnabledTopColor()
{
	return enabledTopColor;
}

QColor MenuText::getEnabledBottomColor()
{
	return enabledBottomColor;
}

void MenuText::setEnabledColor(QColor top, QColor bottom)
{
	enabledTopColor = top;
	enabledBottomColor = bottom;
	updatePixmap();
}

QColor MenuText::getDisabledTopColor()
{
	return disabledTopColor;
}

QColor MenuText::getDisabledBottomColor()
{
	return disabledBottomColor;
}

void MenuText::setDisabledColor(QColor top, QColor bottom)
{
	disabledTopColor = top;
	disabledBottomColor = bottom;
	updatePixmap();
}

int MenuText::getTextWidth()
{
	return textWidth;
}

void MenuText::setTextWidth(int value)
{
	textWidth = value;
	updatePixmap();
}

int MenuText::getAlignment()
{
	return alignment;
}

void MenuText::setAlignment(int value)
{
	alignment = value;
	updatePixmap();
}

int MenuText::getFontID()
{
	return fontID;
}

void MenuText::setFontID(int value)
{
	fontID = value;
	updatePixmap();
}

int MenuText::getSaveSlotID()
{
	return saveSlotID;
}

void MenuText::setSaveSlotID(int value)
{
	saveSlotID = value;
}

QString MenuText::getFinalText()
{
	return finalText;
}

bool MenuText::tryLoad(XMLNode ObjectNode)
{
	QStringList trueFalseList;
	QStringList tempStringList;
	trueFalseList << "true";
	trueFalseList << "false";
	if (!XMLTools::attributeExists(ObjectNode, "x"))
		return false;
	if (!XMLTools::attributeExists(ObjectNode, "y"))
		return false;
	if (!XMLTools::attributeExists(ObjectNode, "textWidth"))
		return false;
	tempStringList.clear();
	tempStringList << "left";
	tempStringList << "center";
	tempStringList << "right";
	if (!XMLTools::attributeExists(ObjectNode, "alignment"))
		return false;
	if (!XMLTools::attributeStringValid(ObjectNode, "alignment", tempStringList))
		return false;
	if (!XMLTools::attributeExists(ObjectNode, "fontID"))
		return false;
	if (!XMLTools::attributeExists(ObjectNode, "saveSlotID"))
		return false;
	if (!XMLTools::attributeIntRangeValid(ObjectNode, "saveSlotID", 0, 16))
		return false;
	if (!XMLTools::attributeExists(ObjectNode, "enabledColor"))
		return false;
	if (!XMLTools::attributeExists(ObjectNode, "disabledColor"))
		return false;
	return MenuText::tryLoadTextData(ObjectNode);
}

bool MenuText::tryLoadTextData(XMLNode TextNode)
{
	QStringList tempStringList;
	QStringList trueFalseList;
	QString temp_string;
	trueFalseList << "true";
	trueFalseList << "false";
	tempStringList << "plain text";
	tempStringList << "party member";
	tempStringList << "monster";
	tempStringList << "item";
	tempStringList << "character";
	tempStringList << "variable";
	tempStringList << "switch";
	tempStringList << "string";
	if (!XMLTools::attributeExists(TextNode, "textType"))
		return false;
	if (!XMLTools::attributeStringValid(TextNode, "textType", tempStringList))
		return false;
	temp_string = TextNode.getAttribute("textType");
	if (temp_string == "plain text")
	{
		if (!XMLTools::attributeExists(TextNode, "plainText"))
			return false;
	}
	else if (temp_string == "party member")
	{
		tempStringList.clear();
		tempStringList << "name";
		tempStringList << "class";
		tempStringList << "condition";
		tempStringList << "level";
		tempStringList << "hp";
		tempStringList << "max hp";
		tempStringList << "mp";
		tempStringList << "max mp";
		tempStringList << "experience";
		tempStringList << "experience to next level";
		tempStringList << "base attack";
		tempStringList << "total attack";
		tempStringList << "base defense";
		tempStringList << "total defense";
		tempStringList << "base wisdom";
		tempStringList << "total wisdom";
		tempStringList << "base agility";
		tempStringList << "total agility";
		tempStringList << "equipped weapon";
		tempStringList << "equipped shield";
		tempStringList << "equipped armor";
		tempStringList << "equipped helmet";
		tempStringList << "equipped accessory";
		if (!XMLTools::attributeExists(TextNode, "partyMemberID"))
			return false;
		if (!XMLTools::attributeExists(TextNode, "partyMemberTextType"))
			return false;
		if (!XMLTools::attributeStringValid(TextNode, "partyMemberTextType", tempStringList))
			return false;
		tempStringList.clear();
		tempStringList << "equipped weapon";
		tempStringList << "equipped shield";
		tempStringList << "equipped armor";
		tempStringList << "equipped helmet";
		tempStringList << "equipped accessory";
		if (tempStringList.contains(TextNode.getAttribute("partyMemberTextType")))
		{
			tempStringList.clear();
			tempStringList << "name";
			tempStringList << "description";
			tempStringList << "attack";
			tempStringList << "defense";
			tempStringList << "wisdom";
			tempStringList << "agility";
			if (!XMLTools::attributeExists(TextNode, "partyMemberEquipmentTextType"))
				return false;
			if (!XMLTools::attributeStringValid(TextNode, "partyMemberEquipmentTextType", tempStringList))
				return false;
		}
	}
	else if (temp_string == "monster")
	{
		tempStringList.clear();
		tempStringList << "name";
		tempStringList << "money";
		tempStringList << "hp";
		tempStringList << "mp";
		tempStringList << "attack";
		tempStringList << "defense";
		tempStringList << "wisdom";
		tempStringList << "agility";
		tempStringList << "strong against";
		tempStringList << "weak against";
		tempStringList << "experience";
		if (!XMLTools::attributeExists(TextNode, "monsterID"))
			return false;
		if (!XMLTools::attributeExists(TextNode, "monsterTextType"))
			return false;
		if (!XMLTools::attributeStringValid(TextNode, "monsterTextType", tempStringList))
			return false;
		if (!XMLTools::attributeExists(TextNode, "monsterIDIsReferencedInVariable"))
			return false;
		if (!XMLTools::attributeStringValid(TextNode, "monsterIDIsReferencedInVariable", trueFalseList, 1))
			return false;
	}
	else if (temp_string == "item")
	{
		tempStringList.clear();
		tempStringList << "name";
		tempStringList << "description";
		tempStringList << "price";
		if (!XMLTools::attributeExists(TextNode, "itemID"))
			return false;
		if (!XMLTools::attributeExists(TextNode, "itemTextType"))
			return false;
		if (!XMLTools::attributeStringValid(TextNode, "itemTextType", tempStringList))
			return false;
		if (!XMLTools::attributeExists(TextNode, "itemIDIsReferencedInVariable"))
			return false;
		if (!XMLTools::attributeStringValid(TextNode, "itemIDIsReferencedInVariable", trueFalseList, 1))
			return false;
	}
	else if (temp_string == "character")
	{
		tempStringList.clear();
		tempStringList << "name";
		tempStringList << "class";
		tempStringList << "condition";
		tempStringList << "level";
		tempStringList << "hp";
		tempStringList << "max hp";
		tempStringList << "mp";
		tempStringList << "max mp";
		tempStringList << "experience";
		tempStringList << "experience to next level";
		tempStringList << "base attack";
		tempStringList << "total attack";
		tempStringList << "base defense";
		tempStringList << "total defense";
		tempStringList << "base wisdom";
		tempStringList << "total wisdom";
		tempStringList << "base agility";
		tempStringList << "total agility";
		tempStringList << "equipped weapon";
		tempStringList << "equipped shield";
		tempStringList << "equipped armor";
		tempStringList << "equipped helmet";
		tempStringList << "equipped accessory";
		if (!XMLTools::attributeExists(TextNode, "characterID"))
			return false;
		if (!XMLTools::attributeExists(TextNode, "characterTextType"))
			return false;
		if (!XMLTools::attributeStringValid(TextNode, "characterTextType", tempStringList))
			return false;
		tempStringList.clear();
		tempStringList << "equipped weapon";
		tempStringList << "equipped shield";
		tempStringList << "equipped armor";
		tempStringList << "equipped helmet";
		tempStringList << "equipped accessory";
		if (tempStringList.contains(TextNode.getAttribute("characterTextType")))
		{
			tempStringList.clear();
			tempStringList << "name";
			tempStringList << "description";
			tempStringList << "attack";
			tempStringList << "defense";
			tempStringList << "wisdom";
			tempStringList << "agility";
			if (!XMLTools::attributeExists(TextNode, "characterEquipmentTextType"))
				return false;
			if (!XMLTools::attributeStringValid(TextNode, "characterEquipmentTextType", tempStringList))
				return false;
		}
		if (!XMLTools::attributeExists(TextNode, "characterIDIsReferencedInVariable"))
			return false;
		if (!XMLTools::attributeStringValid(TextNode, "characterIDIsReferencedInVariable", trueFalseList, 1))
			return false;
	}
	else if (temp_string == "variable")
	{
		if (!XMLTools::attributeExists(TextNode, "variableID"))
			return false;
		if (!XMLTools::attributeExists(TextNode, "variableAlwaysShowsPlusSign"))
			return false;
		if (!XMLTools::attributeStringValid(TextNode, "variableAlwaysShowsPlusSign", trueFalseList, 1))
			return false;
	}
	else if (temp_string == "switch")
	{
		if (!XMLTools::attributeExists(TextNode, "switchID"))
			return false;
		if (!XMLTools::attributeExists(TextNode, "switchOnText"))
			return false;
		if (!XMLTools::attributeExists(TextNode, "switchOffText"))
			return false;
	}
	else if (temp_string == "string")
	{
		if (!XMLTools::attributeExists(TextNode, "stringID"))
			return false;
		if (!XMLTools::attributeExists(TextNode, "systemString"))
			return false;
		if (!XMLTools::attributeStringValid(TextNode, "systemString", trueFalseList, 1))
			return false;
	}
	return true;
}

void MenuText::updatePixmap()
{
	QPixmap pixmap;
	QPainter painter;
	ProjectData::DataReference<Font> *fontRef = new ProjectData::DataReference<Font>(fontID, __FILE__, __LINE__);
	finalText = "";
	if (textType == TYPE_PLAINTEXT)
		finalText = *(text.plainText);
	if (textType == TYPE_PARTYMEMBER)
	{
		int charID;
		if (text.partyMemberText->memberID >= 0 && text.partyMemberText->memberID <= 3)
			charID = ProjectData::initialParty[text.partyMemberText->memberID];
		else
			charID = ProjectData::initialParty[0];
		if (charID >= 0 && charID < ProjectData::characterNames.size())
		{
			ProjectData::DataReference<Character> *charRef = new ProjectData::DataReference<Character>(charID, __FILE__, __LINE__);
			if (text.partyMemberText->textType == PARTYMEMBER_NAME)
				finalText = ProjectData::characterNames[charID];
			else if (text.partyMemberText->textType == PARTYMEMBER_CLASS)
			{
				int classID = (*charRef)->getClassID();
				if (classID >= 0 && classID < ProjectData::classNames.size())
					finalText = ProjectData::classNames[classID];
				else
					finalText = "None";
			}
			else if (text.partyMemberText->textType == PARTYMEMBER_CONDITION)
				finalText = ProjectData::strings[ProjectData::STRING_STATSANDEQUIP_NORMALSTATUS];
			else if (text.partyMemberText->textType == PARTYMEMBER_LEVEL)
				finalText = QString::number((*charRef)->getMinLevel());
			else if (text.partyMemberText->textType == PARTYMEMBER_HP)
				finalText = QString::number((*charRef)->getMaxHP((*charRef)->getMinLevel()));
			else if (text.partyMemberText->textType == PARTYMEMBER_MAXHP)
				finalText = QString::number((*charRef)->getMaxHP((*charRef)->getMinLevel()));
			else if (text.partyMemberText->textType == PARTYMEMBER_MP)
				finalText = QString::number((*charRef)->getMaxMP((*charRef)->getMinLevel()));
			else if (text.partyMemberText->textType == PARTYMEMBER_MAXMP)
				finalText = QString::number((*charRef)->getMaxMP((*charRef)->getMinLevel()));
			else if (text.partyMemberText->textType == PARTYMEMBER_EXPERIENCE)
			{
				if ((*charRef)->getMinLevel() == 99)
					finalText = "-------";
				else
				{
					int totalExperience = (*charRef)->getTotalExperience((*charRef)->getMinLevel());
					if (totalExperience <= 9999999)
						finalText = QString::number(totalExperience);
					else
						finalText = "-------";
				}
			}
			else if (text.partyMemberText->textType == PARTYMEMBER_EXPERIENCETONEXTLEVEL)
			{
				if ((*charRef)->getMinLevel() == 99)
					finalText = "-------";
				else
				{
					int totalExperience = (*charRef)->getTotalExperience((*charRef)->getMinLevel() + 1);
					if (totalExperience <= 9999999)
						finalText = QString::number(totalExperience);
					else
						finalText = "-------";
				}
			}
			else if (text.partyMemberText->textType == PARTYMEMBER_BASEATTACK)
				finalText = QString::number((*charRef)->getAttack((*charRef)->getMinLevel()));
			else if (text.partyMemberText->textType == PARTYMEMBER_TOTALATTACK)
			{
				ProjectData::DataReference<Item> *itemRef;
				int level = (*charRef)->getMinLevel();
				int baseAttack = (*charRef)->getAttack(level);
				int weaponID, weaponAttack;
				int shieldID, shieldAttack;
				int armorID, armorAttack;
				int helmetID, helmetAttack;
				int accessoryID, accessoryAttack;
				weaponID = (*charRef)->getWeaponID();
				shieldID = (*charRef)->getShieldID();
				armorID = (*charRef)->getArmorID();
				helmetID = (*charRef)->getHelmetID();
				accessoryID = (*charRef)->getAccessoryID();
				if (weaponID == -1)
					weaponAttack = 0;
				else
				{
					itemRef = new ProjectData::DataReference<Item>(weaponID, __FILE__, __LINE__);
					weaponAttack = (*itemRef)->getWeaponData()->attackChange;
					delete itemRef;
				}
				if (shieldID == -1)
					shieldAttack = 0;
				else
				{
					itemRef = new ProjectData::DataReference<Item>(shieldID, __FILE__, __LINE__);
					if ((*charRef)->isDuelWielding())
						shieldAttack = (*itemRef)->getWeaponData()->attackChange;
					else
						shieldAttack = (*itemRef)->getEquipmentData()->attackChange;
					delete itemRef;
				}
				if (armorID == -1)
					armorAttack = -1;
				else
				{
					itemRef = new ProjectData::DataReference<Item>(armorID, __FILE__, __LINE__);
					armorAttack = (*itemRef)->getEquipmentData()->attackChange;
					delete itemRef;
				}
				if (helmetID == -1)
					helmetAttack = 0;
				else
				{
					itemRef = new ProjectData::DataReference<Item>(helmetID, __FILE__, __LINE__);
					helmetAttack = (*itemRef)->getEquipmentData()->attackChange;
					delete itemRef;
				}
				if (accessoryID == -1)
					accessoryAttack = 0;
				else
				{
					itemRef = new ProjectData::DataReference<Item>(accessoryID, __FILE__, __LINE__);
					accessoryAttack = (*itemRef)->getEquipmentData()->attackChange;
					delete itemRef;
				}
				finalText = QString::number(baseAttack + weaponAttack + shieldAttack + armorAttack + helmetAttack + accessoryAttack);
			}
			else if (text.partyMemberText->textType == PARTYMEMBER_BASEDEFENSE)
				finalText = QString::number((*charRef)->getDefense((*charRef)->getMinLevel()));
			else if (text.partyMemberText->textType == PARTYMEMBER_TOTALDEFENSE)
			{
				ProjectData::DataReference<Item> *itemRef;
				int level = (*charRef)->getMinLevel();
				int baseDefense = (*charRef)->getDefense(level);
				int weaponID, weaponDefense;
				int shieldID, shieldDefense;
				int armorID, armorDefense;
				int helmetID, helmetDefense;
				int accessoryID, accessoryDefense;
				weaponID = (*charRef)->getWeaponID();
				shieldID = (*charRef)->getShieldID();
				armorID = (*charRef)->getArmorID();
				helmetID = (*charRef)->getHelmetID();
				accessoryID = (*charRef)->getAccessoryID();
				if (weaponID == -1)
					weaponDefense = 0;
				else
				{
					itemRef = new ProjectData::DataReference<Item>(weaponID, __FILE__, __LINE__);
					weaponDefense = (*itemRef)->getWeaponData()->defenseChange;
					delete itemRef;
				}
				if (shieldID == -1)
					shieldDefense = 0;
				else
				{
					itemRef = new ProjectData::DataReference<Item>(shieldID, __FILE__, __LINE__);
					if ((*charRef)->isDuelWielding())
						shieldDefense = (*itemRef)->getWeaponData()->defenseChange;
					else
						shieldDefense = (*itemRef)->getEquipmentData()->defenseChange;
					delete itemRef;
				}
				if (armorID == -1)
					armorDefense = 0;
				else
				{
					itemRef = new ProjectData::DataReference<Item>(armorID, __FILE__, __LINE__);
					armorDefense = (*itemRef)->getEquipmentData()->defenseChange;
					delete itemRef;
				}
				if (helmetID == -1)
					helmetDefense = 0;
				else
				{
					itemRef = new ProjectData::DataReference<Item>(helmetID, __FILE__, __LINE__);
					helmetDefense = (*itemRef)->getEquipmentData()->defenseChange;
					delete itemRef;
				}
				if (accessoryID == -1)
					accessoryDefense = 0;
				else
				{
					itemRef = new ProjectData::DataReference<Item>(accessoryID, __FILE__, __LINE__);
					accessoryDefense = (*itemRef)->getEquipmentData()->defenseChange;
					delete itemRef;
				}
				finalText = QString::number(baseDefense + weaponDefense + shieldDefense + armorDefense + helmetDefense + accessoryDefense);
			}
			else if (text.partyMemberText->textType == PARTYMEMBER_BASEWISDOM)
				finalText = QString::number((*charRef)->getWisdom((*charRef)->getMinLevel()));
			else if (text.partyMemberText->textType == PARTYMEMBER_TOTALWISDOM)
			{
				ProjectData::DataReference<Item> *itemRef;
				int level = (*charRef)->getMinLevel();
				int baseWisdom = (*charRef)->getWisdom(level);
				int weaponID, weaponWisdom;
				int shieldID, shieldWisdom;
				int armorID, armorWisdom;
				int helmetID, helmetWisdom;
				int accessoryID, accessoryWisdom;
				weaponID = (*charRef)->getWeaponID();
				shieldID = (*charRef)->getShieldID();
				armorID = (*charRef)->getArmorID();
				helmetID = (*charRef)->getHelmetID();
				accessoryID = (*charRef)->getAccessoryID();
				if (weaponID == -1)
					weaponWisdom = 0;
				else
				{
					itemRef = new ProjectData::DataReference<Item>(weaponID, __FILE__, __LINE__);
					weaponWisdom = (*itemRef)->getWeaponData()->wisdomChange;
					delete itemRef;
				}
				if (shieldID == -1)
					shieldWisdom = 0;
				else
				{
					itemRef = new ProjectData::DataReference<Item>(shieldID, __FILE__, __LINE__);
					if ((*charRef)->isDuelWielding())
						shieldWisdom = (*itemRef)->getWeaponData()->wisdomChange;
					else
						shieldWisdom = (*itemRef)->getEquipmentData()->wisdomChange;
					delete itemRef;
				}
				if (armorID == -1)
					armorWisdom = 0;
				else
				{
					itemRef = new ProjectData::DataReference<Item>(armorID, __FILE__, __LINE__);
					armorWisdom = (*itemRef)->getEquipmentData()->wisdomChange;
					delete itemRef;
				}
				if (helmetID == -1)
					helmetWisdom = 0;
				else
				{
					itemRef = new ProjectData::DataReference<Item>(helmetID, __FILE__, __LINE__);
					helmetWisdom = (*itemRef)->getEquipmentData()->wisdomChange;
					delete itemRef;
				}
				if (accessoryID == -1)
					accessoryWisdom = 0;
				else
				{
					itemRef = new ProjectData::DataReference<Item>(accessoryID, __FILE__, __LINE__);
					accessoryWisdom = (*itemRef)->getEquipmentData()->wisdomChange;
					delete itemRef;
				}
				finalText = QString::number(baseWisdom + weaponWisdom + shieldWisdom + armorWisdom + helmetWisdom + accessoryWisdom);
			}
			else if (text.partyMemberText->textType == PARTYMEMBER_BASEAGILITY)
				finalText = QString::number((*charRef)->getAgility((*charRef)->getMinLevel()));
			else if (text.partyMemberText->textType == PARTYMEMBER_TOTALAGILITY)
			{
				ProjectData::DataReference<Item> *itemRef;
				int level = (*charRef)->getMinLevel();
				int baseAgility = (*charRef)->getAgility(level);
				int weaponID, weaponAgility;
				int shieldID, shieldAgility;
				int armorID, armorAgility;
				int helmetID, helmetAgility;
				int accessoryID, accessoryAgility;
				weaponID = (*charRef)->getWeaponID();
				shieldID = (*charRef)->getShieldID();
				armorID = (*charRef)->getArmorID();
				helmetID = (*charRef)->getHelmetID();
				accessoryID = (*charRef)->getAccessoryID();
				if (weaponID == -1)
					weaponAgility = 0;
				else
				{
					itemRef = new ProjectData::DataReference<Item>(weaponID, __FILE__, __LINE__);
					weaponAgility = (*itemRef)->getWeaponData()->agilityChange;
					delete itemRef;
				}
				if (shieldID == -1)
					shieldAgility = 0;
				else
				{
					itemRef = new ProjectData::DataReference<Item>(shieldID, __FILE__, __LINE__);
					if ((*charRef)->isDuelWielding())
						shieldAgility = (*itemRef)->getWeaponData()->agilityChange;
					else
						shieldAgility = (*itemRef)->getEquipmentData()->agilityChange;
					delete itemRef;
				}
				if (armorID == -1)
					armorAgility = 0;
				else
				{
					itemRef = new ProjectData::DataReference<Item>(armorID, __FILE__, __LINE__);
					armorAgility = (*itemRef)->getEquipmentData()->agilityChange;
					delete itemRef;
				}
				if (helmetID == -1)
					helmetAgility = 0;
				else
				{
					itemRef = new ProjectData::DataReference<Item>(helmetID, __FILE__, __LINE__);
					helmetAgility = (*itemRef)->getEquipmentData()->agilityChange;
					delete itemRef;
				}
				if (accessoryID == -1)
					accessoryAgility = 0;
				else
				{
					itemRef = new ProjectData::DataReference<Item>(accessoryID, __FILE__, __LINE__);
					accessoryAgility = (*itemRef)->getEquipmentData()->agilityChange;
					delete itemRef;
				}
				finalText = QString::number(baseAgility + weaponAgility + shieldAgility + armorAgility + helmetAgility + accessoryAgility);
			}
			else if (text.partyMemberText->textType == PARTYMEMBER_EQUIPPEDWEAPON)
			{
				if ((*charRef)->getWeaponID() == -1)
					finalText = "";
				else
				{
					ProjectData::DataReference<Item> *itemRef = new ProjectData::DataReference<Item>((*charRef)->getWeaponID(), __FILE__, __LINE__);
					Item::WeaponItem *data = (*itemRef)->getWeaponData();
					if (text.partyMemberText->equipmentTextType == EQUIPMENT_NAME)
						finalText = (*itemRef)->getName();
					else if (text.partyMemberText->equipmentTextType == EQUIPMENT_DESCRIPTION)
						finalText = (*itemRef)->getDescription();
					else if (text.partyMemberText->equipmentTextType == EQUIPMENT_ATTACK)
						finalText = QString::number(data->attackChange);
					else if (text.partyMemberText->equipmentTextType == EQUIPMENT_DEFENSE)
						finalText = QString::number(data->defenseChange);
					else if (text.partyMemberText->equipmentTextType == EQUIPMENT_WISDOM)
						finalText = QString::number(data->wisdomChange);
					else if (text.partyMemberText->equipmentTextType == EQUIPMENT_AGILITY)
						finalText = QString::number(data->agilityChange);
					delete itemRef;
				}
			}
			else if (text.partyMemberText->textType == PARTYMEMBER_EQUIPPEDSHIELD)
			{
				if ((*charRef)->getShieldID() == -1)
					finalText = "";
				else
				{
					ProjectData::DataReference<Item> *itemRef = new ProjectData::DataReference<Item>((*charRef)->getShieldID(), __FILE__, __LINE__);
					if ((*charRef)->isDuelWielding())
					{
						Item::WeaponItem *data = (*itemRef)->getWeaponData();
						if (text.partyMemberText->equipmentTextType == EQUIPMENT_NAME)
							finalText = (*itemRef)->getName();
						else if (text.partyMemberText->equipmentTextType == EQUIPMENT_DESCRIPTION)
							finalText = (*itemRef)->getDescription();
						else if (text.partyMemberText->equipmentTextType == EQUIPMENT_ATTACK)
							finalText = QString::number(data->attackChange);
						else if (text.partyMemberText->equipmentTextType == EQUIPMENT_DEFENSE)
							finalText = QString::number(data->defenseChange);
						else if (text.partyMemberText->equipmentTextType == EQUIPMENT_WISDOM)
							finalText = QString::number(data->wisdomChange);
						else if (text.partyMemberText->equipmentTextType == EQUIPMENT_AGILITY)
							finalText = QString::number(data->agilityChange);
					}
					else
					{
						Item::EquipmentItem *data = (*itemRef)->getEquipmentData();
						if (text.partyMemberText->equipmentTextType == EQUIPMENT_NAME)
							finalText = (*itemRef)->getName();
						else if (text.partyMemberText->equipmentTextType == EQUIPMENT_DESCRIPTION)
							finalText = (*itemRef)->getDescription();
						else if (text.partyMemberText->equipmentTextType == EQUIPMENT_ATTACK)
							finalText = QString::number(data->attackChange);
						else if (text.partyMemberText->equipmentTextType == EQUIPMENT_DEFENSE)
							finalText = QString::number(data->defenseChange);
						else if (text.partyMemberText->equipmentTextType == EQUIPMENT_WISDOM)
							finalText = QString::number(data->wisdomChange);
						else if (text.partyMemberText->equipmentTextType == EQUIPMENT_AGILITY)
							finalText = QString::number(data->agilityChange);
					}
					delete itemRef;
				}
			}
			else if (text.partyMemberText->textType == PARTYMEMBER_EQUIPPEDARMOR)
			{
				if ((*charRef)->getArmorID() == -1)
					finalText = "";
				else
				{
					ProjectData::DataReference<Item> *itemRef = new ProjectData::DataReference<Item>((*charRef)->getArmorID(), __FILE__, __LINE__);
					Item::EquipmentItem *data = (*itemRef)->getEquipmentData();
					if (text.partyMemberText->equipmentTextType == EQUIPMENT_NAME)
						finalText = (*itemRef)->getName();
					else if (text.partyMemberText->equipmentTextType == EQUIPMENT_DESCRIPTION)
						finalText = (*itemRef)->getDescription();
					else if (text.partyMemberText->equipmentTextType == EQUIPMENT_ATTACK)
						finalText = QString::number(data->attackChange);
					else if (text.partyMemberText->equipmentTextType == EQUIPMENT_DEFENSE)
						finalText = QString::number(data->defenseChange);
					else if (text.partyMemberText->equipmentTextType == EQUIPMENT_WISDOM)
						finalText = QString::number(data->wisdomChange);
					else if (text.partyMemberText->equipmentTextType == EQUIPMENT_AGILITY)
						finalText = QString::number(data->agilityChange);
					delete itemRef;
				}
			}
			else if (text.partyMemberText->textType == PARTYMEMBER_EQUIPPEDHELMET)
			{
				if ((*charRef)->getHelmetID() == -1)
					finalText = "";
				else
				{
					ProjectData::DataReference<Item> *itemRef = new ProjectData::DataReference<Item>((*charRef)->getHelmetID(), __FILE__, __LINE__);
					Item::EquipmentItem *data = (*itemRef)->getEquipmentData();
					if (text.partyMemberText->equipmentTextType == EQUIPMENT_NAME)
						finalText = (*itemRef)->getName();
					else if (text.partyMemberText->equipmentTextType == EQUIPMENT_DESCRIPTION)
						finalText = (*itemRef)->getDescription();
					else if (text.partyMemberText->equipmentTextType == EQUIPMENT_ATTACK)
						finalText = QString::number(data->attackChange);
					else if (text.partyMemberText->equipmentTextType == EQUIPMENT_DEFENSE)
						finalText = QString::number(data->defenseChange);
					else if (text.partyMemberText->equipmentTextType == EQUIPMENT_WISDOM)
						finalText = QString::number(data->wisdomChange);
					else if (text.partyMemberText->equipmentTextType == EQUIPMENT_AGILITY)
						finalText = QString::number(data->agilityChange);
					delete itemRef;
				}
			}
			else if (text.partyMemberText->textType == PARTYMEMBER_EQUIPPEDACCESSORY)
			{
				if ((*charRef)->getAccessoryID() == -1)
					finalText = "";
				else
				{
					ProjectData::DataReference<Item> *itemRef = new ProjectData::DataReference<Item>((*charRef)->getAccessoryID(), __FILE__, __LINE__);
					Item::EquipmentItem *data = (*itemRef)->getEquipmentData();
					if (text.partyMemberText->equipmentTextType == EQUIPMENT_NAME)
						finalText = (*itemRef)->getName();
					else if (text.partyMemberText->equipmentTextType == EQUIPMENT_DESCRIPTION)
						finalText = (*itemRef)->getDescription();
					else if (text.partyMemberText->equipmentTextType == EQUIPMENT_ATTACK)
						finalText = QString::number(data->attackChange);
					else if (text.partyMemberText->equipmentTextType == EQUIPMENT_DEFENSE)
						finalText = QString::number(data->defenseChange);
					else if (text.partyMemberText->equipmentTextType == EQUIPMENT_WISDOM)
						finalText = QString::number(data->wisdomChange);
					else if (text.partyMemberText->equipmentTextType == EQUIPMENT_AGILITY)
						finalText = QString::number(data->agilityChange);
					delete itemRef;
				}
			}
			delete charRef;
		}
		else
		{
			if (text.partyMemberText->textType == PARTYMEMBER_NAME)
				finalText = "NAME";
			else if (text.partyMemberText->textType == PARTYMEMBER_CLASS)
				finalText = "CLASS";
			else if (text.partyMemberText->textType == PARTYMEMBER_CONDITION)
				finalText = ProjectData::strings[ProjectData::STRING_STATSANDEQUIP_NORMALSTATUS];
			else if (text.partyMemberText->textType == PARTYMEMBER_LEVEL)
				finalText = "0";
			else if (text.partyMemberText->textType == PARTYMEMBER_HP)
				finalText = "0";
			else if (text.partyMemberText->textType == PARTYMEMBER_MAXHP)
				finalText = "0";
			else if (text.partyMemberText->textType == PARTYMEMBER_MP)
				finalText = "0";
			else if (text.partyMemberText->textType == PARTYMEMBER_MAXMP)
				finalText = "0";
			else if (text.partyMemberText->textType == PARTYMEMBER_EXPERIENCE)
				finalText = "-------";
			else if (text.partyMemberText->textType == PARTYMEMBER_EXPERIENCETONEXTLEVEL)
				finalText = "-------";
			else if (text.partyMemberText->textType == PARTYMEMBER_BASEATTACK)
				finalText = "0";
			else if (text.partyMemberText->textType == PARTYMEMBER_TOTALATTACK)
				finalText = "0";
			else if (text.partyMemberText->textType == PARTYMEMBER_BASEDEFENSE)
				finalText = "0";
			else if (text.partyMemberText->textType == PARTYMEMBER_TOTALDEFENSE)
				finalText = "0";
			else if (text.partyMemberText->textType == PARTYMEMBER_BASEWISDOM)
				finalText = "0";
			else if (text.partyMemberText->textType == PARTYMEMBER_TOTALWISDOM)
				finalText = "0";
			else if (text.partyMemberText->textType == PARTYMEMBER_BASEAGILITY)
				finalText = "0";
			else if (text.partyMemberText->textType == PARTYMEMBER_TOTALAGILITY)
				finalText = "0";
			else if (text.partyMemberText->textType == PARTYMEMBER_EQUIPPEDWEAPON)
			{
				if (text.partyMemberText->equipmentTextType == EQUIPMENT_NAME)
					finalText = "WPN NAME";
				else if (text.partyMemberText->equipmentTextType == EQUIPMENT_DESCRIPTION)
					finalText = "WPN DESC";
				else if (text.partyMemberText->equipmentTextType == EQUIPMENT_ATTACK)
					finalText = "WPN ATK";
				else if (text.partyMemberText->equipmentTextType == EQUIPMENT_DEFENSE)
					finalText = "WPN DEF";
				else if (text.partyMemberText->equipmentTextType == EQUIPMENT_WISDOM)
					finalText = "WPN WIS";
				else if (text.partyMemberText->equipmentTextType == EQUIPMENT_AGILITY)
					finalText = "WPN AGIL";
			}
			else if (text.partyMemberText->textType == PARTYMEMBER_EQUIPPEDSHIELD)
			{
				if (text.partyMemberText->equipmentTextType == EQUIPMENT_NAME)
					finalText = "SHLD NAME";
				else if (text.partyMemberText->equipmentTextType == EQUIPMENT_DESCRIPTION)
					finalText = "SHLD DESC";
				else if (text.partyMemberText->equipmentTextType == EQUIPMENT_ATTACK)
					finalText = "SHLD ATK";
				else if (text.partyMemberText->equipmentTextType == EQUIPMENT_DEFENSE)
					finalText = "SHLD DEF";
				else if (text.partyMemberText->equipmentTextType == EQUIPMENT_WISDOM)
					finalText = "SHLD WIS";
				else if (text.partyMemberText->equipmentTextType == EQUIPMENT_AGILITY)
					finalText = "SHLD AGIL";
			}
			else if (text.partyMemberText->textType == PARTYMEMBER_EQUIPPEDARMOR)
			{
				if (text.partyMemberText->equipmentTextType == EQUIPMENT_NAME)
					finalText = "ARM NAME";
				else if (text.partyMemberText->equipmentTextType == EQUIPMENT_DESCRIPTION)
					finalText = "ARM DESC";
				else if (text.partyMemberText->equipmentTextType == EQUIPMENT_ATTACK)
					finalText = "ARM ATK";
				else if (text.partyMemberText->equipmentTextType == EQUIPMENT_DEFENSE)
					finalText = "ARM DEF";
				else if (text.partyMemberText->equipmentTextType == EQUIPMENT_WISDOM)
					finalText = "ARM WIS";
				else if (text.partyMemberText->equipmentTextType == EQUIPMENT_AGILITY)
					finalText = "ARM AGIL";
			}
			else if (text.partyMemberText->textType == PARTYMEMBER_EQUIPPEDHELMET)
			{
				if (text.partyMemberText->equipmentTextType == EQUIPMENT_NAME)
					finalText = "HELM NAME";
				else if (text.partyMemberText->equipmentTextType == EQUIPMENT_DESCRIPTION)
					finalText = "HELM DESC";
				else if (text.partyMemberText->equipmentTextType == EQUIPMENT_ATTACK)
					finalText = "HELM ATK";
				else if (text.partyMemberText->equipmentTextType == EQUIPMENT_DEFENSE)
					finalText = "HELM DEF";
				else if (text.partyMemberText->equipmentTextType == EQUIPMENT_WISDOM)
					finalText = "HELM WIS";
				else if (text.partyMemberText->equipmentTextType == EQUIPMENT_AGILITY)
					finalText = "HELM AGIL";
			}
			else if (text.partyMemberText->textType == PARTYMEMBER_EQUIPPEDACCESSORY)
			{
				if (text.partyMemberText->equipmentTextType == EQUIPMENT_NAME)
					finalText = "ACSRY NAME";
				else if (text.partyMemberText->equipmentTextType == EQUIPMENT_DESCRIPTION)
					finalText = "ACSRY DESC";
				else if (text.partyMemberText->equipmentTextType == EQUIPMENT_ATTACK)
					finalText = "ACSRY ATK";
				else if (text.partyMemberText->equipmentTextType == EQUIPMENT_DEFENSE)
					finalText = "ACSRY DEF";
				else if (text.partyMemberText->equipmentTextType == EQUIPMENT_WISDOM)
					finalText = "ACSRY WIS";
				else if (text.partyMemberText->equipmentTextType == EQUIPMENT_AGILITY)
					finalText = "ACSRY AGIL";
			}
		}
	}
	else if (textType == TYPE_MONSTER)
	{
		ProjectData::DataReference<Monster> *monsterRef;
		int monsterID;
		if (text.monsterText->idIsReferencedInVariable)
			monsterID = 0;
		else
			monsterID = text.monsterText->monsterID;
		monsterRef = new ProjectData::DataReference<Monster>(monsterID, __FILE__, __LINE__);
		if (text.monsterText->textType == MONSTER_NAME)
			finalText = (*monsterRef)->getName();
		else if (text.monsterText->textType == MONSTER_MONEY)
			finalText = QString::number((*monsterRef)->getMoneyAquired());
		else if (text.monsterText->textType == MONSTER_HP)
			finalText = QString::number((*monsterRef)->getMaxHP());
		else if (text.monsterText->textType == MONSTER_MP)
			finalText = QString::number((*monsterRef)->getMaxMP());
		else if (text.monsterText->textType == MONSTER_ATTACK)
			finalText = QString::number((*monsterRef)->getAttack());
		else if (text.monsterText->textType == MONSTER_DEFENSE)
			finalText = QString::number((*monsterRef)->getDefense());
		else if (text.monsterText->textType == MONSTER_WISDOM)
			finalText = QString::number((*monsterRef)->getWisdom());
		else if (text.monsterText->textType == MONSTER_AGILITY)
			finalText = QString::number((*monsterRef)->getAgility());
		else if (text.monsterText->textType == MONSTER_STRONGAGAINST)
			finalText = (*monsterRef)->getStrongAgainst();
		else if (text.monsterText->textType == MONSTER_WEAKAGAINST)
			finalText = (*monsterRef)->getWeakAgainst();
		else if (text.monsterText->textType == MONSTER_EXPERIENCE)
			finalText = QString::number((*monsterRef)->getExperienceGain());
		delete monsterRef;
	}
	else if (textType == TYPE_ITEM)
	{
		ProjectData::DataReference<Item> *itemRef;
		int itemID;
		if (text.itemText->idIsReferencedInVariable)
			itemID = 0;
		else
			itemID = text.itemText->itemID;
		itemRef = new ProjectData::DataReference<Item>(itemID, __FILE__, __LINE__);
		if (text.itemText->textType == ITEM_NAME)
			finalText = (*itemRef)->getName();
		else if (text.itemText->textType == ITEM_DESCRIPTION)
			finalText = (*itemRef)->getDescription();
		else if (text.itemText->textType == ITEM_PRICE)
			finalText = QString::number((*itemRef)->getPrice());
		delete itemRef;
	}
	else if (textType == TYPE_CHARACTER)
	{
		ProjectData::DataReference<Character> *charRef;
		int characterID;
		if (text.characterText->idIsReferencedInVariable)
			characterID = 0;
		else
			characterID = text.characterText->characterID;
		charRef = new ProjectData::DataReference<Character>(characterID, __FILE__, __LINE__);
		if (text.characterText->textType == CHARACTER_NAME)
			finalText = (*charRef)->getName();
		else if (text.characterText->textType == CHARACTER_CLASS)
		{
			int classID = (*charRef)->getClassID();
			if (classID == -1)
				finalText = "None";
			else
				finalText = ProjectData::classNames[classID];
		}
		else if (text.characterText->textType == CHARACTER_CONDITION)
			finalText = ProjectData::strings[ProjectData::STRING_STATSANDEQUIP_NORMALSTATUS];
		else if (text.characterText->textType == CHARACTER_LEVEL)
			finalText = (*charRef)->getMinLevel();
		else if (text.characterText->textType == CHARACTER_HP)
			finalText = QString::number((*charRef)->getMaxHP((*charRef)->getMinLevel()));
		else if (text.characterText->textType == CHARACTER_MAXHP)
			finalText = QString::number((*charRef)->getMaxHP((*charRef)->getMinLevel()));
		else if (text.characterText->textType == CHARACTER_MP)
			finalText = QString::number((*charRef)->getMaxMP((*charRef)->getMinLevel()));
		else if (text.characterText->textType == CHARACTER_MAXMP)
			finalText = QString::number((*charRef)->getMaxMP((*charRef)->getMinLevel()));
		else if (text.characterText->textType == CHARACTER_EXPERIENCE)
		{
			if ((*charRef)->getMinLevel() == 99)
				finalText = "-------";
			else
			{
				int totalExperience = (*charRef)->getTotalExperience((*charRef)->getMinLevel());
				if (totalExperience <= 9999999)
					finalText = QString::number(totalExperience);
				else
					finalText = "-------";
			}
		}
		else if (text.characterText->textType == CHARACTER_EXPERIENCETONEXTLEVEL)
		{
			if ((*charRef)->getMinLevel() == 99)
				finalText = "-------";
			else
			{
				int totalExperience = (*charRef)->getTotalExperience((*charRef)->getMinLevel() + 1);
				if (totalExperience <= 9999999)
					finalText = QString::number(totalExperience);
				else
					finalText = "-------";
			}
		}
		else if (text.characterText->textType == CHARACTER_BASEATTACK)
			finalText = QString::number((*charRef)->getAttack((*charRef)->getMinLevel()));
		else if (text.characterText->textType == CHARACTER_TOTALATTACK)
		{
			ProjectData::DataReference<Item> *itemRef;
			int level = (*charRef)->getMinLevel();
			int baseAttack = (*charRef)->getAttack(level);
			int weaponID, weaponAttack;
			int shieldID, shieldAttack;
			int armorID, armorAttack;
			int helmetID, helmetAttack;
			int accessoryID, accessoryAttack;
			weaponID = (*charRef)->getWeaponID();
			shieldID = (*charRef)->getShieldID();
			armorID = (*charRef)->getArmorID();
			helmetID = (*charRef)->getHelmetID();
			accessoryID = (*charRef)->getAccessoryID();
			if (weaponID == -1)
				weaponAttack = 0;
			else
			{
				itemRef = new ProjectData::DataReference<Item>(weaponID, __FILE__, __LINE__);
				weaponAttack = (*itemRef)->getWeaponData()->attackChange;
				delete itemRef;
			}
			if (shieldID == -1)
				shieldAttack = 0;
			else
			{
				itemRef = new ProjectData::DataReference<Item>(shieldID, __FILE__, __LINE__);
				if ((*charRef)->isDuelWielding())
					shieldAttack = (*itemRef)->getWeaponData()->attackChange;
				else
					shieldAttack = (*itemRef)->getEquipmentData()->attackChange;
				delete itemRef;
			}
			if (armorID == -1)
				armorAttack = -1;
			else
			{
				itemRef = new ProjectData::DataReference<Item>(armorID, __FILE__, __LINE__);
				armorAttack = (*itemRef)->getEquipmentData()->attackChange;
				delete itemRef;
			}
			if (helmetID == -1)
				helmetAttack = 0;
			else
			{
				itemRef = new ProjectData::DataReference<Item>(helmetID, __FILE__, __LINE__);
				helmetAttack = (*itemRef)->getEquipmentData()->attackChange;
				delete itemRef;
			}
			if (accessoryID == -1)
				accessoryAttack = 0;
			else
			{
				itemRef = new ProjectData::DataReference<Item>(accessoryID, __FILE__, __LINE__);
				accessoryAttack = (*itemRef)->getEquipmentData()->attackChange;
				delete itemRef;
			}
			finalText = QString::number(baseAttack + weaponAttack + shieldAttack + armorAttack + helmetAttack + accessoryAttack);
		}
		else if (text.characterText->textType == CHARACTER_BASEDEFENSE)
			finalText = QString::number((*charRef)->getDefense((*charRef)->getMinLevel()));
		else if (text.characterText->textType == CHARACTER_TOTALDEFENSE)
		{
			ProjectData::DataReference<Item> *itemRef;
			int level = (*charRef)->getMinLevel();
			int baseDefense = (*charRef)->getDefense(level);
			int weaponID, weaponDefense;
			int shieldID, shieldDefense;
			int armorID, armorDefense;
			int helmetID, helmetDefense;
			int accessoryID, accessoryDefense;
			weaponID = (*charRef)->getWeaponID();
			shieldID = (*charRef)->getShieldID();
			armorID = (*charRef)->getArmorID();
			helmetID = (*charRef)->getHelmetID();
			accessoryID = (*charRef)->getAccessoryID();
			if (weaponID == -1)
				weaponDefense = 0;
			else
			{
				itemRef = new ProjectData::DataReference<Item>(weaponID, __FILE__, __LINE__);
				weaponDefense = (*itemRef)->getWeaponData()->defenseChange;
				delete itemRef;
			}
			if (shieldID == -1)
				shieldDefense = 0;
			else
			{
				itemRef = new ProjectData::DataReference<Item>(shieldID, __FILE__, __LINE__);
				if ((*charRef)->isDuelWielding())
					shieldDefense = (*itemRef)->getWeaponData()->defenseChange;
				else
					shieldDefense = (*itemRef)->getEquipmentData()->defenseChange;
				delete itemRef;
			}
			if (armorID == -1)
				armorDefense = 0;
			else
			{
				itemRef = new ProjectData::DataReference<Item>(armorID, __FILE__, __LINE__);
				armorDefense = (*itemRef)->getEquipmentData()->defenseChange;
				delete itemRef;
			}
			if (helmetID == -1)
				helmetDefense = 0;
			else
			{
				itemRef = new ProjectData::DataReference<Item>(helmetID, __FILE__, __LINE__);
				helmetDefense = (*itemRef)->getEquipmentData()->defenseChange;
				delete itemRef;
			}
			if (accessoryID == -1)
				accessoryDefense = 0;
			else
			{
				itemRef = new ProjectData::DataReference<Item>(accessoryID, __FILE__, __LINE__);
				accessoryDefense = (*itemRef)->getEquipmentData()->defenseChange;
				delete itemRef;
			}
			finalText = QString::number(baseDefense + weaponDefense + shieldDefense + armorDefense + helmetDefense + accessoryDefense);
		}
		else if (text.characterText->textType == CHARACTER_BASEWISDOM)
			finalText = QString::number((*charRef)->getWisdom((*charRef)->getMinLevel()));
		else if (text.characterText->textType == CHARACTER_TOTALWISDOM)
		{
			ProjectData::DataReference<Item> *itemRef;
			int level = (*charRef)->getMinLevel();
			int baseWisdom = (*charRef)->getWisdom(level);
			int weaponID, weaponWisdom;
			int shieldID, shieldWisdom;
			int armorID, armorWisdom;
			int helmetID, helmetWisdom;
			int accessoryID, accessoryWisdom;
			weaponID = (*charRef)->getWeaponID();
			shieldID = (*charRef)->getShieldID();
			armorID = (*charRef)->getArmorID();
			helmetID = (*charRef)->getHelmetID();
			accessoryID = (*charRef)->getAccessoryID();
			if (weaponID == -1)
				weaponWisdom = 0;
			else
			{
				itemRef = new ProjectData::DataReference<Item>(weaponID, __FILE__, __LINE__);
				weaponWisdom = (*itemRef)->getWeaponData()->wisdomChange;
				delete itemRef;
			}
			if (shieldID == -1)
				shieldWisdom = 0;
			else
			{
				itemRef = new ProjectData::DataReference<Item>(shieldID, __FILE__, __LINE__);
				if ((*charRef)->isDuelWielding())
					shieldWisdom = (*itemRef)->getWeaponData()->wisdomChange;
				else
					shieldWisdom = (*itemRef)->getEquipmentData()->wisdomChange;
				delete itemRef;
			}
			if (armorID == -1)
				armorWisdom = 0;
			else
			{
				itemRef = new ProjectData::DataReference<Item>(armorID, __FILE__, __LINE__);
				armorWisdom = (*itemRef)->getEquipmentData()->wisdomChange;
				delete itemRef;
			}
			if (helmetID == -1)
				helmetWisdom = 0;
			else
			{
				itemRef = new ProjectData::DataReference<Item>(helmetID, __FILE__, __LINE__);
				helmetWisdom = (*itemRef)->getEquipmentData()->wisdomChange;
				delete itemRef;
			}
			if (accessoryID == -1)
				accessoryWisdom = 0;
			else
			{
				itemRef = new ProjectData::DataReference<Item>(accessoryID, __FILE__, __LINE__);
				accessoryWisdom = (*itemRef)->getEquipmentData()->wisdomChange;
				delete itemRef;
			}
			finalText = QString::number(baseWisdom + weaponWisdom + shieldWisdom + armorWisdom + helmetWisdom + accessoryWisdom);
		}
		else if (text.characterText->textType == CHARACTER_BASEAGILITY)
			finalText = QString::number((*charRef)->getAgility((*charRef)->getMinLevel()));
		else if (text.characterText->textType == CHARACTER_TOTALAGILITY)
		{
			ProjectData::DataReference<Item> *itemRef;
			int level = (*charRef)->getMinLevel();
			int baseAgility = (*charRef)->getAgility(level);
			int weaponID, weaponAgility;
			int shieldID, shieldAgility;
			int armorID, armorAgility;
			int helmetID, helmetAgility;
			int accessoryID, accessoryAgility;
			weaponID = (*charRef)->getWeaponID();
			shieldID = (*charRef)->getShieldID();
			armorID = (*charRef)->getArmorID();
			helmetID = (*charRef)->getHelmetID();
			accessoryID = (*charRef)->getAccessoryID();
			if (weaponID == -1)
				weaponAgility = 0;
			else
			{
				itemRef = new ProjectData::DataReference<Item>(weaponID, __FILE__, __LINE__);
				weaponAgility = (*itemRef)->getWeaponData()->agilityChange;
				delete itemRef;
			}
			if (shieldID == -1)
				shieldAgility = 0;
			else
			{
				itemRef = new ProjectData::DataReference<Item>(shieldID, __FILE__, __LINE__);
				if ((*charRef)->isDuelWielding())
					shieldAgility = (*itemRef)->getWeaponData()->agilityChange;
				else
					shieldAgility = (*itemRef)->getEquipmentData()->agilityChange;
				delete itemRef;
			}
			if (armorID == -1)
				armorAgility = 0;
			else
			{
				itemRef = new ProjectData::DataReference<Item>(armorID, __FILE__, __LINE__);
				armorAgility = (*itemRef)->getEquipmentData()->agilityChange;
				delete itemRef;
			}
			if (helmetID == -1)
				helmetAgility = 0;
			else
			{
				itemRef = new ProjectData::DataReference<Item>(helmetID, __FILE__, __LINE__);
				helmetAgility = (*itemRef)->getEquipmentData()->agilityChange;
				delete itemRef;
			}
			if (accessoryID == -1)
				accessoryAgility = 0;
			else
			{
				itemRef = new ProjectData::DataReference<Item>(accessoryID, __FILE__, __LINE__);
				accessoryAgility = (*itemRef)->getEquipmentData()->agilityChange;
				delete itemRef;
			}
			finalText = QString::number(baseAgility + weaponAgility + shieldAgility + armorAgility + helmetAgility + accessoryAgility);
		}
		else if (text.characterText->textType == CHARACTER_EQUIPPEDWEAPON)
		{
			ProjectData::DataReference<Item> *itemRef;
			Item::WeaponItem *data;
			int itemID = (*charRef)->getWeaponID();
			if (itemID >= 0 && itemID < ProjectData::itemNames.size())
			{
				itemRef = new ProjectData::DataReference<Item>(itemID, __FILE__, __LINE__);
				data = (*itemRef)->getWeaponData();
				if (text.characterText->equipmentTextType == EQUIPMENT_NAME)
					finalText = (*itemRef)->getName();
				else if (text.characterText->equipmentTextType == EQUIPMENT_DESCRIPTION)
					finalText = (*itemRef)->getDescription();
				else if (text.characterText->equipmentTextType == EQUIPMENT_ATTACK)
					finalText = QString::number(data->attackChange);
				else if (text.characterText->equipmentTextType == EQUIPMENT_DEFENSE)
					finalText = QString::number(data->defenseChange);
				else if (text.characterText->equipmentTextType == EQUIPMENT_WISDOM)
					finalText = QString::number(data->wisdomChange);
				else if (text.characterText->equipmentTextType == EQUIPMENT_AGILITY)
					finalText = QString::number(data->agilityChange);
				delete itemRef;
			}
			else
				finalText = "";
		}
		else if (text.characterText->textType == CHARACTER_EQUIPPEDSHIELD)
		{
			ProjectData::DataReference<Item> *itemRef;
			int itemID = (*charRef)->getShieldID();
			if (itemID >= 0 && itemID < ProjectData::itemNames.size())
			{
				itemRef = new ProjectData::DataReference<Item>(itemID, __FILE__, __LINE__);
				if ((*charRef)->isDuelWielding())
				{
					Item::WeaponItem *data = (*itemRef)->getWeaponData();
					if (text.characterText->equipmentTextType == EQUIPMENT_NAME)
						finalText = (*itemRef)->getName();
					else if (text.characterText->equipmentTextType == EQUIPMENT_DESCRIPTION)
						finalText = (*itemRef)->getDescription();
					else if (text.characterText->equipmentTextType == EQUIPMENT_ATTACK)
						finalText = QString::number(data->attackChange);
					else if (text.characterText->equipmentTextType == EQUIPMENT_DEFENSE)
						finalText = QString::number(data->defenseChange);
					else if (text.characterText->equipmentTextType == EQUIPMENT_WISDOM)
						finalText = QString::number(data->wisdomChange);
					else if (text.characterText->equipmentTextType == EQUIPMENT_AGILITY)
						finalText = QString::number(data->agilityChange);
				}
				else
				{
					Item::EquipmentItem *data = (*itemRef)->getEquipmentData();
					if (text.characterText->equipmentTextType == EQUIPMENT_NAME)
						finalText = (*itemRef)->getName();
					else if (text.characterText->equipmentTextType == EQUIPMENT_DESCRIPTION)
						finalText = (*itemRef)->getDescription();
					else if (text.characterText->equipmentTextType == EQUIPMENT_ATTACK)
						finalText = QString::number(data->attackChange);
					else if (text.characterText->equipmentTextType == EQUIPMENT_DEFENSE)
						finalText = QString::number(data->defenseChange);
					else if (text.characterText->equipmentTextType == EQUIPMENT_WISDOM)
						finalText = QString::number(data->wisdomChange);
					else if (text.characterText->equipmentTextType == EQUIPMENT_AGILITY)
						finalText = QString::number(data->agilityChange);
				}
				delete itemRef;
			}
			else
				finalText = "";
		}
		else if (text.characterText->textType == CHARACTER_EQUIPPEDARMOR)
		{
			ProjectData::DataReference<Item> *itemRef;
			Item::EquipmentItem *data;
			int itemID = (*charRef)->getArmorID();
			if (itemID >= 0 && itemID < ProjectData::itemNames.size())
			{
				itemRef = new ProjectData::DataReference<Item>(itemID, __FILE__, __LINE__);
				data = (*itemRef)->getEquipmentData();
				if (text.characterText->equipmentTextType == EQUIPMENT_NAME)
					finalText = (*itemRef)->getName();
				else if (text.characterText->equipmentTextType == EQUIPMENT_DESCRIPTION)
					finalText = (*itemRef)->getDescription();
				else if (text.characterText->equipmentTextType == EQUIPMENT_ATTACK)
					finalText = QString::number(data->attackChange);
				else if (text.characterText->equipmentTextType == EQUIPMENT_DEFENSE)
					finalText = QString::number(data->defenseChange);
				else if (text.characterText->equipmentTextType == EQUIPMENT_WISDOM)
					finalText = QString::number(data->wisdomChange);
				else if (text.characterText->equipmentTextType == EQUIPMENT_AGILITY)
					finalText = QString::number(data->agilityChange);
				delete itemRef;
			}
			else
				finalText = "";
		}
		else if (text.characterText->textType == CHARACTER_EQUIPPEDHELMET)
		{
			ProjectData::DataReference<Item> *itemRef;
			Item::EquipmentItem *data;
			int itemID = (*charRef)->getHelmetID();
			if (itemID >= 0 && itemID < ProjectData::itemNames.size())
			{
				itemRef = new ProjectData::DataReference<Item>(itemID, __FILE__, __LINE__);
				data = (*itemRef)->getEquipmentData();
				if (text.characterText->equipmentTextType == EQUIPMENT_NAME)
					finalText = (*itemRef)->getName();
				else if (text.characterText->equipmentTextType == EQUIPMENT_DESCRIPTION)
					finalText = (*itemRef)->getDescription();
				else if (text.characterText->equipmentTextType == EQUIPMENT_ATTACK)
					finalText = QString::number(data->attackChange);
				else if (text.characterText->equipmentTextType == EQUIPMENT_DEFENSE)
					finalText = QString::number(data->defenseChange);
				else if (text.characterText->equipmentTextType == EQUIPMENT_WISDOM)
					finalText = QString::number(data->wisdomChange);
				else if (text.characterText->equipmentTextType == EQUIPMENT_AGILITY)
					finalText = QString::number(data->agilityChange);
				delete itemRef;
			}
			else
				finalText = "";
		}
		else if (text.characterText->textType == CHARACTER_EQUIPPEDACCESSORY)
		{
			ProjectData::DataReference<Item> *itemRef;
			Item::EquipmentItem *data;
			int itemID = (*charRef)->getAccessoryID();
			if (itemID >= 0 && itemID < ProjectData::itemNames.size())
			{
				itemRef = new ProjectData::DataReference<Item>(itemID, __FILE__, __LINE__);
				data = (*itemRef)->getEquipmentData();
				if (text.characterText->equipmentTextType == EQUIPMENT_NAME)
					finalText = (*itemRef)->getName();
				else if (text.characterText->equipmentTextType == EQUIPMENT_DESCRIPTION)
					finalText = (*itemRef)->getDescription();
				else if (text.characterText->equipmentTextType == EQUIPMENT_ATTACK)
					finalText = QString::number(data->attackChange);
				else if (text.characterText->equipmentTextType == EQUIPMENT_DEFENSE)
					finalText = QString::number(data->defenseChange);
				else if (text.characterText->equipmentTextType == EQUIPMENT_WISDOM)
					finalText = QString::number(data->wisdomChange);
				else if (text.characterText->equipmentTextType == EQUIPMENT_AGILITY)
					finalText = QString::number(data->agilityChange);
				delete itemRef;
			}
			else
				finalText = "";
		}
		delete charRef;
	}
	else if (textType == TYPE_VARIABLE)
		finalText = "0";
	else if (textType == TYPE_SWITCH)
		finalText = text.switchText->offText;
	else if (textType == TYPE_STRING)
	{
		if (text.stringText->systemString)
			finalText = ProjectData::strings[text.stringText->stringID];
		else
			finalText = "";
	}
	if (textWidth == 0)
	{
		if (!finalText.isEmpty())
			renderedPixmap = QPixmap::fromImage((*fontRef)->getRenderedText(finalText, enabledTopColor, enabledBottomColor));
		else
			renderedPixmap = QPixmap(0, 0);
	}
	else
	{
		if (!finalText.isEmpty())
		{
			pixmap = QPixmap::fromImage((*fontRef)->getRenderedText(finalText, enabledTopColor, enabledBottomColor));
			renderedPixmap = QPixmap(textWidth, (*fontRef)->getCharacterHeight() + abs((*fontRef)->getDropShadowYOffset()));
			renderedPixmap.fill(QColor(0, 0, 0, 0));
			painter.begin(&renderedPixmap);
			if (alignment == ALIGN_LEFT)
				painter.drawPixmap(0, 0, pixmap);
			else if (alignment == ALIGN_CENTER)
				painter.drawPixmap((renderedPixmap.width() - pixmap.width()) / 2, 0, pixmap);
			else
				painter.drawPixmap(renderedPixmap.width() - pixmap.width(), 0, pixmap);
			painter.end();
		}
		else
		{
			renderedPixmap = QPixmap(textWidth, (*fontRef)->getCharacterHeight() + abs((*fontRef)->getDropShadowYOffset()));
			renderedPixmap.fill(QColor(0, 0, 0, 0));
		}
	}
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
	width = renderedPixmap.width();
	height = renderedPixmap.height();
	delete fontRef;
}

void MenuText::loadXMLData(XMLNode ObjectNode)
{
	QString temp_string;
	int red, green, blue;
	x = QString(ObjectNode.getAttribute("x")).toInt();
	y = QString(ObjectNode.getAttribute("y")).toInt();
	temp_string = ObjectNode.getAttribute("textType");
	if (temp_string == "plain text")
		textType = TYPE_PLAINTEXT;
	else if (temp_string == "party member")
		textType = TYPE_PARTYMEMBER;
	else if (temp_string == "monster")
		textType = TYPE_MONSTER;
	else if (temp_string == "item")
		textType = TYPE_ITEM;
	else if (temp_string == "character")
		textType = TYPE_CHARACTER;
	else if (temp_string == "variable")
		textType = TYPE_VARIABLE;
	else if (temp_string == "switch")
		textType = TYPE_SWITCH;
	else if (temp_string == "string")
		textType = TYPE_STRING;
	textWidth = QString(ObjectNode.getAttribute("textWidth")).toInt();
	temp_string = ObjectNode.getAttribute("alignment");
	if (temp_string == "left")
		alignment = ALIGN_LEFT;
	else if (temp_string == "center")
		alignment = ALIGN_CENTER;
	else if (temp_string == "right")
		alignment = ALIGN_RIGHT;
	fontID = QString(ObjectNode.getAttribute("fontID")).toInt();
	saveSlotID = QString(ObjectNode.getAttribute("saveSlotID")).toInt();
	temp_string = ObjectNode.getAttribute("enabledColor");
	red = temp_string.mid(1, 2).toInt(0, 16);
	green = temp_string.mid(3, 2).toInt(0, 16);
	blue = temp_string.mid(5, 2).toInt(0, 16);
	enabledTopColor = QColor(red, green, blue);
	red = temp_string.mid(9, 2).toInt(0, 16);
	green = temp_string.mid(11, 2).toInt(0, 16);
	blue = temp_string.mid(13, 2).toInt(0, 16);
	enabledBottomColor = QColor(red, green, blue);
	temp_string = ObjectNode.getAttribute("disabledColor");
	red = temp_string.mid(1, 2).toInt(0, 16);
	green = temp_string.mid(3, 2).toInt(0, 16);
	blue = temp_string.mid(5, 2).toInt(0, 16);
	disabledTopColor = QColor(red, green, blue);
	red = temp_string.mid(9, 2).toInt(0, 16);
	green = temp_string.mid(11, 2).toInt(0, 16);
	blue = temp_string.mid(13, 2).toInt(0, 16);
	disabledBottomColor = QColor(red, green, blue);
	if (textType == TYPE_PLAINTEXT)
		text.plainText = new QString(ObjectNode.getAttribute("plainText"));
	else if (textType == TYPE_PARTYMEMBER)
	{
		text.partyMemberText = new PartyMemberText;
		text.partyMemberText->memberID = QString(ObjectNode.getAttribute("partyMemberID")).toInt();
		temp_string = ObjectNode.getAttribute("partyMemberTextType");
		if (temp_string == "name")
			text.partyMemberText->textType = PARTYMEMBER_NAME;
		else if (temp_string == "class")
			text.partyMemberText->textType = PARTYMEMBER_CLASS;
		else if (temp_string == "condition")
			text.partyMemberText->textType = PARTYMEMBER_CONDITION;
		else if (temp_string == "level")
			text.partyMemberText->textType = PARTYMEMBER_LEVEL;
		else if (temp_string == "hp")
			text.partyMemberText->textType = PARTYMEMBER_HP;
		else if (temp_string == "max hp")
			text.partyMemberText->textType = PARTYMEMBER_MAXHP;
		else if (temp_string == "mp")
			text.partyMemberText->textType = PARTYMEMBER_MP;
		else if (temp_string == "max mp")
			text.partyMemberText->textType = PARTYMEMBER_MAXMP;
		else if (temp_string == "experience")
			text.partyMemberText->textType = PARTYMEMBER_EXPERIENCE;
		else if (temp_string == "experience to next level")
			text.partyMemberText->textType = PARTYMEMBER_EXPERIENCETONEXTLEVEL;
		else if (temp_string == "base attack")
			text.partyMemberText->textType = PARTYMEMBER_BASEATTACK;
		else if (temp_string == "total attack")
			text.partyMemberText->textType = PARTYMEMBER_TOTALATTACK;
		else if (temp_string == "base defense")
			text.partyMemberText->textType = PARTYMEMBER_BASEDEFENSE;
		else if (temp_string == "total defense")
			text.partyMemberText->textType = PARTYMEMBER_TOTALDEFENSE;
		else if (temp_string == "base wisdom")
			text.partyMemberText->textType = PARTYMEMBER_BASEWISDOM;
		else if (temp_string == "total wisdom")
			text.partyMemberText->textType = PARTYMEMBER_TOTALWISDOM;
		else if (temp_string == "base agility")
			text.partyMemberText->textType = PARTYMEMBER_BASEAGILITY;
		else if (temp_string == "total agility")
			text.partyMemberText->textType = PARTYMEMBER_TOTALAGILITY;
		else if (temp_string == "equipped weapon")
			text.partyMemberText->textType = PARTYMEMBER_EQUIPPEDWEAPON;
		else if (temp_string == "equipped shield")
			text.partyMemberText->textType = PARTYMEMBER_EQUIPPEDSHIELD;
		else if (temp_string == "equipped armor")
			text.partyMemberText->textType = PARTYMEMBER_EQUIPPEDARMOR;
		else if (temp_string == "equipped helmet")
			text.partyMemberText->textType = PARTYMEMBER_EQUIPPEDHELMET;
		else if (temp_string == "equipped accessory")
			text.partyMemberText->textType = PARTYMEMBER_EQUIPPEDACCESSORY;
		if (text.partyMemberText->textType >= PARTYMEMBER_EQUIPPEDWEAPON && text.partyMemberText->textType <= PARTYMEMBER_EQUIPPEDACCESSORY)
		{
			temp_string = ObjectNode.getAttribute("partyMemberEquipmentTextType");
			if (temp_string == "name")
				text.partyMemberText->equipmentTextType = EQUIPMENT_NAME;
			else if (temp_string == "description")
				text.partyMemberText->equipmentTextType = EQUIPMENT_DESCRIPTION;
			else if (temp_string == "attack")
				text.partyMemberText->equipmentTextType = EQUIPMENT_ATTACK;
			else if (temp_string == "defense")
				text.partyMemberText->equipmentTextType = EQUIPMENT_DEFENSE;
			else if (temp_string == "wisdom")
				text.partyMemberText->equipmentTextType = EQUIPMENT_WISDOM;
			else if (temp_string == "agility")
				text.partyMemberText->equipmentTextType = EQUIPMENT_AGILITY;
		}
	}
	else if (textType == TYPE_MONSTER)
	{
		text.monsterText = new MonsterText;
		text.monsterText->monsterID = QString(ObjectNode.getAttribute("monsterID")).toInt();
		temp_string = ObjectNode.getAttribute("monsterTextType");
		if (temp_string == "name")
			text.monsterText->textType = MONSTER_NAME;
		else if (temp_string == "money")
			text.monsterText->textType = MONSTER_MONEY;
		else if (temp_string == "hp")
			text.monsterText->textType = MONSTER_HP;
		else if (temp_string == "mp")
			text.monsterText->textType = MONSTER_MP;
		else if (temp_string == "attack")
			text.monsterText->textType = MONSTER_ATTACK;
		else if (temp_string == "defense")
			text.monsterText->textType = MONSTER_DEFENSE;
		else if (temp_string == "wisdom")
			text.monsterText->textType = MONSTER_WISDOM;
		else if (temp_string == "agility")
			text.monsterText->textType = MONSTER_AGILITY;
		else if (temp_string == "strong against")
			text.monsterText->textType = MONSTER_STRONGAGAINST;
		else if (temp_string == "weak against")
			text.monsterText->textType = MONSTER_WEAKAGAINST;
		else if (temp_string == "experience")
			text.monsterText->textType = MONSTER_EXPERIENCE;
		text.monsterText->idIsReferencedInVariable = (QString(ObjectNode.getAttribute("monsterIDIsReferencedInVariable")) == "true") ? true:false;
	}
	else if (textType == TYPE_ITEM)
	{
		text.itemText = new ItemText;
		text.itemText->itemID = QString(ObjectNode.getAttribute("itemID")).toInt();
		temp_string = ObjectNode.getAttribute("itemTextType");
		if (temp_string == "name")
			text.itemText->textType = ITEM_NAME;
		else if (temp_string == "description")
			text.itemText->textType = ITEM_DESCRIPTION;
		else if (temp_string == "price")
			text.itemText->textType = ITEM_PRICE;
		text.itemText->idIsReferencedInVariable = (QString(ObjectNode.getAttribute("itemIDIsReferencedInVariable")) == "true") ? true:false;
	}
	else if (textType == TYPE_CHARACTER)
	{
		text.characterText = new CharacterText;
		text.characterText->characterID = QString(ObjectNode.getAttribute("characterID")).toInt();
		temp_string = ObjectNode.getAttribute("characterTextType");
		if (temp_string == "name")
			text.characterText->textType = CHARACTER_NAME;
		else if (temp_string == "class")
			text.characterText->textType = CHARACTER_CLASS;
		else if (temp_string == "condition")
			text.characterText->textType = CHARACTER_CONDITION;
		else if (temp_string == "level")
			text.characterText->textType = CHARACTER_LEVEL;
		else if (temp_string == "hp")
			text.characterText->textType = CHARACTER_HP;
		else if (temp_string == "max hp")
			text.characterText->textType = CHARACTER_MAXHP;
		else if (temp_string == "mp")
			text.characterText->textType = CHARACTER_MP;
		else if (temp_string == "max mp")
			text.characterText->textType = CHARACTER_MAXMP;
		else if (temp_string == "experience")
			text.characterText->textType = CHARACTER_EXPERIENCE;
		else if (temp_string == "experience to next level")
			text.characterText->textType = CHARACTER_EXPERIENCETONEXTLEVEL;
		else if (temp_string == "base attack")
			text.characterText->textType = CHARACTER_BASEATTACK;
		else if (temp_string == "total attack")
			text.characterText->textType = CHARACTER_TOTALATTACK;
		else if (temp_string == "base defense")
			text.characterText->textType = CHARACTER_BASEDEFENSE;
		else if (temp_string == "total defense")
			text.characterText->textType = CHARACTER_TOTALDEFENSE;
		else if (temp_string == "base wisdom")
			text.characterText->textType = CHARACTER_BASEWISDOM;
		else if (temp_string == "total wisdom")
			text.characterText->textType = CHARACTER_TOTALWISDOM;
		else if (temp_string == "base agility")
			text.characterText->textType = CHARACTER_BASEAGILITY;
		else if (temp_string == "total agility")
			text.characterText->textType = CHARACTER_TOTALAGILITY;
		else if (temp_string == "equipped weapon")
			text.characterText->textType = CHARACTER_EQUIPPEDWEAPON;
		else if (temp_string == "equipped shield")
			text.characterText->textType = CHARACTER_EQUIPPEDSHIELD;
		else if (temp_string == "equipped armor")
			text.characterText->textType = CHARACTER_EQUIPPEDARMOR;
		else if (temp_string == "equipped helmet")
			text.characterText->textType = CHARACTER_EQUIPPEDHELMET;
		else if (temp_string == "equipped accessory")
			text.characterText->textType = CHARACTER_EQUIPPEDACCESSORY;
		if (text.characterText->textType >= CHARACTER_EQUIPPEDWEAPON && text.characterText->textType <= CHARACTER_EQUIPPEDACCESSORY)
		{
			temp_string = ObjectNode.getAttribute("characterEquipmentTextType");
			if (temp_string == "name")
				text.characterText->equipmentTextType = EQUIPMENT_NAME;
			else if (temp_string == "description")
				text.characterText->equipmentTextType = EQUIPMENT_DESCRIPTION;
			else if (temp_string == "attack")
				text.characterText->equipmentTextType = EQUIPMENT_ATTACK;
			else if (temp_string == "defense")
				text.characterText->equipmentTextType = EQUIPMENT_DEFENSE;
			else if (temp_string == "wisdom")
				text.characterText->equipmentTextType = EQUIPMENT_WISDOM;
			else if (temp_string == "agility")
				text.characterText->equipmentTextType = EQUIPMENT_AGILITY;
		}
		text.characterText->idIsReferencedInVariable = (QString(ObjectNode.getAttribute("characterIDIsReferencedInVariable")) == "true") ? true:false;
	}
	else if (textType == TYPE_VARIABLE)
	{
		text.variableText = new VariableText;
		text.variableText->variableID = QString(ObjectNode.getAttribute("variableID")).toInt();
		text.variableText->alwaysShowPlusSign = (QString(ObjectNode.getAttribute("variableAlwaysShowsPlusSign")) == "true") ? true:false;
	}
	else if (textType == TYPE_SWITCH)
	{
		text.switchText = new SwitchText;
		text.switchText->switchID = QString(ObjectNode.getAttribute("switchID")).toInt();
		text.switchText->onText = ObjectNode.getAttribute("switchOnText");
		text.switchText->offText = ObjectNode.getAttribute("switchOffText");
	}
	else if (textType == TYPE_STRING)
	{
		text.stringText = new StringText;
		text.stringText->stringID = QString(ObjectNode.getAttribute("stringID")).toInt();
		text.stringText->systemString = (QString(ObjectNode.getAttribute("systemString")) == "true") ? true:false;
	}
}

void MenuText::loadStorageData(StorageFile &storageFile)
{
	unsigned long color;
	char *temp_string;
	x = storageFile.getSignedInt();
	y = storageFile.getSignedInt();
	color = storageFile.getUnsignedLong();
	enabledTopColor = QColor((int)((color >> 16) % 256), (int)((color >> 8) % 256), (int)(color % 256));
	color = storageFile.getUnsignedLong();
	enabledBottomColor = QColor((int)((color >> 16) % 256), (int)((color >> 8) % 256), (int)(color % 256));
	color = storageFile.getUnsignedLong();
	disabledTopColor = QColor((int)((color >> 16) % 256), (int)((color >> 8) % 256), (int)(color % 256));
	color = storageFile.getUnsignedLong();
	disabledBottomColor = QColor((int)((color >> 16) % 256), (int)((color >> 8) % 256), (int)(color % 256));
	textType = storageFile.getSignedInt();
	textWidth = storageFile.getSignedInt();
	alignment = storageFile.getSignedInt();
	fontID = storageFile.getSignedInt();
	saveSlotID = storageFile.getSignedInt();
	if (textType == TYPE_PLAINTEXT)
	{
		temp_string = storageFile.getString();
		text.plainText = new QString(temp_string);
		delete[] temp_string;
	}
	else if (textType == TYPE_PARTYMEMBER)
	{
		text.partyMemberText = new PartyMemberText;
		text.partyMemberText->memberID = storageFile.getSignedInt();
		text.partyMemberText->textType = storageFile.getSignedInt();
		text.partyMemberText->equipmentTextType = storageFile.getSignedInt();
	}
	else if (textType == TYPE_MONSTER)
	{
		text.monsterText = new MonsterText;
		text.monsterText->monsterID = storageFile.getSignedInt();
		text.monsterText->textType = storageFile.getSignedInt();
		text.monsterText->idIsReferencedInVariable = storageFile.getBool();
	}
	else if (textType == TYPE_ITEM)
	{
		text.itemText = new ItemText;
		text.itemText->itemID = storageFile.getSignedInt();
		text.itemText->textType = storageFile.getSignedInt();
		text.itemText->idIsReferencedInVariable = storageFile.getBool();
	}
	else if (textType == TYPE_CHARACTER)
	{
		text.characterText = new CharacterText;
		text.characterText->characterID = storageFile.getSignedInt();
		text.characterText->textType = storageFile.getSignedInt();
		text.characterText->equipmentTextType = storageFile.getSignedInt();
		text.characterText->idIsReferencedInVariable = storageFile.getBool();
	}
	else if (textType == TYPE_VARIABLE)
	{
		text.variableText = new VariableText;
		text.variableText->variableID = storageFile.getSignedInt();
		text.variableText->alwaysShowPlusSign = storageFile.getBool();
	}
	else if (textType == TYPE_SWITCH)
	{
		text.switchText = new SwitchText;
		text.switchText->switchID = storageFile.getSignedInt();
		temp_string = storageFile.getString();
		text.switchText->onText = temp_string;
		delete[] temp_string;
		temp_string = storageFile.getString();
		text.switchText->offText = temp_string;
		delete[] temp_string;
	}
	else if (textType == TYPE_STRING)
	{
		text.stringText = new StringText;
		text.stringText->stringID = storageFile.getSignedInt();
		text.stringText->systemString = storageFile.getBool();
	}
}
