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
 *  Copyright (C) 2010, Justin Davis <tuxdavis@gmail.com>                   *
 ****************************************************************************/

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include "clipboard.h"
#include "database.h"
#include "database_attributes.h"
#include "database_battleanimations.h"
#include "database_characters.h"
#include "database_charactersprites.h"
#include "database_classes.h"
#include "database_commonevents.h"
#include "database_conditions.h"
#include "database_globalanimations.h"
#include "database_items.h"
#include "database_monsteranimations.h"
#include "database_monstergroups.h"
#include "database_monsters.h"
#include "database_screenlayouts.h"
#include "database_skills.h"
#include "database_strings.h"
#include "database_system.h"
#include "database_terrain.h"
#include "database_tilesets.h"
#include "projectdata.h"
#include "xmlParser.h"

Database::Database(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	currentIndex = TAB_TILESETS;
	stackedWidget->setCurrentWidget(wTilesets);
	buttonBox->button(QDialogButtonBox::Apply)->setShortcut(QKeySequence::Save);
	resize(sizeHint());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
}

Database::~Database()
{
}

void Database::setupDatabase()
{
	ProjectData::backupStorage();
	if (currentIndex == TAB_TILESETS)
		wTilesets->setupTilesets();
	else if (currentIndex == TAB_TERRAIN)
		wTerrain->setupTerrain();
	else if (currentIndex == TAB_STRINGS)
		wStrings->setupStrings();
	else if (currentIndex == TAB_SYSTEM)
		wSystem->setupSystem();
	else if (currentIndex == TAB_COMMONEVENTS)
		wCommonEvents->setupCommonEvents();
	else if (currentIndex == TAB_ITEMS)
		wItems->setupItems();
	else if (currentIndex == TAB_SKILLS)
		wSkills->setupSkills();
	else if (currentIndex == TAB_ATTRIBUTES)
		wAttributes->setupAttributes();
	else if (currentIndex == TAB_CHARACTERS)
		wCharacters->setupCharacters();
	else if (currentIndex == TAB_CLASSES)
		wClasses->setupClasses();
	else if (currentIndex == TAB_CHARACTERSPRITES)
		wCharacterSprites->setupCharacterSprites();
	else if (currentIndex == TAB_CONDITIONS)
		wConditions->setupConditions();
	else if (currentIndex == TAB_MONSTERANIMATIONS)
		wMonsterAnimations->setupMonsterAnimations();
	else if (currentIndex == TAB_MONSTERS)
		wMonsters->setupMonsters();
	else if (currentIndex == TAB_MONSTERGROUPS)
		wMonsterGroups->setupMonsterGroups();
	else if (currentIndex == TAB_BATTLEANIMATIONS)
		wBattleAnimations->setupBattleAnimations();
	else if (currentIndex == TAB_GLOBALANIMATIONS)
		wGlobalAnimations->setupGlobalAnimations();
	else if (currentIndex == TAB_SCREENLAYOUTS)
		wScreenLayouts->setupScreenLayouts();
}

void Database::shutdownDatabase()
{
	if (currentIndex == TAB_TILESETS)
		wTilesets->shutdownTilesets();
	else if (currentIndex == TAB_TERRAIN)
		wTerrain->shutdownTerrain();
	else if (currentIndex == TAB_STRINGS)
		wStrings->shutdownStrings();
	else if (currentIndex == TAB_SYSTEM)
		wSystem->shutdownSystem();
	else if (currentIndex == TAB_COMMONEVENTS)
		wCommonEvents->shutdownCommonEvents();
	else if (currentIndex == TAB_ITEMS)
		wItems->shutdownItems();
	else if (currentIndex == TAB_SKILLS)
		wSkills->shutdownSkills();
	else if (currentIndex == TAB_ATTRIBUTES)
	{
		wAttributes->applyAttributes();
		wAttributes->shutdownAttributes();
	}
	else if (currentIndex == TAB_CHARACTERS)
		wCharacters->shutdownCharacters();
	else if (currentIndex == TAB_CLASSES)
		wClasses->shutdownClasses();
	else if (currentIndex == TAB_CHARACTERSPRITES)
		wCharacterSprites->shutdownCharacterSprites();
	else if (currentIndex == TAB_CONDITIONS)
	{
		wConditions->applyConditions();
		wConditions->shutdownConditions();
	}
	else if (currentIndex == TAB_MONSTERANIMATIONS)
		wMonsterAnimations->shutdownMonsterAnimations();
	else if (currentIndex == TAB_MONSTERS)
		wMonsters->shutdownMonsters();
	else if (currentIndex == TAB_MONSTERGROUPS)
		wMonsterGroups->shutdownMonsterGroups();
	else if (currentIndex == TAB_BATTLEANIMATIONS)
		wBattleAnimations->shutdownBattleAnimations();
	else if (currentIndex == TAB_GLOBALANIMATIONS)
		wGlobalAnimations->shutdownGlobalAnimations();
	else if (currentIndex == TAB_SCREENLAYOUTS)
		wScreenLayouts->shutdownScreenLayouts();
}

