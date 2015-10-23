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

#include "browsemenuevents.h"
#include "projectdata_baseevent.h"

int BrowseMenuEvents::lastUsedTab = BrowseMenuEvents::TAB_MENUMANAGEMENT;

BrowseMenuEvents::BrowseMenuEvents(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	resize(sizeHint());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
	if (lastUsedTab == TAB_AUDIOVIDEO)
		bAudioVideo->setChecked(true);
	else if (lastUsedTab == TAB_EVENTMANAGEMENT)
		bEventManagement->setChecked(true);
	else if (lastUsedTab == TAB_MENUMANAGEMENT)
		bMenuManagement->setChecked(true);
	else if (lastUsedTab == TAB_PARTYCHARACTERMANAGEMENT)
		bPartyCharacterManagement->setChecked(true);
	else if (lastUsedTab == TAB_SYSTEMMANAGEMENT)
		bSystemManagement->setChecked(true);
	else if (lastUsedTab == TAB_VARIABLES)
		bVariables->setChecked(true);
	selectedEvent = BaseEvent::TYPE_TOPLEVELEVENT;
}

BrowseMenuEvents::~BrowseMenuEvents()
{
}

int BrowseMenuEvents::getSelectedEvent()
{
	return selectedEvent;
}

void BrowseMenuEvents::on_bAudioVideo_toggled(bool checked)
{
	if (checked)
	{
		stackedWidget->setCurrentWidget(wAudioVideo);
		lastUsedTab = TAB_AUDIOVIDEO;
	}
}

void BrowseMenuEvents::on_bEventManagement_toggled(bool checked)
{
	if (checked)
	{
		stackedWidget->setCurrentWidget(wEventManagement);
		lastUsedTab = TAB_EVENTMANAGEMENT;
	}
}

void BrowseMenuEvents::on_bMenuManagement_toggled(bool checked)
{
	if (checked)
	{
		stackedWidget->setCurrentWidget(wMenuManagement);
		lastUsedTab = TAB_MENUMANAGEMENT;
	}
}

void BrowseMenuEvents::on_bPartyCharacterManagement_toggled(bool checked)
{
	if (checked)
	{
		stackedWidget->setCurrentWidget(wPartyCharacterManagement);
		lastUsedTab = TAB_PARTYCHARACTERMANAGEMENT;
	}
}

void BrowseMenuEvents::on_bSystemManagement_toggled(bool checked)
{
	if (checked)
	{
		stackedWidget->setCurrentWidget(wSystemManagement);
		lastUsedTab = TAB_SYSTEMMANAGEMENT;
	}
}

void BrowseMenuEvents::on_bVariables_toggled(bool checked)
{
	if (checked)
	{
		stackedWidget->setCurrentWidget(wVariables);
		lastUsedTab = TAB_VARIABLES;
	}
}

void BrowseMenuEvents::on_bBreakOutOfLoop_clicked()
{
	selectedEvent = BaseEvent::TYPE_BREAKOUTOFLOOP;
	accept();
}

void BrowseMenuEvents::on_bCallEvent_clicked()
{
	selectedEvent = BaseEvent::TYPE_CALLCOMMONEVENT;
	accept();
}

void BrowseMenuEvents::on_bChangeBattleCommands_clicked()
{
	selectedEvent = BaseEvent::TYPE_CHANGEBATTLECOMMANDS;
	accept();
}

void BrowseMenuEvents::on_bChangeCharacterBaseStats_clicked()
{
	selectedEvent = BaseEvent::TYPE_CHANGECHARACTERBASESTATS;
	accept();
}

void BrowseMenuEvents::on_bChangeCharacterCondition_clicked()
{
	selectedEvent = BaseEvent::TYPE_CHANGECHARACTERCONDITION;
	accept();
}

void BrowseMenuEvents::on_bChangeCharacterHP_clicked()
{
	selectedEvent = BaseEvent::TYPE_CHANGECHARACTERHP;
	accept();
}

void BrowseMenuEvents::on_bChangeCharacterLevel_clicked()
{
	selectedEvent = BaseEvent::TYPE_CHANGECHARACTERLEVEL;
	accept();
}

void BrowseMenuEvents::on_bChangeCharacterMP_clicked()
{
	selectedEvent = BaseEvent::TYPE_CHANGECHARACTERMP;
	accept();
}

void BrowseMenuEvents::on_bChangeCharacterSprite_clicked()
{
	selectedEvent = BaseEvent::TYPE_CHANGECHARACTERSPRITE;
	accept();
}

