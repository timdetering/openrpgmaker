/****************************************************************************
 *  Open RPG Maker is free software: you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by    *
 *  the Free Software Foundation, either version 3 of the License, or       *
 *  (at your option) any later version.                                     *
 *                                                                          *
 *  Open RPG Maker is distributed in the hope that it will be useful,       *
 *  but WITHOUT ANY WARRANTY without even the implied warranty of          *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *  GNU General Public License for more details.                            *
 *                                                                          *
 *  You should have received a copy of the GNU General Public License       *
 *  along with Open RPG Maker. If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                          *
 *  ---                                                                     *
 *  Copyright (C) 2011, Justin Davis <tuxdavis@gmail.com>                   *
 ****************************************************************************/

#include <QDialog>
#include "browsemapevents.h"
#include "projectdata_baseevent.h"


int BrowseMapEvents::lastUsedTab = BrowseMapEvents::TAB_AUDIOVIDEO;

BrowseMapEvents::BrowseMapEvents(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	resize(sizeHint());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
	selectedEvent = -1;
	if (lastUsedTab == TAB_AUDIOVIDEO)
		bAudioVideo->setChecked(true);
	else if (lastUsedTab == TAB_EVENTMANAGEMENT)
		bEventManagement->setChecked(true);
	else if (lastUsedTab == TAB_MAPMANAGEMENT)
		bMapManagement->setChecked(true);
	else if (lastUsedTab == TAB_MOVEMENT)
		bMovement->setChecked(true);
	else if (lastUsedTab == TAB_PARTYCHARACTERMANAGEMENT)
		bPartyCharacterManagement->setChecked(true);
	else if (lastUsedTab == TAB_SCREENEFFECTS)
		bScreenEffects->setChecked(true);
	else if (lastUsedTab == TAB_SYSTEMMANAGEMENT)
		bSystemManagement->setChecked(true);
	else if (lastUsedTab == TAB_VARIABLES)
		bVariables->setChecked(true);
}

BrowseMapEvents::~BrowseMapEvents()
{
}

int BrowseMapEvents::getSelectedEvent()
{
	return selectedEvent;
}

void BrowseMapEvents::on_bAudioVideo_toggled(bool checked)
{
	if (checked)
	{
		stackedWidget->setCurrentWidget(wAudioVideo);
		lastUsedTab = TAB_AUDIOVIDEO;
	}
}

void BrowseMapEvents::on_bEventManagement_toggled(bool checked)
{
	if (checked)
	{
		stackedWidget->setCurrentWidget(wEventManagement);
		lastUsedTab = TAB_EVENTMANAGEMENT;
	}
}

void BrowseMapEvents::on_bMapManagement_toggled(bool checked)
{
	if (checked)
	{
		stackedWidget->setCurrentWidget(wMapManagement);
		lastUsedTab = TAB_MAPMANAGEMENT;
	}
}

void BrowseMapEvents::on_bMovement_toggled(bool checked)
{
	if (checked)
	{
		stackedWidget->setCurrentWidget(wMovement);
		lastUsedTab = TAB_MOVEMENT;
	}
}

void BrowseMapEvents::on_bPartyCharacterManagement_toggled(bool checked)
{
	if (checked)
	{
		stackedWidget->setCurrentWidget(wPartyCharacterManagement);
		lastUsedTab = TAB_PARTYCHARACTERMANAGEMENT;
	}
}

void BrowseMapEvents::on_bScreenEffects_toggled(bool checked)
{
	if (checked)
	{
		stackedWidget->setCurrentWidget(wScreenEffects);
		lastUsedTab = TAB_SCREENEFFECTS;
	}
}

void BrowseMapEvents::on_bSystemManagement_toggled(bool checked)
{
	if (checked)
	{
		stackedWidget->setCurrentWidget(wSystemManagement);
		lastUsedTab = TAB_SYSTEMMANAGEMENT;
	}
}

