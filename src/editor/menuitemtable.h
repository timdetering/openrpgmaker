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

#ifndef MENUITEMTABLE_H
#define MENUITEMTABLE_H

class BaseEvent;

#include <QPoint>
#include <QMap>
#include "menuobject.h"
#include "menutableitem.h"
#include "xmlParser.h"

class MenuItemTable : public MenuObject
{
	public:
		friend class MenuObject;
		friend class MainMenuLayout;
		friend class PopupMenuLayout;
		friend class TitleMenuLayout;
		friend class MenuItemList;
		enum Alignment {ALIGN_LEFT=0, ALIGN_CENTER, ALIGN_RIGHT};
		enum Type
		{
			TYPE_ITEMS=0,
			TYPE_SKILLS,
			TYPE_EQUIPMENT,
			TYPE_CHARACTERS,
			TYPE_CLASSES,
			TYPE_MONSTERS,
			TYPE_VEHICLES
		};
		struct ItemSettings
		{
			bool showCommonItems;
			bool showWeapons;
			bool showShields;
			bool showArmor;
			bool showHelmets;
			bool showAccessories;
			bool showMedicine;
			bool showSkillBooks;
			bool showSeeds;
			bool showSkillScrolls;
			bool showSwitches;
			bool showEvents;
		};
		struct SkillSettings
		{
			QList<bool> skillTypesToShow;
			int memberID;
		};
		struct EquipmentSettings
		{
			enum EquipmentType {TYPE_WEAPONS=0, TYPE_SHIELDS, TYPE_ARMOR, TYPE_HELMETS, TYPE_ACCESSORIES} equipmentType;
			int memberID;
		};
		struct OtherSettings
		{
			int startVariableID;
			bool useVariablesToSetState;
		};
		union Settings
		{
			ItemSettings *items;
			SkillSettings *skills;
			EquipmentSettings *equipment;
			OtherSettings *other;
		};
		MenuItemTable(MenuObject *parent=0);
		MenuItemTable(MainMenuLayout *layout, XMLNode ObjectNode, MenuObject *parent=0);
		MenuItemTable(PopupMenuLayout *layout, XMLNode ObjectNode, MenuObject *parent=0);
		MenuItemTable(TitleMenuLayout *layout, XMLNode ObjectNode, MenuObject *parent=0);
		MenuItemTable(MainMenuLayout *layout, StorageFile &storageFile, MenuObject *parent=0);
		MenuItemTable(PopupMenuLayout *layout, StorageFile &storageFile, MenuObject *parent=0);
		MenuItemTable(TitleMenuLayout *layout, StorageFile &storageFile, MenuObject *parent=0);
		~MenuItemTable();
		void getCopy(MenuObject *object);
		XMLNode getObjectNode();
		void saveToStorageFile(StorageFile &storageFile);
		QStringList getResources();
		BaseEvent *getSelectionChangedEvent();
		BaseEvent *getAcceptEvent();
		BaseEvent *getCancelEvent();
		QColor getEnabledTopColor();
		QColor getEnabledBottomColor();
		void setEnabledColor(QColor top, QColor bottom);
		QColor getDisabledTopColor();
		QColor getDisabledBottomColor();
		void setDisabledColor(QColor top, QColor bottom);
		int getTableType();
		void setTableType(int value);
		int getNumColumns();
		void setNumColumns(int value);
		int getTextAlignment();
		void setTextAlignment(int value);
		int getSelectionVariableID();
		void setSelectionVariableID(int value);
		int getRowSpacing();
		void setRowSpacing(int value);
		int getColumnSpacing();
		void setColumnSpacing(int value);
		int getRowHeight();
		void setRowHeight(int value);
		int getTextXOffset();
		void setTextXOffset(int value);
		int getTextYOffset();
		void setTextYOffset(int value);
		int getFontID();
		void setFontID(int value);
		bool isSortingEnabled();
		void setSortingEnabled(bool value);
		void getSettings(ItemSettings *value);
		void getSettings(SkillSettings *value);
		void getSettings(EquipmentSettings *value);
		void getSettings(OtherSettings *value);
		void setSettings(ItemSettings *value);
		void setSettings(SkillSettings *value);
		void setSettings(EquipmentSettings *value);
		void setSettings(OtherSettings *value);
		static bool tryLoad(XMLNode ObjectNode);
		static bool tryLoadSettings(XMLNode SettingsNode);
	protected:
		virtual void resized();
		void updatePixmap();
		Settings settings;
		BaseEvent *selectionChangedEvent;
		BaseEvent *acceptEvent;
		BaseEvent *cancelEvent;
		QColor enabledTopColor;
		QColor enabledBottomColor;
		QColor disabledTopColor;
		QColor disabledBottomColor;
		int tableType;
		int numColumns;
		int textAlignment;
		int selectionVariableID;
		int rowSpacing;
		int columnSpacing;
		int rowHeight;
		int textXOffset;
		int textYOffset;
		int fontID;
		bool sortingEnabled;
	private:
		void loadXMLData(XMLNode ObjectNode);
		void loadStorageData(StorageFile &storageFile);
};

#endif // MENUITEMTABLE_H
