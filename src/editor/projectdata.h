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

#ifndef PROJECTDATA_H
#define PROJECTDATA_H

#define DEBUG_MESSAGE "%s: %d\n", __FILE__, __LINE__
#define DO_PRAGMA(x) _Pragma(#x)
#define ERROR(x) DO_PRAGMA(message ("ERROR: "x))

#ifndef TODO
#if (TODO_LEVEL == 0)
#define TODO(x) //x
#elif (TODO_LEVEL == 1)
#define TODO(x) DO_PRAGMA(message ("TODO: "x))
#endif // TODO_LEVEL
#endif // TODO

#ifndef DEBUG_DATAREFERENCE
#define DEBUG_DATAREFERENCE false
#else
#undef DEBUG_DATAREFERENCE
#define DEBUG_DATAREFERENCE true
#endif

#ifdef COMPILING_EDITOR
#define RUNNING_IMPORTER false
#else
#define RUNNING_IMPORTER true
#endif

#define NUM_PHASES		24

class Attribute;
class BattleAnimation;
class BattleEvent;
class BattleLayout;
class Character;
class CharacterSprite;
class Class;
class CommonEvent;
class Condition;
class Font;
class GlobalAnimation;
class Item;
class MainMenuLayout;
class Map;
class MapEvent;
class MonsterAnimation;
class Monster;
class MonsterGroup;
class PopupMenuLayout;
class Skill;
class Terrain;
class Tile;
class Tileset;
class TitleMenuLayout;
class Vehicle;
class ProjectData;
class MainWindow;
class HelpManual;
class QDir;
class AudioManager;
class ProcessLogDialog;

#ifdef COMPILING_EDITOR
#include <QFileInfo>
#include <QDir>
#include <QPoint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <typeinfo>
#include "audiomanager.h"
#include "helpmanual.h"
#include "storagefile.h"
#include "xmlParser.h"
#else
#include <QMap>
#include <QString>
#include <QFileInfo>
#include <QDir>
#include <QPoint>
#include <QColor>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <typeinfo>
#include "audiomanager.h"
#include "storagefile.h"
#include "xmlParser.h"
#endif // COMPILING_EDITOR

bool operator<(const QPoint &p1, const QPoint &p2);