void BrowseMapEvents::on_bVariables_toggled(bool checked)
{
	if (checked)
	{
		stackedWidget->setCurrentWidget(wVariables);
		lastUsedTab = TAB_VARIABLES;
	}
}

void BrowseMapEvents::on_bAllowDisallowEscape_clicked()
{
	selectedEvent = BaseEvent::TYPE_ALLOWDISALLOWESCAPE;
	accept();
}

void BrowseMapEvents::on_bAllowDisallowMainMenu_clicked()
{
	selectedEvent = BaseEvent::TYPE_ALLOWDISALLOWMAINMENU;
	accept();
}

void BrowseMapEvents::on_bAllowDisallowSaving_clicked()
{
	selectedEvent = BaseEvent::TYPE_ALLOWDISALLOWSAVING;
	accept();
}

void BrowseMapEvents::on_bAllowDisallowTeleport_clicked()
{
	selectedEvent = BaseEvent::TYPE_ALLOWDISALLOWTELEPORT;
	accept();
}

void BrowseMapEvents::on_bBreakOutOfLoop_clicked()
{
	selectedEvent = BaseEvent::TYPE_BREAKOUTOFLOOP;
	accept();
}

void BrowseMapEvents::on_bCallCommonEvent_clicked()
{
	selectedEvent = BaseEvent::TYPE_CALLCOMMONEVENT;
	accept();
}

void BrowseMapEvents::on_bCallMapEvent_clicked()
{
	selectedEvent = BaseEvent::TYPE_CALLMAPEVENT;
	accept();
}

void BrowseMapEvents::on_bChangeBattleCommands_clicked()
{
	selectedEvent = BaseEvent::TYPE_CHANGEBATTLECOMMANDS;
	accept();
}

void BrowseMapEvents::on_bChangeBorderTile_clicked()
{
	selectedEvent = BaseEvent::TYPE_CHANGEBORDERTILE;
	accept();
}

void BrowseMapEvents::on_bChangeCharacterBaseStats_clicked()
{
	selectedEvent = BaseEvent::TYPE_CHANGECHARACTERBASESTATS;
	accept();
}

void BrowseMapEvents::on_bChangeCharacterCondition_clicked()
{
	selectedEvent = BaseEvent::TYPE_CHANGECHARACTERCONDITION;
	accept();
}

void BrowseMapEvents::on_bChangeCharacterHP_clicked()
{
	selectedEvent = BaseEvent::TYPE_CHANGECHARACTERHP;
	accept();
}

void BrowseMapEvents::on_bChangeCharacterLevel_clicked()
{
	selectedEvent = BaseEvent::TYPE_CHANGECHARACTERLEVEL;
	accept();
}

void BrowseMapEvents::on_bChangeCharacterMP_clicked()
{
	selectedEvent = BaseEvent::TYPE_CHANGECHARACTERMP;
	accept();
}

void BrowseMapEvents::on_bChangeCharacterSprite_clicked()
{
	selectedEvent = BaseEvent::TYPE_CHANGECHARACTERSPRITE;
	accept();
}

void BrowseMapEvents::on_bChangeClass_clicked()
{
	selectedEvent = BaseEvent::TYPE_CHANGECLASS;
	accept();
}

void BrowseMapEvents::on_bChangeEncounterRate_clicked()
{
	selectedEvent = BaseEvent::TYPE_CHANGEENCOUNTERRATE;
	accept();
}

void BrowseMapEvents::on_bChangeEquippedItems_clicked()
{
	selectedEvent = BaseEvent::TYPE_CHANGEEQUIPPEDITEMS;
	accept();
}

void BrowseMapEvents::on_bChangeEventLocation_clicked()
{
	selectedEvent = BaseEvent::TYPE_CHANGEEVENTLOCATION;
	accept();
}

void BrowseMapEvents::on_bChangeExperiencePoints_clicked()
{
	selectedEvent = BaseEvent::TYPE_CHANGEEXPERIENCEPOINTS;
	accept();
}