void Database::on_bTilesets_toggled(bool on)
{
	if (on)
		changeCurrentIndex(TAB_TILESETS);
}

void Database::on_bTerrain_toggled(bool on)
{
	if (on)
		changeCurrentIndex(TAB_TERRAIN);
}

void Database::on_bCharacters_toggled(bool on)
{
	if (on)
		changeCurrentIndex(TAB_CHARACTERS);
}

void Database::on_bClasses_toggled(bool on)
{
	if (on)
		changeCurrentIndex(TAB_CLASSES);
}

void Database::on_bCharacterSprites_toggled(bool on)
{
	if (on)
		changeCurrentIndex(TAB_CHARACTERSPRITES);
}

void Database::on_bSkills_toggled(bool on)
{
	if (on)
		changeCurrentIndex(TAB_SKILLS);
}

void Database::on_bItems_toggled(bool on)
{
	if (on)
		changeCurrentIndex(TAB_ITEMS);
}

void Database::on_bAttributes_toggled(bool on)
{
	if (on)
		changeCurrentIndex(TAB_ATTRIBUTES);
}

void Database::on_bConditions_toggled(bool on)
{
	if (on)
		changeCurrentIndex(TAB_CONDITIONS);
}

void Database::on_bStrings_toggled(bool on)
{
	if (on)
		changeCurrentIndex(TAB_STRINGS);
}

void Database::on_bSystem_toggled(bool on)
{
	if (on)
		changeCurrentIndex(TAB_SYSTEM);
}

void Database::on_bCommonEvents_toggled(bool on)
{
	if (on)
		changeCurrentIndex(TAB_COMMONEVENTS);
}

void Database::on_bMonsterAnimations_toggled(bool on)
{
	if (on)
		changeCurrentIndex(TAB_MONSTERANIMATIONS);
}

void Database::on_bMonsters_toggled(bool on)
{
	if (on)
		changeCurrentIndex(TAB_MONSTERS);
}

void Database::on_bMonsterGroups_toggled(bool on)
{
	if (on)
		changeCurrentIndex(TAB_MONSTERGROUPS);
}

void Database::on_bBattleAnimations_toggled(bool on)
{
	if (on)
		changeCurrentIndex(TAB_BATTLEANIMATIONS);
}

void Database::on_bGlobalAnimations_toggled(bool on)
{
	if (on)
		changeCurrentIndex(TAB_GLOBALANIMATIONS);
}

void Database::on_bScreenLayouts_toggled(bool on)
{
	if (on)
		changeCurrentIndex(TAB_SCREENLAYOUTS);
}

