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

#ifndef PROJECTDATA_BASEEVENT_H
#define PROJECTDATA_BASEEVENT_H

#include "audiomanager.h"
#include "menuimage.h"
#include "menuitemtable.h"
#include "menutext.h"
#include "projectdata_mapevent.h"
#include "projectdata_terrain.h"
#include "projectdata_tileset.h"
#include "storagefile.h"
#include "xmlParser.h"

class BaseEvent
{
	public:
		enum EventType
		{
			TYPE_TOPLEVELEVENT=0,
			TYPE_ALLOWDISALLOWESCAPE,
			TYPE_ALLOWDISALLOWMAINMENU,
			TYPE_ALLOWDISALLOWSAVING,
			TYPE_ALLOWDISALLOWTELEPORT,
			TYPE_BREAKOUTOFLOOP,
			TYPE_CALLCOMMONEVENT,
			TYPE_CALLMAPEVENT,
			TYPE_CHANGEBATTLECOMMANDS,
			TYPE_CHANGEBORDERTILE,
			TYPE_CHANGECHARACTERBASESTATS,
			TYPE_CHANGECHARACTERCONDITION,
			TYPE_CHANGECHARACTERHP,
			TYPE_CHANGECHARACTERLEVEL,
			TYPE_CHANGECHARACTERMP,
			TYPE_CHANGECHARACTERSPRITE,
			TYPE_CHANGECLASS,
			TYPE_CHANGEENCOUNTERRATE,
			TYPE_CHANGEEQUIPPEDITEMS,
			TYPE_CHANGEEVENTLOCATION,
			TYPE_CHANGEEXPERIENCEPOINTS,
			TYPE_CHANGEFACEGRAPHIC,
			TYPE_CHANGEHERONAME,
			TYPE_CHANGEIMAGE,
			TYPE_CHANGEITEMLIST,
			TYPE_CHANGEMONEY,
			TYPE_CHANGEPARALLAXBACKGROUND,
			TYPE_CHANGEPARTY,
			TYPE_CHANGESELECTEDAREA,
			TYPE_CHANGESKILLS,
			TYPE_CHANGESYSTEMBGM,
			TYPE_CHANGESYSTEMGRAPHICS,
			TYPE_CHANGESYSTEMSOUNDEFFECT,
			TYPE_CHANGETEXT,
			TYPE_CHANGEVEHICLEGRAPHIC,
			TYPE_COMPLETEHEALING,
			TYPE_CONDITIONALBRANCH_MAP,
			TYPE_CONDITIONALBRANCH_MENU,
			TYPE_DISPLAYMESSAGE,
			TYPE_ENABLEDISABLEOBJECT,
			TYPE_ENDEVENTPROCESSING,
			TYPE_ENEMYENCOUNTER,
			TYPE_ERASEEVENT,
			TYPE_EXITMENU,
			TYPE_FADEBGM,
			TYPE_FLASHSCREEN,
			TYPE_FLASHSPRITE_MAP,
			TYPE_GAMEOVER,
			TYPE_HALTALLMOVEMENT,
			TYPE_HEROSPRITETRANSPARENCY,
			TYPE_HIDELAYER,
			TYPE_HIDESCREEN,
			TYPE_HIDESCREENOVERLAY,
			TYPE_INSERTCOMMENT,
			TYPE_ITEMMANAGEMENT,
			TYPE_JUMPTOLABEL,
			TYPE_KEYINPUTPROCESSING,
			TYPE_LABEL,
			TYPE_LOADGAME,
			TYPE_LOOP,
			TYPE_MEMORIZEBGM,
			TYPE_MEMORIZEPOSITION,
			TYPE_MOVEEVENT,
			TYPE_OPENMENU,
			TYPE_PANSCREEN,
			TYPE_PLAYBGM,
			TYPE_PLAYMEMORIZEDBGM,
			TYPE_PLAYMOVIE,
			TYPE_PLAYSOUNDEFFECT,
			TYPE_PLAYSYSTEMBGM,
			TYPE_PLAYSYSTEMSOUNDEFFECT,
			TYPE_PREVIOUSSELECTEDAREABRANCH,
			TYPE_PROCEEDWITHMOVEMENT,
			TYPE_QUITGAME,
			TYPE_SAVEGAME,
			TYPE_SCROLLAREA,
			TYPE_SETSCREENTRANSITION,
			TYPE_SHAKESCREEN,
			TYPE_SHOWBATTLEANIMATION_MAP,
			TYPE_SHOWGLOBALANIMATION,
			TYPE_SHOWLAYER,
			TYPE_SHOWPOPUPMENU,
			TYPE_SHOWSCREEN,
			TYPE_SHOWSCREENOVERLAY,
			TYPE_STOREEVENTID,
			TYPE_STORETERRAINID,
			TYPE_STRINGOPERATION,
			TYPE_SWAPTILE,
			TYPE_SWITCHOPERATION,
			TYPE_TELEPORT,
			TYPE_TILESUBSTITUTION,
			TYPE_TIMEROPERATION,
			TYPE_TINTSCREEN,
			TYPE_TRADETWOEVENTLOCATIONS,
			TYPE_VARIABLEMULTIVALUECONDITIONALBRANCH,
			TYPE_VARIABLEOPERATION,
			TYPE_WAIT,
			TYPE_WEATHEREFFECTS
		};
		class Event
		{
			public:
				static Event *newEvent(BaseEvent *pBase, int type=TYPE_TOPLEVELEVENT);
				static Event *newEvent(XMLNode EventNode, int type, BaseEvent *pBase);
				static Event *newEvent(StorageFile &storageFile, BaseEvent *pBase);
				static Event *newEvent(Event *other, BaseEvent *pBase);
				virtual ~Event() {pBaseEvent = NULL;}
				virtual XMLNode getEventNode()=0;
				virtual void saveToStorageFile(StorageFile &storageFile)=0;
				virtual QStringList getResources()=0;
				virtual QStringList getViewableText(int indent)=0;
				virtual QList<BaseEvent*> getEventPointers() {return QList<BaseEvent*>();}
				virtual int getEventType()=0;
				virtual bool configureEvent(int callerID, int callerType) {Q_UNUSED(callerID);Q_UNUSED(callerType);return true;}
				virtual void addChildEvent(BaseEvent *event, int location, int childTypeID) {Q_UNUSED(event);Q_UNUSED(location);Q_UNUSED(childTypeID);}
				virtual void deleteChildEvent(int location, int childTypeID) {Q_UNUSED(location);Q_UNUSED(childTypeID);}
				virtual void applySwappedTiles(QList<Tileset::SwapTileIDs> swappedTiles) {Q_UNUSED(swappedTiles);}
				virtual void applyChangeMenuObjectName(QString oldName, QString newName) {Q_UNUSED(oldName); Q_UNUSED(newName);}
				static bool tryLoad(XMLNode EventNode, int type, int baseType);
			protected:
				Event(BaseEvent *pBase) {pBaseEvent = pBase;}
				BaseEvent *pBaseEvent;
		};
		class TopLevelEvent : public Event
		{
			public:
				~TopLevelEvent();
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources();
				QStringList getViewableText(int indent);
				QList<BaseEvent*> getEventPointers();
				int getEventType() {return TYPE_TOPLEVELEVENT;}
				void addEvent(BaseEvent *event, int before=-1);
				void deleteEvent(int location);
				BaseEvent *getEvent(int location) {return events[location];}
				int numEvents() {return events.size();}
				void addChildEvent(BaseEvent *event, int location, int childTypeID) {Q_UNUSED(childTypeID);addEvent(event, location);}
				void deleteChildEvent(int location, int childTypeID) {Q_UNUSED(childTypeID);deleteEvent(location);}
				void applySwappedTiles(QList<Tileset::SwapTileIDs> swappedTiles);
				void applyChangeMenuObjectName(QString oldName, QString newName);
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				TopLevelEvent(BaseEvent *pBase) : Event(pBase) {}
				TopLevelEvent(XMLNode EventNode, BaseEvent *pBase);
				TopLevelEvent(StorageFile &storageFile, BaseEvent *pBase);
				TopLevelEvent(TopLevelEvent *other, BaseEvent *pBase);
				QList<BaseEvent*> events;
		};
		class AllowDisallowEscape : public Event
		{
			public:
				~AllowDisallowEscape() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_ALLOWDISALLOWESCAPE;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				bool getAllow() {return allow;}
				void setAllow(bool value) {allow = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				AllowDisallowEscape(BaseEvent *pBase);
				AllowDisallowEscape(XMLNode EventNode, BaseEvent *pBase);
				AllowDisallowEscape(StorageFile &storageFile, BaseEvent *pBase);
				AllowDisallowEscape(AllowDisallowEscape *other, BaseEvent *pBase);
				bool allow;
		};
		class AllowDisallowMainMenu : public Event
		{
			public:
				~AllowDisallowMainMenu() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_ALLOWDISALLOWMAINMENU;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				bool getAllow() {return allow;}
				void setAllow(bool value) {allow = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				AllowDisallowMainMenu(BaseEvent *pBase);
				AllowDisallowMainMenu(XMLNode EventNode, BaseEvent *pBase);
				AllowDisallowMainMenu(StorageFile &storageFile, BaseEvent *pBase);
				AllowDisallowMainMenu(AllowDisallowMainMenu *other, BaseEvent *pBase);
				bool allow;
		};
		class AllowDisallowSaving : public Event
		{
			public:
				~AllowDisallowSaving() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_ALLOWDISALLOWSAVING;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				bool getAllow() {return allow;}
				void setAllow(bool value) {allow = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				AllowDisallowSaving(BaseEvent *pBase);
				AllowDisallowSaving(XMLNode EventNode, BaseEvent *pBase);
				AllowDisallowSaving(StorageFile &storageFile, BaseEvent *pBase);
				AllowDisallowSaving(AllowDisallowSaving *other, BaseEvent *pBase);
				bool allow;
		};
		class AllowDisallowTeleport : public Event
		{
			public:
				~AllowDisallowTeleport() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_ALLOWDISALLOWTELEPORT;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				bool getAllow() {return allow;}
				void setAllow(bool value) {allow = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				AllowDisallowTeleport(BaseEvent *pBase);
				AllowDisallowTeleport(XMLNode EventNode, BaseEvent *pBase);
				AllowDisallowTeleport(StorageFile &storageFile, BaseEvent *pBase);
				AllowDisallowTeleport(AllowDisallowTeleport *other, BaseEvent *pBase);
				bool allow;
		};
		class BreakOutOfLoop : public Event
		{
			public:
				~BreakOutOfLoop() {}
				XMLNode getEventNode()
				{
					XMLNode EventNode = XMLNode::createXMLTopNode("event");
					EventNode.addAttribute("type", "type");
					return EventNode;
				}
				void saveToStorageFile(StorageFile &storageFile) {Q_UNUSED(storageFile);}
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent)
				{
					QStringList viewableText = QStringList("");
					for (int i = 0; i < indent; ++i)
						viewableText[0] += "  ";
					viewableText[0] += "<> ";
					viewableText[0] += "Break Out Of Loop";
					return viewableText;
				}
				int getEventType() {return TYPE_BREAKOUTOFLOOP;}
				static bool tryLoad(XMLNode EventNode, int baseType) {Q_UNUSED(EventNode);Q_UNUSED(baseType);return true;}
				friend class Event;
			private:
				BreakOutOfLoop(BaseEvent *pBase) : Event(pBase) {}
				BreakOutOfLoop(XMLNode EventNode, BaseEvent *pBase) : Event(pBase) {Q_UNUSED(EventNode);}
				BreakOutOfLoop(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase) {Q_UNUSED(storageFile);}
				BreakOutOfLoop(BreakOutOfLoop *other, BaseEvent *pBase) : Event(pBase) {Q_UNUSED(other);}
		};
		class CallCommonEvent : public Event
		{
			public:
				~CallCommonEvent() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_CALLCOMMONEVENT;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				int getEventID() {return eventID;}
				void setEventID(int id) {eventID = id;}
				bool isEventReferencedInVariable() {return eventReferencedInVariable;}
				void setEventReferencedInVariable(bool stored) {eventReferencedInVariable = stored;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				CallCommonEvent(BaseEvent *pBase);
				CallCommonEvent(XMLNode EventNode, BaseEvent *pBase);
				CallCommonEvent(StorageFile &storageFile, BaseEvent *pBase);
				CallCommonEvent(CallCommonEvent *other, BaseEvent *pBase);
				int eventID;
				bool eventReferencedInVariable;
		};
		class CallMapEvent : public Event
		{
			public:
				~CallMapEvent() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_CALLMAPEVENT;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				int getEventID() {return eventID;}
				void setEventID(int value) {eventID = value;}
				int getPageID() {return pageID;}
				void setPageID(int value) {pageID = value;}
				bool isEventReferencedInVariable() {return eventReferencedInVariable;}
				void setEventReferencedInVariable(bool value) {eventReferencedInVariable = value;}
				bool isPageReferencedInVariable() {return pageReferencedInVariable;}
				void setPageReferencedInVariable(bool value) {pageReferencedInVariable = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				CallMapEvent(BaseEvent *pBase);
				CallMapEvent(XMLNode EventNode, BaseEvent *pBase);
				CallMapEvent(StorageFile &storageFile, BaseEvent *pBase);
				CallMapEvent(CallMapEvent *other, BaseEvent *pBase);
				int eventID;
				int pageID;
				bool eventReferencedInVariable;
				bool pageReferencedInVariable;
		};
		class ChangeBattleCommands : public Event
		{
			public:
				enum Operation {REMOVE=0, ADD};
				~ChangeBattleCommands() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_CHANGEBATTLECOMMANDS;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				int getCharacterID() {return characterID;}
				void setCharacterID(int value) {characterID = value;}
				int getCommandID() {return commandID;}
				void setCommandID(int value) {commandID = value;}
				int getOperation() {return operation;}
				void setOperation(int value) {operation = value;}
				bool isCharacterReferencedInVariable() {return characterReferencedInVariable;}
				void setCharacterReferencedInVariable(bool value) {characterReferencedInVariable = value;}
				bool isCommandReferencedInVariable() {return commandReferencedInVariable;}
				void setCommandReferencedInVariable(bool value) {commandReferencedInVariable = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				ChangeBattleCommands(BaseEvent *pBase);
				ChangeBattleCommands(XMLNode EventNode, BaseEvent *pBase);
				ChangeBattleCommands(StorageFile &storageFile, BaseEvent *pBase);
				ChangeBattleCommands(ChangeBattleCommands *other, BaseEvent *pBase);
				int characterID;
				int commandID;
				int operation;
				bool characterReferencedInVariable;
				bool commandReferencedInVariable;
		};
		class ChangeBorderTile : public Event
		{
			public:
				~ChangeBorderTile() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_CHANGEBORDERTILE;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				int getTilesetID() {return tilesetID;}
				void setTilesetID(int value) {tilesetID = value;}
				int getTileID() {return tileID;}
				void setTileID(int value) {tileID = value;}
				bool isTileReferencedInVariables() {return tileReferencedInVariables;}
				void setIsTileReferencedInVariables(bool value) {tileReferencedInVariables = value;}
				void applySwappedTiles(QList<Tileset::SwapTileIDs> swappedTiles);
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				ChangeBorderTile(BaseEvent *pBase);
				ChangeBorderTile(XMLNode EventNode, BaseEvent *pBase);
				ChangeBorderTile(StorageFile &storageFile, BaseEvent *pBase);
				ChangeBorderTile(ChangeBorderTile *other, BaseEvent *pBase);
				int tilesetID;
				int tileID;
				bool tileReferencedInVariables;
		};
		class ChangeCharacterBaseStats : public Event
		{
			public:
				enum Target {ENTIRE_PARTY=0, SPECIFIC_CHARACTER, REFERENCED_CHARACTER};
				enum Operation {INCREASE_STAT=0, DECREASE_STAT};
				enum Stat {MAX_HP=0, MAX_MP, ATTACK, DEFENSE, WISDOM, AGILITY};
				~ChangeCharacterBaseStats() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_CHANGECHARACTERBASESTATS;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				int getCharacterID() {return characterID;}
				void setCharacterID(int value) {characterID = value;}
				int getAmount() {return amount;}
				void setAmount(int value) {amount = value;}
				int getTarget() {return target;}
				void setTarget(int value) {target = value;}
				int getOperation() {return operation;}
				void setOperation(int value) {operation = value;}
				int getStat() {return stat;}
				void setStat(int value) {stat = value;}
				bool isAmountStoredInVariable() {return amountStoredInVariable;}
				void setAmountStoredInVariable(bool value) {amountStoredInVariable = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				ChangeCharacterBaseStats(BaseEvent *pBase);
				ChangeCharacterBaseStats(XMLNode EventNode, BaseEvent *pBase);
				ChangeCharacterBaseStats(StorageFile &storageFile, BaseEvent *pBase);
				ChangeCharacterBaseStats(ChangeCharacterBaseStats *other, BaseEvent *pBase);
				int characterID;
				int amount;
				int target;
				int operation;
				int stat;
				bool amountStoredInVariable;
		};
		class ChangeCharacterCondition : public Event
		{
			public:
				enum Target {ENTIRE_PARTY=0, SPECIFIC_CHARACTER, REFERENCED_CHARACTER};
				enum Operation {ADD_CONDITION=0, REMOVE_CONDITION};
				~ChangeCharacterCondition() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_CHANGECHARACTERCONDITION;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				int getCharacterID() {return characterID;}
				void setCharacterID(int value) {characterID = value;}
				int getConditionID() {return conditionID;}
				void setConditionID(int value) {conditionID = value;}
				int getTarget() {return target;}
				void setTarget(int value) {target = value;}
				int getOperation() {return operation;}
				void setOperation(int value) {operation = value;}
				bool isConditionReferencedInVariable() {return conditionReferencedInVariable;}
				void setConditionReferencedInVariable(bool value) {conditionReferencedInVariable = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				ChangeCharacterCondition(BaseEvent *pBase);
				ChangeCharacterCondition(XMLNode EventNode, BaseEvent *pBase);
				ChangeCharacterCondition(StorageFile &storageFile, BaseEvent *pBase);
				ChangeCharacterCondition(ChangeCharacterCondition *other, BaseEvent *pBase);
				int characterID;
				int conditionID;
				int target;
				int operation;
				bool conditionReferencedInVariable;
		};
		class ChangeCharacterHP : public Event
		{
			public:
				enum Target {ENTIRE_PARTY=0, SPECIFIC_CHARACTER, REFERENCED_CHARACTER};
				enum Operation {INCREASE_HP=0, DECREASE_HP};
				~ChangeCharacterHP() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_CHANGECHARACTERHP;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				int getCharacterID() {return characterID;}
				void setCharacterID(int value) {characterID = value;}
				int getAmount() {return amount;}
				void setAmount(int value) {amount = value;}
				int getTarget() {return target;}
				void setTarget(int value) {target = value;}
				int getOperation() {return operation;}
				void setOperation(int value) {operation = value;}
				bool isAmountStoredInVariable() {return amountStoredInVariable;}
				void setAmountStoredInVariable(bool value) {amountStoredInVariable = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				ChangeCharacterHP(BaseEvent *pBase);
				ChangeCharacterHP(XMLNode EventNode, BaseEvent *pBase);
				ChangeCharacterHP(StorageFile &storageFile, BaseEvent *pBase);
				ChangeCharacterHP(ChangeCharacterHP *other, BaseEvent *pBase);
				int characterID;
				int amount;
				int target;
				int operation;
				bool amountStoredInVariable;
		};
		class ChangeCharacterLevel : public Event
		{
			public:
				enum Target {ENTIRE_PARTY=0, SPECIFIC_CHARACTER, REFERENCED_CHARACTER};
				enum Operation {ADD_LEVELS=0, REMOVE_LEVELS};
				~ChangeCharacterLevel() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_CHANGECHARACTERLEVEL;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				int getCharacterID() {return characterID;}
				void setCharacterID(int value) {characterID = value;}
				int getAmount() {return amount;}
				void setAmount(bool value) {amount = value;}
				int getTarget() {return target;}
				void setTarget(int value) {target = value;}
				int getOperation() {return operation;}
				void setOperation(int value) {operation = value;}
				bool isAmountStoredInVariable() {return amountStoredInVariable;}
				void setAmountStoredInVariable(bool value) {amountStoredInVariable = value;}
				bool getShowLevelUpMessage() {return showLevelUpMessage;}
				void setShowLevelUpMessage(bool value) {showLevelUpMessage = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				ChangeCharacterLevel(BaseEvent *pBase);
				ChangeCharacterLevel(XMLNode EventNode, BaseEvent *pBase);
				ChangeCharacterLevel(StorageFile &storageFile, BaseEvent *pBase);
				ChangeCharacterLevel(ChangeCharacterLevel *other, BaseEvent *pBase);
				int characterID;
				int amount;
				int target;
				int operation;
				bool amountStoredInVariable;
				bool showLevelUpMessage;
		};
		class ChangeCharacterMP : public Event
		{
			public:
				enum Target {ENTIRE_PARTY=0, SPECIFIC_CHARACTER, REFERENCED_CHARACTER};
				enum Operation {INCREASE_MP=0, DECREASE_MP};
				~ChangeCharacterMP() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_CHANGECHARACTERMP;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				int getCharacterID() {return characterID;}
				void setCharacterID(int value) {characterID = value;}
				int getAmount() {return amount;}
				void setAmount(int value) {amount = value;}
				int getTarget() {return target;}
				void setTarget(int value) {target = value;}
				int getOperation() {return operation;}
				void setOperation(int value) {operation = value;}
				bool isAmountStoredInVariable() {return amountStoredInVariable;}
				void setAmountStoredInVariable(bool value) {amountStoredInVariable = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				ChangeCharacterMP(BaseEvent *pBase);
				ChangeCharacterMP(XMLNode EventNode, BaseEvent *pBase);
				ChangeCharacterMP(StorageFile &storageFile, BaseEvent *pBase);
				ChangeCharacterMP(ChangeCharacterMP *other, BaseEvent *pBase);
				int characterID;
				int amount;
				int target;
				int operation;
				bool amountStoredInVariable;
		};
		class ChangeCharacterSprite : public Event
		{
			public:
				~ChangeCharacterSprite() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_CHANGECHARACTERSPRITE;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				int getCharacterID() {return characterID;}
				void setCharacterID(int value) {characterID = value;}
				int getSpriteID() {return spriteID;}
				void setSpriteID(int value) {spriteID = value;}
				bool isCharacterReferencedInVariable() {return characterReferencedInVariable;}
				void setCharacterReferencedInVariable(bool value) {characterReferencedInVariable = value;}
				bool isSpriteReferencedInVariable() {return spriteReferencedInVariable;}
				void setSpriteReferencedInVariable(bool value) {spriteReferencedInVariable = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				ChangeCharacterSprite(BaseEvent *pBase);
				ChangeCharacterSprite(XMLNode EventNode, BaseEvent *pBase);
				ChangeCharacterSprite(StorageFile &storageFile, BaseEvent *pBase);
				ChangeCharacterSprite(ChangeCharacterSprite *other, BaseEvent *pBase);
				int characterID;
				int spriteID;
				bool characterReferencedInVariable;
				bool spriteReferencedInVariable;
		};
		class ChangeClass : public Event
		{
			public:
				enum LearnedSkills {LEARNED_NONE=0, LEARNED_ADDUPTONEWDELETEOLD, LEARNED_ADDUPTONEWKEEPOLD};
				enum BaseStatChange {CHANGE_NONE=0, CHANGE_HALVESTATS, CHANGE_TOLEVEL1OFCLASS, CHANGE_TOCURRENTLEVELOFCLASS};
				~ChangeClass() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_CHANGECLASS;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				int getCharacterID() {return characterID;}
				void setCharacterID(int value) {characterID = value;}
				int getClassID() {return classID;}
				void setClassID(int value) {classID = value;}
				int getLearnedSkills() {return learnedSkills;}
				void setLearnedSkills(int value) {learnedSkills = value;}
				int getBaseStatChange() {return baseStatChange;}
				void setBaseStatChange(int value) {baseStatChange = value;}
				bool isCharacterReferencedInVariable() {return characterReferencedInVariable;}
				void setCharacterReferencedInVariable(bool value) {characterReferencedInVariable = value;}
				bool isClassReferencedInVariable() {return classReferencedInVariable;}
				void setClassReferencedInVariable(bool value) {classReferencedInVariable = value;}
				bool getRetainCurrentLevel() {return retainCurrentLevel;}
				void setRetainCurrentLevel(bool value) {retainCurrentLevel = value;}
				bool getShowLevelUpMessage() {return showLevelUpMessage;}
				void setShowLevelUpMessage(bool value) {showLevelUpMessage = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				ChangeClass(BaseEvent *pBase);
				ChangeClass(XMLNode EventNode, BaseEvent *pBase);
				ChangeClass(StorageFile &storageFile, BaseEvent *pBase);
				ChangeClass(ChangeClass *other, BaseEvent *pBase);
				int characterID;
				int classID;
				int learnedSkills;
				int baseStatChange;
				bool characterReferencedInVariable;
				bool classReferencedInVariable;
				bool retainCurrentLevel;
				bool showLevelUpMessage;
		};
		class ChangeEncounterRate : public Event
		{
			public:
				~ChangeEncounterRate() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_CHANGEENCOUNTERRATE;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				int getEncounterRate() {return encounterRate;}
				void setEncounterRate(int value) {encounterRate = value;}
				int getMinSteps() {return minSteps;}
				void setMinSteps(int value) {minSteps = value;}
				int getMaxSteps() {return maxSteps;}
				void setMaxSteps(int value) {maxSteps = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				ChangeEncounterRate(BaseEvent *pBase);
				ChangeEncounterRate(XMLNode EventNode, BaseEvent *pBase);
				ChangeEncounterRate(StorageFile &storageFile, BaseEvent *pBase);
				ChangeEncounterRate(ChangeEncounterRate *other, BaseEvent *pBase);
				int encounterRate;
				int minSteps;
				int maxSteps;
		};
		class ChangeEquippedItems : public Event
		{
			public:
				enum Target {ENTIRE_PARTY=0, SPECIFIC_CHARACTER, REFERENCED_CHARACTER};
				enum Operation {EQUIP_ITEM=0, UNEQUIP_ITEM};
				enum UnequippedSlot {ALL=0, WEAPON, SHIELD, ARMOR, HELMET, ACCESSORY};
				~ChangeEquippedItems() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_CHANGEEQUIPPEDITEMS;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				int getCharacterID() {return characterID;}
				void setCharacterID(int value) {characterID = value;}
				int getItemID() {return itemID;}
				void setItemID(int value) {itemID = value;}
				int getTarget() {return target;}
				void setTarget(int value) {target = value;}
				int getOperation() {return operation;}
				void setOperation(int value) {operation = value;}
				int getUnequippedSlot() {return unequippedSlot;}
				void setUnequippedSlot(int value) {unequippedSlot = value;}
				bool isItemReferencedInVariable() {return itemReferencedInVariable;}
				void setItemReferencedInVariable(bool value) {itemReferencedInVariable = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				ChangeEquippedItems(BaseEvent *pBase);
				ChangeEquippedItems(XMLNode EventNode, BaseEvent *pBase);
				ChangeEquippedItems(StorageFile &storageFile, BaseEvent *pBase);
				ChangeEquippedItems(ChangeEquippedItems *other, BaseEvent *pBase);
				int characterID;
				int itemID;
				int target;
				int operation;
				int unequippedSlot;
				bool itemReferencedInVariable;
		};
		class ChangeEventLocation : public Event
		{
			public:
				~ChangeEventLocation() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_CHANGEEVENTLOCATION;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				int getEventID() {return eventID;}
				void setEventID(int value) {eventID = value;}
				int getX() {return x;}
				void setX(int value) {x = value;}
				int getY() {return y;}
				void setY(int value) {y = value;}
				int getLayer() {return layer;}
				void setLayer(int value) {layer = value;}
				bool isEventReferencedInVariable() {return eventReferencedInVariable;}
				void setEventReferencedInVariable(bool value) {eventReferencedInVariable = value;}
				bool isLocationStoredInVariables() {return locationStoredInVariables;}
				void setLocationStoredInVariables(bool value) {locationStoredInVariables = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				ChangeEventLocation(BaseEvent *pBase);
				ChangeEventLocation(XMLNode EventNode, BaseEvent *pBase);
				ChangeEventLocation(StorageFile &storageFile, BaseEvent *pBase);
				ChangeEventLocation(ChangeEventLocation *other, BaseEvent *pBase);
				int eventID;
				int x;
				int y;
				int layer;
				bool eventReferencedInVariable;
				bool locationStoredInVariables;
		};
		class ChangeExperiencePoints : public Event
		{
			public:
				enum Target {ENTIRE_PARTY=0, SPECIFIC_CHARACTER, REFERENCED_CHARACTER};
				enum Operation {INCREASE_EXPERIENCE=0, REDUCE_EXPERIENCE};
				~ChangeExperiencePoints() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_CHANGEEXPERIENCEPOINTS;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				int getCharacterID() {return characterID;}
				void setCharacterID(int value) {characterID = value;}
				int getAmount() {return amount;}
				void setAmount(int value) {amount = value;}
				int getTarget() {return target;}
				void setTarget(int value) {target = value;}
				int getOperation() {return operation;}
				void setOperation(int value) {operation = value;}
				bool isAmountStoredInVariable() {return amountStoredInVariable;}
				void setAmountStoredInVariable(bool value) {amountStoredInVariable = value;}
				bool getShowLevelUpMessage() {return showLevelUpMessage;}
				void setShowLevelUpMessage(bool value) {showLevelUpMessage = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				ChangeExperiencePoints(BaseEvent *pBase);
				ChangeExperiencePoints(XMLNode EventNode, BaseEvent *pBase);
				ChangeExperiencePoints(StorageFile &storageFile, BaseEvent *pBase);
				ChangeExperiencePoints(ChangeExperiencePoints *other, BaseEvent *pBase);
				int characterID;
				int amount;
				int target;
				int operation;
				bool amountStoredInVariable;
				bool showLevelUpMessage;
		};
		class ChangeFaceGraphic : public Event
		{
			public:
				~ChangeFaceGraphic() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources();
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_CHANGEFACEGRAPHIC;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				QString getFaceLocation() {return faceLocation;}
				void setFaceLocation(QString value) {faceLocation = value;}
				int getCharacterID() {return characterID;}
				void setCharacterID(int value) {characterID = value;}
				bool isCharacterReferencedInVariable() {return characterReferencedInVariable;}
				void setCharacterReferencedInVariable(bool value) {characterReferencedInVariable = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				ChangeFaceGraphic(BaseEvent *pBase);
				ChangeFaceGraphic(XMLNode EventNode, BaseEvent *pBase);
				ChangeFaceGraphic(StorageFile &storageFile, BaseEvent *pBase);
				ChangeFaceGraphic(ChangeFaceGraphic *other, BaseEvent *pBase);
				QString faceLocation;
				int characterID;
				bool characterReferencedInVariable;
		};
		class ChangeHeroName : public Event
		{
			public:
				~ChangeHeroName() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_CHANGEHERONAME;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				QString getNewName() {return newName;}
				void setNewName(QString value) {newName = value;}
				int getCharacterID() {return characterID;}
				void setCharacterID(int value) {characterID = value;}
				int getStringID() {return stringID;}
				void setStringID(int value) {stringID = value;}
				bool isCharacterReferencedInVariable() {return characterReferencedInVariable;}
				void setCharacterReferencedInVariable(bool value) {characterReferencedInVariable = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				ChangeHeroName(BaseEvent *pBase);
				ChangeHeroName(XMLNode EventNode, BaseEvent *pBase);
				ChangeHeroName(StorageFile &storageFile, BaseEvent *pBase);
				ChangeHeroName(ChangeHeroName *other, BaseEvent *pBase);
				QString newName;
				int characterID;
				int stringID;
				bool characterReferencedInVariable;
		};
		class ChangeImage : public Event
		{
			public:
				~ChangeImage() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources();
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_CHANGEIMAGE;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				QString getObjectName() {return objectName;}
				void setObjectName(QString value) {objectName = value;}
				void getImageLocation(MenuImage::PartyMemberImage *image);
				void getImageLocation(MenuImage::CharacterImage *image);
				void getImageLocation(MenuImage::ClassImage *image);
				void getImageLocation(MenuImage::MonsterImage *image);
				void getImageLocation(MenuImage::VehicleImage *image);
				void getImageLocation(MenuImage::TileImage *image);
				void getImageLocation(QString *fileLocation);
				void setImageLocation(MenuImage::PartyMemberImage *image);
				void setImageLocation(MenuImage::CharacterImage *image);
				void setImageLocation(MenuImage::ClassImage *image);
				void setImageLocation(MenuImage::MonsterImage *image);
				void setImageLocation(MenuImage::VehicleImage *image);
				void setImageLocation(MenuImage::TileImage *image);
				void setImageLocation(QString fileLocation);
				int getImageType() {return imageType;}
				void setImageType(int value) {imageType = value;}
				int getSaveSlotID() {return saveSlotID;}
				void setSaveSlotID(int value) {saveSlotID = value;}
				void applySwappedTiles(QList<Tileset::SwapTileIDs> swappedTiles);
				void applyChangeMenuObjectName(QString oldName, QString newName) {if (objectName == oldName) objectName = newName;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				ChangeImage(BaseEvent *pBase);
				ChangeImage(XMLNode EventNode, BaseEvent *pBase);
				ChangeImage(StorageFile &storageFile, BaseEvent *pBase);
				ChangeImage(ChangeImage *other, BaseEvent *pBase);
				QString objectName;
				MenuImage::ImageLocation imageLocation;
				int imageType;
				int saveSlotID;
		};
		class ChangeItemList : public Event
		{
			public:
				~ChangeItemList() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_CHANGEITEMLIST;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				QString getObjectName() {return objectName;}
				void setObjectName(QString value) {objectName = value;}
				void getSettings(MenuItemTable::ItemSettings *value);
				void getSettings(MenuItemTable::SkillSettings *value);
				void getSettings(MenuItemTable::EquipmentSettings *value);
				void getSettings(MenuItemTable::OtherSettings *value);
				void setSettings(MenuItemTable::ItemSettings *value);
				void setSettings(MenuItemTable::SkillSettings *value);
				void setSettings(MenuItemTable::EquipmentSettings *value);
				void setSettings(MenuItemTable::OtherSettings *value);
				int getTableType() {return tableType;}
				void setTableType(int value);
				void applyChangeMenuObjectName(QString oldName, QString newName) {if (objectName == oldName) objectName = newName;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				ChangeItemList(BaseEvent *pBase);
				ChangeItemList(XMLNode EventNode, BaseEvent *pBase);
				ChangeItemList(StorageFile &storageFile, BaseEvent *pBase);
				ChangeItemList(ChangeItemList *other, BaseEvent *pBase);
				QString objectName;
				MenuItemTable::Settings settings;
				int tableType;
		};
		class ChangeMoney : public Event
		{
			public:
				enum ChangeToMake {INCREASE_MONEY=0, DECREASE_MONEY};
				~ChangeMoney() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_CHANGEMONEY;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				long getAmount() {return amount;}
				void setAmount(long value) {amount = value;}
				int getChangeToMake() {return changeToMake;}
				void setChangeToMake(int value) {changeToMake = value;}
				bool isAmountStoredInVariable() {return amountStoredInVariable;}
				void setAmountStoredInVariable(bool value) {amountStoredInVariable = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				ChangeMoney(BaseEvent *pBase);
				ChangeMoney(XMLNode EventNode, BaseEvent *pBase);
				ChangeMoney(StorageFile &storageFile, BaseEvent *pBase);
				ChangeMoney(ChangeMoney *other, BaseEvent *pBase);
				long amount;
				int changeToMake;
				bool amountStoredInVariable;
		};
		class ChangeParallaxBackground : public Event
		{
			public:
				~ChangeParallaxBackground() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList() << bgLocation;}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_CHANGEPARALLAXBACKGROUND;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				QString getBGLocation() {return bgLocation;}
				void setBGLocation(QString value) {bgLocation = value;}
				int getHScrollSpeed() {return hScrollSpeed;}
				void setHScrollSpeed(int value) {hScrollSpeed = value;}
				int getVScrollSpeed() {return vScrollSpeed;}
				void setVScrollSpeed(int value) {vScrollSpeed = value;}
				bool getAutoHScroll() {return autoHScroll;}
				void setAutoHScroll(bool value) {autoHScroll = value;}
				bool getAutoVScroll() {return autoVScroll;}
				void setAutoVScroll(bool value) {autoVScroll = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				ChangeParallaxBackground(BaseEvent *pBase);
				ChangeParallaxBackground(XMLNode EventNode, BaseEvent *pBase);
				ChangeParallaxBackground(StorageFile &storageFile, BaseEvent *pBase);
				ChangeParallaxBackground(ChangeParallaxBackground *other, BaseEvent *pBase);
				QString bgLocation;
				int hScrollSpeed;
				int vScrollSpeed;
				bool autoHScroll;
				bool autoVScroll;
		};
		class ChangeParty : public Event
		{
			public:
				enum ChangeToMake {ADD_MEMBER=0, REMOVE_MEMBER};
				~ChangeParty() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_CHANGEPARTY;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				int getCharacterID() {return characterID;}
				void setCharacterID(int value) {characterID = value;}
				int getChangeToMake() {return changeToMake;}
				void setChangeToMake(int value) {changeToMake = value;}
				bool isCharacterReferencedInVariable() {return characterReferencedInVariable;}
				void setCharacterReferencedInVariable(bool value) {characterReferencedInVariable = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				ChangeParty(BaseEvent *pBase);
				ChangeParty(XMLNode EventNode, BaseEvent *pBase);
				ChangeParty(StorageFile &storageFile, BaseEvent *pBase);
				ChangeParty(ChangeParty *other, BaseEvent *pBase);
				int characterID;
				int changeToMake;
				bool characterReferencedInVariable;
		};
		class ChangeSelectedArea : public Event
		{
			public:
				~ChangeSelectedArea() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_CHANGESELECTEDAREA;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				QString getObjectName() {return objectName;}
				void setObjectName(QString value) {objectName = value;}
				void applyChangeMenuObjectName(QString oldName, QString newName) {if (objectName == oldName) objectName = newName;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				ChangeSelectedArea(BaseEvent *pBase);
				ChangeSelectedArea(XMLNode EventNode, BaseEvent *pBase);
				ChangeSelectedArea(StorageFile &storageFile, BaseEvent *pBase);
				ChangeSelectedArea(ChangeSelectedArea *other, BaseEvent *pBase);
				QString objectName;
		};
		class ChangeSkills : public Event
		{
			public:
				enum Target {ENTIRE_PARTY=0, SPECIFIC_CHARACTER, REFERENCED_CHARACTER};
				enum Operation {ADD_SKILL=0, REMOVE_SKILL};
				~ChangeSkills() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_CHANGESKILLS;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				int getCharacterID() {return characterID;}
				void setCharacterID(int value) {characterID = value;}
				int getSkillID() {return skillID;}
				void setSkillID(int value) {skillID = value;}
				int getTarget() {return target;}
				void setTarget(int value) {target = value;}
				int getOperation() {return operation;}
				void setOperation(int value) {operation = value;}
				bool isSkillReferencedInVariable() {return skillReferencedInVariable;}
				void setSkillReferencedInVariable(bool value) {skillReferencedInVariable = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				ChangeSkills(BaseEvent *pBase);
				ChangeSkills(XMLNode EventNode, BaseEvent *pBase);
				ChangeSkills(StorageFile &storageFile, BaseEvent *pBase);
				ChangeSkills(ChangeSkills *other, BaseEvent *pBase);
				int characterID;
				int skillID;
				int target;
				int operation;
				bool skillReferencedInVariable;
		};
		class ChangeSystemBGM : public Event
		{
			public:
				~ChangeSystemBGM() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources();
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_CHANGESYSTEMBGM;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				void getNewBGM(AudioManager::MusicInfo *other);
				void setNewBGM(AudioManager::MusicInfo *other);
				int getBGMToChange() {return bgmToChange;}
				void setBGMToChange(int value) {bgmToChange = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				ChangeSystemBGM(BaseEvent *pBase);
				ChangeSystemBGM(XMLNode EventNode, BaseEvent *pBase);
				ChangeSystemBGM(StorageFile &storageFile, BaseEvent *pBase);
				ChangeSystemBGM(ChangeSystemBGM *other, BaseEvent *pBase);
				AudioManager::MusicInfo newBGM;
				int bgmToChange;
		};
		class ChangeSystemGraphics : public Event
		{
			public:
				enum GraphicToChange {FRAME_BG, FRAME_BORDER, SELECTION_BORDER, EQUIPMENT_INDICATORS, MENU_BG, SCROLL_ARROWS, GRAPPLE_ARM, GRAPPLE_HOOK, TIMER_NUMBERS}; 
				~ChangeSystemGraphics() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources();
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_CHANGESYSTEMGRAPHICS;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				QString getNewGraphicLocation() {return newGraphicLocation;}
				void setNewGraphicLocation(QString value) {newGraphicLocation = value;}
				int getGraphicToChange() {return graphicToChange;}
				void setGraphicToChange(int value) {graphicToChange = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				ChangeSystemGraphics(BaseEvent *pBase);
				ChangeSystemGraphics(XMLNode EventNode, BaseEvent *pBase);
				ChangeSystemGraphics(StorageFile &storageFile, BaseEvent *pBase);
				ChangeSystemGraphics(ChangeSystemGraphics *other, BaseEvent *pBase);
				QString newGraphicLocation;
				int graphicToChange;
		};
		class ChangeSystemSoundEffect : public Event
		{
			public:
				~ChangeSystemSoundEffect() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources();
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_CHANGESYSTEMSOUNDEFFECT;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				void getNewSoundEffect(AudioManager::SoundEffectInfo *other);
				void setNewSoundEffect(AudioManager::SoundEffectInfo *other);
				int getSoundEffectToChange() {return soundEffectToChange;}
				void setSoundEffectToChange(int value) {soundEffectToChange = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				ChangeSystemSoundEffect(BaseEvent *pBase);
				ChangeSystemSoundEffect(XMLNode EventNode, BaseEvent *pBase);
				ChangeSystemSoundEffect(StorageFile &storageFile, BaseEvent *pBase);
				ChangeSystemSoundEffect(ChangeSystemSoundEffect *other, BaseEvent *pBase);
				AudioManager::SoundEffectInfo newSoundEffect;
				int soundEffectToChange;
		};
		class ChangeText : public Event
		{
			public:
				~ChangeText() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_CHANGETEXT;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				QString getObjectName() {return objectName;}
				void setObjectName(QString value) {objectName = value;}
				void getText(QString *value);
				void getText(MenuText::PartyMemberText *value);
				void getText(MenuText::MonsterText *value);
				void getText(MenuText::ItemText *value);
				void getText(MenuText::CharacterText *value);
				void getText(MenuText::VariableText *value);
				void getText(MenuText::SwitchText *value);
				void getText(MenuText::StringText *value);
				void setText(QString value);
				void setText(MenuText::PartyMemberText *value);
				void setText(MenuText::MonsterText *value);
				void setText(MenuText::ItemText *value);
				void setText(MenuText::CharacterText *value);
				void setText(MenuText::VariableText *value);
				void setText(MenuText::SwitchText *value);
				void setText(MenuText::StringText *value);
				int getTextType() {return textType;}
				void setTextType(int value);
				int getSaveSlotID() {return saveSlotID;}
				void setSaveSlotID(int value) {saveSlotID = value;}
				void applyChangeMenuObjectName(QString oldName, QString newName) {if (objectName == oldName) objectName = newName;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				ChangeText(BaseEvent *pBase);
				ChangeText(XMLNode EventNode, BaseEvent *pBase);
				ChangeText(StorageFile &storageFile, BaseEvent *pBase);
				ChangeText(ChangeText *other, BaseEvent *pBase);
				QString objectName;
				MenuText::TextData textData;
				int textType;
				int saveSlotID;
		};
		class ChangeVehicleGraphic : public Event
		{
			public:
				~ChangeVehicleGraphic() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources();
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_CHANGEVEHICLEGRAPHIC;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				int getVehicleID() {return vehicleID;}
				void setVehicleID(int value) {vehicleID = value;}
				int getSpriteID() {return spriteID;}
				void setSpriteID(int value) {spriteID = value;}
				bool isVehicleReferencedInVariable() {return vehicleReferencedInVariable;}
				void setVehicleReferencedInVariable(bool value) {vehicleReferencedInVariable = value;}
				bool isSpriteReferencedInVariable() {return spriteReferencedInVariable;}
				void setSpriteReferencedInVariable(bool value) {spriteReferencedInVariable = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				ChangeVehicleGraphic(BaseEvent *pBase);
				ChangeVehicleGraphic(XMLNode EventNode, BaseEvent *pBase);
				ChangeVehicleGraphic(StorageFile &storageFile, BaseEvent *pBase);
				ChangeVehicleGraphic(ChangeVehicleGraphic *other, BaseEvent *pBase);
				QString spriteLocation;
				int vehicleID;
				int spriteID;
				bool vehicleReferencedInVariable;
				bool spriteReferencedInVariable;
		};
		class CompleteHealing : public Event
		{
			enum Target {ENTIRE_PARTY=0, SPECIFIC_CHARACTER, REFERENCED_CHARACTER};
			public:
				~CompleteHealing() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_COMPLETEHEALING;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				int getCharacterID() {return characterID;}
				void setCharacterID(int value) {characterID = value;}
				int getTarget() {return target;}
				void setTarget(int value) {target = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				CompleteHealing(BaseEvent *pBase);
				CompleteHealing(XMLNode EventNode, BaseEvent *pBase);
				CompleteHealing(StorageFile &storageFile, BaseEvent *pBase);
				CompleteHealing(CompleteHealing *other, BaseEvent *pBase);
				int characterID;
				int target;
		};
		class ConditionalBranch_Map : public Event
		{
			public:
				enum ConditionsToMatch {ALL=0, AT_LEAST, NONE};
				struct SwitchCondition
				{
					int switchID;
					bool switchReferencedInVariable;
					bool isOn;
				};
				struct VariableCondition
				{
					enum Comparison {EQUAL_TO=0, NOT_EQUAL_TO, GREATER_THAN, GREATER_THAN_OR_EQUAL_TO, LESS_THAN, LESS_THAN_OR_EQUAL_TO};
					int srcVariableID;
					int dstValue;
					int comparison;
					bool dstValueStoredInVariable;
				};
				struct StringCondition
				{
					enum Comparison {EQUAL_TO=0, NOT_EQUAL_TO, STARTS_WITH, ENDS_WITH, CONTAINS, DOES_NOT_CONTAIN, IS_EMPTY, IS_NOT_EMPTY};
					enum DstStringType {STRING_VARIABLE=0, REFERENCED_STRING, VALUE};
					QString dstValue;
					int srcStringID;
					int dstStringID;
					int comparison;
					int dstStringType;
					bool srcStringReferencedInVariable;
				};
				struct TimerCondition
				{
					enum Comparison {EQUAL_TO=0, NOT_EQUAL_TO, GREATER_THAN, GREATER_THAN_OR_EQUAL_TO, LESS_THAN, LESS_THAN_OR_EQUAL_TO};
					int timerID;
					int minutes;
					int seconds;
					int comparison;
					bool timerReferencedInVariable;
					bool timeStoredInVariable;
				};
				struct MoneyCondition
				{
					enum Comparison {EQUAL_TO=0, NOT_EQUAL_TO, GREATER_THAN, GREATER_THAN_OR_EQUAL_TO, LESS_THAN, LESS_THAN_OR_EQUAL_TO};
					int value;
					int comparison;
					bool valueStoredInVariable;
				};
				struct ItemCondition
				{
					int itemID;
					bool itemReferencedInVariable;
					bool partyPossesses;
				};
				struct HeroCondition
				{
					enum ConditionType {IN_PARTY=0, NAME_IS, LEVEL_IS_AT_LEAST, HP_IS_AT_LEAST, CAN_USE_SKILL, HAS_ITEM_EQUIPPED, AFFLICTED_WITH_CONDITION};
					QString string;
					int value;
					int characterID;
					int conditionType;
					bool characterReferencedInVariable;
					bool valueStoredInVariable;
				};
				struct SpriteCondition
				{
					enum SpriteType {HERO, VEHICLE, EVENT};
					int spriteType;
					int spriteID;
					int direction;
					bool spriteReferencedInVariable;
				};
				struct PartyIsRidingCondition
				{
					int vehicleID;
					bool vehicleReferencedInVariable;
				};
				class Condition
				{
					public:
						enum ConditionType {SWITCH=0, VARIABLE, STRING, TIMER, MONEY, ITEM, HERO, SPRITE, PARTY_IS_RIDING, EVENT_STARTED_WITH_ACTION_KEY, BGM_PLAYED_THROUGH_ONCE};
						Condition();
						Condition(XMLNode ConditionNode);
						Condition(StorageFile &storageFile);
						Condition(Condition *other);
						~Condition();
						XMLNode getConditionNode();
						void saveToStorageFile(StorageFile &storageFile);
						QString getViewableText(int indent) const;
						int getType() {return type;}
						void setType(int value);
						void getSwitchCondition(SwitchCondition *other);
						void setSwitchCondition(SwitchCondition *other);
						void getVariableCondition(VariableCondition *other);
						void setVariableCondition(VariableCondition *other);
						void getStringCondition(StringCondition *other);
						void setStringCondition(StringCondition *other);
						void getTimerCondition(TimerCondition *other);
						void setTimerCondition(TimerCondition *other);
						void getMoneyCondition(MoneyCondition *other);
						void setMoneyCondition(MoneyCondition *other);
						void getItemCondition(ItemCondition *other);
						void setItemCondition(ItemCondition *other);
						void getHeroCondition(HeroCondition *other);
						void setHeroCondition(HeroCondition *other);
						void getSpriteCondition(SpriteCondition *other);
						void setSpriteCondition(SpriteCondition *other);
						void getPartyIsRidingCondition(PartyIsRidingCondition *other);
						void setPartyIsRidingCondition(PartyIsRidingCondition *other);
						static bool tryLoad(XMLNode ConditionNode);
					private:
						int type;
						union ConditionUnion
						{
							SwitchCondition *switchCondition;
							VariableCondition *variableCondition;
							StringCondition *stringCondition;
							TimerCondition *timerCondition;
							MoneyCondition *moneyCondition;
							ItemCondition *itemCondition;
							HeroCondition *heroCondition;
							SpriteCondition *spriteCondition;
							PartyIsRidingCondition *partyIsRidingCondition;
						} conditionUnion;
				};
				~ConditionalBranch_Map();
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources();
				QStringList getViewableText(int indent);
				QList<BaseEvent*> getEventPointers();
				int getEventType() {return TYPE_CONDITIONALBRANCH_MAP;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				void addCondition(Condition *condition);
				void removeCondition(int location);
				Condition *getCondition(int location) {return conditions[location];}
				void addOnTrueEvent(BaseEvent *event, int before=-1);
				void addOnFalseEvent(BaseEvent *event, int before=-1);
				void deleteOnTrueEvent(int location);
				void deleteOnFalseEvent(int location);
				int getConditionsToMatch() {return conditionsToMatch;}
				void setConditionsToMatch(int value) {conditionsToMatch = value;}
				int getNumConditionsToMatch() {return numConditionsToMatch;}
				void setNumConditionsToMatch(int value) {numConditionsToMatch = value;}
				bool getExecuteCustomHandlerIfConditionsNotMet() {return executeCustomHandlerIfConditionsNotMet;}
				void setExecuteCustomHandlerIfConditionsNotMet(bool value) {executeCustomHandlerIfConditionsNotMet = value;}
				void addChildEvent(BaseEvent *event, int locatoin, int childTypeID);
				void deleteChildEvent(int location, int childTypeID);
				void applySwappedTiles(QList<Tileset::SwapTileIDs> swappedTiles);
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				ConditionalBranch_Map(BaseEvent *pBase);
				ConditionalBranch_Map(XMLNode EventNode, BaseEvent *pBase);
				ConditionalBranch_Map(StorageFile &storageFile, BaseEvent *pBase);
				ConditionalBranch_Map(ConditionalBranch_Map *other, BaseEvent *pBase);
				QList<Condition*> conditions;
				QList<BaseEvent*> onTrueEvents;
				QList<BaseEvent*> onFalseEvents;
				int conditionsToMatch;
				int numConditionsToMatch;
				bool executeCustomHandlerIfConditionsNotMet;
		};
		class ConditionalBranch_Menu : public Event
		{
			public:
				enum ConditionsToMatch {ALL=0, AT_LEAST, NONE};
				struct SwitchCondition
				{
					int switchID;
					bool switchReferencedInVariable;
					bool isOn;
				};
				struct VariableCondition
				{
					enum Comparison {EQUAL_TO=0, NOT_EQUAL_TO, GREATER_THAN, GREATER_THAN_OR_EQUAL_TO, LESS_THAN, LESS_THAN_OR_EQUAL_TO};
					int srcVariableID;
					int dstValue;
					int comparison;
					bool dstValueStoredInVariable;
				};
				struct StringCondition
				{
					enum Comparison {EQUAL_TO=0, NOT_EQUAL_TO, STARTS_WITH, ENDS_WITH, CONTAINS, DOES_NOT_CONTAIN, IS_EMPTY, IS_NOT_EMPTY};
					enum DstStringType {STRING_VARIABLE=0, REFERENCED_STRING, VALUE};
					QString dstValue;
					int srcStringID;
					int dstStringID;
					int comparison;
					int dstStringType;
					bool srcStringReferencedInVariable;
				};
				struct TimerCondition
				{
					enum Comparison {EQUAL_TO=0, NOT_EQUAL_TO, GREATER_THAN, GREATER_THAN_OR_EQUAL_TO, LESS_THAN, LESS_THAN_OR_EQUAL_TO};
					int timerID;
					int minutes;
					int seconds;
					int comparison;
					bool timerReferencedInVariable;
					bool timeStoredInVariable;
				};
				struct MoneyCondition
				{
					enum Comparison {EQUAL_TO=0, NOT_EQUAL_TO, GREATER_THAN, GREATER_THAN_OR_EQUAL_TO, LESS_THAN, LESS_THAN_OR_EQUAL_TO};
					int value;
					int comparison;
					bool valueStoredInVariable;
				};
				struct ItemCondition
				{
					int itemID;
					bool itemReferencedInVariable;
					bool partyPossesses;
				};
				struct HeroCondition
				{
					enum ConditionType {IN_PARTY=0, NAME_IS, LEVEL_IS_AT_LEAST, HP_IS_AT_LEAST, CAN_USE_SKILL, HAS_ITEM_EQUIPPED, AFFLICTED_WITH_CONDITION};
					QString string;
					int value;
					int characterID;
					int conditionType;
					bool characterReferencedInVariable;
					bool valueStoredInVariable;
				};
				struct PartyIsRidingCondition
				{
					int vehicleID;
					bool vehicleReferencedInVariable;
				};
				struct PreviousSelectedAreaCondition
				{
					QString objectName;
				};
				struct CurrentHighlightedAreaCondition
				{
					QString objectName;
				};
				class Condition
				{
					public:
						enum ConditionType {SWITCH=0, VARIABLE, STRING, TIMER, MONEY, ITEM, HERO, PARTY_IS_RIDING, PREVIOUS_SELECTED_AREA, CURRENT_HIGHLIGHTED_AREA, BGM_PLAYED_THROUGH_ONCE};
						Condition();
						Condition(XMLNode ConditionNode);
						Condition(StorageFile &storageFile);
						Condition(Condition *other);
						~Condition();
						XMLNode getConditionNode();
						void saveToStorageFile(StorageFile &storageFile);
						QString getViewableText(int indent) const;
						int getType() {return type;}
						void setType(int value);
						void getSwitchCondition(SwitchCondition *other);
						void setSwitchCondition(SwitchCondition *other);
						void getVariableCondition(VariableCondition *other);
						void setVariableCondition(VariableCondition *other);
						void getStringCondition(StringCondition *other);
						void setStringCondition(StringCondition *other);
						void getTimerCondition(TimerCondition *other);
						void setTimerCondition(TimerCondition *other);
						void getMoneyCondition(MoneyCondition *other);
						void setMoneyCondition(MoneyCondition *other);
						void getItemCondition(ItemCondition *other);
						void setItemCondition(ItemCondition *other);
						void getHeroCondition(HeroCondition *other);
						void setHeroCondition(HeroCondition *other);
						void getPartyIsRidingCondition(PartyIsRidingCondition *other);
						void setPartyIsRidingCondition(PartyIsRidingCondition *other);
						void getPreviousSelectedAreaCondition(PreviousSelectedAreaCondition *other);
						void setPreviousSelectedAreaCondition(PreviousSelectedAreaCondition *other);
						void getCurrentHighlightedAreaCondition(CurrentHighlightedAreaCondition *other);
						void setCurrentHighlightedAreaCondition(CurrentHighlightedAreaCondition *other);
						void applyChangeMenuObjectName(QString oldName, QString newName);
						static bool tryLoad(XMLNode ConditionNode);
					private:
						int type;
						union ConditionUnion
						{
							SwitchCondition *switchCondition;
							VariableCondition *variableCondition;
							StringCondition *stringCondition;
							TimerCondition *timerCondition;
							MoneyCondition *moneyCondition;
							ItemCondition *itemCondition;
							HeroCondition *heroCondition;
							PartyIsRidingCondition *partyIsRidingCondition;
							PreviousSelectedAreaCondition *previousSelectedAreaCondition;
							CurrentHighlightedAreaCondition *currentHighlightedAreaCondition;
						} conditionUnion;
				};
				~ConditionalBranch_Menu();
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources();
				QStringList getViewableText(int indent);
				QList<BaseEvent*> getEventPointers();
				int getEventType() {return TYPE_CONDITIONALBRANCH_MENU;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				void addCondition(Condition *condition);
				void removeCondition(int location);
				Condition *getCondition(int location) {return conditions[location];}
				void addOnTrueEvent(BaseEvent *event, int before=-1);
				void addOnFalseEvent(BaseEvent *event, int before=-1);
				void deleteOnTrueEvent(int location);
				void deleteOnFalseEvent(int location);
				int getConditionsToMatch() {return conditionsToMatch;}
				void setConditionsToMatch(int value) {conditionsToMatch = value;}
				int getNumConditionsToMatch() {return numConditionsToMatch;}
				void setNumConditionsToMatch(int value) {numConditionsToMatch = value;}
				bool getExecuteCustomHandlerIfConditionsNotMet() {return executeCustomHandlerIfConditionsNotMet;}
				void setExecuteCustomHandlerIfConditionsNotMet(bool value) {executeCustomHandlerIfConditionsNotMet = value;}
				void addChildEvent(BaseEvent *event, int location, int childTypeID);
				void deleteChildEvent(int location, int childTypeID);
				void applySwappedTiles(QList<Tileset::SwapTileIDs> swappedTiles);
				void applyChangeMenuObjectName(QString oldName, QString newName);
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				ConditionalBranch_Menu(BaseEvent *pBase);
				ConditionalBranch_Menu(XMLNode EventNode, BaseEvent *pBase);
				ConditionalBranch_Menu(StorageFile &storageFile, BaseEvent *pBase);
				ConditionalBranch_Menu(ConditionalBranch_Menu *other, BaseEvent *pBase);
				QList<Condition*> conditions;
				QList<BaseEvent*> onTrueEvents;
				QList<BaseEvent*> onFalseEvents;
				int conditionsToMatch;
				int numConditionsToMatch;
				bool executeCustomHandlerIfConditionsNotMet;
		};
		class DisplayMessage : public Event
		{
			public:
				~DisplayMessage() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources();
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_DISPLAYMESSAGE;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				QString getFaceLocation() {return faceLocation;}
				void setFaceLocation(QString value) {faceLocation = value;}
				QString getMessage() {return message;}
				void setMessage(QString value) {message = value;}
				QColor getTopColor() {return topColor;}
				void setTopColor(QColor value) {topColor = value;}
				QColor getBottomColor() {return bottomColor;}
				void setBottomColor(QColor value) {bottomColor = value;}
				int getX() {return x;}
				void setX(int value) {x = value;}
				int getY() {return y;}
				void setY(int value) {y = value;}
				int getWidth() {return width;}
				void setWidth(int value) {width = value;}
				int getHeight() {return height;}
				void setHeight(int value) {height = value;}
				int getOpacity() {return opacity;}
				void setOpacity(int value) {opacity = value;}
				int getFontID() {return fontID;}
				void setFontID(int value) {fontID = value;}
				bool getCoordinatesStoredInVariables() {return coordinatesStoredInVariables;}
				void setCoordinatesStoredInVariables(bool value) {coordinatesStoredInVariables = value;}
				bool getSizeStoredInVariables() {return sizeStoredInVariables;}
				void setSizeStoredInVariables(bool value) {sizeStoredInVariables = value;}
				bool getOpacityStoredInVariable() {return opacityStoredInVariable;}
				void setOpacityStoredInVariable(bool value) {opacityStoredInVariable = value;}
				bool getBlockEvents() {return blockEvents;}
				void setBlockEvents(bool value) {blockEvents = value;}
				bool getFlipFaceHorizontaly() {return flipFaceHorizontaly;}
				void setFlipFaceHorizontaly(bool value) {flipFaceHorizontaly = value;}
				bool getFaceOnLeft() {return faceOnLeft;}
				void setFaceOnLeft(bool value) {faceOnLeft = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				DisplayMessage(BaseEvent *pBase);
				DisplayMessage(XMLNode EventNode, BaseEvent *pBase);
				DisplayMessage(StorageFile &storageFile, BaseEvent *pBase);
				DisplayMessage(DisplayMessage *other, BaseEvent *pBase);
				QString faceLocation;
				QString message;
				QColor topColor;
				QColor bottomColor;
				int x;
				int y;
				int width;
				int height;
				int xPadding;
				int yPadding;
				int opacity;
				int fontID;
				bool coordinatesStoredInVariables;
				bool sizeStoredInVariables;
				bool opacityStoredInVariable;
				bool blockEvents;
				bool flipFaceHorizontaly;
				bool faceOnLeft;
		};
		class EnableDisableObject : public Event
		{
			public:
				~EnableDisableObject() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_ENABLEDISABLEOBJECT;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				QString getObjectName() {return objectName;}
				void setObjectName(QString value) {objectName = value;}
				bool getEnable() {return enable;}
				void setEnable(bool value) {enable = value;}
				void applyChangeMenuObjectName(QString oldName, QString newName) {if (objectName == oldName) objectName = newName;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				EnableDisableObject(BaseEvent *pBase);
				EnableDisableObject(XMLNode EventNode, BaseEvent *pBase);
				EnableDisableObject(StorageFile &storageFile, BaseEvent *pBase);
				EnableDisableObject(EnableDisableObject *other, BaseEvent *pBase);
				QString objectName;
				bool enable;
		};
		class EndEventProcessing : public Event
		{
			public:
				~EndEventProcessing() {}
				XMLNode getEventNode()
				{
					XMLNode EventNode = XMLNode::createXMLTopNode("event");
					EventNode.addAttribute("type", "type");
					return EventNode;
				}
				void saveToStorageFile(StorageFile &storageFile) {Q_UNUSED(storageFile);}
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent)
				{
					QStringList viewableText = QStringList("");
					for (int i = 0; i < indent; ++i)
						viewableText[0] += "  ";
					viewableText[0] += "<> ";
					viewableText[0] += "End Event Processing";
					return viewableText;
				}
				int getEventType() {return TYPE_ENDEVENTPROCESSING;}
				static bool tryLoad(XMLNode EventNode, int baseType) {Q_UNUSED(EventNode);Q_UNUSED(baseType);return true;}
				friend class Event;
			private:
				EndEventProcessing(BaseEvent *pBase) : Event(pBase) {}
				EndEventProcessing(XMLNode EventNode, BaseEvent *pBase) : Event(pBase) {Q_UNUSED(EventNode);}
				EndEventProcessing(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase) {Q_UNUSED(storageFile);}
				EndEventProcessing(EndEventProcessing *other, BaseEvent *pBase) : Event(pBase) {Q_UNUSED(other);}
		};
		class EnemyEncounter : public Event
		{
			public:
				enum SpecialConditions {NONE=0, INITIATIVE, BACK_ATTACK, SURROUND_ATTACK, PINCERS_ATTACK};
				enum BattleBackgroundType {USE_MAP_OR_TERRAIN_SETTING=0, SPECIFIC_BACKGROUND, USE_TERRAIN_BACKGROUND};
				enum EscapeHandler {DISALLOW_ESCAPE=0, END_EVENT_PROCESSING, CUSTOM_ESCAPE_HANDLER};
				enum DefeatHandler {GAMEOVER=0, CUSTOM_DEFEAT_HANDLER};
				~EnemyEncounter();
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources();
				QStringList getViewableText(int indent);
				QList<BaseEvent*> getEventPointers();
				int getEventType() {return TYPE_ENEMYENCOUNTER;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				void addVictoryEvent(BaseEvent *event, int before=-1);
				void deleteVictoryEvent(int location);
				void addEscapeEvent(BaseEvent *event, int before=-1);
				void deleteEscapeEvent(int location);
				void addDefeatEvent(BaseEvent *event, int before=-1);
				void deleteDefeatEvent(int location);
				QString getSpecificBackgroundLocation() {return specificBackgroundLocation;}
				void setSpecificBackgroundLocation(QString value) {specificBackgroundLocation = value;}
				void getGridDepth(Terrain::GridDepthValues *other);
				void setGridDepth(Terrain::GridDepthValues *other);
				int getMonsterGroupID() {return monsterGroupID;}
				void setMonsterGroupID(int value) {monsterGroupID = value;}
				int getTerrainID() {return terrainID;}
				void setTerrainID(int value) {terrainID = value;}
				int getSpecialConditions() {return specialConditions;}
				void setSpecialConditions(int value) {specialConditions = value;}
				int getBattleBackgroundType() {return battleBackgroundType;}
				void setBattleBackgroundType(int value) {battleBackgroundType = value;}
				int getEscapeHandler() {return escapeHandler;}
				void setEscapeHandler(int value) {escapeHandler = value;}
				int getDefeatHandler() {return defeatHandler;}
				void setDefeatHandler(int value) {defeatHandler = value;}
				bool isMonsterGroupReferencedInVariable() {return monsterGroupReferencedInVariable;}
				void setMonsterGroupReferencedInVariable(bool value) {monsterGroupReferencedInVariable = value;}
				bool getBeginBattleWithFirstStrike() {return beginBattleWithFirstStrike;}
				void setBeginBattleWithFirstStrike(bool value) {beginBattleWithFirstStrike = value;}
				void addChildEvent(BaseEvent *event, int location, int childTypeID);
				void deleteChildEvent(int location, int childTypeID);
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				EnemyEncounter(BaseEvent *pBase);
				EnemyEncounter(XMLNode EventNode, BaseEvent *pBase);
				EnemyEncounter(StorageFile &storageFile, BaseEvent *pBase);
				EnemyEncounter(EnemyEncounter *other, BaseEvent *pBase);
				QList<BaseEvent*> victoryEvents;
				QList<BaseEvent*> escapeEvents;
				QList<BaseEvent*> defeatEvents;
				QString specificBackgroundLocation;
				Terrain::GridDepthValues gridDepth;
				int monsterGroupID;
				int terrainID;
				int specialConditions;
				int battleBackgroundType;
				int escapeHandler;
				int defeatHandler;
				bool monsterGroupReferencedInVariable;
				bool beginBattleWithFirstStrike;
		};
		class EraseEvent : public Event
		{
			public:
				~EraseEvent() {}
				XMLNode getEventNode()
				{
					XMLNode EventNode = XMLNode::createXMLTopNode("event");
					EventNode.addAttribute("type", "type");
					return EventNode;
				}
				void saveToStorageFile(StorageFile &storageFile) {Q_UNUSED(storageFile);}
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent)
				{
					QStringList viewableText = QStringList("");
					for (int i = 0; i < indent; ++i)
						viewableText[0] += "  ";
					viewableText[0] += "<> ";
					viewableText[0] += "Erase Event";
					return viewableText;
				}
				int getEventType() {return TYPE_ERASEEVENT;}
				static bool tryLoad(XMLNode EventNode, int baseType) {Q_UNUSED(EventNode);Q_UNUSED(baseType);return true;}
				friend class Event;
			private:
				EraseEvent(BaseEvent *pBase) : Event(pBase) {}
				EraseEvent(XMLNode EventNode, BaseEvent *pBase) : Event(pBase) {Q_UNUSED(EventNode);}
				EraseEvent(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase) {Q_UNUSED(storageFile);}
				EraseEvent(EraseEvent *other, BaseEvent *pBase) : Event(pBase) {Q_UNUSED(other);}
		};
		class ExitMenu : public Event
		{
			public:
				~ExitMenu() {}
				XMLNode getEventNode()
				{
					XMLNode EventNode = XMLNode::createXMLTopNode("event");
					EventNode.addAttribute("type", "type");
					return EventNode;
				}
				void saveToStorageFile(StorageFile &storageFile) {Q_UNUSED(storageFile);}
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent)
				{
					QStringList viewableText = QStringList("");
					for (int i = 0; i < indent; ++i)
						viewableText[0] += "  ";
					viewableText[0] += "<> ";
					viewableText[0] += "Exit Menu";
					return viewableText;
				}
				int getEventType() {return TYPE_EXITMENU;}
				static bool tryLoad(XMLNode EventNode, int baseType) {Q_UNUSED(EventNode);Q_UNUSED(baseType);return true;}
				friend class Event;
			private:
				ExitMenu(BaseEvent *pBase) : Event(pBase) {}
				ExitMenu(XMLNode EventNode, BaseEvent *pBase) : Event(pBase) {Q_UNUSED(EventNode);}
				ExitMenu(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase) {Q_UNUSED(storageFile);}
				ExitMenu(ExitMenu *other, BaseEvent *pBase) : Event(pBase) {Q_UNUSED(other);}
		};
		class FadeBGM : public Event
		{
			public:
				~FadeBGM() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_FADEBGM;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				int getFadeOutTime() {return fadeOutTime;}
				void setFadeOutTime(int value) {fadeOutTime = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				FadeBGM(BaseEvent *pBase);
				FadeBGM(XMLNode EventNode, BaseEvent *pBase);
				FadeBGM(StorageFile &storageFile, BaseEvent *pBase);
				FadeBGM(FadeBGM *other, BaseEvent *pBase);
				int fadeOutTime;
		};
		class FlashScreen : public Event
		{
			public:
				enum Flash {ONCE=0, BEGIN, STOP};
				~FlashScreen() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_FLASHSCREEN;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				QColor getColor() {return color;}
				void setColor(QColor value) {color = value;}
				int getFlash() {return flash;}
				void setFlash(int value) {flash = value;}
				int getDelayBetweenFlashes() {return delayBetweenFlashes;}
				void setDelayBetweenFlashes(int value) {delayBetweenFlashes = value;}
				int getOpacity() {return opacity;}
				void setOpacity(int value) {opacity = value;}
				bool isOpacityStoredInVariable() {return opacityStoredInVariable;}
				void setOpacityStoredInVariable(bool value) {opacityStoredInVariable = value;}
				bool isDelayStoredInVariable() {return delayStoredInVariable;}
				void setDelayStoredInVariable(bool value) {delayStoredInVariable = value;}
				bool getHaltOtherProcesses() {return haltOtherProcesses;}
				void setHaltOtherProcesses(bool value) {haltOtherProcesses = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				FlashScreen(BaseEvent *pBase);
				FlashScreen(XMLNode EventNode, BaseEvent *pBase);
				FlashScreen(StorageFile &storageFile, BaseEvent *pBase);
				FlashScreen(FlashScreen *other, BaseEvent *pBase);
				QColor color;
				int flash;
				int delayBetweenFlashes;
				int opacity;
				bool opacityStoredInVariable;
				bool delayStoredInVariable;
				bool haltOtherProcesses;
		};
		class FlashSprite_Map : public Event
		{
			public:
				enum EventType {HERO=0, THIS_EVENT, VEHICLE, EVENT};
				enum Flash {ONCE=0, BEGIN, STOP};
				~FlashSprite_Map() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_FLASHSPRITE_MAP;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				QColor getColor() {return color;}
				void setColor(QColor value) {color = value;}
				int getFlash() {return flash;}
				void setFlash(int value) {flash = value;}
				int getDelayBetweenFlashes() {return delayBetweenFlashes;}
				void setDelayBetweenFlashes(int value) {delayBetweenFlashes = value;}
				int getOpacity() {return opacity;}
				void setOpacity(int value) {opacity = value;}
				int getTargetType() {return targetType;}
				void setTargetType(int value) {targetType = value;}
				int getTargetID() {return targetID;}
				void setTargetID(int value) {targetID = value;}
				bool isOpacityStoredInVariable() {return opacityStoredInVariable;}
				void setOpacityStoredInVariable(bool value) {opacityStoredInVariable = value;}
				bool isDelayStoredInVariable() {return delayStoredInVariable;}
				void setDelayStoredInVariable(bool value) {delayStoredInVariable = value;}
				bool getHaltOtherProcesses() {return haltOtherProcesses;}
				void setHaltOtherProcesses(bool value) {haltOtherProcesses = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				FlashSprite_Map(BaseEvent *pBase);
				FlashSprite_Map(XMLNode EventNode, BaseEvent *pBase);
				FlashSprite_Map(StorageFile &storageFile, BaseEvent *pBase);
				FlashSprite_Map(FlashSprite_Map *other, BaseEvent *pBase);
				QColor color;
				int flash;
				int delayBetweenFlashes;
				int opacity;
				int targetType;
				int targetID;
				bool opacityStoredInVariable;
				bool delayStoredInVariable;
				bool haltOtherProcesses;
		};
		class GameOver : public Event
		{
			public:
				~GameOver() {}
				XMLNode getEventNode()
				{
					XMLNode EventNode = XMLNode::createXMLTopNode("event");
					EventNode.addAttribute("type", "type");
					return EventNode;
				}
				void saveToStorageFile(StorageFile &storageFile) {Q_UNUSED(storageFile);}
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent)
				{
					QStringList viewableText = QStringList("");
					for (int i = 0; i < indent; ++i)
						viewableText[0] += "  ";
					viewableText[0] += "<> ";
					viewableText[0] += "Game Over";
					return viewableText;
				}
				int getEventType() {return TYPE_GAMEOVER;}
				static bool tryLoad(XMLNode EventNode, int baseType) {Q_UNUSED(EventNode);Q_UNUSED(baseType);return true;}
				friend class Event;
			private:
				GameOver(BaseEvent *pBase) : Event(pBase) {}
				GameOver(XMLNode EventNode, BaseEvent *pBase) : Event(pBase) {Q_UNUSED(EventNode);}
				GameOver(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase) {Q_UNUSED(storageFile);}
				GameOver(GameOver *other, BaseEvent *pBase) : Event(pBase) {Q_UNUSED(other);}
		};
		class HaltAllMovement : public Event
		{
			public:
				~HaltAllMovement() {}
				XMLNode getEventNode()
				{
					XMLNode EventNode = XMLNode::createXMLTopNode("event");
					EventNode.addAttribute("type", "type");
					return EventNode;
				}
				void saveToStorageFile(StorageFile &storageFile) {Q_UNUSED(storageFile);}
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent)
				{
					QStringList viewableText = QStringList("");
					for (int i = 0; i < indent; ++i)
						viewableText[0] += "  ";
					viewableText[0] += "<> ";
					viewableText[0] += "Halt All Movement";
					return viewableText;
				}
				int getEventType() {return TYPE_HALTALLMOVEMENT;}
				static bool tryLoad(XMLNode EventNode, int baseType) {Q_UNUSED(EventNode);Q_UNUSED(baseType);return true;}
				friend class Event;
			private:
				HaltAllMovement(BaseEvent *pBase) : Event(pBase) {}
				HaltAllMovement(XMLNode EventNode, BaseEvent *pBase) : Event(pBase) {Q_UNUSED(EventNode);}
				HaltAllMovement(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase) {Q_UNUSED(storageFile);}
				HaltAllMovement(HaltAllMovement *other, BaseEvent *pBase) : Event(pBase) {Q_UNUSED(other);}
		};
		class HeroSpriteTransparency : public Event
		{
			public:
				~HeroSpriteTransparency() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_HEROSPRITETRANSPARENCY;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				int getOpacity() {return opacity;}
				void setOpacity(int value) {opacity = value;}
				bool isOpacityStoredInVariable() {return opacityStoredInVariable;}
				void setOpacityStoredInVariable(bool value) {opacityStoredInVariable = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				HeroSpriteTransparency(BaseEvent *pBase);
				HeroSpriteTransparency(XMLNode EventNode, BaseEvent *pBase);
				HeroSpriteTransparency(StorageFile &storageFile, BaseEvent *pBase);
				HeroSpriteTransparency(HeroSpriteTransparency *other, BaseEvent *pBase);
				int opacity;
				bool opacityStoredInVariable;
		};
		class HideLayer : public Event
		{
			public:
				~HideLayer() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_HIDELAYER;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				int getLayer() {return layer;}
				void setLayer(int value) {layer = value;}
				int getTransition() {return transition;}
				void setTransition(int value) {transition = value;}
				bool isLayerReferencedInVariable() {return layerReferencedInVariable;}
				void setLayerReferencedInVariable(bool value) {layerReferencedInVariable = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				HideLayer(BaseEvent *pBase);
				HideLayer(XMLNode EventNode, BaseEvent *pBase);
				HideLayer(StorageFile &storageFile, BaseEvent *pBase);
				HideLayer(HideLayer *other, BaseEvent *pBase);
				int layer;
				int transition;
				bool layerReferencedInVariable;
		};
		class HideScreen : public Event
		{
			public:
				~HideScreen() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_HIDESCREEN;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				int getTransition() {return transition;}
				void setTransition(int value) {transition = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				HideScreen(BaseEvent *pBase);
				HideScreen(XMLNode EventNode, BaseEvent *pBase);
				HideScreen(StorageFile &storageFile, BaseEvent *pBase);
				HideScreen(HideScreen *other, BaseEvent *pBase);
				int transition;
		};
		class HideScreenOverlay : public Event
		{
			public:
				~HideScreenOverlay() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_HIDESCREENOVERLAY;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				int getTransition() {return transition;}
				void setTransition(int value) {transition = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				HideScreenOverlay(BaseEvent *pBase);
				HideScreenOverlay(XMLNode EventNode, BaseEvent *pBase);
				HideScreenOverlay(StorageFile &storageFile, BaseEvent *pBase);
				HideScreenOverlay(HideScreenOverlay *other, BaseEvent *pBase);
				int transition;
		};
		class InsertComment : public Event
		{
			public:
				~InsertComment() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_INSERTCOMMENT;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				QString getComment() {return comment;}
				void setComment(QString value) {comment = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				InsertComment(BaseEvent *pBase);
				InsertComment(XMLNode EventNode, BaseEvent *pBase);
				InsertComment(StorageFile &storageFile, BaseEvent *pBase);
				InsertComment(InsertComment *other, BaseEvent *pBase);
				QString comment;
		};
		class ItemManagement : public Event
		{
			public:
				~ItemManagement() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_ITEMMANAGEMENT;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				int getItemID() {return itemID;}
				void setItemID(int value) {itemID = value;}
				int getAmount() {return amount;}
				void setAmount(int value) {amount = value;}
				bool getAddItem() {return addItem;}
				void setAddItem(bool value) {addItem = value;}
				bool isItemReferencedInVariable() {return itemReferencedInVariable;}
				void setItemReferencedInVariable(bool value) {itemReferencedInVariable = value;}
				bool isAmountStoredInVariable() {return amountStoredInVariable;}
				void setAmountStoredInVariable(bool value) {amountStoredInVariable = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				ItemManagement(BaseEvent *pBase);
				ItemManagement(XMLNode EventNode, BaseEvent *pBase);
				ItemManagement(StorageFile &storageFile, BaseEvent *pBase);
				ItemManagement(ItemManagement *other, BaseEvent *pBase);
				int itemID;
				int amount;
				bool addItem;
				bool itemReferencedInVariable;
				bool amountStoredInVariable;
		};
		class JumpToLabel : public Event
		{
			public:
				~JumpToLabel() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_JUMPTOLABEL;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				int getLabelID() {return labelID;}
				void setLabelID(int value) {labelID = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				JumpToLabel(BaseEvent *pBase);
				JumpToLabel(XMLNode EventNode, BaseEvent *pBase);
				JumpToLabel(StorageFile &storageFile, BaseEvent *pBase);
				JumpToLabel(JumpToLabel *other, BaseEvent *pBase);
				int labelID;
		};
		class KeyInputProcessing : public Event
		{
			public:
				~KeyInputProcessing() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_KEYINPUTPROCESSING;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				int getVariable1ID() {return variable1ID;}
				void setVariable1ID(int value) {variable1ID = value;}
				int getVariable2ID() {return variable2ID;}
				void setVariable2ID(int value) {variable2ID = value;}
				bool getWaitForKeyPress() {return waitForKeyPress;}
				void setWaitForKeyPress(bool value) {waitForKeyPress = value;}
				bool getStoreTimeForKeyPress() {return storeTimeForKeyPress;}
				void setStoreTimeForKeyPress(bool value) {storeTimeForKeyPress = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				KeyInputProcessing(BaseEvent *pBase);
				KeyInputProcessing(XMLNode EventNode, BaseEvent *pBase);
				KeyInputProcessing(StorageFile &storageFile, BaseEvent *pBase);
				KeyInputProcessing(KeyInputProcessing *other, BaseEvent *pBase);
				int variable1ID;
				int variable2ID;
				bool waitForKeyPress;
				bool storeTimeForKeyPress;
		};
		class Label : public Event
		{
			public:
				~Label() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_LABEL;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				int getLabelID() {return labelID;}
				void setLabelID(int value) {labelID = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				Label(BaseEvent *pBase);
				Label(XMLNode EventNode, BaseEvent *pBase);
				Label(StorageFile &storageFile, BaseEvent *pBase);
				Label(Label *other, BaseEvent *pBase);
				int labelID;
		};
		class LoadGame : public Event
		{
			public:
				~LoadGame() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_LOADGAME;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				int getSlot() {return slot;}
				void setSlot(int value) {slot = value;}
				bool isSlotReferencedInVariable() {return slotReferencedInVariable;}
				void setSlotReferencedInVariable(bool value) {slotReferencedInVariable = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				LoadGame(BaseEvent *pBase);
				LoadGame(XMLNode EventNode, BaseEvent *pBase);
				LoadGame(StorageFile &storageFile, BaseEvent *pBase);
				LoadGame(LoadGame *other, BaseEvent *pBase);
				int slot;
				bool slotReferencedInVariable;
		};
		class Loop : public Event
		{
			public:
				~Loop();
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources();
				QStringList getViewableText(int indent);
				QList<BaseEvent*> getEventPointers();
				int getEventType() {return TYPE_LOOP;}
				void addEvent(BaseEvent *event, int before=-1);
				void deleteEvent(int location);
				void addChildEvent(BaseEvent *event, int location, int childTypeID) {Q_UNUSED(childTypeID);addEvent(event, location);}
				void deleteChildEvent(int location, int childTypeID) {Q_UNUSED(childTypeID);deleteEvent(location);}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				Loop(BaseEvent *pBase) : Event(pBase) {}
				Loop(XMLNode EventNode, BaseEvent *pBase);
				Loop(StorageFile &storageFile, BaseEvent *pBase);
				Loop(Loop *other, BaseEvent *pBase);
				QList<BaseEvent*> events;
		};
		class MemorizeBGM : public Event
		{
			public:
				~MemorizeBGM() {}
				XMLNode getEventNode()
				{
					XMLNode EventNode = XMLNode::createXMLTopNode("event");
					EventNode.addAttribute("type", "type");
					return EventNode;
				}
				void saveToStorageFile(StorageFile &storageFile) {Q_UNUSED(storageFile);}
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent)
				{
					QStringList viewableText = QStringList("");
					for (int i = 0; i < indent; ++i)
						viewableText[0] += "  ";
					viewableText[0] += "<> ";
					viewableText[0] += "Memorize BGM";
					return viewableText;
				}
				int getEventType() {return TYPE_MEMORIZEBGM;}
				static bool tryLoad(XMLNode EventNode, int baseType) {Q_UNUSED(EventNode);Q_UNUSED(baseType);return true;}
				friend class Event;
			private:
				MemorizeBGM(BaseEvent *pBase) : Event(pBase) {}
				MemorizeBGM(XMLNode EventNode, BaseEvent *pBase) : Event(pBase) {Q_UNUSED(EventNode);}
				MemorizeBGM(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase) {Q_UNUSED(storageFile);}
				MemorizeBGM(MemorizeBGM *other, BaseEvent *pBase) : Event(pBase) {Q_UNUSED(other);}
		};
		class MemorizePosition : public Event
		{
			public:
				~MemorizePosition() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_MEMORIZEPOSITION;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				int getMapVariableID() {return mapVariableID;}
				void setMapVariableID(int value) {mapVariableID = value;}
				int getXVariableID() {return xVariableID;}
				void setXVariableID(int value) {xVariableID = value;}
				int getYVariableID() {return yVariableID;}
				void setYVariableID(int value) {yVariableID = value;}
				int getLayerVariableID() {return layerVariableID;}
				void setLayerVariableID(int value) {layerVariableID = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				MemorizePosition(BaseEvent *pBase);
				MemorizePosition(XMLNode EventNode, BaseEvent *pBase);
				MemorizePosition(StorageFile &storageFile, BaseEvent *pBase);
				MemorizePosition(MemorizePosition *other, BaseEvent *pBase);
				int mapVariableID;
				int xVariableID;
				int yVariableID;
				int layerVariableID;
		};
		class MoveEvent : public Event
		{
			public:
				enum EventType {HERO=0, THIS_EVENT, VEHICLE, EVENT};
				~MoveEvent();
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources();
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_MOVEEVENT;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				MapEvent::MovementPattern *getPattern(int location) {return pattern[location];}
				void addPattern(MapEvent::MovementPattern *value) {pattern.append(value);}
				void deletePattern(int location) {delete (pattern[location]);pattern.removeAt(location);}
				int patternSize() {return pattern.size();}
				int getSelectedEventType() {return eventType;}
				void setSelectedEventType(int value) {eventType = value;}
				int getEventID() {return eventID;}
				void setEventID(int value) {eventID = value;}
				bool getIgnoreImpossibleMoves() {return ignoreImpossibleMoves;}
				void setIgnoreImpossibleMoves(bool value) {ignoreImpossibleMoves = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				MoveEvent(BaseEvent *pBase);
				MoveEvent(XMLNode EventNode, BaseEvent *pBase);
				MoveEvent(StorageFile &storageFile, BaseEvent *pBase);
				MoveEvent(MoveEvent *other, BaseEvent *pBase);
				QList<MapEvent::MovementPattern*> pattern;
				int eventType;
				int eventID;
				bool ignoreImpossibleMoves;
		};
		class OpenMenu : public Event
		{
			public:
				~OpenMenu() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_OPENMENU;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				int getMenuID() {return menuID;}
				void setMenuID(int value) {menuID = value;}
				bool isMenuReferencedInVariable() {return menuReferencedInVariable;}
				void setMenuReferencedInVariable(bool value) {menuReferencedInVariable = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				OpenMenu(BaseEvent *pBase);
				OpenMenu(XMLNode EventNode, BaseEvent *pBase);
				OpenMenu(StorageFile &storageFile, BaseEvent *pBase);
				OpenMenu(OpenMenu *other, BaseEvent *pBase);
				int menuID;
				bool menuReferencedInVariable;
		};
		class PanScreen : public Event
		{
			public:
				enum Operation {LOCK_PAN_STATE=0, UNLOCK_PAN_STATE, PAN_SCREEN, RETURN_TO_ORIGIN};
				enum Direction {UP=0, DOWN, LEFT, RIGHT};
				enum Pan {DIRECTION=0, LOCATION};
				enum TransitionSpeed {SPEED_ONEEIGHTHNORMAL=0, SPEED_ONEFOURTHNORMAL, SPEED_ONEHALFNORMAL, SPEED_NORMAL, SPEED_TWICENORMAL, SPEED_FOURTIMESNORMAL};
				~PanScreen() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_PANSCREEN;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				int getOperation() {return operation;}
				void setOperation(int value) {operation = value;}
				int getPan() {return pan;}
				void setPan(int value) {pan = value;}
				int getPanDirection() {return panDirection.direction;}
				void setPanDirection(int value) {panDirection.direction = value;}
				int getPanTiles() {return panDirection.tiles;}
				void setPanTiles(int value) {panDirection.tiles = value;}
				int getPanX() {return panLocation.x;}
				void setPanX(int value) {panLocation.x = value;}
				int getPanY() {return panLocation.y;}
				void setPanY(int value) {panLocation.y = value;}
				bool getCoordinatesStoredInVariables() {return panLocation.coordinatesStoredInVariables;}
				void setCoordinatesStoredInVariables(bool value) {panLocation.coordinatesStoredInVariables = value;}
				int getTransitionSpeed() {return transitionSpeed;}
				void setTransitionSpeed(int value) {transitionSpeed = value;}
				bool getHaltOtherProcesses() {return haltOtherProcesses;}
				void setHaltOtherProcesses(bool value) {haltOtherProcesses = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				struct PanDirection
				{
					int direction;
					int tiles;
				};
				struct PanLocation
				{
					int x;
					int y;
					bool coordinatesStoredInVariables;
				};
				friend class Event;
			private:
				PanScreen(BaseEvent *pBase);
				PanScreen(XMLNode EventNode, BaseEvent *pBase);
				PanScreen(StorageFile &storageFile, BaseEvent *pBase);
				PanScreen(PanScreen *other, BaseEvent *pBase);
				PanDirection panDirection;
				PanLocation panLocation;
				int operation;
				int pan;
				int transitionSpeed;
				bool haltOtherProcesses;
		};
		class PlayBGM : public Event
		{
			public:
				~PlayBGM() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources();
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_PLAYBGM;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				void getBGM(AudioManager::MusicInfo *other);
				void setBGM(AudioManager::MusicInfo *other);
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				PlayBGM(BaseEvent *pBase);
				PlayBGM(XMLNode EventNode, BaseEvent *pBase);
				PlayBGM(StorageFile &storageFile, BaseEvent *pBase);
				PlayBGM(PlayBGM *other, BaseEvent *pBase);
				AudioManager::MusicInfo bgm;
		};
		class PlayMemorizedBGM : public Event
		{
			public:
				~PlayMemorizedBGM() {}
				XMLNode getEventNode()
				{
					XMLNode EventNode = XMLNode::createXMLTopNode("event");
					EventNode.addAttribute("type", "type");
					return EventNode;
				}
				void saveToStorageFile(StorageFile &storageFile) {Q_UNUSED(storageFile);}
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent)
				{
					QStringList viewableText = QStringList("");
					for (int i = 0; i < indent; ++i)
						viewableText[0] += "  ";
					viewableText[0] += "<> ";
					viewableText[0] += "Play Memorized BGM";
					return viewableText;
				}
				int getEventType() {return TYPE_PLAYMEMORIZEDBGM;}
				static bool tryLoad(XMLNode EventNode, int baseType) {Q_UNUSED(EventNode);Q_UNUSED(baseType);return true;}
				friend class Event;
			private:
				PlayMemorizedBGM(BaseEvent *pBase) : Event(pBase) {}
				PlayMemorizedBGM(XMLNode EventNode, BaseEvent *pBase) : Event(pBase) {Q_UNUSED(EventNode);}
				PlayMemorizedBGM(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase) {Q_UNUSED(storageFile);}
				PlayMemorizedBGM(PlayMemorizedBGM *other, BaseEvent *pBase) : Event(pBase) {Q_UNUSED(other);}
		};
		class PlayMovie : public Event
		{
			public:
				~PlayMovie() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources();
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_PLAYMOVIE;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				QString getMovieLocation() {return movieLocation;}
				void setMovieLocation(QString value) {movieLocation = value;}
				int getX() {return x;}
				void setX(int value) {x = value;}
				int getY() {return y;}
				void setY(int value) {y = value;}
				int getWidth() {return width;}
				void setWidth(int value) {width = value;}
				int getHeight() {return height;}
				void setHeight(int value) {height = value;}
				bool areCoordinatesStoredInVariables() {return coordinatesStoredInVariables;}
				void setCoordinatesStoredInVariables(bool value) {coordinatesStoredInVariables = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				PlayMovie(BaseEvent *pBase);
				PlayMovie(XMLNode EventNode, BaseEvent *pBase);
				PlayMovie(StorageFile &storageFile, BaseEvent *pBase);
				PlayMovie(PlayMovie *other, BaseEvent *pBase);
				QString movieLocation;
				int x;
				int y;
				int width;
				int height;
				bool coordinatesStoredInVariables;
		};
		class PlaySoundEffect : public Event
		{
			public:
				~PlaySoundEffect() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources();
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_PLAYSOUNDEFFECT;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				void getSoundEffect(AudioManager::SoundEffectInfo *other);
				void setSoundEffect(AudioManager::SoundEffectInfo *other);
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				PlaySoundEffect(BaseEvent *pBase);
				PlaySoundEffect(XMLNode EventNode, BaseEvent *pBase);
				PlaySoundEffect(StorageFile &storageFile, BaseEvent *pBase);
				PlaySoundEffect(PlaySoundEffect *other, BaseEvent *pBase);
				AudioManager::SoundEffectInfo soundEffect;
		};
		class PlaySystemBGM : public Event
		{
			public:
				~PlaySystemBGM() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_PLAYSYSTEMBGM;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				int getBGM() {return bgm;}
				void setBGM(int value) {bgm = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				PlaySystemBGM(BaseEvent *pBase);
				PlaySystemBGM(XMLNode EventNode, BaseEvent *pBase);
				PlaySystemBGM(StorageFile &storageFile, BaseEvent *pBase);
				PlaySystemBGM(PlaySystemBGM *other, BaseEvent *pBase);
				int bgm;
		};
		class PlaySystemSoundEffect : public Event
		{
			public:
				~PlaySystemSoundEffect() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_PLAYSYSTEMSOUNDEFFECT;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				int getSoundEffect() {return soundEffect;}
				void setSoundEffect(int value) {soundEffect = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				PlaySystemSoundEffect(BaseEvent *pBase);
				PlaySystemSoundEffect(XMLNode EventNode, BaseEvent *pBase);
				PlaySystemSoundEffect(StorageFile &storageFile, BaseEvent *pBase);
				PlaySystemSoundEffect(PlaySystemSoundEffect *other, BaseEvent *pBase);
				int soundEffect;
		};
		class PreviousSelectedAreaBranch : public Event
		{
			public:
				~PreviousSelectedAreaBranch();
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources();
				QStringList getViewableText(int indent);
				QList<BaseEvent*> getEventPointers();
				int getEventType() {return TYPE_PREVIOUSSELECTEDAREABRANCH;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				void addSelectableArea(QString selectableArea);
				void deleteSelectableArea(QString selectableArea);
				void addChildEvent(BaseEvent *event, int location, int childTypeID);
				void deleteChildEvent(int location, int childTypeID);
				void applySwappedTiles(QList<Tileset::SwapTileIDs> swappedTiles);
				void applyChangeMenuObjectName(QString oldName, QString newName);
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				PreviousSelectedAreaBranch(BaseEvent *pBase) : Event(pBase) {}
				PreviousSelectedAreaBranch(XMLNode EventNode, BaseEvent *pBase);
				PreviousSelectedAreaBranch(StorageFile &storageFile, BaseEvent *pBase);
				PreviousSelectedAreaBranch(PreviousSelectedAreaBranch *other, BaseEvent *pBase);
				QMap<QString, QList<BaseEvent*> > selectableAreaEvents;
		};
		class ProceedWithMovement : public Event
		{
			public:
				~ProceedWithMovement() {}
				XMLNode getEventNode()
				{
					XMLNode EventNode = XMLNode::createXMLTopNode("event");
					EventNode.addAttribute("type", "type");
					return EventNode;
				}
				void saveToStorageFile(StorageFile &storageFile) {Q_UNUSED(storageFile);}
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent)
				{
					QStringList viewableText = QStringList("");
					for (int i = 0; i < indent; ++i)
						viewableText[0] += "  ";
					viewableText[0] += "<> ";
					viewableText[0] += "Proceed With Movement";
					return viewableText;
				}
				int getEventType() {return TYPE_PROCEEDWITHMOVEMENT;}
				static bool tryLoad(XMLNode EventNode, int baseType) {Q_UNUSED(EventNode);Q_UNUSED(baseType);return true;}
				friend class Event;
			private:
				ProceedWithMovement(BaseEvent *pBase) : Event(pBase) {}
				ProceedWithMovement(XMLNode EventNode, BaseEvent *pBase) : Event(pBase) {Q_UNUSED(EventNode);}
				ProceedWithMovement(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase) {Q_UNUSED(storageFile);}
				ProceedWithMovement(ProceedWithMovement *other, BaseEvent *pBase) : Event(pBase) {Q_UNUSED(other);}
		};
		class QuitGame : public Event
		{
			public:
				~QuitGame() {}
				XMLNode getEventNode()
				{
					XMLNode EventNode = XMLNode::createXMLTopNode("event");
					EventNode.addAttribute("type", "type");
					return EventNode;
				}
				void saveToStorageFile(StorageFile &storageFile) {Q_UNUSED(storageFile);}
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent)
				{
					QStringList viewableText = QStringList("");
					for (int i = 0; i < indent; ++i)
						viewableText[0] += "  ";
					viewableText[0] += "<> ";
					viewableText[0] += "Quit Game";
					return viewableText;
				}
				int getEventType() {return TYPE_QUITGAME;}
				static bool tryLoad(XMLNode EventNode, int baseType) {Q_UNUSED(EventNode);Q_UNUSED(baseType);return true;}
				friend class Event;
			private:
				QuitGame(BaseEvent *pBase) : Event(pBase) {}
				QuitGame(XMLNode EventNode, BaseEvent *pBase) : Event(pBase) {Q_UNUSED(EventNode);}
				QuitGame(StorageFile &storageFile, BaseEvent *pBase) : Event(pBase) {Q_UNUSED(storageFile);}
				QuitGame(QuitGame *other, BaseEvent *pBase) : Event(pBase) {Q_UNUSED(other);}
		};
		class SaveGame : public Event
		{
			public:
				~SaveGame() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_SAVEGAME;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				int getSlot() {return slot;}
				void setSlot(int value) {slot = value;}
				bool isSlotReferencedInVariable() {return slotReferencedInVariable;}
				void setSlotReferencedInVariable(bool value) {slotReferencedInVariable = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				SaveGame(BaseEvent *pBase);
				SaveGame(XMLNode EventNode, BaseEvent *pBase);
				SaveGame(StorageFile &storageFile, BaseEvent *pBase);
				SaveGame(SaveGame *other, BaseEvent *pBase);
				int slot;
				bool slotReferencedInVariable;
		};
		class ScrollArea : public Event
		{
			public:
				enum ScrollTo {CHILD_OBJECT=0, SPECIFIC_LOCATION, STORED_LOCATION};
				~ScrollArea() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_SCROLLAREA;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				QString getScrollAreaObjectName() {return scrollAreaObjectName;}
				void setScrollAreaObjectName(QString value) {scrollAreaObjectName = value;}
				QString getChildObjectName() {return childObjectName;}
				void setChildObjectName(QString value) {childObjectName = value;}
				int getX() {return x;}
				void setX(int value) {x = value;}
				int getY() {return y;}
				void setY(int value) {y = value;}
				int getScrollTo() {return scrollTo;}
				void setScrollTo(int value) {scrollTo = value;}
				void applyChangeMenuObjectName(QString oldName, QString newName);
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				ScrollArea(BaseEvent *pBase);
				ScrollArea(XMLNode EventNode, BaseEvent *pBase);
				ScrollArea(StorageFile &storageFile, BaseEvent *pBase);
				ScrollArea(ScrollArea *other, BaseEvent *pBase);
				QString scrollAreaObjectName;
				QString childObjectName;
				int x;
				int y;
				int scrollTo;
		};
		class SetScreenTransition : public Event
		{
			public:
				enum TransitionType {TELEPORT_ERASE=0, TELEPORT_SHOW, MENU_ERASE, MENU_SHOW, BATTLE_ENTRY_ERASE, BATTLE_ENTRY_SHOW, BATTLE_EXIT_ERASE, BATTLE_EXIT_SHOW};
				~SetScreenTransition() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_SETSCREENTRANSITION;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				int getTransitionType() {return transitionType;}
				void setTransitionType(int value) {transitionType = value;}
				int getTransition() {return transition;}
				void setTransition(int value) {transition = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				SetScreenTransition(BaseEvent *pBase);
				SetScreenTransition(XMLNode EventNode, BaseEvent *pBase);
				SetScreenTransition(StorageFile &storageFile, BaseEvent *pBase);
				SetScreenTransition(SetScreenTransition *other, BaseEvent *pBase);
				int transitionType;
				int transition;
		};
		class ShakeScreen : public Event
		{
			public:
				enum Action {SHAKE_ONCE=0, BEGIN_SHAKING, STOP_SHAKING};
				~ShakeScreen() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_SHAKESCREEN;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				int getAction() {return action;}
				void setAction(int value) {action = value;}
				int getTime() {return time;}
				void setTime(int value) {time = value;}
				int getStrength() {return strength;}
				void setStrength(int value) {strength = value;}
				int getSpeed() {return speed;}
				void setSpeed(int value) {speed = value;}
				bool isTimeStoredInVariable() {return timeStoredInVariable;}
				void setTimeStoredInVariable(bool value) {timeStoredInVariable = value;}
				bool getHaltOtherProcesses() {return haltOtherProcesses;}
				void setHaltOtherProcesses(bool value) {haltOtherProcesses = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
			friend class Event;
			private:
				ShakeScreen(BaseEvent *pBase);
				ShakeScreen(XMLNode EventNode, BaseEvent *pBase);
				ShakeScreen(StorageFile &storageFile, BaseEvent *pBase);
				ShakeScreen(ShakeScreen *other, BaseEvent *pBase);
				int action;
				int time;
				int strength;
				int speed;
				bool timeStoredInVariable;
				bool haltOtherProcesses;
		};
		class ShowBattleAnimation_Map : public Event
		{
			public:
				enum EventType {HERO=0, THIS_EVENT, VEHICLE, EVENT};
				~ShowBattleAnimation_Map() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_SHOWBATTLEANIMATION_MAP;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				int getAnimationID() {return animationID;}
				void setAnimationID(int value) {animationID = value;}
				int getTargetType() {return targetType;}
				void setTargetType(int value) {targetType = value;}
				int getTargetID() {return targetID;}
				void setTargetID(int value) {targetID = value;}
				bool getHaltOtherProcesses() {return haltOtherProcesses;}
				void setHaltOtherProcesses(bool value) {haltOtherProcesses = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				ShowBattleAnimation_Map(BaseEvent *pBase);
				ShowBattleAnimation_Map(XMLNode EventNode, BaseEvent *pBase);
				ShowBattleAnimation_Map(StorageFile &storageFile, BaseEvent *pBase);
				ShowBattleAnimation_Map(ShowBattleAnimation_Map *other, BaseEvent *pBase);
				int animationID;
				int targetType;
				int targetID;
				bool haltOtherProcesses;
		};
		class ShowGlobalAnimation : public Event
		{
			public:
				enum Action {PLAY_ONCE=0, BEGIN_PLAYING, STOP_PLAYING};
				~ShowGlobalAnimation() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_SHOWGLOBALANIMATION;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				int getAction() {return action;}
				void setAction(int value) {action = value;}
				int getAnimationID() {return animationID;}
				void setAnimationID(int value) {animationID = value;}
				bool getHaltOtherProcesses() {return haltOtherProcesses;}
				void setHaltOtherProcesses(bool value) {haltOtherProcesses = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				ShowGlobalAnimation(BaseEvent *pBase);
				ShowGlobalAnimation(XMLNode EventNode, BaseEvent *pBase);
				ShowGlobalAnimation(StorageFile &storageFile, BaseEvent *pBase);
				ShowGlobalAnimation(ShowGlobalAnimation *other, BaseEvent *pBase);
				int action;
				int animationID;
				bool haltOtherProcesses;
		};
		class ShowLayer : public Event
		{
			public:
				~ShowLayer() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_SHOWLAYER;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				int getLayer() {return layer;}
				void setLayer(int value) {layer = value;}
				int getTransition() {return transition;}
				void setTransition(int value) {transition = value;}
				bool isLayerReferencedInVariable() {return layerReferencedInVariable;}
				void setLayerReferencedInVariable(bool value) {layerReferencedInVariable = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				ShowLayer(BaseEvent *pBase);
				ShowLayer(XMLNode EventNode, BaseEvent *pBase);
				ShowLayer(StorageFile &storageFile, BaseEvent *pBase);
				ShowLayer(ShowLayer *other, BaseEvent *pBase);
				int layer;
				int transition;
				bool layerReferencedInVariable;
		};
		class ShowPopupMenu : public Event
		{
			public:
				~ShowPopupMenu() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_SHOWPOPUPMENU;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				int getMenuID() {return menuID;}
				void setMenuID(int value) {menuID = value;}
				int getX() {return x;}
				void setX(int value) {x = value;}
				int getY() {return y;}
				void setY(int value) {y = value;}
				bool isMenuReferencedInVariable() {return menuReferencedInVariable;}
				void setMenuReferencedInVariable(bool value) {menuReferencedInVariable = value;}
				bool areCoordinatesStoredInVariables() {return coordinatesStoredInVariables;}
				void setCoordinatesStoredInVariables(bool value) {coordinatesStoredInVariables = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				ShowPopupMenu(BaseEvent *pBase);
				ShowPopupMenu(XMLNode EventNode, BaseEvent *pBase);
				ShowPopupMenu(StorageFile &storageFile, BaseEvent *pBase);
				ShowPopupMenu(ShowPopupMenu *other, BaseEvent *pBase);
				int menuID;
				int x;
				int y;
				bool menuReferencedInVariable;
				bool coordinatesStoredInVariables;
		};
		class ShowScreen : public Event
		{
			public:
				~ShowScreen() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_SHOWSCREEN;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				int getTransition() {return transition;}
				void setTransition(int value) {transition = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				ShowScreen(BaseEvent *pBase);
				ShowScreen(XMLNode EventNode, BaseEvent *pBase);
				ShowScreen(StorageFile &storageFile, BaseEvent *pBase);
				ShowScreen(ShowScreen *other, BaseEvent *pBase);
				int transition;
		};
		class ShowScreenOverlay : public Event
		{
			public:
				~ShowScreenOverlay() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_SHOWSCREENOVERLAY;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				int getOverlayID() {return overlayID;}
				void setOverlayID(int value) {overlayID = value;}
				int getTransition() {return transition;}
				void setTransition(int value) {transition = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				ShowScreenOverlay(BaseEvent *pBase);
				ShowScreenOverlay(XMLNode EventNode, BaseEvent *pBase);
				ShowScreenOverlay(StorageFile &storageFile, BaseEvent *pBase);
				ShowScreenOverlay(ShowScreenOverlay *other, BaseEvent *pBase);
				int overlayID;
				int transition;
		};
		class StoreEventID : public Event
		{
			public:
				~StoreEventID() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_STOREEVENTID;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				int getX() {return x;}
				void setX(int value) {x = value;}
				int getY() {return y;}
				void setY(int value) {y = value;}
				int getLayer() {return layer;}
				void setLayer(int value) {layer = value;}
				int getVariableID() {return variableID;}
				void setVariableID(int value) {variableID = value;}
				bool isLocationStoredInVariables() {return locationStoredInVariables;}
				void setLocationStoredInVariables(bool value) {locationStoredInVariables = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				StoreEventID(BaseEvent *pBase);
				StoreEventID(XMLNode EventNode, BaseEvent *pBase);
				StoreEventID(StorageFile &storageFile, BaseEvent *pBase);
				StoreEventID(StoreEventID *other, BaseEvent *pBase);
				int x;
				int y;
				int layer;
				int variableID;
				bool locationStoredInVariables;
		};
		class StoreTerrainID : public Event
		{
			public:
				~StoreTerrainID() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_STORETERRAINID;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				int getX() {return x;}
				void setX(int value) {x = value;}
				int getY() {return y;}
				void setY(int value) {y = value;}
				int getLayer() {return layer;}
				void setLayer(int value) {layer = value;}
				int getVariableID() {return variableID;}
				void setVariableID(int value) {variableID = value;}
				bool isLocationStoredInVariables() {return locationStoredInVariables;}
				void setLocationStoredInVariables(bool value) {locationStoredInVariables = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				StoreTerrainID(BaseEvent *pBase);
				StoreTerrainID(XMLNode EventNode, BaseEvent *pBase);
				StoreTerrainID(StorageFile &storageFile, BaseEvent *pBase);
				StoreTerrainID(StoreTerrainID *other, BaseEvent *pBase);
				int x;
				int y;
				int layer;
				int variableID;
				bool locationStoredInVariables;
		};
		class StringOperation : public Event
		{
			public:
				enum StringToChange {SINGLE=0, RANGE, REFERENCE};
				enum Operation {SET=0, ADD, REMOVE_FIRST_CHARACTER, REMOVE_LAST_CHARACTER};
				enum Operand {VALUE=0, STRING, VARIABLE, SWITCH, PARTY_MEMBER_NAME, PARTY_MEMBER_CLASS, CHARACTER_NAME, CHARACTER_CLASS, CLASS_NAME, TERRAIN_NAME, SKILL_NAME, SKILL_DESCRIPTION, ITEM_NAME, ITEM_DESCRIPTION, ATTRIBUTE_NAME, CONDITION_NAME, SYSTEM_STRING, VEHICLE_NAME, MONSTER_NAME, MONSTER_GROUP_NAME};
				~StringOperation() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_STRINGOPERATION;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				QString getValue1() {return value1;}
				void setValue1(QString value) {value1 = value;}
				QString getValue2() {return value2;}
				void setValue2(QString value) {value2 = value;}
				int getStringToChange() {return stringToChange;}
				void setStringToChange(int value) {stringToChange = value;}
				int getOperation() {return operation;}
				void setOperation(int value) {operation = value;}
				int getOperand() {return operand;}
				void setOperand(int value) {operand = value;}
				int getString1ID() {return string1ID;}
				void setString1ID(int value) {string1ID = value;}
				int getString2ID() {return string2ID;}
				void setString2ID(int value) {string2ID = value;}
				int getOperandID() {return operandID;}
				void setOperandID(int value) {operandID = value;}
				bool isOperandReferencedInVariable() {return operandReferencedInVariable;}
				void setOperandReferencedInVariable(bool value) {operandReferencedInVariable = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				StringOperation(BaseEvent *pBase);
				StringOperation(XMLNode EventNode, BaseEvent *pBase);
				StringOperation(StorageFile &storageFile, BaseEvent *pBase);
				StringOperation(StringOperation *other, BaseEvent *pBase);
				QString value1;
				QString value2;
				int stringToChange;
				int operation;
				int operand;
				int string1ID;
				int string2ID;
				int operandID;
				bool operandReferencedInVariable;
		};
		class SwapTile : public Event
		{
			public:
				enum Layer {LOWER=0, HERO, UPPER};
				~SwapTile() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_SWAPTILE;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				int getX() {return x;}
				void setX(int value) {x = value;}
				int getY() {return y;}
				void setY(int value) {y = value;}
				int getLayer() {return layer;}
				void setLayer(int value) {layer = value;}
				int getTilesetID() {return tilesetID;}
				void setTilesetID(int value) {tilesetID = value;}
				int getTileID() {return tileID;}
				void setTileID(int value) {tileID = value;}
				bool isLocationStoredInVariables() {return locationStoredInVariables;}
				void setLocationStoredInVariables(bool value) {locationStoredInVariables = value;}
				bool isTileReferencedInVariables() {return tileReferencedInVariables;}
				void setTileReferencedInVariables(bool value) {tileReferencedInVariables = value;}
				void applySwappedTiles(QList<Tileset::SwapTileIDs> swappedTiles);
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				SwapTile(BaseEvent *pBase);
				SwapTile(XMLNode EventNode, BaseEvent *pBase);
				SwapTile(StorageFile &storageFile, BaseEvent *pBase);
				SwapTile(SwapTile *other, BaseEvent *pBase);
				int x;
				int y;
				int layer;
				int tilesetID;
				int tileID;
				bool locationStoredInVariables;
				bool tileReferencedInVariables;
		};
		class SwitchOperation : public Event
		{
			public:
				enum SwitchToChange {SINGLE=0, RANGE, REFERENCE};
				enum Operation {TURN_ON=0, TURN_OFF, TOGGLE};
				~SwitchOperation() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_SWITCHOPERATION;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				int getSwitchToChange() {return switchToChange;}
				void setSwitchToChange(int value) {switchToChange = value;}
				int getOperation() {return operation;}
				void setOperation(int value) {operation = value;}
				int getSwitch1ID() {return switch1ID;}
				void setSwitch1ID(int value) {switch1ID = value;}
				int getSwitch2ID() {return switch2ID;}
				void setSwitch2ID(int value) {switch2ID = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				SwitchOperation(BaseEvent *pBase);
				SwitchOperation(XMLNode EventNode, BaseEvent *pBase);
				SwitchOperation(StorageFile &storageFile, BaseEvent *pBase);
				SwitchOperation(SwitchOperation *other, BaseEvent *pBase);
				int switchToChange;
				int operation;
				int switch1ID;
				int switch2ID;
		};
		class Teleport : public Event
		{
			public:
				enum Facing {RETAIN_CURRENT_FACING=0, FACE_UP, FACE_RIGHT, FACE_DOWN, FACE_LEFT};
				~Teleport() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_TELEPORT;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				int getMapID() {return mapID;}
				void setMapID(int value) {mapID = value;}
				int getX() {return x;}
				void setX(int value) {x = value;}
				int getY() {return y;}
				void setY(int value) {y = value;}
				int getLayer() {return layer;}
				void setLayer(int value) {layer = value;}
				int getFacing() {return facing;}
				void setFacing(int value) {facing = value;}
				bool areCoordinatesStoredInVariables() {return coordinatesStoredInVariables;}
				void setCoordinatesStoredInVariables(bool value) {coordinatesStoredInVariables = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				Teleport(BaseEvent *pBase);
				Teleport(XMLNode EventNode, BaseEvent *pBase);
				Teleport(StorageFile &storageFile, BaseEvent *pBase);
				Teleport(Teleport *other, BaseEvent *pBase);
				int mapID;
				int x;
				int y;
				int layer;
				int facing;
				bool coordinatesStoredInVariables;
		};
		class TileSubstitution : public Event
		{
			public:
				enum Layer {ALL=0, LOWER, HERO, UPPER};
				~TileSubstitution() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_TILESUBSTITUTION;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				int getTileset1ID() {return tileset1ID;}
				void setTileset1ID(int value) {tileset1ID = value;}
				int getTileset2ID() {return tileset2ID;}
				void setTileset2ID(int value) {tileset2ID = value;}
				int getTile1ID() {return tile1ID;}
				void setTile1ID(int value) {tile1ID = value;}
				int getTile2ID() {return tile2ID;}
				void setTile2ID(int value) {tile2ID = value;}
				bool isTile1ReferencedInVariables() {return tile1ReferencedInVariables;}
				void setTile1ReferencedInVariables(bool value) {tile1ReferencedInVariables = value;}
				bool isTile2ReferencedInVariables() {return tile2ReferencedInVariables;}
				void setTile2ReferencedInVariables(bool value) {tile2ReferencedInVariables = value;}
				void applySwappedTiles(QList<Tileset::SwapTileIDs> swappedTiles);
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				TileSubstitution(BaseEvent *pBase);
				TileSubstitution(XMLNode EventNode, BaseEvent *pBase);
				TileSubstitution(StorageFile &storageFile, BaseEvent *pBase);
				TileSubstitution(TileSubstitution *other, BaseEvent *pBase);
				int tileset1ID;
				int tileset2ID;
				int tile1ID;
				int tile2ID;
				int layer;
				bool tile1ReferencedInVariables;
				bool tile2ReferencedInVariables;
		};
		class TimerOperation : public Event
		{
			public:
				enum Operation {SET=0, START, STOP};
				enum HorizontalAlignment {HALIGN_LEFT=0, HALIGN_CENTER, HALIGN_RIGHT};
				enum VerticalAlignment {VALIGN_TOP=0, VALIGN_CENTER, VALIGN_BOTTOM};
				struct SetData
				{
					int minutes;
					int seconds;
					bool timeStoredInVariable;
				} setData;
				struct StartData
				{
					int horizontalAlignment;
					int verticalAlignment;
					bool displayTimerOnScreen;
					bool timerRunsDuringBattle;
				} startData;
				~TimerOperation() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_TIMEROPERATION;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				void getSetData(SetData *other);
				void setSetData(SetData *other);
				void getStartData(StartData *other);
				void setStartData(StartData *other);
				int getTimerID() {return timerID;}
				void setTimerID(int value) {timerID = value;}
				int getOperation() {return operation;}
				void setOperation(int value) {operation = value;}
				bool isTimerReferencedInVariable() {return timerReferencedInVariable;}
				void setTimerReferencedInVariable(bool value) {timerReferencedInVariable = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				TimerOperation(BaseEvent *pBase);
				TimerOperation(XMLNode EventNode, BaseEvent *pBase);
				TimerOperation(StorageFile &storageFile, BaseEvent *pBase);
				TimerOperation(TimerOperation *other, BaseEvent *pBase);
				union Data
				{
					SetData setData;
					StartData startData;
				} data;
				int timerID;
				int operation;
				bool timerReferencedInVariable;
		};
		class TintScreen : public Event
		{
			public:
				~TintScreen() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_TINTSCREEN;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				QColor getColor() {return color;}
				void setColor(QColor value) {color = value;}
				int getOpacity() {return opacity;}
				void setOpacity(int value) {opacity = value;}
				int getTransitionTime() {return transitionTime;}
				void setTransitionTime(int value) {transitionTime = value;}
				bool isOpacityStoredInVariable() {return opacityStoredInVariable;}
				void setOpacityStoredInVariable(bool value) {opacityStoredInVariable = value;}
				bool isTimeStoredInVariable() {return timeStoredInVariable;}
				void setTimeStoredInVariable(bool value) {timeStoredInVariable = value;}
				bool getHaltOtherProcesses() {return haltOtherProcesses;}
				void setHaltOtherProcesses(bool value) {haltOtherProcesses = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				TintScreen(BaseEvent *pBase);
				TintScreen(XMLNode EventNode, BaseEvent *pBase);
				TintScreen(StorageFile &storageFile, BaseEvent *pBase);
				TintScreen(TintScreen *other, BaseEvent *pBase);
				QColor color;
				int opacity;
				int transitionTime;
				bool opacityStoredInVariable;
				bool timeStoredInVariable;
				bool haltOtherProcesses;
		};
		class TradeTwoEventLocations : public Event
		{
			public:
				enum EventType {HERO, THIS_EVENT, VEHICLE, EVENT};
				~TradeTwoEventLocations() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_TRADETWOEVENTLOCATIONS;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				int getEvent1Type() {return event1Type;}
				void setEvent1Type(int value) {event1Type = value;}
				int getEvent1ID() {return event1ID;}
				void setEvent1ID(int value) {event1ID = value;}
				int getEvent2Type() {return event2Type;}
				void setEvent2Type(int value) {event2Type = value;}
				int getEvent2ID() {return event2ID;}
				void setEvent2ID(int value) {event2ID = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				TradeTwoEventLocations(BaseEvent *pBase);
				TradeTwoEventLocations(XMLNode EventNode, BaseEvent *pBase);
				TradeTwoEventLocations(StorageFile &storageFile, BaseEvent *pBase);
				TradeTwoEventLocations(TradeTwoEventLocations *other, BaseEvent *pBase);
				int event1Type;
				int event1ID;
				int event2Type;
				int event2ID;
		};
		class VariableMultiValueConditionalBranch : public Event
		{
			public:
				struct VariableValue
				{
					QList<BaseEvent*> events;
					int value;
					bool valueStoredInVariable;
				};
				~VariableMultiValueConditionalBranch();
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources();
				QStringList getViewableText(int indent);
				QList<BaseEvent*> getEventPointers();
				int getEventType() {return TYPE_VARIABLEMULTIVALUECONDITIONALBRANCH;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				void addValue(VariableValue *value);
				void removeValue(int location);
				VariableValue *getValue(int location) {return values[location];}
				int getNumValues() {return values.size();}
				int getVariableID() {return variableID;}
				void setVariableID(int value) {variableID = value;}
				bool getExecuteCustomHandlerIfNoValueMet() {return executeCustomHandlerIfNoValueMet;}
				void setExecuteCustomHandlerIfNoValueMet(bool value) {executeCustomHandlerIfNoValueMet = value;}
				void addChildEvent(BaseEvent *event, int location, int childTypeID);
				void deleteChildEvent(int location, int childTypeID);
				void applySwappedTiles(QList<Tileset::SwapTileIDs> swappedTiles);
				void applyChangeMenuObjectName(QString oldName, QString newName);
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				VariableMultiValueConditionalBranch(BaseEvent *pBase);
				VariableMultiValueConditionalBranch(XMLNode EventNode, BaseEvent *pBase);
				VariableMultiValueConditionalBranch(StorageFile &storageFile, BaseEvent *pBase);
				VariableMultiValueConditionalBranch(VariableMultiValueConditionalBranch *other, BaseEvent *pBase);
				QList<VariableValue*> values;
				QList<BaseEvent*> noValueMetEvents;
				int variableID;
				bool executeCustomHandlerIfNoValueMet;
		};
		class VariableOperation : public Event
		{
			public:
				enum Operation {SET=0, ADD, SUB, MUL, DIV, MOD};
				enum VariableToChange {SINGLE=0, RANGE, REFERENCE};
				enum Operand {VALUE=0, VARIABLE, RANDOM_NUMBER, ITEM, PARTY_MEMBER, CHARACTER, MONSTER, TIMER_SECONDS, COMPLEX_EXPRESSION, OTHER};
				enum ItemValue {NUM_HELD=0, NUM_EQUIPPED, BUY_PRICE, SELL_PRICE, ITEM_ATTACK, ITEM_DEFENSE, ITEM_WISDOM, ITEM_AGILITY};
				enum CharacterValue {LEVEL=0, EXPERIENCE, HP, MP, MAX_HP, MAX_MP, ATTACK, DEFENSE, WISDOM, AGILITY, WEAPON_ID, SHIELD_ID, ARMOR_ID, HELMET_ID, ACCESSORY_ID};
				enum MonsterValue {MONSTER_EXPERIENCE_GAIN=0, MONSTER_MONEY_GAIN, MONSTER_MAX_HP, MONSTER_MAX_MP, MONSTER_ATTACK, MONSTER_DEFENSE, MONSTER_WISDOM, MONSTER_AGILITY};
				enum OtherValue {MONEY=0, SIZE_OF_PARTY, NUM_SAVES, NUM_BATTLES, NUM_VICTORIES, NUM_DEFEATS, NUM_ESCAPES, MUSIC_MILLISECONDS};
				~VariableOperation() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_VARIABLEOPERATION;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				long getValue1() {return value1;}
				void setValue1(long value) {value1 = value;}
				long getValue2() {return value2;}
				void setValue2(long value) {value2 = value;}
				int getOperation() {return operation;}
				void setOperation(int value) {operation = value;}
				int getSrcVar1ID() {return srcVar1ID;}
				void setSrcVar1ID(int value) {srcVar1ID = value;}
				int getSrcVar2ID() {return srcVar2ID;}
				void setSrcVar2ID(int value) {srcVar2ID = value;}
				int getDstID() {return dstID;}
				void setDstID(int value) {dstID = value;}
				int getDstValue() {return dstValue;}
				void setDstValue(int value) {dstValue = value;}
				bool isDstIDStoredInVariable() {return dstIDStoredInVariable;}
				void setDstIDStoredInVariable(bool value) {dstIDStoredInVariable = value;}
				QString getExpression() {return expression;}
				void setExpression(QString value) {expression = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				VariableOperation(BaseEvent *pBase);
				VariableOperation(XMLNode EventNode, BaseEvent *pBase);
				VariableOperation(StorageFile &storageFile, BaseEvent *pBase);
				VariableOperation(VariableOperation *other, BaseEvent *pBase);
				QString expression;
				long value1;
				long value2;
				int operation;
				int variableToChange;
				int operand;
				int srcVar1ID;
				int srcVar2ID;
				int dstID;
				int dstValue;
				bool dstIDStoredInVariable;
		};
		class Wait : public Event
		{
			public:
				~Wait() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_WAIT;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				int getTime() {return time;}
				void setTime(int value) {time = value;}
				bool isTimeStoredInVariable() {return timeStoredInVariable;}
				void setTimeStoredInVariable(bool value) {timeStoredInVariable = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				Wait(BaseEvent *pBase);
				Wait(XMLNode EventNode, BaseEvent *pBase);
				Wait(StorageFile &storageFile, BaseEvent *pBase);
				Wait(Wait *other, BaseEvent *pBase);
				int time;
				bool timeStoredInVariable;
		};
		class WeatherEffects : public Event
		{
			public:
				enum Effect {NONE=0, RAIN, SNOW, FOG, SANDSTORM};
				enum Strength {WEAK=0, MEDIUM, STRONG};
				~WeatherEffects() {}
				XMLNode getEventNode();
				void saveToStorageFile(StorageFile &storageFile);
				QStringList getResources() {return QStringList();}
				QStringList getViewableText(int indent);
				int getEventType() {return TYPE_WEATHEREFFECTS;}
				#ifdef COMPILING_EDITOR
				bool configureEvent(int callerID, int callerType);
				#endif
				int getEffect() {return effect;}
				void setEffect(int value) {effect = value;}
				int getStrength() {return strength;}
				void setStrength(int value) {strength = value;}
				static bool tryLoad(XMLNode EventNode, int baseType);
				friend class Event;
			private:
				WeatherEffects(BaseEvent *pBase);
				WeatherEffects(XMLNode EventNode, BaseEvent *pBase);
				WeatherEffects(StorageFile &storageFile, BaseEvent *pBase);
				WeatherEffects(WeatherEffects *other, BaseEvent *pBase);
				int effect;
				int strength;
		};
		enum BaseEventType {MAPEVENT=0, MENUEVENT, BATTLEEVENT, COMMONEVENT};
		BaseEvent(int type, int event=TYPE_TOPLEVELEVENT);
		BaseEvent(int type, XMLNode EventNode);
		BaseEvent(int type, StorageFile &storageFile);
		BaseEvent(BaseEvent *other);
		~BaseEvent();
		XMLNode getEventNode();
		void saveToStorageFile(StorageFile &storageFile);
		QStringList getResources() {return eventData->getResources();}
		QStringList getViewableText(int indent) const {return eventData->getViewableText(indent);}
		QList<BaseEvent*> getEventPointers();
		void addChildEvent(BaseEvent *event, int location, int childTypeID) {eventData->addChildEvent(event, location, childTypeID);}
		void deleteChildEvent(int location, int childTypeID) {eventData->deleteChildEvent(location, childTypeID);}
		bool configureEvent(int callerID, int callerType) {return eventData->configureEvent(callerID, callerType);}
		int getBaseType() {return baseType;}
		int getEventType() {return eventData->getEventType();}
		QString getEventTypeName();
		void applySwappedTiles(QList<Tileset::SwapTileIDs> swappedTiles) {eventData->applySwappedTiles(swappedTiles);}
		void applyChangeMenuObjectName(QString oldName, QString newName) {eventData->applyChangeMenuObjectName(oldName, newName);}
		#ifdef COMPILING_EDITOR
		static BaseEvent *browseEvents(int type, int callerID, int callerType);
		#endif
		static bool tryLoad(XMLNode EventNode, int type);
	protected:
		Event *eventData;
		int baseType;
		static QStringList mapEventTypeNames;
		static QList<int> mapEventTypes;
		static QStringList menuEventTypeNames;
		static QList<int> menuEventTypes;
		static QStringList battleEventTypeNames;
		static QList<int> battleEventTypes;
		static QStringList commonEventTypeNames;
		static QList<int> commonEventTypes;
};

#endif // PROJECTDATA_BASEEVENT_H