void BrowseMapEvents::on_bChangeFaceGraphic_clicked()
{
	selectedEvent = BaseEvent::TYPE_CHANGEFACEGRAPHIC;
	accept();
}

void BrowseMapEvents::on_bChangeHeroName_clicked()
{
	selectedEvent = BaseEvent::TYPE_CHANGEHERONAME;
	accept();
}

void BrowseMapEvents::on_bChangeMoney_clicked()
{
	selectedEvent = BaseEvent::TYPE_CHANGEMONEY;
	accept();
}

void BrowseMapEvents::on_bChangeParallaxBackground_clicked()
{
	selectedEvent = BaseEvent::TYPE_CHANGEPARALLAXBACKGROUND;
	accept();
}

void BrowseMapEvents::on_bChangeParty_clicked()
{
	selectedEvent = BaseEvent::TYPE_CHANGEPARTY;
	accept();
}

void BrowseMapEvents::on_bChangeSkills_clicked()
{
	selectedEvent = BaseEvent::TYPE_CHANGESKILLS;
	accept();
}

void BrowseMapEvents::on_bChangeSystemBGM_clicked()
{
	selectedEvent = BaseEvent::TYPE_CHANGESYSTEMBGM;
	accept();
}

void BrowseMapEvents::on_bChangeSystemGraphic_clicked()
{
	selectedEvent = BaseEvent::TYPE_CHANGESYSTEMGRAPHICS;
	accept();
}

void BrowseMapEvents::on_bChangeSystemSoundEffect_clicked()
{
	selectedEvent = BaseEvent::TYPE_CHANGESYSTEMSOUNDEFFECT;
	accept();
}

void BrowseMapEvents::on_bChangeVehicleGraphic_clicked()
{
	selectedEvent = BaseEvent::TYPE_CHANGEVEHICLEGRAPHIC;
	accept();
}

void BrowseMapEvents::on_bCompleteHealing_clicked()
{
	selectedEvent = BaseEvent::TYPE_COMPLETEHEALING;
	accept();
}

void BrowseMapEvents::on_bConditionalBranch_clicked()
{
	selectedEvent = BaseEvent::TYPE_CONDITIONALBRANCH_MAP;
	accept();
}

void BrowseMapEvents::on_bDisplayMessage_clicked()
{
	selectedEvent = BaseEvent::TYPE_DISPLAYMESSAGE;
	accept();
}

void BrowseMapEvents::on_bEndEventProcessing_clicked()
{
	selectedEvent = BaseEvent::TYPE_ENDEVENTPROCESSING;
	accept();
}

void BrowseMapEvents::on_bEnemyEncounter_clicked()
{
	selectedEvent = BaseEvent::TYPE_ENEMYENCOUNTER;
	accept();
}

void BrowseMapEvents::on_bEraseEvent_clicked()
{
	selectedEvent = BaseEvent::TYPE_ERASEEVENT;
	accept();
}

void BrowseMapEvents::on_bFadeBGM_clicked()
{
	selectedEvent = BaseEvent::TYPE_FADEBGM;
	accept();
}

void BrowseMapEvents::on_bFlashScreen_clicked()
{
	selectedEvent = BaseEvent::TYPE_FLASHSCREEN;
	accept();
}

void BrowseMapEvents::on_bFlashSprite_clicked()
{
	selectedEvent = BaseEvent::TYPE_FLASHSPRITE_MAP;
	accept();
}

void BrowseMapEvents::on_bGameOver_clicked()
{
	selectedEvent = BaseEvent::TYPE_GAMEOVER;
	accept();
}

void BrowseMapEvents::on_bHaltAllMovement_clicked()
{
	selectedEvent = BaseEvent::TYPE_HALTALLMOVEMENT;
	accept();
}

void BrowseMapEvents::on_bHeroSpriteTransparency_clicked()
{
	selectedEvent = BaseEvent::TYPE_HEROSPRITETRANSPARENCY;
	accept();
}

