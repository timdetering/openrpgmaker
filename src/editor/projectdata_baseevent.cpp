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

#include <QFileInfo>
#include <QMessageBox>
#ifdef COMPILING_EDITOR
//#include "browsebattleevents.h"
//#include "browsecommonevents.h"
#include "browsemapevents.h"
#include "browsemenuevents.h"
#include "configureevent_allowdisallow.h"
#include "configureevent_callevent.h"
#include "configureevent_callmapevent.h"
#include "configureevent_changebattlecommands.h"
#include "configureevent_changebordertile.h"
#include "configureevent_changecharacterbasestats.h"
#include "configureevent_changecharactercondition.h"
#include "configureevent_changecharacterhp.h"
#include "configureevent_changecharacterlevel.h"
#include "configureevent_changecharactermp.h"
#include "configureevent_changecharactersprite.h"
#include "configureevent_changeclass.h"
#include "configureevent_changeencounterrate.h"
#include "configureevent_changeequippeditems.h"
#include "configureevent_changeeventlocation.h"
#include "configureevent_changeexperiencepoints.h"
#include "configureevent_changefacegraphic.h"
#include "configureevent_changeheroname.h"
#include "configureevent_changemenuimage.h"
#include "configureevent_changemenuitemlist.h"
#include "configureevent_changemenutext.h"
#include "configureevent_changemoney.h"
#include "configureevent_changeparallaxbackground.h"
#include "configureevent_changeparty.h"
#include "configureevent_changeselectedarea.h"
#include "configureevent_changeskills.h"
#include "configureevent_changesystembgm.h"
#include "configureevent_changesystemgraphic.h"
#include "configureevent_changesystemsoundeffect.h"
#include "configureevent_changevehiclegraphic.h"
#include "configureevent_completehealing.h"
#include "configureevent_displaymessage.h"
#include "configureevent_enabledisableobject.h"
#include "configureevent_enemyencounter.h"
#include "configureevent_fadebgm.h"
#include "configureevent_flashscreen.h"
#include "configureevent_flashsprite_map.h"
#include "configureevent_insertcomment.h"
#include "configureevent_itemmanagement.h"
#include "configureevent_jumptolabel.h"
#include "configureevent_keyinputprocessing.h"
#include "configureevent_label.h"
#include "configureevent_loadgame.h"
#include "configureevent_mapconditionalbranch.h"
#include "configureevent_memorizeposition.h"
#include "configureevent_menuconditionalbranch.h"
#include "configureevent_moveevent.h"
#include "configureevent_openmenu.h"
#include "configureevent_panscreen.h"
#include "configureevent_previousselectedareabranch.h"
#include "configureevent_savegame.h"
#include "configureevent_scrollarea.h"
#include "configureevent_setscreentransition.h"
#include "configureevent_shakescreen.h"
#include "configureevent_showbattleanimation_map.h"
#include "configureevent_showglobalanimation.h"
#include "configureevent_showhidelayer.h"
#include "configureevent_showpopupmenu.h"
#include "configureevent_storetargetid.h"
#include "configureevent_stringoperation.h"
#include "configureevent_swaptile.h"
#include "configureevent_switchoperation.h"
#include "configureevent_teleport.h"
#include "configureevent_tilesubstitution.h"
#include "configureevent_timeroperation.h"
#include "configureevent_tintscreen.h"
#include "configureevent_tradetwoeventlocations.h"
#include "configureevent_variablemultivalueconditionalbranch.h"
#include "configureevent_variableoperation.h"
#include "configureevent_wait.h"
#include "configureevent_weathereffects.h"
#include "getitemdialog.h"
#include "getitemdialog2.h"
#include "getnumberdialog.h"
#include "musicbrowser.h"
#include "selectscreentransition.h"
#include "soundeffectbrowser.h"
#endif // COMPILING_EDITOR
#include "projectdata.h"
#include "projectdata_baseevent.h"
#include "projectdata_mapevent.h"
#include "storagefile.h"
#include "tilechooser.h"
#include "xmlParser.h"
#include "xmlTools.h"

//BEGIN Map Event Types
QStringList BaseEvent::mapEventTypeNames = QStringList() \
	<< "topLevelEvent" \
	<< "allowDisallowEscape" \
	<< "allowDisallowMainMenu" \
	<< "allowDisallowSaving" \
	<< "allowDisallowTeleport" \
	<< "breakOutOfLoop" \
	<< "callCommonEvent" \
	<< "callMapEvent" \
	<< "changeBattleCommands" \
	<< "changeBorderTile" \
	<< "changeCharacterBaseStats" \
	<< "changeCharacterCondition" \
	<< "changeCharacterHP" \
	<< "changeCharacterLevel" \
	<< "changeCharacterMP" \
	<< "changeCharacterSprite" \
	<< "changeClass" \
	<< "changeEncounterRate" \
	<< "changeEquippedItems" \
	<< "changeEventLocation" \
	<< "changeExperiencePoints" \
	<< "changeFaceGraphic" \
	<< "changeHeroName" \
	<< "changeMoney" \
	<< "changeParallaxBackground" \
	<< "changeParty" \
	<< "changeSkills" \
	<< "changeSystemBGM" \
	<< "changeSystemGraphics" \
	<< "changeSystemSoundEffect" \
	<< "changeVehicleGraphic" \
	<< "completeHealing" \
	<< "conditionalBranch" \
	<< "displayMessage" \
	<< "endEventProcessing" \
	<< "enemyEncounter" \
	<< "eraseEvent" \
	<< "fadeBGM" \
	<< "flashScreen" \
	<< "flashSprite" \
	<< "gameOver" \
	<< "haltAllMovement" \
	<< "heroSpriteTransparency" \
	<< "hideLayer" \
	<< "hideScreen" \
	<< "hideScreenOverlay" \
	<< "insertComment" \
	<< "itemManagement" \
	<< "jumpToLabel" \
	<< "keyInputProcessing" \
	<< "label" \
	<< "loadGame" \
	<< "loop" \
	<< "memorizeBGM" \
	<< "memorizePosition" \
	<< "moveEvent" \
	<< "openMenu" \
	<< "panScreen" \
	<< "playBGM" \
	<< "playMemorizedBGM" \
	<< "playMovie" \
	<< "playSoundEffect" \
	<< "playSystemBGM" \
	<< "playSystemSoundEffect" \
	<< "proceedWithMovement" \
	<< "quitGame" \
	<< "saveGame" \
	<< "setScreenTransition" \
	<< "shakeScreen" \
	<< "showBattleAnimation" \
	<< "showGlobalAnimation" \
	<< "showLayer" \
	<< "showPopupMenu" \
	<< "showScreen" \
	<< "showScreenOverlay" \
	<< "storeEventID" \
	<< "storeTerrainID" \
	<< "stringOperation" \
	<< "swapTile" \
	<< "switchOperation" \
	<< "teleport" \
	<< "tileSubstitution" \
	<< "timerOperation" \
	<< "tintScreen" \
	<< "tradeTwoEventLocations" \
	<< "variableMultiValueConditionalBranch" \
	<< "variableOperation" \
	<< "wait" \
	<< "weatherEffects";
QList<int> BaseEvent::mapEventTypes = QList<int>() \
	<< BaseEvent::TYPE_TOPLEVELEVENT \
	<< BaseEvent::TYPE_ALLOWDISALLOWESCAPE \
	<< BaseEvent::TYPE_ALLOWDISALLOWMAINMENU \
	<< BaseEvent::TYPE_ALLOWDISALLOWSAVING \
	<< BaseEvent::TYPE_ALLOWDISALLOWTELEPORT \
	<< BaseEvent::TYPE_BREAKOUTOFLOOP \
	<< BaseEvent::TYPE_CALLCOMMONEVENT \
	<< BaseEvent::TYPE_CALLMAPEVENT \
	<< BaseEvent::TYPE_CHANGEBATTLECOMMANDS \
	<< BaseEvent::TYPE_CHANGEBORDERTILE \
	<< BaseEvent::TYPE_CHANGECHARACTERBASESTATS \
	<< BaseEvent::TYPE_CHANGECHARACTERCONDITION \
	<< BaseEvent::TYPE_CHANGECHARACTERHP \
	<< BaseEvent::TYPE_CHANGECHARACTERLEVEL \
	<< BaseEvent::TYPE_CHANGECHARACTERMP \
	<< BaseEvent::TYPE_CHANGECHARACTERSPRITE \
	<< BaseEvent::TYPE_CHANGECLASS \
	<< BaseEvent::TYPE_CHANGEENCOUNTERRATE \
	<< BaseEvent::TYPE_CHANGEEQUIPPEDITEMS \
	<< BaseEvent::TYPE_CHANGEEVENTLOCATION \
	<< BaseEvent::TYPE_CHANGEEXPERIENCEPOINTS \
	<< BaseEvent::TYPE_CHANGEFACEGRAPHIC \
	<< BaseEvent::TYPE_CHANGEHERONAME \
	<< BaseEvent::TYPE_CHANGEMONEY \
	<< BaseEvent::TYPE_CHANGEPARALLAXBACKGROUND \
	<< BaseEvent::TYPE_CHANGEPARTY \
	<< BaseEvent::TYPE_CHANGESKILLS \
	<< BaseEvent::TYPE_CHANGESYSTEMBGM \
	<< BaseEvent::TYPE_CHANGESYSTEMGRAPHICS \
	<< BaseEvent::TYPE_CHANGESYSTEMSOUNDEFFECT \
	<< BaseEvent::TYPE_CHANGEVEHICLEGRAPHIC \
	<< BaseEvent::TYPE_COMPLETEHEALING \
	<< BaseEvent::TYPE_CONDITIONALBRANCH_MAP \
	<< BaseEvent::TYPE_DISPLAYMESSAGE \
	<< BaseEvent::TYPE_ENDEVENTPROCESSING \
	<< BaseEvent::TYPE_ENEMYENCOUNTER \
	<< BaseEvent::TYPE_ERASEEVENT \
	<< BaseEvent::TYPE_FADEBGM \
	<< BaseEvent::TYPE_FLASHSCREEN \
	<< BaseEvent::TYPE_FLASHSPRITE_MAP \
	<< BaseEvent::TYPE_GAMEOVER \
	<< BaseEvent::TYPE_HALTALLMOVEMENT \
	<< BaseEvent::TYPE_HEROSPRITETRANSPARENCY \
	<< BaseEvent::TYPE_HIDELAYER \
	<< BaseEvent::TYPE_HIDESCREEN \
	<< BaseEvent::TYPE_HIDESCREENOVERLAY \
	<< BaseEvent::TYPE_INSERTCOMMENT \
	<< BaseEvent::TYPE_ITEMMANAGEMENT \
	<< BaseEvent::TYPE_JUMPTOLABEL \
	<< BaseEvent::TYPE_KEYINPUTPROCESSING \
	<< BaseEvent::TYPE_LABEL \
	<< BaseEvent::TYPE_LOADGAME \
	<< BaseEvent::TYPE_LOOP \
	<< BaseEvent::TYPE_MEMORIZEBGM \
	<< BaseEvent::TYPE_MEMORIZEPOSITION \
	<< BaseEvent::TYPE_MOVEEVENT \
	<< BaseEvent::TYPE_OPENMENU \
	<< BaseEvent::TYPE_PANSCREEN \
	<< BaseEvent::TYPE_PLAYBGM \
	<< BaseEvent::TYPE_PLAYMEMORIZEDBGM \
	<< BaseEvent::TYPE_PLAYMOVIE \
	<< BaseEvent::TYPE_PLAYSOUNDEFFECT \
	<< BaseEvent::TYPE_PLAYSYSTEMBGM \
	<< BaseEvent::TYPE_PLAYSYSTEMSOUNDEFFECT \
	<< BaseEvent::TYPE_PROCEEDWITHMOVEMENT \
	<< BaseEvent::TYPE_QUITGAME \
	<< BaseEvent::TYPE_SAVEGAME \
	<< BaseEvent::TYPE_SETSCREENTRANSITION \
	<< BaseEvent::TYPE_SHAKESCREEN \
	<< BaseEvent::TYPE_SHOWBATTLEANIMATION_MAP \
	<< BaseEvent::TYPE_SHOWGLOBALANIMATION \
	<< BaseEvent::TYPE_SHOWLAYER \
	<< BaseEvent::TYPE_SHOWPOPUPMENU \
	<< BaseEvent::TYPE_SHOWSCREEN \
	<< BaseEvent::TYPE_SHOWSCREENOVERLAY \
	<< BaseEvent::TYPE_STOREEVENTID \
	<< BaseEvent::TYPE_STORETERRAINID \
	<< BaseEvent::TYPE_STRINGOPERATION \
	<< BaseEvent::TYPE_SWAPTILE \
	<< BaseEvent::TYPE_SWITCHOPERATION \
	<< BaseEvent::TYPE_TELEPORT \
	<< BaseEvent::TYPE_TILESUBSTITUTION \
	<< BaseEvent::TYPE_TIMEROPERATION \
	<< BaseEvent::TYPE_TINTSCREEN \
	<< BaseEvent::TYPE_TRADETWOEVENTLOCATIONS \
	<< BaseEvent::TYPE_VARIABLEMULTIVALUECONDITIONALBRANCH \
	<< BaseEvent::TYPE_VARIABLEOPERATION \
	<< BaseEvent::TYPE_WAIT \
	<< BaseEvent::TYPE_WEATHEREFFECTS;
//END Map Event Types

//BEGIN Menu Event Types
QStringList BaseEvent::menuEventTypeNames = QStringList() \
	<< "topLevelEvent" \
	<< "breakOutOfLoop" \
	<< "callEvent" \
	<< "changeBattleCommands" \
	<< "changeCharacterBasestats" \
	<< "changeCharacterCondition" \
	<< "changeCharacterHp" \
	<< "changeCharacterLevel" \
	<< "changeCharacterMp" \
	<< "changeCharacterSprite" \
	<< "changeClass" \
	<< "changeEquippedItems" \
	<< "changeExperiencePoints" \
	<< "changeFaceGraphic" \
	<< "changeHeroName" \
	<< "changeImage" \
	<< "changeItemList" \
	<< "changeMoney" \
	<< "changeParty" \
	<< "changeSelectedArea" \
	<< "changeSkills" \
	<< "changeSystemBGM" \
	<< "changeSystemGraphics" \
	<< "changeSystemSoundEffect" \
	<< "changeText" \
	<< "changeVehicleGraphic" \
	<< "completeHealing" \
	<< "conditionalBranch" \
	<< "enableDisableObject" \
	<< "endEventProcessing" \
	<< "enemyEncounter" \
	<< "exitMenu" \
	<< "fadeBGM" \
	<< "gameOver" \
	<< "insertComment" \
	<< "itemManagement" \
	<< "jumpToLabel" \
	<< "keyInputProcessing" \
	<< "label" \
	<< "loadGame" \
	<< "loop" \
	<< "memorizeBGM" \
	<< "openMenu" \
	<< "playBGM" \
	<< "playMemorizedBGM" \
	<< "playMovie" \
	<< "playSoundEffect" \
	<< "playSystemBGM" \
	<< "playSystemSoundEffect" \
	<< "previousSelectedAreaBranch" \
	<< "quitGame" \
	<< "saveGame" \
	<< "scrollArea" \
	<< "setScreenTransition" \
	<< "showPopupMenu" \
	<< "stringOperation" \
	<< "switchOperation" \
	<< "teleport" \
	<< "timerOperation" \
	<< "variableMultiValueConditionalBranch" \
	<< "variableOperation" \
	<< "wait";
QList<int> BaseEvent::menuEventTypes = QList<int>() \
	<< BaseEvent::TYPE_TOPLEVELEVENT \
	<< BaseEvent::TYPE_BREAKOUTOFLOOP \
	<< BaseEvent::TYPE_CALLCOMMONEVENT \
	<< BaseEvent::TYPE_CHANGEBATTLECOMMANDS \
	<< BaseEvent::TYPE_CHANGECHARACTERBASESTATS \
	<< BaseEvent::TYPE_CHANGECHARACTERCONDITION \
	<< BaseEvent::TYPE_CHANGECHARACTERHP \
	<< BaseEvent::TYPE_CHANGECHARACTERLEVEL \
	<< BaseEvent::TYPE_CHANGECHARACTERMP \
	<< BaseEvent::TYPE_CHANGECHARACTERSPRITE \
	<< BaseEvent::TYPE_CHANGECLASS \
	<< BaseEvent::TYPE_CHANGEEQUIPPEDITEMS \
	<< BaseEvent::TYPE_CHANGEEXPERIENCEPOINTS \
	<< BaseEvent::TYPE_CHANGEFACEGRAPHIC \
	<< BaseEvent::TYPE_CHANGEHERONAME \
	<< BaseEvent::TYPE_CHANGEIMAGE \
	<< BaseEvent::TYPE_CHANGEITEMLIST \
	<< BaseEvent::TYPE_CHANGEMONEY \
	<< BaseEvent::TYPE_CHANGEPARTY \
	<< BaseEvent::TYPE_CHANGESELECTEDAREA \
	<< BaseEvent::TYPE_CHANGESKILLS \
	<< BaseEvent::TYPE_CHANGESYSTEMBGM \
	<< BaseEvent::TYPE_CHANGESYSTEMGRAPHICS \
	<< BaseEvent::TYPE_CHANGESYSTEMSOUNDEFFECT \
	<< BaseEvent::TYPE_CHANGETEXT \
	<< BaseEvent::TYPE_CHANGEVEHICLEGRAPHIC \
	<< BaseEvent::TYPE_COMPLETEHEALING \
	<< BaseEvent::TYPE_CONDITIONALBRANCH_MENU \
	<< BaseEvent::TYPE_ENABLEDISABLEOBJECT \
	<< BaseEvent::TYPE_ENDEVENTPROCESSING \
	<< BaseEvent::TYPE_ENEMYENCOUNTER \
	<< BaseEvent::TYPE_EXITMENU \
	<< BaseEvent::TYPE_FADEBGM \
	<< BaseEvent::TYPE_GAMEOVER \
	<< BaseEvent::TYPE_INSERTCOMMENT \
	<< BaseEvent::TYPE_ITEMMANAGEMENT \
	<< BaseEvent::TYPE_JUMPTOLABEL \
	<< BaseEvent::TYPE_KEYINPUTPROCESSING \
	<< BaseEvent::TYPE_LABEL \
	<< BaseEvent::TYPE_LOADGAME \
	<< BaseEvent::TYPE_LOOP \
	<< BaseEvent::TYPE_MEMORIZEBGM \
	<< BaseEvent::TYPE_OPENMENU \
	<< BaseEvent::TYPE_PLAYBGM \
	<< BaseEvent::TYPE_PLAYMEMORIZEDBGM \
	<< BaseEvent::TYPE_PLAYMOVIE \
	<< BaseEvent::TYPE_PLAYSOUNDEFFECT \
	<< BaseEvent::TYPE_PLAYSYSTEMBGM \
	<< BaseEvent::TYPE_PLAYSYSTEMSOUNDEFFECT \
	<< BaseEvent::TYPE_PREVIOUSSELECTEDAREABRANCH \
	<< BaseEvent::TYPE_QUITGAME \
	<< BaseEvent::TYPE_SAVEGAME \
	<< BaseEvent::TYPE_SCROLLAREA \
	<< BaseEvent::TYPE_SETSCREENTRANSITION \
	<< BaseEvent::TYPE_SHOWPOPUPMENU \
	<< BaseEvent::TYPE_STRINGOPERATION \
	<< BaseEvent::TYPE_SWITCHOPERATION \
	<< BaseEvent::TYPE_TELEPORT \
	<< BaseEvent::TYPE_TIMEROPERATION \
	<< BaseEvent::TYPE_VARIABLEMULTIVALUECONDITIONALBRANCH \
	<< BaseEvent::TYPE_VARIABLEOPERATION \
	<< BaseEvent::TYPE_WAIT;
//END Menu Event Types

//BEGIN Battle Event Types
QStringList BaseEvent::battleEventTypeNames = QStringList() \
	<< "topLevelEvent";
QList<int> BaseEvent::battleEventTypes = QList<int>() = QList<int>() \
	<< BaseEvent::TYPE_TOPLEVELEVENT;
//END Battle Event Types

//BEGIN Common Event Types
QStringList BaseEvent::commonEventTypeNames = QStringList() \
	<< "topLevelEvent";
QList<int> BaseEvent::commonEventTypes = QList<int>() \
	<< BaseEvent::TYPE_TOPLEVELEVENT;
//END Common Event Types

//BEGIN BaseEvent Member Functions
BaseEvent::BaseEvent(int type, int event)
{
	baseType = type;
	eventData = Event::newEvent(this, event);
}

BaseEvent::BaseEvent(int type, XMLNode EventNode)
{
	QString typeName = EventNode.getAttribute("type");
	int eventType = -1;
	baseType = type;
	if (baseType == MAPEVENT)
	{
		int index = mapEventTypeNames.indexOf(typeName);
		if (index != -1)
			eventType = mapEventTypes[index];
	}
	else if (baseType == MENUEVENT)
	{
		int index = menuEventTypeNames.indexOf(typeName);
		if (index != -1)
			eventType = menuEventTypes[index];
	}
	else if (baseType == BATTLEEVENT)
	{
		int index = battleEventTypeNames.indexOf(typeName);
		if (index != -1)
			eventType = battleEventTypes[index];
	}
	else if (baseType == COMMONEVENT)
	{
		int index = commonEventTypeNames.indexOf(typeName);
		if (index != -1)
			eventType = commonEventTypes[index];
	}
	if (eventType != -1)
		eventData = Event::newEvent(EventNode, eventType, this);
	else
		eventData = NULL;
}

BaseEvent::BaseEvent(int type, StorageFile &storageFile)
{
	baseType = type;
	eventData = Event::newEvent(storageFile, this);
}

BaseEvent::BaseEvent(BaseEvent *other)
{
	baseType = other->baseType;
	eventData = Event::newEvent(other->eventData, this);
}

BaseEvent::~BaseEvent()
{
	if (eventData != NULL)
	{
		delete eventData;
		eventData = NULL;
	}
}

XMLNode BaseEvent::getEventNode()
{
	XMLNode EventNode = eventData->getEventNode();
	if (baseType == MAPEVENT)
	{
		int index = mapEventTypes.indexOf(eventData->getEventType());
		if (index != -1)
			EventNode.updateAttribute(mapEventTypeNames[index].toUtf8().data(), NULL, "type");
	}
	else if (baseType == MENUEVENT)
	{
		int index = menuEventTypes.indexOf(eventData->getEventType());
		if (index != -1)
			EventNode.updateAttribute(menuEventTypeNames[index].toUtf8().data(), NULL, "type");
	}
	else if (baseType == BATTLEEVENT)
	{
		int index = battleEventTypes.indexOf(eventData->getEventType());
		if (index != -1)
			EventNode.updateAttribute(battleEventTypeNames[index].toUtf8().data(), NULL, "type");
	}
	else if (baseType == COMMONEVENT)
	{
		int index = commonEventTypes.indexOf(eventData->getEventType());
		if (index != -1)
			EventNode.updateAttribute(commonEventTypeNames[index].toUtf8().data(), NULL, "type");
	}
	return EventNode;
}

void BaseEvent::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(eventData->getEventType());
	eventData->saveToStorageFile(storageFile);
}

QList<BaseEvent*> BaseEvent::getEventPointers()
{
	QList<BaseEvent*> eventPointers;
	eventPointers.append(this);
	eventPointers.append(eventData->getEventPointers());
	return eventPointers;
}

QString BaseEvent::getEventTypeName()
{
	int eventType = eventData->getEventType();
	if (baseType == MAPEVENT)
	{
		int index = mapEventTypes.indexOf(eventType);
		if (index != -1)
			return mapEventTypeNames[index];
	}
	else if (baseType == MENUEVENT)
	{
		int index = menuEventTypes.indexOf(eventType);
		if (index != -1)
			return menuEventTypeNames[index];
	}
	else if (baseType == BATTLEEVENT)
	{
		int index = battleEventTypes.indexOf(eventType);
		if (index != -1)
			return battleEventTypeNames[index];
	}
	else if (baseType == COMMONEVENT)
	{
		int index = commonEventTypes.indexOf(eventType);
		if (index != -1)
			return commonEventTypeNames[index];
	}
	return "(UNKNOWN EVENT TYPE)";
}

#ifdef COMPILING_EDITOR
BaseEvent *BaseEvent::browseEvents(int type, int callerID, int callerType)
{
	BaseEvent *event = NULL;
	if (type == MAPEVENT)
	{
		BrowseMapEvents *browseEvents = new BrowseMapEvents;
		if (browseEvents->exec())
			event = new BaseEvent(type, browseEvents->getSelectedEvent());
		delete browseEvents;
	}
	else if (type == MENUEVENT)
	{
		BrowseMenuEvents *browseEvents = new BrowseMenuEvents;
		if (browseEvents->exec())
			event = new BaseEvent(type, browseEvents->getSelectedEvent());
		delete browseEvents;
	}
	else if (type == BATTLEEVENT)
	{
		TODO("Uncomment when battle events are ready.");
		/*BrowseBattleEvents *browseEvents = new BrowseBattleEvents;
		if (browseEvents->exec())
			event = new BaseEvent(type, browseEvents->getSelectedEvent());
		delete browseEvents;*/
	}
	else if (type == COMMONEVENT)
	{
		TODO("Uncomment when common events are ready.");
		/*BrowseCommonEvents *browseCommonEvents = new BrowseCommonEvents;
		if (browseEvents->exec())
			event = new BaseEvent(type, browseEvents->getSelectedEvent());
		delete browseEvents;*/
	}
	if (event != NULL)
	{
		if (!event->configureEvent(callerID, callerType))
		{
			delete event;
			event = NULL;
		}
	}
	return event;
}
#endif

bool BaseEvent::tryLoad(XMLNode EventNode, int type)
{
	QString typeName;
	if (!XMLTools::attributeExists(EventNode, "type"))
		return false;
	if (type == MAPEVENT)
	{
		int index;
		if (!XMLTools::attributeStringValid(EventNode, "type", mapEventTypeNames))
			return false;
		index = mapEventTypeNames.indexOf(typeName);
		if (index != -1)
			return Event::tryLoad(EventNode, mapEventTypes[index], type);
	}
	else if (type == MENUEVENT)
	{
		int index;
		if (!XMLTools::attributeStringValid(EventNode, "type", menuEventTypeNames))
			return false;
		index = menuEventTypeNames.indexOf(typeName);
		if (index != -1)
			return Event::tryLoad(EventNode, menuEventTypes[index], type);
	}
	else if (type == BATTLEEVENT)
	{
		int index;
		if (!XMLTools::attributeStringValid(EventNode, "type", battleEventTypeNames))
			return false;
		index = battleEventTypeNames.indexOf(typeName);
		if (index != -1)
			return Event::tryLoad(EventNode, battleEventTypes[index], type);
	}
	else if (type == COMMONEVENT)
	{
		int index;
		if (!XMLTools::attributeStringValid(EventNode, "type", commonEventTypeNames))
			return false;
		index = commonEventTypeNames.indexOf(typeName);
		if (index != -1)
			return Event::tryLoad(EventNode, commonEventTypes[index], type);
	}
	return true;
}
//END BaseEvent Member Functions

//BEGIN BaseEvent::Event Member Functions
BaseEvent::Event *BaseEvent::Event::newEvent(BaseEvent *pBase, int type)
{
	switch (type)
	{
		case TYPE_TOPLEVELEVENT:
			return new TopLevelEvent(pBase);
		case TYPE_ALLOWDISALLOWESCAPE:
			return new AllowDisallowEscape(pBase);
		case TYPE_ALLOWDISALLOWMAINMENU:
			return new AllowDisallowMainMenu(pBase);
		case TYPE_ALLOWDISALLOWSAVING:
			return new AllowDisallowSaving(pBase);
		case TYPE_ALLOWDISALLOWTELEPORT:
			return new AllowDisallowTeleport(pBase);
		case TYPE_BREAKOUTOFLOOP:
			return new BreakOutOfLoop(pBase);
		case TYPE_CALLCOMMONEVENT:
			return new CallCommonEvent(pBase);
		case TYPE_CALLMAPEVENT:
			return new CallMapEvent(pBase);
		case TYPE_CHANGEBATTLECOMMANDS:
			return new ChangeBattleCommands(pBase);
		case TYPE_CHANGEBORDERTILE:
			return new ChangeBorderTile(pBase);
		case TYPE_CHANGECHARACTERBASESTATS:
			return new ChangeCharacterBaseStats(pBase);
		case TYPE_CHANGECHARACTERCONDITION:
			return new ChangeCharacterCondition(pBase);
		case TYPE_CHANGECHARACTERHP:
			return new ChangeCharacterHP(pBase);
		case TYPE_CHANGECHARACTERLEVEL:
			return new ChangeCharacterLevel(pBase);
		case TYPE_CHANGECHARACTERMP:
			return new ChangeCharacterMP(pBase);
		case TYPE_CHANGECHARACTERSPRITE:
			return new ChangeCharacterSprite(pBase);
		case TYPE_CHANGECLASS:
			return new ChangeClass(pBase);
		case TYPE_CHANGEENCOUNTERRATE:
			return new ChangeEncounterRate(pBase);
		case TYPE_CHANGEEQUIPPEDITEMS:
			return new ChangeEquippedItems(pBase);
		case TYPE_CHANGEEVENTLOCATION:
			return new ChangeEventLocation(pBase);
		case TYPE_CHANGEEXPERIENCEPOINTS:
			return new ChangeExperiencePoints(pBase);
		case TYPE_CHANGEFACEGRAPHIC:
			return new ChangeFaceGraphic(pBase);
		case TYPE_CHANGEHERONAME:
			return new ChangeHeroName(pBase);
		case TYPE_CHANGEIMAGE:
			return new ChangeImage(pBase);
		case TYPE_CHANGEITEMLIST:
			return new ChangeItemList(pBase);
		case TYPE_CHANGEMONEY:
			return new ChangeMoney(pBase);
		case TYPE_CHANGEPARALLAXBACKGROUND:
			return new ChangeParallaxBackground(pBase);
		case TYPE_CHANGEPARTY:
			return new ChangeParty(pBase);
		case TYPE_CHANGESELECTEDAREA:
			return new ChangeSelectedArea(pBase);
		case TYPE_CHANGESKILLS:
			return new ChangeSkills(pBase);
		case TYPE_CHANGESYSTEMBGM:
			return new ChangeSystemBGM(pBase);
		case TYPE_CHANGESYSTEMGRAPHICS:
			return new ChangeSystemGraphics(pBase);
		case TYPE_CHANGESYSTEMSOUNDEFFECT:
			return new ChangeSystemSoundEffect(pBase);
		case TYPE_CHANGETEXT:
			return new ChangeText(pBase);
		case TYPE_CHANGEVEHICLEGRAPHIC:
			return new ChangeVehicleGraphic(pBase);
		case TYPE_COMPLETEHEALING:
			return new CompleteHealing(pBase);
		case TYPE_CONDITIONALBRANCH_MAP:
			return new ConditionalBranch_Map(pBase);
		case TYPE_CONDITIONALBRANCH_MENU:
			return new ConditionalBranch_Menu(pBase);
		case TYPE_DISPLAYMESSAGE:
			return new DisplayMessage(pBase);
		case TYPE_ENABLEDISABLEOBJECT:
			return new EnableDisableObject(pBase);
		case TYPE_ENDEVENTPROCESSING:
			return new EndEventProcessing(pBase);
		case TYPE_ENEMYENCOUNTER:
			return new EnemyEncounter(pBase);
		case TYPE_ERASEEVENT:
			return new EraseEvent(pBase);
		case TYPE_EXITMENU:
			return new ExitMenu(pBase);
		case TYPE_FADEBGM:
			return new FadeBGM(pBase);
		case TYPE_FLASHSCREEN:
			return new FlashScreen(pBase);
		case TYPE_FLASHSPRITE_MAP:
			return new FlashSprite_Map(pBase);
		case TYPE_GAMEOVER:
			return new GameOver(pBase);
		case TYPE_HALTALLMOVEMENT:
			return new HaltAllMovement(pBase);
		case TYPE_HEROSPRITETRANSPARENCY:
			return new HeroSpriteTransparency(pBase);
		case TYPE_HIDELAYER:
			return new HideLayer(pBase);
		case TYPE_HIDESCREEN:
			return new HideScreen(pBase);
		case TYPE_HIDESCREENOVERLAY:
			return new HideScreenOverlay(pBase);
		case TYPE_INSERTCOMMENT:
			return new InsertComment(pBase);
		case TYPE_ITEMMANAGEMENT:
			return new ItemManagement(pBase);
		case TYPE_JUMPTOLABEL:
			return new JumpToLabel(pBase);
		case TYPE_KEYINPUTPROCESSING:
			return new KeyInputProcessing(pBase);
		case TYPE_LABEL:
			return new Label(pBase);
		case TYPE_LOADGAME:
			return new LoadGame(pBase);
		case TYPE_LOOP:
			return new Loop(pBase);
		case TYPE_MEMORIZEBGM:
			return new MemorizeBGM(pBase);
		case TYPE_MEMORIZEPOSITION:
			return new MemorizePosition(pBase);
		case TYPE_MOVEEVENT:
			return new MoveEvent(pBase);
		case TYPE_OPENMENU:
			return new OpenMenu(pBase);
		case TYPE_PANSCREEN:
			return new PanScreen(pBase);
		case TYPE_PLAYBGM:
			return new PlayBGM(pBase);
		case TYPE_PLAYMEMORIZEDBGM:
			return new PlayMemorizedBGM(pBase);
		case TYPE_PLAYMOVIE:
			return new PlayMovie(pBase);
		case TYPE_PLAYSOUNDEFFECT:
			return new PlaySoundEffect(pBase);
		case TYPE_PLAYSYSTEMBGM:
			return new PlaySystemBGM(pBase);
		case TYPE_PLAYSYSTEMSOUNDEFFECT:
			return new PlaySystemSoundEffect(pBase);
		case TYPE_PREVIOUSSELECTEDAREABRANCH:
			return new PreviousSelectedAreaBranch(pBase);
		case TYPE_PROCEEDWITHMOVEMENT:
			return new ProceedWithMovement(pBase);
		case TYPE_QUITGAME:
			return new QuitGame(pBase);
		case TYPE_SAVEGAME:
			return new SaveGame(pBase);
		case TYPE_SCROLLAREA:
			return new ScrollArea(pBase);
		case TYPE_SETSCREENTRANSITION:
			return new SetScreenTransition(pBase);
		case TYPE_SHAKESCREEN:
			return new ShakeScreen(pBase);
		case TYPE_SHOWBATTLEANIMATION_MAP:
			return new ShowBattleAnimation_Map(pBase);
		case TYPE_SHOWGLOBALANIMATION:
			return new ShowGlobalAnimation(pBase);
		case TYPE_SHOWLAYER:
			return new ShowLayer(pBase);
		case TYPE_SHOWPOPUPMENU:
			return new ShowPopupMenu(pBase);
		case TYPE_SHOWSCREEN:
			return new ShowScreen(pBase);
		case TYPE_SHOWSCREENOVERLAY:
			return new ShowScreenOverlay(pBase);
		case TYPE_STOREEVENTID:
			return new StoreEventID(pBase);
		case TYPE_STORETERRAINID:
			return new StoreTerrainID(pBase);
		case TYPE_STRINGOPERATION:
			return new StringOperation(pBase);
		case TYPE_SWAPTILE:
			return new SwapTile(pBase);
		case TYPE_SWITCHOPERATION:
			return new SwitchOperation(pBase);
		case TYPE_TELEPORT:
			return new Teleport(pBase);
		case TYPE_TILESUBSTITUTION:
			return new TileSubstitution(pBase);
		case TYPE_TIMEROPERATION:
			return new TimerOperation(pBase);
		case TYPE_TINTSCREEN:
			return new TintScreen(pBase);
		case TYPE_TRADETWOEVENTLOCATIONS:
			return new TradeTwoEventLocations(pBase);
		case TYPE_VARIABLEMULTIVALUECONDITIONALBRANCH:
			return new VariableMultiValueConditionalBranch(pBase);
		case TYPE_VARIABLEOPERATION:
			return new VariableOperation(pBase);
		case TYPE_WAIT:
			return new Wait(pBase);
		case TYPE_WEATHEREFFECTS:
			return new WeatherEffects(pBase);
	}
	return NULL;
}

BaseEvent::Event *BaseEvent::Event::newEvent(XMLNode EventNode, int type, BaseEvent *pBase)
{
	switch (type)
	{
		case TYPE_TOPLEVELEVENT:
			return new TopLevelEvent(EventNode, pBase);
		case TYPE_ALLOWDISALLOWESCAPE:
			return new AllowDisallowEscape(EventNode, pBase);
		case TYPE_ALLOWDISALLOWMAINMENU:
			return new AllowDisallowMainMenu(EventNode, pBase);
		case TYPE_ALLOWDISALLOWSAVING:
			return new AllowDisallowSaving(EventNode, pBase);
		case TYPE_ALLOWDISALLOWTELEPORT:
			return new AllowDisallowTeleport(EventNode, pBase);
		case TYPE_BREAKOUTOFLOOP:
			return new BreakOutOfLoop(EventNode, pBase);
		case TYPE_CALLCOMMONEVENT:
			return new CallCommonEvent(EventNode, pBase);
		case TYPE_CALLMAPEVENT:
			return new CallMapEvent(EventNode, pBase);
		case TYPE_CHANGEBATTLECOMMANDS:
			return new ChangeBattleCommands(EventNode, pBase);
		case TYPE_CHANGEBORDERTILE:
			return new ChangeBorderTile(EventNode, pBase);
		case TYPE_CHANGECHARACTERBASESTATS:
			return new ChangeCharacterBaseStats(EventNode, pBase);
		case TYPE_CHANGECHARACTERCONDITION:
			return new ChangeCharacterCondition(EventNode, pBase);
		case TYPE_CHANGECHARACTERHP:
			return new ChangeCharacterHP(EventNode, pBase);
		case TYPE_CHANGECHARACTERLEVEL:
			return new ChangeCharacterLevel(EventNode, pBase);
		case TYPE_CHANGECHARACTERMP:
			return new ChangeCharacterMP(EventNode, pBase);
		case TYPE_CHANGECHARACTERSPRITE:
			return new ChangeCharacterSprite(EventNode, pBase);
		case TYPE_CHANGECLASS:
			return new ChangeClass(EventNode, pBase);
		case TYPE_CHANGEENCOUNTERRATE:
			return new ChangeEncounterRate(EventNode, pBase);
		case TYPE_CHANGEEQUIPPEDITEMS:
			return new ChangeEquippedItems(EventNode, pBase);
		case TYPE_CHANGEEVENTLOCATION:
			return new ChangeEventLocation(EventNode, pBase);
		case TYPE_CHANGEEXPERIENCEPOINTS:
			return new ChangeExperiencePoints(EventNode, pBase);
		case TYPE_CHANGEFACEGRAPHIC:
			return new ChangeFaceGraphic(EventNode, pBase);
		case TYPE_CHANGEHERONAME:
			return new ChangeHeroName(EventNode, pBase);
		case TYPE_CHANGEIMAGE:
			return new ChangeImage(EventNode, pBase);
		case TYPE_CHANGEITEMLIST:
			return new ChangeItemList(EventNode, pBase);
		case TYPE_CHANGEMONEY:
			return new ChangeMoney(EventNode, pBase);
		case TYPE_CHANGEPARALLAXBACKGROUND:
			return new ChangeParallaxBackground(EventNode, pBase);
		case TYPE_CHANGEPARTY:
			return new ChangeParty(EventNode, pBase);
		case TYPE_CHANGESELECTEDAREA:
			return new ChangeSelectedArea(EventNode, pBase);
		case TYPE_CHANGESKILLS:
			return new ChangeSkills(EventNode, pBase);
		case TYPE_CHANGESYSTEMBGM:
			return new ChangeSystemBGM(EventNode, pBase);
		case TYPE_CHANGESYSTEMGRAPHICS:
			return new ChangeSystemGraphics(EventNode, pBase);
		case TYPE_CHANGESYSTEMSOUNDEFFECT:
			return new ChangeSystemSoundEffect(EventNode, pBase);
		case TYPE_CHANGETEXT:
			return new ChangeText(EventNode, pBase);
		case TYPE_CHANGEVEHICLEGRAPHIC:
			return new ChangeVehicleGraphic(EventNode, pBase);
		case TYPE_COMPLETEHEALING:
			return new CompleteHealing(EventNode, pBase);
		case TYPE_CONDITIONALBRANCH_MAP:
			return new ConditionalBranch_Map(EventNode, pBase);
		case TYPE_CONDITIONALBRANCH_MENU:
			return new ConditionalBranch_Menu(EventNode, pBase);
		case TYPE_DISPLAYMESSAGE:
			return new DisplayMessage(EventNode, pBase);
		case TYPE_ENABLEDISABLEOBJECT:
			return new EnableDisableObject(EventNode, pBase);
		case TYPE_ENDEVENTPROCESSING:
			return new EndEventProcessing(EventNode, pBase);
		case TYPE_ENEMYENCOUNTER:
			return new EnemyEncounter(EventNode, pBase);
		case TYPE_ERASEEVENT:
			return new EraseEvent(EventNode, pBase);
		case TYPE_EXITMENU:
			return new ExitMenu(EventNode, pBase);
		case TYPE_FADEBGM:
			return new FadeBGM(EventNode, pBase);
		case TYPE_FLASHSCREEN:
			return new FlashScreen(EventNode, pBase);
		case TYPE_FLASHSPRITE_MAP:
			return new FlashSprite_Map(EventNode, pBase);
		case TYPE_GAMEOVER:
			return new GameOver(EventNode, pBase);
		case TYPE_HALTALLMOVEMENT:
			return new HaltAllMovement(EventNode, pBase);
		case TYPE_HEROSPRITETRANSPARENCY:
			return new HeroSpriteTransparency(EventNode, pBase);
		case TYPE_HIDELAYER:
			return new HideLayer(EventNode, pBase);
		case TYPE_HIDESCREEN:
			return new HideScreen(EventNode, pBase);
		case TYPE_HIDESCREENOVERLAY:
			return new HideScreenOverlay(EventNode, pBase);
		case TYPE_INSERTCOMMENT:
			return new InsertComment(EventNode, pBase);
		case TYPE_ITEMMANAGEMENT:
			return new ItemManagement(EventNode, pBase);
		case TYPE_JUMPTOLABEL:
			return new JumpToLabel(EventNode, pBase);
		case TYPE_KEYINPUTPROCESSING:
			return new KeyInputProcessing(EventNode, pBase);
		case TYPE_LABEL:
			return new Label(EventNode, pBase);
		case TYPE_LOADGAME:
			return new LoadGame(EventNode, pBase);
		case TYPE_LOOP:
			return new Loop(EventNode, pBase);
		case TYPE_MEMORIZEBGM:
			return new MemorizeBGM(EventNode, pBase);
		case TYPE_MEMORIZEPOSITION:
			return new MemorizePosition(EventNode, pBase);
		case TYPE_MOVEEVENT:
			return new MoveEvent(EventNode, pBase);
		case TYPE_OPENMENU:
			return new OpenMenu(EventNode, pBase);
		case TYPE_PANSCREEN:
			return new PanScreen(EventNode, pBase);
		case TYPE_PLAYBGM:
			return new PlayBGM(EventNode, pBase);
		case TYPE_PLAYMEMORIZEDBGM:
			return new PlayMemorizedBGM(EventNode, pBase);
		case TYPE_PLAYMOVIE:
			return new PlayMovie(EventNode, pBase);
		case TYPE_PLAYSOUNDEFFECT:
			return new PlaySoundEffect(EventNode, pBase);
		case TYPE_PLAYSYSTEMBGM:
			return new PlaySystemBGM(EventNode, pBase);
		case TYPE_PLAYSYSTEMSOUNDEFFECT:
			return new PlaySystemSoundEffect(EventNode, pBase);
		case TYPE_PREVIOUSSELECTEDAREABRANCH:
			return new PreviousSelectedAreaBranch(EventNode, pBase);
		case TYPE_PROCEEDWITHMOVEMENT:
			return new ProceedWithMovement(EventNode, pBase);
		case TYPE_QUITGAME:
			return new QuitGame(EventNode, pBase);
		case TYPE_SAVEGAME:
			return new SaveGame(EventNode, pBase);
		case TYPE_SCROLLAREA:
			return new ScrollArea(EventNode, pBase);
		case TYPE_SETSCREENTRANSITION:
			return new SetScreenTransition(EventNode, pBase);
		case TYPE_SHAKESCREEN:
			return new ShakeScreen(EventNode, pBase);
		case TYPE_SHOWBATTLEANIMATION_MAP:
			return new ShowBattleAnimation_Map(EventNode, pBase);
		case TYPE_SHOWGLOBALANIMATION:
			return new ShowGlobalAnimation(EventNode, pBase);
		case TYPE_SHOWPOPUPMENU:
			return new ShowPopupMenu(EventNode, pBase);
		case TYPE_SHOWLAYER:
			return new ShowLayer(EventNode, pBase);
		case TYPE_SHOWSCREEN:
			return new ShowScreen(EventNode, pBase);
		case TYPE_SHOWSCREENOVERLAY:
			return new ShowScreenOverlay(EventNode, pBase);
		case TYPE_STOREEVENTID:
			return new StoreEventID(EventNode, pBase);
		case TYPE_STORETERRAINID:
			return new StoreTerrainID(EventNode, pBase);
		case TYPE_STRINGOPERATION:
			return new StringOperation(EventNode, pBase);
		case TYPE_SWAPTILE:
			return new SwapTile(EventNode, pBase);
		case TYPE_SWITCHOPERATION:
			return new SwitchOperation(EventNode, pBase);
		case TYPE_TELEPORT:
			return new Teleport(EventNode, pBase);
		case TYPE_TILESUBSTITUTION:
			return new TileSubstitution(EventNode, pBase);
		case TYPE_TIMEROPERATION:
			return new TimerOperation(EventNode, pBase);
		case TYPE_TINTSCREEN:
			return new TintScreen(EventNode, pBase);
		case TYPE_TRADETWOEVENTLOCATIONS:
			return new TradeTwoEventLocations(EventNode, pBase);
		case TYPE_VARIABLEMULTIVALUECONDITIONALBRANCH:
			return new VariableMultiValueConditionalBranch(EventNode, pBase);
		case TYPE_VARIABLEOPERATION:
			return new VariableOperation(EventNode, pBase);
		case TYPE_WAIT:
			return new Wait(EventNode, pBase);
		case TYPE_WEATHEREFFECTS:
			return new WeatherEffects(EventNode, pBase);
	}
	return NULL;
}

BaseEvent::Event *BaseEvent::Event::newEvent(StorageFile &storageFile, BaseEvent *pBase)
{
	int type = storageFile.getSignedInt();
	switch (type)
	{
		case TYPE_TOPLEVELEVENT:
			return new TopLevelEvent(storageFile, pBase);
		case TYPE_ALLOWDISALLOWESCAPE:
			return new AllowDisallowEscape(storageFile, pBase);
		case TYPE_ALLOWDISALLOWMAINMENU:
			return new AllowDisallowMainMenu(storageFile, pBase);
		case TYPE_ALLOWDISALLOWSAVING:
			return new AllowDisallowSaving(storageFile, pBase);
		case TYPE_ALLOWDISALLOWTELEPORT:
			return new AllowDisallowTeleport(storageFile, pBase);
		case TYPE_BREAKOUTOFLOOP:
			return new BreakOutOfLoop(storageFile, pBase);
		case TYPE_CALLCOMMONEVENT:
			return new CallCommonEvent(storageFile, pBase);
		case TYPE_CALLMAPEVENT:
			return new CallMapEvent(storageFile, pBase);
		case TYPE_CHANGEBATTLECOMMANDS:
			return new ChangeBattleCommands(storageFile, pBase);
		case TYPE_CHANGEBORDERTILE:
			return new ChangeBorderTile(storageFile, pBase);
		case TYPE_CHANGECHARACTERBASESTATS:
			return new ChangeCharacterBaseStats(storageFile, pBase);
		case TYPE_CHANGECHARACTERCONDITION:
			return new ChangeCharacterCondition(storageFile, pBase);
		case TYPE_CHANGECHARACTERHP:
			return new ChangeCharacterHP(storageFile, pBase);
		case TYPE_CHANGECHARACTERLEVEL:
			return new ChangeCharacterLevel(storageFile, pBase);
		case TYPE_CHANGECHARACTERMP:
			return new ChangeCharacterMP(storageFile, pBase);
		case TYPE_CHANGECHARACTERSPRITE:
			return new ChangeCharacterSprite(storageFile, pBase);
		case TYPE_CHANGECLASS:
			return new ChangeClass(storageFile, pBase);
		case TYPE_CHANGEENCOUNTERRATE:
			return new ChangeEncounterRate(storageFile, pBase);
		case TYPE_CHANGEEQUIPPEDITEMS:
			return new ChangeEquippedItems(storageFile, pBase);
		case TYPE_CHANGEEVENTLOCATION:
			return new ChangeEventLocation(storageFile, pBase);
		case TYPE_CHANGEEXPERIENCEPOINTS:
			return new ChangeExperiencePoints(storageFile, pBase);
		case TYPE_CHANGEFACEGRAPHIC:
			return new ChangeFaceGraphic(storageFile, pBase);
		case TYPE_CHANGEHERONAME:
			return new ChangeHeroName(storageFile, pBase);
		case TYPE_CHANGEIMAGE:
			return new ChangeImage(storageFile, pBase);
		case TYPE_CHANGEITEMLIST:
			return new ChangeItemList(storageFile, pBase);
		case TYPE_CHANGEMONEY:
			return new ChangeMoney(storageFile, pBase);
		case TYPE_CHANGEPARALLAXBACKGROUND:
			return new ChangeParallaxBackground(storageFile, pBase);
		case TYPE_CHANGEPARTY:
			return new ChangeParty(storageFile, pBase);
		case TYPE_CHANGESELECTEDAREA:
			return new ChangeSelectedArea(storageFile, pBase);
		case TYPE_CHANGESKILLS:
			return new ChangeSkills(storageFile, pBase);
		case TYPE_CHANGESYSTEMBGM:
			return new ChangeSystemBGM(storageFile, pBase);
		case TYPE_CHANGESYSTEMGRAPHICS:
			return new ChangeSystemGraphics(storageFile, pBase);
		case TYPE_CHANGESYSTEMSOUNDEFFECT:
			return new ChangeSystemSoundEffect(storageFile, pBase);
		case TYPE_CHANGETEXT:
			return new ChangeText(storageFile, pBase);
		case TYPE_CHANGEVEHICLEGRAPHIC:
			return new ChangeVehicleGraphic(storageFile, pBase);
		case TYPE_COMPLETEHEALING:
			return new CompleteHealing(storageFile, pBase);
		case TYPE_CONDITIONALBRANCH_MAP:
			return new ConditionalBranch_Map(storageFile, pBase);
		case TYPE_CONDITIONALBRANCH_MENU:
			return new ConditionalBranch_Menu(storageFile, pBase);
		case TYPE_DISPLAYMESSAGE:
			return new DisplayMessage(storageFile, pBase);
		case TYPE_ENABLEDISABLEOBJECT:
			return new EnableDisableObject(storageFile, pBase);
		case TYPE_ENDEVENTPROCESSING:
			return new EndEventProcessing(storageFile, pBase);
		case TYPE_ENEMYENCOUNTER:
			return new EnemyEncounter(storageFile, pBase);
		case TYPE_ERASEEVENT:
			return new EraseEvent(storageFile, pBase);
		case TYPE_EXITMENU:
			return new ExitMenu(storageFile, pBase);
		case TYPE_FADEBGM:
			return new FadeBGM(storageFile, pBase);
		case TYPE_FLASHSCREEN:
			return new FlashScreen(storageFile, pBase);
		case TYPE_FLASHSPRITE_MAP:
			return new FlashSprite_Map(storageFile, pBase);
		case TYPE_GAMEOVER:
			return new GameOver(storageFile, pBase);
		case TYPE_HALTALLMOVEMENT:
			return new HaltAllMovement(storageFile, pBase);
		case TYPE_HEROSPRITETRANSPARENCY:
			return new HeroSpriteTransparency(storageFile, pBase);
		case TYPE_HIDELAYER:
			return new HideLayer(storageFile, pBase);
		case TYPE_HIDESCREEN:
			return new HideScreen(storageFile, pBase);
		case TYPE_HIDESCREENOVERLAY:
			return new HideScreenOverlay(storageFile, pBase);
		case TYPE_INSERTCOMMENT:
			return new InsertComment(storageFile, pBase);
		case TYPE_ITEMMANAGEMENT:
			return new ItemManagement(storageFile, pBase);
		case TYPE_JUMPTOLABEL:
			return new JumpToLabel(storageFile, pBase);
		case TYPE_KEYINPUTPROCESSING:
			return new KeyInputProcessing(storageFile, pBase);
		case TYPE_LABEL:
			return new Label(storageFile, pBase);
		case TYPE_LOADGAME:
			return new LoadGame(storageFile, pBase);
		case TYPE_LOOP:
			return new Loop(storageFile, pBase);
		case TYPE_MEMORIZEBGM:
			return new MemorizeBGM(storageFile, pBase);
		case TYPE_MEMORIZEPOSITION:
			return new MemorizePosition(storageFile, pBase);
		case TYPE_MOVEEVENT:
			return new MoveEvent(storageFile, pBase);
		case TYPE_OPENMENU:
			return new OpenMenu(storageFile, pBase);
		case TYPE_PANSCREEN:
			return new PanScreen(storageFile, pBase);
		case TYPE_PLAYBGM:
			return new PlayBGM(storageFile, pBase);
		case TYPE_PLAYMEMORIZEDBGM:
			return new PlayMemorizedBGM(storageFile, pBase);
		case TYPE_PLAYMOVIE:
			return new PlayMovie(storageFile, pBase);
		case TYPE_PLAYSOUNDEFFECT:
			return new PlaySoundEffect(storageFile, pBase);
		case TYPE_PLAYSYSTEMBGM:
			return new PlaySystemBGM(storageFile, pBase);
		case TYPE_PLAYSYSTEMSOUNDEFFECT:
			return new PlaySystemSoundEffect(storageFile, pBase);
		case TYPE_PREVIOUSSELECTEDAREABRANCH:
			return new PreviousSelectedAreaBranch(storageFile, pBase);
		case TYPE_PROCEEDWITHMOVEMENT:
			return new ProceedWithMovement(storageFile, pBase);
		case TYPE_QUITGAME:
			return new QuitGame(storageFile, pBase);
		case TYPE_SAVEGAME:
			return new SaveGame(storageFile, pBase);
		case TYPE_SCROLLAREA:
			return new ScrollArea(storageFile, pBase);
		case TYPE_SETSCREENTRANSITION:
			return new SetScreenTransition(storageFile, pBase);
		case TYPE_SHAKESCREEN:
			return new ShakeScreen(storageFile, pBase);
		case TYPE_SHOWBATTLEANIMATION_MAP:
			return new ShowBattleAnimation_Map(storageFile, pBase);
		case TYPE_SHOWGLOBALANIMATION:
			return new ShowGlobalAnimation(storageFile, pBase);
		case TYPE_SHOWLAYER:
			return new ShowLayer(storageFile, pBase);
		case TYPE_SHOWPOPUPMENU:
			return new ShowPopupMenu(storageFile, pBase);
		case TYPE_SHOWSCREEN:
			return new ShowScreen(storageFile, pBase);
		case TYPE_SHOWSCREENOVERLAY:
			return new ShowScreenOverlay(storageFile, pBase);
		case TYPE_STOREEVENTID:
			return new StoreEventID(storageFile, pBase);
		case TYPE_STORETERRAINID:
			return new StoreTerrainID(storageFile, pBase);
		case TYPE_STRINGOPERATION:
			return new StringOperation(storageFile, pBase);
		case TYPE_SWAPTILE:
			return new SwapTile(storageFile, pBase);
		case TYPE_SWITCHOPERATION:
			return new SwitchOperation(storageFile, pBase);
		case TYPE_TELEPORT:
			return new Teleport(storageFile, pBase);
		case TYPE_TILESUBSTITUTION:
			return new TileSubstitution(storageFile, pBase);
		case TYPE_TIMEROPERATION:
			return new TimerOperation(storageFile, pBase);
		case TYPE_TINTSCREEN:
			return new TintScreen(storageFile, pBase);
		case TYPE_TRADETWOEVENTLOCATIONS:
			return new TradeTwoEventLocations(storageFile, pBase);
		case TYPE_VARIABLEMULTIVALUECONDITIONALBRANCH:
			return new VariableMultiValueConditionalBranch(storageFile, pBase);
		case TYPE_VARIABLEOPERATION:
			return new VariableOperation(storageFile, pBase);
		case TYPE_WAIT:
			return new Wait(storageFile, pBase);
		case TYPE_WEATHEREFFECTS:
			return new WeatherEffects(storageFile, pBase);
	}
	return NULL;
}

BaseEvent::Event *BaseEvent::Event::newEvent(Event *other, BaseEvent *pBase)
{
	int type = other->getEventType();
	switch (type)
	{
		case TYPE_TOPLEVELEVENT:
			return new TopLevelEvent((TopLevelEvent*)other, pBase);
		case TYPE_ALLOWDISALLOWESCAPE:
			return new AllowDisallowEscape((AllowDisallowEscape*)other, pBase);
		case TYPE_ALLOWDISALLOWMAINMENU:
			return new AllowDisallowMainMenu((AllowDisallowMainMenu*)other, pBase);
		case TYPE_ALLOWDISALLOWSAVING:
			return new AllowDisallowSaving((AllowDisallowSaving*)other, pBase);
		case TYPE_ALLOWDISALLOWTELEPORT:
			return new AllowDisallowTeleport((AllowDisallowTeleport*)other, pBase);
		case TYPE_BREAKOUTOFLOOP:
			return new BreakOutOfLoop((BreakOutOfLoop*)other, pBase);
		case TYPE_CALLCOMMONEVENT:
			return new CallCommonEvent((CallCommonEvent*)other, pBase);
		case TYPE_CALLMAPEVENT:
			return new CallMapEvent((CallMapEvent*)other, pBase);
		case TYPE_CHANGEBATTLECOMMANDS:
			return new ChangeBattleCommands((ChangeBattleCommands*)other, pBase);
		case TYPE_CHANGEBORDERTILE:
			return new ChangeBorderTile((ChangeBorderTile*)other, pBase);
		case TYPE_CHANGECHARACTERBASESTATS:
			return new ChangeCharacterBaseStats((ChangeCharacterBaseStats*)other, pBase);
		case TYPE_CHANGECHARACTERCONDITION:
			return new ChangeCharacterCondition((ChangeCharacterCondition*)other, pBase);
		case TYPE_CHANGECHARACTERHP:
			return new ChangeCharacterHP((ChangeCharacterHP*)other, pBase);
		case TYPE_CHANGECHARACTERLEVEL:
			return new ChangeCharacterLevel((ChangeCharacterLevel*)other, pBase);
		case TYPE_CHANGECHARACTERMP:
			return new ChangeCharacterMP((ChangeCharacterMP*)other, pBase);
		case TYPE_CHANGECHARACTERSPRITE:
			return new ChangeCharacterSprite((ChangeCharacterSprite*)other, pBase);
		case TYPE_CHANGECLASS:
			return new ChangeClass((ChangeClass*)other, pBase);
		case TYPE_CHANGEENCOUNTERRATE:
			return new ChangeEncounterRate((ChangeEncounterRate*)other, pBase);
		case TYPE_CHANGEEQUIPPEDITEMS:
			return new ChangeEquippedItems((ChangeEquippedItems*)other, pBase);
		case TYPE_CHANGEEVENTLOCATION:
			return new ChangeEventLocation((ChangeEventLocation*)other, pBase);
		case TYPE_CHANGEEXPERIENCEPOINTS:
			return new ChangeExperiencePoints((ChangeExperiencePoints*)other, pBase);
		case TYPE_CHANGEFACEGRAPHIC:
			return new ChangeFaceGraphic((ChangeFaceGraphic*)other, pBase);
		case TYPE_CHANGEHERONAME:
			return new ChangeHeroName((ChangeHeroName*)other, pBase);
		case TYPE_CHANGEIMAGE:
			return new ChangeImage((ChangeImage*)other, pBase);
		case TYPE_CHANGEITEMLIST:
			return new ChangeItemList((ChangeItemList*)other, pBase);
		case TYPE_CHANGEMONEY:
			return new ChangeMoney((ChangeMoney*)other, pBase);
		case TYPE_CHANGEPARALLAXBACKGROUND:
			return new ChangeParallaxBackground((ChangeParallaxBackground*)other, pBase);
		case TYPE_CHANGEPARTY:
			return new ChangeParty((ChangeParty*)other, pBase);
		case TYPE_CHANGESELECTEDAREA:
			return new ChangeSelectedArea((ChangeSelectedArea*)other, pBase);
		case TYPE_CHANGESKILLS:
			return new ChangeSkills((ChangeSkills*)other, pBase);
		case TYPE_CHANGESYSTEMBGM:
			return new ChangeSystemBGM((ChangeSystemBGM*)other, pBase);
		case TYPE_CHANGESYSTEMGRAPHICS:
			return new ChangeSystemGraphics((ChangeSystemGraphics*)other, pBase);
		case TYPE_CHANGESYSTEMSOUNDEFFECT:
			return new ChangeSystemSoundEffect((ChangeSystemSoundEffect*)other, pBase);
		case TYPE_CHANGETEXT:
			return new ChangeText((ChangeText*)other, pBase);
		case TYPE_CHANGEVEHICLEGRAPHIC:
			return new ChangeVehicleGraphic((ChangeVehicleGraphic*)other, pBase);
		case TYPE_COMPLETEHEALING:
			return new CompleteHealing((CompleteHealing*)other, pBase);
		case TYPE_CONDITIONALBRANCH_MAP:
			return new ConditionalBranch_Map((ConditionalBranch_Map*)other, pBase);
		case TYPE_CONDITIONALBRANCH_MENU:
			return new ConditionalBranch_Menu((ConditionalBranch_Menu*)other, pBase);
		case TYPE_DISPLAYMESSAGE:
			return new DisplayMessage((DisplayMessage*)other, pBase);
		case TYPE_ENABLEDISABLEOBJECT:
			return new EnableDisableObject((EnableDisableObject*)other, pBase);
		case TYPE_ENDEVENTPROCESSING:
			return new EndEventProcessing((EndEventProcessing*)other, pBase);
		case TYPE_ENEMYENCOUNTER:
			return new EnemyEncounter((EnemyEncounter*)other, pBase);
		case TYPE_ERASEEVENT:
			return new EraseEvent((EraseEvent*)other, pBase);
		case TYPE_EXITMENU:
			return new ExitMenu((ExitMenu*)other, pBase);
		case TYPE_FADEBGM:
			return new FadeBGM((FadeBGM*)other, pBase);
		case TYPE_FLASHSCREEN:
			return new FlashScreen((FlashScreen*)other, pBase);
		case TYPE_FLASHSPRITE_MAP:
			return new FlashSprite_Map((FlashSprite_Map*)other, pBase);
		case TYPE_GAMEOVER:
			return new GameOver((GameOver*)other, pBase);
		case TYPE_HALTALLMOVEMENT:
			return new HaltAllMovement((HaltAllMovement*)other, pBase);
		case TYPE_HEROSPRITETRANSPARENCY:
			return new HeroSpriteTransparency((HeroSpriteTransparency*)other, pBase);
		case TYPE_HIDELAYER:
			return new HideLayer((HideLayer*)other, pBase);
		case TYPE_HIDESCREEN:
			return new HideScreen((HideScreen*)other, pBase);
		case TYPE_HIDESCREENOVERLAY:
			return new HideScreenOverlay((HideScreenOverlay*)other, pBase);
		case TYPE_INSERTCOMMENT:
			return new InsertComment((InsertComment*)other, pBase);
		case TYPE_ITEMMANAGEMENT:
			return new ItemManagement((ItemManagement*)other, pBase);
		case TYPE_JUMPTOLABEL:
			return new JumpToLabel((JumpToLabel*)other, pBase);
		case TYPE_KEYINPUTPROCESSING:
			return new KeyInputProcessing((KeyInputProcessing*)other, pBase);
		case TYPE_LABEL:
			return new Label((Label*)other, pBase);
		case TYPE_LOADGAME:
			return new LoadGame((LoadGame*)other, pBase);
		case TYPE_LOOP:
			return new Loop((Loop*)other, pBase);
		case TYPE_MEMORIZEBGM:
			return new MemorizeBGM((MemorizeBGM*)other, pBase);
		case TYPE_MEMORIZEPOSITION:
			return new MemorizePosition((MemorizePosition*)other, pBase);
		case TYPE_MOVEEVENT:
			return new MoveEvent((MoveEvent*)other, pBase);
		case TYPE_OPENMENU:
			return new OpenMenu((OpenMenu*)other, pBase);
		case TYPE_PANSCREEN:
			return new PanScreen((PanScreen*)other, pBase);
		case TYPE_PLAYBGM:
			return new PlayBGM((PlayBGM*)other, pBase);
		case TYPE_PLAYMEMORIZEDBGM:
			return new PlayMemorizedBGM((PlayMemorizedBGM*)other, pBase);
		case TYPE_PLAYMOVIE:
			return new PlayMovie((PlayMovie*)other, pBase);
		case TYPE_PLAYSOUNDEFFECT:
			return new PlaySoundEffect((PlaySoundEffect*)other, pBase);
		case TYPE_PLAYSYSTEMBGM:
			return new PlaySystemBGM((PlaySystemBGM*)other, pBase);
		case TYPE_PLAYSYSTEMSOUNDEFFECT:
			return new PlaySystemSoundEffect((PlaySystemSoundEffect*)other, pBase);
		case TYPE_PREVIOUSSELECTEDAREABRANCH:
			return new PreviousSelectedAreaBranch((PreviousSelectedAreaBranch*)other, pBase);
		case TYPE_PROCEEDWITHMOVEMENT:
			return new ProceedWithMovement((ProceedWithMovement*)other, pBase);
		case TYPE_QUITGAME:
			return new QuitGame((QuitGame*)other, pBase);
		case TYPE_SAVEGAME:
			return new SaveGame((SaveGame*)other, pBase);
		case TYPE_SCROLLAREA:
			return new ScrollArea((ScrollArea*)other, pBase);
		case TYPE_SETSCREENTRANSITION:
			return new SetScreenTransition((SetScreenTransition*)other, pBase);
		case TYPE_SHAKESCREEN:
			return new ShakeScreen((ShakeScreen*)other, pBase);
		case TYPE_SHOWBATTLEANIMATION_MAP:
			return new ShowBattleAnimation_Map((ShowBattleAnimation_Map*)other, pBase);
		case TYPE_SHOWGLOBALANIMATION:
			return new ShowGlobalAnimation((ShowGlobalAnimation*)other, pBase);
		case TYPE_SHOWLAYER:
			return new ShowLayer((ShowLayer*)other, pBase);
		case TYPE_SHOWPOPUPMENU:
			return new ShowPopupMenu((ShowPopupMenu*)other, pBase);
		case TYPE_SHOWSCREEN:
			return new ShowScreen((ShowScreen*)other, pBase);
		case TYPE_SHOWSCREENOVERLAY:
			return new ShowScreenOverlay((ShowScreenOverlay*)other, pBase);
		case TYPE_STOREEVENTID:
			return new StoreEventID((StoreEventID*)other, pBase);
		case TYPE_STORETERRAINID:
			return new StoreTerrainID((StoreTerrainID*)other, pBase);
		case TYPE_STRINGOPERATION:
			return new StringOperation((StringOperation*)other, pBase);
		case TYPE_SWAPTILE:
			return new SwapTile((SwapTile*)other, pBase);
		case TYPE_SWITCHOPERATION:
			return new SwitchOperation((SwitchOperation*)other, pBase);
		case TYPE_TELEPORT:
			return new Teleport((Teleport*)other, pBase);
		case TYPE_TILESUBSTITUTION:
			return new TileSubstitution((TileSubstitution*)other, pBase);
		case TYPE_TIMEROPERATION:
			return new TimerOperation((TimerOperation*)other, pBase);
		case TYPE_TINTSCREEN:
			return new TintScreen((TintScreen*)other, pBase);
		case TYPE_TRADETWOEVENTLOCATIONS:
			return new TradeTwoEventLocations((TradeTwoEventLocations*)other, pBase);
		case TYPE_VARIABLEMULTIVALUECONDITIONALBRANCH:
			return new VariableMultiValueConditionalBranch((VariableMultiValueConditionalBranch*)other, pBase);
		case TYPE_VARIABLEOPERATION:
			return new VariableOperation((VariableOperation*)other, pBase);
		case TYPE_WAIT:
			return new Wait((Wait*)other, pBase);
		case TYPE_WEATHEREFFECTS:
			return new WeatherEffects((WeatherEffects*)other, pBase);
	}
	return NULL;
}

bool BaseEvent::Event::tryLoad(XMLNode EventNode, int type, int baseType)
{
	switch (type)
	{
		case TYPE_TOPLEVELEVENT:
			return TopLevelEvent::tryLoad(EventNode, baseType);
		case TYPE_ALLOWDISALLOWESCAPE:
			return AllowDisallowEscape::tryLoad(EventNode, baseType);
		case TYPE_ALLOWDISALLOWMAINMENU:
			return AllowDisallowMainMenu::tryLoad(EventNode, baseType);
		case TYPE_ALLOWDISALLOWSAVING:
			return AllowDisallowSaving::tryLoad(EventNode, baseType);
		case TYPE_ALLOWDISALLOWTELEPORT:
			return AllowDisallowTeleport::tryLoad(EventNode, baseType);
		case TYPE_BREAKOUTOFLOOP:
			return BreakOutOfLoop::tryLoad(EventNode, baseType);
		case TYPE_CALLCOMMONEVENT:
			return CallCommonEvent::tryLoad(EventNode, baseType);
		case TYPE_CALLMAPEVENT:
			return CallMapEvent::tryLoad(EventNode, baseType);
		case TYPE_CHANGEBATTLECOMMANDS:
			return ChangeBattleCommands::tryLoad(EventNode, baseType);
		case TYPE_CHANGEBORDERTILE:
			return ChangeBorderTile::tryLoad(EventNode, baseType);
		case TYPE_CHANGECHARACTERBASESTATS:
			return ChangeCharacterBaseStats::tryLoad(EventNode, baseType);
		case TYPE_CHANGECHARACTERCONDITION:
			return ChangeCharacterCondition::tryLoad(EventNode, baseType);
		case TYPE_CHANGECHARACTERHP:
			return ChangeCharacterHP::tryLoad(EventNode, baseType);
		case TYPE_CHANGECHARACTERLEVEL:
			return ChangeCharacterLevel::tryLoad(EventNode, baseType);
		case TYPE_CHANGECHARACTERMP:
			return ChangeCharacterMP::tryLoad(EventNode, baseType);
		case TYPE_CHANGECHARACTERSPRITE:
			return ChangeCharacterSprite::tryLoad(EventNode, baseType);
		case TYPE_CHANGECLASS:
			return ChangeClass::tryLoad(EventNode, baseType);
		case TYPE_CHANGEENCOUNTERRATE:
			return ChangeEncounterRate::tryLoad(EventNode, baseType);
		case TYPE_CHANGEEQUIPPEDITEMS:
			return ChangeEquippedItems::tryLoad(EventNode, baseType);
		case TYPE_CHANGEEVENTLOCATION:
			return ChangeEventLocation::tryLoad(EventNode, baseType);
		case TYPE_CHANGEEXPERIENCEPOINTS:
			return ChangeExperiencePoints::tryLoad(EventNode, baseType);
		case TYPE_CHANGEFACEGRAPHIC:
			return ChangeFaceGraphic::tryLoad(EventNode, baseType);
		case TYPE_CHANGEHERONAME:
			return ChangeHeroName::tryLoad(EventNode, baseType);
		case TYPE_CHANGEIMAGE:
			return ChangeImage::tryLoad(EventNode, baseType);
		case TYPE_CHANGEITEMLIST:
			return ChangeItemList::tryLoad(EventNode, baseType);
		case TYPE_CHANGEMONEY:
			return ChangeMoney::tryLoad(EventNode, baseType);
		case TYPE_CHANGEPARALLAXBACKGROUND:
			return ChangeParallaxBackground::tryLoad(EventNode, baseType);
		case TYPE_CHANGEPARTY:
			return ChangeParty::tryLoad(EventNode, baseType);
		case TYPE_CHANGESELECTEDAREA:
			return ChangeSelectedArea::tryLoad(EventNode, baseType);
		case TYPE_CHANGESKILLS:
			return ChangeSkills::tryLoad(EventNode, baseType);
		case TYPE_CHANGESYSTEMBGM:
			return ChangeSystemBGM::tryLoad(EventNode, baseType);
		case TYPE_CHANGESYSTEMGRAPHICS:
			return ChangeSystemGraphics::tryLoad(EventNode, baseType);
		case TYPE_CHANGESYSTEMSOUNDEFFECT:
			return ChangeSystemSoundEffect::tryLoad(EventNode, baseType);
		case TYPE_CHANGETEXT:
			return ChangeText::tryLoad(EventNode, baseType);
		case TYPE_CHANGEVEHICLEGRAPHIC:
			return ChangeVehicleGraphic::tryLoad(EventNode, baseType);
		case TYPE_COMPLETEHEALING:
			return CompleteHealing::tryLoad(EventNode, baseType);
		case TYPE_CONDITIONALBRANCH_MAP:
			return ConditionalBranch_Map::tryLoad(EventNode, baseType);
		case TYPE_CONDITIONALBRANCH_MENU:
			return ConditionalBranch_Menu::tryLoad(EventNode, baseType);
		case TYPE_DISPLAYMESSAGE:
			return DisplayMessage::tryLoad(EventNode, baseType);
		case TYPE_ENABLEDISABLEOBJECT:
			return EnableDisableObject::tryLoad(EventNode, baseType);
		case TYPE_ENDEVENTPROCESSING:
			return EndEventProcessing::tryLoad(EventNode, baseType);
		case TYPE_ENEMYENCOUNTER:
			return EnemyEncounter::tryLoad(EventNode, baseType);
		case TYPE_ERASEEVENT:
			return EraseEvent::tryLoad(EventNode, baseType);
		case TYPE_EXITMENU:
			return ExitMenu::tryLoad(EventNode, baseType);
		case TYPE_FADEBGM:
			return FadeBGM::tryLoad(EventNode, baseType);
		case TYPE_FLASHSCREEN:
			return FlashScreen::tryLoad(EventNode, baseType);
		case TYPE_FLASHSPRITE_MAP:
			return FlashSprite_Map::tryLoad(EventNode, baseType);
		case TYPE_GAMEOVER:
			return GameOver::tryLoad(EventNode, baseType);
		case TYPE_HALTALLMOVEMENT:
			return HaltAllMovement::tryLoad(EventNode, baseType);
		case TYPE_HEROSPRITETRANSPARENCY:
			return HeroSpriteTransparency::tryLoad(EventNode, baseType);
		case TYPE_HIDELAYER:
			return HideLayer::tryLoad(EventNode, baseType);
		case TYPE_HIDESCREEN:
			return HideScreen::tryLoad(EventNode, baseType);
		case TYPE_HIDESCREENOVERLAY:
			return HideScreenOverlay::tryLoad(EventNode, baseType);
		case TYPE_INSERTCOMMENT:
			return InsertComment::tryLoad(EventNode, baseType);
		case TYPE_ITEMMANAGEMENT:
			return ItemManagement::tryLoad(EventNode, baseType);
		case TYPE_JUMPTOLABEL:
			return JumpToLabel::tryLoad(EventNode, baseType);
		case TYPE_KEYINPUTPROCESSING:
			return KeyInputProcessing::tryLoad(EventNode, baseType);
		case TYPE_LABEL:
			return Label::tryLoad(EventNode, baseType);
		case TYPE_LOADGAME:
			return LoadGame::tryLoad(EventNode, baseType);
		case TYPE_LOOP:
			return Loop::tryLoad(EventNode, baseType);
		case TYPE_MEMORIZEBGM:
			return MemorizeBGM::tryLoad(EventNode, baseType);
		case TYPE_MEMORIZEPOSITION:
			return MemorizePosition::tryLoad(EventNode, baseType);
		case TYPE_MOVEEVENT:
			return MoveEvent::tryLoad(EventNode, baseType);
		case TYPE_OPENMENU:
			return OpenMenu::tryLoad(EventNode, baseType);
		case TYPE_PANSCREEN:
			return PanScreen::tryLoad(EventNode, baseType);
		case TYPE_PLAYBGM:
			return PlayBGM::tryLoad(EventNode, baseType);
		case TYPE_PLAYMEMORIZEDBGM:
			return PlayMemorizedBGM::tryLoad(EventNode, baseType);
		case TYPE_PLAYMOVIE:
			return PlayMovie::tryLoad(EventNode, baseType);
		case TYPE_PLAYSOUNDEFFECT:
			return PlaySoundEffect::tryLoad(EventNode, baseType);
		case TYPE_PLAYSYSTEMBGM:
			return PlaySystemBGM::tryLoad(EventNode, baseType);
		case TYPE_PLAYSYSTEMSOUNDEFFECT:
			return PlaySystemSoundEffect::tryLoad(EventNode, baseType);
		case TYPE_PREVIOUSSELECTEDAREABRANCH:
			return PreviousSelectedAreaBranch::tryLoad(EventNode, baseType);
		case TYPE_PROCEEDWITHMOVEMENT:
			return ProceedWithMovement::tryLoad(EventNode, baseType);
		case TYPE_QUITGAME:
			return QuitGame::tryLoad(EventNode, baseType);
		case TYPE_SAVEGAME:
			return SaveGame::tryLoad(EventNode, baseType);
		case TYPE_SCROLLAREA:
			return ScrollArea::tryLoad(EventNode, baseType);
		case TYPE_SETSCREENTRANSITION:
			return SetScreenTransition::tryLoad(EventNode, baseType);
		case TYPE_SHAKESCREEN:
			return ShakeScreen::tryLoad(EventNode, baseType);
		case TYPE_SHOWBATTLEANIMATION_MAP:
			return ShowBattleAnimation_Map::tryLoad(EventNode, baseType);
		case TYPE_SHOWGLOBALANIMATION:
			return ShowGlobalAnimation::tryLoad(EventNode, baseType);
		case TYPE_SHOWLAYER:
			return ShowLayer::tryLoad(EventNode, baseType);
		case TYPE_SHOWPOPUPMENU:
			return ShowPopupMenu::tryLoad(EventNode, baseType);
		case TYPE_SHOWSCREEN:
			return ShowScreen::tryLoad(EventNode, baseType);
		case TYPE_SHOWSCREENOVERLAY:
			return ShowScreenOverlay::tryLoad(EventNode, baseType);
		case TYPE_STOREEVENTID:
			return StoreEventID::tryLoad(EventNode, baseType);
		case TYPE_STORETERRAINID:
			return StoreTerrainID::tryLoad(EventNode, baseType);
		case TYPE_STRINGOPERATION:
			return StringOperation::tryLoad(EventNode, baseType);
		case TYPE_SWAPTILE:
			return SwapTile::tryLoad(EventNode, baseType);
		case TYPE_SWITCHOPERATION:
			return SwitchOperation::tryLoad(EventNode, baseType);
		case TYPE_TELEPORT:
			return Teleport::tryLoad(EventNode, baseType);
		case TYPE_TILESUBSTITUTION:
			return TileSubstitution::tryLoad(EventNode, baseType);
		case TYPE_TIMEROPERATION:
			return TimerOperation::tryLoad(EventNode, baseType);
		case TYPE_TINTSCREEN:
			return TintScreen::tryLoad(EventNode, baseType);
		case TYPE_TRADETWOEVENTLOCATIONS:
			return TradeTwoEventLocations::tryLoad(EventNode, baseType);
		case TYPE_VARIABLEMULTIVALUECONDITIONALBRANCH:
			return VariableMultiValueConditionalBranch::tryLoad(EventNode, baseType);
		case TYPE_VARIABLEOPERATION:
			return VariableOperation::tryLoad(EventNode, baseType);
		case TYPE_WAIT:
			return Wait::tryLoad(EventNode, baseType);
		case TYPE_WEATHEREFFECTS:
			return WeatherEffects::tryLoad(EventNode, baseType);
	}
	return false;
}
//END BaseEvent::Event Member Functions

//BEGIN BaseEvent::TopLevelEvent Member Functions
BaseEvent::TopLevelEvent::~TopLevelEvent()
{
	for (int i = 0; i < events.size(); ++i)
		delete (events[i]);
	events.clear();
}

XMLNode BaseEvent::TopLevelEvent::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	for (int i = 0; i < events.size(); ++i)
		EventNode.addChild(events[i]->getEventNode());
	return EventNode;
}

void BaseEvent::TopLevelEvent::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(events.size());
	for (int i = 0; i < events.size(); ++i)
		events[i]->saveToStorageFile(storageFile);
}

QStringList BaseEvent::TopLevelEvent::getResources()
{
	QStringList resources;
	for (int i = 0; i < events.size(); ++i)
		resources += events[i]->getResources();
	return resources;
}

QStringList BaseEvent::TopLevelEvent::getViewableText(int indent)
{
	QStringList viewableText;
	for (int i = 0; i < events.size(); ++i)
		viewableText += events[i]->getViewableText(indent);
	viewableText.append("");
	for (int i = 0; i < indent; ++i)
		viewableText[viewableText.size() - 1] += "  ";
	viewableText[viewableText.size() - 1] += "<>";
	return viewableText;
}

QList<BaseEvent*> BaseEvent::TopLevelEvent::getEventPointers()
{
	QList<BaseEvent*> pointers;
	for (int i = 0; i < events.size(); ++i)
		pointers += events[i]->getEventPointers();
	pointers.append(NULL);
	return pointers;
}

void BaseEvent::TopLevelEvent::addEvent(BaseEvent *event, int before)
{
	if (before == -1)
		events.append(event);
	else
		events.insert(before, event);
}

void BaseEvent::TopLevelEvent::deleteEvent(int location)
{
	delete (events[location]);
	events.removeAt(location);
}

void BaseEvent::TopLevelEvent::applySwappedTiles(QList<Tileset::SwapTileIDs> swappedTiles)
{
	for (int i = 0; i < events.size(); ++i)
		events[i]->applySwappedTiles(swappedTiles);
}

void BaseEvent::TopLevelEvent::applyChangeMenuObjectName(QString oldName, QString newName)
{
	for (int i = 0; i < events.size(); ++i)
		events[i]->applyChangeMenuObjectName(oldName, newName);
}

bool BaseEvent::TopLevelEvent::tryLoad(XMLNode EventNode, int baseType)
{
	for (int i = 0, count = EventNode.nChildNode("event"); i < count; ++i)
	{
		if (!BaseEvent::tryLoad(EventNode.getChildNode("event", i), baseType))
			return false;
	}
	return true;
}

BaseEvent::TopLevelEvent::TopLevelEvent(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	for (int i = 0, count = EventNode.nChildNode("event"); i < count; ++i)
		events.append(new BaseEvent(pBaseEvent->baseType, EventNode.getChildNode("event", i)));
}

BaseEvent::TopLevelEvent::TopLevelEvent(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	for (int i = 0, count = storageFile.getSignedInt(); i < count; ++i)
		events.append(new BaseEvent(pBaseEvent->baseType, storageFile));
}

BaseEvent::TopLevelEvent::TopLevelEvent(TopLevelEvent *other, BaseEvent *pBase) : Event(pBase)
{
	for (int i = 0; i < other->events.size(); ++i)
		events.append(new BaseEvent(other->events[i]));
}
//END BaseEvent::TopLevelEvent Member Functions

//BEGIN BaseEvent::AllowDisallowEscape Member Functions
XMLNode BaseEvent::AllowDisallowEscape::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	EventNode.addAttribute("allow", (allow) ? "true":"false");
	return EventNode;
}

void BaseEvent::AllowDisallowEscape::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putBool(allow);
}

QStringList BaseEvent::AllowDisallowEscape::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Allow/Disallow Escape: ";
	viewableText[0] += (allow) ? "Allow":"Forbid";
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::AllowDisallowEscape::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	ConfigureEvent_AllowDisallow *configAllow = new ConfigureEvent_AllowDisallow("Escaping");
	bool configured = false;
	configAllow->setAllow(allow);
	if (configAllow->exec())
	{
		configured = true;
		allow = configAllow->getAllow();
	}
	delete configAllow;
	return configured;
}
#endif

bool BaseEvent::AllowDisallowEscape::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	if (!XMLTools::attributeExists(EventNode, "allow"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "allow", QStringList() << "true" << "false", 1))
		return false;
	return true;
}

BaseEvent::AllowDisallowEscape::AllowDisallowEscape(BaseEvent *pBase) : Event(pBase)
{
	allow = true;
}

BaseEvent::AllowDisallowEscape::AllowDisallowEscape(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	allow = (QString(EventNode.getAttribute("allow")) == "true") ? true:false;
}

BaseEvent::AllowDisallowEscape::AllowDisallowEscape(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	allow = storageFile.getBool();
}

BaseEvent::AllowDisallowEscape::AllowDisallowEscape(AllowDisallowEscape *other, BaseEvent *pBase) : Event(pBase)
{
	allow = other->allow;
}
//END BaseEvent::AllowDisallowEscape Member Functions

//BEGIN BaseEvent::AllowDisallowMainMenu Member Functions
XMLNode BaseEvent::AllowDisallowMainMenu::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	EventNode.addAttribute("allow", (allow) ? "true":"false");
	return EventNode;
}

void BaseEvent::AllowDisallowMainMenu::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putBool(allow);
}

QStringList BaseEvent::AllowDisallowMainMenu::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Allow/Disallow Main Menu: ";
	viewableText[0] += (allow) ? "Allow":"Forbid";
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::AllowDisallowMainMenu::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	ConfigureEvent_AllowDisallow *configAllow = new ConfigureEvent_AllowDisallow("Main Menu");
	bool configured = false;
	configAllow->setAllow(allow);
	if (configAllow->exec())
	{
		configured = true;
		allow = configAllow->getAllow();
	}
	delete configAllow;
	return configured;
}
#endif

bool BaseEvent::AllowDisallowMainMenu::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	if (!XMLTools::attributeExists(EventNode, "allow"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "allow", QStringList() << "true" << "false", 1))
		return false;
	return true;
}

BaseEvent::AllowDisallowMainMenu::AllowDisallowMainMenu(BaseEvent *pBase) : Event(pBase)
{
	allow = true;
}

BaseEvent::AllowDisallowMainMenu::AllowDisallowMainMenu(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	allow = (QString(EventNode.getAttribute("allow")) == "true") ? true:false;
}

BaseEvent::AllowDisallowMainMenu::AllowDisallowMainMenu(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	allow = storageFile.getBool();
}

BaseEvent::AllowDisallowMainMenu::AllowDisallowMainMenu(AllowDisallowMainMenu *other, BaseEvent *pBase) : Event(pBase)
{
	allow = other->allow;
}
//END BaseEvent::AllowDisallowMainMenu Member Functions

//BEGIN BaseEvent::AllowDisallowSaving Member Functions
XMLNode BaseEvent::AllowDisallowSaving::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	EventNode.addAttribute("allow", (allow) ? "true":"false");
	return EventNode;
}

void BaseEvent::AllowDisallowSaving::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putBool(allow);
}

QStringList BaseEvent::AllowDisallowSaving::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Allow/Disallow Saving: ";
	viewableText[0] += (allow) ? "Allow":"Forbid";
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::AllowDisallowSaving::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	ConfigureEvent_AllowDisallow *configAllow = new ConfigureEvent_AllowDisallow("Saving");
	bool configured = false;
	configAllow->setAllow(allow);
	if (configAllow->exec())
	{
		configured = true;
		allow = configAllow->getAllow();
	}
	delete configAllow;
	return configured;
}
#endif

bool BaseEvent::AllowDisallowSaving::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	if (!XMLTools::attributeExists(EventNode, "allow"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "allow", QStringList() << "true" << "false", 1))
		return false;
	return true;
}

BaseEvent::AllowDisallowSaving::AllowDisallowSaving(BaseEvent *pBase) : Event(pBase)
{
	allow = true;
}

BaseEvent::AllowDisallowSaving::AllowDisallowSaving(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	allow = (QString(EventNode.getAttribute("allow")) == "true") ? true:false;
}

BaseEvent::AllowDisallowSaving::AllowDisallowSaving(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	allow = storageFile.getBool();
}

BaseEvent::AllowDisallowSaving::AllowDisallowSaving(AllowDisallowSaving *other, BaseEvent *pBase) : Event(pBase)
{
	allow = other->allow;
}
//END BaseEvent::AllowDisallowSaving Member Functions

//BEGIN BaseEvent::AllowDisallowTeleport Member Functions
XMLNode BaseEvent::AllowDisallowTeleport::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	EventNode.addAttribute("allow", (allow) ? "true":"false");
	return EventNode;
}

void BaseEvent::AllowDisallowTeleport::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putBool(allow);
}

QStringList BaseEvent::AllowDisallowTeleport::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Allow/Disallow Teleport: ";
	viewableText[0] += (allow) ? "Allow":"Forbid";
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::AllowDisallowTeleport::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	ConfigureEvent_AllowDisallow *configAllow = new ConfigureEvent_AllowDisallow("Teleporting");
	bool configured = false;
	configAllow->setAllow(allow);
	if (configAllow->exec())
	{
		configured = true;
		allow = configAllow->getAllow();
	}
	delete configAllow;
	return configured;
}
#endif

bool BaseEvent::AllowDisallowTeleport::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	if (!XMLTools::attributeExists(EventNode, "allow"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "allow", QStringList() << "true" << "false", 1))
		return false;
	return true;
}

BaseEvent::AllowDisallowTeleport::AllowDisallowTeleport(BaseEvent *pBase) : Event(pBase)
{
	allow = true;
}

BaseEvent::AllowDisallowTeleport::AllowDisallowTeleport(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	allow = (QString(EventNode.getAttribute("allow")) == "true") ? true:false;
}

BaseEvent::AllowDisallowTeleport::AllowDisallowTeleport(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	allow = storageFile.getBool();
}

BaseEvent::AllowDisallowTeleport::AllowDisallowTeleport(AllowDisallowTeleport *other, BaseEvent *pBase) : Event(pBase)
{
	allow = other->allow;
}
//END BaseEvent::AllowDisallowTeleport Member Functions

//BEGIN BaseEvent::CallCommonEvent Member Functions
XMLNode BaseEvent::CallCommonEvent::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	EventNode.addAttribute("eventID", QString::number(eventID).toUtf8().data());
	EventNode.addAttribute("eventReferencedInVariable", (eventReferencedInVariable) ? "true":"false");
	return EventNode;
}

void BaseEvent::CallCommonEvent::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(eventID);
	storageFile.putBool(eventReferencedInVariable);
}

QStringList BaseEvent::CallCommonEvent::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Call Common Event: ";
	if (eventReferencedInVariable)
		viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(eventID));
	else
		viewableText[0] += ProjectData::commonEventNames[eventID];
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::CallCommonEvent::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	ConfigureEvent_CallEvent *configCallEvent = new ConfigureEvent_CallEvent;
	bool configured = false;
	configCallEvent->setIsReferencedInVariable(eventReferencedInVariable);
	configCallEvent->setEventID(eventID);
	if (configCallEvent->exec())
	{
		configured = true;
		eventID = configCallEvent->getEventID();
		eventReferencedInVariable = configCallEvent->isReferencedInVariable();
	}
	delete configCallEvent;
	return configured;
}
#endif

bool BaseEvent::CallCommonEvent::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	QStringList trueFalseList = QStringList() << "true" << "false";
	if (!XMLTools::attributeExists(EventNode, "eventID"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "eventReferencedInVariable"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "eventReferencedInVariable", trueFalseList, 1))
		return false;
	return true;
}

BaseEvent::CallCommonEvent::CallCommonEvent(BaseEvent *pBase) : Event(pBase)
{
	eventID = 0;
	eventReferencedInVariable = false;
}

BaseEvent::CallCommonEvent::CallCommonEvent(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	eventID = QString(EventNode.getAttribute("eventID")).toInt();
	eventReferencedInVariable = (QString(EventNode.getAttribute("eventReferencedInVariable")) == "true") ? true:false;
}

BaseEvent::CallCommonEvent::CallCommonEvent(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	eventID = storageFile.getSignedInt();
	eventReferencedInVariable = storageFile.getBool();
}

BaseEvent::CallCommonEvent::CallCommonEvent(CallCommonEvent *other, BaseEvent *pBase) : Event(pBase)
{
	eventID = other->eventID;
	eventReferencedInVariable = other->eventReferencedInVariable;
}
//END BaseEvent::CallCommonEvent Member Functions

//BEGIN BaseEvent::CallMapEvent Member Functions
XMLNode BaseEvent::CallMapEvent::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	EventNode.addAttribute("eventID", QString::number(eventID).toUtf8().data());
	EventNode.addAttribute("eventReferencedInVariable", (eventReferencedInVariable) ? "true":"false");
	EventNode.addAttribute("pageID", QString::number(pageID).toUtf8().data());
	EventNode.addAttribute("pageReferencedInVariable", (pageReferencedInVariable) ? "true":"false");
	return EventNode;
}

void BaseEvent::CallMapEvent::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(eventID);
	storageFile.putSignedInt(pageID);
	storageFile.putBool(eventReferencedInVariable);
	storageFile.putBool(pageReferencedInVariable);
}

QStringList BaseEvent::CallMapEvent::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Call Map Event: event=";
	if (eventReferencedInVariable)
		viewableText[0] += QString("[%1] page=").arg(ProjectData::numberVariableIdToString(eventID));
	else if (eventID == -1)
		viewableText[0] += "this page=";
	else
		viewableText[0] += QString("%1 page=").arg(eventID);
	if (pageReferencedInVariable)
		viewableText[0] += QString("[%1]").arg(ProjectData::numberVariableIdToString(pageID));
	else
		viewableText[0] += QString::number(pageID);
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::CallMapEvent::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerType);
	ConfigureEvent_CallMapEvent *configCallMapEvent = new ConfigureEvent_CallMapEvent(callerID);
	bool configured = false;
	configCallMapEvent->setEventReferencedInVariable(eventReferencedInVariable);
	configCallMapEvent->setEventID(eventID);
	configCallMapEvent->setPageReferencedInVariable(pageReferencedInVariable);
	configCallMapEvent->setPageID(pageID);
	if (configCallMapEvent->exec())
	{
		configured = true;
		eventID = configCallMapEvent->getEventID();
		eventReferencedInVariable = configCallMapEvent->isEventReferencedInVariable();
		pageID = configCallMapEvent->getPageID();
		pageReferencedInVariable = configCallMapEvent->isPageReferencedInVariable();
	}
	delete configCallMapEvent;
	return configured;
}
#endif

bool BaseEvent::CallMapEvent::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	QStringList trueFalseList = QStringList() << "true" << "false";
	if (!XMLTools::attributeExists(EventNode, "eventID"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "eventReferencedInVariable"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "eventReferencedInVariable", trueFalseList, 1))
		return false;
	if (!XMLTools::attributeExists(EventNode, "pageID"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "pageReferencedInVariable"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "pageReferencedInVariable", trueFalseList, 1))
		return false;
	return true;
}

BaseEvent::CallMapEvent::CallMapEvent(BaseEvent *pBase) : Event(pBase)
{
	eventID = -1;
	pageID = 0;
	eventReferencedInVariable = false;
	pageReferencedInVariable = false;
}

BaseEvent::CallMapEvent::CallMapEvent(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	eventID = QString(EventNode.getAttribute("eventID")).toInt();
	eventReferencedInVariable = (QString(EventNode.getAttribute("eventReferencedInVariable")) == "true") ? true:false;
	pageID = QString(EventNode.getAttribute("pageID")).toInt();
	pageReferencedInVariable = (QString(EventNode.getAttribute("pageReferencedInVariable")) == "true") ? true:false;
}

BaseEvent::CallMapEvent::CallMapEvent(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	eventID = storageFile.getSignedInt();
	pageID = storageFile.getSignedInt();
	eventReferencedInVariable = storageFile.getBool();
	pageReferencedInVariable = storageFile.getBool();
}

BaseEvent::CallMapEvent::CallMapEvent(CallMapEvent *other, BaseEvent *pBase) : Event(pBase)
{
	eventID = other->eventID;
	pageID = other->pageID;
	eventReferencedInVariable = other->eventReferencedInVariable;
	pageReferencedInVariable = other->pageReferencedInVariable;
}
//END BaseEvent::CallMapEvent Member Functions

//BEGIN BaseEvent::ChangeBattleCommands Member Functions
XMLNode BaseEvent::ChangeBattleCommands::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	EventNode.addAttribute("characterID", QString::number(characterID).toUtf8().data());
	EventNode.addAttribute("characterReferencedInVariable", (characterReferencedInVariable) ? "true":"false");
	EventNode.addAttribute("operation", (operation == REMOVE) ? "remove":"add");
	EventNode.addAttribute("commandID", QString::number(commandID).toUtf8().data());
	EventNode.addAttribute("commandReferencedInVariable", (commandReferencedInVariable) ? "true":"false");
	return EventNode;
}

void BaseEvent::ChangeBattleCommands::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(characterID);
	storageFile.putSignedInt(commandID);
	storageFile.putSignedInt(operation);
	storageFile.putBool(characterReferencedInVariable);
	storageFile.putBool(commandReferencedInVariable);
}

QStringList BaseEvent::ChangeBattleCommands::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	QString operationText1;
	QString operationText2;
	QString commandText;
	QString characterText;
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Change Battle Command: ";
	if (operation == REMOVE)
	{
		operationText1 = "remove ";
		operationText2 = " from ";
	}
	else
	{
		operationText1 = "add ";
		operationText2 = " to ";
	}
	if (commandReferencedInVariable)
		commandText = QString("variable[%1]").arg(ProjectData::numberVariableIdToString(commandID));
	else
		commandText = ProjectData::battleCommands[commandID]->name;
	if (characterReferencedInVariable)
		characterText = QString("variable[%1]").arg(ProjectData::numberVariableIdToString(characterID));
	else
		characterText = ProjectData::characterNames[characterID];
	viewableText[0] += operationText1;
	viewableText[0] += commandText;
	viewableText[0] += operationText2;
	viewableText[0] += characterText;
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::ChangeBattleCommands::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	ConfigureEvent_ChangeBattleCommands *configEvent = new ConfigureEvent_ChangeBattleCommands;
	bool configured = false;
	configEvent->setIsReferencedCharacter(characterReferencedInVariable);
	configEvent->setCharacterID(characterID);
	configEvent->setIsReferencedCommand(commandReferencedInVariable);
	configEvent->setCommandID(commandID);
	configEvent->setOperation((operation == ADD) ? true:false);
	if (configEvent->exec())
	{
		configured = true;
		operation = (configEvent->isAddOperation()) ? ADD:REMOVE;
		characterID = configEvent->getCharacterID();
		commandID = configEvent->getCommandID();
		characterReferencedInVariable = configEvent->isReferencedCharacter();
		commandReferencedInVariable = configEvent->isReferencedCommand();
	}
	delete configEvent;
	return configured;
}
#endif

bool BaseEvent::ChangeBattleCommands::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	QStringList trueFalseList = QStringList() << "true" << "false";
	QStringList operationList = QStringList() << "remove" << "add";
	if (!XMLTools::attributeExists(EventNode, "characterID"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "characterReferencedInVariable"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "characterReferencedInVariable", trueFalseList, 1))
		return false;
	if (!XMLTools::attributeExists(EventNode, "operation"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "operation", operationList))
		return false;
	if (!XMLTools::attributeExists(EventNode, "commandID"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "commandReferencedInVariable", trueFalseList, 1))
		return false;
	return true;
}

BaseEvent::ChangeBattleCommands::ChangeBattleCommands(BaseEvent *pBase) : Event(pBase)
{
	characterID = 0;
	commandID = 0;
	operation = REMOVE;
	characterReferencedInVariable = false;
	commandReferencedInVariable = false;
}

BaseEvent::ChangeBattleCommands::ChangeBattleCommands(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	characterID = QString(EventNode.getAttribute("characterID")).toInt();
	commandID = QString(EventNode.getAttribute("commandID")).toInt();
	operation = (QString(EventNode.getAttribute("operation")) == "remove") ? REMOVE:ADD;
	characterReferencedInVariable = (QString(EventNode.getAttribute("characterReferencedInVariable")) == "true") ? true:false;
	commandReferencedInVariable = (QString(EventNode.getAttribute("commandReferencedInVariable")) == "true") ? true:false;
}

BaseEvent::ChangeBattleCommands::ChangeBattleCommands(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	characterID = storageFile.getSignedInt();
	commandID = storageFile.getSignedInt();
	operation = storageFile.getSignedInt();
	characterReferencedInVariable = storageFile.getBool();
	commandReferencedInVariable = storageFile.getBool();
}

BaseEvent::ChangeBattleCommands::ChangeBattleCommands(ChangeBattleCommands *other, BaseEvent *pBase) : Event(pBase)
{
	characterID = other->characterID;
	commandID = other->commandID;
	operation = other->operation;
	characterReferencedInVariable = other->characterReferencedInVariable;
	commandReferencedInVariable = other->commandReferencedInVariable;
}
//END BaseEvent::ChangeBattleCommands Member Functions

//BEGIN BaseEvent::ChangeBorderTile Member Functions
XMLNode BaseEvent::ChangeBorderTile::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	EventNode.addAttribute("tilesetID", QString::number(tilesetID).toUtf8().data());
	EventNode.addAttribute("tileID", QString::number(tileID).toUtf8().data());
	EventNode.addAttribute("tileReferencedInVariables", (tileReferencedInVariables) ? "true":"false");
	return EventNode;
}

void BaseEvent::ChangeBorderTile::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(tilesetID);
	storageFile.putSignedInt(tileID);
	storageFile.putBool(tileReferencedInVariables);
}

QStringList BaseEvent::ChangeBorderTile::getViewableText(int indent)
{
	QStringList viewableText;
	viewableText.append(0);
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> Change Border Tile: ";
	if (tileReferencedInVariables)
	{
		viewableText[0] += QString("tilesetID=variable[%1] ").arg(ProjectData::numberVariableIdToString(tilesetID));
		viewableText[0] += QString("tileID=variable[%1]").arg(ProjectData::numberVariableIdToString(tileID));
	}
	else
		viewableText[0] += QString("tilesetID=%1 tileID=%2").arg(tilesetID).arg(tileID);
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::ChangeBorderTile::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	ConfigureEvent_ChangeBorderTile *configChangeBorderTile = new ConfigureEvent_ChangeBorderTile;
	bool configured = false;
	configChangeBorderTile->setTileReferencedInVariables(tileReferencedInVariables);
	configChangeBorderTile->setTile(tilesetID, tileID);
	if (configChangeBorderTile->exec())
	{
		configured = true;
		tilesetID = configChangeBorderTile->getTilesetID();
		tileID = configChangeBorderTile->getTileID();
		tileReferencedInVariables = configChangeBorderTile->isTileReferencedInVariables();
	}
	delete configChangeBorderTile;
	return configured;
}
#endif

void BaseEvent::ChangeBorderTile::applySwappedTiles(QList<Tileset::SwapTileIDs> swappedTiles)
{
	if (!tileReferencedInVariables)
	{
		for (int i = 0; i < swappedTiles.size(); ++i)
		{
			if (tilesetID == swappedTiles[i].tileset)
			{
				if (tileID == swappedTiles[i].tile1)
					tileID = swappedTiles[i].tile2;
				else if (tileID == swappedTiles[i].tile2)
					tileID = swappedTiles[i].tile1;
			}
		}
	}
}

bool BaseEvent::ChangeBorderTile::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	if (!XMLTools::attributeExists(EventNode, "tilesetID"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "tileID"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "tileReferencedInVariables"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "tileReferencedInVariables", QStringList() << "true" << "false", 1))
		return false;
	return true;
}

BaseEvent::ChangeBorderTile::ChangeBorderTile(BaseEvent *pBase) : Event(pBase)
{
	tilesetID = 0;
	tileID = 0;
	tileReferencedInVariables = false;
}

BaseEvent::ChangeBorderTile::ChangeBorderTile(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	tilesetID = QString(EventNode.getAttribute("tilesetID")).toInt();
	tileID = QString(EventNode.getAttribute("tileID")).toInt();
	tileReferencedInVariables = (QString(EventNode.getAttribute("tileReferencedInVariables")) == "true") ? true:false;
}

BaseEvent::ChangeBorderTile::ChangeBorderTile(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	tilesetID = storageFile.getSignedInt();
	tileID = storageFile.getSignedInt();
	tileReferencedInVariables = false;
}

BaseEvent::ChangeBorderTile::ChangeBorderTile(ChangeBorderTile *other, BaseEvent *pBase) : Event(pBase)
{
	tilesetID = other->tilesetID;
	tileID = other->tileID;
	tileReferencedInVariables = other->tileReferencedInVariables;
}
//END BaseEvent::ChangeBorderTile Member Functions

//BEGIN BaseEvent::ChangeCharacterBaseStats Member Functions
XMLNode BaseEvent::ChangeCharacterBaseStats::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	if (target == ENTIRE_PARTY)
		EventNode.addAttribute("target", "entireParty");
	else if (target == SPECIFIC_CHARACTER)
	{
		EventNode.addAttribute("target", "specificCharacter");
		EventNode.addAttribute("characterID", QString::number(characterID).toUtf8().data());
	}
	else if (target == REFERENCED_CHARACTER)
	{
		EventNode.addAttribute("target", "referencedCharacter");
		EventNode.addAttribute("variableID", QString::number(characterID).toUtf8().data());
	}
	EventNode.addAttribute("operation", (operation == INCREASE_STAT) ? "increaseStat":"decreaseStat");
	if (stat == MAX_HP)
		EventNode.addAttribute("stat", "maxHP");
	else if (stat == MAX_MP)
		EventNode.addAttribute("stat", "maxMP");
	else if (stat == ATTACK)
		EventNode.addAttribute("stat", "attack");
	else if (stat == DEFENSE)
		EventNode.addAttribute("stat", "defense");
	else if (stat == WISDOM)
		EventNode.addAttribute("stat", "wisdom");
	else if (stat == AGILITY)
		EventNode.addAttribute("stat", "agility");
	EventNode.addAttribute("amount", QString::number(amount).toUtf8().data());
	EventNode.addAttribute("amountStoredInVariable", (amountStoredInVariable) ? "true":"false");
	return EventNode;
}

void BaseEvent::ChangeCharacterBaseStats::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(characterID);
	storageFile.putSignedInt(amount);
	storageFile.putSignedInt(target);
	storageFile.putSignedInt(operation);
	storageFile.putSignedInt(stat);
	storageFile.putBool(amountStoredInVariable);
}

QStringList BaseEvent::ChangeCharacterBaseStats::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	QString targetText;
	QString operationText;
	QString statText;
	QString amountText;
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Change Character Base Stats: ";
	if (target == ENTIRE_PARTY)
		targetText = "Entire Party";
	else if (target == SPECIFIC_CHARACTER)
		targetText = ProjectData::characterNames[characterID];
	else
		targetText = QString("variable[%1]").arg(ProjectData::numberVariableIdToString(characterID));
	if (operation == INCREASE_STAT)
		operationText = "+";
	else
		operationText = "-";
	if (stat == MAX_HP)
		statText = "MaxHP";
	else if (stat == MAX_MP)
		statText = "MaxMP";
	else if (stat == ATTACK)
		statText = "Attack";
	else if (stat == DEFENSE)
		statText = "Defense";
	else if (stat == WISDOM)
		statText = "Wisdom";
	else
		statText = "Agility";
	if (amountStoredInVariable)
		amountText = QString("variable[%1]").arg(ProjectData::numberVariableIdToString(amount));
	else
		amountText = QString::number(amount);
	viewableText[0] += targetText + "\'s ";
	viewableText[0] += statText + " ";
	viewableText[0] += operationText;
	viewableText[0] += amountText;
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::ChangeCharacterBaseStats::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	ConfigureEvent_ChangeCharacterBaseStats *configBaseStats = new ConfigureEvent_ChangeCharacterBaseStats;
	bool configured = false;
	configBaseStats->setTarget(target);
	configBaseStats->setCharacterID(characterID);
	configBaseStats->setOperation((operation == INCREASE_STAT) ? true:false);
	configBaseStats->setStat(stat);
	configBaseStats->setIsStoredAmount(amountStoredInVariable);
	configBaseStats->setAmount(amount);
	if (configBaseStats->exec())
	{
		configured = true;
		target = configBaseStats->getTarget();
		characterID = configBaseStats->getCharacterID();
		operation = (configBaseStats->isIncreaseOperation()) ? INCREASE_STAT:DECREASE_STAT;
		stat = configBaseStats->getStat();
		amount = configBaseStats->getAmount();
		amountStoredInVariable = configBaseStats->isStoredAmount();
	}
	delete configBaseStats;
	return configured;
}
#endif

bool BaseEvent::ChangeCharacterBaseStats::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	QStringList trueFalseList = QStringList() << "true" << "false";
	QStringList tempStringList;
	QString tempString;
	if (!XMLTools::attributeExists(EventNode, "target"))
		return false;
	tempStringList.clear();
	tempStringList << "entireParty" << "specificCharacter" << "referencedCharacter";
	if (!XMLTools::attributeStringValid(EventNode, "target", tempStringList))
		return false;
	tempString = EventNode.getAttribute("target");
	if (tempString == "specificCharacter")
	{
		if (!XMLTools::attributeExists(EventNode, "characterID"))
			return false;
	}
	if (tempString == "referencedCharacter")
	{
		if (!XMLTools::attributeExists(EventNode, "variableID"))
			return false;
	}
	if (!XMLTools::attributeExists(EventNode, "operation"))
		return false;
	tempStringList.clear();
	tempStringList << "increaseStat" << "decreaseStat";
	if (!XMLTools::attributeStringValid(EventNode, "operation", tempStringList))
		return false;
	if (!XMLTools::attributeExists(EventNode, "stat"))
		return false;
	tempStringList.clear();
	tempStringList << "maxHP" << "maxMP" << "attack" << "defense" << "wisdom" << "agility";
	if (!XMLTools::attributeStringValid(EventNode, "stat", tempStringList))
		return false;
	if (!XMLTools::attributeExists(EventNode, "amount"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "amountStoredInVariable"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "amountStoredInVariable", trueFalseList, 1))
		return false;
	return true;
}

BaseEvent::ChangeCharacterBaseStats::ChangeCharacterBaseStats(BaseEvent *pBase) : Event(pBase)
{
	characterID = 0;
	amount = 0;
	target = ENTIRE_PARTY;
	operation = INCREASE_STAT;
	stat = MAX_HP;
	amountStoredInVariable = false;
}

BaseEvent::ChangeCharacterBaseStats::ChangeCharacterBaseStats(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	QString tempString;
	tempString = EventNode.getAttribute("target");
	if (tempString == "entireParty")
		target = ENTIRE_PARTY;
	else if (tempString == "specificCharacter")
	{
		target = SPECIFIC_CHARACTER;
		characterID = QString(EventNode.getAttribute("characterID")).toInt();
	}
	else if (tempString == "referencedCharacter")
	{
		target = REFERENCED_CHARACTER;
		characterID = QString(EventNode.getAttribute("variableID")).toInt();
	}
	operation = (QString(EventNode.getAttribute("operation")) == "increaseStat") ? INCREASE_STAT:DECREASE_STAT;
	tempString = EventNode.getAttribute("stat");
	if (tempString == "maxHP")
		stat = MAX_HP;
	else if (tempString == "maxMP")
		stat = MAX_MP;
	else if (tempString == "attack")
		stat = ATTACK;
	else if (tempString == "defense")
		stat = DEFENSE;
	else if (tempString == "wisdom")
		stat = WISDOM;
	else if (tempString == "agility")
		stat = AGILITY;
	amount = QString(EventNode.getAttribute("amount")).toInt();
	amountStoredInVariable = (QString(EventNode.getAttribute("amountStoredInVariable")) == "true") ? true:false;
}

BaseEvent::ChangeCharacterBaseStats::ChangeCharacterBaseStats(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	characterID = storageFile.getSignedInt();
	amount = storageFile.getSignedInt();
	target = storageFile.getSignedInt();
	operation = storageFile.getSignedInt();
	stat = storageFile.getSignedInt();
	amountStoredInVariable = storageFile.getBool();
}

BaseEvent::ChangeCharacterBaseStats::ChangeCharacterBaseStats(ChangeCharacterBaseStats *other, BaseEvent *pBase) : Event(pBase)
{
	characterID = other->characterID;
	amount = other->amount;
	target = other->target;
	operation = other->operation;
	stat = other->stat;
	amountStoredInVariable = other->amountStoredInVariable;
}
//END BaseEvent::ChangeCharacterBaseStats Member Functions

//BEGIN BaseEvent::ChangeCharacterCondition Member Functions
XMLNode BaseEvent::ChangeCharacterCondition::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	if (target == ENTIRE_PARTY)
		EventNode.addAttribute("target", "entireParty");
	else if (target == SPECIFIC_CHARACTER)
	{
		EventNode.addAttribute("target", "specificCharacter");
		EventNode.addAttribute("characterID", QString::number(characterID).toUtf8().data());
	}
	else if (target == REFERENCED_CHARACTER)
	{
		EventNode.addAttribute("target", "referencedCharacter");
		EventNode.addAttribute("variableID", QString::number(characterID).toUtf8().data());
	}
	EventNode.addAttribute("operation", (operation == ADD_CONDITION) ? "addCondition":"removeCondition");
	EventNode.addAttribute("conditionID", QString::number(conditionID).toUtf8().data());
	EventNode.addAttribute("conditionReferencedInVariable", (conditionReferencedInVariable) ? "true":"false");
	return EventNode;
}

void BaseEvent::ChangeCharacterCondition::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(characterID);
	storageFile.putSignedInt(conditionID);
	storageFile.putSignedInt(target);
	storageFile.putSignedInt(operation);
	storageFile.putBool(conditionReferencedInVariable);
}

QStringList BaseEvent::ChangeCharacterCondition::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	QString targetText;
	QString operationText1;
	QString operationText2;
	QString conditionText;
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Change Character Condition: ";
	if (target == ENTIRE_PARTY)
		targetText = "Entire Party";
	else if (target == SPECIFIC_CHARACTER)
		targetText = ProjectData::characterNames[characterID];
	else
		targetText = QString("variable[%1]").arg(ProjectData::numberVariableIdToString(characterID));
	if (operation == ADD_CONDITION)
	{
		operationText1 = "Inflict";
		operationText2 = "on";
	}
	else
	{
		operationText1 = "Remove";
		operationText2 = "from";
	}
	if (conditionReferencedInVariable)
		conditionText = QString("variable[%1]").arg(ProjectData::numberVariableIdToString(conditionID));
	else
		conditionText = ProjectData::conditionNames[conditionID];
	viewableText[0] += operationText1 + " ";
	viewableText[0] += conditionText + " ";
	viewableText[0] += operationText2 + " ";
	viewableText[0] += targetText;
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::ChangeCharacterCondition::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	ConfigureEvent_ChangeCharacterCondition *configCondition = new ConfigureEvent_ChangeCharacterCondition;
	bool configured = false;
	configCondition->setTarget(target);
	configCondition->setCharacterID(characterID);
	configCondition->setOperation((operation == ADD_CONDITION) ? true:false);
	configCondition->setIsReferencedCondition(conditionReferencedInVariable);
	configCondition->setConditionID(conditionID);
	if (configCondition->exec())
	{
		configured = true;
		target = configCondition->getTarget();
		characterID = configCondition->getCharacterID();
		operation = (configCondition->isAddOperation()) ? ADD_CONDITION:REMOVE_CONDITION;
		conditionID = configCondition->getConditionID();
		conditionReferencedInVariable = configCondition->isReferencedCondition();
	}
	delete configCondition;
	return configured;
}
#endif

bool BaseEvent::ChangeCharacterCondition::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	QStringList tempStringList;
	QString tempString;
	tempStringList.clear();
	tempStringList << "entireParty" << "soecificCharacter" << "referencedCharacter";
	if (!XMLTools::attributeExists(EventNode, "target"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "target", tempStringList))
		return false;
	tempString = EventNode.getAttribute("target");
	if (tempString == "specificCharacter")
	{
		if (!XMLTools::attributeExists(EventNode, "characterID"))
			return false;
	}
	else if (tempString == "referencedCharacter")
	{
		if (!XMLTools::attributeExists(EventNode, "referencedCharacter"))
			return false;
	}
	tempStringList.clear();
	tempStringList << "addCondition" << "removeCondition";
	if (!XMLTools::attributeExists(EventNode, "operation"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "operation", tempStringList))
		return false;
	if (!XMLTools::attributeExists(EventNode, "conditionID"))
		return false;
	tempStringList.clear();
	tempStringList << "true" << "false";
	if (!XMLTools::attributeExists(EventNode, "conditionReferencedInVariable"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "conditionReferencedInVariable", tempStringList, 1))
		return false;
	return true;
}

BaseEvent::ChangeCharacterCondition::ChangeCharacterCondition(BaseEvent *pBase) : Event(pBase)
{
	characterID = 0;
	conditionID = 0;
	target = ENTIRE_PARTY;
	operation = ADD_CONDITION;
	conditionReferencedInVariable = false;
}

BaseEvent::ChangeCharacterCondition::ChangeCharacterCondition(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	QString tempString;
	tempString = EventNode.getAttribute("target");
	if (tempString == "entireParty")
	{
		target = ENTIRE_PARTY;
		characterID = 0;
	}
	else if (tempString == "specificCharacter")
	{
		target = SPECIFIC_CHARACTER;
		characterID = QString(EventNode.getAttribute("characterID")).toInt();
	}
	else if (tempString == "referencedCharacter")
	{
		target = REFERENCED_CHARACTER;
		characterID = QString(EventNode.getAttribute("variableID")).toInt();
	}
	tempString = EventNode.getAttribute("operation");
	operation = (tempString == "addCondition") ? ADD_CONDITION:REMOVE_CONDITION;
	conditionID = QString(EventNode.getAttribute("conditionID")).toInt();
	tempString = EventNode.getAttribute("conditionReferencedInVariable");
	conditionReferencedInVariable = (tempString == "true") ? true:false;
}

BaseEvent::ChangeCharacterCondition::ChangeCharacterCondition(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	characterID = storageFile.getSignedInt();
	conditionID = storageFile.getSignedInt();
	target = storageFile.getSignedInt();
	operation = storageFile.getSignedInt();
	conditionReferencedInVariable = storageFile.getBool();
}

BaseEvent::ChangeCharacterCondition::ChangeCharacterCondition(ChangeCharacterCondition *other, BaseEvent *pBase) : Event(pBase)
{
	characterID = other->characterID;
	conditionID = other->conditionID;
	target = other->target;
	operation = other->operation;
	conditionReferencedInVariable = other->conditionReferencedInVariable;
}
//END BaseEvent::ChangeCharacterCondition Member Functions

//BEGIN BaseEvent::ChangeCharacterHP Member Functions
XMLNode BaseEvent::ChangeCharacterHP::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	if (target == ENTIRE_PARTY)
		EventNode.addAttribute("target", "entireParty");
	else if (target == SPECIFIC_CHARACTER)
	{
		EventNode.addAttribute("target", "specificCharacter");
		EventNode.addAttribute("characterID", QString::number(characterID).toUtf8().data());
	}
	else if (target == REFERENCED_CHARACTER)
	{
		EventNode.addAttribute("target", "referencedCharacter");
		EventNode.addAttribute("variableID", QString::number(characterID).toUtf8().data());
	}
	EventNode.addAttribute("operation", (operation == INCREASE_HP) ? "increaseHP":"decreaseHP");
	EventNode.addAttribute("amount", QString::number(amount).toUtf8().data());
	EventNode.addAttribute("amountStoredInVariable", (amountStoredInVariable) ? "true":"false");
	return EventNode;
}

void BaseEvent::ChangeCharacterHP::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(characterID);
	storageFile.putSignedInt(amount);
	storageFile.putSignedInt(target);
	storageFile.putSignedInt(operation);
	storageFile.putBool(amountStoredInVariable);
}

QStringList BaseEvent::ChangeCharacterHP::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	QString targetText;
	QString operationText;
	QString amountText;
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Change Character HP: ";
	if (target == ENTIRE_PARTY)
		targetText = "Entire Party";
	else if (target == SPECIFIC_CHARACTER)
		targetText = ProjectData::characterNames[characterID];
	else
		targetText = QString("variable[%1]").arg(ProjectData::numberVariableIdToString(characterID));
	if (operation == INCREASE_HP)
		operationText = "+";
	else
		operationText = "-";
	if (amountStoredInVariable)
		amountText = QString("variable[%1]").arg(ProjectData::numberVariableIdToString(amount));
	else
		amountText = QString::number(amount);
	viewableText[0] += targetText + "\'s HP ";
	viewableText[0] += operationText;
	viewableText[0] += amountText;
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::ChangeCharacterHP::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	ConfigureEvent_ChangeCharacterHP *configChangeHP = new ConfigureEvent_ChangeCharacterHP;
	bool configured = false;
	configChangeHP->setTarget(target);
	configChangeHP->setCharacterID(characterID);
	configChangeHP->setOperation((operation == INCREASE_HP) ? true:false);
	configChangeHP->setIsStoredAmount(amountStoredInVariable);
	configChangeHP->setAmount(amount);
	if (configChangeHP->exec())
	{
		configured = true;
		target = configChangeHP->getTarget();
		characterID = configChangeHP->getCharacterID();
		operation = (configChangeHP->isIncreaseOperation()) ? INCREASE_HP:DECREASE_HP;
		amount = configChangeHP->getAmount();
		amountStoredInVariable = configChangeHP->isStoredAmount();
	}
	delete configChangeHP;
	return configured;
}
#endif

bool BaseEvent::ChangeCharacterHP::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	QStringList tempStringList;
	QString tempString;
	tempStringList.clear();
	tempStringList << "entireParty" << "specificCharacter" << "referencedCharacter";
	if (!XMLTools::attributeExists(EventNode, "target"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "target", tempStringList))
		return false;
	tempString = EventNode.getAttribute("target");
	if (tempString == "specificCharacter")
	{
		if (!XMLTools::attributeExists(EventNode, "characterID"))
			return false;
	}
	else if (tempString == "referencedCharacter")
	{
		if (!XMLTools::attributeExists(EventNode, "variableID"))
			return false;
	}
	tempStringList.clear();
	tempStringList << "increaseHP" << "decreaseHP";
	if (!XMLTools::attributeExists(EventNode, "operation"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "operation", tempStringList))
		return false;
	if (!XMLTools::attributeExists(EventNode, "amount"))
		return false;
	tempStringList.clear();
	tempStringList << "true" << "false";
	if (!XMLTools::attributeExists(EventNode, "amountStoredInVariable"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "amountStoredInVariable", tempStringList))
		return false;
	return true;
}

BaseEvent::ChangeCharacterHP::ChangeCharacterHP(BaseEvent *pBase) : Event(pBase)
{
	characterID = 0;
	amount = 0;
	target = ENTIRE_PARTY;
	operation = INCREASE_HP;
	amountStoredInVariable = false;
}

BaseEvent::ChangeCharacterHP::ChangeCharacterHP(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	QString tempString;
	tempString = EventNode.getAttribute("target");
	if (tempString == "entireParty")
	{
		target = ENTIRE_PARTY;
		characterID = 0;
	}
	else if (tempString == "specificCharacter")
	{
		target = SPECIFIC_CHARACTER;
		characterID = QString(EventNode.getAttribute("characterID")).toInt();
	}
	else if (tempString == "referencedCharacter")
	{
		target = REFERENCED_CHARACTER;
		characterID = QString(EventNode.getAttribute("variableID")).toInt();
	}
	tempString = EventNode.getAttribute("operation");
	operation = (tempString == "increaseHP") ? INCREASE_HP:DECREASE_HP;
	amount = QString(EventNode.getAttribute("amount")).toInt();
	tempString = EventNode.getAttribute("amountStoredInVariable");
	amountStoredInVariable = (tempString == "true") ? true:false;
}

BaseEvent::ChangeCharacterHP::ChangeCharacterHP(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	characterID = storageFile.getSignedInt();
	amount = storageFile.getSignedInt();
	target = storageFile.getSignedInt();
	operation = storageFile.getSignedInt();
	amountStoredInVariable = storageFile.getBool();
}

BaseEvent::ChangeCharacterHP::ChangeCharacterHP(ChangeCharacterHP *other, BaseEvent *pBase) : Event(pBase)
{
	characterID = other->characterID;
	amount = other->amount;
	target = other->target;
	operation = other->operation;
	amountStoredInVariable = other->amountStoredInVariable;
}
//END BaseEvent::ChangeCharacterHP Member Functions

//BEGIN BaseEvent::ChangeCharacterLevel Member Functions
XMLNode BaseEvent::ChangeCharacterLevel::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	if (target == ENTIRE_PARTY)
		EventNode.addAttribute("target", "entireParty");
	else if (target == SPECIFIC_CHARACTER)
	{
		EventNode.addAttribute("target", "specificCharacter");
		EventNode.addAttribute("characterID", QString::number(characterID).toUtf8().data());
	}
	else if (target == REFERENCED_CHARACTER)
	{
		EventNode.addAttribute("target", "referencedCharacter");
		EventNode.addAttribute("variableID", QString::number(characterID).toUtf8().data());
	}
	EventNode.addAttribute("operation", (operation == ADD_LEVELS) ? "addLevels":"removeLevels");
	EventNode.addAttribute("amount", QString::number(amount).toUtf8().data());
	EventNode.addAttribute("amountStoredInVariable", (amountStoredInVariable) ? "true":"false");
	EventNode.addAttribute("showLevelUpMessage", (showLevelUpMessage) ? "true":"false");
	return EventNode;
}

void BaseEvent::ChangeCharacterLevel::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(characterID);
	storageFile.putSignedInt(amount);
	storageFile.putSignedInt(target);
	storageFile.putSignedInt(operation);
	storageFile.putBool(amountStoredInVariable);
	storageFile.putBool(showLevelUpMessage);
}

QStringList BaseEvent::ChangeCharacterLevel::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	QString targetText;
	QString operationText;
	QString amountText;
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Change Character Level: ";
	if (target == ENTIRE_PARTY)
		targetText = "Entire Party";
	else if (target == SPECIFIC_CHARACTER)
		targetText = ProjectData::characterNames[characterID];
	else
		targetText = QString("variable[%1]").arg(ProjectData::numberVariableIdToString(characterID));
	if (operation == ADD_LEVELS)
		operationText = "+";
	else
		operationText = "-";
	if (amountStoredInVariable)
		amountText = QString("variable[%1]").arg(ProjectData::numberVariableIdToString(amount));
	else
		amountText = QString::number(amount);
	viewableText[0] += targetText + "\'s Level ";
	viewableText[0] += operationText;
	viewableText[0] += amountText;
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::ChangeCharacterLevel::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	ConfigureEvent_ChangeCharacterLevel *configChangeLevel = new ConfigureEvent_ChangeCharacterLevel;
	bool configured = false;
	configChangeLevel->setTarget(target);
	configChangeLevel->setCharacterID(characterID);
	configChangeLevel->setOperation((operation == ADD_LEVELS) ? true:false);
	configChangeLevel->setIsStoredAmount(amountStoredInVariable);
	configChangeLevel->setAmount(amount);
	configChangeLevel->setShowLevelUpMessage(showLevelUpMessage);
	if (configChangeLevel->exec())
	{
		configured = true;
		target = configChangeLevel->getTarget();
		characterID = configChangeLevel->getCharacterID();
		operation = (configChangeLevel->isAddOperation()) ? ADD_LEVELS:REMOVE_LEVELS;
		amount = configChangeLevel->getAmount();
		amountStoredInVariable = configChangeLevel->isStoredAmount();
		showLevelUpMessage = configChangeLevel->showLevelUpMessage();
	}
	delete configChangeLevel;
	return configured;
}
#endif

bool BaseEvent::ChangeCharacterLevel::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	QStringList tempStringList;
	QString tempString;
	tempStringList.clear();
	tempStringList << "entireParty" << "specificCharacter" << "referencedCharacter";
	if (!XMLTools::attributeExists(EventNode, "target"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "target", tempStringList))
		return false;
	tempString = EventNode.getAttribute("target");
	if (tempString == "specificCharacter")
	{
		if (!XMLTools::attributeExists(EventNode, "characterID"))
			return false;
	}
	else if (tempString == "referencedCharacter")
	{
		if (!XMLTools::attributeExists(EventNode, "variableID"))
			return false;
	}
	tempStringList.clear();
	tempStringList << "addLevels" << "removeLevels";
	if (!XMLTools::attributeExists(EventNode, "operation"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "operation", tempStringList))
		return false;
	if (!XMLTools::attributeExists(EventNode, "amount"))
		return false;
	tempStringList.clear();
	tempStringList << "true" << "false";
	if (!XMLTools::attributeExists(EventNode, "amountStoredInVariable"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "amountStoredInVariable", tempStringList))
		return false;
	if (!XMLTools::attributeExists(EventNode, "showLevelUpMessage"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "showLevelUpMessage", tempStringList))
		return false;
	return true;
}

BaseEvent::ChangeCharacterLevel::ChangeCharacterLevel(BaseEvent *pBase) : Event(pBase)
{
	characterID = 0;
	amount = 0;
	target = ENTIRE_PARTY;
	operation = ADD_LEVELS;
	amountStoredInVariable = false;
	showLevelUpMessage = false;
}

BaseEvent::ChangeCharacterLevel::ChangeCharacterLevel(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	QString tempString;
	tempString = EventNode.getAttribute("target");
	if (tempString == "entireParty")
	{
		target = ENTIRE_PARTY;
		characterID = 0;
	}
	else if (tempString == "specificCharacter")
	{
		target = SPECIFIC_CHARACTER;
		characterID = QString(EventNode.getAttribute("characterID")).toInt();
	}
	else if (tempString == "referencedCharacter")
	{
		target = REFERENCED_CHARACTER;
		characterID = QString(EventNode.getAttribute("variableID")).toInt();
	}
	tempString = EventNode.getAttribute("operation");
	operation = (tempString == "addLevels") ? ADD_LEVELS:REMOVE_LEVELS;
	amount = QString(EventNode.getAttribute("amount")).toInt();
	tempString = EventNode.getAttribute("amountStoredInVariable");
	amountStoredInVariable = (tempString == "true") ? true:false;
	tempString = EventNode.getAttribute("showLevelUpMessage");
	showLevelUpMessage = (tempString == "true") ? true:false;
}

BaseEvent::ChangeCharacterLevel::ChangeCharacterLevel(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	characterID = storageFile.getSignedInt();
	amount = storageFile.getSignedInt();
	target = storageFile.getSignedInt();
	operation = storageFile.getSignedInt();
	amountStoredInVariable = storageFile.getBool();
	showLevelUpMessage = storageFile.getBool();
}

BaseEvent::ChangeCharacterLevel::ChangeCharacterLevel(ChangeCharacterLevel *other, BaseEvent *pBase) : Event(pBase)
{
	characterID = other->characterID;
	amount = other->amount;
	target = other->target;
	operation = other->operation;
	amountStoredInVariable = other->amountStoredInVariable;
	showLevelUpMessage = other->showLevelUpMessage;
}
//END BaseEvent::ChangeCharacterLevel Member Functions

//BEGIN BaseEvent::ChangeCharacterMP Member Functions
XMLNode BaseEvent::ChangeCharacterMP::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	if (target == ENTIRE_PARTY)
		EventNode.addAttribute("target", "entireParty");
	else if (target == SPECIFIC_CHARACTER)
	{
		EventNode.addAttribute("target", "specificCharacter");
		EventNode.addAttribute("characterID", QString::number(characterID).toUtf8().data());
	}
	else if (target == REFERENCED_CHARACTER)
	{
		EventNode.addAttribute("target", "referencedCharacter");
		EventNode.addAttribute("variableID", QString::number(characterID).toUtf8().data());
	}
	EventNode.addAttribute("operation", (operation == INCREASE_MP) ? "increaseMP":"decreaseMP");
	EventNode.addAttribute("amount", QString::number(amount).toUtf8().data());
	EventNode.addAttribute("amountStoredInVariable", (amountStoredInVariable) ? "true":"false");
	return EventNode;
}

void BaseEvent::ChangeCharacterMP::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(characterID);
	storageFile.putSignedInt(amount);
	storageFile.putSignedInt(target);
	storageFile.putSignedInt(operation);
	storageFile.putBool(amountStoredInVariable);
}

QStringList BaseEvent::ChangeCharacterMP::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	QString targetText;
	QString operationText;
	QString amountText;
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Change Character MP: ";
	if (target == ENTIRE_PARTY)
		targetText = "Entire Party";
	else if (target == SPECIFIC_CHARACTER)
		targetText = ProjectData::characterNames[characterID];
	else
		targetText = QString("variable[%1]").arg(ProjectData::numberVariableIdToString(characterID));
	if (operation == INCREASE_MP)
		operationText = "+";
	else
		operationText = "-";
	if (amountStoredInVariable)
		amountText = QString("variable[%1]").arg(ProjectData::numberVariableIdToString(amount));
	else
		amountText = QString::number(amount);
	viewableText[0] += targetText + "\'s MP ";
	viewableText[0] += operationText;
	viewableText[0] += amountText;
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::ChangeCharacterMP::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	ConfigureEvent_ChangeCharacterMP *configChangeMP = new ConfigureEvent_ChangeCharacterMP;
	bool configured = false;
	configChangeMP->setTarget(target);
	configChangeMP->setCharacterID(characterID);
	configChangeMP->setOperation((operation == INCREASE_MP) ? true:false);
	configChangeMP->setIsStoredAmount(amountStoredInVariable);
	configChangeMP->setAmount(amount);
	if (configChangeMP->exec())
	{
		configured = true;
		target = configChangeMP->getTarget();
		characterID = configChangeMP->getCharacterID();
		operation = (configChangeMP->isIncreaseOperation()) ? INCREASE_MP:DECREASE_MP;
		amount = configChangeMP->getAmount();
		amountStoredInVariable = configChangeMP->isStoredAmount();
	}
	delete configChangeMP;
	return configured;
}
#endif

bool BaseEvent::ChangeCharacterMP::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	QStringList tempStringList;
	QString tempString;
	tempStringList.clear();
	tempStringList << "entireParty" << "specificCharacter" << "referencedCharacter";
	if (!XMLTools::attributeExists(EventNode, "target"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "target", tempStringList))
		return false;
	tempString = EventNode.getAttribute("target");
	if (tempString == "specificCharacter")
	{
		if (!XMLTools::attributeExists(EventNode, "characterID"))
			return false;
	}
	else if (tempString == "referencedCharacter")
	{
		if (!XMLTools::attributeExists(EventNode, "variableID"))
			return false;
	}
	tempStringList.clear();
	tempStringList << "increaseMP" << "decreaseMP";
	if (!XMLTools::attributeExists(EventNode, "operation"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "operation", tempStringList))
		return false;
	if (!XMLTools::attributeExists(EventNode, "amount"))
		return false;
	tempStringList.clear();
	tempStringList << "true" << "false";
	if (!XMLTools::attributeExists(EventNode, "amountStoredInVariable"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "amountStoredInVariable", tempStringList))
		return false;
	return true;
}

BaseEvent::ChangeCharacterMP::ChangeCharacterMP(BaseEvent *pBase) : Event(pBase)
{
	characterID = 0;
	amount = 0;
	target = ENTIRE_PARTY;
	operation = INCREASE_MP;
	amountStoredInVariable = false;
}

BaseEvent::ChangeCharacterMP::ChangeCharacterMP(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	QString tempString;
	tempString = EventNode.getAttribute("target");
	if (tempString == "entireParty")
	{
		target = ENTIRE_PARTY;
		characterID = 0;
	}
	else if (tempString == "specificCharacter")
	{
		target = SPECIFIC_CHARACTER;
		characterID = QString(EventNode.getAttribute("characterID")).toInt();
	}
	else if (tempString == "referencedCharacter")
	{
		target = REFERENCED_CHARACTER;
		characterID = QString(EventNode.getAttribute("variableID")).toInt();
	}
	tempString = EventNode.getAttribute("operation");
	operation = (tempString == "increaseMP") ? INCREASE_MP:DECREASE_MP;
	amount = QString(EventNode.getAttribute("amount")).toInt();
	tempString = EventNode.getAttribute("amountStoredInVariable");
	amountStoredInVariable = (tempString == "true") ? true:false;
}

BaseEvent::ChangeCharacterMP::ChangeCharacterMP(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	characterID = storageFile.getSignedInt();
	amount = storageFile.getSignedInt();
	target = storageFile.getSignedInt();
	operation = storageFile.getSignedInt();
	amountStoredInVariable = storageFile.getBool();
}

BaseEvent::ChangeCharacterMP::ChangeCharacterMP(ChangeCharacterMP *other, BaseEvent *pBase) : Event(pBase)
{
	characterID = other->characterID;
	amount = other->amount;
	target = other->target;
	operation = other->operation;
	amountStoredInVariable = other->amountStoredInVariable;
}
//END BaseEvent::ChangeCharacterMP Member Functions

//BEGIN BaseEvent::ChangeCharacterSprite Member Functions
XMLNode BaseEvent::ChangeCharacterSprite::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	EventNode.addAttribute("characterID", QString::number(characterID).toUtf8().data());
	EventNode.addAttribute("characterReferencedInVariable", (characterReferencedInVariable) ? "true":"false");
	EventNode.addAttribute("spriteID", QString::number(spriteID).toUtf8().data());
	EventNode.addAttribute("spriteReferencedInVariable", (spriteReferencedInVariable) ? "true":"false");
	return EventNode;
}

void BaseEvent::ChangeCharacterSprite::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(characterID);
	storageFile.putSignedInt(spriteID);
	storageFile.putBool(characterReferencedInVariable);
	storageFile.putBool(spriteReferencedInVariable);
}

QStringList BaseEvent::ChangeCharacterSprite::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Change Character Sprite: set ";
	if (characterReferencedInVariable)
		viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(characterID));
	else
		viewableText[0] += ProjectData::characterNames[characterID] + "\'s";
	viewableText[0] += " sprite to ";
	if (spriteReferencedInVariable)
		viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(spriteID));
	else
		viewableText[0] += ProjectData::characterSpriteNames[spriteID];
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::ChangeCharacterSprite::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	ConfigureEvent_ChangeCharacterSprite *configCharacterSprite = new ConfigureEvent_ChangeCharacterSprite;
	bool configured = false;
	configCharacterSprite->setIsReferencedCharacter(characterReferencedInVariable);
	configCharacterSprite->setCharacterID(characterID);
	configCharacterSprite->setIsReferencedSprite(spriteReferencedInVariable);
	configCharacterSprite->setSpriteID(spriteID);
	if (configCharacterSprite->exec())
	{
		configured = true;
		characterID = configCharacterSprite->getCharacterID();
		spriteID = configCharacterSprite->getSpriteID();
		characterReferencedInVariable = configCharacterSprite->isReferencedCharacter();
		spriteReferencedInVariable = configCharacterSprite->isReferencedSprite();
	}
	delete configCharacterSprite;
	return configured;
}
#endif

bool BaseEvent::ChangeCharacterSprite::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	QStringList trueFalseList = QStringList() << "true" << "false";
	if (!XMLTools::attributeExists(EventNode, "characterID"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "characterReferencedInVariable"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "characterReferencedInVariable", trueFalseList, 1))
		return false;
	if (!XMLTools::attributeExists(EventNode, "spriteID"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "spriteReferencedInVariable"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "spriteReferencedInVariable", trueFalseList, 1))
		return false;
	return true;
}

BaseEvent::ChangeCharacterSprite::ChangeCharacterSprite(BaseEvent *pBase) : Event(pBase)
{
	characterID = 0;
	spriteID = 0;
	characterReferencedInVariable = false;
	spriteReferencedInVariable = false;
}

BaseEvent::ChangeCharacterSprite::ChangeCharacterSprite(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	characterID = QString(EventNode.getAttribute("characterID")).toInt();
	spriteID = QString(EventNode.getAttribute("spriteID")).toInt();
	characterReferencedInVariable = (QString(EventNode.getAttribute("characterReferencedInVariable")) == "true") ? true:false;
	spriteReferencedInVariable = (QString(EventNode.getAttribute("spriteReferencedInVariable")) == "true") ? true:false;
}

BaseEvent::ChangeCharacterSprite::ChangeCharacterSprite(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	characterID = storageFile.getSignedInt();
	spriteID = storageFile.getSignedInt();
	characterReferencedInVariable = storageFile.getBool();
	spriteReferencedInVariable = storageFile.getBool();
}

BaseEvent::ChangeCharacterSprite::ChangeCharacterSprite(ChangeCharacterSprite *other, BaseEvent *pBase) : Event(pBase)
{
	characterID = other->characterID;
	spriteID = other->spriteID;
	characterReferencedInVariable = other->characterReferencedInVariable;
	spriteReferencedInVariable = other->spriteReferencedInVariable;
}
//END BaseEvent::ChangeCharacterSprite Member Functions

//BEGIN BaseEvent::ChangeClass Member Functions
XMLNode BaseEvent::ChangeClass::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	EventNode.addAttribute("characterID", QString::number(characterID).toUtf8().data());
	EventNode.addAttribute("characterReferencedInVariable", (characterReferencedInVariable) ? "true":"false");
	EventNode.addAttribute("classID", QString::number(classID).toUtf8().data());
	EventNode.addAttribute("classReferencedInVariable", (classReferencedInVariable) ? "true":"false");
	if (learnedSkills == LEARNED_NONE)
		EventNode.addAttribute("learnedSkills", "none");
	else if (learnedSkills == LEARNED_ADDUPTONEWDELETEOLD)
		EventNode.addAttribute("learnedSkills", "addUpToNewDeleteOld");
	else if (learnedSkills == LEARNED_ADDUPTONEWKEEPOLD)
		EventNode.addAttribute("learnedSkills", "addUpToNewKeepOld");
	if (baseStatChange == CHANGE_NONE)
		EventNode.addAttribute("baseStatChange", "none");
	else if (baseStatChange == CHANGE_HALVESTATS)
		EventNode.addAttribute("baseStatChange", "halveStats");
	else if (baseStatChange == CHANGE_TOLEVEL1OFCLASS)
		EventNode.addAttribute("baseStatChange", "toLevel1OfClass");
	else if (baseStatChange == CHANGE_TOCURRENTLEVELOFCLASS)
		EventNode.addAttribute("baseStatChange", "toCurrentLevelOfClass");
	EventNode.addAttribute("retainCurrentLevel", (retainCurrentLevel) ? "true":"false");
	EventNode.addAttribute("showLevelUpMessage", (showLevelUpMessage) ? "true":"false");
	return EventNode;
}

void BaseEvent::ChangeClass::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(characterID);
	storageFile.putSignedInt(classID);
	storageFile.putSignedInt(learnedSkills);
	storageFile.putSignedInt(baseStatChange);
	storageFile.putBool(characterReferencedInVariable);
	storageFile.putBool(classReferencedInVariable);
	storageFile.putBool(retainCurrentLevel);
	storageFile.putBool(showLevelUpMessage);
}

QStringList BaseEvent::ChangeClass::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Change Class: ";
	if (characterReferencedInVariable)
		viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(characterID));
	else
		viewableText[0] += ProjectData::characterNames[characterID];
	viewableText[0] += " to ";
	if (classReferencedInVariable)
		viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(classID));
	else
		viewableText[0] += ProjectData::classNames[classID];
	viewableText[0] += ", ";
	if (retainCurrentLevel)
		viewableText[0] += "No Level Change, ";
	else
		viewableText[0] += "Change to Level 1, ";
	if (learnedSkills == LEARNED_NONE)
		viewableText[0] += "No Skill Change, ";
	else if (learnedSkills == LEARNED_ADDUPTONEWDELETEOLD)
		viewableText[0] += "Add New Skills and Delete Old, ";
	else
		viewableText[0] += "Add New Skills but Keep Old, ";
	if (baseStatChange == CHANGE_NONE)
		viewableText[0] += "No Stat Change";
	else if (baseStatChange == CHANGE_HALVESTATS)
		viewableText[0] += "Halve Stats";
	else if (baseStatChange == CHANGE_TOLEVEL1OFCLASS)
		viewableText[0] += "Stats to Lvl1 of New Class";
	else
		viewableText[0] += "Stats to Current Level of New Class";
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::ChangeClass::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	ConfigureEvent_ChangeClass *configClass = new ConfigureEvent_ChangeClass;
	bool configured = false;
	configClass->setIsReferencedCharacter(characterReferencedInVariable);
	configClass->setCharacterID(characterID);
	configClass->setIsReferencedClass(classReferencedInVariable);
	configClass->setClassID(classID);
	configClass->setRetainCurrentLevel(retainCurrentLevel);
	configClass->setBaseStatChange(baseStatChange);
	configClass->setLearnedSkills(learnedSkills);
	configClass->setShowLevelUpMessage(showLevelUpMessage);
	if (configClass->exec())
	{
		configured = true;
		characterID = configClass->getCharacterID();
		characterReferencedInVariable = configClass->isReferencedCharacter();
		classID = configClass->getClassID();
		classReferencedInVariable = configClass->isReferencedClass();
		retainCurrentLevel = configClass->retainCurrentLevel();
		baseStatChange = configClass->getBaseStatChange();
		learnedSkills = configClass->getLearnedSkills();
		showLevelUpMessage = configClass->showLevelUpMessage();
	}
	delete configClass;
	return configured;
}
#endif

bool BaseEvent::ChangeClass::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	QStringList tempStringList;
	if (!XMLTools::attributeExists(EventNode, "characterID"))
		return false;
	tempStringList.clear();
	tempStringList << "true" << "false";
	if (!XMLTools::attributeExists(EventNode, "characterReferencedInVariable"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "characterReferencedInVariable", tempStringList, 1))
		return false;
	if (!XMLTools::attributeExists(EventNode, "classID"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "classReferencedInVariable"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "classReferencedInVariable", tempStringList, 1))
		return false;
	tempStringList.clear();
	tempStringList << "none" << "addUpToNewDeleteOld" << "addUpToNewKeepOld";
	if (!XMLTools::attributeExists(EventNode, "learnedSkills"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "learnedSkills", tempStringList))
		return false;
	tempStringList.clear();
	tempStringList << "none" << "halveStats" << "toLevel1OfClass" << "toCurrentLevelOfClass";
	if (!XMLTools::attributeExists(EventNode, "baseStatChange"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "baseStatChange", tempStringList))
		return false;
	tempStringList.clear();
	tempStringList << "true" << "false";
	if (!XMLTools::attributeExists(EventNode, "retainCurrentLevel"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "retainCurrentLevel", tempStringList, 1))
		return false;
	if (!XMLTools::attributeExists(EventNode, "showLevelUpMessage"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "showLevelUpMessage", tempStringList, 1))
		return false;
	return true;
}

BaseEvent::ChangeClass::ChangeClass(BaseEvent *pBase) : Event(pBase)
{
	characterID = 0;
	classID = 0;
	learnedSkills = LEARNED_NONE;
	baseStatChange = CHANGE_NONE;
	characterReferencedInVariable = false;
	classReferencedInVariable = false;
	retainCurrentLevel = false;
	showLevelUpMessage = false;
}

BaseEvent::ChangeClass::ChangeClass(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	QString tempString;
	characterID = QString(EventNode.getAttribute("characterID")).toInt();
	tempString = EventNode.getAttribute("characterReferencedInVariable");
	characterReferencedInVariable = (tempString == "true") ? true:false;
	classID = QString(EventNode.getAttribute("classID")).toInt();
	tempString = EventNode.getAttribute("classReferencedInVariable");
	classReferencedInVariable = (tempString == "true") ? true:false;
	tempString = EventNode.getAttribute("learnedSkills");
	if (tempString == "none")
		learnedSkills = LEARNED_NONE;
	else if (tempString == "addUpToNewDeleteOld")
		learnedSkills = LEARNED_ADDUPTONEWDELETEOLD;
	else if (tempString == "addUpToNewKeepOld")
		learnedSkills = LEARNED_ADDUPTONEWKEEPOLD;
	tempString = EventNode.getAttribute("baseStatChange");
	if (tempString == "none")
		baseStatChange = CHANGE_NONE;
	else if (tempString == "halveStats")
		baseStatChange = CHANGE_HALVESTATS;
	else if (tempString == "toLevel1OfClass")
		baseStatChange = CHANGE_TOLEVEL1OFCLASS;
	else if (tempString == "toCurrentLevelOfClass")
		baseStatChange = CHANGE_TOCURRENTLEVELOFCLASS;
	tempString = EventNode.getAttribute("retainCurrentLevel");
	retainCurrentLevel = (tempString == "true") ? true:false;
	tempString = EventNode.getAttribute("showLevelUpMessage");
	showLevelUpMessage = (tempString == "true") ? true:false;
}

BaseEvent::ChangeClass::ChangeClass(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	characterID = storageFile.getSignedInt();
	classID = storageFile.getSignedInt();
	learnedSkills = storageFile.getSignedInt();
	baseStatChange = storageFile.getSignedInt();
	characterReferencedInVariable = storageFile.getBool();
	classReferencedInVariable = storageFile.getBool();
	retainCurrentLevel = storageFile.getBool();
	showLevelUpMessage = storageFile.getBool();
}

BaseEvent::ChangeClass::ChangeClass(ChangeClass *other, BaseEvent *pBase) : Event(pBase)
{
	characterID = other->characterID;
	classID = other->classID;
	learnedSkills = other->learnedSkills;
	baseStatChange = other->baseStatChange;
	characterReferencedInVariable = other->characterReferencedInVariable;
	classReferencedInVariable = other->classReferencedInVariable;
	retainCurrentLevel = other->retainCurrentLevel;
	showLevelUpMessage = other->showLevelUpMessage;
}
//END BaseEvent::ChangeClass Member Functions

//BEGIN BaseEvent::ChangeEncounterRate Member Functions
XMLNode BaseEvent::ChangeEncounterRate::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	EventNode.addAttribute("encounterRate", QString::number(encounterRate).toUtf8().data());
	EventNode.addAttribute("minSteps", QString::number(minSteps).toUtf8().data());
	EventNode.addAttribute("maxSteps", QString::number(maxSteps).toUtf8().data());
	return EventNode;
}

void BaseEvent::ChangeEncounterRate::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(encounterRate);
	storageFile.putSignedInt(minSteps);
	storageFile.putSignedInt(maxSteps);
}

QStringList BaseEvent::ChangeEncounterRate::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Change Encounter Rate: rate=";
	viewableText[0] += QString("%1% steps=").arg(encounterRate);
	viewableText[0] += QString("%1-%2").arg(minSteps).arg(maxSteps);
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::ChangeEncounterRate::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	ConfigureEvent_ChangeEncounterRate *configRate = new ConfigureEvent_ChangeEncounterRate();
	bool configured = false;
	configRate->setEncounterRate(encounterRate);
	configRate->setMinSteps(minSteps);
	configRate->setMaxSteps(maxSteps);
	if (configRate->exec())
	{
		configured = true;
		encounterRate = configRate->getEncounterRate();
		minSteps = configRate->getMinSteps();
		maxSteps = configRate->getMaxSteps();
	}
	delete configRate;
	return configured;
}
#endif

bool BaseEvent::ChangeEncounterRate::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	if (!XMLTools::attributeExists(EventNode, "encounterRate"))
		return false;
	if (!XMLTools::attributeIntRangeValid(EventNode, "encounterRate", 0, 100))
		return false;
	if (!XMLTools::attributeExists(EventNode, "minSteps"))
		return false;
	if (!XMLTools::attributeIntRangeValid(EventNode, "minSteps", 0, 100))
		return false;
	if (!XMLTools::attributeExists(EventNode, "maxSteps"))
		return false;
	if (!XMLTools::attributeIntRangeValid(EventNode, "maxSteps", 0, 100))
		return false;
	return true;
}

BaseEvent::ChangeEncounterRate::ChangeEncounterRate(BaseEvent *pBase) : Event(pBase)
{
	encounterRate = 0;
	minSteps = 0;
	maxSteps = 100;
}

BaseEvent::ChangeEncounterRate::ChangeEncounterRate(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	encounterRate = QString(EventNode.getAttribute("encounterRate")).toInt();
	minSteps = QString(EventNode.getAttribute("minSteps")).toInt();
	maxSteps = QString(EventNode.getAttribute("maxSteps")).toInt();
}

BaseEvent::ChangeEncounterRate::ChangeEncounterRate(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	encounterRate = storageFile.getSignedInt();
	minSteps = storageFile.getSignedInt();
	maxSteps = storageFile.getSignedInt();
}

BaseEvent::ChangeEncounterRate::ChangeEncounterRate(ChangeEncounterRate *other, BaseEvent *pBase) : Event(pBase)
{
	encounterRate = other->encounterRate;
	minSteps = other->minSteps;
	maxSteps = other->maxSteps;
}
//END BaseEvent::ChangeEncounterRate Member Functions

//BEGIN BaseEvent::ChangeEquippedItems Member Functions
XMLNode BaseEvent::ChangeEquippedItems::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	if (target == ENTIRE_PARTY)
		EventNode.addAttribute("target", "entireParty");
	else if (target == SPECIFIC_CHARACTER)
	{
		EventNode.addAttribute("target", "specificCharacter");
		EventNode.addAttribute("characterID", QString::number(characterID).toUtf8().data());
	}
	else if (target == REFERENCED_CHARACTER)
	{
		EventNode.addAttribute("target", "referencedCharacter");
		EventNode.addAttribute("variableID", QString::number(characterID).toUtf8().data());
	}
	if (operation == EQUIP_ITEM)
	{
		EventNode.addAttribute("operation", "equip");
		EventNode.addAttribute("itemID", QString::number(itemID).toUtf8().data());
		EventNode.addAttribute("itemReferencedInVariable", (itemReferencedInVariable) ? "true":"false");
	}
	else
	{
		EventNode.addAttribute("operation", "unequip");
		if (unequippedSlot == ALL)
			EventNode.addAttribute("slot", "all");
		else if (unequippedSlot == WEAPON)
			EventNode.addAttribute("slot", "weapon");
		else if (unequippedSlot == SHIELD)
			EventNode.addAttribute("slot", "shield");
		else if (unequippedSlot == ARMOR)
			EventNode.addAttribute("slot", "armor");
		else if (unequippedSlot == HELMET)
			EventNode.addAttribute("slot", "helmet");
		else if (unequippedSlot == ACCESSORY)
			EventNode.addAttribute("slot", "accessory");
	}
	return EventNode;
}

void BaseEvent::ChangeEquippedItems::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(characterID);
	storageFile.putSignedInt(itemID);
	storageFile.putSignedInt(target);
	storageFile.putSignedInt(operation);
	storageFile.putSignedInt(unequippedSlot);
	storageFile.putBool(itemReferencedInVariable);
}

QStringList BaseEvent::ChangeEquippedItems::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	QString targetText;
	QString operationText1;
	QString operationText2;
	QString itemText;
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] = "Change Equipped Items: ";
	if (target == ENTIRE_PARTY)
		targetText = "Entire Party";
	else if (target == SPECIFIC_CHARACTER)
		targetText = ProjectData::characterNames[characterID];
	else
		targetText = QString("variable[%1]").arg(ProjectData::numberVariableIdToString(characterID));
	if (operation == EQUIP_ITEM)
	{
		operationText1 = "Equip";
		operationText2 = " with ";
		if (itemReferencedInVariable)
			itemText = QString("variable[%1]").arg(ProjectData::numberVariableIdToString(itemID));
		else
			itemText = ProjectData::itemNames[itemID];
	}
	else
	{
		operationText1 = "Unequip";
		operationText2 = "";
		if (unequippedSlot == ALL)
			itemText = "";
		else if (unequippedSlot == WEAPON)
			itemText = "\'s Weapon";
		else if (unequippedSlot == SHIELD)
			itemText = "\'s Shield";
		else if (unequippedSlot == ARMOR)
			itemText = "\'s Armor";
		else if (unequippedSlot == HELMET)
			itemText = "\'s Helmet";
		else if (unequippedSlot == ACCESSORY)
			itemText = "\'s Accessory";
	}
	viewableText[0] += operationText1 + " ";
	viewableText[0] += targetText;
	viewableText[0] += operationText2;
	viewableText[0] += itemText;
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::ChangeEquippedItems::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	ConfigureEvent_ChangeEquippedItems *configItems = new ConfigureEvent_ChangeEquippedItems;
	bool configured = false;
	configItems->setTarget(target);
	configItems->setCharacterID(characterID);
	configItems->setOperation((operation == EQUIP_ITEM) ? true:false);
	configItems->setItemID(itemID);
	configItems->setIsReferencedItem(itemReferencedInVariable);
	configItems->setUnequippedSlot(unequippedSlot);
	if (configItems->exec())
	{
		configured = true;
		target = configItems->getTarget();
		characterID = configItems->getCharacterID();
		operation = (configItems->isEquipOperation()) ? EQUIP_ITEM:UNEQUIP_ITEM;
		itemID = configItems->getItemID();
		itemReferencedInVariable = configItems->isReferencedItem();
		unequippedSlot = configItems->getUnequippedSlot();
	}
	delete configItems;
	return configured;
}
#endif

bool BaseEvent::ChangeEquippedItems::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	QStringList tempStringList;
	QString tempString;
	tempStringList.clear();
	tempStringList << "entireParty" << "specificCharacter" << "referencedCharacter";
	if (!XMLTools::attributeExists(EventNode, "target"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "target", tempStringList))
		return false;
	tempString = EventNode.getAttribute("target");
	if (tempString == "specificCharacter")
	{
		if (!XMLTools::attributeExists(EventNode, "characterID"))
			return false;
	}
	else if (tempString == "referencedCharacter")
	{
		if (!XMLTools::attributeExists(EventNode, "variableID"))
			return false;
	}
	tempStringList.clear();
	tempStringList << "equip" << "unequip";
	if (!XMLTools::attributeExists(EventNode, "operation"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "operation", tempStringList))
		return false;
	tempString = EventNode.getAttribute("operation");
	if (tempString == "equip")
	{
		if (!XMLTools::attributeExists(EventNode, "itemID"))
			return false;
		tempStringList.clear();
		tempStringList << "true" << "false";
		if (!XMLTools::attributeExists(EventNode ,"itemReferencedInVariable"))
			return false;
		if (!XMLTools::attributeStringValid(EventNode, "itemReferencedInVariable", tempStringList, 1))
			return false;
	}
	else if (tempString == "unequip")
	{
		tempStringList.clear();
		tempStringList << "all" << "weapon" << "shield" << "armor" << "helmet" << "accessory";
		if (!XMLTools::attributeExists(EventNode, "slot"))
			return false;
		if (!XMLTools::attributeStringValid(EventNode, "slot", tempStringList))
			return false;
	}
	return true;
}

BaseEvent::ChangeEquippedItems::ChangeEquippedItems(BaseEvent *pBase) : Event(pBase)
{
	characterID = 0;
	itemID = 0;
	target = ENTIRE_PARTY;
	operation = EQUIP_ITEM;
	unequippedSlot = ALL;
	itemReferencedInVariable = false;
}

BaseEvent::ChangeEquippedItems::ChangeEquippedItems(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	QString tempString;
	tempString = EventNode.getAttribute("target");
	if (tempString == "entireParty")
	{
		target = ENTIRE_PARTY;
		characterID = 0;
	}
	else if (tempString == "specificCharacter")
	{
		target = SPECIFIC_CHARACTER;
		characterID = QString(EventNode.getAttribute("characterID")).toInt();
	}
	else if (tempString == "referencedCharacter")
	{
		target = REFERENCED_CHARACTER;
		characterID = QString(EventNode.getAttribute("variableID")).toInt();
	}
	tempString = EventNode.getAttribute("operation");
	if (tempString == "equip")
	{
		operation = EQUIP_ITEM;
		itemID = QString(EventNode.getAttribute("itemID")).toInt();
		tempString = EventNode.getAttribute("itemReferencedInVariable");
		itemReferencedInVariable = (tempString == "true") ? true:false;
		unequippedSlot = ALL;
	}
	else if (tempString == "unequip")
	{
		operation = UNEQUIP_ITEM;
		tempString = EventNode.getAttribute("slot");
		if (tempString == "all")
			unequippedSlot = ALL;
		else if (tempString == "weapon")
			unequippedSlot = WEAPON;
		else if (tempString == "shield")
			unequippedSlot = SHIELD;
		else if (tempString == "armor")
			unequippedSlot = ARMOR;
		else if (tempString == "helmet")
			unequippedSlot = HELMET;
		else if (tempString == "accessory")
			unequippedSlot = ACCESSORY;
		itemID = 0;
		itemReferencedInVariable = false;
	}
}

BaseEvent::ChangeEquippedItems::ChangeEquippedItems(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	characterID = storageFile.getSignedInt();
	itemID = storageFile.getSignedInt();
	target = storageFile.getSignedInt();
	operation = storageFile.getSignedInt();
	unequippedSlot = storageFile.getSignedInt();
	itemReferencedInVariable = storageFile.getBool();
}

BaseEvent::ChangeEquippedItems::ChangeEquippedItems(ChangeEquippedItems *other, BaseEvent *pBase) : Event(pBase)
{
	characterID = other->characterID;
	itemID = other->itemID;
	target = other->target;
	operation = other->operation;
	unequippedSlot = other->unequippedSlot;
	itemReferencedInVariable = other->itemReferencedInVariable;
}
//END BaseEvent::ChangeEquippedItems Member Functions

//BEGIN BaseEvent::ChangeEventLocation Member Functions
XMLNode BaseEvent::ChangeEventLocation::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	EventNode.addAttribute("eventID", QString::number(eventID).toUtf8().data());
	EventNode.addAttribute("eventReferencedInVariable", (eventReferencedInVariable) ? "true":"false");
	EventNode.addAttribute("x", QString::number(x).toUtf8().data());
	EventNode.addAttribute("y", QString::number(y).toUtf8().data());
	EventNode.addAttribute("layer", QString::number(layer).toUtf8().data());
	EventNode.addAttribute("locationStoredInVariables", (locationStoredInVariables) ? "true":"false");
	return EventNode;
}

void BaseEvent::ChangeEventLocation::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(eventID);
	storageFile.putSignedInt(x);
	storageFile.putSignedInt(y);
	storageFile.putSignedInt(layer);
	storageFile.putBool(eventReferencedInVariable);
	storageFile.putBool(locationStoredInVariables);
}

QStringList BaseEvent::ChangeEventLocation::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Change Event Location: event=";
	if (eventReferencedInVariable)
		viewableText[0] += QString("[%1] location=").arg(ProjectData::numberVariableIdToString(eventID));
	else if (eventID == -1)
		viewableText[0] += "this location=";
	else
		viewableText[0] += QString("%1 location=").arg(eventID);
	if (locationStoredInVariables)
		viewableText[0] += QString("(variable[%1], variable[%2]) layer=variable[%3]").arg(ProjectData::numberVariableIdToString(x)).arg(ProjectData::numberVariableIdToString(y)).arg(ProjectData::numberVariableIdToString(layer));
	else
		viewableText[0] += QString("(%1, %2) layer=%3").arg(x).arg(y).arg(layer);
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::ChangeEventLocation::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerType);
	ConfigureEvent_ChangeEventLocation *configLocation = new ConfigureEvent_ChangeEventLocation(callerID);
	bool configured = false;
	configLocation->setEventReferencedInVariable(eventReferencedInVariable);
	configLocation->setEventID(eventID);
	configLocation->setLocationStoredInVariables(locationStoredInVariables);
	configLocation->setLocation(x, y, layer);
	if (configLocation->exec())
	{
		configured = true;
		eventID = configLocation->getEventID();
		eventReferencedInVariable = configLocation->isEventReferencedInVariable();
		x = configLocation->getXLocation();
		y = configLocation->getYLocation();
		layer = configLocation->getLayer();
		locationStoredInVariables = configLocation->isLocationStoredInVariables();
	}
	delete configLocation;
	return configured;
}
#endif

bool BaseEvent::ChangeEventLocation::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	QStringList trueFalseList = QStringList() << "true" << "false";
	if (!XMLTools::attributeExists(EventNode, "eventID"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "eventReferencedInVariable"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "eventReferencedInVariable", trueFalseList, 1))
		return false;
	if (!XMLTools::attributeExists(EventNode, "x"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "y"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "layer"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "locationStoredInVariables"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "locationStoredInVariables", trueFalseList, 1))
		return false;
	return true;
}

BaseEvent::ChangeEventLocation::ChangeEventLocation(BaseEvent *pBase) : Event(pBase)
{
	eventID = -1;
	x = 0;
	y = 0;
	layer = 0;
	eventReferencedInVariable = false;
	locationStoredInVariables = false;
}

BaseEvent::ChangeEventLocation::ChangeEventLocation(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	eventID = QString(EventNode.getAttribute("eventID")).toInt();
	eventReferencedInVariable = (QString(EventNode.getAttribute("eventReferencedInVariable")) == "true") ? true:false;
	x = QString(EventNode.getAttribute("x")).toInt();
	y = QString(EventNode.getAttribute("y")).toInt();
	layer = QString(EventNode.getAttribute("layer")).toInt();
	locationStoredInVariables = (QString(EventNode.getAttribute("locationStoredInVariables")) == "true") ? true:false;
}

BaseEvent::ChangeEventLocation::ChangeEventLocation(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	eventID = storageFile.getSignedInt();
	x = storageFile.getSignedInt();
	y = storageFile.getSignedInt();
	layer = storageFile.getSignedInt();
	eventReferencedInVariable = storageFile.getBool();
	locationStoredInVariables = storageFile.getBool();
}

BaseEvent::ChangeEventLocation::ChangeEventLocation(ChangeEventLocation *other, BaseEvent *pBase) : Event(pBase)
{
	eventID = other->eventID;
	x = other->x;
	y = other->y;
	layer = other->layer;
	eventReferencedInVariable = other->eventReferencedInVariable;
	locationStoredInVariables = other->locationStoredInVariables;
}
//END BaseEvent::ChangeEventLocation Member Functions

//BEGIN BaseEvent::ChangeExperiencePoints Member Functions
XMLNode BaseEvent::ChangeExperiencePoints::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	if (target == ENTIRE_PARTY)
		EventNode.addAttribute("target", "entireParty");
	else if (target == SPECIFIC_CHARACTER)
	{
		EventNode.addAttribute("target", "specificCharacter");
		EventNode.addAttribute("characterID", QString::number(characterID).toUtf8().data());
	}
	else if (target == REFERENCED_CHARACTER)
	{
		EventNode.addAttribute("target", "referencedCharacter");
		EventNode.addAttribute("variableID", QString::number(characterID).toUtf8().data());
	}
	if (operation == INCREASE_EXPERIENCE)
		EventNode.addAttribute("operation", "increaseExperience");
	else if (operation == REDUCE_EXPERIENCE)
		EventNode.addAttribute("operation", "reduceExperience");
	EventNode.addAttribute("amount", QString::number(amount).toUtf8().data());
	EventNode.addAttribute("amountStoredInVariable", (amountStoredInVariable) ? "true":"false");
	EventNode.addAttribute("showLevelUpMessage", (showLevelUpMessage) ? "true":"false");
	return EventNode;
}

void BaseEvent::ChangeExperiencePoints::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(characterID);
	storageFile.putSignedInt(amount);
	storageFile.putSignedInt(target);
	storageFile.putSignedInt(operation);
	storageFile.putBool(amountStoredInVariable);
	storageFile.putBool(showLevelUpMessage);
}

QStringList BaseEvent::ChangeExperiencePoints::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Change Experience Points: ";
	if (operation == INCREASE_EXPERIENCE)
		viewableText[0] += "Increase ";
	if (target == ENTIRE_PARTY)
		viewableText[0] += "Entire Party";
	else if (target == SPECIFIC_CHARACTER)
		viewableText[0] += ProjectData::characterNames[characterID];
	else
		viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(characterID));
	viewableText[0] += "\'s Experience by ";
	if (amountStoredInVariable)
		viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(amount));
	else
		viewableText[0] += QString::number(amount);
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::ChangeExperiencePoints::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	ConfigureEvent_ChangeExperiencePoints *configExperience = new ConfigureEvent_ChangeExperiencePoints;
	bool configured = false;
	configExperience->setTarget(target);
	configExperience->setCharacterID(characterID);
	configExperience->setOperation((operation == INCREASE_EXPERIENCE) ? true:false);
	configExperience->setIsStoredAmount(amountStoredInVariable);
	configExperience->setAmount(amount);
	configExperience->setShowLevelUpMessage(showLevelUpMessage);
	if (configExperience->exec())
	{
		configured = true;
		target = configExperience->getTarget();
		characterID = configExperience->getCharacterID();
		operation = (configExperience->isIncreaseOperation()) ? INCREASE_EXPERIENCE:REDUCE_EXPERIENCE;
		amount = configExperience->getAmount();
		amountStoredInVariable = configExperience->isStoredAmount();
		showLevelUpMessage = configExperience->showLevelUpMessage();
	}
	delete configExperience;
	return configured;
}
#endif

bool BaseEvent::ChangeExperiencePoints::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	QStringList tempStringList;
	QString tempString;
	tempStringList.clear();
	tempStringList << "entireParty" << "specificCharacter" << "referencedCharacter";
	if (!XMLTools::attributeExists(EventNode, "target"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "target", tempStringList))
		return false;
	tempString = EventNode.getAttribute("target");
	if (tempString == "specificCharacter")
	{
		if (!XMLTools::attributeExists(EventNode, "characterID"))
			return false;
	}
	else if (tempString == "referencedCharacter")
	{
		if (!XMLTools::attributeExists(EventNode, "variableID"))
			return false;
	}
	tempStringList.clear();
	tempStringList << "increaseExperience" << "reduceExperience";
	if (!XMLTools::attributeExists(EventNode, "operation"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "operation", tempStringList))
		return false;
	if (!XMLTools::attributeExists(EventNode, "amount"))
		return false;
	tempStringList.clear();
	tempStringList << "true" << "false";
	if (!XMLTools::attributeExists(EventNode, "amountStoredInVariable"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "amountStoredInVariable", tempStringList, 1))
		return false;
	if (!XMLTools::attributeExists(EventNode, "showLevelUpMessage"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "showLevelUpMessage", tempStringList, 1))
		return false;
	return true;
}

BaseEvent::ChangeExperiencePoints::ChangeExperiencePoints(BaseEvent *pBase) : Event(pBase)
{
	characterID = 0;
	amount = 0;
	target = ENTIRE_PARTY;
	operation = INCREASE_EXPERIENCE;
	amountStoredInVariable = false;
	showLevelUpMessage = false;
}

BaseEvent::ChangeExperiencePoints::ChangeExperiencePoints(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	QString tempString;
	tempString = EventNode.getAttribute("target");
	if (tempString == "entireParty")
	{
		target = ENTIRE_PARTY;
		characterID = 0;
	}
	else if (tempString == "specificCharacter")
	{
		target = SPECIFIC_CHARACTER;
		characterID = QString(EventNode.getAttribute("characterID")).toInt();
	}
	else if (tempString == "referencedCharacter")
	{
		target = REFERENCED_CHARACTER;
		characterID = QString(EventNode.getAttribute("variableID")).toInt();
	}
	tempString = EventNode.getAttribute("operation");
	operation = (tempString == "increaseExperience") ? INCREASE_EXPERIENCE:REDUCE_EXPERIENCE;
	amount = QString(EventNode.getAttribute("amount")).toInt();
	tempString = EventNode.getAttribute("amountStoredInVariable");
	amountStoredInVariable = (tempString == "true") ? true:false;
	tempString = EventNode.getAttribute("showLevelUpMessage");
	showLevelUpMessage = (tempString == "true") ? true:false;
}

BaseEvent::ChangeExperiencePoints::ChangeExperiencePoints(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	characterID = storageFile.getSignedInt();
	amount = storageFile.getSignedInt();
	target = storageFile.getSignedInt();
	operation = storageFile.getSignedInt();
	amountStoredInVariable = storageFile.getBool();
	showLevelUpMessage = storageFile.getBool();
}

BaseEvent::ChangeExperiencePoints::ChangeExperiencePoints(ChangeExperiencePoints *other, BaseEvent *pBase) : Event(pBase)
{
	characterID = other->characterID;
	amount = other->amount;
	target = other->target;
	operation = other->operation;
	amountStoredInVariable = other->amountStoredInVariable;
	showLevelUpMessage = other->showLevelUpMessage;
}
//END BaseEvent::ChangeExperiencePoints Member Functions

//BEGIN BaseEvent::ChangeFaceGraphic Member Functions
XMLNode BaseEvent::ChangeFaceGraphic::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	EventNode.addAttribute("characterID", QString::number(characterID).toUtf8().data());
	EventNode.addAttribute("characterReferencedInVariable", (characterReferencedInVariable) ? "true":"false");
	EventNode.addAttribute("faceLocation", ProjectData::getRelativeResourcePath(faceLocation).toUtf8().data());
	return EventNode;
}

void BaseEvent::ChangeFaceGraphic::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putString(faceLocation.toUtf8().data());
	storageFile.putSignedInt(characterID);
	storageFile.putBool(characterReferencedInVariable);
}

QStringList BaseEvent::ChangeFaceGraphic::getResources()
{
	return QStringList() << faceLocation;
}

QStringList BaseEvent::ChangeFaceGraphic::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	QString location = "";
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Change Face Graphic: Set ";
	if (characterReferencedInVariable)
		viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(characterID));
	else
		viewableText[0] += ProjectData::characterNames[characterID];
	viewableText[0] += " to \"";
	viewableText[0] += ProjectData::getRelativeResourcePath(faceLocation);
	viewableText[0] += "\"";
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::ChangeFaceGraphic::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	ConfigureEvent_ChangeFaceGraphic *configFaceGraphic = new ConfigureEvent_ChangeFaceGraphic;
	bool configured = false;
	configFaceGraphic->setIsReferencedCharacter(characterReferencedInVariable);
	configFaceGraphic->setCharacterID(characterID);
	configFaceGraphic->setFaceGraphicLocation(faceLocation);
	if (configFaceGraphic->exec())
	{
		configured = true;
		characterID = configFaceGraphic->getCharacterID();
		characterReferencedInVariable = configFaceGraphic->isReferencedCharacter();
		faceLocation = configFaceGraphic->getFaceGraphicLocation();
	}
	delete configFaceGraphic;
	return configured;
}
#endif

bool BaseEvent::ChangeFaceGraphic::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	QStringList trueFalseList = QStringList() << "true" << "false";
	if (!XMLTools::attributeExists(EventNode, "characterID"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "characterReferencedInVariable"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "characterReferencedInVariable", trueFalseList, 1))
		return false;
	if (!XMLTools::attributeExists(EventNode, "faceLocation"))
		return false;
	return true;
}

BaseEvent::ChangeFaceGraphic::ChangeFaceGraphic(BaseEvent *pBase) : Event(pBase)
{
	faceLocation = "";
	characterID = 0;
	characterReferencedInVariable = false;
}

BaseEvent::ChangeFaceGraphic::ChangeFaceGraphic(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	characterID = QString(EventNode.getAttribute("characterID")).toInt();
	characterReferencedInVariable = (QString(EventNode.getAttribute("characterReferencedInVariable")) == "true") ? true:false;
	faceLocation = ProjectData::getAbsoluteResourcePath(EventNode.getAttribute("faceLocation"));
}

BaseEvent::ChangeFaceGraphic::ChangeFaceGraphic(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	char *temp_string;
	temp_string = storageFile.getString();
	faceLocation = temp_string;
	delete[] temp_string;
	characterID = storageFile.getSignedInt();
	characterReferencedInVariable = storageFile.getBool();
}

BaseEvent::ChangeFaceGraphic::ChangeFaceGraphic(ChangeFaceGraphic *other, BaseEvent *pBase) : Event(pBase)
{
	faceLocation = other->faceLocation;
	characterID = other->characterID;
	characterReferencedInVariable = other->characterReferencedInVariable;
}
//END BaseEvent::ChangeFaceGraphic Member Functions

//BEGIN BaseEvent::ChangeHeroName Member Functions
XMLNode BaseEvent::ChangeHeroName::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	EventNode.addAttribute("characterID", QString::number(characterID).toUtf8().data());
	EventNode.addAttribute("characterReferencedInVariable", (characterReferencedInVariable) ? "true":"false");
	if (stringID == -1)
		EventNode.addAttribute("newName", newName.toUtf8().data());
	else
		EventNode.addAttribute("stringID", QString::number(stringID).toUtf8().data());
	return EventNode;
}

void BaseEvent::ChangeHeroName::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putString(newName.toUtf8().data());
	storageFile.putSignedInt(characterID);
	storageFile.putSignedInt(stringID);
	storageFile.putBool(characterReferencedInVariable);
}

QStringList BaseEvent::ChangeHeroName::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Change Hero Name: Change ";
	if (characterReferencedInVariable)
		viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(characterID));
	else
		viewableText[0] += ProjectData::characterNames[characterID];
	viewableText[0] += "\'s name to ";
	if (stringID == -1)
		viewableText[0] += newName;
	else
		viewableText[0] += QString("string[%1]").arg(ProjectData::stringVariableIdToString(stringID));
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::ChangeHeroName::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	ConfigureEvent_ChangeHeroName *configName = new ConfigureEvent_ChangeHeroName;
	bool configured = false;
	configName->setString(newName);
	configName->setStringID(stringID);
	configName->setIsReferencedCharacter(characterReferencedInVariable);
	configName->setCharacterID(characterID);
	if (configName->exec())
	{
		configured = true;
		characterID = configName->getCharacterID();
		characterReferencedInVariable = configName->isReferencedCharacter();
		stringID = configName->getStringID();
		newName.clear();
		newName = configName->getString();
	}
	delete configName;
	return configured;
}
#endif

bool BaseEvent::ChangeHeroName::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	QStringList tempStringList = QStringList() << "true" << "false";
	if (!XMLTools::attributeExists(EventNode, "characterID"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "characterReferencedInVariable"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "characterReferencedInVariable", tempStringList))
		return false;
	tempStringList.clear();
	tempStringList << "newName" << "stringID";
	if (!XMLTools::attributeNameValid(EventNode, 2, tempStringList))
		return false;
	return true;
}

BaseEvent::ChangeHeroName::ChangeHeroName(BaseEvent *pBase) : Event(pBase)
{
	newName = "";
	characterID = 0;
	stringID = -1;
	characterReferencedInVariable = false;
}

BaseEvent::ChangeHeroName::ChangeHeroName(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	characterID = QString(EventNode.getAttribute("characterID")).toInt();
	characterReferencedInVariable = (QString(EventNode.getAttribute("characterReferencedInVariable")) == "true") ? true:false;
	if (EventNode.isAttributeSet("stringID"))
	{
		newName = "";
		stringID = QString(EventNode.getAttribute("stringID")).toInt();
	}
	else
	{
		newName = EventNode.getAttribute("newName");
		stringID = -1;
	}
}

BaseEvent::ChangeHeroName::ChangeHeroName(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	char *temp_string;
	temp_string = storageFile.getString();
	newName = temp_string;
	delete[] temp_string;
	characterID = storageFile.getSignedInt();
	stringID = storageFile.getSignedInt();
	characterReferencedInVariable = storageFile.getBool();
}

BaseEvent::ChangeHeroName::ChangeHeroName(ChangeHeroName *other, BaseEvent *pBase) : Event(pBase)
{
	newName = other->newName;
	characterID = other->characterID;
	stringID = other->stringID;
	characterReferencedInVariable = other->characterReferencedInVariable;
}
//END BaseEvent::ChangeHeroName Member Functions

//BEGIN BaseEvent::ChangeImage Member Functions
XMLNode BaseEvent::ChangeImage::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	EventNode.addAttribute("objectName", objectName.toUtf8().data());
	EventNode.addAttribute("saveSlotID", QString::number(saveSlotID).toUtf8().data());
	if (imageType == MenuImage::TYPE_PARTYMEMBER)
	{
		EventNode.addAttribute("imageType", "party member");
		EventNode.addAttribute("memberID", QString::number(imageLocation.partyMemberImage->memberID).toUtf8().data());
		if (imageLocation.partyMemberImage->graphic == MenuImage::GRAPHIC_FACE)
			EventNode.addAttribute("graphic", "face");
		else if (imageLocation.partyMemberImage->graphic == MenuImage::GRAPHIC_MAPSPRITE)
		{
			EventNode.addAttribute("graphic", "map sprite");
			EventNode.addAttribute("direction", QString::number(imageLocation.partyMemberImage->direction).toUtf8().data());
			EventNode.addAttribute("pose", QString::number(imageLocation.partyMemberImage->pose).toUtf8().data());
		}
		else if (imageLocation.partyMemberImage->graphic == MenuImage::GRAPHIC_BATTLESPRITE)
		{
			EventNode.addAttribute("graphic", "battle sprite");
			EventNode.addAttribute("pose", QString::number(imageLocation.partyMemberImage->pose).toUtf8().data());
		}
	}
	else if (imageType == MenuImage::TYPE_CHARACTER)
	{
		EventNode.addAttribute("imageType", "character");
		EventNode.addAttribute("characterID", QString::number(imageLocation.characterImage->characterID).toUtf8().data());
		EventNode.addAttribute("idIsReferencedInVariable", (imageLocation.characterImage->idIsReferencedInVariable) ? "true":"false");
		if (imageLocation.characterImage->graphic == MenuImage::GRAPHIC_FACE)
			EventNode.addAttribute("graphic", "face");
		else if (imageLocation.characterImage->graphic == MenuImage::GRAPHIC_MAPSPRITE)
		{
			EventNode.addAttribute("graphic", "map sprite");
			EventNode.addAttribute("direction", QString::number(imageLocation.characterImage->direction).toUtf8().data());
			EventNode.addAttribute("pose", QString::number(imageLocation.characterImage->pose).toUtf8().data());
		}
		else if (imageLocation.characterImage->graphic == MenuImage::GRAPHIC_BATTLESPRITE)
		{
			EventNode.addAttribute("graphic", "battle sprite");
			EventNode.addAttribute("pose", QString::number(imageLocation.characterImage->pose).toUtf8().data());
		}
	}
	else if (imageType == MenuImage::TYPE_CLASS)
	{
		EventNode.addAttribute("imageType", "class");
		EventNode.addAttribute("classID", QString::number(imageLocation.classImage->classID).toUtf8().data());
		if (imageLocation.classImage->graphic == MenuImage::GRAPHIC_MAPSPRITE)
		{
			EventNode.addAttribute("graphic", "map sprite");
			EventNode.addAttribute("direction", QString::number(imageLocation.classImage->direction).toUtf8().data());
			EventNode.addAttribute("pose", QString::number(imageLocation.classImage->pose).toUtf8().data());
		}
		else if (imageLocation.classImage->graphic == MenuImage::GRAPHIC_BATTLESPRITE)
		{
			EventNode.addAttribute("graphic", "battle sprite");
			EventNode.addAttribute("pose", QString::number(imageLocation.classImage->pose).toUtf8().data());
		}
		EventNode.addAttribute("idIsReferencedInVariable", (imageLocation.classImage->idIsReferencedInVariable) ? "true":"false");
	}
	else if (imageType == MenuImage::TYPE_MONSTER)
	{
		EventNode.addAttribute("imageType", "monster");
		EventNode.addAttribute("monsterID", QString::number(imageLocation.monsterImage->monsterID).toUtf8().data());
		EventNode.addAttribute("pose", QString::number(imageLocation.monsterImage->pose).toUtf8().data());
		EventNode.addAttribute("idIsReferencedInVariable", (imageLocation.monsterImage->idIsReferencedInVariable) ? "true":"false");
	}
	else if (imageType == MenuImage::TYPE_VEHICLE)
	{
		EventNode.addAttribute("imageType", "vehicle");
		EventNode.addAttribute("vehicleID", QString::number(imageLocation.vehicleImage->vehicleID).toUtf8().data());
		EventNode.addAttribute("direction", QString::number(imageLocation.vehicleImage->direction).toUtf8().data());
		EventNode.addAttribute("pose", QString::number(imageLocation.vehicleImage->pose).toUtf8().data());
		EventNode.addAttribute("idIsReferencedInVariable", (imageLocation.vehicleImage->idIsReferencedInVariable) ? "true":"false");
	}
	else if (imageType == MenuImage::TYPE_TILE)
	{
		EventNode.addAttribute("imageType", "tile");
		EventNode.addAttribute("tilesetID", QString::number(imageLocation.tileImage->tilesetID).toUtf8().data());
		EventNode.addAttribute("tileID", QString::number(imageLocation.tileImage->tileID).toUtf8().data());
		EventNode.addAttribute("idsReferencedInVariables", (imageLocation.tileImage->idsReferencedInVariables) ? "true":"false");
	}
	else if (imageType == MenuImage::TYPE_FILE)
	{
		EventNode.addAttribute("imageType", "file");
		EventNode.addAttribute("fileLocation", ProjectData::getRelativeResourcePath(*(imageLocation.fileLocation)).toUtf8().data());
	}
	return EventNode;
}

void BaseEvent::ChangeImage::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putString(objectName.toUtf8().data());
	storageFile.putSignedInt(saveSlotID);
	storageFile.putSignedInt(imageType);
	if (imageType == MenuImage::TYPE_PARTYMEMBER)
	{
		storageFile.putSignedInt(imageLocation.partyMemberImage->memberID);
		storageFile.putSignedInt(imageLocation.partyMemberImage->graphic);
		storageFile.putSignedInt(imageLocation.partyMemberImage->direction);
		storageFile.putSignedInt(imageLocation.partyMemberImage->pose);
	}
	else if (imageType == MenuImage::TYPE_CHARACTER)
	{
		storageFile.putSignedInt(imageLocation.characterImage->characterID);
		storageFile.putSignedInt(imageLocation.characterImage->graphic);
		storageFile.putSignedInt(imageLocation.characterImage->direction);
		storageFile.putSignedInt(imageLocation.characterImage->pose);
		storageFile.putBool(imageLocation.characterImage->idIsReferencedInVariable);
	}
	else if (imageType == MenuImage::TYPE_CLASS)
	{
		storageFile.putSignedInt(imageLocation.classImage->classID);
		storageFile.putSignedInt(imageLocation.classImage->graphic);
		storageFile.putSignedInt(imageLocation.classImage->direction);
		storageFile.putSignedInt(imageLocation.classImage->pose);
		storageFile.putBool(imageLocation.classImage->idIsReferencedInVariable);
	}
	else if (imageType == MenuImage::TYPE_MONSTER)
	{
		storageFile.putSignedInt(imageLocation.monsterImage->monsterID);
		storageFile.putSignedInt(imageLocation.monsterImage->pose);
		storageFile.putBool(imageLocation.monsterImage->idIsReferencedInVariable);
	}
	else if (imageType == MenuImage::TYPE_VEHICLE)
	{
		storageFile.putSignedInt(imageLocation.vehicleImage->vehicleID);
		storageFile.putSignedInt(imageLocation.vehicleImage->direction);
		storageFile.putSignedInt(imageLocation.vehicleImage->pose);
		storageFile.putBool(imageLocation.vehicleImage->idIsReferencedInVariable);
	}
	else if (imageType == MenuImage::TYPE_TILE)
	{
		storageFile.putSignedInt(imageLocation.tileImage->tilesetID);
		storageFile.putSignedInt(imageLocation.tileImage->tileID);
		storageFile.putBool(imageLocation.tileImage->idsReferencedInVariables);
	}
	else if (imageType == MenuImage::TYPE_FILE)
		storageFile.putString(imageLocation.fileLocation->toUtf8().data());
}

QStringList BaseEvent::ChangeImage::getResources()
{
	QStringList resources = QStringList();
	if (imageType == MenuImage::TYPE_FILE)
		resources.append(*(imageLocation.fileLocation));
	return resources;
}

QStringList BaseEvent::ChangeImage::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Change Image: Set ";
	viewableText[0] += objectName;
	viewableText[0] += " to ";
	if (imageType == MenuImage::TYPE_PARTYMEMBER)
	{
		viewableText[0] += "Party Member ";
		if (imageLocation.partyMemberImage->memberID < 4)
			viewableText[0] += QString::number(imageLocation.partyMemberImage->memberID + 1);
		else
			viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(imageLocation.partyMemberImage->memberID - 4));
		viewableText[0] += "\'s ";
		if (imageLocation.partyMemberImage->graphic == MenuImage::GRAPHIC_FACE)
			viewableText[0] += "Face Graphic";
		else if (imageLocation.partyMemberImage->graphic == MenuImage::GRAPHIC_MAPSPRITE)
		{
			viewableText[0] += "Map Sprite (";
			viewableText[0] += QString("%1, ").arg(imageLocation.partyMemberImage->pose);
			if (imageLocation.partyMemberImage->direction == 0)
				viewableText[0] += "up)";
			else if (imageLocation.partyMemberImage->direction == 1)
				viewableText[0] += "right)";
			else if (imageLocation.partyMemberImage->direction == 2)
				viewableText[0] += "down)";
			else if (imageLocation.partyMemberImage->direction == 3)
				viewableText[0] += "left)";
		}
		else
			viewableText[0] += "Battle Sprite (pose=" + QString::number(imageLocation.partyMemberImage->pose) + ")";
	}
	else if (imageType == MenuImage::TYPE_CHARACTER)
	{
		viewableText[0] += "Character ";
		if (imageLocation.characterImage->idIsReferencedInVariable)
			viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(imageLocation.characterImage->characterID));
		else
			viewableText[0] += ProjectData::characterNames[imageLocation.characterImage->characterID];
		viewableText[0] += "\'s ";
		if (imageLocation.characterImage->graphic == MenuImage::GRAPHIC_FACE)
			viewableText[0] += "Face Graphic";
		else if (imageLocation.characterImage->graphic == MenuImage::GRAPHIC_MAPSPRITE)
		{
			viewableText[0] += "Map Sprite (";
			viewableText[0] += QString("%1, ").arg(imageLocation.characterImage->pose);
			if (imageLocation.characterImage->direction == 0)
				viewableText[0] += "up)";
			else if (imageLocation.characterImage->direction == 1)
				viewableText[0] += "right)";
			else if (imageLocation.characterImage->direction == 2)
				viewableText[0] += "down)";
			else if (imageLocation.characterImage->direction == 3)
				viewableText[0] += "left)";
		}
		else
			viewableText[0] += "Battle Sprite (pose=" + QString::number(imageLocation.characterImage->pose) + ")";
	}
	else if (imageType == MenuImage::TYPE_CLASS)
	{
		viewableText[0] += "Class ";
		if (imageLocation.classImage->idIsReferencedInVariable)
			viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(imageLocation.classImage->classID));
		else
			viewableText[0] += ProjectData::classNames[imageLocation.classImage->classID];
		viewableText[0] += "\'s ";
		if (imageLocation.classImage->graphic == MenuImage::GRAPHIC_MAPSPRITE)
		{
			viewableText[0] += "Map Sprite (";
			viewableText[0] += QString("%1, ").arg(imageLocation.classImage->pose);
			if (imageLocation.classImage->direction == 0)
				viewableText[0] += "up)";
			else if (imageLocation.classImage->direction == 1)
				viewableText[0] += "right)";
			else if (imageLocation.classImage->direction == 2)
				viewableText[0] += "down)";
			else if (imageLocation.classImage->direction == 3)
				viewableText[0] += "left)";
		}
		else
			viewableText[0] += "Battle Sprite (pose=" + QString::number(imageLocation.classImage->pose) + ")";
	}
	else if (imageType == MenuImage::TYPE_MONSTER)
	{
		viewableText[0] += "Monster ";
		if (imageLocation.monsterImage->idIsReferencedInVariable)
			viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(imageLocation.monsterImage->monsterID));
		else
			viewableText[0] += ProjectData::monsterNames[imageLocation.monsterImage->monsterID];
		viewableText[0] += "\'s sprite (pos=";
		viewableText[0] += QString::number(imageLocation.monsterImage->pose) + ")";
	}
	else if (imageType == MenuImage::TYPE_VEHICLE)
	{
		viewableText[0] += "Vehicle ";
		if (imageLocation.vehicleImage->idIsReferencedInVariable)
			viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(imageLocation.vehicleImage->vehicleID));
		else
			viewableText[0] += ProjectData::vehicleNames[imageLocation.vehicleImage->vehicleID];
		viewableText[0] += "\'s Map Sprite (";
		viewableText[0] += QString("%1, ").arg(imageLocation.vehicleImage->pose);
		if (imageLocation.vehicleImage->direction == 0)
			viewableText[0] += "up)";
		else if (imageLocation.vehicleImage->direction == 1)
			viewableText[0] += "right)";
		else if (imageLocation.vehicleImage->direction == 2)
			viewableText[0] += "down)";
		else if (imageLocation.vehicleImage->direction == 3)
			viewableText[0] += "left)";
	}
	else if (imageType == MenuImage::TYPE_TILE)
	{
		viewableText[0] += "Tile ";
		if (imageLocation.tileImage->idsReferencedInVariables)
		{
			viewableText[0] += "set=";
			viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(imageLocation.tileImage->tilesetID));
			viewableText[0] += " tile=";
			viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(imageLocation.tileImage->tileID));
		}
		else
			viewableText[0] += QString("set=%1 tile=%2").arg(imageLocation.tileImage->tilesetID, 2, 16, QChar('0')).arg(imageLocation.tileImage->tileID, 2, 16, QChar('0'));
	}
	else if (imageType == MenuImage::TYPE_FILE)
	{
		viewableText[0] += "File \"";
		viewableText[0] += ProjectData::getRelativeResourcePath(*(imageLocation.fileLocation));
		viewableText[0] += "\"";
	}
	if (saveSlotID > 0)
		viewableText[0] += QString(" (SaveSlot=%1)").arg(saveSlotID);
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::ChangeImage::configureEvent(int callerID, int callerType)
{
	ConfigureEvent_ChangeMenuImage *configMenuImage = new ConfigureEvent_ChangeMenuImage(saveSlotID, imageType, &imageLocation, callerID, callerType); 
	bool configured = false;
	configMenuImage->setImageObject(objectName);
	if (configMenuImage->exec())
	{
		configured = true;
		imageType = configMenuImage->getImageType();
		objectName = configMenuImage->getImageObject();
	}
	delete configMenuImage;
	return configured;
}
#endif

void BaseEvent::ChangeImage::getImageLocation(MenuImage::PartyMemberImage *image)
{
	image->memberID = imageLocation.partyMemberImage->memberID;
	image->graphic = imageLocation.partyMemberImage->graphic;
	image->direction = imageLocation.partyMemberImage->direction;
	image->pose = imageLocation.partyMemberImage->pose;
}

void BaseEvent::ChangeImage::getImageLocation(MenuImage::CharacterImage *image)
{
	image->characterID = imageLocation.characterImage->characterID;
	image->graphic = imageLocation.characterImage->graphic;
	image->direction = imageLocation.characterImage->direction;
	image->pose = imageLocation.characterImage->pose;
	image->idIsReferencedInVariable = imageLocation.characterImage->idIsReferencedInVariable;
}

void BaseEvent::ChangeImage::getImageLocation(MenuImage::ClassImage *image)
{
	image->classID = imageLocation.classImage->classID;
	image->graphic = imageLocation.classImage->graphic;
	image->direction = imageLocation.classImage->direction;
	image->pose = imageLocation.classImage->pose;
	image->idIsReferencedInVariable = imageLocation.classImage->idIsReferencedInVariable;
}

void BaseEvent::ChangeImage::getImageLocation(MenuImage::MonsterImage *image)
{
	image->monsterID = imageLocation.monsterImage->monsterID;
	image->pose = imageLocation.monsterImage->pose;
	image->idIsReferencedInVariable = imageLocation.monsterImage->idIsReferencedInVariable;
}

void BaseEvent::ChangeImage::getImageLocation(MenuImage::VehicleImage *image)
{
	image->vehicleID = imageLocation.vehicleImage->vehicleID;
	image->direction = imageLocation.vehicleImage->direction;
	image->pose = imageLocation.vehicleImage->pose;
	image->idIsReferencedInVariable = imageLocation.vehicleImage->idIsReferencedInVariable;
}

void BaseEvent::ChangeImage::getImageLocation(MenuImage::TileImage *image)
{
	image->tilesetID = imageLocation.tileImage->tilesetID;
	image->tileID = imageLocation.tileImage->tileID;
	image->idsReferencedInVariables = imageLocation.tileImage->idsReferencedInVariables;
}

void BaseEvent::ChangeImage::getImageLocation(QString *fileLocation)
{
	fileLocation->clear();
	fileLocation->append(*(imageLocation.fileLocation));
}

void BaseEvent::ChangeImage::setImageLocation(MenuImage::PartyMemberImage *image)
{
	imageLocation.partyMemberImage->memberID = image->memberID;
	imageLocation.partyMemberImage->graphic = image->graphic;
	imageLocation.partyMemberImage->direction = image->direction;
	imageLocation.partyMemberImage->pose = image->pose;
}

void BaseEvent::ChangeImage::setImageLocation(MenuImage::CharacterImage *image)
{
	imageLocation.characterImage->characterID = image->characterID;
	imageLocation.characterImage->graphic = image->graphic;
	imageLocation.characterImage->direction = image->direction;
	imageLocation.characterImage->pose = image->pose;
	imageLocation.characterImage->idIsReferencedInVariable = image->idIsReferencedInVariable;
}

void BaseEvent::ChangeImage::setImageLocation(MenuImage::ClassImage *image)
{
	imageLocation.classImage->classID = image->classID;
	imageLocation.classImage->graphic = image->graphic;
	imageLocation.classImage->direction = image->direction;
	imageLocation.classImage->pose = image->pose;
	imageLocation.classImage->idIsReferencedInVariable = image->idIsReferencedInVariable;
}

void BaseEvent::ChangeImage::setImageLocation(MenuImage::MonsterImage *image)
{
	imageLocation.monsterImage->monsterID = image->monsterID;
	imageLocation.monsterImage->pose = image->pose;
	imageLocation.monsterImage->idIsReferencedInVariable = image->idIsReferencedInVariable;
}

void BaseEvent::ChangeImage::setImageLocation(MenuImage::VehicleImage *image)
{
	imageLocation.vehicleImage->vehicleID = image->vehicleID;
	imageLocation.vehicleImage->direction = image->direction;
	imageLocation.vehicleImage->pose = image->pose;
	imageLocation.vehicleImage->idIsReferencedInVariable = image->idIsReferencedInVariable;
}

void BaseEvent::ChangeImage::setImageLocation(MenuImage::TileImage *image)
{
	imageLocation.tileImage->tilesetID = image->tilesetID;
	imageLocation.tileImage->tileID = image->tileID;
	imageLocation.tileImage->idsReferencedInVariables = image->idsReferencedInVariables;
}

void BaseEvent::ChangeImage::setImageLocation(QString fileLocation)
{
	imageLocation.fileLocation->clear();
	imageLocation.fileLocation->append(fileLocation);
}

void BaseEvent::ChangeImage::applySwappedTiles(QList<Tileset::SwapTileIDs> swappedTiles)
{
	if (imageType == MenuImage::TYPE_TILE)
	{
		if (!imageLocation.tileImage->idsReferencedInVariables)
		{
			for (int i = 0; i < swappedTiles.size(); ++i)
			{
				if (imageLocation.tileImage->tilesetID == swappedTiles[i].tileset)
				{
					if (imageLocation.tileImage->tileID == swappedTiles[i].tile1)
						imageLocation.tileImage->tileID = swappedTiles[i].tile2;
					else if (imageLocation.tileImage->tileID == swappedTiles[i].tile2)
						imageLocation.tileImage->tileID = swappedTiles[i].tile1;
				}
			}
		}
	}
}

bool BaseEvent::ChangeImage::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	if (!XMLTools::attributeExists(EventNode, "objectName"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "saveSlotID"))
		return false;
	if (!XMLTools::attributeIntRangeValid(EventNode, "saveSlotID", 0, 16))
		return false;
	if (!MenuImage::tryLoadImageLocation(EventNode))
		return false;
	return true;
}

BaseEvent::ChangeImage::ChangeImage(BaseEvent *pBase) : Event(pBase)
{
	objectName = "";
	imageLocation.fileLocation = new QString("");
	imageType = MenuImage::TYPE_FILE;
	saveSlotID = 0;
}

BaseEvent::ChangeImage::ChangeImage(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	QString tempString;
	objectName = EventNode.getAttribute("objectName");
	saveSlotID = QString(EventNode.getAttribute("saveSlotID")).toInt();
	tempString = EventNode.getAttribute("imageType");
	if (tempString == "party member")
	{
		imageType = MenuImage::TYPE_PARTYMEMBER;
		imageLocation.partyMemberImage = new MenuImage::PartyMemberImage;
		imageLocation.partyMemberImage->memberID = QString(EventNode.getAttribute("memberID")).toInt();
		tempString = EventNode.getAttribute("graphic");
		if (tempString == "face")
		{
			imageLocation.partyMemberImage->graphic = MenuImage::GRAPHIC_FACE;
			imageLocation.partyMemberImage->direction = 0;
			imageLocation.partyMemberImage->pose = 0;
		}
		else if (tempString == "map sprite")
		{
			imageLocation.partyMemberImage->graphic = MenuImage::GRAPHIC_MAPSPRITE;
			imageLocation.partyMemberImage->direction = QString(EventNode.getAttribute("direction")).toInt();
			imageLocation.partyMemberImage->pose = QString(EventNode.getAttribute("pose")).toInt();
		}
		else
		{
			imageLocation.partyMemberImage->graphic = MenuImage::GRAPHIC_BATTLESPRITE;
			imageLocation.partyMemberImage->direction = 0;
			imageLocation.partyMemberImage->pose = QString(EventNode.getAttribute("pose")).toInt();
		}
	}
	else if (tempString == "character")
	{
		imageType = MenuImage::TYPE_CHARACTER;
		imageLocation.characterImage = new MenuImage::CharacterImage;
		imageLocation.characterImage->characterID = QString(EventNode.getAttribute("characterID")).toInt();
		tempString = EventNode.getAttribute("graphic");
		if (tempString == "face")
		{
			imageLocation.characterImage->graphic = MenuImage::GRAPHIC_FACE;
			imageLocation.characterImage->direction = 0;
			imageLocation.characterImage->pose = 0;
		}
		else if (tempString == "map sprite")
		{
			imageLocation.characterImage->graphic = MenuImage::GRAPHIC_MAPSPRITE;
			imageLocation.characterImage->direction = QString(EventNode.getAttribute("direction")).toInt();
			imageLocation.characterImage->pose = QString(EventNode.getAttribute("pose")).toInt();
		}
		else
		{
			imageLocation.characterImage->graphic = MenuImage::GRAPHIC_BATTLESPRITE;
			imageLocation.characterImage->direction = 0;
			imageLocation.characterImage->pose = QString(EventNode.getAttribute("pose")).toInt();
		}
		imageLocation.characterImage->idIsReferencedInVariable = (QString(EventNode.getAttribute("idIsReferencedInVariable")) == "true") ? true:false;
	}
	else if (tempString == "class")
	{
		imageType = MenuImage::TYPE_CLASS;
		imageLocation.classImage = new MenuImage::ClassImage;
		imageLocation.classImage->classID = QString(EventNode.getAttribute("classID")).toInt();
		tempString = EventNode.getAttribute("graphic");
		if (tempString == "map sprite")
		{
			imageLocation.classImage->graphic = MenuImage::GRAPHIC_MAPSPRITE;
			imageLocation.classImage->direction = QString(EventNode.getAttribute("direction")).toInt();
			imageLocation.classImage->pose = QString(EventNode.getAttribute("pose")).toInt();
		}
		else
		{
			imageLocation.classImage->graphic = MenuImage::GRAPHIC_BATTLESPRITE;
			imageLocation.classImage->direction = 0;
			imageLocation.classImage->pose = QString(EventNode.getAttribute("pose")).toInt();
		}
		imageLocation.classImage->idIsReferencedInVariable = (QString(EventNode.getAttribute("idIsReferencedInVariable")) == "true") ? true:false;
	}
	else if (tempString == "monster")
	{
		imageType = MenuImage::TYPE_MONSTER;
		imageLocation.monsterImage = new MenuImage::MonsterImage;
		imageLocation.monsterImage->monsterID = QString(EventNode.getAttribute("monsterID")).toInt();
		imageLocation.monsterImage->pose = QString(EventNode.getAttribute("pose")).toInt();
		imageLocation.monsterImage->idIsReferencedInVariable = (QString(EventNode.getAttribute("idIsReferencedInVariable")) == "true") ? true:false;
	}
	else if (tempString == "vehicle")
	{
		imageType = MenuImage::TYPE_VEHICLE;
		imageLocation.vehicleImage = new MenuImage::VehicleImage;
		imageLocation.vehicleImage->vehicleID = QString(EventNode.getAttribute("vehicleID")).toInt();
		imageLocation.vehicleImage->direction = QString(EventNode.getAttribute("direction")).toInt();
		imageLocation.vehicleImage->pose = QString(EventNode.getAttribute("pose")).toInt();
		imageLocation.vehicleImage->idIsReferencedInVariable = (QString(EventNode.getAttribute("idIsReferencedInVariable")) == "true") ? true:false;
	}
	else if (tempString == "tile")
	{
		imageType = MenuImage::TYPE_TILE;
		imageLocation.tileImage = new MenuImage::TileImage;
		imageLocation.tileImage->tilesetID = QString(EventNode.getAttribute("tilesetID")).toInt();
		imageLocation.tileImage->tileID = QString(EventNode.getAttribute("tileID")).toInt();
		imageLocation.tileImage->idsReferencedInVariables = (QString(EventNode.getAttribute("idsReferencedInVariables")) == "true") ? true:false;
	}
	else if (tempString == "file")
	{
		imageType = MenuImage::TYPE_FILE;
		imageLocation.fileLocation = new QString(ProjectData::getAbsoluteResourcePath(EventNode.getAttribute("fileLocation")));
	}
}

BaseEvent::ChangeImage::ChangeImage(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	char *temp_string;
	temp_string = storageFile.getString();
	objectName = temp_string;
	delete[] temp_string;
	saveSlotID = storageFile.getSignedInt();
	imageType = storageFile.getSignedInt();
	if (imageType == MenuImage::TYPE_PARTYMEMBER)
	{
		imageLocation.partyMemberImage = new MenuImage::PartyMemberImage;
		imageLocation.partyMemberImage->memberID = storageFile.getSignedInt();
		imageLocation.partyMemberImage->graphic = storageFile.getSignedInt();
		imageLocation.partyMemberImage->direction = storageFile.getSignedInt();
		imageLocation.partyMemberImage->pose = storageFile.getSignedInt();
	}
	else if (imageType == MenuImage::TYPE_CHARACTER)
	{
		imageLocation.characterImage = new MenuImage::CharacterImage;
		imageLocation.characterImage->characterID = storageFile.getSignedInt();
		imageLocation.characterImage->graphic = storageFile.getSignedInt();
		imageLocation.characterImage->direction = storageFile.getSignedInt();
		imageLocation.characterImage->pose = storageFile.getSignedInt();
		imageLocation.characterImage->idIsReferencedInVariable = storageFile.getBool();
	}
	else if (imageType == MenuImage::TYPE_CLASS)
	{
		imageLocation.classImage = new MenuImage::ClassImage;
		imageLocation.classImage->classID = storageFile.getSignedInt();
		imageLocation.classImage->direction = storageFile.getSignedInt();
		imageLocation.classImage->pose = storageFile.getSignedInt();
		imageLocation.classImage->idIsReferencedInVariable = storageFile.getBool();
	}
	else if (imageType == MenuImage::TYPE_MONSTER)
	{
		imageLocation.monsterImage = new MenuImage::MonsterImage;
		imageLocation.monsterImage->monsterID = storageFile.getSignedInt();
		imageLocation.monsterImage->pose = storageFile.getSignedInt();
		imageLocation.monsterImage->idIsReferencedInVariable = storageFile.getBool();
	}
	else if (imageType == MenuImage::TYPE_VEHICLE)
	{
		imageLocation.vehicleImage = new MenuImage::VehicleImage;
		imageLocation.vehicleImage->vehicleID = storageFile.getSignedInt();
		imageLocation.vehicleImage->direction = storageFile.getSignedInt();
		imageLocation.vehicleImage->pose = storageFile.getSignedInt();
		imageLocation.vehicleImage->idIsReferencedInVariable = storageFile.getBool();
	}
	else if (imageType == MenuImage::TYPE_TILE)
	{
		imageLocation.tileImage = new MenuImage::TileImage;
		imageLocation.tileImage->tilesetID = storageFile.getSignedInt();
		imageLocation.tileImage->tileID = storageFile.getSignedInt();
		imageLocation.tileImage->idsReferencedInVariables = storageFile.getBool();
	}
	else if (imageType == MenuImage::TYPE_FILE)
	{
		temp_string = storageFile.getString();
		imageLocation.fileLocation = new QString(temp_string);
		delete[] temp_string;
	}
}

BaseEvent::ChangeImage::ChangeImage(ChangeImage *other, BaseEvent *pBase) : Event(pBase)
{
	objectName = "";
	objectName.append(other->objectName);
	saveSlotID = other->saveSlotID;
	imageType = other->imageType;
	if (imageType == MenuImage::TYPE_PARTYMEMBER)
	{
		imageLocation.partyMemberImage = new MenuImage::PartyMemberImage;
		imageLocation.partyMemberImage->memberID = other->imageLocation.partyMemberImage->memberID;
		imageLocation.partyMemberImage->graphic = other->imageLocation.partyMemberImage->graphic;
		imageLocation.partyMemberImage->direction = other->imageLocation.partyMemberImage->direction;
		imageLocation.partyMemberImage->pose = other->imageLocation.partyMemberImage->pose;
	}
	else if (imageType == MenuImage::TYPE_CHARACTER)
	{
		imageLocation.characterImage = new MenuImage::CharacterImage;
		imageLocation.characterImage->characterID = other->imageLocation.characterImage->characterID;
		imageLocation.characterImage->graphic = other->imageLocation.characterImage->graphic;
		imageLocation.characterImage->direction = other->imageLocation.characterImage->direction;
		imageLocation.characterImage->pose = other->imageLocation.characterImage->pose;
		imageLocation.characterImage->idIsReferencedInVariable = other->imageLocation.characterImage->idIsReferencedInVariable;
	}
	else if (imageType == MenuImage::TYPE_CLASS)
	{
		imageLocation.classImage = new MenuImage::ClassImage;
		imageLocation.classImage->classID = other->imageLocation.classImage->classID;
		imageLocation.classImage->graphic = other->imageLocation.classImage->graphic;
		imageLocation.classImage->direction = other->imageLocation.classImage->direction;
		imageLocation.classImage->pose = other->imageLocation.classImage->pose;
		imageLocation.classImage->idIsReferencedInVariable = other->imageLocation.classImage->idIsReferencedInVariable;
	}
	else if (imageType == MenuImage::TYPE_MONSTER)
	{
		imageLocation.monsterImage = new MenuImage::MonsterImage;
		imageLocation.monsterImage->monsterID = other->imageLocation.monsterImage->monsterID;
		imageLocation.monsterImage->pose = other->imageLocation.monsterImage->pose;
		imageLocation.monsterImage->idIsReferencedInVariable = other->imageLocation.monsterImage->idIsReferencedInVariable;
	}
	else if (imageType == MenuImage::TYPE_VEHICLE)
	{
		imageLocation.vehicleImage = new MenuImage::VehicleImage;
		imageLocation.vehicleImage->vehicleID = other->imageLocation.vehicleImage->vehicleID;
		imageLocation.vehicleImage->direction = other->imageLocation.vehicleImage->direction;
		imageLocation.vehicleImage->pose = other->imageLocation.vehicleImage->pose;
		imageLocation.vehicleImage->idIsReferencedInVariable = other->imageLocation.vehicleImage->idIsReferencedInVariable;
	}
	else if (imageType == MenuImage::TYPE_TILE)
	{
		imageLocation.tileImage = new MenuImage::TileImage;
		imageLocation.tileImage->tilesetID = other->imageLocation.tileImage->tilesetID;
		imageLocation.tileImage->tileID = other->imageLocation.tileImage->tileID;
		imageLocation.tileImage->idsReferencedInVariables = other->imageLocation.tileImage->idsReferencedInVariables;
	}
	else if (imageType == MenuImage::TYPE_FILE)
	{
		imageLocation.fileLocation = new QString("");
		imageLocation.fileLocation->append(*(other->imageLocation.fileLocation));
	}
}
//END BaseEvent::ChangeImage Member Functions

//BEGIN BaseEvent::ChangeItemList Member Functions
XMLNode BaseEvent::ChangeItemList::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	QString tempString;
	EventNode.addAttribute("type", "type");
	EventNode.addAttribute("objectName", objectName.toUtf8().data());
	if (tableType == MenuItemTable::TYPE_ITEMS)
		EventNode.addAttribute("tableType", "items");
	else if (tableType == MenuItemTable::TYPE_SKILLS)
		EventNode.addAttribute("tableType", "skills");
	else if (tableType == MenuItemTable::TYPE_EQUIPMENT)
		EventNode.addAttribute("tableType", "equipment");
	else if (tableType == MenuItemTable::TYPE_CHARACTERS)
		EventNode.addAttribute("tableType", "characters");
	else if (tableType == MenuItemTable::TYPE_CLASSES)
		EventNode.addAttribute("tableType", "classes");
	else if (tableType == MenuItemTable::TYPE_MONSTERS)
		EventNode.addAttribute("tableType", "monsters");
	if (tableType == MenuItemTable::TYPE_ITEMS)
	{
		EventNode.addAttribute("showCommonItems", (settings.items->showCommonItems) ? "true":"false");
		EventNode.addAttribute("showWeapons", (settings.items->showWeapons) ? "true":"false");
		EventNode.addAttribute("showShields", (settings.items->showShields) ? "true":"false");
		EventNode.addAttribute("showArmor", (settings.items->showArmor) ? "true":"false");
		EventNode.addAttribute("showHelmets", (settings.items->showHelmets) ? "true":"false");
		EventNode.addAttribute("showAccessories", (settings.items->showAccessories) ? "true":"false");
		EventNode.addAttribute("showMedicine", (settings.items->showMedicine) ? "true":"false");
		EventNode.addAttribute("showSkillBooks", (settings.items->showSkillBooks) ? "true":"false");
		EventNode.addAttribute("showSeeds", (settings.items->showSeeds) ? "true":"false");
		EventNode.addAttribute("showSkillScrolls", (settings.items->showSkillScrolls) ? "true":"false");
		EventNode.addAttribute("showSwitches", (settings.items->showSwitches) ? "true":"false");
		EventNode.addAttribute("showEvents", (settings.items->showEvents) ? "true":"false");
	}
	else if (tableType == MenuItemTable::TYPE_SKILLS)
	{
		tempString = "";
		for (int i = 0; i < settings.skills->skillTypesToShow.size(); ++i)
			tempString += (settings.skills->skillTypesToShow[i]) ? "1":"0";
		EventNode.addAttribute("partyMemberID", QString::number(settings.skills->memberID).toUtf8().data());
		EventNode.addAttribute("skillTypesToShow", tempString.toUtf8().data());
	}
	else if (tableType == MenuItemTable::TYPE_EQUIPMENT)
	{
		EventNode.addAttribute("partyMemberID", QString::number(settings.equipment->memberID).toUtf8().data());
		if (settings.equipment->equipmentType == MenuItemTable::EquipmentSettings::TYPE_WEAPONS)
			EventNode.addAttribute("equipmentType", "weapons");
		else if (settings.equipment->equipmentType == MenuItemTable::EquipmentSettings::TYPE_SHIELDS)
			EventNode.addAttribute("equipmentType", "shields");
		else if (settings.equipment->equipmentType == MenuItemTable::EquipmentSettings::TYPE_ARMOR)
			EventNode.addAttribute("equipmentType", "armor");
		else if (settings.equipment->equipmentType == MenuItemTable::EquipmentSettings::TYPE_HELMETS)
			EventNode.addAttribute("equipmentType", "helmets");
		else if (settings.equipment->equipmentType == MenuItemTable::EquipmentSettings::TYPE_ACCESSORIES)
			EventNode.addAttribute("equipmentType", "accessories");
	}
	else
	{
		EventNode.addAttribute("useVariablesToSetState", (settings.other->useVariablesToSetState) ? "true":"false");
		if (settings.other->useVariablesToSetState)
			EventNode.addAttribute("startVariableID", QString::number(settings.other->startVariableID).toUtf8().data());
	}
	return EventNode;
}

void BaseEvent::ChangeItemList::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putString(objectName.toUtf8().data());
	storageFile.putSignedInt(tableType);
	if (tableType == MenuItemTable::TYPE_ITEMS)
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
	else if (tableType == MenuItemTable::TYPE_SKILLS)
	{
		storageFile.putSignedInt(settings.skills->memberID);
		storageFile.putSignedInt(settings.skills->skillTypesToShow.size());
		for (int i = 0; i < settings.skills->skillTypesToShow.size(); ++i)
			storageFile.putBool(settings.skills->skillTypesToShow[i]);
	}
	else if (tableType == MenuItemTable::TYPE_EQUIPMENT)
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

QStringList BaseEvent::ChangeItemList::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Change Item List: ";
	viewableText[0] += objectName;
	viewableText[0] += " to ";
	if (tableType == MenuItemTable::TYPE_ITEMS)
		viewableText[0] += "Items";
	else if (tableType == MenuItemTable::TYPE_SKILLS)
		viewableText[0] += "Skills";
	else if (tableType == MenuItemTable::TYPE_EQUIPMENT)
		viewableText[0] += "Available Equipment";
	else if (tableType == MenuItemTable::TYPE_CHARACTERS)
		viewableText[0] += "Characters";
	else if (tableType == MenuItemTable::TYPE_CLASSES)
		viewableText[0] += "Classes";
	else if (tableType == MenuItemTable::TYPE_MONSTERS)
		viewableText[0] += "Monsters";
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::ChangeItemList::configureEvent(int callerID, int callerType)
{
	ConfigureEvent_ChangeMenuItemList *configItemList = new ConfigureEvent_ChangeMenuItemList(tableType, &settings, callerID, callerType);
	bool configured = false;
	configItemList->setItemListObject(objectName);
	if (configItemList->exec())
	{
		configured = true;
		tableType = configItemList->getTableType();
		objectName = configItemList->getItemListObject();
	}
	delete configItemList;
	return configured;
}
#endif

void BaseEvent::ChangeItemList::getSettings(MenuItemTable::ItemSettings *value)
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

void BaseEvent::ChangeItemList::getSettings(MenuItemTable::SkillSettings *value)
{
	for (int i = 0; i < settings.skills->skillTypesToShow.size(); ++i)
		value->skillTypesToShow.append(settings.skills->skillTypesToShow[i]);
	value->memberID = settings.skills->memberID;
}

void BaseEvent::ChangeItemList::getSettings(MenuItemTable::EquipmentSettings *value)
{
	value->equipmentType = settings.equipment->equipmentType;
	value->memberID = settings.equipment->memberID;
}

void BaseEvent::ChangeItemList::getSettings(MenuItemTable::OtherSettings *value)
{
	value->startVariableID = settings.other->startVariableID;
	value->useVariablesToSetState = settings.other->useVariablesToSetState;
}

void BaseEvent::ChangeItemList::setSettings(MenuItemTable::ItemSettings *value)
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

void BaseEvent::ChangeItemList::setSettings(MenuItemTable::SkillSettings *value)
{
	for (int i = 0; i < value->skillTypesToShow.size(); ++i)
		settings.skills->skillTypesToShow.append(value->skillTypesToShow[i]);
	settings.skills->memberID = value->memberID;
}

void BaseEvent::ChangeItemList::setSettings(MenuItemTable::EquipmentSettings *value)
{
	settings.equipment->equipmentType = value->equipmentType;
	settings.equipment->memberID = value->memberID;
}

void BaseEvent::ChangeItemList::setSettings(MenuItemTable::OtherSettings *value)
{
	settings.other->startVariableID =value->startVariableID;
	settings.other->useVariablesToSetState = value->useVariablesToSetState;
}

void BaseEvent::ChangeItemList::setTableType(int value)
{
	if (tableType == MenuItemTable::TYPE_ITEMS)
		delete (settings.items);
	else if (tableType == MenuItemTable::TYPE_SKILLS)
		delete (settings.skills);
	else if (tableType == MenuItemTable::TYPE_EQUIPMENT)
		delete (settings.equipment);
	else
		delete (settings.other);
	tableType = value;
	if (tableType == MenuItemTable::TYPE_ITEMS)
		settings.items = new MenuItemTable::ItemSettings;
	else if (tableType == MenuItemTable::TYPE_SKILLS)
		settings.skills = new MenuItemTable::SkillSettings;
	else if (tableType == MenuItemTable::TYPE_EQUIPMENT)
		settings.equipment = new MenuItemTable::EquipmentSettings;
	else
		settings.other = new MenuItemTable::OtherSettings;
}

bool BaseEvent::ChangeItemList::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	if (!XMLTools::attributeExists(EventNode, "objectName"))
		return false;
	if (!MenuItemTable::tryLoadSettings(EventNode))
		return false;
	return true;
}

BaseEvent::ChangeItemList::ChangeItemList(BaseEvent *pBase) : Event(pBase)
{
	objectName = "";
	tableType = MenuItemTable::TYPE_ITEMS;
	settings.items = new MenuItemTable::ItemSettings;
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

BaseEvent::ChangeItemList::ChangeItemList(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	QString tempString;
	objectName = EventNode.getAttribute("objectName");
	tempString = EventNode.getAttribute("tableType");
	if (tempString == "items")
		tableType = MenuItemTable::TYPE_ITEMS;
	else if (tempString == "skills")
		tableType = MenuItemTable::TYPE_SKILLS;
	else if (tempString == "equipment")
		tableType = MenuItemTable::TYPE_EQUIPMENT;
	else if (tempString == "characters")
		tableType = MenuItemTable::TYPE_CHARACTERS;
	else if (tempString == "classes")
		tableType = MenuItemTable::TYPE_CLASSES;
	else if (tempString == "monsters")
		tableType = MenuItemTable::TYPE_MONSTERS;
	if (tableType == MenuItemTable::TYPE_ITEMS)
	{
		settings.items = new MenuItemTable::ItemSettings;
		settings.items->showCommonItems = (QString(EventNode.getAttribute("showCommonItems")) == "true") ? true:false;
		settings.items->showWeapons = (QString(EventNode.getAttribute("showWeapons")) == "true") ? true:false;
		settings.items->showShields = (QString(EventNode.getAttribute("showShields")) == "true") ? true:false;
		settings.items->showArmor = (QString(EventNode.getAttribute("showArmor")) == "true") ? true:false;
		settings.items->showHelmets = (QString(EventNode.getAttribute("showHelmets")) == "true") ? true:false;
		settings.items->showAccessories = (QString(EventNode.getAttribute("showAccessories")) == "true") ? true:false;
		settings.items->showMedicine = (QString(EventNode.getAttribute("showMedicine")) == "true") ? true:false;
		settings.items->showSkillBooks = (QString(EventNode.getAttribute("showSkillBooks")) == "true") ? true:false;
		settings.items->showSeeds = (QString(EventNode.getAttribute("showSeeds")) == "true") ? true:false;
		settings.items->showSkillScrolls = (QString(EventNode.getAttribute("showSkillScrolls")) == "true") ? true:false;
		settings.items->showSwitches = (QString(EventNode.getAttribute("showSwitches")) == "true") ? true:false;
		settings.items->showEvents = (QString(EventNode.getAttribute("showEvents")) == "true") ? true:false;
	}
	else if (tableType == MenuItemTable::TYPE_SKILLS)
	{
		settings.skills = new MenuItemTable::SkillSettings;
		settings.skills->memberID = QString(EventNode.getAttribute("partyMemberID")).toInt();
		tempString = EventNode.getAttribute("skillTypesToShow");
		for (int i = 0; i < tempString.size(); ++i)
			settings.skills->skillTypesToShow.append((tempString[i] == QChar('1')) ? true:false);
	}
	else if (tableType == MenuItemTable::TYPE_EQUIPMENT)
	{
		settings.equipment = new MenuItemTable::EquipmentSettings;
		settings.equipment->memberID = QString(EventNode.getAttribute("partyMemberID")).toInt();
		tempString = EventNode.getAttribute("equipmentType");
		if (tempString == "weapons")
			settings.equipment->equipmentType = MenuItemTable::EquipmentSettings::TYPE_WEAPONS;
		else if (tempString == "shields")
			settings.equipment->equipmentType = MenuItemTable::EquipmentSettings::TYPE_SHIELDS;
		else if (tempString == "armor")
			settings.equipment->equipmentType = MenuItemTable::EquipmentSettings::TYPE_ARMOR;
		else if (tempString == "helmets")
			settings.equipment->equipmentType = MenuItemTable::EquipmentSettings::TYPE_HELMETS;
		else if (tempString == "accessories")
			settings.equipment->equipmentType = MenuItemTable::EquipmentSettings::TYPE_ACCESSORIES;
	}
	else
	{
		settings.other = new MenuItemTable::OtherSettings;
		settings.other->useVariablesToSetState = (QString(EventNode.getAttribute("useVariablesToSetState")) == "true") ? true:false;
		if (settings.other->useVariablesToSetState)
			settings.other->startVariableID = QString(EventNode.getAttribute("startVariableID")).toInt();
		else
			settings.other->startVariableID = 0;
	}
}

BaseEvent::ChangeItemList::ChangeItemList(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	char *temp_string;
	temp_string = storageFile.getString();
	objectName = temp_string;
	delete[] temp_string;
	tableType = storageFile.getSignedInt();
	if (tableType == MenuItemTable::TYPE_ITEMS)
	{
		settings.items = new MenuItemTable::ItemSettings;
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
	else if (tableType == MenuItemTable::TYPE_SKILLS)
	{
		settings.skills = new MenuItemTable::SkillSettings;
		settings.skills->memberID = storageFile.getSignedInt();
		for (int i = 0, count = storageFile.getSignedInt(); i < count; ++i)
			settings.skills->skillTypesToShow.append(storageFile.getBool());
	}
	else if (tableType == MenuItemTable::TYPE_EQUIPMENT)
	{
		settings.equipment = new MenuItemTable::EquipmentSettings;
		settings.equipment->memberID = storageFile.getSignedInt();
		settings.equipment->equipmentType = (MenuItemTable::EquipmentSettings::EquipmentType)storageFile.getSignedInt();
	}
	else
	{
		settings.other = new MenuItemTable::OtherSettings;
		settings.other->useVariablesToSetState = storageFile.getBool();
		if (settings.other->useVariablesToSetState)
			settings.other->startVariableID = storageFile.getSignedInt();
		else
			settings.other->startVariableID = 0;
	}
}

BaseEvent::ChangeItemList::ChangeItemList(ChangeItemList *other, BaseEvent *pBase) : Event(pBase)
{
	objectName = "";
	objectName.append(other->objectName);
	tableType = other->tableType;
	if (tableType == MenuItemTable::TYPE_ITEMS)
	{
		settings.items = new MenuItemTable::ItemSettings;
		settings.items->showCommonItems = other->settings.items->showCommonItems;
		settings.items->showWeapons = other->settings.items->showWeapons;
		settings.items->showShields = other->settings.items->showShields;
		settings.items->showArmor = other->settings.items->showArmor;
		settings.items->showHelmets = other->settings.items->showHelmets;
		settings.items->showAccessories = other->settings.items->showAccessories;
		settings.items->showMedicine = other->settings.items->showMedicine;
		settings.items->showSkillBooks = other->settings.items->showSkillBooks;
		settings.items->showSeeds = other->settings.items->showSeeds;
		settings.items->showSkillScrolls = other->settings.items->showSkillScrolls;
		settings.items->showSwitches = other->settings.items->showSwitches;
		settings.items->showEvents = other->settings.items->showEvents;
	}
	else if (tableType == MenuItemTable::TYPE_SKILLS)
	{
		settings.skills = new MenuItemTable::SkillSettings;
		settings.skills->memberID = other->settings.skills->memberID;
		for (int i = 0; i < other->settings.skills->skillTypesToShow.size(); ++i)
			settings.skills->skillTypesToShow.append(other->settings.skills->skillTypesToShow[i]);
	}
	else if (tableType == MenuItemTable::TYPE_EQUIPMENT)
	{
		settings.equipment = new MenuItemTable::EquipmentSettings;
		settings.equipment->equipmentType = other->settings.equipment->equipmentType;
		settings.equipment->memberID = other->settings.equipment->memberID;
	}
	else
	{
		settings.other = new MenuItemTable::OtherSettings;
		settings.other->startVariableID = other->settings.other->startVariableID;
		settings.other->useVariablesToSetState = other->settings.other->useVariablesToSetState;
	}
}
//END BaseEvent::ChangeItemList Member Functions

//BEGIN BaseEvent::ChangeMoney Member Functions
XMLNode BaseEvent::ChangeMoney::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	if (changeToMake == INCREASE_MONEY)
		EventNode.addAttribute("changeToMake", "increaseMoney");
	else if (changeToMake == DECREASE_MONEY)
		EventNode.addAttribute("changeToMake", "decreaseMoney");
	EventNode.addAttribute("amount", QString::number(amount).toUtf8().data());
	EventNode.addAttribute("amountStoredInVariable", (amountStoredInVariable) ? "true":"false");
	return EventNode;
}

void BaseEvent::ChangeMoney::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedLong(amount);
	storageFile.putSignedInt(changeToMake);
	storageFile.putBool(amountStoredInVariable);
}

QStringList BaseEvent::ChangeMoney::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Change Money: ";
	if (changeToMake == INCREASE_MONEY)
		viewableText[0] += "+";
	else
		viewableText[0] += "-";
	if (amountStoredInVariable)
		viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(amount));
	else
		viewableText[0] += QString::number(amount);
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::ChangeMoney::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	ConfigureEvent_ChangeMoney *configMoney = new ConfigureEvent_ChangeMoney;
	bool configured = false;
	configMoney->setOperation((changeToMake == INCREASE_MONEY) ? true:false);
	configMoney->setIsStoredAmount(amountStoredInVariable);
	configMoney->setAmount(amount);
	if (configMoney->exec())
	{
		configured = true;
		changeToMake = (configMoney->isIncreaseOperation()) ? INCREASE_MONEY:DECREASE_MONEY;
		amount = configMoney->getAmount();
		amountStoredInVariable = configMoney->isStoredAmount();
	}
	delete configMoney;
	return configured;
}
#endif

bool BaseEvent::ChangeMoney::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	QStringList tempStringList;
	tempStringList.clear();
	tempStringList << "increaseMoney" << "decreaseMoney";
	if (!XMLTools::attributeExists(EventNode, "changeToMake"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "changeToMake", tempStringList))
		return false;
	if (!XMLTools::attributeExists(EventNode, "amount"))
		return false;
	tempStringList.clear();
	tempStringList << "true" << "false";
	if (!XMLTools::attributeExists(EventNode, "amountStoredInVariable"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "amountStoredInVariable", tempStringList, 1))
		return false;
	return true;
}

BaseEvent::ChangeMoney::ChangeMoney(BaseEvent *pBase) : Event(pBase)
{
	amount = 0;
	changeToMake = INCREASE_MONEY;
	amountStoredInVariable = false;
}

BaseEvent::ChangeMoney::ChangeMoney(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	QString tempString;
	tempString = EventNode.getAttribute("changeToMake");
	changeToMake = (tempString == "increaseMoney") ? INCREASE_MONEY:DECREASE_MONEY;
	amount = QString(EventNode.getAttribute("amount")).toLong();
	tempString = EventNode.getAttribute("amountStoredInVariable");
	amountStoredInVariable = (tempString == "true") ? true:false;
}

BaseEvent::ChangeMoney::ChangeMoney(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	amount = storageFile.getSignedLong();
	changeToMake = storageFile.getSignedInt();
	amountStoredInVariable = storageFile.getBool();
}

BaseEvent::ChangeMoney::ChangeMoney(ChangeMoney *other, BaseEvent *pBase) : Event(pBase)
{
	amount = other->amount;
	changeToMake = other->changeToMake;
	amountStoredInVariable = other->amountStoredInVariable;
}
//END BaseEvent::ChangeMoney Member Functions

//BEGIN BaseEvent::ChangeParallaxBackground Member Functions
XMLNode BaseEvent::ChangeParallaxBackground::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	EventNode.addAttribute("bgLocation", ProjectData::getRelativeResourcePath(bgLocation).toUtf8().data());
	EventNode.addAttribute("hScrollSpeed", QString::number(hScrollSpeed).toUtf8().data());
	EventNode.addAttribute("vScrollSpeed", QString::number(vScrollSpeed).toUtf8().data());
	EventNode.addAttribute("autoHScroll", (autoHScroll) ? "true":"false");
	EventNode.addAttribute("autoVScroll", (autoVScroll) ? "true":"false");
	return EventNode;
}

void BaseEvent::ChangeParallaxBackground::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putString(bgLocation.toUtf8().data());
	storageFile.putSignedInt(hScrollSpeed);
	storageFile.putSignedInt(vScrollSpeed);
	storageFile.putBool(autoHScroll);
	storageFile.putBool(autoVScroll);
}

QStringList BaseEvent::ChangeParallaxBackground::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Change Parallax Background: ";
	viewableText[0] += QFileInfo(bgLocation).fileName();
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::ChangeParallaxBackground::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	ConfigureEvent_ChangeParallaxBackground *configBackground = new ConfigureEvent_ChangeParallaxBackground;
	bool configured = false;
	configBackground->setBackgroundLocation(bgLocation);
	configBackground->setHScrollSpeed(hScrollSpeed);
	configBackground->setVScrollSpeed(vScrollSpeed);
	configBackground->setAutoHScroll(autoHScroll);
	configBackground->setAutoVScroll(autoVScroll);
	if (configBackground->exec())
	{
		configured = true;
		bgLocation = configBackground->getBackgroundLocation();
		hScrollSpeed = configBackground->getHScrollSpeed();
		vScrollSpeed = configBackground->getVScrollSpeed();
		autoHScroll = configBackground->getAutoHScroll();
		autoVScroll = configBackground->getAutoVScroll();
	}
	delete configBackground;
	return configured;
}
#endif

bool BaseEvent::ChangeParallaxBackground::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	QStringList trueFalseList = QStringList() << "true" << "false";
	if (!XMLTools::attributeExists(EventNode, "bgLocation"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "hScrollSpeed"))
		return false;
	if (!XMLTools::attributeIntRangeValid(EventNode, "hScrollSpeed", -16, 16))
		return false;
	if (!XMLTools::attributeExists(EventNode, "vScrollSpeed"))
		return false;
	if (!XMLTools::attributeIntRangeValid(EventNode, "vScrollSpeed", -16, 16))
		return false;
	if (!XMLTools::attributeExists(EventNode, "autoHScroll"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "autoHScroll", trueFalseList, 1))
		return false;
	if (!XMLTools::attributeExists(EventNode, "autoVScroll"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "autoVScroll", trueFalseList, 1))
		return false;
	return true;
}

BaseEvent::ChangeParallaxBackground::ChangeParallaxBackground(BaseEvent *pBase) : Event(pBase)
{
	bgLocation = "";
	hScrollSpeed = 0;
	vScrollSpeed = 0;
	autoHScroll = false;
	autoVScroll = false;
}

BaseEvent::ChangeParallaxBackground::ChangeParallaxBackground(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	bgLocation = ProjectData::getAbsoluteResourcePath(EventNode.getAttribute("bgLocation"));
	hScrollSpeed = QString(EventNode.getAttribute("hScrollSpeed")).toInt();
	vScrollSpeed = QString(EventNode.getAttribute("vScrollSpeed")).toInt();
	autoHScroll = (QString(EventNode.getAttribute("autoHScroll")) == "true") ? true:false;
	autoVScroll = (QString(EventNode.getAttribute("autoVScroll")) == "true") ? true:false;
}

BaseEvent::ChangeParallaxBackground::ChangeParallaxBackground(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	char *temp_string = storageFile.getString();
	bgLocation = temp_string;
	delete[] temp_string;
	hScrollSpeed = storageFile.getSignedInt();
	vScrollSpeed = storageFile.getSignedInt();
	autoHScroll = storageFile.getBool();
	autoVScroll = storageFile.getBool();
}

BaseEvent::ChangeParallaxBackground::ChangeParallaxBackground(ChangeParallaxBackground *other, BaseEvent *pBase) : Event(pBase)
{
	bgLocation = other->bgLocation;
	hScrollSpeed = other->hScrollSpeed;
	vScrollSpeed = other->vScrollSpeed;
	autoHScroll = other->autoHScroll;
	autoVScroll = other->autoVScroll;
}
//END BaseEvent::ChangeParallaxBackground Member Functions

//BEGIN BaseEvent::ChangeParty Member Functions
XMLNode BaseEvent::ChangeParty::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	if (changeToMake == ADD_MEMBER)
		EventNode.addAttribute("changeToMake", "addMember");
	else if (changeToMake == REMOVE_MEMBER)
		EventNode.addAttribute("changeToMake", "removeMember");
	EventNode.addAttribute("characterID", QString::number(characterID).toUtf8().data());
	EventNode.addAttribute("characterReferencedInVariable", (characterReferencedInVariable) ? "true":"false");
	return EventNode;
}

void BaseEvent::ChangeParty::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(characterID);
	storageFile.putSignedInt(changeToMake);
	storageFile.putBool(characterReferencedInVariable);
}

QStringList BaseEvent::ChangeParty::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Change Party: ";
	if (changeToMake == ADD_MEMBER)
		viewableText[0] += "Add ";
	else
		viewableText[0] += "Remove ";
	if (characterReferencedInVariable)
		viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(characterID));
	else
		viewableText[0] += ProjectData::characterNames[characterID];
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::ChangeParty::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	ConfigureEvent_ChangeParty *configParty = new ConfigureEvent_ChangeParty;
	bool configured = false;
	configParty->setOperation((changeToMake == ADD_MEMBER) ? true:false);
	configParty->setIsReferencedCharacter(characterReferencedInVariable);
	configParty->setCharacterID(characterID);
	if (configParty->exec())
	{
		configured = true;
		changeToMake = (configParty->isAddOperation()) ? ADD_MEMBER:REMOVE_MEMBER;
		characterID = configParty->getCharacterID();
		characterReferencedInVariable = configParty->isReferencedCharacter();
	}
	delete configParty;
	return configured;
}
#endif

bool BaseEvent::ChangeParty::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	QStringList tempStringList;
	tempStringList.clear();
	tempStringList << "addMember" << "removeMember";
	if (!XMLTools::attributeExists(EventNode, "changeToMake"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "changeToMake", tempStringList))
		return false;
	if (!XMLTools::attributeExists(EventNode, "characterID"))
		return false;
	tempStringList.clear();
	tempStringList << "true" << "false";
	if (!XMLTools::attributeExists(EventNode, "characterReferencedInVariable"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "characterReferencedInVariable", tempStringList, 1))
		return false;
	return true;
}

BaseEvent::ChangeParty::ChangeParty(BaseEvent *pBase) : Event(pBase)
{
	characterID = 0;
	changeToMake = ADD_MEMBER;
	characterReferencedInVariable = false;
}

BaseEvent::ChangeParty::ChangeParty(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	QString tempString;
	tempString = EventNode.getAttribute("changeToMake");
	changeToMake = (tempString == "addMember") ? ADD_MEMBER:REMOVE_MEMBER;
	characterID = QString(EventNode.getAttribute("characterID")).toInt();
	tempString = EventNode.getAttribute("characterReferencedInVariable");
	characterReferencedInVariable = (tempString == "true") ? true:false;
}

BaseEvent::ChangeParty::ChangeParty(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	characterID = storageFile.getSignedInt();
	changeToMake = storageFile.getSignedInt();
	characterReferencedInVariable = storageFile.getBool();
}

BaseEvent::ChangeParty::ChangeParty(ChangeParty *other, BaseEvent *pBase) : Event(pBase)
{
	characterID = other->characterID;
	changeToMake = other->changeToMake;
	characterReferencedInVariable = other->characterReferencedInVariable;
}
//END BaseEvent::ChangeParty Member Functions

//BEGIN BaseEvent::ChangeSelectedArea Member Functions
XMLNode BaseEvent::ChangeSelectedArea::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	EventNode.addAttribute("objectName", objectName.toUtf8().data());
	return EventNode;
}

void BaseEvent::ChangeSelectedArea::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putString(objectName.toUtf8().data());
}

QStringList BaseEvent::ChangeSelectedArea::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Change Selected Area: ";
	viewableText[0] += objectName;
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::ChangeSelectedArea::configureEvent(int callerID, int callerType)
{
	ConfigureEvent_ChangeSelectedArea *configChangeSelectedArea = new ConfigureEvent_ChangeSelectedArea(callerID, callerType);
	bool configured = false;
	configChangeSelectedArea->setMenuObject(objectName);
	if (configChangeSelectedArea->exec())
	{
		configured = true;
		objectName = configChangeSelectedArea->getMenuObject();
	}
	delete configChangeSelectedArea;
	return configured;
}
#endif

bool BaseEvent::ChangeSelectedArea::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	if (!XMLTools::attributeExists(EventNode, "objectName"))
		return false;
	return true;
}

BaseEvent::ChangeSelectedArea::ChangeSelectedArea(BaseEvent *pBase) : Event(pBase)
{
	objectName = "";
}

BaseEvent::ChangeSelectedArea::ChangeSelectedArea(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	objectName = EventNode.getAttribute("objectName");
}

BaseEvent::ChangeSelectedArea::ChangeSelectedArea(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	char *temp_string;
	temp_string = storageFile.getString();
	objectName = temp_string;
	delete[] temp_string;
}

BaseEvent::ChangeSelectedArea::ChangeSelectedArea(ChangeSelectedArea *other, BaseEvent *pBase) : Event(pBase)
{
	objectName = "";
	objectName.append(other->objectName);
}
//END BaseEvent::ChangeSelectedArea Member Functions

//BEGIN BaseEvent::ChangeSkills Member Functions
XMLNode BaseEvent::ChangeSkills::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	if (target == ENTIRE_PARTY)
		EventNode.addAttribute("target", "entireParty");
	else if (target == SPECIFIC_CHARACTER)
	{
		EventNode.addAttribute("target", "specificCharacter");
		EventNode.addAttribute("characterID", QString::number(characterID).toUtf8().data());
	}
	else if (target == REFERENCED_CHARACTER)
	{
		EventNode.addAttribute("target", "referencedCharacter");
		EventNode.addAttribute("variableID", QString::number(characterID).toUtf8().data());
	}
	if (operation == ADD_SKILL)
		EventNode.addAttribute("operation", "addSkill");
	else if (operation == REMOVE_SKILL)
		EventNode.addAttribute("operation", "removeSkill");
	EventNode.addAttribute("skillID", QString::number(skillID).toUtf8().data());
	EventNode.addAttribute("skillReferencedInVariable", (skillReferencedInVariable) ? "true":"false");
	return EventNode;
}

void BaseEvent::ChangeSkills::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(characterID);
	storageFile.putSignedInt(target);
	storageFile.putSignedInt(operation);
	storageFile.putBool(skillReferencedInVariable);
}

QStringList BaseEvent::ChangeSkills::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Change Skills: ";
	if (target == ENTIRE_PARTY)
		viewableText[0] += "Entire Party";
	else if (target == SPECIFIC_CHARACTER)
		viewableText[0] += ProjectData::characterNames[characterID];
	else
		viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(characterID));
	viewableText[0] += " ";
	if (operation == ADD_SKILL)
		viewableText[0] += "Learns ";
	else
		viewableText[0] += "Forgets ";
	if (skillReferencedInVariable)
		viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(skillID));
	else
		viewableText[0] += ProjectData::skillNames[skillID];
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::ChangeSkills::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	ConfigureEvent_ChangeSkills *configSkills = new ConfigureEvent_ChangeSkills;
	bool configured = false;
	configSkills->setTarget(target);
	configSkills->setCharacterID(characterID);
	configSkills->setOperation((operation == ADD_SKILL) ? true:false);
	configSkills->setIsReferencedSkill(skillReferencedInVariable);
	configSkills->setSkillID(skillID);
	if (configSkills->exec())
	{
		configured = true;
		target = configSkills->getTarget();
		characterID = configSkills->getCharacterID();
		operation = (configSkills->isAddOperation()) ? ADD_SKILL:REMOVE_SKILL;
		skillID = configSkills->getSkillID();
		skillReferencedInVariable = configSkills->isReferencedSkill();
	}
	delete configSkills;
	return configured;
}
#endif

bool BaseEvent::ChangeSkills::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	QStringList tempStringList;
	QString tempString;
	tempStringList.clear();
	tempStringList << "entireParty" << "specificCharacter" << "referencedCharacter";
	if (!XMLTools::attributeExists(EventNode, "target"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "target", tempStringList))
		return false;
	tempString = EventNode.getAttribute("target");
	if (tempString == "specificCharacter")
	{
		if (!XMLTools::attributeExists(EventNode, "characterID"))
			return false;
	}
	else if (tempString == "referencedCharacter")
	{
		if (!XMLTools::attributeExists(EventNode, "variableID"))
			return false;
	}
	tempStringList.clear();
	tempStringList << "addSkill" << "removeSkill";
	if (!XMLTools::attributeExists(EventNode, "operation"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "operation", tempStringList))
		return false;
	if (!XMLTools::attributeExists(EventNode, "skillID"))
		return false;
	tempStringList.clear();
	tempStringList << "true" << "false";
	if (!XMLTools::attributeExists(EventNode, "skillReferencedInVariable"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "skillReferencedInVariable", tempStringList, 1))
		return false;
	return true;
}

BaseEvent::ChangeSkills::ChangeSkills(BaseEvent *pBase) : Event(pBase)
{
	characterID = 0;
	skillID = 0;
	target = ENTIRE_PARTY;
	operation = ADD_SKILL;
	skillReferencedInVariable = false;
}

BaseEvent::ChangeSkills::ChangeSkills(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	QString tempString;
	tempString = EventNode.getAttribute("target");
	if (tempString == "entireParty")
	{
		target = ENTIRE_PARTY;
		characterID = 0;
	}
	else if (tempString == "specificCharacter")
	{
		target = SPECIFIC_CHARACTER;
		characterID = QString(EventNode.getAttribute("characterID")).toInt();
	}
	else if (tempString == "referencedCharacter")
	{
		target = REFERENCED_CHARACTER;
		characterID = QString(EventNode.getAttribute("variableID")).toInt();
	}
	tempString = EventNode.getAttribute("operation");
	operation = (tempString == "addSkill") ? ADD_SKILL:REMOVE_SKILL;
	skillID = QString(EventNode.getAttribute("skillID")).toInt();
	tempString = EventNode.getAttribute("skillReferencedInVariable");
	skillReferencedInVariable = (tempString == "true") ? true:false;
}

BaseEvent::ChangeSkills::ChangeSkills(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	characterID = storageFile.getSignedInt();
	skillID = storageFile.getSignedInt();
	target = storageFile.getSignedInt();
	operation = storageFile.getSignedInt();
	skillReferencedInVariable = storageFile.getBool();
}

BaseEvent::ChangeSkills::ChangeSkills(ChangeSkills *other, BaseEvent *pBase) : Event(pBase)
{
	characterID = other->characterID;
	skillID = other->skillID;
	target = other->target;
	operation = other->operation;
	skillReferencedInVariable = other->skillReferencedInVariable;
}
//END BaseEvent::ChangeSkills Member Functions

//BEGIN BaseEvent::ChangeSystemBGM Member Functions
XMLNode BaseEvent::ChangeSystemBGM::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	if (bgmToChange == ProjectData::BGM_TITLESCREEN)
		EventNode.addAttribute("bgmToChange", "titleScreen");
	else if (bgmToChange == ProjectData::BGM_BATTLE)
		EventNode.addAttribute("bgmToChange", "battle");
	else if (bgmToChange == ProjectData::BGM_VICTORY)
		EventNode.addAttribute("bgmToChange", "victory");
	else if (bgmToChange == ProjectData::BGM_INN)
		EventNode.addAttribute("bgmToChange", "inn");
	else if (bgmToChange == ProjectData::BGM_GAMEOVER)
		EventNode.addAttribute("bgmToChange", "gameOver");
	EventNode.addAttribute("bgmLocation", ProjectData::getRelativeResourcePath(newBGM.location).toUtf8().data());
	EventNode.addAttribute("volume", QString::number(newBGM.volume).toUtf8().data());
	EventNode.addAttribute("panning", QString::number(newBGM.panning).toUtf8().data());
	EventNode.addAttribute("speed", QString::number(newBGM.speed).toUtf8().data());
	EventNode.addAttribute("fadeInTime", QString::number(newBGM.fadeInTime).toUtf8().data());
	EventNode.addAttribute("retainPitch", (newBGM.retainPitch) ? "true":"false");
	return EventNode;
}

void BaseEvent::ChangeSystemBGM::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putString(newBGM.location.toUtf8().data());
	storageFile.putFloat(newBGM.volume);
	storageFile.putFloat(newBGM.panning);
	storageFile.putFloat(newBGM.speed);
	storageFile.putSignedInt(newBGM.fadeInTime);
	storageFile.putBool(newBGM.retainPitch);
	storageFile.putSignedInt(bgmToChange);
}

QStringList BaseEvent::ChangeSystemBGM::getResources()
{
	return QStringList() << newBGM.location;
}

QStringList BaseEvent::ChangeSystemBGM::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	QFileInfo info = QFileInfo(newBGM.location);
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Change System BGM: Set ";
	if (bgmToChange == ProjectData::BGM_TITLESCREEN)
		viewableText[0] += "Title Screen to ";
	else if (bgmToChange == ProjectData::BGM_BATTLE)
		viewableText[0] += "Battle to ";
	else if (bgmToChange == ProjectData::BGM_VICTORY)
		viewableText[0] += "Victory to ";
	else if (bgmToChange == ProjectData::BGM_INN)
		viewableText[0] += "Inn to ";
	else if (bgmToChange == ProjectData::BGM_GAMEOVER)
		viewableText[0] += "Game Over to ";
	if (newBGM.location.isEmpty())
		viewableText[0] += "(OFF)";
	else
		viewableText[0] += info.fileName();
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::ChangeSystemBGM::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	ConfigureEvent_ChangeSystemBGM *configBGM = new ConfigureEvent_ChangeSystemBGM;
	bool configured = false;
	configBGM->setBGMToChange(bgmToChange);
	configBGM->setBGM(&newBGM);
	if (configBGM->exec())
	{
		configured = true;
		bgmToChange = configBGM->getBGMToChange();
		configBGM->getBGM(&newBGM);
	}
	delete configBGM;
	return configured;
}
#endif

void BaseEvent::ChangeSystemBGM::getNewBGM(AudioManager::MusicInfo *other)
{
	other->location = newBGM.location;
	other->volume = newBGM.volume;
	other->panning = newBGM.panning;
	other->speed = newBGM.speed;
	other->fadeInTime = newBGM.fadeInTime;
	other->retainPitch = newBGM.retainPitch;
}

void BaseEvent::ChangeSystemBGM::setNewBGM(AudioManager::MusicInfo *other)
{
	newBGM.location = other->location;
	newBGM.volume = other->volume;
	newBGM.panning = other->panning;
	newBGM.speed = other->speed;
	newBGM.fadeInTime = other->fadeInTime;
	newBGM.retainPitch = other->retainPitch;
}

bool BaseEvent::ChangeSystemBGM::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	QStringList tempStringList;
	if (!XMLTools::attributeExists(EventNode, "bgmToChange"))
		return false;
	tempStringList.clear();
	tempStringList << "titleScreen" << "battle" << "victory" << "inn" << "gameOver";
	if (!XMLTools::attributeStringValid(EventNode, "bgmToChange", tempStringList))
		return false;
	if (!XMLTools::attributeExists(EventNode, "bgmLocation"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "volume"))
		return false;
	if (!XMLTools::attributeFloatRangeValid(EventNode, "volume", 0.0, 1.0))
		return false;
	if (!XMLTools::attributeExists(EventNode, "panning"))
		return false;
	if (!XMLTools::attributeFloatRangeValid(EventNode, "panning", -1.0, 1.0))
		return false;
	if (!XMLTools::attributeExists(EventNode, "speed"))
		return false;
	if (!XMLTools::attributeFloatRangeValid(EventNode, "speed", 0.5, 2.0))
		return false;
	if (!XMLTools::attributeExists(EventNode, "fadeInTime"))
		return false;
	if (!XMLTools::attributeIntRangeValid(EventNode, "fadeInTime", 0, 10))
		return false;
	tempStringList.clear();
	tempStringList << "true" << "false";
	if (!XMLTools::attributeExists(EventNode, "retainPitch"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "retainPitch", tempStringList, 1))
		return false;
	return true;
}

BaseEvent::ChangeSystemBGM::ChangeSystemBGM(BaseEvent *pBase) : Event(pBase)
{
	newBGM.location = "";
	newBGM.volume = 1.0;
	newBGM.panning = 0.0;
	newBGM.speed = 1.0;
	newBGM.fadeInTime = 0;
	newBGM.retainPitch = false;
	bgmToChange = ProjectData::BGM_TITLESCREEN;
}

BaseEvent::ChangeSystemBGM::ChangeSystemBGM(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	QString tempString;
	tempString = EventNode.getAttribute("bgmToChange");
	if (tempString == "titleScreen")
		bgmToChange = ProjectData::BGM_TITLESCREEN;
	else if (tempString == "battle")
		bgmToChange = ProjectData::BGM_BATTLE;
	else if (tempString == "victory")
		bgmToChange = ProjectData::BGM_VICTORY;
	else if (tempString == "inn")
		bgmToChange = ProjectData::BGM_INN;
	else if (tempString == "gameOver")
		bgmToChange = ProjectData::BGM_GAMEOVER;
	newBGM.location = ProjectData::getAbsoluteResourcePath(EventNode.getAttribute("bgmLocation"));
	newBGM.volume = QString(EventNode.getAttribute("volume")).toFloat();
	newBGM.panning = QString(EventNode.getAttribute("panning")).toFloat();
	newBGM.speed = QString(EventNode.getAttribute("speed")).toFloat();
	newBGM.fadeInTime = QString(EventNode.getAttribute("fadeInTime")).toInt();
	tempString = EventNode.getAttribute("retainPitch");
	newBGM.retainPitch = (tempString == "true") ? true:false;
}

BaseEvent::ChangeSystemBGM::ChangeSystemBGM(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	char *temp_string;
	temp_string = storageFile.getString();
	newBGM.location = temp_string;
	delete[] temp_string;
	newBGM.volume = storageFile.getFloat();
	newBGM.panning = storageFile.getFloat();
	newBGM.speed = storageFile.getFloat();
	newBGM.fadeInTime = storageFile.getSignedInt();
	newBGM.retainPitch = storageFile.getBool();
	bgmToChange = storageFile.getSignedInt();
}

BaseEvent::ChangeSystemBGM::ChangeSystemBGM(ChangeSystemBGM *other, BaseEvent *pBase) : Event(pBase)
{
	newBGM.location = "";
	newBGM.location.append(other->newBGM.location);
	newBGM.volume = other->newBGM.volume;
	newBGM.panning = other->newBGM.panning;
	newBGM.speed = other->newBGM.speed;
	newBGM.fadeInTime = other->newBGM.fadeInTime;
	newBGM.retainPitch = other->newBGM.retainPitch;
	bgmToChange = other->bgmToChange;
}
//END BaseEvent::ChangeSystemBGM Member Functions

//BEGIN BaseEvent::ChangeSystemGraphics Member Functions
XMLNode BaseEvent::ChangeSystemGraphics::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	if (graphicToChange == FRAME_BG)
		EventNode.addAttribute("graphicToChange", "frameBG");
	else if (graphicToChange == FRAME_BORDER)
		EventNode.addAttribute("graphicToChange", "frameBorder");
	else if (graphicToChange == SELECTION_BORDER)
		EventNode.addAttribute("graphicToChange", "selectionBorder");
	else if (graphicToChange == EQUIPMENT_INDICATORS)
		EventNode.addAttribute("graphicToChange", "equipmentIndicators");
	else if (graphicToChange == MENU_BG)
		EventNode.addAttribute("graphicToChange", "menuBG");
	else if (graphicToChange == SCROLL_ARROWS)
		EventNode.addAttribute("graphicToChange", "scrollArrows");
	else if (graphicToChange == GRAPPLE_ARM)
		EventNode.addAttribute("graphicToChange", "grappleArm");
	else if (graphicToChange == GRAPPLE_HOOK)
		EventNode.addAttribute("graphicToChange", "grappleHook");
	else if (graphicToChange == TIMER_NUMBERS)
		EventNode.addAttribute("graphicToChange", "timerNumbers");
	EventNode.addAttribute("newGraphicLocation", ProjectData::getRelativeResourcePath(newGraphicLocation).toUtf8().data());
	return EventNode;
}

void BaseEvent::ChangeSystemGraphics::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putString(newGraphicLocation.toUtf8().data());
	storageFile.putSignedInt(graphicToChange);
}

QStringList BaseEvent::ChangeSystemGraphics::getResources()
{
	return QStringList() << newGraphicLocation;
}

QStringList BaseEvent::ChangeSystemGraphics::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	QString location = "";
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Change System Graphics: Set ";
	if (graphicToChange == FRAME_BG)
		viewableText[0] += "Frame Background to \"";
	else if (graphicToChange == FRAME_BORDER)
		viewableText[0] += "Frame Border to \"";
	else if (graphicToChange == SELECTION_BORDER)
		viewableText[0] += "Selection Border to \"";
	else if (graphicToChange == EQUIPMENT_INDICATORS)
		viewableText[0] += "Equipment Indicators to \"";
	else if (graphicToChange == MENU_BG)
		viewableText[0] += "Menu Background to \"";
	else if (graphicToChange == SCROLL_ARROWS)
		viewableText[0] += "Scroll Arrows to \"";
	else if (graphicToChange == GRAPPLE_ARM)
		viewableText[0] += "Grapple Arm to \"";
	else if (graphicToChange == GRAPPLE_HOOK)
		viewableText[0] += "Grapple Hook to \"";
	else if (graphicToChange == TIMER_NUMBERS)
		viewableText[0] += "Timer Numbers to \"";
	viewableText[0] += ProjectData::getRelativeResourcePath(newGraphicLocation);
	viewableText[0] += "\"";
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::ChangeSystemGraphics::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	ConfigureEvent_ChangeSystemGraphic *configSystemGraphic = new ConfigureEvent_ChangeSystemGraphic;
	bool configured = false;
	configSystemGraphic->setGraphicToChange(graphicToChange);
	configSystemGraphic->setGraphicLocation(newGraphicLocation);
	if (configSystemGraphic->exec())
	{
		configured = true;
		graphicToChange = configSystemGraphic->getGraphicToChange();
		newGraphicLocation = configSystemGraphic->getGraphicLocation();
	}
	delete configSystemGraphic;
	return configured;
}
#endif

bool BaseEvent::ChangeSystemGraphics::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	QStringList tempStringList = QStringList() << "frameBG" << "frameBorder" << "selectionBorder" << "equipmentIndicators" << "menuBG" << "scrollArrows" << "grappleArm" << "grappleHook" << "timerNumbers";
	if (!XMLTools::attributeExists(EventNode, "graphicToChange"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "graphicToChange", tempStringList))
		return false;
	if (!XMLTools::attributeExists(EventNode, "newGraphicLocation"))
		return false;
	return true;
}

BaseEvent::ChangeSystemGraphics::ChangeSystemGraphics(BaseEvent *pBase) : Event(pBase)
{
	newGraphicLocation = "";
	graphicToChange = FRAME_BG;
}

BaseEvent::ChangeSystemGraphics::ChangeSystemGraphics(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	QStringList tempStringList = QStringList() << "frameBG" << "frameBorder" << "selectionBorder" << "equipmentIndicators" << "menuBG" << "scrollArrows" << "grappleArm" << "grappleHook" << "timerNumbers";
	graphicToChange = tempStringList.indexOf(EventNode.getAttribute("graphicToChange"));
	newGraphicLocation = ProjectData::getAbsoluteResourcePath(EventNode.getAttribute("newGraphicLocation"));
}

BaseEvent::ChangeSystemGraphics::ChangeSystemGraphics(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	char *temp_string;
	temp_string = storageFile.getString();
	newGraphicLocation = temp_string;
	delete[] temp_string;
	graphicToChange = storageFile.getSignedInt();
}

BaseEvent::ChangeSystemGraphics::ChangeSystemGraphics(ChangeSystemGraphics *other, BaseEvent *pBase) : Event(pBase)
{
	newGraphicLocation = "";
	newGraphicLocation.append(other->newGraphicLocation);
	graphicToChange = other->graphicToChange;
}
//END BaseEvent::ChangeSystemGraphics Member Functions

//BEGIN BaseEvent::ChangeSystemSoundEffect Member Functions
XMLNode BaseEvent::ChangeSystemSoundEffect::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	if (soundEffectToChange == ProjectData::SFX_CURSOR)
		EventNode.addAttribute("soundEffectToChange", "cursor");
	else if (soundEffectToChange == ProjectData::SFX_SELECTCHOICE)
		EventNode.addAttribute("soundEffectToChange", "selectChoice");
	else if (soundEffectToChange == ProjectData::SFX_CANCEL)
		EventNode.addAttribute("soundEffectToChange", "cancel");
	else if (soundEffectToChange == ProjectData::SFX_BATTLETRANSITION)
		EventNode.addAttribute("soundEffectToChange", "battleTransition");
	else if (soundEffectToChange == ProjectData::SFX_FLEEBATTLE)
		EventNode.addAttribute("soundEffectToChange", "fleeBattle");
	else if (soundEffectToChange == ProjectData::SFX_CHARACTERLEVELUP)
		EventNode.addAttribute("soundEffectToChange", "characterLevelUp");
	else if (soundEffectToChange == ProjectData::SFX_EVADEATTACK)
		EventNode.addAttribute("soundEffectToChange", "evadeAttack");
	else if (soundEffectToChange == ProjectData::SFX_ITEMUSED)
		EventNode.addAttribute("soundEffectToChange", "itemUsed");
	EventNode.addAttribute("sfxLocation", ProjectData::getRelativeResourcePath(newSoundEffect.location).toUtf8().data());
	EventNode.addAttribute("volume", QString::number(newSoundEffect.volume).toUtf8().data());
	EventNode.addAttribute("panning", QString::number(newSoundEffect.panning).toUtf8().data());
	EventNode.addAttribute("speed", QString::number(newSoundEffect.speed).toUtf8().data());
	EventNode.addAttribute("numLoops", QString::number(newSoundEffect.numLoops).toUtf8().data());
	EventNode.addAttribute("retainPitch", (newSoundEffect.retainPitch) ? "true":"false");
	return EventNode;
}

void BaseEvent::ChangeSystemSoundEffect::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putString(newSoundEffect.location.toUtf8().data());
	storageFile.putFloat(newSoundEffect.volume);
	storageFile.putFloat(newSoundEffect.panning);
	storageFile.putFloat(newSoundEffect.speed);
	storageFile.putSignedInt(newSoundEffect.numLoops);
	storageFile.putBool(newSoundEffect.retainPitch);
	storageFile.putSignedInt(soundEffectToChange);
}

QStringList BaseEvent::ChangeSystemSoundEffect::getResources()
{
	return QStringList() << newSoundEffect.location;
}

QStringList BaseEvent::ChangeSystemSoundEffect::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	QFileInfo info = QFileInfo(newSoundEffect.location);
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Change System Sound Effect: Set ";
	if (soundEffectToChange == ProjectData::SFX_CURSOR)
		viewableText[0] += "Cursor to ";
	else if (soundEffectToChange == ProjectData::SFX_SELECTCHOICE)
		viewableText[0] += "Select Choice to ";
	else if (soundEffectToChange == ProjectData::SFX_BUZZER)
		viewableText[0] += "Buzzer to ";
	else if (soundEffectToChange == ProjectData::SFX_CANCEL)
		viewableText[0] += "Cancel to ";
	else if (soundEffectToChange == ProjectData::SFX_BATTLETRANSITION)
		viewableText[0] += "Battle Transition to ";
	else if (soundEffectToChange == ProjectData::SFX_FLEEBATTLE)
		viewableText[0] += "Flee Battle to ";
	else if (soundEffectToChange == ProjectData::SFX_CHARACTERLEVELUP)
		viewableText[0] += "Character Level Up to ";
	else if (soundEffectToChange == ProjectData::SFX_EVADEATTACK)
		viewableText[0] += "Evade Attack to ";
	else if (soundEffectToChange == ProjectData::SFX_ITEMUSED)
		viewableText[0] += "Item Used to ";
	if (newSoundEffect.location.isEmpty())
		viewableText[0] += "(OFF)";
	else
		viewableText[0] += info.fileName();
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::ChangeSystemSoundEffect::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	ConfigureEvent_ChangeSystemSoundEffect *configSFX = new ConfigureEvent_ChangeSystemSoundEffect;
	bool configured = false;
	configSFX->setSoundEffectToChange(soundEffectToChange);
	configSFX->setSoundEffect(&newSoundEffect);
	if (configSFX->exec())
	{
		configured = true;
		soundEffectToChange = configSFX->getSoundEffectToChange();
		configSFX->getSoundEffect(&newSoundEffect);
	}
	delete configSFX;
	return configured;
}
#endif

void BaseEvent::ChangeSystemSoundEffect::getNewSoundEffect(AudioManager::SoundEffectInfo *other)
{
	other->location = "";
	other->location.append(newSoundEffect.location);
	other->volume = newSoundEffect.volume;
	other->panning = newSoundEffect.panning;
	other->speed = newSoundEffect.speed;
	other->numLoops = newSoundEffect.numLoops;
	other->retainPitch = newSoundEffect.retainPitch;
}

void BaseEvent::ChangeSystemSoundEffect::setNewSoundEffect(AudioManager::SoundEffectInfo *other)
{
	newSoundEffect.location = "";
	newSoundEffect.location.append(other->location);
	newSoundEffect.volume = other->volume;
	newSoundEffect.panning = other->panning;
	newSoundEffect.speed = other->speed;
	newSoundEffect.numLoops = other->numLoops;
	newSoundEffect.retainPitch = other->retainPitch;
}

bool BaseEvent::ChangeSystemSoundEffect::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	QStringList tempStringList;
	if (!XMLTools::attributeExists(EventNode, "soundEffectToChange"))
		return false;
	tempStringList.clear();
	tempStringList << "cursor" << "selectChoice" << "buzzer" << "cancel" << "battleTransition" << "fleeBattle" << "characterLevelUp" << "evadeAttack" << "itemUsed";
	if (!XMLTools::attributeStringValid(EventNode, "soundEffectToChange", tempStringList))
		return false;
	if (!XMLTools::attributeExists(EventNode, "sfxLocation"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "volume"))
		return false;
	if (!XMLTools::attributeFloatRangeValid(EventNode, "volume", 0.0, 1.0))
		return false;
	if (!XMLTools::attributeExists(EventNode, "panning"))
		return false;
	if (!XMLTools::attributeFloatRangeValid(EventNode, "panning", -1.0, 1.0))
		return false;
	if (!XMLTools::attributeExists(EventNode, "speed"))
		return false;
	if (!XMLTools::attributeFloatRangeValid(EventNode, "speed", 0.5, 2.0))
		return false;
	if (!XMLTools::attributeExists(EventNode, "numLoops"))
		return false;
	tempStringList.clear();
	tempStringList << "true" << "false";
	if (!XMLTools::attributeExists(EventNode, "retainPitch"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "retainPitch", tempStringList, 1))
		return false;
	return true;
}

BaseEvent::ChangeSystemSoundEffect::ChangeSystemSoundEffect(BaseEvent *pBase) : Event(pBase)
{
	newSoundEffect.location = "";
	newSoundEffect.volume = 1.0;
	newSoundEffect.panning = 0.0;
	newSoundEffect.speed = 1.0;
	newSoundEffect.numLoops = 0;
	newSoundEffect.sfxID = -1;
	newSoundEffect.retainPitch = false;
	soundEffectToChange = ProjectData::SFX_CURSOR;
}

BaseEvent::ChangeSystemSoundEffect::ChangeSystemSoundEffect(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	QString tempString = EventNode.getAttribute("soundEffectToChange");
	if (tempString == "cursor")
		soundEffectToChange = ProjectData::SFX_CURSOR;
	else if (tempString == "selectChoice")
		soundEffectToChange = ProjectData::SFX_SELECTCHOICE;
	else if (tempString == "buzzer")
		soundEffectToChange = ProjectData::SFX_CANCEL;
	else if (tempString == "battleTransition")
		soundEffectToChange = ProjectData::SFX_BATTLETRANSITION;
	else if (tempString == "fleeBattle")
		soundEffectToChange = ProjectData::SFX_FLEEBATTLE;
	else if (tempString == "characterLevelUp")
		soundEffectToChange = ProjectData::SFX_CHARACTERLEVELUP;
	else if (tempString == "evadeAttack")
		soundEffectToChange = ProjectData::SFX_EVADEATTACK;
	else if (tempString == "itemUsed")
		soundEffectToChange = ProjectData::SFX_ITEMUSED;
	newSoundEffect.location = ProjectData::getAbsoluteResourcePath(EventNode.getAttribute("sfxLocation"));
	newSoundEffect.volume = QString(EventNode.getAttribute("volume")).toFloat();
	newSoundEffect.panning = QString(EventNode.getAttribute("panning")).toFloat();
	newSoundEffect.speed = QString(EventNode.getAttribute("speed")).toFloat();
	newSoundEffect.numLoops = QString(EventNode.getAttribute("numLoops")).toInt();
	newSoundEffect.sfxID = -1;
	tempString = EventNode.getAttribute("retainPitch");
	newSoundEffect.retainPitch = (tempString == "true") ? true:false;
}

BaseEvent::ChangeSystemSoundEffect::ChangeSystemSoundEffect(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	char *temp_string;
	temp_string = storageFile.getString();
	newSoundEffect.location = temp_string;
	delete[] temp_string;
	newSoundEffect.volume = storageFile.getFloat();
	newSoundEffect.panning = storageFile.getFloat();
	newSoundEffect.speed = storageFile.getFloat();
	newSoundEffect.numLoops = storageFile.getSignedInt();
	newSoundEffect.sfxID = -1;
	newSoundEffect.retainPitch = storageFile.getBool();
	soundEffectToChange = storageFile.getSignedInt();
}

BaseEvent::ChangeSystemSoundEffect::ChangeSystemSoundEffect(ChangeSystemSoundEffect *other, BaseEvent *pBase) : Event(pBase)
{
	newSoundEffect.location = "";
	newSoundEffect.location.append(other->newSoundEffect.location);
	newSoundEffect.volume = other->newSoundEffect.volume;
	newSoundEffect.panning = other->newSoundEffect.panning;
	newSoundEffect.speed = other->newSoundEffect.speed;
	newSoundEffect.numLoops = other->newSoundEffect.numLoops;
	newSoundEffect.sfxID = -1;
	newSoundEffect.retainPitch = other->newSoundEffect.retainPitch;
	soundEffectToChange = other->soundEffectToChange;
}
//END BaseEvent::ChangeSystemSoundEffect Member Functions

//BEGIN BaseEvent::ChangeText Member Functions
XMLNode BaseEvent::ChangeText::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	EventNode.addAttribute("objectName", objectName.toUtf8().data());
	EventNode.addAttribute("saveSlotID", QString::number(saveSlotID).toUtf8().data());
	if (textType == MenuText::TYPE_PLAINTEXT)
	{
		EventNode.addAttribute("plainText", textData.plainText->toUtf8().data());
		EventNode.addAttribute("textType", "plain text");
	}
	else if (textType == MenuText::TYPE_PARTYMEMBER)
	{
		EventNode.addAttribute("textType", "party member");
		EventNode.addAttribute("partyMemberID", QString::number(textData.partyMemberText->memberID).toUtf8().data());
		if (textData.partyMemberText->textType == MenuText::PARTYMEMBER_NAME)
			EventNode.addAttribute("partyMemberTextType", "name");
		else if (textData.partyMemberText->textType == MenuText::PARTYMEMBER_CLASS)
			EventNode.addAttribute("partyMemberTextType", "class");
		else if (textData.partyMemberText->textType == MenuText::PARTYMEMBER_CONDITION)
			EventNode.addAttribute("partyMemberTextType", "condition");
		else if (textData.partyMemberText->textType == MenuText::PARTYMEMBER_LEVEL)
			EventNode.addAttribute("partyMemberTextType", "level");
		else if (textData.partyMemberText->textType == MenuText::PARTYMEMBER_HP)
			EventNode.addAttribute("partyMemberTextType", "hp");
		else if (textData.partyMemberText->textType == MenuText::PARTYMEMBER_MAXHP)
			EventNode.addAttribute("partyMemberTextType", "max hp");
		else if (textData.partyMemberText->textType == MenuText::PARTYMEMBER_MP)
			EventNode.addAttribute("partyMemberTextType", "mp");
		else if (textData.partyMemberText->textType == MenuText::PARTYMEMBER_MAXMP)
			EventNode.addAttribute("partyMemberTextType", "max mp");
		else if (textData.partyMemberText->textType == MenuText::PARTYMEMBER_EXPERIENCE)
			EventNode.addAttribute("partyMemberTextType", "experience");
		else if (textData.partyMemberText->textType == MenuText::PARTYMEMBER_EXPERIENCETONEXTLEVEL)
			EventNode.addAttribute("partyMemberTextType", "experience to next level");
		else if (textData.partyMemberText->textType == MenuText::PARTYMEMBER_BASEATTACK)
			EventNode.addAttribute("partyMemberTextType", "base attack");
		else if (textData.partyMemberText->textType == MenuText::PARTYMEMBER_TOTALATTACK)
			EventNode.addAttribute("partyMemberTextType", "total attack");
		else if (textData.partyMemberText->textType == MenuText::PARTYMEMBER_BASEDEFENSE)
			EventNode.addAttribute("partyMemberTextType", "base defense");
		else if (textData.partyMemberText->textType == MenuText::PARTYMEMBER_TOTALDEFENSE)
			EventNode.addAttribute("partyMemberTextType", "total defense");
		else if (textData.partyMemberText->textType == MenuText::PARTYMEMBER_BASEWISDOM)
			EventNode.addAttribute("partyMemberTextType", "base wisdom");
		else if (textData.partyMemberText->textType == MenuText::PARTYMEMBER_TOTALWISDOM)
			EventNode.addAttribute("partyMemberTextType", "total wisdom");
		else if (textData.partyMemberText->textType == MenuText::PARTYMEMBER_BASEAGILITY)
			EventNode.addAttribute("partyMemberTextType", "base agility");
		else if (textData.partyMemberText->textType == MenuText::PARTYMEMBER_TOTALAGILITY)
			EventNode.addAttribute("partyMemberTextType", "total agility");
		else if (textData.partyMemberText->textType == MenuText::PARTYMEMBER_EQUIPPEDWEAPON)
			EventNode.addAttribute("partyMemberTextType", "equipped weapon");
		else if (textData.partyMemberText->textType == MenuText::PARTYMEMBER_EQUIPPEDSHIELD)
			EventNode.addAttribute("partyMemberTextType", "equipped shield");
		else if (textData.partyMemberText->textType == MenuText::PARTYMEMBER_EQUIPPEDARMOR)
			EventNode.addAttribute("partyMemberTextType", "equipped armor");
		else if (textData.partyMemberText->textType == MenuText::PARTYMEMBER_EQUIPPEDHELMET)
			EventNode.addAttribute("partyMemberTextType", "equipped helmet");
		else if (textData.partyMemberText->textType == MenuText::PARTYMEMBER_EQUIPPEDACCESSORY)
			EventNode.addAttribute("partyMemberTextType", "equipped accessory");
		if (textData.partyMemberText->textType >= MenuText::PARTYMEMBER_EQUIPPEDWEAPON && textData.partyMemberText->textType <= MenuText::PARTYMEMBER_EQUIPPEDACCESSORY)
		{
			if (textData.partyMemberText->equipmentTextType == MenuText::EQUIPMENT_NAME)
				EventNode.addAttribute("partyMemberEquipmentTextType", "name");
			else if (textData.partyMemberText->equipmentTextType == MenuText::EQUIPMENT_DESCRIPTION)
				EventNode.addAttribute("partyMemberEquipmentTextType", "description");
			else if (textData.partyMemberText->equipmentTextType == MenuText::EQUIPMENT_ATTACK)
				EventNode.addAttribute("partyMemberEquipmentTextType", "attack");
			else if (textData.partyMemberText->equipmentTextType == MenuText::EQUIPMENT_DEFENSE)
				EventNode.addAttribute("partyMemberEquipmentTextType", "defense");
			else if (textData.partyMemberText->equipmentTextType == MenuText::EQUIPMENT_WISDOM)
				EventNode.addAttribute("partyMemberEquipmentTextType", "wisdom");
			else if (textData.partyMemberText->equipmentTextType == MenuText::EQUIPMENT_AGILITY)
				EventNode.addAttribute("partyMemberEquipmentTextType", "agility");
		}
	}
	else if (textType == MenuText::TYPE_MONSTER)
	{
		EventNode.addAttribute("textType", "monster");
		EventNode.addAttribute("monsterID", QString::number(textData.monsterText->monsterID).toUtf8().data());
		if (textData.monsterText->textType == MenuText::MONSTER_NAME)
			EventNode.addAttribute("monsterTextType", "name");
		else if (textData.monsterText->textType == MenuText::MONSTER_MONEY)
			EventNode.addAttribute("monsterTextType", "money");
		else if (textData.monsterText->textType == MenuText::MONSTER_HP)
			EventNode.addAttribute("monsterTextType", "hp");
		else if (textData.monsterText->textType == MenuText::MONSTER_MP)
			EventNode.addAttribute("monsterTextType", "mp");
		else if (textData.monsterText->textType == MenuText::MONSTER_ATTACK)
			EventNode.addAttribute("monsterTextType", "attack");
		else if (textData.monsterText->textType == MenuText::MONSTER_DEFENSE)
			EventNode.addAttribute("monsterTextType", "defense");
		else if (textData.monsterText->textType == MenuText::MONSTER_WISDOM)
			EventNode.addAttribute("monsterTextType", "wisdom");
		else if (textData.monsterText->textType == MenuText::MONSTER_AGILITY)
			EventNode.addAttribute("monsterTextType", "agility");
		else if (textData.monsterText->textType == MenuText::MONSTER_STRONGAGAINST)
			EventNode.addAttribute("monsterTextType", "strong against");
		else if (textData.monsterText->textType == MenuText::MONSTER_WEAKAGAINST)
			EventNode.addAttribute("monsterTextType", "weak against");
		else if (textData.monsterText->textType == MenuText::MONSTER_EXPERIENCE)
			EventNode.addAttribute("monsterTextType", "experience");
		EventNode.addAttribute("monsterIDIsReferencedInVariable", (textData.monsterText->idIsReferencedInVariable) ? "true":"false");
	}
	else if (textType == MenuText::TYPE_ITEM)
	{
		EventNode.addAttribute("textType", "item");
		EventNode.addAttribute("itemID", QString::number(textData.itemText->itemID).toUtf8().data());
		if (textData.itemText->textType == MenuText::ITEM_NAME)
			EventNode.addAttribute("itemTextType", "name");
		else if (textData.itemText->textType == MenuText::ITEM_DESCRIPTION)
			EventNode.addAttribute("itemTextType", "description");
		else if (textData.itemText->textType == MenuText::ITEM_PRICE)
			EventNode.addAttribute("itemTextType", "price");
		EventNode.addAttribute("itemIDIsReferencedInVariable", (textData.itemText->idIsReferencedInVariable) ? "true":"false");
	}
	else if (textType == MenuText::TYPE_CHARACTER)
	{
		EventNode.addAttribute("textType", "character");
		EventNode.addAttribute("characterID", QString::number(textData.characterText->characterID).toUtf8().data());
		if (textData.characterText->textType == MenuText::CHARACTER_NAME)
			EventNode.addAttribute("characterTextType", "name");
		else if (textData.characterText->textType == MenuText::CHARACTER_CLASS)
			EventNode.addAttribute("characterTextType", "class");
		else if (textData.characterText->textType == MenuText::CHARACTER_CONDITION)
			EventNode.addAttribute("characterTextType", "condition");
		else if (textData.characterText->textType == MenuText::CHARACTER_LEVEL)
			EventNode.addAttribute("characterTextType", "level");
		else if (textData.characterText->textType == MenuText::CHARACTER_HP)
			EventNode.addAttribute("characterTextType", "hp");
		else if (textData.characterText->textType == MenuText::CHARACTER_MAXHP)
			EventNode.addAttribute("characterTextType", "max hp");
		else if (textData.characterText->textType == MenuText::CHARACTER_MP)
			EventNode.addAttribute("characterTextType", "mp");
		else if (textData.characterText->textType == MenuText::CHARACTER_MAXMP)
			EventNode.addAttribute("characterTextType", "max mp");
		else if (textData.characterText->textType == MenuText::CHARACTER_EXPERIENCE)
			EventNode.addAttribute("characterTextType", "experience");
		else if (textData.characterText->textType == MenuText::CHARACTER_EXPERIENCETONEXTLEVEL)
			EventNode.addAttribute("characterTextType", "experience to next level");
		else if (textData.characterText->textType == MenuText::CHARACTER_BASEATTACK)
			EventNode.addAttribute("characterTextType", "base attack");
		else if (textData.characterText->textType == MenuText::CHARACTER_TOTALATTACK)
			EventNode.addAttribute("characterTextType", "total attack");
		else if (textData.characterText->textType == MenuText::CHARACTER_BASEDEFENSE)
			EventNode.addAttribute("characterTextType", "base defense");
		else if (textData.characterText->textType == MenuText::CHARACTER_TOTALDEFENSE)
			EventNode.addAttribute("characterTextType", "total defense");
		else if (textData.characterText->textType == MenuText::CHARACTER_BASEWISDOM)
			EventNode.addAttribute("characterTextType", "base wisdom");
		else if (textData.characterText->textType == MenuText::CHARACTER_TOTALWISDOM)
			EventNode.addAttribute("characterTextType", "total wisdom");
		else if (textData.characterText->textType == MenuText::CHARACTER_BASEAGILITY)
			EventNode.addAttribute("characterTextType", "base agility");
		else if (textData.characterText->textType == MenuText::CHARACTER_TOTALAGILITY)
			EventNode.addAttribute("characterTextType", "total agility");
		else if (textData.characterText->textType == MenuText::CHARACTER_EQUIPPEDWEAPON)
			EventNode.addAttribute("characterTextType", "equipped weapon");
		else if (textData.characterText->textType == MenuText::CHARACTER_EQUIPPEDSHIELD)
			EventNode.addAttribute("characterTextType", "equipped shield");
		else if (textData.characterText->textType == MenuText::CHARACTER_EQUIPPEDARMOR)
			EventNode.addAttribute("characterTextType", "equipped armor");
		else if (textData.characterText->textType == MenuText::CHARACTER_EQUIPPEDHELMET)
			EventNode.addAttribute("characterTextType", "equipped helmet");
		else if (textData.characterText->textType == MenuText::CHARACTER_EQUIPPEDACCESSORY)
			EventNode.addAttribute("characterTextType", "equipped accessory");
		if (textData.characterText->textType >= MenuText::CHARACTER_EQUIPPEDWEAPON && textData.characterText->textType <= MenuText::CHARACTER_EQUIPPEDACCESSORY)
		{
			if (textData.characterText->equipmentTextType == MenuText::EQUIPMENT_NAME)
				EventNode.addAttribute("characterEquipmentTextType", "name");
			else if (textData.characterText->equipmentTextType == MenuText::EQUIPMENT_DESCRIPTION)
				EventNode.addAttribute("characterEquipmentTextType", "description");
			else if (textData.characterText->equipmentTextType == MenuText::EQUIPMENT_ATTACK)
				EventNode.addAttribute("characterEquipmentTextType", "attack");
			else if (textData.characterText->equipmentTextType == MenuText::EQUIPMENT_DEFENSE)
				EventNode.addAttribute("characterEquipmentTextType", "defense");
			else if (textData.characterText->equipmentTextType == MenuText::EQUIPMENT_WISDOM)
				EventNode.addAttribute("characterEquipmentTextType", "wisdom");
			else if (textData.characterText->equipmentTextType == MenuText::EQUIPMENT_AGILITY)
				EventNode.addAttribute("characterEquipmentTextType", "agility");
		}
		EventNode.addAttribute("characterIDIsReferencedInVariable", (textData.characterText->idIsReferencedInVariable) ? "true":"false");
	}
	else if (textType == MenuText::TYPE_VARIABLE)
	{
		EventNode.addAttribute("textType", "variable");
		EventNode.addAttribute("variableID", QString::number(textData.variableText->variableID).toUtf8().data());
		EventNode.addAttribute("variableAlwaysShowsPlusSign", (textData.variableText->alwaysShowPlusSign) ? "true":"false");
	}
	else if (textType == MenuText::TYPE_SWITCH)
	{
		EventNode.addAttribute("textType", "switch");
		EventNode.addAttribute("switchID", QString::number(textData.switchText->switchID).toUtf8().data());
		EventNode.addAttribute("switchOnText", textData.switchText->onText.toUtf8().data());
		EventNode.addAttribute("switchOffText", textData.switchText->offText.toUtf8().data());
	}
	else if (textType == MenuText::TYPE_STRING)
	{
		EventNode.addAttribute("textType", "string");
		EventNode.addAttribute("stringID", QString::number(textData.stringText->stringID).toUtf8().data());
		EventNode.addAttribute("systemString", (textData.stringText->systemString) ? "true":"false");
	}
	return EventNode;
}

void BaseEvent::ChangeText::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putString(objectName.toUtf8().data());
	storageFile.putSignedInt(saveSlotID);
	storageFile.putSignedInt(textType);
	if (textType == MenuText::TYPE_PLAINTEXT)
		storageFile.putString(textData.plainText->toUtf8().data());
	else if (textType == MenuText::TYPE_PARTYMEMBER)
	{
		storageFile.putSignedInt(textData.partyMemberText->memberID);
		storageFile.putSignedInt(textData.partyMemberText->textType);
		storageFile.putSignedInt(textData.partyMemberText->equipmentTextType);
	}
	else if (textType == MenuText::TYPE_MONSTER)
	{
		storageFile.putSignedInt(textData.monsterText->monsterID);
		storageFile.putSignedInt(textData.monsterText->textType);
		storageFile.putBool(textData.monsterText->idIsReferencedInVariable);
	}
	else if (textType == MenuText::TYPE_ITEM)
	{
		storageFile.putSignedInt(textData.itemText->itemID);
		storageFile.putSignedInt(textData.itemText->textType);
		storageFile.putBool(textData.itemText->idIsReferencedInVariable);
	}
	else if (textType == MenuText::TYPE_CHARACTER)
	{
		storageFile.putSignedInt(textData.characterText->characterID);
		storageFile.putSignedInt(textData.characterText->textType);
		storageFile.putSignedInt(textData.characterText->equipmentTextType);
		storageFile.putBool(textData.characterText->idIsReferencedInVariable);
	}
	else if (textType == MenuText::TYPE_VARIABLE)
	{
		storageFile.putSignedInt(textData.variableText->variableID);
		storageFile.putBool(textData.variableText->alwaysShowPlusSign);
	}
	else if (textType == MenuText::TYPE_SWITCH)
	{
		storageFile.putSignedInt(textData.switchText->switchID);
		storageFile.putString(textData.switchText->onText.toUtf8().data());
		storageFile.putString(textData.switchText->offText.toUtf8().data());
	}
	else if (textType == MenuText::TYPE_STRING)
	{
		storageFile.putSignedInt(textData.stringText->stringID);
		storageFile.putBool(textData.stringText->systemString);
	}
}

QStringList BaseEvent::ChangeText::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Change Text: Set ";
	viewableText[0] += objectName;
	viewableText[0] += " to ";
	if (textType == MenuText::TYPE_PLAINTEXT)
	{
		viewableText[0] += "\"";
		viewableText[0] += *(textData.plainText);
		viewableText[0] += "\"";
	}
	else if (textType == MenuText::TYPE_PARTYMEMBER)
	{
		viewableText[0] += "Party Member ";
		if (textData.partyMemberText->memberID < 4)
			viewableText[0] += QString::number(textData.partyMemberText->memberID + 1);
		else
			viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(textData.partyMemberText->memberID - 4));
		viewableText[0] += "\'s ";
		if (textData.partyMemberText->textType == MenuText::CHARACTER_NAME)
			viewableText[0] += "Name";
		else if (textData.partyMemberText->textType == MenuText::CHARACTER_CLASS)
			viewableText[0] += "Class";
		else if (textData.partyMemberText->textType == MenuText::CHARACTER_CONDITION)
			viewableText[0] += "Condition";
		else if (textData.partyMemberText->textType == MenuText::CHARACTER_LEVEL)
			viewableText[0] += "Level";
		else if (textData.partyMemberText->textType == MenuText::CHARACTER_HP)
			viewableText[0] += "HP";
		else if (textData.partyMemberText->textType == MenuText::CHARACTER_MAXHP)
			viewableText[0] += "Max HP";
		else if (textData.partyMemberText->textType == MenuText::CHARACTER_MP)
			viewableText[0] += "MP";
		else if (textData.partyMemberText->textType == MenuText::CHARACTER_MAXMP)
			viewableText[0] += "Max MP";
		else if (textData.partyMemberText->textType == MenuText::CHARACTER_EXPERIENCE)
			viewableText[0] += "Experience";
		else if (textData.partyMemberText->textType == MenuText::CHARACTER_EXPERIENCETONEXTLEVEL)
			viewableText[0] += "Experience To Next Level";
		else if (textData.partyMemberText->textType == MenuText::CHARACTER_BASEATTACK)
			viewableText[0] += "Base Attack";
		else if (textData.partyMemberText->textType == MenuText::CHARACTER_TOTALATTACK)
			viewableText[0] += "Total Attack";
		else if (textData.partyMemberText->textType == MenuText::CHARACTER_BASEDEFENSE)
			viewableText[0] += "Base Defense";
		else if (textData.partyMemberText->textType == MenuText::CHARACTER_TOTALDEFENSE)
			viewableText[0] += "Total Defense";
		else if (textData.partyMemberText->textType == MenuText::CHARACTER_BASEWISDOM)
			viewableText[0] += "Base Wisdom";
		else if (textData.partyMemberText->textType == MenuText::CHARACTER_TOTALWISDOM)
			viewableText[0] += "Total Wisdom";
		else if (textData.partyMemberText->textType == MenuText::CHARACTER_BASEAGILITY)
			viewableText[0] += "Base Agility";
		else if (textData.partyMemberText->textType == MenuText::CHARACTER_TOTALAGILITY)
			viewableText[0] += "Total Agility";
		else if (textData.partyMemberText->textType == MenuText::CHARACTER_EQUIPPEDWEAPON)
			viewableText[0] += "Equipped Weapon\'s ";
		else if (textData.partyMemberText->textType == MenuText::CHARACTER_EQUIPPEDSHIELD)
			viewableText[0] += "Equipped Shield\'s ";
		else if (textData.partyMemberText->textType == MenuText::CHARACTER_EQUIPPEDARMOR)
			viewableText[0] += "Equipped Armor\'s ";
		else if (textData.partyMemberText->textType == MenuText::CHARACTER_EQUIPPEDHELMET)
			viewableText[0] += "Equipped Helmet\'s ";
		else if (textData.partyMemberText->textType == MenuText::CHARACTER_EQUIPPEDACCESSORY)
			viewableText[0] += "Equipped Accessory\'s ";
		if (textData.partyMemberText->textType >= MenuText::CHARACTER_EQUIPPEDWEAPON && textData.partyMemberText->textType <= MenuText::CHARACTER_EQUIPPEDACCESSORY)
		{
			if (textData.partyMemberText->equipmentTextType == MenuText::EQUIPMENT_NAME)
				viewableText[0] += "Name";
			else if (textData.partyMemberText->equipmentTextType == MenuText::EQUIPMENT_DESCRIPTION)
				viewableText[0] += "Description";
			else if (textData.partyMemberText->equipmentTextType == MenuText::EQUIPMENT_ATTACK)
				viewableText[0] += "Attack";
			else if (textData.partyMemberText->equipmentTextType == MenuText::EQUIPMENT_DEFENSE)
				viewableText[0] += "Defense";
			else if (textData.partyMemberText->equipmentTextType == MenuText::EQUIPMENT_WISDOM)
				viewableText[0] += "Wisdom";
			else if (textData.partyMemberText->equipmentTextType == MenuText::EQUIPMENT_AGILITY)
				viewableText[0] += "Agility";
		}
	}
	else if (textType == MenuText::TYPE_MONSTER)
	{
		viewableText[0] += "Monster ";
		if (textData.monsterText->idIsReferencedInVariable)
			viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(textData.monsterText->monsterID));
		else
			viewableText[0] += ProjectData::monsterNames[textData.monsterText->monsterID];
		viewableText[0] += "\'s ";
		if (textData.monsterText->textType == MenuText::MONSTER_NAME)
			viewableText[0] += "Name";
		else if (textData.monsterText->textType == MenuText::MONSTER_MONEY)
			viewableText[0] += "Money";
		else if (textData.monsterText->textType == MenuText::MONSTER_HP)
			viewableText[0] += "HP";
		else if (textData.monsterText->textType == MenuText::MONSTER_MP)
			viewableText[0] += "MP";
		else if (textData.monsterText->textType == MenuText::MONSTER_ATTACK)
			viewableText[0] += "Attack";
		else if (textData.monsterText->textType == MenuText::MONSTER_DEFENSE)
			viewableText[0] += "Defense";
		else if (textData.monsterText->textType == MenuText::MONSTER_WISDOM)
			viewableText[0] += "Wisdom";
		else if (textData.monsterText->textType == MenuText::MONSTER_AGILITY)
			viewableText[0] += "Agility";
		else if (textData.monsterText->textType == MenuText::MONSTER_STRONGAGAINST)
			viewableText[0] += "Strengths";
		else if (textData.monsterText->textType == MenuText::MONSTER_WEAKAGAINST)
			viewableText[0] += "Weaknesses";
		else if (textData.monsterText->textType == MenuText::MONSTER_EXPERIENCE)
			viewableText[0] += "Experience";
	}
	else if (textType == MenuText::TYPE_ITEM)
	{
		viewableText[0] += "Item ";
		if (textData.itemText->idIsReferencedInVariable)
			viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(textData.itemText->itemID));
		else
			viewableText[0] += ProjectData::itemNames[textData.itemText->itemID];
		viewableText[0] += "\'s ";
		if (textData.itemText->textType == MenuText::ITEM_NAME)
			viewableText[0] += "Name";
		else if (textData.itemText->textType == MenuText::ITEM_DESCRIPTION)
			viewableText[0] += "Description";
		else if (textData.itemText->textType == MenuText::ITEM_PRICE)
			viewableText[0] += "Price";
	}
	else if (textType == MenuText::TYPE_CHARACTER)
	{
		viewableText[0] += "Character ";
		if (textData.characterText->idIsReferencedInVariable)
			viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(textData.characterText->characterID));
		else
			viewableText[0] += ProjectData::characterNames[textData.characterText->characterID];
		viewableText[0] += "\'s ";
		if (textData.characterText->textType == MenuText::CHARACTER_NAME)
			viewableText[0] += "Name";
		else if (textData.characterText->textType == MenuText::CHARACTER_CLASS)
			viewableText[0] += "Class";
		else if (textData.characterText->textType == MenuText::CHARACTER_CONDITION)
			viewableText[0] += "Condition";
		else if (textData.characterText->textType == MenuText::CHARACTER_LEVEL)
			viewableText[0] += "Level";
		else if (textData.characterText->textType == MenuText::CHARACTER_HP)
			viewableText[0] += "HP";
		else if (textData.characterText->textType == MenuText::CHARACTER_MAXHP)
			viewableText[0] += "Max HP";
		else if (textData.characterText->textType == MenuText::CHARACTER_MP)
			viewableText[0] += "MP";
		else if (textData.characterText->textType == MenuText::CHARACTER_MAXMP)
			viewableText[0] += "Max MP";
		else if (textData.characterText->textType == MenuText::CHARACTER_EXPERIENCE)
			viewableText[0] += "Experience";
		else if (textData.characterText->textType == MenuText::CHARACTER_EXPERIENCETONEXTLEVEL)
			viewableText[0] += "Experience To Next Level";
		else if (textData.characterText->textType == MenuText::CHARACTER_BASEATTACK)
			viewableText[0] += "Base Attack";
		else if (textData.characterText->textType == MenuText::CHARACTER_TOTALATTACK)
			viewableText[0] += "Total Attack";
		else if (textData.characterText->textType == MenuText::CHARACTER_BASEDEFENSE)
			viewableText[0] += "Base Defense";
		else if (textData.characterText->textType == MenuText::CHARACTER_TOTALDEFENSE)
			viewableText[0] += "Total Defense";
		else if (textData.characterText->textType == MenuText::CHARACTER_BASEWISDOM)
			viewableText[0] += "Base Wisdom";
		else if (textData.characterText->textType == MenuText::CHARACTER_TOTALWISDOM)
			viewableText[0] += "Total Wisdom";
		else if (textData.characterText->textType == MenuText::CHARACTER_BASEAGILITY)
			viewableText[0] += "Base Agility";
		else if (textData.characterText->textType == MenuText::CHARACTER_TOTALAGILITY)
			viewableText[0] += "Total Agility";
		else if (textData.characterText->textType == MenuText::CHARACTER_EQUIPPEDWEAPON)
			viewableText[0] += "Equipped Weapon\'s ";
		else if (textData.characterText->textType == MenuText::CHARACTER_EQUIPPEDSHIELD)
			viewableText[0] += "Equipped Shield\'s ";
		else if (textData.characterText->textType == MenuText::CHARACTER_EQUIPPEDARMOR)
			viewableText[0] += "Equipped Armor\'s ";
		else if (textData.characterText->textType == MenuText::CHARACTER_EQUIPPEDHELMET)
			viewableText[0] += "Equipped Helmet\'s ";
		else if (textData.characterText->textType == MenuText::CHARACTER_EQUIPPEDACCESSORY)
			viewableText[0] += "Equipped Accessory\'s ";
		if (textData.characterText->textType >= MenuText::CHARACTER_EQUIPPEDWEAPON && textData.characterText->textType <= MenuText::CHARACTER_EQUIPPEDACCESSORY)
		{
			if (textData.characterText->equipmentTextType == MenuText::EQUIPMENT_NAME)
				viewableText[0] += "Name";
			else if (textData.characterText->equipmentTextType == MenuText::EQUIPMENT_DESCRIPTION)
				viewableText[0] += "Description";
			else if (textData.characterText->equipmentTextType == MenuText::EQUIPMENT_ATTACK)
				viewableText[0] += "Attack";
			else if (textData.characterText->equipmentTextType == MenuText::EQUIPMENT_DEFENSE)
				viewableText[0] += "Defense";
			else if (textData.characterText->equipmentTextType == MenuText::EQUIPMENT_WISDOM)
				viewableText[0] += "Wisdom";
			else if (textData.characterText->equipmentTextType == MenuText::EQUIPMENT_AGILITY)
				viewableText[0] += "Agility";
		}
	}
	else if (textType == MenuText::TYPE_VARIABLE)
		viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(textData.variableText->variableID));
	else if (textType == MenuText::TYPE_SWITCH)
	{
		viewableText[0] += QString("string[%1]").arg(ProjectData::switchVariableIdToString(textData.switchText->switchID));
		viewableText[0] += " On=\"";
		viewableText[0] += textData.switchText->onText;
		viewableText[0] += "\" Off=\"";
		viewableText[0] += textData.switchText->offText;
		viewableText[0] += "\"";
	}
	else if (textType == MenuText::TYPE_STRING)
	{
		if (textData.stringText->systemString)
		{
			viewableText[0] += "System String (";
			if (textData.stringText->stringID == ProjectData::STRING_BATTLEMSG_BEGIN)
				viewableText[0] += "Battle Message: Begin";
			else if (textData.stringText->stringID == ProjectData::STRING_BATTLEMSG_INITIATIVE)
				viewableText[0] += "Battle Message: Initiative";
			else if (textData.stringText->stringID == ProjectData::STRING_BATTLEMSG_MISS)
				viewableText[0] += "Battle Message: Miss";
			else if (textData.stringText->stringID == ProjectData::STRING_BATTLEMSG_FAILESCAPE)
				viewableText[0] += "Battle Message: Failed Escape";
			else if (textData.stringText->stringID == ProjectData::STRING_BATTLEMSG_VICTORY)
				viewableText[0] += "Battle Message: Victory";
			else if (textData.stringText->stringID == ProjectData::STRING_BATTLEMSG_DEFEAT)
				viewableText[0] += "Battle Message: Defeat";
			else if (textData.stringText->stringID == ProjectData::STRING_BATTLEMSG_EXPERIENCEGAIN)
				viewableText[0] += "Battle Message: Experience Gained";
			else if (textData.stringText->stringID == ProjectData::STRING_BATTLEMSG_MONEYGAIN)
				viewableText[0] += "Battle Message: Money Gained";
			else if (textData.stringText->stringID == ProjectData::STRING_BATTLEMSG_ITEMGAIN)
				viewableText[0] += "Battle Message: Item Gained";
			else if (textData.stringText->stringID == ProjectData::STRING_LEVELUP)
				viewableText[0] += "Level Up";
			else if (textData.stringText->stringID == ProjectData::STRING_SKILLLEARNED)
				viewableText[0] += "Skill Learned";
			else if (textData.stringText->stringID == ProjectData::STRING_SHOPMSG_GREETING)
				viewableText[0] += "Shop Message: Greeting";
			else if (textData.stringText->stringID == ProjectData::STRING_SHOPMSG_AFTERFIRSTTRANSACTION)
				viewableText[0] += "Shop Message: After First Transaction";
			else if (textData.stringText->stringID == ProjectData::STRING_SHOPMSG_BUY)
				viewableText[0] += "Shop Message: Buy";
			else if (textData.stringText->stringID == ProjectData::STRING_SHOPMSG_SELL)
				viewableText[0] += "Shop Message: Sell";
			else if (textData.stringText->stringID == ProjectData::STRING_SHOPMSG_DONE)
				viewableText[0] += "Shop Message: Done";
			else if (textData.stringText->stringID == ProjectData::STRING_SHOPMSG_BUYITEM)
				viewableText[0] += "Shop Message: Buy Item";
			else if (textData.stringText->stringID == ProjectData::STRING_SHOPMSG_BUYHOWMANY)
				viewableText[0] += "Shop Message: Buy How Many";
			else if (textData.stringText->stringID == ProjectData::STRING_SHOPMSG_BUYCOMPLETE)
				viewableText[0] += "Shop Message: Buy Complete";
			else if (textData.stringText->stringID == ProjectData::STRING_SHOPMSG_SELLITEM)
				viewableText[0] += "Shop Message: Sell Item";
			else if (textData.stringText->stringID == ProjectData::STRING_SHOPMSG_SELLHOWMANY)
				viewableText[0] += "Shop Message: Sell How Many";
			else if (textData.stringText->stringID == ProjectData::STRING_SHOPMSG_SELLCOMPLETE)
				viewableText[0] += "Shop Message: Sell Complete";
			else if (textData.stringText->stringID == ProjectData::STRING_INNMSG_PRICEINTRO)
				viewableText[0] += "Inn Message: Price Intro";
			else if (textData.stringText->stringID == ProjectData::STRING_INNMSG_ACCEPTINQUIRY)
				viewableText[0] += "Inn Message: Accept Inquiry";
			else if (textData.stringText->stringID == ProjectData::STRING_INNMSG_ACCEPT)
				viewableText[0] += "Inn Message: Accept";
			else if (textData.stringText->stringID == ProjectData::STRING_INNMSG_DECLINE)
				viewableText[0] += "Inn Message: Decline";
			else if (textData.stringText->stringID == ProjectData::STRING_OTHER_ITEMSPOSSESSED)
				viewableText[0] += "Other: Items Possessed";
			else if (textData.stringText->stringID == ProjectData::STRING_OTHER_ITEMSEQUIPPED)
				viewableText[0] += "Other: Items Equipped";
			else if (textData.stringText->stringID == ProjectData::STRING_OTHER_CURRENCY)
				viewableText[0] += "Other: Currency";
			else if (textData.stringText->stringID == ProjectData::STRING_STATSANDEQUIP_LEVEL)
				viewableText[0] += "Stat: Level";
			else if (textData.stringText->stringID == ProjectData::STRING_STATSANDEQUIP_LEVELSHORT)
				viewableText[0] += "Stat: Level short form";
			else if (textData.stringText->stringID == ProjectData::STRING_STATSANDEQUIP_HP)
				viewableText[0] += "Stat: HP";
			else if (textData.stringText->stringID == ProjectData::STRING_STATSANDEQUIP_HPSHORT)
				viewableText[0] += "Stat: HP short form";
			else if (textData.stringText->stringID == ProjectData::STRING_STATSANDEQUIP_MP)
				viewableText[0] += "Stat: MP";
			else if (textData.stringText->stringID == ProjectData::STRING_STATSANDEQUIP_MPSHORT)
				viewableText[0] += "Stat: MP short form";
			else if (textData.stringText->stringID == ProjectData::STRING_STATSANDEQUIP_NORMALSTATUS)
				viewableText[0] += "Stat: Normal Status";
			else if (textData.stringText->stringID == ProjectData::STRING_STATSANDEQUIP_EXPERIENCE)
				viewableText[0] += "Stat: Experience";
			else if (textData.stringText->stringID == ProjectData::STRING_STATSANDEQUIP_ATTACK)
				viewableText[0] += "Stat: Attack";
			else if (textData.stringText->stringID == ProjectData::STRING_STATSANDEQUIP_DEFENSE)
				viewableText[0] += "Stat: Defense";
			else if (textData.stringText->stringID == ProjectData::STRING_STATSANDEQUIP_WISDOM)
				viewableText[0] += "Stat: Wisdom";
			else if (textData.stringText->stringID == ProjectData::STRING_STATSANDEQUIP_AGILITY)
				viewableText[0] += "Stat: Agility";
			else if (textData.stringText->stringID == ProjectData::STRING_STATSANDEQUIP_MPUSED)
				viewableText[0] += "Equipment: MP Used";
			else if (textData.stringText->stringID == ProjectData::STRING_STATSANDEQUIP_WEAPON)
				viewableText[0] += "Equipment: Weapon";
			else if (textData.stringText->stringID == ProjectData::STRING_STATSANDEQUIP_SHIELD)
				viewableText[0] += "Equipment: Shield";
			else if (textData.stringText->stringID == ProjectData::STRING_STATSANDEQUIP_ARMOR)
				viewableText[0] += "Equipment: Armor";
			else if (textData.stringText->stringID == ProjectData::STRING_STATSANDEQUIP_HELMET)
				viewableText[0] += "Equipment: Helmet";
			else if (textData.stringText->stringID == ProjectData::STRING_STATSANDEQUIP_ACCESSORY)
				viewableText[0] += "Equipment: Accessory";
			else if (textData.stringText->stringID == ProjectData::STRING_COMMAND_FIGHT)
				viewableText[0] += "Command: Fight";
			else if (textData.stringText->stringID == ProjectData::STRING_COMMAND_AUTOFIGHT)
				viewableText[0] += "Command: Auto-Fight";
			else if (textData.stringText->stringID == ProjectData::STRING_COMMAND_ESCAPE)
				viewableText[0] += "Command: Escape";
			else if (textData.stringText->stringID == ProjectData::STRING_COMMAND_ITEM)
				viewableText[0] += "Command: Item";
			else if (textData.stringText->stringID == ProjectData::STRING_COMMAND_SKILL)
				viewableText[0] += "Command: Skill";
			else if (textData.stringText->stringID == ProjectData::STRING_COMMAND_EQUIP)
				viewableText[0] += "Command: Equip";
			else if (textData.stringText->stringID == ProjectData::STRING_COMMAND_SAVE)
				viewableText[0] += "Command: Save";
			else if (textData.stringText->stringID == ProjectData::STRING_COMMAND_QUIT)
				viewableText[0] += "Command: Quit";
			else if (textData.stringText->stringID == ProjectData::STRING_COMMAND_STATUS)
				viewableText[0] += "Command: Status";
			else if (textData.stringText->stringID == ProjectData::STRING_COMMAND_ROW)
				viewableText[0] += "Command: Row";
			else if (textData.stringText->stringID == ProjectData::STRING_COMMAND_ORDER)
				viewableText[0] += "Command: Order";
			else if (textData.stringText->stringID == ProjectData::STRING_COMMAND_NEWGAME)
				viewableText[0] += "Command: New Game";
			else if (textData.stringText->stringID == ProjectData::STRING_COMMAND_LOADGAME)
				viewableText[0] += "Command: Load Game";
			else if (textData.stringText->stringID == ProjectData::STRING_COMMAND_ENDGAME)
				viewableText[0] += "Command: End Game";
			else if (textData.stringText->stringID == ProjectData::STRING_COMMAND_WAITON)
				viewableText[0] += "Command: Wait ON";
			else if (textData.stringText->stringID == ProjectData::STRING_COMMAND_WAITOFF)
				viewableText[0] += "Command: Wait OFF";
			else if (textData.stringText->stringID == ProjectData::STRING_SAVEQUITSCREENMSG_SAVESLOT)
				viewableText[0] += "Save/Quit: Save Slot";
			else if (textData.stringText->stringID == ProjectData::STRING_SAVEQUITSCREENMSG_LOADSLOT)
				viewableText[0] += "Save/Quit: Load Slot";
			else if (textData.stringText->stringID == ProjectData::STRING_SAVEQUITSCREENMSG_CONFIRMQUIT)
				viewableText[0] += "Save/Quit: Confirm Quit";
			else if (textData.stringText->stringID == ProjectData::STRING_SAVEQUITSCREENMSG_YES)
				viewableText[0] += "Save/Quit: Yes";
			else if (textData.stringText->stringID == ProjectData::STRING_SAVEQUITSCREENMSG_NO)
				viewableText[0] += "Save/Quit: No";
			else if (textData.stringText->stringID == ProjectData::STRING_MISC_PARTYSMONEY)
				viewableText[0] += "Misc: Party\'s Money";
			else if (textData.stringText->stringID == ProjectData::STRING_MISC_TOTALTIMEPLAYED)
				viewableText[0] += "Misc: Total Time Played";
			viewableText[0] += ")";
		}
		else
			viewableText[0] += QString("string[%1]").arg(ProjectData::stringVariableIdToString(textData.stringText->stringID));
	}
	if (saveSlotID > 0)
		viewableText[0] += QString(" (SaveSlot=%1)").arg(saveSlotID);
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::ChangeText::configureEvent(int callerID, int callerType)
{
	ConfigureEvent_ChangeMenuText *configMenuText = new ConfigureEvent_ChangeMenuText(saveSlotID, textType, &textData, callerID, callerType);
	bool configured = false;
	configMenuText->setTextObject(objectName);
	if (configMenuText->exec())
	{
		configured = true;
		textType = configMenuText->getTextType();
		objectName = configMenuText->getTextObject();
	}
	delete configMenuText;
	return configured;
}
#endif

void BaseEvent::ChangeText::getText(QString *value)
{
	value->clear();
	value->append(*(textData.plainText));
}

void BaseEvent::ChangeText::getText(MenuText::PartyMemberText *value)
{
	value->memberID = textData.partyMemberText->memberID;
	value->textType = textData.partyMemberText->textType;
	value->equipmentTextType = textData.partyMemberText->equipmentTextType;
}

void BaseEvent::ChangeText::getText(MenuText::MonsterText *value)
{
	value->monsterID = textData.monsterText->monsterID;
	value->textType = textData.monsterText->textType;
	value->idIsReferencedInVariable = textData.monsterText->idIsReferencedInVariable;
}

void BaseEvent::ChangeText::getText(MenuText::ItemText *value)
{
	value->itemID = textData.itemText->itemID;
	value->textType = textData.itemText->textType;
	value->idIsReferencedInVariable = textData.itemText->idIsReferencedInVariable;
}

void BaseEvent::ChangeText::getText(MenuText::CharacterText *value)
{
	value->characterID = textData.characterText->characterID;
	value->textType = textData.characterText->textType;
	value->equipmentTextType = textData.characterText->equipmentTextType;
	value->idIsReferencedInVariable = textData.characterText->idIsReferencedInVariable;
}

void BaseEvent::ChangeText::getText(MenuText::VariableText *value)
{
	value->variableID = textData.variableText->variableID;
	value->alwaysShowPlusSign = textData.variableText->alwaysShowPlusSign;
}

void BaseEvent::ChangeText::getText(MenuText::SwitchText *value)
{
	value->switchID = textData.switchText->switchID;
	value->onText = textData.switchText->onText;
	value->offText = textData.switchText->offText;
}

void BaseEvent::ChangeText::getText(MenuText::StringText *value)
{
	value->stringID = textData.stringText->stringID;
	value->systemString = textData.stringText->systemString;
}

void BaseEvent::ChangeText::setText(QString value)
{
	textData.plainText->clear();
	textData.plainText->append(value);
}

void BaseEvent::ChangeText::setText(MenuText::PartyMemberText *value)
{
	textData.partyMemberText->memberID = value->memberID;
	textData.partyMemberText->textType = value->textType;
	textData.partyMemberText->equipmentTextType = value->equipmentTextType;
}

void BaseEvent::ChangeText::setText(MenuText::MonsterText *value)
{
	textData.monsterText->monsterID = value->monsterID;
	textData.monsterText->textType = value->textType;
	textData.monsterText->idIsReferencedInVariable = value->idIsReferencedInVariable;
}

void BaseEvent::ChangeText::setText(MenuText::ItemText *value)
{
	textData.itemText->itemID = value->itemID;
	textData.itemText->textType = value->textType;
	textData.itemText->idIsReferencedInVariable = value->idIsReferencedInVariable;
}

void BaseEvent::ChangeText::setText(MenuText::CharacterText *value)
{
	textData.characterText->characterID = value->characterID;
	textData.characterText->textType = value->textType;
	textData.characterText->equipmentTextType = value->equipmentTextType;
	textData.characterText->idIsReferencedInVariable = value->idIsReferencedInVariable;
}

void BaseEvent::ChangeText::setText(MenuText::VariableText *value)
{
	textData.variableText->variableID = value->variableID;
	textData.variableText->alwaysShowPlusSign = value->alwaysShowPlusSign;
}

void BaseEvent::ChangeText::setText(MenuText::SwitchText *value)
{
	textData.switchText->switchID = value->switchID;
	textData.switchText->onText = value->onText;
	textData.switchText->offText = value->offText;
}

void BaseEvent::ChangeText::setText(MenuText::StringText *value)
{
	textData.stringText->stringID = value->stringID;
	textData.stringText->systemString = value->systemString;
}

void BaseEvent::ChangeText::setTextType(int value)
{
	if (textType == MenuText::TYPE_PLAINTEXT)
		delete textData.plainText;
	if (textType == MenuText::TYPE_PARTYMEMBER)
		delete textData.partyMemberText;
	if (textType == MenuText::TYPE_MONSTER)
		delete textData.monsterText;
	if (textType == MenuText::TYPE_ITEM)
		delete textData.itemText;
	if (textType == MenuText::TYPE_CHARACTER)
		delete textData.characterText;
	if (textType == MenuText::TYPE_VARIABLE)
		delete textData.variableText;
	if (textType == MenuText::TYPE_SWITCH)
		delete textData.switchText;
	if (textType == MenuText::TYPE_STRING)
		delete textData.stringText;
	textType = value;
	if (textType == MenuText::TYPE_PLAINTEXT)
		textData.plainText = new QString("");
	if (textType == MenuText::TYPE_PARTYMEMBER)
		textData.partyMemberText = new MenuText::PartyMemberText;
	if (textType == MenuText::TYPE_MONSTER)
		textData.monsterText = new MenuText::MonsterText;
	if (textType == MenuText::TYPE_ITEM)
		textData.itemText = new MenuText::ItemText;
	if (textType == MenuText::TYPE_CHARACTER)
		textData.characterText = new MenuText::CharacterText;
	if (textType == MenuText::TYPE_VARIABLE)
		textData.variableText = new MenuText::VariableText;
	if (textType == MenuText::TYPE_SWITCH)
		textData.switchText = new MenuText::SwitchText;
	if (textType == MenuText::TYPE_STRING)
		textData.stringText = new MenuText::StringText;
}

bool BaseEvent::ChangeText::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	if (!XMLTools::attributeExists(EventNode, "objectName"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "saveSlotID"))
		return false;
	if (!XMLTools::attributeIntRangeValid(EventNode, "saveSlotID", 0, 16))
		return false;
	if (!MenuText::tryLoadTextData(EventNode))
		return false;
	return true;
}

BaseEvent::ChangeText::ChangeText(BaseEvent *pBase) : Event(pBase)
{
	objectName = "";
	textData.plainText = new QString("");
	textType = MenuText::TYPE_PLAINTEXT;
	saveSlotID = 0;
}

BaseEvent::ChangeText::ChangeText(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	QString tempString;
	objectName = EventNode.getAttribute("objectName");
	saveSlotID = QString(EventNode.getAttribute("saveSlotID")).toInt();
	tempString = EventNode.getAttribute("textType");
	if (tempString == "plain text")
	{
		textType = MenuText::TYPE_PLAINTEXT;
		textData.plainText = new QString(EventNode.getAttribute("plainText"));
	}
	else if (tempString == "party member")
	{
		textType = MenuText::TYPE_PARTYMEMBER;
		textData.partyMemberText = new MenuText::PartyMemberText;
		textData.partyMemberText->memberID = QString(EventNode.getAttribute("partyMemberID")).toInt();
		tempString = EventNode.getAttribute("partyMemberTextType");
		if (tempString == "name")
			textData.partyMemberText->textType = MenuText::PARTYMEMBER_NAME;
		else if (tempString == "class")
			textData.partyMemberText->textType = MenuText::PARTYMEMBER_CLASS;
		else if (tempString == "condition")
			textData.partyMemberText->textType = MenuText::PARTYMEMBER_CONDITION;
		else if (tempString == "level")
			textData.partyMemberText->textType = MenuText::PARTYMEMBER_LEVEL;
		else if (tempString == "hp")
			textData.partyMemberText->textType = MenuText::PARTYMEMBER_HP;
		else if (tempString == "max hp")
			textData.partyMemberText->textType = MenuText::PARTYMEMBER_MAXHP;
		else if (tempString == "mp")
			textData.partyMemberText->textType = MenuText::PARTYMEMBER_MP;
		else if (tempString == "max mp")
			textData.partyMemberText->textType = MenuText::PARTYMEMBER_MAXMP;
		else if (tempString == "experience")
			textData.partyMemberText->textType = MenuText::PARTYMEMBER_EXPERIENCE;
		else if (tempString == "experience to next level")
			textData.partyMemberText->textType = MenuText::PARTYMEMBER_EXPERIENCETONEXTLEVEL;
		else if (tempString == "base attack")
			textData.partyMemberText->textType = MenuText::PARTYMEMBER_BASEATTACK;
		else if (tempString == "total attack")
			textData.partyMemberText->textType = MenuText::PARTYMEMBER_TOTALATTACK;
		else if (tempString == "base defense")
			textData.partyMemberText->textType = MenuText::PARTYMEMBER_BASEDEFENSE;
		else if (tempString == "total defense")
			textData.partyMemberText->textType = MenuText::PARTYMEMBER_TOTALDEFENSE;
		else if (tempString == "base wisdom")
			textData.partyMemberText->textType = MenuText::PARTYMEMBER_BASEWISDOM;
		else if (tempString == "total wisdom")
			textData.partyMemberText->textType = MenuText::PARTYMEMBER_TOTALWISDOM;
		else if (tempString == "base agility")
			textData.partyMemberText->textType = MenuText::PARTYMEMBER_BASEAGILITY;
		else if (tempString == "total agility")
			textData.partyMemberText->textType = MenuText::PARTYMEMBER_TOTALAGILITY;
		else if (tempString == "equipped weapon")
			textData.partyMemberText->textType = MenuText::PARTYMEMBER_EQUIPPEDWEAPON;
		else if (tempString == "equipped shield")
			textData.partyMemberText->textType = MenuText::PARTYMEMBER_EQUIPPEDSHIELD;
		else if (tempString == "equipped armor")
			textData.partyMemberText->textType = MenuText::PARTYMEMBER_EQUIPPEDARMOR;
		else if (tempString == "equipped helmet")
			textData.partyMemberText->textType = MenuText::PARTYMEMBER_EQUIPPEDHELMET;
		else if (tempString == "equipped accessory")
			textData.partyMemberText->textType = MenuText::PARTYMEMBER_EQUIPPEDACCESSORY;
		if (textData.partyMemberText->textType >= MenuText::PARTYMEMBER_EQUIPPEDWEAPON && textData.partyMemberText->textType <= MenuText::PARTYMEMBER_EQUIPPEDACCESSORY)
		{
			tempString = EventNode.getAttribute("partyMemberEquipmentTextType");
			if (tempString == "name")
				textData.partyMemberText->equipmentTextType = MenuText::EQUIPMENT_NAME;
			else if (tempString == "description")
				textData.partyMemberText->equipmentTextType = MenuText::EQUIPMENT_DESCRIPTION;
			else if (tempString == "attack")
				textData.partyMemberText->equipmentTextType = MenuText::EQUIPMENT_ATTACK;
			else if (tempString == "defense")
				textData.partyMemberText->equipmentTextType = MenuText::EQUIPMENT_DEFENSE;
			else if (tempString == "wisdom")
				textData.partyMemberText->equipmentTextType = MenuText::EQUIPMENT_WISDOM;
			else if (tempString == "agility")
				textData.partyMemberText->equipmentTextType = MenuText::EQUIPMENT_AGILITY;
		}
	}
	else if (tempString == "monster")
	{
		textType = MenuText::TYPE_MONSTER;
		textData.monsterText = new MenuText::MonsterText;
		textData.monsterText->monsterID = QString(EventNode.getAttribute("monsterID")).toInt();
		tempString = EventNode.getAttribute("monsterTextType");
		if (tempString == "name")
			textData.monsterText->textType = MenuText::MONSTER_NAME;
		else if (tempString == "money")
			textData.monsterText->textType = MenuText::MONSTER_MONEY;
		else if (tempString == "hp")
			textData.monsterText->textType = MenuText::MONSTER_HP;
		else if (tempString == "mp")
			textData.monsterText->textType = MenuText::MONSTER_MP;
		else if (tempString == "attack")
			textData.monsterText->textType = MenuText::MONSTER_ATTACK;
		else if (tempString == "defense")
			textData.monsterText->textType = MenuText::MONSTER_DEFENSE;
		else if (tempString == "wisdom")
			textData.monsterText->textType = MenuText::MONSTER_WISDOM;
		else if (tempString == "agility")
			textData.monsterText->textType = MenuText::MONSTER_AGILITY;
		else if (tempString == "strong against")
			textData.monsterText->textType = MenuText::MONSTER_STRONGAGAINST;
		else if (tempString == "weak against")
			textData.monsterText->textType = MenuText::MONSTER_WEAKAGAINST;
		else if (tempString == "experience")
			textData.monsterText->textType = MenuText::MONSTER_EXPERIENCE;
		textData.monsterText->idIsReferencedInVariable = (QString(EventNode.getAttribute("monsterIDIsReferencedInVariable")) == "true") ? true:false;
	}
	else if (tempString == "item")
	{
		textType = MenuText::TYPE_ITEM;
		textData.itemText = new MenuText::ItemText;
		textData.itemText->itemID = QString(EventNode.getAttribute("itemID")).toInt();
		tempString = EventNode.getAttribute("itemTextType");
		if (tempString == "name")
			textData.itemText->textType = MenuText::ITEM_NAME;
		else if (tempString == "description")
			textData.itemText->textType = MenuText::ITEM_DESCRIPTION;
		else if (tempString == "price")
			textData.itemText->textType = MenuText::ITEM_PRICE;
		textData.itemText->idIsReferencedInVariable = (QString(EventNode.getAttribute("itemIDIsReferencedInVariable")) == "true") ? true:false;
	}
	else if (tempString == "character")
	{
		textType = MenuText::TYPE_CHARACTER;
		textData.characterText = new MenuText::CharacterText;
		textData.characterText->characterID = QString(EventNode.getAttribute("characterID")).toInt();
		tempString = EventNode.getAttribute("characterTextType");
		if (tempString == "name")
			textData.characterText->textType = MenuText::CHARACTER_NAME;
		else if (tempString == "class")
			textData.characterText->textType = MenuText::CHARACTER_CLASS;
		else if (tempString == "condition")
			textData.characterText->textType = MenuText::CHARACTER_CONDITION;
		else if (tempString == "level")
			textData.characterText->textType = MenuText::CHARACTER_LEVEL;
		else if (tempString == "hp")
			textData.characterText->textType = MenuText::CHARACTER_HP;
		else if (tempString == "max hp")
			textData.characterText->textType = MenuText::CHARACTER_MAXHP;
		else if (tempString == "mp")
			textData.characterText->textType = MenuText::CHARACTER_MP;
		else if (tempString == "max mp")
			textData.characterText->textType = MenuText::CHARACTER_MAXMP;
		else if (tempString == "experience")
			textData.characterText->textType = MenuText::CHARACTER_EXPERIENCE;
		else if (tempString == "experience to next level")
			textData.characterText->textType = MenuText::CHARACTER_EXPERIENCETONEXTLEVEL;
		else if (tempString == "base attack")
			textData.characterText->textType = MenuText::CHARACTER_BASEATTACK;
		else if (tempString == "total attack")
			textData.characterText->textType = MenuText::CHARACTER_TOTALATTACK;
		else if (tempString == "base defense")
			textData.characterText->textType = MenuText::CHARACTER_BASEDEFENSE;
		else if (tempString == "total defense")
			textData.characterText->textType = MenuText::CHARACTER_TOTALDEFENSE;
		else if (tempString == "base wisdom")
			textData.characterText->textType = MenuText::CHARACTER_BASEWISDOM;
		else if (tempString == "total wisdom")
			textData.characterText->textType = MenuText::CHARACTER_TOTALWISDOM;
		else if (tempString == "base agility")
			textData.characterText->textType = MenuText::CHARACTER_BASEAGILITY;
		else if (tempString == "total agility")
			textData.characterText->textType = MenuText::CHARACTER_TOTALAGILITY;
		else if (tempString == "equipped weapon")
			textData.characterText->textType = MenuText::CHARACTER_EQUIPPEDWEAPON;
		else if (tempString == "equipped shield")
			textData.characterText->textType = MenuText::CHARACTER_EQUIPPEDSHIELD;
		else if (tempString == "equipped armor")
			textData.characterText->textType = MenuText::CHARACTER_EQUIPPEDARMOR;
		else if (tempString == "equipped helmet")
			textData.characterText->textType = MenuText::CHARACTER_EQUIPPEDHELMET;
		else if (tempString == "equipped accessory")
			textData.characterText->textType = MenuText::CHARACTER_EQUIPPEDACCESSORY;
		if (textData.characterText->textType >= MenuText::CHARACTER_EQUIPPEDWEAPON && textData.characterText->textType <= MenuText::CHARACTER_EQUIPPEDACCESSORY)
		{
			tempString = EventNode.getAttribute("characterEquipmentTextType");
			if (tempString == "name")
				textData.characterText->equipmentTextType = MenuText::EQUIPMENT_NAME;
			else if (tempString == "description")
				textData.characterText->equipmentTextType = MenuText::EQUIPMENT_DESCRIPTION;
			else if (tempString == "attack")
				textData.characterText->equipmentTextType = MenuText::EQUIPMENT_ATTACK;
			else if (tempString == "defense")
				textData.characterText->equipmentTextType = MenuText::EQUIPMENT_DEFENSE;
			else if (tempString == "wisdom")
				textData.characterText->equipmentTextType = MenuText::EQUIPMENT_WISDOM;
			else if (tempString == "agility")
				textData.characterText->equipmentTextType = MenuText::EQUIPMENT_AGILITY;
		}
		textData.characterText->idIsReferencedInVariable = (QString(EventNode.getAttribute("characterIDIsReferencedInVariable")) == "true") ? true:false;
	}
	else if (tempString == "variable")
	{
		textType = MenuText::TYPE_VARIABLE;
		textData.variableText = new MenuText::VariableText;
		textData.variableText->variableID = QString(EventNode.getAttribute("variableID")).toInt();
		textData.variableText->alwaysShowPlusSign = (QString(EventNode.getAttribute("variableAlwaysShowsPlusSign")) == "true") ? true:false;
	}
	else if (tempString == "switch")
	{
		textType = MenuText::TYPE_SWITCH;
		textData.switchText = new MenuText::SwitchText;
		textData.switchText->switchID = QString(EventNode.getAttribute("switchID")).toInt();
		textData.switchText->onText = EventNode.getAttribute("switchOnText");
		textData.switchText->offText = EventNode.getAttribute("switchOffText");
	}
	else if (tempString == "string")
	{
		textType = MenuText::TYPE_STRING;
		textData.stringText = new MenuText::StringText;
		textData.stringText->stringID = QString(EventNode.getAttribute("stringID")).toInt();
		textData.stringText->systemString = (QString(EventNode.getAttribute("systemString")) == "true") ? true:false;
	}
}

BaseEvent::ChangeText::ChangeText(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	char *temp_string;
	temp_string = storageFile.getString();
	objectName = temp_string;
	delete[] temp_string;
	saveSlotID = storageFile.getSignedInt();
	textType = storageFile.getSignedInt();
	if (textType == MenuText::TYPE_PLAINTEXT)
	{
		temp_string = storageFile.getString();
		textData.plainText = new QString(temp_string);
		delete[] temp_string;
	}
	else if (textType == MenuText::TYPE_PARTYMEMBER)
	{
		textData.partyMemberText = new MenuText::PartyMemberText;
		textData.partyMemberText->memberID = storageFile.getSignedInt();
		textData.partyMemberText->textType = storageFile.getSignedInt();
		textData.partyMemberText->equipmentTextType = storageFile.getSignedInt();
	}
	else if (textType == MenuText::TYPE_MONSTER)
	{
		textData.monsterText = new MenuText::MonsterText;
		textData.monsterText->monsterID = storageFile.getSignedInt();
		textData.monsterText->textType = storageFile.getSignedInt();
		textData.monsterText->idIsReferencedInVariable = storageFile.getBool();
	}
	else if (textType == MenuText::TYPE_ITEM)
	{
		textData.itemText = new MenuText::ItemText;
		textData.itemText->itemID = storageFile.getSignedInt();
		textData.itemText->textType = storageFile.getSignedInt();
		textData.itemText->idIsReferencedInVariable = storageFile.getBool();
	}
	else if (textType == MenuText::TYPE_CHARACTER)
	{
		textData.characterText = new MenuText::CharacterText;
		textData.characterText->characterID = storageFile.getSignedInt();
		textData.characterText->textType = storageFile.getSignedInt();
		textData.characterText->equipmentTextType = storageFile.getSignedInt();
		textData.characterText->idIsReferencedInVariable = storageFile.getBool();
	}
	else if (textType == MenuText::TYPE_VARIABLE)
	{
		textData.variableText = new MenuText::VariableText;
		textData.variableText->variableID = storageFile.getSignedInt();
		textData.variableText->alwaysShowPlusSign = storageFile.getBool();
	}
	else if (textType == MenuText::TYPE_SWITCH)
	{
		textData.switchText = new MenuText::SwitchText;
		textData.switchText->switchID = storageFile.getSignedInt();
		temp_string = storageFile.getString();
		textData.switchText->onText = temp_string;
		delete[] temp_string;
		temp_string = storageFile.getString();
		textData.switchText->offText = temp_string;
		delete[] temp_string;
	}
	else if (textType == MenuText::TYPE_STRING)
	{
		textData.stringText = new MenuText::StringText;
		textData.stringText->stringID = storageFile.getSignedInt();
		textData.stringText->systemString = storageFile.getBool();
	}
}

BaseEvent::ChangeText::ChangeText(ChangeText *other, BaseEvent *pBase) : Event(pBase)
{
	objectName = "";
	objectName.append(other->objectName);
	saveSlotID = other->saveSlotID;
	textType = other->textType;
	if (textType == MenuText::TYPE_PLAINTEXT)
	{
		textData.plainText = new QString("");
		textData.plainText->append(*(other->textData.plainText));
	}
	else if (textType == MenuText::TYPE_PARTYMEMBER)
	{
		textData.partyMemberText = new MenuText::PartyMemberText;
		textData.partyMemberText->memberID = other->textData.partyMemberText->memberID;
		textData.partyMemberText->textType = other->textData.partyMemberText->textType;
		textData.partyMemberText->equipmentTextType = other->textData.partyMemberText->equipmentTextType;
	}
	else if (textType == MenuText::TYPE_MONSTER)
	{
		textData.monsterText = new MenuText::MonsterText;
		textData.monsterText->monsterID = other->textData.monsterText->monsterID;
		textData.monsterText->textType = other->textData.monsterText->textType;
		textData.monsterText->idIsReferencedInVariable = other->textData.monsterText->idIsReferencedInVariable;
	}
	else if (textType == MenuText::TYPE_ITEM)
	{
		textData.itemText = new MenuText::ItemText;
		textData.itemText->itemID = other->textData.itemText->itemID;
		textData.itemText->textType = other->textData.itemText->textType;
		textData.itemText->idIsReferencedInVariable = other->textData.itemText->idIsReferencedInVariable;
	}
	else if (textType == MenuText::TYPE_CHARACTER)
	{
		textData.characterText = new MenuText::CharacterText;
		textData.characterText->characterID = other->textData.characterText->characterID;
		textData.characterText->textType = other->textData.characterText->textType;
		textData.characterText->equipmentTextType = other->textData.characterText->equipmentTextType;
		textData.characterText->idIsReferencedInVariable = other->textData.characterText->idIsReferencedInVariable;
	}
	else if (textType == MenuText::TYPE_VARIABLE)
	{
		textData.variableText = new MenuText::VariableText;
		textData.variableText->variableID = other->textData.variableText->variableID;
		textData.variableText->alwaysShowPlusSign = other->textData.variableText->alwaysShowPlusSign;
	}
	else if (textType == MenuText::TYPE_SWITCH)
	{
		textData.switchText = new MenuText::SwitchText;
		textData.switchText->switchID = other->textData.switchText->switchID;
		textData.switchText->onText = "";
		textData.switchText->onText.append(other->textData.switchText->onText);
		textData.switchText->offText = "";
		textData.switchText->offText.append(other->textData.switchText->offText);
	}
	else if (textType == MenuText::TYPE_STRING)
	{
		textData.stringText = new MenuText::StringText;
		textData.stringText->stringID = other->textData.stringText->stringID;
		textData.stringText->systemString = other->textData.stringText->systemString;
	}
}
//END BaseEvent::ChangeText Member Functions

//BEGIN BaseEvent::ChangeVehicleGraphic Member Functions
XMLNode BaseEvent::ChangeVehicleGraphic::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	EventNode.addAttribute("vehicleID", QString::number(vehicleID).toUtf8().data());
	EventNode.addAttribute("vehicleReferencedInVariable", (vehicleReferencedInVariable) ? "true":"false");
	EventNode.addAttribute("spriteID", QString::number(spriteID).toUtf8().data());
	EventNode.addAttribute("spriteReferencedInVariable", (spriteReferencedInVariable) ? "true":"false");
	return EventNode;
}

void BaseEvent::ChangeVehicleGraphic::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(vehicleID);
	storageFile.putSignedInt(spriteID);
	storageFile.putBool(vehicleReferencedInVariable);
	storageFile.putBool(spriteReferencedInVariable);
}

QStringList BaseEvent::ChangeVehicleGraphic::getResources()
{
	return QStringList();
}

QStringList BaseEvent::ChangeVehicleGraphic::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Change Vehicle Graphic: vehicle=";
	if (vehicleReferencedInVariable)
		viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(vehicleID));
	else
		viewableText[0] += QString("\"%1\"").arg(ProjectData::vehicleNames[vehicleID]);
	viewableText[0] += " sprite=";
	if (spriteReferencedInVariable)
		viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(spriteID));
	else
		viewableText[0] += QString("\"%1\"").arg(ProjectData::characterSpriteNames[spriteID]);
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::ChangeVehicleGraphic::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	ConfigureEvent_ChangeVehicleGraphic *configVehicleGraphic = new ConfigureEvent_ChangeVehicleGraphic;
	bool configured = false;
	configVehicleGraphic->setIsReferencedVehicle(vehicleReferencedInVariable);
	configVehicleGraphic->setIsReferencedSprite(spriteReferencedInVariable);
	configVehicleGraphic->setVehicleID(vehicleID);
	configVehicleGraphic->setSpriteID(spriteID);
	if (configVehicleGraphic->exec())
	{
		configured = true;
		vehicleID = configVehicleGraphic->getVehicleID();
		spriteID = configVehicleGraphic->getSpriteID();
		vehicleReferencedInVariable = configVehicleGraphic->isReferencedVehicle();
		spriteReferencedInVariable = configVehicleGraphic->isReferencedSprite();
	}
	delete configVehicleGraphic;
	return configured;
}
#endif

bool BaseEvent::ChangeVehicleGraphic::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	QStringList trueFalseList = QStringList() << "true" << "false";
	if (!XMLTools::attributeExists(EventNode, "vehicleID"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "vehicleReferencedInVariable"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "vehicleReferencedInVariable", trueFalseList, 1))
		return false;
	if (!XMLTools::attributeExists(EventNode, "spriteID"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "spriteReferencedInVariable"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "spriteReferencedInVariable", trueFalseList, 1))
		return false;
	return true;
}

BaseEvent::ChangeVehicleGraphic::ChangeVehicleGraphic(BaseEvent *pBase) : Event(pBase)
{
	vehicleID = 0;
	spriteID = 0;
	vehicleReferencedInVariable = false;
	spriteReferencedInVariable = false;
}

BaseEvent::ChangeVehicleGraphic::ChangeVehicleGraphic(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	vehicleID = QString(EventNode.getAttribute("vehicleID")).toInt();
	vehicleReferencedInVariable = (QString(EventNode.getAttribute("vehicleReferencedInVariable")) == "true") ? true:false;
	spriteID = QString(EventNode.getAttribute("spriteID")).toInt();
	spriteReferencedInVariable = (QString(EventNode.getAttribute("spriteReferencedInVariable")) == "true") ? true:false;
}

BaseEvent::ChangeVehicleGraphic::ChangeVehicleGraphic(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	vehicleID = storageFile.getSignedInt();
	spriteID = storageFile.getSignedInt();
	vehicleReferencedInVariable = storageFile.getBool();
	spriteReferencedInVariable = storageFile.getBool();
}

BaseEvent::ChangeVehicleGraphic::ChangeVehicleGraphic(ChangeVehicleGraphic *other, BaseEvent *pBase) : Event(pBase)
{
	vehicleID = other->vehicleID;
	spriteID = other->spriteID;
	vehicleReferencedInVariable = other->vehicleReferencedInVariable;
	spriteReferencedInVariable = other->spriteReferencedInVariable;
}
//END BaseEvent::ChangeVehicleGraphic Member Functions

//BEGIN BaseEvent::CompleteHealing Member Functions
XMLNode BaseEvent::CompleteHealing::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	if (target == ENTIRE_PARTY)
		EventNode.addAttribute("target", "entireParty");
	else if (target == SPECIFIC_CHARACTER)
	{
		EventNode.addAttribute("target", "specificCharacter");
		EventNode.addAttribute("characterID", QString::number(characterID).toUtf8().data());
	}
	else if (target == REFERENCED_CHARACTER)
	{
		EventNode.addAttribute("target", "referencedCharacter");
		EventNode.addAttribute("variableID", QString::number(characterID).toUtf8().data());
	}
	return EventNode;
}

void BaseEvent::CompleteHealing::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(characterID);
	storageFile.putSignedInt(target);
}

QStringList BaseEvent::CompleteHealing::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Complete Healing: ";
	if (target == ENTIRE_PARTY)
		viewableText[0] += "Entire Party";
	else if (target == SPECIFIC_CHARACTER)
		viewableText[0] += ProjectData::characterNames[characterID];
	else
		viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(characterID));
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::CompleteHealing::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	ConfigureEvent_CompleteHealing *configHealing = new ConfigureEvent_CompleteHealing;
	bool configured = false;
	configHealing->setTarget(target);
	configHealing->setCharacterID(characterID);
	if (configHealing->exec())
	{
		configured = true;
		target = configHealing->getTarget();
		characterID = configHealing->getCharacterID();
	}
	delete configHealing;
	return configured;
}
#endif

bool BaseEvent::CompleteHealing::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	QStringList tempStringList;
	QString tempString;
	tempStringList.clear();
	tempStringList << "entireParty" << "specificCharacter" << "referencedCharacter";
	if (!XMLTools::attributeExists(EventNode, "target"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "target", tempStringList))
		return false;
	tempString = EventNode.getAttribute("target");
	if (tempString == "specificCharacter")
	{
		if (!XMLTools::attributeExists(EventNode, "characterID"))
			return false;
	}
	else if (tempString == "referencedCharacter")
	{
		if (!XMLTools::attributeExists(EventNode, "variableID"))
			return false;
	}
	return true;
}

BaseEvent::CompleteHealing::CompleteHealing(BaseEvent *pBase) : Event(pBase)
{
	characterID = 0;
	target = ENTIRE_PARTY;
}

BaseEvent::CompleteHealing::CompleteHealing(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	QString tempString;
	tempString = EventNode.getAttribute("target");
	if (tempString == "entireParty")
	{
		target = ENTIRE_PARTY;
		characterID = 0;
	}
	else if (tempString == "specificCharacter")
	{
		target = SPECIFIC_CHARACTER;
		characterID = QString(EventNode.getAttribute("characterID")).toInt();
	}
	else if (tempString == "referencedCharacter")
	{
		target = REFERENCED_CHARACTER;
		characterID = QString(EventNode.getAttribute("variableID")).toInt();
	}
}

BaseEvent::CompleteHealing::CompleteHealing(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	characterID = storageFile.getSignedInt();
	target = storageFile.getSignedInt();
}

BaseEvent::CompleteHealing::CompleteHealing(CompleteHealing *other, BaseEvent *pBase) : Event(pBase)
{
	characterID = other->characterID;
	target = other->target;
}
//END BaseEvent::CompleteHealing Member Functions

//BEGIN BaseEvent::ConditionalBranch_Map Member Functions
BaseEvent::ConditionalBranch_Map::~ConditionalBranch_Map()
{
	while (conditions.size() > 0)
	{
		delete (conditions.last());
		conditions.removeLast();
	}
	while (onTrueEvents.size() > 0)
	{
		delete (onTrueEvents.last());
		onTrueEvents.removeLast();
	}
	while (onFalseEvents.size() > 0)
	{
		delete (onFalseEvents.last());
		onFalseEvents.removeLast();
	}
}

XMLNode BaseEvent::ConditionalBranch_Map::getEventNode()
{
	XMLNode TempNode1, EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	TempNode1 = EventNode.addChild("conditions");
	if (conditionsToMatch == ALL)
		TempNode1.addAttribute("conditionsToMatch", "all");
	if (conditionsToMatch == AT_LEAST)
	{
		TempNode1.addAttribute("conditionsToMatch", "atLeast");
		TempNode1.addAttribute("numConditionsToMatch", QString::number(numConditionsToMatch).toUtf8().data());
	}
	else if (conditionsToMatch == NONE)
		TempNode1.addAttribute("conditionsToMatch", "none");
	TempNode1.addAttribute("executeCustomHandlerIfConditionsNotMet", (executeCustomHandlerIfConditionsNotMet) ? "true":"false");
	for (int i = 0; i < conditions.size(); ++i)
		TempNode1.addChild(conditions[i]->getConditionNode());
	TempNode1 = EventNode.addChild("onTrueEvents");
	for (int i = 0; i < onTrueEvents.size(); ++i)
		TempNode1.addChild(onTrueEvents[i]->getEventNode());
	if (executeCustomHandlerIfConditionsNotMet)
	{
		TempNode1 = EventNode.addChild("onFalseEvents");
		for (int i = 0; i < onFalseEvents.size(); ++i)
			TempNode1.addChild(onFalseEvents[i]->getEventNode());
	}
	return EventNode;
}

void BaseEvent::ConditionalBranch_Map::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(conditionsToMatch);
	storageFile.putSignedInt(numConditionsToMatch);
	storageFile.putBool(executeCustomHandlerIfConditionsNotMet);
	storageFile.putSignedInt(conditions.size());
	for (int i = 0; i < conditions.size(); ++i)
		conditions[i]->saveToStorageFile(storageFile);
	storageFile.putSignedInt(onTrueEvents.size());
	for (int i = 0; i < onTrueEvents.size(); ++i)
		onTrueEvents[i]->saveToStorageFile(storageFile);
	if (executeCustomHandlerIfConditionsNotMet)
	{
		storageFile.putSignedInt(onFalseEvents.size());
		for (int i = 0; i < onFalseEvents.size(); ++i)
			onFalseEvents[i]->saveToStorageFile(storageFile);
	}
}

QStringList BaseEvent::ConditionalBranch_Map::getResources()
{
	QStringList resources;
	for (int i = 0; i < onTrueEvents.size(); ++i)
		resources.append(onTrueEvents[i]->getResources());
	if (executeCustomHandlerIfConditionsNotMet)
	{
		for (int i = 0; i < onFalseEvents.size(); ++i)
			resources.append(onFalseEvents[i]->getResources());
	}
	return resources;
}

QStringList BaseEvent::ConditionalBranch_Map::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Conditional Branch: If ";
	if (conditionsToMatch == ALL)
		viewableText[0] += "All Conditions Are Met";
	else if (conditionsToMatch == AT_LEAST)
		viewableText[0] += QString("At Least %1 Conditions Are Met").arg(numConditionsToMatch);
	else
		viewableText[0] += "No Conditions Are Met";
	viewableText.append("");
	for (int i = 0; i < indent; ++i)
		viewableText[1].append("  ");
	viewableText[1] += "::Conditions";
	for (int i = 0; i < conditions.size(); ++i)
		viewableText.append(conditions[i]->getViewableText(indent + 1));
	viewableText.append("");
	for (int i = 0; i < indent; ++i)
		viewableText[viewableText.size() - 1] += "  ";
	viewableText[viewableText.size() - 1] += "::True";
	for (int i = 0; i < onTrueEvents.size(); ++i)
		viewableText += onTrueEvents[i]->getViewableText(indent + 1);
	viewableText.append("");
	for (int i = 0; i < indent + 1; ++i)
		viewableText[viewableText.size() - 1] += "  ";
	viewableText[viewableText.size() - 1] += "<>";
	if (executeCustomHandlerIfConditionsNotMet)
	{
		viewableText.append("");
		for (int i = 0; i < indent; ++i)
			viewableText[viewableText.size() - 1] += "  ";
		viewableText[viewableText.size() - 1] += "::False";
		for (int i = 0; i < onFalseEvents.size(); ++i)
			viewableText += onFalseEvents[i]->getViewableText(indent + 1);
		viewableText.append("");
		for (int i = 0; i < indent + 1; ++i)
			viewableText[viewableText.size() - 1] += "  ";
		viewableText[viewableText.size() - 1] += "<>";
	}
	viewableText.append("");
	for (int i = 0; i < indent; ++i)
		viewableText[viewableText.size() - 1] += "  ";
	viewableText[viewableText.size() - 1] += "::End";
	return viewableText;
}

QList<BaseEvent*> BaseEvent::ConditionalBranch_Map::getEventPointers()
{
	QList<BaseEvent*> pointers;
	for (int i = 0; i < onTrueEvents.size(); ++i)
		pointers += onTrueEvents[i]->getEventPointers();
	pointers.append(NULL);
	if (executeCustomHandlerIfConditionsNotMet)
	{
		for (int i = 0; i < onFalseEvents.size(); ++i)
			pointers += onFalseEvents[i]->getEventPointers();
		pointers.append(NULL);
	}
	return pointers;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::ConditionalBranch_Map::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerType);
	ConfigureEvent_MapConditionalBranch *configureConditionalBranch = new ConfigureEvent_MapConditionalBranch(callerID);
	bool configured = false;
	configureConditionalBranch->setConditionsToMatch(conditionsToMatch);
	configureConditionalBranch->setNumConditionsToMatch(numConditionsToMatch);
	configureConditionalBranch->setExecuteCustomHandler(executeCustomHandlerIfConditionsNotMet);
	configureConditionalBranch->addConditions(conditions);
	if (configureConditionalBranch->exec())
	{
		QList<Condition*> newConditions;
		configured = true;
		for (int i = 0; i < conditions.size(); ++i)
			delete (conditions[i]);
		conditions.clear();
		conditionsToMatch = configureConditionalBranch->getConditionsToMatch();
		numConditionsToMatch = configureConditionalBranch->getNumConditionsToMatch();
		executeCustomHandlerIfConditionsNotMet = configureConditionalBranch->executeCustomHandler();
		newConditions = configureConditionalBranch->getConditions();
		for (int i = 0; i < newConditions.size(); ++i)
			conditions.append(new Condition(newConditions[i]));
	}
	delete configureConditionalBranch;
	return configured;
}
#endif

void BaseEvent::ConditionalBranch_Map::addCondition(Condition *condition)
{
	conditions.append(condition);
}

void BaseEvent::ConditionalBranch_Map::removeCondition(int location)
{
	delete (conditions[location]);
	conditions.removeAt(location);
}

void BaseEvent::ConditionalBranch_Map::addOnTrueEvent(BaseEvent *event, int before)
{
	if (before == -1)
		onTrueEvents.append(event);
	else
		onTrueEvents.insert(before, event);
}

void BaseEvent::ConditionalBranch_Map::addOnFalseEvent(BaseEvent *event, int before)
{
	if (before == -1)
		onFalseEvents.append(event);
	else
		onFalseEvents.insert(before, event);
}

void BaseEvent::ConditionalBranch_Map::deleteOnTrueEvent(int location)
{
	delete (onTrueEvents[location]);
	onTrueEvents.removeAt(location);
}

void BaseEvent::ConditionalBranch_Map::deleteOnFalseEvent(int location)
{
	delete (onFalseEvents[location]);
	onFalseEvents.removeAt(location);
}

void BaseEvent::ConditionalBranch_Map::addChildEvent(BaseEvent *event, int location, int childTypeID)
{
	if (childTypeID == 1)
		addOnTrueEvent(event, location);
	else if (childTypeID == 2)
		addOnFalseEvent(event, location);
}

void BaseEvent::ConditionalBranch_Map::deleteChildEvent(int location, int childTypeID)
{
	if (childTypeID == 1)
		deleteOnTrueEvent(location);
	else if (childTypeID == 2)
		deleteOnFalseEvent(location);
}

void BaseEvent::ConditionalBranch_Map::applySwappedTiles(QList<Tileset::SwapTileIDs> swappedTiles)
{
	for (int i = 0; i < onTrueEvents.size(); ++i)
		onTrueEvents[i]->applySwappedTiles(swappedTiles);
	for (int i = 0; i < onFalseEvents.size(); ++i)
		onFalseEvents[i]->applySwappedTiles(swappedTiles);
}

bool BaseEvent::ConditionalBranch_Map::tryLoad(XMLNode EventNode, int baseType)
{
	XMLNode TempNode1, TempNode2;
	QStringList tempStringList;
	int count;
	bool executeCustomHandler;
	if (!XMLTools::nodeExists(EventNode, "conditions"))
		return false;
	TempNode1 = EventNode.getChildNode("conditions");
	if (!XMLTools::attributeExists(TempNode1, "conditionsToMatch"))
		return false;
	tempStringList.clear();
	tempStringList << "all" << "atLeast" << "none";
	if (!XMLTools::attributeStringValid(TempNode1, "conditionsToMatch", tempStringList))
		return false;
	if (QString(TempNode1.getAttribute("conditionsToMatch")) == "atLeast")
	{
		if (!XMLTools::attributeExists(TempNode1, "numConditionsToMatch"))
			return false;
	}
	tempStringList.clear();
	tempStringList << "true" << "false";
	if (!XMLTools::attributeExists(TempNode1, "executeCustomHandlerIfConditionsNotMet"))
		return false;
	if (!XMLTools::attributeStringValid(TempNode1, "executeCustomHandlerIfConditionsNotMet", tempStringList, 1))
		return false;
	executeCustomHandler = (QString(TempNode1.getAttribute("executeCustomHandlerIfConditionsNotMet")) == "true") ? true:false;
	count = TempNode1.nChildNode("condition");
	for (int i = 0; i < count; ++i)
	{
		TempNode2 = TempNode1.getChildNode("condition", i);
		if (!Condition::tryLoad(TempNode2))
			return false;
	}
	if (!XMLTools::nodeExists(EventNode, "onTrueEvents"))
		return false;
	TempNode1 = EventNode.getChildNode("onTrueEvents");
	count = TempNode1.nChildNode("event");
	for (int i = 0; i < count; ++i)
	{
		if (!BaseEvent::tryLoad(TempNode1.getChildNode("event", i), baseType))
			return false;
	}
	if (executeCustomHandler)
	{
		if (!XMLTools::nodeExists(EventNode, "onFalseEvents"))
			return false;
		TempNode1 = EventNode.getChildNode("onFalseEvents");
		count = TempNode1.nChildNode("event");
		for (int i = 0; i < count; ++i)
		{
			if (!BaseEvent::tryLoad(TempNode1.getChildNode("event", i), baseType))
				return false;
		}
	}
	return true;
}

BaseEvent::ConditionalBranch_Map::ConditionalBranch_Map(BaseEvent *pBase) : Event(pBase)
{
	conditionsToMatch = ALL;
	numConditionsToMatch = 0;
	executeCustomHandlerIfConditionsNotMet = false;
}

BaseEvent::ConditionalBranch_Map::ConditionalBranch_Map(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	XMLNode TempNode1 = EventNode.getChildNode("conditions");
	QString tempString = TempNode1.getAttribute("conditionsToMatch");
	if (tempString == "all")
	{
		conditionsToMatch = ALL;
		numConditionsToMatch = 0;
	}
	else if (tempString == "atLeast")
	{
		conditionsToMatch = AT_LEAST;
		numConditionsToMatch = QString(TempNode1.getAttribute("numConditionsToMatch")).toInt();
	}
	else if (tempString == "none")
	{
		conditionsToMatch = NONE;
		numConditionsToMatch = 0;
	}
	tempString = TempNode1.getAttribute("executeCustomHandlerIfConditionsNotMet");
	executeCustomHandlerIfConditionsNotMet = (tempString == "true") ? true:false;
	for (int i = 0, count = TempNode1.nChildNode("condition"); i < count; ++i)
		addCondition(new Condition(TempNode1.getChildNode("condition", i)));
	TempNode1 = EventNode.getChildNode("onTrueEvents");
	for (int i = 0, count = TempNode1.nChildNode("event"); i < count; ++i)
		addOnTrueEvent(new BaseEvent(pBaseEvent->baseType, TempNode1.getChildNode("event", i)));
	if (executeCustomHandlerIfConditionsNotMet)
	{
		TempNode1 = EventNode.getChildNode("onFalseEvents");
		for (int i = 0, count = TempNode1.nChildNode("event"); i < count; ++i)
			addOnFalseEvent(new BaseEvent(pBaseEvent->baseType, TempNode1.getChildNode("event", i)));
	}
}

BaseEvent::ConditionalBranch_Map::ConditionalBranch_Map(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	int numConditions;
	int numOnTrueEvents;
	int numOnFalseEvents;
	conditionsToMatch = storageFile.getSignedInt();
	numConditionsToMatch = storageFile.getSignedInt();
	executeCustomHandlerIfConditionsNotMet = storageFile.getBool();
	numConditions = storageFile.getSignedInt();
	for (int i = 0; i < numConditions; ++i)
		addCondition(new Condition(storageFile));
	numOnTrueEvents = storageFile.getSignedInt();
	for (int i = 0; i < numOnTrueEvents; ++i)
		addOnTrueEvent(new BaseEvent(pBaseEvent->baseType, storageFile));
	if (executeCustomHandlerIfConditionsNotMet)
	{
		numOnFalseEvents = storageFile.getSignedInt();
		for (int i = 0; i < numOnFalseEvents; ++i)
			addOnFalseEvent(new BaseEvent(pBaseEvent->baseType, storageFile));
	}
}

BaseEvent::ConditionalBranch_Map::ConditionalBranch_Map(ConditionalBranch_Map *other, BaseEvent *pBase) : Event(pBase)
{
	conditionsToMatch = other->conditionsToMatch;
	numConditionsToMatch = other->numConditionsToMatch;
	executeCustomHandlerIfConditionsNotMet = other->executeCustomHandlerIfConditionsNotMet;
	for (int i = 0; i < other->conditions.size(); ++i)
		addCondition(new Condition(other->conditions[i]));
	for (int i = 0; i < other->onTrueEvents.size(); ++i)
		addOnTrueEvent(new BaseEvent(other->onTrueEvents[i]));
	for (int i = 0; i < other->onFalseEvents.size(); ++i)
		addOnFalseEvent(new BaseEvent(other->onFalseEvents[i]));
}
//END BaseEvent::ConditionalBranch_Map Member Functions

//BEGIN BaseEvent::ConditionalBranch_Map::Condition Member Functions
BaseEvent::ConditionalBranch_Map::Condition::Condition()
{
	type = SWITCH;
	conditionUnion.switchCondition = new SwitchCondition;
	conditionUnion.switchCondition->switchID = 0;
	conditionUnion.switchCondition->switchReferencedInVariable = false;
	conditionUnion.switchCondition->isOn = true;
}

BaseEvent::ConditionalBranch_Map::Condition::Condition(XMLNode ConditionNode)
{
	QString tempString;
	tempString = ConditionNode.getAttribute("type");
	if (tempString == "switch")
	{
		type = SWITCH;
		conditionUnion.switchCondition = new SwitchCondition;
		conditionUnion.switchCondition->switchID = QString(ConditionNode.getAttribute("switchID")).toInt();
		tempString = ConditionNode.getAttribute("switchReferencedInVariable");
		conditionUnion.switchCondition->switchReferencedInVariable = (tempString == "true") ? true:false;
		tempString = ConditionNode.getAttribute("isOn");
		conditionUnion.switchCondition->isOn = (tempString == "true") ? true:false;
	}
	else if (tempString == "variable")
	{
		type = VARIABLE;
		conditionUnion.variableCondition = new VariableCondition;
		tempString = ConditionNode.getAttribute("comparison");
		if (tempString == "==")
			conditionUnion.variableCondition->comparison = VariableCondition::EQUAL_TO;
		else if (tempString == "!=")
			conditionUnion.variableCondition->comparison = VariableCondition::NOT_EQUAL_TO;
		else if (tempString == ">")
			conditionUnion.variableCondition->comparison = VariableCondition::GREATER_THAN;
		else if (tempString == ">=")
			conditionUnion.variableCondition->comparison = VariableCondition::GREATER_THAN_OR_EQUAL_TO;
		else if (tempString == "<")
			conditionUnion.variableCondition->comparison = VariableCondition::LESS_THAN;
		else if (tempString == "<=")
			conditionUnion.variableCondition->comparison = VariableCondition::LESS_THAN_OR_EQUAL_TO;
		conditionUnion.variableCondition->srcVariableID = QString(ConditionNode.getAttribute("srcVariableID")).toInt();
		conditionUnion.variableCondition->dstValue = QString(ConditionNode.getAttribute("dstValue")).toInt();
		tempString = ConditionNode.getAttribute("dstValueStoredInVariable");
		conditionUnion.variableCondition->dstValueStoredInVariable = (tempString == "true") ? true:false;
	}
	else if (tempString == "string")
	{
		type = STRING;
		conditionUnion.stringCondition = new StringCondition;
		tempString = ConditionNode.getAttribute("comparison");
		if (tempString == "==")
			conditionUnion.stringCondition->comparison = StringCondition::EQUAL_TO;
		else if (tempString == "!=")
			conditionUnion.stringCondition->comparison = StringCondition::NOT_EQUAL_TO;
		else if (tempString == "startsWith")
			conditionUnion.stringCondition->comparison = StringCondition::STARTS_WITH;
		else if (tempString == "endsWith")
			conditionUnion.stringCondition->comparison = StringCondition::ENDS_WITH;
		else if (tempString == "contains")
			conditionUnion.stringCondition->comparison = StringCondition::CONTAINS;
		else if (tempString == "doesNotContain")
			conditionUnion.stringCondition->comparison = StringCondition::DOES_NOT_CONTAIN;
		else if (tempString == "isEmpty")
			conditionUnion.stringCondition->comparison = StringCondition::IS_EMPTY;
		else if (tempString == "isNotEmpty")
			conditionUnion.stringCondition->comparison = StringCondition::IS_NOT_EMPTY;
		conditionUnion.stringCondition->srcStringID = QString(ConditionNode.getAttribute("srcStringID")).toInt();
		tempString = ConditionNode.getAttribute("srcStringReferencedInVariable");
		conditionUnion.stringCondition->srcStringReferencedInVariable = (tempString == "true") ? true:false;
		tempString = ConditionNode.getAttribute("dstStringType");
		if (tempString == "stringVariable")
		{
			conditionUnion.stringCondition->dstStringType = StringCondition::STRING_VARIABLE;
			conditionUnion.stringCondition->dstValue = "";
			conditionUnion.stringCondition->dstStringID = QString(ConditionNode.getAttribute("dstStringID")).toInt();
		}
		else if (tempString == "referencedString")
		{
			conditionUnion.stringCondition->dstStringType = StringCondition::REFERENCED_STRING;
			conditionUnion.stringCondition->dstValue = "";
			conditionUnion.stringCondition->dstStringID = QString(ConditionNode.getAttribute("dstVariableID")).toInt();
		}
		else if (tempString == "value")
		{
			conditionUnion.stringCondition->dstStringType = StringCondition::VALUE;
			conditionUnion.stringCondition->dstValue = ConditionNode.getAttribute("dstValue");
			conditionUnion.stringCondition->dstStringID = 0;
		}
	}
	else if (tempString == "timer")
	{
		type = TIMER;
		conditionUnion.timerCondition = new TimerCondition;
		conditionUnion.timerCondition->timerID = QString(ConditionNode.getAttribute("timerID")).toInt();
		tempString = ConditionNode.getAttribute("timerReferencedInVariable");
		conditionUnion.timerCondition->timerReferencedInVariable = (tempString == "true") ? true:false;
		tempString = ConditionNode.getAttribute("timeStoredInVariable");
		conditionUnion.timerCondition->timeStoredInVariable = (tempString == "true") ? true:false;
		if (conditionUnion.timerCondition->timeStoredInVariable)
		{
			conditionUnion.timerCondition->minutes = 0;
			conditionUnion.timerCondition->seconds = QString(ConditionNode.getAttribute("variableID")).toInt();
		}
		else
		{
			conditionUnion.timerCondition->minutes = QString(ConditionNode.getAttribute("minutes")).toInt();
			conditionUnion.timerCondition->seconds = QString(ConditionNode.getAttribute("seconds")).toInt();
		}
		tempString = ConditionNode.getAttribute("comparison");
		if (tempString == "==")
			conditionUnion.timerCondition->comparison = TimerCondition::EQUAL_TO;
		else if (tempString == "!=")
			conditionUnion.timerCondition->comparison = TimerCondition::NOT_EQUAL_TO;
		else if (tempString == ">")
			conditionUnion.timerCondition->comparison = TimerCondition::GREATER_THAN;
		else if (tempString == ">=")
			conditionUnion.timerCondition->comparison = TimerCondition::GREATER_THAN_OR_EQUAL_TO;
		else if (tempString == "<")
			conditionUnion.timerCondition->comparison = TimerCondition::LESS_THAN;
		else if (tempString == "<=")
			conditionUnion.timerCondition->comparison = TimerCondition::LESS_THAN_OR_EQUAL_TO;
	}
	else if (tempString == "money")
	{
		type = MONEY;
		conditionUnion.moneyCondition = new MoneyCondition;
		tempString = ConditionNode.getAttribute("comparison");
		if (tempString == "==")
			conditionUnion.moneyCondition->comparison = MoneyCondition::EQUAL_TO;
		else if (tempString == "!=")
			conditionUnion.moneyCondition->comparison = MoneyCondition::NOT_EQUAL_TO;
		else if (tempString == ">")
			conditionUnion.moneyCondition->comparison = MoneyCondition::GREATER_THAN;
		else if (tempString == ">=")
			conditionUnion.moneyCondition->comparison = MoneyCondition::GREATER_THAN_OR_EQUAL_TO;
		else if (tempString == "<")
			conditionUnion.moneyCondition->comparison = MoneyCondition::LESS_THAN;
		else if (tempString == "<=")
			conditionUnion.moneyCondition->comparison = MoneyCondition::LESS_THAN_OR_EQUAL_TO;
		conditionUnion.moneyCondition->value = QString(ConditionNode.getAttribute("value")).toInt();
		tempString = ConditionNode.getAttribute("valueStoredInVariable");
		conditionUnion.moneyCondition->valueStoredInVariable = (tempString == "true") ? true:false;
	}
	else if (tempString == "item")
	{
		type = ITEM;
		conditionUnion.itemCondition = new ItemCondition;
		conditionUnion.itemCondition->itemID = QString(ConditionNode.getAttribute("itemID")).toInt();
		tempString = ConditionNode.getAttribute("itemReferencedInVariable");
		conditionUnion.itemCondition->itemReferencedInVariable = (tempString == "true") ? true:false;
		tempString = ConditionNode.getAttribute("partyPossesses");
		conditionUnion.itemCondition->partyPossesses = (tempString == "true") ? true:false;
	}
	else if (tempString == "hero")
	{
		type = HERO;
		conditionUnion.heroCondition = new HeroCondition;
		conditionUnion.heroCondition->characterID = QString(ConditionNode.getAttribute("characterID")).toInt();
		tempString = ConditionNode.getAttribute("characterReferencedInVariable");
		conditionUnion.heroCondition->characterReferencedInVariable = (tempString == "true") ? true:false;
		tempString = ConditionNode.getAttribute("heroConditionType");
		if (tempString == "inParty")
		{
			conditionUnion.heroCondition->conditionType = HeroCondition::IN_PARTY;
			conditionUnion.heroCondition->string = "";
			conditionUnion.heroCondition->value = 0;
			conditionUnion.heroCondition->valueStoredInVariable = false;
		}
		else if (tempString == "nameIs")
		{
			conditionUnion.heroCondition->conditionType = HeroCondition::NAME_IS;
			conditionUnion.heroCondition->string = "";
			conditionUnion.heroCondition->value = 0;
			tempString = ConditionNode.getAttribute("stringStoredInVariable");
			conditionUnion.heroCondition->valueStoredInVariable = (tempString == "true") ? true:false;
			if (tempString == "true")
				conditionUnion.heroCondition->value = QString(ConditionNode.getAttribute("string")).toInt();
			else
				conditionUnion.heroCondition->string = ConditionNode.getAttribute("string");
		}
		else if (tempString == "levelIsAtLeast")
		{
			conditionUnion.heroCondition->conditionType = HeroCondition::LEVEL_IS_AT_LEAST;
			conditionUnion.heroCondition->string = "";
			conditionUnion.heroCondition->value = QString(ConditionNode.getAttribute("level")).toInt();
			tempString = ConditionNode.getAttribute("levelStoredInVariable");
			conditionUnion.heroCondition->valueStoredInVariable = (tempString == "true") ? true:false;
		}
		else if (tempString == "hpIsAtLeast")
		{
			conditionUnion.heroCondition->conditionType = HeroCondition::HP_IS_AT_LEAST;
			conditionUnion.heroCondition->string = "";
			conditionUnion.heroCondition->value = QString(ConditionNode.getAttribute("hp")).toInt();
			tempString = ConditionNode.getAttribute("hpStoredInVariable");
			conditionUnion.heroCondition->valueStoredInVariable = (tempString == "true") ? true:false;
		}
		else if (tempString == "canUseSkill")
		{
			conditionUnion.heroCondition->conditionType = HeroCondition::CAN_USE_SKILL;
			conditionUnion.heroCondition->string = "";
			conditionUnion.heroCondition->value = QString(ConditionNode.getAttribute("skillID")).toInt();
			tempString = ConditionNode.getAttribute("skillReferencedInVariable");
			conditionUnion.heroCondition->valueStoredInVariable = (tempString == "true") ? true:false;
		}
		else if (tempString == "hasItemEquipped")
		{
			conditionUnion.heroCondition->conditionType = HeroCondition::HAS_ITEM_EQUIPPED;
			conditionUnion.heroCondition->string = "";
			conditionUnion.heroCondition->value = QString(ConditionNode.getAttribute("itemID")).toInt();
			tempString = ConditionNode.getAttribute("itemReferencedInVariable");
			conditionUnion.heroCondition->valueStoredInVariable = (tempString == "true") ? true:false;
		}
		else if (tempString == "afflictedWithCondition")
		{
			conditionUnion.heroCondition->conditionType = HeroCondition::AFFLICTED_WITH_CONDITION;
			conditionUnion.heroCondition->string = "";
			conditionUnion.heroCondition->value = QString(ConditionNode.getAttribute("conditionID")).toInt();
			tempString = ConditionNode.getAttribute("conditionReferencedInVariable");
			conditionUnion.heroCondition->valueStoredInVariable = (tempString == "true") ? true:false;
		}
	}
	else if (tempString == "sprite")
	{
		type = SPRITE;
		conditionUnion.spriteCondition = new SpriteCondition;
		tempString = ConditionNode.getAttribute("spriteType");
		if (tempString == "hero")
		{
			conditionUnion.spriteCondition->spriteType = SpriteCondition::HERO;
			conditionUnion.spriteCondition->spriteID = 0;
			conditionUnion.spriteCondition->spriteReferencedInVariable = false;
		}
		else if (tempString == "vehicle")
		{
			conditionUnion.spriteCondition->spriteType = SpriteCondition::VEHICLE;
			conditionUnion.spriteCondition->spriteID = QString(ConditionNode.getAttribute("vehicleID")).toInt();
			conditionUnion.spriteCondition->spriteReferencedInVariable = (QString(ConditionNode.getAttribute("vehicleReferencedInVariable")) == "true") ? true:false;
		}
		else if (tempString == "event")
		{
			conditionUnion.spriteCondition->spriteType = SpriteCondition::EVENT;
			conditionUnion.spriteCondition->spriteID = QString(ConditionNode.getAttribute("eventID")).toInt();
			conditionUnion.spriteCondition->spriteReferencedInVariable = (QString(ConditionNode.getAttribute("eventReferencedInVariable")) == "true") ? true:false;
		}
		tempString = ConditionNode.getAttribute("isFacing");
		if (tempString == "up")
			conditionUnion.spriteCondition->direction = MapEvent::DIRECTION_UP;
		else if (tempString == "right")
			conditionUnion.spriteCondition->direction = MapEvent::DIRECTION_RIGHT;
		else if (tempString == "down")
			conditionUnion.spriteCondition->direction = MapEvent::DIRECTION_DOWN;
		else if (tempString == "left")
			conditionUnion.spriteCondition->direction = MapEvent::DIRECTION_LEFT;
	}
	else if (tempString == "partyIsRiding")
	{
		type = PARTY_IS_RIDING;
		conditionUnion.partyIsRidingCondition = new PartyIsRidingCondition;
		conditionUnion.partyIsRidingCondition->vehicleID = QString(ConditionNode.getAttribute("vehicleID")).toInt();
		tempString = ConditionNode.getAttribute("vehicleReferencedInVariable");
		conditionUnion.partyIsRidingCondition->vehicleReferencedInVariable = (tempString == "true") ? true:false;
	}
	else if (tempString == "eventStartedWithActionKey")
		type = EVENT_STARTED_WITH_ACTION_KEY;
	else if (tempString == "bgmPlayedThroughOnce")
		type = BGM_PLAYED_THROUGH_ONCE;
}

BaseEvent::ConditionalBranch_Map::Condition::Condition(StorageFile &storageFile)
{
	char *temp_string;
	type = (ConditionType)storageFile.getSignedInt();
	if (type == SWITCH)
	{
		conditionUnion.switchCondition = new SwitchCondition;
		conditionUnion.switchCondition->switchID = storageFile.getSignedInt();
		conditionUnion.switchCondition->switchReferencedInVariable = storageFile.getBool();
		conditionUnion.switchCondition->isOn = storageFile.getBool();
	}
	else if (type == VARIABLE)
	{
		conditionUnion.variableCondition = new VariableCondition;
		conditionUnion.variableCondition->comparison = storageFile.getSignedInt();
		conditionUnion.variableCondition->srcVariableID = storageFile.getSignedInt();
		conditionUnion.variableCondition->dstValue = storageFile.getSignedInt();
		conditionUnion.variableCondition->dstValueStoredInVariable = storageFile.getBool();
	}
	else if (type == STRING)
	{
		conditionUnion.stringCondition = new StringCondition;
		conditionUnion.stringCondition->comparison = storageFile.getSignedInt();
		conditionUnion.stringCondition->dstStringType = storageFile.getSignedInt();
		conditionUnion.stringCondition->srcStringID = storageFile.getSignedInt();
		conditionUnion.stringCondition->srcStringReferencedInVariable = storageFile.getBool();
		if (conditionUnion.stringCondition->dstStringType == StringCondition::STRING_VARIABLE || conditionUnion.stringCondition->dstStringType == StringCondition::REFERENCED_STRING)
		{
			conditionUnion.stringCondition->dstValue = "";
			conditionUnion.stringCondition->dstStringID = storageFile.getSignedInt();
		}
		else if (conditionUnion.stringCondition->dstStringType == StringCondition::VALUE)
		{
			temp_string = storageFile.getString();
			conditionUnion.stringCondition->dstValue = temp_string;
			delete[] temp_string;
			conditionUnion.stringCondition->dstStringID = 0;
		}
	}
	else if (type == TIMER)
	{
		conditionUnion.timerCondition = new TimerCondition;
		conditionUnion.timerCondition->comparison = storageFile.getSignedInt();
		conditionUnion.timerCondition->timerID = storageFile.getSignedInt();
		conditionUnion.timerCondition->timerReferencedInVariable = storageFile.getBool();
		conditionUnion.timerCondition->timeStoredInVariable = storageFile.getBool();
		if (conditionUnion.timerCondition->timeStoredInVariable)
		{
			conditionUnion.timerCondition->minutes = 0;
			conditionUnion.timerCondition->seconds = storageFile.getSignedInt();
		}
		else
		{
			conditionUnion.timerCondition->minutes = storageFile.getSignedInt();
			conditionUnion.timerCondition->seconds = storageFile.getSignedInt();
		}
	}
	else if (type == MONEY)
	{
		conditionUnion.moneyCondition = new MoneyCondition;
		conditionUnion.moneyCondition->comparison = storageFile.getSignedInt();
		conditionUnion.moneyCondition->value = storageFile.getSignedInt();
		conditionUnion.moneyCondition->valueStoredInVariable = storageFile.getBool();
	}
	else if (type == ITEM)
	{
		conditionUnion.itemCondition = new ItemCondition;
		conditionUnion.itemCondition->itemID = storageFile.getSignedInt();
		conditionUnion.itemCondition->itemReferencedInVariable = storageFile.getBool();
		conditionUnion.itemCondition->partyPossesses = storageFile.getBool();
	}
	else if (type == HERO)
	{
		conditionUnion.heroCondition = new HeroCondition;
		conditionUnion.heroCondition->conditionType = storageFile.getSignedInt();
		conditionUnion.heroCondition->characterID = storageFile.getSignedInt();
		conditionUnion.heroCondition->characterReferencedInVariable = storageFile.getBool();
		if (conditionUnion.heroCondition->conditionType == HeroCondition::IN_PARTY)
		{
			conditionUnion.heroCondition->string = "";
			conditionUnion.heroCondition->value = 0;
			conditionUnion.heroCondition->valueStoredInVariable = false;
		}
		else if (conditionUnion.heroCondition->conditionType == HeroCondition::NAME_IS)
		{
			conditionUnion.heroCondition->string = "";
			conditionUnion.heroCondition->value = 0;
			conditionUnion.heroCondition->valueStoredInVariable = storageFile.getBool();
			if (conditionUnion.heroCondition->valueStoredInVariable)
				conditionUnion.heroCondition->value = storageFile.getSignedInt();
			else
			{
				temp_string = storageFile.getString();
				conditionUnion.heroCondition->string = temp_string;
				delete[] temp_string;
			}
		}
		else
		{
			conditionUnion.heroCondition->string = "";
			conditionUnion.heroCondition->value = storageFile.getSignedInt();
			conditionUnion.heroCondition->valueStoredInVariable = storageFile.getBool();
		}
	}
	else if (type == SPRITE)
	{
		conditionUnion.spriteCondition = new SpriteCondition;
		conditionUnion.spriteCondition->spriteType = storageFile.getSignedInt();
		conditionUnion.spriteCondition->spriteID = storageFile.getSignedInt();
		conditionUnion.spriteCondition->direction = storageFile.getSignedInt();
		conditionUnion.spriteCondition->spriteReferencedInVariable = storageFile.getBool();
	}
	else if (type == PARTY_IS_RIDING)
	{
		conditionUnion.partyIsRidingCondition = new PartyIsRidingCondition;
		conditionUnion.partyIsRidingCondition->vehicleID = storageFile.getSignedInt();
		conditionUnion.partyIsRidingCondition->vehicleReferencedInVariable = storageFile.getBool();
	}
}

BaseEvent::ConditionalBranch_Map::Condition::Condition(Condition *other)
{
	type = other->type;
	if (type == SWITCH)
	{
		conditionUnion.switchCondition = new SwitchCondition;
		conditionUnion.switchCondition->switchID = other->conditionUnion.switchCondition->switchID;
		conditionUnion.switchCondition->switchReferencedInVariable = other->conditionUnion.switchCondition->switchReferencedInVariable;
		conditionUnion.switchCondition->isOn = other->conditionUnion.switchCondition->isOn;
	}
	else if (type == VARIABLE)
	{
		conditionUnion.variableCondition = new VariableCondition;
		conditionUnion.variableCondition->comparison = other->conditionUnion.variableCondition->comparison;
		conditionUnion.variableCondition->srcVariableID = other->conditionUnion.variableCondition->srcVariableID;
		conditionUnion.variableCondition->dstValue = other->conditionUnion.variableCondition->dstValue;
		conditionUnion.variableCondition->dstValueStoredInVariable = other->conditionUnion.variableCondition->dstValueStoredInVariable;
	}
	else if (type == STRING)
	{
		conditionUnion.stringCondition = new StringCondition;
		conditionUnion.stringCondition->comparison = other->conditionUnion.stringCondition->comparison;
		conditionUnion.stringCondition->dstStringType = other->conditionUnion.stringCondition->dstStringType;
		conditionUnion.stringCondition->dstValue = "";
		conditionUnion.stringCondition->dstValue.append(other->conditionUnion.stringCondition->dstValue);
		conditionUnion.stringCondition->srcStringID = other->conditionUnion.stringCondition->srcStringID;
		conditionUnion.stringCondition->dstStringID = other->conditionUnion.stringCondition->dstStringID;
		conditionUnion.stringCondition->srcStringReferencedInVariable = other->conditionUnion.stringCondition->srcStringReferencedInVariable;
	}
	else if (type == TIMER)
	{
		conditionUnion.timerCondition = new TimerCondition;
		conditionUnion.timerCondition->comparison = other->conditionUnion.timerCondition->comparison;
		conditionUnion.timerCondition->timerID = other->conditionUnion.timerCondition->timerID;
		conditionUnion.timerCondition->minutes = other->conditionUnion.timerCondition->minutes;
		conditionUnion.timerCondition->seconds = other->conditionUnion.timerCondition->seconds;
		conditionUnion.timerCondition->timerReferencedInVariable = other->conditionUnion.timerCondition->timerReferencedInVariable;
		conditionUnion.timerCondition->timeStoredInVariable = other->conditionUnion.timerCondition->timeStoredInVariable;
	}
	else if (type == MONEY)
	{
		conditionUnion.moneyCondition = new MoneyCondition;
		conditionUnion.moneyCondition->comparison = other->conditionUnion.moneyCondition->comparison;
		conditionUnion.moneyCondition->value = other->conditionUnion.moneyCondition->value;
		conditionUnion.moneyCondition->valueStoredInVariable = other->conditionUnion.moneyCondition->valueStoredInVariable;
	}
	else if (type == ITEM)
	{
		conditionUnion.itemCondition = new ItemCondition;
		conditionUnion.itemCondition->itemID = other->conditionUnion.itemCondition->itemID;
		conditionUnion.itemCondition->itemReferencedInVariable = other->conditionUnion.itemCondition->itemReferencedInVariable;
		conditionUnion.itemCondition->partyPossesses = other->conditionUnion.itemCondition->partyPossesses;
	}
	else if (type == HERO)
	{
		conditionUnion.heroCondition = new HeroCondition;
		conditionUnion.heroCondition->conditionType = other->conditionUnion.heroCondition->conditionType;
		conditionUnion.heroCondition->string = "";
		conditionUnion.heroCondition->string.append(other->conditionUnion.heroCondition->string);
		conditionUnion.heroCondition->value = other->conditionUnion.heroCondition->value;
		conditionUnion.heroCondition->characterID = other->conditionUnion.heroCondition->characterID;
		conditionUnion.heroCondition->characterReferencedInVariable = other->conditionUnion.heroCondition->characterReferencedInVariable;
		conditionUnion.heroCondition->valueStoredInVariable = other->conditionUnion.heroCondition->valueStoredInVariable;
	}
	else if (type == SPRITE)
	{
		conditionUnion.spriteCondition = new SpriteCondition;
		conditionUnion.spriteCondition->spriteType = other->conditionUnion.spriteCondition->spriteType;
		conditionUnion.spriteCondition->spriteID = other->conditionUnion.spriteCondition->spriteID;
		conditionUnion.spriteCondition->direction = other->conditionUnion.spriteCondition->direction;
		conditionUnion.spriteCondition->spriteReferencedInVariable = other->conditionUnion.spriteCondition->spriteReferencedInVariable;
	}
	else if (type == PARTY_IS_RIDING)
	{
		conditionUnion.partyIsRidingCondition = new PartyIsRidingCondition;
		conditionUnion.partyIsRidingCondition->vehicleID = other->conditionUnion.partyIsRidingCondition->vehicleID;
		conditionUnion.partyIsRidingCondition->vehicleReferencedInVariable = other->conditionUnion.partyIsRidingCondition->vehicleReferencedInVariable;
	}
}

BaseEvent::ConditionalBranch_Map::Condition::~Condition()
{
	if (type == SWITCH)
		delete (conditionUnion.switchCondition);
	else if (type == VARIABLE)
		delete (conditionUnion.variableCondition);
	else if (type == STRING)
		delete (conditionUnion.stringCondition);
	else if (type == TIMER)
		delete (conditionUnion.timerCondition);
	else if (type == MONEY)
		delete (conditionUnion.moneyCondition);
	else if (type == ITEM)
		delete (conditionUnion.itemCondition);
	else if (type == HERO)
		delete (conditionUnion.heroCondition);
	else if (type == SPRITE)
		delete (conditionUnion.spriteCondition);
	else if (type == PARTY_IS_RIDING)
		delete (conditionUnion.partyIsRidingCondition);
}

XMLNode BaseEvent::ConditionalBranch_Map::Condition::getConditionNode()
{
	XMLNode ConditionNode = XMLNode::createXMLTopNode("condition");
	if (type == SWITCH)
	{
		ConditionNode.addAttribute("type", "switch");
		ConditionNode.addAttribute("switchID", QString::number(conditionUnion.switchCondition->switchID).toUtf8().data());
		ConditionNode.addAttribute("switchReferencedInVariable", (conditionUnion.switchCondition->switchReferencedInVariable) ? "true":"false");
		ConditionNode.addAttribute("isOn", (conditionUnion.switchCondition->isOn) ? "true":"false");
	}
	else if (type == VARIABLE)
	{
		ConditionNode.addAttribute("type", "variable");
		if (conditionUnion.variableCondition->comparison == VariableCondition::EQUAL_TO)
			ConditionNode.addAttribute("comparison", "==");
		else if (conditionUnion.variableCondition->comparison == VariableCondition::NOT_EQUAL_TO)
			ConditionNode.addAttribute("comparison", "!=");
		else if (conditionUnion.variableCondition->comparison == VariableCondition::GREATER_THAN)
			ConditionNode.addAttribute("comparison", ">");
		else if (conditionUnion.variableCondition->comparison == VariableCondition::GREATER_THAN_OR_EQUAL_TO)
			ConditionNode.addAttribute("comparison", ">=");
		else if (conditionUnion.variableCondition->comparison == VariableCondition::LESS_THAN)
			ConditionNode.addAttribute("comparison", "<");
		else if (conditionUnion.variableCondition->comparison == VariableCondition::LESS_THAN_OR_EQUAL_TO)
			ConditionNode.addAttribute("comparison", "<=");
		ConditionNode.addAttribute("srcVariableID", QString::number(conditionUnion.variableCondition->srcVariableID).toUtf8().data());
		ConditionNode.addAttribute("dstValue", QString::number(conditionUnion.variableCondition->dstValue).toUtf8().data());
		ConditionNode.addAttribute("dstValueStoredInVariable", (conditionUnion.variableCondition->dstValueStoredInVariable) ? "true":"false");
	}
	else if (type == STRING)
	{
		ConditionNode.addAttribute("type", "string");
		if (conditionUnion.stringCondition->comparison == StringCondition::EQUAL_TO)
			ConditionNode.addAttribute("comparison", "==");
		else if (conditionUnion.stringCondition->comparison == StringCondition::NOT_EQUAL_TO)
			ConditionNode.addAttribute("comparison", "!=");
		else if (conditionUnion.stringCondition->comparison == StringCondition::STARTS_WITH)
			ConditionNode.addAttribute("comparison", "startsWith");
		else if (conditionUnion.stringCondition->comparison == StringCondition::ENDS_WITH)
			ConditionNode.addAttribute("comparison", "endsWith");
		else if (conditionUnion.stringCondition->comparison == StringCondition::CONTAINS)
			ConditionNode.addAttribute("comparison", "contains");
		else if (conditionUnion.stringCondition->comparison == StringCondition::DOES_NOT_CONTAIN)
			ConditionNode.addAttribute("comparison", "doesNotContain");
		else if (conditionUnion.stringCondition->comparison == StringCondition::IS_EMPTY)
			ConditionNode.addAttribute("comparison", "isEmpty");
		else if (conditionUnion.stringCondition->comparison == StringCondition::IS_NOT_EMPTY)
			ConditionNode.addAttribute("comparison", "isNotEmpty");
		ConditionNode.addAttribute("srcStringID", QString::number(conditionUnion.stringCondition->srcStringID).toUtf8().data());
		ConditionNode.addAttribute("srcStringReferencedInVariable", (conditionUnion.stringCondition->srcStringReferencedInVariable) ? "true":"false");
		if (conditionUnion.stringCondition->dstStringType == StringCondition::STRING_VARIABLE)
		{
			ConditionNode.addAttribute("dstStringType", "stringVariable");
			ConditionNode.addAttribute("dstStringID", QString::number(conditionUnion.stringCondition->dstStringID).toUtf8().data());
		}
		else if (conditionUnion.stringCondition->dstStringType == StringCondition::REFERENCED_STRING)
		{
			ConditionNode.addAttribute("dstStringType", "referencedString");
			ConditionNode.addAttribute("dstVariableID", QString::number(conditionUnion.stringCondition->dstStringID).toUtf8().data());
		}
		else if (conditionUnion.stringCondition->dstStringType == StringCondition::VALUE)
		{
			ConditionNode.addAttribute("dstStringType", "value");
			ConditionNode.addAttribute("dstValue", conditionUnion.stringCondition->dstValue.toUtf8().data());
		}
	}
	else if (type == TIMER)
	{
		ConditionNode.addAttribute("type", "timer");
		if (conditionUnion.timerCondition->comparison == TimerCondition::EQUAL_TO)
			ConditionNode.addAttribute("comparison", "==");
		else if (conditionUnion.timerCondition->comparison == TimerCondition::NOT_EQUAL_TO)
			ConditionNode.addAttribute("comparison", "!=");
		else if (conditionUnion.timerCondition->comparison == TimerCondition::GREATER_THAN)
			ConditionNode.addAttribute("comparison", ">");
		else if (conditionUnion.timerCondition->comparison == TimerCondition::GREATER_THAN_OR_EQUAL_TO)
			ConditionNode.addAttribute("comparison", ">=");
		else if (conditionUnion.timerCondition->comparison == TimerCondition::LESS_THAN)
			ConditionNode.addAttribute("comparison", "<");
		else if (conditionUnion.timerCondition->comparison == TimerCondition::LESS_THAN_OR_EQUAL_TO)
			ConditionNode.addAttribute("comparison", "<=");
		ConditionNode.addAttribute("timerID", QString::number(conditionUnion.timerCondition->timerID).toUtf8().data());
		ConditionNode.addAttribute("timerReferencedInVariable", (conditionUnion.timerCondition->timerReferencedInVariable) ? "true":"false");
		ConditionNode.addAttribute("timeStoredInVariable", (conditionUnion.timerCondition->timeStoredInVariable) ? "true":"false");
		if (conditionUnion.timerCondition->timeStoredInVariable)
			ConditionNode.addAttribute("variableID", QString::number(conditionUnion.timerCondition->seconds).toUtf8().data());
		else
		{
			ConditionNode.addAttribute("minutes", QString::number(conditionUnion.timerCondition->minutes).toUtf8().data());
			ConditionNode.addAttribute("seconds", QString::number(conditionUnion.timerCondition->seconds).toUtf8().data());
		}
	}
	else if (type == MONEY)
	{
		ConditionNode.addAttribute("type", "money");
		if (conditionUnion.moneyCondition->comparison == MoneyCondition::EQUAL_TO)
			ConditionNode.addAttribute("comparison", "==");
		else if (conditionUnion.moneyCondition->comparison == MoneyCondition::NOT_EQUAL_TO)
			ConditionNode.addAttribute("comparison", "!=");
		else if (conditionUnion.moneyCondition->comparison == MoneyCondition::GREATER_THAN)
			ConditionNode.addAttribute("comparison", ">");
		else if (conditionUnion.moneyCondition->comparison == MoneyCondition::GREATER_THAN_OR_EQUAL_TO)
			ConditionNode.addAttribute("comparison", ">=");
		else if (conditionUnion.moneyCondition->comparison == MoneyCondition::LESS_THAN)
			ConditionNode.addAttribute("comparison", "<");
		else if (conditionUnion.moneyCondition->comparison == MoneyCondition::LESS_THAN_OR_EQUAL_TO)
			ConditionNode.addAttribute("comparison", "<=");
		ConditionNode.addAttribute("value", QString::number(conditionUnion.moneyCondition->value).toUtf8().data());
		ConditionNode.addAttribute("valueStoredInVariable", (conditionUnion.moneyCondition->valueStoredInVariable) ? "true":"false");
	}
	else if (type == ITEM)
	{
		ConditionNode.addAttribute("type", "item");
		ConditionNode.addAttribute("itemID", QString::number(conditionUnion.itemCondition->itemID).toUtf8().data());
		ConditionNode.addAttribute("itemReferencedInVariable", (conditionUnion.itemCondition->itemReferencedInVariable) ? "true":"false");
		ConditionNode.addAttribute("partyPossesses", (conditionUnion.itemCondition->partyPossesses) ? "true":"false");
	}
	else if (type == HERO)
	{
		ConditionNode.addAttribute("type", "hero");
		ConditionNode.addAttribute("characterID", QString::number(conditionUnion.heroCondition->characterID).toUtf8().data());
		ConditionNode.addAttribute("characterReferencedInVariable", (conditionUnion.heroCondition->characterReferencedInVariable) ? "true":"false");
		if (conditionUnion.heroCondition->conditionType == HeroCondition::IN_PARTY)
			ConditionNode.addAttribute("heroConditionType", "inParty");
		else if (conditionUnion.heroCondition->conditionType == HeroCondition::NAME_IS)
		{
			ConditionNode.addAttribute("heroConditionType", "nameIs");
			ConditionNode.addAttribute("stringStoredInVariable", (conditionUnion.heroCondition->valueStoredInVariable) ? "true":"false");
			if (conditionUnion.heroCondition->valueStoredInVariable)
				ConditionNode.addAttribute("string", QString::number(conditionUnion.heroCondition->value).toUtf8().data());
			else
				ConditionNode.addAttribute("string", conditionUnion.heroCondition->string.toUtf8().data());
		}
		else if (conditionUnion.heroCondition->conditionType == HeroCondition::LEVEL_IS_AT_LEAST)
		{
			ConditionNode.addAttribute("heroConditionType", "levelIsAtLeast");
			ConditionNode.addAttribute("level", QString::number(conditionUnion.heroCondition->value).toUtf8().data());
			ConditionNode.addAttribute("levelStoredInVariable", (conditionUnion.heroCondition->valueStoredInVariable) ? "true":"false");
		}
		else if (conditionUnion.heroCondition->conditionType == HeroCondition::HP_IS_AT_LEAST)
		{
			ConditionNode.addAttribute("heroConditionType", "hpIsAtLeast");
			ConditionNode.addAttribute("hp", QString::number(conditionUnion.heroCondition->value).toUtf8().data());
			ConditionNode.addAttribute("hpStoredInVariable", (conditionUnion.heroCondition->valueStoredInVariable) ? "true":"false");
		}
		else if (conditionUnion.heroCondition->conditionType == HeroCondition::CAN_USE_SKILL)
		{
			ConditionNode.addAttribute("heroConditionType", "canUseSkill");
			ConditionNode.addAttribute("skillID", QString::number(conditionUnion.heroCondition->value).toUtf8().data());
			ConditionNode.addAttribute("skillReferencedInVariable", (conditionUnion.heroCondition->valueStoredInVariable) ? "true":"false");
		}
		else if (conditionUnion.heroCondition->conditionType == HeroCondition::HAS_ITEM_EQUIPPED)
		{
			ConditionNode.addAttribute("heroConditionType", "hasItemEquipped");
			ConditionNode.addAttribute("itemID", QString::number(conditionUnion.heroCondition->value).toUtf8().data());
			ConditionNode.addAttribute("itemReferencedInVariable", (conditionUnion.heroCondition->valueStoredInVariable) ? "true":"false");
		}
		else if (conditionUnion.heroCondition->conditionType == HeroCondition::AFFLICTED_WITH_CONDITION)
		{
			ConditionNode.addAttribute("heroConditionType", "afflictedWithCondition");
			ConditionNode.addAttribute("conditionID", QString::number(conditionUnion.heroCondition->value).toUtf8().data());
			ConditionNode.addAttribute("conditionReferencedInVariable", (conditionUnion.heroCondition->valueStoredInVariable) ? "true":"false");
		}
	}
	else if (type == SPRITE)
	{
		ConditionNode.addAttribute("type", "sprite");
		if (conditionUnion.spriteCondition->spriteType == SpriteCondition::HERO)
			ConditionNode.addAttribute("spriteType", "hero");
		else if (conditionUnion.spriteCondition->spriteType == SpriteCondition::VEHICLE)
		{
			ConditionNode.addAttribute("spriteType", "vehicle");
			ConditionNode.addAttribute("vehicleID", QString::number(conditionUnion.spriteCondition->spriteID).toUtf8().data());
			ConditionNode.addAttribute("vehicleReferencedInVariable", (conditionUnion.spriteCondition->spriteReferencedInVariable) ? "true":"false");
		}
		else if (conditionUnion.spriteCondition->spriteType == SpriteCondition::EVENT)
		{
			ConditionNode.addAttribute("spriteType", "event");
			ConditionNode.addAttribute("eventID", QString::number(conditionUnion.spriteCondition->spriteID).toUtf8().data());
			ConditionNode.addAttribute("eventReferencedInVariable", (conditionUnion.spriteCondition->spriteReferencedInVariable) ? "true":"false");
		}
		if (conditionUnion.spriteCondition->direction == MapEvent::DIRECTION_UP)
			ConditionNode.addAttribute("isFacing", "up");
		else if (conditionUnion.spriteCondition->direction == MapEvent::DIRECTION_RIGHT)
			ConditionNode.addAttribute("isFacing", "right");
		else if (conditionUnion.spriteCondition->direction == MapEvent::DIRECTION_DOWN)
			ConditionNode.addAttribute("isFacing", "down");
		else if (conditionUnion.spriteCondition->direction == MapEvent::DIRECTION_LEFT)
			ConditionNode.addAttribute("isFacing", "left");
	}
	else if (type == PARTY_IS_RIDING)
	{
		ConditionNode.addAttribute("type", "partyIsRiding");
		ConditionNode.addAttribute("vehicleID", QString::number(conditionUnion.partyIsRidingCondition->vehicleID).toUtf8().data());
		ConditionNode.addAttribute("vehicleReferencedInVariable", (conditionUnion.partyIsRidingCondition->vehicleReferencedInVariable) ? "true":"false");
	}
	else if (type == EVENT_STARTED_WITH_ACTION_KEY)
		ConditionNode.addAttribute("type", "eventStartedWithActionKey");
	else if (type == BGM_PLAYED_THROUGH_ONCE)
		ConditionNode.addAttribute("type", "bgmPlayedThroughOnce");
	return ConditionNode;
}

void BaseEvent::ConditionalBranch_Map::Condition::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt((int)type);
	if (type == SWITCH)
	{
		storageFile.putSignedInt(conditionUnion.switchCondition->switchID);
		storageFile.putBool(conditionUnion.switchCondition->switchReferencedInVariable);
		storageFile.putBool(conditionUnion.switchCondition->isOn);
	}
	else if (type == VARIABLE)
	{
		storageFile.putSignedInt(conditionUnion.variableCondition->comparison);
		storageFile.putSignedInt(conditionUnion.variableCondition->srcVariableID);
		storageFile.putSignedInt(conditionUnion.variableCondition->dstValue);
		storageFile.putBool(conditionUnion.variableCondition->dstValueStoredInVariable);
	}
	else if (type == STRING)
	{
		storageFile.putSignedInt(conditionUnion.stringCondition->comparison);
		storageFile.putSignedInt(conditionUnion.stringCondition->dstStringType);
		storageFile.putSignedInt(conditionUnion.stringCondition->srcStringID);
		storageFile.putBool(conditionUnion.stringCondition->srcStringReferencedInVariable);
		if (conditionUnion.stringCondition->dstStringType == StringCondition::STRING_VARIABLE || conditionUnion.stringCondition->dstStringType == StringCondition::REFERENCED_STRING)
			storageFile.putSignedInt(conditionUnion.stringCondition->dstStringID);
		else if (conditionUnion.stringCondition->dstStringType == StringCondition::VALUE)
			storageFile.putString(conditionUnion.stringCondition->dstValue.toUtf8().data());
	}
	else if (type == TIMER)
	{
		storageFile.putSignedInt((int)conditionUnion.timerCondition->comparison);
		storageFile.putSignedInt(conditionUnion.timerCondition->timerID);
		storageFile.putBool(conditionUnion.timerCondition->timerReferencedInVariable);
		storageFile.putBool(conditionUnion.timerCondition->timeStoredInVariable);
		if (conditionUnion.timerCondition->timeStoredInVariable)
			storageFile.putSignedInt(conditionUnion.timerCondition->seconds);
		else
		{
			storageFile.putSignedInt(conditionUnion.timerCondition->minutes);
			storageFile.putSignedInt(conditionUnion.timerCondition->seconds);
		}
	}
	else if (type == MONEY)
	{
		storageFile.putSignedInt((int)conditionUnion.moneyCondition->comparison);
		storageFile.putSignedInt(conditionUnion.moneyCondition->value);
		storageFile.putBool(conditionUnion.moneyCondition->valueStoredInVariable);
	}
	else if (type == ITEM)
	{
		storageFile.putSignedInt(conditionUnion.itemCondition->itemID);
		storageFile.putBool(conditionUnion.itemCondition->itemReferencedInVariable);
		storageFile.putBool(conditionUnion.itemCondition->partyPossesses);
	}
	else if (type == HERO)
	{
		storageFile.putSignedInt((int)conditionUnion.heroCondition->conditionType);
		storageFile.putSignedInt(conditionUnion.heroCondition->characterID);
		storageFile.putBool(conditionUnion.heroCondition->characterReferencedInVariable);
		if (conditionUnion.heroCondition->conditionType == HeroCondition::NAME_IS)
		{
			storageFile.putBool(conditionUnion.heroCondition->valueStoredInVariable);
			if (conditionUnion.heroCondition->valueStoredInVariable)
				storageFile.putSignedInt(conditionUnion.heroCondition->value);
			else
				storageFile.putString(conditionUnion.heroCondition->string.toUtf8().data());
		}
		else
		{
			storageFile.putSignedInt(conditionUnion.heroCondition->value);
			storageFile.putBool(conditionUnion.heroCondition->valueStoredInVariable);
		}
	}
	else if (type == SPRITE)
	{
		storageFile.putSignedInt(conditionUnion.spriteCondition->spriteType);
		storageFile.putSignedInt(conditionUnion.spriteCondition->spriteID);
		storageFile.putSignedInt(conditionUnion.spriteCondition->direction);
		storageFile.putBool(conditionUnion.spriteCondition->spriteReferencedInVariable);
	}
	else if (type == PARTY_IS_RIDING)
	{
		storageFile.putSignedInt(conditionUnion.partyIsRidingCondition->vehicleID);
		storageFile.putBool(conditionUnion.partyIsRidingCondition->vehicleReferencedInVariable);
	}
}

QString BaseEvent::ConditionalBranch_Map::Condition::getViewableText(int indent) const
{
	QString viewableText = "";
	for (int i = 0; i < indent; ++i)
		viewableText += "  ";
	viewableText += "::";
	if (type == SWITCH)
	{
		viewableText += "Switch: ";
		if (conditionUnion.switchCondition->switchReferencedInVariable)
			viewableText += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(conditionUnion.switchCondition->switchID));
		else
			viewableText += QString("switch[%1]").arg(ProjectData::switchVariableIdToString(conditionUnion.switchCondition->switchID));
		if (conditionUnion.switchCondition->isOn)
			viewableText += " Is ON";
		else
			viewableText += " Is OFF";
	}
	else if (type == VARIABLE)
	{
		viewableText += "Variable: ";
		viewableText += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(conditionUnion.variableCondition->srcVariableID));
		if (conditionUnion.variableCondition->comparison == VariableCondition::EQUAL_TO)
			viewableText += " == ";
		if (conditionUnion.variableCondition->comparison == VariableCondition::NOT_EQUAL_TO)
			viewableText += " != ";
		if (conditionUnion.variableCondition->comparison == VariableCondition::GREATER_THAN)
			viewableText += " > ";
		if (conditionUnion.variableCondition->comparison == VariableCondition::GREATER_THAN_OR_EQUAL_TO)
			viewableText += " >= ";
		if (conditionUnion.variableCondition->comparison == VariableCondition::LESS_THAN)
			viewableText += " < ";
		if (conditionUnion.variableCondition->comparison == VariableCondition::LESS_THAN_OR_EQUAL_TO)
			viewableText += " <= ";
		if (conditionUnion.variableCondition->dstValueStoredInVariable)
			viewableText += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(conditionUnion.variableCondition->dstValue));
		else
			viewableText += QString::number(conditionUnion.variableCondition->dstValue);
	}
	else if (type == STRING)
	{
		viewableText += "String: ";
		if (conditionUnion.stringCondition->srcStringReferencedInVariable)
			viewableText += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(conditionUnion.stringCondition->srcStringID));
		else
			viewableText += QString("string[%1]").arg(ProjectData::stringVariableIdToString(conditionUnion.stringCondition->srcStringID));
		if (conditionUnion.stringCondition->comparison == StringCondition::EQUAL_TO)
			viewableText += " Is Equal To ";
		if (conditionUnion.stringCondition->comparison == StringCondition::NOT_EQUAL_TO)
			viewableText += " Isn\'t Equal To ";
		if (conditionUnion.stringCondition->comparison == StringCondition::STARTS_WITH)
			viewableText += " Starts With ";
		if (conditionUnion.stringCondition->comparison == StringCondition::ENDS_WITH)
			viewableText += " Ends With ";
		if (conditionUnion.stringCondition->comparison == StringCondition::CONTAINS)
			viewableText += " Contains ";
		if (conditionUnion.stringCondition->comparison == StringCondition::DOES_NOT_CONTAIN)
			viewableText += " Doesn\'t Contain ";
		if (conditionUnion.stringCondition->comparison == StringCondition::IS_EMPTY)
			viewableText += " Is Empty ";
		if (conditionUnion.stringCondition->comparison == StringCondition::IS_NOT_EMPTY)
			viewableText += " Isn\'t Empty ";
		if (conditionUnion.stringCondition->dstStringType == StringCondition::STRING_VARIABLE)
			viewableText += QString("string[%1]").arg(ProjectData::stringVariableIdToString(conditionUnion.stringCondition->dstStringID));
		else if (conditionUnion.stringCondition->dstStringType == StringCondition::REFERENCED_STRING)
			viewableText += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(conditionUnion.stringCondition->dstStringID));
		else
			viewableText += QString("\"%1\"").arg(conditionUnion.stringCondition->dstValue);
	}
	else if (type == TIMER)
	{
		viewableText += "Timer: ";
		if (conditionUnion.timerCondition->timerReferencedInVariable)
			viewableText += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(conditionUnion.timerCondition->timerID));
		else
			viewableText += QString::number(conditionUnion.timerCondition->timerID);
		if (conditionUnion.timerCondition->comparison == TimerCondition::EQUAL_TO)
			viewableText += " == ";
		if (conditionUnion.timerCondition->comparison == TimerCondition::NOT_EQUAL_TO)
			viewableText += " != ";
		if (conditionUnion.timerCondition->comparison == TimerCondition::GREATER_THAN)
			viewableText += " > ";
		if (conditionUnion.timerCondition->comparison == TimerCondition::GREATER_THAN_OR_EQUAL_TO)
			viewableText += " >= ";
		if (conditionUnion.timerCondition->comparison == TimerCondition::LESS_THAN)
			viewableText += " < ";
		if (conditionUnion.timerCondition->comparison == TimerCondition::LESS_THAN_OR_EQUAL_TO)
			viewableText += " <= ";
		if (conditionUnion.timerCondition->timeStoredInVariable)
			viewableText += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(conditionUnion.timerCondition->seconds));
		else
			viewableText += QString("%1:%2").arg(conditionUnion.timerCondition->minutes).arg(conditionUnion.timerCondition->seconds);
	}
	else if (type == MONEY)
	{
		viewableText += "Money: ";
		if (conditionUnion.variableCondition->comparison == VariableCondition::EQUAL_TO)
			viewableText += "== ";
		if (conditionUnion.variableCondition->comparison == VariableCondition::NOT_EQUAL_TO)
			viewableText += "!= ";
		if (conditionUnion.variableCondition->comparison == VariableCondition::GREATER_THAN)
			viewableText += "> ";
		if (conditionUnion.variableCondition->comparison == VariableCondition::GREATER_THAN_OR_EQUAL_TO)
			viewableText += ">= ";
		if (conditionUnion.variableCondition->comparison == VariableCondition::LESS_THAN)
			viewableText += "< ";
		if (conditionUnion.variableCondition->comparison == VariableCondition::LESS_THAN_OR_EQUAL_TO)
			viewableText += "<= ";
		if (conditionUnion.moneyCondition->valueStoredInVariable)
			viewableText += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(conditionUnion.moneyCondition->value));
		else
			viewableText += QString::number(conditionUnion.moneyCondition->value);
	}
	else if (type == ITEM)
	{
		viewableText += "Item: ";
		if (conditionUnion.itemCondition->partyPossesses)
			viewableText += "Party Possesses ";
		else
			viewableText += "Party Doesn't Possess ";
		if (conditionUnion.itemCondition->itemReferencedInVariable)
			viewableText += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(conditionUnion.itemCondition->itemID));
		else
			viewableText += ProjectData::itemNames[conditionUnion.itemCondition->itemID];
	}
	else if (type == HERO)
	{
		viewableText += "Hero: ";
		if (conditionUnion.heroCondition->characterReferencedInVariable)
			viewableText += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(conditionUnion.heroCondition->characterID));
		else
			viewableText += ProjectData::characterNames[conditionUnion.heroCondition->characterID];
		if (conditionUnion.heroCondition->conditionType == HeroCondition::IN_PARTY)
			viewableText += " Is In Party";
		else if (conditionUnion.heroCondition->conditionType == HeroCondition::NAME_IS)
		{
			viewableText += "\'s Name Is ";
			if (conditionUnion.heroCondition->valueStoredInVariable)
				viewableText += QString("string[%1]").arg(ProjectData::stringVariableIdToString(conditionUnion.heroCondition->value));
			else
				viewableText += conditionUnion.heroCondition->string;
		}
		else if (conditionUnion.heroCondition->conditionType == HeroCondition::LEVEL_IS_AT_LEAST)
		{
			viewableText += "\'s Level Is At Least ";
			viewableText += QString::number(conditionUnion.heroCondition->value);
		}
		else if (conditionUnion.heroCondition->conditionType == HeroCondition::HP_IS_AT_LEAST)
		{
			viewableText += "\'s HP Is At Least ";
			viewableText += QString::number(conditionUnion.heroCondition->value);
		}
		else if (conditionUnion.heroCondition->conditionType == HeroCondition::CAN_USE_SKILL)
		{
			viewableText += " Can Use ";
			viewableText += ProjectData::skillNames[conditionUnion.heroCondition->value];
		}
		else if (conditionUnion.heroCondition->conditionType == HeroCondition::HAS_ITEM_EQUIPPED)
		{
			viewableText += " Has ";
			viewableText += ProjectData::itemNames[conditionUnion.heroCondition->value];
			viewableText += " Equipped";
		}
		else if (conditionUnion.heroCondition->conditionType == HeroCondition::AFFLICTED_WITH_CONDITION)
		{
			viewableText += " Afflicted With ";
			viewableText += ProjectData::attributeNames[conditionUnion.heroCondition->value];
		}
	}
	else if (type == SPRITE)
	{
		viewableText += "Sprite: ";
		if (conditionUnion.spriteCondition->spriteType == SpriteCondition::HERO)
			viewableText += "Hero";
		else if (conditionUnion.spriteCondition->spriteType == SpriteCondition::VEHICLE)
		{
			viewableText += "Vehicle=";
			if (conditionUnion.spriteCondition->spriteReferencedInVariable)
				viewableText += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(conditionUnion.spriteCondition->spriteID));
			else
				viewableText += ProjectData::vehicleNames[conditionUnion.spriteCondition->spriteID];
		}
		else if (conditionUnion.spriteCondition->spriteType == SpriteCondition::EVENT)
		{
			viewableText += "Event=";
			if (conditionUnion.spriteCondition->spriteReferencedInVariable)
				viewableText += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(conditionUnion.spriteCondition->spriteID));
			else if (conditionUnion.spriteCondition->spriteID == -1)
				viewableText += "this";
			else
				viewableText += QString::number(conditionUnion.spriteCondition->spriteID);
		}
		viewableText += " Is Facing ";
		if (conditionUnion.spriteCondition->direction == MapEvent::DIRECTION_UP)
			viewableText += "Up";
		else if (conditionUnion.spriteCondition->direction == MapEvent::DIRECTION_RIGHT)
			viewableText += "Right";
		else if (conditionUnion.spriteCondition->direction == MapEvent::DIRECTION_DOWN)
			viewableText += "Down";
		else if (conditionUnion.spriteCondition->direction == MapEvent::DIRECTION_LEFT)
			viewableText += "Left";
	}
	else if (type == PARTY_IS_RIDING)
	{
		viewableText += "Party Is Riding: ";
		if (conditionUnion.partyIsRidingCondition->vehicleReferencedInVariable)
			viewableText += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(conditionUnion.partyIsRidingCondition->vehicleID));
		else
			viewableText += ProjectData::vehicleNames[conditionUnion.partyIsRidingCondition->vehicleID];
	}
	else if (type == EVENT_STARTED_WITH_ACTION_KEY)
		viewableText += "Event Started With Action Key";
	else if (type == BGM_PLAYED_THROUGH_ONCE)
		viewableText += "BGM Has Played Through Once";
	return viewableText;
}

void BaseEvent::ConditionalBranch_Map::Condition::setType(int value)
{
	if (type == SWITCH)
		delete (conditionUnion.switchCondition);
	else if (type == VARIABLE)
		delete (conditionUnion.variableCondition);
	else if (type == STRING)
		delete (conditionUnion.stringCondition);
	else if (type == TIMER)
		delete (conditionUnion.timerCondition);
	else if (type == MONEY)
		delete (conditionUnion.moneyCondition);
	else if (type == ITEM)
		delete (conditionUnion.itemCondition);
	else if (type == HERO)
		delete (conditionUnion.heroCondition);
	else if (type == SPRITE)
		delete (conditionUnion.spriteCondition);
	else if (type == PARTY_IS_RIDING)
		delete (conditionUnion.partyIsRidingCondition);
	type = value;
	if (type == SWITCH)
		conditionUnion.switchCondition = new SwitchCondition;
	else if (type == VARIABLE)
		conditionUnion.variableCondition = new VariableCondition;
	else if (type == STRING)
		conditionUnion.stringCondition = new StringCondition;
	else if (type == TIMER)
		conditionUnion.timerCondition = new TimerCondition;
	else if (type == MONEY)
		conditionUnion.moneyCondition = new MoneyCondition;
	else if (type == ITEM)
		conditionUnion.itemCondition = new ItemCondition;
	else if (type == HERO)
		conditionUnion.heroCondition = new HeroCondition;
	else if (type == SPRITE)
		conditionUnion.spriteCondition = new SpriteCondition;
	else if (type == PARTY_IS_RIDING)
		conditionUnion.partyIsRidingCondition = new PartyIsRidingCondition;
}

void BaseEvent::ConditionalBranch_Map::Condition::getSwitchCondition(SwitchCondition *other)
{
	other->switchID = conditionUnion.switchCondition->switchID;
	other->switchReferencedInVariable = conditionUnion.switchCondition->switchReferencedInVariable;
	other->isOn = conditionUnion.switchCondition->isOn;
}

void BaseEvent::ConditionalBranch_Map::Condition::setSwitchCondition(SwitchCondition *other)
{
	conditionUnion.switchCondition->switchID = other->switchID;
	conditionUnion.switchCondition->switchReferencedInVariable = other->switchReferencedInVariable;
	conditionUnion.switchCondition->isOn = other->isOn;
}

void BaseEvent::ConditionalBranch_Map::Condition::getVariableCondition(VariableCondition *other)
{
	other->srcVariableID = conditionUnion.variableCondition->srcVariableID;
	other->dstValue = conditionUnion.variableCondition->dstValue;
	other->comparison = conditionUnion.variableCondition->comparison;
	other->dstValueStoredInVariable = conditionUnion.variableCondition->dstValueStoredInVariable;
}

void BaseEvent::ConditionalBranch_Map::Condition::setVariableCondition(VariableCondition *other)
{
	conditionUnion.variableCondition->srcVariableID = other->srcVariableID;
	conditionUnion.variableCondition->dstValue = other->dstValue;
	conditionUnion.variableCondition->comparison = other->comparison;
	conditionUnion.variableCondition->dstValueStoredInVariable = other->dstValueStoredInVariable;
}

void BaseEvent::ConditionalBranch_Map::Condition::getStringCondition(StringCondition *other)
{
	other->dstValue = conditionUnion.stringCondition->dstValue;
	other->srcStringID = conditionUnion.stringCondition->srcStringID;
	other->dstStringID = conditionUnion.stringCondition->dstStringID;
	other->comparison = conditionUnion.stringCondition->comparison;
	other->dstStringType = conditionUnion.stringCondition->dstStringType;
	other->srcStringReferencedInVariable = conditionUnion.stringCondition->srcStringReferencedInVariable;
}

void BaseEvent::ConditionalBranch_Map::Condition::setStringCondition(StringCondition *other)
{
	conditionUnion.stringCondition->dstValue = other->dstValue;
	conditionUnion.stringCondition->srcStringID = other->srcStringID;
	conditionUnion.stringCondition->dstStringID = other->dstStringID;
	conditionUnion.stringCondition->comparison = other->comparison;
	conditionUnion.stringCondition->dstStringType = other->dstStringType;
	conditionUnion.stringCondition->srcStringReferencedInVariable = other->srcStringReferencedInVariable;
}

void BaseEvent::ConditionalBranch_Map::Condition::getTimerCondition(TimerCondition *other)
{
	other->timerID = conditionUnion.timerCondition->timerID;
	other->minutes = conditionUnion.timerCondition->minutes;
	other->seconds = conditionUnion.timerCondition->seconds;
	other->comparison = conditionUnion.timerCondition->comparison;
	other->timerReferencedInVariable = conditionUnion.timerCondition->timerReferencedInVariable;
	other->timeStoredInVariable = conditionUnion.timerCondition->timeStoredInVariable;
}

void BaseEvent::ConditionalBranch_Map::Condition::setTimerCondition(TimerCondition *other)
{
	conditionUnion.timerCondition->timerID = other->timerID;
	conditionUnion.timerCondition->minutes = other->minutes;
	conditionUnion.timerCondition->seconds = other->seconds;
	conditionUnion.timerCondition->comparison = other->comparison;
	conditionUnion.timerCondition->timerReferencedInVariable = other->timerReferencedInVariable;
	conditionUnion.timerCondition->timeStoredInVariable = other->timeStoredInVariable;
}

void BaseEvent::ConditionalBranch_Map::Condition::getMoneyCondition(MoneyCondition *other)
{
	other->value = conditionUnion.moneyCondition->value;
	other->comparison = conditionUnion.moneyCondition->comparison;
	other->valueStoredInVariable = conditionUnion.moneyCondition->valueStoredInVariable;
}

void BaseEvent::ConditionalBranch_Map::Condition::setMoneyCondition(MoneyCondition *other)
{
	conditionUnion.moneyCondition->value = other->value;
	conditionUnion.moneyCondition->comparison = other->comparison;
	conditionUnion.moneyCondition->valueStoredInVariable = other->valueStoredInVariable;
}

void BaseEvent::ConditionalBranch_Map::Condition::getItemCondition(ItemCondition *other)
{
	other->itemID = conditionUnion.itemCondition->itemID;
	other->itemReferencedInVariable = conditionUnion.itemCondition->itemReferencedInVariable;
	other->partyPossesses = conditionUnion.itemCondition->partyPossesses;
}

void BaseEvent::ConditionalBranch_Map::Condition::setItemCondition(ItemCondition *other)
{
	conditionUnion.itemCondition->itemID = other->itemID;
	conditionUnion.itemCondition->itemReferencedInVariable = other->itemReferencedInVariable;
	conditionUnion.itemCondition->partyPossesses = other->partyPossesses;
}

void BaseEvent::ConditionalBranch_Map::Condition::getHeroCondition(HeroCondition *other)
{
	other->string = conditionUnion.heroCondition->string;
	other->value = conditionUnion.heroCondition->value;
	other->characterID = conditionUnion.heroCondition->characterID;
	other->conditionType = conditionUnion.heroCondition->conditionType;
	other->characterReferencedInVariable = conditionUnion.heroCondition->characterReferencedInVariable;
	other->valueStoredInVariable = conditionUnion.heroCondition->valueStoredInVariable;
}

void BaseEvent::ConditionalBranch_Map::Condition::setHeroCondition(HeroCondition *other)
{
	conditionUnion.heroCondition->string = other->string;
	conditionUnion.heroCondition->value = other->value;
	conditionUnion.heroCondition->characterID = other->characterID;
	conditionUnion.heroCondition->conditionType = other->conditionType;
	conditionUnion.heroCondition->characterReferencedInVariable = other->characterReferencedInVariable;
	conditionUnion.heroCondition->valueStoredInVariable = other->valueStoredInVariable;
}

void BaseEvent::ConditionalBranch_Map::Condition::getSpriteCondition(SpriteCondition *other)
{
	other->spriteType = conditionUnion.spriteCondition->spriteType;
	other->spriteID = conditionUnion.spriteCondition->spriteID;
	other->direction = conditionUnion.spriteCondition->direction;
	other->spriteReferencedInVariable = conditionUnion.spriteCondition->spriteReferencedInVariable;
}

void BaseEvent::ConditionalBranch_Map::Condition::setSpriteCondition(SpriteCondition *other)
{
	conditionUnion.spriteCondition->spriteType = other->spriteType;
	conditionUnion.spriteCondition->spriteID = other->spriteID;
	conditionUnion.spriteCondition->direction = other->direction;
	conditionUnion.spriteCondition->spriteReferencedInVariable = other->spriteReferencedInVariable;
}

void BaseEvent::ConditionalBranch_Map::Condition::getPartyIsRidingCondition(PartyIsRidingCondition *other)
{
	other->vehicleID = conditionUnion.partyIsRidingCondition->vehicleID;
	other->vehicleReferencedInVariable = conditionUnion.partyIsRidingCondition->vehicleReferencedInVariable;
}

void BaseEvent::ConditionalBranch_Map::Condition::setPartyIsRidingCondition(PartyIsRidingCondition *other)
{
	conditionUnion.partyIsRidingCondition->vehicleID = other->vehicleID;
	conditionUnion.partyIsRidingCondition->vehicleReferencedInVariable = other->vehicleReferencedInVariable;
}

bool BaseEvent::ConditionalBranch_Map::Condition::tryLoad(XMLNode ConditionNode)
{
	QStringList tempStringList;
	QStringList trueFalseList = QStringList() << "true" << "false";
	QString tempString;
	int conditionType;
	tempStringList.clear();
	tempStringList << "switch" << "variable" << "string" << "timer" << "money" << "item" << "hero" << "partyIsRiding" << "previousSelectedArea" << "bgmPlayedThroughOnce";
	if (!XMLTools::attributeExists(ConditionNode, "type"))
		return false;
	if (!XMLTools::attributeStringValid(ConditionNode, "type", tempStringList))
		return false;
	conditionType = tempStringList.indexOf(QString(ConditionNode.getAttribute("type")));
	if (conditionType == SWITCH)
	{
		if (!XMLTools::attributeExists(ConditionNode, "switchID"))
			return false;
		if (!XMLTools::attributeExists(ConditionNode, "switchReferencedInVariable"))
			return false;
		if (!XMLTools::attributeStringValid(ConditionNode, "switchReferencedInVariable", trueFalseList, 1))
			return false;
		if (!XMLTools::attributeExists(ConditionNode, "isOn"))
			return false;
		if (!XMLTools::attributeStringValid(ConditionNode, "isOn", trueFalseList, 1))
			return false;
	}
	else if (conditionType == VARIABLE)
	{
		if (!XMLTools::attributeExists(ConditionNode, "comparison"))
			return false;
		tempStringList.clear();
		tempStringList << "==" << "!=" << ">" << ">=" << "<" << "<=";
		if (!XMLTools::attributeStringValid(ConditionNode, "comparison", tempStringList))
			return false;
		if (!XMLTools::attributeExists(ConditionNode, "srcVariableID"))
			return false;
		if (!XMLTools::attributeExists(ConditionNode, "dstValue"))
			return false;
		if (!XMLTools::attributeExists(ConditionNode, "dstValueStoredInVariable"))
			return false;
		if (!XMLTools::attributeStringValid(ConditionNode, "dstValueStoredInVariable", trueFalseList, 1))
			return false;
	}
	else if (conditionType == STRING)
	{
		if (!XMLTools::attributeExists(ConditionNode, "comparison"))
			return false;
		tempStringList.clear();
		tempStringList << "==" << "!=" << "startsWith" << "endsWith" << "contains" << "doesNotContain" << "isEmpty" << "isNotEmpty";
		if (!XMLTools::attributeStringValid(ConditionNode, "comparison", tempStringList))
			return false;
		if (!XMLTools::attributeExists(ConditionNode, "srcStringID"))
			return false;
		if (!XMLTools::attributeExists(ConditionNode, "srcStringReferencedInVariable"))
			return false;
		if (!XMLTools::attributeStringValid(ConditionNode, "srcStringReferencedInVariable", trueFalseList, 1))
			return false;
		if (!XMLTools::attributeExists(ConditionNode, "dstStringType"))
			return false;
		tempStringList.clear();
		tempStringList << "stringVariable" << "referencedString" << "value";
		if (!XMLTools::attributeStringValid(ConditionNode, "dstStringType", tempStringList))
			return false;
		tempString = ConditionNode.getAttribute("dstStringType");
		if (tempString == "stringVariable")
		{
			if (!XMLTools::attributeExists(ConditionNode, "dstStringID"))
				return false;
		}
		else if (tempString == "referencedString")
		{
			if (!XMLTools::attributeExists(ConditionNode, "dstVariableID"))
				return false;
		}
		else if (tempString == "value")
		{
			if (!XMLTools::attributeExists(ConditionNode, "dstValue"))
				return false;
		}
	}
	else if (conditionType == TIMER)
	{
		if (!XMLTools::attributeExists(ConditionNode, "comparison"))
			return false;
		tempStringList.clear();
		tempStringList << "==" << "!=" << ">" << ">=" << "<" << "<=";
		if (!XMLTools::attributeStringValid(ConditionNode, "comparison", tempStringList))
			return false;
		if (!XMLTools::attributeExists(ConditionNode, "timerID"))
			return false;
		if (!XMLTools::attributeExists(ConditionNode, "timerReferencedInVariable"))
			return false;
		if (!XMLTools::attributeStringValid(ConditionNode, "timerReferencedInVariable", trueFalseList, 1))
			return false;
		if (!XMLTools::attributeExists(ConditionNode, "timeStoredInVariable"))
			return false;
		if (!XMLTools::attributeStringValid(ConditionNode, "timeStoredInVariable", trueFalseList, 1))
			return false;
		if (QString(ConditionNode.getAttribute("timeStoredInVariable")) == "true")
		{
			if (!XMLTools::attributeExists(ConditionNode, "variableID"))
				return false;
		}
		else
		{
			if (!XMLTools::attributeExists(ConditionNode, "minutes"))
				return false;
			if (!XMLTools::attributeExists(ConditionNode, "seconds"))
				return false;
			if (!XMLTools::attributeIntRangeValid(ConditionNode, "seconds", 0, 59))
				return false;
		}
	}
	else if (conditionType == MONEY)
	{
		if (!XMLTools::attributeExists(ConditionNode, "comparison"))
			return false;
		tempStringList.clear();
		tempStringList << "==" << "!=" << ">" << ">=" << "<" << "<=";
		if (!XMLTools::attributeStringValid(ConditionNode, "comparison", tempStringList))
			return false;
		if (!XMLTools::attributeExists(ConditionNode, "value"))
			return false;
		if (!XMLTools::attributeExists(ConditionNode, "valueStoredInVariable"))
			return false;
		if (!XMLTools::attributeStringValid(ConditionNode, "valueStoredInVariable", trueFalseList, 1))
			return false;
	}
	else if (conditionType == ITEM)
	{
		if (!XMLTools::attributeExists(ConditionNode, "itemID"))
			return false;
		if (!XMLTools::attributeExists(ConditionNode, "itemReferencedInVariable"))
			return false;
		if (!XMLTools::attributeStringValid(ConditionNode, "itemReferencedInVariable", trueFalseList, 1))
			return false;
		if (!XMLTools::attributeExists(ConditionNode, "partyPossesses"))
			return false;
		if (!XMLTools::attributeStringValid(ConditionNode, "partyPossesses", trueFalseList, 1))
			return false;
	}
	else if (conditionType == HERO)
	{
		int heroConditionType;
		if (!XMLTools::attributeExists(ConditionNode, "heroConditionType"))
			return false;
		tempStringList.clear();
		tempStringList << "inParty" << "nameIs" << "levelIsAtLeast" << "hpIsAtLeast" << "canUseSkill" << "hasItemEquipped" << "afflictedWithCondition";
		if (!XMLTools::attributeStringValid(ConditionNode, "heroConditionType", tempStringList))
			return false;
		if (!XMLTools::attributeExists(ConditionNode, "characterID"))
			return false;
		if (!XMLTools::attributeExists(ConditionNode, "characterReferencedInVariable"))
			return false;
		if (!XMLTools::attributeStringValid(ConditionNode, "characterReferencedInVariable", trueFalseList, 1))
			return false;
		heroConditionType = tempStringList.indexOf(QString(ConditionNode.getAttribute("heroConditionType")));
		if (heroConditionType == HeroCondition::NAME_IS)
		{
			if (!XMLTools::attributeExists(ConditionNode, "string"))
				return false;
			if (!XMLTools::attributeExists(ConditionNode, "stringStoredInVariable"))
				return false;
			if (!XMLTools::attributeStringValid(ConditionNode, "stringStoredInVariable", trueFalseList, 1))
				return false;
		}
		else if (heroConditionType == HeroCondition::LEVEL_IS_AT_LEAST)
		{
			if (!XMLTools::attributeExists(ConditionNode, "level"))
				return false;
			if (!XMLTools::attributeExists(ConditionNode, "levelStoredInVariable"))
				return false;
			if (!XMLTools::attributeStringValid(ConditionNode, "levelStoredInVariable", trueFalseList, 1))
				return false;
			if (QString(ConditionNode.getAttribute("levelStoredInVariable")) != "true")
			{
				if (!XMLTools::attributeIntRangeValid(ConditionNode, "level", 1, 99))
					return false;
			}
		}
		else if (heroConditionType == HeroCondition::HP_IS_AT_LEAST)
		{
			if (!XMLTools::attributeExists(ConditionNode, "hp"))
				return false;
			if (!XMLTools::attributeExists(ConditionNode, "hpStoredInVariable"))
				return false;
			if (!XMLTools::attributeStringValid(ConditionNode, "hpStoredInVariable", trueFalseList, 1))
				return false;
			if (QString(ConditionNode.getAttribute("hpStoredInVariable")) != "true")
			{
				if (!XMLTools::attributeIntRangeValid(ConditionNode, "hp", 1, 9999))
					return false;
			}
		}
		else if (heroConditionType == HeroCondition::CAN_USE_SKILL)
		{
			if (!XMLTools::attributeExists(ConditionNode, "skillID"))
				return false;
			if (!XMLTools::attributeExists(ConditionNode, "skillReferencedInVariable"))
				return false;
			if (!XMLTools::attributeStringValid(ConditionNode, "skillReferencedInVariable", trueFalseList, 1))
				return false;
		}
		else if (heroConditionType == HeroCondition::HAS_ITEM_EQUIPPED)
		{
			if (!XMLTools::attributeExists(ConditionNode, "itemID"))
				return false;
			if (!XMLTools::attributeExists(ConditionNode, "itemReferencedInVariable"))
				return false;
			if (!XMLTools::attributeStringValid(ConditionNode, "itemReferencedInVariable", trueFalseList, 1))
				return false;
		}
		else if (heroConditionType == HeroCondition::AFFLICTED_WITH_CONDITION)
		{
			if (!XMLTools::attributeExists(ConditionNode, "conditionID"))
				return false;
			if (!XMLTools::attributeExists(ConditionNode, "conditionReferencedInVariable"))
				return false;
			if (!XMLTools::attributeStringValid(ConditionNode, "conditionReferencedInVariable", trueFalseList, 1))
				return false;
		}
	}
	else if (conditionType == SPRITE)
	{
		tempStringList.clear();
		tempStringList << "hero" << "vehicle" << "event";
		if (!XMLTools::attributeExists(ConditionNode, "spriteType"))
			return false;
		if (!XMLTools::attributeStringValid(ConditionNode, "spriteType", tempStringList))
			return false;
		tempString = ConditionNode.getAttribute("spriteType");
		if (tempString == "vehicle")
		{
			if (!XMLTools::attributeExists(ConditionNode, "vehicleID"))
				return false;
			if (!XMLTools::attributeExists(ConditionNode, "vehicleReferencedInVariable"))
				return false;
			if (!XMLTools::attributeStringValid(ConditionNode, "vehicleReferencedInVariable", trueFalseList, 1))
				return false;
		}
		else if (tempString == "event")
		{
			if (!XMLTools::attributeExists(ConditionNode, "eventID"))
				return false;
			if (!XMLTools::attributeExists(ConditionNode, "eventReferencedInVariable"))
				return false;
			if (!XMLTools::attributeStringValid(ConditionNode, "eventReferencedInVariable", trueFalseList, 1))
				return false;
		}
		tempStringList.clear();
		tempStringList << "up" << "right" << "down" << "left";
		if (!XMLTools::attributeExists(ConditionNode, "isFacing"))
			return false;
		if (!XMLTools::attributeStringValid(ConditionNode, "isFacing", tempStringList))
			return false;
	}
	else if (conditionType == PARTY_IS_RIDING)
	{
		if (!XMLTools::attributeExists(ConditionNode, "vehicleID"))
			return false;
		if (!XMLTools::attributeExists(ConditionNode, "vehicleReferencedInVariable"))
			return false;
		if (!XMLTools::attributeStringValid(ConditionNode, "vehicleReferencedInVariable", trueFalseList, 1))
			return false;
	}
	return true;
}
//END BaseEvent::ConditionalBranch_Map::Condition Member Functions

//BEGIN BaseEvent::ConditionalBranch_Menu Member Functions
BaseEvent::ConditionalBranch_Menu::~ConditionalBranch_Menu()
{
	while (conditions.size() > 0)
	{
		delete (conditions.last());
		conditions.removeLast();
	}
	while (onTrueEvents.size() > 0)
	{
		delete (onTrueEvents.last());
		onTrueEvents.removeLast();
	}
	while (onFalseEvents.size() > 0)
	{
		delete (onFalseEvents.last());
		onFalseEvents.removeLast();
	}
}

XMLNode BaseEvent::ConditionalBranch_Menu::getEventNode()
{
	XMLNode TempNode1, EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	TempNode1 = EventNode.addChild("conditions");
	if (conditionsToMatch == ALL)
		TempNode1.addAttribute("conditionsToMatch", "all");
	if (conditionsToMatch == AT_LEAST)
	{
		TempNode1.addAttribute("conditionsToMatch", "atLeast");
		TempNode1.addAttribute("numConditionsToMatch", QString::number(numConditionsToMatch).toUtf8().data());
	}
	else if (conditionsToMatch == NONE)
		TempNode1.addAttribute("conditionsToMatch", "none");
	TempNode1.addAttribute("executeCustomHandlerIfConditionsNotMet", (executeCustomHandlerIfConditionsNotMet) ? "true":"false");
	for (int i = 0; i < conditions.size(); ++i)
		TempNode1.addChild(conditions[i]->getConditionNode());
	TempNode1 = EventNode.addChild("onTrueEvents");
	for (int i = 0; i < onTrueEvents.size(); ++i)
		TempNode1.addChild(onTrueEvents[i]->getEventNode());
	if (executeCustomHandlerIfConditionsNotMet)
	{
		TempNode1 = EventNode.addChild("onFalseEvents");
		for (int i = 0; i < onFalseEvents.size(); ++i)
			TempNode1.addChild(onFalseEvents[i]->getEventNode());
	}
	return EventNode;
}

void BaseEvent::ConditionalBranch_Menu::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(conditionsToMatch);
	storageFile.putSignedInt(numConditionsToMatch);
	storageFile.putBool(executeCustomHandlerIfConditionsNotMet);
	storageFile.putSignedInt(conditions.size());
	for (int i = 0; i < conditions.size(); ++i)
		conditions[i]->saveToStorageFile(storageFile);
	storageFile.putSignedInt(onTrueEvents.size());
	for (int i = 0; i < onTrueEvents.size(); ++i)
		onTrueEvents[i]->saveToStorageFile(storageFile);
	if (executeCustomHandlerIfConditionsNotMet)
	{
		storageFile.putSignedInt(onFalseEvents.size());
		for (int i = 0; i < onFalseEvents.size(); ++i)
			onFalseEvents[i]->saveToStorageFile(storageFile);
	}
}

QStringList BaseEvent::ConditionalBranch_Menu::getResources()
{
	QStringList resources;
	for (int i = 0; i < onTrueEvents.size(); ++i)
		resources.append(onTrueEvents[i]->getResources());
	if (executeCustomHandlerIfConditionsNotMet)
	{
		for (int i = 0; i < onFalseEvents.size(); ++i)
			resources.append(onFalseEvents[i]->getResources());
	}
	return resources;
}

QStringList BaseEvent::ConditionalBranch_Menu::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Conditional Branch: If ";
	if (conditionsToMatch == ALL)
		viewableText[0] += "All Conditions Are Met";
	else if (conditionsToMatch == AT_LEAST)
		viewableText[0] += QString("At Least %1 Conditions Are Met").arg(numConditionsToMatch);
	else
		viewableText[0] += "No Conditions Are Met";
	viewableText.append("");
	for (int i = 0; i < indent; ++i)
		viewableText[1].append("  ");
	viewableText[1] += "::Conditions";
	for (int i = 0; i < conditions.size(); ++i)
		viewableText.append(conditions[i]->getViewableText(indent + 1));
	viewableText.append("");
	for (int i = 0; i < indent; ++i)
		viewableText[viewableText.size() - 1] += "  ";
	viewableText[viewableText.size() - 1] += "::True";
	for (int i = 0; i < onTrueEvents.size(); ++i)
		viewableText += onTrueEvents[i]->getViewableText(indent + 1);
	viewableText.append("");
	for (int i = 0; i < indent + 1; ++i)
		viewableText[viewableText.size() - 1] += "  ";
	viewableText[viewableText.size() - 1] += "<>";
	if (executeCustomHandlerIfConditionsNotMet)
	{
		viewableText.append("");
		for (int i = 0; i < indent; ++i)
			viewableText[viewableText.size() - 1] += "  ";
		viewableText[viewableText.size() - 1] += "::False";
		for (int i = 0; i < onFalseEvents.size(); ++i)
			viewableText += onFalseEvents[i]->getViewableText(indent + 1);
		viewableText.append("");
		for (int i = 0; i < indent + 1; ++i)
			viewableText[viewableText.size() - 1] += "  ";
		viewableText[viewableText.size() - 1] += "<>";
	}
	viewableText.append("");
	for (int i = 0; i < indent; ++i)
		viewableText[viewableText.size() - 1] += "  ";
	viewableText[viewableText.size() - 1] += "::End";
	return viewableText;
}

QList<BaseEvent*> BaseEvent::ConditionalBranch_Menu::getEventPointers()
{
	QList<BaseEvent*> pointers;
	for (int i = 0; i < onTrueEvents.size(); ++i)
		pointers += onTrueEvents[i]->getEventPointers();
	pointers.append(NULL);
	if (executeCustomHandlerIfConditionsNotMet)
	{
		for (int i = 0; i < onFalseEvents.size(); ++i)
			pointers += onFalseEvents[i]->getEventPointers();
		pointers.append(NULL);
	}
	return pointers;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::ConditionalBranch_Menu::configureEvent(int callerID, int callerType)
{
	ConfigureEvent_MenuConditionalBranch *configureConditionalBranch = new ConfigureEvent_MenuConditionalBranch(callerID, callerType);
	bool configured = false;
	configureConditionalBranch->setConditionsToMatch(conditionsToMatch);
	configureConditionalBranch->setNumConditionsToMatch(numConditionsToMatch);
	configureConditionalBranch->setExecuteCustomHandler(executeCustomHandlerIfConditionsNotMet);
	configureConditionalBranch->addConditions(conditions);
	if (configureConditionalBranch->exec())
	{
		QList<Condition*> newConditions;
		configured = true;
		for (int i = 0; i < conditions.size(); ++i)
			delete (conditions[i]);
		conditions.clear();
		conditionsToMatch = configureConditionalBranch->getConditionsToMatch();
		numConditionsToMatch = configureConditionalBranch->getNumConditionsToMatch();
		executeCustomHandlerIfConditionsNotMet = configureConditionalBranch->executeCustomHandler();
		newConditions = configureConditionalBranch->getConditions();
		for (int i = 0; i < newConditions.size(); ++i)
			conditions.append(new Condition(newConditions[i]));
	}
	else
		configured = false;
	delete configureConditionalBranch;
	return configured;
}
#endif

void BaseEvent::ConditionalBranch_Menu::addCondition(Condition *condition)
{
	conditions.append(condition);
}

void BaseEvent::ConditionalBranch_Menu::removeCondition(int location)
{
	delete (conditions[location]);
	conditions.removeAt(location);
}

void BaseEvent::ConditionalBranch_Menu::addOnTrueEvent(BaseEvent *event, int before)
{
	if (before == -1)
		onTrueEvents.append(event);
	else
		onTrueEvents.insert(before, event);
}

void BaseEvent::ConditionalBranch_Menu::addOnFalseEvent(BaseEvent *event, int before)
{
	if (before == -1)
		onFalseEvents.append(event);
	else
		onFalseEvents.insert(before, event);
}

void BaseEvent::ConditionalBranch_Menu::deleteOnTrueEvent(int location)
{
	delete (onTrueEvents[location]);
	onTrueEvents.removeAt(location);
}

void BaseEvent::ConditionalBranch_Menu::deleteOnFalseEvent(int location)
{
	delete (onFalseEvents[location]);
	onFalseEvents.removeAt(location);
}

void BaseEvent::ConditionalBranch_Menu::addChildEvent(BaseEvent *event, int location, int childTypeID)
{
	if (childTypeID == 1)
		addOnTrueEvent(event, location);
	else if (childTypeID == 2)
		addOnFalseEvent(event, location);
}

void BaseEvent::ConditionalBranch_Menu::deleteChildEvent(int location, int childTypeID)
{
	if (childTypeID == 1)
		deleteOnTrueEvent(location);
	else if (childTypeID == 2)
		deleteOnFalseEvent(location);
}

void BaseEvent::ConditionalBranch_Menu::applySwappedTiles(QList<Tileset::SwapTileIDs> swappedTiles)
{
	for (int i = 0; i < onTrueEvents.size(); ++i)
		onTrueEvents[i]->applySwappedTiles(swappedTiles);
	for (int i = 0; i < onFalseEvents.size(); ++i)
		onFalseEvents[i]->applySwappedTiles(swappedTiles);
}

void BaseEvent::ConditionalBranch_Menu::applyChangeMenuObjectName(QString oldName, QString newName)
{
	for (int i = 0; i < conditions.size(); ++i)
		conditions[i]->applyChangeMenuObjectName(oldName, newName);
	for (int i = 0; i < onTrueEvents.size(); ++i)
		onTrueEvents[i]->applyChangeMenuObjectName(oldName, newName);
	for (int i = 0; i < onFalseEvents.size(); ++i)
		onFalseEvents[i]->applyChangeMenuObjectName(oldName, newName);
}

bool BaseEvent::ConditionalBranch_Menu::tryLoad(XMLNode EventNode, int baseType)
{
	XMLNode TempNode1, TempNode2;
	QStringList tempStringList;
	int count;
	bool executeCustomHandler;
	if (!XMLTools::nodeExists(EventNode, "conditions"))
		return false;
	TempNode1 = EventNode.getChildNode("conditions");
	if (!XMLTools::attributeExists(TempNode1, "conditionsToMatch"))
		return false;
	tempStringList.clear();
	tempStringList << "all" << "atLeast" << "none";
	if (!XMLTools::attributeStringValid(TempNode1, "conditionsToMatch", tempStringList))
		return false;
	if (QString(TempNode1.getAttribute("conditionsToMatch")) == "atLeast")
	{
		if (!XMLTools::attributeExists(TempNode1, "numConditionsToMatch"))
			return false;
	}
	tempStringList.clear();
	tempStringList << "true" << "false";
	if (!XMLTools::attributeExists(TempNode1, "executeCustomHandlerIfConditionsNotMet"))
		return false;
	if (!XMLTools::attributeStringValid(TempNode1, "executeCustomHandlerIfConditionsNotMet", tempStringList, 1))
		return false;
	executeCustomHandler = (QString(TempNode1.getAttribute("executeCustomHandlerIfConditionsNotMet")) == "true") ? true:false;
	count = TempNode1.nChildNode("condition");
	for (int i = 0; i < count; ++i)
	{
		TempNode2 = TempNode1.getChildNode("condition", i);
		if (!Condition::tryLoad(TempNode2))
			return false;
	}
	if (!XMLTools::nodeExists(EventNode, "onTrueEvents"))
		return false;
	TempNode1 = EventNode.getChildNode("onTrueEvents");
	count = TempNode1.nChildNode("event");
	for (int i = 0; i < count; ++i)
	{
		if (!BaseEvent::tryLoad(TempNode1.getChildNode("event", i), baseType))
			return false;
	}
	if (executeCustomHandler)
	{
		if (!XMLTools::nodeExists(EventNode, "onFalseEvents"))
			return false;
		TempNode1 = EventNode.getChildNode("onFalseEvents");
		count = TempNode1.nChildNode("event");
		for (int i = 0; i < count; ++i)
		{
			if (!BaseEvent::tryLoad(TempNode1.getChildNode("event", i), baseType))
				return false;
		}
	}
	return true;
}

BaseEvent::ConditionalBranch_Menu::ConditionalBranch_Menu(BaseEvent *pBase) : Event(pBase)
{
	conditionsToMatch = ALL;
	numConditionsToMatch = 0;
	executeCustomHandlerIfConditionsNotMet = false;
}

BaseEvent::ConditionalBranch_Menu::ConditionalBranch_Menu(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	XMLNode TempNode1 = EventNode.getChildNode("conditions");
	QString tempString = TempNode1.getAttribute("conditionsToMatch");
	if (tempString == "all")
	{
		conditionsToMatch = ALL;
		numConditionsToMatch = 0;
	}
	else if (tempString == "atLeast")
	{
		conditionsToMatch = AT_LEAST;
		numConditionsToMatch = QString(TempNode1.getAttribute("numConditionsToMatch")).toInt();
	}
	else if (tempString == "none")
	{
		conditionsToMatch = NONE;
		numConditionsToMatch = 0;
	}
	tempString = TempNode1.getAttribute("executeCustomHandlerIfConditionsNotMet");
	executeCustomHandlerIfConditionsNotMet = (tempString == "true") ? true:false;
	for (int i = 0, count = TempNode1.nChildNode("condition"); i < count; ++i)
		addCondition(new Condition(TempNode1.getChildNode("condition", i)));
	TempNode1 = EventNode.getChildNode("onTrueEvents");
	for (int i = 0, count = TempNode1.nChildNode("event"); i < count; ++i)
		addOnTrueEvent(new BaseEvent(pBaseEvent->baseType, TempNode1.getChildNode("event", i)));
	if (executeCustomHandlerIfConditionsNotMet)
	{
		TempNode1 = EventNode.getChildNode("onFalseEvents");
		for (int i = 0, count = TempNode1.nChildNode("event"); i < count; ++i)
			addOnFalseEvent(new BaseEvent(pBaseEvent->baseType, TempNode1.getChildNode("event", i)));
	}
}

BaseEvent::ConditionalBranch_Menu::ConditionalBranch_Menu(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	int numConditions;
	int numOnTrueEvents;
	int numOnFalseEvents;
	conditionsToMatch = storageFile.getSignedInt();
	numConditionsToMatch = storageFile.getSignedInt();
	executeCustomHandlerIfConditionsNotMet = storageFile.getBool();
	numConditions = storageFile.getSignedInt();
	for (int i = 0; i < numConditions; ++i)
		addCondition(new Condition(storageFile));
	numOnTrueEvents = storageFile.getSignedInt();
	for (int i = 0; i < numOnTrueEvents; ++i)
		addOnTrueEvent(new BaseEvent(pBaseEvent->baseType, storageFile));
	if (executeCustomHandlerIfConditionsNotMet)
	{
		numOnFalseEvents = storageFile.getSignedInt();
		for (int i = 0; i < numOnFalseEvents; ++i)
			addOnFalseEvent(new BaseEvent(pBaseEvent->baseType, storageFile));
	}
}

BaseEvent::ConditionalBranch_Menu::ConditionalBranch_Menu(ConditionalBranch_Menu *other, BaseEvent *pBase) : Event(pBase)
{
	conditionsToMatch = other->conditionsToMatch;
	numConditionsToMatch = other->numConditionsToMatch;
	executeCustomHandlerIfConditionsNotMet = other->executeCustomHandlerIfConditionsNotMet;
	for (int i = 0; i < other->conditions.size(); ++i)
		addCondition(new Condition(other->conditions[i]));
	for (int i = 0; i < other->onTrueEvents.size(); ++i)
		addOnTrueEvent(new BaseEvent(other->onTrueEvents[i]));
	for (int i = 0; i < other->onFalseEvents.size(); ++i)
		addOnFalseEvent(new BaseEvent(other->onFalseEvents[i]));
}
//END BaseEvent::ConditionalBranch_Menu Member Functions

//BEGIN BaseEvent::ConditionBranch_Menu::Condition Member Functions
BaseEvent::ConditionalBranch_Menu::Condition::Condition()
{
	type = SWITCH;
	conditionUnion.switchCondition = new SwitchCondition;
	conditionUnion.switchCondition->switchID = 0;
	conditionUnion.switchCondition->switchReferencedInVariable = false;
	conditionUnion.switchCondition->isOn = true;
}

BaseEvent::ConditionalBranch_Menu::Condition::Condition(XMLNode ConditionNode)
{
	QString tempString;
	tempString = ConditionNode.getAttribute("type");
	if (tempString == "switch")
	{
		type = SWITCH;
		conditionUnion.switchCondition = new SwitchCondition;
		conditionUnion.switchCondition->switchID = QString(ConditionNode.getAttribute("switchID")).toInt();
		tempString = ConditionNode.getAttribute("switchReferencedInVariable");
		conditionUnion.switchCondition->switchReferencedInVariable = (tempString == "true") ? true:false;
		tempString = ConditionNode.getAttribute("isOn");
		conditionUnion.switchCondition->isOn = (tempString == "true") ? true:false;
	}
	else if (tempString == "variable")
	{
		type = VARIABLE;
		conditionUnion.variableCondition = new VariableCondition;
		tempString = ConditionNode.getAttribute("comparison");
		if (tempString == "==")
			conditionUnion.variableCondition->comparison = VariableCondition::EQUAL_TO;
		else if (tempString == "!=")
			conditionUnion.variableCondition->comparison = VariableCondition::NOT_EQUAL_TO;
		else if (tempString == ">")
			conditionUnion.variableCondition->comparison = VariableCondition::GREATER_THAN;
		else if (tempString == ">=")
			conditionUnion.variableCondition->comparison = VariableCondition::GREATER_THAN_OR_EQUAL_TO;
		else if (tempString == "<")
			conditionUnion.variableCondition->comparison = VariableCondition::LESS_THAN;
		else if (tempString == "<=")
			conditionUnion.variableCondition->comparison = VariableCondition::LESS_THAN_OR_EQUAL_TO;
		conditionUnion.variableCondition->srcVariableID = QString(ConditionNode.getAttribute("srcVariableID")).toInt();
		conditionUnion.variableCondition->dstValue = QString(ConditionNode.getAttribute("dstValue")).toInt();
		tempString = ConditionNode.getAttribute("dstValueStoredInVariable");
		conditionUnion.variableCondition->dstValueStoredInVariable = (tempString == "true") ? true:false;
	}
	else if (tempString == "string")
	{
		type = STRING;
		conditionUnion.stringCondition = new StringCondition;
		tempString = ConditionNode.getAttribute("comparison");
		if (tempString == "==")
			conditionUnion.stringCondition->comparison = StringCondition::EQUAL_TO;
		else if (tempString == "!=")
			conditionUnion.stringCondition->comparison = StringCondition::NOT_EQUAL_TO;
		else if (tempString == "startsWith")
			conditionUnion.stringCondition->comparison = StringCondition::STARTS_WITH;
		else if (tempString == "endsWith")
			conditionUnion.stringCondition->comparison = StringCondition::ENDS_WITH;
		else if (tempString == "contains")
			conditionUnion.stringCondition->comparison = StringCondition::CONTAINS;
		else if (tempString == "doesNotContain")
			conditionUnion.stringCondition->comparison = StringCondition::DOES_NOT_CONTAIN;
		else if (tempString == "isEmpty")
			conditionUnion.stringCondition->comparison = StringCondition::IS_EMPTY;
		else if (tempString == "isNotEmpty")
			conditionUnion.stringCondition->comparison = StringCondition::IS_NOT_EMPTY;
		conditionUnion.stringCondition->srcStringID = QString(ConditionNode.getAttribute("srcStringID")).toInt();
		tempString = ConditionNode.getAttribute("srcStringReferencedInVariable");
		conditionUnion.stringCondition->srcStringReferencedInVariable = (tempString == "true") ? true:false;
		tempString = ConditionNode.getAttribute("dstStringType");
		if (tempString == "stringVariable")
		{
			conditionUnion.stringCondition->dstStringType = StringCondition::STRING_VARIABLE;
			conditionUnion.stringCondition->dstValue = "";
			conditionUnion.stringCondition->dstStringID = QString(ConditionNode.getAttribute("dstStringID")).toInt();
		}
		else if (tempString == "referencedString")
		{
			conditionUnion.stringCondition->dstStringType = StringCondition::REFERENCED_STRING;
			conditionUnion.stringCondition->dstValue = "";
			conditionUnion.stringCondition->dstStringID = QString(ConditionNode.getAttribute("dstVariableID")).toInt();
		}
		else if (tempString == "value")
		{
			conditionUnion.stringCondition->dstStringType = StringCondition::VALUE;
			conditionUnion.stringCondition->dstValue = ConditionNode.getAttribute("dstValue");
			conditionUnion.stringCondition->dstStringID = 0;
		}
	}
	else if (tempString == "timer")
	{
		type = TIMER;
		conditionUnion.timerCondition = new TimerCondition;
		conditionUnion.timerCondition->timerID = QString(ConditionNode.getAttribute("timerID")).toInt();
		tempString = ConditionNode.getAttribute("timerReferencedInVariable");
		conditionUnion.timerCondition->timerReferencedInVariable = (tempString == "true") ? true:false;
		tempString = ConditionNode.getAttribute("timeStoredInVariable");
		conditionUnion.timerCondition->timeStoredInVariable = (tempString == "true") ? true:false;
		if (conditionUnion.timerCondition->timeStoredInVariable)
		{
			conditionUnion.timerCondition->minutes = 0;
			conditionUnion.timerCondition->seconds = QString(ConditionNode.getAttribute("variableID")).toInt();
		}
		else
		{
			conditionUnion.timerCondition->minutes = QString(ConditionNode.getAttribute("minutes")).toInt();
			conditionUnion.timerCondition->seconds = QString(ConditionNode.getAttribute("seconds")).toInt();
		}
		tempString = ConditionNode.getAttribute("comparison");
		if (tempString == "==")
			conditionUnion.timerCondition->comparison = TimerCondition::EQUAL_TO;
		else if (tempString == "!=")
			conditionUnion.timerCondition->comparison = TimerCondition::NOT_EQUAL_TO;
		else if (tempString == ">")
			conditionUnion.timerCondition->comparison = TimerCondition::GREATER_THAN;
		else if (tempString == ">=")
			conditionUnion.timerCondition->comparison = TimerCondition::GREATER_THAN_OR_EQUAL_TO;
		else if (tempString == "<")
			conditionUnion.timerCondition->comparison = TimerCondition::LESS_THAN;
		else if (tempString == "<=")
			conditionUnion.timerCondition->comparison = TimerCondition::LESS_THAN_OR_EQUAL_TO;
	}
	else if (tempString == "money")
	{
		type = MONEY;
		conditionUnion.moneyCondition = new MoneyCondition;
		tempString = ConditionNode.getAttribute("comparison");
		if (tempString == "==")
			conditionUnion.moneyCondition->comparison = MoneyCondition::EQUAL_TO;
		else if (tempString == "!=")
			conditionUnion.moneyCondition->comparison = MoneyCondition::NOT_EQUAL_TO;
		else if (tempString == ">")
			conditionUnion.moneyCondition->comparison = MoneyCondition::GREATER_THAN;
		else if (tempString == ">=")
			conditionUnion.moneyCondition->comparison = MoneyCondition::GREATER_THAN_OR_EQUAL_TO;
		else if (tempString == "<")
			conditionUnion.moneyCondition->comparison = MoneyCondition::LESS_THAN;
		else if (tempString == "<=")
			conditionUnion.moneyCondition->comparison = MoneyCondition::LESS_THAN_OR_EQUAL_TO;
		conditionUnion.moneyCondition->value = QString(ConditionNode.getAttribute("value")).toInt();
		tempString = ConditionNode.getAttribute("valueStoredInVariable");
		conditionUnion.moneyCondition->valueStoredInVariable = (tempString == "true") ? true:false;
	}
	else if (tempString == "item")
	{
		type = ITEM;
		conditionUnion.itemCondition = new ItemCondition;
		conditionUnion.itemCondition->itemID = QString(ConditionNode.getAttribute("itemID")).toInt();
		tempString = ConditionNode.getAttribute("itemReferencedInVariable");
		conditionUnion.itemCondition->itemReferencedInVariable = (tempString == "true") ? true:false;
		tempString = ConditionNode.getAttribute("partyPossesses");
		conditionUnion.itemCondition->partyPossesses = (tempString == "true") ? true:false;
	}
	else if (tempString == "hero")
	{
		type = HERO;
		conditionUnion.heroCondition = new HeroCondition;
		conditionUnion.heroCondition->characterID = QString(ConditionNode.getAttribute("characterID")).toInt();
		tempString = ConditionNode.getAttribute("characterReferencedInVariable");
		conditionUnion.heroCondition->characterReferencedInVariable = (tempString == "true") ? true:false;
		tempString = ConditionNode.getAttribute("heroConditionType");
		if (tempString == "inParty")
		{
			conditionUnion.heroCondition->conditionType = HeroCondition::IN_PARTY;
			conditionUnion.heroCondition->string = "";
			conditionUnion.heroCondition->value = 0;
			conditionUnion.heroCondition->valueStoredInVariable = false;
		}
		else if (tempString == "nameIs")
		{
			conditionUnion.heroCondition->conditionType = HeroCondition::NAME_IS;
			conditionUnion.heroCondition->string = "";
			conditionUnion.heroCondition->value = 0;
			tempString = ConditionNode.getAttribute("stringStoredInVariable");
			conditionUnion.heroCondition->valueStoredInVariable = (tempString == "true") ? true:false;
			if (tempString == "true")
				conditionUnion.heroCondition->value = QString(ConditionNode.getAttribute("string")).toInt();
			else
				conditionUnion.heroCondition->string = ConditionNode.getAttribute("string");
		}
		else if (tempString == "levelIsAtLeast")
		{
			conditionUnion.heroCondition->conditionType = HeroCondition::LEVEL_IS_AT_LEAST;
			conditionUnion.heroCondition->string = "";
			conditionUnion.heroCondition->value = QString(ConditionNode.getAttribute("level")).toInt();
			tempString = ConditionNode.getAttribute("levelStoredInVariable");
			conditionUnion.heroCondition->valueStoredInVariable = (tempString == "true") ? true:false;
		}
		else if (tempString == "hpIsAtLeast")
		{
			conditionUnion.heroCondition->conditionType = HeroCondition::HP_IS_AT_LEAST;
			conditionUnion.heroCondition->string = "";
			conditionUnion.heroCondition->value = QString(ConditionNode.getAttribute("hp")).toInt();
			tempString = ConditionNode.getAttribute("hpStoredInVariable");
			conditionUnion.heroCondition->valueStoredInVariable = (tempString == "true") ? true:false;
		}
		else if (tempString == "canUseSkill")
		{
			conditionUnion.heroCondition->conditionType = HeroCondition::CAN_USE_SKILL;
			conditionUnion.heroCondition->string = "";
			conditionUnion.heroCondition->value = QString(ConditionNode.getAttribute("skillID")).toInt();
			tempString = ConditionNode.getAttribute("skillReferencedInVariable");
			conditionUnion.heroCondition->valueStoredInVariable = (tempString == "true") ? true:false;
		}
		else if (tempString == "hasItemEquipped")
		{
			conditionUnion.heroCondition->conditionType = HeroCondition::HAS_ITEM_EQUIPPED;
			conditionUnion.heroCondition->string = "";
			conditionUnion.heroCondition->value = QString(ConditionNode.getAttribute("itemID")).toInt();
			tempString = ConditionNode.getAttribute("itemReferencedInVariable");
			conditionUnion.heroCondition->valueStoredInVariable = (tempString == "true") ? true:false;
		}
		else if (tempString == "afflictedWithCondition")
		{
			conditionUnion.heroCondition->conditionType = HeroCondition::AFFLICTED_WITH_CONDITION;
			conditionUnion.heroCondition->string = "";
			conditionUnion.heroCondition->value = QString(ConditionNode.getAttribute("conditionID")).toInt();
			tempString = ConditionNode.getAttribute("conditionReferencedInVariable");
			conditionUnion.heroCondition->valueStoredInVariable = (tempString == "true") ? true:false;
		}
	}
	else if (tempString == "partyIsRiding")
	{
		type = PARTY_IS_RIDING;
		conditionUnion.partyIsRidingCondition = new PartyIsRidingCondition;
		conditionUnion.partyIsRidingCondition->vehicleID = QString(ConditionNode.getAttribute("vehicleID")).toInt();
		tempString = ConditionNode.getAttribute("vehicleReferencedInVariable");
		conditionUnion.partyIsRidingCondition->vehicleReferencedInVariable = (tempString == "true") ? true:false;
	}
	else if (tempString == "previousSelectedArea")
	{
		type = PREVIOUS_SELECTED_AREA;
		conditionUnion.previousSelectedAreaCondition = new PreviousSelectedAreaCondition;
		conditionUnion.previousSelectedAreaCondition->objectName = ConditionNode.getAttribute("objectName");
	}
	else if (tempString == "currentHighlightedArea")
	{
		type = CURRENT_HIGHLIGHTED_AREA;
		conditionUnion.currentHighlightedAreaCondition = new CurrentHighlightedAreaCondition;
		conditionUnion.currentHighlightedAreaCondition->objectName = ConditionNode.getAttribute("objectName");
	}
	else if (tempString == "bgmPlayedThroughOnce")
		type = BGM_PLAYED_THROUGH_ONCE;
}

BaseEvent::ConditionalBranch_Menu::Condition::Condition(StorageFile &storageFile)
{
	char *temp_string;
	type = (ConditionType)storageFile.getSignedInt();
	if (type == SWITCH)
	{
		conditionUnion.switchCondition = new SwitchCondition;
		conditionUnion.switchCondition->switchID = storageFile.getSignedInt();
		conditionUnion.switchCondition->switchReferencedInVariable = storageFile.getBool();
		conditionUnion.switchCondition->isOn = storageFile.getBool();
	}
	else if (type == VARIABLE)
	{
		conditionUnion.variableCondition = new VariableCondition;
		conditionUnion.variableCondition->comparison = storageFile.getSignedInt();
		conditionUnion.variableCondition->srcVariableID = storageFile.getSignedInt();
		conditionUnion.variableCondition->dstValue = storageFile.getSignedInt();
		conditionUnion.variableCondition->dstValueStoredInVariable = storageFile.getBool();
	}
	else if (type == STRING)
	{
		conditionUnion.stringCondition = new StringCondition;
		conditionUnion.stringCondition->comparison = storageFile.getSignedInt();
		conditionUnion.stringCondition->dstStringType = storageFile.getSignedInt();
		conditionUnion.stringCondition->srcStringID = storageFile.getSignedInt();
		conditionUnion.stringCondition->srcStringReferencedInVariable = storageFile.getBool();
		if (conditionUnion.stringCondition->dstStringType == StringCondition::STRING_VARIABLE || conditionUnion.stringCondition->dstStringType == StringCondition::REFERENCED_STRING)
		{
			conditionUnion.stringCondition->dstValue = "";
			conditionUnion.stringCondition->dstStringID = storageFile.getSignedInt();
		}
		else if (conditionUnion.stringCondition->dstStringType == StringCondition::VALUE)
		{
			temp_string = storageFile.getString();
			conditionUnion.stringCondition->dstValue = temp_string;
			delete[] temp_string;
			conditionUnion.stringCondition->dstStringID = 0;
		}
	}
	else if (type == TIMER)
	{
		conditionUnion.timerCondition = new TimerCondition;
		conditionUnion.timerCondition->comparison = storageFile.getSignedInt();
		conditionUnion.timerCondition->timerID = storageFile.getSignedInt();
		conditionUnion.timerCondition->timerReferencedInVariable = storageFile.getBool();
		conditionUnion.timerCondition->timeStoredInVariable = storageFile.getBool();
		if (conditionUnion.timerCondition->timeStoredInVariable)
		{
			conditionUnion.timerCondition->minutes = 0;
			conditionUnion.timerCondition->seconds = storageFile.getSignedInt();
		}
		else
		{
			conditionUnion.timerCondition->minutes = storageFile.getSignedInt();
			conditionUnion.timerCondition->seconds = storageFile.getSignedInt();
		}
	}
	else if (type == MONEY)
	{
		conditionUnion.moneyCondition = new MoneyCondition;
		conditionUnion.moneyCondition->comparison = storageFile.getSignedInt();
		conditionUnion.moneyCondition->value = storageFile.getSignedInt();
		conditionUnion.moneyCondition->valueStoredInVariable = storageFile.getBool();
	}
	else if (type == ITEM)
	{
		conditionUnion.itemCondition = new ItemCondition;
		conditionUnion.itemCondition->itemID = storageFile.getSignedInt();
		conditionUnion.itemCondition->itemReferencedInVariable = storageFile.getBool();
		conditionUnion.itemCondition->partyPossesses = storageFile.getBool();
	}
	else if (type == HERO)
	{
		conditionUnion.heroCondition = new HeroCondition;
		conditionUnion.heroCondition->conditionType = storageFile.getSignedInt();
		conditionUnion.heroCondition->characterID = storageFile.getSignedInt();
		conditionUnion.heroCondition->characterReferencedInVariable = storageFile.getBool();
		if (conditionUnion.heroCondition->conditionType == HeroCondition::IN_PARTY)
		{
			conditionUnion.heroCondition->string = "";
			conditionUnion.heroCondition->value = 0;
			conditionUnion.heroCondition->valueStoredInVariable = false;
		}
		else if (conditionUnion.heroCondition->conditionType == HeroCondition::NAME_IS)
		{
			conditionUnion.heroCondition->string = "";
			conditionUnion.heroCondition->value = 0;
			conditionUnion.heroCondition->valueStoredInVariable = storageFile.getBool();
			if (conditionUnion.heroCondition->valueStoredInVariable)
				conditionUnion.heroCondition->value = storageFile.getSignedInt();
			else
			{
				temp_string = storageFile.getString();
				conditionUnion.heroCondition->string = temp_string;
				delete[] temp_string;
			}
		}
		else
		{
			conditionUnion.heroCondition->string = "";
			conditionUnion.heroCondition->value = storageFile.getSignedInt();
			conditionUnion.heroCondition->valueStoredInVariable = storageFile.getBool();
		}
	}
	else if (type == PARTY_IS_RIDING)
	{
		conditionUnion.partyIsRidingCondition = new PartyIsRidingCondition;
		conditionUnion.partyIsRidingCondition->vehicleID = storageFile.getSignedInt();
		conditionUnion.partyIsRidingCondition->vehicleReferencedInVariable = storageFile.getBool();
	}
	else if (type == PREVIOUS_SELECTED_AREA)
	{
		conditionUnion.previousSelectedAreaCondition = new PreviousSelectedAreaCondition;
		temp_string = storageFile.getString();
		conditionUnion.previousSelectedAreaCondition->objectName = temp_string;
		delete[] temp_string;
	}
	else if (type == CURRENT_HIGHLIGHTED_AREA)
	{
		conditionUnion.currentHighlightedAreaCondition = new CurrentHighlightedAreaCondition;
		temp_string = storageFile.getString();
		conditionUnion.currentHighlightedAreaCondition->objectName = temp_string;
		delete[] temp_string;
	}
}

BaseEvent::ConditionalBranch_Menu::Condition::Condition(Condition *other)
{
	type = other->type;
	if (type == SWITCH)
	{
		conditionUnion.switchCondition = new SwitchCondition;
		conditionUnion.switchCondition->switchID = other->conditionUnion.switchCondition->switchID;
		conditionUnion.switchCondition->switchReferencedInVariable = other->conditionUnion.switchCondition->switchReferencedInVariable;
		conditionUnion.switchCondition->isOn = other->conditionUnion.switchCondition->isOn;
	}
	else if (type == VARIABLE)
	{
		conditionUnion.variableCondition = new VariableCondition;
		conditionUnion.variableCondition->comparison = other->conditionUnion.variableCondition->comparison;
		conditionUnion.variableCondition->srcVariableID = other->conditionUnion.variableCondition->srcVariableID;
		conditionUnion.variableCondition->dstValue = other->conditionUnion.variableCondition->dstValue;
		conditionUnion.variableCondition->dstValueStoredInVariable = other->conditionUnion.variableCondition->dstValueStoredInVariable;
	}
	else if (type == STRING)
	{
		conditionUnion.stringCondition = new StringCondition;
		conditionUnion.stringCondition->comparison = other->conditionUnion.stringCondition->comparison;
		conditionUnion.stringCondition->dstStringType = other->conditionUnion.stringCondition->dstStringType;
		conditionUnion.stringCondition->dstValue = "";
		conditionUnion.stringCondition->dstValue.append(other->conditionUnion.stringCondition->dstValue);
		conditionUnion.stringCondition->srcStringID = other->conditionUnion.stringCondition->srcStringID;
		conditionUnion.stringCondition->dstStringID = other->conditionUnion.stringCondition->dstStringID;
		conditionUnion.stringCondition->srcStringReferencedInVariable = other->conditionUnion.stringCondition->srcStringReferencedInVariable;
	}
	else if (type == TIMER)
	{
		conditionUnion.timerCondition = new TimerCondition;
		conditionUnion.timerCondition->comparison = other->conditionUnion.timerCondition->comparison;
		conditionUnion.timerCondition->timerID = other->conditionUnion.timerCondition->timerID;
		conditionUnion.timerCondition->minutes = other->conditionUnion.timerCondition->minutes;
		conditionUnion.timerCondition->seconds = other->conditionUnion.timerCondition->seconds;
		conditionUnion.timerCondition->timerReferencedInVariable = other->conditionUnion.timerCondition->timerReferencedInVariable;
		conditionUnion.timerCondition->timeStoredInVariable = other->conditionUnion.timerCondition->timeStoredInVariable;
	}
	else if (type == MONEY)
	{
		conditionUnion.moneyCondition = new MoneyCondition;
		conditionUnion.moneyCondition->comparison = other->conditionUnion.moneyCondition->comparison;
		conditionUnion.moneyCondition->value = other->conditionUnion.moneyCondition->value;
		conditionUnion.moneyCondition->valueStoredInVariable = other->conditionUnion.moneyCondition->valueStoredInVariable;
	}
	else if (type == ITEM)
	{
		conditionUnion.itemCondition = new ItemCondition;
		conditionUnion.itemCondition->itemID = other->conditionUnion.itemCondition->itemID;
		conditionUnion.itemCondition->itemReferencedInVariable = other->conditionUnion.itemCondition->itemReferencedInVariable;
		conditionUnion.itemCondition->partyPossesses = other->conditionUnion.itemCondition->partyPossesses;
	}
	else if (type == HERO)
	{
		conditionUnion.heroCondition = new HeroCondition;
		conditionUnion.heroCondition->conditionType = other->conditionUnion.heroCondition->conditionType;
		conditionUnion.heroCondition->string = "";
		conditionUnion.heroCondition->string.append(other->conditionUnion.heroCondition->string);
		conditionUnion.heroCondition->value = other->conditionUnion.heroCondition->value;
		conditionUnion.heroCondition->characterID = other->conditionUnion.heroCondition->characterID;
		conditionUnion.heroCondition->characterReferencedInVariable = other->conditionUnion.heroCondition->characterReferencedInVariable;
		conditionUnion.heroCondition->valueStoredInVariable = other->conditionUnion.heroCondition->valueStoredInVariable;
	}
	else if (type == PARTY_IS_RIDING)
	{
		conditionUnion.partyIsRidingCondition = new PartyIsRidingCondition;
		conditionUnion.partyIsRidingCondition->vehicleID = other->conditionUnion.partyIsRidingCondition->vehicleID;
		conditionUnion.partyIsRidingCondition->vehicleReferencedInVariable = other->conditionUnion.partyIsRidingCondition->vehicleReferencedInVariable;
	}
	else if (type == PREVIOUS_SELECTED_AREA)
	{
		conditionUnion.previousSelectedAreaCondition = new PreviousSelectedAreaCondition;
		conditionUnion.previousSelectedAreaCondition->objectName = "";
		conditionUnion.previousSelectedAreaCondition->objectName.append(other->conditionUnion.previousSelectedAreaCondition->objectName);
	}
	else if (type == CURRENT_HIGHLIGHTED_AREA)
	{
		conditionUnion.currentHighlightedAreaCondition = new CurrentHighlightedAreaCondition;
		conditionUnion.currentHighlightedAreaCondition->objectName = "";
		conditionUnion.currentHighlightedAreaCondition->objectName.append(other->conditionUnion.currentHighlightedAreaCondition->objectName);
	}
}

BaseEvent::ConditionalBranch_Menu::Condition::~Condition()
{
	if (type == SWITCH)
		delete (conditionUnion.switchCondition);
	else if (type == VARIABLE)
		delete (conditionUnion.variableCondition);
	else if (type == STRING)
		delete (conditionUnion.stringCondition);
	else if (type == TIMER)
		delete (conditionUnion.timerCondition);
	else if (type == MONEY)
		delete (conditionUnion.moneyCondition);
	else if (type == ITEM)
		delete (conditionUnion.itemCondition);
	else if (type == HERO)
		delete (conditionUnion.heroCondition);
	else if (type == PARTY_IS_RIDING)
		delete (conditionUnion.partyIsRidingCondition);
	else if (type == PREVIOUS_SELECTED_AREA)
		delete (conditionUnion.previousSelectedAreaCondition);
	else if (type == CURRENT_HIGHLIGHTED_AREA)
		delete (conditionUnion.currentHighlightedAreaCondition);
}

XMLNode BaseEvent::ConditionalBranch_Menu::Condition::getConditionNode()
{
	XMLNode ConditionNode = XMLNode::createXMLTopNode("condition");
	if (type == SWITCH)
	{
		ConditionNode.addAttribute("type", "switch");
		ConditionNode.addAttribute("switchID", QString::number(conditionUnion.switchCondition->switchID).toUtf8().data());
		ConditionNode.addAttribute("switchReferencedInVariable", (conditionUnion.switchCondition->switchReferencedInVariable) ? "true":"false");
		ConditionNode.addAttribute("isOn", (conditionUnion.switchCondition->isOn) ? "true":"false");
	}
	else if (type == VARIABLE)
	{
		ConditionNode.addAttribute("type", "variable");
		if (conditionUnion.variableCondition->comparison == VariableCondition::EQUAL_TO)
			ConditionNode.addAttribute("comparison", "==");
		else if (conditionUnion.variableCondition->comparison == VariableCondition::NOT_EQUAL_TO)
			ConditionNode.addAttribute("comparison", "!=");
		else if (conditionUnion.variableCondition->comparison == VariableCondition::GREATER_THAN)
			ConditionNode.addAttribute("comparison", ">");
		else if (conditionUnion.variableCondition->comparison == VariableCondition::GREATER_THAN_OR_EQUAL_TO)
			ConditionNode.addAttribute("comparison", ">=");
		else if (conditionUnion.variableCondition->comparison == VariableCondition::LESS_THAN)
			ConditionNode.addAttribute("comparison", "<");
		else if (conditionUnion.variableCondition->comparison == VariableCondition::LESS_THAN_OR_EQUAL_TO)
			ConditionNode.addAttribute("comparison", "<=");
		ConditionNode.addAttribute("srcVariableID", QString::number(conditionUnion.variableCondition->srcVariableID).toUtf8().data());
		ConditionNode.addAttribute("dstValue", QString::number(conditionUnion.variableCondition->dstValue).toUtf8().data());
		ConditionNode.addAttribute("dstValueStoredInVariable", (conditionUnion.variableCondition->dstValueStoredInVariable) ? "true":"false");
	}
	else if (type == STRING)
	{
		ConditionNode.addAttribute("type", "string");
		if (conditionUnion.stringCondition->comparison == StringCondition::EQUAL_TO)
			ConditionNode.addAttribute("comparison", "==");
		else if (conditionUnion.stringCondition->comparison == StringCondition::NOT_EQUAL_TO)
			ConditionNode.addAttribute("comparison", "!=");
		else if (conditionUnion.stringCondition->comparison == StringCondition::STARTS_WITH)
			ConditionNode.addAttribute("comparison", "startsWith");
		else if (conditionUnion.stringCondition->comparison == StringCondition::ENDS_WITH)
			ConditionNode.addAttribute("comparison", "endsWith");
		else if (conditionUnion.stringCondition->comparison == StringCondition::CONTAINS)
			ConditionNode.addAttribute("comparison", "contains");
		else if (conditionUnion.stringCondition->comparison == StringCondition::DOES_NOT_CONTAIN)
			ConditionNode.addAttribute("comparison", "doesNotContain");
		else if (conditionUnion.stringCondition->comparison == StringCondition::IS_EMPTY)
			ConditionNode.addAttribute("comparison", "isEmpty");
		else if (conditionUnion.stringCondition->comparison == StringCondition::IS_NOT_EMPTY)
			ConditionNode.addAttribute("comparison", "isNotEmpty");
		ConditionNode.addAttribute("srcStringID", QString::number(conditionUnion.stringCondition->srcStringID).toUtf8().data());
		ConditionNode.addAttribute("srcStringReferencedInVariable", (conditionUnion.stringCondition->srcStringReferencedInVariable) ? "true":"false");
		if (conditionUnion.stringCondition->dstStringType == StringCondition::STRING_VARIABLE)
		{
			ConditionNode.addAttribute("dstStringType", "stringVariable");
			ConditionNode.addAttribute("dstStringID", QString::number(conditionUnion.stringCondition->dstStringID).toUtf8().data());
		}
		else if (conditionUnion.stringCondition->dstStringType == StringCondition::REFERENCED_STRING)
		{
			ConditionNode.addAttribute("dstStringType", "referencedString");
			ConditionNode.addAttribute("dstVariableID", QString::number(conditionUnion.stringCondition->dstStringID).toUtf8().data());
		}
		else if (conditionUnion.stringCondition->dstStringType == StringCondition::VALUE)
		{
			ConditionNode.addAttribute("dstStringType", "value");
			ConditionNode.addAttribute("dstValue", conditionUnion.stringCondition->dstValue.toUtf8().data());
		}
	}
	else if (type == TIMER)
	{
		ConditionNode.addAttribute("type", "timer");
		if (conditionUnion.timerCondition->comparison == TimerCondition::EQUAL_TO)
			ConditionNode.addAttribute("comparison", "==");
		else if (conditionUnion.timerCondition->comparison == TimerCondition::NOT_EQUAL_TO)
			ConditionNode.addAttribute("comparison", "!=");
		else if (conditionUnion.timerCondition->comparison == TimerCondition::GREATER_THAN)
			ConditionNode.addAttribute("comparison", ">");
		else if (conditionUnion.timerCondition->comparison == TimerCondition::GREATER_THAN_OR_EQUAL_TO)
			ConditionNode.addAttribute("comparison", ">=");
		else if (conditionUnion.timerCondition->comparison == TimerCondition::LESS_THAN)
			ConditionNode.addAttribute("comparison", "<");
		else if (conditionUnion.timerCondition->comparison == TimerCondition::LESS_THAN_OR_EQUAL_TO)
			ConditionNode.addAttribute("comparison", "<=");
		ConditionNode.addAttribute("timerID", QString::number(conditionUnion.timerCondition->timerID).toUtf8().data());
		ConditionNode.addAttribute("timerReferencedInVariable", (conditionUnion.timerCondition->timerReferencedInVariable) ? "true":"false");
		ConditionNode.addAttribute("timeStoredInVariable", (conditionUnion.timerCondition->timeStoredInVariable) ? "true":"false");
		if (conditionUnion.timerCondition->timeStoredInVariable)
			ConditionNode.addAttribute("variableID", QString::number(conditionUnion.timerCondition->seconds).toUtf8().data());
		else
		{
			ConditionNode.addAttribute("minutes", QString::number(conditionUnion.timerCondition->minutes).toUtf8().data());
			ConditionNode.addAttribute("seconds", QString::number(conditionUnion.timerCondition->seconds).toUtf8().data());
		}
	}
	else if (type == MONEY)
	{
		ConditionNode.addAttribute("type", "money");
		if (conditionUnion.moneyCondition->comparison == MoneyCondition::EQUAL_TO)
			ConditionNode.addAttribute("comparison", "==");
		else if (conditionUnion.moneyCondition->comparison == MoneyCondition::NOT_EQUAL_TO)
			ConditionNode.addAttribute("comparison", "!=");
		else if (conditionUnion.moneyCondition->comparison == MoneyCondition::GREATER_THAN)
			ConditionNode.addAttribute("comparison", ">");
		else if (conditionUnion.moneyCondition->comparison == MoneyCondition::GREATER_THAN_OR_EQUAL_TO)
			ConditionNode.addAttribute("comparison", ">=");
		else if (conditionUnion.moneyCondition->comparison == MoneyCondition::LESS_THAN)
			ConditionNode.addAttribute("comparison", "<");
		else if (conditionUnion.moneyCondition->comparison == MoneyCondition::LESS_THAN_OR_EQUAL_TO)
			ConditionNode.addAttribute("comparison", "<=");
		ConditionNode.addAttribute("value", QString::number(conditionUnion.moneyCondition->value).toUtf8().data());
		ConditionNode.addAttribute("valueStoredInVariable", (conditionUnion.moneyCondition->valueStoredInVariable) ? "true":"false");
	}
	else if (type == ITEM)
	{
		ConditionNode.addAttribute("type", "item");
		ConditionNode.addAttribute("itemID", QString::number(conditionUnion.itemCondition->itemID).toUtf8().data());
		ConditionNode.addAttribute("itemReferencedInVariable", (conditionUnion.itemCondition->itemReferencedInVariable) ? "true":"false");
		ConditionNode.addAttribute("partyPossesses", (conditionUnion.itemCondition->partyPossesses) ? "true":"false");
	}
	else if (type == HERO)
	{
		ConditionNode.addAttribute("type", "hero");
		ConditionNode.addAttribute("characterID", QString::number(conditionUnion.heroCondition->characterID).toUtf8().data());
		ConditionNode.addAttribute("characterReferencedInVariable", (conditionUnion.heroCondition->characterReferencedInVariable) ? "true":"false");
		if (conditionUnion.heroCondition->conditionType == HeroCondition::IN_PARTY)
			ConditionNode.addAttribute("heroConditionType", "inParty");
		else if (conditionUnion.heroCondition->conditionType == HeroCondition::NAME_IS)
		{
			ConditionNode.addAttribute("heroConditionType", "nameIs");
			ConditionNode.addAttribute("stringStoredInVariable", (conditionUnion.heroCondition->valueStoredInVariable) ? "true":"false");
			if (conditionUnion.heroCondition->valueStoredInVariable)
				ConditionNode.addAttribute("string", QString::number(conditionUnion.heroCondition->value).toUtf8().data());
			else
				ConditionNode.addAttribute("string", conditionUnion.heroCondition->string.toUtf8().data());
		}
		else if (conditionUnion.heroCondition->conditionType == HeroCondition::LEVEL_IS_AT_LEAST)
		{
			ConditionNode.addAttribute("heroConditionType", "levelIsAtLeast");
			ConditionNode.addAttribute("level", QString::number(conditionUnion.heroCondition->value).toUtf8().data());
			ConditionNode.addAttribute("levelStoredInVariable", (conditionUnion.heroCondition->valueStoredInVariable) ? "true":"false");
		}
		else if (conditionUnion.heroCondition->conditionType == HeroCondition::HP_IS_AT_LEAST)
		{
			ConditionNode.addAttribute("heroConditionType", "hpIsAtLeast");
			ConditionNode.addAttribute("hp", QString::number(conditionUnion.heroCondition->value).toUtf8().data());
			ConditionNode.addAttribute("hpStoredInVariable", (conditionUnion.heroCondition->valueStoredInVariable) ? "true":"false");
		}
		else if (conditionUnion.heroCondition->conditionType == HeroCondition::CAN_USE_SKILL)
		{
			ConditionNode.addAttribute("heroConditionType", "canUseSkill");
			ConditionNode.addAttribute("skillID", QString::number(conditionUnion.heroCondition->value).toUtf8().data());
			ConditionNode.addAttribute("skillReferencedInVariable", (conditionUnion.heroCondition->valueStoredInVariable) ? "true":"false");
		}
		else if (conditionUnion.heroCondition->conditionType == HeroCondition::HAS_ITEM_EQUIPPED)
		{
			ConditionNode.addAttribute("heroConditionType", "hasItemEquipped");
			ConditionNode.addAttribute("itemID", QString::number(conditionUnion.heroCondition->value).toUtf8().data());
			ConditionNode.addAttribute("itemReferencedInVariable", (conditionUnion.heroCondition->valueStoredInVariable) ? "true":"false");
		}
		else if (conditionUnion.heroCondition->conditionType == HeroCondition::AFFLICTED_WITH_CONDITION)
		{
			ConditionNode.addAttribute("heroConditionType", "afflictedWithCondition");
			ConditionNode.addAttribute("conditionID", QString::number(conditionUnion.heroCondition->value).toUtf8().data());
			ConditionNode.addAttribute("conditionReferencedInVariable", (conditionUnion.heroCondition->valueStoredInVariable) ? "true":"false");
		}
	}
	else if (type == PARTY_IS_RIDING)
	{
		ConditionNode.addAttribute("type", "partyIsRiding");
		ConditionNode.addAttribute("vehicleID", QString::number(conditionUnion.partyIsRidingCondition->vehicleID).toUtf8().data());
		ConditionNode.addAttribute("vehicleReferencedInVariable", (conditionUnion.partyIsRidingCondition->vehicleReferencedInVariable) ? "true":"false");
	}
	else if (type == PREVIOUS_SELECTED_AREA)
	{
		ConditionNode.addAttribute("type", "previousSelectedArea");
		ConditionNode.addAttribute("objectName", conditionUnion.previousSelectedAreaCondition->objectName.toUtf8().data());
	}
	else if (type == CURRENT_HIGHLIGHTED_AREA)
	{
		ConditionNode.addAttribute("type", "currentHighlightedArea");
		ConditionNode.addAttribute("objectName", conditionUnion.currentHighlightedAreaCondition->objectName.toUtf8().data());
	}
	else if (type == BGM_PLAYED_THROUGH_ONCE)
		ConditionNode.addAttribute("type", "bgmPlayedThroughOnce");
	return ConditionNode;
}

void BaseEvent::ConditionalBranch_Menu::Condition::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt((int)type);
	if (type == SWITCH)
	{
		storageFile.putSignedInt(conditionUnion.switchCondition->switchID);
		storageFile.putBool(conditionUnion.switchCondition->switchReferencedInVariable);
		storageFile.putBool(conditionUnion.switchCondition->isOn);
	}
	else if (type == VARIABLE)
	{
		storageFile.putSignedInt(conditionUnion.variableCondition->comparison);
		storageFile.putSignedInt(conditionUnion.variableCondition->srcVariableID);
		storageFile.putSignedInt(conditionUnion.variableCondition->dstValue);
		storageFile.putBool(conditionUnion.variableCondition->dstValueStoredInVariable);
	}
	else if (type == STRING)
	{
		storageFile.putSignedInt(conditionUnion.stringCondition->comparison);
		storageFile.putSignedInt(conditionUnion.stringCondition->dstStringType);
		storageFile.putSignedInt(conditionUnion.stringCondition->srcStringID);
		storageFile.putBool(conditionUnion.stringCondition->srcStringReferencedInVariable);
		if (conditionUnion.stringCondition->dstStringType == StringCondition::STRING_VARIABLE || conditionUnion.stringCondition->dstStringType == StringCondition::REFERENCED_STRING)
			storageFile.putSignedInt(conditionUnion.stringCondition->dstStringID);
		else if (conditionUnion.stringCondition->dstStringType == StringCondition::VALUE)
			storageFile.putString(conditionUnion.stringCondition->dstValue.toUtf8().data());
	}
	else if (type == TIMER)
	{
		storageFile.putSignedInt((int)conditionUnion.timerCondition->comparison);
		storageFile.putSignedInt(conditionUnion.timerCondition->timerID);
		storageFile.putBool(conditionUnion.timerCondition->timerReferencedInVariable);
		storageFile.putBool(conditionUnion.timerCondition->timeStoredInVariable);
		if (conditionUnion.timerCondition->timeStoredInVariable)
			storageFile.putSignedInt(conditionUnion.timerCondition->seconds);
		else
		{
			storageFile.putSignedInt(conditionUnion.timerCondition->minutes);
			storageFile.putSignedInt(conditionUnion.timerCondition->seconds);
		}
	}
	else if (type == MONEY)
	{
		storageFile.putSignedInt((int)conditionUnion.moneyCondition->comparison);
		storageFile.putSignedInt(conditionUnion.moneyCondition->value);
		storageFile.putBool(conditionUnion.moneyCondition->valueStoredInVariable);
	}
	else if (type == ITEM)
	{
		storageFile.putSignedInt(conditionUnion.itemCondition->itemID);
		storageFile.putBool(conditionUnion.itemCondition->itemReferencedInVariable);
		storageFile.putBool(conditionUnion.itemCondition->partyPossesses);
	}
	else if (type == HERO)
	{
		storageFile.putSignedInt((int)conditionUnion.heroCondition->conditionType);
		storageFile.putSignedInt(conditionUnion.heroCondition->characterID);
		storageFile.putBool(conditionUnion.heroCondition->characterReferencedInVariable);
		if (conditionUnion.heroCondition->conditionType == HeroCondition::NAME_IS)
		{
			storageFile.putBool(conditionUnion.heroCondition->valueStoredInVariable);
			if (conditionUnion.heroCondition->valueStoredInVariable)
				storageFile.putSignedInt(conditionUnion.heroCondition->value);
			else
				storageFile.putString(conditionUnion.heroCondition->string.toUtf8().data());
		}
		else
		{
			storageFile.putSignedInt(conditionUnion.heroCondition->value);
			storageFile.putBool(conditionUnion.heroCondition->valueStoredInVariable);
		}
	}
	else if (type == PARTY_IS_RIDING)
	{
		storageFile.putSignedInt(conditionUnion.partyIsRidingCondition->vehicleID);
		storageFile.putBool(conditionUnion.partyIsRidingCondition->vehicleReferencedInVariable);
	}
	else if (type == PREVIOUS_SELECTED_AREA)
		storageFile.putString(conditionUnion.previousSelectedAreaCondition->objectName.toUtf8().data());
	else if (type == CURRENT_HIGHLIGHTED_AREA)
		storageFile.putString(conditionUnion.currentHighlightedAreaCondition->objectName.toUtf8().data());
}

QString BaseEvent::ConditionalBranch_Menu::Condition::getViewableText(int indent) const
{
	QString viewableText = "";
	for (int i = 0; i < indent; ++i)
		viewableText += "  ";
	viewableText += "::";
	if (type == SWITCH)
	{
		viewableText += "Switch: ";
		if (conditionUnion.switchCondition->switchReferencedInVariable)
			viewableText += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(conditionUnion.switchCondition->switchID));
		else
			viewableText += QString("switch[%1]").arg(ProjectData::switchVariableIdToString(conditionUnion.switchCondition->switchID));
		if (conditionUnion.switchCondition->isOn)
			viewableText += " Is ON";
		else
			viewableText += " Is OFF";
	}
	else if (type == VARIABLE)
	{
		viewableText += "Variable: ";
		viewableText += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(conditionUnion.variableCondition->srcVariableID));
		if (conditionUnion.variableCondition->comparison == VariableCondition::EQUAL_TO)
			viewableText += " == ";
		if (conditionUnion.variableCondition->comparison == VariableCondition::NOT_EQUAL_TO)
			viewableText += " != ";
		if (conditionUnion.variableCondition->comparison == VariableCondition::GREATER_THAN)
			viewableText += " > ";
		if (conditionUnion.variableCondition->comparison == VariableCondition::GREATER_THAN_OR_EQUAL_TO)
			viewableText += " >= ";
		if (conditionUnion.variableCondition->comparison == VariableCondition::LESS_THAN)
			viewableText += " < ";
		if (conditionUnion.variableCondition->comparison == VariableCondition::LESS_THAN_OR_EQUAL_TO)
			viewableText += " <= ";
		if (conditionUnion.variableCondition->dstValueStoredInVariable)
			viewableText += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(conditionUnion.variableCondition->dstValue));
		else
			viewableText += QString::number(conditionUnion.variableCondition->dstValue);
	}
	else if (type == STRING)
	{
		viewableText += "String: ";
		if (conditionUnion.stringCondition->srcStringReferencedInVariable)
			viewableText += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(conditionUnion.stringCondition->srcStringID));
		else
			viewableText += QString("string[%1]").arg(ProjectData::stringVariableIdToString(conditionUnion.stringCondition->srcStringID));
		if (conditionUnion.stringCondition->comparison == StringCondition::EQUAL_TO)
			viewableText += " Is Equal To ";
		if (conditionUnion.stringCondition->comparison == StringCondition::NOT_EQUAL_TO)
			viewableText += " Isn\'t Equal To ";
		if (conditionUnion.stringCondition->comparison == StringCondition::STARTS_WITH)
			viewableText += " Starts With ";
		if (conditionUnion.stringCondition->comparison == StringCondition::ENDS_WITH)
			viewableText += " Ends With ";
		if (conditionUnion.stringCondition->comparison == StringCondition::CONTAINS)
			viewableText += " Contains ";
		if (conditionUnion.stringCondition->comparison == StringCondition::DOES_NOT_CONTAIN)
			viewableText += " Doesn\'t Contain ";
		if (conditionUnion.stringCondition->comparison == StringCondition::IS_EMPTY)
			viewableText += " Is Empty ";
		if (conditionUnion.stringCondition->comparison == StringCondition::IS_NOT_EMPTY)
			viewableText += " Isn\'t Empty ";
		if (conditionUnion.stringCondition->dstStringType == StringCondition::STRING_VARIABLE)
			viewableText += QString("string[%1]").arg(ProjectData::stringVariableIdToString(conditionUnion.stringCondition->dstStringID));
		else if (conditionUnion.stringCondition->dstStringType == StringCondition::REFERENCED_STRING)
			viewableText += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(conditionUnion.stringCondition->dstStringID));
		else
			viewableText += QString("\"%1\"").arg(conditionUnion.stringCondition->dstValue);
	}
	else if (type == TIMER)
	{
		viewableText += "Timer: ";
		if (conditionUnion.timerCondition->timerReferencedInVariable)
			viewableText += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(conditionUnion.timerCondition->timerID));
		else
			viewableText += QString::number(conditionUnion.timerCondition->timerID);
		if (conditionUnion.timerCondition->comparison == TimerCondition::EQUAL_TO)
			viewableText += " == ";
		if (conditionUnion.timerCondition->comparison == TimerCondition::NOT_EQUAL_TO)
			viewableText += " != ";
		if (conditionUnion.timerCondition->comparison == TimerCondition::GREATER_THAN)
			viewableText += " > ";
		if (conditionUnion.timerCondition->comparison == TimerCondition::GREATER_THAN_OR_EQUAL_TO)
			viewableText += " >= ";
		if (conditionUnion.timerCondition->comparison == TimerCondition::LESS_THAN)
			viewableText += " < ";
		if (conditionUnion.timerCondition->comparison == TimerCondition::LESS_THAN_OR_EQUAL_TO)
			viewableText += " <= ";
		if (conditionUnion.timerCondition->timeStoredInVariable)
			viewableText += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(conditionUnion.timerCondition->seconds));
		else
			viewableText += QString("%1:%2").arg(conditionUnion.timerCondition->minutes).arg(conditionUnion.timerCondition->seconds);
	}
	else if (type == MONEY)
	{
		viewableText += "Money: ";
		if (conditionUnion.variableCondition->comparison == VariableCondition::EQUAL_TO)
			viewableText += "== ";
		if (conditionUnion.variableCondition->comparison == VariableCondition::NOT_EQUAL_TO)
			viewableText += "!= ";
		if (conditionUnion.variableCondition->comparison == VariableCondition::GREATER_THAN)
			viewableText += "> ";
		if (conditionUnion.variableCondition->comparison == VariableCondition::GREATER_THAN_OR_EQUAL_TO)
			viewableText += ">= ";
		if (conditionUnion.variableCondition->comparison == VariableCondition::LESS_THAN)
			viewableText += "< ";
		if (conditionUnion.variableCondition->comparison == VariableCondition::LESS_THAN_OR_EQUAL_TO)
			viewableText += "<= ";
		if (conditionUnion.moneyCondition->valueStoredInVariable)
			viewableText += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(conditionUnion.moneyCondition->value));
		else
			viewableText += QString::number(conditionUnion.moneyCondition->value);
	}
	else if (type == ITEM)
	{
		viewableText += "Item: ";
		if (conditionUnion.itemCondition->partyPossesses)
			viewableText += "Party Possesses ";
		else
			viewableText += "Party Doesn't Possess ";
		if (conditionUnion.itemCondition->itemReferencedInVariable)
			viewableText += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(conditionUnion.itemCondition->itemID));
		else
			viewableText += ProjectData::itemNames[conditionUnion.itemCondition->itemID];
	}
	else if (type == HERO)
	{
		viewableText += "Hero: ";
		if (conditionUnion.heroCondition->characterReferencedInVariable)
			viewableText += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(conditionUnion.heroCondition->characterID));
		else
			viewableText += ProjectData::characterNames[conditionUnion.heroCondition->characterID];
		if (conditionUnion.heroCondition->conditionType == HeroCondition::IN_PARTY)
			viewableText += " Is In Party";
		else if (conditionUnion.heroCondition->conditionType == HeroCondition::NAME_IS)
		{
			viewableText += "\'s Name Is ";
			if (conditionUnion.heroCondition->valueStoredInVariable)
				viewableText += QString("string[%1]").arg(ProjectData::stringVariableIdToString(conditionUnion.heroCondition->value));
			else
				viewableText += conditionUnion.heroCondition->string;
		}
		else if (conditionUnion.heroCondition->conditionType == HeroCondition::LEVEL_IS_AT_LEAST)
		{
			viewableText += "\'s Level Is At Least ";
			viewableText += QString::number(conditionUnion.heroCondition->value);
		}
		else if (conditionUnion.heroCondition->conditionType == HeroCondition::HP_IS_AT_LEAST)
		{
			viewableText += "\'s HP Is At Least ";
			viewableText += QString::number(conditionUnion.heroCondition->value);
		}
		else if (conditionUnion.heroCondition->conditionType == HeroCondition::CAN_USE_SKILL)
		{
			viewableText += " Can Use ";
			viewableText += ProjectData::skillNames[conditionUnion.heroCondition->value];
		}
		else if (conditionUnion.heroCondition->conditionType == HeroCondition::HAS_ITEM_EQUIPPED)
		{
			viewableText += " Has ";
			viewableText += ProjectData::itemNames[conditionUnion.heroCondition->value];
			viewableText += " Equipped";
		}
		else if (conditionUnion.heroCondition->conditionType == HeroCondition::AFFLICTED_WITH_CONDITION)
		{
			viewableText += " Afflicted With ";
			viewableText += ProjectData::attributeNames[conditionUnion.heroCondition->value];
		}
	}
	else if (type == PARTY_IS_RIDING)
	{
		viewableText += "Party Is Riding: ";
		if (conditionUnion.partyIsRidingCondition->vehicleReferencedInVariable)
			viewableText += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(conditionUnion.partyIsRidingCondition->vehicleID));
		else
			viewableText += ProjectData::vehicleNames[conditionUnion.partyIsRidingCondition->vehicleID];
	}
	else if (type == PREVIOUS_SELECTED_AREA)
	{
		viewableText += "Previously Selected Area: ";
		viewableText += conditionUnion.previousSelectedAreaCondition->objectName;
	}
	else if (type == CURRENT_HIGHLIGHTED_AREA)
	{
		viewableText += "Current Highlighted Area: ";
		viewableText += conditionUnion.currentHighlightedAreaCondition->objectName;
	}
	else if (type == BGM_PLAYED_THROUGH_ONCE)
		viewableText += "BGM Has Played Through Once";
	return viewableText;
}

void BaseEvent::ConditionalBranch_Menu::Condition::setType(int value)
{
	if (type == SWITCH)
		delete (conditionUnion.switchCondition);
	else if (type == VARIABLE)
		delete (conditionUnion.variableCondition);
	else if (type == STRING)
		delete (conditionUnion.stringCondition);
	else if (type == TIMER)
		delete (conditionUnion.timerCondition);
	else if (type == MONEY)
		delete (conditionUnion.moneyCondition);
	else if (type == ITEM)
		delete (conditionUnion.itemCondition);
	else if (type == HERO)
		delete (conditionUnion.heroCondition);
	else if (type == PARTY_IS_RIDING)
		delete (conditionUnion.partyIsRidingCondition);
	else if (type == PREVIOUS_SELECTED_AREA)
		delete (conditionUnion.previousSelectedAreaCondition);
	else if (type == CURRENT_HIGHLIGHTED_AREA)
		delete (conditionUnion.currentHighlightedAreaCondition);
	type = value;
	if (type == SWITCH)
		conditionUnion.switchCondition = new SwitchCondition;
	else if (type == VARIABLE)
		conditionUnion.variableCondition = new VariableCondition;
	else if (type == STRING)
		conditionUnion.stringCondition = new StringCondition;
	else if (type == TIMER)
		conditionUnion.timerCondition = new TimerCondition;
	else if (type == MONEY)
		conditionUnion.moneyCondition = new MoneyCondition;
	else if (type == ITEM)
		conditionUnion.itemCondition = new ItemCondition;
	else if (type == HERO)
		conditionUnion.heroCondition = new HeroCondition;
	else if (type == PARTY_IS_RIDING)
		conditionUnion.partyIsRidingCondition = new PartyIsRidingCondition;
	else if (type == PREVIOUS_SELECTED_AREA)
		conditionUnion.previousSelectedAreaCondition = new PreviousSelectedAreaCondition;
	else if (type == CURRENT_HIGHLIGHTED_AREA)
		conditionUnion.currentHighlightedAreaCondition = new CurrentHighlightedAreaCondition;
}

void BaseEvent::ConditionalBranch_Menu::Condition::getSwitchCondition(SwitchCondition *other)
{
	other->switchID = conditionUnion.switchCondition->switchID;
	other->switchReferencedInVariable = conditionUnion.switchCondition->switchReferencedInVariable;
	other->isOn = conditionUnion.switchCondition->isOn;
}

void BaseEvent::ConditionalBranch_Menu::Condition::setSwitchCondition(SwitchCondition *other)
{
	conditionUnion.switchCondition->switchID = other->switchID;
	conditionUnion.switchCondition->switchReferencedInVariable = other->switchReferencedInVariable;
	conditionUnion.switchCondition->isOn = other->isOn;
}

void BaseEvent::ConditionalBranch_Menu::Condition::getVariableCondition(VariableCondition *other)
{
	other->srcVariableID = conditionUnion.variableCondition->srcVariableID;
	other->dstValue = conditionUnion.variableCondition->dstValue;
	other->comparison = conditionUnion.variableCondition->comparison;
	other->dstValueStoredInVariable = conditionUnion.variableCondition->dstValueStoredInVariable;
}

void BaseEvent::ConditionalBranch_Menu::Condition::setVariableCondition(VariableCondition *other)
{
	conditionUnion.variableCondition->srcVariableID = other->srcVariableID;
	conditionUnion.variableCondition->dstValue = other->dstValue;
	conditionUnion.variableCondition->comparison = other->comparison;
	conditionUnion.variableCondition->dstValueStoredInVariable = other->dstValueStoredInVariable;
}

void BaseEvent::ConditionalBranch_Menu::Condition::getStringCondition(StringCondition *other)
{
	other->dstValue = conditionUnion.stringCondition->dstValue;
	other->srcStringID = conditionUnion.stringCondition->srcStringID;
	other->dstStringID = conditionUnion.stringCondition->dstStringID;
	other->comparison = conditionUnion.stringCondition->comparison;
	other->dstStringType = conditionUnion.stringCondition->dstStringType;
	other->srcStringReferencedInVariable = conditionUnion.stringCondition->srcStringReferencedInVariable;
}

void BaseEvent::ConditionalBranch_Menu::Condition::setStringCondition(StringCondition *other)
{
	conditionUnion.stringCondition->dstValue = other->dstValue;
	conditionUnion.stringCondition->srcStringID = other->srcStringID;
	conditionUnion.stringCondition->dstStringID = other->dstStringID;
	conditionUnion.stringCondition->comparison = other->comparison;
	conditionUnion.stringCondition->dstStringType = other->dstStringType;
	conditionUnion.stringCondition->srcStringReferencedInVariable = other->srcStringReferencedInVariable;
}

void BaseEvent::ConditionalBranch_Menu::Condition::getTimerCondition(TimerCondition *other)
{
	other->timerID = conditionUnion.timerCondition->timerID;
	other->minutes = conditionUnion.timerCondition->minutes;
	other->seconds = conditionUnion.timerCondition->seconds;
	other->comparison = conditionUnion.timerCondition->comparison;
	other->timerReferencedInVariable = conditionUnion.timerCondition->timerReferencedInVariable;
	other->timeStoredInVariable = conditionUnion.timerCondition->timeStoredInVariable;
}

void BaseEvent::ConditionalBranch_Menu::Condition::setTimerCondition(TimerCondition *other)
{
	conditionUnion.timerCondition->timerID = other->timerID;
	conditionUnion.timerCondition->minutes = other->minutes;
	conditionUnion.timerCondition->seconds = other->seconds;
	conditionUnion.timerCondition->comparison = other->comparison;
	conditionUnion.timerCondition->timerReferencedInVariable = other->timerReferencedInVariable;
	conditionUnion.timerCondition->timeStoredInVariable = other->timeStoredInVariable;
}

void BaseEvent::ConditionalBranch_Menu::Condition::getMoneyCondition(MoneyCondition *other)
{
	other->value = conditionUnion.moneyCondition->value;
	other->comparison = conditionUnion.moneyCondition->comparison;
	other->valueStoredInVariable = conditionUnion.moneyCondition->valueStoredInVariable;
}

void BaseEvent::ConditionalBranch_Menu::Condition::setMoneyCondition(MoneyCondition *other)
{
	conditionUnion.moneyCondition->value = other->value;
	conditionUnion.moneyCondition->comparison = other->comparison;
	conditionUnion.moneyCondition->valueStoredInVariable = other->valueStoredInVariable;
}

void BaseEvent::ConditionalBranch_Menu::Condition::getItemCondition(ItemCondition *other)
{
	other->itemID = conditionUnion.itemCondition->itemID;
	other->itemReferencedInVariable = conditionUnion.itemCondition->itemReferencedInVariable;
	other->partyPossesses = conditionUnion.itemCondition->partyPossesses;
}

void BaseEvent::ConditionalBranch_Menu::Condition::setItemCondition(ItemCondition *other)
{
	conditionUnion.itemCondition->itemID = other->itemID;
	conditionUnion.itemCondition->itemReferencedInVariable = other->itemReferencedInVariable;
	conditionUnion.itemCondition->partyPossesses = other->partyPossesses;
}

void BaseEvent::ConditionalBranch_Menu::Condition::getHeroCondition(HeroCondition *other)
{
	other->string = conditionUnion.heroCondition->string;
	other->value = conditionUnion.heroCondition->value;
	other->characterID = conditionUnion.heroCondition->characterID;
	other->conditionType = conditionUnion.heroCondition->conditionType;
	other->characterReferencedInVariable = conditionUnion.heroCondition->characterReferencedInVariable;
	other->valueStoredInVariable = conditionUnion.heroCondition->valueStoredInVariable;
}

void BaseEvent::ConditionalBranch_Menu::Condition::setHeroCondition(HeroCondition *other)
{
	conditionUnion.heroCondition->string = other->string;
	conditionUnion.heroCondition->value = other->value;
	conditionUnion.heroCondition->characterID = other->characterID;
	conditionUnion.heroCondition->conditionType = other->conditionType;
	conditionUnion.heroCondition->characterReferencedInVariable = other->characterReferencedInVariable;
	conditionUnion.heroCondition->valueStoredInVariable = other->valueStoredInVariable;
}

void BaseEvent::ConditionalBranch_Menu::Condition::getPartyIsRidingCondition(PartyIsRidingCondition *other)
{
	other->vehicleID = conditionUnion.partyIsRidingCondition->vehicleID;
	other->vehicleReferencedInVariable = conditionUnion.partyIsRidingCondition->vehicleReferencedInVariable;
}

void BaseEvent::ConditionalBranch_Menu::Condition::setPartyIsRidingCondition(PartyIsRidingCondition *other)
{
	conditionUnion.partyIsRidingCondition->vehicleID = other->vehicleID;
	conditionUnion.partyIsRidingCondition->vehicleReferencedInVariable = other->vehicleReferencedInVariable;
}

void BaseEvent::ConditionalBranch_Menu::Condition::getPreviousSelectedAreaCondition(PreviousSelectedAreaCondition *other)
{
	other->objectName = conditionUnion.previousSelectedAreaCondition->objectName;
}

void BaseEvent::ConditionalBranch_Menu::Condition::setPreviousSelectedAreaCondition(PreviousSelectedAreaCondition *other)
{
	conditionUnion.previousSelectedAreaCondition->objectName = other->objectName;
}

void BaseEvent::ConditionalBranch_Menu::Condition::getCurrentHighlightedAreaCondition(CurrentHighlightedAreaCondition *other)
{
	other->objectName = conditionUnion.currentHighlightedAreaCondition->objectName;
}

void BaseEvent::ConditionalBranch_Menu::Condition::setCurrentHighlightedAreaCondition(CurrentHighlightedAreaCondition *other)
{
	conditionUnion.currentHighlightedAreaCondition->objectName = other->objectName;
}

void BaseEvent::ConditionalBranch_Menu::Condition::applyChangeMenuObjectName(QString oldName, QString newName)
{
	if (type == PREVIOUS_SELECTED_AREA)
	{
		if (conditionUnion.previousSelectedAreaCondition->objectName == oldName)
			conditionUnion.previousSelectedAreaCondition->objectName = newName;
	}
	else if (type == CURRENT_HIGHLIGHTED_AREA)
	{
		if (conditionUnion.currentHighlightedAreaCondition->objectName == oldName)
			conditionUnion.currentHighlightedAreaCondition->objectName = newName;
	}
}

bool BaseEvent::ConditionalBranch_Menu::Condition::tryLoad(XMLNode ConditionNode)
{
	QStringList tempStringList;
	QStringList trueFalseList = QStringList() << "true" << "false";
	QString tempString;
	int conditionType;
	tempStringList.clear();
	tempStringList << "switch" << "variable" << "string" << "timer" << "money" << "item" << "hero" << "partyIsRiding" << "previousSelectedArea" << "bgmPlayedThroughOnce";
	if (!XMLTools::attributeExists(ConditionNode, "type"))
		return false;
	if (!XMLTools::attributeStringValid(ConditionNode, "type", tempStringList))
		return false;
	conditionType = tempStringList.indexOf(QString(ConditionNode.getAttribute("type")));
	if (conditionType == SWITCH)
	{
		if (!XMLTools::attributeExists(ConditionNode, "switchID"))
			return false;
		if (!XMLTools::attributeExists(ConditionNode, "switchReferencedInVariable"))
			return false;
		if (!XMLTools::attributeStringValid(ConditionNode, "switchReferencedInVariable", trueFalseList, 1))
			return false;
		if (!XMLTools::attributeExists(ConditionNode, "isOn"))
			return false;
		if (!XMLTools::attributeStringValid(ConditionNode, "isOn", trueFalseList, 1))
			return false;
	}
	else if (conditionType == VARIABLE)
	{
		if (!XMLTools::attributeExists(ConditionNode, "comparison"))
			return false;
		tempStringList.clear();
		tempStringList << "==" << "!=" << ">" << ">=" << "<" << "<=";
		if (!XMLTools::attributeStringValid(ConditionNode, "comparison", tempStringList))
			return false;
		if (!XMLTools::attributeExists(ConditionNode, "srcVariableID"))
			return false;
		if (!XMLTools::attributeExists(ConditionNode, "dstValue"))
			return false;
		if (!XMLTools::attributeExists(ConditionNode, "dstValueStoredInVariable"))
			return false;
		if (!XMLTools::attributeStringValid(ConditionNode, "dstValueStoredInVariable", trueFalseList, 1))
			return false;
	}
	else if (conditionType == STRING)
	{
		if (!XMLTools::attributeExists(ConditionNode, "comparison"))
			return false;
		tempStringList.clear();
		tempStringList << "==" << "!=" << "startsWith" << "endsWith" << "contains" << "doesNotContain" << "isEmpty" << "isNotEmpty";
		if (!XMLTools::attributeStringValid(ConditionNode, "comparison", tempStringList))
			return false;
		if (!XMLTools::attributeExists(ConditionNode, "srcStringID"))
			return false;
		if (!XMLTools::attributeExists(ConditionNode, "srcStringReferencedInVariable"))
			return false;
		if (!XMLTools::attributeStringValid(ConditionNode, "srcStringReferencedInVariable", trueFalseList, 1))
			return false;
		if (!XMLTools::attributeExists(ConditionNode, "dstStringType"))
			return false;
		tempStringList.clear();
		tempStringList << "stringVariable" << "referencedString" << "value";
		if (!XMLTools::attributeStringValid(ConditionNode, "dstStringType", tempStringList))
			return false;
		tempString = ConditionNode.getAttribute("dstStringType");
		if (tempString == "stringVariable")
		{
			if (!XMLTools::attributeExists(ConditionNode, "dstStringID"))
				return false;
		}
		else if (tempString == "referencedString")
		{
			if (!XMLTools::attributeExists(ConditionNode, "dstVariableID"))
				return false;
		}
		else if (tempString == "value")
		{
			if (!XMLTools::attributeExists(ConditionNode, "dstValue"))
				return false;
		}
	}
	else if (conditionType == TIMER)
	{
		if (!XMLTools::attributeExists(ConditionNode, "comparison"))
			return false;
		tempStringList.clear();
		tempStringList << "==" << "!=" << ">" << ">=" << "<" << "<=";
		if (!XMLTools::attributeStringValid(ConditionNode, "comparison", tempStringList))
			return false;
		if (!XMLTools::attributeExists(ConditionNode, "timerID"))
			return false;
		if (!XMLTools::attributeExists(ConditionNode, "timerReferencedInVariable"))
			return false;
		if (!XMLTools::attributeStringValid(ConditionNode, "timerReferencedInVariable", trueFalseList, 1))
			return false;
		if (!XMLTools::attributeExists(ConditionNode, "timeStoredInVariable"))
			return false;
		if (!XMLTools::attributeStringValid(ConditionNode, "timeStoredInVariable", trueFalseList, 1))
			return false;
		if (QString(ConditionNode.getAttribute("timeStoredInVariable")) == "true")
		{
			if (!XMLTools::attributeExists(ConditionNode, "variableID"))
				return false;
		}
		else
		{
			if (!XMLTools::attributeExists(ConditionNode, "minutes"))
				return false;
			if (!XMLTools::attributeExists(ConditionNode, "seconds"))
				return false;
			if (!XMLTools::attributeIntRangeValid(ConditionNode, "seconds", 0, 59))
				return false;
		}
	}
	else if (conditionType == MONEY)
	{
		if (!XMLTools::attributeExists(ConditionNode, "comparison"))
			return false;
		tempStringList.clear();
		tempStringList << "==" << "!=" << ">" << ">=" << "<" << "<=";
		if (!XMLTools::attributeStringValid(ConditionNode, "comparison", tempStringList))
			return false;
		if (!XMLTools::attributeExists(ConditionNode, "value"))
			return false;
		if (!XMLTools::attributeExists(ConditionNode, "valueStoredInVariable"))
			return false;
		if (!XMLTools::attributeStringValid(ConditionNode, "valueStoredInVariable", trueFalseList, 1))
			return false;
	}
	else if (conditionType == ITEM)
	{
		if (!XMLTools::attributeExists(ConditionNode, "itemID"))
			return false;
		if (!XMLTools::attributeExists(ConditionNode, "itemReferencedInVariable"))
			return false;
		if (!XMLTools::attributeStringValid(ConditionNode, "itemReferencedInVariable", trueFalseList, 1))
			return false;
		if (!XMLTools::attributeExists(ConditionNode, "partyPossesses"))
			return false;
		if (!XMLTools::attributeStringValid(ConditionNode, "partyPossesses", trueFalseList, 1))
			return false;
	}
	else if (conditionType == HERO)
	{
		int heroConditionType;
		if (!XMLTools::attributeExists(ConditionNode, "heroConditionType"))
			return false;
		tempStringList.clear();
		tempStringList << "inParty" << "nameIs" << "levelIsAtLeast" << "hpIsAtLeast" << "canUseSkill" << "hasItemEquipped" << "afflictedWithCondition";
		if (!XMLTools::attributeStringValid(ConditionNode, "heroConditionType", tempStringList))
			return false;
		if (!XMLTools::attributeExists(ConditionNode, "characterID"))
			return false;
		if (!XMLTools::attributeExists(ConditionNode, "characterReferencedInVariable"))
			return false;
		if (!XMLTools::attributeStringValid(ConditionNode, "characterReferencedInVariable", trueFalseList, 1))
			return false;
		heroConditionType = tempStringList.indexOf(QString(ConditionNode.getAttribute("heroConditionType")));
		if (heroConditionType == HeroCondition::NAME_IS)
		{
			if (!XMLTools::attributeExists(ConditionNode, "string"))
				return false;
			if (!XMLTools::attributeExists(ConditionNode, "stringStoredInVariable"))
				return false;
			if (!XMLTools::attributeStringValid(ConditionNode, "stringStoredInVariable", trueFalseList, 1))
				return false;
		}
		else if (heroConditionType == HeroCondition::LEVEL_IS_AT_LEAST)
		{
			if (!XMLTools::attributeExists(ConditionNode, "level"))
				return false;
			if (!XMLTools::attributeExists(ConditionNode, "levelStoredInVariable"))
				return false;
			if (!XMLTools::attributeStringValid(ConditionNode, "levelStoredInVariable", trueFalseList, 1))
				return false;
			if (QString(ConditionNode.getAttribute("levelStoredInVariable")) != "true")
			{
				if (!XMLTools::attributeIntRangeValid(ConditionNode, "level", 1, 99))
					return false;
			}
		}
		else if (heroConditionType == HeroCondition::HP_IS_AT_LEAST)
		{
			if (!XMLTools::attributeExists(ConditionNode, "hp"))
				return false;
			if (!XMLTools::attributeExists(ConditionNode, "hpStoredInVariable"))
				return false;
			if (!XMLTools::attributeStringValid(ConditionNode, "hpStoredInVariable", trueFalseList, 1))
				return false;
			if (QString(ConditionNode.getAttribute("hpStoredInVariable")) != "true")
			{
				if (!XMLTools::attributeIntRangeValid(ConditionNode, "hp", 1, 9999))
					return false;
			}
		}
		else if (heroConditionType == HeroCondition::CAN_USE_SKILL)
		{
			if (!XMLTools::attributeExists(ConditionNode, "skillID"))
				return false;
			if (!XMLTools::attributeExists(ConditionNode, "skillReferencedInVariable"))
				return false;
			if (!XMLTools::attributeStringValid(ConditionNode, "skillReferencedInVariable", trueFalseList, 1))
				return false;
		}
		else if (heroConditionType == HeroCondition::HAS_ITEM_EQUIPPED)
		{
			if (!XMLTools::attributeExists(ConditionNode, "itemID"))
				return false;
			if (!XMLTools::attributeExists(ConditionNode, "itemReferencedInVariable"))
				return false;
			if (!XMLTools::attributeStringValid(ConditionNode, "itemReferencedInVariable", trueFalseList, 1))
				return false;
		}
		else if (heroConditionType == HeroCondition::AFFLICTED_WITH_CONDITION)
		{
			if (!XMLTools::attributeExists(ConditionNode, "conditionID"))
				return false;
			if (!XMLTools::attributeExists(ConditionNode, "conditionReferencedInVariable"))
				return false;
			if (!XMLTools::attributeStringValid(ConditionNode, "conditionReferencedInVariable", trueFalseList, 1))
				return false;
		}
	}
	else if (conditionType == PARTY_IS_RIDING)
	{
		if (!XMLTools::attributeExists(ConditionNode, "vehicleID"))
			return false;
		if (!XMLTools::attributeExists(ConditionNode, "vehicleReferencedInVariable"))
			return false;
		if (!XMLTools::attributeStringValid(ConditionNode, "vehicleReferencedInVariable", trueFalseList, 1))
			return false;
	}
	else if (conditionType == PREVIOUS_SELECTED_AREA || conditionType == CURRENT_HIGHLIGHTED_AREA)
	{
		if (!XMLTools::attributeExists(ConditionNode, "objectName"))
			return false;
	}
	return true;
}
//END BaseEvent::ConditionBranch_Menu::Condition Member Functions

//BEGIN BaseEvent::DisplayMessage Member Functions
XMLNode BaseEvent::DisplayMessage::getEventNode()
{
	XMLNode TempNode1, EventNode = XMLNode::createXMLTopNode("event");
	QStringList messageLines;
	QString location = "";
	EventNode.addAttribute("type", "type");
	EventNode.addAttribute("fontID", QString::number(fontID).toUtf8().data());
	EventNode.addAttribute("topColor", QString("#%1%2%3").arg(topColor.red(), 2, 16, QChar('0')).arg(topColor.green(), 2, 16, QChar('0')).arg(topColor.blue(), 2, 16, QChar('0')).toUtf8().data());
	EventNode.addAttribute("bottomColor", QString("#%1%2%3").arg(bottomColor.red(), 2, 16, QChar('0')).arg(bottomColor.green(), 2, 16, QChar('0')).arg(bottomColor.blue(), 2, 16, QChar('0')).toUtf8().data());
	TempNode1 = EventNode.addChild("displayOptions");
	TempNode1.addAttribute("x", QString::number(x).toUtf8().data());
	TempNode1.addAttribute("y", QString::number(y).toUtf8().data());
	TempNode1.addAttribute("coordinatesStoredInVariables", (coordinatesStoredInVariables) ? "true":"false");
	TempNode1.addAttribute("width", QString::number(width).toUtf8().data());
	TempNode1.addAttribute("height", QString::number(height).toUtf8().data());
	TempNode1.addAttribute("sizeStoredInVariables", (sizeStoredInVariables) ? "true":"false");
	TempNode1.addAttribute("xPadding", QString::number(xPadding).toUtf8().data());
	TempNode1.addAttribute("yPadding", QString::number(yPadding).toUtf8().data());
	TempNode1.addAttribute("opacity", QString::number(opacity).toUtf8().data());
	TempNode1.addAttribute("opacityStoredInVariable", (opacityStoredInVariable) ? "true":"false");
	TempNode1.addAttribute("blockEvents", (blockEvents) ? "true":"false");
	TempNode1 = EventNode.addChild("faceGraphic");
	TempNode1.addAttribute("location", ProjectData::getRelativeResourcePath(faceLocation).toUtf8().data());
	TempNode1.addAttribute("displaySide", (faceOnLeft) ? "left":"right");
	TempNode1.addAttribute("flipHorizontaly", (flipFaceHorizontaly) ? "true":"false");
	messageLines = message.split(QChar('\n'));
	for (int i = 0; i < messageLines.size(); ++i)
		EventNode.addChild("line").addText(messageLines[i].toUtf8().data());
	return EventNode;
}

void BaseEvent::DisplayMessage::saveToStorageFile(StorageFile &storageFile)
{
	unsigned long color;
	storageFile.putString(faceLocation.toUtf8().data());
	storageFile.putString(message.toUtf8().data());
	color = (((unsigned long)topColor.red()) << 16) + (((unsigned long)topColor.green()) << 8) + (unsigned long)topColor.blue();
	storageFile.putUnsignedLong(color);
	color = (((unsigned long)bottomColor.red()) << 16) + (((unsigned long)bottomColor.green()) << 8) + (unsigned long)bottomColor.blue();
	storageFile.putUnsignedLong(color);
	storageFile.putSignedInt(x);
	storageFile.putSignedInt(y);
	storageFile.putSignedInt(width);
	storageFile.putSignedInt(height);
	storageFile.putSignedInt(xPadding);
	storageFile.putSignedInt(yPadding);
	storageFile.putSignedInt(opacity);
	storageFile.putSignedInt(fontID);
	storageFile.putBool(coordinatesStoredInVariables);
	storageFile.putBool(sizeStoredInVariables);
	storageFile.putBool(opacityStoredInVariable);
	storageFile.putBool(blockEvents);
	storageFile.putBool(flipFaceHorizontaly);
	storageFile.putBool(faceOnLeft);
}

QStringList BaseEvent::DisplayMessage::getResources()
{
	return QStringList() << faceLocation;
}

QStringList BaseEvent::DisplayMessage::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	QStringList messageLines = message.split(QChar('\n'));
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Display Message: ";
	for (int i = 0; i < messageLines.size(); ++i)
	{
		if (i > 0)
		{
			viewableText[0] += "\n";
			for (int j = 0; j < indent; ++j)
				viewableText[0] += "  ";
			viewableText[0] += "                    ";
		}
		viewableText[0] += messageLines[i];
	}
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::DisplayMessage::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	ConfigureEvent_DisplayMessage *configMessage = new ConfigureEvent_DisplayMessage();
	bool configured = false;
	configMessage->setFaceLocation(faceLocation);
	configMessage->setMessage(message);
	configMessage->setColor(topColor, bottomColor);
	configMessage->setCoordinatesStoredInVariables(coordinatesStoredInVariables);
	configMessage->setSizeStoredInVariables(sizeStoredInVariables);
	configMessage->setOpacityStoredInVariable(opacityStoredInVariable);
	configMessage->setX(x);
	configMessage->setY(y);
	configMessage->setWidth(width);
	configMessage->setHeight(height);
	configMessage->setXPadding(xPadding);
	configMessage->setYPadding(yPadding);
	configMessage->setOpacity(opacity);
	configMessage->setFontID(fontID);
	configMessage->setBlockEvents(blockEvents);
	configMessage->setFlipFaceHorizontaly(flipFaceHorizontaly);
	configMessage->setFaceDisplayedOnLeft(faceOnLeft);
	if (configMessage->exec())
	{
		configured = true;
		faceLocation = configMessage->getFaceLocation();
		message = configMessage->getMessage();
		topColor = configMessage->getTopColor();
		bottomColor = configMessage->getBottomColor();
		x = configMessage->getX();
		y = configMessage->getY();
		width = configMessage->getWidth();
		height = configMessage->getHeight();
		xPadding = configMessage->getXPadding();
		yPadding = configMessage->getYPadding();
		opacity = configMessage->getOpacity();
		fontID = configMessage->getFontID();
		coordinatesStoredInVariables = configMessage->getCoordinatesStoredInVariables();
		sizeStoredInVariables = configMessage->getSizeStoredInVariables();
		opacityStoredInVariable = configMessage->getOpacityStoredInVariable();
		blockEvents = configMessage->getBlockEvents();
		flipFaceHorizontaly = configMessage->getFlipFaceHorizontaly();
		faceOnLeft = configMessage->getFaceDisplayedOnLeft();
	}
	delete configMessage;
	return configured;
}
#endif

bool BaseEvent::DisplayMessage::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	XMLNode TempNode1;
	QStringList trueFalseList = QStringList() << "true" << "false";
	if (!XMLTools::attributeExists(EventNode, "fontID"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "topColor"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "bottomColor"))
		return false;
	if (!XMLTools::nodeExists(EventNode, "displayOptions"))
		return false;
	TempNode1 = EventNode.getChildNode("displayOptions");
	if (!XMLTools::attributeExists(TempNode1, "x"))
		return false;
	if (!XMLTools::attributeExists(TempNode1, "y"))
		return false;
	if (!XMLTools::attributeExists(TempNode1, "coordinatesStoredInVariables"))
		return false;
	if (!XMLTools::attributeStringValid(TempNode1, "coordinatesStoredInVariables", trueFalseList, 1))
		return false;
	if (!XMLTools::attributeExists(TempNode1, "width"))
		return false;
	if (!XMLTools::attributeExists(TempNode1, "height"))
		return false;
	if (!XMLTools::attributeExists(TempNode1, "sizeStoredInVariables"))
		return false;
	if (!XMLTools::attributeStringValid(TempNode1, "sizeStoredInVariables", trueFalseList, 1))
		return false;
	if (!XMLTools::attributeExists(TempNode1, "xPadding"))
		return false;
	if (!XMLTools::attributeIntRangeValid(TempNode1, "xPadding", 0, 32))
		return false;
	if (!XMLTools::attributeExists(TempNode1, "yPadding"))
		return false;
	if (!XMLTools::attributeIntRangeValid(TempNode1, "yPadding", 0, 32))
		return false;
	if (!XMLTools::attributeExists(TempNode1, "opacity"))
		return false;
	if (!XMLTools::attributeExists(TempNode1, "opacityStoredInVariable"))
		return false;
	if (!XMLTools::attributeStringValid(TempNode1, "opacityStoredInVariable", trueFalseList, 1))
		return false;
	if (!XMLTools::attributeExists(TempNode1, "blockEvents"))
		return false;
	if (!XMLTools::attributeStringValid(TempNode1, "blockEvents", trueFalseList, 1))
		return false;
	if (!XMLTools::nodeExists(EventNode, "faceGraphic"))
		return false;
	TempNode1 = EventNode.getChildNode("faceGraphic");
	if (!XMLTools::attributeExists(TempNode1, "location"))
		return false;
	if (!XMLTools::attributeExists(TempNode1, "displaySide"))
		return false;
	if (!XMLTools::attributeStringValid(TempNode1, "displaySide", QStringList() << "left" << "right"))
		return false;
	if (!XMLTools::attributeExists(TempNode1, "flipHorizontaly"))
		return false;
	if (!XMLTools::attributeStringValid(TempNode1, "flipHorizontaly", trueFalseList, 1))
		return false;
	return true;
}

BaseEvent::DisplayMessage::DisplayMessage(BaseEvent *pBase) : Event(pBase)
{
	faceLocation = "";
	message = "";
	topColor = ProjectData::systemGraphics.defaultFontEnabledTopColor;
	bottomColor = ProjectData::systemGraphics.defaultFontEnabledBottomColor;
	x = 0;
	y = 0;
	width = 640;
	height = 4;
	xPadding = 8;
	yPadding = 8;
	opacity = 100;
	fontID = ProjectData::defaultFontID;
	coordinatesStoredInVariables = false;
	sizeStoredInVariables = false;
	opacityStoredInVariable = false;
	blockEvents = true;
	flipFaceHorizontaly = false;
	faceOnLeft = true;
}

BaseEvent::DisplayMessage::DisplayMessage(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	XMLNode TempNode1;
	QString tempString;
	fontID = QString(EventNode.getAttribute("fontID")).toInt();
	tempString = EventNode.getAttribute("topColor");
	topColor = QColor(tempString.mid(1, 2).toInt(0, 16), tempString.mid(3, 2).toInt(0, 16), tempString.mid(5, 2).toInt(0, 16));
	tempString = EventNode.getAttribute("bottomColor");
	bottomColor = QColor(tempString.mid(1, 2).toInt(0, 16), tempString.mid(3, 2).toInt(0, 16), tempString.mid(5, 2).toInt(0, 16));
	TempNode1 = EventNode.getChildNode("displayOptions");
	x = QString(TempNode1.getAttribute("x")).toInt();
	y = QString(TempNode1.getAttribute("y")).toInt();
	coordinatesStoredInVariables = (QString(TempNode1.getAttribute("coordinatesStoredInVariables")) == "true") ? true:false;
	width = QString(TempNode1.getAttribute("width")).toInt();
	height = QString(TempNode1.getAttribute("height")).toInt();
	sizeStoredInVariables = (QString(TempNode1.getAttribute("sizeStoredInVariables")) == "true") ? true:false;
	xPadding = QString(TempNode1.getAttribute("xPadding")).toInt();
	yPadding = QString(TempNode1.getAttribute("yPadding")).toInt();
	opacity = QString(TempNode1.getAttribute("opacity")).toInt();
	opacityStoredInVariable = (QString(TempNode1.getAttribute("opacityStoredInVariable")) == "true") ? true:false;
	blockEvents = (QString(TempNode1.getAttribute("blockEvents")) == "true") ? true:false;
	TempNode1 = EventNode.getChildNode("faceGraphic");
	faceLocation = ProjectData::getAbsoluteResourcePath(TempNode1.getAttribute("location"));
	faceOnLeft = (QString(TempNode1.getAttribute("displaySide")) == "left") ? true:false;
	flipFaceHorizontaly = (QString(TempNode1.getAttribute("flipFaceHorizontaly")) == "true") ? true:false;
	message = "";
	for (int i = 0, count = EventNode.nChildNode("line"); i < count; ++i)
	{
		message += EventNode.getChildNode("line", i).getText();
		if (i < count - 1)
			message += "\n";
	}
}

BaseEvent::DisplayMessage::DisplayMessage(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	unsigned long color;
	char *temp_string;
	temp_string = storageFile.getString();
	faceLocation = temp_string;
	delete[] temp_string;
	temp_string = storageFile.getString();
	message = temp_string;
	delete[] temp_string;
	color = storageFile.getUnsignedLong();
	topColor = QColor((int)((color >> 16) % 256), (int)((color >> 8) % 256), (int)(color % 256));
	color = storageFile.getUnsignedLong();
	bottomColor = QColor((int)((color >> 16) % 256), (int)((color >> 8) % 256), (int)(color % 256));
	x = storageFile.getSignedInt();
	y = storageFile.getSignedInt();
	width = storageFile.getSignedInt();
	height = storageFile.getSignedInt();
	xPadding = storageFile.getSignedInt();
	yPadding = storageFile.getSignedInt();
	opacity = storageFile.getSignedInt();
	fontID = storageFile.getSignedInt();
	coordinatesStoredInVariables = storageFile.getBool();
	sizeStoredInVariables = storageFile.getBool();
	opacityStoredInVariable = storageFile.getBool();
	blockEvents = storageFile.getBool();
	flipFaceHorizontaly = storageFile.getBool();
	faceOnLeft = storageFile.getBool();
}

BaseEvent::DisplayMessage::DisplayMessage(DisplayMessage *other, BaseEvent *pBase) : Event(pBase)
{
	faceLocation = other->faceLocation;
	message = other->message;
	topColor = other->topColor;
	bottomColor = other->bottomColor;
	x = other->x;
	y = other->y;
	width = other->width;
	height = other->height;
	xPadding = other->xPadding;
	yPadding = other->yPadding;
	opacity = other->opacity;
	fontID = other->fontID;
	coordinatesStoredInVariables = other->coordinatesStoredInVariables;
	sizeStoredInVariables = other->sizeStoredInVariables;
	opacityStoredInVariable = other->opacityStoredInVariable;
	blockEvents = other->blockEvents;
	flipFaceHorizontaly = other->flipFaceHorizontaly;
	faceOnLeft = other->faceOnLeft;
}
//END BaseEvent::DisplayMessage Member Functions

//BEGIN BaseEvent::EnableDisableObject Member Functions
XMLNode BaseEvent::EnableDisableObject::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	EventNode.addAttribute("objectName", objectName.toUtf8().data());
	EventNode.addAttribute("operation", (enable) ? "enable":"disable");
	return EventNode;
}

void BaseEvent::EnableDisableObject::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putString(objectName.toUtf8().data());
	storageFile.putBool(enable);
}

QStringList BaseEvent::EnableDisableObject::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	if (enable)
		viewableText[0] += "Enable Object: ";
	else
		viewableText[0] += "Disable Object: ";
	viewableText[0] += objectName;
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::EnableDisableObject::configureEvent(int callerID, int callerType)
{
		ConfigureEvent_EnableDisableObject *configureEnable = new ConfigureEvent_EnableDisableObject(callerID, callerType);
		bool configured = false;
		configureEnable->setMenuObject(objectName);
		configureEnable->setEnable(enable);
		if (configureEnable->exec())
		{
			configured = true;
			objectName = configureEnable->getMenuObject();
			enable = configureEnable->enable();
		}
		delete configureEnable;
		return configured;
}
#endif

bool BaseEvent::EnableDisableObject::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	QStringList tempStringList = QStringList() << "enable" << "disable";
	if (!XMLTools::attributeExists(EventNode, "objectName"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "operation"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "operation", tempStringList))
		return false;
	return true;
}

BaseEvent::EnableDisableObject::EnableDisableObject(BaseEvent *pBase) : Event(pBase)
{
	objectName = "";
	enable = true;
}

BaseEvent::EnableDisableObject::EnableDisableObject(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	objectName = EventNode.getAttribute("objectName");
	enable = (QString(EventNode.getAttribute("operation")) == "enable") ? true:false;
}

BaseEvent::EnableDisableObject::EnableDisableObject(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	char *temp_string;
	temp_string = storageFile.getString();
	objectName = temp_string;
	delete[] temp_string;
	enable = storageFile.getBool();
}

BaseEvent::EnableDisableObject::EnableDisableObject(EnableDisableObject *other, BaseEvent *pBase) : Event(pBase)
{
	objectName = other->objectName;
	enable = other->enable;
}
//END BaseEvent::EnableDisableObject Member Functions

//BEGIN BaseEvent::EnemyEncounter Member Functions
BaseEvent::EnemyEncounter::~EnemyEncounter()
{
	while (escapeEvents.size() > 0)
	{
		delete (escapeEvents.last());
		escapeEvents.removeLast();
	}
	while (defeatEvents.size() > 0)
	{
		delete (defeatEvents.last());
		defeatEvents.removeLast();
	}
	while (victoryEvents.size() > 0)
	{
		delete (victoryEvents.last());
		victoryEvents.removeLast();
	}
}

XMLNode BaseEvent::EnemyEncounter::getEventNode()
{
	XMLNode TempNode1, EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	if (specialConditions == NONE)
		EventNode.addAttribute("specialConditions", "none");
	else if (specialConditions == INITIATIVE)
		EventNode.addAttribute("specialConditions", "initiative");
	else if (specialConditions == BACK_ATTACK)
		EventNode.addAttribute("specialConditions", "backAttack");
	else if (specialConditions == SURROUND_ATTACK)
		EventNode.addAttribute("specialConditions", "surroundAttack");
	else if (specialConditions == PINCERS_ATTACK)
		EventNode.addAttribute("specialConditions", "pincersAttack");
	if (battleBackgroundType == USE_MAP_OR_TERRAIN_SETTING)
		EventNode.addAttribute("battleBackgroundType", "useMapOrTerrainSetting");
	else if (battleBackgroundType == SPECIFIC_BACKGROUND)
	{
		QString location = "";
		EventNode.addAttribute("battleBackgroundType", "specificBackground");
		EventNode.addAttribute("bgLocation", ProjectData::getRelativeResourcePath(specificBackgroundLocation).toUtf8().data());
		TempNode1 = EventNode.addChild("gridDepth");
		if (gridDepth.depth == Terrain::DEPTH_SHALLOW)
			TempNode1.addAttribute("depth", "shallow");
		else if (gridDepth.depth == Terrain::DEPTH_DEEP)
			TempNode1.addAttribute("depth", "deep");
		else if (gridDepth.depth == Terrain::DEPTH_CUSTOM)
		{
			TempNode1.addAttribute("depth", "custom");
			TempNode1.addAttribute("hTopLineLocation", QString::number(gridDepth.hTopLineLocation).toUtf8().data());
			TempNode1.addAttribute("hBottomLineLocation", QString::number(gridDepth.hBottomLineLocation).toUtf8().data());
			TempNode1.addAttribute("vMonsterLine_TopLocation", QString::number(gridDepth.vMonsterLine_TopLocation).toUtf8().data());
			TempNode1.addAttribute("vMonsterLine_BottomLocation", QString::number(gridDepth.vMonsterLine_BottomLocation).toUtf8().data());
			TempNode1.addAttribute("vMonsterLine_Spacing", QString::number(gridDepth.vMonsterLine_Spacing).toUtf8().data());
			TempNode1.addAttribute("vCharacterLine_TopLocation", QString::number(gridDepth.vCharacterLine_TopLocation).toUtf8().data());
			TempNode1.addAttribute("vCharacterLine_BottomLocation", QString::number(gridDepth.vCharacterLine_BottomLocation).toUtf8().data());
			TempNode1.addAttribute("vCharacterLine_Spacing", QString::number(gridDepth.vCharacterLine_Spacing).toUtf8().data());
		}
	}
	else if (battleBackgroundType == USE_TERRAIN_BACKGROUND)
	{
		EventNode.addAttribute("battleBackgroundType", "useTerrainBackground");
		EventNode.addAttribute("terrainID", QString::number(terrainID).toUtf8().data());
	}
	TempNode1 = EventNode.addChild("victoryEvents");
	for (int i = 0; i < victoryEvents.size(); ++i)
		TempNode1.addChild(victoryEvents[i]->getEventNode());
	if (escapeHandler == DISALLOW_ESCAPE)
		EventNode.addAttribute("escapeHandler", "disallowEscape");
	else if (escapeHandler == END_EVENT_PROCESSING)
		EventNode.addAttribute("escapeHandler", "endEventProcessing");
	else if (escapeHandler == CUSTOM_ESCAPE_HANDLER)
	{
		EventNode.addAttribute("escapeHandler", "customEscapeHandler");
		TempNode1 = EventNode.addChild("customEscapeHandler");
		for (int i = 0; i < escapeEvents.size(); ++i)
			TempNode1.addChild(escapeEvents[i]->getEventNode());
	}
	if (defeatHandler == GAMEOVER)
		EventNode.addAttribute("defeatHandler", "gameOver");
	else if (defeatHandler == CUSTOM_DEFEAT_HANDLER)
	{
		EventNode.addAttribute("defeatHandler", "customDefeatHandler");
		TempNode1 = EventNode.addChild("customDefeatHandler");
		for (int i = 0; i < defeatEvents.size(); ++i)
			TempNode1.addChild(defeatEvents[i]->getEventNode());
	}
	EventNode.addAttribute("monsterGroupID", QString::number(monsterGroupID).toUtf8().data());
	EventNode.addAttribute("monsterGroupReferencedInVariable", (monsterGroupReferencedInVariable) ? "true":"false");
	EventNode.addAttribute("beginBattleWithFirstStrike", (beginBattleWithFirstStrike) ? "true":"false");
	return EventNode;
}

void BaseEvent::EnemyEncounter::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(specialConditions);
	storageFile.putSignedInt(battleBackgroundType);
	storageFile.putSignedInt(escapeHandler);
	storageFile.putSignedInt(defeatHandler);
	storageFile.putSignedInt(gridDepth.depth);
	if (battleBackgroundType == SPECIFIC_BACKGROUND)
		storageFile.putString(specificBackgroundLocation.toUtf8().data());
	else if (battleBackgroundType == USE_TERRAIN_BACKGROUND)
		storageFile.putSignedInt(terrainID);
	storageFile.putSignedInt(victoryEvents.size());
	for (int i = 0; i < victoryEvents.size(); ++i)
		victoryEvents[i]->saveToStorageFile(storageFile);
	if (escapeHandler == CUSTOM_ESCAPE_HANDLER)
	{
		storageFile.putSignedInt(escapeEvents.size());
		for (int i = 0; i < escapeEvents.size(); ++i)
			escapeEvents[i]->saveToStorageFile(storageFile);
	}
	if (defeatHandler == CUSTOM_DEFEAT_HANDLER)
	{
		storageFile.putSignedInt(defeatEvents.size());
		for (int i = 0; i < defeatEvents.size(); ++i)
			defeatEvents[i]->saveToStorageFile(storageFile);
	}
	if (gridDepth.depth == Terrain::DEPTH_CUSTOM)
	{
		storageFile.putSignedInt(gridDepth.hTopLineLocation);
		storageFile.putSignedInt(gridDepth.hBottomLineLocation);
		storageFile.putSignedInt(gridDepth.vMonsterLine_TopLocation);
		storageFile.putSignedInt(gridDepth.vMonsterLine_BottomLocation);
		storageFile.putSignedInt(gridDepth.vMonsterLine_Spacing);
		storageFile.putSignedInt(gridDepth.vCharacterLine_TopLocation);
		storageFile.putSignedInt(gridDepth.vCharacterLine_BottomLocation);
		storageFile.putSignedInt(gridDepth.vCharacterLine_Spacing);
	}
	storageFile.putSignedInt(monsterGroupID);
	storageFile.putBool(monsterGroupReferencedInVariable);
	storageFile.putBool(beginBattleWithFirstStrike);
}

QStringList BaseEvent::EnemyEncounter::getResources()
{
	QStringList resources;
	for (int i = 0; i < victoryEvents.size(); ++i)
		resources.append(victoryEvents[i]->getResources());
	if (escapeHandler == CUSTOM_ESCAPE_HANDLER)
	{
		for (int i = 0; i < escapeEvents.size(); ++i)
			resources.append(escapeEvents[i]->getResources());
	}
	if (defeatHandler == CUSTOM_DEFEAT_HANDLER)
	{
		for (int i = 0; i < defeatEvents.size(); ++i)
			resources.append(escapeEvents[i]->getResources());
	}
	return resources;
}

QStringList BaseEvent::EnemyEncounter::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Enemy Encounter: ";
	if (specialConditions == NONE)
		viewableText[0] += "Normal, ";
	else if (specialConditions == INITIATIVE)
		viewableText[0] += "Initiative, ";
	else if (specialConditions == BACK_ATTACK)
		viewableText[0] += "Back Attack, ";
	else if (specialConditions == SURROUND_ATTACK)
		viewableText[0] += "Surround Attack, ";
	else if (specialConditions == PINCERS_ATTACK)
		viewableText[0] += "Pincers Attack, ";
	if (monsterGroupReferencedInVariable)
		viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(monsterGroupID));
	else
		viewableText[0] += ProjectData::monsterGroupNames[monsterGroupID];
	if (battleBackgroundType == SPECIFIC_BACKGROUND)
	{
		QFileInfo info = QFileInfo(specificBackgroundLocation);
		viewableText[0] += ", ";
		viewableText[0] += info.baseName();
	}
	else if (battleBackgroundType == USE_TERRAIN_BACKGROUND)
	{
		viewableText[0] += ", ";
		viewableText[0] += ProjectData::terrainNames[terrainID];
	}
	if (beginBattleWithFirstStrike)
		viewableText[0] += ", First Strike";
	viewableText.append("");
	for (int i = 0; i < indent; ++i)
		viewableText[viewableText.size() - 1] += "  ";
	viewableText[viewableText.size() - 1] += "::Victory Handler";
	for (int i = 0; i < victoryEvents.size(); ++i)
		viewableText += victoryEvents[i]->getViewableText(indent + 1);
	viewableText.append("");
	for (int i = 0; i < indent + 1; ++i)
		viewableText[viewableText.size() - 1] += "  ";
	viewableText[viewableText.size() - 1] += "<>";
	if (escapeHandler == CUSTOM_ESCAPE_HANDLER)
	{
		viewableText.append("");
		for (int i = 0; i < indent; ++i)
			viewableText[viewableText.size() - 1] += "  ";
		viewableText[viewableText.size() - 1] += "::Escape Handler";
		for (int i = 0; i < escapeEvents.size(); ++i)
			viewableText += escapeEvents[i]->getViewableText(indent + 1);
		viewableText.append("");
		for (int i = 0; i < indent + 1; ++i)
			viewableText[viewableText.size() - 1] += "  ";
		viewableText[viewableText.size() - 1] += "<>";
	}
	if (defeatHandler == CUSTOM_DEFEAT_HANDLER)
	{
		viewableText.append("");
		for (int i = 0; i < indent; ++i)
			viewableText[viewableText.size() - 1] += "  ";
		viewableText[viewableText.size() - 1] += "::Defeat Handler";
		for (int i = 0; i < defeatEvents.size(); ++i)
			viewableText += defeatEvents[i]->getViewableText(indent + 1);
		viewableText.append("");
		for (int i = 0; i < indent + 1; ++i)
			viewableText[viewableText.size() - 1] += "  ";
		viewableText[viewableText.size() - 1] += "<>";
	}
	viewableText.append("");
	for (int i = 0; i < indent; ++i)
		viewableText[viewableText.size() - 1] += "  ";
	viewableText[viewableText.size() - 1] += "::End";
	return viewableText;
}

QList<BaseEvent*> BaseEvent::EnemyEncounter::getEventPointers()
{
	QList<BaseEvent*> pointers;
	for (int i = 0; i < victoryEvents.size(); ++i)
		pointers += victoryEvents[i]->getEventPointers();
	pointers.append(NULL);
	if (escapeHandler == CUSTOM_ESCAPE_HANDLER)
	{
		for (int i = 0; i < escapeEvents.size(); ++i)
			pointers += escapeEvents[i]->getEventPointers();
		pointers.append(NULL);
	}
	if (defeatHandler == CUSTOM_DEFEAT_HANDLER)
	{
		for (int i = 0; i < defeatEvents.size(); ++i)
			pointers += defeatEvents[i]->getEventPointers();
		pointers.append(NULL);
	}
	return pointers;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::EnemyEncounter::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	ConfigureEvent_EnemyEncounter *configEnemyEncounter = new ConfigureEvent_EnemyEncounter;
	bool configured = false;
	configEnemyEncounter->setSpecialConditions(specialConditions);
	configEnemyEncounter->setIsReferencedMonsterGroup(monsterGroupReferencedInVariable);
	configEnemyEncounter->setMonsterGroupID(monsterGroupID);
	configEnemyEncounter->setBattleBackgroundType(battleBackgroundType);
	if (battleBackgroundType == SPECIFIC_BACKGROUND)
	{
		configEnemyEncounter->setBackgroundLocation(specificBackgroundLocation);
		configEnemyEncounter->setGridDepth(gridDepth.depth);
		if (gridDepth.depth == Terrain::DEPTH_CUSTOM)
			configEnemyEncounter->setGridDepthValues(&gridDepth);
	}
	else if (battleBackgroundType == USE_TERRAIN_BACKGROUND)
		configEnemyEncounter->setTerrainID(terrainID);
	configEnemyEncounter->setEscapeHandler(escapeHandler);
	configEnemyEncounter->setDefeatHandler(defeatHandler);
	configEnemyEncounter->setBeginBattleWithFirstStrike(beginBattleWithFirstStrike);
	if (configEnemyEncounter->exec())
	{
		configured = true;
		specialConditions = configEnemyEncounter->getSpecialConditions();
		monsterGroupID = configEnemyEncounter->getMonsterGroupID();
		monsterGroupReferencedInVariable = configEnemyEncounter->isReferencedMonsterGroup();
		battleBackgroundType = configEnemyEncounter->getBattleBackgroundType();
		if (battleBackgroundType == SPECIFIC_BACKGROUND)
		{
			specificBackgroundLocation = configEnemyEncounter->getBackgroundLocation();
			gridDepth.depth = configEnemyEncounter->getGridDepth();
			configEnemyEncounter->getGridDepthValues(&gridDepth);
		}
		else if (battleBackgroundType == USE_TERRAIN_BACKGROUND)
			terrainID = configEnemyEncounter->getTerrainID();
		escapeHandler = configEnemyEncounter->getEscapeHandler();
		defeatHandler = configEnemyEncounter->getDefeatHandler();
		beginBattleWithFirstStrike = configEnemyEncounter->beginBattleWithFirstStrike();
	}
	delete configEnemyEncounter;
	return configured;
}
#endif

void BaseEvent::EnemyEncounter::addVictoryEvent(BaseEvent *event, int before)
{
	if (before == -1)
		victoryEvents.append(event);
	else
		victoryEvents.insert(before, event);
}

void BaseEvent::EnemyEncounter::deleteVictoryEvent(int location)
{
	delete (victoryEvents[location]);
	victoryEvents.removeAt(location);
}

void BaseEvent::EnemyEncounter::addEscapeEvent(BaseEvent *event, int before)
{
	if (before == -1)
		escapeEvents.append(event);
	else
		escapeEvents.insert(before, event);
}

void BaseEvent::EnemyEncounter::deleteEscapeEvent(int location)
{
	delete (escapeEvents[location]);
	escapeEvents.removeAt(location);
}

void BaseEvent::EnemyEncounter::addDefeatEvent(BaseEvent *event, int before)
{
	if (before == -1)
		defeatEvents.append(event);
	else
		defeatEvents.insert(before, event);
}

void BaseEvent::EnemyEncounter::deleteDefeatEvent(int location)
{
	delete (defeatEvents[location]);
	defeatEvents.removeAt(location);
}

void BaseEvent::EnemyEncounter::getGridDepth(Terrain::GridDepthValues *other)
{
	other->depth = gridDepth.depth;
	other->hTopLineLocation = gridDepth.hTopLineLocation;
	other->hBottomLineLocation = gridDepth.hBottomLineLocation;
	other->vMonsterLine_TopLocation = gridDepth.vMonsterLine_TopLocation;
	other->vMonsterLine_BottomLocation = gridDepth.vMonsterLine_BottomLocation;
	other->vMonsterLine_Spacing = gridDepth.vMonsterLine_Spacing;
	other->vCharacterLine_TopLocation = gridDepth.vCharacterLine_TopLocation;
	other->vCharacterLine_BottomLocation = gridDepth.vCharacterLine_BottomLocation;
	other->vCharacterLine_Spacing = gridDepth.vCharacterLine_Spacing;
}

void BaseEvent::EnemyEncounter::setGridDepth(Terrain::GridDepthValues *other)
{
	gridDepth.depth = other->depth;
	gridDepth.hTopLineLocation = other->hTopLineLocation;
	gridDepth.hBottomLineLocation = other->hBottomLineLocation;
	gridDepth.vMonsterLine_TopLocation = other->vMonsterLine_TopLocation;
	gridDepth.vMonsterLine_BottomLocation = other->vMonsterLine_BottomLocation;
	gridDepth.vMonsterLine_Spacing = other->vMonsterLine_Spacing;
	gridDepth.vCharacterLine_TopLocation = other->vCharacterLine_TopLocation;
	gridDepth.vCharacterLine_BottomLocation = other->vCharacterLine_BottomLocation;
	gridDepth.vCharacterLine_Spacing = other->vCharacterLine_Spacing;
}

void BaseEvent::EnemyEncounter::addChildEvent(BaseEvent *event, int location, int childTypeID)
{
	if (childTypeID == 0)
		addVictoryEvent(event, location);
	else if (childTypeID == 1)
	{
		if (escapeHandler == CUSTOM_ESCAPE_HANDLER)
			addEscapeEvent(event, location);
		else
			addDefeatEvent(event, location);
	}
	else if (childTypeID == 2)
		addDefeatEvent(event, location);
}

void BaseEvent::EnemyEncounter::deleteChildEvent(int location, int childTypeID)
{
	if (childTypeID == 0)
		deleteVictoryEvent(location);
	else if (childTypeID == 1)
	{
		if (escapeHandler == CUSTOM_ESCAPE_HANDLER)
			deleteEscapeEvent(location);
		else
			deleteDefeatEvent(location);
	}
	else if (childTypeID == 2)
		deleteDefeatEvent(location);
}

bool BaseEvent::EnemyEncounter::tryLoad(XMLNode EventNode, int baseType)
{
	XMLNode TempNode1;
	QStringList tempStringList;
	QStringList trueFalseList = QStringList() << "true" << "false";
	QString tempString;
	if (!XMLTools::attributeExists(EventNode, "specialConditions"))
		return false;
	tempStringList.clear();
	tempStringList << "none" << "initiative" << "backAttack" << "surroundAttack" << "pincersAttack";
	if (!XMLTools::attributeStringValid(EventNode, "specialConditions", tempStringList, 0))
		return false;
	if (!XMLTools::attributeExists(EventNode, "battleBackground"))
		return false;
	tempStringList.clear();
	tempStringList << "useMapOrTerrainSetting" << "specificBackground" << "specificTerrainsBackground";
	if (!XMLTools::attributeStringValid(EventNode, "battleBackground", tempStringList))
		return false;
	tempString = EventNode.getAttribute("battleBackground");
	if (tempString == "specificBackground")
	{
		if (!XMLTools::attributeExists(EventNode, "bgLocation"))
			return false;
		if (!XMLTools::nodeExists(EventNode, "gridDepth"))
			return false;
		TempNode1 = EventNode.getChildNode("gridDepth");
		if (!XMLTools::attributeExists(TempNode1, "depth"))
			return false;
		tempStringList.clear();
		tempStringList << "shallow" << "deep" << "custom";
		if (!XMLTools::attributeStringValid(TempNode1, "depth", tempStringList))
			return false;
		if (QString(EventNode.getAttribute("depth")) == "custom")
		{
			if (!XMLTools::attributeExists(TempNode1, "hTopLineLocation"))
				return false;
			if (!XMLTools::attributeExists(TempNode1, "hBottomLineLocation"))
				return false;
			if (!XMLTools::attributeExists(TempNode1, "vMonsterLine_TopLocation"))
				return false;
			if (!XMLTools::attributeExists(TempNode1, "vMonsterLine_BottomLocation"))
				return false;
			if (!XMLTools::attributeExists(TempNode1, "vMonsterLine_Spacing"))
				return false;
			if (!XMLTools::attributeExists(TempNode1, "vCharacterLine_TopLocation"))
				return false;
			if (!XMLTools::attributeExists(TempNode1, "vCharacterLine_BottomLocation"))
				return false;
			if (!XMLTools::attributeExists(TempNode1, "vCharacterLine_Spacing"))
				return false;
		}
	}
	else if (tempString == "specificTerrainsBackground")
	{
		if (!XMLTools::attributeExists(EventNode, "terrainID"))
			return false;
	}
	if (!XMLTools::nodeExists(EventNode, "victoryEvents"))
		return false;
	TempNode1 = EventNode.getChildNode("victoryEvents");
	for (int i = 0, count = TempNode1.nChildNode("event"); i < count; ++i)
	{
		if (!BaseEvent::tryLoad(TempNode1.getChildNode("event", i), baseType))
			return false;
	}
	if (!XMLTools::attributeExists(EventNode, "escapeHandler"))
		return false;
	tempStringList.clear();
	tempStringList << "dissalowEscape" << "endEventProcessing" << "customEscapeHandler";
	if (!XMLTools::attributeStringValid(EventNode, "escapeHandler", tempStringList))
		return false;
	if (QString(EventNode.getAttribute("escapeHandler")) == "customEscapeHandler")
	{
		if (!XMLTools::nodeExists(EventNode, "customEscapeHandler"))
			return false;
		TempNode1 = EventNode.getChildNode("customEscapeHandler");
		for (int i = 0, count = TempNode1.nChildNode("event"); i < count; ++i)
		{
			if (!BaseEvent::tryLoad(TempNode1.getChildNode("event", i), baseType))
				return false;
		}
	}
	if (!XMLTools::attributeExists(EventNode, "defeatHandler"))
		return false;
	tempStringList.clear();
	tempStringList << "gameOver" << "customDefeatHandler";
	if (!XMLTools::attributeStringValid(EventNode, "customDefeatHandler", tempStringList))
		return false;
	if (QString(EventNode.getAttribute("defeatHandler")) == "customDefeatHandler")
	{
		if (!XMLTools::nodeExists(EventNode, "customDefeatHandler"))
			return false;
		TempNode1 = EventNode.getChildNode("customDefeatHandler");
		for (int i = 0, count = TempNode1.nChildNode("event"); i < count; ++i)
		{
			if (!BaseEvent::tryLoad(TempNode1.getChildNode("event", i), baseType))
				return false;
		}
	}
	if (!XMLTools::attributeExists(EventNode, "monsterGroupID"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "monsterGroupReferencedInVariable"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "monsterGroupReferencedInVariable", trueFalseList, 1))
		return false;
	if (!XMLTools::attributeExists(EventNode, "beginBattleWithFirstStrike"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "beginBattleWithFirstStrike", trueFalseList, 1))
		return false;
	return true;
}

BaseEvent::EnemyEncounter::EnemyEncounter(BaseEvent *pBase) : Event(pBase)
{
	specialConditions = NONE;
	battleBackgroundType = USE_MAP_OR_TERRAIN_SETTING;
	escapeHandler = DISALLOW_ESCAPE;
	defeatHandler = GAMEOVER;
	gridDepth.depth = Terrain::DEPTH_SHALLOW;
	gridDepth.hTopLineLocation = Terrain::gridDepth_ShallowValues.hTopLineLocation;
	gridDepth.hBottomLineLocation = Terrain::gridDepth_ShallowValues.hBottomLineLocation;
	gridDepth.vMonsterLine_TopLocation = Terrain::gridDepth_ShallowValues.vMonsterLine_TopLocation;
	gridDepth.vMonsterLine_BottomLocation = Terrain::gridDepth_ShallowValues.vMonsterLine_BottomLocation;
	gridDepth.vMonsterLine_Spacing = Terrain::gridDepth_ShallowValues.vMonsterLine_Spacing;
	gridDepth.vCharacterLine_TopLocation = Terrain::gridDepth_ShallowValues.vCharacterLine_TopLocation;
	gridDepth.vCharacterLine_BottomLocation = Terrain::gridDepth_ShallowValues.vCharacterLine_BottomLocation;
	gridDepth.vCharacterLine_Spacing = Terrain::gridDepth_ShallowValues.vCharacterLine_Spacing;
	specificBackgroundLocation = "";
	monsterGroupID = 0;
	terrainID = 0;
	monsterGroupReferencedInVariable = false;
	beginBattleWithFirstStrike = false;
}

BaseEvent::EnemyEncounter::EnemyEncounter(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	XMLNode TempNode1;
	QString tempString;
	tempString = EventNode.getAttribute("specialConditions");
	if (tempString == "none")
		specialConditions = NONE;
	else if (tempString == "initiative")
		specialConditions = INITIATIVE;
	else if (tempString == "backAttack")
		specialConditions = BACK_ATTACK;
	else if (tempString == "surroundAttack")
		specialConditions = SURROUND_ATTACK;
	else if (tempString == "pincersAttack")
		specialConditions = PINCERS_ATTACK;
	tempString = EventNode.getAttribute("battleBackgroundType");
	if (tempString == "useMapOrTerrainSetting")
	{
		battleBackgroundType = USE_MAP_OR_TERRAIN_SETTING;
		specificBackgroundLocation = "";
		terrainID = 0;
		gridDepth.depth = Terrain::DEPTH_SHALLOW;
		gridDepth.hTopLineLocation = Terrain::gridDepth_ShallowValues.hTopLineLocation;
		gridDepth.hBottomLineLocation = Terrain::gridDepth_ShallowValues.hBottomLineLocation;
		gridDepth.vMonsterLine_TopLocation = Terrain::gridDepth_ShallowValues.vMonsterLine_TopLocation;
		gridDepth.vMonsterLine_BottomLocation = Terrain::gridDepth_ShallowValues.vMonsterLine_BottomLocation;
		gridDepth.vMonsterLine_Spacing = Terrain::gridDepth_ShallowValues.vMonsterLine_Spacing;
		gridDepth.vCharacterLine_TopLocation = Terrain::gridDepth_ShallowValues.vCharacterLine_TopLocation;
		gridDepth.vCharacterLine_BottomLocation = Terrain::gridDepth_ShallowValues.vCharacterLine_BottomLocation;
		gridDepth.vCharacterLine_Spacing = Terrain::gridDepth_ShallowValues.vCharacterLine_Spacing;
	}
	else if (tempString == "specificBackground")
	{
		battleBackgroundType = SPECIFIC_BACKGROUND;
		specificBackgroundLocation = ProjectData::getAbsoluteResourcePath(EventNode.getAttribute("bgLocation"));
		TempNode1 = EventNode.getChildNode("gridDepth");
		tempString = TempNode1.getAttribute("depth");
		if (tempString == "shallow")
		{
			gridDepth.depth = Terrain::DEPTH_SHALLOW;
			gridDepth.hTopLineLocation = Terrain::gridDepth_ShallowValues.hTopLineLocation;
			gridDepth.hBottomLineLocation = Terrain::gridDepth_ShallowValues.hBottomLineLocation;
			gridDepth.vMonsterLine_TopLocation = Terrain::gridDepth_ShallowValues.vMonsterLine_TopLocation;
			gridDepth.vMonsterLine_BottomLocation = Terrain::gridDepth_ShallowValues.vMonsterLine_BottomLocation;
			gridDepth.vMonsterLine_Spacing = Terrain::gridDepth_ShallowValues.vMonsterLine_Spacing;
			gridDepth.vCharacterLine_TopLocation = Terrain::gridDepth_ShallowValues.vCharacterLine_TopLocation;
			gridDepth.vCharacterLine_BottomLocation = Terrain::gridDepth_ShallowValues.vCharacterLine_BottomLocation;
			gridDepth.vCharacterLine_Spacing = Terrain::gridDepth_ShallowValues.vCharacterLine_Spacing;
		}
		else if (tempString == "deep")
		{
			gridDepth.depth = Terrain::DEPTH_DEEP;
			gridDepth.hTopLineLocation = Terrain::gridDepth_DeepValues.hTopLineLocation;
			gridDepth.hBottomLineLocation = Terrain::gridDepth_DeepValues.hBottomLineLocation;
			gridDepth.vMonsterLine_TopLocation = Terrain::gridDepth_DeepValues.vMonsterLine_TopLocation;
			gridDepth.vMonsterLine_BottomLocation = Terrain::gridDepth_DeepValues.vMonsterLine_BottomLocation;
			gridDepth.vMonsterLine_Spacing = Terrain::gridDepth_DeepValues.vMonsterLine_Spacing;
			gridDepth.vCharacterLine_TopLocation = Terrain::gridDepth_DeepValues.vCharacterLine_TopLocation;
			gridDepth.vCharacterLine_BottomLocation = Terrain::gridDepth_DeepValues.vCharacterLine_BottomLocation;
			gridDepth.vCharacterLine_Spacing = Terrain::gridDepth_DeepValues.vCharacterLine_Spacing;
		}
		else if (tempString == "custom")
		{
			gridDepth.depth = Terrain::DEPTH_CUSTOM;
			gridDepth.hTopLineLocation = QString(EventNode.getAttribute("hTopLineLocation")).toInt();
			gridDepth.hBottomLineLocation = QString(EventNode.getAttribute("hBottomLineLocation")).toInt();
			gridDepth.vMonsterLine_TopLocation = QString(EventNode.getAttribute("vMonsterLine_TopLocation")).toInt();
			gridDepth.vMonsterLine_BottomLocation = QString(EventNode.getAttribute("vMonsterLine_BottomLocation")).toInt();
			gridDepth.vMonsterLine_Spacing = QString(EventNode.getAttribute("vMonsterLine_Spacing")).toInt();
			gridDepth.vCharacterLine_TopLocation = QString(EventNode.getAttribute("vCharacterLine_TopLocation")).toInt();
			gridDepth.vCharacterLine_BottomLocation = QString(EventNode.getAttribute("vCharacterLine_BottomLocation")).toInt();
			gridDepth.vCharacterLine_Spacing = QString(EventNode.getAttribute("vCharacterLine_Spacing")).toInt();
		}
		terrainID = 0;
	}
	else if (tempString == "useTerrainBackground")
	{
		battleBackgroundType = USE_TERRAIN_BACKGROUND;
		specificBackgroundLocation = "";
		terrainID = QString(EventNode.getAttribute("terrainID")).toInt();
		gridDepth.depth = Terrain::DEPTH_SHALLOW;
		gridDepth.hTopLineLocation = Terrain::gridDepth_ShallowValues.hTopLineLocation;
		gridDepth.hBottomLineLocation = Terrain::gridDepth_ShallowValues.hBottomLineLocation;
		gridDepth.vMonsterLine_TopLocation = Terrain::gridDepth_ShallowValues.vMonsterLine_TopLocation;
		gridDepth.vMonsterLine_BottomLocation = Terrain::gridDepth_ShallowValues.vMonsterLine_BottomLocation;
		gridDepth.vMonsterLine_Spacing = Terrain::gridDepth_ShallowValues.vMonsterLine_Spacing;
		gridDepth.vCharacterLine_TopLocation = Terrain::gridDepth_ShallowValues.vCharacterLine_TopLocation;
		gridDepth.vCharacterLine_BottomLocation = Terrain::gridDepth_ShallowValues.vCharacterLine_BottomLocation;
		gridDepth.vCharacterLine_Spacing = Terrain::gridDepth_ShallowValues.vCharacterLine_Spacing;
	}
	TempNode1 = EventNode.getChildNode("victoryEvents");
	for (int i = 0, count = TempNode1.nChildNode("event"); i < count; ++i)
		victoryEvents.append(new BaseEvent(pBaseEvent->baseType, TempNode1.getChildNode("event", i)));
	tempString = EventNode.getAttribute("escapeHandler");
	if (tempString == "disallowEscape")
		escapeHandler = DISALLOW_ESCAPE;
	else if (tempString == "endEventProcessing")
		escapeHandler = END_EVENT_PROCESSING;
	else if (tempString == "customEscapeHandler")
	{
		escapeHandler = CUSTOM_ESCAPE_HANDLER;
		TempNode1 = EventNode.getChildNode("customEscapeHandler");
		for (int i = 0, count = TempNode1.nChildNode("event"); i < count; ++i)
			escapeEvents.append(new BaseEvent(pBaseEvent->baseType, TempNode1.getChildNode("event", i)));
	}
	tempString = EventNode.getAttribute("defeatHandler");
	if (tempString == "gameOver")
		defeatHandler = GAMEOVER;
	else if (tempString == "customDefeatHandler")
	{
		defeatHandler = CUSTOM_DEFEAT_HANDLER;
		TempNode1 = EventNode.getChildNode("customDefeatHandler");
		for (int i = 0, count = TempNode1.nChildNode("event"); i < count; ++i)
			defeatEvents.append(new BaseEvent(pBaseEvent->baseType, TempNode1.getChildNode("event", i)));
	}
	monsterGroupID = QString(EventNode.getAttribute("monsterGroupID")).toInt();
	tempString = EventNode.getAttribute("monsterGroupReferencedInVariable");
	monsterGroupReferencedInVariable = (tempString == "true") ? true:false;
	tempString = EventNode.getAttribute("beginBattleWithFirstStrike");
	beginBattleWithFirstStrike = (tempString == "true") ? true:false;
}

BaseEvent::EnemyEncounter::EnemyEncounter(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	char *temp_string;
	specialConditions = storageFile.getSignedInt();
	battleBackgroundType = storageFile.getSignedInt();
	escapeHandler = storageFile.getSignedInt();
	defeatHandler = storageFile.getSignedInt();
	gridDepth.depth = storageFile.getSignedInt();
	if (battleBackgroundType == USE_MAP_OR_TERRAIN_SETTING)
	{
		specificBackgroundLocation = "";
		terrainID = 0;
	}
	else if (battleBackgroundType == SPECIFIC_BACKGROUND)
	{
		temp_string = storageFile.getString();
		specificBackgroundLocation = temp_string;
		delete[] temp_string;
		terrainID = 0;
	}
	else if (battleBackgroundType == USE_TERRAIN_BACKGROUND)
	{
		specificBackgroundLocation = "";
		terrainID = storageFile.getSignedInt();
	}
	for (int i = 0, numEvents = storageFile.getSignedInt(); i < numEvents; ++i)
		victoryEvents.append(new BaseEvent(pBaseEvent->baseType, storageFile));
	if (escapeHandler == CUSTOM_ESCAPE_HANDLER)
	{
		int numEvents = storageFile.getSignedInt();
		for (int i = 0; i < numEvents; ++i)
			escapeEvents.append(new BaseEvent(pBaseEvent->baseType, storageFile));
	}
	if (defeatHandler == CUSTOM_DEFEAT_HANDLER)
	{
		int numEvents = storageFile.getSignedInt();
		for (int i = 0; i < numEvents; ++i)
			defeatEvents.append(new BaseEvent(pBaseEvent->baseType, storageFile));
	}
	if (gridDepth.depth == Terrain::DEPTH_SHALLOW)
	{
		gridDepth.hTopLineLocation = Terrain::gridDepth_ShallowValues.hTopLineLocation;
		gridDepth.hBottomLineLocation = Terrain::gridDepth_ShallowValues.hBottomLineLocation;
		gridDepth.vMonsterLine_TopLocation = Terrain::gridDepth_ShallowValues.vMonsterLine_TopLocation;
		gridDepth.vMonsterLine_BottomLocation = Terrain::gridDepth_ShallowValues.vMonsterLine_BottomLocation;
		gridDepth.vMonsterLine_Spacing = Terrain::gridDepth_ShallowValues.vMonsterLine_Spacing;
		gridDepth.vCharacterLine_TopLocation = Terrain::gridDepth_ShallowValues.vCharacterLine_TopLocation;
		gridDepth.vCharacterLine_BottomLocation = Terrain::gridDepth_ShallowValues.vCharacterLine_BottomLocation;
		gridDepth.vCharacterLine_Spacing = Terrain::gridDepth_ShallowValues.vCharacterLine_Spacing;
	}
	else if (gridDepth.depth == Terrain::DEPTH_DEEP)
	{
		gridDepth.hTopLineLocation = Terrain::gridDepth_DeepValues.hTopLineLocation;
		gridDepth.hBottomLineLocation = Terrain::gridDepth_DeepValues.hBottomLineLocation;
		gridDepth.vMonsterLine_TopLocation = Terrain::gridDepth_DeepValues.vMonsterLine_TopLocation;
		gridDepth.vMonsterLine_BottomLocation = Terrain::gridDepth_DeepValues.vMonsterLine_BottomLocation;
		gridDepth.vMonsterLine_Spacing = Terrain::gridDepth_DeepValues.vMonsterLine_Spacing;
		gridDepth.vCharacterLine_TopLocation = Terrain::gridDepth_DeepValues.vCharacterLine_TopLocation;
		gridDepth.vCharacterLine_BottomLocation = Terrain::gridDepth_DeepValues.vCharacterLine_BottomLocation;
		gridDepth.vCharacterLine_Spacing = Terrain::gridDepth_DeepValues.vCharacterLine_Spacing;
	}
	else if (gridDepth.depth == Terrain::DEPTH_CUSTOM)
	{
		gridDepth.hTopLineLocation = storageFile.getSignedInt();
		gridDepth.hBottomLineLocation = storageFile.getSignedInt();
		gridDepth.vMonsterLine_TopLocation = storageFile.getSignedInt();
		gridDepth.vMonsterLine_BottomLocation = storageFile.getSignedInt();
		gridDepth.vMonsterLine_Spacing = storageFile.getSignedInt();
		gridDepth.vCharacterLine_TopLocation = storageFile.getSignedInt();
		gridDepth.vCharacterLine_BottomLocation = storageFile.getSignedInt();
		gridDepth.vCharacterLine_Spacing = storageFile.getSignedInt();
	}
	monsterGroupID = storageFile.getSignedInt();
	monsterGroupReferencedInVariable = storageFile.getBool();
	beginBattleWithFirstStrike = storageFile.getBool();
}

BaseEvent::EnemyEncounter::EnemyEncounter(EnemyEncounter *other, BaseEvent *pBase) : Event(pBase)
{
	specialConditions = other->specialConditions;
	battleBackgroundType = other->battleBackgroundType;
	escapeHandler = other->escapeHandler;
	defeatHandler = other->defeatHandler;
	gridDepth.depth = other->gridDepth.depth;
	gridDepth.hTopLineLocation = other->gridDepth.hTopLineLocation;
	gridDepth.hBottomLineLocation = other->gridDepth.hBottomLineLocation;
	gridDepth.vMonsterLine_TopLocation = other->gridDepth.vMonsterLine_TopLocation;
	gridDepth.vMonsterLine_BottomLocation = other->gridDepth.vMonsterLine_BottomLocation;
	gridDepth.vMonsterLine_Spacing = other->gridDepth.vMonsterLine_Spacing;
	gridDepth.vCharacterLine_TopLocation = other->gridDepth.vCharacterLine_TopLocation;
	gridDepth.vCharacterLine_BottomLocation = other->gridDepth.vCharacterLine_BottomLocation;
	gridDepth.vCharacterLine_Spacing = other->gridDepth.vCharacterLine_Spacing;
	specificBackgroundLocation = "";
	specificBackgroundLocation.append(other->specificBackgroundLocation);
	monsterGroupID = other->monsterGroupID;
	terrainID = other->terrainID;
	monsterGroupReferencedInVariable = other->monsterGroupReferencedInVariable;
	beginBattleWithFirstStrike = other->beginBattleWithFirstStrike;
	for (int i = 0; i < other->escapeEvents.size(); ++i)
		escapeEvents.append(new BaseEvent(other->escapeEvents[i]));
	for (int i = 0; i < other->defeatEvents.size(); ++i)
		defeatEvents.append(new BaseEvent(other->defeatEvents[i]));
	for (int i = 0; i < other->victoryEvents.size(); ++i)
		defeatEvents.append(new BaseEvent(other->victoryEvents[i]));
}
//END BaseEvent::EnemyEncounter Member Functions

//BEGIN BaseEvent::FadeBGM Member Functions
XMLNode BaseEvent::FadeBGM::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	EventNode.addAttribute("fadeOutTime", QString::number(fadeOutTime).toUtf8().data());
	return EventNode;
}

void BaseEvent::FadeBGM::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(fadeOutTime);
}

QStringList BaseEvent::FadeBGM::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += QString("Fade BGM: %1sec.").arg(fadeOutTime);
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::FadeBGM::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	ConfigureEvent_FadeBGM *configFadeBGM = new ConfigureEvent_FadeBGM;
	bool configured = false;
	configFadeBGM->setFadeOutTime(fadeOutTime);
	if (configFadeBGM->exec())
	{
		configured = true;
		fadeOutTime = configFadeBGM->getFadeOutTime();
	}
	delete configFadeBGM;
	return configured;
}
#endif

bool BaseEvent::FadeBGM::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	if (!XMLTools::attributeExists(EventNode, "fadeOutTime"))
		return false;
	if (!XMLTools::attributeIntRangeValid(EventNode, "fadeOutTime", 0, 10))
		return false;
	return true;
}

BaseEvent::FadeBGM::FadeBGM(BaseEvent *pBase) : Event(pBase)
{
	fadeOutTime = 0;
}

BaseEvent::FadeBGM::FadeBGM(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	fadeOutTime = QString(EventNode.getAttribute("fadeOutTime")).toInt();
}

BaseEvent::FadeBGM::FadeBGM(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	fadeOutTime = storageFile.getSignedInt();
}

BaseEvent::FadeBGM::FadeBGM(FadeBGM *other, BaseEvent *pBase) : Event(pBase)
{
	fadeOutTime = other->fadeOutTime;
}
//END BaseEvent::FadeBGM Member Functions

//BEGIN BaseEvent::FlashScreen Member Functions
XMLNode BaseEvent::FlashScreen::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	if (flash == ONCE)
	{
		EventNode.addAttribute("flash", "once");
		EventNode.addAttribute("colorRed", QString::number(color.red()).toUtf8().data());
		EventNode.addAttribute("colorGreen", QString::number(color.green()).toUtf8().data());
		EventNode.addAttribute("colorBlue", QString::number(color.blue()).toUtf8().data());
		EventNode.addAttribute("opacity", QString::number(opacity).toUtf8().data());
		EventNode.addAttribute("opacityStoredInVariable", (opacityStoredInVariable) ? "true":"false");
		EventNode.addAttribute("haltOtherProcesses", (haltOtherProcesses) ? "true":"false");
	}
	else if (flash == BEGIN)
	{
		EventNode.addAttribute("flash", "begin");
		EventNode.addAttribute("colorRed", QString::number(color.red()).toUtf8().data());
		EventNode.addAttribute("colorGreen", QString::number(color.green()).toUtf8().data());
		EventNode.addAttribute("colorBlue", QString::number(color.blue()).toUtf8().data());
		EventNode.addAttribute("opacity", QString::number(opacity).toUtf8().data());
		EventNode.addAttribute("opacityStoredInVariable", (opacityStoredInVariable) ? "true":"false");
		EventNode.addAttribute("delayBetweenFlashes", QString::number(delayBetweenFlashes).toUtf8().data());
		EventNode.addAttribute("delayStoredInVariable", (delayStoredInVariable) ? "true":"false");
	}
	else if (flash == STOP)
		EventNode.addAttribute("flash", "stop");
	return EventNode;
}

void BaseEvent::FlashScreen::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(color.red());
	storageFile.putSignedInt(color.green());
	storageFile.putSignedInt(color.blue());
	storageFile.putSignedInt(flash);
	storageFile.putSignedInt(delayBetweenFlashes);
	storageFile.putSignedInt(opacity);
	storageFile.putBool(opacityStoredInVariable);
	storageFile.putBool(delayStoredInVariable);
	storageFile.putBool(haltOtherProcesses);
}

QStringList BaseEvent::FlashScreen::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	if (flash == ONCE)
	{
		viewableText[0] += "Flash Once: ";
		viewableText[0] += QString("color=rgb(%1, %2, %3) ").arg(color.red()).arg(color.green()).arg(color.blue());
		if (opacityStoredInVariable)
			viewableText[0] += QString("opacity=variable[%1]").arg(ProjectData::numberVariableIdToString(opacity));
		else
			viewableText[0] += QString("opacity=%1").arg(opacity);
		if (haltOtherProcesses)
			viewableText[0] += " Halt Other Processes";
	}
	else if (flash == BEGIN)
	{
		viewableText[0] += "Begin Flashing: ";
		viewableText[0] += QString("color=rgb(%1, %2, %3) ").arg(color.red()).arg(color.green()).arg(color.blue());
		if (opacityStoredInVariable)
			viewableText[0] += QString("opacity=variable[%1] ").arg(ProjectData::numberVariableIdToString(opacity));
		else
			viewableText[0] += QString("opacity=%1 ").arg(opacity);
		if (delayStoredInVariable)
			viewableText[0] += QString("delay=variable[%1]").arg(ProjectData::numberVariableIdToString(delayBetweenFlashes));
		else
			viewableText[0] += QString("delay=%1").arg(delayBetweenFlashes);
	}
	else if (flash == STOP)
		viewableText[0] += "Stop Flashing";
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::FlashScreen::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	ConfigureEvent_FlashScreen *configFlash = new ConfigureEvent_FlashScreen;
	bool configured = false;
	configFlash->setFlash(flash);
	if (flash == ONCE)
	{
		configFlash->setColor(color);
		configFlash->setOpacityStoredInVariable(opacityStoredInVariable);
		configFlash->setOpacity(opacity);
		configFlash->setHaltOtherProcesses(haltOtherProcesses);
	}
	else if (flash == BEGIN)
	{
		configFlash->setColor(color);
		configFlash->setOpacityStoredInVariable(opacityStoredInVariable);
		configFlash->setOpacity(opacity);
		configFlash->setDelayStoredInVariable(delayStoredInVariable);
		configFlash->setDelayBetweenFlashes(delayBetweenFlashes);
	}
	if (configFlash->exec())
	{
		configured = true;
		flash = configFlash->getFlash();
		if (flash == ONCE)
		{
			color = configFlash->getColor();
			opacity = configFlash->getOpacity();
			opacityStoredInVariable = configFlash->isOpacityStoredInVariable();
			haltOtherProcesses = configFlash->getHaltOtherProcesses();
		}
		else if (flash == BEGIN)
		{
			color = configFlash->getColor();
			opacity = configFlash->getOpacity();
			opacityStoredInVariable = configFlash->isOpacityStoredInVariable();
			delayBetweenFlashes = configFlash->getDelayBetweenFlashes();
			delayStoredInVariable = configFlash->isDelayStoredInVariable();
		}
	}
	delete configFlash;
	return configured;
}
#endif

bool BaseEvent::FlashScreen::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	QStringList flashList = QStringList() << "once" << "begin" << "stop";
	QStringList trueFalseList = QStringList() << "true" << "false";
	QString tempString;
	if (!XMLTools::attributeExists(EventNode, "flash"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "flash", flashList))
		return false;
	tempString = EventNode.getAttribute("flash");
	if (tempString == "once")
	{
		if (!XMLTools::attributeExists(EventNode, "colorRed"))
			return false;
		if (!XMLTools::attributeIntRangeValid(EventNode, "colorRed", 0, 255))
			return false;
		if (!XMLTools::attributeExists(EventNode, "colorGreen"))
			return false;
		if (!XMLTools::attributeIntRangeValid(EventNode, "colorGreen", 0, 255))
			return false;
		if (!XMLTools::attributeExists(EventNode, "colorBlue"))
			return false;
		if (!XMLTools::attributeIntRangeValid(EventNode, "colorBlue", 0, 255))
			return false;
		if (!XMLTools::attributeExists(EventNode, "opacity"))
			return false;
		if (!XMLTools::attributeExists(EventNode, "opacityStoredInVariable"))
			return false;
		if (!XMLTools::attributeStringValid(EventNode, "opacityStoredInVariable", trueFalseList, 1))
			return false;
		if (!XMLTools::attributeExists(EventNode, "haltOtherProcesses"))
			return false;
		if (!XMLTools::attributeStringValid(EventNode, "haltOtherProcesses", trueFalseList, 1))
			return false;
	}
	else if (tempString == "begin")
	{
		if (!XMLTools::attributeExists(EventNode, "colorRed"))
			return false;
		if (!XMLTools::attributeIntRangeValid(EventNode, "colorRed", 0, 255))
			return false;
		if (!XMLTools::attributeExists(EventNode, "colorGreen"))
			return false;
		if (!XMLTools::attributeIntRangeValid(EventNode, "colorGreen", 0, 255))
			return false;
		if (!XMLTools::attributeExists(EventNode, "colorBlue"))
			return false;
		if (!XMLTools::attributeIntRangeValid(EventNode, "colorBlue", 0, 255))
			return false;
		if (!XMLTools::attributeExists(EventNode, "opacity"))
			return false;
		if (!XMLTools::attributeExists(EventNode, "opacityStoredInVariable"))
			return false;
		if (!XMLTools::attributeStringValid(EventNode, "opacityStoredInVariable", trueFalseList, 1))
			return false;
		if (!XMLTools::attributeExists(EventNode, "delayBetweenFlashes"))
			return false;
		if (!XMLTools::attributeExists(EventNode, "delayStoredInVariable"))
			return false;
		if (!XMLTools::attributeStringValid(EventNode, "delayStoredInVariable", trueFalseList, 1))
			return false;
	}
	return true;
}

BaseEvent::FlashScreen::FlashScreen(BaseEvent *pBase) : Event(pBase)
{
	color = QColor(255, 255, 255);
	flash = ONCE;
	delayBetweenFlashes = 5;
	opacity = 100;
	opacityStoredInVariable = false;
	delayStoredInVariable = false;
	haltOtherProcesses = false;
}

BaseEvent::FlashScreen::FlashScreen(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	QString tempString = EventNode.getAttribute("flash");
	color = QColor(255, 255, 255);
	delayBetweenFlashes = 5;
	opacity = 100;
	opacityStoredInVariable = false;
	delayStoredInVariable = false;
	haltOtherProcesses = false;
	if (tempString == "once")
	{
		int red = QString(EventNode.getAttribute("colorRed")).toInt();
		int green = QString(EventNode.getAttribute("colorGreen")).toInt();
		int blue = QString(EventNode.getAttribute("colorBlue")).toInt();
		flash = ONCE;
		color = QColor(red, green, blue);
		opacity = QString(EventNode.getAttribute("opacity")).toInt();
		opacityStoredInVariable = (QString(EventNode.getAttribute("opacityStoredInVariable")) == "true") ? true:false;
		haltOtherProcesses = (QString(EventNode.getAttribute("haltOtherProcesses")) == "true") ? true:false;
	}
	else if (tempString == "begin")
	{
		int red = QString(EventNode.getAttribute("colorRed")).toInt();
		int green = QString(EventNode.getAttribute("colorGreen")).toInt();
		int blue = QString(EventNode.getAttribute("colorBlue")).toInt();
		flash = BEGIN;
		color = QColor(red, green, blue);
		opacity = QString(EventNode.getAttribute("opacity")).toInt();
		opacityStoredInVariable = (QString(EventNode.getAttribute("opacityStoredInVariable")) == "true") ? true:false;
		delayBetweenFlashes = QString(EventNode.getAttribute("delayBetweenFlashes")).toInt();
		delayStoredInVariable = (QString(EventNode.getAttribute("delayStoredInVariable")) == "true") ? true:false;
	}
	else if (tempString == "stop")
		flash = STOP;
}

BaseEvent::FlashScreen::FlashScreen(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	int red = storageFile.getSignedInt();
	int green = storageFile.getSignedInt();
	int blue = storageFile.getSignedInt();
	color = QColor(red, green, blue);
	flash = storageFile.getSignedInt();
	delayBetweenFlashes = storageFile.getSignedInt();
	opacity = storageFile.getSignedInt();
	opacityStoredInVariable = storageFile.getBool();
	delayStoredInVariable = storageFile.getBool();
	haltOtherProcesses = storageFile.getBool();
}

BaseEvent::FlashScreen::FlashScreen(FlashScreen *other, BaseEvent *pBase) : Event(pBase)
{
	color = other->color;
	flash = other->flash;
	delayBetweenFlashes = other->delayBetweenFlashes;
	opacity = other->opacity;
	opacityStoredInVariable = other->opacityStoredInVariable;
	delayStoredInVariable = other->delayStoredInVariable;
	haltOtherProcesses = other->haltOtherProcesses;
}
//END BaseEvent::FlashScreen Member Functions

//BEGIN BaseEvent::FlashSprite_Map Member Functions
XMLNode BaseEvent::FlashSprite_Map::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	if (flash == ONCE)
	{
		EventNode.addAttribute("flash", "once");
		EventNode.addAttribute("colorRed", QString::number(color.red()).toUtf8().data());
		EventNode.addAttribute("colorGreen", QString::number(color.green()).toUtf8().data());
		EventNode.addAttribute("colorBlue", QString::number(color.blue()).toUtf8().data());
		EventNode.addAttribute("opacity", QString::number(opacity).toUtf8().data());
		EventNode.addAttribute("opacityStoredInVariable", (opacityStoredInVariable) ? "true":"false");
		EventNode.addAttribute("haltOtherProcesses", (haltOtherProcesses) ? "true":"false");
	}
	else if (flash == BEGIN)
	{
		EventNode.addAttribute("flash", "begin");
		EventNode.addAttribute("colorRed", QString::number(color.red()).toUtf8().data());
		EventNode.addAttribute("colorGreen", QString::number(color.green()).toUtf8().data());
		EventNode.addAttribute("colorBlue", QString::number(color.blue()).toUtf8().data());
		EventNode.addAttribute("opacity", QString::number(opacity).toUtf8().data());
		EventNode.addAttribute("opacityStoredInVariable", (opacityStoredInVariable) ? "true":"false");
		EventNode.addAttribute("delayBetweenFlashes", QString::number(delayBetweenFlashes).toUtf8().data());
		EventNode.addAttribute("delayStoredInVariable", (delayStoredInVariable) ? "true":"false");
	}
	else if (flash == STOP)
		EventNode.addAttribute("flash", "stop");
	if (targetType == HERO)
		EventNode.addAttribute("targetType", "hero");
	else if (targetType == THIS_EVENT)
		EventNode.addAttribute("targetType", "thisEvent");
	else if (targetType == VEHICLE)
	{
		EventNode.addAttribute("targetType", "vehicle");
		EventNode.addAttribute("vehicleID", QString::number(targetID).toUtf8().data());
	}
	else if (targetType == EVENT)
	{
		EventNode.addAttribute("targetType", "event");
		EventNode.addAttribute("eventID", QString::number(targetID).toUtf8().data());
	}
	return EventNode;
}

void BaseEvent::FlashSprite_Map::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(color.red());
	storageFile.putSignedInt(color.green());
	storageFile.putSignedInt(color.blue());
	storageFile.putSignedInt(flash);
	storageFile.putSignedInt(delayBetweenFlashes);
	storageFile.putSignedInt(opacity);
	storageFile.putSignedInt(targetType);
	storageFile.putSignedInt(targetID);
	storageFile.putBool(opacityStoredInVariable);
	storageFile.putBool(delayStoredInVariable);
	storageFile.putBool(haltOtherProcesses);
}

QStringList BaseEvent::FlashSprite_Map::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	if (flash == ONCE)
	{
		viewableText[0] += "Flash Once: ";
		if (targetType == HERO)
			viewableText[0] += "Hero ";
		else if (targetType == THIS_EVENT)
			viewableText[0] += "This Event ";
		else if (targetType == VEHICLE)
			viewableText[0] += QString("%1 ").arg(ProjectData::vehicleNames[targetID]);
		else if (targetType == EVENT)
			viewableText[0] += QString("Event%1 ").arg(targetID, 3, 10, QChar('0'));
		viewableText[0] += QString("color=rgb(%1, %2, %3) ").arg(color.red()).arg(color.green()).arg(color.blue());
		if (opacityStoredInVariable)
			viewableText[0] += QString("opacity=variable[%1]").arg(ProjectData::numberVariableIdToString(opacity));
		else
			viewableText[0] += QString("opacity=%1").arg(opacity);
		if (haltOtherProcesses)
			viewableText[0] += " Halt Other Processes";
	}
	else if (flash == BEGIN)
	{
		viewableText[0] += "Begin Flashing: ";
		if (targetType == HERO)
			viewableText[0] += "Hero ";
		else if (targetType == THIS_EVENT)
			viewableText[0] += "This Event ";
		else if (targetType == VEHICLE)
			viewableText[0] += QString("%1 ").arg(ProjectData::vehicleNames[targetID]);
		else if (targetType == EVENT)
			viewableText[0] += QString("Event%1 ").arg(targetID, 3, 10, QChar('0'));
		viewableText[0] += QString("color=rgb(%1, %2, %3) ").arg(color.red()).arg(color.green()).arg(color.blue());
		if (opacityStoredInVariable)
			viewableText[0] += QString("opacity=variable[%1] ").arg(ProjectData::numberVariableIdToString(opacity));
		else
			viewableText[0] += QString("opacity=%1 ").arg(opacity);
		if (delayStoredInVariable)
			viewableText[0] += QString("delay=variable[%1]").arg(ProjectData::numberVariableIdToString(delayBetweenFlashes));
		else
			viewableText[0] += QString("delay=%1").arg(delayBetweenFlashes);
	}
	else if (flash == STOP)
	{
		viewableText[0] += "Stop Flashing";
		if (targetType == HERO)
			viewableText[0] += "Hero ";
		else if (targetType == THIS_EVENT)
			viewableText[0] += "This Event ";
		else if (targetType == VEHICLE)
			viewableText[0] += QString("%1 ").arg(ProjectData::vehicleNames[targetID]);
		else if (targetType == EVENT)
			viewableText[0] += QString("Event%1 ").arg(targetID, 3, 10, QChar('0'));
	}
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::FlashSprite_Map::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerType);
	ConfigureEvent_FlashSprite_Map *configFlash = new ConfigureEvent_FlashSprite_Map(callerID);
	bool configured = false;
	configFlash->setTargetType(targetType);
	configFlash->setTargetID(targetID);
	configFlash->setFlash(flash);
	if (flash == ONCE)
	{
		configFlash->setColor(color);
		configFlash->setOpacityStoredInVariable(opacityStoredInVariable);
		configFlash->setOpacity(opacity);
		configFlash->setHaltOtherProcesses(haltOtherProcesses);
	}
	else if (flash == BEGIN)
	{
		configFlash->setColor(color);
		configFlash->setOpacityStoredInVariable(opacityStoredInVariable);
		configFlash->setOpacity(opacity);
		configFlash->setDelayStoredInVariable(delayStoredInVariable);
		configFlash->setDelayBetweenFlashes(delayBetweenFlashes);
	}
	if (configFlash->exec())
	{
		configured = true;
		flash = configFlash->getFlash();
		if (flash == ONCE)
		{
			color = configFlash->getColor();
			opacity = configFlash->getOpacity();
			opacityStoredInVariable = configFlash->isOpacityStoredInVariable();
			haltOtherProcesses = configFlash->getHaltOtherProcesses();
		}
		else if (flash == BEGIN)
		{
			color = configFlash->getColor();
			opacity = configFlash->getOpacity();
			opacityStoredInVariable = configFlash->isOpacityStoredInVariable();
			delayBetweenFlashes = configFlash->getDelayBetweenFlashes();
			delayStoredInVariable = configFlash->isDelayStoredInVariable();
		}
	}
	delete configFlash;
	return configured;
}
#endif

bool BaseEvent::FlashSprite_Map::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	QStringList flashList = QStringList() << "once" << "begin" << "stop";
	QStringList trueFalseList = QStringList() << "true" << "false";
	QString tempString;
	if (!XMLTools::attributeExists(EventNode, "flash"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "flash", flashList))
		return false;
	if (!XMLTools::attributeExists(EventNode, "targetType"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "targetType", QStringList() << "hero" << "thisEvent" << "vehicle" << "event"))
		return false;
	tempString = EventNode.getAttribute("targetType");
	if (tempString == "vehicle")
	{
		if (!XMLTools::attributeExists(EventNode, "vehicleID"))
			return false;
	}
	else if (tempString == "event")
	{
		if (!XMLTools::attributeExists(EventNode, "eventID"))
			return false;
	}
	tempString = EventNode.getAttribute("flash");
	if (tempString == "once")
	{
		if (!XMLTools::attributeExists(EventNode, "colorRed"))
			return false;
		if (!XMLTools::attributeIntRangeValid(EventNode, "colorRed", 0, 255))
			return false;
		if (!XMLTools::attributeExists(EventNode, "colorGreen"))
			return false;
		if (!XMLTools::attributeIntRangeValid(EventNode, "colorGreen", 0, 255))
			return false;
		if (!XMLTools::attributeExists(EventNode, "colorBlue"))
			return false;
		if (!XMLTools::attributeIntRangeValid(EventNode, "colorBlue", 0, 255))
			return false;
		if (!XMLTools::attributeExists(EventNode, "opacity"))
			return false;
		if (!XMLTools::attributeExists(EventNode, "opacityStoredInVariable"))
			return false;
		if (!XMLTools::attributeStringValid(EventNode, "opacityStoredInVariable", trueFalseList, 1))
			return false;
		if (!XMLTools::attributeExists(EventNode, "haltOtherProcesses"))
			return false;
		if (!XMLTools::attributeStringValid(EventNode, "haltOtherProcesses", trueFalseList, 1))
			return false;
	}
	else if (tempString == "begin")
	{
		if (!XMLTools::attributeExists(EventNode, "colorRed"))
			return false;
		if (!XMLTools::attributeIntRangeValid(EventNode, "colorRed", 0, 255))
			return false;
		if (!XMLTools::attributeExists(EventNode, "colorGreen"))
			return false;
		if (!XMLTools::attributeIntRangeValid(EventNode, "colorGreen", 0, 255))
			return false;
		if (!XMLTools::attributeExists(EventNode, "colorBlue"))
			return false;
		if (!XMLTools::attributeIntRangeValid(EventNode, "colorBlue", 0, 255))
			return false;
		if (!XMLTools::attributeExists(EventNode, "opacity"))
			return false;
		if (!XMLTools::attributeExists(EventNode, "opacityStoredInVariable"))
			return false;
		if (!XMLTools::attributeStringValid(EventNode, "opacityStoredInVariable", trueFalseList, 1))
			return false;
		if (!XMLTools::attributeExists(EventNode, "delayBetweenFlashes"))
			return false;
		if (!XMLTools::attributeExists(EventNode, "delayStoredInVariable"))
			return false;
		if (!XMLTools::attributeStringValid(EventNode, "delayStoredInVariable", trueFalseList, 1))
			return false;
	}
	return true;
}

BaseEvent::FlashSprite_Map::FlashSprite_Map(BaseEvent *pBase) : Event(pBase)
{
	color = QColor(255, 255, 255);
	flash = ONCE;
	delayBetweenFlashes = 5;
	opacity = 100;
	targetType = HERO;
	targetID = -1;
	opacityStoredInVariable = false;
	delayStoredInVariable = false;
	haltOtherProcesses = false;
}

BaseEvent::FlashSprite_Map::FlashSprite_Map(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	QString tempString;
	color = QColor(255, 255, 255);
	delayBetweenFlashes = 5;
	opacity = 100;
	targetType = HERO;
	targetID = -1;
	opacityStoredInVariable = false;
	delayStoredInVariable = false;
	haltOtherProcesses = false;
	tempString = EventNode.getAttribute("targetType");
	if (tempString == "hero")
	{
		targetType = HERO;
		targetID = -1;
	}
	else if (tempString == "thisEvent")
	{
		targetType = THIS_EVENT;
		targetID = -1;
	}
	else if (tempString == "vehicle")
	{
		targetType = VEHICLE;
		targetID = QString(EventNode.getAttribute("vehicleID")).toInt();
	}
	else if (tempString == "event")
	{
		targetType = EVENT;
		targetID = QString(EventNode.getAttribute("eventID")).toInt();
	}
	tempString = EventNode.getAttribute("flash");
	if (tempString == "once")
	{
		int red = QString(EventNode.getAttribute("colorRed")).toInt();
		int green = QString(EventNode.getAttribute("colorGreen")).toInt();
		int blue = QString(EventNode.getAttribute("colorBlue")).toInt();
		flash = ONCE;
		color = QColor(red, green, blue);
		opacity = QString(EventNode.getAttribute("opacity")).toInt();
		opacityStoredInVariable = (QString(EventNode.getAttribute("opacityStoredInVariable")) == "true") ? true:false;
		haltOtherProcesses = (QString(EventNode.getAttribute("haltOtherProcesses")) == "true") ? true:false;
	}
	else if (tempString == "begin")
	{
		int red = QString(EventNode.getAttribute("colorRed")).toInt();
		int green = QString(EventNode.getAttribute("colorGreen")).toInt();
		int blue = QString(EventNode.getAttribute("colorBlue")).toInt();
		flash = BEGIN;
		color = QColor(red, green, blue);
		opacity = QString(EventNode.getAttribute("opacity")).toInt();
		opacityStoredInVariable = (QString(EventNode.getAttribute("opacityStoredInVariable")) == "true") ? true:false;
		delayBetweenFlashes = QString(EventNode.getAttribute("delayBetweenFlashes")).toInt();
		delayStoredInVariable = (QString(EventNode.getAttribute("delayStoredInVariable")) == "true") ? true:false;
	}
	else if (tempString == "stop")
		flash = STOP;
}

BaseEvent::FlashSprite_Map::FlashSprite_Map(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	int red = storageFile.getSignedInt();
	int green = storageFile.getSignedInt();
	int blue = storageFile.getSignedInt();
	color = QColor(red, green, blue);
	flash = storageFile.getSignedInt();
	delayBetweenFlashes = storageFile.getSignedInt();
	opacity = storageFile.getSignedInt();
	targetType = storageFile.getSignedInt();
	targetID = storageFile.getSignedInt();
	opacityStoredInVariable = storageFile.getBool();
	delayStoredInVariable = storageFile.getBool();
	haltOtherProcesses = storageFile.getBool();
}

BaseEvent::FlashSprite_Map::FlashSprite_Map(FlashSprite_Map *other, BaseEvent *pBase) : Event(pBase)
{
	color = other->color;
	flash = other->flash;
	delayBetweenFlashes = other->delayBetweenFlashes;
	opacity = other->opacity;
	targetType = other->targetType;
	targetID = other->targetID;
	opacityStoredInVariable = other->opacityStoredInVariable;
	delayStoredInVariable = other->delayStoredInVariable;
	haltOtherProcesses = other->haltOtherProcesses;
}
//END BaseEvent::FlashSprite_Map Member Functions

//BEGIN BaseEvent::HeroSpriteTransparency Member Functions
XMLNode BaseEvent::HeroSpriteTransparency::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	EventNode.addAttribute("opacity", QString::number(opacity).toUtf8().data());
	EventNode.addAttribute("opacityStoredInVariable", (opacityStoredInVariable) ? "true":"false");
	return EventNode;
}

void BaseEvent::HeroSpriteTransparency::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(opacity);
	storageFile.putBool(opacityStoredInVariable);
}

QStringList BaseEvent::HeroSpriteTransparency::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Hero Sprite Transparency: ";
	if (opacityStoredInVariable)
		viewableText[0] += QString("opacity=variable[%1]").arg(ProjectData::numberVariableIdToString(opacity));
	else
		viewableText[0] += QString("opacity=%1").arg(opacity);
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::HeroSpriteTransparency::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	GetNumberDialog *dialog = new GetNumberDialog("Opacity", 0, 100);
	bool configured = false;
	dialog->setValueStoredInVariable(opacityStoredInVariable);
	dialog->setValue(opacity);
	if (dialog->exec())
	{
		configured = true;
		opacity = dialog->getValue();
		opacityStoredInVariable = dialog->isValueStoredInVariable();
	}
	delete dialog;
	return configured;
}
#endif

bool BaseEvent::HeroSpriteTransparency::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	if (!XMLTools::attributeExists(EventNode, "opacity"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "opacityStoredInVariable"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "opacityStoredInVariable", QStringList() << "true" << "false", 1))
		return false;
	return true;
}

BaseEvent::HeroSpriteTransparency::HeroSpriteTransparency(BaseEvent *pBase) : Event(pBase)
{
	opacity = 100;
	opacityStoredInVariable = false;
}

BaseEvent::HeroSpriteTransparency::HeroSpriteTransparency(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	opacity = QString(EventNode.getAttribute("opacity")).toInt();
	opacityStoredInVariable = (QString(EventNode.getAttribute("opacityStoredInVariable")) == "true") ? true:false;
}

BaseEvent::HeroSpriteTransparency::HeroSpriteTransparency(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	opacity = storageFile.getSignedInt();
	opacityStoredInVariable = storageFile.getBool();
}

BaseEvent::HeroSpriteTransparency::HeroSpriteTransparency(HeroSpriteTransparency *other, BaseEvent *pBase) : Event(pBase)
{
	opacity = other->opacity;
	opacityStoredInVariable = other->opacityStoredInVariable;
}
//END BaseEvent::HeroSpriteTransparency Member Functions

//BEGIN BaseEvent::HideLayer Member Functions
XMLNode BaseEvent::HideLayer::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	EventNode.addAttribute("layer", QString::number(layer).toUtf8().data());
	EventNode.addAttribute("layerReferencedInVariable", (layerReferencedInVariable) ? "true":"false");
	if (transition == ProjectData::ERASE_FADEOUT)
		EventNode.addAttribute("transition", "fadeOut");
	else if (transition == ProjectData::ERASE_REMOVEBLOCKS)
		EventNode.addAttribute("transition", "removeBlocks");
	else if (transition == ProjectData::ERASE_WIPEDOWNWARD)
		EventNode.addAttribute("transition", "wipeDownward");
	else if (transition == ProjectData::ERASE_WIPEUPWARD)
		EventNode.addAttribute("transition", "wipeUpward");
	else if (transition == ProjectData::ERASE_VENETIANBLINDS)
		EventNode.addAttribute("transition", "venetianBlinds");
	else if (transition == ProjectData::ERASE_VERTICALBLINDS)
		EventNode.addAttribute("transition", "verticalBlinds");
	else if (transition == ProjectData::ERASE_HORIZONTALBLINDS)
		EventNode.addAttribute("transition", "horizontalBlinds");
	else if (transition == ProjectData::ERASE_RECEDINGSQUARE)
		EventNode.addAttribute("transition", "recedingSquare");
	else if (transition == ProjectData::ERASE_EXPANDINGSQUARE)
		EventNode.addAttribute("transition", "expandingSquare");
	else if (transition == ProjectData::ERASE_SCREENMOVESUP)
		EventNode.addAttribute("transition", "screenMovesUp");
	else if (transition == ProjectData::ERASE_SCREENMOVESDOWN)
		EventNode.addAttribute("transition", "screenMovesDown");
	else if (transition == ProjectData::ERASE_SCREENMOVESLEFT)
		EventNode.addAttribute("transition", "screenMovesLeft");
	else if (transition == ProjectData::ERASE_SCREENMOVESRIGHT)
		EventNode.addAttribute("transition", "screenMovesRight");
	else if (transition == ProjectData::ERASE_VERTICALDIVISION)
		EventNode.addAttribute("transition", "verticalDivision");
	else if (transition == ProjectData::ERASE_HORIZONTALDIVISION)
		EventNode.addAttribute("transition", "horizontalDivision");
	else if (transition == ProjectData::ERASE_QUADRASECTION)
		EventNode.addAttribute("transition", "quadraSection");
	else if (transition == ProjectData::ERASE_ZOOMIN)
		EventNode.addAttribute("transition", "zoomIn");
	else if (transition == ProjectData::ERASE_MOSAIC)
		EventNode.addAttribute("transition", "mosaic");
	else if (transition == ProjectData::ERASE_WAVERSCREEN)
		EventNode.addAttribute("transition", "waverScreen");
	else if (transition == ProjectData::ERASE_INSTANTANEOUS)
		EventNode.addAttribute("transition", "instantaneous");
	else if (transition == ProjectData::ERASE_NOTRANSITION)
		EventNode.addAttribute("transition", "noTransition");
	return EventNode;
}

void BaseEvent::HideLayer::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(layer);
	storageFile.putSignedInt(transition);
	storageFile.putBool(layerReferencedInVariable);
}

QStringList BaseEvent::HideLayer::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Hide Layer: ";
	if (layerReferencedInVariable)
		viewableText[0] += QString("layer=variable[%1], ").arg(ProjectData::numberVariableIdToString(layer));
	else
		viewableText[0] += QString("layer=%1, ").arg(layer);
	if (transition == ProjectData::ERASE_FADEOUT)
		viewableText[0] += "Fade Out";
	else if (transition == ProjectData::ERASE_REMOVEBLOCKS)
		viewableText[0] += "Remove Blocks";
	else if (transition == ProjectData::ERASE_WIPEDOWNWARD)
		viewableText[0] += "Wipe Downward";
	else if (transition == ProjectData::ERASE_WIPEUPWARD)
		viewableText[0] += "Wipe Upward";
	else if (transition == ProjectData::ERASE_VENETIANBLINDS)
		viewableText[0] += "Venetian Blinds";
	else if (transition == ProjectData::ERASE_VERTICALBLINDS)
		viewableText[0] += "Vertical Blinds";
	else if (transition == ProjectData::ERASE_HORIZONTALBLINDS)
		viewableText[0] += "Horizontal Blinds";
	else if (transition == ProjectData::ERASE_RECEDINGSQUARE)
		viewableText[0] += "Receding Square";
	else if (transition == ProjectData::ERASE_EXPANDINGSQUARE)
		viewableText[0] += "Expanding Square";
	else if (transition == ProjectData::ERASE_SCREENMOVESUP)
		viewableText[0] += "Screen Moves Up";
	else if (transition == ProjectData::ERASE_SCREENMOVESDOWN)
		viewableText[0] += "Screen Moves Down";
	else if (transition == ProjectData::ERASE_SCREENMOVESLEFT)
		viewableText[0] += "Screen Moves Left";
	else if (transition == ProjectData::ERASE_SCREENMOVESRIGHT)
		viewableText[0] += "Screen Moves Right";
	else if (transition == ProjectData::ERASE_VERTICALDIVISION)
		viewableText[0] += "Vertical Division";
	else if (transition == ProjectData::ERASE_HORIZONTALDIVISION)
		viewableText[0] += "Horizontal Division";
	else if (transition == ProjectData::ERASE_QUADRASECTION)
		viewableText[0] += "Quadrasection";
	else if (transition == ProjectData::ERASE_ZOOMIN)
		viewableText[0] += "Zoom In";
	else if (transition == ProjectData::ERASE_MOSAIC)
		viewableText[0] += "Mosaic";
	else if (transition == ProjectData::ERASE_WAVERSCREEN)
		viewableText[0] += "Waver Screen";
	else if (transition == ProjectData::ERASE_INSTANTANEOUS)
		viewableText[0] += "Instant";
	else if (transition == ProjectData::ERASE_NOTRANSITION)
		viewableText[0] += "No Transition";
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::HideLayer::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerType);
	ConfigureEvent_ShowHideLayer *configShowHideLayer = new ConfigureEvent_ShowHideLayer(callerID, false);
	bool configured = false;
	configShowHideLayer->setLayerReferencedInVariable(layerReferencedInVariable);
	configShowHideLayer->setLayer(layer);
	configShowHideLayer->setTransition(transition);
	if (configShowHideLayer->exec())
	{
		configured = true;
		layer = configShowHideLayer->getLayer();
		transition = configShowHideLayer->getTransition();
		layerReferencedInVariable = configShowHideLayer->isLayerReferencedInVariable();
	}
	delete configShowHideLayer;
	return configured;
}
#endif

bool BaseEvent::HideLayer::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	QStringList transitionList = QStringList() << "fadeOut" << "removeBlocks" << "wipeDownward" << "wipeUpward" << "venetianBlinds" << "verticalBlinds" << "horizontalBlinds" << "recedingSquare" << "expandingSquare" << "screenMovesUp" << "screenMovesDown" << "screenMovesLeft" << "screenMovesRight" << "verticalDivision" << "horizontalDivision" << "quadraSection" << "zoomIn" << "mosaic" << "waverScreen" << "instantaneous" << "noTransition";
	if (!XMLTools::attributeExists(EventNode, "layer"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "layerReferencedInVariable"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "layerReferencedInVariable", QStringList() << "true" << "false", 1))
		return false;
	if (!XMLTools::attributeExists(EventNode, "transition"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "transition", transitionList))
		return false;
	return true;
}

BaseEvent::HideLayer::HideLayer(BaseEvent *pBase) : Event(pBase)
{
	layer = 0;
	transition = ProjectData::ERASE_FADEOUT;
	layerReferencedInVariable = false;
}

BaseEvent::HideLayer::HideLayer(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	QStringList transitionList = QStringList() << "fadeOut" << "removeBlocks" << "wipeDownward" << "wipeUpward" << "venetianBlinds" << "verticalBlinds" << "horizontalBlinds" << "recedingSquare" << "expandingSquare" << "screenMovesUp" << "screenMovesDown" << "screenMovesLeft" << "screenMovesRight" << "verticalDivision" << "horizontalDivision" << "quadraSection" << "zoomIn" << "mosaic" << "waverScreen" << "instantaneous" << "noTransition";
	layer = QString(EventNode.getAttribute("layer")).toInt();
	layerReferencedInVariable = (QString(EventNode.getAttribute("layerReferencedInVariable")) == "true") ? true:false;
	transition = transitionList.indexOf(EventNode.getAttribute("transition"));
}

BaseEvent::HideLayer::HideLayer(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	layer = storageFile.getSignedInt();
	transition = storageFile.getSignedInt();
	layerReferencedInVariable = storageFile.getBool();
}

BaseEvent::HideLayer::HideLayer(HideLayer *other, BaseEvent *pBase) : Event(pBase)
{
	layer = other->layer;
	transition = other->transition;
	layerReferencedInVariable = other->layerReferencedInVariable;
}
//END BaseEvent::HideLayer Member Functions

//BEGIN BaseEvent::HideScreen Member Functions
XMLNode BaseEvent::HideScreen::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	if (transition == ProjectData::ERASE_FADEOUT)
		EventNode.addAttribute("transition", "fadeOut");
	else if (transition == ProjectData::ERASE_REMOVEBLOCKS)
		EventNode.addAttribute("transition", "removeBlocks");
	else if (transition == ProjectData::ERASE_WIPEDOWNWARD)
		EventNode.addAttribute("transition", "wipeDownward");
	else if (transition == ProjectData::ERASE_WIPEUPWARD)
		EventNode.addAttribute("transition", "wipeUpward");
	else if (transition == ProjectData::ERASE_VENETIANBLINDS)
		EventNode.addAttribute("transition", "venetianBlinds");
	else if (transition == ProjectData::ERASE_VERTICALBLINDS)
		EventNode.addAttribute("transition", "verticalBlinds");
	else if (transition == ProjectData::ERASE_HORIZONTALBLINDS)
		EventNode.addAttribute("transition", "horizontalBlinds");
	else if (transition == ProjectData::ERASE_RECEDINGSQUARE)
		EventNode.addAttribute("transition", "recedingSquare");
	else if (transition == ProjectData::ERASE_EXPANDINGSQUARE)
		EventNode.addAttribute("transition", "expandingSquare");
	else if (transition == ProjectData::ERASE_SCREENMOVESUP)
		EventNode.addAttribute("transition", "screenMovesUp");
	else if (transition == ProjectData::ERASE_SCREENMOVESDOWN)
		EventNode.addAttribute("transition", "screenMovesDown");
	else if (transition == ProjectData::ERASE_SCREENMOVESLEFT)
		EventNode.addAttribute("transition", "screenMovesLeft");
	else if (transition == ProjectData::ERASE_SCREENMOVESRIGHT)
		EventNode.addAttribute("transition", "screenMovesRight");
	else if (transition == ProjectData::ERASE_VERTICALDIVISION)
		EventNode.addAttribute("transition", "verticalDivision");
	else if (transition == ProjectData::ERASE_HORIZONTALDIVISION)
		EventNode.addAttribute("transition", "horizontalDivision");
	else if (transition == ProjectData::ERASE_QUADRASECTION)
		EventNode.addAttribute("transition", "quadraSection");
	else if (transition == ProjectData::ERASE_ZOOMIN)
		EventNode.addAttribute("transition", "zoomIn");
	else if (transition == ProjectData::ERASE_MOSAIC)
		EventNode.addAttribute("transition", "mosaic");
	else if (transition == ProjectData::ERASE_WAVERSCREEN)
		EventNode.addAttribute("transition", "waverScreen");
	else if (transition == ProjectData::ERASE_INSTANTANEOUS)
		EventNode.addAttribute("transition", "instantaneous");
	else if (transition == ProjectData::ERASE_NOTRANSITION)
		EventNode.addAttribute("transition", "noTransition");
	return EventNode;
}

void BaseEvent::HideScreen::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(transition);
}

QStringList BaseEvent::HideScreen::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Hide Screen: ";
	if (transition == ProjectData::ERASE_FADEOUT)
		viewableText[0] += "Fade Out";
	else if (transition == ProjectData::ERASE_REMOVEBLOCKS)
		viewableText[0] += "Remove Blocks";
	else if (transition == ProjectData::ERASE_WIPEDOWNWARD)
		viewableText[0] += "Wipe Downward";
	else if (transition == ProjectData::ERASE_WIPEUPWARD)
		viewableText[0] += "Wipe Upward";
	else if (transition == ProjectData::ERASE_VENETIANBLINDS)
		viewableText[0] += "Venetian Blinds";
	else if (transition == ProjectData::ERASE_VERTICALBLINDS)
		viewableText[0] += "Vertical Blinds";
	else if (transition == ProjectData::ERASE_HORIZONTALBLINDS)
		viewableText[0] += "Horizontal Blinds";
	else if (transition == ProjectData::ERASE_RECEDINGSQUARE)
		viewableText[0] += "Receding Square";
	else if (transition == ProjectData::ERASE_EXPANDINGSQUARE)
		viewableText[0] += "Expanding Square";
	else if (transition == ProjectData::ERASE_SCREENMOVESUP)
		viewableText[0] += "Screen Moves Up";
	else if (transition == ProjectData::ERASE_SCREENMOVESDOWN)
		viewableText[0] += "Screen Moves Down";
	else if (transition == ProjectData::ERASE_SCREENMOVESLEFT)
		viewableText[0] += "Screen Moves Left";
	else if (transition == ProjectData::ERASE_SCREENMOVESRIGHT)
		viewableText[0] += "Screen Moves Right";
	else if (transition == ProjectData::ERASE_VERTICALDIVISION)
		viewableText[0] += "Vertical Division";
	else if (transition == ProjectData::ERASE_HORIZONTALDIVISION)
		viewableText[0] += "Horizontal Division";
	else if (transition == ProjectData::ERASE_QUADRASECTION)
		viewableText[0] += "Quadrasection";
	else if (transition == ProjectData::ERASE_ZOOMIN)
		viewableText[0] += "Zoom In";
	else if (transition == ProjectData::ERASE_MOSAIC)
		viewableText[0] += "Mosaic";
	else if (transition == ProjectData::ERASE_WAVERSCREEN)
		viewableText[0] += "Waver Screen";
	else if (transition == ProjectData::ERASE_INSTANTANEOUS)
		viewableText[0] += "Instant";
	else if (transition == ProjectData::ERASE_NOTRANSITION)
		viewableText[0] += "No Transition";
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::HideScreen::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	SelectScreenTransition *selectTransition = new SelectScreenTransition(true);
	bool configured = false;
	selectTransition->setTransition(transition);
	if (selectTransition->exec())
	{
		configured = true;
		transition = selectTransition->getTransition();
	}
	delete selectTransition;
	return configured;
}
#endif

bool BaseEvent::HideScreen::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	QStringList transitionList = QStringList() << "fadeOut" << "removeBlocks" << "wipeDownward" << "wipeUpward" << "venetianBlinds" << "verticalBlinds" << "horizontalBlinds" << "recedingSquare" << "expandingSquare" << "screenMovesUp" << "screenMovesDown" << "screenMovesLeft" << "screenMovesRight" << "verticalDivision" << "horizontalDivision" << "quadraSection" << "zoomIn" << "mosaic" << "waverScreen" << "instantaneous" << "noTransition";
	if (!XMLTools::attributeExists(EventNode, "transition"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "transition", transitionList))
		return false;
	return true;
}

BaseEvent::HideScreen::HideScreen(BaseEvent *pBase) : Event(pBase)
{
	transition = ProjectData::ERASE_FADEOUT;
}

BaseEvent::HideScreen::HideScreen(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	QStringList transitionList = QStringList() << "fadeOut" << "removeBlocks" << "wipeDownward" << "wipeUpward" << "venetianBlinds" << "verticalBlinds" << "horizontalBlinds" << "recedingSquare" << "expandingSquare" << "screenMovesUp" << "screenMovesDown" << "screenMovesLeft" << "screenMovesRight" << "verticalDivision" << "horizontalDivision" << "quadraSection" << "zoomIn" << "mosaic" << "waverScreen" << "instantaneous" << "noTransition";
	transition = transitionList.indexOf(EventNode.getAttribute("transition"));
}

BaseEvent::HideScreen::HideScreen(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	transition = storageFile.getSignedInt();
}

BaseEvent::HideScreen::HideScreen(HideScreen *other, BaseEvent *pBase) : Event(pBase)
{
	transition = other->transition;
}
//END BaseEvent::HideScreen Member Functions

//BEGIN BaseEvent::HideScreenOverlay Member Functions
XMLNode BaseEvent::HideScreenOverlay::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	if (transition == ProjectData::ERASE_FADEOUT)
		EventNode.addAttribute("transition", "fadeOut");
	else if (transition == ProjectData::ERASE_REMOVEBLOCKS)
		EventNode.addAttribute("transition", "removeBlocks");
	else if (transition == ProjectData::ERASE_WIPEDOWNWARD)
		EventNode.addAttribute("transition", "wipeDownward");
	else if (transition == ProjectData::ERASE_WIPEUPWARD)
		EventNode.addAttribute("transition", "wipeUpward");
	else if (transition == ProjectData::ERASE_VENETIANBLINDS)
		EventNode.addAttribute("transition", "venetianBlinds");
	else if (transition == ProjectData::ERASE_VERTICALBLINDS)
		EventNode.addAttribute("transition", "verticalBlinds");
	else if (transition == ProjectData::ERASE_HORIZONTALBLINDS)
		EventNode.addAttribute("transition", "horizontalBlinds");
	else if (transition == ProjectData::ERASE_RECEDINGSQUARE)
		EventNode.addAttribute("transition", "recedingSquare");
	else if (transition == ProjectData::ERASE_EXPANDINGSQUARE)
		EventNode.addAttribute("transition", "expandingSquare");
	else if (transition == ProjectData::ERASE_SCREENMOVESUP)
		EventNode.addAttribute("transition", "screenMovesUp");
	else if (transition == ProjectData::ERASE_SCREENMOVESDOWN)
		EventNode.addAttribute("transition", "screenMovesDown");
	else if (transition == ProjectData::ERASE_SCREENMOVESLEFT)
		EventNode.addAttribute("transition", "screenMovesLeft");
	else if (transition == ProjectData::ERASE_SCREENMOVESRIGHT)
		EventNode.addAttribute("transition", "screenMovesRight");
	else if (transition == ProjectData::ERASE_VERTICALDIVISION)
		EventNode.addAttribute("transition", "verticalDivision");
	else if (transition == ProjectData::ERASE_HORIZONTALDIVISION)
		EventNode.addAttribute("transition", "horizontalDivision");
	else if (transition == ProjectData::ERASE_QUADRASECTION)
		EventNode.addAttribute("transition", "quadraSection");
	else if (transition == ProjectData::ERASE_ZOOMIN)
		EventNode.addAttribute("transition", "zoomIn");
	else if (transition == ProjectData::ERASE_MOSAIC)
		EventNode.addAttribute("transition", "mosaic");
	else if (transition == ProjectData::ERASE_WAVERSCREEN)
		EventNode.addAttribute("transition", "waverScreen");
	else if (transition == ProjectData::ERASE_INSTANTANEOUS)
		EventNode.addAttribute("transition", "instantaneous");
	else if (transition == ProjectData::ERASE_NOTRANSITION)
		EventNode.addAttribute("transition", "noTransition");
	return EventNode;
}

void BaseEvent::HideScreenOverlay::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(transition);
}

QStringList BaseEvent::HideScreenOverlay::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Hide Screen Overlay: ";
	if (transition == ProjectData::ERASE_FADEOUT)
		viewableText[0] += "Fade Out";
	else if (transition == ProjectData::ERASE_REMOVEBLOCKS)
		viewableText[0] += "Remove Blocks";
	else if (transition == ProjectData::ERASE_WIPEDOWNWARD)
		viewableText[0] += "Wipe Downward";
	else if (transition == ProjectData::ERASE_WIPEUPWARD)
		viewableText[0] += "Wipe Upward";
	else if (transition == ProjectData::ERASE_VENETIANBLINDS)
		viewableText[0] += "Venetian Blinds";
	else if (transition == ProjectData::ERASE_VERTICALBLINDS)
		viewableText[0] += "Vertical Blinds";
	else if (transition == ProjectData::ERASE_HORIZONTALBLINDS)
		viewableText[0] += "Horizontal Blinds";
	else if (transition == ProjectData::ERASE_RECEDINGSQUARE)
		viewableText[0] += "Receding Square";
	else if (transition == ProjectData::ERASE_EXPANDINGSQUARE)
		viewableText[0] += "Expanding Square";
	else if (transition == ProjectData::ERASE_SCREENMOVESUP)
		viewableText[0] += "Screen Moves Up";
	else if (transition == ProjectData::ERASE_SCREENMOVESDOWN)
		viewableText[0] += "Screen Moves Down";
	else if (transition == ProjectData::ERASE_SCREENMOVESLEFT)
		viewableText[0] += "Screen Moves Left";
	else if (transition == ProjectData::ERASE_SCREENMOVESRIGHT)
		viewableText[0] += "Screen Moves Right";
	else if (transition == ProjectData::ERASE_VERTICALDIVISION)
		viewableText[0] += "Vertical Division";
	else if (transition == ProjectData::ERASE_HORIZONTALDIVISION)
		viewableText[0] += "Horizontal Division";
	else if (transition == ProjectData::ERASE_QUADRASECTION)
		viewableText[0] += "Quadrasection";
	else if (transition == ProjectData::ERASE_ZOOMIN)
		viewableText[0] += "Zoom In";
	else if (transition == ProjectData::ERASE_MOSAIC)
		viewableText[0] += "Mosaic";
	else if (transition == ProjectData::ERASE_WAVERSCREEN)
		viewableText[0] += "Waver Screen";
	else if (transition == ProjectData::ERASE_INSTANTANEOUS)
		viewableText[0] += "Instant";
	else if (transition == ProjectData::ERASE_NOTRANSITION)
		viewableText[0] += "No Transition";
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::HideScreenOverlay::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	SelectScreenTransition *selectTransition = new SelectScreenTransition(true);
	bool configured = false;
	selectTransition->setTransition(transition);
	if (selectTransition->exec())
	{
		configured = true;
		transition = selectTransition->getTransition();
	}
	delete selectTransition;
	return configured;
}
#endif

bool BaseEvent::HideScreenOverlay::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	QStringList transitionList = QStringList() << "fadeOut" << "removeBlocks" << "wipeDownward" << "wipeUpward" << "venetianBlinds" << "verticalBlinds" << "horizontalBlinds" << "recedingSquare" << "expandingSquare" << "screenMovesUp" << "screenMovesDown" << "screenMovesLeft" << "screenMovesRight" << "verticalDivision" << "horizontalDivision" << "quadraSection" << "zoomIn" << "mosaic" << "waverScreen" << "instantaneous" << "noTransition";
	if (!XMLTools::attributeExists(EventNode, "transition"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "transition", transitionList))
		return false;
	return true;
}

BaseEvent::HideScreenOverlay::HideScreenOverlay(BaseEvent *pBase) : Event(pBase)
{
	transition = ProjectData::ERASE_FADEOUT;
}

BaseEvent::HideScreenOverlay::HideScreenOverlay(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	QStringList transitionList = QStringList() << "fadeOut" << "removeBlocks" << "wipeDownward" << "wipeUpward" << "venetianBlinds" << "verticalBlinds" << "horizontalBlinds" << "recedingSquare" << "expandingSquare" << "screenMovesUp" << "screenMovesDown" << "screenMovesLeft" << "screenMovesRight" << "verticalDivision" << "horizontalDivision" << "quadraSection" << "zoomIn" << "mosaic" << "waverScreen" << "instantaneous" << "noTransition";
	transition = transitionList.indexOf(EventNode.getAttribute("transition"));
}

BaseEvent::HideScreenOverlay::HideScreenOverlay(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	transition = storageFile.getSignedInt();
}

BaseEvent::HideScreenOverlay::HideScreenOverlay(HideScreenOverlay *other, BaseEvent *pBase) : Event(pBase)
{
	transition = other->transition;
}
//END BaseEvent::HideScreenOverlay Member Functions

//BEGIN BaseEvent::InsertComment Member Functions
XMLNode BaseEvent::InsertComment::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	QString adjustedComment = "";
	EventNode.addAttribute("type", "type");
	adjustedComment.append(comment);
	adjustedComment.replace("\\", "\\\\");
	adjustedComment.replace("\n", "\\n");
	EventNode.addAttribute("comment", adjustedComment.toUtf8().data());
	return EventNode;
}

void BaseEvent::InsertComment::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putString(comment.toUtf8().data());
}

QStringList BaseEvent::InsertComment::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	QStringList commentLines = comment.split(QChar('\n'));
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "/*";
	if (commentLines.size() == 1)
		viewableText[0] += commentLines[0];
	else if (commentLines.size() > 1)
	{
		int maxLineLength = commentLines[0].size();
		viewableText[0] += "*";
		for (int i = 1; i < commentLines.size(); ++i)
		{
			if (commentLines[i].size() > maxLineLength)
				maxLineLength = commentLines[i].size();
		}
		for (int i = 0; i < maxLineLength; ++i)
			viewableText[0] += "*";
		viewableText[0] += "**\n";
		for (int i = 0; i < commentLines.size(); ++i)
		{
			for (int j = 0; j < indent; ++j)
				viewableText[0] += "  ";
			viewableText[0] += "    * ";
			viewableText[0] += commentLines[i];
			for (int j = commentLines[i].size(); j < maxLineLength; ++j)
				viewableText[0] += " ";
			viewableText[0] += " *\n";
		}
		for (int i = 0; i < indent; ++i)
			viewableText[0] += "  ";
		viewableText[0] += "    **";
		for (int i = 0; i < maxLineLength; ++i)
			viewableText[0] += "*";
		viewableText[0] += "*";
	}
	viewableText[0] += "*/";
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::InsertComment::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	ConfigureEvent_InsertComment *configComment = new ConfigureEvent_InsertComment;
	bool configured = false;
	configComment->setComment(comment);
	if (configComment->exec())
	{
		configured = true;
		comment = configComment->getComment();
	}
	delete configComment;
	return configured;
}
#endif

bool BaseEvent::InsertComment::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	if (!XMLTools::attributeExists(EventNode, "comment"))
		return false;
	return true;
}

BaseEvent::InsertComment::InsertComment(BaseEvent *pBase) : Event(pBase)
{
	comment = "";
}

BaseEvent::InsertComment::InsertComment(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	comment = EventNode.getAttribute("comment");
	comment.replace("\\n", "\n");
	comment.replace("\\\\", "\\");
}

BaseEvent::InsertComment::InsertComment(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	char *temp_string = storageFile.getString();
	comment = temp_string;
	delete[] temp_string;
}

BaseEvent::InsertComment::InsertComment(InsertComment *other, BaseEvent *pBase) : Event(pBase)
{
	comment = "";
	comment.append(other->comment);
}
//END BaseEvent::InsertComment Member Functions

//BEGIN BaseEvent::ItemManagement Member Functions
XMLNode BaseEvent::ItemManagement::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	if (addItem)
		EventNode.addAttribute("operation", "addItem");
	else
		EventNode.addAttribute("operation", "removeItem");
	EventNode.addAttribute("itemID", QString::number(itemID).toUtf8().data());
	EventNode.addAttribute("itemReferencedInVariable", (itemReferencedInVariable) ? "true":"false");
	EventNode.addAttribute("amount", QString::number(amount).toUtf8().data());
	EventNode.addAttribute("amountStoredInVariable", (amountStoredInVariable) ? "true":"false");
	return EventNode;
}

void BaseEvent::ItemManagement::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(itemID);
	storageFile.putSignedInt(amount);
	storageFile.putBool(addItem);
	storageFile.putBool(itemReferencedInVariable);
	storageFile.putBool(amountStoredInVariable);
}

QStringList BaseEvent::ItemManagement::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Item Management: ";
	if (addItem)
		viewableText[0] += "Add ";
	else
		viewableText[0] += "Remove ";
	if (amountStoredInVariable)
		viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(amount));
	else
		viewableText[0] += QString::number(amount);
	viewableText[0] += " ";
	if (itemReferencedInVariable)
		viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(itemID));
	else
		viewableText[0] += ProjectData::itemNames[itemID];
	viewableText[0] += "(s)";
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::ItemManagement::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	ConfigureEvent_ItemManagement *configItems = new ConfigureEvent_ItemManagement;
	bool configured = false;
	configItems->setIsReferencedItem(itemReferencedInVariable);
	configItems->setItemID(itemID);
	configItems->setOperation(addItem);
	configItems->setIsStoredAmount(amountStoredInVariable);
	configItems->setAmount(amount);
	if (configItems->exec())
	{
		configured = true;
		itemID = configItems->getItemID();
		itemReferencedInVariable = configItems->isReferencedItem();
		addItem = configItems->isAddItemOperation();
		amount = configItems->getAmount();
		amountStoredInVariable = configItems->isStoredAmount();
	}
	delete configItems;
	return configured;
}
#endif

bool BaseEvent::ItemManagement::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	QStringList tempStringList = QStringList() << "addItem" << "removeItem";
	if (!XMLTools::attributeExists(EventNode, "operation"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "operation", tempStringList, 1))
		return false;
	if (!XMLTools::attributeExists(EventNode, "characterID"))
		return false;
	tempStringList.clear();
	tempStringList << "true" << "false";
	if (!XMLTools::attributeExists(EventNode, "characterReferencedInVariable"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "characterReferencedInVariable", tempStringList, 1))
		return false;
	if (!XMLTools::attributeExists(EventNode, "amount"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "amountStoredInVariable"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "amountStoredInVariable", tempStringList, 1))
		return false;
	return true;
}

BaseEvent::ItemManagement::ItemManagement(BaseEvent *pBase) : Event(pBase)
{
	itemID = 0;
	amount = 0;
	addItem = true;
	itemReferencedInVariable = false;
	amountStoredInVariable = false;
}

BaseEvent::ItemManagement::ItemManagement(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	addItem = (QString(EventNode.getAttribute("operation")) == "addItem") ? true:false;
	itemID = QString(EventNode.getAttribute("itemID")).toInt();
	itemReferencedInVariable = (QString(EventNode.getAttribute("itemReferencedInVariable")) == "true") ? true:false;
	amount = QString(EventNode.getAttribute("amount")).toInt();
	amountStoredInVariable = (QString(EventNode.getAttribute("amountStoredInVariable")) == "true") ? true:false;
}

BaseEvent::ItemManagement::ItemManagement(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	itemID = storageFile.getSignedInt();
	amount = storageFile.getSignedInt();
	addItem = storageFile.getBool();
	itemReferencedInVariable = storageFile.getBool();
	amountStoredInVariable = storageFile.getBool();
}

BaseEvent::ItemManagement::ItemManagement(ItemManagement *other, BaseEvent *pBase) : Event(pBase)
{
	itemID = other->itemID;
	amount = other->amount;
	addItem = other->addItem;
	itemReferencedInVariable = other->itemReferencedInVariable;
	amountStoredInVariable = other->amountStoredInVariable;
}
//END BaseEvent::ItemManagement Member Functions

//BEGIN BaseEvent::JumpToLabel Member Functions
XMLNode BaseEvent::JumpToLabel::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	EventNode.addAttribute("labelID", QString::number(labelID).toUtf8().data());
	return EventNode;
}

void BaseEvent::JumpToLabel::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(labelID);
}

QStringList BaseEvent::JumpToLabel::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Jump to Label: ";
	viewableText[0] += QString::number(labelID);
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::JumpToLabel::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	ConfigureEvent_JumpToLabel *configJump = new ConfigureEvent_JumpToLabel;
	bool configured = false;
	configJump->setLabel(labelID);
	if (configJump->exec())
	{
		configured = true;
		labelID = configJump->getLabel();
	}
	delete configJump;
	return configured;
}
#endif

bool BaseEvent::JumpToLabel::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	if (!XMLTools::attributeExists(EventNode, "labelID"))
		return false;
	if (!XMLTools::attributeIntRangeValid(EventNode, "labelID", 0, 99))
		return false;
	return true;
}

BaseEvent::JumpToLabel::JumpToLabel(BaseEvent *pBase) : Event(pBase)
{
	labelID = 0;
}

BaseEvent::JumpToLabel::JumpToLabel(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	labelID = QString(EventNode.getAttribute("labelID")).toInt();
}

BaseEvent::JumpToLabel::JumpToLabel(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	labelID = storageFile.getSignedInt();
}

BaseEvent::JumpToLabel::JumpToLabel(JumpToLabel *other, BaseEvent *pBase) : Event(pBase)
{
	labelID = other->labelID;
}
//END BaseEvent::JumpToLabel Member Functions

//BEGIN BaseEvent::KeyInputProcessing Member Functions
XMLNode BaseEvent::KeyInputProcessing::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	EventNode.addAttribute("keyPressVariableID", QString::number(variable1ID).toUtf8().data());
	EventNode.addAttribute("waitForKeyPress", (waitForKeyPress) ? "true":"false");
	if (waitForKeyPress)
	{
		EventNode.addAttribute("storeTimeForKeyPress", (storeTimeForKeyPress) ? "true":"false");
		if (storeTimeForKeyPress)
			EventNode.addAttribute("timeTakenVariableID", QString::number(variable2ID).toUtf8().data());
	}
	return EventNode;
}

void BaseEvent::KeyInputProcessing::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(variable1ID);
	storageFile.putSignedInt(variable2ID);
	storageFile.putBool(waitForKeyPress);
	storageFile.putBool(storeTimeForKeyPress);
}

QStringList BaseEvent::KeyInputProcessing::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Key Input Processing: ";
	viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(variable1ID));
	if (waitForKeyPress)
	{
		viewableText[0] += ", Wait";
		if (storeTimeForKeyPress)
		{
			viewableText[0] += ", Store Time Taken in ";
			viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(variable2ID));
		}
	}
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::KeyInputProcessing::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	ConfigureEvent_KeyInputProcessing *configInput = new ConfigureEvent_KeyInputProcessing;
	bool configured = false;
	configInput->setVariableID(variable1ID);
	configInput->setWaitForKeyPress(waitForKeyPress);
	configInput->setStoreTimeTaken(storeTimeForKeyPress);
	configInput->setTimeTakenVariableID(variable2ID);
	if (configInput->exec())
	{
		configured = true;
		variable1ID = configInput->getVariableID();
		waitForKeyPress = configInput->waitForKeyPress();
		storeTimeForKeyPress = configInput->storeTimeTaken();
		variable2ID = configInput->getTimeTakenVariableID();
	}
	delete configInput;
	return configured;
}
#endif

bool BaseEvent::KeyInputProcessing::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	QStringList trueFalseList = QStringList() << "true" << "false";
	if (!XMLTools::attributeExists(EventNode," keyPressVariableID"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "waitForKeyPress"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "waitForKeyPress", trueFalseList, 1))
		return false;
	if (QString(EventNode.getAttribute("waitForKeyPress")) == "true")
	{
		if (!XMLTools::attributeExists(EventNode, "storeTimeForKeyPress"))
			return false;
		if (!XMLTools::attributeStringValid(EventNode, "storeTimeForKeyPress", trueFalseList, 1))
			return false;
		if (QString(EventNode.getAttribute("storeTimeForKeyPress")) == "true")
		{
			if (!XMLTools::attributeExists(EventNode, "timeTakenVariableID"))
				return false;
		}
	}
	return true;
}

BaseEvent::KeyInputProcessing::KeyInputProcessing(BaseEvent *pBase) : Event(pBase)
{
	variable1ID = 0;
	variable2ID = 0;
	waitForKeyPress = false;
	storeTimeForKeyPress = false;
}

BaseEvent::KeyInputProcessing::KeyInputProcessing(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	variable1ID = QString(EventNode.getAttribute("keyPressVariableID")).toInt();
	waitForKeyPress = (QString(EventNode.getAttribute("waitForKeyPress")) == "true") ? true:false;
	if (waitForKeyPress)
	{
		storeTimeForKeyPress = (QString(EventNode.getAttribute("storeTimeForKeyPress")) == "true") ? true:false;
		if (storeTimeForKeyPress)
			variable2ID = QString(EventNode.getAttribute("timeTakenVariableID")).toInt();
	}
}

BaseEvent::KeyInputProcessing::KeyInputProcessing(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	variable1ID = storageFile.getSignedInt();
	variable2ID = storageFile.getSignedInt();
	waitForKeyPress = storageFile.getBool();
	storeTimeForKeyPress = storageFile.getBool();
}

BaseEvent::KeyInputProcessing::KeyInputProcessing(KeyInputProcessing *other, BaseEvent *pBase) : Event(pBase)
{
	variable1ID = other->variable1ID;
	variable2ID = other->variable2ID;
	waitForKeyPress = other->waitForKeyPress;
	storeTimeForKeyPress = other->storeTimeForKeyPress;
}
//END BaseEvent::KeyInputProcessing Member Functions

//BEGIN BaseEvent::Label Member Functions
XMLNode BaseEvent::Label::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	EventNode.addAttribute("labelID", QString::number(labelID).toUtf8().data());
	return EventNode;
}

void BaseEvent::Label::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(labelID);
}

QStringList BaseEvent::Label::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Label: ";
	viewableText[0] += QString::number(labelID);
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::Label::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	ConfigureEvent_Label *configJump = new ConfigureEvent_Label;
	bool configured = false;
	configJump->setLabel(labelID);
	if (configJump->exec())
	{
		configured = true;
		labelID = configJump->getLabel();
	}
	delete configJump;
	return configured;
}
#endif

bool BaseEvent::Label::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	if (!XMLTools::attributeExists(EventNode, "labelID"))
		return false;
	if (!XMLTools::attributeIntRangeValid(EventNode, "labelID", 0, 99))
		return false;
	return true;
}

BaseEvent::Label::Label(BaseEvent *pBase) : Event(pBase)
{
	labelID = 0;
}

BaseEvent::Label::Label(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	labelID = QString(EventNode.getAttribute("labelID")).toInt();
}

BaseEvent::Label::Label(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	labelID = storageFile.getSignedInt();
}

BaseEvent::Label::Label(Label *other, BaseEvent *pBase) : Event(pBase)
{
	labelID = other->labelID;
}
//END BaseEvent::Label Member Functions

//BEGIN BaseEvent::LoadGame Member Functions
XMLNode BaseEvent::LoadGame::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	EventNode.addAttribute("slot", QString::number(slot).toUtf8().data());
	EventNode.addAttribute("slotReferencedInVariable", (slotReferencedInVariable) ? "true":"false");
	return EventNode;
}

void BaseEvent::LoadGame::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(slot);
	storageFile.putBool(slotReferencedInVariable);
}

QStringList BaseEvent::LoadGame::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Load Game: Slot=";
	if (slotReferencedInVariable)
		viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(slot));
	else
		viewableText[0] += QString::number(slot);
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::LoadGame::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	ConfigureEvent_LoadGame *configLoadGame = new ConfigureEvent_LoadGame;
	bool configured = false;
	configLoadGame->setIsReferencedSaveSlot(slotReferencedInVariable);
	configLoadGame->setSaveSlotID(slot);
	if (configLoadGame->exec())
	{
		configured = true;
		slot = configLoadGame->getSaveSlotID();
		slotReferencedInVariable = configLoadGame->isReferencedSaveSlot();
	}
	delete configLoadGame;
	return configured;
}
#endif

bool BaseEvent::LoadGame::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	if (!XMLTools::attributeExists(EventNode, "slot"))
		return false;
	if (!XMLTools::attributeIntRangeValid(EventNode, "slot", 1, 16))
		return false;
	if (!XMLTools::attributeExists(EventNode, "slotReferencedInVariable"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "slotReferencedInVariable", QStringList() << "true" << "false", 1))
		return false;
	return true;
}

BaseEvent::LoadGame::LoadGame(BaseEvent *pBase) : Event(pBase)
{
	slot = 1;
	slotReferencedInVariable = false;
}

BaseEvent::LoadGame::LoadGame(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	slot = QString(EventNode.getAttribute("slot")).toInt();
	slotReferencedInVariable = (QString(EventNode.getAttribute("slotReferencedInVariable")) == "true") ? true:false;
}

BaseEvent::LoadGame::LoadGame(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	slot = storageFile.getSignedInt();
	slotReferencedInVariable = storageFile.getBool();
}

BaseEvent::LoadGame::LoadGame(LoadGame *other, BaseEvent *pBase) : Event(pBase)
{
	slot = other->slot;
	slotReferencedInVariable = other->slotReferencedInVariable;
}
//END BaseEvent::LoadGame Member Functions

//BEGIN BaseEvent::Loop Member Functions
BaseEvent::Loop::~Loop()
{
	while (events.size() > 0)
	{
		delete (events.last());
		events.removeLast();
	}
}

XMLNode BaseEvent::Loop::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	for (int i = 0; i < events.size(); ++i)
		EventNode.addChild(events[i]->getEventNode());
	return EventNode;
}

void BaseEvent::Loop::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(events.size());
	for (int i = 0; i < events.size(); ++i)
		events[i]->saveToStorageFile(storageFile);
}

QStringList BaseEvent::Loop::getResources()
{
	QStringList resources;
	for (int i = 0; i < events.size(); ++i)
		resources.append(events[i]->getResources());
	return resources;
}

QStringList BaseEvent::Loop::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Loop";
	for (int i = 0; i < events.size(); ++i)
		viewableText += events[i]->getViewableText(indent + 1);
	viewableText.append("");
	for (int i = 0; i < indent + 1; ++i)
		viewableText[viewableText.size() - 1] += "  ";
	viewableText[viewableText.size() - 1] += "<>";
	viewableText.append("");
	for (int i = 0; i < indent; ++i)
		viewableText[viewableText.size() - 1] += "  ";
	viewableText[viewableText.size() - 1] += "::End";
	return viewableText;
}

QList<BaseEvent*> BaseEvent::Loop::getEventPointers()
{
	QList<BaseEvent*> pointers;
	for (int i = 0; i < events.size(); ++i)
		pointers += events[i]->getEventPointers();
	pointers.append(NULL);
	return pointers;
}

void BaseEvent::Loop::addEvent(BaseEvent *event, int before)
{
	if (before == -1)
		events.append(event);
	else
		events.insert(before, event);
}

void BaseEvent::Loop::deleteEvent(int location)
{
	delete (events[location]);
	events.removeAt(location);
}

bool BaseEvent::Loop::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	for (int i = 0, count = EventNode.nChildNode("event"); i < count; ++i)
	{
		if (!BaseEvent::tryLoad(EventNode.getChildNode("event", i), baseType))
			return false;
	}
	return true;
}

BaseEvent::Loop::Loop(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	for (int i = 0, count = EventNode.nChildNode("event"); i < count; ++i)
		events.append(new BaseEvent(pBaseEvent->baseType, EventNode.getChildNode("event", i)));
}

BaseEvent::Loop::Loop(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	for (int i = 0, count = storageFile.getSignedInt(); i < count; ++i)
		events.append(new BaseEvent(pBaseEvent->baseType, storageFile));
}

BaseEvent::Loop::Loop(Loop *other, BaseEvent *pBase) : Event(pBase)
{
	for (int i = 0; i < other->events.size(); ++i)
		events.append(new BaseEvent(other->events[i]));
}
//END BaseEvent::Loop Member Functions

//BEGIN BaseEvent::MemorizePosition Member Functions
XMLNode BaseEvent::MemorizePosition::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	EventNode.addAttribute("mapVariableID", QString::number(mapVariableID).toUtf8().data());
	EventNode.addAttribute("xVariableID", QString::number(xVariableID).toUtf8().data());
	EventNode.addAttribute("yVariableID", QString::number(yVariableID).toUtf8().data());
	EventNode.addAttribute("layerVariableID", QString::number(layerVariableID).toUtf8().data());
	return EventNode;
}

void BaseEvent::MemorizePosition::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(mapVariableID);
	storageFile.putSignedInt(xVariableID);
	storageFile.putSignedInt(yVariableID);
	storageFile.putSignedInt(layerVariableID);
}

QStringList BaseEvent::MemorizePosition::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Memorize Position: ";
	viewableText[0] += QString("map[%1] ").arg(ProjectData::numberVariableIdToString(mapVariableID));
	viewableText[0] += QString("x[%1] ").arg(ProjectData::numberVariableIdToString(xVariableID));
	viewableText[0] += QString("y[%1] ").arg(ProjectData::numberVariableIdToString(yVariableID));
	viewableText[0] += QString("layer[%1]").arg(ProjectData::numberVariableIdToString(layerVariableID));
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::MemorizePosition::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	ConfigureEvent_MemorizePosition *configEvent = new ConfigureEvent_MemorizePosition;
	bool configured = false;
	configEvent->setMapVariableID(mapVariableID);
	configEvent->setXVariableID(xVariableID);
	configEvent->setYVariableID(yVariableID);
	configEvent->setLayerVariableID(layerVariableID);
	if (configEvent->exec())
	{
		configured = true;
		mapVariableID = configEvent->getMapVariableID();
		xVariableID = configEvent->getXVariableID();
		yVariableID = configEvent->getYVariableID();
		layerVariableID = configEvent->getLayerVariableID();
	}
	delete configEvent;
	return configured;
}
#endif

bool BaseEvent::MemorizePosition::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	if (!XMLTools::attributeExists(EventNode, "mapVariableID"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "xVariableID"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "yVariableID"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "layerVariableID"))
		return false;
	return true;
}

BaseEvent::MemorizePosition::MemorizePosition(BaseEvent *pBase) : Event(pBase)
{
	mapVariableID = 0;
	xVariableID = 0;
	yVariableID = 0;
	layerVariableID = 0;
}

BaseEvent::MemorizePosition::MemorizePosition(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	mapVariableID = QString(EventNode.getAttribute("mapVariableID")).toInt();
	xVariableID = QString(EventNode.getAttribute("xVariableID")).toInt();
	yVariableID = QString(EventNode.getAttribute("yVariableID")).toInt();
	layerVariableID = QString(EventNode.getAttribute("layerVariableID")).toInt();
}

BaseEvent::MemorizePosition::MemorizePosition(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	mapVariableID = storageFile.getSignedInt();
	xVariableID = storageFile.getSignedInt();
	yVariableID = storageFile.getSignedInt();
	layerVariableID = storageFile.getSignedInt();
}

BaseEvent::MemorizePosition::MemorizePosition(MemorizePosition *other, BaseEvent *pBase) : Event(pBase)
{
	mapVariableID = other->mapVariableID;
	xVariableID = other->xVariableID;
	yVariableID = other->yVariableID;
	layerVariableID = other->layerVariableID;
}
//END BaseEvent::MemorizePosition Member Functions

//BEGIN BaseEvent::MoveEvent Member Functions
BaseEvent::MoveEvent::~MoveEvent()
{
	for (int i = 0; i < pattern.size(); ++i)
		delete (pattern[i]);
	pattern.clear();
}

XMLNode BaseEvent::MoveEvent::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	if (eventType == HERO)
		EventNode.addAttribute("eventType", "hero");
	else if (eventType == THIS_EVENT)
		EventNode.addAttribute("eventType", "thisEvent");
	else if (eventType == VEHICLE)
	{
		EventNode.addAttribute("eventType", "vehicle");
		EventNode.addAttribute("vehicleID", QString::number(eventID).toUtf8().data());
	}
	else if (eventType == EVENT)
	{
		EventNode.addAttribute("eventType", "event");
		EventNode.addAttribute("eventID", QString::number(eventID).toUtf8().data());
	}
	EventNode.addAttribute("ignoreImpossibleMoves", (ignoreImpossibleMoves) ? "true":"false");
	for (int i = 0; i < pattern.size(); ++i)
		EventNode.addChild(pattern[i]->getPatternNode());
	return EventNode;
}

void BaseEvent::MoveEvent::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(eventType);
	storageFile.putSignedInt(eventID);
	storageFile.putBool(ignoreImpossibleMoves);
	storageFile.putSignedInt(pattern.size());
	for (int i = 0; i < pattern.size(); ++i)
		pattern[i]->saveToStorageFile(storageFile);
}

QStringList BaseEvent::MoveEvent::getResources()
{
	QStringList resources = QStringList();
	for (int i = 0; i < pattern.size(); ++i)
	{
		if (pattern[i]->type == MapEvent::PATTERN_CHANGEGRAPHIC)
			resources.append(pattern[i]->stringValue);
		else if (pattern[i]->type == MapEvent::PATTERN_PLAYSOUNDEFFECT)
			resources.append(pattern[i]->soundEffect.location);
	}
	return resources;
}

QStringList BaseEvent::MoveEvent::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Move Event: ";
	if (eventType == HERO)
		viewableText[0] += "Hero";
	else if (eventType == THIS_EVENT)
		viewableText[0] += "This Event";
	else if (eventType == VEHICLE)
		viewableText[0] += ProjectData::vehicleNames[eventID];
	else if (eventType == EVENT)
		viewableText[0] += QString("Event%1").arg(eventID, 3, 10, QChar('0'));
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::MoveEvent::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerType);
	ConfigureEvent_MoveEvent *configMoveEvent = new ConfigureEvent_MoveEvent(callerID);
	bool configured = false;
	configMoveEvent->setEventType(eventType);
	configMoveEvent->setEventID(eventID);
	configMoveEvent->setIgnoreImpossibleMoves(ignoreImpossibleMoves);
	for (int i = 0; i < pattern.size(); ++i)
		configMoveEvent->addPattern(pattern[i]);
	if (configMoveEvent->exec())
	{
		configured = true;
		eventType = configMoveEvent->getEventType();
		eventID = configMoveEvent->getEventID();
		ignoreImpossibleMoves = configMoveEvent->getIgnoreImpossibleMoves();
		for (int i = 0; i < pattern.size(); ++i)
			delete (pattern[i]);
		pattern.clear();
		for (int i = 0; i < configMoveEvent->numPatterns(); ++i)
			pattern.append(new MapEvent::MovementPattern(configMoveEvent->getPattern(i)));
	}
	delete configMoveEvent;
	return configured;
}
#endif

bool BaseEvent::MoveEvent::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	QStringList tempStringList = QStringList() << "hero" << "thisEvent" << "vehicle" << "event";
	QString tempString;
	if (!XMLTools::attributeExists(EventNode, "eventType"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "eventType", tempStringList))
		return false;
	tempString = EventNode.getAttribute("eventType");
	if (tempString == "vehicle")
	{
		if (!XMLTools::attributeExists(EventNode, "vehicleID"))
			return false;
	}
	if (tempString == "event")
	{
		if (!XMLTools::attributeExists(EventNode, "eventID"))
			return false;
	}
	for (int i = 0; i < EventNode.nChildNode("pattern"); ++i)
	{
		if (!MapEvent::MovementPattern::tryLoad(EventNode.getChildNode("pattern", i)))
			return false;
	}
	return true;
}

BaseEvent::MoveEvent::MoveEvent(BaseEvent *pBase) : Event(pBase)
{
	eventType = HERO;
	eventID = -1;
	ignoreImpossibleMoves = false;
}

BaseEvent::MoveEvent::MoveEvent(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	QString tempString = EventNode.getAttribute("eventType");
	if (tempString == "hero")
	{
		eventType = HERO;
		eventID = -1;
	}
	else if (tempString == "thisEvent")
	{
		eventType = THIS_EVENT;
		eventID = -1;
	}
	else if (tempString == "vehicle")
	{
		eventType = VEHICLE;
		eventID = QString(EventNode.getAttribute("vehicleID")).toInt();
	}
	else if (tempString == "event")
	{
		eventType = EVENT;
		eventID = QString(EventNode.getAttribute("eventID")).toInt();
	}
	ignoreImpossibleMoves = (QString(EventNode.getAttribute("ignoreImpossibleMoves")) == "true") ? true:false;
	for (int i = 0; i < EventNode.nChildNode("pattern"); ++i)
		pattern.append(new MapEvent::MovementPattern(EventNode.getChildNode("pattern", i)));
}

BaseEvent::MoveEvent::MoveEvent(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	eventType = storageFile.getSignedInt();
	eventID = storageFile.getSignedInt();
	ignoreImpossibleMoves = storageFile.getBool();
	for (int i = 0, count = storageFile.getSignedInt(); i < count; ++i)
		pattern.append(new MapEvent::MovementPattern(storageFile));
}

BaseEvent::MoveEvent::MoveEvent(MoveEvent *other, BaseEvent *pBase) : Event(pBase)
{
	eventType = other->eventType;
	eventID = other->eventID;
	ignoreImpossibleMoves = other->ignoreImpossibleMoves;
	for (int i = 0; i < other->pattern.size(); ++i)
		pattern.append(new MapEvent::MovementPattern(other->pattern[i]));
}
//END BaseEvent::MoveEvent Member Functions

//BEGIN BaseEvent::OpenMenu Member Functions
XMLNode BaseEvent::OpenMenu::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	EventNode.addAttribute("menuID", QString::number(menuID).toUtf8().data());
	EventNode.addAttribute("menuReferencedInVariable", (menuReferencedInVariable) ? "true":"false");
	return EventNode;
}

void BaseEvent::OpenMenu::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(menuID);
	storageFile.putBool(menuReferencedInVariable);
}

QStringList BaseEvent::OpenMenu::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Open Menu: ";
	if (menuReferencedInVariable)
		viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(menuID));
	else
		viewableText[0] += ProjectData::mainMenuLayoutNames[menuID];
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::OpenMenu::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	ConfigureEvent_OpenMenu *configOpenMenu = new ConfigureEvent_OpenMenu;
	bool configured = false;
	configOpenMenu->setIsReferencedMenu(menuReferencedInVariable);
	configOpenMenu->setMenuID(menuID);
	if (configOpenMenu->exec())
	{
		configured = true;
		menuID = configOpenMenu->getMenuID();
		menuReferencedInVariable = configOpenMenu->isReferencedMenu();
	}
	delete configOpenMenu;
	return configured;
}
#endif

bool BaseEvent::OpenMenu::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	if (!XMLTools::attributeExists(EventNode, "menuID"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "menuReferencedInVariable"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "menuReferencedInVariable", QStringList() << "true" << "false", 1))
		return false;
	return true;
}

BaseEvent::OpenMenu::OpenMenu(BaseEvent *pBase) : Event(pBase)
{
	menuID = 0;
	menuReferencedInVariable = false;
}

BaseEvent::OpenMenu::OpenMenu(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	menuID = QString(EventNode.getAttribute("menuID")).toInt();
	menuReferencedInVariable = (QString(EventNode.getAttribute("menuReferencedInVariable")) == "true") ? true:false;
}

BaseEvent::OpenMenu::OpenMenu(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	menuID = storageFile.getSignedInt();
	menuReferencedInVariable = storageFile.getBool();
}

BaseEvent::OpenMenu::OpenMenu(OpenMenu *other, BaseEvent *pBase) : Event(pBase)
{
	menuID = other->menuID;
	menuReferencedInVariable = other->menuReferencedInVariable;
}
//END BaseEvent::OpenMenu Member Functions

//BEGIN BaseEvent::PanScreen Member Functions
XMLNode BaseEvent::PanScreen::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	if (operation == LOCK_PAN_STATE)
		EventNode.addAttribute("operation", "lockPanState");
	else if (operation == UNLOCK_PAN_STATE)
		EventNode.addAttribute("operation", "unlockPanState");
	else if (operation == PAN_SCREEN)
	{
		EventNode.addAttribute("operation", "panScreen");
		if (pan == DIRECTION)
		{
			EventNode.addAttribute("pan", "direction");
			if (panDirection.direction == UP)
				EventNode.addAttribute("direction", "up");
			else if (panDirection.direction == DOWN)
				EventNode.addAttribute("direction", "down");
			else if (panDirection.direction == LEFT)
				EventNode.addAttribute("direction", "left");
			else if (panDirection.direction == RIGHT)
				EventNode.addAttribute("direction", "right");
			if (transitionSpeed == SPEED_ONEEIGHTHNORMAL)
				EventNode.addAttribute("transitionSpeed", "1/8 normal");
			else if (transitionSpeed == SPEED_ONEFOURTHNORMAL)
				EventNode.addAttribute("transitionSpeed", "1/4 normal");
			else if (transitionSpeed == SPEED_ONEHALFNORMAL)
				EventNode.addAttribute("transitionSpeed", "1/2 normal");
			else if (transitionSpeed == SPEED_NORMAL)
				EventNode.addAttribute("transitionSpeed", "normal");
			else if (transitionSpeed == SPEED_TWICENORMAL)
				EventNode.addAttribute("transitionSpeed", "2x normal");
			else if (transitionSpeed == SPEED_FOURTIMESNORMAL)
				EventNode.addAttribute("transitionSpeed", "4x normal");
			EventNode.addAttribute("tiles", QString::number(panDirection.tiles).toUtf8().data());
			EventNode.addAttribute("haltOtherProcesses", (haltOtherProcesses) ? "true":"false");
		}
		else if (pan == LOCATION)
		{
			EventNode.addAttribute("pan", "location");
			EventNode.addAttribute("x", QString::number(panLocation.x).toUtf8().data());
			EventNode.addAttribute("y", QString::number(panLocation.y).toUtf8().data());
			EventNode.addAttribute("coordinatesStoredInVariables", (panLocation.coordinatesStoredInVariables) ? "true":"false");
			if (transitionSpeed == SPEED_ONEEIGHTHNORMAL)
				EventNode.addAttribute("transitionSpeed", "1/8 normal");
			else if (transitionSpeed == SPEED_ONEFOURTHNORMAL)
				EventNode.addAttribute("transitionSpeed", "1/4 normal");
			else if (transitionSpeed == SPEED_ONEHALFNORMAL)
				EventNode.addAttribute("transitionSpeed", "1/2 normal");
			else if (transitionSpeed == SPEED_NORMAL)
				EventNode.addAttribute("transitionSpeed", "normal");
			else if (transitionSpeed == SPEED_TWICENORMAL)
				EventNode.addAttribute("transitionSpeed", "2x normal");
			else if (transitionSpeed == SPEED_FOURTIMESNORMAL)
				EventNode.addAttribute("transitionSpeed", "4x normal");
			EventNode.addAttribute("haltOtherProcesses", (haltOtherProcesses) ? "true":"false");
		}
	}
	else if (operation == RETURN_TO_ORIGIN)
	{
		EventNode.addAttribute("operation", "returnToOrigin");
		if (transitionSpeed == SPEED_ONEEIGHTHNORMAL)
			EventNode.addAttribute("transitionSpeed", "1/8 normal");
		else if (transitionSpeed == SPEED_ONEFOURTHNORMAL)
			EventNode.addAttribute("transitionSpeed", "1/4 normal");
		else if (transitionSpeed == SPEED_ONEHALFNORMAL)
			EventNode.addAttribute("transitionSpeed", "1/2 normal");
		else if (transitionSpeed == SPEED_NORMAL)
			EventNode.addAttribute("transitionSpeed", "normal");
		else if (transitionSpeed == SPEED_TWICENORMAL)
			EventNode.addAttribute("transitionSpeed", "2x normal");
		else if (transitionSpeed == SPEED_FOURTIMESNORMAL)
			EventNode.addAttribute("transitionSpeed", "4x normal");
		EventNode.addAttribute("haltOtherProcesses", (haltOtherProcesses) ? "true":"false");
	}
	return EventNode;
}

void BaseEvent::PanScreen::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(operation);
	if (operation == PAN_SCREEN)
	{
		storageFile.putSignedInt(pan);
		if (pan == DIRECTION)
		{
			storageFile.putSignedInt(panDirection.direction);
			storageFile.putSignedInt(panDirection.tiles);
		}
		else if (pan == LOCATION)
		{
			storageFile.putSignedInt(panLocation.x);
			storageFile.putSignedInt(panLocation.y);
			storageFile.putBool(panLocation.coordinatesStoredInVariables);
		}
		storageFile.putSignedInt(transitionSpeed);
		storageFile.putBool(haltOtherProcesses);
	}
	else if (operation == RETURN_TO_ORIGIN)
	{
		storageFile.putSignedInt(transitionSpeed);
		storageFile.putBool(haltOtherProcesses);
	}
}

QStringList BaseEvent::PanScreen::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Pan Screen: ";
	if (operation == LOCK_PAN_STATE)
		viewableText[0] += "Lock Pan State";
	else if (operation == UNLOCK_PAN_STATE)
		viewableText[0] += "Unlock Pan State";
	else if (operation == PAN_SCREEN)
	{
		if (pan == DIRECTION)
		{
			if (panDirection.direction == UP)
				viewableText[0] += "Pan Up ";
			else if (panDirection.direction == DOWN)
				viewableText[0] += "Pan Down ";
			else if (panDirection.direction == LEFT)
				viewableText[0] += "Pan Left ";
			else if (panDirection.direction == RIGHT)
				viewableText[0] += "Pan Right ";
			viewableText[0] += QString("%1 tiles").arg(panDirection.tiles);
		}
		else if (pan == LOCATION)
		{
			viewableText[0] += "Pan to (";
			if (panLocation.coordinatesStoredInVariables)
			{
				viewableText[0] += QString("variable[%1], ").arg(ProjectData::numberVariableIdToString(panLocation.x));
				viewableText[0] += QString("variable[%1])").arg(ProjectData::numberVariableIdToString(panLocation.y));
			}
			else
				viewableText[0] += QString("%1, %2)").arg(panLocation.x).arg(panLocation.y);
		}
		if (haltOtherProcesses)
			viewableText[0] += " (Halt Other Processes During Transition)";
	}
	else if (operation == RETURN_TO_ORIGIN)
	{
		viewableText[0] += "Return To Origin";
		if (haltOtherProcesses)
			viewableText[0] += " (Halt Other Processes During Transition)";
	}
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::PanScreen::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerType);
	ConfigureEvent_PanScreen *configPanScreen = new ConfigureEvent_PanScreen(callerID);
	bool configured = false;
	configPanScreen->setOperation(operation);
	if (operation == PAN_SCREEN)
	{
		configPanScreen->setPan(pan);
		if (pan == DIRECTION)
		{
			configPanScreen->setDirection(panDirection.direction);
			configPanScreen->setTiles(panDirection.tiles);
		}
		else if (pan == LOCATION)
		{
			configPanScreen->setXLocation(panLocation.x);
			configPanScreen->setYLocation(panLocation.y);
			configPanScreen->setCoordinatesStoredInVariables(panLocation.coordinatesStoredInVariables);
		}
		configPanScreen->setTransitionSpeed(transitionSpeed);
		configPanScreen->setHaltOtherProcesses(haltOtherProcesses);
	}
	else if (operation == RETURN_TO_ORIGIN)
	{
		configPanScreen->setTransitionSpeed(transitionSpeed);
		configPanScreen->setHaltOtherProcesses(haltOtherProcesses);
	}
	if (configPanScreen->exec())
	{
		configured = true;
		operation = configPanScreen->getOperation();
		if (operation == PAN_SCREEN)
		{
			pan = configPanScreen->getPan();
			if (pan == DIRECTION)
			{
				panDirection.direction = configPanScreen->getDirection();
				panDirection.tiles = configPanScreen->getTiles();
			}
			else if (pan == LOCATION)
			{
				panLocation.x = configPanScreen->getXLocation();
				panLocation.y = configPanScreen->getYLocation();
				panLocation.coordinatesStoredInVariables = configPanScreen->getCoordinatesStoredInVariables();
			}
			transitionSpeed = configPanScreen->getTransitionSpeed();
			haltOtherProcesses = configPanScreen->getHaltOtherProcesses();
		}
		else if (operation == RETURN_TO_ORIGIN)
		{
			transitionSpeed = configPanScreen->getTransitionSpeed();
			haltOtherProcesses = configPanScreen->getHaltOtherProcesses();
		}
	}
	delete configPanScreen;
	return configured;
}
#endif

bool BaseEvent::PanScreen::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	QStringList tempStringList;
	QStringList trueFalseList = QStringList() << "true" << "false";
	QString tempString;
	tempStringList.clear();
	tempStringList << "lockPanState" << "unlockPanState" << "panScreen" << "returnToOrigin";
	if (!XMLTools::attributeExists(EventNode, "operation"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "operation", tempStringList))
		return false;
	tempString = EventNode.getAttribute("operation");
	if (tempString == "panScreen")
	{
		tempStringList.clear();
		tempStringList << "direction" << "location";
		if (!XMLTools::attributeExists(EventNode, "pan"))
			return false;
		if (!XMLTools::attributeStringValid(EventNode, "pan", tempStringList))
			return false;
		tempString = EventNode.getAttribute("pan");
		if (tempString == "direction")
		{
			tempStringList.clear();
			tempStringList << "up" << "down" << "left" << "right";
			if (!XMLTools::attributeExists(EventNode, "direction"))
				return false;
			if (!XMLTools::attributeStringValid(EventNode, "direction", tempStringList))
				return false;
			if (!XMLTools::attributeExists(EventNode, "tiles"))
				return false;
			if (!XMLTools::attributeIntRangeValid(EventNode, "tiles", 1, 100))
				return false;
		}
		else if (tempString == "location")
		{
			if (!XMLTools::attributeExists(EventNode, "x"))
				return false;
			if (!XMLTools::attributeExists(EventNode, "y"))
				return false;
			if (!XMLTools::attributeExists(EventNode, "coordinatesStoredInVariables"))
				return false;
			if (!XMLTools::attributeStringValid(EventNode, "coordinatesStoredInVariables", trueFalseList, 1))
				return false;
		}
		tempStringList.clear();
		tempStringList << "1/8 normal" << "1/4 normal" << "1/2 normal" << "normal" << "2x normal" << "4x normal";
		if (!XMLTools::attributeExists(EventNode, "transitionType"))
			return false;
		if (!XMLTools::attributeStringValid(EventNode, "transitionType", tempStringList))
			return false;
		if (!XMLTools::attributeExists(EventNode, "haltOtherProcesses"))
			return false;
		if (!XMLTools::attributeStringValid(EventNode, "haltOtherProcesses", trueFalseList, 1))
			return false;
	}
	else if (tempString == "returnToOrigin")
	{
		tempStringList.clear();
		tempStringList << "1/8 normal" << "1/4 normal" << "1/2 normal" << "normal" << "2x normal" << "4x normal";
		if (!XMLTools::attributeExists(EventNode, "transitionType"))
			return false;
		if (!XMLTools::attributeStringValid(EventNode, "transitionType", tempStringList))
			return false;
		if (!XMLTools::attributeExists(EventNode, "haltOtherProcesses"))
			return false;
		if (!XMLTools::attributeStringValid(EventNode, "haltOtherProcesses", trueFalseList, 1))
			return false;
	}
	return true;
}

BaseEvent::PanScreen::PanScreen(BaseEvent *pBase) : Event(pBase)
{
	operation = LOCK_PAN_STATE;
	pan = DIRECTION;
	panDirection.direction = UP;
	panDirection.tiles = 1;
	panLocation.x = 0;
	panLocation.y = 0;
	panLocation.coordinatesStoredInVariables = false;
	transitionSpeed = SPEED_NORMAL;
	haltOtherProcesses = false;
}

BaseEvent::PanScreen::PanScreen(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	QString tempString = EventNode.getAttribute("operation");
	if (tempString == "lockPanState")
		operation = LOCK_PAN_STATE;
	else if (tempString == "unlockPanState")
		operation = UNLOCK_PAN_STATE;
	else if (tempString == "panScreen")
	{
		operation = PAN_SCREEN;
		tempString = EventNode.getAttribute("pan");
		if (tempString == "direction")
		{
			pan = DIRECTION;
			tempString = EventNode.getAttribute("direction");
			if (tempString == "up")
				panDirection.direction = UP;
			else if (tempString == "down")
				panDirection.direction = DOWN;
			else if (tempString == "left")
				panDirection.direction = LEFT;
			else if (tempString == "right")
				panDirection.direction = RIGHT;
			panDirection.tiles = QString(EventNode.getAttribute("tiles")).toInt();
		}
		else if (tempString == "location")
		{
			pan = LOCATION;
			panLocation.x = QString(EventNode.getAttribute("x")).toInt();
			panLocation.y = QString(EventNode.getAttribute("y")).toInt();
			panLocation.coordinatesStoredInVariables = (QString(EventNode.getAttribute("coordinatesStoredInVariables")) == "true") ? true:false;
		}
		tempString = EventNode.getAttribute("transitionSpeed");
		if (tempString == "1/8 normal")
			transitionSpeed = SPEED_ONEEIGHTHNORMAL;
		else if (tempString == "1/4 normal")
			transitionSpeed = SPEED_ONEFOURTHNORMAL;
		else if (tempString == "1/2 normal")
			transitionSpeed = SPEED_ONEHALFNORMAL;
		else if (tempString == "normal")
			transitionSpeed = SPEED_NORMAL;
		else if (tempString == "2x normal")
			transitionSpeed = SPEED_TWICENORMAL;
		else if (tempString == "4x normal")
			transitionSpeed = SPEED_FOURTIMESNORMAL;
		haltOtherProcesses = (QString(EventNode.getAttribute("haltOtherProcesses")) == "true") ? true:false;
	}
	else if (tempString == "returnToOrigin")
	{
		tempString = EventNode.getAttribute("transitionSpeed");
		if (tempString == "1/8 normal")
			transitionSpeed = SPEED_ONEEIGHTHNORMAL;
		else if (tempString == "1/4 normal")
			transitionSpeed = SPEED_ONEFOURTHNORMAL;
		else if (tempString == "1/2 normal")
			transitionSpeed = SPEED_ONEHALFNORMAL;
		else if (tempString == "normal")
			transitionSpeed = SPEED_NORMAL;
		else if (tempString == "2x normal")
			transitionSpeed = SPEED_TWICENORMAL;
		else if (tempString == "4x normal")
			transitionSpeed = SPEED_FOURTIMESNORMAL;
		haltOtherProcesses = (QString(EventNode.getAttribute("haltOtherProcesses")) == "true") ? true:false;
	}
}

BaseEvent::PanScreen::PanScreen(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	operation = storageFile.getSignedInt();
	if (operation == PAN_SCREEN)
	{
		storageFile.putSignedInt(pan);
		if (pan == DIRECTION)
		{
			storageFile.putSignedInt(panDirection.direction);
			storageFile.putSignedInt(panDirection.tiles);
		}
		else if (pan == LOCATION)
		{
			storageFile.putSignedInt(panLocation.x);
			storageFile.putSignedInt(panLocation.y);
			storageFile.putBool(panLocation.coordinatesStoredInVariables);
		}
		storageFile.putSignedInt(transitionSpeed);
		storageFile.putBool(haltOtherProcesses);
	}
	else if (operation == RETURN_TO_ORIGIN)
	{
		storageFile.putSignedInt(transitionSpeed);
		storageFile.putBool(haltOtherProcesses);
	}
}

BaseEvent::PanScreen::PanScreen(PanScreen *other, BaseEvent *pBase) : Event(pBase)
{
	operation = other->operation;
	pan = other->pan;
	panDirection.direction = other->panDirection.direction;
	panDirection.tiles = other->panDirection.tiles;
	panLocation.x = other->panLocation.x;
	panLocation.y = other->panLocation.y;
	panLocation.coordinatesStoredInVariables = other->panLocation.coordinatesStoredInVariables;
	transitionSpeed = other->transitionSpeed;
	haltOtherProcesses = other->haltOtherProcesses;
}
//END BaseEvent::PanScreen Member Functions

//BEGIN BaseEvent::PlayBGM Member Functions
XMLNode BaseEvent::PlayBGM::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	EventNode.addAttribute("location", ProjectData::getRelativeResourcePath(bgm.location).toUtf8().data());
	EventNode.addAttribute("volume", QString::number(bgm.volume).toUtf8().data());
	EventNode.addAttribute("panning", QString::number(bgm.panning).toUtf8().data());
	EventNode.addAttribute("speed", QString::number(bgm.speed).toUtf8().data());
	EventNode.addAttribute("fadeInTime", QString::number(bgm.fadeInTime).toUtf8().data());
	EventNode.addAttribute("retainPitch", (bgm.retainPitch) ? "true":"false");
	return EventNode;
}

void BaseEvent::PlayBGM::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putString(bgm.location.toUtf8().data());
	storageFile.putFloat(bgm.volume);
	storageFile.putFloat(bgm.panning);
	storageFile.putFloat(bgm.speed);
	storageFile.putSignedInt(bgm.fadeInTime);
	storageFile.putBool(bgm.retainPitch);
}

QStringList BaseEvent::PlayBGM::getResources()
{
	return QStringList() << bgm.location;
}

QStringList BaseEvent::PlayBGM::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Play BGM: ";
	viewableText[0] += ProjectData::getRelativeResourcePath(bgm.location);
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::PlayBGM::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	MusicBrowser *musicBrowser = new MusicBrowser;
	bool configured = false;
	musicBrowser->setupBrowser();
	musicBrowser->setMusicInfo(bgm);
	if (musicBrowser->exec())
	{
		configured = true;
		musicBrowser->getMusicInfo(&bgm);
	}
	delete musicBrowser;
	return configured;
}
#endif

void BaseEvent::PlayBGM::getBGM(AudioManager::MusicInfo *other)
{
	other->location = "";
	other->location.append(bgm.location);
	other->volume = bgm.volume;
	other->panning = bgm.panning;
	other->speed = bgm.speed;
	other->fadeInTime = bgm.fadeInTime;
	other->retainPitch = bgm.retainPitch;
}

void BaseEvent::PlayBGM::setBGM(AudioManager::MusicInfo *other)
{
	bgm.location = "";
	bgm.location.append(other->location);
	bgm.volume = other->volume;
	bgm.panning = other->panning;
	bgm.speed = other->speed;
	bgm.fadeInTime = other->fadeInTime;
	bgm.retainPitch = other->retainPitch;
}

bool BaseEvent::PlayBGM::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	if (!XMLTools::attributeExists(EventNode, "location"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "volume"))
		return false;
	if (!XMLTools::attributeFloatRangeValid(EventNode, "volume", 0.0, 1.0))
		return false;
	if (!XMLTools::attributeExists(EventNode, "panning"))
		return false;
	if (!XMLTools::attributeFloatRangeValid(EventNode, "panning", -1.0, 1.0))
		return false;
	if (!XMLTools::attributeExists(EventNode, "speed"))
		return false;
	if (!XMLTools::attributeFloatRangeValid(EventNode, "speed", 0.5, 2.0))
		return false;
	if (!XMLTools::attributeExists(EventNode, "fadeInTime"))
		return false;
	if (!XMLTools::attributeIntRangeValid(EventNode, "fadeInTime", 0, 10))
		return false;
	if (!XMLTools::attributeExists(EventNode, "retainPitch"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "retainPitch", QStringList() << "true" << "false", 1))
		return false;
	return true;
}

BaseEvent::PlayBGM::PlayBGM(BaseEvent *pBase) : Event(pBase)
{
	bgm.location = "";
	bgm.volume = 1.0;
	bgm.panning = 0.0;
	bgm.speed = 1.0;
	bgm.fadeInTime = 0;
	bgm.retainPitch = false;
}

BaseEvent::PlayBGM::PlayBGM(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	bgm.location = ProjectData::getAbsoluteResourcePath(EventNode.getAttribute("location"));
	bgm.volume = QString(EventNode.getAttribute("volume")).toFloat();
	bgm.panning = QString(EventNode.getAttribute("panning")).toFloat();
	bgm.speed = QString(EventNode.getAttribute("speed")).toFloat();
	bgm.fadeInTime = QString(EventNode.getAttribute("fadeInTime")).toInt();
	bgm.retainPitch = (QString(EventNode.getAttribute("retainPitch")) == "true") ? true:false;
}

BaseEvent::PlayBGM::PlayBGM(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	char *temp_string = storageFile.getString();
	bgm.location = temp_string;
	delete[] temp_string;
	bgm.volume = storageFile.getFloat();
	bgm.panning = storageFile.getFloat();
	bgm.speed = storageFile.getFloat();
	bgm.fadeInTime = storageFile.getSignedInt();
	bgm.retainPitch = storageFile.getBool();
}

BaseEvent::PlayBGM::PlayBGM(PlayBGM *other, BaseEvent *pBase) : Event(pBase)
{
	bgm.location = "";
	bgm.location.append(other->bgm.location);
	bgm.volume = other->bgm.volume;
	bgm.panning = other->bgm.panning;
	bgm.speed = other->bgm.speed;
	bgm.fadeInTime = other->bgm.fadeInTime;
	bgm.retainPitch = other->bgm.retainPitch;
}
//END BaseEvent::PlayBGM Member Functions

//BEGIN BaseEvent::PlayMovie Member Functions
XMLNode BaseEvent::PlayMovie::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	EventNode.addAttribute("location", ProjectData::getRelativeResourcePath(movieLocation).toUtf8().data());
	EventNode.addAttribute("x", QString::number(x).toUtf8().data());
	EventNode.addAttribute("y", QString::number(y).toUtf8().data());
	EventNode.addAttribute("width", QString::number(width).toUtf8().data());
	EventNode.addAttribute("height", QString::number(height).toUtf8().data());
	EventNode.addAttribute("coordinatesStoredInVariables", (coordinatesStoredInVariables) ? "true":"false");
	return EventNode;
}

void BaseEvent::PlayMovie::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putString(movieLocation.toUtf8().data());
	storageFile.putSignedInt(x);
	storageFile.putSignedInt(y);
	storageFile.putSignedInt(width);
	storageFile.putSignedInt(height);
	storageFile.putBool(coordinatesStoredInVariables);
}

QStringList BaseEvent::PlayMovie::getResources()
{
	return QStringList() << movieLocation;
}

QStringList BaseEvent::PlayMovie::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Play Movie: ";
	viewableText[0] += ProjectData::getRelativeResourcePath(movieLocation);
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::PlayMovie::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	bool configured = false;
	QMessageBox::information(0, "Not Yet Implemented", "Movie's are not yet implemented");
	return configured;
}
#endif

bool BaseEvent::PlayMovie::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	if (!XMLTools::attributeExists(EventNode, "location"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "x"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "y"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "width"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "height"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "coordinatesStoredInVariables"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "coordinatesStoredInVariables", QStringList() << "true" << "false", 1))
		return false;
	return true;
}

BaseEvent::PlayMovie::PlayMovie(BaseEvent *pBase) : Event(pBase)
{
	movieLocation = "";
	x = 0;
	y = 0;
	width = 0;
	height = 0;
	coordinatesStoredInVariables = false;
}

BaseEvent::PlayMovie::PlayMovie(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	movieLocation = ProjectData::getAbsoluteResourcePath(EventNode.getAttribute("location"));
	x = QString(EventNode.getAttribute("x")).toInt();
	y = QString(EventNode.getAttribute("y")).toInt();
	width = QString(EventNode.getAttribute("width")).toInt();
	height = QString(EventNode.getAttribute("height")).toInt();
	coordinatesStoredInVariables = (QString(EventNode.getAttribute("coordinatesStoredInVariables")) == "true") ? true:false;
}

BaseEvent::PlayMovie::PlayMovie(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	char *temp_string = storageFile.getString();
	movieLocation = temp_string;
	delete[] temp_string;
	x = storageFile.getSignedInt();
	y = storageFile.getSignedInt();
	width = storageFile.getSignedInt();
	height = storageFile.getSignedInt();
	coordinatesStoredInVariables = storageFile.getBool();
}

BaseEvent::PlayMovie::PlayMovie(PlayMovie *other, BaseEvent *pBase) : Event(pBase)
{
	movieLocation = "";
	movieLocation.append(other->movieLocation);
	x = other->x;
	y = other->y;
	width = other->width;
	height = other->height;
	coordinatesStoredInVariables = other->coordinatesStoredInVariables;
}
//END BaseEvent::PlayMovie Member Functions

//BEGIN BaseEvent::PlaySoundEffect Member Functions
XMLNode BaseEvent::PlaySoundEffect::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	EventNode.addAttribute("location", ProjectData::getRelativeResourcePath(soundEffect.location).toUtf8().data());
	EventNode.addAttribute("volume", QString::number(soundEffect.volume).toUtf8().data());
	EventNode.addAttribute("panning", QString::number(soundEffect.panning).toUtf8().data());
	EventNode.addAttribute("speed", QString::number(soundEffect.speed).toUtf8().data());
	EventNode.addAttribute("numLoops", QString::number(soundEffect.numLoops).toUtf8().data());
	EventNode.addAttribute("retainPitch", (soundEffect.retainPitch) ? "true":"false");
	return EventNode;
}

void BaseEvent::PlaySoundEffect::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putString(soundEffect.location.toUtf8().data());
	storageFile.putFloat(soundEffect.volume);
	storageFile.putFloat(soundEffect.panning);
	storageFile.putFloat(soundEffect.speed);
	storageFile.putSignedInt(soundEffect.numLoops);
	storageFile.putBool(soundEffect.retainPitch);
}

QStringList BaseEvent::PlaySoundEffect::getResources()
{
	return QStringList() << soundEffect.location;
}

QStringList BaseEvent::PlaySoundEffect::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Play Sound Effect: ";
	viewableText[0] += ProjectData::getRelativeResourcePath(soundEffect.location);
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::PlaySoundEffect::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	SoundEffectBrowser *soundBrowser = new SoundEffectBrowser;
	bool configured = false;
	soundBrowser->setupBrowser();
	soundBrowser->setSoundEffectInfo(soundEffect);
	if (soundBrowser->exec())
	{
		configured = true;
		soundBrowser->getSoundEffectInfo(&soundEffect);
	}
	delete soundBrowser;
	return configured;
}
#endif

void BaseEvent::PlaySoundEffect::getSoundEffect(AudioManager::SoundEffectInfo *other)
{
	other->location = "";
	other->location.append(soundEffect.location);
	other->volume = soundEffect.volume;
	other->panning = soundEffect.panning;
	other->speed = soundEffect.speed;
	other->numLoops = soundEffect.numLoops;
	other->retainPitch = soundEffect.retainPitch;
}

void BaseEvent::PlaySoundEffect::setSoundEffect(AudioManager::SoundEffectInfo *other)
{
	soundEffect.location = "";
	soundEffect.location.append(other->location);
	soundEffect.volume = other->volume;
	soundEffect.panning = other->panning;
	soundEffect.speed = other->speed;
	soundEffect.numLoops = other->numLoops;
	soundEffect.retainPitch = other->retainPitch;
}

bool BaseEvent::PlaySoundEffect::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	if (!XMLTools::attributeExists(EventNode, "location"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "volume"))
		return false;
	if (!XMLTools::attributeFloatRangeValid(EventNode, "volume", 0.0, 1.0))
		return false;
	if (!XMLTools::attributeExists(EventNode, "panning"))
		return false;
	if (!XMLTools::attributeFloatRangeValid(EventNode, "panning", -1.0, 1.0))
		return false;
	if (!XMLTools::attributeExists(EventNode, "speed"))
		return false;
	if (!XMLTools::attributeFloatRangeValid(EventNode, "speed", 0.5, 2.0))
		return false;
	if (!XMLTools::attributeExists(EventNode, "numLoops"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "retainPitch"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "retainPitch", QStringList() << "true" << "false", 1))
		return false;
	return true;
}

BaseEvent::PlaySoundEffect::PlaySoundEffect(BaseEvent *pBase) : Event(pBase)
{
	soundEffect.location = "";
	soundEffect.volume = 1.0;
	soundEffect.panning = 0.0;
	soundEffect.speed = 1.0;
	soundEffect.numLoops = 0;
	soundEffect.sfxID = -1;
	soundEffect.retainPitch = false;
}

BaseEvent::PlaySoundEffect::PlaySoundEffect(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	soundEffect.location = ProjectData::getAbsoluteResourcePath(EventNode.getAttribute("location"));
	soundEffect.volume = QString(EventNode.getAttribute("volume")).toFloat();
	soundEffect.panning = QString(EventNode.getAttribute("panning")).toFloat();
	soundEffect.speed = QString(EventNode.getAttribute("speed")).toFloat();
	soundEffect.numLoops = QString(EventNode.getAttribute("numLoops")).toInt();
	soundEffect.sfxID = -1;
	soundEffect.retainPitch = (QString(EventNode.getAttribute("retainPitch")) == "true") ? true:false;
}

BaseEvent::PlaySoundEffect::PlaySoundEffect(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	char *temp_string = storageFile.getString();
	soundEffect.location = temp_string;
	delete[] temp_string;
	soundEffect.volume = storageFile.getFloat();
	soundEffect.panning = storageFile.getFloat();
	soundEffect.speed = storageFile.getFloat();
	soundEffect.numLoops = storageFile.getSignedInt();
	soundEffect.sfxID = -1;
	soundEffect.retainPitch = storageFile.getBool();
}

BaseEvent::PlaySoundEffect::PlaySoundEffect(PlaySoundEffect *other, BaseEvent *pBase) : Event(pBase)
{
	soundEffect.location = "";
	soundEffect.location.append(other->soundEffect.location);
	soundEffect.volume = other->soundEffect.volume;
	soundEffect.panning = other->soundEffect.panning;
	soundEffect.speed = other->soundEffect.speed;
	soundEffect.numLoops = other->soundEffect.numLoops;
	soundEffect.sfxID = -1;
	soundEffect.retainPitch = other->soundEffect.retainPitch;
}
//END BaseEvent::PlaySoundEffect Member Functions

//BEGIN BaseEvent::PlaySystemBGM Member Functions
XMLNode BaseEvent::PlaySystemBGM::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	if (bgm == ProjectData::BGM_TITLESCREEN)
		EventNode.addAttribute("effect", "Title Screen");
	else if (bgm == ProjectData::BGM_BATTLE)
		EventNode.addAttribute("effect", "Battle");
	else if (bgm == ProjectData::BGM_VICTORY)
		EventNode.addAttribute("effect", "Victory");
	else if (bgm == ProjectData::BGM_INN)
		EventNode.addAttribute("effect", "Inn");
	else if (bgm == ProjectData::BGM_GAMEOVER)
		EventNode.addAttribute("effect", "Game Over");
	return EventNode;
}

void BaseEvent::PlaySystemBGM::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(bgm);
}

QStringList BaseEvent::PlaySystemBGM::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	QString location = "";
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Play System BGM: ";
	if (bgm == ProjectData::BGM_TITLESCREEN)
		viewableText[0] += "Title Screen";
	else if (bgm == ProjectData::BGM_BATTLE)
		viewableText[0] += "Battle";
	else if (bgm == ProjectData::BGM_VICTORY)
		viewableText[0] += "Victory";
	else if (bgm == ProjectData::BGM_INN)
		viewableText[0] += "Inn";
	else if (bgm == ProjectData::BGM_GAMEOVER)
		viewableText[0] += "Game Over";
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::PlaySystemBGM::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	GetItemDialog2 *getBGM = new GetItemDialog2("System BGM", "BGM Type", QStringList() << "Title Screen" << "Battle" << "Victory" << "Inn" << "Game Over");
	bool configured = false;
	getBGM->setItemID(bgm);
	if (getBGM->exec())
	{
		configured = true;
		bgm = getBGM->getItemID();
	}
	delete getBGM;
	return configured;
}
#endif

bool BaseEvent::PlaySystemBGM::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	QStringList bgmType;
	bgmType << "Title Screen";
	bgmType << "Battle";
	bgmType << "Victory";
	bgmType << "Inn";
	bgmType << "Game Over";
	if (!XMLTools::attributeExists(EventNode, "bgm"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "bgm", bgmType, -1))
		return false;
	return true;
}

BaseEvent::PlaySystemBGM::PlaySystemBGM(BaseEvent *pBase) : Event(pBase)
{
	bgm = ProjectData::BGM_TITLESCREEN;
}

BaseEvent::PlaySystemBGM::PlaySystemBGM(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	QString type = EventNode.getAttribute("bgm");
	if (type == "Title Screen")
		bgm = ProjectData::BGM_TITLESCREEN;
	else if (type == "Battle")
		bgm = ProjectData::BGM_BATTLE;
	else if (type == "Victory")
		bgm = ProjectData::BGM_VICTORY;
	else if (type == "Inn")
		bgm = ProjectData::BGM_INN;
	else if (type == "Game Over")
		bgm = ProjectData::BGM_GAMEOVER;
}

BaseEvent::PlaySystemBGM::PlaySystemBGM(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	bgm = storageFile.getSignedInt();
}

BaseEvent::PlaySystemBGM::PlaySystemBGM(PlaySystemBGM *other, BaseEvent *pBase) : Event(pBase)
{
	bgm = other->bgm;
}
//END BaseEvent::PlaySystemBGM Member Functions

//BEGIN BaseEvent::PlaySystemSoundEffect Member Functions
XMLNode BaseEvent::PlaySystemSoundEffect::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	if (soundEffect == ProjectData::SFX_CURSOR)
		EventNode.addAttribute("effect", "Cursor");
	else if (soundEffect == ProjectData::SFX_SELECTCHOICE)
		EventNode.addAttribute("effect", "Select Choice");
	else if (soundEffect == ProjectData::SFX_BUZZER)
		EventNode.addAttribute("effect", "Buzzer");
	else if (soundEffect == ProjectData::SFX_CANCEL)
		EventNode.addAttribute("effect", "Cancel");
	else if (soundEffect == ProjectData::SFX_BATTLETRANSITION)
		EventNode.addAttribute("effect", "Battle Transition");
	else if (soundEffect == ProjectData::SFX_FLEEBATTLE)
		EventNode.addAttribute("effect", "Flee Battle");
	else if (soundEffect == ProjectData::SFX_CHARACTERLEVELUP)
		EventNode.addAttribute("effect", "Character Level Up");
	else if (soundEffect == ProjectData::SFX_EVADEATTACK)
		EventNode.addAttribute("effect", "Evade Attack");
	else if (soundEffect == ProjectData::SFX_ITEMUSED)
		EventNode.addAttribute("effect", "Item Used");
	return EventNode;
}

void BaseEvent::PlaySystemSoundEffect::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(soundEffect);
}

QStringList BaseEvent::PlaySystemSoundEffect::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	QString location = "";
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Play System Sound Effect: ";
	if (soundEffect == ProjectData::SFX_CURSOR)
		viewableText[0] += "Cursor";
	else if (soundEffect == ProjectData::SFX_SELECTCHOICE)
		viewableText[0] += "Select Choice";
	else if (soundEffect == ProjectData::SFX_BUZZER)
		viewableText[0] += "Buzzer";
	else if (soundEffect == ProjectData::SFX_CANCEL)
		viewableText[0] += "Cancel";
	else if (soundEffect == ProjectData::SFX_BATTLETRANSITION)
		viewableText[0] += "Battle Transition";
	else if (soundEffect == ProjectData::SFX_FLEEBATTLE)
		viewableText[0] += "Flee Battle";
	else if (soundEffect == ProjectData::SFX_CHARACTERLEVELUP)
		viewableText[0] += "Character Level Up";
	else if (soundEffect == ProjectData::SFX_EVADEATTACK)
		viewableText[0] += "Evade Attack";
	else if (soundEffect == ProjectData::SFX_ITEMUSED)
		viewableText[0] += "Item Used";
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::PlaySystemSoundEffect::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	GetItemDialog2 *getSoundEffect = new GetItemDialog2("System SFX", "SFX Type", QStringList() << "Cursor" << "Select Choice" << "Buzzer" << "Cancel" << "Battle Transition" << "Flee Battle" << "Character Level Up" << "Evade Attack" << "Item Used");
	bool configured = false;
	getSoundEffect->setItemID(soundEffect);
	if (getSoundEffect->exec())
	{
		configured = true;
		soundEffect = getSoundEffect->getItemID();
	}
	delete getSoundEffect;
	return configured;
}
#endif

bool BaseEvent::PlaySystemSoundEffect::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	QStringList effectType;
	effectType << "Cursor";
	effectType << "Select Choice";
	effectType << "Buzzer";
	effectType << "Cancel";
	effectType << "Battle Transition";
	effectType << "Flee Battle";
	effectType << "Character Level Up";
	effectType << "Evade Attack";
	effectType << "Item Used";
	if (!XMLTools::attributeExists(EventNode, "effect"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "effect", effectType, -1))
		return false;
	return true;
}

BaseEvent::PlaySystemSoundEffect::PlaySystemSoundEffect(BaseEvent *pBase) : Event(pBase)
{
	soundEffect = ProjectData::SFX_CURSOR;
}

BaseEvent::PlaySystemSoundEffect::PlaySystemSoundEffect(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	QString effect = EventNode.getAttribute("effect");
	if (effect == "Cursor")
		soundEffect = ProjectData::SFX_CURSOR;
	else if (effect == "Select Choice")
		soundEffect = ProjectData::SFX_SELECTCHOICE;
	else if (effect == "Buzzer")
		soundEffect = ProjectData::SFX_BUZZER;
	else if (effect == "Cancel")
		soundEffect = ProjectData::SFX_CANCEL;
	else if (effect == "Battle Transition")
		soundEffect = ProjectData::SFX_BATTLETRANSITION;
	else if (effect == "Flee Battle")
		soundEffect = ProjectData::SFX_FLEEBATTLE;
	else if (effect == "Character Level Up")
		soundEffect = ProjectData::SFX_CHARACTERLEVELUP;
	else if (effect == "Evade Attack")
		soundEffect = ProjectData::SFX_EVADEATTACK;
	else if (effect == "Item Used")
		soundEffect = ProjectData::SFX_ITEMUSED;
}

BaseEvent::PlaySystemSoundEffect::PlaySystemSoundEffect(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	soundEffect = storageFile.getSignedInt();
}

BaseEvent::PlaySystemSoundEffect::PlaySystemSoundEffect(PlaySystemSoundEffect *other, BaseEvent *pBase) : Event(pBase)
{
	soundEffect = other->soundEffect;
}
//END BaseEvent::PlaySystemSoundEffect Member Functions

//BEGIN BaseEvent::PreviousSelectedAreaBranch Member Functions
BaseEvent::PreviousSelectedAreaBranch::~PreviousSelectedAreaBranch()
{
	QStringList selectableAreaNames = selectableAreaEvents.keys();
	for (int i = 0; i < selectableAreaNames.size(); ++i)
	{
		QList<BaseEvent*> *pEventList = &(selectableAreaEvents[selectableAreaNames[i]]);
		for (int j = 0; j < pEventList->size(); ++j)
			delete (pEventList->takeFirst());
	}
	selectableAreaEvents.clear();
}

XMLNode BaseEvent::PreviousSelectedAreaBranch::getEventNode()
{
	XMLNode TempNode, EventNode = XMLNode::createXMLTopNode("event");
	QStringList selectableAreaNames = selectableAreaEvents.keys();
	for (int i = 0; i < selectableAreaNames.size(); ++i)
	{
		QList<BaseEvent*> *pEventList = &(selectableAreaEvents[selectableAreaNames[i]]);
		TempNode = EventNode.addChild("previousSelectedArea");
		TempNode.addAttribute("objectName", selectableAreaNames[i].toUtf8().data());
		for (int j = 0; j < pEventList->size(); ++j)
			TempNode.addChild(pEventList->at(j)->getEventNode());
	}
	return EventNode;
}

void BaseEvent::PreviousSelectedAreaBranch::saveToStorageFile(StorageFile &storageFile)
{
	QStringList selectableAreaNames = selectableAreaEvents.keys();
	storageFile.putSignedInt(selectableAreaNames.size());
	for (int i = 0; i < selectableAreaNames.size(); ++i)
	{
		QList<BaseEvent*> *pEventList = &(selectableAreaEvents[selectableAreaNames[i]]);
		storageFile.putString(selectableAreaNames[i].toUtf8().data());
		storageFile.putSignedInt(pEventList->size());
		for (int j = 0; j < pEventList->size(); ++j)
			pEventList->at(j)->saveToStorageFile(storageFile);
	}
}

QStringList BaseEvent::PreviousSelectedAreaBranch::getResources()
{
	QStringList resources;
	QStringList selectableAreaNames = selectableAreaEvents.keys();
	for (int i = 0; i < selectableAreaNames.size(); ++i)
	{
		QList<BaseEvent*> *pEventList = &(selectableAreaEvents[selectableAreaNames[i]]);
		for (int j = 0; j < pEventList->size(); ++j)
			resources += pEventList->at(j)->getResources();
	}
	return resources;
}

QStringList BaseEvent::PreviousSelectedAreaBranch::getViewableText(int indent)
{
	QStringList viewableText;
	QStringList selectableAreaNames = selectableAreaEvents.keys();
	QString text = "";
	for (int i = 0; i < indent; ++i)
		text += "  ";
	text += "<> ";
	text += "Previous Selected Area Branch";
	viewableText.append(text);
	for (int i = 0; i < selectableAreaNames.size(); ++i)
	{
		QList<BaseEvent*> *pEventList = &(selectableAreaEvents[selectableAreaNames[i]]);
		text = "";
		for (int j = 0; j < indent; ++j)
			text += "  ";
		text += "::";
		text += selectableAreaNames[i];
		viewableText.append(text);
		for (int j = 0; j < pEventList->size(); ++j)
			viewableText.append(pEventList->at(j)->getViewableText(indent + 1));
		text = "";
		for (int i = 0; i < indent + 1; ++i)
			text += "  ";
		text += "<>";
		viewableText.append(text);
	}
	text = "";
	for (int j = 0; j < indent; ++j)
		text += "  ";
	text += "::End";
	viewableText.append(text);
	return viewableText;
}

QList<BaseEvent*> BaseEvent::PreviousSelectedAreaBranch::getEventPointers()
{
	QStringList selectableAreaNames = selectableAreaEvents.keys();
	QList<BaseEvent*> eventPointers;
	for (int i = 0; i < selectableAreaNames.size(); ++i)
	{
		eventPointers.append(selectableAreaEvents[selectableAreaNames[i]]);
		eventPointers.append(NULL);
	}
	return eventPointers;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::PreviousSelectedAreaBranch::configureEvent(int callerID, int callerType)
{
	ConfigureEvent_PreviousSelectedAreaBranch *configureBranch = new ConfigureEvent_PreviousSelectedAreaBranch(callerID, callerType);
	bool configured = false;
	configureBranch->setSelectedAreas(selectableAreaEvents.keys());
	if (configureBranch->exec())
	{
		QStringList currentAreas = selectableAreaEvents.keys();
		QStringList newAreas = configureBranch->getSelectedAreas();
		for (int i = 0; i < currentAreas.size(); ++i)
		{
			if (!newAreas.contains(currentAreas[i]))
			{
				QList<BaseEvent*> *pEventList = &(selectableAreaEvents[currentAreas[i]]);
				for (int j = 0; j < pEventList->size(); ++j)
					delete (pEventList->takeFirst());
				selectableAreaEvents.remove(currentAreas[i]);
			}
		}
		for (int i = 0; i < newAreas.size(); ++i)
		{
			if (!currentAreas.contains(newAreas[i]))
				selectableAreaEvents.insert(newAreas[i], QList<BaseEvent*>());
		}
		configured = true;
	}
	delete configureBranch;
	return configured;
}
#endif

void BaseEvent::PreviousSelectedAreaBranch::addSelectableArea(QString selectableArea)
{
	QList<BaseEvent*> eventList;
	selectableAreaEvents.insert(selectableArea, eventList);
}

void BaseEvent::PreviousSelectedAreaBranch::deleteSelectableArea(QString selectableArea)
{
	QList<BaseEvent*> *pEventList = &(selectableAreaEvents[selectableArea]);
	for (int i = 0; i < pEventList->size(); ++i)
		delete (pEventList->takeFirst());
	pEventList->clear();
	selectableAreaEvents.remove(selectableArea);
}

void BaseEvent::PreviousSelectedAreaBranch::addChildEvent(BaseEvent *event, int location, int childTypeID)
{
	QStringList selectableAreaNames = selectableAreaEvents.keys();
	QList<BaseEvent*> *pEventList = &(selectableAreaEvents[selectableAreaNames[childTypeID]]);
	pEventList->insert(location, event);
}

void BaseEvent::PreviousSelectedAreaBranch::deleteChildEvent(int location, int childTypeID)
{
	QStringList selectableAreaNames = selectableAreaEvents.keys();
	QList<BaseEvent*> *pEventList = &(selectableAreaEvents[selectableAreaNames[childTypeID]]);
	delete (pEventList->takeAt(location));
}

void BaseEvent::PreviousSelectedAreaBranch::applySwappedTiles(QList<Tileset::SwapTileIDs> swappedTiles)
{
	QMap<QString, QList<BaseEvent*> >::iterator iter;
	for (iter = selectableAreaEvents.begin(); iter != selectableAreaEvents.end(); ++iter)
	{
		for (int i = 0; i < iter.value().size(); ++i)
			iter.value().value(i)->applySwappedTiles(swappedTiles);
	}
}

void BaseEvent::PreviousSelectedAreaBranch::applyChangeMenuObjectName(QString oldName, QString newName)
{
	QMap<QString, QList<BaseEvent*> >::iterator iter;
	bool foundOne = true;
	while (foundOne)
	{
		foundOne = false;
		for (iter = selectableAreaEvents.begin(); iter != selectableAreaEvents.end() && !foundOne; ++iter)
		{
			if (iter.key() == oldName)
			{
				selectableAreaEvents.insert(newName, selectableAreaEvents.take(oldName));
				foundOne = true;
			}
		}
	}
	for (iter = selectableAreaEvents.begin(); iter != selectableAreaEvents.end(); ++iter)
	{
		for (int i = 0; i < iter.value().size(); ++i)
			iter.value().value(i)->applyChangeMenuObjectName(oldName, newName);
	}
}

bool BaseEvent::PreviousSelectedAreaBranch::tryLoad(XMLNode EventNode, int baseType)
{
	XMLNode TempNode;
	for (int i = 0; i < EventNode.nChildNode("previousSelectedArea"); ++i)
	{
		TempNode = EventNode.getChildNode("previousSelectedArea", i);
		if (!XMLTools::attributeExists(TempNode, "objectName"))
			return false;
		for (int j = 0; j < TempNode.nChildNode("event"); ++j)
			BaseEvent::tryLoad(TempNode.getChildNode("event", j), baseType);
	}
	return true;
}

BaseEvent::PreviousSelectedAreaBranch::PreviousSelectedAreaBranch(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	XMLNode TempNode;
	for (int i = 0; i < EventNode.nChildNode("previousSelectedArea"); ++i)
	{
		QList<BaseEvent*> eventList;
		QString objectName;
		TempNode = EventNode.getChildNode("previousSelectedArea", i);
		objectName = TempNode.getAttribute("objectName");
		for (int j = 0; j < TempNode.nChildNode("event"); ++j)
			eventList.append(new BaseEvent(pBaseEvent->baseType, TempNode.getChildNode("event", j)));
		selectableAreaEvents.insert(objectName, eventList);
	}
}

BaseEvent::PreviousSelectedAreaBranch::PreviousSelectedAreaBranch(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	QString objectName;
	int numEvents, numAreas = storageFile.getSignedInt();
	char *temp_string;
	for (int i = 0; i < numAreas; ++i)
	{
		QList<BaseEvent*> eventList;
		temp_string = storageFile.getString();
		objectName = temp_string;
		delete[] temp_string;
		numEvents = storageFile.getSignedInt();
		for (int j = 0; j < numEvents; ++j)
			eventList.append(new BaseEvent(pBaseEvent->baseType, storageFile));
		selectableAreaEvents.insert(objectName, eventList);
	}
}

BaseEvent::PreviousSelectedAreaBranch::PreviousSelectedAreaBranch(PreviousSelectedAreaBranch *other, BaseEvent *pBase) : Event(pBase)
{
	QStringList selectableAreaNames = other->selectableAreaEvents.keys();
	for (int i = 0; i < selectableAreaNames.size(); ++i)
	{
		QList<BaseEvent*> *pEventList = &(other->selectableAreaEvents[selectableAreaNames[i]]);
		QList<BaseEvent*> eventList;
		for (int j = 0; j < pEventList->size(); ++j)
			eventList.append(new BaseEvent(pEventList->at(j)));
		selectableAreaEvents.insert(selectableAreaNames[i], eventList);
	}
}
//END BaseEvent::PreviousSelectedAreaBranch Member Functions

//BEGIN BaseEvent::SaveGame Member Functions
XMLNode BaseEvent::SaveGame::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	EventNode.addAttribute("slot", QString::number(slot).toUtf8().data());
	EventNode.addAttribute("slotReferencedInVariable", (slotReferencedInVariable) ? "true":"false");
	return EventNode;
}

void BaseEvent::SaveGame::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(slot);
	storageFile.putBool(slotReferencedInVariable);
}

QStringList BaseEvent::SaveGame::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Save Game: Slot=";
	if (slotReferencedInVariable)
		viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(slot));
	else
		viewableText[0] += QString::number(slot);
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::SaveGame::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	ConfigureEvent_SaveGame *configSaveGame = new ConfigureEvent_SaveGame;
	bool configured = false;
	configSaveGame->setIsReferencedSaveSlot(slotReferencedInVariable);
	configSaveGame->setSaveSlotID(slot);
	if (configSaveGame->exec())
	{
		configured = true;
		slot = configSaveGame->getSaveSlotID();
		slotReferencedInVariable = configSaveGame->isReferencedSaveSlot();
	}
	delete configSaveGame;
	return configured;
}
#endif

bool BaseEvent::SaveGame::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	if (!XMLTools::attributeExists(EventNode, "slot"))
		return false;
	if (!XMLTools::attributeIntRangeValid(EventNode, "slot", 1, 16))
		return false;
	if (!XMLTools::attributeExists(EventNode, "slotReferencedInVariable"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "slotReferencedInVariable", QStringList() << "true" << "false", 1))
		return false;
	return true;
}

BaseEvent::SaveGame::SaveGame(BaseEvent *pBase) : Event(pBase)
{
	slot = 1;
	slotReferencedInVariable = false;
}

BaseEvent::SaveGame::SaveGame(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	slot = QString(EventNode.getAttribute("slot")).toInt();
	slotReferencedInVariable = (QString(EventNode.getAttribute("slotReferencedInVariable")) == "true") ? true:false;
}

BaseEvent::SaveGame::SaveGame(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	slot = storageFile.getSignedInt();
	slotReferencedInVariable = storageFile.getBool();
}

BaseEvent::SaveGame::SaveGame(SaveGame *other, BaseEvent *pBase) : Event(pBase)
{
	slot = other->slot;
	slotReferencedInVariable = other->slotReferencedInVariable;
}
//END BaseEvent::SaveGame Member Functions

//BEGIN BaseEvent::ScrollArea Member Functions
XMLNode BaseEvent::ScrollArea::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	EventNode.addAttribute("objectName", scrollAreaObjectName.toUtf8().data());
	if (scrollTo == CHILD_OBJECT)
	{
		EventNode.addAttribute("scrollTo", "childObject");
		EventNode.addAttribute("childName", childObjectName.toUtf8().data());
	}
	else if (scrollTo == SPECIFIC_LOCATION)
	{
		EventNode.addAttribute("scrollTo", "specificLocation");
		EventNode.addAttribute("x", QString::number(x).toUtf8().data());
		EventNode.addAttribute("y", QString::number(y).toUtf8().data());
	}
	else if (scrollTo == STORED_LOCATION)
	{
		EventNode.addAttribute("scrollTo", "storedLocation");
		EventNode.addAttribute("xVariableID", QString::number(x).toUtf8().data());
		EventNode.addAttribute("yVariableID", QString::number(y).toUtf8().data());
	}
	return EventNode;
}

void BaseEvent::ScrollArea::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putString(scrollAreaObjectName.toUtf8().data());
	storageFile.putString(childObjectName.toUtf8().data());
	storageFile.putSignedInt(x);
	storageFile.putSignedInt(y);
	storageFile.putSignedInt(scrollTo);
}

QStringList BaseEvent::ScrollArea::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Scroll Area: ";
	viewableText[0] += scrollAreaObjectName;
	viewableText[0] += " scroll to ";
	if (scrollTo == CHILD_OBJECT)
		viewableText[0] += childObjectName;
	else if (scrollTo == SPECIFIC_LOCATION)
	{
		viewableText[0] += "(";
		viewableText[0] += QString::number(x);
		viewableText[0] += ", ";
		viewableText[0] += QString::number(y);
		viewableText[0] += ")";
	}
	else
	{
		viewableText[0] += "(";
		viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(x));
		viewableText[0] += ", ";
		viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(y));
		viewableText[0] += ")";
	}
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::ScrollArea::configureEvent(int callerID, int callerType)
{
	ConfigureEvent_ScrollArea *configScrollArea = new ConfigureEvent_ScrollArea(callerID, callerType);
	bool configured = false;
	configScrollArea->setScrollArea(scrollAreaObjectName);
	configScrollArea->setScrollTo(scrollTo);
	if (scrollTo == CHILD_OBJECT)
		configScrollArea->setChildObject(childObjectName);
	else
		configScrollArea->setLocation(x, y);
	if (configScrollArea->exec())
	{
		configured = true;
		scrollAreaObjectName.clear();
		scrollAreaObjectName.append(configScrollArea->getScrollArea());
		scrollTo = configScrollArea->getScrollTo();
		childObjectName.clear();
		if (scrollTo == CHILD_OBJECT)
		{
			childObjectName.append(configScrollArea->getChildObject());
			x = 0;
			y = 0;
		}
		else
		{
			x = configScrollArea->getXLocation();
			y = configScrollArea->getYLocation();
		}
	}
	delete configScrollArea;
	return configured;
}
#endif

void BaseEvent::ScrollArea::applyChangeMenuObjectName(QString oldName, QString newName)
{
	if (scrollAreaObjectName == oldName)
		scrollAreaObjectName = newName;
	if (childObjectName == oldName)
		childObjectName = newName;
}

bool BaseEvent::ScrollArea::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	QStringList tempStringList;
	QString tempString;
	if (!XMLTools::attributeExists(EventNode, "objectName"))
		return false;
	tempStringList.clear();
	tempStringList << "childObject" << "specificLocation" << "storedLocation";
	if (!XMLTools::attributeExists(EventNode, "scrollTo"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "scrollTo", tempStringList))
		return false;
	tempString = EventNode.getAttribute("scrollTo");
	if (tempString == "childObject")
	{
		if (!XMLTools::attributeExists(EventNode, "childName"))
			return false;
	}
	else if (tempString == "specificLocation")
	{
		if (!XMLTools::attributeExists(EventNode, "x"))
			return false;
		if (!XMLTools::attributeExists(EventNode, "y"))
			return false;
	}
	else if (tempString == "storedLocation")
	{
		if (!XMLTools::attributeExists(EventNode, "xVariableID"))
			return false;
		if (!XMLTools::attributeExists(EventNode, "yVariableID"))
			return false;
	}
	return true;
}

BaseEvent::ScrollArea::ScrollArea(BaseEvent *pBase) : Event(pBase)
{
	scrollAreaObjectName = "";
	childObjectName = "";
	x = 0;
	y = 0;
	scrollTo = SPECIFIC_LOCATION;
}

BaseEvent::ScrollArea::ScrollArea(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	QString tempString;
	scrollAreaObjectName = EventNode.getAttribute("objectName");
	tempString = EventNode.getAttribute("scrollTo");
	if (tempString == "childObject")
	{
		scrollTo = CHILD_OBJECT;
		childObjectName = EventNode.getAttribute("childName");
	}
	else if (tempString == "specificLocation")
	{
		scrollTo = SPECIFIC_LOCATION;
		x = QString(EventNode.getAttribute("x")).toInt();
		y = QString(EventNode.getAttribute("y")).toInt();
	}
	else if (tempString == "storedLocation")
	{
		scrollTo = STORED_LOCATION;
		x = QString(EventNode.getAttribute("xVariableID")).toInt();
		y = QString(EventNode.getAttribute("yVariableID")).toInt();
	}
}

BaseEvent::ScrollArea::ScrollArea(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	char *temp_string;
	temp_string = storageFile.getString();
	scrollAreaObjectName = temp_string;
	delete[] temp_string;
	temp_string = storageFile.getString();
	childObjectName = temp_string;
	delete[] temp_string;
	x = storageFile.getSignedInt();
	y = storageFile.getSignedInt();
	scrollTo = storageFile.getSignedInt();
}

BaseEvent::ScrollArea::ScrollArea(ScrollArea *other, BaseEvent *pBase) : Event(pBase)
{
	scrollAreaObjectName = other->scrollAreaObjectName;
	childObjectName = other->childObjectName;
	x = other->x;
	y = other->y;
	scrollTo = other->scrollTo;
}
//END BaseEvent::ScrollArea Member Functions

//BEGIN BaseEvent::SetScreenTransition Member Functions
XMLNode BaseEvent::SetScreenTransition::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	if (transitionType == TELEPORT_ERASE)
		EventNode.addAttribute("transitionType", "teleportErase");
	else if (transitionType == TELEPORT_SHOW)
		EventNode.addAttribute("transitionType", "teleportShow");
	else if (transitionType == MENU_ERASE)
		EventNode.addAttribute("transitionType", "menuErase");
	else if (transitionType == MENU_SHOW)
		EventNode.addAttribute("transitionType", "menuShow");
	else if (transitionType == BATTLE_ENTRY_ERASE)
		EventNode.addAttribute("transitionType", "battleEntryErase");
	else if (transitionType == BATTLE_ENTRY_SHOW)
		EventNode.addAttribute("transitionType", "battleEntryShow");
	else if (transitionType == BATTLE_EXIT_ERASE)
		EventNode.addAttribute("transitionType", "battleExitErase");
	else if (transitionType == BATTLE_EXIT_SHOW)
		EventNode.addAttribute("transitionType", "battleExitShow");
	if (transitionType == TELEPORT_ERASE || transitionType == MENU_ERASE || transitionType == BATTLE_ENTRY_ERASE || transitionType == BATTLE_EXIT_ERASE)
	{
		if (transition == ProjectData::ERASE_FADEOUT)
			EventNode.addAttribute("transition", "fadeOut");
		else if (transition == ProjectData::ERASE_REMOVEBLOCKS)
			EventNode.addAttribute("transition", "removeBlocks");
		else if (transition == ProjectData::ERASE_WIPEDOWNWARD)
			EventNode.addAttribute("transition", "wipeDownward");
		else if (transition == ProjectData::ERASE_WIPEUPWARD)
			EventNode.addAttribute("transition", "wipeUpward");
		else if (transition == ProjectData::ERASE_VENETIANBLINDS)
			EventNode.addAttribute("transition", "venetianBlinds");
		else if (transition == ProjectData::ERASE_VERTICALBLINDS)
			EventNode.addAttribute("transition", "verticalBlinds");
		else if (transition == ProjectData::ERASE_HORIZONTALBLINDS)
			EventNode.addAttribute("transition", "horizontalBlinds");
		else if (transition == ProjectData::ERASE_RECEDINGSQUARE)
			EventNode.addAttribute("transition", "recedingSquare");
		else if (transition == ProjectData::ERASE_EXPANDINGSQUARE)
			EventNode.addAttribute("transition", "expandingSquare");
		else if (transition == ProjectData::ERASE_SCREENMOVESUP)
			EventNode.addAttribute("transition", "screenMovesUp");
		else if (transition == ProjectData::ERASE_SCREENMOVESDOWN)
			EventNode.addAttribute("transition", "screenMovesDown");
		else if (transition == ProjectData::ERASE_SCREENMOVESLEFT)
			EventNode.addAttribute("transition", "screenMovesLeft");
		else if (transition == ProjectData::ERASE_SCREENMOVESRIGHT)
			EventNode.addAttribute("transition", "screenMovesRight");
		else if (transition == ProjectData::ERASE_VERTICALDIVISION)
			EventNode.addAttribute("transition", "verticalDivision");
		else if (transition == ProjectData::ERASE_HORIZONTALDIVISION)
			EventNode.addAttribute("transition", "horizontalDivision");
		else if (transition == ProjectData::ERASE_QUADRASECTION)
			EventNode.addAttribute("transition", "quadraSection");
		else if (transition == ProjectData::ERASE_ZOOMIN)
			EventNode.addAttribute("transition", "zoomIn");
		else if (transition == ProjectData::ERASE_MOSAIC)
			EventNode.addAttribute("transition", "mosaic");
		else if (transition == ProjectData::ERASE_WAVERSCREEN)
			EventNode.addAttribute("transition", "waverScreen");
		else if (transition == ProjectData::ERASE_INSTANTANEOUS)
			EventNode.addAttribute("transition", "instantaneous");
		else if (transition == ProjectData::ERASE_NOTRANSITION)
			EventNode.addAttribute("transition", "noTransition");
	}
	else
	{
		if (transition == ProjectData::SHOW_FADEIN)
			EventNode.addAttribute("transition", "fadeIn");
		else if (transition == ProjectData::SHOW_RECONSTITUTEBLOCKS)
			EventNode.addAttribute("transition", "reconstituteBlocks");
		else if (transition == ProjectData::SHOW_UNWIPEDOWNWARD)
			EventNode.addAttribute("transition", "unwipeDownward");
		else if (transition == ProjectData::SHOW_UNWIPEUPWARD)
			EventNode.addAttribute("transition", "unwipeUpward");
		else if (transition == ProjectData::SHOW_VENETIANBLINDS)
			EventNode.addAttribute("transition", "venetianBlinds");
		else if (transition == ProjectData::SHOW_VERTICALBLINDS)
			EventNode.addAttribute("transition", "verticalBlinds");
		else if (transition == ProjectData::SHOW_HORIZONTALBLINDS)
			EventNode.addAttribute("transition", "horizontalBlinds");
		else if (transition == ProjectData::SHOW_RECEDINGSQUARE)
			EventNode.addAttribute("transition", "recedingSquare");
		else if (transition == ProjectData::SHOW_EXPANDINGSQUARE)
			EventNode.addAttribute("transition", "expandingSquare");
		else if (transition == ProjectData::SHOW_SCREENMOVESDOWN)
			EventNode.addAttribute("transition", "screenMovesDown");
		else if (transition == ProjectData::SHOW_SCREENMOVESUP)
			EventNode.addAttribute("transition", "screenMovesUp");
		else if (transition == ProjectData::SHOW_SCREENMOVESRIGHT)
			EventNode.addAttribute("transition", "screenMovesRight");
		else if (transition == ProjectData::SHOW_SCREENMOVESLEFT)
			EventNode.addAttribute("transition", "screenMovesLeft");
		else if (transition == ProjectData::SHOW_VERTICALUNIFY)
			EventNode.addAttribute("transition", "verticalUnify");
		else if (transition == ProjectData::SHOW_HORIZONTALUNIFY)
			EventNode.addAttribute("transition", "horizontalUnify");
		else if (transition == ProjectData::SHOW_UNIFYQUADRANTS)
			EventNode.addAttribute("transition", "unifyQuadrants");
		else if (transition == ProjectData::SHOW_ZOOMOUT)
			EventNode.addAttribute("transition", "zoomOut");
		else if (transition == ProjectData::SHOW_MOSAIC)
			EventNode.addAttribute("transition", "mosaic");
		else if (transition == ProjectData::SHOW_WAVERSCREEN)
			EventNode.addAttribute("transition", "waverScreen");
		else if (transition == ProjectData::SHOW_INSTANTANEOUS)
			EventNode.addAttribute("transition", "instantaneous");
	}
	return EventNode;
}

void BaseEvent::SetScreenTransition::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(transitionType);
	storageFile.putSignedInt(transition);
}

QStringList BaseEvent::SetScreenTransition::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Set Screen Transition: ";
	if (transitionType == TELEPORT_ERASE)
		viewableText[0] += "Teleport Erase, ";
	else if (transitionType == TELEPORT_SHOW)
		viewableText[0] += "Teleport Show, ";
	else if (transitionType == MENU_ERASE)
		viewableText[0] += "Menu Erase, ";
	else if (transitionType == MENU_SHOW)
		viewableText[0] += "Menu Show, ";
	else if (transitionType == BATTLE_ENTRY_ERASE)
		viewableText[0] += "Battle Entry Erase, ";
	else if (transitionType == BATTLE_ENTRY_SHOW)
		viewableText[0] += "Battle Entry Show, ";
	else if (transitionType == BATTLE_EXIT_ERASE)
		viewableText[0] += "Battle Exit Erase, ";
	else if (transitionType == BATTLE_EXIT_SHOW)
		viewableText[0] += "Battle Exit Show, ";
	if (transitionType == TELEPORT_ERASE || transitionType == MENU_ERASE || transitionType == BATTLE_ENTRY_ERASE || transitionType == BATTLE_EXIT_ERASE)
	{
		if (transition == ProjectData::ERASE_FADEOUT)
			viewableText[0] += "Fade Out";
		else if (transition == ProjectData::ERASE_REMOVEBLOCKS)
			viewableText[0] += "Remove Blocks";
		else if (transition == ProjectData::ERASE_WIPEDOWNWARD)
			viewableText[0] += "Wipe Downward";
		else if (transition == ProjectData::ERASE_WIPEUPWARD)
			viewableText[0] += "Wipe Upward";
		else if (transition == ProjectData::ERASE_VENETIANBLINDS)
			viewableText[0] += "Venetian Blinds";
		else if (transition == ProjectData::ERASE_VERTICALBLINDS)
			viewableText[0] += "Vertical Blinds";
		else if (transition == ProjectData::ERASE_HORIZONTALBLINDS)
			viewableText[0] += "Horizontal Blinds";
		else if (transition == ProjectData::ERASE_RECEDINGSQUARE)
			viewableText[0] += "Receding Square";
		else if (transition == ProjectData::ERASE_EXPANDINGSQUARE)
			viewableText[0] += "Expanding Square";
		else if (transition == ProjectData::ERASE_SCREENMOVESUP)
			viewableText[0] += "Screen Moves Up";
		else if (transition == ProjectData::ERASE_SCREENMOVESDOWN)
			viewableText[0] += "Screen Moves Down";
		else if (transition == ProjectData::ERASE_SCREENMOVESLEFT)
			viewableText[0] += "Screen Moves Left";
		else if (transition == ProjectData::ERASE_SCREENMOVESRIGHT)
			viewableText[0] += "Screen Moves Right";
		else if (transition == ProjectData::ERASE_VERTICALDIVISION)
			viewableText[0] += "Vertical Division";
		else if (transition == ProjectData::ERASE_HORIZONTALDIVISION)
			viewableText[0] += "Horizontal Division";
		else if (transition == ProjectData::ERASE_QUADRASECTION)
			viewableText[0] += "Quadrasection";
		else if (transition == ProjectData::ERASE_ZOOMIN)
			viewableText[0] += "Zoom In";
		else if (transition == ProjectData::ERASE_MOSAIC)
			viewableText[0] += "Mosaic";
		else if (transition == ProjectData::ERASE_WAVERSCREEN)
			viewableText[0] += "Waver Screen";
		else if (transition == ProjectData::ERASE_INSTANTANEOUS)
			viewableText[0] += "Instant";
		else if (transition == ProjectData::ERASE_NOTRANSITION)
			viewableText[0] += "No Transition";
	}
	else
	{
		if (transition == ProjectData::SHOW_FADEIN)
			viewableText[0] += "Fade In";
		else if (transition == ProjectData::SHOW_RECONSTITUTEBLOCKS)
			viewableText[0] += "Reconstitute Blocks";
		else if (transition == ProjectData::SHOW_UNWIPEDOWNWARD)
			viewableText[0] += "Unwipe Downward";
		else if (transition == ProjectData::SHOW_UNWIPEUPWARD)
			viewableText[0] += "Unwipe Upward";
		else if (transition == ProjectData::SHOW_VENETIANBLINDS)
			viewableText[0] += "Venetian Blinds";
		else if (transition == ProjectData::SHOW_VERTICALBLINDS)
			viewableText[0] += "Vertical Blinds";
		else if (transition == ProjectData::SHOW_HORIZONTALBLINDS)
			viewableText[0] += "Horizontal Blinds";
		else if (transition == ProjectData::SHOW_RECEDINGSQUARE)
			viewableText[0] += "Receding Square";
		else if (transition == ProjectData::SHOW_EXPANDINGSQUARE)
			viewableText[0] += "Expanding Square";
		else if (transition == ProjectData::SHOW_SCREENMOVESDOWN)
			viewableText[0] += "Screen Moves Down";
		else if (transition == ProjectData::SHOW_SCREENMOVESUP)
			viewableText[0] += "Screen Moves Up";
		else if (transition == ProjectData::SHOW_SCREENMOVESRIGHT)
			viewableText[0] += "Screen Moves Right";
		else if (transition == ProjectData::SHOW_SCREENMOVESLEFT)
			viewableText[0] += "Screen Moves Left";
		else if (transition == ProjectData::SHOW_VERTICALUNIFY)
			viewableText[0] += "Vertical Unify";
		else if (transition == ProjectData::SHOW_HORIZONTALUNIFY)
			viewableText[0] += "Horizontal Unify";
		else if (transition == ProjectData::SHOW_UNIFYQUADRANTS)
			viewableText[0] += "Unify Quadrants";
		else if (transition == ProjectData::SHOW_ZOOMOUT)
			viewableText[0] += "Zoom Out";
		else if (transition == ProjectData::SHOW_MOSAIC)
			viewableText[0] += "Mosaic";
		else if (transition == ProjectData::SHOW_WAVERSCREEN)
			viewableText[0] += "Waver Screen";
		else if (transition == ProjectData::SHOW_INSTANTANEOUS)
			viewableText[0] += "Instant";
	}
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::SetScreenTransition::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	ConfigureEvent_SetScreenTransition *configTransition = new ConfigureEvent_SetScreenTransition;
	bool configured = false;
	configTransition->setScreenTransition(transitionType);
	configTransition->setTransition(transition);
	if (configTransition->exec())
	{
		configured = true;
		transitionType = configTransition->getScreenTransition();
		transition = configTransition->getTransition();
	}
	delete configTransition;
	return configured;
}
#endif

bool BaseEvent::SetScreenTransition::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	QStringList tempStringList;
	tempStringList.clear();
	tempStringList << "teleportErase" << "teleportShow" << "menuErase" << "menuShow" << "battleEntryErase" << "battleEntryShow" << "battleExitErase" << "battleExitShow";
	if (!XMLTools::attributeExists(EventNode, "transitionType"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "transitionType", tempStringList))
		return false;
	tempStringList.clear();
	if (QString(EventNode.getAttribute("transitionType")).endsWith("Erase"))
		tempStringList << "fadeOut" << "removeBlocks" << "wipeDownward" << "wipeUpward" << "venetianBlinds" << "verticalBlinds" << "horizontalBlinds" << "recedingSquare" << "expandingSquare" << "screenMovesUp" << "screenMovesDown" << "screenMovesLeft" << "screenMovesRight" << "verticalDivision" << "horizontalDivision" << "quadraSection" << "zoomIn" << "mosaic" << "waverScreen" << "instantaneous" << "noTransition";
	else
		tempStringList << "fadeIn" << "reconstituteBlocks" << "unwipeDownward" << "unwipeUpward" << "venetianBlinds" << "verticalBlinds" << "horizontalBlinds" << "recedingSquare" << "expandingSquare" << "screenMovesDown" << "screenMovesUp" << "screenMovesRight" << "screenMovesLeft" << "verticalUnify" << "horizontalUnify" << "unifyQuadrants" << "zoomOut" << "mosaic" << "waverScreen" << "instantaneous";
	if (!XMLTools::attributeExists(EventNode, "transition"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "transition", tempStringList))
		return false;
	return true;
}

BaseEvent::SetScreenTransition::SetScreenTransition(BaseEvent *pBase) : Event(pBase)
{
	transitionType = TELEPORT_ERASE;
	transition = ProjectData::ERASE_FADEOUT;
}

BaseEvent::SetScreenTransition::SetScreenTransition(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	QStringList tempStringList;
	tempStringList.clear();
	tempStringList << "teleportErase" << "teleportShow" << "menuErase" << "menuShow" << "battleEntryErase" << "battleEntryShow" << "battleExitErase" << "battleExitShow";
	transitionType = tempStringList.indexOf(EventNode.getAttribute("transitionType"));
	tempStringList.clear();
	if (transitionType == TELEPORT_ERASE || transitionType == MENU_ERASE || transitionType == BATTLE_ENTRY_ERASE || transitionType == BATTLE_EXIT_ERASE)
		tempStringList << "fadeOut" << "removeBlocks" << "wipeDownward" << "wipeUpward" << "venetianBlinds" << "verticalBlinds" << "horizontalBlinds" << "recedingSquare" << "expandingSquare" << "screenMovesUp" << "screenMovesDown" << "screenMovesLeft" << "screenMovesRight" << "verticalDivision" << "horizontalDivision" << "quadraSection" << "zoomIn" << "mosaic" << "waverScreen" << "instantaneous" << "noTransition";
	else
		tempStringList << "fadeIn" << "reconstituteBlocks" << "unwipeDownward" << "unwipeUpward" << "venetianBlinds" << "verticalBlinds" << "horizontalBlinds" << "recedingSquare" << "expandingSquare" << "screenMovesDown" << "screenMovesUp" << "screenMovesRight" << "screenMovesLeft" << "verticalUnify" << "horizontalUnify" << "unifyQuadrants" << "zoomOut" << "mosaic" << "waverScreen" << "instantaneous";
	transition = tempStringList.indexOf(EventNode.getAttribute("transition"));
}

BaseEvent::SetScreenTransition::SetScreenTransition(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	transitionType = storageFile.getSignedInt();
	transition = storageFile.getSignedInt();
}

BaseEvent::SetScreenTransition::SetScreenTransition(SetScreenTransition *other, BaseEvent *pBase) : Event(pBase)
{
	transitionType = other->transitionType;
	transition = other->transition;
}
//END BaseEvent::SetScreenTransition Member Functions

//BEGIN BaseEvent::ShakeScreen Member Functions
XMLNode BaseEvent::ShakeScreen::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	if (action == SHAKE_ONCE)
	{
		EventNode.addAttribute("action", "shakeOnce");
		EventNode.addAttribute("time", QString::number(time).toUtf8().data());
		EventNode.addAttribute("timeStoredInVariable", (timeStoredInVariable) ? "true":"false");
		EventNode.addAttribute("strength", QString::number(strength).toUtf8().data());
		EventNode.addAttribute("speed", QString::number(speed).toUtf8().data());
		EventNode.addAttribute("haltOtherProcesses", (haltOtherProcesses) ? "true":"false");
	}
	else if (action == BEGIN_SHAKING)
	{
		EventNode.addAttribute("action", "beginShaking");
		EventNode.addAttribute("strength", QString::number(strength).toUtf8().data());
		EventNode.addAttribute("speed", QString::number(speed).toUtf8().data());
	}
	else if (action == STOP_SHAKING)
		EventNode.addAttribute("action", "stopShaking");
	return EventNode;
}

void BaseEvent::ShakeScreen::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(action);
	storageFile.putSignedInt(time);
	storageFile.putSignedInt(strength);
	storageFile.putSignedInt(speed);
	storageFile.putBool(timeStoredInVariable);
	storageFile.putBool(haltOtherProcesses);
}

QStringList BaseEvent::ShakeScreen::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Shake Screen: ";
	if (action == SHAKE_ONCE)
	{
		viewableText[0] += "Once For ";
		if (timeStoredInVariable)
			viewableText[0] += QString("variable[%1] deciseconds ").arg(ProjectData::numberVariableIdToString(time));
		else
			viewableText[0] += QString("%1 deciseconds ").arg(time);
		viewableText[0] += QString("strength=%1 speed=%2").arg(strength).arg(speed);
		if (haltOtherProcesses)
			viewableText[0] += " (Halt Other Processes During Transition)";
	}
	else if (action == BEGIN_SHAKING)
	{
		viewableText[0] += "Begin Shaking ";
		viewableText[0] += QString("strength=%1 speed=%2").arg(strength).arg(speed);
	}
	else if (action == STOP_SHAKING)
		viewableText[0] += "Stop Shaking";
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::ShakeScreen::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	ConfigureEvent_ShakeScreen *configEvent = new ConfigureEvent_ShakeScreen;
	bool configured = false;
	configEvent->setAction(action);
	configEvent->setTimeStoredInVariable(timeStoredInVariable);
	configEvent->setTime(time);
	configEvent->setStrength(strength);
	configEvent->setSpeed(speed);
	configEvent->setHaltOtherProcesses(haltOtherProcesses);
	if (configEvent->exec())
	{
		configured = true;
		action = configEvent->getAction();
		time = configEvent->getTime();
		strength = configEvent->getStrength();
		speed = configEvent->getSpeed();
		timeStoredInVariable = configEvent->isTimeStoredInVariable();
		haltOtherProcesses = configEvent->getHaltOtherProcesses();
	}
	delete configEvent;
	return configured;
}
#endif

bool BaseEvent::ShakeScreen::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	QStringList tempStringList;
	QString tempString;
	tempStringList.clear();
	tempStringList << "shakeOnce" << "beginShaking" << "stopShaking";
	if (!XMLTools::attributeExists(EventNode ,"action"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "action", tempStringList))
		return false;
	tempString = EventNode.getAttribute("action");
	if (tempString == "shakeOnce")
	{
		if (!XMLTools::attributeExists(EventNode, "time"))
			return false;
		if (!XMLTools::attributeExists(EventNode, "timeStoredInVariable"))
			return false;
		if (!XMLTools::attributeStringValid(EventNode, "timeStoredInVariable", QStringList() << "true" << "false", 1))
			return false;
		if (!XMLTools::attributeExists(EventNode, "strength"))
			return false;
		if (!XMLTools::attributeIntRangeValid(EventNode, "strength", 1, 16))
			return false;
		if (!XMLTools::attributeExists(EventNode, "speed"))
			return false;
		if (!XMLTools::attributeIntRangeValid(EventNode, "speed", 1, 16))
			return false;
		if (!XMLTools::attributeExists(EventNode, "haltOtherProcesses"))
			return false;
		if (!XMLTools::attributeStringValid(EventNode, "haltOtherProcesses", QStringList() << "true" << "false", 1))
			return false;
	}
	else if (tempString == "beginShaking")
	{
		if (!XMLTools::attributeExists(EventNode, "strength"))
			return false;
		if (!XMLTools::attributeIntRangeValid(EventNode, "strength", 1, 16))
			return false;
		if (!XMLTools::attributeExists(EventNode, "speed"))
			return false;
		if (!XMLTools::attributeIntRangeValid(EventNode, "speed", 1, 16))
			return false;
	}
	return true;
}

BaseEvent::ShakeScreen::ShakeScreen(BaseEvent *pBase) : Event(pBase)
{
	action = SHAKE_ONCE;
	time = 2;
	strength = 8;
	speed = 8;
	timeStoredInVariable = false;
	haltOtherProcesses = false;
}

BaseEvent::ShakeScreen::ShakeScreen(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	QString tempString = EventNode.getAttribute("action");
	time = 2;
	strength = 8;
	speed = 8;
	timeStoredInVariable = false;
	haltOtherProcesses = false;
	if (tempString == "shakeOnce")
	{
		action = SHAKE_ONCE;
		time = QString(EventNode.getAttribute("time")).toInt();
		timeStoredInVariable = (QString(EventNode.getAttribute("timeStoredInVariable")) == "true") ? true:false;
		strength = QString(EventNode.getAttribute("strength")).toInt();
		speed = QString(EventNode.getAttribute("speed")).toInt();
		haltOtherProcesses = (QString(EventNode.getAttribute("haltOtherProcesses")) == "true") ? true:false;
	}
	else if (tempString == "beginShaking")
	{
		action = BEGIN_SHAKING;
		strength = QString(EventNode.getAttribute("strength")).toInt();
		speed = QString(EventNode.getAttribute("speed")).toInt();
	}
	else if (tempString == "stopShaking")
		action = STOP_SHAKING;
}

BaseEvent::ShakeScreen::ShakeScreen(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	action = storageFile.getSignedInt();
	time = storageFile.getSignedInt();
	strength = storageFile.getSignedInt();
	speed = storageFile.getSignedInt();
	timeStoredInVariable = storageFile.getBool();
	haltOtherProcesses = storageFile.getBool();
}

BaseEvent::ShakeScreen::ShakeScreen(ShakeScreen *other, BaseEvent *pBase) : Event(pBase)
{
	action = other->action;
	time = other->time;
	strength = other->strength;
	speed = other->speed;
	timeStoredInVariable = other->timeStoredInVariable;
	haltOtherProcesses = other->haltOtherProcesses;
}
//END BaseEvent::ShakeScreen Member Functions

//BEGIN BaseEvent::ShowBattleAnimation_Map Member Functions
XMLNode BaseEvent::ShowBattleAnimation_Map::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	EventNode.addAttribute("animationID", QString::number(animationID).toUtf8().data());
	if (targetType == HERO)
		EventNode.addAttribute("targetType", "hero");
	else if (targetType == THIS_EVENT)
		EventNode.addAttribute("targetType", "thisEvent");
	else if (targetType == VEHICLE)
	{
		EventNode.addAttribute("targetType", "vehicle");
		EventNode.addAttribute("vehicleID", QString::number(targetID).toUtf8().data());
	}
	else if (targetType == EVENT)
	{
		EventNode.addAttribute("targetType", "event");
		EventNode.addAttribute("eventID", QString::number(targetID).toUtf8().data());
	}
	EventNode.addAttribute("haltOtherProcesses", (haltOtherProcesses) ? "true":"false");
	return EventNode;
}

void BaseEvent::ShowBattleAnimation_Map::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(animationID);
	storageFile.putSignedInt(targetType);
	storageFile.putSignedInt(targetID);
	storageFile.putBool(haltOtherProcesses);
}

QStringList BaseEvent::ShowBattleAnimation_Map::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Show Battle Animation: ";
	viewableText[0] += ProjectData::battleAnimationNames[animationID];
	if (targetType == HERO)
		viewableText[0] += ", Hero";
	else if (targetType == THIS_EVENT)
		viewableText[0] += ", This Event";
	else if (targetType == VEHICLE)
		viewableText[0] += QString(", %1").arg(ProjectData::vehicleNames[targetID]);
	else if (targetType == EVENT)
		viewableText[0] += QString(", Event%1").arg(targetID, 3, 10, QChar('0'));
	if (haltOtherProcesses)
		viewableText[0] += " (Halt Other Processes)";
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::ShowBattleAnimation_Map::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerType);
	ConfigureEvent_ShowBattleAnimation_Map *configEvent = new ConfigureEvent_ShowBattleAnimation_Map(callerID);
	bool configured = false;
	configEvent->setBattleAnimationID(animationID);
	configEvent->setTargetType(targetType);
	configEvent->setTargetID(targetID);
	configEvent->setHaltOtherProcesses(haltOtherProcesses);
	if (configEvent->exec())
	{
		configured = true;
		animationID = configEvent->getBattleAnimationID();
		targetType = configEvent->getTargetType();
		targetID = configEvent->getTargetID();
		haltOtherProcesses = configEvent->getHaltOtherProcesses();
	}
	delete configEvent;
	return configured;
}
#endif

bool BaseEvent::ShowBattleAnimation_Map::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	QString tempString;
	if (!XMLTools::attributeExists(EventNode, "animationID"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "targetType"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "targetType", QStringList() << "hero" << "thisEvent" << "vehicle" << "event"))
		return false;
	tempString = EventNode.getAttribute("targetType");
	if (tempString == "vehicle")
	{
		if (!XMLTools::attributeExists(EventNode, "vehicleID"))
			return false;
	}
	else if (tempString == "event")
	{
		if (!XMLTools::attributeExists(EventNode, "eventID"))
			return false;
	}
	if (!XMLTools::attributeExists(EventNode, "haltOtherProcesses"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "haltOtherProcesses", QStringList() << "true" << "false", 1))
		return false;
	return true;
}

BaseEvent::ShowBattleAnimation_Map::ShowBattleAnimation_Map(BaseEvent *pBase) : Event(pBase)
{
	animationID = 0;
	targetType = HERO;
	targetID = -1;
	haltOtherProcesses = false;
}

BaseEvent::ShowBattleAnimation_Map::ShowBattleAnimation_Map(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	QString tempString;
	animationID = QString(EventNode.getAttribute("animationID")).toInt();
	tempString = EventNode.getAttribute("targetType");
	if (tempString == "hero")
	{
		targetType = HERO;
		targetID = -1;
	}
	else if (tempString == "thisEvent")
	{
		targetType = THIS_EVENT;
		targetID = -1;
	}
	else if (tempString == "vehicle")
	{
		targetType = VEHICLE;
		targetID = QString(EventNode.getAttribute("vehicleID")).toInt();
	}
	else if (tempString == "event")
	{
		targetType = EVENT;
		targetID = QString(EventNode.getAttribute("eventID")).toInt();
	}
	haltOtherProcesses = (QString(EventNode.getAttribute("haltOtherProcesses")) == "true") ? true:false;
}

BaseEvent::ShowBattleAnimation_Map::ShowBattleAnimation_Map(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	animationID = storageFile.getSignedInt();
	targetType = storageFile.getSignedInt();
	targetID = storageFile.getSignedInt();
	haltOtherProcesses = storageFile.getBool();
}

BaseEvent::ShowBattleAnimation_Map::ShowBattleAnimation_Map(ShowBattleAnimation_Map *other, BaseEvent *pBase) : Event(pBase)
{
	animationID = other->animationID;
	targetType = other->targetType;
	targetID = other->targetID;
	haltOtherProcesses = other->haltOtherProcesses;
}
//END BaseEvent::ShowBattleAnimation_Map Member Functions

//BEGIN BaseEvent::ShowGlobalAnimation Member Functions
XMLNode BaseEvent::ShowGlobalAnimation::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	if (action == PLAY_ONCE)
	{
		EventNode.addAttribute("action", "playOnce");
		EventNode.addAttribute("animationID", QString::number(animationID).toUtf8().data());
		EventNode.addAttribute("haltOtherProcesses", (haltOtherProcesses) ? "true":"false");
	}
	else if (action == BEGIN_PLAYING)
	{
		EventNode.addAttribute("action", "beginPlaying");
		EventNode.addAttribute("animationID", QString::number(animationID).toUtf8().data());
	}
	else if (action == STOP_PLAYING)
		EventNode.addAttribute("action", "stopPlaying");
	return EventNode;
}

void BaseEvent::ShowGlobalAnimation::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(action);
	storageFile.putSignedInt(animationID);
	storageFile.putBool(haltOtherProcesses);
}

QStringList BaseEvent::ShowGlobalAnimation::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Show Global Animation: ";
	if (action == PLAY_ONCE)
	{
		viewableText[0] += QString("Play %1 Once").arg(ProjectData::globalAnimationNames[animationID]);
		if (haltOtherProcesses)
			viewableText[0] += " (Halt Other Processes)";
	}
	else if (action == BEGIN_PLAYING)
		viewableText[0] += QString("Start Playing %1").arg(ProjectData::globalAnimationNames[animationID]);
	else if (action == STOP_PLAYING)
		viewableText[0] += "Stop Playing";
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::ShowGlobalAnimation::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	ConfigureEvent_ShowGlobalAnimation *configEvent = new ConfigureEvent_ShowGlobalAnimation;
	bool configured = false;
	configEvent->setAction(action);
	configEvent->setAnimationID(animationID);
	configEvent->setHaltOtherProcesses(haltOtherProcesses);
	if (configEvent->exec())
	{
		configured = true;
		action = configEvent->getAction();
		animationID = configEvent->getAnimationID();
		haltOtherProcesses = configEvent->getHaltOtherProcesses();
	}
	delete configEvent;
	return configured;
}
#endif

bool BaseEvent::ShowGlobalAnimation::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	QString tempString;
	if (!XMLTools::attributeExists(EventNode, "action"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "action", QStringList() << "playOnce" << "beginPlaying" << "stopPlaying"))
		return false;
	tempString = EventNode.getAttribute("action");
	if (tempString == "playOnce")
	{
		if (!XMLTools::attributeExists(EventNode, "animationID"))
			return false;
		if (!XMLTools::attributeExists(EventNode, "haltOtherProcesses"))
			return false;
		if (!XMLTools::attributeStringValid(EventNode, "haltOtherProcesses", QStringList() << "true" << "false", 1))
			return false;
	}
	else if (tempString == "beginPlaying")
	{
		if (!XMLTools::attributeExists(EventNode, "animationID"))
			return false;
	}
	return true;
}

BaseEvent::ShowGlobalAnimation::ShowGlobalAnimation(BaseEvent *pBase) : Event(pBase)
{
	action = PLAY_ONCE;
	animationID = 0;
	haltOtherProcesses = true;
}

BaseEvent::ShowGlobalAnimation::ShowGlobalAnimation(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	QString tempString = EventNode.getAttribute("action");
	if (tempString == "playOnce")
	{
		action = PLAY_ONCE;
		animationID = QString(EventNode.getAttribute("animationID")).toInt();
		haltOtherProcesses = (QString(EventNode.getAttribute("haltOtherProcesses")) == "true") ? true:false;
	}
	else if (tempString == "beginPlaying")
	{
		action = BEGIN_PLAYING;
		animationID = QString(EventNode.getAttribute("animationID")).toInt();
		haltOtherProcesses = true;
	}
	else if (tempString == "stopPlaying")
	{
		action = STOP_PLAYING;
		animationID = 0;
		haltOtherProcesses = true;
	}
}

BaseEvent::ShowGlobalAnimation::ShowGlobalAnimation(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	action = storageFile.getSignedInt();
	animationID = storageFile.getSignedInt();
	haltOtherProcesses = storageFile.getBool();
}

BaseEvent::ShowGlobalAnimation::ShowGlobalAnimation(ShowGlobalAnimation *other, BaseEvent *pBase) : Event(pBase)
{
	action = other->action;
	animationID = other->animationID;
	haltOtherProcesses = other->haltOtherProcesses;
}
//END BaseEvent::ShowGlobalAnimation Member Functions

//BEGIN BaseEvent::ShowLayer Member Functions
XMLNode BaseEvent::ShowLayer::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	EventNode.addAttribute("layer", QString::number(layer).toUtf8().data());
	EventNode.addAttribute("layerReferencedInVariable", (layerReferencedInVariable) ? "true":"false");
	if (transition == ProjectData::SHOW_FADEIN)
		EventNode.addAttribute("transition", "fadeIn");
	else if (transition == ProjectData::SHOW_RECONSTITUTEBLOCKS)
		EventNode.addAttribute("transition", "reconstituteBlocks");
	else if (transition == ProjectData::SHOW_UNWIPEDOWNWARD)
		EventNode.addAttribute("transition", "unwipeDownward");
	else if (transition == ProjectData::SHOW_UNWIPEUPWARD)
		EventNode.addAttribute("transition", "unwipeUpward");
	else if (transition == ProjectData::SHOW_VENETIANBLINDS)
		EventNode.addAttribute("transition", "venetianBlinds");
	else if (transition == ProjectData::SHOW_VERTICALBLINDS)
		EventNode.addAttribute("transition", "verticalBlinds");
	else if (transition == ProjectData::SHOW_HORIZONTALBLINDS)
		EventNode.addAttribute("transition", "horizontalBlinds");
	else if (transition == ProjectData::SHOW_RECEDINGSQUARE)
		EventNode.addAttribute("transition", "recedingSquare");
	else if (transition == ProjectData::SHOW_EXPANDINGSQUARE)
		EventNode.addAttribute("transition", "expandingSquare");
	else if (transition == ProjectData::SHOW_SCREENMOVESDOWN)
		EventNode.addAttribute("transition", "screenMovesDown");
	else if (transition == ProjectData::SHOW_SCREENMOVESUP)
		EventNode.addAttribute("transition", "screenMovesUp");
	else if (transition == ProjectData::SHOW_SCREENMOVESRIGHT)
		EventNode.addAttribute("transition", "screenMovesRight");
	else if (transition == ProjectData::SHOW_SCREENMOVESLEFT)
		EventNode.addAttribute("transition", "screenMovesLeft");
	else if (transition == ProjectData::SHOW_VERTICALUNIFY)
		EventNode.addAttribute("transition", "verticalUnify");
	else if (transition == ProjectData::SHOW_HORIZONTALUNIFY)
		EventNode.addAttribute("transition", "horizontalUnify");
	else if (transition == ProjectData::SHOW_UNIFYQUADRANTS)
		EventNode.addAttribute("transition", "unifyQuadrants");
	else if (transition == ProjectData::SHOW_ZOOMOUT)
		EventNode.addAttribute("transition", "zoomOut");
	else if (transition == ProjectData::SHOW_MOSAIC)
		EventNode.addAttribute("transition", "mosaic");
	else if (transition == ProjectData::SHOW_WAVERSCREEN)
		EventNode.addAttribute("transition", "waverScreen");
	else if (transition == ProjectData::SHOW_INSTANTANEOUS)
		EventNode.addAttribute("transition", "instantaneous");
	return EventNode;
}

void BaseEvent::ShowLayer::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(layer);
	storageFile.putSignedInt(transition);
	storageFile.putBool(layerReferencedInVariable);
}

QStringList BaseEvent::ShowLayer::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Show Layer: ";
	if (layerReferencedInVariable)
		viewableText[0] += QString("layer=variable[%1], ").arg(ProjectData::numberVariableIdToString(layer));
	else
		viewableText[0] += QString("layer=%1, ").arg(layer);
	if (transition == ProjectData::SHOW_FADEIN)
		viewableText[0] += "Fade In";
	else if (transition == ProjectData::SHOW_RECONSTITUTEBLOCKS)
		viewableText[0] += "Reconstitute Blocks";
	else if (transition == ProjectData::SHOW_UNWIPEDOWNWARD)
		viewableText[0] += "Unwipe Downward";
	else if (transition == ProjectData::SHOW_UNWIPEUPWARD)
		viewableText[0] += "Unwipe Upward";
	else if (transition == ProjectData::SHOW_VENETIANBLINDS)
		viewableText[0] += "Venetian Blinds";
	else if (transition == ProjectData::SHOW_VERTICALBLINDS)
		viewableText[0] += "Vertical Blinds";
	else if (transition == ProjectData::SHOW_HORIZONTALBLINDS)
		viewableText[0] += "Horizontal Blinds";
	else if (transition == ProjectData::SHOW_RECEDINGSQUARE)
		viewableText[0] += "Receding Square";
	else if (transition == ProjectData::SHOW_EXPANDINGSQUARE)
		viewableText[0] += "Expanding Square";
	else if (transition == ProjectData::SHOW_SCREENMOVESDOWN)
		viewableText[0] += "Screen Moves Down";
	else if (transition == ProjectData::SHOW_SCREENMOVESUP)
		viewableText[0] += "Screen Moves Up";
	else if (transition == ProjectData::SHOW_SCREENMOVESRIGHT)
		viewableText[0] += "Screen Moves Right";
	else if (transition == ProjectData::SHOW_SCREENMOVESLEFT)
		viewableText[0] += "Screen Moves Left";
	else if (transition == ProjectData::SHOW_VERTICALUNIFY)
		viewableText[0] += "Vertical Unify";
	else if (transition == ProjectData::SHOW_HORIZONTALUNIFY)
		viewableText[0] += "Horizontal Unify";
	else if (transition == ProjectData::SHOW_UNIFYQUADRANTS)
		viewableText[0] += "Unify Quadrants";
	else if (transition == ProjectData::SHOW_ZOOMOUT)
		viewableText[0] += "Zoom Out";
	else if (transition == ProjectData::SHOW_MOSAIC)
		viewableText[0] += "Mosaic";
	else if (transition == ProjectData::SHOW_WAVERSCREEN)
		viewableText[0] += "Waver Screen";
	else if (transition == ProjectData::SHOW_INSTANTANEOUS)
		viewableText[0] += "Instant";
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::ShowLayer::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerType);
	ConfigureEvent_ShowHideLayer *configShowHideLayer = new ConfigureEvent_ShowHideLayer(callerID, true);
	bool configured = false;
	configShowHideLayer->setLayerReferencedInVariable(layerReferencedInVariable);
	configShowHideLayer->setLayer(layer);
	configShowHideLayer->setTransition(transition);
	if (configShowHideLayer->exec())
	{
		configured = true;
		layer = configShowHideLayer->getLayer();
		layerReferencedInVariable = configShowHideLayer->isLayerReferencedInVariable();
		transition = configShowHideLayer->getTransition();
	}
	delete configShowHideLayer;
	return configured;
}
#endif

bool BaseEvent::ShowLayer::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	QStringList tempStringList = QStringList() << "fadeIn" << "reconstituteBlocks" << "unwipeDownward" << "unwipeUpward" << "venetianBlinds" << "verticalBlinds" << "horizontalBlinds" << "recedingSquare" << "expandingSquare" << "screenMovesDown" << "screenMovesUp" << "screenMovesRight" << "screenMovesLeft" << "verticalUnify" << "horizontalUnify" << "unifyQuadrants" << "zoomOut" << "mosaic" << "waverScreen" << "instantaneous";
	if (!XMLTools::attributeExists(EventNode, "layer"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "layerReferencedInVariable"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "layerReferencedInVariable", QStringList() << "true" << "false", 1))
		return false;
	if (!XMLTools::attributeExists(EventNode, "transition"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "transition", tempStringList))
		return false;
	return true;
}

BaseEvent::ShowLayer::ShowLayer(BaseEvent *pBase) : Event(pBase)
{
	layer = 0;
	layerReferencedInVariable = false;
	transition = ProjectData::SHOW_FADEIN;
}

BaseEvent::ShowLayer::ShowLayer(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	QStringList tempStringList = QStringList() << "fadeIn" << "reconstituteBlocks" << "unwipeDownward" << "unwipeUpward" << "venetianBlinds" << "verticalBlinds" << "horizontalBlinds" << "recedingSquare" << "expandingSquare" << "screenMovesDown" << "screenMovesUp" << "screenMovesRight" << "screenMovesLeft" << "verticalUnify" << "horizontalUnify" << "unifyQuadrants" << "zoomOut" << "mosaic" << "waverScreen" << "instantaneous";
	layer = QString(EventNode.getAttribute("layer")).toInt();
	layerReferencedInVariable = (QString(EventNode.getAttribute("layerReferencedInVariable")) == "true") ? true:false;
	transition = tempStringList.indexOf(EventNode.getAttribute("transition"));
}

BaseEvent::ShowLayer::ShowLayer(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	layer = storageFile.getSignedInt();
	transition = storageFile.getSignedInt();
	layerReferencedInVariable = storageFile.getBool();
}

BaseEvent::ShowLayer::ShowLayer(ShowLayer *other, BaseEvent *pBase) : Event(pBase)
{
	layer = other->layer;
	transition = other->transition;
	layerReferencedInVariable = other->layerReferencedInVariable;
}
//END BaseEvent::ShowLayer Member Functions

//BEGIN BaseEvent::ShowPopupMenu Member Functions
XMLNode BaseEvent::ShowPopupMenu::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	EventNode.addAttribute("menuID", QString::number(menuID).toUtf8().data());
	EventNode.addAttribute("menuReferencedInVariable", (menuReferencedInVariable) ? "true":"false");
	EventNode.addAttribute("x", QString::number(x).toUtf8().data());
	EventNode.addAttribute("y", QString::number(y).toUtf8().data());
	EventNode.addAttribute("coordinatesStoredInVariables", (coordinatesStoredInVariables) ? "true":"false");
	return EventNode;
}

void BaseEvent::ShowPopupMenu::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(menuID);
	storageFile.putSignedInt(x);
	storageFile.putSignedInt(y);
	storageFile.putBool(menuReferencedInVariable);
	storageFile.putBool(coordinatesStoredInVariables);
}

QStringList BaseEvent::ShowPopupMenu::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Show Popup Menu: ";
	if (menuReferencedInVariable)
		viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(menuID));
	else
		viewableText[0] += ProjectData::popupMenuLayoutNames[menuID];
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::ShowPopupMenu::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	ConfigureEvent_ShowPopupMenu *configShowPopupMenu = new ConfigureEvent_ShowPopupMenu;
	bool configured = false;
	configShowPopupMenu->setIsReferencedMenu(menuReferencedInVariable);
	configShowPopupMenu->setMenuID(menuID);
	configShowPopupMenu->setIsStoredCoordinates(coordinatesStoredInVariables);
	configShowPopupMenu->setCoordinates(x, y);
	if (configShowPopupMenu->exec())
	{
		configured = true;
		menuID = configShowPopupMenu->getMenuID();
		menuReferencedInVariable = configShowPopupMenu->isReferencedMenu();
		x = configShowPopupMenu->getXCoordinate();
		y = configShowPopupMenu->getYCoordinate();
		coordinatesStoredInVariables = configShowPopupMenu->isStoredCoordinates();
	}
	delete configShowPopupMenu;
	return configured;
}
#endif

bool BaseEvent::ShowPopupMenu::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	QStringList trueFalseList = QStringList() << "true" << "false";
	if (!XMLTools::attributeExists(EventNode, "menuID"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "menuReferencedInVariable"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "menuReferencedInVariable", trueFalseList, 1))
		return false;
	if (!XMLTools::attributeExists(EventNode, "x"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "y"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "coordinatesStoredInVariables"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "coordinatesStoredInVariables", trueFalseList, 1))
		return false;
	return true;
}

BaseEvent::ShowPopupMenu::ShowPopupMenu(BaseEvent *pBase) : Event(pBase)
{
	menuID = 0;
	x = 0;
	y = 0;
	menuReferencedInVariable = false;
	coordinatesStoredInVariables = false;
}

BaseEvent::ShowPopupMenu::ShowPopupMenu(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	menuID = QString(EventNode.getAttribute("menuID")).toInt();
	x = QString(EventNode.getAttribute("x")).toInt();
	y = QString(EventNode.getAttribute("y")).toInt();
	menuReferencedInVariable = (QString(EventNode.getAttribute("menuReferencedInVariable")) == "true") ? true:false;
	coordinatesStoredInVariables = (QString(EventNode.getAttribute("coordinatesStoredInVariables")) == "true") ? true:false;
}

BaseEvent::ShowPopupMenu::ShowPopupMenu(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	menuID = storageFile.getSignedInt();
	x = storageFile.getSignedInt();
	y = storageFile.getSignedInt();
	menuReferencedInVariable = storageFile.getBool();
	coordinatesStoredInVariables = storageFile.getBool();
}

BaseEvent::ShowPopupMenu::ShowPopupMenu(ShowPopupMenu *other, BaseEvent *pBase) : Event(pBase)
{
	menuID = other->menuID;
	x = other->x;
	y = other->y;
	menuReferencedInVariable = other->menuReferencedInVariable;
	coordinatesStoredInVariables = other->coordinatesStoredInVariables;
}
//END BaseEvent::ShowPopupMenu Member Functions

//BEGIN BaseEvent::ShowScreen Member Functions
XMLNode BaseEvent::ShowScreen::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	if (transition == ProjectData::SHOW_FADEIN)
		EventNode.addAttribute("transition", "fadeIn");
	else if (transition == ProjectData::SHOW_RECONSTITUTEBLOCKS)
		EventNode.addAttribute("transition", "reconstituteBlocks");
	else if (transition == ProjectData::SHOW_UNWIPEDOWNWARD)
		EventNode.addAttribute("transition", "unwipeDownward");
	else if (transition == ProjectData::SHOW_UNWIPEUPWARD)
		EventNode.addAttribute("transition", "unwipeUpward");
	else if (transition == ProjectData::SHOW_VENETIANBLINDS)
		EventNode.addAttribute("transition", "venetianBlinds");
	else if (transition == ProjectData::SHOW_VERTICALBLINDS)
		EventNode.addAttribute("transition", "verticalBlinds");
	else if (transition == ProjectData::SHOW_HORIZONTALBLINDS)
		EventNode.addAttribute("transition", "horizontalBlinds");
	else if (transition == ProjectData::SHOW_RECEDINGSQUARE)
		EventNode.addAttribute("transition", "recedingSquare");
	else if (transition == ProjectData::SHOW_EXPANDINGSQUARE)
		EventNode.addAttribute("transition", "expandingSquare");
	else if (transition == ProjectData::SHOW_SCREENMOVESDOWN)
		EventNode.addAttribute("transition", "screenMovesDown");
	else if (transition == ProjectData::SHOW_SCREENMOVESUP)
		EventNode.addAttribute("transition", "screenMovesUp");
	else if (transition == ProjectData::SHOW_SCREENMOVESRIGHT)
		EventNode.addAttribute("transition", "screenMovesRight");
	else if (transition == ProjectData::SHOW_SCREENMOVESLEFT)
		EventNode.addAttribute("transition", "screenMovesLeft");
	else if (transition == ProjectData::SHOW_VERTICALUNIFY)
		EventNode.addAttribute("transition", "verticalUnify");
	else if (transition == ProjectData::SHOW_HORIZONTALUNIFY)
		EventNode.addAttribute("transition", "horizontalUnify");
	else if (transition == ProjectData::SHOW_UNIFYQUADRANTS)
		EventNode.addAttribute("transition", "unifyQuadrants");
	else if (transition == ProjectData::SHOW_ZOOMOUT)
		EventNode.addAttribute("transition", "zoomOut");
	else if (transition == ProjectData::SHOW_MOSAIC)
		EventNode.addAttribute("transition", "mosaic");
	else if (transition == ProjectData::SHOW_WAVERSCREEN)
		EventNode.addAttribute("transition", "waverScreen");
	else if (transition == ProjectData::SHOW_INSTANTANEOUS)
		EventNode.addAttribute("transition", "instantaneous");
	return EventNode;
}

void BaseEvent::ShowScreen::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(transition);
}

QStringList BaseEvent::ShowScreen::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Show Screen: ";
	if (transition == ProjectData::SHOW_FADEIN)
		viewableText[0] += "Fade In";
	else if (transition == ProjectData::SHOW_RECONSTITUTEBLOCKS)
		viewableText[0] += "Reconstitute Blocks";
	else if (transition == ProjectData::SHOW_UNWIPEDOWNWARD)
		viewableText[0] += "Unwipe Downward";
	else if (transition == ProjectData::SHOW_UNWIPEUPWARD)
		viewableText[0] += "Unwipe Upward";
	else if (transition == ProjectData::SHOW_VENETIANBLINDS)
		viewableText[0] += "Venetian Blinds";
	else if (transition == ProjectData::SHOW_VERTICALBLINDS)
		viewableText[0] += "Vertical Blinds";
	else if (transition == ProjectData::SHOW_HORIZONTALBLINDS)
		viewableText[0] += "Horizontal Blinds";
	else if (transition == ProjectData::SHOW_RECEDINGSQUARE)
		viewableText[0] += "Receding Square";
	else if (transition == ProjectData::SHOW_EXPANDINGSQUARE)
		viewableText[0] += "Expanding Square";
	else if (transition == ProjectData::SHOW_SCREENMOVESDOWN)
		viewableText[0] += "Screen Moves Down";
	else if (transition == ProjectData::SHOW_SCREENMOVESUP)
		viewableText[0] += "Screen Moves Up";
	else if (transition == ProjectData::SHOW_SCREENMOVESRIGHT)
		viewableText[0] += "Screen Moves Right";
	else if (transition == ProjectData::SHOW_SCREENMOVESLEFT)
		viewableText[0] += "Screen Moves Left";
	else if (transition == ProjectData::SHOW_VERTICALUNIFY)
		viewableText[0] += "Vertical Unify";
	else if (transition == ProjectData::SHOW_HORIZONTALUNIFY)
		viewableText[0] += "Horizontal Unify";
	else if (transition == ProjectData::SHOW_UNIFYQUADRANTS)
		viewableText[0] += "Unify Quadrants";
	else if (transition == ProjectData::SHOW_ZOOMOUT)
		viewableText[0] += "Zoom Out";
	else if (transition == ProjectData::SHOW_MOSAIC)
		viewableText[0] += "Mosaic";
	else if (transition == ProjectData::SHOW_WAVERSCREEN)
		viewableText[0] += "Waver Screen";
	else if (transition == ProjectData::SHOW_INSTANTANEOUS)
		viewableText[0] += "Instant";
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::ShowScreen::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	SelectScreenTransition *selectTransition = new SelectScreenTransition(false);
	bool configured = false;
	selectTransition->setTransition(transition);
	if (selectTransition->exec())
	{
		configured = true;
		transition = selectTransition->getTransition();
	}
	delete selectTransition;
	return configured;
}
#endif

bool BaseEvent::ShowScreen::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	QStringList tempStringList = QStringList() << "fadeIn" << "reconstituteBlocks" << "unwipeDownward" << "unwipeUpward" << "venetianBlinds" << "verticalBlinds" << "horizontalBlinds" << "recedingSquare" << "expandingSquare" << "screenMovesDown" << "screenMovesUp" << "screenMovesRight" << "screenMovesLeft" << "verticalUnify" << "horizontalUnify" << "unifyQuadrants" << "zoomOut" << "mosaic" << "waverScreen" << "instantaneous";
	if (!XMLTools::attributeExists(EventNode, "transition"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "transition", tempStringList))
		return false;
	return true;
}

BaseEvent::ShowScreen::ShowScreen(BaseEvent *pBase) : Event(pBase)
{
	transition = ProjectData::SHOW_FADEIN;
}

BaseEvent::ShowScreen::ShowScreen(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	QStringList tempStringList = QStringList() << "fadeIn" << "reconstituteBlocks" << "unwipeDownward" << "unwipeUpward" << "venetianBlinds" << "verticalBlinds" << "horizontalBlinds" << "recedingSquare" << "expandingSquare" << "screenMovesDown" << "screenMovesUp" << "screenMovesRight" << "screenMovesLeft" << "verticalUnify" << "horizontalUnify" << "unifyQuadrants" << "zoomOut" << "mosaic" << "waverScreen" << "instantaneous";
	transition = tempStringList.indexOf(EventNode.getAttribute("transition"));
}

BaseEvent::ShowScreen::ShowScreen(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	transition = storageFile.getSignedInt();
}

BaseEvent::ShowScreen::ShowScreen(ShowScreen *other, BaseEvent *pBase) : Event(pBase)
{
	transition = other->transition;
}
//END BaseEvent::ShowScreen Member Functions

//BEGIN BaseEvent::ShowScreenOverlay Member Functions
TODO("Uncomment code when screen overlays are done.");
XMLNode BaseEvent::ShowScreenOverlay::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	EventNode.addAttribute("overlayID", QString::number(overlayID).toUtf8());
	if (transition == ProjectData::SHOW_FADEIN)
		EventNode.addAttribute("transition", "fadeIn");
	else if (transition == ProjectData::SHOW_RECONSTITUTEBLOCKS)
		EventNode.addAttribute("transition", "reconstituteBlocks");
	else if (transition == ProjectData::SHOW_UNWIPEDOWNWARD)
		EventNode.addAttribute("transition", "unwipeDownward");
	else if (transition == ProjectData::SHOW_UNWIPEUPWARD)
		EventNode.addAttribute("transition", "unwipeUpward");
	else if (transition == ProjectData::SHOW_VENETIANBLINDS)
		EventNode.addAttribute("transition", "venetianBlinds");
	else if (transition == ProjectData::SHOW_VERTICALBLINDS)
		EventNode.addAttribute("transition", "verticalBlinds");
	else if (transition == ProjectData::SHOW_HORIZONTALBLINDS)
		EventNode.addAttribute("transition", "horizontalBlinds");
	else if (transition == ProjectData::SHOW_RECEDINGSQUARE)
		EventNode.addAttribute("transition", "recedingSquare");
	else if (transition == ProjectData::SHOW_EXPANDINGSQUARE)
		EventNode.addAttribute("transition", "expandingSquare");
	else if (transition == ProjectData::SHOW_SCREENMOVESDOWN)
		EventNode.addAttribute("transition", "screenMovesDown");
	else if (transition == ProjectData::SHOW_SCREENMOVESUP)
		EventNode.addAttribute("transition", "screenMovesUp");
	else if (transition == ProjectData::SHOW_SCREENMOVESRIGHT)
		EventNode.addAttribute("transition", "screenMovesRight");
	else if (transition == ProjectData::SHOW_SCREENMOVESLEFT)
		EventNode.addAttribute("transition", "screenMovesLeft");
	else if (transition == ProjectData::SHOW_VERTICALUNIFY)
		EventNode.addAttribute("transition", "verticalUnify");
	else if (transition == ProjectData::SHOW_HORIZONTALUNIFY)
		EventNode.addAttribute("transition", "horizontalUnify");
	else if (transition == ProjectData::SHOW_UNIFYQUADRANTS)
		EventNode.addAttribute("transition", "unifyQuadrants");
	else if (transition == ProjectData::SHOW_ZOOMOUT)
		EventNode.addAttribute("transition", "zoomOut");
	else if (transition == ProjectData::SHOW_MOSAIC)
		EventNode.addAttribute("transition", "mosaic");
	else if (transition == ProjectData::SHOW_WAVERSCREEN)
		EventNode.addAttribute("transition", "waverScreen");
	else if (transition == ProjectData::SHOW_INSTANTANEOUS)
		EventNode.addAttribute("transition", "instantaneous");
	return EventNode;
}

void BaseEvent::ShowScreenOverlay::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(overlayID);
	storageFile.putSignedInt(transition);
}

QStringList BaseEvent::ShowScreenOverlay::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Show Screen Overlay: ";
	//viewableText[0] += ProjectData::screenOverlayNames[overlayID];
	viewableText[0] += " ";
	if (transition == ProjectData::SHOW_FADEIN)
		viewableText[0] += "Fade In";
	else if (transition == ProjectData::SHOW_RECONSTITUTEBLOCKS)
		viewableText[0] += "Reconstitute Blocks";
	else if (transition == ProjectData::SHOW_UNWIPEDOWNWARD)
		viewableText[0] += "Unwipe Downward";
	else if (transition == ProjectData::SHOW_UNWIPEUPWARD)
		viewableText[0] += "Unwipe Upward";
	else if (transition == ProjectData::SHOW_VENETIANBLINDS)
		viewableText[0] += "Venetian Blinds";
	else if (transition == ProjectData::SHOW_VERTICALBLINDS)
		viewableText[0] += "Vertical Blinds";
	else if (transition == ProjectData::SHOW_HORIZONTALBLINDS)
		viewableText[0] += "Horizontal Blinds";
	else if (transition == ProjectData::SHOW_RECEDINGSQUARE)
		viewableText[0] += "Receding Square";
	else if (transition == ProjectData::SHOW_EXPANDINGSQUARE)
		viewableText[0] += "Expanding Square";
	else if (transition == ProjectData::SHOW_SCREENMOVESDOWN)
		viewableText[0] += "Screen Moves Down";
	else if (transition == ProjectData::SHOW_SCREENMOVESUP)
		viewableText[0] += "Screen Moves Up";
	else if (transition == ProjectData::SHOW_SCREENMOVESRIGHT)
		viewableText[0] += "Screen Moves Right";
	else if (transition == ProjectData::SHOW_SCREENMOVESLEFT)
		viewableText[0] += "Screen Moves Left";
	else if (transition == ProjectData::SHOW_VERTICALUNIFY)
		viewableText[0] += "Vertical Unify";
	else if (transition == ProjectData::SHOW_HORIZONTALUNIFY)
		viewableText[0] += "Horizontal Unify";
	else if (transition == ProjectData::SHOW_UNIFYQUADRANTS)
		viewableText[0] += "Unify Quadrants";
	else if (transition == ProjectData::SHOW_ZOOMOUT)
		viewableText[0] += "Zoom Out";
	else if (transition == ProjectData::SHOW_MOSAIC)
		viewableText[0] += "Mosaic";
	else if (transition == ProjectData::SHOW_WAVERSCREEN)
		viewableText[0] += "Waver Screen";
	else if (transition == ProjectData::SHOW_INSTANTANEOUS)
		viewableText[0] += "Instant";
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::ShowScreenOverlay::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	QMessageBox::information(0, "Not Implemented", "Screen Overlays are not yet implemented.");
	TODO("Uncomment following when screen overlays are ready.");
	/*ConfigureEvent_ShowScreenOverlay *configEvent = new ConfigureEvent_ShowScreenOverlay;
	bool configured = false;
	configEvent->setOverlayID(overlayID);
	configEvent->setTransition(transition);
	if (configEvent->exec())
	{
		configured = true;
		overlayID = configEvent->getOverlayID();
		transition = configEvent->getTransition();
	}
	delete configEvent;
	return configured;*/
	return false;
}
#endif

bool BaseEvent::ShowScreenOverlay::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	QStringList tempStringList;
	if (!XMLTools::attributeExists(EventNode, "overlayID"))
		return false;
	tempStringList << "fadeIn" << "reconstituteBlocks" << "unwipeDownward" << "unwipeUpward" << "venetianBlinds" << "verticalBlinds" << "horizontalBlinds" << "recedingSquare" << "expandingSquare" << "screenMovesDown" << "screenMovesUp" << "screenMovesRight" << "screenMovesLeft" << "verticalUnify" << "horizontalUnify" << "unifyQuadrants" << "zoomOut" << "mosaic" << "waverScreen" << "instantaneous";
	if (!XMLTools::attributeExists(EventNode, "transition"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "transition", tempStringList))
		return false;
	return true;
}

BaseEvent::ShowScreenOverlay::ShowScreenOverlay(BaseEvent *pBase) : Event(pBase)
{
	overlayID = 0;
	transition = ProjectData::SHOW_FADEIN;
}

BaseEvent::ShowScreenOverlay::ShowScreenOverlay(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	QStringList tempStringList = QStringList() << "fadeIn" << "reconstituteBlocks" << "unwipeDownward" << "unwipeUpward" << "venetianBlinds" << "verticalBlinds" << "horizontalBlinds" << "recedingSquare" << "expandingSquare" << "screenMovesDown" << "screenMovesUp" << "screenMovesRight" << "screenMovesLeft" << "verticalUnify" << "horizontalUnify" << "unifyQuadrants" << "zoomOut" << "mosaic" << "waverScreen" << "instantaneous";
	overlayID = QString(EventNode.getAttribute("overlayID")).toInt();
	transition = tempStringList.indexOf(EventNode.getAttribute("transition"));
}

BaseEvent::ShowScreenOverlay::ShowScreenOverlay(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	overlayID = storageFile.getSignedInt();
	transition = storageFile.getSignedInt();
}

BaseEvent::ShowScreenOverlay::ShowScreenOverlay(ShowScreenOverlay *other, BaseEvent *pBase) : Event(pBase)
{
	overlayID = other->overlayID;
	transition = other->transition;
}
//END BaseEvent::ShowScreenOverlay Member Functions

//BEGIN BaseEvent::StoreEventID Member Functions
XMLNode BaseEvent::StoreEventID::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	EventNode.addAttribute("x", QString::number(x).toUtf8().data());
	EventNode.addAttribute("y", QString::number(y).toUtf8().data());
	EventNode.addAttribute("layer", QString::number(layer).toUtf8().data());
	EventNode.addAttribute("locationStoredInVariables", (locationStoredInVariables) ? "true":"false");
	EventNode.addAttribute("variableID", QString::number(variableID).toUtf8().data());
	return EventNode;
}

void BaseEvent::StoreEventID::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(x);
	storageFile.putSignedInt(y);
	storageFile.putSignedInt(layer);
	storageFile.putSignedInt(variableID);
	storageFile.putBool(locationStoredInVariables);
}

QStringList BaseEvent::StoreEventID::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Store Event ID: location=";
	if (locationStoredInVariables)
		viewableText[0] += QString("(variable[%1], variable[%2]) layer=variable[%3] ").arg(ProjectData::numberVariableIdToString(x)).arg(ProjectData::numberVariableIdToString(y)).arg(ProjectData::numberVariableIdToString(layer));
	else
		viewableText[0] += QString("(%1, %2) layer=%3 ").arg(x).arg(y).arg(layer);
	viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(variableID));
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::StoreEventID::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerType);
	ConfigureEvent_StoreTargetID *configEvent = new ConfigureEvent_StoreTargetID("Event", callerID);
	bool configured = false;
	configEvent->setXLocation(x);
	configEvent->setYLocation(y);
	configEvent->setLayer(layer);
	configEvent->setTargetVariableID(variableID);
	configEvent->setLocationStoredInVariables(locationStoredInVariables);
	if (configEvent->exec())
	{
		configured = true;
		x = configEvent->getXLocation();
		y = configEvent->getYLocation();
		layer = configEvent->getLayer();
		variableID = configEvent->getTargetVariableID();
		locationStoredInVariables = configEvent->isLocationStoredInVariables();
	}
	delete configEvent;
	return configured;
}
#endif

bool BaseEvent::StoreEventID::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	if (!XMLTools::attributeExists(EventNode, "x"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "y"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "layer"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "locationStoredInVariables"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "locationStoredInVariables", QStringList() << "true" << "false", 1))
		return false;
	if (!XMLTools::attributeExists(EventNode, "variableID"))
		return false;
	return true;
}

BaseEvent::StoreEventID::StoreEventID(BaseEvent *pBase) : Event(pBase)
{
	x = 0;
	y = 0;
	layer = 0;
	variableID = 0;
	locationStoredInVariables = false;
}

BaseEvent::StoreEventID::StoreEventID(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	x = QString(EventNode.getAttribute("x")).toInt();
	y = QString(EventNode.getAttribute("y")).toInt();
	layer = QString(EventNode.getAttribute("layer")).toInt();
	locationStoredInVariables = (QString(EventNode.getAttribute("locationStoredInVariables")) == "true") ? true:false;
	variableID = QString(EventNode.getAttribute("variableID")).toInt();
}

BaseEvent::StoreEventID::StoreEventID(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	x = storageFile.getSignedInt();
	y = storageFile.getSignedInt();
	layer = storageFile.getSignedInt();
	variableID = storageFile.getSignedInt();
	locationStoredInVariables = storageFile.getBool();
}

BaseEvent::StoreEventID::StoreEventID(StoreEventID *other, BaseEvent *pBase) : Event(pBase)
{
	x = other->x;
	y = other->y;
	layer = other->layer;
	variableID = other->variableID;
	locationStoredInVariables = other->locationStoredInVariables;
}
//END BaseEvent::StoreEventID Member Functions

//BEGIN BaseEvent::StoreTerrainID Member Functions
XMLNode BaseEvent::StoreTerrainID::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	EventNode.addAttribute("x", QString::number(x).toUtf8().data());
	EventNode.addAttribute("y", QString::number(y).toUtf8().data());
	EventNode.addAttribute("layer", QString::number(layer).toUtf8().data());
	EventNode.addAttribute("locationStoredInVariables", (locationStoredInVariables) ? "true":"false");
	EventNode.addAttribute("variableID", QString::number(variableID).toUtf8().data());
	return EventNode;
}

void BaseEvent::StoreTerrainID::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(x);
	storageFile.putSignedInt(y);
	storageFile.putSignedInt(layer);
	storageFile.putSignedInt(variableID);
	storageFile.putBool(locationStoredInVariables);
}

QStringList BaseEvent::StoreTerrainID::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Store Terrain ID: location=";
	if (locationStoredInVariables)
		viewableText[0] += QString("(variable[%1], variable[%2]) layer=variable[%3] ").arg(ProjectData::numberVariableIdToString(x)).arg(ProjectData::numberVariableIdToString(y)).arg(ProjectData::numberVariableIdToString(layer));
	else
		viewableText[0] += QString("(%1, %2) layer=%3 ").arg(x).arg(y).arg(layer);
	viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(variableID));
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::StoreTerrainID::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerType);
	ConfigureEvent_StoreTargetID *configEvent = new ConfigureEvent_StoreTargetID("Terrain", callerID);
	bool configured = false;
	configEvent->setXLocation(x);
	configEvent->setYLocation(y);
	configEvent->setLayer(layer);
	configEvent->setTargetVariableID(variableID);
	configEvent->setLocationStoredInVariables(locationStoredInVariables);
	if (configEvent->exec())
	{
		configured = true;
		x = configEvent->getXLocation();
		y = configEvent->getYLocation();
		layer = configEvent->getLayer();
		variableID = configEvent->getTargetVariableID();
		locationStoredInVariables = configEvent->isLocationStoredInVariables();
	}
	delete configEvent;
	return configured;
}
#endif

bool BaseEvent::StoreTerrainID::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	if (!XMLTools::attributeExists(EventNode, "x"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "y"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "layer"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "locationStoredInVariables"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "locationStoredInVariables", QStringList() << "true" << "false", 1))
		return false;
	if (!XMLTools::attributeExists(EventNode, "variableID"))
		return false;
	return true;
}

BaseEvent::StoreTerrainID::StoreTerrainID(BaseEvent *pBase) : Event(pBase)
{
	x = 0;
	y = 0;
	layer = 0;
	variableID = 0;
	locationStoredInVariables = false;
}

BaseEvent::StoreTerrainID::StoreTerrainID(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	x = QString(EventNode.getAttribute("x")).toInt();
	y = QString(EventNode.getAttribute("y")).toInt();
	layer = QString(EventNode.getAttribute("layer")).toInt();
	locationStoredInVariables = (QString(EventNode.getAttribute("locationStoredInVariables")) == "true") ? true:false;
	variableID = QString(EventNode.getAttribute("variableID")).toInt();
}

BaseEvent::StoreTerrainID::StoreTerrainID(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	x = storageFile.getSignedInt();
	y = storageFile.getSignedInt();
	layer = storageFile.getSignedInt();
	variableID = storageFile.getSignedInt();
	locationStoredInVariables = storageFile.getBool();
}

BaseEvent::StoreTerrainID::StoreTerrainID(StoreTerrainID *other, BaseEvent *pBase) : Event(pBase)
{
	x = other->x;
	y = other->y;
	layer = other->layer;
	variableID = other->variableID;
	locationStoredInVariables = other->locationStoredInVariables;
}
//END BaseEvent::StoreTerrainID Member Functions

//BEGIN BaseEvent::StringOperation Member Functions
XMLNode BaseEvent::StringOperation::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	if (stringToChange == SINGLE)
	{
		EventNode.addAttribute("stringToChange", "single");
		EventNode.addAttribute("stringID", QString::number(string1ID).toUtf8().data());
	}
	else if (stringToChange == RANGE)
	{
		EventNode.addAttribute("stringToChange", "range");
		EventNode.addAttribute("startStringID", QString::number(string1ID).toUtf8().data());
		EventNode.addAttribute("endStringID", QString::number(string2ID).toUtf8().data());
	}
	else if (stringToChange == REFERENCE)
	{
		EventNode.addAttribute("stringToChange", "reference");
		EventNode.addAttribute("variableID", QString::number(string1ID).toUtf8().data());
	}
	if (operation == SET)
		EventNode.addAttribute("operation", "set");
	else if (operation == ADD)
		EventNode.addAttribute("operation", "add");
	else if (operation == REMOVE_FIRST_CHARACTER)
		EventNode.addAttribute("operation", "removeFirstCharacter");
	else if (operation == REMOVE_LAST_CHARACTER)
		EventNode.addAttribute("operation", "removeLastCharacter");
	if (operation == SET || operation == ADD)
	{
		if (operand == VALUE)
		{
			EventNode.addAttribute("operand", "value");
			EventNode.addAttribute("value", value1.toUtf8().data());
		}
		else if (operand == STRING)
		{
			EventNode.addAttribute("operand", "string");
			EventNode.addAttribute("stringID", QString::number(operandID).toUtf8().data());
			EventNode.addAttribute("stringReferencedInVariable", (operandReferencedInVariable) ? "true":"false");
		}
		else if (operand == VARIABLE)
		{
			EventNode.addAttribute("operand", "variable");
			EventNode.addAttribute("variableID", QString::number(operandID).toUtf8().data());
			EventNode.addAttribute("variableReferencedInVariable", (operandReferencedInVariable) ? "true":"false");
		}
		else if (operand == SWITCH)
		{
			EventNode.addAttribute("operand", "switch");
			EventNode.addAttribute("switchID", QString::number(operandID).toUtf8().data());
			EventNode.addAttribute("switchReferencedInVariable", (operandReferencedInVariable) ? "true":"false");
			EventNode.addAttribute("onText", value1.toUtf8().data());
			EventNode.addAttribute("offText", value2.toUtf8().data());
		}
		else if (operand == PARTY_MEMBER_NAME)
		{
			EventNode.addAttribute("operand", "partyMemberName");
			EventNode.addAttribute("partyMember", QString::number(operandID).toUtf8().data());
		}
		else if (operand == PARTY_MEMBER_CLASS)
		{
			EventNode.addAttribute("operand", "partyMemberClass");
			EventNode.addAttribute("partyMember", QString::number(operandID).toUtf8().data());
		}
		else if (operand == CHARACTER_NAME)
		{
			EventNode.addAttribute("operand", "characterName");
			EventNode.addAttribute("characterID", QString::number(operandID).toUtf8().data());
			EventNode.addAttribute("characterReferencedInVariable", (operandReferencedInVariable) ? "true":"false");
		}
		else if (operand == CHARACTER_CLASS)
		{
			EventNode.addAttribute("operand", "characterClass");
			EventNode.addAttribute("characterID", QString::number(operandID).toUtf8().data());
			EventNode.addAttribute("characterReferencedInVariable", (operandReferencedInVariable) ? "true":"false");
		}
		else if (operand == CLASS_NAME)
		{
			EventNode.addAttribute("operand", "className");
			EventNode.addAttribute("classID", QString::number(operandID).toUtf8().data());
			EventNode.addAttribute("classReferencedInVariable", (operandReferencedInVariable) ? "true":"false");
		}
		else if (operand == TERRAIN_NAME)
		{
			EventNode.addAttribute("operand", "terrainName");
			EventNode.addAttribute("terrainID", QString::number(operandID).toUtf8().data());
			EventNode.addAttribute("terrainReferencedInVariable", (operandReferencedInVariable) ? "true":"false");
		}
		else if (operand == SKILL_NAME)
		{
			EventNode.addAttribute("operand", "skillName");
			EventNode.addAttribute("skillID", QString::number(operandID).toUtf8().data());
			EventNode.addAttribute("skillReferencedInVariable", (operandReferencedInVariable) ? "true":"false");
		}
		else if (operand == SKILL_DESCRIPTION)
		{
			EventNode.addAttribute("operand", "skillDescription");
			EventNode.addAttribute("skillID", QString::number(operandID).toUtf8().data());
			EventNode.addAttribute("skillReferencedInVariable", (operandReferencedInVariable) ? "true":"false");
		}
		else if (operand == ITEM_NAME)
		{
			EventNode.addAttribute("operand", "itemName");
			EventNode.addAttribute("itemID", QString::number(operandID).toUtf8().data());
			EventNode.addAttribute("itemReferencedInVariable", (operandReferencedInVariable) ? "true":"false");
		}
		else if (operand == ITEM_DESCRIPTION)
		{
			EventNode.addAttribute("operand", "itemDescription");
			EventNode.addAttribute("itemID", QString::number(operandID).toUtf8().data());
			EventNode.addAttribute("itemReferencedInVariable", (operandReferencedInVariable) ? "true":"false");
		}
		else if (operand == ATTRIBUTE_NAME)
		{
			EventNode.addAttribute("operand", "attributeName");
			EventNode.addAttribute("attributeID", QString::number(operandID).toUtf8().data());
			EventNode.addAttribute("attributeReferencedInVariable", (operandReferencedInVariable) ? "true":"false");
		}
		else if (operand == CONDITION_NAME)
		{
			EventNode.addAttribute("operand", "conditionName");
			EventNode.addAttribute("conditionID", QString::number(operandID).toUtf8().data());
			EventNode.addAttribute("conditionReferencedInVariable", (operandReferencedInVariable) ? "true":"false");
		}
		else if (operand == SYSTEM_STRING)
		{
			EventNode.addAttribute("operand", "systemString");
			EventNode.addAttribute("stringID", QString::number(operandID).toUtf8().data());
		}
		else if (operand == VEHICLE_NAME)
		{
			EventNode.addAttribute("operand", "vehicleName");
			EventNode.addAttribute("vehicleID", QString::number(operandID).toUtf8().data());
			EventNode.addAttribute("vehicleReferencedInVariable", (operandReferencedInVariable) ? "true":"false");
		}
		else if (operand == MONSTER_NAME)
		{
			EventNode.addAttribute("operand", "monsterName");
			EventNode.addAttribute("monsterID", QString::number(operandID).toUtf8().data());
			EventNode.addAttribute("monsterReferencedInVariable", (operandReferencedInVariable) ? "true":"false");
		}
		else if (operand == MONSTER_GROUP_NAME)
		{
			EventNode.addAttribute("operand", "monsterGroupName");
			EventNode.addAttribute("monsterGroupID", QString::number(operandID).toUtf8().data());
			EventNode.addAttribute("monsterGroupReferencedInVariable", (operandReferencedInVariable) ? "true":"false");
		}
	}
	return EventNode;
}

void BaseEvent::StringOperation::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putString(value1.toUtf8().data());
	storageFile.putString(value2.toUtf8().data());
	storageFile.putSignedInt(stringToChange);
	storageFile.putSignedInt(operation);
	storageFile.putSignedInt(operand);
	storageFile.putSignedInt(string1ID);
	storageFile.putSignedInt(string2ID);
	storageFile.putSignedInt(operandID);
	storageFile.putBool(operandReferencedInVariable);
}

QStringList BaseEvent::StringOperation::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	QString tempString;
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "String Operation: ";
	if (stringToChange == SINGLE)
		viewableText[0] += QString("string[%1]").arg(ProjectData::stringVariableIdToString(string1ID));
	else if (stringToChange == RANGE)
	{
		viewableText[0] += QString("string[%1]").arg(ProjectData::stringVariableIdToString(string1ID));
		viewableText[0] += " - ";
		viewableText[0] += QString("string[%1]").arg(ProjectData::stringVariableIdToString(string2ID));
	}
	if (operation == SET)
		viewableText[0] += ", Set, ";
	else if (operation == ADD)
		viewableText[0] += ", Add, ";
	else if (operation == REMOVE_FIRST_CHARACTER)
		viewableText[0] += ", Remove First Character";
	else if (operation == REMOVE_LAST_CHARACTER)
		viewableText[0] += ", Remove Last Character";
	if (operation == SET || operation == ADD)
	{
		if (operand == VALUE)
		{
			viewableText[0] += "\"";
			tempString = value1;
			tempString.replace("\"", "\\\"");
			viewableText[0] += tempString;
			viewableText[0] += "\"";
		}
		else if (operand == STRING)
		{
			viewableText[0] += "String ";
			if (operandReferencedInVariable)
				viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(operandID));
			else
				viewableText[0] += QString("string[%1]").arg(ProjectData::stringVariableIdToString(operandID));
		}
		else if (operand == VARIABLE)
		{
			if (operandReferencedInVariable)
				viewableText[0] += "Referenced Variable ";
			else
				viewableText[0] += "Variable ";
			viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(operandID));
		}
		else if (operand == SWITCH)
		{
			viewableText[0] += "Switch ";
			if (operandReferencedInVariable)
				viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(operandID));
			else
				viewableText[0] += QString("string[%1]").arg(ProjectData::switchVariableIdToString(operandID));
			viewableText[0] += " On=\"";
			viewableText[0] += value1;
			viewableText[0] += "\" Off=\"";
			viewableText[0] += value2;
			viewableText[0] += "\"";
		}
		else if (operand == PARTY_MEMBER_NAME)
		{
			viewableText[0] += "Party Member ";
			if (operandID < 4)
				viewableText[0] += QString::number(operandID);
			else
				viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(operandID - 4));
			viewableText[0] += "\'s Name";
		}
		else if (operand == PARTY_MEMBER_CLASS)
		{
			viewableText[0] += "Party Member ";
			if (operandID < 4)
				viewableText[0] += QString::number(operandID);
			else
				viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(operandID - 4));
			viewableText[0] += "\'s Class";
		}
		else if (operand == CHARACTER_NAME)
		{
			viewableText[0] += "Character ";
			if (operandReferencedInVariable)
				viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(operandID));
			else
				viewableText[0] += ProjectData::characterNames[operandID];
			viewableText[0] += "\'s Name";
		}
		else if (operand == CHARACTER_CLASS)
		{
			viewableText[0] += "Character ";
			if (operandReferencedInVariable)
				viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(operandID));
			else
				viewableText[0] += ProjectData::characterNames[operandID];
			viewableText[0] += "\'s Class";
		}
		else if (operand == CLASS_NAME)
		{
			viewableText[0] += "Class ";
			if (operandReferencedInVariable)
				viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(operandID));
			else
				viewableText[0] += ProjectData::classNames[operandID];
			viewableText[0] += "\'s Name";
		}
		else if (operand == TERRAIN_NAME)
		{
			viewableText[0] += "Terrain ";
			if (operandReferencedInVariable)
				viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(operandID));
			else
				viewableText[0] += ProjectData::terrainNames[operandID];
			viewableText[0] += "\'s Name";
		}
		else if (operand == SKILL_NAME)
		{
			viewableText[0] += "Skill ";
			if (operandReferencedInVariable)
				viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(operandID));
			else
				viewableText[0] += ProjectData::skillNames[operandID];
			viewableText[0] += "\'s Name";
		}
		else if (operand == SKILL_DESCRIPTION)
		{
			viewableText[0] += "Skill ";
			if (operandReferencedInVariable)
				viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(operandID));
			else
				viewableText[0] += ProjectData::skillNames[operandID];
			viewableText[0] += "\'s Description";
		}
		else if (operand == ITEM_NAME)
		{
			viewableText[0] += "Item ";
			if (operandReferencedInVariable)
				viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(operandID));
			else
				viewableText[0] += ProjectData::itemNames[operandID];
			viewableText[0] += "\'s Name";
		}
		else if (operand == ITEM_DESCRIPTION)
		{
			viewableText[0] += "Item ";
			if (operandReferencedInVariable)
				viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(operandID));
			else
				viewableText[0] += ProjectData::itemNames[operandID];
			viewableText[0] += "\'s Description";
		}
		else if (operand == ATTRIBUTE_NAME)
		{
			viewableText[0] += "Attribute ";
			if (operandReferencedInVariable)
				viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(operandID));
			else
				viewableText[0] += ProjectData::attributeNames[operandID];
			viewableText[0] += "\'s Name";
		}
		else if (operand == CONDITION_NAME)
		{
			viewableText[0] += "Condition ";
			if (operandReferencedInVariable)
				viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(operandID));
			else
				viewableText[0] += ProjectData::conditionNames[operandID];
			viewableText[0] += "\'s Name";
		}
		else if (operand == SYSTEM_STRING)
		{
			viewableText[0] += "System String (";
			if (operandID == ProjectData::STRING_BATTLEMSG_BEGIN)
				viewableText[0] += "Battle Message: Begin";
			else if (operandID == ProjectData::STRING_BATTLEMSG_INITIATIVE)
				viewableText[0] += "Battle Message: Initiative";
			else if (operandID == ProjectData::STRING_BATTLEMSG_MISS)
				viewableText[0] += "Battle Message: Miss";
			else if (operandID == ProjectData::STRING_BATTLEMSG_FAILESCAPE)
				viewableText[0] += "Battle Message: Failed Escape";
			else if (operandID == ProjectData::STRING_BATTLEMSG_VICTORY)
				viewableText[0] += "Battle Message: Victory";
			else if (operandID == ProjectData::STRING_BATTLEMSG_DEFEAT)
				viewableText[0] += "Battle Message: Defeat";
			else if (operandID == ProjectData::STRING_BATTLEMSG_EXPERIENCEGAIN)
				viewableText[0] += "Battle Message: Experience Gained";
			else if (operandID == ProjectData::STRING_BATTLEMSG_MONEYGAIN)
				viewableText[0] += "Battle Message: Money Gained";
			else if (operandID == ProjectData::STRING_BATTLEMSG_ITEMGAIN)
				viewableText[0] += "Battle Message: Item Gained";
			else if (operandID == ProjectData::STRING_LEVELUP)
				viewableText[0] += "Level Up";
			else if (operandID == ProjectData::STRING_SKILLLEARNED)
				viewableText[0] += "Skill Learned";
			else if (operandID == ProjectData::STRING_SHOPMSG_GREETING)
				viewableText[0] += "Shop Message: Greeting";
			else if (operandID == ProjectData::STRING_SHOPMSG_AFTERFIRSTTRANSACTION)
				viewableText[0] += "Shop Message: After First Transaction";
			else if (operandID == ProjectData::STRING_SHOPMSG_BUY)
				viewableText[0] += "Shop Message: Buy";
			else if (operandID == ProjectData::STRING_SHOPMSG_SELL)
				viewableText[0] += "Shop Message: Sell";
			else if (operandID == ProjectData::STRING_SHOPMSG_DONE)
				viewableText[0] += "Shop Message: Done";
			else if (operandID == ProjectData::STRING_SHOPMSG_BUYITEM)
				viewableText[0] += "Shop Message: Buy Item";
			else if (operandID == ProjectData::STRING_SHOPMSG_BUYHOWMANY)
				viewableText[0] += "Shop Message: Buy How Many";
			else if (operandID == ProjectData::STRING_SHOPMSG_BUYCOMPLETE)
				viewableText[0] += "Shop Message: Buy Complete";
			else if (operandID == ProjectData::STRING_SHOPMSG_SELLITEM)
				viewableText[0] += "Shop Message: Sell Item";
			else if (operandID == ProjectData::STRING_SHOPMSG_SELLHOWMANY)
				viewableText[0] += "Shop Message: Sell How Many";
			else if (operandID == ProjectData::STRING_SHOPMSG_SELLCOMPLETE)
				viewableText[0] += "Shop Message: Sell Complete";
			else if (operandID == ProjectData::STRING_INNMSG_PRICEINTRO)
				viewableText[0] += "Inn Message: Price Intro";
			else if (operandID == ProjectData::STRING_INNMSG_ACCEPTINQUIRY)
				viewableText[0] += "Inn Message: Accept Inquiry";
			else if (operandID == ProjectData::STRING_INNMSG_ACCEPT)
				viewableText[0] += "Inn Message: Accept";
			else if (operandID == ProjectData::STRING_INNMSG_DECLINE)
				viewableText[0] += "Inn Message: Decline";
			else if (operandID == ProjectData::STRING_OTHER_ITEMSPOSSESSED)
				viewableText[0] += "Other: Items Possessed";
			else if (operandID == ProjectData::STRING_OTHER_ITEMSEQUIPPED)
				viewableText[0] += "Other: Items Equipped";
			else if (operandID == ProjectData::STRING_OTHER_CURRENCY)
				viewableText[0] += "Other: Currency";
			else if (operandID == ProjectData::STRING_STATSANDEQUIP_LEVEL)
				viewableText[0] += "Stat: Level";
			else if (operandID == ProjectData::STRING_STATSANDEQUIP_LEVELSHORT)
				viewableText[0] += "Stat: Level short form";
			else if (operandID == ProjectData::STRING_STATSANDEQUIP_HP)
				viewableText[0] += "Stat: HP";
			else if (operandID == ProjectData::STRING_STATSANDEQUIP_HPSHORT)
				viewableText[0] += "Stat: HP short form";
			else if (operandID == ProjectData::STRING_STATSANDEQUIP_MP)
				viewableText[0] += "Stat: MP";
			else if (operandID == ProjectData::STRING_STATSANDEQUIP_MPSHORT)
				viewableText[0] += "Stat: MP short form";
			else if (operandID == ProjectData::STRING_STATSANDEQUIP_NORMALSTATUS)
				viewableText[0] += "Stat: Normal Status";
			else if (operandID == ProjectData::STRING_STATSANDEQUIP_EXPERIENCE)
				viewableText[0] += "Stat: Experience";
			else if (operandID == ProjectData::STRING_STATSANDEQUIP_ATTACK)
				viewableText[0] += "Stat: Attack";
			else if (operandID == ProjectData::STRING_STATSANDEQUIP_DEFENSE)
				viewableText[0] += "Stat: Defense";
			else if (operandID == ProjectData::STRING_STATSANDEQUIP_WISDOM)
				viewableText[0] += "Stat: Wisdom";
			else if (operandID == ProjectData::STRING_STATSANDEQUIP_AGILITY)
				viewableText[0] += "Stat: Agility";
			else if (operandID == ProjectData::STRING_STATSANDEQUIP_MPUSED)
				viewableText[0] += "Equipment: MP Used";
			else if (operandID == ProjectData::STRING_STATSANDEQUIP_WEAPON)
				viewableText[0] += "Equipment: Weapon";
			else if (operandID == ProjectData::STRING_STATSANDEQUIP_SHIELD)
				viewableText[0] += "Equipment: Shield";
			else if (operandID == ProjectData::STRING_STATSANDEQUIP_ARMOR)
				viewableText[0] += "Equipment: Armor";
			else if (operandID == ProjectData::STRING_STATSANDEQUIP_HELMET)
				viewableText[0] += "Equipment: Helmet";
			else if (operandID == ProjectData::STRING_STATSANDEQUIP_ACCESSORY)
				viewableText[0] += "Equipment: Accessory";
			else if (operandID == ProjectData::STRING_COMMAND_FIGHT)
				viewableText[0] += "Command: Fight";
			else if (operandID == ProjectData::STRING_COMMAND_AUTOFIGHT)
				viewableText[0] += "Command: Auto-Fight";
			else if (operandID == ProjectData::STRING_COMMAND_ESCAPE)
				viewableText[0] += "Command: Escape";
			else if (operandID == ProjectData::STRING_COMMAND_ITEM)
				viewableText[0] += "Command: Item";
			else if (operandID == ProjectData::STRING_COMMAND_SKILL)
				viewableText[0] += "Command: Skill";
			else if (operandID == ProjectData::STRING_COMMAND_EQUIP)
				viewableText[0] += "Command: Equip";
			else if (operandID == ProjectData::STRING_COMMAND_SAVE)
				viewableText[0] += "Command: Save";
			else if (operandID == ProjectData::STRING_COMMAND_QUIT)
				viewableText[0] += "Command: Quit";
			else if (operandID == ProjectData::STRING_COMMAND_STATUS)
				viewableText[0] += "Command: Status";
			else if (operandID == ProjectData::STRING_COMMAND_ROW)
				viewableText[0] += "Command: Row";
			else if (operandID == ProjectData::STRING_COMMAND_ORDER)
				viewableText[0] += "Command: Order";
			else if (operandID == ProjectData::STRING_COMMAND_NEWGAME)
				viewableText[0] += "Command: New Game";
			else if (operandID == ProjectData::STRING_COMMAND_LOADGAME)
				viewableText[0] += "Command: Load Game";
			else if (operandID == ProjectData::STRING_COMMAND_ENDGAME)
				viewableText[0] += "Command: End Game";
			else if (operandID == ProjectData::STRING_COMMAND_WAITON)
				viewableText[0] += "Command: Wait ON";
			else if (operandID == ProjectData::STRING_COMMAND_WAITOFF)
				viewableText[0] += "Command: Wait OFF";
			else if (operandID == ProjectData::STRING_SAVEQUITSCREENMSG_SAVESLOT)
				viewableText[0] += "Save/Quit: Save Slot";
			else if (operandID == ProjectData::STRING_SAVEQUITSCREENMSG_LOADSLOT)
				viewableText[0] += "Save/Quit: Load Slot";
			else if (operandID == ProjectData::STRING_SAVEQUITSCREENMSG_CONFIRMQUIT)
				viewableText[0] += "Save/Quit: Confirm Quit";
			else if (operandID == ProjectData::STRING_SAVEQUITSCREENMSG_YES)
				viewableText[0] += "Save/Quit: Yes";
			else if (operandID == ProjectData::STRING_SAVEQUITSCREENMSG_NO)
				viewableText[0] += "Save/Quit: No";
			else if (operandID == ProjectData::STRING_MISC_PARTYSMONEY)
				viewableText[0] += "Misc: Party\'s Money";
			else if (operandID == ProjectData::STRING_MISC_TOTALTIMEPLAYED)
				viewableText[0] += "Misc: Total Time Played";
			viewableText[0] += ")";
		}
		else if (operand == VEHICLE_NAME)
		{
			viewableText[0] += "Vehicle ";
			if (operandReferencedInVariable)
				viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(operandID));
			else
				viewableText[0] += ProjectData::vehicleNames[operandID];
			viewableText[0] += "\'s Name";
		}
		else if (operand == MONSTER_NAME)
		{
			viewableText[0] += "Monster ";
			if (operandReferencedInVariable)
				viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(operandID));
			else
				viewableText[0] += ProjectData::monsterNames[operandID];
			viewableText[0] += "\'s Name";
		}
		else if (operand == MONSTER_GROUP_NAME)
		{
			viewableText[0] += "Monster Group ";
			if (operandReferencedInVariable)
				viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(operandID));
			else
				viewableText[0] += ProjectData::monsterGroupNames[operandID];
			viewableText[0] += "\'s Name";
		}
	}
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::StringOperation::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	ConfigureEvent_StringOperation *configStringOperation = new ConfigureEvent_StringOperation;
	bool configured = false;
	configStringOperation->setStringToChange(stringToChange);
	configStringOperation->setStringIDs(string1ID, string2ID);
	configStringOperation->setOperation(operation);
	configStringOperation->setOperand(operand);
	configStringOperation->setIsReferencedOperand(operandReferencedInVariable);
	configStringOperation->setOperandValues(value1, value2, operandID);
	if (configStringOperation->exec())
	{
		configured = true;
		stringToChange = configStringOperation->getStringToChange();
		string1ID = configStringOperation->getString1ID();
		string2ID = configStringOperation->getString2ID();
		operation = configStringOperation->getOperation();
		operand = configStringOperation->getOperand();
		value1 = configStringOperation->getOperandValue1();
		value2 = configStringOperation->getOperandValue2();
		operandID = configStringOperation->getOperandID();
		operandReferencedInVariable = configStringOperation->isReferencedOperand();
	}
	delete configStringOperation;
	return configured;
}
#endif

bool BaseEvent::StringOperation::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	QStringList tempStringList;
	QStringList trueFalseList = QStringList() << "true" << "false";
	QString tempString;
	if (!XMLTools::attributeExists(EventNode, "stringToChange"))
		return false;
	tempStringList.clear();
	tempStringList << "single" << "range" << "referenced";
	if (!XMLTools::attributeStringValid(EventNode, "stringToChange", tempStringList))
		return false;
	tempString = EventNode.getAttribute("stringToChange");
	if (tempString == "single")
	{
		if (!XMLTools::attributeExists(EventNode, "stringID"))
			return false;
	}
	else if (tempString == "range")
	{
		if (!XMLTools::attributeExists(EventNode, "startStringID"))
			return false;
		if (!XMLTools::attributeExists(EventNode, "endStringID"))
			return false;
	}
	else if (tempString == "reference")
	{
		if (!XMLTools::attributeExists(EventNode, "variableID"))
			return false;
	}
	if (!XMLTools::attributeExists(EventNode, "operation"))
		return false;
	tempStringList.clear();
	tempStringList << "set" << "add" << "removeFirstCharacter" << "removeLastCharacter";
	if (!XMLTools::attributeStringValid(EventNode, "operation", tempStringList))
		return false;
	tempString = EventNode.getAttribute("operation");
	if (tempString == "set" || tempString == "add")
	{
		if (!XMLTools::attributeExists(EventNode, "operand"))
			return false;
		tempStringList.clear();
		tempStringList << "value" << "string" << "variable" << "switch" << "partyMemberName" << "partyMemberClass" << "characterName" << "characterClass" << "className" << "terrainName" << "skillName" << "itemName" << "itemDescription" << "attributeName" << "conditionName" << "systemString" << "vehicleName" << "monsterName" << "monsterGroupName";
		if (!XMLTools::attributeStringValid(EventNode, "operand", tempStringList))
			return false;
		tempString = EventNode.getAttribute("operand");
		if (tempString == "value")
		{
			if (!XMLTools::attributeExists(EventNode, "value"))
				return false;
		}
		else if (tempString == "string")
		{
			if (!XMLTools::attributeExists(EventNode, "stringID"))
				return false;
			if (!XMLTools::attributeExists(EventNode, "stringReferencedInVariable"))
				return false;
			if (!XMLTools::attributeStringValid(EventNode, "stringReferencedInVariable", trueFalseList, 1))
				return false;
		}
		else if (tempString == "systemString")
		{
			if (!XMLTools::attributeExists(EventNode, "stringID"))
				return false;
		}
		else if (tempString == "variable")
		{
			if (!XMLTools::attributeExists(EventNode, "variableID"))
				return false;
			if (!XMLTools::attributeExists(EventNode, "variableReferencedInVariable"))
				return false;
			if (!XMLTools::attributeStringValid(EventNode, "variableReferencedInVariable", trueFalseList, 1))
				return false;
		}
		else if (tempString == "switch")
		{
			if (!XMLTools::attributeExists(EventNode, "switchID"))
				return false;
			if (!XMLTools::attributeExists(EventNode, "switchReferencedInVariable"))
				return false;
			if (!XMLTools::attributeStringValid(EventNode, "switchReferencedInVariable", trueFalseList, 1))
				return false;
			if (!XMLTools::attributeExists(EventNode, "onText"))
				return false;
			if (!XMLTools::attributeExists(EventNode, "offText"))
				return false;
		}
		else if (tempString == "partyMemberName" || tempString == "partyMemberClass")
		{
			if (!XMLTools::attributeExists(EventNode, "partyMember"))
				return false;
		}
		else if (tempString == "characterName" || tempString == "characterClass")
		{
			if (!XMLTools::attributeExists(EventNode, "characterID"))
				return false;
			if (!XMLTools::attributeExists(EventNode, "characterReferencedInVariable"))
				return false;
			if (!XMLTools::attributeStringValid(EventNode, "characterReferencedInVariable", trueFalseList, 1))
				return false;
		}
		else if (tempString == "className")
		{
			if (!XMLTools::attributeExists(EventNode, "classID"))
				return false;
			if (!XMLTools::attributeExists(EventNode, "classReferencedInVariable"))
				return false;
			if (!XMLTools::attributeStringValid(EventNode, "classReferencedInVariable", trueFalseList, 1))
				return false;
		}
		else if (tempString == "terrainName")
		{
			if (!XMLTools::attributeExists(EventNode, "terrainID"))
				return false;
			if (!XMLTools::attributeExists(EventNode, "terrainReferencedInVariable"))
				return false;
			if (!XMLTools::attributeStringValid(EventNode, "terrainReferencedInVariable", trueFalseList, 1))
				return false;
		}
		else if (tempString == "skillName" || tempString == "skillDescription")
		{
			if (!XMLTools::attributeExists(EventNode, "skillID"))
				return false;
			if (!XMLTools::attributeExists(EventNode, "skillReferencedInVariable"))
				return false;
			if (!XMLTools::attributeStringValid(EventNode, "skillReferencedInVariable", trueFalseList, 1))
				return false;
		}
		else if (tempString == "itemName" || tempString == "itemDescription")
		{
			if (!XMLTools::attributeExists(EventNode, "itemID"))
				return false;
			if (!XMLTools::attributeExists(EventNode, "itemReferencedInVariable"))
				return false;
			if (!XMLTools::attributeStringValid(EventNode, "itemReferencedInVariable", trueFalseList, 1))
				return false;
		}
		else if (tempString == "attributeName")
		{
			if (!XMLTools::attributeExists(EventNode, "attributeID"))
				return false;
			if (!XMLTools::attributeExists(EventNode, "attributeReferencedInVariable"))
				return false;
			if (!XMLTools::attributeStringValid(EventNode, "attributeReferencedInVariable", trueFalseList, 1))
				return false;
		}
		else if (tempString == "conditionName")
		{
			if (!XMLTools::attributeExists(EventNode, "conditionID"))
				return false;
			if (!XMLTools::attributeExists(EventNode, "conditionReferencedInVariable"))
				return false;
			if (!XMLTools::attributeStringValid(EventNode, "conditionReferencedInVariable", trueFalseList, 1))
				return false;
		}
		else if (tempString == "vehicleName")
		{
			if (!XMLTools::attributeExists(EventNode, "vehicleID"))
				return false;
			if (!XMLTools::attributeExists(EventNode, "vehicleReferencedInVariable"))
				return false;
			if (!XMLTools::attributeStringValid(EventNode, "vehicleReferencedInVariable", trueFalseList, 1))
				return false;
		}
		else if (tempString == "monsterName")
		{
			if (!XMLTools::attributeExists(EventNode, "monsterID"))
				return false;
			if (!XMLTools::attributeExists(EventNode, "monsterReferencedInVariable"))
				return false;
			if (!XMLTools::attributeStringValid(EventNode, "monsterReferencedInVariable", trueFalseList, 1))
				return false;
		}
		else if (tempString == "monsterGroupName")
		{
			if (!XMLTools::attributeExists(EventNode, "monsterGroupID"))
				return false;
			if (!XMLTools::attributeExists(EventNode, "monsterGroupReferencedInVariable"))
				return false;
			if (!XMLTools::attributeStringValid(EventNode, "monsterGroupReferencedInVariable", trueFalseList, 1))
				return false;
		}
	}
	return true;
}

BaseEvent::StringOperation::StringOperation(BaseEvent *pBase) : Event(pBase)
{
	stringToChange = SINGLE;
	operation = SET;
	operand = VALUE;
	string1ID = 0;
	string2ID = 0;
	value1 = "";
	value2 = "";
	operandID = 0;
	operandReferencedInVariable = false;
}

BaseEvent::StringOperation::StringOperation(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	QString tempString;
	tempString = EventNode.getAttribute("stringToChange");
	if (tempString == "single")
	{
		stringToChange = SINGLE;
		string1ID = QString(EventNode.getAttribute("stringID")).toInt();
		string2ID = 0;
	}
	else if (tempString == "range")
	{
		stringToChange = RANGE;
		string1ID = QString(EventNode.getAttribute("startStringID")).toInt();
		string2ID = QString(EventNode.getAttribute("endStringID")).toInt();
	}
	else if (tempString == "reference")
	{
		stringToChange = REFERENCE;
		string1ID = QString(EventNode.getAttribute("variableID")).toInt();
		string2ID = 0;
	}
	tempString = EventNode.getAttribute("operation");
	if (tempString == "set")
		operation = SET;
	else if (tempString == "add")
		operation = ADD;
	else if (tempString == "removeFirstCharacter")
		operation = REMOVE_FIRST_CHARACTER;
	else if (tempString == "removeLastCharacter")
		operation = REMOVE_LAST_CHARACTER;
	if (operation == SET || operation == ADD)
	{
		tempString = EventNode.getAttribute("operand");
		if (tempString == "value")
		{
			operand = VALUE;
			value1 = EventNode.getAttribute("value");
			value2 = "";
			operandID = 0;
			operandReferencedInVariable = false;
		}
		else if (tempString == "string")
		{
			operand = STRING;
			value1 = "";
			value2 = "";
			operandID = QString(EventNode.getAttribute("stringID")).toInt();
			operandReferencedInVariable = (QString(EventNode.getAttribute("stringReferencedInVariable")) == "true") ? true:false;
		}
		else if (tempString == "variable")
		{
			operand = VARIABLE;
			value1 = "";
			value2 = "";
			operandID = QString(EventNode.getAttribute("variableID")).toInt();
			operandReferencedInVariable = (QString(EventNode.getAttribute("variableReferencedInVariable")) == "true") ? true:false;
		}
		else if (tempString == "switch")
		{
			operand = SWITCH;
			value1 = EventNode.getAttribute("onText");
			value2 = EventNode.getAttribute("offText");
			operandID = QString(EventNode.getAttribute("switchID")).toInt();
			operandReferencedInVariable = (QString(EventNode.getAttribute("switchReferencedInVariable")) == "true") ? true:false;
		}
		else if (tempString == "partyMemberName")
		{
			operand = PARTY_MEMBER_NAME;
			value1 = "";
			value2 = "";
			operandID = QString(EventNode.getAttribute("partyMember")).toInt();
			operandReferencedInVariable = false;
		}
		else if (tempString == "partyMemberClass")
		{
			operand = PARTY_MEMBER_CLASS;
			value1 = "";
			value2 = "";
			operandID = QString(EventNode.getAttribute("partyMember")).toInt();
			operandReferencedInVariable = false;
		}
		else if (tempString == "characterName")
		{
			operand = CHARACTER_NAME;
			value1 = "";
			value2 = "";
			operandID = QString(EventNode.getAttribute("characterID")).toInt();
			operandReferencedInVariable = (QString(EventNode.getAttribute("characterReferencedInVariable")) == "true") ? true:false;
		}
		else if (tempString == "characterClass")
		{
			operand = CHARACTER_CLASS;
			value1 = "";
			value2 = "";
			operandID = QString(EventNode.getAttribute("characterID")).toInt();
			operandReferencedInVariable = (QString(EventNode.getAttribute("characterReferencedInVariable")) == "true") ? true:false;
		}
		else if (tempString == "className")
		{
			operand = CLASS_NAME;
			value1 = "";
			value2 = "";
			operandID = QString(EventNode.getAttribute("classID")).toInt();
			operandReferencedInVariable = (QString(EventNode.getAttribute("classReferencedInVariable")) == "true") ? true:false;
		}
		else if (tempString == "terrainName")
		{
			operand = TERRAIN_NAME;
			value1 = "";
			value2 = "";
			operandID = QString(EventNode.getAttribute("terrainID")).toInt();
			operandReferencedInVariable = (QString(EventNode.getAttribute("terrainReferencedInVariable")) == "true") ? true:false;
		}
		else if (tempString == "skillName")
		{
			operand = SKILL_NAME;
			value1 = "";
			value2 = "";
			operandID = QString(EventNode.getAttribute("skillID")).toInt();
			operandReferencedInVariable = (QString(EventNode.getAttribute("skillReferencedInVariable")) == "true") ? true:false;
		}
		else if (tempString == "skillDescription")
		{
			operand = SKILL_DESCRIPTION;
			value1 = "";
			value2 = "";
			operandID = QString(EventNode.getAttribute("skillID")).toInt();
			operandReferencedInVariable = (QString(EventNode.getAttribute("skillReferencedInVariable")) == "true") ? true:false;
		}
		else if (tempString == "itemName")
		{
			operand = ITEM_NAME;
			value1 = "";
			value2 = "";
			operandID = QString(EventNode.getAttribute("itemID")).toInt();
			operandReferencedInVariable = (QString(EventNode.getAttribute("itemReferencedInVariable")) == "true") ? true:false;
		}
		else if (tempString == "itemDescription")
		{
			operand = ITEM_DESCRIPTION;
			value1 = "";
			value2 = "";
			operandID = QString(EventNode.getAttribute("itemID")).toInt();
			operandReferencedInVariable = (QString(EventNode.getAttribute("itemReferencedInVariable")) == "true") ? true:false;
		}
		else if (tempString == "attributeName")
		{
			operand = ATTRIBUTE_NAME;
			value1 = "";
			value2 = "";
			operandID = QString(EventNode.getAttribute("attributeID")).toInt();
			operandReferencedInVariable = (QString(EventNode.getAttribute("attributeReferencedInVariable")) == "true") ? true:false;
		}
		else if (tempString == "conditionName")
		{
			operand = CONDITION_NAME;
			value1 = "";
			value2 = "";
			operandID = QString(EventNode.getAttribute("conditionID")).toInt();
			operandReferencedInVariable = (QString(EventNode.getAttribute("conditionReferencedInVariable")) == "true") ? true:false;
		}
		else if (tempString == "systemString")
		{
			operand = SYSTEM_STRING;
			value1 = "";
			value2 = "";
			operandID = QString(EventNode.getAttribute("stringID")).toInt();
			operandReferencedInVariable = false;
		}
		else if (tempString == "vehicleName")
		{
			operand = VEHICLE_NAME;
			value1 = "";
			value2 = "";
			operandID = QString(EventNode.getAttribute("vehicleID")).toInt();
			operandReferencedInVariable = (QString(EventNode.getAttribute("vehicleReferencedInVariable")) == "true") ? true:false;
		}
		else if (tempString == "monsterName")
		{
			operand = MONSTER_NAME;
			value1 = "";
			value2 = "";
			operandID = QString(EventNode.getAttribute("monsterID")).toInt();
			operandReferencedInVariable = (QString(EventNode.getAttribute("monsterReferencedInVariable")) == "true") ? true:false;
		}
		else if (tempString == "monsterGroupName")
		{
			operand = MONSTER_GROUP_NAME;
			value1 = "";
			value2 = "";
			operandID = QString(EventNode.getAttribute("monsterGroupID")).toInt();
			operandReferencedInVariable = (QString(EventNode.getAttribute("monsterGroupReferencedInVariable")) == "true") ? true:false;
		}
	}
	else
	{
		operand = VALUE;
		value1 = "";
		value2 = "";
		operandID = 0;
		operandReferencedInVariable = false;
	}
}

BaseEvent::StringOperation::StringOperation(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	char *temp_string;
	temp_string = storageFile.getString();
	value1 = temp_string;
	delete[] temp_string;
	temp_string = storageFile.getString();
	value2 = temp_string;
	stringToChange = storageFile.getSignedInt();
	operation = storageFile.getSignedInt();
	operand = storageFile.getSignedInt();
	string1ID = storageFile.getSignedInt();
	string2ID = storageFile.getSignedInt();
	operandID = storageFile.getSignedInt();
	operandReferencedInVariable = storageFile.getBool();
}

BaseEvent::StringOperation::StringOperation(StringOperation *other, BaseEvent *pBase) : Event(pBase)
{
	value1 = other->value1;
	value2 = other->value2;
	stringToChange = other->stringToChange;
	operation = other->operation;
	operand = other->operand;
	string1ID = other->string1ID;
	string2ID = other->string2ID;
	operandID = other->operandID;
	operandReferencedInVariable = other->operandReferencedInVariable;
}
//END BaseEvent::StringOperation Member Functions

//BEGIN BaseEvent::SwapTile Member Functions
XMLNode BaseEvent::SwapTile::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	EventNode.addAttribute("x", QString::number(x).toUtf8().data());
	EventNode.addAttribute("y", QString::number(y).toUtf8().data());
	EventNode.addAttribute("layer", QString::number(layer).toUtf8().data());
	EventNode.addAttribute("locationStoredInVariables", (locationStoredInVariables) ? "true":"false");
	EventNode.addAttribute("tilesetID", QString::number(tilesetID).toUtf8().data());
	EventNode.addAttribute("tileID", QString::number(tileID).toUtf8().data());
	EventNode.addAttribute("tileReferencedInVariables", (tileReferencedInVariables) ? "true":"false");
	return EventNode;
}

void BaseEvent::SwapTile::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(x);
	storageFile.putSignedInt(y);
	storageFile.putSignedInt(layer);
	storageFile.putSignedInt(tilesetID);
	storageFile.putSignedInt(tileID);
	storageFile.putBool(locationStoredInVariables);
	storageFile.putBool(tileReferencedInVariables);
}

QStringList BaseEvent::SwapTile::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Swap Tile: location=";
	if (locationStoredInVariables)
		viewableText[0] += QString("(variable[%1], variable[%2]) layer=variable[%3] ").arg(ProjectData::numberVariableIdToString(x)).arg(ProjectData::numberVariableIdToString(y)).arg(ProjectData::numberVariableIdToString(layer));
	else
		viewableText[0] += QString("(%1, %2) layer=%3 ").arg(x).arg(y).arg(layer);
	if (tileReferencedInVariables)
	{
		viewableText[0] += QString("tileset=variable[%1] ").arg(ProjectData::numberVariableIdToString(tilesetID));
		viewableText[0] += QString("tile=variable[%1]").arg(ProjectData::numberVariableIdToString(tileID));
	}
	else
		viewableText[0] += QString("tileset=%1 tile=%2").arg(tilesetID).arg(tileID);
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::SwapTile::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerType);
	ConfigureEvent_SwapTile *configEvent = new ConfigureEvent_SwapTile(callerID);
	bool configured = false;
	configEvent->setLocationStoredInVariables(locationStoredInVariables);
	configEvent->setTileReferencedInVariables(tileReferencedInVariables);
	configEvent->setXLocation(x);
	configEvent->setYLocation(y);
	configEvent->setLayer(layer);
	configEvent->setTile(tilesetID, tileID);
	if (configEvent->exec())
	{
		configured = true;
		x = configEvent->getXLocation();
		y = configEvent->getYLocation();
		layer = configEvent->getLayer();
		tilesetID = configEvent->getTilesetID();
		tileID = configEvent->getTileID();
		locationStoredInVariables = configEvent->isLocationStoredInVariables();
		tileReferencedInVariables = configEvent->isTileReferencedInVariables();
	}
	delete configEvent;
	return configured;
}
#endif

void BaseEvent::SwapTile::applySwappedTiles(QList<Tileset::SwapTileIDs> swappedTiles)
{
	if (!tileReferencedInVariables)
	{
		for (int i = 0; i < swappedTiles.size(); ++i)
		{
			if (tilesetID == swappedTiles[i].tileset)
			{
				if (tileID == swappedTiles[i].tile1)
					tileID = swappedTiles[i].tile2;
				else if (tileID == swappedTiles[i].tile2)
					tileID = swappedTiles[i].tile1;
			}
		}
	}
}

bool BaseEvent::SwapTile::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	if (!XMLTools::attributeExists(EventNode, "x"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "y"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "layer"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "locationStoredInVariables"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "locationStoredInVariables", QStringList() << "true" << "false", 1))
		return false;
	if (!XMLTools::attributeExists(EventNode, "tilesetID"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "tileID"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "tileReferencedInVariables"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "tileReferencedInVariables", QStringList() << "true" << "false", 1))
		return false;
	return true;
}

BaseEvent::SwapTile::SwapTile(BaseEvent *pBase) : Event(pBase)
{
	x = 0;
	y = 0;
	layer = 0;
	tilesetID = 0;
	tileID = 0;
	locationStoredInVariables = false;
	tileReferencedInVariables = false;
}

BaseEvent::SwapTile::SwapTile(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	QString tempString;
	x = QString(EventNode.getAttribute("x")).toInt();
	y = QString(EventNode.getAttribute("y")).toInt();
	layer = QString(EventNode.getAttribute("layer")).toInt();
	locationStoredInVariables = (QString(EventNode.getAttribute("locationStoredInVariables")) == "true") ? true:false;
	tilesetID = QString(EventNode.getAttribute("tilesetID")).toInt();
	tileID = QString(EventNode.getAttribute("tileID")).toInt();
	tileReferencedInVariables = (QString(EventNode.getAttribute("tileReferencedInVariables")) == "true") ? true:false;
}

BaseEvent::SwapTile::SwapTile(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	x = storageFile.getSignedInt();
	y = storageFile.getSignedInt();
	layer = storageFile.getSignedInt();
	tilesetID = storageFile.getSignedInt();
	tileID = storageFile.getSignedInt();
	locationStoredInVariables = storageFile.getBool();
	tileReferencedInVariables = storageFile.getBool();
}

BaseEvent::SwapTile::SwapTile(SwapTile *other, BaseEvent *pBase) : Event(pBase)
{
	x = other->x;
	y = other->y;
	layer = other->layer;
	tilesetID = other->tilesetID;
	tileID = other->tileID;
	locationStoredInVariables = other->locationStoredInVariables;
	tileReferencedInVariables = other->tileReferencedInVariables;
}
//END BaseEvent::SwapTile Member Functions

//BEGIN BaseEvent::SwitchOperation Member Functions
XMLNode BaseEvent::SwitchOperation::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	if (switchToChange == SINGLE)
	{
		EventNode.addAttribute("switchToChange", "single");
		EventNode.addAttribute("switchID", QString::number(switch1ID).toUtf8().data());
	}
	else if (switchToChange == RANGE)
	{
		EventNode.addAttribute("switchToChange", "range");
		EventNode.addAttribute("startSwitchID", QString::number(switch1ID).toUtf8().data());
		EventNode.addAttribute("endSwitchID", QString::number(switch2ID).toUtf8().data());
	}
	else if (switchToChange == REFERENCE)
	{
		EventNode.addAttribute("switchToChange", "referenced");
		EventNode.addAttribute("variableID", QString::number(switch1ID).toUtf8().data());
	}
	if (operation == TURN_ON)
		EventNode.addAttribute("operation", "turnOn");
	else if (operation == TURN_OFF)
		EventNode.addAttribute("operation", "turnOff");
	else if (operation == TOGGLE)
		EventNode.addAttribute("operation", "toggle");
	return EventNode;
}

void BaseEvent::SwitchOperation::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(switchToChange);
	storageFile.putSignedInt(operation);
	storageFile.putSignedInt(switch1ID);
	storageFile.putSignedInt(switch2ID);
}

QStringList BaseEvent::SwitchOperation::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Switch Operation: ";
	if (operation == TURN_ON)
		viewableText[0] += "Turn On ";
	else if (operation == TURN_OFF)
		viewableText[0] += "Turn Off ";
	else
		viewableText[0] += "Toggle ";
	if (switchToChange == SINGLE)
		viewableText[0] += QString("string[%1]").arg(ProjectData::switchVariableIdToString(switch1ID));
	else if (switchToChange == RANGE)
	{
		viewableText[0] += QString("string[%1]").arg(ProjectData::switchVariableIdToString(switch1ID));
		viewableText[0] += " - ";
		viewableText[0] += QString("string[%1]").arg(ProjectData::switchVariableIdToString(switch2ID));
	}
	else
		viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(switch1ID));
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::SwitchOperation::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	ConfigureEvent_SwitchOperation *configSwitchOperation = new ConfigureEvent_SwitchOperation;
	bool configured = false;
	configSwitchOperation->setSwitchToChange(switchToChange);
	configSwitchOperation->setSwitchIDs(switch1ID, switch2ID);
	configSwitchOperation->setOperation(operation);
	if (configSwitchOperation->exec())
	{
		configured = true;
		switchToChange = configSwitchOperation->getSwitchToChange();
		switch1ID = configSwitchOperation->getSwitch1ID();
		switch2ID = configSwitchOperation->getSwitch2ID();
		operation = configSwitchOperation->getOperation();
	}
	delete configSwitchOperation;
	return configured;
}
#endif

bool BaseEvent::SwitchOperation::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	QStringList tempStringList;
	QString tempString;
	tempStringList.clear();
	tempStringList << "single" << "range" << "referenced";
	if (!XMLTools::attributeExists(EventNode, "switchToChange"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "switchToChange", tempStringList))
		return false;
	tempString = EventNode.getAttribute("switchToChange");
	if (tempString == "single")
	{
		if (!XMLTools::attributeExists(EventNode, "switchID"))
			return false;
	}
	else if (tempString == "range")
	{
		if (!XMLTools::attributeExists(EventNode, "startSwitchID"))
			return false;
		if (!XMLTools::attributeExists(EventNode, "endSwitchID"))
			return false;
	}
	else if (tempString == "referenced")
	{
		if (!XMLTools::attributeExists(EventNode, "variableID"))
			return false;
	}
	tempStringList.clear();
	tempStringList << "turnOn" << "turnOff" << "toggle";
	if (!XMLTools::attributeExists(EventNode, "operation"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "operation", tempStringList))
		return false;
	return true;
}

BaseEvent::SwitchOperation::SwitchOperation(BaseEvent *pBase) : Event(pBase)
{
	switchToChange = SINGLE;
	operation = TURN_ON;
	switch1ID = 0;
	switch2ID = 0;
}

BaseEvent::SwitchOperation::SwitchOperation(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	QString tempString;
	tempString = EventNode.getAttribute("switchToChange");
	if (tempString == "single")
	{
		switchToChange = SINGLE;
		switch1ID = QString(EventNode.getAttribute("switchID")).toInt();
		switch2ID = 0;
	}
	else if (tempString == "range")
	{
		switchToChange = RANGE;
		switch1ID = QString(EventNode.getAttribute("startSwitchID")).toInt();
		switch2ID = QString(EventNode.getAttribute("endSwitchID")).toInt();
	}
	else if (tempString == "referenced")
	{
		switchToChange = REFERENCE;
		switch1ID = QString(EventNode.getAttribute("variableID")).toInt();
		switch2ID = 0;
	}
	tempString = EventNode.getAttribute("operation");
	if (tempString == "turnOn")
		operation = TURN_ON;
	else if (tempString == "turnOff")
		operation = TURN_OFF;
	else if (tempString == "toggle")
		operation = TOGGLE;
}

BaseEvent::SwitchOperation::SwitchOperation(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	switchToChange = storageFile.getSignedInt();
	operation = storageFile.getSignedInt();
	switch1ID = storageFile.getSignedInt();
	switch2ID = storageFile.getSignedInt();
}

BaseEvent::SwitchOperation::SwitchOperation(SwitchOperation *other, BaseEvent *pBase) : Event(pBase)
{
	switchToChange = other->switchToChange;
	operation = other->operation;
	switch1ID = other->switch1ID;
	switch2ID = other->switch2ID;
}
//END BaseEvent::SwitchOperation Member Functions

//BEGIN BaseEvent::Teleport Member Functions
XMLNode BaseEvent::Teleport::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	EventNode.addAttribute("mapID", QString::number(mapID).toUtf8().data());
	EventNode.addAttribute("x", QString::number(x).toUtf8().data());
	EventNode.addAttribute("y", QString::number(y).toUtf8().data());
	EventNode.addAttribute("layer", QString::number(layer).toUtf8().data());
	EventNode.addAttribute("coordinatesStoredInVariables", (coordinatesStoredInVariables) ? "true":"false");
	if (facing == RETAIN_CURRENT_FACING)
		EventNode.addAttribute("facing", "retainCurrent");
	else if (facing == FACE_UP)
		EventNode.addAttribute("facing", "faceUp");
	else if (facing == FACE_RIGHT)
		EventNode.addAttribute("facing", "right");
	else if (facing == FACE_DOWN)
		EventNode.addAttribute("facing", "down");
	else if (facing == FACE_LEFT)
		EventNode.addAttribute("facing", "left");
	return EventNode;
}

void BaseEvent::Teleport::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(mapID);
	storageFile.putSignedInt(x);
	storageFile.putSignedInt(y);
	storageFile.putSignedInt(layer);
	storageFile.putSignedInt(facing);
	storageFile.putBool(coordinatesStoredInVariables);
}

QStringList BaseEvent::Teleport::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Teleport: ";
	if (coordinatesStoredInVariables)
	{
		viewableText[0] += QString("map=variable[%1]").arg(ProjectData::numberVariableIdToString(mapID));
		viewableText[0] += " (";
		viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(x));
		viewableText[0] += ", ";
		viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(y));
		viewableText[0] += ") ";
		viewableText[0] += QString("layer=variable[%1]").arg(ProjectData::numberVariableIdToString(layer));
	}
	else
	{
		viewableText[0] += QString("map=\"%1\"").arg(ProjectData::mapNames[mapID]);
		viewableText[0] += QString(" (%1, %2) layer=%3 ").arg(x).arg(y).arg(layer);
	}
	if (facing == RETAIN_CURRENT_FACING)
		viewableText[0] += "Retain Facing";
	else if (facing == FACE_UP)
		viewableText[0] += "Face Up";
	else if (facing == FACE_RIGHT)
		viewableText[0] += "Face Right";
	else if (facing == FACE_DOWN)
		viewableText[0] += "Face Down";
	else if (facing == FACE_LEFT)
		viewableText[0] += "Face Left";
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::Teleport::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	ConfigureEvent_Teleport *configTeleport = new ConfigureEvent_Teleport;
	bool configured = false;
	configTeleport->setIsStoredLocation(coordinatesStoredInVariables);
	configTeleport->setLocation(mapID, x, y, layer);
	configTeleport->setFacing(facing);
	if (configTeleport->exec())
	{
		configured = true;
		mapID = configTeleport->getMapID();
		x = configTeleport->getX();
		y = configTeleport->getY();
		layer = configTeleport->getLayer();
		facing = configTeleport->getFacing();
		coordinatesStoredInVariables = configTeleport->isStoredLocation();
	}
	delete configTeleport;
	return configured;
}
#endif

bool BaseEvent::Teleport::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	QStringList tempStringList = QStringList() << "true" << "false";
	if (!XMLTools::attributeExists(EventNode, "mapID"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "x"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "y"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "layer"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "coordinatesStoredInVariables"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "coordinatesStoredInVariables", tempStringList, 1))
		return false;
	tempStringList.clear();
	tempStringList << "retainCurrent" << "faceUp" << "faceRight" << "faceDown" << "faceLeft";
	if (!XMLTools::attributeExists(EventNode, "facing"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "facing", tempStringList))
		return false;
	return true;
}

BaseEvent::Teleport::Teleport(BaseEvent *pBase) : Event(pBase)
{
	mapID = 0;
	x = 0;
	y = 0;
	layer = 0;
	facing = RETAIN_CURRENT_FACING;
	coordinatesStoredInVariables = false;
}

BaseEvent::Teleport::Teleport(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	QString tempString;
	mapID = QString(EventNode.getAttribute("mapID")).toInt();
	x = QString(EventNode.getAttribute("x")).toInt();
	y = QString(EventNode.getAttribute("y")).toInt();
	layer = QString(EventNode.getAttribute("layer")).toInt();
	tempString = EventNode.getAttribute("coordinatesStoredInVariables");
	coordinatesStoredInVariables = (tempString == "true") ? true:false;
	tempString = EventNode.getAttribute("facing");
	if (tempString == "retainCurrent")
		facing = RETAIN_CURRENT_FACING;
	else if (tempString == "faceUp")
		facing = FACE_UP;
	else if (tempString == "faceRight")
		facing = FACE_RIGHT;
	else if (tempString == "faceDown")
		facing = FACE_DOWN;
	else if (tempString == "faceLeft")
		facing = FACE_LEFT;
}

BaseEvent::Teleport::Teleport(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	mapID = storageFile.getSignedInt();
	x = storageFile.getSignedInt();
	y = storageFile.getSignedInt();
	layer = storageFile.getSignedInt();
	facing = storageFile.getSignedInt();
	coordinatesStoredInVariables = storageFile.getBool();
}

BaseEvent::Teleport::Teleport(Teleport *other, BaseEvent *pBase) : Event(pBase)
{
	mapID = other->mapID;
	x = other->x;
	y = other->y;
	layer = other->layer;
	facing = other->facing;
	coordinatesStoredInVariables = other->coordinatesStoredInVariables;
}
//END BaseEvent::Teleport Member Functions

//BEGIN BaseEvent::TileSubstitution Member Functions
XMLNode BaseEvent::TileSubstitution::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	EventNode.addAttribute("tileset1ID", QString::number(tileset1ID).toUtf8().data());
	EventNode.addAttribute("tile1ID", QString::number(tile1ID).toUtf8().data());
	EventNode.addAttribute("tile1ReferencedInVariables", (tile1ReferencedInVariables) ? "true":"false");
	EventNode.addAttribute("tileset2ID", QString::number(tileset2ID).toUtf8().data());
	EventNode.addAttribute("tile2ID", QString::number(tile2ID).toUtf8().data());
	EventNode.addAttribute("tile2ReferencedInVariables", (tile2ReferencedInVariables) ? "true":"false");
	if (layer == ALL)
		EventNode.addAttribute("layer", "all");
	else if (layer == LOWER)
		EventNode.addAttribute("layer", "lower");
	else if (layer == HERO)
		EventNode.addAttribute("layer", "hero");
	else if (layer == UPPER)
		EventNode.addAttribute("layer", "upper");
	return EventNode;
}

void BaseEvent::TileSubstitution::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(tileset1ID);
	storageFile.putSignedInt(tileset2ID);
	storageFile.putSignedInt(tile1ID);
	storageFile.putSignedInt(tile2ID);
	storageFile.putSignedInt(layer);
	storageFile.putBool(tile1ReferencedInVariables);
	storageFile.putBool(tile2ReferencedInVariables);
}

QStringList BaseEvent::TileSubstitution::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Tile Substitution: ";
	if (tile1ReferencedInVariables)
	{
		viewableText[0] += QString("tileset1=variable[%1] ").arg(ProjectData::numberVariableIdToString(tileset1ID));
		viewableText[0] += QString("tile1=variable[%1] ").arg(ProjectData::numberVariableIdToString(tile1ID));
	}
	else
		viewableText[0] += QString("tileset1=%1 tile1=%2 ").arg(tileset1ID).arg(tile1ID);
	if (tile2ReferencedInVariables)
	{
		viewableText[0] += QString("tileset2=variable[%1] ").arg(ProjectData::numberVariableIdToString(tileset2ID));
		viewableText[0] += QString("tile2=variable[%1] ").arg(ProjectData::numberVariableIdToString(tile2ID));
	}
	else
		viewableText[0] += QString("tileset2=%1 tile2=%2 ").arg(tileset2ID).arg(tile2ID);
	if (layer == ALL)
		viewableText[0] += "(All Layers)";
	else if (layer == LOWER)
		viewableText[0] += "(Lower Layer)";
	else if (layer == HERO)
		viewableText[0] += "(Hero Layer)";
	else if (layer == UPPER)
		viewableText[0] += "(Upper Layer)";
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::TileSubstitution::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	ConfigureEvent_TileSubstitution *configEvent = new ConfigureEvent_TileSubstitution;
	bool configured = false;
	configEvent->setTile1ReferencedInVariables(tile1ReferencedInVariables);
	configEvent->setTile2ReferencedInVariables(tile2ReferencedInVariables);
	configEvent->setTile1(tileset1ID, tile1ID);
	configEvent->setTile2(tileset2ID, tile2ID);
	configEvent->setLayer(layer);
	if (configEvent->exec())
	{
		configured = true;
		tileset1ID = configEvent->getTileset1ID();
		tile1ID = configEvent->getTile1ID();
		tileset2ID = configEvent->getTileset2ID();
		tile2ID = configEvent->getTile2ID();
		layer = configEvent->getLayer();
		tile1ReferencedInVariables = configEvent->isTile1ReferencedInVariables();
		tile2ReferencedInVariables = configEvent->isTile2ReferencedInVariables();
	}
	delete configEvent;
	return configured;
}
#endif

void BaseEvent::TileSubstitution::applySwappedTiles(QList<Tileset::SwapTileIDs> swappedTiles)
{
	for (int i = 0; i < swappedTiles.size(); ++i)
	{
		if (!tile1ReferencedInVariables && tileset1ID == swappedTiles[i].tileset)
		{
			if (tile1ID == swappedTiles[i].tile1)
				tile1ID = swappedTiles[i].tile2;
			else if (tile1ID == swappedTiles[i].tile2)
				tile1ID = swappedTiles[i].tile1;
		}
		if (!tile2ReferencedInVariables && tileset2ID == swappedTiles[i].tileset)
		{
			if (tile2ID == swappedTiles[i].tile1)
				tile2ID = swappedTiles[i].tile2;
			else if (tile2ID == swappedTiles[i].tile2)
				tile2ID = swappedTiles[i].tile1;
		}
	}
}

bool BaseEvent::TileSubstitution::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	if (!XMLTools::attributeExists(EventNode, "tileset1ID"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "tile1ID"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "tile1ReferencedInVariables"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "tile1ReferencedInVariables", QStringList() << "true" << "false", 1))
		return false;
	if (!XMLTools::attributeExists(EventNode, "tileset2ID"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "tile2ID"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "tile2ReferencedInVariables"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "tile2ReferencedInVariables", QStringList() << "true" << "false", 1))
		return false;
	if (!XMLTools::attributeExists(EventNode, "layer"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "layer", QStringList() << "all" << "lower" << "hero" << "upper"))
		return false;
	return true;
}

BaseEvent::TileSubstitution::TileSubstitution(BaseEvent *pBase) : Event(pBase)
{
	tileset1ID = 0;
	tileset2ID = 0;
	tile1ID = 0;
	tile2ID = 0;
	layer = ALL;
	tile1ReferencedInVariables = false;
	tile2ReferencedInVariables = false;
}

BaseEvent::TileSubstitution::TileSubstitution(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	QString tempString;
	tileset1ID = QString(EventNode.getAttribute("tileset1ID")).toInt();
	tile1ID = QString(EventNode.getAttribute("tile1ID")).toInt();
	tile1ReferencedInVariables = (QString(EventNode.getAttribute("tile1ReferencedInVariables")) == "true") ? true:false;
	tileset2ID = QString(EventNode.getAttribute("tileset2ID")).toInt();
	tile2ID = QString(EventNode.getAttribute("tile2ID")).toInt();
	tile2ReferencedInVariables = (QString(EventNode.getAttribute("tile2ReferencedInVariables")) == "true") ? true:false;
	tempString = EventNode.getAttribute("layer");
	if (tempString == "all")
		layer = ALL;
	else if (tempString == "lower")
		layer = LOWER;
	else if (tempString == "hero")
		layer = HERO;
	else if (tempString == "upper")
		layer = UPPER;
}

BaseEvent::TileSubstitution::TileSubstitution(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	tileset1ID = storageFile.getSignedInt();
	tileset2ID = storageFile.getSignedInt();
	tile1ID = storageFile.getSignedInt();
	tile2ID = storageFile.getSignedInt();
	layer = storageFile.getSignedInt();
	tile1ReferencedInVariables = storageFile.getBool();
	tile2ReferencedInVariables = storageFile.getBool();
}

BaseEvent::TileSubstitution::TileSubstitution(TileSubstitution *other, BaseEvent *pBase) : Event(pBase)
{
	tileset1ID = other->tileset1ID;
	tileset2ID = other->tileset2ID;
	tile1ID = other->tile1ID;
	tile2ID = other->tile2ID;
	layer = other->layer;
	tile1ReferencedInVariables = other->tile1ReferencedInVariables;
	tile2ReferencedInVariables = other->tile2ReferencedInVariables;
}
//END BaseEvent::TileSubstitution Member Functions

//BEGIN BaseEvent::TimerOperation Member Functions
XMLNode BaseEvent::TimerOperation::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	EventNode.addAttribute("timerID", QString::number(timerID).toUtf8().data());
	EventNode.addAttribute("timerReferencedInVariable", (timerReferencedInVariable) ? "true":"false");
	if (operation == SET)
	{
		EventNode.addAttribute("operation", "set");
		EventNode.addAttribute("timeStoredInVariable", (data.setData.timeStoredInVariable) ? "true":"false");
		if (data.setData.timeStoredInVariable)
			EventNode.addAttribute("variableID", QString::number(data.setData.seconds).toUtf8().data());
		else
		{
			EventNode.addAttribute("minutes", QString::number(data.setData.minutes).toUtf8().data());
			EventNode.addAttribute("seconds", QString::number(data.setData.seconds).toUtf8().data());
		}
	}
	else if (operation == START)
	{
		EventNode.addAttribute("operation", "start");
		if (data.startData.horizontalAlignment == HALIGN_LEFT)
			EventNode.addAttribute("horizontalAlignment", "left");
		else if (data.startData.horizontalAlignment == HALIGN_CENTER)
			EventNode.addAttribute("horizontalAlignment", "center");
		else if (data.startData.horizontalAlignment == HALIGN_RIGHT)
			EventNode.addAttribute("horizontalAlignment", "right");
		if (data.startData.verticalAlignment == VALIGN_TOP)
			EventNode.addAttribute("verticalAlignment", "top");
		else if (data.startData.verticalAlignment == VALIGN_CENTER)
			EventNode.addAttribute("verticalAlignment", "center");
		else if (data.startData.verticalAlignment == VALIGN_BOTTOM)
			EventNode.addAttribute("verticalAlignment", "bottom");
		EventNode.addAttribute("displayTimerOnScreen", (data.startData.displayTimerOnScreen) ? "true":"false");
		EventNode.addAttribute("timerRunsDuringBattle", (data.startData.timerRunsDuringBattle) ? "true":"false");
	}
	else if (operation == STOP)
		EventNode.addAttribute("operation", "stop");
	return EventNode;
}

void BaseEvent::TimerOperation::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(timerID);
	storageFile.putSignedInt(operation);
	if (operation == SET)
	{
		storageFile.putSignedInt(data.setData.minutes);
		storageFile.putSignedInt(data.setData.seconds);
		storageFile.putBool(data.setData.timeStoredInVariable);
	}
	else if (operation == START)
	{
		storageFile.putSignedInt(data.startData.horizontalAlignment);
		storageFile.putSignedInt(data.startData.verticalAlignment);
		storageFile.putBool(data.startData.displayTimerOnScreen);
		storageFile.putBool(data.startData.timerRunsDuringBattle);
	}
	storageFile.putBool(timerReferencedInVariable);
}

QStringList BaseEvent::TimerOperation::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Timer Operation: Timer=";
	if (timerReferencedInVariable)
		viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(timerID));
	else
		viewableText[0] += QString::number(timerID);
	viewableText[0] += ", ";
	if (operation == SET)
	{
		viewableText[0] += "Set ";
		if (data.setData.timeStoredInVariable)
			viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(data.setData.seconds));
		else
			viewableText[0] += QString("%1:%2").arg(data.setData.minutes).arg(data.setData.seconds);
	}
	else if (operation == START)
		viewableText[0] += "Start";
	else
		viewableText[0] += "Stop";
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::TimerOperation::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	ConfigureEvent_TimerOperation *configTimerOperation = new ConfigureEvent_TimerOperation;
	bool configured = false;
	configTimerOperation->setIsReferencedTimer(timerReferencedInVariable);
	configTimerOperation->setTimerID(timerID);
	configTimerOperation->setOperation(operation);
	if (operation == SET)
	{
		configTimerOperation->setIsStoredTime(data.setData.timeStoredInVariable);
		configTimerOperation->setTime(data.setData.minutes, data.setData.seconds);
	}
	else if (operation == START)
	{
		configTimerOperation->setAlignment(data.startData.horizontalAlignment, data.startData.verticalAlignment);
		configTimerOperation->setDisplayTimerOnScreen(data.startData.displayTimerOnScreen);
		configTimerOperation->setTimerRunsDuringBattle(data.startData.timerRunsDuringBattle);
	}
	if (configTimerOperation->exec())
	{
		configured = true;
		timerID = configTimerOperation->getTimerID();
		timerReferencedInVariable = configTimerOperation->isReferencedTimer();
		operation = (Operation)configTimerOperation->getOperation();
		if (operation == SET)
		{
			data.setData.minutes = configTimerOperation->getMinutes();
			data.setData.seconds = configTimerOperation->getSeconds();
			data.setData.timeStoredInVariable = configTimerOperation->isStoredTime();
		}
		else if (operation == START)
		{
			data.startData.horizontalAlignment = configTimerOperation->getHAlignment();
			data.startData.verticalAlignment = configTimerOperation->getVAlignment();
			data.startData.displayTimerOnScreen = configTimerOperation->displayTimerOnScreen();
			data.startData.timerRunsDuringBattle = configTimerOperation->timerRunsDuringBattle();
		}
	}
	delete configTimerOperation;
	return configured;
}
#endif

void BaseEvent::TimerOperation::getSetData(SetData *other)
{
	other->minutes = data.setData.minutes;
	other->seconds = data.setData.seconds;
	other->timeStoredInVariable = data.setData.timeStoredInVariable;
}

void BaseEvent::TimerOperation::setSetData(SetData *other)
{
	data.setData.minutes = other->minutes;
	data.setData.seconds = other->seconds;
	data.setData.timeStoredInVariable = other->timeStoredInVariable;
}

void BaseEvent::TimerOperation::getStartData(StartData *other)
{
	other->horizontalAlignment = data.startData.horizontalAlignment;
	other->verticalAlignment = data.startData.verticalAlignment;
	other->displayTimerOnScreen = data.startData.displayTimerOnScreen;
	other->timerRunsDuringBattle = data.startData.timerRunsDuringBattle;
}

void BaseEvent::TimerOperation::setStartData(StartData *other)
{
	data.startData.horizontalAlignment = other->horizontalAlignment;
	data.startData.verticalAlignment = other->verticalAlignment;
	data.startData.displayTimerOnScreen = other->displayTimerOnScreen;
	data.startData.timerRunsDuringBattle = other->timerRunsDuringBattle;
}

bool BaseEvent::TimerOperation::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	QStringList tempStringList = QStringList() << "true" << "false";
	QString tempString;
	if (!XMLTools::attributeExists(EventNode, "timerID"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "timerReferencedInVariable"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "timerReferencedInVariable", tempStringList, 1))
		return false;
	tempStringList.clear();
	tempStringList << "set" << "start" << "stop";
	if (!XMLTools::attributeExists(EventNode, "operation"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "operation", tempStringList))
		return false;
	tempString = EventNode.getAttribute("operation");
	if (tempString == "set")
	{
		tempStringList.clear();
		tempStringList << "true" << "false";
		if (!XMLTools::attributeExists(EventNode, "timeStoredInVariable"))
			return false;
		if (!XMLTools::attributeStringValid(EventNode, "timeStoredInVariable", tempStringList, 1))
			return false;
		if (QString(EventNode.getAttribute("timeStoredInVariable")) == "true")
		{
			if (!XMLTools::attributeExists(EventNode, "variableID"))
				return false;
		}
		else
		{
			if (!XMLTools::attributeExists(EventNode, "minutes"))
				return false;
			if (!XMLTools::attributeIntRangeValid(EventNode, "minutes", 0, 99))
				return false;
			if (!XMLTools::attributeExists(EventNode, "seconds"))
				return false;
			if (!XMLTools::attributeIntRangeValid(EventNode, "seconds", 0, 59))
				return false;
		}
	}
	else if (tempString == "start")
	{
		tempStringList.clear();
		tempStringList << "left" << "center" << "right";
		if (!XMLTools::attributeExists(EventNode, "horizontalAlignment"))
			return false;
		if (!XMLTools::attributeStringValid(EventNode, "horizontalAlignment", tempStringList))
			return false;
		tempStringList.clear();
		tempStringList << "top" << "center" << "bottom";
		if (!XMLTools::attributeExists(EventNode, "verticalAlignment"))
			return false;
		if (!XMLTools::attributeStringValid(EventNode, "verticalAlignment", tempStringList))
			return false;
		tempStringList.clear();
		tempStringList << "true" << "false";
		if (!XMLTools::attributeExists(EventNode, "displayTimerOnScreen"))
			return false;
		if (!XMLTools::attributeStringValid(EventNode, "displayTimerOnScreen", tempStringList, 1))
			return false;
		if (!XMLTools::attributeExists(EventNode, "timerRunsDuringBattle"))
			return false;
		if (!XMLTools::attributeStringValid(EventNode, "timerRunsDuringBattle", tempStringList, 1))
			return false;
	}
	return true;
}

BaseEvent::TimerOperation::TimerOperation(BaseEvent *pBase) : Event(pBase)
{
	timerID = 0;
	operation = STOP;
	timerReferencedInVariable = false;
}

BaseEvent::TimerOperation::TimerOperation(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	QString tempString;
	timerID = QString(EventNode.getAttribute("timerID")).toInt();
	timerReferencedInVariable = (QString(EventNode.getAttribute("timerReferencedInVariable")) == "true") ? true:false;
	tempString = EventNode.getAttribute("operation");
	if (tempString == "set")
	{
		operation = SET;
		data.setData.timeStoredInVariable = (QString(EventNode.getAttribute("timeStoredInVariable")) == "true") ? true:false;
		if (data.setData.timeStoredInVariable)
		{
			data.setData.minutes = 0;
			data.setData.seconds = QString(EventNode.getAttribute("variableID")).toInt();
		}
		else
		{
			data.setData.minutes = QString(EventNode.getAttribute("minutes")).toInt();
			data.setData.seconds = QString(EventNode.getAttribute("seconds")).toInt();
		}
	}
	else if (tempString == "start")
	{
		operation = START;
		tempString = EventNode.getAttribute("horizontalAlignment");
		if (tempString == "left")
			data.startData.horizontalAlignment = HALIGN_LEFT;
		else if (tempString == "center")
			data.startData.horizontalAlignment = HALIGN_CENTER;
		else if (tempString == "right")
			data.startData.horizontalAlignment = HALIGN_RIGHT;
		tempString = EventNode.getAttribute("verticalAlignment");
		if (tempString == "top")
			data.startData.verticalAlignment = VALIGN_TOP;
		else if (tempString == "center")
			data.startData.verticalAlignment = VALIGN_CENTER;
		else if (tempString == "bottom")
			data.startData.verticalAlignment = VALIGN_BOTTOM;
		tempString = EventNode.getAttribute("displayTimerOnScreen");
		data.startData.displayTimerOnScreen = (tempString == "true") ? true:false;
		tempString = EventNode.getAttribute("timerRunsDuringBattle");
		data.startData.timerRunsDuringBattle = (tempString == "true") ? true:false;
	}
	else if (tempString == "stop")
		operation = STOP;
}

BaseEvent::TimerOperation::TimerOperation(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	timerID = storageFile.getSignedInt();
	operation = storageFile.getSignedInt();
	if (operation == SET)
	{
		data.setData.minutes = storageFile.getSignedInt();
		data.setData.seconds = storageFile.getSignedInt();
		data.setData.timeStoredInVariable = storageFile.getBool();
	}
	else if (operation == START)
	{
		data.startData.horizontalAlignment = storageFile.getSignedInt();
		data.startData.verticalAlignment = storageFile.getSignedInt();
		data.startData.displayTimerOnScreen = storageFile.getBool();
		data.startData.timerRunsDuringBattle = storageFile.getBool();
	}
	timerReferencedInVariable = storageFile.getBool();
}

BaseEvent::TimerOperation::TimerOperation(TimerOperation *other, BaseEvent *pBase) : Event(pBase)
{
	timerID = other->timerID;
	operation = other->operation;
	timerReferencedInVariable = other->timerReferencedInVariable;
	if (operation == SET)
	{
		data.setData.minutes = other->data.setData.minutes;
		data.setData.seconds = other->data.setData.seconds;
		data.setData.timeStoredInVariable = other->data.setData.timeStoredInVariable;
	}
	else if (operation == START)
	{
		data.startData.horizontalAlignment = other->data.startData.horizontalAlignment;
		data.startData.verticalAlignment = other->data.startData.verticalAlignment;
		data.startData.displayTimerOnScreen = other->data.startData.displayTimerOnScreen;
		data.startData.timerRunsDuringBattle = other->data.startData.timerRunsDuringBattle;
	}
}
//END BaseEvent::TimerOperation Member Functions

//BEGIN BaseEvent::TintScreen Member Functions
XMLNode BaseEvent::TintScreen::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	EventNode.addAttribute("colorRed", QString::number(color.red()).toUtf8().data());
	EventNode.addAttribute("colorGreen", QString::number(color.green()).toUtf8().data());
	EventNode.addAttribute("colorBlue", QString::number(color.blue()).toUtf8().data());
	EventNode.addAttribute("opacity", QString::number(opacity).toUtf8().data());
	EventNode.addAttribute("opacityStoredInVariable", (opacityStoredInVariable) ? "true":"false");
	EventNode.addAttribute("transitionTime", QString::number(transitionTime).toUtf8().data());
	EventNode.addAttribute("timeStoredInVariable", (timeStoredInVariable) ? "true":"false");
	EventNode.addAttribute("haltOtherProcesses", (haltOtherProcesses) ? "true":"false");
	return EventNode;
}

void BaseEvent::TintScreen::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(color.red());
	storageFile.putSignedInt(color.green());
	storageFile.putSignedInt(color.blue());
	storageFile.putSignedInt(opacity);
	storageFile.putSignedInt(transitionTime);
	storageFile.putBool(opacityStoredInVariable);
	storageFile.putBool(timeStoredInVariable);
	storageFile.putBool(haltOtherProcesses);
}

QStringList BaseEvent::TintScreen::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Tint Screen: color=rgb";
	viewableText[0] += QString("(%1, %2, %3) ").arg(color.red()).arg(color.green()).arg(color.blue());
	if (opacityStoredInVariable)
		viewableText[0] += QString("opacity=variable[%1] ").arg(ProjectData::numberVariableIdToString(opacity));
	else
		viewableText[0] += QString("opacity=%1 ").arg(opacity);
	if (timeStoredInVariable)
		viewableText[0] += QString("transitionTime=variable[%1]").arg(ProjectData::numberVariableIdToString(transitionTime));
	else
		viewableText[0] += QString("transitionTime=%1").arg(transitionTime);
	if (haltOtherProcesses)
		viewableText[0] += " (Halt Other Processes During Transition)";
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::TintScreen::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	ConfigureEvent_TintScreen *configEvent = new ConfigureEvent_TintScreen;
	bool configured = false;
	configEvent->setOpacityStoredInVariable(opacityStoredInVariable);
	configEvent->setTimeStoredInVariable(timeStoredInVariable);
	configEvent->setColor(color);
	configEvent->setOpacity(opacity);
	configEvent->setTransitionTime(transitionTime);
	configEvent->setHaltOtherProcesses(haltOtherProcesses);
	if (configEvent->exec())
	{
		configured = true;
		color = configEvent->getColor();
		opacity = configEvent->getOpacity();
		transitionTime = configEvent->getTransitionTime();
		opacityStoredInVariable = configEvent->isOpacityStoredInVariable();
		timeStoredInVariable = configEvent->isTimeStoredInVariable();
		haltOtherProcesses = configEvent->getHaltOtherProcesses();
	}
	delete configEvent;
	return configured;
}
#endif

bool BaseEvent::TintScreen::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	QStringList trueFalseList = QStringList() << "true" << "false";
	if (!XMLTools::attributeExists(EventNode, "colorRed"))
		return false;
	if (!XMLTools::attributeIntRangeValid(EventNode, "colorRed", 0, 255))
		return false;
	if (!XMLTools::attributeExists(EventNode, "colorGreen"))
		return false;
	if (!XMLTools::attributeIntRangeValid(EventNode, "colorGreen", 0, 255))
		return false;
	if (!XMLTools::attributeExists(EventNode, "colorBlue"))
		return false;
	if (!XMLTools::attributeIntRangeValid(EventNode, "colorBlue", 0, 255))
		return false;
	if (!XMLTools::attributeExists(EventNode, "opacity"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "opacityStoredInVariable"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "opacityStoredInVariable", trueFalseList, 1))
		return false;
	if (!XMLTools::attributeExists(EventNode, "transitionTime"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "timeStoredInVariable"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "timeStoredInVariable", trueFalseList, 1))
		return false;
	if (!XMLTools::attributeExists(EventNode, "haltOtherProcesses"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "haltOtherProcesses", trueFalseList, 1))
		return false;
	return true;
}

BaseEvent::TintScreen::TintScreen(BaseEvent *pBase) : Event(pBase)
{
	color = QColor(255, 255, 255);
	opacity = 100;
	transitionTime = 10;
	opacityStoredInVariable = false;
	timeStoredInVariable = false;
	haltOtherProcesses = false;
}

BaseEvent::TintScreen::TintScreen(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	int red = QString(EventNode.getAttribute("colorRed")).toInt();
	int green = QString(EventNode.getAttribute("colorGreen")).toInt();
	int blue = QString(EventNode.getAttribute("colorBlue")).toInt();
	color = QColor(red, green, blue);
	opacity = QString(EventNode.getAttribute("opacity")).toInt();
	opacityStoredInVariable = (QString(EventNode.getAttribute("opacityStoredInVariable")) == "true") ? true:false;
	transitionTime = QString(EventNode.getAttribute("transitionTime")).toInt();
	timeStoredInVariable = (QString(EventNode.getAttribute("timeStoredInVariable")) == "true") ? true:false;
	haltOtherProcesses = (QString(EventNode.getAttribute("haltOtherProcesses")) == "true") ? true:false;
}

BaseEvent::TintScreen::TintScreen(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	color = QColor(storageFile.getSignedInt(), storageFile.getSignedInt(), storageFile.getSignedInt());
	opacity = storageFile.getSignedInt();
	transitionTime = storageFile.getSignedInt();
	opacityStoredInVariable = storageFile.getBool();
	timeStoredInVariable = storageFile.getBool();
	haltOtherProcesses = storageFile.getBool();
}

BaseEvent::TintScreen::TintScreen(TintScreen *other, BaseEvent *pBase) : Event(pBase)
{
	color = other->color;
	opacity = other->opacity;
	transitionTime = other->transitionTime;
	opacityStoredInVariable = other->opacityStoredInVariable;
	timeStoredInVariable = other->timeStoredInVariable;
	haltOtherProcesses = other->haltOtherProcesses;
}
//END BaseEvent::TintScreen Member Functions

//BEGIN BaseEvent::TradeTwoEventLocations Member Functions
XMLNode BaseEvent::TradeTwoEventLocations::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	if (event1Type == HERO)
		EventNode.addAttribute("event1Type", "hero");
	else if (event1Type == THIS_EVENT)
		EventNode.addAttribute("event1Type", "thisEvent");
	else if (event1Type == VEHICLE)
		EventNode.addAttribute("event1Type", "vehicle");
	else if (event1Type == EVENT)
		EventNode.addAttribute("event1Type", "event");
	if (event1Type == VEHICLE || event1Type == EVENT)
		EventNode.addAttribute("event1ID", QString::number(event1ID).toUtf8().data());
	if (event2Type == HERO)
		EventNode.addAttribute("event2Type", "hero");
	else if (event2Type == THIS_EVENT)
		EventNode.addAttribute("event2Type", "thisEvent");
	else if (event2Type == VEHICLE)
		EventNode.addAttribute("event2Type", "vehicle");
	else if (event2Type == EVENT)
		EventNode.addAttribute("event2Type", "event");
	if (event2Type == VEHICLE || event2Type == EVENT)
		EventNode.addAttribute("event2ID", QString::number(event2ID).toUtf8().data());
	return EventNode;
}

void BaseEvent::TradeTwoEventLocations::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(event1Type);
	storageFile.putSignedInt(event1ID);
	storageFile.putSignedInt(event2Type);
	storageFile.putSignedInt(event2ID);
}

QStringList BaseEvent::TradeTwoEventLocations::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Trade Two Event Locations: ";
	viewableText[0] += "Event1=";
	if (event1Type == HERO)
		viewableText[0] += "Hero";
	else if (event1Type == THIS_EVENT)
		viewableText[0] += "This Event";
	else if (event1Type == VEHICLE)
		viewableText[0] += ProjectData::vehicleNames[event1ID];
	else if (event1Type == EVENT)
		viewableText[0] += QString("Event%1").arg(event1ID, 3, 10, QChar('0'));
	viewableText[0] += " Event2=";
	if (event2Type == HERO)
		viewableText[0] += "Event2=Hero";
	else if (event2Type == THIS_EVENT)
		viewableText[0] += "This Event";
	else if (event2Type == VEHICLE)
		viewableText[0] += ProjectData::vehicleNames[event2ID];
	else if (event2Type == EVENT)
		viewableText[0] += QString("Event%1").arg(event2ID, 3, 10, QChar('0'));
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::TradeTwoEventLocations::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerType);
	ConfigureEvent_TradeTwoEventLocations *configEvent = new ConfigureEvent_TradeTwoEventLocations(callerID);
	bool configured = false;
	configEvent->setEvent1Type(event1Type);
	configEvent->setEvent1ID(event1ID);
	configEvent->setEvent2Type(event2Type);
	configEvent->setEvent2ID(event2ID);
	if (configEvent->exec())
	{
		configured = true;
		event1Type = configEvent->getEvent1Type();
		event1ID = configEvent->getEvent1ID();
		event2Type = configEvent->getEvent2Type();
		event2ID = configEvent->getEvent2ID();
	}
	delete configEvent;
	return configured;
}
#endif

bool BaseEvent::TradeTwoEventLocations::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	QStringList tempStringList = QStringList() << "hero" << "thisEvent" << "vehicle" << "event";
	QString tempString;
	if (!XMLTools::attributeExists(EventNode, "event1Type"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "event1Type", tempStringList))
		return false;
	tempString = EventNode.getAttribute("event1Type");
	if (tempString == "vehicle" || tempString == "event")
	{
		if (!XMLTools::attributeExists(EventNode, "event1ID"))
			return false;
	}
	if (!XMLTools::attributeExists(EventNode, "event2Type"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "event2Type", tempStringList))
		return false;
	tempString = EventNode.getAttribute("event2Type");
	if (tempString == "vehicle" || tempString == "event")
	{
		if (!XMLTools::attributeExists(EventNode, "event2ID"))
			return false;
	}
	return true;
}

BaseEvent::TradeTwoEventLocations::TradeTwoEventLocations(BaseEvent *pBase) : Event(pBase)
{
	event1Type = HERO;
	event1ID = -1;
	event2Type = HERO;
	event2ID = -1;
}

BaseEvent::TradeTwoEventLocations::TradeTwoEventLocations(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	QString tempString = EventNode.getAttribute("event1Type");
	if (tempString == "hero")
		event1Type = HERO;
	else if (tempString == "thisEvent")
		event1Type = THIS_EVENT;
	else if (tempString == "vehicle")
		event1Type = VEHICLE;
	else if (tempString == "event")
		event1Type = EVENT;
	if (event1Type == VEHICLE || event1Type == EVENT)
		event1ID = QString(EventNode.getAttribute("event1ID")).toInt();
	tempString = EventNode.getAttribute("event2Type");
	if (tempString == "hero")
		event2Type = HERO;
	else if (tempString == "thisEvent")
		event2Type = THIS_EVENT;
	else if (tempString == "vehicle")
		event2Type = VEHICLE;
	else if (tempString == "event")
		event2Type = EVENT;
	if (event2Type == VEHICLE || event2Type == EVENT)
		event2ID = QString(EventNode.getAttribute("event2ID")).toInt();
}

BaseEvent::TradeTwoEventLocations::TradeTwoEventLocations(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	event1Type = storageFile.getSignedInt();
	event1ID = storageFile.getSignedInt();
	event2Type = storageFile.getSignedInt();
	event2ID = storageFile.getSignedInt();
}

BaseEvent::TradeTwoEventLocations::TradeTwoEventLocations(TradeTwoEventLocations *other, BaseEvent *pBase) : Event(pBase)
{
	event1Type = other->event1Type;
	event1ID = other->event1ID;
	event2Type = other->event2Type;
	event2ID = other->event2ID;
}
//END BaseEvent::TradeTwoEventLocations Member Functions

//BEGIN BaseEvent::VariableMultiValueConditionalBranch
BaseEvent::VariableMultiValueConditionalBranch::~VariableMultiValueConditionalBranch()
{
	for (int i = 0; i < values.size(); ++i)
	{
		for (int j = 0; j < values[i]->events.size(); ++j)
			delete (values[i]->events[j]);
		values[i]->events.clear();
		delete (values[i]);
	}
	for (int i = 0; i < noValueMetEvents.size(); ++i)
		delete (noValueMetEvents[i]);
	values.clear();
	noValueMetEvents.clear();
}

XMLNode BaseEvent::VariableMultiValueConditionalBranch::getEventNode()
{
	XMLNode TempNode, EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	EventNode.addAttribute("variableID", QString::number(variableID).toUtf8().data());
	EventNode.addAttribute("executeCustomHandlerIfNoValueMet", (executeCustomHandlerIfNoValueMet) ? "true":"false");
	for (int i = 0; i < values.size(); ++i)
	{
		TempNode = EventNode.addChild("value");
		TempNode.addAttribute("value", QString::number(values[i]->value).toUtf8().data());
		TempNode.addAttribute("valueStoredInVariable", (values[i]->valueStoredInVariable) ? "true":"false");
		for (int j = 0; j < values[i]->events.size(); ++j)
			TempNode.addChild(values[i]->events[j]->getEventNode());
	}
	if (executeCustomHandlerIfNoValueMet)
	{
		TempNode = EventNode.addChild("noValueMet");
		for (int i = 0; i < noValueMetEvents.size(); ++i)
			TempNode.addChild(noValueMetEvents[i]->getEventNode());
	}
	return EventNode;
}

void BaseEvent::VariableMultiValueConditionalBranch::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(variableID);
	storageFile.putBool(executeCustomHandlerIfNoValueMet);
	storageFile.putSignedInt(values.size());
	for (int i = 0; i < values.size(); ++i)
	{
		storageFile.putSignedInt(values[i]->value);
		storageFile.putBool(values[i]->valueStoredInVariable);
		storageFile.putSignedInt(values[i]->events.size());
		for (int j = 0; j < values[i]->events.size(); ++j)
			values[i]->events[j]->saveToStorageFile(storageFile);
	}
	storageFile.putSignedInt(noValueMetEvents.size());
	for (int i = 0; i < noValueMetEvents.size(); ++i)
		noValueMetEvents[i]->saveToStorageFile(storageFile);
}

QStringList BaseEvent::VariableMultiValueConditionalBranch::getResources()
{
	QStringList resources;
	for (int i = 0; i < values.size(); ++i)
	{
		for (int j = 0; j < values[i]->events.size(); ++j)
			resources += values[i]->events[j]->getResources();
	}
	if (executeCustomHandlerIfNoValueMet)
	{
		for (int i = 0; i < noValueMetEvents.size(); ++i)
			resources += noValueMetEvents[i]->getResources();
	}
	return resources;
}

QStringList BaseEvent::VariableMultiValueConditionalBranch::getViewableText(int indent)
{
	QStringList viewableText;
	QString text = "";
	for (int i = 0; i < indent; ++i)
		text += "  ";
	text += "<> ";
	text += "Variable Multi-Value Conditional Branch: ";
	text += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(variableID));
	viewableText.append(text);
	for (int i = 0; i < values.size(); ++i)
	{
		text = "";
		for (int j = 0; j < indent; ++j)
			text += "  ";
		text += "::Value: ";
		if (values[i]->valueStoredInVariable)
			text += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(values[i]->value));
		else
			text += QString::number(values[i]->value);
		viewableText.append(text);
		for (int j = 0; j < values[i]->events.size(); ++j)
			viewableText.append(values[i]->events[j]->getViewableText(indent + 1));
		text = "";
		for (int i = 0; i < indent + 1; ++i)
			text += "  ";
		text += "<>";
		viewableText.append(text);
	}
	if (executeCustomHandlerIfNoValueMet)
	{
		text = "";
		for (int i = 0; i < indent; ++i)
			text += "  ";
		text += "::No Value Met";
		viewableText.append(text);
		for (int i = 0; i < noValueMetEvents.size(); ++i)
			viewableText.append(noValueMetEvents[i]->getViewableText(indent + 1));
		text = "";
		for (int i = 0; i < indent + 1; ++i)
			text += "  ";
		text += "<>";
		viewableText.append(text);
	}
	text = "";
	for (int j = 0; j < indent; ++j)
		text += "  ";
	text += "::End";
	viewableText.append(text);
	return viewableText;
}

QList<BaseEvent*> BaseEvent::VariableMultiValueConditionalBranch::getEventPointers()
{
	QList<BaseEvent*> pointers;
	for (int i = 0; i < values.size(); ++i)
	{
		for (int j = 0; j < values[i]->events.size(); ++j)
			pointers.append(values[i]->events[j]);
		pointers.append(NULL);
	}
	if (executeCustomHandlerIfNoValueMet)
	{
		for (int i = 0; i < noValueMetEvents.size(); ++i)
			pointers.append(noValueMetEvents[i]);
		pointers.append(NULL);
	}
	return pointers;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::VariableMultiValueConditionalBranch::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	ConfigureEvent_VariableMultiValueConditionalBranch *configureBranch = new ConfigureEvent_VariableMultiValueConditionalBranch;
	bool configured = false;
	configureBranch->setVariableID(variableID);
	configureBranch->setExecuteCustomHandlerIfNoValueMet(executeCustomHandlerIfNoValueMet);
	configureBranch->addValues(values);
	if (configureBranch->exec())
	{
		QList<VariableValue*> newValues;
		configured = true;
		for (int i = 0; i < values.size(); ++i)
		{
			for (int j = 0; j < values[i]->events.size(); ++j)
				delete (values[i]->events[j]);
			values[i]->events.clear();
			delete (values[i]);
		}
		values.clear();
		variableID = configureBranch->getVariableID();
		executeCustomHandlerIfNoValueMet = configureBranch->getExecuteCustomHandlerIfNoValueMet();
		newValues = configureBranch->getValues();
		for (int i = 0; i < newValues.size(); ++i)
		{
			VariableValue *value = new VariableValue;
			value->value = newValues[i]->value;
			value->valueStoredInVariable = newValues[i]->valueStoredInVariable;
			for (int j = 0; j < newValues[i]->events.size(); ++j)
				value->events.append(new BaseEvent(newValues[i]->events[j]));
			values.append(value);
		}
	}
	delete configureBranch;
	return configured;
}
#endif

void BaseEvent::VariableMultiValueConditionalBranch::addValue(VariableValue *value)
{
	values.append(value);
}

void BaseEvent::VariableMultiValueConditionalBranch::removeValue(int location)
{
	for (int i = 0; i < values[location]->events.size(); ++i)
		delete (values[location]->events[i]);
	values[location]->events.clear();
	delete (values[location]);
	values.removeAt(location);
}

void BaseEvent::VariableMultiValueConditionalBranch::addChildEvent(BaseEvent *event, int location, int childTypeID)
{
	if (childTypeID == values.size() && executeCustomHandlerIfNoValueMet)
		noValueMetEvents.insert(location, event);
	else
		values[childTypeID]->events.insert(location, event);
}

void BaseEvent::VariableMultiValueConditionalBranch::deleteChildEvent(int location, int childTypeID)
{
	if (childTypeID == values.size() && executeCustomHandlerIfNoValueMet)
	{
		delete (noValueMetEvents[location]);
		noValueMetEvents.removeAt(location);
	}
	else
	{
		delete (values[childTypeID]->events[location]);
		values[childTypeID]->events.removeAt(location);
	}
}

void BaseEvent::VariableMultiValueConditionalBranch::applySwappedTiles(QList<Tileset::SwapTileIDs> swappedTiles)
{
	for (int i = 0; i < values.size(); ++i)
	{
		for (int j = 0; j < values[i]->events.size(); ++j)
			values[i]->events[j]->applySwappedTiles(swappedTiles);
	}
	for (int i = 0; i < noValueMetEvents.size(); ++i)
		noValueMetEvents[i]->applySwappedTiles(swappedTiles);
}

void BaseEvent::VariableMultiValueConditionalBranch::applyChangeMenuObjectName(QString oldName, QString newName)
{
	for (int i = 0; i < values.size(); ++i)
	{
		for (int j = 0; j < values[i]->events.size(); ++j)
			values[i]->events[j]->applyChangeMenuObjectName(oldName, newName);
	}
	for (int i = 0; i < noValueMetEvents.size(); ++i)
		noValueMetEvents[i]->applyChangeMenuObjectName(oldName, newName);
}

bool BaseEvent::VariableMultiValueConditionalBranch::tryLoad(XMLNode EventNode, int baseType)
{
	XMLNode TempNode;
	QStringList trueFalseList = QStringList() << "true" << "false";
	if (!XMLTools::attributeExists(EventNode, "variable"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "executeCustomHandlerIfNoValueMet"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "executeCustomHandlerIfNoValueMet", trueFalseList, 1))
		return false;
	for (int i = 0; i < EventNode.nChildNode("value"); ++i)
	{
		TempNode = EventNode.getChildNode("value", i);
		if (!XMLTools::attributeExists(TempNode, "value"))
			return false;
		if (!XMLTools::attributeExists(TempNode, "valueStoredInVariable"))
			return false;
		if (!XMLTools::attributeStringValid(TempNode, "valueStoredInVariable", trueFalseList, 1))
			return false;
		for (int j = 0; j < TempNode.nChildNode("event"); ++j)
			BaseEvent::tryLoad(TempNode.getChildNode("event", j), baseType);
	}
	if (QString(EventNode.getAttribute("executeCustomHandlerIfNoValueMet")) == "true")
	{
		if (!XMLTools::nodeExists(EventNode, "noValueMet"))
			return false;
		TempNode = EventNode.getChildNode("noValueMet");
		for (int i = 0; i < TempNode.nChildNode("event"); ++i)
			BaseEvent::tryLoad(TempNode.getChildNode("event", i), baseType);
	}
	return true;
}

BaseEvent::VariableMultiValueConditionalBranch::VariableMultiValueConditionalBranch(BaseEvent *pBase) : Event(pBase)
{
	variableID = 0;
	executeCustomHandlerIfNoValueMet = false;
}

BaseEvent::VariableMultiValueConditionalBranch::VariableMultiValueConditionalBranch(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	XMLNode TempNode;
	variableID = QString(EventNode.getAttribute("variableID")).toInt();
	executeCustomHandlerIfNoValueMet = (QString(EventNode.getAttribute("executeCustomHandlerIfNoValueMet")) == "true") ? true:false;
	for (int i = 0; i < EventNode.nChildNode("value"); ++i)
	{
		VariableValue *value = new VariableValue;
		TempNode = EventNode.getChildNode("value", i);
		value->value = QString(TempNode.getAttribute("value")).toInt();
		value->valueStoredInVariable = (QString(TempNode.getAttribute("valueStoredInVariable")) == "true") ? true:false;
		for (int j = 0; j < TempNode.nChildNode("event"); ++j)
			value->events.append(new BaseEvent(pBaseEvent->baseType, TempNode.getChildNode("event", j)));
		values.append(value);
	}
	if (executeCustomHandlerIfNoValueMet)
	{
		TempNode = EventNode.getChildNode("noValueMet");
		for (int i = 0; i < TempNode.nChildNode("event"); ++i)
			noValueMetEvents.append(new BaseEvent(pBaseEvent->baseType, TempNode.getChildNode("event", i)));
	}
}

BaseEvent::VariableMultiValueConditionalBranch::VariableMultiValueConditionalBranch(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	variableID = storageFile.getSignedInt();
	executeCustomHandlerIfNoValueMet = storageFile.getBool();
	for (int i = 0, count = storageFile.getSignedInt(); i < count; ++i)
	{
		VariableValue *value = new VariableValue;
		value->value = storageFile.getSignedInt();
		value->valueStoredInVariable = storageFile.getBool();
		for (int j = 0, count = storageFile.getSignedInt(); j < count; ++j)
			value->events.append(new BaseEvent(pBaseEvent->baseType, storageFile));
		values.append(value);
	}
	for (int i = 0, count = storageFile.getSignedInt(); i < count; ++i)
		noValueMetEvents.append(new BaseEvent(pBaseEvent->baseType, storageFile));
}

BaseEvent::VariableMultiValueConditionalBranch::VariableMultiValueConditionalBranch(VariableMultiValueConditionalBranch *other, BaseEvent *pBase) : Event(pBase)
{
	variableID = other->variableID;
	executeCustomHandlerIfNoValueMet = other->executeCustomHandlerIfNoValueMet;
	for (int i = 0; i < other->values.size(); ++i)
	{
		VariableValue *value;
		value->value = other->values[i]->value;
		value->valueStoredInVariable = other->values[i]->valueStoredInVariable;
		for (int j = 0; j < other->values[i]->events.size(); ++j)
			value->events.append(new BaseEvent(other->values[i]->events[j]));
		values.append(value);
	}
	for (int i = 0; i < other->noValueMetEvents.size(); ++i)
		noValueMetEvents.append(new BaseEvent(other->noValueMetEvents[i]));
}
//END BaseEvent::VariableMultiValueConditionalBranch

//BEGIN BaseEvent::VariableOperation Member Functions
XMLNode BaseEvent::VariableOperation::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	if (operation == SET)
		EventNode.addAttribute("operation", "set");
	else if (operation == ADD)
		EventNode.addAttribute("operation", "add");
	else if (operation == SUB)
		EventNode.addAttribute("operation", "subtract");
	else if (operation == MUL)
		EventNode.addAttribute("operation", "multiply");
	else if (operation == DIV)
		EventNode.addAttribute("operation", "divide");
	else if (operation == MOD)
		EventNode.addAttribute("operation", "modulous");
	if (variableToChange == SINGLE)
	{
		EventNode.addAttribute("variableToChange", "single");
		EventNode.addAttribute("variableID", QString::number(srcVar1ID).toUtf8().data());
	}
	else if (variableToChange == RANGE)
	{
		EventNode.addAttribute("variableToChange", "range");
		EventNode.addAttribute("startVariableID", QString::number(srcVar1ID).toUtf8().data());
		EventNode.addAttribute("endVariableID", QString::number(srcVar2ID).toUtf8().data());
	}
	else if (variableToChange == REFERENCE)
	{
		EventNode.addAttribute("variableToChange", "reference");
		EventNode.addAttribute("variableID", QString::number(srcVar1ID).toUtf8().data());
	}
	if (operand == VALUE)
	{
		EventNode.addAttribute("operand", "value");
		EventNode.addAttribute("value", QString::number(dstValue).toUtf8().data());
	}
	else if (operand == VARIABLE)
	{
		EventNode.addAttribute("operand", "variable");
		EventNode.addAttribute("variableID", QString::number(dstID).toUtf8().data());
		EventNode.addAttribute("variableReferencedInVariable", (dstIDStoredInVariable) ? "true":"false");
	}
	else if (operand == RANDOM_NUMBER)
	{
		EventNode.addAttribute("operand", "randomNumber");
		EventNode.addAttribute("min", QString::number(value1).toUtf8().data());
		EventNode.addAttribute("max", QString::number(value2).toUtf8().data());
		EventNode.addAttribute("valuesStoredInVariables", (dstIDStoredInVariable) ? "true":"false");
	}
	else if (operand == ITEM)
	{
		EventNode.addAttribute("operand", "item");
		EventNode.addAttribute("itemID", QString::number(dstID).toUtf8().data());
		EventNode.addAttribute("itemReferencedInVariable", (dstIDStoredInVariable) ? "true":"false");
		if (dstValue == NUM_HELD)
			EventNode.addAttribute("value", "numHeld");
		else if (dstValue == NUM_EQUIPPED)
			EventNode.addAttribute("value", "numEquipped");
		else if (dstValue == BUY_PRICE)
			EventNode.addAttribute("value", "buyPrice");
		else if (dstValue == SELL_PRICE)
			EventNode.addAttribute("value", "sellPrice");
		else if (dstValue == ITEM_ATTACK)
			EventNode.addAttribute("value", "attack");
		else if (dstValue == ITEM_DEFENSE)
			EventNode.addAttribute("value", "defense");
		else if (dstValue == ITEM_WISDOM)
			EventNode.addAttribute("value", "wisdom");
		else if (dstValue == ITEM_AGILITY)
			EventNode.addAttribute("value", "agility");
	}
	else if (operand == PARTY_MEMBER)
	{
		EventNode.addAttribute("operand", "partyMember");
		EventNode.addAttribute("partyMember", QString::number(dstID).toUtf8().data());
		if (dstValue == LEVEL)
			EventNode.addAttribute("value", "level");
		else if (dstValue == EXPERIENCE)
			EventNode.addAttribute("value", "experience");
		else if (dstValue == HP)
			EventNode.addAttribute("value", "hp");
		else if (dstValue == MP)
			EventNode.addAttribute("value", "mp");
		else if (dstValue == MAX_HP)
			EventNode.addAttribute("value", "maxHP");
		else if (dstValue == MAX_MP)
			EventNode.addAttribute("value", "maxMP");
		else if (dstValue == ATTACK)
			EventNode.addAttribute("value", "attack");
		else if (dstValue == DEFENSE)
			EventNode.addAttribute("value", "defense");
		else if (dstValue == WISDOM)
			EventNode.addAttribute("value", "wisdom");
		else if (dstValue == AGILITY)
			EventNode.addAttribute("value", "agility");
		else if (dstValue == WEAPON_ID)
			EventNode.addAttribute("value", "weaponID");
		else if (dstValue == SHIELD_ID)
			EventNode.addAttribute("value", "shieldID");
		else if (dstValue == ARMOR_ID)
			EventNode.addAttribute("value", "armorID");
		else if (dstValue == HELMET_ID)
			EventNode.addAttribute("value", "helmetID");
		else if (dstValue == ACCESSORY_ID)
			EventNode.addAttribute("value", "accessoryID");
	}
	else if (operand == CHARACTER)
	{
		EventNode.addAttribute("operand", "character");
		EventNode.addAttribute("characterID", QString::number(dstID).toUtf8().data());
		EventNode.addAttribute("characterReferencedInVariable", (dstIDStoredInVariable) ? "true":"false");
		if (dstValue == LEVEL)
			EventNode.addAttribute("value", "level");
		else if (dstValue == EXPERIENCE)
			EventNode.addAttribute("value", "experience");
		else if (dstValue == HP)
			EventNode.addAttribute("value", "hp");
		else if (dstValue == MP)
			EventNode.addAttribute("value", "mp");
		else if (dstValue == MAX_HP)
			EventNode.addAttribute("value", "maxHP");
		else if (dstValue == MAX_MP)
			EventNode.addAttribute("value", "maxMP");
		else if (dstValue == ATTACK)
			EventNode.addAttribute("value", "attack");
		else if (dstValue == DEFENSE)
			EventNode.addAttribute("value", "defense");
		else if (dstValue == WISDOM)
			EventNode.addAttribute("value", "wisdom");
		else if (dstValue == AGILITY)
			EventNode.addAttribute("value", "agility");
		else if (dstValue == WEAPON_ID)
			EventNode.addAttribute("value", "weaponID");
		else if (dstValue == SHIELD_ID)
			EventNode.addAttribute("value", "shieldID");
		else if (dstValue == ARMOR_ID)
			EventNode.addAttribute("value", "armorID");
		else if (dstValue == HELMET_ID)
			EventNode.addAttribute("value", "helmetID");
		else if (dstValue == ACCESSORY_ID)
			EventNode.addAttribute("value", "accessoryID");
	}
	else if (operand == MONSTER)
	{
		EventNode.addAttribute("operand", "monster");
		EventNode.addAttribute("monsterID", QString::number(dstID).toUtf8().data());
		EventNode.addAttribute("monsterReferencedInVariable", (dstIDStoredInVariable) ? "true":"false");
		if (dstValue == MONSTER_EXPERIENCE_GAIN)
			EventNode.addAttribute("value", "experienceGain");
		else if (dstValue == MONSTER_MONEY_GAIN)
			EventNode.addAttribute("value", "moneyGain");
		else if (dstValue == MONSTER_MAX_HP)
			EventNode.addAttribute("value", "maxHP");
		else if (dstValue == MONSTER_MAX_MP)
			EventNode.addAttribute("value", "maxMP");
		else if (dstValue == MONSTER_ATTACK)
			EventNode.addAttribute("value", "attack");
		else if (dstValue == MONSTER_DEFENSE)
			EventNode.addAttribute("value", "defense");
		else if (dstValue == MONSTER_WISDOM)
			EventNode.addAttribute("value", "wisdom");
		else if (dstValue == MONSTER_AGILITY)
			EventNode.addAttribute("value", "agility");
	}
	else if (operand == TIMER_SECONDS)
	{
		EventNode.addAttribute("operand", "timerSeconds");
		EventNode.addAttribute("timerID", QString::number(dstID).toUtf8().data());
	}
	else if (operand == COMPLEX_EXPRESSION)
	{
		EventNode.addAttribute("operand", "expression");
		EventNode.addAttribute("expression", expression.toUtf8().data());
	}
	else if (operand == OTHER)
	{
		EventNode.addAttribute("operand", "other");
		if (dstValue == MONEY)
			EventNode.addAttribute("value", "money");
		else if (dstValue == SIZE_OF_PARTY)
			EventNode.addAttribute("value", "sizeOfParty");
		else if (dstValue == NUM_SAVES)
			EventNode.addAttribute("value", "numSaves");
		else if (dstValue == NUM_BATTLES)
			EventNode.addAttribute("value", "numBattles");
		else if (dstValue == NUM_VICTORIES)
			EventNode.addAttribute("value", "numVictories");
		else if (dstValue == NUM_DEFEATS)
			EventNode.addAttribute("value", "numDefeats");
		else if (dstValue == NUM_ESCAPES)
			EventNode.addAttribute("value", "numEscapes");
		else if (dstValue == MUSIC_MILLISECONDS)
			EventNode.addAttribute("value", "musicMilliseconds");
	}
	return EventNode;
}

void BaseEvent::VariableOperation::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(operation);
	storageFile.putSignedInt(variableToChange);
	storageFile.putSignedInt(srcVar1ID);
	if (variableToChange == RANGE)
		storageFile.putSignedInt(srcVar2ID);
	storageFile.putSignedInt(operand);
	if (operand == VALUE)
		storageFile.putSignedInt(dstValue);
	else if (operand == VARIABLE)
	{
		storageFile.putSignedInt(dstID);
		storageFile.putBool(dstIDStoredInVariable);
	}
	else if (operand == RANDOM_NUMBER)
	{
		storageFile.putSignedLong(value1);
		storageFile.putSignedLong(value2);
		storageFile.putBool(dstIDStoredInVariable);
	}
	else if (operand == ITEM || operand == PARTY_MEMBER || operand == CHARACTER || operand == MONSTER)
	{
		storageFile.putSignedInt(dstID);
		storageFile.putSignedInt(dstValue);
		storageFile.putBool(dstIDStoredInVariable);
	}
	else if (operand == TIMER_SECONDS)
	{
		storageFile.putSignedInt(dstID);
		storageFile.putBool(dstIDStoredInVariable);
	}
	else if (operand == COMPLEX_EXPRESSION)
		storageFile.putString(expression.toUtf8().data());
	else if (operand == OTHER)
		storageFile.putSignedInt(dstValue);
}

QStringList BaseEvent::VariableOperation::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Variable Operation: ";
	if (variableToChange == SINGLE)
		viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(srcVar1ID));
	else if (variableToChange == RANGE)
	{
		viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(srcVar1ID));
		viewableText[0] += " - ";
		viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(srcVar2ID));
	}
	else
		viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(srcVar1ID));
	viewableText[0] += ", ";
	if (operation == SET)
		viewableText[0] += "Set, ";
	else if (operation == ADD)
		viewableText[0] += "Add, ";
	else if (operation == SUB)
		viewableText[0] += "Subtract, ";
	else if (operation == MUL)
		viewableText[0] += "Multiply, ";
	else if (operation == DIV)
		viewableText[0] += "Divide, ";
	else if (operation == MOD)
		viewableText[0] += "Modulus, ";
	if (operand == VALUE)
		viewableText[0] += QString::number(dstValue);
	else if (operand == VARIABLE)
	{
		if (dstIDStoredInVariable)
		{
			QString string = QString("variable[%1]").arg(ProjectData::numberVariableIdToString(dstID));
			string[0] = QChar('V');
			viewableText[0] += "referenced";
			viewableText[0] += string;
		}
		else
			viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(dstID));
	}
	else if (operand == RANDOM_NUMBER)
	{
		viewableText[0] += "Random Number min=";
		if (dstIDStoredInVariable)
			viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(value1));
		else
			viewableText[0] += QString::number(value1);
		viewableText[0] += " max=";
		if (dstIDStoredInVariable)
			viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(value2));
		else
			viewableText[0] += QString::number(value2);
	}
	else if (operand == ITEM)
	{
		viewableText[0] += "Item ";
		if (dstIDStoredInVariable)
			viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(dstID));
		else
			viewableText[0] += ProjectData::itemNames[dstID];
		viewableText[0] += "\'s ";
		if (dstValue == NUM_HELD)
			viewableText[0] += "Number Held";
		else if (dstValue == NUM_EQUIPPED)
			viewableText[0] += "Number Equipped";
		else if (dstValue == BUY_PRICE)
			viewableText[0] += "Buy Price";
		else if (dstValue == SELL_PRICE)
			viewableText[0] += "Sell Price";
		else if (dstValue == ITEM_ATTACK)
			viewableText[0] += "Attack";
		else if (dstValue == ITEM_DEFENSE)
			viewableText[0] += "Defense";
		else if (dstValue == ITEM_WISDOM)
			viewableText[0] += "Wisdom";
		else if (dstValue == ITEM_AGILITY)
			viewableText[0] += "Agility";
		
	}
	else if (operand == PARTY_MEMBER)
	{
		viewableText[0] += "Party Member ";
		if (dstID > 3)
			viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(dstID));
		else
			viewableText[0] += QString::number(dstID + 1);
		viewableText[0] += "\'s ";
		if (dstValue == LEVEL)
			viewableText[0] += "Level";
		else if (dstValue == EXPERIENCE)
			viewableText[0] += "Experience";
		else if (dstValue == HP)
			viewableText[0] += "HP";
		else if (dstValue == MP)
			viewableText[0] += "MP";
		else if (dstValue == MAX_HP)
			viewableText[0] += "Max HP";
		else if (dstValue == MAX_MP)
			viewableText[0] += "Max MP";
		else if (dstValue == ATTACK)
			viewableText[0] += "Attack";
		else if (dstValue == DEFENSE)
			viewableText[0] += "Defense";
		else if (dstValue == WISDOM)
			viewableText[0] += "Wisdom";
		else if (dstValue == AGILITY)
			viewableText[0] += "Agility";
		else if (dstValue == WEAPON_ID)
			viewableText[0] += "Weapon ID";
		else if (dstValue == SHIELD_ID)
			viewableText[0] += "Shield ID";
		else if (dstValue == ARMOR_ID)
			viewableText[0] += "Armor ID";
		else if (dstValue == HELMET_ID)
			viewableText[0] += "Helmet ID";
		else if (dstValue == ACCESSORY_ID)
			viewableText[0] += "Accessory ID";
	}
	else if (operand == CHARACTER)
	{
		viewableText[0] += "Character ";
		if (dstIDStoredInVariable)
			viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(dstID));
		else
			viewableText[0] += ProjectData::characterNames[dstID];
		viewableText[0] += "\'s ";
		if (dstValue == LEVEL)
			viewableText[0] += "Level";
		else if (dstValue == EXPERIENCE)
			viewableText[0] += "Experience";
		else if (dstValue == HP)
			viewableText[0] += "HP";
		else if (dstValue == MP)
			viewableText[0] += "MP";
		else if (dstValue == MAX_HP)
			viewableText[0] += "Max HP";
		else if (dstValue == MAX_MP)
			viewableText[0] += "Max MP";
		else if (dstValue == ATTACK)
			viewableText[0] += "Attack";
		else if (dstValue == DEFENSE)
			viewableText[0] += "Defense";
		else if (dstValue == WISDOM)
			viewableText[0] += "Wisdom";
		else if (dstValue == AGILITY)
			viewableText[0] += "Agility";
		else if (dstValue == WEAPON_ID)
			viewableText[0] += "Weapon ID";
		else if (dstValue == SHIELD_ID)
			viewableText[0] += "Shield ID";
		else if (dstValue == ARMOR_ID)
			viewableText[0] += "Armor ID";
		else if (dstValue == HELMET_ID)
			viewableText[0] += "Helmet ID";
		else if (dstValue == ACCESSORY_ID)
			viewableText[0] += "Accessory ID";
	}
	else if (operand == MONSTER)
	{
		viewableText[0] += "Monster ";
		if (dstIDStoredInVariable)
			viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(dstID));
		else
			viewableText[0] += ProjectData::monsterNames[dstID];
		viewableText[0] += "\'s ";
		if (dstValue == MONSTER_EXPERIENCE_GAIN)
			viewableText[0] += "Experience Gain";
		else if (dstValue == MONSTER_MONEY_GAIN)
			viewableText[0] += "Money Gain";
		else if (dstValue == MONSTER_MAX_HP)
			viewableText[0] += "Max HP";
		else if (dstValue == MONSTER_MAX_MP)
			viewableText[0] += "Max MP";
		else if (dstValue == MONSTER_ATTACK)
			viewableText[0] += "Attack";
		else if (dstValue == MONSTER_DEFENSE)
			viewableText[0] += "Defense";
		else if (dstValue == MONSTER_WISDOM)
			viewableText[0] += "Wisdom";
		else if (dstValue == MONSTER_AGILITY)
			viewableText[0] += "Agility";
	}
	else if (operand == TIMER_SECONDS)
	{
		viewableText[0] += "Timer ";
		if (dstIDStoredInVariable)
			viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(dstID));
		else
			viewableText[0] += QString::number(dstID);
		viewableText[0] += "\'s seconds";
	}
	else if (operand == COMPLEX_EXPRESSION)
		viewableText[0] += QString("Expression \"%1\"").arg(expression);
	else if (operand == OTHER)
	{
		if (dstValue == MONEY)
			viewableText[0] += "Party's Money";
		else if (dstValue == SIZE_OF_PARTY)
			viewableText[0] += "Size of Party";
		else if (dstValue == NUM_SAVES)
			viewableText[0] += "Number of Saves";
		else if (dstValue == NUM_BATTLES)
			viewableText[0] += "Number of Battles";
		else if (dstValue == NUM_VICTORIES)
			viewableText[0] += "Number of Victories";
		else if (dstValue == NUM_DEFEATS)
			viewableText[0] += "Number of Defeats";
		else if (dstValue == NUM_ESCAPES)
			viewableText[0] += "Number of Escapes";
		else if (dstValue == MUSIC_MILLISECONDS)
			viewableText[0] += "BGM's Time in Milliseconds";
	}
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::VariableOperation::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	ConfigureEvent_VariableOperation *configVariableOperation = new ConfigureEvent_VariableOperation;
	bool configured = false;
	configVariableOperation->setVariableToChange(variableToChange);
	configVariableOperation->setVariableIDs(srcVar1ID, srcVar2ID);
	configVariableOperation->setOperation(operation);
	configVariableOperation->setOperand(operand);
	configVariableOperation->setIsReferencedOperand(dstIDStoredInVariable);
	configVariableOperation->setOperandValues(value1, value2, dstID, dstValue, expression);
	if (configVariableOperation->exec())
	{
		configured = true;
		variableToChange = configVariableOperation->getVariableToChange();
		srcVar1ID = configVariableOperation->getVariable1ID();
		srcVar2ID = configVariableOperation->getVariable2ID();
		operation = configVariableOperation->getOperation();
		operand = configVariableOperation->getOperand();
		value1 = configVariableOperation->getValue1();
		value2 = configVariableOperation->getValue2();
		dstID = configVariableOperation->getOperandID();
		dstValue = configVariableOperation->getOperandValue();
		dstIDStoredInVariable = configVariableOperation->isReferencedOperand();
		if (operand == COMPLEX_EXPRESSION)
			expression = configVariableOperation->getExpression();
		else
			expression = "";
	}
	delete configVariableOperation;
	return configured;
}
#endif

bool BaseEvent::VariableOperation::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	QStringList tempStringList;
	QStringList trueFalseList = QStringList() << "true" << "false";
	QString tempString;
	if (!XMLTools::attributeExists(EventNode, "variableToChange"))
		return false;
	tempStringList.clear();
	tempStringList << "single" << "range" << "reference";
	if (!XMLTools::attributeStringValid(EventNode, "variableToChange", tempStringList))
		return false;
	if (QString(EventNode.getAttribute("variableToChange")) == "range")
	{
		if (!XMLTools::attributeExists(EventNode, "startVariableID"))
			return false;
		if (!XMLTools::attributeExists(EventNode, "endVariableID"))
			return false;
	}
	else
	{
		if (!XMLTools::attributeExists(EventNode, "variableID"))
			return false;
	}
	if (!XMLTools::attributeExists(EventNode, "operation"))
		return false;
	tempStringList.clear();
	tempStringList << "set" << "add" << "subtract" << "multiply" << "divide" << "modulous";
	if (!XMLTools::attributeStringValid(EventNode, "operation", tempStringList))
		return false;
	if (!XMLTools::attributeExists(EventNode, "operand"))
		return false;
	tempStringList.clear();
	tempStringList << "value" << "variable" << "randomNumber" << "item" << "partyMember" << "character" << "monster" << "timerSeconds" << "expression" << "other";
	if (!XMLTools::attributeStringValid(EventNode, "operand", tempStringList))
		return false;
	tempString = EventNode.getAttribute("operand");
	if (tempString == "value")
	{
		if (!XMLTools::attributeExists(EventNode, "value"))
			return false;
	}
	else if (tempString == "variable")
	{
		if (!XMLTools::attributeExists(EventNode, "variableID"))
			return false;
		if (!XMLTools::attributeExists(EventNode, "variableReferencedInVariable"))
			return false;
		if (!XMLTools::attributeStringValid(EventNode, "variableReferencedInVariable", trueFalseList, 1))
			return false;
	}
	else if (tempString == "randomNumber")
	{
		if (!XMLTools::attributeExists(EventNode, "min"))
			return false;
		if (!XMLTools::attributeExists(EventNode, "max"))
			return false;
		if (!XMLTools::attributeExists(EventNode, "valuesStoredInVariables"))
			return false;
		if (!XMLTools::attributeStringValid(EventNode, "valuesStoredInVariables", trueFalseList, 1))
			return false;
	}
	else if (tempString == "item")
	{
		if (!XMLTools::attributeExists(EventNode, "itemID"))
			return false;
		if (!XMLTools::attributeExists(EventNode, "itemReferencedInVariable"))
			return false;
		if (!XMLTools::attributeStringValid(EventNode, "itemReferencedInVariable", trueFalseList, 1))
			return false;
		if (!XMLTools::attributeExists(EventNode, "value"))
			return false;
		tempStringList.clear();
		tempStringList << "numHeld" << "numEquipped" << "buyPrice" << "sellPrice" << "attack" << "defense" << "wisdom" << "agility";
		if (!XMLTools::attributeStringValid(EventNode, "value", tempStringList))
			return false;
	}
	else if (tempString == "partyMember")
	{
		if (!XMLTools::attributeExists(EventNode, "partyMember"))
			return false;
		if (!XMLTools::attributeExists(EventNode, "value"))
			return false;
		tempStringList.clear();
		tempStringList << "level" << "experience" << "hp" << "mp" << "maxHP" << "maxMP" << "attack" << "defense" << "wisdom" << "agility" << "weaponID" << "shieldID" << "armorID" << "helmetID" << "accessoryID";
		if (!XMLTools::attributeStringValid(EventNode, "value", tempStringList))
			return false;
	}
	else if (tempString == "character")
	{
		if (!XMLTools::attributeExists(EventNode, "characterID"))
			return false;
		if (!XMLTools::attributeExists(EventNode, "characterReferencedInVariable"))
			return false;
		if (!XMLTools::attributeStringValid(EventNode, "characterReferencedInVariable", trueFalseList, 1))
			return false;
		if (!XMLTools::attributeExists(EventNode, "value"))
			return false;
		tempStringList.clear();
		tempStringList << "level" << "experience" << "hp" << "mp" << "maxHP" << "maxMP" << "attack" << "defense" << "wisdom" << "agility" << "weaponID" << "shieldID" << "armorID" << "helmetID" << "accessoryID";
		if (!XMLTools::attributeStringValid(EventNode, "value", tempStringList))
			return false;
	}
	else if (tempString == "monster")
	{
		if (!XMLTools::attributeExists(EventNode, "monsterID"))
			return false;
		if (!XMLTools::attributeExists(EventNode, "monsterReferencedInVariable"))
			return false;
		if (!XMLTools::attributeStringValid(EventNode, "monsterReferencedInVariable", trueFalseList, 1))
			return false;
		if (!XMLTools::attributeExists(EventNode, "value"))
			return false;
		tempStringList.clear();
		tempStringList << "experienceGain" << "moneyGain" << "maxHP" << "maxMP" << "attack" << "defense" << "wisdom" << "agility";
		if (!XMLTools::attributeStringValid(EventNode, "value", tempStringList))
			return false;
	}
	else if (tempString == "timerSeconds")
	{
		if (!XMLTools::attributeExists(EventNode, "timer"))
			return false;
		if (!XMLTools::attributeExists(EventNode, "timerReferencedInVariable"))
			return false;
		if (!XMLTools::attributeStringValid(EventNode, "timerReferencedInVariable", trueFalseList, 1))
			return false;
	}
	else if (tempString == "expression")
	{
		if (!XMLTools::attributeExists(EventNode, "expression"))
			return false;
	}
	else if (tempString == "other")
	{
		if (!XMLTools::attributeExists(EventNode, "value"))
			return false;
		tempStringList.clear();
		tempStringList << "money" << "sizeOfParty" << "numSaves" << "numBattles" << "numVictories" << "numDefeats" << "numEscapes" << "musicMilliseconds";
		if (!XMLTools::attributeStringValid(EventNode, "value", tempStringList))
			return false;
	}
	return false;
}

BaseEvent::VariableOperation::VariableOperation(BaseEvent *pBase) : Event(pBase)
{
	operation = SET;
	variableToChange = SINGLE;
	operand = VALUE;
	value1 = 0l;
	value2 = 0l;
	srcVar1ID = 0;
	srcVar2ID = 0;
	dstID = 0;
	dstValue = 0;
	dstIDStoredInVariable = false;
	expression = "";
}

BaseEvent::VariableOperation::VariableOperation(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	QString tempString;
	tempString = EventNode.getAttribute("operation");
	if (tempString == "set")
		operation = SET;
	else if (tempString == "add")
		operation = ADD;
	else if (tempString == "subtract")
		operation = SUB;
	else if (tempString == "multiply")
		operation = MUL;
	else if (tempString == "divide")
		operation = DIV;
	else if (tempString == "modulous")
		operation = MOD;
	tempString = EventNode.getAttribute("variableToChange");
	if (tempString == "single")
	{
		variableToChange = SINGLE;
		srcVar1ID = QString(EventNode.getAttribute("variableID")).toInt();
		srcVar2ID = 0;
	}
	else if (tempString == "range")
	{
		variableToChange = RANGE;
		srcVar1ID = QString(EventNode.getAttribute("startVariableID")).toInt();
		srcVar2ID = QString(EventNode.getAttribute("endVariableID")).toInt();
	}
	else if (tempString == "reference")
	{
		variableToChange = REFERENCE;
		srcVar1ID = QString(EventNode.getAttribute("variableID")).toInt();
		srcVar2ID = 0;
	}
	tempString = EventNode.getAttribute("operand");
	if (tempString == "value")
	{
		operand = VALUE;
		value1 = 0l;
		value2 = 0l;
		dstID = 0;
		dstValue = QString(EventNode.getAttribute("value")).toInt();
		dstIDStoredInVariable = false;
		expression = "";
	}
	else if (tempString == "variable")
	{
		operand = VARIABLE;
		value1 = 0l;
		value2 = 0l;
		dstID = QString(EventNode.getAttribute("variableID")).toInt();
		dstValue = 0;
		dstIDStoredInVariable = (QString(EventNode.getAttribute("variableReferencedInVariable")) == "true") ? true:false;
		expression = "";
	}
	else if (tempString == "randomNumber")
	{
		operand = RANDOM_NUMBER;
		value1 = QString(EventNode.getAttribute("min")).toLong();
		value2 = QString(EventNode.getAttribute("max")).toLong();
		dstID = 0;
		dstValue = 0;
		dstIDStoredInVariable = (QString(EventNode.getAttribute("valuesStoredInVariables")) == "true") ? true:false;
		expression = "";
	}
	else if (tempString == "item")
	{
		operand = ITEM;
		value1 = 0l;
		value2 = 0l;
		dstID = QString(EventNode.getAttribute("itemID")).toInt();
		dstIDStoredInVariable = (QString(EventNode.getAttribute("itemReferencedInVariable")) == "true") ? true:false;
		tempString = EventNode.getAttribute("value");
		if (tempString == "numHeld")
			dstValue = NUM_HELD;
		else if (tempString == "numEquipped")
			dstValue = NUM_EQUIPPED;
		else if (tempString == "buyPrice")
			dstValue = BUY_PRICE;
		else if (tempString == "sellPrice")
			dstValue = SELL_PRICE;
		else if (tempString == "attack")
			dstValue = ITEM_ATTACK;
		else if (tempString == "defense")
			dstValue = ITEM_DEFENSE;
		else if (tempString == "wisdom")
			dstValue = ITEM_WISDOM;
		else if (tempString == "agility")
			dstValue = ITEM_AGILITY;
		expression = "";
	}
	else if (tempString == "partyMember")
	{
		operand = PARTY_MEMBER;
		value1 = 0l;
		value2 = 0l;
		dstID = QString(EventNode.getAttribute("partyMember")).toInt();
		tempString = EventNode.getAttribute("value");
		if (tempString == "level")
			dstValue = LEVEL;
		else if (tempString == "experience")
			dstValue = EXPERIENCE;
		else if (tempString == "hp")
			dstValue = HP;
		else if (tempString == "mp")
			dstValue = MP;
		else if (tempString == "maxHP")
			dstValue = MAX_HP;
		else if (tempString == "maxMP")
			dstValue = MAX_MP;
		else if (tempString == "attack")
			dstValue = ATTACK;
		else if (tempString == "defense")
			dstValue = DEFENSE;
		else if (tempString == "wisdom")
			dstValue = WISDOM;
		else if (tempString == "agility")
			dstValue = AGILITY;
		else if (tempString == "weaponID")
			dstValue = WEAPON_ID;
		else if (tempString == "shieldID")
			dstValue = SHIELD_ID;
		else if (tempString == "armorID")
			dstValue = ARMOR_ID;
		else if (tempString == "helmetID")
			dstValue = HELMET_ID;
		else if (tempString == "accessoryID")
			dstValue = ACCESSORY_ID;
		dstIDStoredInVariable = false;
		expression = "";
	}
	else if (tempString == "character")
	{
		operand = CHARACTER;
		value1 = 0l;
		value2 = 0l;
		dstID = QString(EventNode.getAttribute("characterID")).toInt();
		dstIDStoredInVariable = (QString(EventNode.getAttribute("characterReferencedInVariable")) == "true") ? true:false;
		tempString = EventNode.getAttribute("value");
		if (tempString == "level")
			dstValue = LEVEL;
		else if (tempString == "experience")
			dstValue = EXPERIENCE;
		else if (tempString == "hp")
			dstValue = HP;
		else if (tempString == "mp")
			dstValue = MP;
		else if (tempString == "maxHP")
			dstValue = MAX_HP;
		else if (tempString == "maxMP")
			dstValue = MAX_MP;
		else if (tempString == "attack")
			dstValue = ATTACK;
		else if (tempString == "defense")
			dstValue = DEFENSE;
		else if (tempString == "wisdom")
			dstValue = WISDOM;
		else if (tempString == "agility")
			dstValue = AGILITY;
		else if (tempString == "weaponID")
			dstValue = WEAPON_ID;
		else if (tempString == "shieldID")
			dstValue = SHIELD_ID;
		else if (tempString == "armorID")
			dstValue = ARMOR_ID;
		else if (tempString == "helmetID")
			dstValue = HELMET_ID;
		else if (tempString == "accessoryID")
			dstValue = ACCESSORY_ID;
		expression = "";
	}
	else if (tempString == "monster")
	{
		operand = MONSTER;
		value1 = 0l;
		value2 = 0l;
		dstID = QString(EventNode.getAttribute("monsterID")).toInt();
		dstIDStoredInVariable = (QString(EventNode.getAttribute("monsterReferencedInVariable")) == "true") ? true:false;
		tempString = EventNode.getAttribute("value");
		if (tempString == "experienceGain")
			dstValue = MONSTER_EXPERIENCE_GAIN;
		else if (tempString == "moneyGain")
			dstValue = MONSTER_MONEY_GAIN;
		else if (tempString == "maxHP")
			dstValue = MONSTER_MAX_HP;
		else if (tempString == "maxMP")
			dstValue = MONSTER_MAX_MP;
		else if (tempString == "attack")
			dstValue = MONSTER_ATTACK;
		else if (tempString == "defense")
			dstValue = MONSTER_DEFENSE;
		else if (tempString == "wisdom")
			dstValue = MONSTER_WISDOM;
		else if (tempString == "agility")
			dstValue = MONSTER_AGILITY;
		expression = "";
	}
	else if (tempString == "timerSeconds")
	{
		operand = TIMER_SECONDS;
		value1 = 0l;
		value2 = 0l;
		dstID = QString(EventNode.getAttribute("timerID")).toInt();
		dstIDStoredInVariable = (QString(EventNode.getAttribute("timerReferencedInVariable")) == "true") ? true:false;
		dstValue = 0;
		expression = "";
	}
	else if (tempString == "expression")
	{
		operand = COMPLEX_EXPRESSION;
		value1 = 0l;
		value2 = 0l;
		dstID = 0;
		dstIDStoredInVariable = false;
		dstValue = 0;
		expression = EventNode.getAttribute("expression");
	}
	else if (tempString == "other")
	{
		operand = OTHER;
		value1 = 0l;
		value2 = 0l;
		dstID = 0;
		dstIDStoredInVariable = false;
		tempString = EventNode.getAttribute("value");
		if (tempString == "money")
			dstValue = MONEY;
		else if (tempString == "sizeOfParty")
			dstValue = SIZE_OF_PARTY;
		else if (tempString == "numSaves")
			dstValue = NUM_SAVES;
		else if (tempString == "numBattles")
			dstValue = NUM_BATTLES;
		else if (tempString == "numVictories")
			dstValue = NUM_VICTORIES;
		else if (tempString == "numDefeats")
			dstValue = NUM_DEFEATS;
		else if (tempString == "numEscapes")
			dstValue = NUM_ESCAPES;
		else if (tempString == "musicMilliseconds")
			dstValue = MUSIC_MILLISECONDS;
		expression = "";
	}
}

BaseEvent::VariableOperation::VariableOperation(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	operation = storageFile.getSignedInt();
	variableToChange = storageFile.getSignedInt();
	srcVar1ID = storageFile.getSignedInt();
	if (variableToChange == RANGE)
		srcVar2ID = storageFile.getSignedInt();
	else
		srcVar2ID = 0;
	operand = storageFile.getSignedInt();
	value1 = 0l;
	value2 = 0l;
	dstID = 0;
	dstValue = 0;
	dstIDStoredInVariable = false;
	if (operand == VALUE)
		dstValue = storageFile.getSignedInt();
	else if (operand == VARIABLE)
	{
		dstID = storageFile.getSignedInt();
		dstIDStoredInVariable = storageFile.getBool();
	}
	else if (operand == RANDOM_NUMBER)
	{
		value1 = storageFile.getSignedLong();
		value2 = storageFile.getSignedLong();
		dstIDStoredInVariable = storageFile.getBool();
	}
	else if (operand == ITEM || operand == PARTY_MEMBER || operand == CHARACTER || operand == MONSTER)
	{
		dstID = storageFile.getSignedInt();
		dstValue = storageFile.getSignedInt();
		dstIDStoredInVariable = storageFile.getBool();
	}
	else if (operand == TIMER_SECONDS)
	{
		dstID = storageFile.getSignedInt();
		dstIDStoredInVariable = storageFile.getBool();
	}
	else if (operand == COMPLEX_EXPRESSION)
	{
		char *temp_string = storageFile.getString();
		expression = temp_string;
		delete[] temp_string;
	}
	else if (operand == OTHER)
		dstValue = storageFile.getSignedInt();
}

BaseEvent::VariableOperation::VariableOperation(VariableOperation *other, BaseEvent *pBase) : Event(pBase)
{
	operation = other->operation;
	variableToChange = other->variableToChange;
	operand = other->operand;
	value1 = other->value1;
	value2 = other->value2;
	srcVar1ID = other->srcVar1ID;
	srcVar2ID = other->srcVar2ID;
	dstID = other->dstID;
	dstValue = other->dstValue;
	dstIDStoredInVariable = other->dstIDStoredInVariable;
	expression = other->expression;
}
//END BaseEvent::VariableOperation Member Functions

//BEGIN BaseEvent::Wait Member Functions
XMLNode BaseEvent::Wait::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	EventNode.addAttribute("time", QString::number(time).toUtf8().data());
	EventNode.addAttribute("timeStoredInVariable", (timeStoredInVariable) ? "true":"false");
	return EventNode;
}

void BaseEvent::Wait::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(time);
	storageFile.putBool(timeStoredInVariable);
}

QStringList BaseEvent::Wait::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Wait: ";
	if (timeStoredInVariable)
		viewableText[0] += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(time));
	else
		viewableText[0] += QString::number(time);
	viewableText[0] += " decisecond(s)";
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::Wait::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	ConfigureEvent_Wait *configWait = new ConfigureEvent_Wait;
	bool configured = false;
	configWait->setIsStoredTime(timeStoredInVariable);
	configWait->setTime(time);
	if (configWait->exec())
	{
		configured = true;
		time = configWait->getTime();
		timeStoredInVariable = configWait->isStoredTime();
	}
	delete configWait;
	return configured;
}
#endif

bool BaseEvent::Wait::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	if (!XMLTools::attributeExists(EventNode, "time"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "timeStoredInVariable"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "timeStoredInVariable", QStringList() << "true" << "false", 1))
		return false;
	return true;
}

BaseEvent::Wait::Wait(BaseEvent *pBase) : Event(pBase)
{
	time = 0;
	timeStoredInVariable = false;
}

BaseEvent::Wait::Wait(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	time = QString(EventNode.getAttribute("time")).toInt();
	timeStoredInVariable = (QString(EventNode.getAttribute("timeStoredInVariable")) == "true") ? true:false;
}

BaseEvent::Wait::Wait(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	storageFile.putSignedInt(time);
	storageFile.putBool(timeStoredInVariable);
}

BaseEvent::Wait::Wait(Wait *other, BaseEvent *pBase) : Event(pBase)
{
	time = other->time;
	timeStoredInVariable = other->timeStoredInVariable;
}
//END  Wait Member Functions

//BEGIN BaseEvent::WeatherEffects Member Functions
XMLNode BaseEvent::WeatherEffects::getEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("event");
	EventNode.addAttribute("type", "type");
	if (effect == NONE)
		EventNode.addAttribute("effect", "none");
	else if (effect == RAIN)
		EventNode.addAttribute("effect", "rain");
	else if (effect == SNOW)
		EventNode.addAttribute("effect", "snow");
	else if (effect == FOG)
		EventNode.addAttribute("effect", "fog");
	else if (effect == SANDSTORM)
		EventNode.addAttribute("effect", "sandstorm");
	if (effect != NONE)
	{
		if (strength == WEAK)
			EventNode.addAttribute("strength", "weak");
		else if (strength == MEDIUM)
			EventNode.addAttribute("strength", "medium");
		else if (strength == STRONG)
			EventNode.addAttribute("strength", "strong");
	}
	return EventNode;
}

void BaseEvent::WeatherEffects::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(effect);
	storageFile.putSignedInt(strength);
}

QStringList BaseEvent::WeatherEffects::getViewableText(int indent)
{
	QStringList viewableText = QStringList("");
	for (int i = 0; i < indent; ++i)
		viewableText[0] += "  ";
	viewableText[0] += "<> ";
	viewableText[0] += "Weather Effects: ";
	if (effect == NONE)
		viewableText[0] += "None";
	else
	{
		if (strength == WEAK)
			viewableText[0] += "Weak ";
		else if (strength == MEDIUM)
			viewableText[0] += "Medium ";
		else if (strength == STRONG)
			viewableText[0] += "Strong ";
		if (effect == RAIN)
			viewableText[0] += "Rain";
		else if (effect == SNOW)
			viewableText[0] += "Snow";
		else if (effect == FOG)
			viewableText[0] += "Fog";
		else if (effect == SANDSTORM)
			viewableText[0] += "Sandstorm";
	}
	return viewableText;
}

#ifdef COMPILING_EDITOR
bool BaseEvent::WeatherEffects::configureEvent(int callerID, int callerType)
{
	Q_UNUSED(callerID);
	Q_UNUSED(callerType);
	ConfigureEvent_WeatherEffects *configEvent = new ConfigureEvent_WeatherEffects;
	bool configured = false;
	configEvent->setEffect(effect);
	configEvent->setStrength(strength);
	if (configEvent->exec())
	{
		configured = true;
		effect = configEvent->getEffect();
		strength = configEvent->getStrength();
	}
	delete configEvent;
	return configured;
}
#endif

bool BaseEvent::WeatherEffects::tryLoad(XMLNode EventNode, int baseType)
{
	Q_UNUSED(baseType);
	QStringList effectList = QStringList() << "none" << "rain" << "snow" << "fog" << "sandstorm";
	QStringList strengthList = QStringList() << "weak" << "medium" << "strong";
	if (!XMLTools::attributeExists(EventNode, "effect"))
		return false;
	if (!XMLTools::attributeStringValid(EventNode, "effect", effectList))
		return false;
	if (QString(EventNode.getAttribute("effect")) != "none")
	{
		if (!XMLTools::attributeExists(EventNode, "strength"))
			return false;
		if (!XMLTools::attributeStringValid(EventNode, "strength", strengthList))
			return false;
	}
	return true;
}

BaseEvent::WeatherEffects::WeatherEffects(BaseEvent *pBase) : Event(pBase)
{
	effect = NONE;
	strength = MEDIUM;
}

BaseEvent::WeatherEffects::WeatherEffects(XMLNode EventNode, BaseEvent *pBase) : Event(pBase)
{
	QStringList effectList = QStringList() << "none" << "rain" << "snow" << "fog" << "sandstorm";
	QStringList strengthList = QStringList() << "weak" << "medium" << "strong";
	effect = effectList.indexOf(EventNode.getAttribute("effect"));
	if (effect != NONE)
		strength = strengthList.indexOf(EventNode.getAttribute("strength"));
	else
		strength = MEDIUM;
}

BaseEvent::WeatherEffects::WeatherEffects(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase)
{
	effect = storageFile.getSignedInt();
	strength = storageFile.getSignedInt();
}

BaseEvent::WeatherEffects::WeatherEffects(WeatherEffects *other, BaseEvent *pBase) : Event(pBase)
{
	effect = other->effect;
	strength = other->strength;
}
//END BaseEvent::WeatherEffects Member Functions