void BrowseMenuEvents::on_bChangeClass_clicked()
{
	selectedEvent = BaseEvent::TYPE_CHANGECLASS;
	accept();
}

void BrowseMenuEvents::on_bChangeEquippedItems_clicked()
{
	selectedEvent = BaseEvent::TYPE_CHANGEEQUIPPEDITEMS;
	accept();
}

void BrowseMenuEvents::on_bChangeExperiencePoints_clicked()
{
	selectedEvent = BaseEvent::TYPE_CHANGEEXPERIENCEPOINTS;
	accept();
}

void BrowseMenuEvents::on_bChangeFaceGraphic_clicked()
{
	selectedEvent = BaseEvent::TYPE_CHANGEFACEGRAPHIC;
	accept();
}

void BrowseMenuEvents::on_bChangeHeroName_clicked()
{
	selectedEvent = BaseEvent::TYPE_CHANGEHERONAME;
	accept();
}

void BrowseMenuEvents::on_bChangeImage_clicked()
{
	selectedEvent = BaseEvent::TYPE_CHANGEIMAGE;
	accept();
}

void BrowseMenuEvents::on_bChangeItemList_clicked()
{
	selectedEvent = BaseEvent::TYPE_CHANGEITEMLIST;
	accept();
}

void BrowseMenuEvents::on_bChangeMoney_clicked()
{
	selectedEvent = BaseEvent::TYPE_CHANGEMONEY;
	accept();
}

void BrowseMenuEvents::on_bChangeParty_clicked()
{
	selectedEvent = BaseEvent::TYPE_CHANGEPARTY;
	accept();
}

void BrowseMenuEvents::on_bChangeSelectedArea_clicked()
{
	selectedEvent = BaseEvent::TYPE_CHANGESELECTEDAREA;
	accept();
}

void BrowseMenuEvents::on_bChangeSkills_clicked()
{
	selectedEvent = BaseEvent::TYPE_CHANGESKILLS;
	accept();
}

void BrowseMenuEvents::on_bChangeSystemBGM_clicked()
{
	selectedEvent = BaseEvent::TYPE_CHANGESYSTEMBGM;
	accept();
}

void BrowseMenuEvents::on_bChangeSystemGraphic_clicked()
{
	selectedEvent = BaseEvent::TYPE_CHANGESYSTEMGRAPHICS;
	accept();
}

void BrowseMenuEvents::on_bChangeSystemSoundEffect_clicked()
{
	selectedEvent = BaseEvent::TYPE_CHANGESYSTEMSOUNDEFFECT;
	accept();
}

void BrowseMenuEvents::on_bChangeText_clicked()
{
	selectedEvent = BaseEvent::TYPE_CHANGETEXT;
	accept();
}

void BrowseMenuEvents::on_bChangeVehicleGraphic_clicked()
{
	selectedEvent = BaseEvent::TYPE_CHANGEVEHICLEGRAPHIC;
	accept();
}

void BrowseMenuEvents::on_bCompleteHealing_clicked()
{
	selectedEvent = BaseEvent::TYPE_COMPLETEHEALING;
	accept();
}

void BrowseMenuEvents::on_bConditionalBranch_clicked()
{
	selectedEvent = BaseEvent::TYPE_CONDITIONALBRANCH_MENU;
	accept();
}

void BrowseMenuEvents::on_bEnableDisableObject_clicked()
{
	selectedEvent = BaseEvent::TYPE_ENABLEDISABLEOBJECT;
	accept();
}

void BrowseMenuEvents::on_bEndEventProcessing_clicked()
{
	selectedEvent = BaseEvent::TYPE_ENDEVENTPROCESSING;
	accept();
}

void BrowseMenuEvents::on_bEnemyEncounter_clicked()
{
	selectedEvent = BaseEvent::TYPE_ENEMYENCOUNTER;
	accept();
}

void BrowseMenuEvents::on_bExitMenu_clicked()
{
	selectedEvent = BaseEvent::TYPE_EXITMENU;
	accept();
}

void BrowseMenuEvents::on_bFadeBGM_clicked()
{
	selectedEvent = BaseEvent::TYPE_FADEBGM;
	accept();
}

void BrowseMenuEvents::on_bInsertComment_clicked()
{
	selectedEvent = BaseEvent::TYPE_INSERTCOMMENT;
	accept();
}

void BrowseMenuEvents::on_bItemManagement_clicked()
{
	selectedEvent = BaseEvent::TYPE_ITEMMANAGEMENT;
	accept();
}

