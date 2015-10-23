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

#ifndef MENUTEXT_H
#define MENUTEXT_H

class Character;
class Monster;
class Item;

#include "menuobject.h"
#include "xmlParser.h"

class MenuText : public MenuObject
{
	public:
		friend class MenuObject;
		friend class MainMenuLayout;
		friend class PopupMenuLayout;
		friend class TitleMenuLayout;
		friend class MenuItemList;
		enum Alignment {ALIGN_LEFT=0, ALIGN_CENTER, ALIGN_RIGHT};
		enum TextType
		{
			TYPE_PLAINTEXT=0,
			TYPE_PARTYMEMBER,
			TYPE_MONSTER,
			TYPE_ITEM,
			TYPE_CHARACTER,
			TYPE_VARIABLE,
			TYPE_SWITCH,
			TYPE_STRING
		};
		enum EquipmentTextType
		{
			EQUIPMENT_NAME=0,
			EQUIPMENT_DESCRIPTION,
			EQUIPMENT_ATTACK,
			EQUIPMENT_DEFENSE,
			EQUIPMENT_WISDOM,
			EQUIPMENT_AGILITY
		};
		enum PartyMemberTextType
		{
			PARTYMEMBER_NAME=0,
			PARTYMEMBER_CLASS,
			PARTYMEMBER_CONDITION,
			PARTYMEMBER_LEVEL,
			PARTYMEMBER_HP,
			PARTYMEMBER_MAXHP,
			PARTYMEMBER_MP,
			PARTYMEMBER_MAXMP,
			PARTYMEMBER_EXPERIENCE,
			PARTYMEMBER_EXPERIENCETONEXTLEVEL,
			PARTYMEMBER_BASEATTACK,
			PARTYMEMBER_TOTALATTACK,
			PARTYMEMBER_BASEDEFENSE,
			PARTYMEMBER_TOTALDEFENSE,
			PARTYMEMBER_BASEWISDOM,
			PARTYMEMBER_TOTALWISDOM,
			PARTYMEMBER_BASEAGILITY,
			PARTYMEMBER_TOTALAGILITY,
			PARTYMEMBER_EQUIPPEDWEAPON,
			PARTYMEMBER_EQUIPPEDSHIELD,
			PARTYMEMBER_EQUIPPEDARMOR,
			PARTYMEMBER_EQUIPPEDHELMET,
			PARTYMEMBER_EQUIPPEDACCESSORY
		};
		enum MonsterTextType
		{
			MONSTER_NAME=0,
			MONSTER_MONEY,
			MONSTER_HP,
			MONSTER_MP,
			MONSTER_ATTACK,
			MONSTER_DEFENSE,
			MONSTER_WISDOM,
			MONSTER_AGILITY,
			MONSTER_STRONGAGAINST,
			MONSTER_WEAKAGAINST,
			MONSTER_EXPERIENCE
		};
		enum ItemTextType
		{
			ITEM_NAME=0,
			ITEM_DESCRIPTION,
			ITEM_PRICE
		};
		enum CharacterTextType
		{
			CHARACTER_NAME=0,
			CHARACTER_CLASS,
			CHARACTER_CONDITION,
			CHARACTER_LEVEL,
			CHARACTER_HP,
			CHARACTER_MAXHP,
			CHARACTER_MP,
			CHARACTER_MAXMP,
			CHARACTER_EXPERIENCE,
			CHARACTER_EXPERIENCETONEXTLEVEL,
			CHARACTER_BASEATTACK,
			CHARACTER_TOTALATTACK,
			CHARACTER_BASEDEFENSE,
			CHARACTER_TOTALDEFENSE,
			CHARACTER_BASEWISDOM,
			CHARACTER_TOTALWISDOM,
			CHARACTER_BASEAGILITY,
			CHARACTER_TOTALAGILITY,
			CHARACTER_EQUIPPEDWEAPON,
			CHARACTER_EQUIPPEDSHIELD,
			CHARACTER_EQUIPPEDARMOR,
			CHARACTER_EQUIPPEDHELMET,
			CHARACTER_EQUIPPEDACCESSORY
		};
		struct PartyMemberText
		{
			int memberID;
			int textType;
			int equipmentTextType;
		};
		struct MonsterText
		{
			int monsterID;
			int textType;
			bool idIsReferencedInVariable;
		};
		struct ItemText
		{
			int itemID;
			int textType;
			bool idIsReferencedInVariable;
		};
		struct CharacterText
		{
			int characterID;
			int textType;
			int equipmentTextType;
			bool idIsReferencedInVariable;
		};
		struct VariableText
		{
			int variableID;
			bool alwaysShowPlusSign;
		};
		struct SwitchText
		{
			int switchID;
			QString onText;
			QString offText;
		};
		struct StringText
		{
			int stringID;
			bool systemString;
		};
		MenuText(MenuObject *parent=0);
		MenuText(MainMenuLayout *layout, XMLNode ObjectNode, MenuObject *parent=0);
		MenuText(PopupMenuLayout *layout, XMLNode ObjectNode, MenuObject *parent=0);
		MenuText(TitleMenuLayout *layout, XMLNode ObjectNode, MenuObject *parent=0);
		MenuText(MainMenuLayout *layout, StorageFile &storageFile, MenuObject *parent=0);
		MenuText(PopupMenuLayout *layout, StorageFile &storageFile, MenuObject *parent=0);
		MenuText(TitleMenuLayout *layout, StorageFile &storageFile, MenuObject *parent=0);
		~MenuText();
		void getCopy(MenuObject *object);
		XMLNode getObjectNode();
		void saveToStorageFile(StorageFile &storageFile);
		QStringList getResources();
		int getTextType();
		void setTextType(int value);
		void getText(QString *value);
		void getText(PartyMemberText *value);
		void getText(MonsterText *value);
		void getText(ItemText *value);
		void getText(CharacterText *value);
		void getText(VariableText *value);
		void getText(SwitchText *value);
		void getText(StringText *value);
		void setText(QString value);
		void setText(PartyMemberText *value);
		void setText(MonsterText *value);
		void setText(ItemText *value);
		void setText(CharacterText *value);
		void setText(VariableText *value);
		void setText(SwitchText *value);
		void setText(StringText *value);
		QColor getEnabledTopColor();
		QColor getEnabledBottomColor();
		void setEnabledColor(QColor top, QColor bottom);
		QColor getDisabledTopColor();
		QColor getDisabledBottomColor();
		void setDisabledColor(QColor top, QColor bottom);
		int getTextWidth();
		void setTextWidth(int value);
		int getAlignment();
		void setAlignment(int value);
		int getFontID();
		void setFontID(int value);
		int getSaveSlotID();
		void setSaveSlotID(int value);
		QString getFinalText();
		static bool tryLoad(XMLNode ObjectNode);
		static bool tryLoadTextData(XMLNode TextNode);
		union TextData
		{
			QString *plainText;
			PartyMemberText *partyMemberText;
			MonsterText *monsterText;
			ItemText *itemText;
			CharacterText *characterText;
			VariableText *variableText;
			SwitchText *switchText;
			StringText *stringText;
		};
	protected:
		void updatePixmap();
		QColor enabledTopColor;
		QColor enabledBottomColor;
		QColor disabledTopColor;
		QColor disabledBottomColor;
		QString finalText;
		TextData text;
		int textType;
		int textWidth;
		int alignment;
		int fontID;
		int saveSlotID;
	private:
		void loadXMLData(XMLNode ObjectNode);
		void loadStorageData(StorageFile &storageFile);
};

#endif // MENUTEXT_H