void BrowseMapEvents::on_bHideLayer_clicked()
{
	selectedEvent = BaseEvent::TYPE_HIDELAYER;
	accept();
}

void BrowseMapEvents::on_bHideScreenOverlay_clicked()
{
	selectedEvent = BaseEvent::TYPE_HIDESCREENOVERLAY;
	accept();
}

void BrowseMapEvents::on_bHideScreen_clicked()
{
	selectedEvent = BaseEvent::TYPE_HIDESCREEN;
	accept();
}

void BrowseMapEvents::on_bInsertComment_clicked()
{
	selectedEvent = BaseEvent::TYPE_INSERTCOMMENT;
	accept();
}

void BrowseMapEvents::on_bItemManagement_clicked()
{
	selectedEvent = BaseEvent::TYPE_ITEMMANAGEMENT;
	accept();
}

void BrowseMapEvents::on_bJumpToLabel_clicked()
{
	selectedEvent = BaseEvent::TYPE_JUMPTOLABEL;
	accept();
}

void BrowseMapEvents::on_bKeyInputProcessing_clicked()
{
	selectedEvent = BaseEvent::TYPE_KEYINPUTPROCESSING;
	accept();
}

void BrowseMapEvents::on_bLabel_clicked()
{
	selectedEvent = BaseEvent::TYPE_LABEL;
	accept();
}

void BrowseMapEvents::on_bLoadGame_clicked()
{
	selectedEvent = BaseEvent::TYPE_LOADGAME;
	accept();
}

void BrowseMapEvents::on_bLoop_clicked()
{
	selectedEvent = BaseEvent::TYPE_LOOP;
	accept();
}

void BrowseMapEvents::on_bMemorizeBGM_clicked()
{
	selectedEvent = BaseEvent::TYPE_MEMORIZEBGM;
	accept();
}

void BrowseMapEvents::on_bMemorizePosition_clicked()
{
	selectedEvent = BaseEvent::TYPE_MEMORIZEPOSITION;
	accept();
}

void BrowseMapEvents::on_bMoveEvent_clicked()
{
	selectedEvent = BaseEvent::TYPE_MOVEEVENT;
	accept();
}

void BrowseMapEvents::on_bOpenMenu_clicked()
{
	selectedEvent = BaseEvent::TYPE_OPENMENU;
	accept();
}

void BrowseMapEvents::on_bPanScreen_clicked()
{
	selectedEvent = BaseEvent::TYPE_PANSCREEN;
	accept();
}

void BrowseMapEvents::on_bPlayBGM_clicked()
{
	selectedEvent = BaseEvent::TYPE_PLAYBGM;
	accept();
}

void BrowseMapEvents::on_bPlayMemorizedBGM_clicked()
{
	selectedEvent = BaseEvent::TYPE_PLAYMEMORIZEDBGM;
	accept();
}

void BrowseMapEvents::on_bPlayMovie_clicked()
{
	selectedEvent = BaseEvent::TYPE_PLAYMOVIE;
	accept();
}

void BrowseMapEvents::on_bPlaySoundEffect_clicked()
{
	selectedEvent = BaseEvent::TYPE_PLAYSOUNDEFFECT;
	accept();
}

void BrowseMapEvents::on_bPlaySystemBGM_clicked()
{
	selectedEvent = BaseEvent::TYPE_PLAYSYSTEMBGM;
	accept();
}

void BrowseMapEvents::on_bPlaySystemSoundEffect_clicked()
{
	selectedEvent = BaseEvent::TYPE_PLAYSYSTEMSOUNDEFFECT;
	accept();
}

void BrowseMapEvents::on_bProceedWithMovement_clicked()
{
	selectedEvent = BaseEvent::TYPE_PROCEEDWITHMOVEMENT;
	accept();
}

void BrowseMapEvents::on_bQuitGame_clicked()
{
	selectedEvent = BaseEvent::TYPE_QUITGAME;
	accept();
}