void BrowseMenuEvents::on_bJumpToLabel_clicked()
{
	selectedEvent = BaseEvent::TYPE_JUMPTOLABEL;
	accept();
}

void BrowseMenuEvents::on_bKeyInputProcessing_clicked()
{
	selectedEvent = BaseEvent::TYPE_KEYINPUTPROCESSING;
	accept();
}

void BrowseMenuEvents::on_bLabel_clicked()
{
	selectedEvent = BaseEvent::TYPE_LABEL;
	accept();
}

void BrowseMenuEvents::on_bLoadGame_clicked()
{
	selectedEvent = BaseEvent::TYPE_LOADGAME;
	accept();
}

void BrowseMenuEvents::on_bLoop_clicked()
{
	selectedEvent = BaseEvent::TYPE_LOOP;
	accept();
}

void BrowseMenuEvents::on_bMemorizeBGM_clicked()
{
	selectedEvent = BaseEvent::TYPE_MEMORIZEBGM;
	accept();
}

void BrowseMenuEvents::on_bOpenMenu_clicked()
{
	selectedEvent = BaseEvent::TYPE_OPENMENU;
	accept();
}

void BrowseMenuEvents::on_bPlayBGM_clicked()
{
	selectedEvent = BaseEvent::TYPE_PLAYBGM;
	accept();
}

void BrowseMenuEvents::on_bPlayMemorizedBGM_clicked()
{
	selectedEvent = BaseEvent::TYPE_PLAYMEMORIZEDBGM;
	accept();
}

void BrowseMenuEvents::on_bPlayMovie_clicked()
{
	selectedEvent = BaseEvent::TYPE_PLAYMOVIE;
	accept();
}

void BrowseMenuEvents::on_bPlaySoundEffect_clicked()
{
	selectedEvent = BaseEvent::TYPE_PLAYSOUNDEFFECT;
	accept();
}

void BrowseMenuEvents::on_bPlaySystemBGM_clicked()
{
	selectedEvent = BaseEvent::TYPE_PLAYSYSTEMBGM;
	accept();
}

void BrowseMenuEvents::on_bPlaySystemSoundEffect_clicked()
{
	selectedEvent = BaseEvent::TYPE_PLAYSYSTEMSOUNDEFFECT;
	accept();
}

void BrowseMenuEvents::on_bPreviousSelectedAreaBranch_clicked()
{
	selectedEvent = BaseEvent::TYPE_PREVIOUSSELECTEDAREABRANCH;
	accept();
}

void BrowseMenuEvents::on_bQuitGame_clicked()
{
	selectedEvent = BaseEvent::TYPE_QUITGAME;
	accept();
}

void BrowseMenuEvents::on_bSaveGame_clicked()
{
	selectedEvent = BaseEvent::TYPE_SAVEGAME;
	accept();
}

void BrowseMenuEvents::on_bScrollArea_clicked()
{
	selectedEvent = BaseEvent::TYPE_SCROLLAREA;
	accept();
}

void BrowseMenuEvents::on_bSetScreenTransition_clicked()
{
	selectedEvent = BaseEvent::TYPE_SETSCREENTRANSITION;
	accept();
}

void BrowseMenuEvents::on_bShowPopupMenu_clicked()
{
	selectedEvent = BaseEvent::TYPE_SHOWPOPUPMENU;
	accept();
}

void BrowseMenuEvents::on_bStringOperation_clicked()
{
	selectedEvent = BaseEvent::TYPE_STRINGOPERATION;
	accept();
}

void BrowseMenuEvents::on_bSwitchOperation_clicked()
{
	selectedEvent = BaseEvent::TYPE_SWITCHOPERATION;
	accept();
}

void BrowseMenuEvents::on_bTeleport_clicked()
{
	selectedEvent = BaseEvent::TYPE_TELEPORT;
	accept();
}

void BrowseMenuEvents::on_bTimerOperation_clicked()
{
	selectedEvent = BaseEvent::TYPE_TIMEROPERATION;
	accept();
}

void BrowseMenuEvents::on_bVariableMultiValueBranch_clicked()
{
	selectedEvent = BaseEvent::TYPE_VARIABLEMULTIVALUECONDITIONALBRANCH;
	accept();
}

void BrowseMenuEvents::on_bVariableOperation_clicked()
{
	selectedEvent = BaseEvent::TYPE_VARIABLEOPERATION;
	accept();
}

void BrowseMenuEvents::on_bWait_clicked()
{
	selectedEvent = BaseEvent::TYPE_WAIT;
	accept();
}