void Database::on_buttonBox_clicked(QAbstractButton *button)
{
	int buttonClicked = buttonBox->standardButton(button);
	if (buttonClicked == QDialogButtonBox::Ok)
	{
		ProjectData::cleanBackupStorage();
		ProjectData::saved = false;
		accept();
	}
	else if (buttonClicked == QDialogButtonBox::Cancel)
	{
		ProjectData::loadBackupStorage();
		ProjectData::cleanBackupStorage();
		reject();
	}
	else if (buttonClicked == QDialogButtonBox::Apply)
	{
		if (currentIndex == TAB_ATTRIBUTES)
			wAttributes->applyAttributes();
		else if (currentIndex == TAB_CONDITIONS)
			wConditions->applyConditions();
		ProjectData::saveAllDataReferencesToStorage();
		ProjectData::backupStorage();
		ProjectData::saved = false;
	}
	else if (buttonClicked == QDialogButtonBox::Help)
	{
		if (currentIndex == TAB_TILESETS)
			ProjectData::helpManual->loadIndex("Database::Tilesets");
		else if (currentIndex == TAB_TERRAIN)
			ProjectData::helpManual->loadIndex("Database::Terrain");
		else if (currentIndex == TAB_STRINGS)
			ProjectData::helpManual->loadIndex("Database::Strings");
		else if (currentIndex == TAB_SYSTEM)
			ProjectData::helpManual->loadIndex("Database::System");
		else if (currentIndex == TAB_COMMONEVENTS)
			ProjectData::helpManual->loadIndex("Database::CommonEvents");
		else if (currentIndex == TAB_ITEMS)
			ProjectData::helpManual->loadIndex("Database::Items");
		else if (currentIndex == TAB_SKILLS)
			ProjectData::helpManual->loadIndex("Database::Skills");
		else if (currentIndex == TAB_ATTRIBUTES)
			ProjectData::helpManual->loadIndex("Database::Attributes");
		else if (currentIndex == TAB_CHARACTERS)
			ProjectData::helpManual->loadIndex("Database::Characters");
		else if (currentIndex == TAB_CLASSES)
			ProjectData::helpManual->loadIndex("Database::Classes");
		else if (currentIndex == TAB_CONDITIONS)
			ProjectData::helpManual->loadIndex("Database::Conditions");
		else if (currentIndex == TAB_MONSTERS)
			ProjectData::helpManual->loadIndex("Database::Monsters");
		else if (currentIndex == TAB_MONSTERGROUPS)
			ProjectData::helpManual->loadIndex("Database::MonsterGroups");
		else if (currentIndex == TAB_BATTLEANIMATIONS)
			ProjectData::helpManual->loadIndex("Database::BattleAnimations");
		else if (currentIndex == TAB_GLOBALANIMATIONS)
			ProjectData::helpManual->loadIndex("Database::GlobalAnimations");
		else if (currentIndex == TAB_CHARACTERSPRITES)
			ProjectData::helpManual->loadIndex("Database::CharacterSprites");
		else if (currentIndex == TAB_SCREENLAYOUTS)
			ProjectData::helpManual->loadIndex("Database::ScreenLayouts");
		if (!ProjectData::helpManual->isVisible())
			ProjectData::helpManual->show();
		else
			ProjectData::helpManual->raise();
		ProjectData::helpManual->activateWindow();
	}
}

