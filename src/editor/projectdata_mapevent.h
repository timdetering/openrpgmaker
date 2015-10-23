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

#ifndef PROJECTDATA_MAPEVENT_H
#define PROJECTDATA_MAPEVENT_H

class BaseEvent;

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "audiomanager.h"
#include "projectdata_tileset.h"
#include "storagefile.h"
#include "xmlParser.h"

class MapEvent
{
	public:
		friend class ProjectData;
		enum MovementPatterns
		{
			PATTERN_MOVEUPLEFT=0,
			PATTERN_MOVEUP,
			PATTERN_MOVEUPRIGHT,
			PATTERN_MOVERIGHT,
			PATTERN_MOVEDOWNRIGHT,
			PATTERN_MOVEDOWN,
			PATTERN_MOVEDOWNLEFT,
			PATTERN_MOVELEFT,
			PATTERN_MOVERANDOMLY,
			PATTERN_INCREASEMOVEMENTSPEED,
			PATTERN_DECREASEMOVEMENTSPEED,
			PATTERN_MOVEFORWARD,
			PATTERN_MOVEBACKWARD,
			PATTERN_MOVETOWARDHERO,
			PATTERN_MOVEAWAYFROMHERO,
			PATTERN_FACEUP,
			PATTERN_FACEDOWN,
			PATTERN_FACELEFT,
			PATTERN_FACERIGHT,
			PATTERN_TURNLEFT,
			PATTERN_TURNRIGHT,
			PATTERN_UTURN,
			PATTERN_FACERANDOMDIRECTION,
			PATTERN_FACELEFTRIGHTRANDOMLY,
			PATTERN_FACEHERO,
			PATTERN_FACEAWAYFROMHERO,
			PATTERN_INCREASEMOVEMENTFREQUENCY,
			PATTERN_DECREASEMOVEMENTFREQUENCY,
			PATTERN_BEGINJUMP,
			PATTERN_ENDJUMP,
			PATTERN_LOCKFACING,
			PATTERN_UNLOCKFACING,
			PATTERN_WAIT,
			PATTERN_PHASINGMODEOFF,
			PATTERN_PHASINGMODEON,
			PATTERN_STOPANIMATION,
			PATTERN_RESUMEANIMATION,
			PATTERN_CHANGEGRAPHIC,
			PATTERN_INCREASETRANSPARENCY,
			PATTERN_DECREASETRANSPARENCY,
			PATTERN_SWITCHON,
			PATTERN_SWITCHOFF,
			PATTERN_PLAYSOUNDEFFECT
		};
		enum SpritePose
		{
			POSE_RIGHTFOOT=0,
			POSE_IDLE=48,
			POSE_LEFTFOOT=96
		};
		enum SpriteDirection
		{
			DIRECTION_UP=0,
			DIRECTION_RIGHT=64,
			DIRECTION_DOWN=128,
			DIRECTION_LEFT=192
		};
		struct MovementPattern
		{
			MovementPattern(int type=PATTERN_MOVEUPLEFT);
			MovementPattern(XMLNode PatternNode);
			MovementPattern(StorageFile &storageFile);
			MovementPattern(MovementPattern *other);
			XMLNode getPatternNode();
			void saveToStorageFile(StorageFile &storageFile);
			static bool tryLoad(XMLNode PatternNode);
			AudioManager::SoundEffectInfo soundEffect;
			QString stringValue;
			int type;
			int intValue;
		};
		enum TriggerCondition
		{
			TRIGGER_ACTIONKEY=0,
			TRIGGER_TOUCHEDBYHERO,
			TRIGGER_COLLIDESWITHHERO,
			TRIGGER_COLLIDESWITHEVENT,
			TRIGGER_AUTOSTART,
			TRIGGER_PARALLELPROCESS
		};
		enum RelationToHero
		{
			RELATION_BELOWHERO=0,
			RELATION_BESIDEHERO,
			RELATION_ABOVEHERO
		};
		enum AnimationType
		{
			ANIMTYPE_NONCONTINUOUS=0,
			ANIMTYPE_CONTINUOUS,
			ANIMTYPE_FIXEDNONCONTINUOUS,
			ANIMTYPE_FIXEDCONTINUOUS,
			ANIMTYPE_FIXEDGRAPHIC,
			ANIMTYPE_SPINAROUND
		};
		enum MovementType
		{
			MOVETYPE_STATIONARY=0,
			MOVETYPE_RANDOM,
			MOVETYPE_VERTICAL,
			MOVETYPE_HORIZONTAL,
			MOVETYPE_TOWARDHERO,
			MOVETYPE_AWAYFROMHERO,
			MOVETYPE_CUSTOMPATTERN
		};
		enum MovementFrequency
		{
			FREQUENCY_ONEEIGHTHNORMAL=0,
			FREQUENCY_ONEFOURTHNORMAL,
			FREQUENCY_ONEHALFNORMAL,
			FREQUENCY_NORMAL,
			FREQUENCY_TWICENORMAL,
			FREQUENCY_FOURTIMESNORMAL
		};
		enum PreconditionsMet
		{
			MEET_ALL=0,
			MEET_ATLEAST,
			MEET_NONE
		};
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
		class Precondition
		{
			public:
				enum ConditionType {SWITCH=0, VARIABLE, STRING, TIMER, MONEY, ITEM, HERO, PARTY_IS_RIDING};
				Precondition();
				Precondition(XMLNode ConditionNode);
				Precondition(StorageFile &storageFile);
				Precondition(Precondition *other);
				~Precondition();
				XMLNode getConditionNode();
				void saveToStorageFile(StorageFile &storageFile);
				QString getViewableText() const;
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
				static bool tryLoad(XMLNode ConditionNode);
			private:
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
				} conditionUnion;
				int type;
		};
		class Page
		{
			public:
				Page();
				Page(XMLNode PageNode);
				Page(StorageFile &storageFile);
				Page(Page *other);
				~Page();
				XMLNode getPageNode();
				void saveToStorageFile(StorageFile &storageFile);
				int numPreconditions() {return preconditions.size();}
				Precondition *getPrecondition(int location) {return preconditions[location];}
				void addPrecondition(Precondition *condition) {preconditions.append(condition);}
				void deletePrecondition(int location) {delete (preconditions[location]);preconditions.removeAt(location);}
				int customMovementPatternSize() {return customMovementPattern.size();}
				MovementPattern *getMovementPattern(int location) {return customMovementPattern[location];}
				void addMovementPattern(MovementPattern *pattern) {customMovementPattern.append(pattern);}
				void deleteMovementPattern(int location) {delete (customMovementPattern[location]);customMovementPattern.removeAt(location);}
				BaseEvent *getEvent() {return event;}
				QString getSpriteLocation() {return spriteLocation;}
				void setSpriteLocation(QString value) {spriteLocation = value;}
				int getSpriteOpacity() {return spriteOpacity;}
				void setSpriteOpacity(int value) {spriteOpacity = value;}
				int getTriggerCondition() {return triggerCondition;}
				void setTriggerCondition(int value) {triggerCondition = value;}
				int getLayer() {return layer;}
				void setLayer(int value) {layer = value;}
				int getRelationToHero() {return relationToHero;}
				void setRelationToHero(int value) {relationToHero = value;}
				int getAnimationType() {return animationType;}
				void setAnimationType(int value) {animationType = value;}
				int getMovementFrequency() {return movementFrequency;}
				void setMovementFrequency(int value) {movementFrequency = value;}
				int getMovementType() {return movementType;}
				void setMovementType(int value) {movementType = value;}
				int getMovementSpeed() {return movementSpeed;}
				void setMovementSpeed(int value) {movementSpeed = value;}
				int getPreconditionsMet() {return preconditionsMet;}
				void setPreconditionsMet(int value) {preconditionsMet = value;}
				int getNumPreconditionsMet() {return numPreconditionsMet;}
				void setNumPreconditionsMet(int value) {numPreconditionsMet = value;}
				int getCollidesWithEventID() {return collidesWithEventID;}
				void setCollidesWithEventID(int value) {collidesWithEventID = value;}
				bool getRepeatMovementPattern() {return repeatMovementPattern;}
				void setRepeatMovementPattern(bool value) {repeatMovementPattern = value;}
				bool getIgnoreImpossibleMoves() {return ignoreImpossibleMoves;}
				void setIgnoreImpossibleMoves(bool value) {ignoreImpossibleMoves = value;}
				bool getForbidEventOverlap() {return forbidEventOverlap;}
				void setForbidEventOverlap(bool value) {forbidEventOverlap = value;}
				QImage getImage();
				QImage getImage(int direction, int pose);
				void applySwappedTiles(QList<Tileset::SwapTileIDs> swappedTiles);
				static bool tryLoad(XMLNode PageNode);
			private:
				QList<Precondition*> preconditions;
				QList<MovementPattern*> customMovementPattern;
				BaseEvent *event;
				QString spriteLocation;
				int spriteOpacity;
				int triggerCondition;
				int layer;
				int relationToHero;
				int animationType;
				int movementFrequency;
				int movementType;
				int movementSpeed;
				int preconditionsMet;
				int numPreconditionsMet;
				int collidesWithEventID;
				bool repeatMovementPattern;
				bool ignoreImpossibleMoves;
				bool forbidEventOverlap;
		};
		MapEvent();
		MapEvent(MapEvent *other);
		MapEvent(XMLNode MapEventNode);
		MapEvent(StorageFile &storageFile);
		~MapEvent();
		void saveToStorageFile(StorageFile &storageFile);
		XMLNode getMapEventNode();
		int numPages();
		Page *getPage(int pageID);
		void addPage(Page *page);
		void deletePage(int pageID);
		QString getName();
		void setName(QString value);
		int getXLocation();
		int getYLocation();
		void setLocation(int x, int y);
		void applySwappedTiles(QList<Tileset::SwapTileIDs> swappedTiles);
		static bool tryLoad(XMLNode EventNode);
	private:
		QList<Page*> pages;
		QString name;
		int xLocation;
		int yLocation;
};

#endif // PROJECTDATA_MAPEVENT_H