void BrowseMapEvents::on_bSaveGame_clicked()
{
	selectedEvent = BaseEvent::TYPE_SAVEGAME;
	accept();
}

void BrowseMapEvents::on_bSetScreenTransition_clicked()
{
	selectedEvent = BaseEvent::TYPE_SETSCREENTRANSITION;
	accept();
}

void BrowseMapEvents::on_bShakeScreen_clicked()
{
	selectedEvent = BaseEvent::TYPE_SHAKESCREEN;
	accept();
}

void BrowseMapEvents::on_bShowBattleAnimation_clicked()
{
	selectedEvent = BaseEvent::TYPE_SHOWBATTLEANIMATION_MAP;
	accept();
}

void BrowseMapEvents::on_bShowGlobalAnimation_clicked()
{
	selectedEvent = BaseEvent::TYPE_SHOWGLOBALANIMATION;
	accept();
}

void BrowseMapEvents::on_bShowLayer_clicked()
{
	selectedEvent = BaseEvent::TYPE_SHOWLAYER;
	accept();
}

void BrowseMapEvents::on_bShowPopupMenu_clicked()
{
	selectedEvent = BaseEvent::TYPE_SHOWPOPUPMENU;
	accept();
}

void BrowseMapEvents::on_bShowScreenOverlay_clicked()
{
	selectedEvent = BaseEvent::TYPE_SHOWSCREENOVERLAY;
	accept();
}

void BrowseMapEvents::on_bShowScreen_clicked()
{
	selectedEvent = BaseEvent::TYPE_SHOWSCREEN;
	accept();
}

void BrowseMapEvents::on_bStoreEventID_clicked()
{
	selectedEvent = BaseEvent::TYPE_STOREEVENTID;
	accept();
}

void BrowseMapEvents::on_bStoreTerrainID_clicked()
{
	selectedEvent = BaseEvent::TYPE_STORETERRAINID;
	accept();
}

void BrowseMapEvents::on_bStringOperation_clicked()
{
	selectedEvent = BaseEvent::TYPE_STRINGOPERATION;
	accept();
}

void BrowseMapEvents::on_bSwapTile_clicked()
{
	selectedEvent = BaseEvent::TYPE_SWAPTILE;
	accept();
}

void BrowseMapEvents::on_bSwitchOperation_clicked()
{
	selectedEvent = BaseEvent::TYPE_SWITCHOPERATION;
	accept();
}

void BrowseMapEvents::on_bTeleport_clicked()
{
	selectedEvent = BaseEvent::TYPE_TELEPORT;
	accept();
}

void BrowseMapEvents::on_bTileSubstitution_clicked()
{
	selectedEvent = BaseEvent::TYPE_TILESUBSTITUTION;
	accept();
}

void BrowseMapEvents::on_bTimerOperation_clicked()
{
	selectedEvent = BaseEvent::TYPE_TIMEROPERATION;
	accept();
}

void BrowseMapEvents::on_bTintScreen_clicked()
{
	selectedEvent = BaseEvent::TYPE_TINTSCREEN;
	accept();
}

void BrowseMapEvents::on_bTradeTwoEventLocations_clicked()
{
	selectedEvent = BaseEvent::TYPE_TRADETWOEVENTLOCATIONS;
	accept();
}

void BrowseMapEvents::on_bVariableMultiValueBranch_clicked()
{
	selectedEvent = BaseEvent::TYPE_VARIABLEMULTIVALUECONDITIONALBRANCH;
	accept();
}

void BrowseMapEvents::on_bVariableOperation_clicked()
{
	selectedEvent = BaseEvent::TYPE_VARIABLEOPERATION;
	accept();
}

void BrowseMapEvents::on_bWait_clicked()
{
	selectedEvent = BaseEvent::TYPE_WAIT;
	accept();
}

void BrowseMapEvents::on_bWeatherEffects_clicked()
{
	selectedEvent = BaseEvent::TYPE_WEATHEREFFECTS;
	accept();
}