void Database::changeCurrentIndex(int index)
{
	if (currentIndex == TAB_TILESETS)
		wTilesets->shutdownTilesets();
	else if (currentIndex == TAB_TERRAIN)
		wTerrain->shutdownTerrain();
	else if (currentIndex == TAB_STRINGS)
		wStrings->shutdownStrings();
	else if (currentIndex == TAB_SYSTEM)
		wSystem->shutdownSystem();
	else if (currentIndex == TAB_COMMONEVENTS)
		wCommonEvents->shutdownCommonEvents();
	else if (currentIndex == TAB_ITEMS)
		wItems->shutdownItems();
	else if (currentIndex == TAB_SKILLS)
		wSkills->shutdownSkills();
	else if (currentIndex == TAB_ATTRIBUTES)
	{
		wAttributes->applyAttributes();
		wAttributes->shutdownAttributes();
	}
	else if (currentIndex == TAB_CHARACTERS)
		wCharacters->shutdownCharacters();
	else if (currentIndex == TAB_CLASSES)
		wClasses->shutdownClasses();
	else if (currentIndex == TAB_CHARACTERSPRITES)
		wCharacterSprites->shutdownCharacterSprites();
	else if (currentIndex == TAB_CONDITIONS)
	{
		wConditions->applyConditions();
		wConditions->shutdownConditions();
	}
	else if (currentIndex == TAB_MONSTERANIMATIONS)
		wMonsterAnimations->shutdownMonsterAnimations();
	else if (currentIndex == TAB_MONSTERS)
		wMonsters->shutdownMonsters();
	else if (currentIndex == TAB_MONSTERGROUPS)
		wMonsterGroups->shutdownMonsterGroups();
	else if (currentIndex == TAB_BATTLEANIMATIONS)
		wBattleAnimations->shutdownBattleAnimations();
	else if (currentIndex == TAB_GLOBALANIMATIONS)
		wGlobalAnimations->shutdownGlobalAnimations();
	else if (currentIndex == TAB_SCREENLAYOUTS)
		wScreenLayouts->shutdownScreenLayouts();
	currentIndex = index;
	if (currentIndex == TAB_TILESETS)
	{
		wTilesets->setupTilesets();
		stackedWidget->setCurrentWidget(wTilesets);
	}
	else if (currentIndex == TAB_TERRAIN)
	{
		wTerrain->setupTerrain();
		stackedWidget->setCurrentWidget(wTerrain);
	}
	else if (currentIndex == TAB_STRINGS)
	{
		wStrings->setupStrings();
		stackedWidget->setCurrentWidget(wStrings);
	}
	else if (currentIndex == TAB_SYSTEM)
	{
		wSystem->setupSystem();
		stackedWidget->setCurrentWidget(wSystem);
	}
	else if (currentIndex == TAB_COMMONEVENTS)
	{
		wCommonEvents->setupCommonEvents();
		stackedWidget->setCurrentWidget(wCommonEvents);
	}
	else if (currentIndex == TAB_ITEMS)
	{
		wItems->setupItems();
		stackedWidget->setCurrentWidget(wItems);
	}
	else if (currentIndex == TAB_SKILLS)
	{
		wSkills->setupSkills();
		stackedWidget->setCurrentWidget(wSkills);
	}
	else if (currentIndex == TAB_ATTRIBUTES)
	{
		wAttributes->setupAttributes();
		stackedWidget->setCurrentWidget(wAttributes);
	}
	else if (currentIndex == TAB_CHARACTERS)
	{
		wCharacters->setupCharacters();
		stackedWidget->setCurrentWidget(wCharacters);
	}
	else if (currentIndex == TAB_CLASSES)
	{
		wClasses->setupClasses();
		stackedWidget->setCurrentWidget(wClasses);
	}
	else if (currentIndex == TAB_CHARACTERSPRITES)
	{
		wCharacterSprites->setupCharacterSprites();
		stackedWidget->setCurrentWidget(wCharacterSprites);
	}
	else if (currentIndex == TAB_CONDITIONS)
	{
		wConditions->setupConditions();
		stackedWidget->setCurrentWidget(wConditions);
	}
	else if (currentIndex == TAB_MONSTERANIMATIONS)
	{
		wMonsterAnimations->setupMonsterAnimations();
		stackedWidget->setCurrentWidget(wMonsterAnimations);
	}
	else if (currentIndex == TAB_MONSTERS)
	{
		wMonsters->setupMonsters();
		stackedWidget->setCurrentWidget(wMonsters);
	}
	else if (currentIndex == TAB_MONSTERGROUPS)
	{
		wMonsterGroups->setupMonsterGroups();
		stackedWidget->setCurrentWidget(wMonsterGroups);
	}
	else if (currentIndex == TAB_BATTLEANIMATIONS)
	{
		wBattleAnimations->setupBattleAnimations();
		stackedWidget->setCurrentWidget(wBattleAnimations);
	}
	else if (currentIndex == TAB_GLOBALANIMATIONS)
	{
		wGlobalAnimations->setupGlobalAnimations();
		stackedWidget->setCurrentWidget(wGlobalAnimations);
	}
	else if (currentIndex == TAB_SCREENLAYOUTS)
	{
		wScreenLayouts->setupScreenLayouts();
		stackedWidget->setCurrentWidget(wScreenLayouts);
	}
}