class ProjectData
{
	public:
		template<class T> class DataReference;
		template<class T> class ReferenceCounter;
		template<class T> class DataReference
		{
			public:
				DataReference(int dataID, const char *file, int line);
				~DataReference()
				{
					pCounter->removeReference(this);
					data = NULL;
					pCounter = NULL;
				}
				int getDataID() {return pCounter->dataID;}
				T *getDataPointer() {return data;}
				void saveToStorage() {pCounter->saveData();}
				void reloadFromStorage() {pCounter->reloadData();}
				void backupStorage() {pCounter->backupData();}
				void deleteBackupStorage() {pCounter->deleteBackupData();}
				void reloadBackupStorage() {pCounter->reloadBackupData();}
				T * operator->() {return data;}
				T const * operator->() const {return data;}
				T & operator*() {return *data;}
				T const & operator*() const {return *data;}
				friend class ProjectData;
			private:
				T *data;
				ReferenceCounter<T> *pCounter;
		};
		template<class T> class ReferenceCounter
		{
			public:
				ReferenceCounter(int id)
				{
					data = NULL;
					referenceCount = 0;
					dataID = id;
					setType();
				}
				~ReferenceCounter()
				{
					if (data != NULL)
					{
						if (dataReferences.size() > 0)
						{
							if (DEBUG_DATAREFERENCE)
								printf("ORPGM Warning: Floating references to project data found.\n");
							for (int i = 0; i < dataReferences.size(); ++i)
							{
								if (DEBUG_DATAREFERENCE)
									printf("  DataReference: file=\"%s\" line=%d\n", dataReferences[i].allocatedInFile.toUtf8().data(), dataReferences[i].allocatedOnLine);
								delete (dataReferences[i].dataReference);
							}
						}
						else if (DEBUG_DATAREFERENCE)
						{
							printf("ORPGM Warning: Something requested all data of type=");
							printType();
							printf(" to be loaded but never released the data.\n");
						}
						unloadData();
					}
				}
				T *getReference(ProjectData::DataReference<T> *dataReference, QString file, int line)
				{
					DataReferenceInfo info;
					incrementReferenceCount();
					info.dataReference = dataReference;
					info.allocatedInFile = file;
					info.allocatedOnLine = line;
					dataReferences.append(info);
					return data;
				}
				void removeReference(ProjectData::DataReference<T> *dataReference)
				{
					int index = -1;
					bool found = false;
					for (int i = 0; i < dataReferences.size() && !found; ++i)
					{
						if (dataReferences[i].dataReference == dataReference)
						{
							index = i;
							found = true;
						}
					}
					if (index != -1)
					{
						if (DEBUG_DATAREFERENCE)
						{
							printf("Deleting ");
							printType();
							printf("=%d file=\"%s\" line=%d\n", dataID, dataReferences[index].allocatedInFile.toUtf8().data(), dataReferences[index].allocatedOnLine);
						}
						dataReferences.removeAt(index);
					}
					decrementReferenceCount();
				}
				void incrementReferenceCount()
				{
					if (referenceCount == 0)
						loadData();
					++referenceCount;
				}
				void decrementReferenceCount()
				{
					--referenceCount;
					if (referenceCount == 0)
						unloadData();
				}
				friend class ProjectData;
			private:
				void setType()
				{
					if (typeid(T) == typeid(Attribute))
						type = TYPE_ATTRIBUTE;
					else if (typeid(T) == typeid(BattleAnimation))
						type = TYPE_BATTLEANIMATION;
					else if (typeid(T) == typeid(Character))
						type = TYPE_CHARACTER;
					else if (typeid(T) == typeid(CharacterSprite))
						type = TYPE_CHARACTERSPRITE;
					else if (typeid(T) == typeid(Class))
						type = TYPE_CLASS;
					else if (typeid(T) == typeid(CommonEvent))
						type = TYPE_COMMONEVENT;
					else if (typeid(T) == typeid(Condition))
						type = TYPE_CONDITION;
					else if (typeid(T) == typeid(Font))
						type = TYPE_FONT;
					else if (typeid(T) == typeid(GlobalAnimation))
						type = TYPE_GLOBALANIMATION;
					else if (typeid(T) == typeid(Item))
						type = TYPE_ITEM;
					else if (typeid(T) == typeid(MainMenuLayout))
						type = TYPE_MAINMENULAYOUT;
					else if (typeid(T) == typeid(Map))
						type = TYPE_MAP;
					else if (typeid(T) == typeid(MonsterAnimation))
						type = TYPE_MONSTERANIMATION;
					else if (typeid(T) == typeid(Monster))
						type = TYPE_MONSTER;
					else if (typeid(T) == typeid(MonsterGroup))
						type = TYPE_MONSTERGROUP;
					else if (typeid(T) == typeid(PopupMenuLayout))
						type = TYPE_POPUPMENULAYOUT;
					else if (typeid(T) == typeid(Skill))
						type = TYPE_SKILL;
					else if (typeid(T) == typeid(Terrain))
						type = TYPE_TERRAIN;
					else if (typeid(T) == typeid(Tileset))
						type = TYPE_TILESET;
					else if (typeid(T) == typeid(Vehicle))
						type = TYPE_VEHICLE;
					else
					{
						type = TYPE_UNKNOWN;
						printf("Unknown project data type accessed.");
						abort();
					}
				}
				void loadData()
				{
					StorageFile storageFile;
					storageFile.openFile(getStorageLocation().toLocal8Bit().data(), StorageFile::MODE_READ);
					data = new T(storageFile);
					storageFile.closeFile();
				}
				void unloadData()
				{
					if (data != NULL)
					{
						saveData();
						delete data;
						data = NULL;
					}
				}
				void reloadData()
				{
					if (data != NULL)
					{
						delete data;
						loadData();
						for (int i = 0; i < dataReferences.size(); ++i)
							dataReferences[i].dataReference->data = data;
					}
				}
				void saveData()
				{
					if (data != NULL)
					{
						StorageFile storageFile;
						storageFile.openFile(getStorageLocation().toLocal8Bit().data(), StorageFile::MODE_WRITE);
						data->saveToStorageFile(storageFile);
						storageFile.closeFile();
					}
				}
				void backupData()
				{
					QString storageLocation = getStorageLocation();
					QString backupLocation = storageLocation + ".bak";
					FILE *storageFile = fopen(storageLocation.toLocal8Bit().data(), "rb");
					FILE *backupFile = fopen(backupLocation.toLocal8Bit().data(), "wb");
					size_t result;
					char buffer[512];
					while (!feof(storageFile))
					{
						result = fread(buffer, 1, sizeof(buffer), storageFile);
						fwrite(buffer, 1, result, backupFile);
					}
					fclose(storageFile);
					fclose(backupFile);
				}
				void deleteBackupData()
				{
					QString backupLocation = getStorageLocation() + ".bak";
					QFileInfo info = QFileInfo(backupLocation);
					info.dir().remove(info.fileName());
				}
				void reloadBackupData()
				{
					QString storageLocation = getStorageLocation();
					QString backupLocation = storageLocation + ".bak";
					FILE *storageFile = fopen(storageLocation.toLocal8Bit().data(), "wb");
					FILE *backupFile = fopen(backupLocation.toLocal8Bit().data(), "rb");
					size_t result;
					char buffer[512];
					while (!feof(backupFile))
					{
						result = fread(buffer, 1, sizeof(buffer), backupFile);
						fwrite(buffer, 1, result, storageFile);
					}
					fclose(storageFile);
					fclose(backupFile);
					deleteBackupData();
					reloadData();
				}
				void changeDataID(int id) {dataID = id;}
				QString getStorageLocation()
				{
					QString storageLocation = ProjectData::homePath + "/storage/";
					switch (type)
					{
						case TYPE_UNKNOWN:
							storageLocation += "unkown";
							break;
						case TYPE_ATTRIBUTE:
							storageLocation += "attribute";
							break;
						case TYPE_BATTLEANIMATION:
							storageLocation += "battleanimation";
							break;
						case TYPE_CHARACTER:
							storageLocation += "character";
							break;
						case TYPE_CHARACTERSPRITE:
							storageLocation += "charactersprite";
							break;
						case TYPE_CLASS:
							storageLocation += "class";
							break;
						case TYPE_COMMONEVENT:
							storageLocation += "commonevent";
							break;
						case TYPE_CONDITION:
							storageLocation += "condition";
							break;
						case TYPE_FONT:
							storageLocation += "font";
							break;
						case TYPE_GLOBALANIMATION:
							storageLocation += "globalanimation";
							break;
						case TYPE_ITEM:
							storageLocation += "item";
							break;
						case TYPE_MAINMENULAYOUT:
							storageLocation += "mainmenulayout";
							break;
						case TYPE_MAP:
							storageLocation += "map";
							break;
						case TYPE_MONSTERANIMATION:
							storageLocation += "monsteranimation";
							break;
						case TYPE_MONSTER:
							storageLocation += "monster";
							break;
						case TYPE_MONSTERGROUP:
							storageLocation += "monstergroup";
							break;
						case TYPE_POPUPMENULAYOUT:
							storageLocation += "popupmenulayout";
							break;
						case TYPE_SKILL:
							storageLocation += "skill";
							break;
						case TYPE_TERRAIN:
							storageLocation += "terrain";
							break;
						case TYPE_TILESET:
							storageLocation += "tileset";
							break;
						case TYPE_VEHICLE:
							storageLocation += "vehicle";
							break;
					}
					storageLocation += QString("%1.storage").arg(dataID, 4, 10, QChar('0'));
					return storageLocation;
				}
				void printType()
				{
					switch (type)
					{
						case TYPE_UNKNOWN:
							printf("Unkown");
							break;
						case TYPE_ATTRIBUTE:
							printf("Attribute");
							break;
						case TYPE_BATTLEANIMATION:
							printf("BattleAnimation");
							break;
						case TYPE_CHARACTER:
							printf("Character");
							break;
						case TYPE_CHARACTERSPRITE:
							printf("CharacterSprite");
							break;
						case TYPE_CLASS:
							printf("Class");
							break;
						case TYPE_COMMONEVENT:
							printf("CommonEvent");
							break;
						case TYPE_CONDITION:
							printf("Condition");
							break;
						case TYPE_FONT:
							printf("Font");
							break;
						case TYPE_GLOBALANIMATION:
							printf("GlobalAnimation");
							break;
						case TYPE_ITEM:
							printf("Item");
							break;
						case TYPE_MAINMENULAYOUT:
							printf("MainMenuLayout");
							break;
						case TYPE_MAP:
							printf("Map");
							break;
						case TYPE_MONSTERANIMATION:
							printf("MonsterAnimation");
							break;
						case TYPE_MONSTER:
							printf("Monster");
							break;
						case TYPE_MONSTERGROUP:
							printf("MonsterGroup");
							break;
						case TYPE_POPUPMENULAYOUT:
							printf("PopupMenuLayout");
							break;
						case TYPE_SKILL:
							printf("Skill");
							break;
						case TYPE_TERRAIN:
							printf("Terrain");
							break;
						case TYPE_TILESET:
							printf("Tileset");
							break;
						case TYPE_VEHICLE:
							printf("Vehicle");
							break;
					}
				}
				enum ExpectedTypes
				{
					TYPE_UNKNOWN=-1,
					TYPE_ATTRIBUTE,
					TYPE_BATTLEANIMATION,
					TYPE_CHARACTER,
					TYPE_CHARACTERSPRITE,
					TYPE_CLASS,
					TYPE_COMMONEVENT,
					TYPE_CONDITION,
					TYPE_FONT,
					TYPE_GLOBALANIMATION,
					TYPE_ITEM,
					TYPE_MAINMENULAYOUT,
					TYPE_MAP,
					TYPE_MONSTERANIMATION,
					TYPE_MONSTER,
					TYPE_MONSTERGROUP,
					TYPE_POPUPMENULAYOUT,
					TYPE_SKILL,
					TYPE_TERRAIN,
					TYPE_TILESET,
					TYPE_VEHICLE
				};
				struct DataReferenceInfo
				{
					DataReference<T> *dataReference;
					QString allocatedInFile;
					int allocatedOnLine;
				};
				QList<DataReferenceInfo> dataReferences;
				T *data;
				ExpectedTypes type;
				int referenceCount;
				int dataID;
		};
#ifdef COMPILING_EDITOR
		static void setupProjectData();
		static bool createProject(QString gameName, QString dirName);
		static bool openProject(QString dirname);
		static void closeProject();
		static void cleanStorageData();
		static bool loadSystemData(QString filelocation);
		static void saveSystemData(QString fileLocation);
#endif // COMPILING_EDITOR
		static void recoverProject();
		static void loadImportedData();
		static void saveImportedData();
		static void loadFromStorage();
		static void saveToStorage();
#ifdef COMPILING_EDITOR
		static void changeName(QString name);
		static int addMap(QTreeWidgetItem *parent, XMLNode MapNode);
		static int addMap(QTreeWidgetItem *parent, Map *map);
		static int addMap(QTreeWidgetItem *parent);
		static void buildMapTree(XMLNode ParentNode);
		static void addMapArea(QTreeWidgetItem *parent, int areaID);
		static void deleteMap(QTreeWidgetItem *mapItem);
		static void deleteMapArea(QTreeWidgetItem *areaItem);
		static void changeCurrentMap(int mapID);
		static void changeCurrentTileset(int tilesetID);
		static int getSelectedMapID();
		static int getSelectedTilesetID();
		static XMLNode getProjectNode();
		static XMLNode getMapNode(QTreeWidgetItem *item);
		static void getSystemSFXInfo(int sfxID, AudioManager::SoundEffectInfo *soundInfo);
		static void setSystemSFXInfo(int sfxID, AudioManager::SoundEffectInfo soundInfo);
		static void getSystemBGMInfo(int bgmID, AudioManager::MusicInfo *musicInfo);
		static void setSystemBGMInfo(int bgmID, AudioManager::MusicInfo musicInfo);
		static void backupStorage();
		static void loadBackupStorage();
		static void cleanBackupStorage();
		static void saveProject();
		static void saveConfiguration();
#endif // COMPILING_EDITOR
		static int numberVariableIdFromString(QString string);
		static int stringVariableIdFromString(QString string);
		static int switchVariableIdFromString(QString string);
		static QString numberVariableName(int varID);
		static QString stringVariableName(int stringID);
		static QString switchVariableName(int switchID);
		static void setNumberVariableName(int varID, QString string);
		static void setStringVariableName(int stringID, QString string);
		static void setSwitchVariableName(int switchID, QString string);
		static QString numberVariableIdToString(int varID, bool includeName=true);
		static QString stringVariableIdToString(int stringID, bool includeName=true);
		static QString switchVariableIdToString(int switchID, bool includeName=true);
		static int numNumberVariables();
		static int numStringVariables();
		static int numSwitchVariables();
		static void resizeNumberVariables(int newSize);
		static void resizeStringVariables(int newSize);
		static void resizeSwitchVariables(int newSize);
		static QString getAbsoluteResourcePath(QString relativePath);
		static QString getRelativeResourcePath(QString absolutePath, bool changeGlobalToLocal=false);
#ifdef COMPILING_EDITOR
		static void saveAllDataReferencesToStorage();
#endif // COMPILING_EDITOR
		static void addAttribute(Attribute *value);
#ifdef COMPILING_EDITOR
		static void deleteAttribute(int num);
		static bool loadAttributes();
		static bool checkAttributes();
		static void saveAttributes();
#endif // COMPILING_EDITOR
		static void recoverAttributes();
		static void freeAttributes();
		static void addBattleAnimation(BattleAnimation *value);
#ifdef COMPILING_EDITOR
		static void deleteBattleAnimation(int num);
		static bool loadBattleAnimations();
		static bool checkBattleAnimations();
		static void saveBattleAnimations();
#endif // COMPILING_EDITOR
		static void recoverBattleAnimations();
		static void freeBattleAnimations();
		static void addCharacter(Character *value);
#ifdef COMPILING_EDITOR
		static void deleteCharacter(int num);
		static bool loadCharacters();
		static bool checkCharacters();
		static void saveCharacters();
#endif // COMPILING_EDITOR
		static void recoverCharacters();
		static void freeCharacters();
		static void addCharacterSprite(CharacterSprite *value);
#ifdef COMPILING_EDITOR
		static void deleteCharacterSprite(int num);
		static bool loadCharacterSprites();
		static bool checkCharacterSprites();
		static void saveCharacterSprites();
#endif // COMPILING_EDITOR
		static void recoverCharacterSprites();
		static void freeCharacterSprites();
		static void addClass(Class *value);
#ifdef COMPILING_EDITOR
		static void deleteClass(int num);
		static bool loadClasses();
		static bool checkClasses();
		static void saveClasses();
#endif // COMPILING_EDITOR
		static void recoverClasses();
		static void freeClasses();
		static void addCommonEvent(CommonEvent *value);
#ifdef COMPILING_EDITOR
		static void deleteCommonEvent(int num);
		static bool loadCommonEvents();
		static bool checkCommonEvents();
		static void saveCommonEvents();
#endif // COMPILING_EDITOR
		static void recoverCommonEvents();
		static void freeCommonEvents();
		static void addCondition(Condition *value);
#ifdef COMPILING_EDITOR
		static void deleteCondition(int num);
		static bool loadConditions();
		static bool checkConditions();
		static void saveConditions();
#endif // COMPILING_EDITOR
		static void recoverConditions();
		static void freeConditions();
		static void addFont(Font *value);
#ifdef COMPILING_EDITOR
		static void deleteFont(int num);
		static bool loadFonts();
		static bool checkFonts();
		static void saveFonts();
#endif // COMPILING_EDITOR
		static void recoverFonts();
		static void freeFonts();
		static void addGlobalAnimation(GlobalAnimation *value);
#ifdef COMPILING_EDITOR
		static void deleteGlobalAnimation(int num);
		static bool loadGlobalAnimations();
		static bool checkGlobalAnimations();
		static void saveGlobalAnimations();
#endif // COMPILING_EDITOR
		static void recoverGlobalAnimations();
		static void freeGlobalAnimations();
		static void addItem(Item *value);
#ifdef COMPILING_EDITOR
		static void deleteItem(int num);
		static bool loadItems();
		static bool checkItems();
		static void saveItems();
#endif // COMPILING_EDITOR
		static void recoverItems();
		static void freeItems();
		static int addMainMenuLayout(MainMenuLayout *value);
#ifdef COMPILING_EDITOR
		static void deleteMainMenuLayout(int num);
		static bool loadMainMenuLayouts();
		static bool checkMainMenuLayouts();
		static void saveMainMenuLayouts();
#endif // COMPILING_EDITOR
		static void recoverMainMenuLayouts();
		static void freeMainMenuLayouts();
		static int addMap(Map *value);
#ifdef COMPILING_EDITOR
		static void deleteMap(int num);
		static bool loadMaps();
		static bool checkMaps();
		static void saveMaps();
#endif // COMPILING_EDITOR
		static void recoverMaps();
		static void freeMaps();
		static void addMonsterAnimation(MonsterAnimation *value);
#ifdef COMPILING_EDITOR
		static void deleteMonsterAnimation(int num);
		static bool loadMonsterAnimations();
		static bool checkMonsterAnimations();
		static void saveMonsterAnimations();
#endif // COMPILING_EDITOR
		static void recoverMonsterAnimations();
		static void freeMonsterAnimations();
		static void addMonster(Monster *value);
#ifdef COMPILING_EDITOR
		static void deleteMonster(int num);
		static bool loadMonsters();
		static bool checkMonsters();
		static void saveMonsters();
#endif // COMPILING_EDITOR
		static void recoverMonsters();
		static void freeMonsters();
		static void addMonsterGroup(MonsterGroup *value);
#ifdef COMPILING_EDITOR
		static void deleteMonsterGroup(int num);
		static bool loadMonsterGroups();
		static bool checkMonsterGroups();
		static void saveMonsterGroups();
#endif // COMPILING_EDITOR
		static void recoverMonsterGroups();
		static void freeMonsterGroups();
		static int addPopupMenuLayout(PopupMenuLayout *value);
#ifdef COMPILING_EDITOR
		static void deletePopupMenuLayout(int num);
		static bool loadPopupMenuLayouts();
		static bool checkPopupMenuLayouts();
		static void savePopupMenuLayouts();
#endif // COMPILING_EDITOR
		static void recoverPopupMenuLayouts();
		static void freePopupMenuLayouts();
		static void addSkill(Skill *value);
#ifdef COMPILING_EDITOR
		static void deleteSkill(int num);
		static bool loadSkills();
		static bool checkSkills();
		static void saveSkills();
#endif // COMPILING_EDITOR
		static void recoverSkills();
		static void freeSkills();
		static void saveTitleMenuLayoutToStorage();
		static void loadTitleMenuLayoutFromStorage();
#ifdef COMPILING_EDITOR
		static bool loadTitleMenuLayout();
		static bool checkTitleMenuLayout();
		static void saveTitleMenuLayout();
#endif // COMPILING_EDITOR
		static void recoverTitleMenuLayout();
		static void freeTitleMenuLayout();
		static void addTerrain(Terrain *value);
#ifdef COMPILING_EDITOR
		static void deleteTerrain(int num);
		static bool loadTerrain();
		static bool checkTerrain();
		static void saveTerrain();
#endif // COMPILING_EDITOR
		static void recoverTerrain();
		static void freeTerrain();
		static void addTileset(Tileset *value);
#ifdef COMPILING_EDITOR
		static void deleteTileset(int num);
		static bool loadTilesets();
		static bool checkTilesets();
		static void saveTilesets();
#endif // COMPILING_EDITOR
		static void recoverTilesets();
		static void freeTilesets();
		static void addVehicle(Vehicle *value);
#ifdef COMPILING_EDITOR
		static void deleteVehicle(int num);
		static bool loadVehicles();
		static bool checkVehicles();
		static void saveVehicles();
#endif // COMPILING_EDITOR
		static void recoverVehicles();
		static void freeVehicles();
		enum StringTypes
		{
			STRING_BATTLEMSG_BEGIN=0,
			STRING_BATTLEMSG_INITIATIVE,
			STRING_BATTLEMSG_MISS,
			STRING_BATTLEMSG_FAILESCAPE,
			STRING_BATTLEMSG_VICTORY,
			STRING_BATTLEMSG_DEFEAT,
			STRING_BATTLEMSG_EXPERIENCEGAIN,
			STRING_BATTLEMSG_MONEYGAIN,
			STRING_BATTLEMSG_ITEMGAIN,
			STRING_LEVELUP,
			STRING_SKILLLEARNED,
			STRING_SHOPMSG_GREETING,
			STRING_SHOPMSG_AFTERFIRSTTRANSACTION,
			STRING_SHOPMSG_BUY,
			STRING_SHOPMSG_SELL,
			STRING_SHOPMSG_DONE,
			STRING_SHOPMSG_BUYITEM,
			STRING_SHOPMSG_BUYHOWMANY,
			STRING_SHOPMSG_BUYCOMPLETE,
			STRING_SHOPMSG_SELLITEM,
			STRING_SHOPMSG_SELLHOWMANY,
			STRING_SHOPMSG_SELLCOMPLETE,
			STRING_INNMSG_PRICEINTRO,
			STRING_INNMSG_ACCEPTINQUIRY,
			STRING_INNMSG_ACCEPT,
			STRING_INNMSG_DECLINE,
			STRING_OTHER_ITEMSPOSSESSED,
			STRING_OTHER_ITEMSEQUIPPED,
			STRING_OTHER_CURRENCY,
			STRING_STATSANDEQUIP_LEVEL,
			STRING_STATSANDEQUIP_LEVELSHORT,
			STRING_STATSANDEQUIP_HP,
			STRING_STATSANDEQUIP_HPSHORT,
			STRING_STATSANDEQUIP_MP,
			STRING_STATSANDEQUIP_MPSHORT,
			STRING_STATSANDEQUIP_NORMALSTATUS,
			STRING_STATSANDEQUIP_EXPERIENCE,
			STRING_STATSANDEQUIP_ATTACK,
			STRING_STATSANDEQUIP_DEFENSE,
			STRING_STATSANDEQUIP_WISDOM,
			STRING_STATSANDEQUIP_AGILITY,
			STRING_STATSANDEQUIP_MPUSED,
			STRING_STATSANDEQUIP_WEAPON,
			STRING_STATSANDEQUIP_SHIELD,
			STRING_STATSANDEQUIP_ARMOR,
			STRING_STATSANDEQUIP_HELMET,
			STRING_STATSANDEQUIP_ACCESSORY,
			STRING_COMMAND_FIGHT,
			STRING_COMMAND_AUTOFIGHT,
			STRING_COMMAND_ESCAPE,
			STRING_COMMAND_ITEM,
			STRING_COMMAND_SKILL,
			STRING_COMMAND_EQUIP,
			STRING_COMMAND_SAVE,
			STRING_COMMAND_QUIT,
			STRING_COMMAND_STATUS,
			STRING_COMMAND_ROW,
			STRING_COMMAND_ORDER,
			STRING_COMMAND_NEWGAME,
			STRING_COMMAND_LOADGAME,
			STRING_COMMAND_ENDGAME,
			STRING_COMMAND_WAITON,
			STRING_COMMAND_WAITOFF,
			STRING_SAVEQUITSCREENMSG_SAVESLOT,
			STRING_SAVEQUITSCREENMSG_LOADSLOT,
			STRING_SAVEQUITSCREENMSG_CONFIRMQUIT,
			STRING_SAVEQUITSCREENMSG_YES,
			STRING_SAVEQUITSCREENMSG_NO,
			STRING_MISC_PARTYSMONEY,
			STRING_MISC_TOTALTIMEPLAYED
		};
		enum EraseScreenTransitions
		{
			ERASE_FADEOUT=0,
			ERASE_REMOVEBLOCKS,
			ERASE_WIPEDOWNWARD,
			ERASE_WIPEUPWARD,
			ERASE_VENETIANBLINDS,
			ERASE_VERTICALBLINDS,
			ERASE_HORIZONTALBLINDS,
			ERASE_RECEDINGSQUARE,
			ERASE_EXPANDINGSQUARE,
			ERASE_SCREENMOVESUP,
			ERASE_SCREENMOVESDOWN,
			ERASE_SCREENMOVESLEFT,
			ERASE_SCREENMOVESRIGHT,
			ERASE_VERTICALDIVISION,
			ERASE_HORIZONTALDIVISION,
			ERASE_QUADRASECTION,
			ERASE_ZOOMIN,
			ERASE_MOSAIC,
			ERASE_WAVERSCREEN,
			ERASE_INSTANTANEOUS,
			ERASE_NOTRANSITION
		};
		enum ShowScreenTransitions
		{
			SHOW_FADEIN=0,
			SHOW_RECONSTITUTEBLOCKS,
			SHOW_UNWIPEDOWNWARD,
			SHOW_UNWIPEUPWARD,
			SHOW_VENETIANBLINDS,
			SHOW_VERTICALBLINDS,
			SHOW_HORIZONTALBLINDS,
			SHOW_RECEDINGSQUARE,
			SHOW_EXPANDINGSQUARE,
			SHOW_SCREENMOVESDOWN,
			SHOW_SCREENMOVESUP,
			SHOW_SCREENMOVESRIGHT,
			SHOW_SCREENMOVESLEFT,
			SHOW_VERTICALUNIFY,
			SHOW_HORIZONTALUNIFY,
			SHOW_UNIFYQUADRANTS,
			SHOW_ZOOMOUT,
			SHOW_MOSAIC,
			SHOW_WAVERSCREEN,
			SHOW_INSTANTANEOUS
		};
		struct ScreenTransition
		{
			int erase;
			int show;
		};
		enum BattleCommandTypes
		{
			COMMAND_ATTACK=0,
			COMMAND_SKILL,
			COMMAND_SKILLSUBSET,
			COMMAND_DEFEND,
			COMMAND_ITEM,
			COMMAND_ESCAPE,
			COMMAND_LINKTOEVENT
		};
		struct BattleCommand
		{
			QString name;
			int type;
		};
		enum ScreenTransitionTypes
		{
			SCREENTRANSITION_TELEPORT=0,
			SCREENTRANSITION_MENU,
			SCREENTRANSITION_BATTLEENTER,
			SCREENTRANSITION_BATTLEEXIT,
		};
		enum SystemBackgroundMusics
		{
			BGM_TITLESCREEN=0,
			BGM_BATTLE,
			BGM_VICTORY,
			BGM_INN,
			BGM_GAMEOVER
		};
		enum SystemSoundEffect
		{
			SFX_CURSOR=0,
			SFX_SELECTCHOICE,
			SFX_BUZZER,
			SFX_CANCEL,
			SFX_BATTLETRANSITION,
			SFX_FLEEBATTLE,
			SFX_CHARACTERLEVELUP,
			SFX_EVADEATTACK,
			SFX_ITEMUSED
		};
		enum MenuCommands
		{
			CMD_NONE=-1,
			CMD_ITEM=0,
			CMD_SKILL=1,
			CMD_EQUIP=2,
			CMD_SAVE=3,
			CMD_STATUS=4,
			CMD_ROW=5,
			CMD_ORDER=6,
			CMD_ATBMODE=7
		};
		struct SystemGraphics
		{
			QString frameBGLocation;
			QString frameBorderLocation;
			QString selectionBorderLocation;
			QString equipmentIndicatorsLocation;
			QString menuBGLocation;
			QString scrollArrowsLocation;
			QString grappleArmLocation;
			QString grappleHookLocation;
			QString timerNumbersLocation;
			QColor fontShadowColor;
			QColor defaultFontEnabledTopColor;
			QColor defaultFontEnabledBottomColor;
			QColor defaultFontDisabledTopColor;
			QColor defaultFontDisabledBottomColor;
			QString fontShadowLocation;
			QString defaultColor;
			bool stretchToFit;
		};
		enum SkillType
		{
			SKILLTYPE_NORMAL=0,
			SKILLTYPE_TELEPORT,
			SKILLTYPE_ESCAPE,
			SKILLTYPE_SWITCH
		};
		struct SkillSubset
		{
			QString name;
			int type;
		};
		struct StartLocation
		{
			int mapID;
			int x;
			int y;
			int layer;
		};
#ifdef COMPILING_EDITOR
		static MainWindow *mainwindow;
		static HelpManual *helpManual;
		static QTreeWidgetItem *mapItems;
#endif // COMPILING_EDITOR
		static QMap<int, QString> mapNames;
		static QMap<int, QString>::iterator mapNamesIter;
		static QMap<int, QString> mainMenuLayoutNames;
		static QMap<int, QString>::iterator mainMenuLayoutNamesIter;
		static QMap<int, QString> popupMenuLayoutNames;
		static QMap<int, QString>::iterator popupMenuLayoutNamesIter;
		static QList<BattleCommand*> battleCommands;
		static QList<SkillSubset*> skillSubsets;
		static QList<StartLocation> vehicleStartLocations;
		static BattleLayout *battleLayout;
		static TitleMenuLayout *titleMenuLayout;
		static QStringList attributeNames;
		static QStringList battleAnimationNames;
		static QStringList characterNames;
		static QStringList characterSpriteNames;
		static QStringList classNames;
		static QStringList commonEventNames;
		static QStringList conditionNames;
		static QStringList fontNames;
		static QStringList globalAnimationNames;
		static QStringList itemNames;
		static QStringList monsterAnimationNames;
		static QStringList monsterNames;
		static QStringList monsterGroupNames;
		static QStringList skillNames;
		static QStringList terrainNames;
		static QStringList tilesetNames;
		static QStringList vehicleNames;
		static QString gameName;
		static QString directoryName;
		static QString homePath;
		static QString titleLocation;
		static QString gameoverLocation;
		static QString projectLocation;
		static QString datadir;
		static QString projectsdir;
		static QString errorMessage;
		static QString fontCharacterList;
		static QString strings[70];
		static QString phaseNames[NUM_PHASES];
		static ScreenTransition screentransitions[4];
		static AudioManager::SoundEffectInfo systemSFX[9];
		static AudioManager::MusicInfo systemBGMs[5];
		static SystemGraphics systemGraphics;
		static StartLocation partyStartLocation;
		static AudioManager *audioManager;
		static int initialParty[4];
		static int mainMenuID;
		static int defaultFontID;
#ifdef COMPILING_EDITOR
		static int selectedMapID;
		static int selectedTilesetID;
		static int majorVersion;
		static int minorVersion;
		static int develVersion;
		static unsigned int randomSeed;
		static float tilesPerSecond[7];
#endif // COMPILING_EDITOR
		static bool saved;
#ifdef COMPILING_EDITOR
		static bool open;
#endif // COMPILING_EDITOR
		static bool battleAnimationsReversedFromBehind;
		static bool fullscreen;
		static bool showtitle;
	private:
#ifdef COMPILING_EDITOR
		static void copyDirectory(QString dirLocation, QString newLocation);
		static void copyFile(QString fileLocation, QString newLocation);
		static void deleteDirectory(QString dirLocation);
		static bool convertProjectToLatestVersion();
		static void convertProjectFromVersion0to1(ProcessLogDialog *log);
		static void convertProjectFromVersion1to2(ProcessLogDialog *log);
		static void convertProjectFromVersion2to3(ProcessLogDialog *log);
		static void convertProjectFromVersion3to4(ProcessLogDialog *log);
#endif // COMPILING_EDITOR
#ifndef COMPILING_EDITOR
	public:
#endif // COMPILING_EDITOR
		static XMLNode ImporterNode;
		static QMap<int, ReferenceCounter<MainMenuLayout>*> mainMenuLayouts;
		static QMap<int, ReferenceCounter<MainMenuLayout>*>::iterator mainMenuLayoutsIter;
		static QMap<int, ReferenceCounter<Map>*> maps;
		static QMap<int, ReferenceCounter<Map>*>::iterator mapsIter;
		static QMap<int, ReferenceCounter<PopupMenuLayout>*> popupMenuLayouts;
		static QMap<int, ReferenceCounter<PopupMenuLayout>*>::iterator popupMenuLayoutsIter;
		static QList<ReferenceCounter<Attribute>*> attributes;
		static QList<ReferenceCounter<BattleAnimation>*> battleAnimations;
		static QList<ReferenceCounter<Character>*> characters;
		static QList<ReferenceCounter<CharacterSprite>*> characterSprites;
		static QList<ReferenceCounter<Class>*> classes;
		static QList<ReferenceCounter<CommonEvent>*> commonEvents;
		static QList<ReferenceCounter<Condition>*> conditions;
		static QList<ReferenceCounter<Font>*> fonts;
		static QList<ReferenceCounter<GlobalAnimation>*> globalAnimations;
		static QList<ReferenceCounter<Item>*> items;
		static QList<ReferenceCounter<MonsterAnimation>*> monsterAnimations;
		static QList<ReferenceCounter<Monster>*> monsters;
		static QList<ReferenceCounter<MonsterGroup>*> monsterGroups;
		static QList<ReferenceCounter<Skill>*> skills;
		static QList<ReferenceCounter<Terrain>*> terrain;
		static QList<ReferenceCounter<Tileset>*> tilesets;
		static QList<ReferenceCounter<Vehicle>*> vehicles;
		static QStringList numberVariableNames;
		static QStringList stringVariableNames;
		static QStringList switchVariableNames;
};

#endif // PROJECTDATA_H
