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

#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QDateTime>
#include <QPixmap>
#include <QBitmap>
#include <QPainter>
#include <QCoreApplication>
#include <QPluginLoader>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <typeinfo>
#include "audiomanager.h"
#ifdef COMPILING_EDITOR
#include "convertprojectdialog.h"
#include "helpmanual.h"
#include "mainwindow.h"
#include "processlogdialog.h"
#endif // COMPILING_EDITOR
#include "projectdata.h"
#include "projectdata_attribute.h"
#include "projectdata_battleanimation.h"
#include "projectdata_battleevent.h"
#include "projectdata_battlelayout.h"
#include "projectdata_character.h"
#include "projectdata_charactersprite.h"
#include "projectdata_class.h"
#include "projectdata_commonevent.h"
#include "projectdata_condition.h"
#include "projectdata_font.h"
#include "projectdata_globalanimation.h"
#include "projectdata_item.h"
#include "projectdata_mainmenulayout.h"
#include "projectdata_map.h"
#include "projectdata_mapevent.h"
#include "projectdata_monsteranimation.h"
#include "projectdata_monster.h"
#include "projectdata_monstergroup.h"
#include "projectdata_popupmenulayout.h"
#include "projectdata_skill.h"
#include "projectdata_terrain.h"
#include "projectdata_tile.h"
#include "projectdata_tileset.h"
#include "projectdata_titlemenulayout.h"
#include "projectdata_vehicle.h"
#include "xmlParser.h"
#include "xmlTools.h"

#ifdef COMPILING_EDITOR
MainWindow *ProjectData::mainwindow = NULL;
HelpManual *ProjectData::helpManual = NULL;
QTreeWidgetItem *ProjectData::mapItems = NULL;
#endif // COMPILING_EDITOR
XMLNode ProjectData::ImporterNode;
QMap<int, QString> ProjectData::mapNames;
QMap<int, QString>::iterator ProjectData::mapNamesIter;
QMap<int, QString> ProjectData::mainMenuLayoutNames;
QMap<int, QString>::iterator ProjectData::mainMenuLayoutNamesIter;
QMap<int, QString> ProjectData::popupMenuLayoutNames;
QMap<int, QString>::iterator ProjectData::popupMenuLayoutNamesIter;
QMap<int, ProjectData::ReferenceCounter<MainMenuLayout>*> ProjectData::mainMenuLayouts;
QMap<int, ProjectData::ReferenceCounter<MainMenuLayout>*>::iterator ProjectData::mainMenuLayoutsIter;
QMap<int, ProjectData::ReferenceCounter<Map>*> ProjectData::maps;
QMap<int, ProjectData::ReferenceCounter<Map>*>::iterator ProjectData::mapsIter;
QMap<int, ProjectData::ReferenceCounter<PopupMenuLayout>*> ProjectData::popupMenuLayouts;
QMap<int, ProjectData::ReferenceCounter<PopupMenuLayout>*>::iterator ProjectData::popupMenuLayoutsIter;
QList<ProjectData::ReferenceCounter<Attribute>*> ProjectData::attributes;
QList<ProjectData::ReferenceCounter<BattleAnimation>*> ProjectData::battleAnimations;
QList<ProjectData::ReferenceCounter<Character>*> ProjectData::characters;
QList<ProjectData::ReferenceCounter<CharacterSprite>*> ProjectData::characterSprites;
QList<ProjectData::ReferenceCounter<Class>*> ProjectData::classes;
QList<ProjectData::ReferenceCounter<CommonEvent>*> ProjectData::commonEvents;
QList<ProjectData::ReferenceCounter<Condition>*> ProjectData::conditions;
QList<ProjectData::ReferenceCounter<Font>*> ProjectData::fonts;
QList<ProjectData::ReferenceCounter<GlobalAnimation>*> ProjectData::globalAnimations;
QList<ProjectData::ReferenceCounter<Item>*> ProjectData::items;
QList<ProjectData::ReferenceCounter<MonsterAnimation>*> ProjectData::monsterAnimations;
QList<ProjectData::ReferenceCounter<Monster>*> ProjectData::monsters;
QList<ProjectData::ReferenceCounter<MonsterGroup>*> ProjectData::monsterGroups;
QList<ProjectData::ReferenceCounter<Skill>*> ProjectData::skills;
QList<ProjectData::ReferenceCounter<Terrain>*> ProjectData::terrain;
QList<ProjectData::ReferenceCounter<Tileset>*> ProjectData::tilesets;
QList<ProjectData::ReferenceCounter<Vehicle>*> ProjectData::vehicles;
QList<ProjectData::BattleCommand*> ProjectData::battleCommands;
QList<ProjectData::SkillSubset*> ProjectData::skillSubsets;
QList<ProjectData::StartLocation> ProjectData::vehicleStartLocations;
BattleLayout *ProjectData::battleLayout;
TitleMenuLayout *ProjectData::titleMenuLayout;
QStringList ProjectData::attributeNames;
QStringList ProjectData::battleAnimationNames;
QStringList ProjectData::characterNames;
QStringList ProjectData::characterSpriteNames;
QStringList ProjectData::classNames;
QStringList ProjectData::commonEventNames;
QStringList ProjectData::conditionNames;
QStringList ProjectData::fontNames;
QStringList ProjectData::globalAnimationNames;
QStringList ProjectData::itemNames;
QStringList ProjectData::monsterAnimationNames;
QStringList ProjectData::monsterNames;
QStringList ProjectData::monsterGroupNames;
QStringList ProjectData::skillNames;
QStringList ProjectData::terrainNames;
QStringList ProjectData::tilesetNames;
QStringList ProjectData::vehicleNames;
QStringList ProjectData::stringVariableNames;
QStringList ProjectData::numberVariableNames;
QStringList ProjectData::switchVariableNames;
QString ProjectData::gameName;
QString ProjectData::directoryName;
QString ProjectData::homePath;
QString ProjectData::titleLocation;
QString ProjectData::gameoverLocation;
QString ProjectData::projectLocation;
QString ProjectData::datadir;
QString ProjectData::projectsdir;
QString ProjectData::errorMessage;
QString ProjectData::fontCharacterList = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789`~!@#$%^&*()_-+=[]{};':\",.<>/?\\|";
QString ProjectData::strings[70];
QString ProjectData::phaseNames[NUM_PHASES];
ProjectData::ScreenTransition ProjectData::screentransitions[4];
AudioManager::SoundEffectInfo ProjectData::systemSFX[9];
AudioManager::MusicInfo ProjectData::systemBGMs[5];
ProjectData::SystemGraphics ProjectData::systemGraphics;
ProjectData::StartLocation ProjectData::partyStartLocation = {-1, 0, 0, 1};
AudioManager *ProjectData::audioManager = NULL;
int ProjectData::initialParty[4];
int ProjectData::mainMenuID = 0;
int ProjectData::defaultFontID = 0;
#ifdef COMPILING_EDITOR
int ProjectData::selectedMapID = 0;
int ProjectData::selectedTilesetID = 0;
int ProjectData::majorVersion;
int ProjectData::minorVersion;
int ProjectData::develVersion;
unsigned int ProjectData::randomSeed;
float ProjectData::tilesPerSecond[7] = {0.125, 0.25, 0.5, 1.0, 2.0, 4.0, 8.0};
#endif // COMPILING_EDITOR
bool ProjectData::saved = true;
#ifdef COMPILING_EDITOR
bool ProjectData::open;
#endif // COMPILING_EDITOR
bool ProjectData::battleAnimationsReversedFromBehind;
bool ProjectData::fullscreen;
bool ProjectData::showtitle;

#define DECLARE_DATAREFERENCE_TYPE(type, list) \
template<> ProjectData::DataReference<type>::DataReference(int dataID, const char *file, int line)\
{\
	pCounter = list[dataID];\
	data = pCounter->getReference(this, QString(file), line);\
	if (DEBUG_DATAREFERENCE)\
		printf("Allocating "#type"=%d file=\"%s\" line=%d\n", dataID, file, line);\
}

template<class T> ProjectData::DataReference<T>::DataReference(int dataID, const char *file, int line)
{
	printf("ORPGM Error: Unknown project data type requested.\n");
	printf("  File: %s\n", file);
	printf("  Line: %d\n", line);
	abort();
}

DECLARE_DATAREFERENCE_TYPE(Attribute, ProjectData::attributes);
DECLARE_DATAREFERENCE_TYPE(BattleAnimation, ProjectData::battleAnimations);
DECLARE_DATAREFERENCE_TYPE(Character, ProjectData::characters);
DECLARE_DATAREFERENCE_TYPE(CharacterSprite, ProjectData::characterSprites);
DECLARE_DATAREFERENCE_TYPE(Class, ProjectData::classes);
DECLARE_DATAREFERENCE_TYPE(CommonEvent, ProjectData::commonEvents);
DECLARE_DATAREFERENCE_TYPE(Condition, ProjectData::conditions);
DECLARE_DATAREFERENCE_TYPE(Font, ProjectData::fonts);
DECLARE_DATAREFERENCE_TYPE(GlobalAnimation, ProjectData::globalAnimations);
DECLARE_DATAREFERENCE_TYPE(Item, ProjectData::items);
DECLARE_DATAREFERENCE_TYPE(MainMenuLayout, ProjectData::mainMenuLayouts);
DECLARE_DATAREFERENCE_TYPE(Map, ProjectData::maps);
DECLARE_DATAREFERENCE_TYPE(MonsterAnimation, ProjectData::monsterAnimations);
DECLARE_DATAREFERENCE_TYPE(Monster, ProjectData::monsters);
DECLARE_DATAREFERENCE_TYPE(MonsterGroup, ProjectData::monsterGroups);
DECLARE_DATAREFERENCE_TYPE(PopupMenuLayout, ProjectData::popupMenuLayouts);
DECLARE_DATAREFERENCE_TYPE(Skill, ProjectData::skills);
DECLARE_DATAREFERENCE_TYPE(Terrain, ProjectData::terrain);
DECLARE_DATAREFERENCE_TYPE(Tileset, ProjectData::tilesets);
DECLARE_DATAREFERENCE_TYPE(Vehicle, ProjectData::vehicles);

bool operator<(const QPoint &p1, const QPoint &p2)
{
	if (p1.x() < p2.x())
		return true;
	else if (p1.x() == p2.x() && p1.y() < p2.y())
		return true;
	return false;
}

#ifdef COMPILING_EDITOR
void ProjectData::setupProjectData()
{
	homePath = QDir::homePath() + "/.OpenRPGMaker";
	datadir = QFileInfo(QCoreApplication::applicationDirPath()).absoluteFilePath() + "/data";
	projectsdir = QDir::homePath() + "/.OpenRPGMaker/Projects";
	strings[STRING_MISC_PARTYSMONEY] = "0";
	strings[STRING_MISC_TOTALTIMEPLAYED] = "0:00";
}

bool ProjectData::createProject(QString name, QString dirName)
{
	QTreeWidgetItem *map;
	QDir projectDir;
	QDir homeDir = QDir::home();
	if (!homeDir.exists(".OpenRPGMaker"))
		homeDir.mkdir(".OpenRPGMaker");
	homeDir.cd(".OpenRPGMaker");
	homeDir.mkdir("storage");
	gameName = name;
	directoryName = dirName;
	selectedMapID = 0;
	selectedTilesetID = 0;
	projectDir.setPath(projectsdir);
	projectDir.mkdir(directoryName);
	projectDir.cd(directoryName);
	projectLocation = projectsdir + "/" + directoryName;
	projectDir.mkdir("attributes");
	copyDirectory(datadir + "/xmlfiles/attributes", projectLocation + "/attributes");
	projectDir.mkdir("battleanimations");
	copyDirectory(datadir + "/xmlfiles/battleanimations", projectLocation + "/battleanimations");
	projectDir.mkdir("characters");
	copyDirectory(datadir + "/xmlfiles/characters", projectLocation + "/characters");
	projectDir.mkdir("charactersprites");
	copyDirectory(datadir + "/xmlfiles/charactersprites", projectLocation + "/charactersprites");
	projectDir.mkdir("classes");
	copyDirectory(datadir + "/xmlfiles/classes", projectLocation + "/classes");
	projectDir.mkdir("commonevents");
	copyDirectory(datadir + "/xmlfiles/commonevents", projectLocation + "/commonevents");
	projectDir.mkdir("conditions");
	copyDirectory(datadir + "/xmlfiles/conditions", projectLocation + "/conditions");
	projectDir.mkdir("fonts");
	copyDirectory(datadir + "/xmlfiles/fonts", projectLocation + "/fonts");
	projectDir.mkdir("globalanimations");
	copyDirectory(datadir + "/xmlfiles/globalanimations", projectLocation + "/globalanimations");
	projectDir.mkdir("images");
	projectDir.mkdir("items");
	copyDirectory(datadir + "/xmlfiles/items", projectLocation + "/items");
	projectDir.mkdir("layouts");
	copyDirectory(datadir + "/xmlfiles/layouts", projectLocation + "/layouts");
	projectDir.mkdir("maps");
	copyDirectory(datadir + "/xmlfiles/maps", projectLocation + "/maps");
	projectDir.mkdir("monsteranimations");
	copyDirectory(datadir + "/xmlfiles/monsteranimations", projectLocation + "/monsteranimations");
	projectDir.mkdir("monsters");
	copyDirectory(datadir + "/xmlfiles/monsters", projectLocation + "/monsters");
	projectDir.mkdir("monstergroups");
	copyDirectory(datadir + "/xmlfiles/monstergroups", projectLocation + "/monstergroups");
	projectDir.mkdir("music");
	projectDir.mkdir("layouts");
	copyDirectory(datadir + "/xmlfiles/layouts", projectLocation + "/layouts");
	projectDir.mkdir("skills");
	copyDirectory(datadir + "/xmlfiles/skills", projectLocation + "/skills");
	projectDir.mkdir("sounds");
	projectDir.mkdir("terrain");
	copyDirectory(datadir + "/xmlfiles/terrain", projectLocation + "/terrain");
	projectDir.mkdir("tilesets");
	copyDirectory(datadir + "/xmlfiles/tilesets", projectLocation + "/tilesets");
	projectDir.mkdir("vehicles");
	copyDirectory(datadir + "/xmlfiles/vehicles", projectLocation + "/vehicles");
	copyFile(datadir + "/xmlfiles/system.xml", projectLocation + "/system.xml");
	projectDir.cd("images");
	projectDir.mkdir("animations");
	projectDir.mkdir("backgrounds");
	projectDir.mkdir("battlecharacters");
	projectDir.mkdir("faces");
	projectDir.mkdir("fonts");
	projectDir.mkdir("icons");
	projectDir.mkdir("monsters");
	projectDir.mkdir("sprites");
	projectDir.mkdir("system");
	projectDir.mkdir("tiles");
	projectDir.cdUp();
	projectDir.cd("sounds");
	projectDir.mkdir("ambient");
	projectDir.mkdir("battle");
	projectDir.mkdir("misc");
	if (!loadSystemData(projectLocation + QString("/system.xml")))
		return false;
	saveToStorage();
	for (int i = 0; i < fonts.size(); ++i)
		fonts[i]->incrementReferenceCount();
	for (int i = 0; i < tilesets.size(); ++i)
		tilesets[i]->incrementReferenceCount();
	mapItems = new QTreeWidgetItem();
	mapItems->setText(0, QString(gameName));
	mapItems->setText(1, QString("top"));
	mapItems->setText(2, QString("true"));
	mapItems->setIcon(0, QIcon(":/icons/folder_green.png"));
	mapItems->setFlags(Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEnabled);
	map = new QTreeWidgetItem();
	map->setText(0, QString("Map0000"));
	map->setText(1, QString("0"));
	map->setText(2, QString("false"));
	map->setText(3, QString("map"));
	map->setIcon(0, QIcon(":/icons/folder_green.png"));
	mapItems->addChild(map);
	mainwindow->twMapList->addTopLevelItem(mapItems);
	mainwindow->twMapList->setCurrentItem(map);
	for (int i = 0; i < 100; ++i)
	{
		stringVariableNames.append("");
		numberVariableNames.append("");
		switchVariableNames.append("");
	}
	open = true;
	saveProject();
	strings[STRING_MISC_PARTYSMONEY] = "0";
	strings[STRING_MISC_TOTALTIMEPLAYED] = "0:00";
	return true;
}

bool ProjectData::openProject(QString dirname)
{
	QString tempString;
	StorageFile storageFile;
	QFileInfo info;
	XMLNode TempNode, ProjectDataNode;
	QDir homeDir = QDir::home();
	int nummaps;
	if (dirname.startsWith("/"))
		dirname.remove(0, 1);
	info = QFileInfo(projectsdir + "/" + dirname);
	if (!info.exists())
		return false;
	if (!homeDir.exists(".OpenRPGMaker"))
		homeDir.mkdir(".OpenRPGMaker");
	homeDir.cd(".OpenRPGMaker");
	homeDir.mkdir("storage");
	directoryName = dirname;
	projectLocation = projectsdir + "/" + directoryName;
	if (!convertProjectToLatestVersion())
	{
		QMessageBox::critical(0, "Error", "Failed to convert project to the new file format specification.");
		open = false;
		return false;
	}
	ProjectDataNode = XMLNode::openFileHelper((projectLocation + QString("/projectdata.xml")).toLocal8Bit().data(), "openrpgmaker-projectdata");
	if (ProjectDataNode.isEmpty() == TRUE)
	{
		QMessageBox::critical(0, "XML Error", XMLNode::parseError);
		open = false;
		return false;
	}
	QCoreApplication::processEvents();
	gameName = ProjectDataNode.getAttribute("name");
	if (!loadSystemData(projectLocation + QString("/system.xml")))
		return false;
	QCoreApplication::processEvents();
	saveToStorage();
	QCoreApplication::processEvents();
	for (int i = 0; i < fonts.size(); ++i)
		fonts[i]->incrementReferenceCount();
	for (int i = 0; i < tilesets.size(); ++i)
		tilesets[i]->incrementReferenceCount();
	QCoreApplication::processEvents();
	mapItems = new QTreeWidgetItem();
	mapItems->setText(0, QString(ProjectDataNode.getAttribute("name")));
	mapItems->setText(1, QString("top"));
	mapItems->setText(2, QString("true"));
	mapItems->setIcon(0, QIcon(":/icons/folder_green.png"));
	mapItems->setFlags(Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEnabled);
	mainwindow->twMapList->addTopLevelItem(mapItems);
	mainwindow->aGameOptions_FS->setChecked(QString(ProjectDataNode.getAttribute("fullscreen")).toLower() == "true");
	mainwindow->aGameOptions_DisplayTitle->setChecked(QString(ProjectDataNode.getAttribute("displaytitle")).toLower() == "true");
	TempNode = ProjectDataNode.getChildNode("editor");
	QCoreApplication::processEvents();
	selectedTilesetID = atoi(TempNode.getAttribute("tileset"));
	if (selectedTilesetID < 0 || selectedTilesetID >= tilesets.size())
		selectedTilesetID = 0;
	QCoreApplication::processEvents();
	tempString = QString(TempNode.getAttribute("tool")).toLower();
	if (tempString == "select")
		mainwindow->toolMakeSelection->setChecked(true);
	else if (tempString == "rectangle")
		mainwindow->toolDrawRectangle->setChecked(true);
	else if (tempString == "circle")
		mainwindow->toolDrawCircle->setChecked(true);
	else if (tempString == "fill")
		mainwindow->toolBucketFill->setChecked(true);
	else if (tempString == "pencil")
		mainwindow->toolPencil->setChecked(true);
	else if (tempString == "tiling")
		mainwindow->toolTiling->setChecked(true);
	tempString = QString(TempNode.getAttribute("editMode")).toLower();
	if (tempString == "layers")
		mainwindow->aEditLayers->setChecked(true);
	else if (tempString == "passability")
		mainwindow->aEditPassability->setChecked(true);
	else if (tempString == "events")
		mainwindow->aEdit_EventLayer->setChecked(true);
	tempString = QString(TempNode.getAttribute("zoom"));
	if (tempString == "1:1")
		mainwindow->aZoom_1to1->setChecked(true);
	else if (tempString == "1:2")
		mainwindow->aZoom_1to2->setChecked(true);
	else if (tempString == "1:4")
		mainwindow->aZoom_1to4->setChecked(true);
	else if (tempString == "1:8")
		mainwindow->aZoom_1to8->setChecked(true);
	TempNode = ProjectDataNode.getChildNode("maplist");
	selectedMapID = QString(TempNode.getAttribute("selectedmap")).toInt();
	if (!maps.contains(selectedMapID))
		selectedMapID = maps.keys().first();
	nummaps = TempNode.nChildNode("map");
	QCoreApplication::processEvents();
	for (int i = 0; i < nummaps; ++i)
		addMap(mapItems, TempNode.getChildNode("map", i));
	QCoreApplication::processEvents();
	mapItems->setExpanded(true);
	saved = true;
	open = true;
	return true;
}

void ProjectData::closeProject()
{
	for (int i = 0; i < fonts.size(); ++i)
		fonts[i]->decrementReferenceCount();
	for (int i = 0; i < tilesets.size(); ++i)
		tilesets[i]->decrementReferenceCount();
	freeAttributes();
	freeBattleAnimations();
	freeCharacters();
	freeCharacterSprites();
	freeClasses();
	freeCommonEvents();
	freeConditions();
	freeFonts();
	freeGlobalAnimations();
	freeItems();
	freeMainMenuLayouts();
	freeMaps();
	freeMonsterAnimations();
	freeMonsters();
	freeMonsterGroups();
	freePopupMenuLayouts();
	freeSkills();
	freeTerrain();
	freeTilesets();
	freeVehicles();
	cleanStorageData();
	for (int i = 0; i < battleCommands.size(); ++i)
	{
		if (battleCommands[i] != NULL)
		{
			delete (battleCommands[i]);
			battleCommands[i] = NULL;
		}
	}
	for (int i = 0; i < skillSubsets.size(); ++i)
	{
		if (skillSubsets[i] != NULL)
		{
			delete (skillSubsets[i]);
			skillSubsets[i] = NULL;
		}
	}
	mapNames.clear();
	attributeNames.clear();
	battleAnimationNames.clear();
	characterNames.clear();
	characterSpriteNames.clear();
	classNames.clear();
	commonEventNames.clear();
	conditionNames.clear();
	fontNames.clear();
	globalAnimationNames.clear();
	itemNames.clear();
	mainMenuLayoutNames.clear();
	monsterAnimationNames.clear();
	monsterNames.clear();
	monsterGroupNames.clear();
	popupMenuLayoutNames.clear();
	skillNames.clear();
	terrainNames.clear();
	tilesetNames.clear();
	vehicleNames.clear();
	battleCommands.clear();
	skillSubsets.clear();
	stringVariableNames.clear();
	numberVariableNames.clear();
	switchVariableNames.clear();
	vehicleStartLocations.clear();
	open = false;
}

void ProjectData::cleanStorageData()
{
	QStringList storageFileList;
	QDir dir(homePath + "/storage");
	if (dir.exists())
	{
		storageFileList = dir.entryList();
		for (int i = 0; i < storageFileList.size(); ++i)
			dir.remove(storageFileList[i]);
		dir.setPath(homePath);
		dir.rmdir("storage");
	}
}

bool ProjectData::loadSystemData(QString filelocation)
{
	QFileInfo fileinfo;
	QString temp_string;
	int red, green, blue;
	XMLNode TempNode1, TempNode2, xmlSystemData = XMLNode::openFileHelper(filelocation.toLocal8Bit().data(), "openrpgmaker-system");
	if (xmlSystemData.isEmpty() == TRUE)
	{
		QMessageBox::critical(0, "XML Error", QString(XMLNode::parseError));
		open = false;
		return false;
	}
	QCoreApplication::processEvents();
	titleLocation = ProjectData::getAbsoluteResourcePath(xmlSystemData.getAttribute("title"));
	gameoverLocation = ProjectData::getAbsoluteResourcePath(xmlSystemData.getAttribute("gameover"));
	mainMenuID = QString(xmlSystemData.getAttribute("mainMenuID")).toInt();
	TempNode1 = xmlSystemData.getChildNode("startLocations");
	TempNode2 = TempNode1.getChildNode("party");
	partyStartLocation.mapID = QString(TempNode2.getAttribute("mapID")).toInt();
	partyStartLocation.x = QString(TempNode2.getAttribute("x")).toInt();
	partyStartLocation.y = QString(TempNode2.getAttribute("y")).toInt();
	partyStartLocation.layer = QString(TempNode2.getAttribute("layer")).toInt();
	vehicleStartLocations.clear();
	QCoreApplication::processEvents();
	for (int i = 0, count = TempNode1.nChildNode("vehicle"); i < count; ++i)
	{
		StartLocation location;
		TempNode2 = TempNode1.getChildNode("vehicle", i);
		location.mapID = QString(TempNode2.getAttribute("mapID")).toInt();
		location.x = QString(TempNode2.getAttribute("x")).toInt();
		location.y = QString(TempNode2.getAttribute("y")).toInt();
		location.layer = QString(TempNode2.getAttribute("layer")).toInt();
		vehicleStartLocations.append(location);
	}
	QCoreApplication::processEvents();
	TempNode1 = xmlSystemData.getChildNode("systemGraphics");
	systemGraphics.frameBGLocation = getAbsoluteResourcePath(TempNode1.getAttribute("frameBGLocation"));
	systemGraphics.frameBorderLocation = getAbsoluteResourcePath(TempNode1.getAttribute("frameBorderLocation"));
	systemGraphics.selectionBorderLocation = getAbsoluteResourcePath(TempNode1.getAttribute("selectionBorderLocation"));
	systemGraphics.equipmentIndicatorsLocation = getAbsoluteResourcePath(TempNode1.getAttribute("equipmentIndicatorsLocation"));
	systemGraphics.menuBGLocation = getAbsoluteResourcePath(TempNode1.getAttribute("menuBGLocation"));
	systemGraphics.scrollArrowsLocation = getAbsoluteResourcePath(TempNode1.getAttribute("scrollArrowsLocation"));
	systemGraphics.grappleArmLocation = getAbsoluteResourcePath(TempNode1.getAttribute("grappleArmLocation"));
	systemGraphics.grappleHookLocation = getAbsoluteResourcePath(TempNode1.getAttribute("grappleHookLocation"));
	systemGraphics.timerNumbersLocation = getAbsoluteResourcePath(TempNode1.getAttribute("timerNumbersLocation"));
	systemGraphics.fontShadowLocation = getAbsoluteResourcePath(TempNode1.getAttribute("fontShadowLocation"));
	systemGraphics.defaultColor = getAbsoluteResourcePath(TempNode1.getAttribute("defaultColor"));
	temp_string = TempNode1.getAttribute("fontShadowColor");
	red = temp_string.mid(1, 2).toInt(0, 16);
	green = temp_string.mid(3, 2).toInt(0, 16);
	blue = temp_string.mid(5, 2).toInt(0, 16);
	systemGraphics.fontShadowColor = QColor(red, green, blue);
	temp_string = TempNode1.getAttribute("defaultFontEnabledColor");
	red = temp_string.mid(1, 2).toInt(0, 16);
	green = temp_string.mid(3, 2).toInt(0, 16);
	blue = temp_string.mid(5, 2).toInt(0, 16);
	systemGraphics.defaultFontEnabledTopColor = QColor(red, green, blue);
	red = temp_string.mid(9, 2).toInt(0, 16);
	green = temp_string.mid(11, 2).toInt(0, 16);
	blue = temp_string.mid(13, 2).toInt(0, 16);
	systemGraphics.defaultFontEnabledBottomColor = QColor(red, green, blue);
	temp_string = TempNode1.getAttribute("defaultFontDisabledColor");
	red = temp_string.mid(1, 2).toInt(0, 16);
	green = temp_string.mid(3, 2).toInt(0, 16);
	blue = temp_string.mid(5, 2).toInt(0, 16);
	systemGraphics.defaultFontDisabledTopColor = QColor(red, green, blue);
	red = temp_string.mid(9, 2).toInt(0, 16);
	green = temp_string.mid(11, 2).toInt(0, 16);
	blue = temp_string.mid(13, 2).toInt(0, 16);
	systemGraphics.defaultFontDisabledBottomColor = QColor(red, green, blue);
	systemGraphics.stretchToFit = (QString(TempNode1.getAttribute("stretchToFit")) == "true") ? true:false;
	TempNode1 = xmlSystemData.getChildNode("strings");
	QCoreApplication::processEvents();
	for (int i = 0; i < 68; ++i)
		strings[i] = TempNode1.getChildNode("string", i).getAttribute("value");
	TempNode1 = xmlSystemData.getChildNode("randommapgenerators");
	TempNode1.writeToFile((homePath + "/storage/randommapgenerators.xml").toLocal8Bit().data());
	TempNode1 = xmlSystemData.getChildNode("variables");
	for (int i = 0; i < TempNode1.nChildNode("string"); ++i)
		stringVariableNames.append(TempNode1.getChildNode("string", i).getAttribute("name"));
	for (int i = 0; i < TempNode1.nChildNode("number"); ++i)
		numberVariableNames.append(TempNode1.getChildNode("number", i).getAttribute("name"));
	for (int i = 0; i < TempNode1.nChildNode("switch"); ++i)
		switchVariableNames.append(TempNode1.getChildNode("switch", i).getAttribute("name"));
	TempNode1 = xmlSystemData.getChildNode("battlecommands");
	for (int i = 0; i < TempNode1.nChildNode("command"); ++i)
	{
		BattleCommand *command = new BattleCommand;
		TempNode2 = TempNode1.getChildNode("command", i);
		command->name = TempNode2.getAttribute("name");
		temp_string = TempNode2.getAttribute("type");
		if (temp_string == "attack")
			command->type = COMMAND_ATTACK;
		else if (temp_string == "skill")
			command->type = COMMAND_SKILL;
		else if (temp_string == "skillsubset")
			command->type = COMMAND_SKILLSUBSET;
		else if (temp_string == "defend")
			command->type = COMMAND_DEFEND;
		else if (temp_string == "item")
			command->type = COMMAND_ITEM;
		else if (temp_string == "escape")
			command->type = COMMAND_ESCAPE;
		else
			command->type = COMMAND_LINKTOEVENT;
		battleCommands.append(command);
	}
	QCoreApplication::processEvents();
	skillSubsets.append(new SkillSubset);
	skillSubsets.append(new SkillSubset);
	skillSubsets.append(new SkillSubset);
	skillSubsets.append(new SkillSubset);
	skillSubsets[0]->name = "Normal";
	skillSubsets[0]->type = SKILLTYPE_NORMAL;
	skillSubsets[1]->name = "Teleport";
	skillSubsets[1]->type = SKILLTYPE_TELEPORT;
	skillSubsets[2]->name = "Escape";
	skillSubsets[2]->type = SKILLTYPE_ESCAPE;
	skillSubsets[3]->name = "Switch";
	skillSubsets[3]->type = SKILLTYPE_SWITCH;
	TempNode1 = xmlSystemData.getChildNode("skillSubsets");
	QCoreApplication::processEvents();
	for (int i = 0; i < TempNode1.nChildNode("subset"); ++i)
	{
		SkillSubset *subset = new SkillSubset;
		TempNode2 = TempNode1.getChildNode("subset", i);
		subset->name = TempNode2.getAttribute("name");
		temp_string = TempNode2.getAttribute("type");
		if (temp_string == "normal")
			subset->type = SKILLTYPE_NORMAL;
		else if (temp_string == "teleport")
			subset->type = SKILLTYPE_TELEPORT;
		else if (temp_string == "escape")
			subset->type = SKILLTYPE_ESCAPE;
		else if (temp_string == "switch")
			subset->type = SKILLTYPE_SWITCH;
		skillSubsets.append(subset);
	}
	QCoreApplication::processEvents();
	TempNode1 = xmlSystemData.getChildNode("screentransitions");
	screentransitions[SCREENTRANSITION_TELEPORT].erase = QString(TempNode1.getAttribute("teleportErase")).toInt();
	screentransitions[SCREENTRANSITION_TELEPORT].show = QString(TempNode1.getAttribute("teleportShow")).toInt();
	screentransitions[SCREENTRANSITION_MENU].erase = QString(TempNode1.getAttribute("menuErase")).toInt();
	screentransitions[SCREENTRANSITION_MENU].show = QString(TempNode1.getAttribute("menuShow")).toInt();
	screentransitions[SCREENTRANSITION_BATTLEENTER].erase = QString(TempNode1.getAttribute("battleEntryErase")).toInt();
	screentransitions[SCREENTRANSITION_BATTLEENTER].show = QString(TempNode1.getAttribute("battleEntryShow")).toInt();
	screentransitions[SCREENTRANSITION_BATTLEEXIT].erase = QString(TempNode1.getAttribute("battleExitErase")).toInt();
	screentransitions[SCREENTRANSITION_BATTLEEXIT].show = QString(TempNode1.getAttribute("battleExitShow")).toInt();
	TempNode1 = xmlSystemData.getChildNode("sfx");
	TempNode2 = TempNode1.getChildNode("cursor");
	systemSFX[SFX_CURSOR].location = getAbsoluteResourcePath(TempNode2.getAttribute("location"));
	systemSFX[SFX_CURSOR].volume = QString(TempNode2.getAttribute("volume")).toFloat();
	systemSFX[SFX_CURSOR].panning = QString(TempNode2.getAttribute("panning")).toFloat();
	systemSFX[SFX_CURSOR].speed = QString(TempNode2.getAttribute("speed")).toFloat();
	systemSFX[SFX_CURSOR].numLoops = QString(TempNode2.getAttribute("numloops")).toInt();
	systemSFX[SFX_CURSOR].retainPitch = (QString(TempNode2.getAttribute("retainpitch")) == "true") ? true:false;
	TempNode2 = TempNode1.getChildNode("selectchoice");
	systemSFX[SFX_SELECTCHOICE].location = getAbsoluteResourcePath(TempNode2.getAttribute("location"));
	systemSFX[SFX_SELECTCHOICE].volume = QString(TempNode2.getAttribute("volume")).toFloat();
	systemSFX[SFX_SELECTCHOICE].panning = QString(TempNode2.getAttribute("panning")).toFloat();
	systemSFX[SFX_SELECTCHOICE].speed = QString(TempNode2.getAttribute("speed")).toFloat();
	systemSFX[SFX_SELECTCHOICE].numLoops = QString(TempNode2.getAttribute("numloops")).toInt();
	systemSFX[SFX_SELECTCHOICE].retainPitch = (QString(TempNode2.getAttribute("retainpitch")) == "true") ? true:false;
	TempNode2 = TempNode1.getChildNode("buzzer");
	systemSFX[SFX_BUZZER].location = getAbsoluteResourcePath(TempNode2.getAttribute("location"));
	systemSFX[SFX_BUZZER].volume = QString(TempNode2.getAttribute("volume")).toFloat();
	systemSFX[SFX_BUZZER].panning = QString(TempNode2.getAttribute("panning")).toFloat();
	systemSFX[SFX_BUZZER].speed = QString(TempNode2.getAttribute("speed")).toFloat();
	systemSFX[SFX_BUZZER].numLoops = QString(TempNode2.getAttribute("numloops")).toInt();
	systemSFX[SFX_BUZZER].retainPitch = (QString(TempNode2.getAttribute("retainpitch")) == "true") ? true:false;
	TempNode2 = TempNode1.getChildNode("cancel");
	systemSFX[SFX_CANCEL].location = getAbsoluteResourcePath(TempNode2.getAttribute("location"));
	systemSFX[SFX_CANCEL].volume = QString(TempNode2.getAttribute("volume")).toFloat();
	systemSFX[SFX_CANCEL].panning = QString(TempNode2.getAttribute("panning")).toFloat();
	systemSFX[SFX_CANCEL].speed = QString(TempNode2.getAttribute("speed")).toFloat();
	systemSFX[SFX_CANCEL].numLoops = QString(TempNode2.getAttribute("numloops")).toInt();
	systemSFX[SFX_CANCEL].retainPitch = (QString(TempNode2.getAttribute("retainpitch")) == "true") ? true:false;
	TempNode2 = TempNode1.getChildNode("battletransition");
	systemSFX[SFX_BATTLETRANSITION].location = getAbsoluteResourcePath(TempNode2.getAttribute("location"));
	systemSFX[SFX_BATTLETRANSITION].volume = QString(TempNode2.getAttribute("volume")).toFloat();
	systemSFX[SFX_BATTLETRANSITION].panning = QString(TempNode2.getAttribute("panning")).toFloat();
	systemSFX[SFX_BATTLETRANSITION].speed = QString(TempNode2.getAttribute("speed")).toFloat();
	systemSFX[SFX_BATTLETRANSITION].numLoops = QString(TempNode2.getAttribute("numloops")).toInt();
	systemSFX[SFX_BATTLETRANSITION].retainPitch = (QString(TempNode2.getAttribute("retainpitch")) == "true") ? true:false;
	TempNode2 = TempNode1.getChildNode("fleebattle");
	systemSFX[SFX_FLEEBATTLE].location = getAbsoluteResourcePath(TempNode2.getAttribute("location"));
	systemSFX[SFX_FLEEBATTLE].volume = QString(TempNode2.getAttribute("volume")).toFloat();
	systemSFX[SFX_FLEEBATTLE].panning = QString(TempNode2.getAttribute("panning")).toFloat();
	systemSFX[SFX_FLEEBATTLE].speed = QString(TempNode2.getAttribute("speed")).toFloat();
	systemSFX[SFX_FLEEBATTLE].numLoops = QString(TempNode2.getAttribute("numloops")).toInt();
	systemSFX[SFX_FLEEBATTLE].retainPitch = (QString(TempNode2.getAttribute("retainpitch")) == "true") ? true:false;
	TempNode2 = TempNode1.getChildNode("characterlevelup");
	systemSFX[SFX_CHARACTERLEVELUP].location = getAbsoluteResourcePath(TempNode2.getAttribute("location"));
	systemSFX[SFX_CHARACTERLEVELUP].volume = QString(TempNode2.getAttribute("volume")).toFloat();
	systemSFX[SFX_CHARACTERLEVELUP].panning = QString(TempNode2.getAttribute("panning")).toFloat();
	systemSFX[SFX_CHARACTERLEVELUP].speed = QString(TempNode2.getAttribute("speed")).toFloat();
	systemSFX[SFX_CHARACTERLEVELUP].numLoops = QString(TempNode2.getAttribute("numloops")).toInt();
	systemSFX[SFX_CHARACTERLEVELUP].retainPitch = (QString(TempNode2.getAttribute("retainpitch")) == "true") ? true:false;
	TempNode2 = TempNode1.getChildNode("evadeattack");
	systemSFX[SFX_EVADEATTACK].location = getAbsoluteResourcePath(TempNode2.getAttribute("location"));
	systemSFX[SFX_EVADEATTACK].volume = QString(TempNode2.getAttribute("volume")).toFloat();
	systemSFX[SFX_EVADEATTACK].panning = QString(TempNode2.getAttribute("panning")).toFloat();
	systemSFX[SFX_EVADEATTACK].speed = QString(TempNode2.getAttribute("speed")).toFloat();
	systemSFX[SFX_EVADEATTACK].numLoops = QString(TempNode2.getAttribute("numloops")).toInt();
	systemSFX[SFX_EVADEATTACK].retainPitch = (QString(TempNode2.getAttribute("retainpitch")) == "true") ? true:false;
	TempNode2 = TempNode1.getChildNode("itemused");
	systemSFX[SFX_ITEMUSED].location = getAbsoluteResourcePath(TempNode2.getAttribute("location"));
	systemSFX[SFX_ITEMUSED].volume = QString(TempNode2.getAttribute("volume")).toFloat();
	systemSFX[SFX_ITEMUSED].panning = QString(TempNode2.getAttribute("panning")).toFloat();
	systemSFX[SFX_ITEMUSED].speed = QString(TempNode2.getAttribute("speed")).toFloat();
	systemSFX[SFX_ITEMUSED].numLoops = QString(TempNode2.getAttribute("numloops")).toInt();
	systemSFX[SFX_ITEMUSED].retainPitch = (QString(TempNode2.getAttribute("retainpitch")) == "true") ? true:false;
	TempNode1 = xmlSystemData.getChildNode("bgms");
	TempNode2 = TempNode1.getChildNode("titlescreen");
	systemBGMs[BGM_TITLESCREEN].location = getAbsoluteResourcePath(TempNode2.getAttribute("location"));
	systemBGMs[BGM_TITLESCREEN].volume = QString(TempNode2.getAttribute("volume")).toFloat();
	systemBGMs[BGM_TITLESCREEN].panning = QString(TempNode2.getAttribute("panning")).toFloat();
	systemBGMs[BGM_TITLESCREEN].speed = QString(TempNode2.getAttribute("speed")).toFloat();
	systemBGMs[BGM_TITLESCREEN].fadeInTime = QString(TempNode2.getAttribute("fadeintime")).toInt();
	systemBGMs[BGM_TITLESCREEN].retainPitch = (QString(TempNode2.getAttribute("retainpitch")) == "true") ? true:false;
	TempNode2 = TempNode1.getChildNode("battle");
	systemBGMs[BGM_BATTLE].location = getAbsoluteResourcePath(TempNode2.getAttribute("location"));
	systemBGMs[BGM_BATTLE].volume = QString(TempNode2.getAttribute("volume")).toFloat();
	systemBGMs[BGM_BATTLE].panning = QString(TempNode2.getAttribute("panning")).toFloat();
	systemBGMs[BGM_BATTLE].speed = QString(TempNode2.getAttribute("speed")).toFloat();
	systemBGMs[BGM_BATTLE].fadeInTime = QString(TempNode2.getAttribute("fadeintime")).toInt();
	systemBGMs[BGM_BATTLE].retainPitch = (QString(TempNode2.getAttribute("retainpitch")) == "true") ? true:false;
	TempNode2 = TempNode1.getChildNode("victory");
	systemBGMs[BGM_VICTORY].location = getAbsoluteResourcePath(TempNode2.getAttribute("location"));
	systemBGMs[BGM_VICTORY].volume = QString(TempNode2.getAttribute("volume")).toFloat();
	systemBGMs[BGM_VICTORY].panning = QString(TempNode2.getAttribute("panning")).toFloat();
	systemBGMs[BGM_VICTORY].speed = QString(TempNode2.getAttribute("speed")).toFloat();
	systemBGMs[BGM_VICTORY].fadeInTime = QString(TempNode2.getAttribute("fadeintime")).toInt();
	systemBGMs[BGM_VICTORY].retainPitch = (QString(TempNode2.getAttribute("retainpitch")) == "true") ? true:false;
	TempNode2 = TempNode1.getChildNode("inn");
	systemBGMs[BGM_INN].location = getAbsoluteResourcePath(TempNode2.getAttribute("location"));
	systemBGMs[BGM_INN].volume = QString(TempNode2.getAttribute("volume")).toFloat();
	systemBGMs[BGM_INN].panning = QString(TempNode2.getAttribute("panning")).toFloat();
	systemBGMs[BGM_INN].speed = QString(TempNode2.getAttribute("speed")).toFloat();
	systemBGMs[BGM_INN].fadeInTime = QString(TempNode2.getAttribute("fadeintime")).toInt();
	systemBGMs[BGM_INN].retainPitch = (QString(TempNode2.getAttribute("retainpitch")) == "true") ? true:false;
	TempNode2 = TempNode1.getChildNode("gameover");
	systemBGMs[BGM_GAMEOVER].location = getAbsoluteResourcePath(TempNode2.getAttribute("location"));
	systemBGMs[BGM_GAMEOVER].volume = QString(TempNode2.getAttribute("volume")).toFloat();
	systemBGMs[BGM_GAMEOVER].panning = QString(TempNode2.getAttribute("panning")).toFloat();
	systemBGMs[BGM_GAMEOVER].speed = QString(TempNode2.getAttribute("speed")).toFloat();
	systemBGMs[BGM_GAMEOVER].fadeInTime = QString(TempNode2.getAttribute("fadeintime")).toInt();
	systemBGMs[BGM_GAMEOVER].retainPitch = (QString(TempNode2.getAttribute("retainpitch")) == "true") ? true:false;
	TempNode1 = xmlSystemData.getChildNode("initialparty");
	initialParty[0] = QString(TempNode1.getAttribute("first")).toInt();
	initialParty[1] = QString(TempNode1.getAttribute("second")).toInt();
	initialParty[2] = QString(TempNode1.getAttribute("third")).toInt();
	initialParty[3] = QString(TempNode1.getAttribute("fourth")).toInt();
	QCoreApplication::processEvents();
	TempNode1 = xmlSystemData.getChildNode("phases");
	for (int i = 0; i < NUM_PHASES; ++i)
		phaseNames[i] = TempNode1.getChildNode("phase", i).getAttribute("name");
	phaseNames[0] = "Player Passability";
	phaseNames[1] = "Jumping Phase";
	phaseNames[2] = "Grapple Pass";
	phaseNames[3] = "Grapple Hook";
	phaseNames[4] = "Move Down (Start)";
	phaseNames[5] = "Move Down (End)";
	phaseNames[6] = "Move Up (Start)";
	phaseNames[7] = "Move Up (End)";
	QCoreApplication::processEvents();
	if (!checkAttributes())
	{
		closeProject();
		return false;
	}
	QCoreApplication::processEvents();
	if (!checkBattleAnimations())
	{
		closeProject();
		return false;
	}
	QCoreApplication::processEvents();
	if (!checkCharacters())
	{
		closeProject();
		return false;
	}
	QCoreApplication::processEvents();
	if (!checkCharacterSprites())
	{
		closeProject();
		return false;
	}
	QCoreApplication::processEvents();
	if (!checkClasses())
	{
		closeProject();
		return false;
	}
	QCoreApplication::processEvents();
	if (!checkCommonEvents())
	{
		closeProject();
		return false;
	}
	QCoreApplication::processEvents();
	if (!checkConditions())
	{
		closeProject();
		return false;
	}
	QCoreApplication::processEvents();
	if (!checkFonts())
	{
		closeProject();
		return false;
	}
	QCoreApplication::processEvents();
	if (!checkGlobalAnimations())
	{
		closeProject();
		return false;
	}
	QCoreApplication::processEvents();
	if (!checkItems())
	{
		closeProject();
		return false;
	}
	QCoreApplication::processEvents();
	if (!checkMainMenuLayouts())
	{
		closeProject();
		return false;
	}
	QCoreApplication::processEvents();
	if (!checkMaps())
	{
		closeProject();
		return false;
	}
	QCoreApplication::processEvents();
	if (!checkMonsterAnimations())
	{
		closeProject();
		return false;
	}
	QCoreApplication::processEvents();
	if (!checkMonsters())
	{
		closeProject();
		return false;
	}
	QCoreApplication::processEvents();
	if (!checkMonsterGroups())
	{
		closeProject();
		return false;
	}
	QCoreApplication::processEvents();
	if (!checkPopupMenuLayouts())
	{
		closeProject();
		return false;
	}
	QCoreApplication::processEvents();
	if (!checkSkills())
	{
		closeProject();
		return false;
	}
	QCoreApplication::processEvents();
	if (!checkTitleMenuLayout())
	{
		closeProject();
		return false;
	}
	QCoreApplication::processEvents();
	if (!checkTerrain())
	{
		closeProject();
		return false;
	}
	QCoreApplication::processEvents();
	if (!checkTilesets())
	{
		closeProject();
		return false;
	}
	QCoreApplication::processEvents();
	if (!checkVehicles())
	{
		closeProject();
		return false;
	}
	TODO("check then load battle layout.");
	QCoreApplication::processEvents();
	loadAttributes();
	QCoreApplication::processEvents();
	loadBattleAnimations();
	QCoreApplication::processEvents();
	loadCharacters();
	QCoreApplication::processEvents();
	loadCharacterSprites();
	QCoreApplication::processEvents();
	loadClasses();
	QCoreApplication::processEvents();
	loadCommonEvents();
	QCoreApplication::processEvents();
	loadConditions();
	QCoreApplication::processEvents();
	loadFonts();
	QCoreApplication::processEvents();
	loadGlobalAnimations();
	QCoreApplication::processEvents();
	loadItems();
	QCoreApplication::processEvents();
	loadMainMenuLayouts();
	QCoreApplication::processEvents();
	loadMaps();
	QCoreApplication::processEvents();
	loadMonsterAnimations();
	QCoreApplication::processEvents();
	loadMonsters();
	QCoreApplication::processEvents();
	loadMonsterGroups();
	QCoreApplication::processEvents();
	loadPopupMenuLayouts();
	QCoreApplication::processEvents();
	loadSkills();
	QCoreApplication::processEvents();
	loadTitleMenuLayout();
	QCoreApplication::processEvents();
	loadTerrain();
	QCoreApplication::processEvents();
	loadTilesets();
	QCoreApplication::processEvents();
	loadVehicles();
	QCoreApplication::processEvents();
	return true;
}

void ProjectData::saveSystemData(QString fileLocation)
{
	XMLNode TempNode1, TempNode2, SystemNode = XMLNode::createXMLTopNode("openrpgmaker-system");
	QString tempString;
	SystemNode.addAttribute("title", getRelativeResourcePath(titleLocation).toUtf8().data());
	SystemNode.addAttribute("gameover", getRelativeResourcePath(gameoverLocation).toUtf8().data());
	SystemNode.addAttribute("mainMenuID", QString::number(mainMenuID).toUtf8().data());
	TempNode1 = SystemNode.addChild("startLocations");
	TempNode2 = TempNode1.addChild("party");
	TempNode2.addAttribute("mapID", QString::number(partyStartLocation.mapID).toUtf8().data());
	TempNode2.addAttribute("x", QString::number(partyStartLocation.x).toUtf8().data());
	TempNode2.addAttribute("y", QString::number(partyStartLocation.y).toUtf8().data());
	TempNode2.addAttribute("layer", QString::number(partyStartLocation.layer).toUtf8().data());
	for (int i = 0; i < vehicleStartLocations.size(); ++i)
	{
		TempNode2 = TempNode1.addChild("vehicle");
		TempNode2.addAttribute("mapID", QString::number(vehicleStartLocations[i].mapID).toUtf8().data());
		TempNode2.addAttribute("x", QString::number(vehicleStartLocations[i].x).toUtf8().data());
		TempNode2.addAttribute("y", QString::number(vehicleStartLocations[i].y).toUtf8().data());
		TempNode2.addAttribute("layer", QString::number(vehicleStartLocations[i].layer).toUtf8().data());
	}
	TempNode1 = SystemNode.addChild("systemGraphics");
	TempNode1.addAttribute("frameBGLocation", getRelativeResourcePath(systemGraphics.frameBGLocation).toUtf8().data());
	TempNode1.addAttribute("frameBorderLocation", getRelativeResourcePath(systemGraphics.frameBorderLocation).toUtf8().data());
	TempNode1.addAttribute("selectionBorderLocation", getRelativeResourcePath(systemGraphics.selectionBorderLocation).toUtf8().data());
	TempNode1.addAttribute("equipmentIndicatorsLocation", getRelativeResourcePath(systemGraphics.equipmentIndicatorsLocation).toUtf8().data());
	TempNode1.addAttribute("menuBGLocation", getRelativeResourcePath(systemGraphics.menuBGLocation).toUtf8().data());
	TempNode1.addAttribute("scrollArrowsLocation", getRelativeResourcePath(systemGraphics.scrollArrowsLocation).toUtf8().data());
	TempNode1.addAttribute("grappleArmLocation", getRelativeResourcePath(systemGraphics.grappleArmLocation).toUtf8().data());
	TempNode1.addAttribute("grappleHookLocation", getRelativeResourcePath(systemGraphics.grappleHookLocation).toUtf8().data());
	TempNode1.addAttribute("timerNumbersLocation", getRelativeResourcePath(systemGraphics.timerNumbersLocation).toUtf8().data());
	TempNode1.addAttribute("fontShadowLocation", getRelativeResourcePath(systemGraphics.fontShadowLocation).toUtf8().data());
	TempNode1.addAttribute("defaultColor", getRelativeResourcePath(systemGraphics.defaultColor).toUtf8().data());
	tempString = QString("#%1%2%3").arg(systemGraphics.fontShadowColor.red(), 2, 16, QChar('0')).arg(systemGraphics.fontShadowColor.green(), 2, 16, QChar('0')).arg(systemGraphics.fontShadowColor.blue(), 2, 16, QChar('0'));
	TempNode1.addAttribute("fontShadowColor", tempString.toUtf8().data());
	tempString = QString("#%1%2%3").arg(systemGraphics.defaultFontEnabledTopColor.red(), 2, 16, QChar('0')).arg(systemGraphics.defaultFontEnabledTopColor.green(), 2, 16, QChar('0')).arg(systemGraphics.defaultFontEnabledTopColor.blue(), 2, 16, QChar('0'));
	tempString += QString("-#%1%2%3").arg(systemGraphics.defaultFontEnabledBottomColor.red(), 2, 16, QChar('0')).arg(systemGraphics.defaultFontEnabledBottomColor.green(), 2, 16, QChar('0')).arg(systemGraphics.defaultFontEnabledBottomColor.blue(), 2, 16, QChar('0'));
	TempNode1.addAttribute("defaultFontEnabledColor", tempString.toUtf8().data());
	tempString = QString("#%1%2%3").arg(systemGraphics.defaultFontDisabledTopColor.red(), 2, 16, QChar('0')).arg(systemGraphics.defaultFontDisabledTopColor.green(), 2, 16, QChar('0')).arg(systemGraphics.defaultFontDisabledTopColor.blue(), 2, 16, QChar('0'));
	tempString += QString("-#%1%2%3").arg(systemGraphics.defaultFontDisabledBottomColor.red(), 2, 16, QChar('0')).arg(systemGraphics.defaultFontDisabledBottomColor.green(), 2, 16, QChar('0')).arg(systemGraphics.defaultFontDisabledBottomColor.blue(), 2, 16, QChar('0'));
	TempNode1.addAttribute("defaultFontDisabledColor", tempString.toUtf8().data());
	TempNode1.addAttribute("stretchToFit", (systemGraphics.stretchToFit) ? "true":"false");
	TempNode1 = SystemNode.addChild("strings");
	for (int i = 0; i < 68; ++i)
		TempNode1.addChild("string").addAttribute("value", strings[i].toUtf8().data());
	TempNode2 = XMLNode::openFileHelper((homePath + "/storage/randommapgenerators.xml").toLocal8Bit().data(), "randommapgenerators");
	SystemNode.addChild(TempNode2);
	TempNode1 = SystemNode.addChild("variables");
	for (int i = 0; i < stringVariableNames.size(); ++i)
		TempNode1.addChild("string").addAttribute("name", stringVariableNames[i].toUtf8().data());
	for (int i = 0; i < numberVariableNames.size(); ++i)
		TempNode1.addChild("number").addAttribute("name", numberVariableNames[i].toUtf8().data());
	for (int i = 0; i < switchVariableNames.size(); ++i)
		TempNode1.addChild("switch").addAttribute("name", switchVariableNames[i].toUtf8().data());
	TempNode1 = SystemNode.addChild("battlecommands");
	for (int i = 0; i < battleCommands.size(); ++i)
	{
		TempNode2 = TempNode1.addChild("command");
		TempNode2.addAttribute("name", battleCommands[i]->name.toUtf8().data());
		if (battleCommands[i]->type == COMMAND_ATTACK)
			TempNode2.addAttribute("type", "attack");
		if (battleCommands[i]->type == COMMAND_SKILL)
			TempNode2.addAttribute("type", "skill");
		if (battleCommands[i]->type == COMMAND_SKILLSUBSET)
			TempNode2.addAttribute("type", "skillsubset");
		if (battleCommands[i]->type == COMMAND_DEFEND)
			TempNode2.addAttribute("type", "defend");
		if (battleCommands[i]->type == COMMAND_ITEM)
			TempNode2.addAttribute("type", "item");
		if (battleCommands[i]->type == COMMAND_ESCAPE)
			TempNode2.addAttribute("type", "escape");
		if (battleCommands[i]->type == COMMAND_LINKTOEVENT)
			TempNode2.addAttribute("type", "linktoevent");
	}
	TempNode1 = SystemNode.addChild("skillSubsets");
	for (int i = 4; i < skillSubsets.size(); ++i)
	{
		TempNode2 = TempNode1.addChild("subset");
		TempNode2.addAttribute("name", skillSubsets[i]->name.toUtf8().data());
		if (skillSubsets[i]->type == SKILLTYPE_NORMAL)
			TempNode2.addAttribute("type", "normal");
		else if (skillSubsets[i]->type == SKILLTYPE_TELEPORT)
			TempNode2.addAttribute("type", "teleport");
		else if (skillSubsets[i]->type == SKILLTYPE_ESCAPE)
			TempNode2.addAttribute("type", "escape");
		else if (skillSubsets[i]->type == SKILLTYPE_SWITCH)
			TempNode2.addAttribute("type", "switch");
	}
	TempNode1 = SystemNode.addChild("screentransitions");
	TempNode1.addAttribute("teleportErase", QString::number(screentransitions[SCREENTRANSITION_TELEPORT].erase).toUtf8().data());
	TempNode1.addAttribute("teleportShow", QString::number(screentransitions[SCREENTRANSITION_TELEPORT].show).toUtf8().data());
	TempNode1.addAttribute("menuErase", QString::number(screentransitions[SCREENTRANSITION_MENU].erase).toUtf8().data());
	TempNode1.addAttribute("menuShow", QString::number(screentransitions[SCREENTRANSITION_MENU].erase).toUtf8().data());
	TempNode1.addAttribute("battleEntryErase", QString::number(screentransitions[SCREENTRANSITION_BATTLEENTER].erase).toUtf8().data());
	TempNode1.addAttribute("battleEntryShow", QString::number(screentransitions[SCREENTRANSITION_BATTLEENTER].show).toUtf8().data());
	TempNode1.addAttribute("battleExitErase", QString::number(screentransitions[SCREENTRANSITION_BATTLEEXIT].erase).toUtf8().data());
	TempNode1.addAttribute("battleExitShow", QString::number(screentransitions[SCREENTRANSITION_BATTLEEXIT].show).toUtf8().data());
	TempNode1 = SystemNode.addChild("sfx");
	TempNode2 = TempNode1.addChild("cursor");
	TempNode2.addAttribute("location", ProjectData::getRelativeResourcePath(systemSFX[SFX_CURSOR].location).toUtf8().data());
	TempNode2.addAttribute("volume", QString::number(systemSFX[SFX_CURSOR].volume).toUtf8().data());
	TempNode2.addAttribute("panning", QString::number(systemSFX[SFX_CURSOR].panning).toUtf8().data());
	TempNode2.addAttribute("speed", QString::number(systemSFX[SFX_CURSOR].speed).toUtf8().data());
	TempNode2.addAttribute("numloops", QString::number(systemSFX[SFX_CURSOR].numLoops).toUtf8().data());
	TempNode2.addAttribute("retainpitch", (systemSFX[SFX_CURSOR].retainPitch) ? "true":"false");
	TempNode2 = TempNode1.addChild("selectchoice");
	TempNode2.addAttribute("location", ProjectData::getRelativeResourcePath(systemSFX[SFX_SELECTCHOICE].location).toUtf8().data());
	TempNode2.addAttribute("volume", QString::number(systemSFX[SFX_SELECTCHOICE].volume).toUtf8().data());
	TempNode2.addAttribute("panning", QString::number(systemSFX[SFX_SELECTCHOICE].panning).toUtf8().data());
	TempNode2.addAttribute("speed", QString::number(systemSFX[SFX_SELECTCHOICE].speed).toUtf8().data());
	TempNode2.addAttribute("numloops", QString::number(systemSFX[SFX_SELECTCHOICE].numLoops).toUtf8().data());
	TempNode2.addAttribute("retainpitch", (systemSFX[SFX_SELECTCHOICE].retainPitch) ? "true":"false");
	TempNode2 = TempNode1.addChild("buzzer");
	TempNode2.addAttribute("location", ProjectData::getRelativeResourcePath(systemSFX[SFX_BUZZER].location).toUtf8().data());
	TempNode2.addAttribute("volume", QString::number(systemSFX[SFX_BUZZER].volume).toUtf8().data());
	TempNode2.addAttribute("panning", QString::number(systemSFX[SFX_BUZZER].panning).toUtf8().data());
	TempNode2.addAttribute("speed", QString::number(systemSFX[SFX_BUZZER].speed).toUtf8().data());
	TempNode2.addAttribute("numloops", QString::number(systemSFX[SFX_BUZZER].numLoops).toUtf8().data());
	TempNode2.addAttribute("retainpitch", (systemSFX[SFX_BUZZER].retainPitch) ? "true":"false");
	TempNode2 = TempNode1.addChild("cancel");
	TempNode2.addAttribute("location", ProjectData::getRelativeResourcePath(systemSFX[SFX_CANCEL].location).toUtf8().data());
	TempNode2.addAttribute("volume", QString::number(systemSFX[SFX_CANCEL].volume).toUtf8().data());
	TempNode2.addAttribute("panning", QString::number(systemSFX[SFX_CANCEL].panning).toUtf8().data());
	TempNode2.addAttribute("speed", QString::number(systemSFX[SFX_CANCEL].speed).toUtf8().data());
	TempNode2.addAttribute("numloops", QString::number(systemSFX[SFX_CANCEL].numLoops).toUtf8().data());
	TempNode2.addAttribute("retainpitch", (systemSFX[SFX_CANCEL].retainPitch) ? "true":"false");
	TempNode2 = TempNode1.addChild("battletransition");
	TempNode2.addAttribute("location", ProjectData::getRelativeResourcePath(systemSFX[SFX_BATTLETRANSITION].location).toUtf8().data());
	TempNode2.addAttribute("volume", QString::number(systemSFX[SFX_BATTLETRANSITION].volume).toUtf8().data());
	TempNode2.addAttribute("panning", QString::number(systemSFX[SFX_BATTLETRANSITION].panning).toUtf8().data());
	TempNode2.addAttribute("speed", QString::number(systemSFX[SFX_BATTLETRANSITION].speed).toUtf8().data());
	TempNode2.addAttribute("numloops", QString::number(systemSFX[SFX_BATTLETRANSITION].numLoops).toUtf8().data());
	TempNode2.addAttribute("retainpitch", (systemSFX[SFX_BATTLETRANSITION].retainPitch) ? "true":"false");
	TempNode2 = TempNode1.addChild("fleebattle");
	TempNode2.addAttribute("location", ProjectData::getRelativeResourcePath(systemSFX[SFX_FLEEBATTLE].location).toUtf8().data());
	TempNode2.addAttribute("volume", QString::number(systemSFX[SFX_FLEEBATTLE].volume).toUtf8().data());
	TempNode2.addAttribute("panning", QString::number(systemSFX[SFX_FLEEBATTLE].panning).toUtf8().data());
	TempNode2.addAttribute("speed", QString::number(systemSFX[SFX_FLEEBATTLE].speed).toUtf8().data());
	TempNode2.addAttribute("numloops", QString::number(systemSFX[SFX_FLEEBATTLE].numLoops).toUtf8().data());
	TempNode2.addAttribute("retainpitch", (systemSFX[SFX_FLEEBATTLE].retainPitch) ? "true":"false");
	TempNode2 = TempNode1.addChild("characterlevelup");
	TempNode2.addAttribute("location", ProjectData::getRelativeResourcePath(systemSFX[SFX_CHARACTERLEVELUP].location).toUtf8().data());
	TempNode2.addAttribute("volume", QString::number(systemSFX[SFX_CHARACTERLEVELUP].volume).toUtf8().data());
	TempNode2.addAttribute("panning", QString::number(systemSFX[SFX_CHARACTERLEVELUP].panning).toUtf8().data());
	TempNode2.addAttribute("speed", QString::number(systemSFX[SFX_CHARACTERLEVELUP].speed).toUtf8().data());
	TempNode2.addAttribute("numloops", QString::number(systemSFX[SFX_CHARACTERLEVELUP].numLoops).toUtf8().data());
	TempNode2.addAttribute("retainpitch", (systemSFX[SFX_CHARACTERLEVELUP].retainPitch) ? "true":"false");
	TempNode2 = TempNode1.addChild("evadeattack");
	TempNode2.addAttribute("location", ProjectData::getRelativeResourcePath(systemSFX[SFX_EVADEATTACK].location).toUtf8().data());
	TempNode2.addAttribute("volume", QString::number(systemSFX[SFX_EVADEATTACK].volume).toUtf8().data());
	TempNode2.addAttribute("panning", QString::number(systemSFX[SFX_EVADEATTACK].panning).toUtf8().data());
	TempNode2.addAttribute("speed", QString::number(systemSFX[SFX_EVADEATTACK].speed).toUtf8().data());
	TempNode2.addAttribute("numloops", QString::number(systemSFX[SFX_EVADEATTACK].numLoops).toUtf8().data());
	TempNode2.addAttribute("retainpitch", (systemSFX[SFX_EVADEATTACK].retainPitch) ? "true":"false");
	TempNode2 = TempNode1.addChild("itemused");
	TempNode2.addAttribute("location", ProjectData::getRelativeResourcePath(systemSFX[SFX_ITEMUSED].location).toUtf8().data());
	TempNode2.addAttribute("volume", QString::number(systemSFX[SFX_ITEMUSED].volume).toUtf8().data());
	TempNode2.addAttribute("panning", QString::number(systemSFX[SFX_ITEMUSED].panning).toUtf8().data());
	TempNode2.addAttribute("speed", QString::number(systemSFX[SFX_ITEMUSED].speed).toUtf8().data());
	TempNode2.addAttribute("numloops", QString::number(systemSFX[SFX_ITEMUSED].numLoops).toUtf8().data());
	TempNode2.addAttribute("retainpitch", (systemSFX[SFX_ITEMUSED].retainPitch) ? "true":"false");
	TempNode1 = SystemNode.addChild("bgms");
	TempNode2 = TempNode1.addChild("titlescreen");
	TempNode2.addAttribute("location", ProjectData::getRelativeResourcePath(systemBGMs[BGM_TITLESCREEN].location).toUtf8().data());
	TempNode2.addAttribute("volume", QString::number(systemBGMs[BGM_TITLESCREEN].volume).toUtf8().data());
	TempNode2.addAttribute("panning", QString::number(systemBGMs[BGM_TITLESCREEN].panning).toUtf8().data());
	TempNode2.addAttribute("speed", QString::number(systemBGMs[BGM_TITLESCREEN].speed).toUtf8().data());
	TempNode2.addAttribute("fadeintime", QString::number(systemBGMs[BGM_TITLESCREEN].fadeInTime).toUtf8().data());
	TempNode2.addAttribute("retainpitch", (systemBGMs[BGM_TITLESCREEN].retainPitch) ? "true":"false");
	TempNode2 = TempNode1.addChild("battle");
	TempNode2.addAttribute("location", ProjectData::getRelativeResourcePath(systemBGMs[BGM_BATTLE].location).toUtf8().data());
	TempNode2.addAttribute("volume", QString::number(systemBGMs[BGM_BATTLE].volume).toUtf8().data());
	TempNode2.addAttribute("panning", QString::number(systemBGMs[BGM_BATTLE].panning).toUtf8().data());
	TempNode2.addAttribute("speed", QString::number(systemBGMs[BGM_BATTLE].speed).toUtf8().data());
	TempNode2.addAttribute("fadeintime", QString::number(systemBGMs[BGM_BATTLE].fadeInTime).toUtf8().data());
	TempNode2.addAttribute("retainpitch", (systemBGMs[BGM_BATTLE].retainPitch) ? "true":"false");
	TempNode2 = TempNode1.addChild("victory");
	TempNode2.addAttribute("location", ProjectData::getRelativeResourcePath(systemBGMs[BGM_VICTORY].location).toUtf8().data());
	TempNode2.addAttribute("volume", QString::number(systemBGMs[BGM_VICTORY].volume).toUtf8().data());
	TempNode2.addAttribute("panning", QString::number(systemBGMs[BGM_VICTORY].panning).toUtf8().data());
	TempNode2.addAttribute("speed", QString::number(systemBGMs[BGM_VICTORY].speed).toUtf8().data());
	TempNode2.addAttribute("fadeintime", QString::number(systemBGMs[BGM_VICTORY].fadeInTime).toUtf8().data());
	TempNode2.addAttribute("retainpitch", (systemBGMs[BGM_VICTORY].retainPitch) ? "true":"false");
	TempNode2 = TempNode1.addChild("inn");
	TempNode2.addAttribute("location", ProjectData::getRelativeResourcePath(systemBGMs[BGM_INN].location).toUtf8().data());
	TempNode2.addAttribute("volume", QString::number(systemBGMs[BGM_INN].volume).toUtf8().data());
	TempNode2.addAttribute("panning", QString::number(systemBGMs[BGM_INN].panning).toUtf8().data());
	TempNode2.addAttribute("speed", QString::number(systemBGMs[BGM_INN].speed).toUtf8().data());
	TempNode2.addAttribute("fadeintime", QString::number(systemBGMs[BGM_INN].fadeInTime).toUtf8().data());
	TempNode2.addAttribute("retainpitch", (systemBGMs[BGM_INN].retainPitch) ? "true":"false");
	TempNode2 = TempNode1.addChild("gameover");
	TempNode2.addAttribute("location", ProjectData::getRelativeResourcePath(systemBGMs[BGM_GAMEOVER].location).toUtf8().data());
	TempNode2.addAttribute("volume", QString::number(systemBGMs[BGM_GAMEOVER].volume).toUtf8().data());
	TempNode2.addAttribute("panning", QString::number(systemBGMs[BGM_GAMEOVER].panning).toUtf8().data());
	TempNode2.addAttribute("speed", QString::number(systemBGMs[BGM_GAMEOVER].speed).toUtf8().data());
	TempNode2.addAttribute("fadeintime", QString::number(systemBGMs[BGM_GAMEOVER].fadeInTime).toUtf8().data());
	TempNode2.addAttribute("retainpitch", (systemBGMs[BGM_GAMEOVER].retainPitch) ? "true":"false");
	TempNode1 = SystemNode.addChild("initialparty");
	TempNode1.addAttribute("first", QString::number(initialParty[0]).toUtf8().data());
	TempNode1.addAttribute("second", QString::number(initialParty[1]).toUtf8().data());
	TempNode1.addAttribute("third", QString::number(initialParty[2]).toUtf8().data());
	TempNode1.addAttribute("fourth", QString::number(initialParty[3]).toUtf8().data());
	TempNode1 = SystemNode.addChild("phases");
	for (int i = 0; i < NUM_PHASES; ++i)
	{
		TempNode2 = TempNode1.addChild("phase");
		TempNode2.addAttribute("name", phaseNames[i].toUtf8().data());
	}
	SystemNode.writeToFile(fileLocation.toLocal8Bit().data());
	TODO("load then save then free battle layout.");
	saveAttributes();
	saveBattleAnimations();
	saveCharacters();
	saveCharacterSprites();
	saveClasses();
	saveCommonEvents();
	saveConditions();
	saveFonts();
	saveGlobalAnimations();
	saveItems();
	saveMainMenuLayouts();
	saveMaps();
	saveMonsterAnimations();
	saveMonsters();
	saveMonsterGroups();
	savePopupMenuLayouts();
	saveSkills();
	saveTitleMenuLayout();
	saveTerrain();
	saveTilesets();
	saveVehicles();
}
#endif // COMPILING_EDITOR

#ifdef COMPILING_EDITOR
void ProjectData::recoverProject()
{
	QFileInfo fileInfo(homePath + "/storage/projectdata.storage");
	if (fileInfo.exists())
	{
		QCoreApplication::processEvents();
		loadFromStorage();
		QCoreApplication::processEvents();
		recoverAttributes();
		QCoreApplication::processEvents();
		recoverBattleAnimations();
		QCoreApplication::processEvents();
		recoverCharacters();
		QCoreApplication::processEvents();
		recoverCharacterSprites();
		QCoreApplication::processEvents();
		recoverClasses();
		QCoreApplication::processEvents();
		recoverCommonEvents();
		QCoreApplication::processEvents();
		recoverConditions();
		QCoreApplication::processEvents();
		recoverFonts();
		QCoreApplication::processEvents();
		recoverGlobalAnimations();
		QCoreApplication::processEvents();
		recoverItems();
		QCoreApplication::processEvents();
		recoverMainMenuLayouts();
		QCoreApplication::processEvents();
		recoverMaps();
		QCoreApplication::processEvents();
		recoverMonsterAnimations();
		QCoreApplication::processEvents();
		recoverMonsters();
		QCoreApplication::processEvents();
		recoverMonsterGroups();
		QCoreApplication::processEvents();
		recoverPopupMenuLayouts();
		QCoreApplication::processEvents();
		recoverSkills();
		QCoreApplication::processEvents();
		recoverTitleMenuLayout();
		QCoreApplication::processEvents();
		recoverTerrain();
		QCoreApplication::processEvents();
		recoverTilesets();
		QCoreApplication::processEvents();
		recoverVehicles();
		QCoreApplication::processEvents();
		selectedMapID = 0;
		selectedTilesetID = 0;
		mapItems = new QTreeWidgetItem();
		mapItems->setText(0, QString(gameName));
		mapItems->setText(1, QString("top"));
		mapItems->setText(2, QString("true"));
		mapItems->setIcon(0, QIcon(":/icons/folder_green.png"));
		mapItems->setFlags(Qt::ItemIsEditable|Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled|Qt::ItemIsEnabled);
		mainwindow->twMapList->addTopLevelItem(mapItems);
		for (mapsIter = maps.begin(); mapsIter != maps.end(); ++mapsIter)
		{
			DataReference<Map> mapRef(mapsIter.key(), __FILE__, __LINE__);
			if (mapRef->getParent() == -1)
			{
				XMLNode MapNode = XMLNode::createXMLTopNode("map");
				MapNode.addAttribute("id", QString::number(mapsIter.key(), 16).toUtf8().data());
				MapNode.addAttribute("name", mapRef->getName().toUtf8().data());
				MapNode.addAttribute("expanded", "false");
				buildMapTree(MapNode);
				addMap(mapItems, MapNode);
			}
		}
	}
	open = true;
	saved = false;
}
#else
void ProjectData::recoverProject()
{
	QFileInfo fileInfo(homePath + "/storage/projectdata.storage");
	if (fileInfo.exists())
	{
		loadFromStorage();
		recoverAttributes();
		recoverBattleAnimations();
		recoverCharacters();
		recoverCharacterSprites();
		recoverClasses();
		recoverCommonEvents();
		recoverConditions();
		recoverFonts();
		recoverGlobalAnimations();
		recoverItems();
		recoverMainMenuLayouts();
		recoverMaps();
		recoverMonsterAnimations();
		recoverMonsters();
		recoverMonsterGroups();
		recoverPopupMenuLayouts();
		recoverSkills();
		recoverTitleMenuLayout();
		recoverTerrain();
		recoverTilesets();
		recoverVehicles();
	}
}
#endif // COMPILING_EDITOR

void ProjectData::loadFromStorage()
{
	StorageFile storageFile;
	unsigned long color;
	char *temp_string;
	storageFile.openFile((homePath + "/storage/projectdata.storage").toLocal8Bit().data(), StorageFile::MODE_READ);
	temp_string = storageFile.getString();
	gameName = temp_string;
	delete temp_string;
	temp_string = storageFile.getString();
	directoryName = temp_string;
	delete temp_string;
	temp_string = storageFile.getString();
	titleLocation = temp_string;
	delete temp_string;
	temp_string = storageFile.getString();
	gameoverLocation = temp_string;
	delete temp_string;
	temp_string = storageFile.getString();
	projectLocation = temp_string;
	delete temp_string;
	temp_string = storageFile.getString();
	datadir = temp_string;
	delete temp_string;
	temp_string = storageFile.getString();
	projectsdir = temp_string;
	delete temp_string;
	mainMenuID = storageFile.getSignedInt();
	partyStartLocation.mapID = storageFile.getSignedInt();
	partyStartLocation.x = storageFile.getSignedInt();
	partyStartLocation.y = storageFile.getSignedInt();
	partyStartLocation.layer = storageFile.getSignedInt();
	vehicleStartLocations.clear();
	for (int i = 0, count = storageFile.getSignedInt(); i < count; ++i)
	{
		StartLocation location;
		location.mapID = storageFile.getSignedInt();
		location.x = storageFile.getSignedInt();
		location.y = storageFile.getSignedInt();
		location.layer = storageFile.getSignedInt();
		vehicleStartLocations.append(location);
	}
	for (int i = 0; i < 4; ++i)
		initialParty[i] = storageFile.getSignedInt();
	for (int i = 0; i < 68; ++i)
	{
		temp_string = storageFile.getString();
		strings[i] = temp_string;
		delete temp_string;
	}
	for (int i = 0; i < 4; ++i)
	{
		screentransitions[i].erase = storageFile.getSignedInt();
		screentransitions[i].show = storageFile.getSignedInt();
	}
	for (int i = 0; i < 9; ++i)
	{
		temp_string = storageFile.getString();
		systemSFX[i].location = temp_string;
		delete[] temp_string;
		systemSFX[i].volume = storageFile.getFloat();
		systemSFX[i].panning = storageFile.getFloat();
		systemSFX[i].speed = storageFile.getFloat();
		systemSFX[i].numLoops = storageFile.getSignedInt();
		systemSFX[i].retainPitch = storageFile.getBool();
	}
	for (int i = 0; i < 5; ++i)
	{
		temp_string = storageFile.getString();
		systemBGMs[i].location = temp_string;
		delete[] temp_string;
		systemBGMs[i].volume = storageFile.getFloat();
		systemBGMs[i].panning = storageFile.getFloat();
		systemBGMs[i].speed = storageFile.getFloat();
		systemBGMs[i].fadeInTime = storageFile.getSignedInt();
		systemBGMs[i].retainPitch = storageFile.getBool();
	}
	temp_string = storageFile.getString();
	systemGraphics.frameBGLocation = temp_string;
	delete[] temp_string;
	temp_string = storageFile.getString();
	systemGraphics.frameBorderLocation = temp_string;
	delete[] temp_string;
	temp_string = storageFile.getString();
	systemGraphics.selectionBorderLocation = temp_string;
	delete[] temp_string;
	temp_string = storageFile.getString();
	systemGraphics.equipmentIndicatorsLocation = temp_string;
	delete[] temp_string;
	temp_string = storageFile.getString();
	systemGraphics.menuBGLocation = temp_string;
	delete[] temp_string;
	temp_string = storageFile.getString();
	systemGraphics.scrollArrowsLocation = temp_string;
	delete[] temp_string;
	temp_string = storageFile.getString();
	systemGraphics.grappleArmLocation = temp_string;
	delete[] temp_string;
	temp_string = storageFile.getString();
	systemGraphics.grappleHookLocation = temp_string;
	delete[] temp_string;
	temp_string = storageFile.getString();
	systemGraphics.timerNumbersLocation = temp_string;
	delete[] temp_string;
	temp_string = storageFile.getString();
	systemGraphics.fontShadowLocation = temp_string;
	delete[] temp_string;
	temp_string = storageFile.getString();
	systemGraphics.defaultColor = temp_string;
	delete[] temp_string;
	color = storageFile.getUnsignedLong();
	systemGraphics.fontShadowColor = QColor((int)((color >> 16) % 256), (int)((color >> 8) % 256), (int)(color % 256));
	color = storageFile.getUnsignedLong();
	systemGraphics.defaultFontEnabledTopColor = QColor((int)((color >> 16) % 256), (int)((color >> 8) % 256), (int)(color % 256));
	color = storageFile.getUnsignedLong();
	systemGraphics.defaultFontEnabledBottomColor = QColor((int)((color >> 16) % 256), (int)((color >> 8) % 256), (int)(color % 256));
	color = storageFile.getUnsignedLong();
	systemGraphics.defaultFontDisabledTopColor = QColor((int)((color >> 16) % 256), (int)((color >> 8) % 256), (int)(color % 256));
	color = storageFile.getUnsignedLong();
	systemGraphics.defaultFontDisabledBottomColor = QColor((int)((color >> 16) % 256), (int)((color >> 8) % 256), (int)(color % 256));
	systemGraphics.stretchToFit = storageFile.getBool();
	battleAnimationsReversedFromBehind = storageFile.getBool();
	stringVariableNames.clear();
	numberVariableNames.clear();
	switchVariableNames.clear();
	for (int i = 0, count = storageFile.getSignedInt(); i < count; ++i)
	{
		temp_string = storageFile.getString();
		stringVariableNames.append(temp_string);
		delete[] temp_string;
	}
	for (int i = 0, count = storageFile.getSignedInt(); i < count; ++i)
	{
		temp_string = storageFile.getString();
		numberVariableNames.append(temp_string);
		delete[] temp_string;
	}
	for (int i = 0, count = storageFile.getSignedInt(); i < count; ++i)
	{
		temp_string = storageFile.getString();
		switchVariableNames.append(temp_string);
		delete[] temp_string;
	}
	for (int i = 0; i < battleCommands.size(); ++i)
	{
		if (battleCommands[i] != NULL)
		{
			delete battleCommands[i];
			battleCommands[i] = NULL;
		}
	}
	battleCommands.clear();
	for (int i = 0, count = storageFile.getSignedInt(); i < count; ++i)
	{
		BattleCommand *command = new BattleCommand;
		temp_string = storageFile.getString();
		command->name = temp_string;
		delete[] temp_string;
		command->type = storageFile.getSignedInt();
	}
	for (int i = 0; i < skillSubsets.size(); ++i)
	{
		if (skillSubsets[i] != NULL)
		{
			delete skillSubsets[i];
			skillSubsets[i] = NULL;
		}
	}
	skillSubsets.clear();
	skillSubsets.append(new SkillSubset);
	skillSubsets.append(new SkillSubset);
	skillSubsets.append(new SkillSubset);
	skillSubsets.append(new SkillSubset);
	skillSubsets[0]->name = "Normal";
	skillSubsets[0]->type = SKILLTYPE_NORMAL;
	skillSubsets[1]->name = "Teleport";
	skillSubsets[1]->type = SKILLTYPE_TELEPORT;
	skillSubsets[2]->name = "Escape";
	skillSubsets[2]->type = SKILLTYPE_ESCAPE;
	skillSubsets[3]->name = "Switch";
	skillSubsets[3]->type = SKILLTYPE_SWITCH;
	for (int i = 0, count = storageFile.getSignedInt(); i < count; ++i)
	{
		SkillSubset *subset = new SkillSubset;
		temp_string = storageFile.getString();
		subset->name = temp_string;
		delete[] temp_string;
		subset->type = storageFile.getSignedInt();
		skillSubsets.append(subset);
	}
	for (int i = 0; i < NUM_PHASES; ++i)
	{
		temp_string = storageFile.getString();
		phaseNames[i] = temp_string;
		delete[] temp_string;
	}
	projectLocation = projectsdir + QString("/") + directoryName;
	battleAnimationsReversedFromBehind = storageFile.getBool();
	fullscreen = storageFile.getBool();
	showtitle = storageFile.getBool();
	storageFile.closeFile();
	for (mainMenuLayoutsIter = mainMenuLayouts.begin(); mainMenuLayoutsIter != mainMenuLayouts.end(); ++mainMenuLayoutsIter)
		mainMenuLayoutsIter.value()->reloadData();
	for (mapsIter = maps.begin(); mapsIter != maps.end(); ++mapsIter)
		mapsIter.value()->reloadData();
	for (popupMenuLayoutsIter = popupMenuLayouts.begin(); popupMenuLayoutsIter != popupMenuLayouts.end(); ++popupMenuLayoutsIter)
		popupMenuLayoutsIter.value()->reloadData();
	for (int i = 0; i < attributes.size(); ++i)
		attributes[i]->reloadData();
	for (int i = 0; i < battleAnimations.size(); ++i)
		battleAnimations[i]->reloadData();
	for (int i = 0; i < characters.size(); ++i)
		characters[i]->reloadData();
	for (int i = 0; i < characterSprites.size(); ++i)
		characterSprites[i]->reloadData();
	for (int i = 0; i < classes.size(); ++i)
		classes[i]->reloadData();
	for (int i = 0; i < commonEvents.size(); ++i)
		commonEvents[i]->reloadData();
	for (int i = 0; i < conditions.size(); ++i)
		conditions[i]->reloadData();
	for (int i = 0; i < fonts.size(); ++i)
		fonts[i]->reloadData();
	for (int i = 0; i < globalAnimations.size(); ++i)
		globalAnimations[i]->reloadData();
	for (int i = 0; i < items.size(); ++i)
		items[i]->reloadData();
	for (int i = 0; i < monsterAnimations.size(); ++i)
		monsterAnimations[i]->reloadData();
	for (int i = 0; i < monsters.size(); ++i)
		monsters[i]->reloadData();
	for (int i = 0; i < monsterGroups.size(); ++i)
		monsterGroups[i]->reloadData();
	for (int i = 0; i < skills.size(); ++i)
		skills[i]->reloadData();
	for (int i = 0; i < terrain.size(); ++i)
		terrain[i]->reloadData();
	for (int i = 0; i < tilesets.size(); ++i)
		tilesets[i]->reloadData();
	for (int i = 0; i < vehicles.size(); ++i)
		vehicles[i]->reloadData();
	TODO("load battle layout from storage.");
	loadTitleMenuLayoutFromStorage();
}

void ProjectData::loadImportedData()
{
	QString fileLocation = homePath + "/imports.xml";
	QString tempString;
	XMLNode TempNode;
	ImporterNode = XMLNode::openFileHelper(fileLocation.toLocal8Bit().data(), "openrpgmaker-imports");
	for (int i = 0; i < ImporterNode.nChildNode(); ++i)
	{
		TempNode = ImporterNode.getChildNode(i);
		tempString = TempNode.getName();
		if (tempString == "attribute")
		{
			ReferenceCounter<Attribute> *refCounter = new ReferenceCounter<Attribute>(attributes.size());
			attributes.append(refCounter);
			attributeNames.append(TempNode.getAttribute("name"));
		}
		else if (tempString == "battleAnimation")
		{
			ReferenceCounter<BattleAnimation> *refCounter = new ReferenceCounter<BattleAnimation>(battleAnimations.size());
			battleAnimations.append(refCounter);
			battleAnimationNames.append(TempNode.getAttribute("name"));
		}
		else if (tempString == "character")
		{
			ReferenceCounter<Character> *refCounter = new ReferenceCounter<Character>(characters.size());
			characters.append(refCounter);
			characterNames.append(TempNode.getAttribute("name"));
		}
		else if (tempString == "characterSprite")
		{
			ReferenceCounter<CharacterSprite> *refCounter = new ReferenceCounter<CharacterSprite>(characterSprites.size());
			characterSprites.append(refCounter);
			characterSpriteNames.append(TempNode.getAttribute("name"));
		}
		else if (tempString == "classe")
		{
			ReferenceCounter<Class> *refCounter = new ReferenceCounter<Class>(classes.size());
			classes.append(refCounter);
			classNames.append(TempNode.getAttribute("name"));
		}
		else if (tempString == "commonEvent")
		{
			ReferenceCounter<CommonEvent> *refCounter = new ReferenceCounter<CommonEvent>(commonEvents.size());
			commonEvents.append(refCounter);
			commonEventNames.append(TempNode.getAttribute("name"));
		}
		else if (tempString == "condition")
		{
			ReferenceCounter<Condition> *refCounter = new ReferenceCounter<Condition>(conditions.size());
			conditions.append(refCounter);
			conditionNames.append(TempNode.getAttribute("name"));
		}
		else if (tempString == "font")
		{
			ReferenceCounter<Font> *refCounter = new ReferenceCounter<Font>(fonts.size());
			fonts.append(refCounter);
			fontNames.append(TempNode.getAttribute("name"));
			refCounter->incrementReferenceCount();
		}
		else if (tempString == "globalAnimation")
		{
			ReferenceCounter<GlobalAnimation> *refCounter = new ReferenceCounter<GlobalAnimation>(globalAnimations.size());
			globalAnimations.append(refCounter);
			globalAnimationNames.append(TempNode.getAttribute("name"));
		}
		else if (tempString == "item")
		{
			ReferenceCounter<Item> *refCounter = new ReferenceCounter<Item>(items.size());
			items.append(refCounter);
			itemNames.append(TempNode.getAttribute("name"));
		}
		else if (tempString == "mainMenuLayout")
		{
			int menuID = QString(TempNode.getAttribute("id")).toInt();
			ReferenceCounter<MainMenuLayout> *refCounter = new ReferenceCounter<MainMenuLayout>(menuID);
			mainMenuLayouts.insert(menuID, refCounter);
			mainMenuLayoutNames.insert(menuID, TempNode.getAttribute("name"));
		}
		else if (tempString == "map")
		{
			int mapID = QString(TempNode.getAttribute("id")).toInt();
			ReferenceCounter<Map> *refCounter = new ReferenceCounter<Map>(mapID);
			maps.insert(mapID, refCounter);
			mapNames.insert(mapID, TempNode.getAttribute("name"));
		}
		else if (tempString == "monsterAnimation")
		{
			ReferenceCounter<MonsterAnimation> *refCounter = new ReferenceCounter<MonsterAnimation>(monsterAnimations.size());
			monsterAnimations.append(refCounter);
			monsterAnimationNames.append(TempNode.getAttribute("name"));
		}
		else if (tempString == "monster")
		{
			ReferenceCounter<Monster> *refCounter = new ReferenceCounter<Monster>(monsters.size());
			monsters.append(refCounter);
			monsterNames.append(TempNode.getAttribute("name"));
		}
		else if (tempString == "monsterGroup")
		{
			ReferenceCounter<MonsterGroup> *refCounter = new ReferenceCounter<MonsterGroup>(monsterGroups.size());
			monsterGroups.append(refCounter);
			monsterGroupNames.append(TempNode.getAttribute("name"));
		}
		else if (tempString == "popupMenuLayout")
		{
			int menuID = QString(TempNode.getAttribute("id")).toInt();
			ReferenceCounter<PopupMenuLayout> *refCounter = new ReferenceCounter<PopupMenuLayout>(menuID);
			popupMenuLayouts.insert(menuID, refCounter);
			popupMenuLayoutNames.insert(menuID, TempNode.getAttribute("name"));
		}
		else if (tempString == "skill")
		{
			ReferenceCounter<Skill> *refCounter = new ReferenceCounter<Skill>(skills.size());
			skills.append(refCounter);
			skillNames.append(TempNode.getAttribute("name"));
		}
		else if (tempString == "terrain")
		{
			ReferenceCounter<Terrain> *refCounter = new ReferenceCounter<Terrain>(terrain.size());
			terrain.append(refCounter);
			terrainNames.append(TempNode.getAttribute("name"));
		}
		else if (tempString == "tileset")
		{
			ReferenceCounter<Tileset> *refCounter = new ReferenceCounter<Tileset>(tilesets.size());
			tilesets.append(refCounter);
			tilesetNames.append(TempNode.getAttribute("name"));
			refCounter->incrementReferenceCount();
		}
		else if (tempString == "vehicle")
		{
			ReferenceCounter<Vehicle> *refCounter = new ReferenceCounter<Vehicle>(vehicles.size());
			vehicles.append(refCounter);
			vehicleNames.append(TempNode.getAttribute("name"));
		}
	}
	QDir(homePath).remove("imports.xml");
	saved = false;
}

void ProjectData::saveImportedData()
{
	QString fileLocation = homePath + "/imports.xml";
	ImporterNode.writeToFile(fileLocation.toLocal8Bit().data());
}

void ProjectData::saveToStorage()
{
	StorageFile storageFile;
	unsigned long color;
	storageFile.openFile((homePath + "/storage/projectdata.storage").toLocal8Bit().data(), StorageFile::MODE_WRITE);
	storageFile.putString(gameName.toUtf8().data());
	storageFile.putString(directoryName.toUtf8().data());
	storageFile.putString(titleLocation.toUtf8().data());
	storageFile.putString(gameoverLocation.toUtf8().data());
	storageFile.putString(projectLocation.toUtf8().data());
	storageFile.putString(datadir.toUtf8().data());
	storageFile.putString(projectsdir.toUtf8().data());
	storageFile.putSignedInt(mainMenuID);
	storageFile.putSignedInt(partyStartLocation.mapID);
	storageFile.putSignedInt(partyStartLocation.x);
	storageFile.putSignedInt(partyStartLocation.y);
	storageFile.putSignedInt(partyStartLocation.layer);
	storageFile.putSignedInt(vehicleStartLocations.size());
	for (int i = 0; i < vehicleStartLocations.size(); ++i)
	{
		storageFile.putSignedInt(vehicleStartLocations[i].mapID);
		storageFile.putSignedInt(vehicleStartLocations[i].x);
		storageFile.putSignedInt(vehicleStartLocations[i].y);
		storageFile.putSignedInt(vehicleStartLocations[i].layer);
	}
	for (int i = 0; i < 4; ++i)
		storageFile.putSignedInt(initialParty[i]);
	for (int i = 0; i < 68; ++i)
		storageFile.putString(strings[i].toUtf8().data());
	for (int i = 0; i < 4; ++i)
	{
		storageFile.putSignedInt(screentransitions[i].erase);
		storageFile.putSignedInt(screentransitions[i].show);
	}
	for (int i = 0; i < 9; ++i)
	{
		storageFile.putString(systemSFX[i].location.toUtf8().data());
		storageFile.putFloat(systemSFX[i].volume);
		storageFile.putFloat(systemSFX[i].panning);
		storageFile.putFloat(systemSFX[i].speed);
		storageFile.putSignedInt(systemSFX[i].numLoops);
		storageFile.putBool(systemSFX[i].retainPitch);
	}
	for (int i = 0; i < 5; ++i)
	{
		storageFile.putString(systemBGMs[i].location.toUtf8().data());
		storageFile.putFloat(systemBGMs[i].volume);
		storageFile.putFloat(systemBGMs[i].panning);
		storageFile.putFloat(systemBGMs[i].speed);
		storageFile.putSignedInt(systemBGMs[i].fadeInTime);
		storageFile.putBool(systemBGMs[i].retainPitch);
	}
	storageFile.putString(systemGraphics.frameBGLocation.toUtf8().data());
	storageFile.putString(systemGraphics.frameBorderLocation.toUtf8().data());
	storageFile.putString(systemGraphics.selectionBorderLocation.toUtf8().data());
	storageFile.putString(systemGraphics.equipmentIndicatorsLocation.toUtf8().data());
	storageFile.putString(systemGraphics.menuBGLocation.toUtf8().data());
	storageFile.putString(systemGraphics.scrollArrowsLocation.toUtf8().data());
	storageFile.putString(systemGraphics.grappleArmLocation.toUtf8().data());
	storageFile.putString(systemGraphics.grappleHookLocation.toUtf8().data());
	storageFile.putString(systemGraphics.timerNumbersLocation.toUtf8().data());
	storageFile.putString(systemGraphics.fontShadowLocation.toUtf8().data());
	storageFile.putString(systemGraphics.defaultColor.toUtf8().data());
	color = (((unsigned long)systemGraphics.fontShadowColor.red()) << 16) + (((unsigned long)systemGraphics.fontShadowColor.green()) << 8) + (unsigned long)systemGraphics.fontShadowColor.blue();
	storageFile.putUnsignedLong(color);
	color = (((unsigned long)systemGraphics.defaultFontEnabledTopColor.red()) << 16) + (((unsigned long)systemGraphics.defaultFontEnabledTopColor.green()) << 8) + (unsigned long)systemGraphics.defaultFontEnabledTopColor.blue();
	storageFile.putUnsignedLong(color);
	color = (((unsigned long)systemGraphics.defaultFontEnabledBottomColor.red()) << 16) + (((unsigned long)systemGraphics.defaultFontEnabledBottomColor.green()) << 8) + (unsigned long)systemGraphics.defaultFontEnabledBottomColor.blue();
	storageFile.putUnsignedLong(color);
	color = (((unsigned long)systemGraphics.defaultFontDisabledTopColor.red()) << 16) + (((unsigned long)systemGraphics.defaultFontDisabledTopColor.green()) << 8) + (unsigned long)systemGraphics.defaultFontDisabledTopColor.blue();
	storageFile.putUnsignedLong(color);
	color = (((unsigned long)systemGraphics.defaultFontDisabledBottomColor.red()) << 16) + (((unsigned long)systemGraphics.defaultFontDisabledBottomColor.green()) << 8) + (unsigned long)systemGraphics.defaultFontDisabledBottomColor.blue();
	storageFile.putUnsignedLong(color);
	storageFile.putBool(systemGraphics.stretchToFit);
	storageFile.putBool(battleAnimationsReversedFromBehind);
	storageFile.putSignedInt(stringVariableNames.size());
	for (int i = 0; i < stringVariableNames.size(); ++i)
		storageFile.putString(stringVariableNames[i].toUtf8().data());
	storageFile.putSignedInt(numberVariableNames.size());
	for (int i = 0; i < numberVariableNames.size(); ++i)
		storageFile.putString(numberVariableNames[i].toUtf8().data());
	storageFile.putSignedInt(switchVariableNames.size());
	for (int i = 0; i < switchVariableNames.size(); ++i)
		storageFile.putString(switchVariableNames[i].toUtf8().data());
	storageFile.putSignedInt(battleCommands.size());
	for (int i = 0; i < battleCommands.size(); ++i)
	{
		storageFile.putString(battleCommands[i]->name.toUtf8().data());
		storageFile.putSignedInt(battleCommands[i]->type);
	}
	storageFile.putSignedInt(skillSubsets.size() - 4);
	for (int i = 4; i < skillSubsets.size(); ++i)
	{
		storageFile.putString(skillSubsets[i]->name.toUtf8().data());
		storageFile.putSignedInt(skillSubsets[i]->type);
	}
	for (int i = 0; i < NUM_PHASES; ++i)
		storageFile.putString(phaseNames[i].toUtf8().data());
	storageFile.putBool(battleAnimationsReversedFromBehind);
	storageFile.putBool(fullscreen);
	storageFile.putBool(showtitle);
	storageFile.closeFile();
	TODO("save battle layout to storage.");
	saveTitleMenuLayoutToStorage();
}

#ifdef COMPILING_EDITOR

void ProjectData::changeName(QString name)
{
	gameName = name;
	mapItems->setText(0, name);
	saved = false;
}

int ProjectData::addMap(QTreeWidgetItem *parent, XMLNode MapNode)
{
	QTreeWidgetItem *item = new QTreeWidgetItem;
	DataReference<Map> *mapRef;
	int nummaps, mapID = QString(MapNode.getAttribute("id")).toInt(NULL, 16);
	mapRef = new DataReference<Map>(mapID, __FILE__, __LINE__);
	item->setText(0, QString(MapNode.getAttribute("name")));
	item->setText(1, QString(MapNode.getAttribute("id")));
	item->setText(2, QString(MapNode.getAttribute("expanded")).toLower());
	item->setText(3, "map");
	item->setIcon(0, QIcon(":/icons/folder_green.png"));
	parent->addChild(item);
	if (item->text(1).toInt(NULL, 16) == selectedMapID)
		mainwindow->twMapList->setCurrentItem(item);
	nummaps = MapNode.nChildNode("map");
	QCoreApplication::processEvents();
	for (int i = 0; i < nummaps; ++i)
		addMap(item, MapNode.getChildNode("map", i));
	QCoreApplication::processEvents();
	for (int i = 0; i < (*mapRef)->numAreas(); ++i)
		addMapArea(item, i);
	QCoreApplication::processEvents();
	saved = false;
	item->setExpanded(item->text(2) == "true");
	delete mapRef;
	return mapID;
}

int ProjectData::addMap(QTreeWidgetItem *parent, Map *map)
{
	QTreeWidgetItem *mapItem = new QTreeWidgetItem;
	int mapID = addMap(map);
	mapItem->setText(0, map->getName());
	mapItem->setText(1, QString("%1").arg(mapID, 0, 16));
	mapItem->setText(2, QString("false"));
	mapItem->setText(3, "map");
	mapItem->setIcon(0, QIcon(":/icons/folder_green.png"));
	mapItem->setExpanded(false);
	parent->addChild(mapItem);
	selectedMapID = mapItem->text(1).toInt();
	mainwindow->twMapList->setCurrentItem(mapItem);
	saved = false;
	return mapID;
}

int ProjectData::addMap(QTreeWidgetItem *parent)
{
	Map *map = new Map;
	QTreeWidgetItem *mapItem = new QTreeWidgetItem;
	int mapID = addMap(map);
	map->setName(QString("Map%1").arg(mapID, 4, 16, QLatin1Char('0')));
	mapNames[mapID] = map->getName();
	mapItem->setText(0, QString("Map%1").arg(mapID, 4, 16, QLatin1Char('0')));
	mapItem->setText(1, QString("%1").arg(mapID, 0, 16));
	mapItem->setText(2, QString("false"));
	mapItem->setText(3, "map");
	mapItem->setIcon(0, QIcon(":/icons/folder_green.png"));
	mapItem->setExpanded(false);
	parent->addChild(mapItem);
	selectedMapID = mapItem->text(1).toInt();
	mainwindow->twMapList->setCurrentItem(mapItem);
	saved = false;
	delete map;
	return mapID;
}

void ProjectData::buildMapTree(XMLNode ParentNode)
{
	QMap<int, ReferenceCounter<Map>*>::iterator iter;
	int mapID = QString(ParentNode.getAttribute("id")).toInt(NULL, 16);
	for (iter = maps.begin(); iter != maps.end(); ++iter)
	{
		DataReference<Map> mapRef(iter.key(), __FILE__, __LINE__);
		if (mapRef->getParent() == mapID)
		{
			XMLNode MapNode = ParentNode.addChild("map");
			MapNode.addAttribute("id", QString::number(iter.key(), 16).toUtf8().data());
			MapNode.addAttribute("name", mapRef->getName().toUtf8().data());
			MapNode.addAttribute("expanded", "false");
			buildMapTree(MapNode);
		}
	}
}

void ProjectData::addMapArea(QTreeWidgetItem *parent, int areaID)
{
	QTreeWidgetItem *item = new QTreeWidgetItem;
	item->setText(0, QString("Area %1").arg(areaID, 2, 16, QChar('0')));
	item->setText(1, QString("%1").arg(areaID));
	item->setText(2, QString("false"));
	item->setText(3, "area");
	item->setIcon(0, QIcon(":/icons/area.png"));
	parent->addChild(item);
	saved = false;
}

void ProjectData::deleteMap(QTreeWidgetItem *mapItem)
{
	QTreeWidgetItem *parentItem = mapItem->parent();
	int mapID = mapItem->text(1).toInt(NULL, 16);
	parentItem->removeChild(mapItem);
	deleteMap(mapID);
	mainwindow->twMapList->setCurrentItem(parentItem);
	saved = false;
}

void ProjectData::deleteMapArea(QTreeWidgetItem *areaItem)
{
	QTreeWidgetItem *parentItem = areaItem->parent();
	DataReference<Map> *mapRef;
	int areaID = areaItem->text(1).toInt();
	int mapID = parentItem->text(1).toInt(NULL, 16);
	mapRef = new DataReference<Map>(mapID, __FILE__, __LINE__);
	(*mapRef)->deleteArea(areaID);
	delete mapRef;
	parentItem->removeChild(areaItem);
	mainwindow->twMapList->setCurrentItem(parentItem);
	saved = false;
}

void ProjectData::changeCurrentMap(int mapID)
{
	selectedMapID = mapID;
}

void ProjectData::changeCurrentTileset(int tilesetID)
{
	selectedTilesetID = tilesetID;
}

int ProjectData::getSelectedMapID()
{
	return selectedMapID;
}

int ProjectData::getSelectedTilesetID()
{
	return selectedTilesetID;
}

XMLNode ProjectData::getProjectNode()
{
	QTreeWidgetItem *topItem;
	XMLNode ProjectNode = XMLNode::createXMLTopNode("projectdata");
	XMLNode TempNode;
	ProjectNode.addAttribute("fileFormatVersion", QString::number(FILE_FORMAT_VERSION).toUtf8().data());
	ProjectNode.addAttribute("name", gameName.toUtf8().data());
	ProjectNode.addAttribute("fullscreen", (mainwindow->aGameOptions_FS->isChecked()) ? "true":"false");
	ProjectNode.addAttribute("displaytitle", (mainwindow->aGameOptions_DisplayTitle->isChecked()) ? "true":"false");
	TempNode = ProjectNode.addChild("editor");
	if (mainwindow->toolMakeSelection->isChecked())
		TempNode.addAttribute("tool", "Select");
	else if (mainwindow->toolDrawRectangle->isChecked())
		TempNode.addAttribute("tool", "Rectangle");
	else if (mainwindow->toolDrawCircle->isChecked())
		TempNode.addAttribute("tool", "Circle");
	else if (mainwindow->toolBucketFill->isChecked())
		TempNode.addAttribute("tool", "Fill");
	else if (mainwindow->toolPencil->isChecked())
		TempNode.addAttribute("tool", "Pencil");
	else if (mainwindow->toolTiling->isChecked())
		TempNode.addAttribute("tool", "Tiling");
	if (mainwindow->aEditLayers->isChecked())
		TempNode.addAttribute("editMode", "Layers");
	else if (mainwindow->aEditPassability->isChecked())
		TempNode.addAttribute("editMode", "Passability");
	else if (mainwindow->aEdit_EventLayer->isChecked())
		TempNode.addAttribute("editMode", "Events");
	if (mainwindow->aZoom_1to1->isChecked())
		TempNode.addAttribute("zoom", "1:1");
	else if (mainwindow->aZoom_1to2->isChecked())
		TempNode.addAttribute("zoom", "1:2");
	else if (mainwindow->aZoom_1to4->isChecked())
		TempNode.addAttribute("zoom", "1:4");
	else if (mainwindow->aZoom_1to8->isChecked())
		TempNode.addAttribute("zoom", "1:8");
	TempNode.addAttribute("tileset", QString::number(selectedTilesetID).toUtf8().data());
	TempNode = ProjectNode.addChild("maplist");
	TempNode.addAttribute("selectedmap", QString::number(selectedMapID).toUtf8().data());
	topItem = mainwindow->twMapList->topLevelItem(0);
	for (int i = 0, childCount = topItem->childCount(); i < childCount; ++i)
		TempNode.addChild(getMapNode(topItem->child(i)));
	return ProjectNode;
}

XMLNode ProjectData::getMapNode(QTreeWidgetItem *item)
{
	XMLNode MapNode = XMLNode::createXMLTopNode("map");
	MapNode.addAttribute("id", item->text(1).toUtf8().data());
	MapNode.addAttribute("name", item->text(0).toUtf8().data());
	MapNode.addAttribute("expanded", item->text(2).toUtf8().data());
	for (int i = 0, childCount = item->childCount(); i < childCount; ++i)
	{
		QTreeWidgetItem *child = item->child(i);
		if (item->text(3) == "map")
			MapNode.addChild(getMapNode(child));
	}
	return MapNode;
}

void ProjectData::getSystemSFXInfo(int sfxID, AudioManager::SoundEffectInfo *soundInfo)
{
	soundInfo->location = systemSFX[sfxID].location;
	soundInfo->volume = systemSFX[sfxID].volume;
	soundInfo->panning = systemSFX[sfxID].panning;
	soundInfo->speed = systemSFX[sfxID].speed;
	soundInfo->numLoops = systemSFX[sfxID].numLoops;
	soundInfo->retainPitch = systemSFX[sfxID].retainPitch;
}

void ProjectData::setSystemSFXInfo(int sfxID, AudioManager::SoundEffectInfo soundInfo)
{
	systemSFX[sfxID].location = soundInfo.location;
	systemSFX[sfxID].volume = soundInfo.volume;
	systemSFX[sfxID].panning = soundInfo.panning;
	systemSFX[sfxID].speed = soundInfo.speed;
	systemSFX[sfxID].numLoops = soundInfo.numLoops;
	systemSFX[sfxID].retainPitch = soundInfo.retainPitch;
}

void ProjectData::getSystemBGMInfo(int bgmID, AudioManager::MusicInfo *musicInfo)
{
	musicInfo->location = systemBGMs[bgmID].location;
	musicInfo->volume = systemBGMs[bgmID].volume;
	musicInfo->panning = systemBGMs[bgmID].panning;
	musicInfo->speed = systemBGMs[bgmID].speed;
	musicInfo->fadeInTime = systemBGMs[bgmID].fadeInTime;
	musicInfo->retainPitch = systemBGMs[bgmID].retainPitch;
}

void ProjectData::setSystemBGMInfo(int bgmID, AudioManager::MusicInfo musicInfo)
{
	systemBGMs[bgmID].location = musicInfo.location;
	systemBGMs[bgmID].volume = musicInfo.volume;
	systemBGMs[bgmID].panning = musicInfo.panning;
	systemBGMs[bgmID].speed = musicInfo.speed;
	systemBGMs[bgmID].fadeInTime = musicInfo.fadeInTime;
	systemBGMs[bgmID].retainPitch = musicInfo.retainPitch;
}

void ProjectData::backupStorage()
{
	QFileInfoList fileList = QDir(homePath + "/storage").entryInfoList(QDir::Files|QDir::NoDotAndDotDot);
	for (int i = 0, count = fileList.count(); i < count; ++i)
	{
		if (fileList[i].suffix() == "storage")
		{
			FILE *storagefile = fopen(fileList[i].filePath().toUtf8().data(), "rb");
			FILE *backupfile = fopen(QString(fileList[i].filePath() + ".bak").toUtf8().data(), "wb");
			size_t result;
			char buffer[512];
			while (!feof(storagefile))
			{
				result = fread(buffer, 1, sizeof(buffer), storagefile);
				fwrite(buffer, 1, result, backupfile);
			}
			fclose(storagefile);
			fclose(backupfile);
		}
	}
}

void ProjectData::loadBackupStorage()
{
	QFileInfoList fileList = QDir(homePath + "/storage").entryInfoList(QStringList("*.bak"));
	for (int i = 0, count = fileList.count(); i < count; ++i)
	{
		FILE *storagefile;
		FILE *backupfile;
		size_t result;
		char buffer[512];
		sprintf(buffer, "%s", fileList[i].filePath().toUtf8().data());
		buffer[strlen(buffer) - 4] = '\0';
		storagefile = fopen(buffer, "wb");
		backupfile = fopen(fileList[i].filePath().toUtf8().data(), "rb");
		while (!feof(backupfile))
		{
			result = fread(buffer, 1, sizeof(buffer), backupfile);
			fwrite(buffer, 1, result, storagefile);
		}
		fclose(storagefile);
		fclose(backupfile);
	}
	for (mainMenuLayoutsIter = mainMenuLayouts.begin(); mainMenuLayoutsIter != mainMenuLayouts.end(); ++mainMenuLayoutsIter)
		mainMenuLayoutsIter.value()->reloadData();
	for (mapsIter = maps.begin(); mapsIter != maps.end(); ++mapsIter)
		mapsIter.value()->reloadData();
	for (popupMenuLayoutsIter = popupMenuLayouts.begin(); popupMenuLayoutsIter != popupMenuLayouts.end(); ++popupMenuLayoutsIter)
		popupMenuLayoutsIter.value()->reloadData();
	for (int i = 0; i < attributes.size(); ++i)
		attributes[i]->reloadData();
	for (int i = 0; i < battleAnimations.size(); ++i)
		battleAnimations[i]->reloadData();
	for (int i = 0; i < characters.size(); ++i)
		characters[i]->reloadData();
	for (int i = 0; i < characterSprites.size(); ++i)
		characterSprites[i]->reloadData();
	for (int i = 0; i < classes.size(); ++i)
		classes[i]->reloadData();
	for (int i = 0; i < commonEvents.size(); ++i)
		commonEvents[i]->reloadData();
	for (int i = 0; i < conditions.size(); ++i)
		conditions[i]->reloadData();
	for (int i = 0; i < fonts.size(); ++i)
		fonts[i]->reloadData();
	for (int i = 0; i < globalAnimations.size(); ++i)
		globalAnimations[i]->reloadData();
	for (int i = 0; i < items.size(); ++i)
		items[i]->reloadData();
	for (int i = 0; i < monsterAnimations.size(); ++i)
		monsterAnimations[i]->reloadData();
	for (int i = 0; i < monsters.size(); ++i)
		monsters[i]->reloadData();
	for (int i = 0; i < monsterGroups.size(); ++i)
		monsterGroups[i]->reloadData();
	for (int i = 0; i < skills.size(); ++i)
		skills[i]->reloadData();
	loadTitleMenuLayoutFromStorage();
	for (int i = 0; i < terrain.size(); ++i)
		terrain[i]->reloadData();
	for (int i = 0; i < tilesets.size(); ++i)
		tilesets[i]->reloadData();
	for (int i = 0; i < vehicles.size(); ++i)
		vehicles[i]->reloadData();
}

void ProjectData::cleanBackupStorage()
{
	QDir dir(homePath + "/storage");
	QStringList fileList = dir.entryList(QStringList("*.bak"));
	for (int i = 0, count = fileList.count(); i < count; ++i)
		dir.remove(fileList[i]);
}

void ProjectData::saveProject()
{
	if (open)
	{
		XMLNode ProjectNode = getProjectNode();
		char temp_string[128];
		sprintf(temp_string, "openrpgmaker-%s", ProjectNode.getName());
		ProjectNode.updateName(temp_string);
		ProjectNode.writeToFile(QString(projectLocation + "/projectdata.xml").toLocal8Bit().data());
		saveSystemData(projectLocation + QString("/system.xml"));
		saved = true;
	}
}

void ProjectData::saveConfiguration()
{
	XMLNode ConfigNode = XMLNode::createXMLTopNode("openrpgmaker-config", TRUE);
	QString tempString;
	QList<int> sizes;
	ConfigNode.addAttribute("windowX", QString::number(mainwindow->x()).toUtf8().data());
	ConfigNode.addAttribute("windowY", QString::number(mainwindow->y()).toUtf8().data());
	ConfigNode.addAttribute("windowWidth", QString::number(mainwindow->width()).toUtf8().data());
	ConfigNode.addAttribute("windowHeight", QString::number(mainwindow->height()).toUtf8().data());
	tempString = "";
	sizes = mainwindow->mapSplitter->sizes();
	for (int i = 0; i < sizes.size(); ++i)
	{
		if (i != 0)
			tempString += ";";
		tempString += QString::number(sizes[i]);
	}
	ConfigNode.addAttribute("mapSplitterSizes", tempString.toUtf8().data());
	tempString = "";
	sizes = mainwindow->layersSplitter->sizes();
	for (int i = 0; i < sizes.size(); ++i)
	{
		if (i != 0)
			tempString += ";";
		tempString += QString::number(sizes[i]);
	}
	ConfigNode.addAttribute("layersSplitterSizes", tempString.toUtf8().data());
	ConfigNode.addAttribute("projectsdir", projectsdir.toUtf8().data());
	if (open)
		ConfigNode.addAttribute("projectLocation", directoryName.toUtf8().data());
	else
		ConfigNode.addAttribute("projectLocation", "");
	ConfigNode.writeToFile(QString(homePath + "/config.xml").toLocal8Bit().data());
}

#endif // COMPILING_EDITOR

int ProjectData::numberVariableIdFromString(QString string)
{
	return string.left(4).toInt() - 1;
}

int ProjectData::stringVariableIdFromString(QString string)
{
	return string.left(4).toInt() - 1;
}

int ProjectData::switchVariableIdFromString(QString string)
{
	return string.left(4).toInt() - 1;
}

QString ProjectData::numberVariableName(int varID)
{
	return numberVariableNames[varID];
}

QString ProjectData::stringVariableName(int stringID)
{
	return stringVariableNames[stringID];
}

QString ProjectData::switchVariableName(int switchID)
{
	return switchVariableNames[switchID];
}

void ProjectData::setNumberVariableName(int varID, QString string)
{
	numberVariableNames[varID] = string;
}

void ProjectData::setStringVariableName(int stringID, QString string)
{
	stringVariableNames[stringID] = string;
}

void ProjectData::setSwitchVariableName(int switchID, QString string)
{
	switchVariableNames[switchID] = string;
}

QString ProjectData::numberVariableIdToString(int varID, bool includeName)
{
	if (includeName)
		return QString("%1: %2").arg(varID + 1, 4, 10, QChar('0')).arg(numberVariableNames[varID]);
	return QString("%1").arg(varID + 1);
}

QString ProjectData::stringVariableIdToString(int stringID, bool includeName)
{
	if (includeName)
		return QString("%1: %2").arg(stringID + 1, 4, 10, QChar('0')).arg(stringVariableNames[stringID]);
	return QString("%1").arg(stringID + 1);
}

QString ProjectData::switchVariableIdToString(int switchID, bool includeName)
{
	if (includeName)
		return QString("%1: %2").arg(switchID + 1, 4, 10, QChar('0')).arg(switchVariableNames[switchID]);
	return QString("%1").arg(switchID + 1);
}

int ProjectData::numNumberVariables()
{
	return numberVariableNames.size();
}

int ProjectData::numStringVariables()
{
	return stringVariableNames.size();
}

int ProjectData::numSwitchVariables()
{
	return switchVariableNames.size();
}

void ProjectData::resizeNumberVariables(int newSize)
{
	if (newSize > numberVariableNames.size())
	{
		for (int i = numberVariableNames.size(); i < newSize; ++i)
			numberVariableNames.append("");
	}
	else if (newSize < numberVariableNames.size())
	{
		while (numberVariableNames.size() > newSize)
			numberVariableNames.removeLast();
	}
}

void ProjectData::resizeStringVariables(int newSize)
{
	if (newSize > stringVariableNames.size())
	{
		for (int i = stringVariableNames.size(); i < newSize; ++i)
			stringVariableNames.append("");
	}
	else if (newSize < stringVariableNames.size())
	{
		while (stringVariableNames.size() > newSize)
			stringVariableNames.removeLast();
	}
}

void ProjectData::resizeSwitchVariables(int newSize)
{
	if (newSize > switchVariableNames.size())
	{
		for (int i = switchVariableNames.size(); i < newSize; ++i)
			switchVariableNames.append("");
	}
	else if (newSize < switchVariableNames.size())
	{
		while (switchVariableNames.size() > newSize)
			switchVariableNames.removeLast();
	}
}

QString ProjectData::getAbsoluteResourcePath(QString relativePath)
{
	QString absolutePath = relativePath;
	absolutePath.replace("local::", projectLocation);
	absolutePath.replace("global::", datadir);
	return absolutePath;
}

QString ProjectData::getRelativeResourcePath(QString absolutePath, bool changeGlobalToLocal)
{
	QString relativePath = absolutePath;
	relativePath.replace(projectLocation, "local::");
	if (changeGlobalToLocal)
		relativePath.replace(datadir, "local::");
	else
		relativePath.replace(datadir, "global::");
	return relativePath;
}

#ifdef COMPILING_EDITOR

void ProjectData::saveAllDataReferencesToStorage()
{
	for (int i = 0; i < attributes.size(); ++i)
		attributes[i]->saveData();
	for (int i = 0; i < battleAnimations.size(); ++i)
		battleAnimations[i]->saveData();
	for (int i = 0; i < characters.size(); ++i)
		characters[i]->saveData();
	for (int i = 0; i < characterSprites.size(); ++i)
		characterSprites[i]->saveData();
	for (int i = 0; i < classes.size(); ++i)
		classes[i]->saveData();
	for (int i = 0; i < commonEvents.size(); ++i)
		commonEvents[i]->saveData();
	for (int i = 0; i < conditions.size(); ++i)
		conditions[i]->saveData();
	for (int i = 0; i < fonts.size(); ++i)
		fonts[i]->saveData();
	for (int i = 0; i < globalAnimations.size(); ++i)
		globalAnimations[i]->saveData();
	for (int i = 0; i < items.size(); ++i)
		items[i]->saveData();
	for (mainMenuLayoutsIter = mainMenuLayouts.begin(); mainMenuLayoutsIter != mainMenuLayouts.end(); ++mainMenuLayoutsIter)
		mainMenuLayoutsIter.value()->saveData();
	for (mapsIter = maps.begin(); mapsIter != maps.end(); ++mapsIter)
		mapsIter.value()->saveData();
	for (int i = 0; i < monsterAnimations.size(); ++i)
		monsterAnimations[i]->saveData();
	for (int i = 0; i < monsters.size(); ++i)
		monsters[i]->saveData();
	for (int i = 0; i < monsterGroups.size(); ++i)
		monsterGroups[i]->saveData();
	for (popupMenuLayoutsIter = popupMenuLayouts.begin(); popupMenuLayoutsIter != popupMenuLayouts.end(); ++popupMenuLayoutsIter)
		popupMenuLayoutsIter.value()->saveData();
	for (int i = 0; i < skills.size(); ++i)
		skills[i]->saveData();
	for (int i = 0; i < terrain.size(); ++i)
		terrain[i]->saveData();
	for (int i = 0; i < tilesets.size(); ++i)
		tilesets[i]->saveData();
	for (int i = 0; i < vehicles.size(); ++i)
		vehicles[i]->saveData();
	saveTitleMenuLayoutToStorage();
}

#endif // COMPILING_EDITOR

void ProjectData::addAttribute(Attribute *value)
{
	StorageFile storageFile;
	ReferenceCounter<Attribute> *refCounter = new ReferenceCounter<Attribute>(attributes.size());
	storageFile.openFile(refCounter->getStorageLocation().toLocal8Bit().data(), StorageFile::MODE_WRITE);
	value->saveToStorageFile(storageFile);
	storageFile.closeFile();
	attributes.append(refCounter);
	attributeNames.append(value->getName());
	if (RUNNING_IMPORTER)
		ImporterNode.addChild("attribute").addAttribute("name", value->getName().toUtf8().data());
	saved = false;
}

#ifdef COMPILING_EDITOR

void ProjectData::deleteAttribute(int num)
{
	if (attributes.size() > num)
	{
		if (attributes[num] != NULL)
		{
			delete (attributes[num]);
			attributes[num] = NULL;
		}
		attributes.removeAt(num);
		for (int i = num; i < attributes.size(); ++i)
			attributes[i]->changeDataID(i);
	}
	if (attributeNames.size() > num)
		attributeNames.removeAt(num);
	saved = false;
}

bool ProjectData::loadAttributes()
{
	QFileInfo fileinfo;
	XMLNode AttributeNode;
	Attribute *attribute;
	ReferenceCounter<Attribute> *refCounter;
	StorageFile storageFile;
	bool done = false;
	attributeNames.clear();
	for (int i = 0; !done; ++i)
	{
		fileinfo.setFile(projectLocation + QString("/attributes/attribute%1.xml").arg(i, 4, 10, QChar('0')));
		if (fileinfo.exists())
		{
			AttributeNode = XMLNode::openFileHelper(fileinfo.absoluteFilePath().toLocal8Bit().data(), "openrpgmaker-attribute");
			if (AttributeNode.isEmpty() == TRUE)
			{
				QMessageBox::critical(0, "XML Error", QString(XMLNode::parseError));
				return false;
			}
			attribute = new Attribute(AttributeNode);
			refCounter = new ReferenceCounter<Attribute>(i);
			storageFile.openFile(refCounter->getStorageLocation().toLocal8Bit().data(), StorageFile::MODE_WRITE);
			attribute->saveToStorageFile(storageFile);
			storageFile.closeFile();
			attributes.append(refCounter);
			attributeNames.append(attribute->getName());
			delete attribute;
		}
		else
			done = true;
	}
	return true;
}

bool ProjectData::checkAttributes()
{
	QFileInfo fileinfo;
	XMLNode AttributeNode;
	bool done = false;
	for (int i = 0; !done; ++i)
	{
		fileinfo.setFile(projectLocation + QString("/attributes/attribute%1.xml").arg(i, 4, 10, QChar('0')));
		XMLTools::setCurrentFileName(fileinfo.fileName());
		if (fileinfo.exists())
		{
			AttributeNode = XMLNode::openFileHelper(fileinfo.absoluteFilePath().toLocal8Bit().data(), "openrpgmaker-attribute");
			if (AttributeNode.isEmpty() == TRUE)
			{
				QMessageBox::critical(0, "XML Error", QString(XMLNode::parseError));
				return false;
			}
			else if (!Attribute::tryLoad(AttributeNode))
				return false;
		}
		else
			done = true;
	}
	return true;
}

void ProjectData::saveAttributes()
{
	QDir directory = QDir(projectLocation + "/attributes");
	QStringList xmlFiles = directory.entryList(QStringList("*.xml"), QDir::Files|QDir::Writable);
	char temp_string[128];
	for (int i = 0; i < xmlFiles.size(); ++i)
		directory.remove(xmlFiles[i]);
	for (int i = 0; i < attributes.size(); ++i)
	{
		DataReference<Attribute> *dataRef = new DataReference<Attribute>(i, __FILE__, __LINE__);
		XMLNode AttributeNode = (*dataRef)->getAttributeNode();
		sprintf(temp_string, "openrpgmaker-%s", AttributeNode.getName());
		AttributeNode.updateName(temp_string);
		AttributeNode.writeToFile(QString(projectLocation + QString("/attributes/attribute%1.xml").arg(i, 4, 10, QChar('0'))).toLocal8Bit().data());
		delete dataRef;
	}
}

#endif // COMPILING_EDITOR

void ProjectData::recoverAttributes()
{
	QFileInfo fileinfo;
	Attribute *attribute;
	ReferenceCounter<Attribute> *refCounter;
	bool done = false;
	attributeNames.clear();
	for (int i = 0; !done; ++i)
	{
		fileinfo.setFile(homePath + QString("/storage/attribute%1.storage").arg(i, 4, 10, QChar('0')));
		if (fileinfo.exists())
		{
			StorageFile storageFile;
			storageFile.openFile(fileinfo.absoluteFilePath().toLocal8Bit().data(), StorageFile::MODE_READ);
			attribute = new Attribute(storageFile);
			storageFile.closeFile();
			refCounter = new ReferenceCounter<Attribute>(i);
			attributes.append(refCounter);
			attributeNames.append(attribute->getName());
			delete attribute;
		}
		else
			done = true;
	}
}

void ProjectData::freeAttributes()
{
	for (int i = 0; i < attributes.size(); ++i)
	{
		if (attributes[i] != NULL)
		{
			delete (attributes[i]);
			attributes[i] = NULL;
		}
	}
	attributes.clear();
}

void ProjectData::addBattleAnimation(BattleAnimation *value)
{
	StorageFile storageFile;
	ReferenceCounter<BattleAnimation> *refCounter = new ReferenceCounter<BattleAnimation>(battleAnimations.size());
	storageFile.openFile(refCounter->getStorageLocation().toLocal8Bit().data(), StorageFile::MODE_WRITE);
	value->saveToStorageFile(storageFile);
	storageFile.closeFile();
	battleAnimations.append(refCounter);
	battleAnimationNames.append(value->getName());
	if (RUNNING_IMPORTER)
		ImporterNode.addChild("battleAnimation").addAttribute("name", value->getName().toUtf8().data());
	saved = false;
}

#ifdef COMPILING_EDITOR

void ProjectData::deleteBattleAnimation(int num)
{
	if (battleAnimations.size() > num)
	{
		if (battleAnimations[num] != NULL)
		{
			delete (battleAnimations[num]);
			battleAnimations[num] = NULL;
		}
		battleAnimations.removeAt(num);
		for (int i = num; i < battleAnimations.size(); ++i)
			battleAnimations[i]->changeDataID(i);
	}
	if (battleAnimationNames.size() > num)
		battleAnimationNames.removeAt(num);
	saved = false;
}

bool ProjectData::loadBattleAnimations()
{
	QFileInfo fileinfo;
	XMLNode BattleAnimationNode;
	BattleAnimation *battleAnimation;
	ReferenceCounter<BattleAnimation> *refCounter;
	StorageFile storageFile;
	bool done = false;
	battleAnimationNames.clear();
	for (int i = 0; !done; ++i)
	{
		fileinfo.setFile(projectLocation + QString("/battleanimations/battleanimation%1.xml").arg(i, 4, 10, QChar('0')));
		if (fileinfo.exists())
		{
			BattleAnimationNode = XMLNode::openFileHelper(fileinfo.absoluteFilePath().toLocal8Bit().data(), "openrpgmaker-battleanimation");
			if (BattleAnimationNode.isEmpty() == TRUE)
			{
				QMessageBox::critical(0, "XML Error", QString(XMLNode::parseError));
				return false;
			}
			battleAnimation = new BattleAnimation(BattleAnimationNode);
			refCounter = new ReferenceCounter<BattleAnimation>(i);
			storageFile.openFile(refCounter->getStorageLocation().toLocal8Bit().data(), StorageFile::MODE_WRITE);
			battleAnimation->saveToStorageFile(storageFile);
			storageFile.closeFile();
			battleAnimations.append(refCounter);
			battleAnimationNames.append(battleAnimation->getName());
			delete battleAnimation;
		}
		else
			done = true;
	}
	return true;
}

bool ProjectData::checkBattleAnimations()
{
	QFileInfo fileinfo;
	XMLNode BattleAnimationNode;
	bool done = false;
	for (int i = 0; !done; ++i)
	{
		fileinfo.setFile(projectLocation + QString("/battleanimations/battleanimation%1.xml").arg(i, 4, 10, QChar('0')));
		XMLTools::setCurrentFileName(fileinfo.fileName());
		if (fileinfo.exists())
		{
			BattleAnimationNode = XMLNode::openFileHelper(fileinfo.absoluteFilePath().toLocal8Bit().data(), "openrpgmaker-battleanimation");
			if (BattleAnimationNode.isEmpty() == TRUE)
			{
				QMessageBox::critical(0, "XML Error", QString(XMLNode::parseError));
				return false;
			}
			else if (!BattleAnimation::tryLoad(BattleAnimationNode))
				return false;
		}
		else
			done = true;
	}
	return true;
}

void ProjectData::saveBattleAnimations()
{
	QDir directory = QDir(projectLocation + "/battleanimations");
	QStringList xmlFiles = directory.entryList(QStringList("*.xml"), QDir::Files|QDir::Writable);
	char temp_string[128];
	for (int i = 0; i < xmlFiles.size(); ++i)
		directory.remove(xmlFiles[i]);
	for (int i = 0; i < battleAnimations.size(); ++i)
	{
		DataReference<BattleAnimation> *dataRef = new DataReference<BattleAnimation>(i, __FILE__, __LINE__);
		XMLNode BattleAnimationNode = (*dataRef)->getAnimationNode();
		sprintf(temp_string, "openrpgmaker-%s", BattleAnimationNode.getName());
		BattleAnimationNode.updateName(temp_string);
		BattleAnimationNode.writeToFile(QString(projectLocation + QString("/battleanimations/battleanimation%1.xml").arg(i, 4, 10, QChar('0'))).toLocal8Bit().data());
		delete dataRef;
	}
}

#endif // COMPILING_EDITOR

void ProjectData::recoverBattleAnimations()
{
	QFileInfo fileinfo;
	BattleAnimation *battleAnimation;
	ReferenceCounter<BattleAnimation> *refCounter;
	bool done = false;
	battleAnimationNames.clear();
	for (int i = 0; !done; ++i)
	{
		fileinfo.setFile(homePath + QString("/storage/battleanimation%1.storage").arg(i, 4, 10, QChar('0')));
		if (fileinfo.exists())
		{
			StorageFile storageFile;
			storageFile.openFile(fileinfo.absoluteFilePath().toLocal8Bit().data(), StorageFile::MODE_READ);
			battleAnimation = new BattleAnimation(storageFile);
			storageFile.closeFile();
			refCounter = new ReferenceCounter<BattleAnimation>(i);
			battleAnimations.append(refCounter);
			battleAnimationNames.append(battleAnimation->getName());
			delete battleAnimation;
		}
		else
			done = true;
	}
}

void ProjectData::freeBattleAnimations()
{
	for (int i = 0; i < battleAnimations.size(); ++i)
	{
		if (battleAnimations[i] != NULL)
		{
			delete (battleAnimations[i]);
			battleAnimations[i] = NULL;
		}
	}
	battleAnimations.clear();
}

void ProjectData::addCharacter(Character *value)
{
	StorageFile storageFile;
	ReferenceCounter<Character> *refCounter = new ReferenceCounter<Character>(characters.size());
	storageFile.openFile(refCounter->getStorageLocation().toLocal8Bit().data(), StorageFile::MODE_WRITE);
	value->saveToStorageFile(storageFile);
	storageFile.closeFile();
	characters.append(refCounter);
	characterNames.append(value->getName());
	if (RUNNING_IMPORTER)
		ImporterNode.addChild("character").addAttribute("name", value->getName().toUtf8().data());
	saved = false;
}

#ifdef COMPILING_EDITOR

void ProjectData::deleteCharacter(int num)
{
	if (characters.size() > num)
	{
		if (characters[num] != NULL)
		{
			delete (characters[num]);
			characters[num] = NULL;
		}
		characters.removeAt(num);
		for (int i = num; i < characters.size(); ++i)
			characters[i]->changeDataID(i);
	}
	if (characterNames.size() > num)
		characterNames.removeAt(num);
	saved = false;
}

bool ProjectData::loadCharacters()
{
	QFileInfo fileinfo;
	XMLNode CharacterNode;
	Character *character;
	ReferenceCounter<Character> *refCounter;
	StorageFile storageFile;
	bool done = false;
	characterNames.clear();
	for (int i = 0; !done; ++i)
	{
		fileinfo.setFile(projectLocation + QString("/characters/character%1.xml").arg(i, 4, 10, QChar('0')));
		if (fileinfo.exists())
		{
			CharacterNode = XMLNode::openFileHelper(fileinfo.absoluteFilePath().toLocal8Bit().data(), "openrpgmaker-character");
			if (CharacterNode.isEmpty() == TRUE)
			{
				QMessageBox::critical(0, "XML Error", QString(XMLNode::parseError));
				return false;
			}
			character = new Character(CharacterNode);
			refCounter = new ReferenceCounter<Character>(i);
			storageFile.openFile(refCounter->getStorageLocation().toLocal8Bit().data(), StorageFile::MODE_WRITE);
			character->saveToStorageFile(storageFile);
			storageFile.closeFile();
			characters.append(refCounter);
			characterNames.append(character->getName());
			delete character;
		}
		else
			done = true;
	}
	return true;
}

bool ProjectData::checkCharacters()
{
	QFileInfo fileinfo;
	XMLNode CharacterNode;
	bool done = false;
	for (int i = 0; !done; ++i)
	{
		fileinfo.setFile(projectLocation + QString("/characters/character%1.xml").arg(i, 4, 10, QChar('0')));
		XMLTools::setCurrentFileName(fileinfo.fileName());
		if (fileinfo.exists())
		{
			CharacterNode = XMLNode::openFileHelper(fileinfo.absoluteFilePath().toLocal8Bit().data(), "openrpgmaker-character");
			if (CharacterNode.isEmpty() == TRUE)
			{
				QMessageBox::critical(0, "XML Error", QString(XMLNode::parseError));
				return false;
			}
			else if (!Character::tryLoad(CharacterNode))
				return false;
		}
		else
			done = true;
	}
	return true;
}

void ProjectData::saveCharacters()
{
	QDir directory = QDir(projectLocation + "/characters");
	QStringList xmlFiles = directory.entryList(QStringList("*.xml"), QDir::Files|QDir::Writable);
	char temp_string[128];
	for (int i = 0; i < xmlFiles.size(); ++i)
		directory.remove(xmlFiles[i]);
	for (int i = 0; i < characters.size(); ++i)
	{
		DataReference<Character> *dataRef = new DataReference<Character>(i, __FILE__, __LINE__);
		XMLNode CharacterNode = (*dataRef)->getCharacterNode();
		sprintf(temp_string, "openrpgmaker-%s", CharacterNode.getName());
		CharacterNode.updateName(temp_string);
		CharacterNode.writeToFile(QString(projectLocation + QString("/characters/character%1.xml").arg(i, 4, 10, QChar('0'))).toLocal8Bit().data());
		delete dataRef;
	}
}

#endif // COMPILING_EDITOR

void ProjectData::recoverCharacters()
{
	QFileInfo fileinfo;
	Character *character;
	ReferenceCounter<Character> *refCounter;
	bool done = false;
	characterNames.clear();
	for (int i = 0; !done; ++i)
	{
		fileinfo.setFile(homePath + QString("/storage/character%1.storage").arg(i, 4, 10, QChar('0')));
		if (fileinfo.exists())
		{
			StorageFile storageFile;
			storageFile.openFile(fileinfo.absoluteFilePath().toLocal8Bit().data(), StorageFile::MODE_READ);
			character = new Character(storageFile);
			storageFile.closeFile();
			refCounter = new ReferenceCounter<Character>(i);
			characters.append(refCounter);
			characterNames.append(character->getName());
			delete character;
		}
		else
			done = true;
	}
}

void ProjectData::freeCharacters()
{
	for (int i = 0; i < characters.size(); ++i)
	{
		if (characters[i] != NULL)
		{
			delete (characters[i]);
			characters[i] = NULL;
		}
	}
	characters.clear();
}

void ProjectData::addCharacterSprite(CharacterSprite *value)
{
	StorageFile storageFile;
	ReferenceCounter<CharacterSprite> *refCounter = new ReferenceCounter<CharacterSprite>(characterSprites.size());
	storageFile.openFile(refCounter->getStorageLocation().toLocal8Bit().data(), StorageFile::MODE_WRITE);
	value->saveToStorageFile(storageFile);
	storageFile.closeFile();
	characterSprites.append(refCounter);
	characterSpriteNames.append(value->getName());
	if (RUNNING_IMPORTER)
		ImporterNode.addChild("characterSprite").addAttribute("name", value->getName().toUtf8().data());
	saved = false;
}

#ifdef COMPILING_EDITOR

void ProjectData::deleteCharacterSprite(int num)
{
	if (characterSprites.size() > num)
	{
		if (characterSprites[num] != NULL)
		{
			delete (characterSprites[num]);
			characterSprites[num] = NULL;
		}
		characterSprites.removeAt(num);
		for (int i = num; i < characterSprites.size(); ++i)
			characterSprites[i]->changeDataID(i);
	}
	if (characterSpriteNames.size() > num)
		characterSpriteNames.removeAt(num);
	saved = false;
}

bool ProjectData::loadCharacterSprites()
{
	QFileInfo fileinfo;
	XMLNode SpriteNode;
	CharacterSprite *characterSprite;
	ReferenceCounter<CharacterSprite> *refCounter;
	StorageFile storageFile;
	bool done = false;
	characterSpriteNames.clear();
	for (int i = 0; !done; ++i)
	{
		fileinfo.setFile(projectLocation + QString("/charactersprites/charactersprite%1.xml").arg(i, 4, 10, QChar('0')));
		if (fileinfo.exists())
		{
			SpriteNode = XMLNode::openFileHelper(fileinfo.absoluteFilePath().toLocal8Bit().data(), "openrpgmaker-charactersprite");
			if (SpriteNode.isEmpty() == TRUE)
			{
				QMessageBox::critical(0, "XML Error", QString(XMLNode::parseError));
				return false;
			}
			characterSprite = new CharacterSprite(SpriteNode);
			refCounter = new ReferenceCounter<CharacterSprite>(i);
			storageFile.openFile(refCounter->getStorageLocation().toLocal8Bit().data(), StorageFile::MODE_WRITE);
			characterSprite->saveToStorageFile(storageFile);
			storageFile.closeFile();
			characterSprites.append(refCounter);
			characterSpriteNames.append(characterSprite->getName());
			delete characterSprite;
		}
		else
			done = true;
	}
	return true;
}

bool ProjectData::checkCharacterSprites()
{
	QFileInfo fileinfo;
	XMLNode SpriteNode;
	bool done = false;
	for (int i = 0; !done; ++i)
	{
		fileinfo.setFile(projectLocation + QString("/charactersprites/charactersprite%1.xml").arg(i, 4, 10, QChar('0')));
		XMLTools::setCurrentFileName(fileinfo.fileName());
		if (fileinfo.exists())
		{
			SpriteNode = XMLNode::openFileHelper(fileinfo.absoluteFilePath().toLocal8Bit().data(), "openrpgmaker-charactersprite");
			if (SpriteNode.isEmpty() == TRUE)
			{
				QMessageBox::critical(0, "XML Error", QString(XMLNode::parseError));
				return false;
			}
			else if (!CharacterSprite::tryLoad(SpriteNode))
				return false;
		}
		else
			done = true;
	}
	return true;
}

void ProjectData::saveCharacterSprites()
{
	QDir directory = QDir(projectLocation + "/charactersprites");
	QStringList xmlFiles = directory.entryList(QStringList("*.xml"), QDir::Files|QDir::Writable);
	char temp_string[128];
	for (int i = 0; i < xmlFiles.size(); ++i)
		directory.remove(xmlFiles[i]);
	for (int i = 0; i < characterSprites.size(); ++i)
	{
		DataReference<CharacterSprite> *dataRef = new DataReference<CharacterSprite>(i, __FILE__, __LINE__);
		XMLNode SpriteNode = (*dataRef)->getCharacterSpriteNode();
		sprintf(temp_string, "openrpgmaker-%s", SpriteNode.getName());
		SpriteNode.updateName(temp_string);
		SpriteNode.writeToFile(QString(projectLocation + QString("/charactersprites/charactersprite%1.xml").arg(i, 4, 10, QChar('0'))).toLocal8Bit().data());
		delete dataRef;
	}
}

#endif // COMPILING_EDITOR

void ProjectData::recoverCharacterSprites()
{
	QFileInfo fileinfo;
	CharacterSprite *characterSprite;
	ReferenceCounter<CharacterSprite> *refCounter;
	bool done = false;
	characterSpriteNames.clear();
	for (int i = 0; !done; ++i)
	{
		fileinfo.setFile(homePath + QString("/storage/charactersprite%1.storage").arg(i, 4, 10, QChar('0')));
		if (fileinfo.exists())
		{
			StorageFile storageFile;
			storageFile.openFile(fileinfo.absoluteFilePath().toLocal8Bit().data(), StorageFile::MODE_READ);
			characterSprite = new CharacterSprite(storageFile);
			storageFile.closeFile();
			refCounter = new ReferenceCounter<CharacterSprite>(i);
			characterSprites.append(refCounter);
			characterSpriteNames.append(characterSprite->getName());
			delete characterSprite;
		}
		else
			done = true;
	}
}

void ProjectData::freeCharacterSprites()
{
	for (int i = 0; i < characterSprites.size(); ++i)
	{
		if (characterSprites[i] != NULL)
		{
			delete (characterSprites[i]);
			characterSprites[i] = NULL;
		}
	}
	characterSprites.clear();
}

void ProjectData::addClass(Class *value)
{
	StorageFile storageFile;
	ReferenceCounter<Class> *refCounter = new ReferenceCounter<Class>(classes.size());
	storageFile.openFile(refCounter->getStorageLocation().toLocal8Bit().data(), StorageFile::MODE_WRITE);
	value->saveToStorageFile(storageFile);
	storageFile.closeFile();
	classes.append(refCounter);
	classNames.append(value->getName());
	if (RUNNING_IMPORTER)
		ImporterNode.addChild("class").addAttribute("name", value->getName().toUtf8().data());
	saved = false;
}

#ifdef COMPILING_EDITOR

void ProjectData::deleteClass(int num)
{
	if (classes.size() > num)
	{
		if (classes[num] != NULL)
		{
			delete (classes[num]);
			classes[num] = NULL;
		}
		classes.removeAt(num);
		for (int i = num; i < classes.size(); ++i)
			classes[i]->changeDataID(i);
	}
	if (classNames.size() > num)
		classNames.removeAt(num);
	saved = false;
}

bool ProjectData::loadClasses()
{
	QFileInfo fileinfo;
	XMLNode ClassNode;
	Class *characterClass;
	ReferenceCounter<Class> *refCounter;
	StorageFile storageFile;
	bool done = false;
	classNames.clear();
	for (int i = 0; !done; ++i)
	{
		fileinfo.setFile(projectLocation + QString("/classes/class%1.xml").arg(i, 4, 10, QChar('0')));
		if (fileinfo.exists())
		{
			ClassNode = XMLNode::openFileHelper(fileinfo.absoluteFilePath().toLocal8Bit().data(), "openrpgmaker-class");
			if (ClassNode.isEmpty() == TRUE)
			{
				QMessageBox::critical(0, "XML Error", QString(XMLNode::parseError));
				return false;
			}
			characterClass = new Class(ClassNode);
			refCounter = new ReferenceCounter<Class>(i);
			storageFile.openFile(refCounter->getStorageLocation().toLocal8Bit().data(), StorageFile::MODE_WRITE);
			characterClass->saveToStorageFile(storageFile);
			storageFile.closeFile();
			classes.append(refCounter);
			classNames.append(characterClass->getName());
			delete characterClass;
		}
		else
			done = true;
	}
	return true;
}

bool ProjectData::checkClasses()
{
	QFileInfo fileinfo;
	XMLNode ClassNode;
	bool done = false;
	for (int i = 0; !done; ++i)
	{
		fileinfo.setFile(projectLocation + QString("/classes/class%1.xml").arg(i, 4, 10, QChar('0')));
		XMLTools::setCurrentFileName(fileinfo.fileName());
		if (fileinfo.exists())
		{
			ClassNode = XMLNode::openFileHelper(fileinfo.absoluteFilePath().toLocal8Bit().data(), "openrpgmaker-class");
			if (ClassNode.isEmpty() == TRUE)
			{
				QMessageBox::critical(0, "XML Error", QString(XMLNode::parseError));
				return false;
			}
			else if (!Class::tryLoad(ClassNode))
				return false;
		}
		else
			done = true;
	}
	return true;
}

void ProjectData::saveClasses()
{
	QDir directory = QDir(projectLocation + "/classes");
	QStringList xmlFiles = directory.entryList(QStringList("*.xml"), QDir::Files|QDir::Writable);
	char temp_string[128];
	for (int i = 0; i < xmlFiles.size(); ++i)
		directory.remove(xmlFiles[i]);
	for (int i = 0; i < classes.size(); ++i)
	{
		DataReference<Class> *dataRef = new DataReference<Class>(i, __FILE__, __LINE__);
		XMLNode ClassNode = (*dataRef)->getClassNode();
		sprintf(temp_string, "openrpgmaker-%s", ClassNode.getName());
		ClassNode.updateName(temp_string);
		ClassNode.writeToFile(QString(projectLocation + QString("/classes/class%1.xml").arg(i, 4, 10, QChar('0'))).toLocal8Bit().data());
		delete dataRef;
	}
}

#endif // COMPILING_EDITOR

void ProjectData::recoverClasses()
{
	QFileInfo fileinfo;
	Class *characterClass;
	ReferenceCounter<Class> *refCounter;
	bool done = false;
	classNames.clear();
	for (int i = 0; !done; ++i)
	{
		fileinfo.setFile(homePath + QString("/storage/class%1.storage").arg(i, 4, 10, QChar('0')));
		if (fileinfo.exists())
		{
			StorageFile storageFile;
			storageFile.openFile(fileinfo.absoluteFilePath().toLocal8Bit().data(), StorageFile::MODE_READ);
			characterClass = new Class(storageFile);
			storageFile.closeFile();
			refCounter = new ReferenceCounter<Class>(i);
			classes.append(refCounter);
			classNames.append(characterClass->getName());
			delete characterClass;
		}
		else
			done = true;
	}
}

void ProjectData::freeClasses()
{
	for (int i = 0; i < classes.size(); ++i)
	{
		if (classes[i] != NULL)
		{
			delete (classes[i]);
			classes[i] = NULL;
		}
	}
	classes.clear();
}

void ProjectData::addCommonEvent(CommonEvent *value)
{
	StorageFile storageFile;
	ReferenceCounter<CommonEvent> *refCounter = new ReferenceCounter<CommonEvent>(commonEvents.size());
	storageFile.openFile(refCounter->getStorageLocation().toLocal8Bit().data(), StorageFile::MODE_WRITE);
	value->saveToStorageFile(storageFile);
	storageFile.closeFile();
	commonEvents.append(refCounter);
	commonEventNames.append(value->getName());
	if (RUNNING_IMPORTER)
		ImporterNode.addChild("commonEvent").addAttribute("name", value->getName().toUtf8().data());
	saved = false;
}

#ifdef COMPILING_EDITOR

void ProjectData::deleteCommonEvent(int num)
{
	if (commonEvents.size() > num)
	{
		if (commonEvents[num] != NULL)
		{
			delete (commonEvents[num]);
			commonEvents[num] = NULL;
		}
		commonEvents.removeAt(num);
		for (int i = num; i < commonEvents.size(); ++i)
			commonEvents[i]->changeDataID(i);
	}
	if (commonEventNames.size() > num)
		commonEventNames.removeAt(num);
	saved = false;
}

bool ProjectData::loadCommonEvents()
{
	QFileInfo fileinfo;
	XMLNode CommonEventNode;
	CommonEvent *commonEvent;
	ReferenceCounter<CommonEvent> *refCounter;
	StorageFile storageFile;
	bool done = false;
	commonEventNames.clear();
	for (int i = 0; !done; ++i)
	{
		fileinfo.setFile(projectLocation + QString("/commonevents/commonevent%1.xml").arg(i, 4, 10, QChar('0')));
		if (fileinfo.exists())
		{
			CommonEventNode = XMLNode::openFileHelper(fileinfo.absoluteFilePath().toLocal8Bit().data(), "openrpgmaker-commonevent");
			if (CommonEventNode.isEmpty() == TRUE)
			{
				QMessageBox::critical(0, "XML Error", QString(XMLNode::parseError));
				return false;
			}
			commonEvent = new CommonEvent(CommonEventNode);
			refCounter = new ReferenceCounter<CommonEvent>(i);
			storageFile.openFile(refCounter->getStorageLocation().toLocal8Bit().data(), StorageFile::MODE_WRITE);
			commonEvent->saveToStorageFile(storageFile);
			storageFile.closeFile();
			commonEvents.append(refCounter);
			commonEventNames.append(commonEvent->getName());
			delete commonEvent;
		}
		else
			done = true;
	}
	return true;
}

bool ProjectData::checkCommonEvents()
{
	QFileInfo fileinfo;
	XMLNode CommonEventNode;
	bool done = false;
	for (int i = 0; !done; ++i)
	{
		fileinfo.setFile(projectLocation + QString("/commonevents/commonevent%1.xml").arg(i, 4, 10, QChar('0')));
		XMLTools::setCurrentFileName(fileinfo.fileName());
		if (fileinfo.exists())
		{
			CommonEventNode = XMLNode::openFileHelper(fileinfo.absoluteFilePath().toLocal8Bit().data(), "openrpgmaker-commonevent");
			if (CommonEventNode.isEmpty() == TRUE)
			{
				QMessageBox::critical(0, "XML Error", QString(XMLNode::parseError));
				return false;
			}
			else if (!CommonEvent::tryLoad(CommonEventNode))
				return false;
		}
		else
			done = true;
	}
	return true;
}

void ProjectData::saveCommonEvents()
{
	QDir directory = QDir(projectLocation + "/commonevents");
	QStringList xmlFiles = directory.entryList(QStringList("*.xml"), QDir::Files|QDir::Writable);
	char temp_string[128];
	for (int i = 0; i < xmlFiles.size(); ++i)
		directory.remove(xmlFiles[i]);
	for (int i = 0; i < commonEvents.size(); ++i)
	{
		DataReference<CommonEvent> *dataRef = new DataReference<CommonEvent>(i, __FILE__, __LINE__);
		XMLNode CommonEventNode = (*dataRef)->getCommonEventNode();
		sprintf(temp_string, "openrpgmaker-%s", CommonEventNode.getName());
		CommonEventNode.updateName(temp_string);
		CommonEventNode.writeToFile(QString(projectLocation + QString("/commonevents/commonevent%1.xml").arg(i, 4, 10, QChar('0'))).toLocal8Bit().data());
		delete dataRef;
	}
}

#endif // COMPILING_EDITOR

void ProjectData::recoverCommonEvents()
{
	QFileInfo fileinfo;
	CommonEvent *commonEvent;
	ReferenceCounter<CommonEvent> *refCounter;
	bool done = false;
	commonEventNames.clear();
	for (int i = 0; !done; ++i)
	{
		fileinfo.setFile(homePath + QString("/storage/commonevent%1.storage").arg(i, 4, 10, QChar('0')));
		if (fileinfo.exists())
		{
			StorageFile storageFile;
			storageFile.openFile(fileinfo.absoluteFilePath().toLocal8Bit().data(), StorageFile::MODE_READ);
			commonEvent = new CommonEvent(storageFile);
			storageFile.closeFile();
			refCounter = new ReferenceCounter<CommonEvent>(i);
			commonEvents.append(refCounter);
			commonEventNames.append(commonEvent->getName());
			delete commonEvent;
		}
		else
			done = true;
	}
}

void ProjectData::freeCommonEvents()
{
	for (int i = 0; i < commonEvents.size(); ++i)
	{
		if (commonEvents[i] != NULL)
		{
			delete (commonEvents[i]);
			commonEvents[i] = NULL;
		}
	}
	commonEvents.clear();
}

void ProjectData::addCondition(Condition *value)
{
	StorageFile storageFile;
	ReferenceCounter<Condition> *refCounter = new ReferenceCounter<Condition>(conditions.size());
	storageFile.openFile(refCounter->getStorageLocation().toLocal8Bit().data(), StorageFile::MODE_WRITE);
	value->saveToStorageFile(storageFile);
	storageFile.closeFile();
	conditions.append(refCounter);
	conditionNames.append(value->getName());
	if (RUNNING_IMPORTER)
		ImporterNode.addChild("condition").addAttribute("name", value->getName().toUtf8().data());
	saved = false;
}

#ifdef COMPILING_EDITOR

void ProjectData::deleteCondition(int num)
{
	if (conditions.size() > num)
	{
		if (conditions[num] != NULL)
		{
			delete (conditions[num]);
			conditions[num] = NULL;
		}
		conditions.removeAt(num);
		for (int i = num; i < conditions.size(); ++i)
			conditions[i]->changeDataID(i);
	}
	if (conditionNames.size() > num)
		conditionNames.removeAt(num);
	saved = false;
}

bool ProjectData::loadConditions()
{
	QFileInfo fileinfo;
	XMLNode ConditionNode;
	Condition *condition;
	ReferenceCounter<Condition> *refCounter;
	StorageFile storageFile;
	bool done = false;
	conditionNames.clear();
	for (int i = 0; !done; ++i)
	{
		fileinfo.setFile(projectLocation + QString("/conditions/condition%1.xml").arg(i, 4, 10, QChar('0')));
		if (fileinfo.exists())
		{
			ConditionNode = XMLNode::openFileHelper(fileinfo.absoluteFilePath().toLocal8Bit().data(), "openrpgmaker-condition");
			if (ConditionNode.isEmpty() == TRUE)
			{
				QMessageBox::critical(0, "XML Error", QString(XMLNode::parseError));
				return false;
			}
			condition = new Condition(ConditionNode);
			refCounter = new ReferenceCounter<Condition>(i);
			storageFile.openFile(refCounter->getStorageLocation().toLocal8Bit().data(), StorageFile::MODE_WRITE);
			condition->saveToStorageFile(storageFile);
			storageFile.closeFile();
			conditions.append(refCounter);
			conditionNames.append(condition->getName());
			delete condition;
		}
		else
			done = true;
	}
	return true;
}

bool ProjectData::checkConditions()
{
	QFileInfo fileinfo;
	XMLNode ConditionNode;
	bool done = false;
	for (int i = 0; !done; ++i)
	{
		fileinfo.setFile(projectLocation + QString("/conditions/condition%1.xml").arg(i, 4, 10, QChar('0')));
		XMLTools::setCurrentFileName(fileinfo.fileName());
		if (fileinfo.exists())
		{
			ConditionNode = XMLNode::openFileHelper(fileinfo.absoluteFilePath().toLocal8Bit().data(), "openrpgmaker-condition");
			if (ConditionNode.isEmpty() == TRUE)
			{
				QMessageBox::critical(0, "XML Error", QString(XMLNode::parseError));
				return false;
			}
			else if (!Condition::tryLoad(ConditionNode))
				return false;
		}
		else
			done = true;
	}
	return true;
}

void ProjectData::saveConditions()
{
	QDir directory = QDir(projectLocation + "/conditions");
	QStringList xmlFiles = directory.entryList(QStringList("*.xml"), QDir::Files|QDir::Writable);
	char temp_string[128];
	for (int i = 0; i < xmlFiles.size(); ++i)
		directory.remove(xmlFiles[i]);
	for (int i = 0; i < conditions.size(); ++i)
	{
		DataReference<Condition> *dataRef = new DataReference<Condition>(i, __FILE__, __LINE__);
		XMLNode ConditionNode = (*dataRef)->getConditionNode();
		sprintf(temp_string, "openrpgmaker-%s", ConditionNode.getName());
		ConditionNode.updateName(temp_string);
		ConditionNode.writeToFile(QString(projectLocation + QString("/conditions/condition%1.xml").arg(i, 4, 10, QChar('0'))).toLocal8Bit().data());
		delete dataRef;
	}
}

#endif // COMPILING_EDITOR

void ProjectData::recoverConditions()
{
	QFileInfo fileinfo;
	Condition *condition;
	ReferenceCounter<Condition> *refCounter;
	bool done = false;
	conditionNames.clear();
	for (int i = 0; !done; ++i)
	{
		fileinfo.setFile(homePath + QString("/storage/condition%1.storage").arg(i, 4, 10, QChar('0')));
		if (fileinfo.exists())
		{
			StorageFile storageFile;
			storageFile.openFile(fileinfo.absoluteFilePath().toLocal8Bit().data(), StorageFile::MODE_READ);
			condition = new Condition(storageFile);
			storageFile.closeFile();
			refCounter = new ReferenceCounter<Condition>(i);
			conditions.append(refCounter);
			conditionNames.append(condition->getName());
			delete condition;
		}
		else
			done = true;
	}
}

void ProjectData::freeConditions()
{
	for (int i = 0; i < conditions.size(); ++i)
	{
		if (conditions[i] != NULL)
		{
			delete (conditions[i]);
			conditions[i] = NULL;
		}
	}
	conditions.clear();
}

void ProjectData::addFont(Font *value)
{
	StorageFile storageFile;
	ReferenceCounter<Font> *refCounter = new ReferenceCounter<Font>(fonts.size());
	storageFile.openFile(refCounter->getStorageLocation().toLocal8Bit().data(), StorageFile::MODE_WRITE);
	value->saveToStorageFile(storageFile);
	storageFile.closeFile();
	fonts.append(refCounter);
	fontNames.append(value->getName());
	if (RUNNING_IMPORTER)
		ImporterNode.addChild("font").addAttribute("name", value->getName().toUtf8().data());
	saved = false;
}

#ifdef COMPILING_EDITOR

void ProjectData::deleteFont(int num)
{
	if (fonts.size() > num)
	{
		if (fonts[num] != NULL)
		{
			delete (fonts[num]);
			fonts[num] = NULL;
		}
		fonts.removeAt(num);
		for (int i = num; i < fonts.size(); ++i)
			fonts[i]->changeDataID(i);
	}
	if (fontNames.size() > num)
		fontNames.removeAt(num);
	saved = false;
}

bool ProjectData::loadFonts()
{
	QFileInfo fileinfo;
	XMLNode FontNode;
	Font *font;
	ReferenceCounter<Font> *refCounter;
	StorageFile storageFile;
	bool done = false;
	fontNames.clear();
	for (int i = 0; !done; ++i)
	{
		fileinfo.setFile(projectLocation + QString("/fonts/font%1.xml").arg(i, 2, 16, QChar('0')));
		if (fileinfo.exists())
		{
			FontNode = XMLNode::openFileHelper(fileinfo.absoluteFilePath().toLocal8Bit().data(), "openrpgmaker-font");
			if (FontNode.isEmpty() == TRUE)
			{
				QMessageBox::critical(0, "XML Error", QString(XMLNode::parseError));
				return false;
			}
			font = new Font(FontNode);
			refCounter = new ReferenceCounter<Font>(i);
			storageFile.openFile(refCounter->getStorageLocation().toLocal8Bit().data(), StorageFile::MODE_WRITE);
			font->saveToStorageFile(storageFile);
			storageFile.closeFile();
			fonts.append(refCounter);
			fontNames.append(font->getName());
			delete font;
		}
		else
			done = true;
	}
	return true;
}

bool ProjectData::checkFonts()
{
	QFileInfo fileinfo;
	XMLNode FontNode;
	bool done = false;
	for (int i = 0; !done; ++i)
	{
		fileinfo.setFile(projectLocation + QString("/fonts/font%1.xml").arg(i, 2, 16, QChar('0')));
		XMLTools::setCurrentFileName(fileinfo.fileName());
		if (fileinfo.exists())
		{
			FontNode = XMLNode::openFileHelper(fileinfo.absoluteFilePath().toLocal8Bit().data(), "openrpgmaker-font");
			if (FontNode.isEmpty() == TRUE)
			{
				QMessageBox::critical(0, "XML Error", QString(XMLNode::parseError));
				return false;
			}
			else if (!Font::tryLoad(FontNode))
				return false;
		}
		else
			done = true;
	}
	return true;
}

void ProjectData::saveFonts()
{
	QDir directory = QDir(projectLocation + "/fonts");
	QStringList xmlFiles = directory.entryList(QStringList("*.xml"), QDir::Files|QDir::Writable);
	char temp_string[128];
	for (int i = 0; i < xmlFiles.size(); ++i)
		directory.remove(xmlFiles[i]);
	for (int i = 0; i < fonts.size(); ++i)
	{
		DataReference<Font> *dataRef = new DataReference<Font>(i, __FILE__, __LINE__);
		XMLNode FontNode = (*dataRef)->getFontNode();
		sprintf(temp_string, "openrpgmaker-%s", FontNode.getName());
		FontNode.updateName(temp_string);
		FontNode.writeToFile(QString(projectLocation + QString("/fonts/font%1.xml").arg(i, 2, 16, QChar('0'))).toLocal8Bit().data());
		delete dataRef;
	}
}

#endif // COMPILING_EDITOR

void ProjectData::recoverFonts()
{
	QFileInfo fileinfo;
	Font *font;
	ReferenceCounter<Font> *refCounter;
	bool done = false;
	fontNames.clear();
	for (int i = 0; !done; ++i)
	{
		fileinfo.setFile(homePath + QString("/storage/font%1.storage").arg(i, 4, 10, QChar('0')));
		if (fileinfo.exists())
		{
			StorageFile storageFile;
			storageFile.openFile(fileinfo.absoluteFilePath().toLocal8Bit().data(), StorageFile::MODE_READ);
			font = new Font(storageFile);
			storageFile.closeFile();
			refCounter = new ReferenceCounter<Font>(i);
			fonts.append(refCounter);
			fontNames.append(font->getName());
			delete font;
		}
		else
			done = true;
	}
}

void ProjectData::freeFonts()
{
	for (int i = 0; i < fonts.size(); ++i)
	{
		if (fonts[i] != NULL)
		{
			delete (fonts[i]);
			fonts[i] = NULL;
		}
	}
	fonts.clear();
}

void ProjectData::addGlobalAnimation(GlobalAnimation *value)
{
	StorageFile storageFile;
	ReferenceCounter<GlobalAnimation> *refCounter = new ReferenceCounter<GlobalAnimation>(globalAnimations.size());
	storageFile.openFile(refCounter->getStorageLocation().toLocal8Bit().data(), StorageFile::MODE_WRITE);
	value->saveToStorageFile(storageFile);
	storageFile.closeFile();
	globalAnimations.append(refCounter);
	globalAnimationNames.append(value->getName());
	if (RUNNING_IMPORTER)
		ImporterNode.addChild("globalAnimation").addAttribute("name", value->getName().toUtf8().data());
	saved = false;
}

#ifdef COMPILING_EDITOR

void ProjectData::deleteGlobalAnimation(int num)
{
	if (globalAnimations.size() > num)
	{
		if (globalAnimations[num] != NULL)
		{
			delete (globalAnimations[num]);
			globalAnimations[num] = NULL;
		}
		globalAnimations.removeAt(num);
		for (int i = num; i < globalAnimations.size(); ++i)
			globalAnimations[i]->changeDataID(i);
	}
	if (globalAnimationNames.size() > num)
		globalAnimationNames.removeAt(num);
	saved = false;
}

bool ProjectData::loadGlobalAnimations()
{
	QFileInfo fileinfo;
	XMLNode AnimationNode;
	GlobalAnimation *globalAnimation;
	ReferenceCounter<GlobalAnimation> *refCounter;
	StorageFile storageFile;
	bool done = false;
	globalAnimationNames.clear();
	for (int i = 0; !done; ++i)
	{
		fileinfo.setFile(projectLocation + QString("/globalanimations/globalanimation%1.xml").arg(i, 4, 10, QChar('0')));
		if (fileinfo.exists())
		{
			AnimationNode = XMLNode::openFileHelper(fileinfo.absoluteFilePath().toLocal8Bit().data(), "openrpgmaker-globalanimation");
			if (AnimationNode.isEmpty() == TRUE)
			{
				QMessageBox::critical(0, "XML Error", QString(XMLNode::parseError));
				return false;
			}
			globalAnimation = new GlobalAnimation(AnimationNode);
			refCounter = new ReferenceCounter<GlobalAnimation>(i);
			storageFile.openFile(refCounter->getStorageLocation().toLocal8Bit().data(), StorageFile::MODE_WRITE);
			globalAnimation->saveToStorageFile(storageFile);
			storageFile.closeFile();
			globalAnimations.append(refCounter);
			globalAnimationNames.append(globalAnimation->getName());
			delete globalAnimation;
		}
		else
			done = true;
	}
	return true;
}

bool ProjectData::checkGlobalAnimations()
{
	QFileInfo fileinfo;
	XMLNode AnimationNode;
	bool done = false;
	for (int i = 0; !done; ++i)
	{
		fileinfo.setFile(projectLocation + QString("/globalanimations/globalanimation%1.xml").arg(i, 4, 10, QChar('0')));
		XMLTools::setCurrentFileName(fileinfo.fileName());
		if (fileinfo.exists())
		{
			AnimationNode = XMLNode::openFileHelper(fileinfo.absoluteFilePath().toLocal8Bit().data(), "openrpgmaker-globalanimation");
			if (AnimationNode.isEmpty() == TRUE)
			{
				QMessageBox::critical(0, "XML Error", QString(XMLNode::parseError));
				return false;
			}
			else if (!GlobalAnimation::tryLoad(AnimationNode))
				return false;
		}
		else
			done = true;
	}
	return true;
}

void ProjectData::saveGlobalAnimations()
{
	QDir directory = QDir(projectLocation + "/globalanimations");
	QStringList xmlFiles = directory.entryList(QStringList("*.xml"), QDir::Files|QDir::Writable);
	char temp_string[128];
	for (int i = 0; i < xmlFiles.size(); ++i)
		directory.remove(xmlFiles[i]);
	for (int i = 0; i < globalAnimations.size(); ++i)
	{
		DataReference<GlobalAnimation> *dataRef = new DataReference<GlobalAnimation>(i, __FILE__, __LINE__);
		XMLNode AnimationNode = (*dataRef)->getAnimationNode();
		sprintf(temp_string, "openrpgmaker-%s", AnimationNode.getName());
		AnimationNode.updateName(temp_string);
		AnimationNode.writeToFile(QString(projectLocation + QString("/globalanimations/globalanimation%1.xml").arg(i, 4, 10, QChar('0'))).toLocal8Bit().data());
		delete dataRef;
	}
}

#endif // COMPILING_EDITOR

void ProjectData::recoverGlobalAnimations()
{
	QFileInfo fileinfo;
	GlobalAnimation *globalAnimation;
	ReferenceCounter<GlobalAnimation> *refCounter;
	bool done = false;
	globalAnimationNames.clear();
	for (int i = 0; !done; ++i)
	{
		fileinfo.setFile(homePath + QString("/storage/globalanimation%1.storage").arg(i, 4, 10, QChar('0')));
		if (fileinfo.exists())
		{
			StorageFile storageFile;
			storageFile.openFile(fileinfo.absoluteFilePath().toLocal8Bit().data(), StorageFile::MODE_READ);
			globalAnimation = new GlobalAnimation(storageFile);
			storageFile.closeFile();
			refCounter = new ReferenceCounter<GlobalAnimation>(i);
			globalAnimations.append(refCounter);
			globalAnimationNames.append(globalAnimation->getName());
			delete globalAnimation;
		}
		else
			done = true;
	}
}

void ProjectData::freeGlobalAnimations()
{
	for (int i = 0; i < globalAnimations.size(); ++i)
	{
		if (globalAnimations[i] != NULL)
		{
			delete (globalAnimations[i]);
			globalAnimations[i] = NULL;
		}
	}
	globalAnimations.clear();
}

void ProjectData::addItem(Item *value)
{
	StorageFile storageFile;
	ReferenceCounter<Item> *refCounter = new ReferenceCounter<Item>(items.size());
	storageFile.openFile(refCounter->getStorageLocation().toLocal8Bit().data(), StorageFile::MODE_WRITE);
	value->saveToStorageFile(storageFile);
	storageFile.closeFile();
	items.append(refCounter);
	itemNames.append(value->getName());
	if (RUNNING_IMPORTER)
		ImporterNode.addChild("item").addAttribute("name", value->getName().toUtf8().data());
	saved = false;
}

#ifdef COMPILING_EDITOR

void ProjectData::deleteItem(int num)
{
	if (items.size() > num)
	{
		if (items[num] != NULL)
		{
			delete (items[num]);
			items[num] = NULL;
		}
		items.removeAt(num);
		for (int i = num; i < items.size(); ++i)
			items[i]->changeDataID(i);
	}
	if (itemNames.size() > num)
		itemNames.removeAt(num);
	saved = false;
}

bool ProjectData::loadItems()
{
	QFileInfo fileinfo;
	XMLNode AnimationNode;
	Item *item;
	ReferenceCounter<Item> *refCounter;
	StorageFile storageFile;
	bool done = false;
	itemNames.clear();
	for (int i = 0; !done; ++i)
	{
		fileinfo.setFile(projectLocation + QString("/items/item%1.xml").arg(i, 4, 10, QChar('0')));
		if (fileinfo.exists())
		{
			AnimationNode = XMLNode::openFileHelper(fileinfo.absoluteFilePath().toLocal8Bit().data(), "openrpgmaker-item");
			if (AnimationNode.isEmpty() == TRUE)
			{
				QMessageBox::critical(0, "XML Error", QString(XMLNode::parseError));
				return false;
			}
			item = new Item(AnimationNode);
			refCounter = new ReferenceCounter<Item>(i);
			storageFile.openFile(refCounter->getStorageLocation().toLocal8Bit().data(), StorageFile::MODE_WRITE);
			item->saveToStorageFile(storageFile);
			storageFile.closeFile();
			items.append(refCounter);
			itemNames.append(item->getName());
			delete item;
		}
		else
			done = true;
	}
	return true;
}

bool ProjectData::checkItems()
{
	QFileInfo fileinfo;
	XMLNode AnimationNode;
	bool done = false;
	for (int i = 0; !done; ++i)
	{
		fileinfo.setFile(projectLocation + QString("/items/item%1.xml").arg(i, 4, 10, QChar('0')));
		XMLTools::setCurrentFileName(fileinfo.fileName());
		if (fileinfo.exists())
		{
			AnimationNode = XMLNode::openFileHelper(fileinfo.absoluteFilePath().toLocal8Bit().data(), "openrpgmaker-item");
			if (AnimationNode.isEmpty() == TRUE)
			{
				QMessageBox::critical(0, "XML Error", QString(XMLNode::parseError));
				return false;
			}
			else if (!Item::tryLoad(AnimationNode))
				return false;
		}
		else
			done = true;
	}
	return true;
}

void ProjectData::saveItems()
{
	QDir directory = QDir(projectLocation + "/items");
	QStringList xmlFiles = directory.entryList(QStringList("*.xml"), QDir::Files|QDir::Writable);
	char temp_string[128];
	for (int i = 0; i < xmlFiles.size(); ++i)
		directory.remove(xmlFiles[i]);
	for (int i = 0; i < items.size(); ++i)
	{
		DataReference<Item> *dataRef = new DataReference<Item>(i, __FILE__, __LINE__);
		XMLNode AnimationNode = (*dataRef)->getItemNode();
		sprintf(temp_string, "openrpgmaker-%s", AnimationNode.getName());
		AnimationNode.updateName(temp_string);
		AnimationNode.writeToFile(QString(projectLocation + QString("/items/item%1.xml").arg(i, 4, 10, QChar('0'))).toLocal8Bit().data());
		delete dataRef;
	}
}

#endif // COMPILING_EDITOR

void ProjectData::recoverItems()
{
	QFileInfo fileinfo;
	Item *item;
	ReferenceCounter<Item> *refCounter;
	bool done = false;
	itemNames.clear();
	for (int i = 0; !done; ++i)
	{
		fileinfo.setFile(homePath + QString("/storage/item%1.storage").arg(i, 4, 10, QChar('0')));
		if (fileinfo.exists())
		{
			StorageFile storageFile;
			storageFile.openFile(fileinfo.absoluteFilePath().toLocal8Bit().data(), StorageFile::MODE_READ);
			item = new Item(storageFile);
			storageFile.closeFile();
			refCounter = new ReferenceCounter<Item>(i);
			items.append(refCounter);
			itemNames.append(item->getName());
			delete item;
		}
		else
			done = true;
	}
}

void ProjectData::freeItems()
{
	for (int i = 0; i < items.size(); ++i)
	{
		if (items[i] != NULL)
		{
			delete (items[i]);
			items[i] = NULL;
		}
	}
	items.clear();
}

int ProjectData::addMainMenuLayout(MainMenuLayout *value)
{
	StorageFile storageFile;
	ReferenceCounter<MainMenuLayout> *refCounter;
	QList<int> usedMenuIDs = mainMenuLayouts.keys();
	int menuID = 0;
	while (usedMenuIDs.contains(menuID))
		++menuID;
	refCounter = new ReferenceCounter<MainMenuLayout>(menuID);
	storageFile.openFile(refCounter->getStorageLocation().toLocal8Bit().data(), StorageFile::MODE_WRITE);
	value->saveToStorageFile(storageFile);
	storageFile.closeFile();
	mainMenuLayouts.insert(menuID, refCounter);
	mainMenuLayoutNames.insert(menuID, value->getName());
	if (RUNNING_IMPORTER)
	{
		XMLNode TempNode = ImporterNode.addChild("mainMenuLayout");
		TempNode.addAttribute("name", value->getName().toUtf8().data());
		TempNode.addAttribute("id", QString::number(menuID).toUtf8().data());
	}
	saved = false;
	return menuID;
}

#ifdef COMPILING_EDITOR

void ProjectData::deleteMainMenuLayout(int num)
{
	QFileInfo info;
	if (mainMenuLayouts.contains(num))
	{
		info.setFile(mainMenuLayouts[num]->getStorageLocation());
		if (info.exists())
			info.dir().remove(info.fileName());
		if (mainMenuLayouts[num] != NULL)
		{
			delete (mainMenuLayouts[num]);
			mainMenuLayouts[num] = NULL;
		}
		mainMenuLayouts.remove(num);
	}
	if (mainMenuLayoutNames.contains(num))
		mainMenuLayoutNames.remove(num);
	saved = false;
}

bool ProjectData::loadMainMenuLayouts()
{
	QStringList fileList;
	XMLNode MenuNode;
	ReferenceCounter<MainMenuLayout> *refCounter;
	StorageFile storageFile;
	MainMenuLayout *layout;
	mainMenuLayoutNames.clear();
	fileList = QDir(projectLocation + "/layouts").entryList(QStringList("mainmenulayout*.xml"), QDir::Files|QDir::NoSymLinks|QDir::Readable|QDir::Writable, QDir::Name);
	for (int i = 0; i < fileList.size(); ++i)
	{
		QString fileName = fileList[i];
		int menuID = fileName.mid(14, 4).toInt();
		fileName = fileList[i];
		MenuNode = XMLNode::openFileHelper((projectLocation + "/layouts/" + fileName).toLocal8Bit().data(), "openrpgmaker-mainmenulayout");
		if (MenuNode.isEmpty() == TRUE)
		{
			QMessageBox::critical(0, "XML Error", QString(XMLNode::parseError));
			return false;
		}
		refCounter = new ReferenceCounter<MainMenuLayout>(menuID);
		layout = new MainMenuLayout(MenuNode);
		storageFile.openFile(refCounter->getStorageLocation().toLocal8Bit().data(), StorageFile::MODE_WRITE);
		layout->saveToStorageFile(storageFile);
		storageFile.closeFile();
		mainMenuLayouts.insert(menuID, refCounter);
		mainMenuLayoutNames.insert(menuID, layout->getName());
		delete layout;
	}
	return true;
}

bool ProjectData::checkMainMenuLayouts()
{
	QStringList fileList;
	XMLNode MenuNode;
	fileList = QDir(projectLocation + "/layouts").entryList(QStringList("mainmenulayout*.xml"), QDir::Files|QDir::NoSymLinks|QDir::Readable|QDir::Writable, QDir::Name);
	for (int i = 0; i < fileList.size(); ++i)
	{
		XMLTools::setCurrentFileName(fileList[i]);
		MenuNode = XMLNode::openFileHelper((projectLocation + "/layouts/" + fileList[i]).toLocal8Bit().data(), "openrpgmaker-mainmenulayout");
		if (MenuNode.isEmpty() == TRUE)
		{
			QMessageBox::critical(0, "XML Error", QString(XMLNode::parseError));
			return false;
		}
		else if (!MainMenuLayout::tryLoad(MenuNode))
			return false;
	}
	return true;
}

void ProjectData::saveMainMenuLayouts()
{
	QDir directory = QDir(projectLocation + "/layouts");
	QStringList xmlFiles = directory.entryList(QStringList("mainmenulayout*.xml"), QDir::Files|QDir::Writable);
	QList<int> menuIDs = mainMenuLayouts.keys();
	char temp_string[128];
	for (int i = 0; i < xmlFiles.size(); ++i)
		directory.remove(xmlFiles[i]);
	for (int i = 0; i < menuIDs.size(); ++i)
	{
		DataReference<MainMenuLayout> *dataRef = new DataReference<MainMenuLayout>(i, __FILE__, __LINE__);
		XMLNode MenuNode = (*dataRef)->getMenuNode();
		sprintf(temp_string, "openrpgmaker-%s", MenuNode.getName());
		MenuNode.updateName(temp_string);
		MenuNode.writeToFile(QString(projectLocation + QString("/layouts/mainmenulayout%1.xml").arg(menuIDs[i], 4, 10, QChar('0'))).toLocal8Bit().data());
		delete dataRef;
	}
}

#endif // COMPILING_EDITOR

void ProjectData::recoverMainMenuLayouts()
{
	QStringList fileList;
	MainMenuLayout *layout;
	ReferenceCounter<MainMenuLayout> *refCounter;
	mainMenuLayoutNames.clear();
	fileList = QDir(homePath + "/storage").entryList(QStringList("mainmenulayout*.storage"), QDir::Files|QDir::NoSymLinks|QDir::Readable|QDir::Writable, QDir::Name);
	for (int i = 0; i < fileList.size(); ++i)
	{
		QString location = homePath + "/storage/" + fileList[i];
		int menuID = fileList[i].mid(14, 4).toInt();
		StorageFile storageFile;
		storageFile.openFile(location.toLocal8Bit().data(), StorageFile::MODE_READ);
		layout = new MainMenuLayout(storageFile);
		storageFile.closeFile();
		refCounter = new ReferenceCounter<MainMenuLayout>(menuID);
		mainMenuLayouts.insert(menuID, refCounter);
		mainMenuLayoutNames.insert(menuID, layout->getName());
		delete layout;
	}
}

void ProjectData::freeMainMenuLayouts()
{
	for (mainMenuLayoutsIter = mainMenuLayouts.begin(); mainMenuLayoutsIter != mainMenuLayouts.end(); ++mainMenuLayoutsIter)
	{
		if (mainMenuLayoutsIter.value() != NULL)
		{
			delete (mainMenuLayoutsIter.value());
			mainMenuLayoutsIter.value() = NULL;
		}
	}
	mainMenuLayouts.clear();
}

int ProjectData::addMap(Map *value)
{
	StorageFile storageFile;
	ReferenceCounter<Map> *refCounter;
	QList<int> usedMapIDs = maps.keys();
	int mapID = 0;
	while (usedMapIDs.contains(mapID))
		++mapID;
	refCounter = new ReferenceCounter<Map>(mapID);
	storageFile.openFile(refCounter->getStorageLocation().toLocal8Bit().data(), StorageFile::MODE_WRITE);
	value->saveToStorageFile(storageFile);
	storageFile.closeFile();
	maps.insert(mapID, refCounter);
	mapNames.insert(mapID, value->getName());
	if (RUNNING_IMPORTER)
	{
		XMLNode TempNode = ImporterNode.addChild("map");
		TempNode.addAttribute("name", value->getName().toUtf8().data());
		TempNode.addAttribute("id", QString::number(mapID).toUtf8().data());
	}
	saved = false;
	return mapID;
}

#ifdef COMPILING_EDITOR

void ProjectData::deleteMap(int num)
{
	QFileInfo info;
	if (maps.contains(num))
	{
		info.setFile(maps[num]->getStorageLocation());
		if (info.exists())
			info.dir().remove(info.fileName());
		if (maps[num] != NULL)
		{
			delete (maps[num]);
			maps[num] = NULL;
		}
		maps.remove(num);
	}
	if (mapNames.contains(num))
		mapNames.remove(num);
	saved = false;
}

bool ProjectData::loadMaps()
{
	QStringList fileList;
	XMLNode MapNode;
	ReferenceCounter<Map> *refCounter;
	StorageFile storageFile;
	Map *map;
	mapNames.clear();
	fileList = QDir(projectLocation + "/maps").entryList(QStringList("map*.xml"), QDir::Files|QDir::NoSymLinks|QDir::Readable|QDir::Writable, QDir::Name);
	for (int i = 0; i < fileList.size(); ++i)
	{
		QString fileName = fileList[i];
		int mapID = fileName.mid(3, 4).toInt();
		fileName = fileList[i];
		MapNode = XMLNode::openFileHelper((projectLocation + "/maps/" + fileName).toLocal8Bit().data(), "openrpgmaker-map");
		if (MapNode.isEmpty() == TRUE)
		{
			QMessageBox::critical(0, "XML Error", QString(XMLNode::parseError));
			return false;
		}
		refCounter = new ReferenceCounter<Map>(mapID);
		map = new Map(MapNode);
		storageFile.openFile(refCounter->getStorageLocation().toLocal8Bit().data(), StorageFile::MODE_WRITE);
		map->saveToStorageFile(storageFile);
		storageFile.closeFile();
		maps.insert(mapID, refCounter);
		mapNames.insert(mapID, map->getName());
		delete map;
	}
	return true;
}

bool ProjectData::checkMaps()
{
	QStringList fileList;
	XMLNode MapNode;
	fileList = QDir(projectLocation + "/maps").entryList(QStringList("map*.xml"), QDir::Files|QDir::NoSymLinks|QDir::Readable|QDir::Writable, QDir::Name);
	for (int i = 0; i < fileList.size(); ++i)
	{
		XMLTools::setCurrentFileName(fileList[i]);
		MapNode = XMLNode::openFileHelper((projectLocation + "/maps/" + fileList[i]).toLocal8Bit().data(), "openrpgmaker-map");
		if (MapNode.isEmpty() == TRUE)
		{
			QMessageBox::critical(0, "XML Error", QString(XMLNode::parseError));
			return false;
		}
		else if (!Map::tryLoad(MapNode))
			return false;
	}
	return true;
}

void ProjectData::saveMaps()
{
	QDir directory = QDir(projectLocation + "/maps");
	QStringList xmlFiles = directory.entryList(QStringList("map*.xml"), QDir::Files|QDir::Writable);
	QList<int> mapIDs = maps.keys();
	char temp_string[128];
	for (int i = 0; i < xmlFiles.size(); ++i)
		directory.remove(xmlFiles[i]);
	for (int i = 0; i < mapIDs.size(); ++i)
	{
		DataReference<Map> *dataRef = new DataReference<Map>(mapIDs[i], __FILE__, __LINE__);
		XMLNode MapNode = (*dataRef)->getMapNode();
		sprintf(temp_string, "openrpgmaker-%s", MapNode.getName());
		MapNode.updateName(temp_string);
		MapNode.writeToFile(QString(projectLocation + QString("/maps/map%1.xml").arg(mapIDs[i], 4, 10, QChar('0'))).toLocal8Bit().data());
		delete dataRef;
	}
}

#endif // COMPILING_EDITOR

void ProjectData::recoverMaps()
{
	QStringList fileList;
	Map *map;
	ReferenceCounter<Map> *refCounter;
	mapNames.clear();
	fileList = QDir(homePath + "/storage").entryList(QStringList("map*.storage"), QDir::Files|QDir::NoSymLinks|QDir::Readable|QDir::Writable, QDir::Name);
	for (int i = 0; i < fileList.size(); ++i)
	{
		QString location = homePath + "/storage/" + fileList[i];
		int mapID = fileList[i].mid(3, 4).toInt();
		StorageFile storageFile;
		storageFile.openFile(location.toLocal8Bit().data(), StorageFile::MODE_READ);
		map = new Map(storageFile);
		storageFile.closeFile();
		refCounter = new ReferenceCounter<Map>(mapID);
		maps.insert(mapID, refCounter);
		mapNames.insert(mapID, map->getName());
		delete map;
	}
}

void ProjectData::freeMaps()
{
	for (mapsIter = maps.begin(); mapsIter != maps.end(); ++mapsIter)
	{
		if (mapsIter.value() != NULL)
		{
			delete (mapsIter.value());
			mapsIter.value() = NULL;
		}
	}
	maps.clear();
}

void ProjectData::addMonsterAnimation(MonsterAnimation *value)
{
	StorageFile storageFile;
	ReferenceCounter<MonsterAnimation> *refCounter = new ReferenceCounter<MonsterAnimation>(monsterAnimations.size());
	storageFile.openFile(refCounter->getStorageLocation().toLocal8Bit().data(), StorageFile::MODE_WRITE);
	value->saveToStorageFile(storageFile);
	storageFile.closeFile();
	monsterAnimations.append(refCounter);
	monsterAnimationNames.append(value->getName());
	if (RUNNING_IMPORTER)
		ImporterNode.addChild("monsterAnimation").addAttribute("name", value->getName().toUtf8().data());
	saved = false;
}

#ifdef COMPILING_EDITOR

void ProjectData::deleteMonsterAnimation(int num)
{
	if (monsterAnimations.size() > num)
	{
		if (monsterAnimations[num] != NULL)
		{
			delete (monsterAnimations[num]);
			monsterAnimations[num] = NULL;
		}
		monsterAnimations.removeAt(num);
		for (int i = num; i < monsterAnimations.size(); ++i)
			monsterAnimations[i]->changeDataID(i);
	}
	if (monsterAnimationNames.size() > num)
		monsterAnimationNames.removeAt(num);
	saved = false;
}

bool ProjectData::loadMonsterAnimations()
{
	QFileInfo fileinfo;
	XMLNode AnimationNode;
	MonsterAnimation *monsterAnimation;
	ReferenceCounter<MonsterAnimation> *refCounter;
	StorageFile storageFile;
	bool done = false;
	monsterAnimationNames.clear();
	for (int i = 0; !done; ++i)
	{
		fileinfo.setFile(projectLocation + QString("/monsteranimations/monsteranimation%1.xml").arg(i, 4, 10, QChar('0')));
		if (fileinfo.exists())
		{
			AnimationNode = XMLNode::openFileHelper(fileinfo.absoluteFilePath().toLocal8Bit().data(), "openrpgmaker-monsteranimation");
			if (AnimationNode.isEmpty() == TRUE)
			{
				QMessageBox::critical(0, "XML Error", QString(XMLNode::parseError));
				return false;
			}
			monsterAnimation = new MonsterAnimation(AnimationNode);
			refCounter = new ReferenceCounter<MonsterAnimation>(i);
			storageFile.openFile(refCounter->getStorageLocation().toLocal8Bit().data(), StorageFile::MODE_WRITE);
			monsterAnimation->saveToStorageFile(storageFile);
			storageFile.closeFile();
			monsterAnimations.append(refCounter);
			monsterAnimationNames.append(monsterAnimation->getName());
			delete monsterAnimation;
		}
		else
			done = true;
	}
	return true;
}

bool ProjectData::checkMonsterAnimations()
{
	QFileInfo fileinfo;
	XMLNode AnimationNode;
	bool done = false;
	for (int i = 0; !done; ++i)
	{
		fileinfo.setFile(projectLocation + QString("/monsteranimations/monsteranimation%1.xml").arg(i, 4, 10, QChar('0')));
		XMLTools::setCurrentFileName(fileinfo.fileName());
		if (fileinfo.exists())
		{
			AnimationNode = XMLNode::openFileHelper(fileinfo.absoluteFilePath().toLocal8Bit().data(), "openrpgmaker-monsteranimation");
			if (AnimationNode.isEmpty() == TRUE)
			{
				QMessageBox::critical(0, "XML Error", QString(XMLNode::parseError));
				return false;
			}
			else if (!MonsterAnimation::tryLoad(AnimationNode))
				return false;
		}
		else
			done = true;
	}
	return true;
}

void ProjectData::saveMonsterAnimations()
{
	QDir directory = QDir(projectLocation + "/monsteranimations");
	QStringList xmlFiles = directory.entryList(QStringList("*.xml"), QDir::Files|QDir::Writable);
	char temp_string[128];
	for (int i = 0; i < xmlFiles.size(); ++i)
		directory.remove(xmlFiles[i]);
	for (int i = 0; i < monsterAnimations.size(); ++i)
	{
		DataReference<MonsterAnimation> *dataRef = new DataReference<MonsterAnimation>(i, __FILE__, __LINE__);
		XMLNode AnimationNode = (*dataRef)->getMonsterAnimationNode();
		sprintf(temp_string, "openrpgmaker-%s", AnimationNode.getName());
		AnimationNode.updateName(temp_string);
		AnimationNode.writeToFile(QString(projectLocation + QString("/monsteranimations/monsteranimation%1.xml").arg(i, 4, 10, QChar('0'))).toLocal8Bit().data());
		delete dataRef;
	}
}

#endif // COMPILING_EDITOR

void ProjectData::recoverMonsterAnimations()
{
	QFileInfo fileinfo;
	MonsterAnimation *monsterAnimation;
	ReferenceCounter<MonsterAnimation> *refCounter;
	bool done = false;
	monsterAnimationNames.clear();
	for (int i = 0; !done; ++i)
	{
		fileinfo.setFile(homePath + QString("/storage/monsteranimation%1.storage").arg(i, 4, 10, QChar('0')));
		if (fileinfo.exists())
		{
			StorageFile storageFile;
			storageFile.openFile(fileinfo.absoluteFilePath().toLocal8Bit().data(), StorageFile::MODE_READ);
			monsterAnimation = new MonsterAnimation(storageFile);
			storageFile.closeFile();
			refCounter = new ReferenceCounter<MonsterAnimation>(i);
			monsterAnimations.append(refCounter);
			monsterAnimationNames.append(monsterAnimation->getName());
			delete monsterAnimation;
		}
		else
			done = true;
	}
}

void ProjectData::freeMonsterAnimations()
{
	for (int i = 0; i < monsterAnimations.size(); ++i)
	{
		if (monsterAnimations[i] != NULL)
		{
			delete (monsterAnimations[i]);
			monsterAnimations[i] = NULL;
		}
	}
	monsterAnimations.clear();
}

void ProjectData::addMonster(Monster *value)
{
	StorageFile storageFile;
	ReferenceCounter<Monster> *refCounter = new ReferenceCounter<Monster>(monsters.size());
	storageFile.openFile(refCounter->getStorageLocation().toLocal8Bit().data(), StorageFile::MODE_WRITE);
	value->saveToStorageFile(storageFile);
	storageFile.closeFile();
	monsters.append(refCounter);
	monsterNames.append(value->getName());
	if (RUNNING_IMPORTER)
		ImporterNode.addChild("monster").addAttribute("name", value->getName().toUtf8().data());
	saved = false;
}

#ifdef COMPILING_EDITOR

void ProjectData::deleteMonster(int num)
{
	if (monsters.size() > num)
	{
		if (monsters[num] != NULL)
		{
			delete (monsters[num]);
			monsters[num] = NULL;
		}
		monsters.removeAt(num);
		for (int i = num; i < monsters.size(); ++i)
			monsters[i]->changeDataID(i);
	}
	if (monsterNames.size() > num)
		monsterNames.removeAt(num);
	saved = false;
}

bool ProjectData::loadMonsters()
{
	QFileInfo fileinfo;
	XMLNode AnimationNode;
	Monster *monster;
	ReferenceCounter<Monster> *refCounter;
	StorageFile storageFile;
	bool done = false;
	monsterNames.clear();
	for (int i = 0; !done; ++i)
	{
		fileinfo.setFile(projectLocation + QString("/monsters/monster%1.xml").arg(i, 4, 10, QChar('0')));
		if (fileinfo.exists())
		{
			AnimationNode = XMLNode::openFileHelper(fileinfo.absoluteFilePath().toLocal8Bit().data(), "openrpgmaker-monster");
			if (AnimationNode.isEmpty() == TRUE)
			{
				QMessageBox::critical(0, "XML Error", QString(XMLNode::parseError));
				return false;
			}
			monster = new Monster(AnimationNode);
			refCounter = new ReferenceCounter<Monster>(i);
			storageFile.openFile(refCounter->getStorageLocation().toLocal8Bit().data(), StorageFile::MODE_WRITE);
			monster->saveToStorageFile(storageFile);
			storageFile.closeFile();
			monsters.append(refCounter);
			monsterNames.append(monster->getName());
			delete monster;
		}
		else
			done = true;
	}
	return true;
}

bool ProjectData::checkMonsters()
{
	QFileInfo fileinfo;
	XMLNode AnimationNode;
	bool done = false;
	for (int i = 0; !done; ++i)
	{
		fileinfo.setFile(projectLocation + QString("/monsters/monster%1.xml").arg(i, 4, 10, QChar('0')));
		XMLTools::setCurrentFileName(fileinfo.fileName());
		if (fileinfo.exists())
		{
			AnimationNode = XMLNode::openFileHelper(fileinfo.absoluteFilePath().toLocal8Bit().data(), "openrpgmaker-monster");
			if (AnimationNode.isEmpty() == TRUE)
			{
				QMessageBox::critical(0, "XML Error", QString(XMLNode::parseError));
				return false;
			}
			else if (!Monster::tryLoad(AnimationNode))
				return false;
		}
		else
			done = true;
	}
	return true;
}

void ProjectData::saveMonsters()
{
	QDir directory = QDir(projectLocation + "/monsters");
	QStringList xmlFiles = directory.entryList(QStringList("*.xml"), QDir::Files|QDir::Writable);
	char temp_string[128];
	for (int i = 0; i < xmlFiles.size(); ++i)
		directory.remove(xmlFiles[i]);
	for (int i = 0; i < monsters.size(); ++i)
	{
		DataReference<Monster> *dataRef = new DataReference<Monster>(i, __FILE__, __LINE__);
		XMLNode AnimationNode = (*dataRef)->getMonsterNode();
		sprintf(temp_string, "openrpgmaker-%s", AnimationNode.getName());
		AnimationNode.updateName(temp_string);
		AnimationNode.writeToFile(QString(projectLocation + QString("/monsters/monster%1.xml").arg(i, 4, 10, QChar('0'))).toLocal8Bit().data());
		delete dataRef;
	}
}

#endif // COMPILING_EDITOR

void ProjectData::recoverMonsters()
{
	QFileInfo fileinfo;
	Monster *monster;
	ReferenceCounter<Monster> *refCounter;
	bool done = false;
	monsterNames.clear();
	for (int i = 0; !done; ++i)
	{
		fileinfo.setFile(homePath + QString("/storage/monster%1.storage").arg(i, 4, 10, QChar('0')));
		if (fileinfo.exists())
		{
			StorageFile storageFile;
			storageFile.openFile(fileinfo.absoluteFilePath().toLocal8Bit().data(), StorageFile::MODE_READ);
			monster = new Monster(storageFile);
			storageFile.closeFile();
			refCounter = new ReferenceCounter<Monster>(i);
			monsters.append(refCounter);
			monsterNames.append(monster->getName());
			delete monster;
		}
		else
			done = true;
	}
}

void ProjectData::freeMonsters()
{
	for (int i = 0; i < monsters.size(); ++i)
	{
		if (monsters[i] != NULL)
		{
			delete (monsters[i]);
			monsters[i] = NULL;
		}
	}
	monsters.clear();
}

void ProjectData::addMonsterGroup(MonsterGroup *value)
{
	StorageFile storageFile;
	ReferenceCounter<MonsterGroup> *refCounter = new ReferenceCounter<MonsterGroup>(monsterGroups.size());
	storageFile.openFile(refCounter->getStorageLocation().toLocal8Bit().data(), StorageFile::MODE_WRITE);
	value->saveToStorageFile(storageFile);
	storageFile.closeFile();
	monsterGroups.append(refCounter);
	monsterGroupNames.append(value->getName());
	if (RUNNING_IMPORTER)
		ImporterNode.addChild("monsterGroup").addAttribute("name", value->getName().toUtf8().data());
	saved = false;
}

#ifdef COMPILING_EDITOR

void ProjectData::deleteMonsterGroup(int num)
{
	if (monsterGroups.size() > num)
	{
		if (monsterGroups[num] != NULL)
		{
			delete (monsterGroups[num]);
			monsterGroups[num] = NULL;
		}
		monsterGroups.removeAt(num);
		for (int i = num; i < monsterGroups.size(); ++i)
			monsterGroups[i]->changeDataID(i);
	}
	if (monsterGroupNames.size() > num)
		monsterGroupNames.removeAt(num);
	saved = false;
}

bool ProjectData::loadMonsterGroups()
{
	QFileInfo fileinfo;
	XMLNode MonsterGroupNode;
	MonsterGroup *monsterGroup;
	ReferenceCounter<MonsterGroup> *refCounter;
	StorageFile storageFile;
	bool done = false;
	monsterGroupNames.clear();
	for (int i = 0; !done; ++i)
	{
		fileinfo.setFile(projectLocation + QString("/monstergroups/monstergroup%1.xml").arg(i, 4, 10, QChar('0')));
		if (fileinfo.exists())
		{
			MonsterGroupNode = XMLNode::openFileHelper(fileinfo.absoluteFilePath().toLocal8Bit().data(), "openrpgmaker-monstergroup");
			if (MonsterGroupNode.isEmpty() == TRUE)
			{
				QMessageBox::critical(0, "XML Error", QString(XMLNode::parseError));
				return false;
			}
			monsterGroup = new MonsterGroup(MonsterGroupNode);
			refCounter = new ReferenceCounter<MonsterGroup>(i);
			storageFile.openFile(refCounter->getStorageLocation().toLocal8Bit().data(), StorageFile::MODE_WRITE);
			monsterGroup->saveToStorageFile(storageFile);
			storageFile.closeFile();
			monsterGroups.append(refCounter);
			monsterGroupNames.append(monsterGroup->getName());
			delete monsterGroup;
		}
		else
			done = true;
	}
	return true;
}

bool ProjectData::checkMonsterGroups()
{
	QFileInfo fileinfo;
	XMLNode MonsterGroupNode;
	bool done = false;
	for (int i = 0; !done; ++i)
	{
		fileinfo.setFile(projectLocation + QString("/monstergroups/monstergroup%1.xml").arg(i, 4, 10, QChar('0')));
		XMLTools::setCurrentFileName(fileinfo.fileName());
		if (fileinfo.exists())
		{
			MonsterGroupNode = XMLNode::openFileHelper(fileinfo.absoluteFilePath().toLocal8Bit().data(), "openrpgmaker-monstergroup");
			if (MonsterGroupNode.isEmpty() == TRUE)
			{
				QMessageBox::critical(0, "XML Error", QString(XMLNode::parseError));
				return false;
			}
			else if (!MonsterGroup::tryLoad(MonsterGroupNode))
				return false;
		}
		else
			done = true;
	}
	return true;
}

void ProjectData::saveMonsterGroups()
{
	QDir directory = QDir(projectLocation + "/monstergroups");
	QStringList xmlFiles = directory.entryList(QStringList("*.xml"), QDir::Files|QDir::Writable);
	char temp_string[128];
	for (int i = 0; i < xmlFiles.size(); ++i)
		directory.remove(xmlFiles[i]);
	for (int i = 0; i < monsterGroups.size(); ++i)
	{
		DataReference<MonsterGroup> *dataRef = new DataReference<MonsterGroup>(i, __FILE__, __LINE__);
		XMLNode MonsterGroupNode = (*dataRef)->getMonsterGroupNode();
		sprintf(temp_string, "openrpgmaker-%s", MonsterGroupNode.getName());
		MonsterGroupNode.updateName(temp_string);
		MonsterGroupNode.writeToFile(QString(projectLocation + QString("/monstergroups/monstergroup%1.xml").arg(i, 4, 10, QChar('0'))).toLocal8Bit().data());
		delete dataRef;
	}
}

#endif // COMPILING_EDITOR

void ProjectData::recoverMonsterGroups()
{
	QFileInfo fileinfo;
	MonsterGroup *monsterGroup;
	ReferenceCounter<MonsterGroup> *refCounter;
	bool done = false;
	monsterGroupNames.clear();
	for (int i = 0; !done; ++i)
	{
		fileinfo.setFile(homePath + QString("/storage/monstergroup%1.storage").arg(i, 4, 10, QChar('0')));
		if (fileinfo.exists())
		{
			StorageFile storageFile;
			storageFile.openFile(fileinfo.absoluteFilePath().toLocal8Bit().data(), StorageFile::MODE_READ);
			monsterGroup = new MonsterGroup(storageFile);
			storageFile.closeFile();
			refCounter = new ReferenceCounter<MonsterGroup>(i);
			monsterGroups.append(refCounter);
			monsterGroupNames.append(monsterGroup->getName());
			delete monsterGroup;
		}
		else
			done = true;
	}
}

void ProjectData::freeMonsterGroups()
{
	for (int i = 0; i < monsterGroups.size(); ++i)
	{
		if (monsterGroups[i] != NULL)
		{
			delete (monsterGroups[i]);
			monsterGroups[i] = NULL;
		}
	}
	monsterGroups.clear();
}

int ProjectData::addPopupMenuLayout(PopupMenuLayout *value)
{
	StorageFile storageFile;
	ReferenceCounter<PopupMenuLayout> *refCounter;
	QList<int> usedMenuIDs = popupMenuLayouts.keys();
	int menuID = 0;
	while (usedMenuIDs.contains(menuID))
		++menuID;
	refCounter = new ReferenceCounter<PopupMenuLayout>(menuID);
	storageFile.openFile(refCounter->getStorageLocation().toLocal8Bit().data(), StorageFile::MODE_WRITE);
	value->saveToStorageFile(storageFile);
	storageFile.closeFile();
	popupMenuLayouts.insert(menuID, refCounter);
	popupMenuLayoutNames.insert(menuID, value->getName());
	if (RUNNING_IMPORTER)
	{
		XMLNode TempNode = ImporterNode.addChild("popupMenuLayout");
		TempNode.addAttribute("name", value->getName().toUtf8().data());
		TempNode.addAttribute("id", QString::number(menuID).toUtf8().data());
	}
	saved = false;
	return menuID;
}

#ifdef COMPILING_EDITOR

void ProjectData::deletePopupMenuLayout(int num)
{
	QFileInfo info;
	if (popupMenuLayouts.contains(num))
	{
		info.setFile(popupMenuLayouts[num]->getStorageLocation());
		if (info.exists())
			info.dir().remove(info.fileName());
		if (popupMenuLayouts[num] != NULL)
		{
			delete (popupMenuLayouts[num]);
			popupMenuLayouts[num] = NULL;
		}
		popupMenuLayouts.remove(num);
	}
	if (popupMenuLayoutNames.contains(num))
		popupMenuLayoutNames.remove(num);
	saved = false;
}

bool ProjectData::loadPopupMenuLayouts()
{
	QStringList fileList;
	XMLNode MenuNode;
	ReferenceCounter<PopupMenuLayout> *refCounter;
	StorageFile storageFile;
	PopupMenuLayout *layout;
	popupMenuLayoutNames.clear();
	fileList = QDir(projectLocation + "/layouts").entryList(QStringList("popupmenulayout*.xml"), QDir::Files|QDir::NoSymLinks|QDir::Readable|QDir::Writable, QDir::Name);
	for (int i = 0; i < fileList.size(); ++i)
	{
		QString fileName = fileList[i];
		int menuID = fileName.mid(15, 4).toInt();
		fileName = fileList[i];
		MenuNode = XMLNode::openFileHelper((projectLocation + "/layouts/" + fileName).toLocal8Bit().data(), "openrpgmaker-popupmenulayout");
		if (MenuNode.isEmpty() == TRUE)
		{
			QMessageBox::critical(0, "XML Error", QString(XMLNode::parseError));
			return false;
		}
		refCounter = new ReferenceCounter<PopupMenuLayout>(menuID);
		layout = new PopupMenuLayout(MenuNode);
		storageFile.openFile(refCounter->getStorageLocation().toLocal8Bit().data(), StorageFile::MODE_WRITE);
		layout->saveToStorageFile(storageFile);
		storageFile.closeFile();
		popupMenuLayouts.insert(menuID, refCounter);
		popupMenuLayoutNames.insert(menuID, layout->getName());
		delete layout;
	}
	return true;
}

bool ProjectData::checkPopupMenuLayouts()
{
	QStringList fileList;
	XMLNode MenuNode;
	fileList = QDir(projectLocation + "/layouts").entryList(QStringList("popupmenulayout*.xml"), QDir::Files|QDir::NoSymLinks|QDir::Readable|QDir::Writable, QDir::Name);
	for (int i = 0; i < fileList.size(); ++i)
	{
		XMLTools::setCurrentFileName(fileList[i]);
		MenuNode = XMLNode::openFileHelper((projectLocation + "/layouts/" + fileList[i]).toLocal8Bit().data(), "openrpgmaker-popupmenulayout");
		if (MenuNode.isEmpty() == TRUE)
		{
			QMessageBox::critical(0, "XML Error", QString(XMLNode::parseError));
			return false;
		}
		else if (!PopupMenuLayout::tryLoad(MenuNode))
			return false;
	}
	return true;
}

void ProjectData::savePopupMenuLayouts()
{
	QDir directory = QDir(projectLocation + "/layouts");
	QStringList xmlFiles = directory.entryList(QStringList("popupmenulayout*.xml"), QDir::Files|QDir::Writable);
	QList<int> menuIDs = popupMenuLayouts.keys();
	char temp_string[128];
	for (int i = 0; i < xmlFiles.size(); ++i)
		directory.remove(xmlFiles[i]);
	for (int i = 0; i < menuIDs.size(); ++i)
	{
		DataReference<PopupMenuLayout> *dataRef = new DataReference<PopupMenuLayout>(i, __FILE__, __LINE__);
		XMLNode MenuNode = (*dataRef)->getMenuNode();
		sprintf(temp_string, "openrpgmaker-%s", MenuNode.getName());
		MenuNode.updateName(temp_string);
		MenuNode.writeToFile(QString(projectLocation + QString("/layouts/popupmenulayout%1.xml").arg(menuIDs[i], 4, 10, QChar('0'))).toLocal8Bit().data());
		delete dataRef;
	}
}

#endif // COMPILING_EDITOR

void ProjectData::recoverPopupMenuLayouts()
{
	QStringList fileList;
	PopupMenuLayout *layout;
	ReferenceCounter<PopupMenuLayout> *refCounter;
	popupMenuLayoutNames.clear();
	fileList = QDir(homePath + "/storage").entryList(QStringList("popupmenulayout*.storage"), QDir::Files|QDir::NoSymLinks|QDir::Readable|QDir::Writable, QDir::Name);
	for (int i = 0; i < fileList.size(); ++i)
	{
		QString location = homePath + "/storage/" + fileList[i];
		int menuID = fileList[i].mid(15, 4).toInt();
		StorageFile storageFile;
		storageFile.openFile(location.toLocal8Bit().data(), StorageFile::MODE_READ);
		layout = new PopupMenuLayout(storageFile);
		storageFile.closeFile();
		refCounter = new ReferenceCounter<PopupMenuLayout>(menuID);
		popupMenuLayouts.insert(menuID, refCounter);
		popupMenuLayoutNames.insert(menuID, layout->getName());
		delete layout;
	}
}

void ProjectData::freePopupMenuLayouts()
{
	for (popupMenuLayoutsIter = popupMenuLayouts.begin(); popupMenuLayoutsIter != popupMenuLayouts.end(); ++popupMenuLayoutsIter)
	{
		if (popupMenuLayoutsIter.value() != NULL)
		{
			delete (popupMenuLayoutsIter.value());
			popupMenuLayoutsIter.value() = NULL;
		}
	}
	popupMenuLayouts.clear();
}

void ProjectData::addSkill(Skill *value)
{
	StorageFile storageFile;
	ReferenceCounter<Skill> *refCounter = new ReferenceCounter<Skill>(skills.size());
	storageFile.openFile(refCounter->getStorageLocation().toLocal8Bit().data(), StorageFile::MODE_WRITE);
	value->saveToStorageFile(storageFile);
	storageFile.closeFile();
	skills.append(refCounter);
	skillNames.append(value->getName());
	if (RUNNING_IMPORTER)
		ImporterNode.addChild("skill").addAttribute("name", value->getName().toUtf8().data());
	saved = false;
}

#ifdef COMPILING_EDITOR

void ProjectData::deleteSkill(int num)
{
	if (skills.size() > num)
	{
		if (skills[num] != NULL)
		{
			delete (skills[num]);
			skills[num] = NULL;
		}
		skills.removeAt(num);
		for (int i = num; i < skills.size(); ++i)
			skills[i]->changeDataID(i);
	}
	if (skillNames.size() > num)
		skillNames.removeAt(num);
	saved = false;
}

bool ProjectData::loadSkills()
{
	QFileInfo fileinfo;
	XMLNode SkillNode;
	Skill *skill;
	ReferenceCounter<Skill> *refCounter;
	StorageFile storageFile;
	bool done = false;
	skillNames.clear();
	for (int i = 0; !done; ++i)
	{
		fileinfo.setFile(projectLocation + QString("/skills/skill%1.xml").arg(i, 4, 10, QChar('0')));
		if (fileinfo.exists())
		{
			SkillNode = XMLNode::openFileHelper(fileinfo.absoluteFilePath().toLocal8Bit().data(), "openrpgmaker-skill");
			if (SkillNode.isEmpty() == TRUE)
			{
				QMessageBox::critical(0, "XML Error", QString(XMLNode::parseError));
				return false;
			}
			skill = new Skill(SkillNode);
			refCounter = new ReferenceCounter<Skill>(i);
			storageFile.openFile(refCounter->getStorageLocation().toLocal8Bit().data(), StorageFile::MODE_WRITE);
			skill->saveToStorageFile(storageFile);
			storageFile.closeFile();
			skills.append(refCounter);
			skillNames.append(skill->getName());
			delete skill;
		}
		else
			done = true;
	}
	return true;
}

bool ProjectData::checkSkills()
{
	QFileInfo fileinfo;
	XMLNode SkillNode;
	bool done = false;
	for (int i = 0; !done; ++i)
	{
		fileinfo.setFile(projectLocation + QString("/skills/skill%1.xml").arg(i, 4, 10, QChar('0')));
		XMLTools::setCurrentFileName(fileinfo.fileName());
		if (fileinfo.exists())
		{
			SkillNode = XMLNode::openFileHelper(fileinfo.absoluteFilePath().toLocal8Bit().data(), "openrpgmaker-skill");
			if (SkillNode.isEmpty() == TRUE)
			{
				QMessageBox::critical(0, "XML Error", QString(XMLNode::parseError));
				return false;
			}
			else if (!Skill::tryLoad(SkillNode))
				return false;
		}
		else
			done = true;
	}
	return true;
}

void ProjectData::saveSkills()
{
	QDir directory = QDir(projectLocation + "/skills");
	QStringList xmlFiles = directory.entryList(QStringList("*.xml"), QDir::Files|QDir::Writable);
	char temp_string[128];
	for (int i = 0; i < xmlFiles.size(); ++i)
		directory.remove(xmlFiles[i]);
	for (int i = 0; i < skills.size(); ++i)
	{
		DataReference<Skill> *dataRef = new DataReference<Skill>(i, __FILE__, __LINE__);
		XMLNode SkillNode = (*dataRef)->getSkillNode();
		sprintf(temp_string, "openrpgmaker-%s", SkillNode.getName());
		SkillNode.updateName(temp_string);
		SkillNode.writeToFile(QString(projectLocation + QString("/skills/skill%1.xml").arg(i, 4, 10, QChar('0'))).toLocal8Bit().data());
		delete dataRef;
	}
}

#endif // COMPILING_EDITOR

void ProjectData::recoverSkills()
{
	QFileInfo fileinfo;
	Skill *skill;
	ReferenceCounter<Skill> *refCounter;
	bool done = false;
	skillNames.clear();
	for (int i = 0; !done; ++i)
	{
		fileinfo.setFile(homePath + QString("/storage/skill%1.storage").arg(i, 4, 10, QChar('0')));
		if (fileinfo.exists())
		{
			StorageFile storageFile;
			storageFile.openFile(fileinfo.absoluteFilePath().toLocal8Bit().data(), StorageFile::MODE_READ);
			skill = new Skill(storageFile);
			storageFile.closeFile();
			refCounter = new ReferenceCounter<Skill>(i);
			skills.append(refCounter);
			skillNames.append(skill->getName());
			delete skill;
		}
		else
			done = true;
	}
}

void ProjectData::freeSkills()
{
	for (int i = 0; i < skills.size(); ++i)
	{
		if (skills[i] != NULL)
		{
			delete (skills[i]);
			skills[i] = NULL;
		}
	}
	skills.clear();
}

void ProjectData::saveTitleMenuLayoutToStorage()
{
	StorageFile storageFile;
	storageFile.openFile(QString(homePath + "/storage/titlemenulayout.storage").toLocal8Bit().data(), StorageFile::MODE_WRITE);
	titleMenuLayout->saveToStorageFile(storageFile);
	storageFile.closeFile();
}

void ProjectData::loadTitleMenuLayoutFromStorage()
{
	StorageFile storageFile;
	if (titleMenuLayout != NULL)
		delete titleMenuLayout;
	storageFile.openFile(QString(homePath + "/storage/titlemenulayout.storage").toLocal8Bit().data(), StorageFile::MODE_READ);
	titleMenuLayout = new TitleMenuLayout(storageFile);
	storageFile.closeFile();
}

#ifdef COMPILING_EDITOR

bool ProjectData::loadTitleMenuLayout()
{
	QFileInfo fileinfo;
	XMLNode MenuNode;
	StorageFile storageFile;
	fileinfo.setFile(projectLocation + "/layouts/titlemenulayout.xml");
	if (fileinfo.exists())
	{
		MenuNode = XMLNode::openFileHelper(fileinfo.absoluteFilePath().toLocal8Bit().data(), "openrpgmaker-titlemenulayout");
		if (MenuNode.isEmpty() == TRUE)
		{
			QMessageBox::critical(0, "XML Error", QString(XMLNode::parseError));
			return false;
		}
		titleMenuLayout = new TitleMenuLayout(MenuNode);
	}
	else
		titleMenuLayout = new TitleMenuLayout();
	storageFile.openFile(QString(homePath + "/storage/titlemenulayout.storage").toLocal8Bit().data(), StorageFile::MODE_WRITE);
	titleMenuLayout->saveToStorageFile(storageFile);
	storageFile.closeFile();
	return true;
}

bool ProjectData::checkTitleMenuLayout()
{
	QFileInfo fileinfo;
	XMLNode MenuNode;
	fileinfo.setFile(projectLocation + "/layouts/titlemenulayout.xml");
	XMLTools::setCurrentFileName(fileinfo.fileName());
	if (fileinfo.exists())
	{
		MenuNode = XMLNode::openFileHelper(fileinfo.absoluteFilePath().toLocal8Bit().data(), "openrpgmaker-titlemenulayout");
		if (MenuNode.isEmpty() == TRUE)
		{
			QMessageBox::critical(0, "XML Error", QString(XMLNode::parseError));
			return false;
		}
		else if (!TitleMenuLayout::tryLoad(MenuNode))
			return false;
	}
	return true;
}

void ProjectData::saveTitleMenuLayout()
{
	XMLNode MenuNode = titleMenuLayout->getMenuNode();
	char temp_string[128];
	sprintf(temp_string, "openrpgmaker-%s", MenuNode.getName());
	MenuNode.updateName(temp_string);
	MenuNode.writeToFile(QString(projectLocation + "/layouts/titlemenulayout.xml").toLocal8Bit().data());
}

#endif // COMPILING_EDITOR

void ProjectData::recoverTitleMenuLayout()
{
	QFileInfo fileinfo;
	fileinfo.setFile(homePath + "/storage/titlemenulayout.storage");
	if (fileinfo.exists())
	{
		StorageFile storageFile;
		storageFile.openFile(fileinfo.absoluteFilePath().toLocal8Bit().data(), StorageFile::MODE_READ);
		titleMenuLayout = new TitleMenuLayout(storageFile);
		storageFile.closeFile();
	}
}

void ProjectData::freeTitleMenuLayout()
{
	delete titleMenuLayout;
	titleMenuLayout = NULL;
}

void ProjectData::addTerrain(Terrain *value)
{
	StorageFile storageFile;
	ReferenceCounter<Terrain> *refCounter = new ReferenceCounter<Terrain>(terrain.size());
	storageFile.openFile(refCounter->getStorageLocation().toLocal8Bit().data(), StorageFile::MODE_WRITE);
	value->saveToStorageFile(storageFile);
	storageFile.closeFile();
	terrain.append(refCounter);
	terrainNames.append(value->getName());
	if (RUNNING_IMPORTER)
		ImporterNode.addChild("terrain").addAttribute("name", value->getName().toUtf8().data());
	saved = false;
}

#ifdef COMPILING_EDITOR

void ProjectData::deleteTerrain(int num)
{
	if (terrain.size() > num)
	{
		if (terrain[num] != NULL)
		{
			delete (terrain[num]);
			terrain[num] = NULL;
		}
		terrain.removeAt(num);
		for (int i = num; i < terrain.size(); ++i)
			terrain[i]->changeDataID(i);
	}
	if (terrainNames.size() > num)
		terrainNames.removeAt(num);
	saved = false;
}

bool ProjectData::loadTerrain()
{
	QFileInfo fileinfo;
	XMLNode TerrainNode;
	Terrain *currentTerrain;
	ReferenceCounter<Terrain> *refCounter;
	StorageFile storageFile;
	bool done = false;
	terrainNames.clear();
	for (int i = 0; !done; ++i)
	{
		fileinfo.setFile(projectLocation + QString("/terrain/terrain%1.xml").arg(i, 4, 10, QChar('0')));
		if (fileinfo.exists())
		{
			TerrainNode = XMLNode::openFileHelper(fileinfo.absoluteFilePath().toLocal8Bit().data(), "openrpgmaker-terrain");
			if (TerrainNode.isEmpty() == TRUE)
			{
				QMessageBox::critical(0, "XML Error", QString(XMLNode::parseError));
				return false;
			}
			currentTerrain = new Terrain(TerrainNode);
			refCounter = new ReferenceCounter<Terrain>(i);
			storageFile.openFile(refCounter->getStorageLocation().toLocal8Bit().data(), StorageFile::MODE_WRITE);
			currentTerrain->saveToStorageFile(storageFile);
			storageFile.closeFile();
			terrain.append(refCounter);
			terrainNames.append(currentTerrain->getName());
			delete currentTerrain;
		}
		else
			done = true;
	}
	return true;
}

bool ProjectData::checkTerrain()
{
	QFileInfo fileinfo;
	XMLNode TerrainNode;
	bool done = false;
	for (int i = 0; !done; ++i)
	{
		fileinfo.setFile(projectLocation + QString("/terrain/terrain%1.xml").arg(i, 4, 10, QChar('0')));
		XMLTools::setCurrentFileName(fileinfo.fileName());
		if (fileinfo.exists())
		{
			TerrainNode = XMLNode::openFileHelper(fileinfo.absoluteFilePath().toLocal8Bit().data(), "openrpgmaker-terrain");
			if (TerrainNode.isEmpty() == TRUE)
			{
				QMessageBox::critical(0, "XML Error", QString(XMLNode::parseError));
				return false;
			}
			else if (!Terrain::tryLoad(TerrainNode))
				return false;
		}
		else
			done = true;
	}
	return true;
}

void ProjectData::saveTerrain()
{
	QDir directory = QDir(projectLocation + "/terrain");
	QStringList xmlFiles = directory.entryList(QStringList("*.xml"), QDir::Files|QDir::Writable);
	char temp_string[128];
	for (int i = 0; i < xmlFiles.size(); ++i)
		directory.remove(xmlFiles[i]);
	for (int i = 0; i < terrain.size(); ++i)
	{
		DataReference<Terrain> *dataRef = new DataReference<Terrain>(i, __FILE__, __LINE__);
		XMLNode TerrainNode = (*dataRef)->getTerrainNode();
		sprintf(temp_string, "openrpgmaker-%s", TerrainNode.getName());
		TerrainNode.updateName(temp_string);
		TerrainNode.writeToFile(QString(projectLocation + QString("/terrain/terrain%1.xml").arg(i, 4, 10, QChar('0'))).toLocal8Bit().data());
		delete dataRef;
	}
}

#endif // COMPILING_EDITOR

void ProjectData::recoverTerrain()
{
	QFileInfo fileinfo;
	Terrain *currentTerrain;
	ReferenceCounter<Terrain> *refCounter;
	bool done = false;
	terrainNames.clear();
	for (int i = 0; !done; ++i)
	{
		fileinfo.setFile(homePath + QString("/storage/terrain%1.storage").arg(i, 4, 10, QChar('0')));
		if (fileinfo.exists())
		{
			StorageFile storageFile;
			storageFile.openFile(fileinfo.absoluteFilePath().toLocal8Bit().data(), StorageFile::MODE_READ);
			currentTerrain = new Terrain(storageFile);
			storageFile.closeFile();
			refCounter = new ReferenceCounter<Terrain>(i);
			terrain.append(refCounter);
			terrainNames.append(currentTerrain->getName());
			delete currentTerrain;
		}
		else
			done = true;
	}
}

void ProjectData::freeTerrain()
{
	for (int i = 0; i < terrain.size(); ++i)
	{
		if (terrain[i] != NULL)
		{
			delete (terrain[i]);
			terrain[i] = NULL;
		}
	}
	terrain.clear();
}

void ProjectData::addTileset(Tileset *value)
{
	StorageFile storageFile;
	ReferenceCounter<Tileset> *refCounter = new ReferenceCounter<Tileset>(tilesets.size());
	storageFile.openFile(refCounter->getStorageLocation().toLocal8Bit().data(), StorageFile::MODE_WRITE);
	value->saveToStorageFile(storageFile);
	storageFile.closeFile();
	tilesets.append(refCounter);
	tilesetNames.append(value->getName());
	if (RUNNING_IMPORTER)
		ImporterNode.addChild("tileset").addAttribute("name", value->getName().toUtf8().data());
	saved = false;
}

#ifdef COMPILING_EDITOR

void ProjectData::deleteTileset(int num)
{
	if (tilesets.size() > num)
	{
		if (tilesets[num] != NULL)
		{
			delete (tilesets[num]);
			tilesets[num] = NULL;
		}
		tilesets.removeAt(num);
		for (int i = num; i < tilesets.size(); ++i)
			tilesets[i]->changeDataID(i);
	}
	if (tilesetNames.size() > num)
		tilesetNames.removeAt(num);
	saved = false;
}

bool ProjectData::loadTilesets()
{
	QFileInfo fileinfo;
	XMLNode TilesetNode;
	Tileset *tileset;
	ReferenceCounter<Tileset> *refCounter;
	StorageFile storageFile;
	bool done = false;
	tilesetNames.clear();
	for (int i = 0; !done; ++i)
	{
		fileinfo.setFile(projectLocation + QString("/tilesets/tileset%1.xml").arg(i, 2, 16, QChar('0')));
		if (fileinfo.exists())
		{
			TilesetNode = XMLNode::openFileHelper(fileinfo.absoluteFilePath().toLocal8Bit().data(), "openrpgmaker-tileset");
			if (TilesetNode.isEmpty() == TRUE)
			{
				QMessageBox::critical(0, "XML Error", QString(XMLNode::parseError));
				return false;
			}
			tileset = new Tileset(TilesetNode);
			refCounter = new ReferenceCounter<Tileset>(i);
			storageFile.openFile(refCounter->getStorageLocation().toLocal8Bit().data(), StorageFile::MODE_WRITE);
			tileset->saveToStorageFile(storageFile);
			storageFile.closeFile();
			tilesets.append(refCounter);
			tilesetNames.append(tileset->getName());
			delete tileset;
		}
		else
			done = true;
	}
	return true;
}

bool ProjectData::checkTilesets()
{
	QFileInfo fileinfo;
	XMLNode TilesetNode;
	bool done = false;
	for (int i = 0; !done; ++i)
	{
		fileinfo.setFile(projectLocation + QString("/tilesets/tileset%1.xml").arg(i, 2, 16, QChar('0')));
		XMLTools::setCurrentFileName(fileinfo.fileName());
		if (fileinfo.exists())
		{
			TilesetNode = XMLNode::openFileHelper(fileinfo.absoluteFilePath().toLocal8Bit().data(), "openrpgmaker-tileset");
			if (TilesetNode.isEmpty() == TRUE)
			{
				QMessageBox::critical(0, "XML Error", QString(XMLNode::parseError));
				return false;
			}
			else if (!Tileset::tryLoad(TilesetNode))
				return false;
		}
		else
			done = true;
	}
	return true;
}

void ProjectData::saveTilesets()
{
	QDir directory = QDir(projectLocation + "/tilesets");
	QStringList xmlFiles = directory.entryList(QStringList("*.xml"), QDir::Files|QDir::Writable);
	char temp_string[128];
	for (int i = 0; i < xmlFiles.size(); ++i)
		directory.remove(xmlFiles[i]);
	for (int i = 0; i < tilesets.size(); ++i)
	{
		DataReference<Tileset> *dataRef = new DataReference<Tileset>(i, __FILE__, __LINE__);
		XMLNode TilesetNode = (*dataRef)->getTilesetNode();
		sprintf(temp_string, "openrpgmaker-%s", TilesetNode.getName());
		TilesetNode.updateName(temp_string);
		TilesetNode.writeToFile(QString(projectLocation + QString("/tilesets/tileset%1.xml").arg(i, 2, 16, QChar('0'))).toLocal8Bit().data());
		delete dataRef;
	}
}

#endif // COMPILING_EDITOR

void ProjectData::recoverTilesets()
{
	QFileInfo fileinfo;
	Tileset *tileset;
	ReferenceCounter<Tileset> *refCounter;
	bool done = false;
	tilesetNames.clear();
	for (int i = 0; !done; ++i)
	{
		fileinfo.setFile(homePath + QString("/storage/tileset%1.storage").arg(i, 4, 10, QChar('0')));
		if (fileinfo.exists())
		{
			StorageFile storageFile;
			storageFile.openFile(fileinfo.absoluteFilePath().toLocal8Bit().data(), StorageFile::MODE_READ);
			tileset = new Tileset(storageFile);
			storageFile.closeFile();
			refCounter = new ReferenceCounter<Tileset>(i);
			tilesets.append(refCounter);
			tilesetNames.append(tileset->getName());
			delete tileset;
		}
		else
			done = true;
	}
}

void ProjectData::freeTilesets()
{
	for (int i = 0; i < tilesets.size(); ++i)
	{
		if (tilesets[i] != NULL)
		{
			delete (tilesets[i]);
			tilesets[i] = NULL;
		}
	}
	tilesets.clear();
}

void ProjectData::addVehicle(Vehicle *value)
{
	StorageFile storageFile;
	StartLocation location;
	ReferenceCounter<Vehicle> *refCounter = new ReferenceCounter<Vehicle>(vehicles.size());
	storageFile.openFile(refCounter->getStorageLocation().toLocal8Bit().data(), StorageFile::MODE_WRITE);
	value->saveToStorageFile(storageFile);
	storageFile.closeFile();
	vehicles.append(refCounter);
	vehicleNames.append(value->getName());
	location.mapID = -1;
	location.x = 0;
	location.y = 0;
	vehicleStartLocations.append(location);
	if (RUNNING_IMPORTER)
		ImporterNode.addChild("vehicle").addAttribute("name", value->getName().toUtf8().data());
	saved = false;
}

#ifdef COMPILING_EDITOR

void ProjectData::deleteVehicle(int num)
{
	if (vehicles.size() > num)
	{
		if (vehicles[num] != NULL)
		{
			delete (vehicles[num]);
			vehicles[num] = NULL;
		}
		vehicles.removeAt(num);
		for (int i = num; i < vehicles.size(); ++i)
			vehicles[i]->changeDataID(i);
	}
	if (vehicleNames.size() > num)
		vehicleNames.removeAt(num);
	vehicleStartLocations.removeAt(num);
	saved = false;
}

bool ProjectData::loadVehicles()
{
	QFileInfo fileinfo;
	XMLNode VehicleNode;
	Vehicle *vehicle;
	ReferenceCounter<Vehicle> *refCounter;
	StorageFile storageFile;
	bool done = false;
	vehicleNames.clear();
	for (int i = 0; !done; ++i)
	{
		fileinfo.setFile(projectLocation + QString("/vehicles/vehicle%1.xml").arg(i, 4, 10, QChar('0')));
		if (fileinfo.exists())
		{
			VehicleNode = XMLNode::openFileHelper(fileinfo.absoluteFilePath().toLocal8Bit().data(), "openrpgmaker-vehicle");
			if (VehicleNode.isEmpty() == TRUE)
			{
				QMessageBox::critical(0, "XML Error", QString(XMLNode::parseError));
				return false;
			}
			vehicle = new Vehicle(VehicleNode);
			refCounter = new ReferenceCounter<Vehicle>(i);
			storageFile.openFile(refCounter->getStorageLocation().toLocal8Bit().data(), StorageFile::MODE_WRITE);
			vehicle->saveToStorageFile(storageFile);
			storageFile.closeFile();
			vehicles.append(refCounter);
			vehicleNames.append(vehicle->getName());
			delete vehicle;
		}
		else
			done = true;
	}
	return true;
}

bool ProjectData::checkVehicles()
{
	QFileInfo fileinfo;
	XMLNode VehicleNode;
	bool done = false;
	for (int i = 0; !done; ++i)
	{
		fileinfo.setFile(projectLocation + QString("/vehicles/vehicle%1.xml").arg(i, 4, 10, QChar('0')));
		XMLTools::setCurrentFileName(fileinfo.fileName());
		if (fileinfo.exists())
		{
			VehicleNode = XMLNode::openFileHelper(fileinfo.absoluteFilePath().toLocal8Bit().data(), "openrpgmaker-vehicle");
			if (VehicleNode.isEmpty() == TRUE)
			{
				QMessageBox::critical(0, "XML Error", QString(XMLNode::parseError));
				return false;
			}
			else if (!Vehicle::tryLoad(VehicleNode))
				return false;
		}
		else
			done = true;
	}
	return true;
}

void ProjectData::saveVehicles()
{
	QDir directory = QDir(projectLocation + "/vehicles");
	QStringList xmlFiles = directory.entryList(QStringList("*.xml"), QDir::Files|QDir::Writable);
	char temp_string[128];
	for (int i = 0; i < xmlFiles.size(); ++i)
		directory.remove(xmlFiles[i]);
	for (int i = 0; i < vehicles.size(); ++i)
	{
		DataReference<Vehicle> *dataRef = new DataReference<Vehicle>(i, __FILE__, __LINE__);
		XMLNode VehicleNode = (*dataRef)->getVehicleNode();
		sprintf(temp_string, "openrpgmaker-%s", VehicleNode.getName());
		VehicleNode.updateName(temp_string);
		VehicleNode.writeToFile(QString(projectLocation + QString("/vehicles/vehicle%1.xml").arg(i, 4, 10, QChar('0'))).toLocal8Bit().data());
		delete dataRef;
	}
}

#endif // COMPILING_EDITOR

void ProjectData::recoverVehicles()
{
	QFileInfo fileinfo;
	Vehicle *vehicle;
	ReferenceCounter<Vehicle> *refCounter;
	bool done = false;
	vehicleNames.clear();
	for (int i = 0; !done; ++i)
	{
		fileinfo.setFile(homePath + QString("/storage/vehicle%1.storage").arg(i, 4, 10, QChar('0')));
		if (fileinfo.exists())
		{
			StorageFile storageFile;
			storageFile.openFile(fileinfo.absoluteFilePath().toLocal8Bit().data(), StorageFile::MODE_READ);
			vehicle = new Vehicle(storageFile);
			storageFile.closeFile();
			refCounter = new ReferenceCounter<Vehicle>(i);
			vehicles.append(refCounter);
			vehicleNames.append(vehicle->getName());
			delete vehicle;
		}
		else
			done = true;
	}
}

void ProjectData::freeVehicles()
{
	for (int i = 0; i < vehicles.size(); ++i)
	{
		if (vehicles[i] != NULL)
		{
			delete (vehicles[i]);
			vehicles[i] = NULL;
		}
	}
	vehicles.clear();
}

#ifdef COMPILING_EDITOR

void ProjectData::copyDirectory(QString dirLocation, QString newLocation)
{
	QFileInfoList fileList;
	QDir dir(dirLocation);
	QDir newDir(newLocation);
	fileList = dir.entryInfoList(QDir::Dirs|QDir::Files|QDir::Readable|QDir::NoDotAndDotDot);
	for (int i = 0; i < fileList.size(); ++i)
	{
		if (fileList[i].isDir())
		{
			newDir.mkdir(fileList[i].fileName());
			copyDirectory(dirLocation + QString("/") + fileList[i].fileName(), newLocation + QString("/") + fileList[i].fileName());
		}
		else
			copyFile(dirLocation + QString("/") + fileList[i].fileName(), newLocation + QString("/") + fileList[i].fileName());
	}
}

void ProjectData::copyFile(QString fileLocation, QString newLocation)
{
	FILE *file1;
	FILE *file2;
	size_t bytesRead;
	char temp_string[2048];
	file1 = fopen(fileLocation.toUtf8().data(), "rb");
	if (file1 == NULL)
	{
		QMessageBox::critical(0, "Critical Error", QString("Failed to open file \"%1\" in read-only mode.").arg(fileLocation));
		return;
	}
	file2 = fopen(newLocation.toUtf8().data(), "wb");
	if (file2 == NULL)
	{
		QMessageBox::critical(0, "Critical Error", QString("Failed to open file \"%1\" in write-only mode.").arg(newLocation));
		return;
	}
	while (!feof(file1))
	{
		bytesRead = fread(temp_string, 1, sizeof(temp_string), file1);
		fwrite(temp_string, 1, bytesRead, file2);
	}
	fclose(file1);
	fclose(file2);
}

void ProjectData::deleteDirectory(QString dirLocation)
{
	QDir dir = QDir(dirLocation);
	QFileInfoList infoList = dir.entryInfoList(QDir::AllEntries|QDir::NoDotAndDotDot|QDir::Hidden|QDir::System);
	for (int i = 0; i < infoList.size(); ++i)
	{
		if (infoList[i].isDir())
		{
			deleteDirectory(infoList[i].absoluteFilePath());
			dir.rmdir(infoList[i].fileName());
		}
		else
			dir.remove(infoList[i].fileName());
	}
	infoList.clear();
	dir.cdUp();
	dir.rmdir(QFileInfo(dirLocation).fileName());
}

bool ProjectData::convertProjectToLatestVersion()
{
	XMLNode ProjectDataNode = XMLNode::openFileHelper((projectLocation + QString("/projectdata.xml")).toLocal8Bit().data(), "openrpgmaker-projectdata");
	ConvertProjectDialog *convertDialog;
	ProcessLogDialog *log;
	QDir dir = QDir(projectsdir);
	int projectDataFileFormatVersion = 0;
	if (ProjectDataNode.isAttributeSet("fileFormatVersion") == TRUE)
		projectDataFileFormatVersion = QString(ProjectDataNode.getAttribute("fileFormatVersion")).toInt();
	if (projectDataFileFormatVersion < FILE_FORMAT_VERSION)
	{
		convertDialog = new ConvertProjectDialog(projectDataFileFormatVersion);
		if (!convertDialog->exec())
			return false;
		if (!dir.mkdir(directoryName + "_backup"))
		{
			QMessageBox::critical(0, "Error", QString("Failed to make directory: \"%1\"").arg(dir.absoluteFilePath(directoryName + "_backup")));
			return false;
		}
		copyDirectory(projectLocation, projectLocation + "_backup");
		log = new ProcessLogDialog(homePath + "/projectUpgradeLog.txt");
		log->show();
		for (int i = projectDataFileFormatVersion; i < FILE_FORMAT_VERSION; ++i)
		{
			QCoreApplication::processEvents();
			log->setCurrentTask(QString("Converting from file format version %1 to %2.").arg(i).arg(i + 1));
			if (i == 0)
				convertProjectFromVersion0to1(log);
			else if (i == 1)
				convertProjectFromVersion1to2(log);
			else if (i == 2)
				convertProjectFromVersion2to3(log);
			else if (i == 3)
				convertProjectFromVersion3to4(log);
		}
		log->close();
		delete log;
		ProjectDataNode.updateAttribute(QString::number(FILE_FORMAT_VERSION).toUtf8().data(), NULL, "fileFormatVersion");
		ProjectDataNode.writeToFile((projectLocation + QString("/projectdata.xml")).toLocal8Bit().data());
	}
	else if (projectDataFileFormatVersion > FILE_FORMAT_VERSION)
	{
		QMessageBox::critical(0, "Error", QString("The current project was made with the file format version %1.\nThis version of Open RPG Maker only supports up to version %2.").arg(projectDataFileFormatVersion).arg(FILE_FORMAT_VERSION));
		return false;
	}
	return true;
}

void ProjectData::convertProjectFromVersion0to1(ProcessLogDialog *log)
{
	/* Changes made
	system.xml
		For each child node in <startLocations>
			add attribute <layer="1">
		Delete <menucommands> node from <openrpgmaker-system>
		Delete attribute "spriteShadowLocation" from <systemGraphics> node
		Add attribute <grappleArmLocation=""> to <systemGraphics> node
		Add attribute <grappleHookLocation=""> to <systemGraphics> node
		Add <phases> child node to <openrpgmaker-system>
		Add 32 <phase> child nodes to <phases>
		For each <phase> child node add "name" attribute
			Phase1 name: "Player Passability"
			Phase2 name: "Jumping Phase 1"
			Phase3 name: "Grabber Phase 1"
			Phase4 name: "Grabber Phase 2"
			Phase5 name: "Move Down (Start)"
			Phase6 name: "Move Down (End)"
			Phase7 name: "Move Up (Start)"
			Phase8 name: "Move Up (End)"
			Rest of phase's names: "Phase %d", phaseID + 1
	map*.xml
		Add attribute <borderTile="0000"> to top node
		Add attribute <name="Lower Layer"> to <lowerlayer> node
		Add attribute <name="Hero Layer"> to <herolayer> node
		Add attribute <name="Upper Layer"> to <upperlayer> node
		If attribute "wrapping" inside top node is
		  "none"
				Add attribute <horizontalMode="lockToEdge"> to top node
				Add attribute <verticalMode="lockToEdge"> to top node
			"horizontal"
				Add attribute <horizontalMode="wrapAround"> to top node
				Add attribute <verticalMode="lockToEdge"> to top node
			"vertical"
				Add attribute <horizontalMode="lockToEdge"> to top node
				Add attribute <verticalMode="wrapAround"> to top node
			"both"
				Add attribute <horizontalMode="wrapAround"> to top node
				Add attribute <verticalMode="wrapAround"> to top node
		Delete attribute "wrapping" from top node
		Rename <lowerlayer> node to <layer>
		Rename <herolayer> node to <layer>
		Rename <upperlayer> node to <layer>
		Rename <eventlayer> node to <mapEvents>
		For each <layer>
			iterate over each tile coordinate and set it to "00000001SSTTFF"
				replacing "SS" with the tilesetID
				replacing "TT" with the tileID
				replacing "FF" with the tileFlags
		For each <mapEvent> node inside the <mapEvents> node
			For each <page> node inside the <mapEvent> node
				add attribute <layer="1">
				rename attribute "eventLayer" to "relationToHero"
				if "relationToHero" is "hero" chage value to "besideHero"
				for each <event> node and child <event> nodes
					add layer information to any events specifying map location
	tileset*.xml
		change attribute <passability> to <phasePassability="00000000">
	terrain*.xml
		remove <vehicleinteractions> node from <openrpgmaker-terrain> node
	vehicle*.xml
		add attribute <passingPhase="0"> to <openrpgmaker-vehicle> node
		add attribute <landingPhase="0"> to <openrpgmaker-vehicle> node
	battleanimation*.xml
		add attribute <frameRate="12"> to <openrpgmaker-battleanimation> node
	globalanimation*.xml
		add attribute <frameRate="12"> to <openrpgmaker-globalanimation> node
	*/
	XMLNode Node1, Node2, Node3, Node4;
	QDir folder = QDir(projectLocation + "/maps");
	QList<XMLNode> nodeList;
	QStringList files = folder.entryList(QStringList() << "*.xml");
	QStringList stringList;
	QString tempString;
	QString eventType;
	int mapWidth, mapHeight;
	log->setCurrentTask("Updating /system.xml");
	Node1 = XMLNode::openFileHelper(QString("%1/system.xml").arg(projectLocation).toLocal8Bit().data(), "openrpgmaker-system");
	Node2 = Node1.getChildNode("startLocations");
	for (int i = 0; i < Node2.nChildNode(); ++i)
	{
		Node3 = Node2.getChildNode(i);
		Node3.addAttribute("layer", "1");
	}
	QCoreApplication::processEvents();
	Node2 = Node1.getChildNode("menucommands");
	Node2.deleteNodeContent();
	Node2 = Node1.getChildNode("systemGraphics");
	Node2.deleteAttribute("spriteShadowLocation");
	Node2.deleteAttribute("fontShadowLocation");
	Node2.deleteAttribute("defaultColor");
	Node2.addAttribute("grappleArmLocation", "");
	Node2.addAttribute("grappleHookLocation", "");
	Node2.addAttribute("fontShadowColor", "000000");
	Node2.addAttribute("defaultTopColor", "FFFFFF");
	Node2.addAttribute("defaultBottomColor", "FFFFFF");
	Node2 = Node1.addChild("phases");
	QCoreApplication::processEvents();
	for (int i = 0; i < 32; ++i)
		stringList.append(QString("Phase %1").arg(i + 1));
	QCoreApplication::processEvents();
	stringList[0] = "Player Passability";
	stringList[1] = "Jumping Phase";
	stringList[2] = "Grapple Pass";
	stringList[3] = "Grapple Hook";
	stringList[4] = "Move Down (Start)";
	stringList[5] = "Move Down (End)";
	stringList[6] = "Move Up (Start)";
	stringList[7] = "Move Up (End)";
	QCoreApplication::processEvents();
	for (int i = 0; i < 32; ++i)
	{
		Node3 = Node2.addChild("phase");
		Node3.addAttribute("name", stringList[i].toUtf8().data());
	}
	QCoreApplication::processEvents();
	Node1.writeToFile(QString("%1/system.xml").arg(projectLocation).toLocal8Bit().data());
	log->setCurrentTask("Updating Maps");
	for (int i = 0; i < files.size(); ++i)
	{
		QCoreApplication::processEvents();
		log->addLogMessage(QString("Updating map: %1").arg(files[i]));
		Node1 = XMLNode::openFileHelper(QString("%1/maps/%2").arg(projectLocation).arg(files[i]).toLocal8Bit().data(), "openrpgmaker-map");
		Node1.addAttribute("borderTile", "0000");
		mapWidth = QString(Node1.getAttribute("width")).toInt();
		mapHeight = QString(Node1.getAttribute("height")).toInt();
		if (Node1.isAttributeSet("wrapping") == TRUE)
		{
			tempString = Node1.getAttribute("wrapping");
			if (tempString == "none")
			{
				Node1.addAttribute("horizontalMode", "lockToEdge");
				Node1.addAttribute("verticalMode", "lockToEdge");
			}
			else if (tempString == "horizontal")
			{
				Node1.addAttribute("horizontalMode", "wrapAround");
				Node1.addAttribute("verticalMode", "lockToEdge");
			}
			else if (tempString == "vertical")
			{
				Node1.addAttribute("horizontalMode", "lockToEdge");
				Node1.addAttribute("verticalMode", "wrapAround");
			}
			else if (tempString == "both")
			{
				Node1.addAttribute("horizontalMode", "wrapAround");
				Node1.addAttribute("verticalMode", "wrapAround");
			}
		}
		else
		{
			Node1.addAttribute("horizontalMode", "lockToEdge");
			Node1.addAttribute("verticalMode", "lockToEdge");
		}
		Node1.deleteAttribute("wrapping");
		Node2 = Node1.getChildNode("eventLayer");
		Node2.updateName("mapEvents");
		for (int j = 0; j < Node2.nChildNode("mapEvent"); ++j)
		{
			QCoreApplication::processEvents();
			Node3 = Node2.getChildNode("mapEvent", j);
			for (int k = 0; k < Node3.nChildNode("page"); ++k)
			{
				QCoreApplication::processEvents();
				Node4 = Node3.getChildNode("page", k);
				tempString = Node4.getAttribute("eventLayer");
				if (tempString == "belowHero")
					Node4.updateAttribute("belowHero", "relationToHero", "eventLayer");
				else if (tempString == "hero")
					Node4.updateAttribute("besideHero", "relationToHero", "eventLayer");
				else
					Node4.updateAttribute("aboveHero", "relationToHero", "eventLayer");
				Node4.addAttribute("layer", "1");
				for (int l = 0; l < Node4.nChildNode("event"); ++l)
					nodeList.append(Node4.getChildNode("event", l));
				while (!nodeList.isEmpty())
				{
					QCoreApplication::processEvents();
					Node4 = nodeList.takeFirst();
					eventType = Node4.getAttribute("type");
					if (eventType == "changeEventLocation")
						Node4.addAttribute("layer", "1");
					else if (eventType == "memorizePosition")
						Node4.addAttribute("layerVariableID", "0");
					else if (eventType == "storeEventID")
						Node4.addAttribute("layer", "1");
					else if (eventType == "storeTerrainID")
						Node4.addAttribute("layer", "1");
					else if (eventType == "swapTile")
					{
						if (QString(Node4.getAttribute("layer")) == "lower")
							Node4.updateAttribute("0", NULL, "layer");
						else if (QString(Node4.getAttribute("layer")) == "hero")
							Node4.updateAttribute("1", NULL, "layer");
						else
							Node4.updateAttribute("2", NULL, "layer");
					}
					else if (eventType == "teleport")
						Node4.addAttribute("layer", "1");
					for (int l = 0; l < Node4.nChildNode("event"); ++l)
					{
						QCoreApplication::processEvents();
						nodeList.append(Node4.getChildNode("event", l));
					}
				}
			}
		}
		QCoreApplication::processEvents();
		Node2 = Node1.getChildNode("lowerlayer");
		Node2.addAttribute("name", "Lower Layer");
		Node2.updateName("layer");
		stringList = QString(Node2.getText()).split(QRegExp("\\s"), QString::SkipEmptyParts);
		Node2.deleteText();
		for (int y = 0; y < mapHeight; ++y)
		{
			QCoreApplication::processEvents();
			tempString = "";
			for (int x = 0; x < mapWidth; ++x)
			{
				QCoreApplication::processEvents();
				if (x != 0)
					tempString += " ";
				tempString += "00000001";
				tempString += stringList[y * mapWidth + x];
			}
			Node2.addText(tempString.toUtf8().data());
		}
		QCoreApplication::processEvents();
		Node2 = Node1.getChildNode("herolayer");
		Node2.addAttribute("name", "Hero Layer");
		Node2.updateName("layer");
		stringList = QString(Node2.getText()).split(QRegExp("\\s"), QString::SkipEmptyParts);
		Node2.deleteText();
		for (int y = 0; y < mapHeight; ++y)
		{
			QCoreApplication::processEvents();
			tempString = "";
			for (int x = 0; x < mapWidth; ++x)
			{
				QCoreApplication::processEvents();
				if (x != 0)
					tempString += " ";
				tempString += "00000001";
				tempString += stringList[y * mapWidth + x];
			}
			Node2.addText(tempString.toUtf8().data());
		}
		QCoreApplication::processEvents();
		Node2 = Node1.getChildNode("upperlayer");
		Node2.addAttribute("name", "Upper Layer");
		Node2.updateName("layer");
		stringList = QString(Node2.getText()).split(QRegExp("\\s"), QString::SkipEmptyParts);
		Node2.deleteText();
		for (int y = 0; y < mapHeight; ++y)
		{
			QCoreApplication::processEvents();
			tempString = "";
			for (int x = 0; x < mapWidth; ++x)
			{
				QCoreApplication::processEvents();
				if (x != 0)
					tempString += " ";
				tempString += "00000001";
				tempString += stringList[y * mapWidth + x];
			}
			Node2.addText(tempString.toUtf8().data());
		}
		QCoreApplication::processEvents();
		Node1.writeToFile(QString("%1/maps/%2").arg(projectLocation).arg(files[i]).toLocal8Bit().data());
	}
	log->setCurrentTask("Updating Tilesets");
	folder = QDir(projectLocation + "/tilesets");
	files = folder.entryList(QStringList() << "*.xml");
	QCoreApplication::processEvents();
	for (int i = 0; i < files.size(); ++i)
	{
		log->addLogMessage(QString("Updating tileset: %1").arg(files[i]));
		QCoreApplication::processEvents();
		Node1 = XMLNode::openFileHelper(QString("%1/tilesets/%2").arg(projectLocation).arg(files[i]).toLocal8Bit().data(), "openrpgmaker-tileset");
		for (int j = 0; j < Node1.nChildNode("tile"); ++j)
		{
			QCoreApplication::processEvents();
			Node2 = Node1.getChildNode("tile", j);
			Node2.updateAttribute("00000000", "phasePassability", "passability");
		}
		QCoreApplication::processEvents();
		Node1.writeToFile(QString("%1/tilesets/%2").arg(projectLocation).arg(files[i]).toLocal8Bit().data());
	}
	QCoreApplication::processEvents();
	log->setCurrentTask("Updating Terrain");
	folder = QDir(projectLocation + "/terrain");
	files = folder.entryList(QStringList() << "*.xml");
	for (int i = 0; i < files.size(); ++i)
	{
		log->addLogMessage(QString("Updating terrain: %1").arg(files[i]));
		QCoreApplication::processEvents();
		Node1 = XMLNode::openFileHelper(QString("%1/terrain/%2").arg(projectLocation).arg(files[i]).toLocal8Bit().data(), "openrpgmaker-terrain");
		Node2 = Node1.getChildNode("vehicleinteractions");
		Node2.deleteNodeContent();
		Node1.writeToFile(QString("%1/terrain/%2").arg(projectLocation).arg(files[i]).toLocal8Bit().data());
	}
	log->setCurrentTask("Updating Vehicles");
	QCoreApplication::processEvents();
	folder = QDir(projectLocation + "/vehicles");
	files = folder.entryList(QStringList() << "*.xml");
	for (int i = 0; i < files.size(); ++i)
	{
		log->addLogMessage(QString("Updating vehicle: %1").arg(files[i]));
		QCoreApplication::processEvents();
		Node1 = XMLNode::openFileHelper(QString("%1/vehicles/%2").arg(projectLocation).arg(files[i]).toLocal8Bit().data(), "openrpgmaker-vehicle");
		Node1.addAttribute("passingPhase", "0");
		Node1.addAttribute("landingPhase", "0");
		Node1.writeToFile(QString("%1/vehicles/%2").arg(projectLocation).arg(files[i]).toLocal8Bit().data());
	}
	log->setCurrentTask("Updating Battle Animations");
	folder = QDir(projectLocation + "/battleanimations");
	files = folder.entryList(QStringList() << "*.xml");
	for (int i = 0; i < files.size(); ++i)
	{
		log->addLogMessage(QString("Updating battle animation: %1").arg(files[i]));
		QCoreApplication::processEvents();
		Node1 = XMLNode::openFileHelper(QString("%1/battleanimations/%2").arg(projectLocation).arg(files[i]).toLocal8Bit().data(), "openrpgmaker-battleanimation");
		Node1.addAttribute("frameRate", "12");
		Node1.writeToFile(QString("%1/battleanimations/%2").arg(projectLocation).arg(files[i]).toLocal8Bit().data());
	}
	log->setCurrentTask("Updating Global Animations");
	folder = QDir(projectLocation + "/globalanimations");
	files = folder.entryList(QStringList() << "*.xml");
	for (int i = 0; i < files.size(); ++i)
	{
		log->addLogMessage(QString("Updating global animation: %1").arg(files[i]));
		QCoreApplication::processEvents();
		Node1 = XMLNode::openFileHelper(QString("%1/globalanimations/%2").arg(projectLocation).arg(files[i]).toLocal8Bit().data(), "openrpgmaker-globalanimation");
		Node1.addAttribute("frameRate", "12");
		Node1.writeToFile(QString("%1/globalanimations/%2").arg(projectLocation).arg(files[i]).toLocal8Bit().data());
	}
}

void ProjectData::convertProjectFromVersion1to2(ProcessLogDialog *log)
{
	/* Changes made
	tilesets
		For each <tile> node
			Add attribute <fps="8">
			if tile type is animated-auto tile
				Add attribute <pingPong="true">
			else
				Add attribute <pingPong="false">
	images
		All images need to be converted to 32bit with full alpha support
		All animated single tiles need to be re-arranged into a single row of tiles
		All map sprites need to be separated into individual images as follows
			idle.png
			walking.png
	Create folder "/charactersprites"
	for each "battlesprite*.xml" file in "/battlesprites"
		change top node to "openrpgmaker-charactersprite"
		create child node "mapSprite" inside top node
		create child node "battleSprite" inside top node
		add 32 "pose" nodes with default values to "mapSprite" node
		move all "pose" nodes under top level node to "battleSprite" node
	replace each reference to a mapSprite's original image with an id of 0
	classes
		change attribute "battleSprite" in <openrpgmaker-class> node to "sprite"
		add attribute <spriteOpacity="100"> to <openrpgmaker-class> node
	*/
	XMLNode Node1, Node2, Node3;
	QList<XMLNode> nodeList;
	QDir dir;
	QFileInfoList infoList;
	QFileInfo info;
	QString tempString;
	QStringList stringList;
	QStringList mapSprites;
	QImage oldImage, newImage;
	QRgb pixel, tcolor = qRgb(255, 0, 255);
	QPainter painter;
	//Convert tileset xml files
	log->setCurrentTask("Updating Tilesets");
	dir = QDir(projectLocation + "/tilesets");
	stringList = dir.entryList(QStringList() << "*.xml");
	for (int i = 0; i < stringList.size(); ++i)
	{
		log->addLogMessage(QString("Updating tileset: %1").arg(stringList[i]));
		QCoreApplication::processEvents();
		tempString = QString("%1/tilesets/%2").arg(projectLocation).arg(stringList[i]);
		Node1 = XMLNode::openFileHelper(tempString.toLocal8Bit().data(), "openrpgmaker-tileset");
		for (int j = 0; j < Node1.nChildNode("tile"); ++j)
		{
			QCoreApplication::processEvents();
			Node2 = Node1.getChildNode("tile", j);
			Node2.addAttribute("fps", "8");
			oldImage = QImage(getAbsoluteResourcePath(Node2.getAttribute("location")));
			if (oldImage.width() == 96 && oldImage.height() == 160)
				Node2.addAttribute("pingPong", "true");
			else
				Node2.addAttribute("pingPong", "false");
		}
		QCoreApplication::processEvents();
		Node1.writeToFile(tempString.toLocal8Bit().data());
	}
	log->setCurrentTask("Moving /battlecharacters to /charactersprites");
	dir = QDir(projectLocation);
	dir.mkdir("charactersprites");
	copyDirectory(projectLocation + "/battlecharacters", projectLocation + "/charactersprites");
	deleteDirectory(projectLocation + "/battlecharacters");
	dir = QDir(projectLocation + "/charactersprites");
	stringList = dir.entryList(QStringList() << "*.xml");
	log->setCurrentTask("Updating Character Sprites");
	for (int i = 0; i < stringList.size(); ++i)
	{
		log->addLogMessage(QString("Updating character sprite: %1").arg(stringList[i]));
		QCoreApplication::processEvents();
		int id = stringList[i].mid(15, 4).toInt();
		tempString = QString("%1/charactersprites/%2").arg(projectLocation).arg(stringList[i]);
		Node1 = XMLNode::openFileHelper(tempString.toLocal8Bit().data(), "openrpgmaker-battlecharacter");
		Node1.updateName("openrpgmaker-charactersprite");
		Node2 = Node1.addChild("mapSprite");
		for (int j = 0; j < 32; ++j)
		{
			QCoreApplication::processEvents();
			Node3 = Node2.addChild("pose");
			Node3.addAttribute("name", "");
			Node3.addAttribute("location", "");
			Node3.addAttribute("numLoops", "1");
			Node3.addAttribute("pingPong", "false");
		}
		Node3 = Node2.getChildNode("pose", CharacterSprite::MAPPOSE_IDLE);
		Node3.updateAttribute("Idle", NULL, "name");
		Node3 = Node2.getChildNode("pose", CharacterSprite::MAPPOSE_WALKING);
		Node3.updateAttribute("Walking", NULL, "name");
		Node3 = Node2.getChildNode("pose", CharacterSprite::MAPPOSE_JUMPING);
		Node3.updateAttribute("Jumping", NULL, "name");
		Node3 = Node2.getChildNode("pose", CharacterSprite::MAPPOSE_GRAPPLING);
		Node3.updateAttribute("Grappling", NULL, "name");
		Node2 = Node1.addChild("battleSprite");
		for (int j = 0; j < 32; ++j)
		{
			QCoreApplication::processEvents();
			Node3 = Node1.getChildNode("pose");
			Node2.addChild(Node3);
		}
		Node1.writeToFile(tempString.toLocal8Bit().data());
		dir.rename(stringList[i], QString("charactersprite%1.xml").arg(id, 4, 10, QChar('0')));
	}
	//Convert all images to 32bit with full alpha support
	log->setCurrentTask("Converting all images to 32bit with full alpha support");
	dir = QDir(projectLocation + "/images");
	stringList = QStringList() << "*.png" << "*.bmp";
	infoList = dir.entryInfoList(stringList, QDir::AllDirs|QDir::Files|QDir::NoSymLinks|QDir::NoDotAndDotDot|QDir::Readable|QDir::Writable, QDir::Name|QDir::DirsFirst|QDir::IgnoreCase);
	while (!infoList.empty())
	{
		QCoreApplication::processEvents();
		info = infoList.takeFirst();
		if (info.isDir() && getRelativeResourcePath(info.absoluteFilePath()) != "local::/images/fonts")
		{
			dir = QDir(info.absoluteFilePath());
			infoList.append(dir.entryInfoList(stringList, QDir::AllDirs|QDir::Files|QDir::NoSymLinks|QDir::NoDotAndDotDot|QDir::Readable|QDir::Writable, QDir::Name|QDir::DirsFirst|QDir::IgnoreCase));
		}
		else
		{
			log->addLogMessage(QString("Processing image: %1").arg(info.absoluteFilePath().replace(projectLocation, "")));
			oldImage = QImage(info.absoluteFilePath());
			newImage = QImage(oldImage.width(), oldImage.height(), QImage::Format_ARGB32);
			for (int y = 0; y < oldImage.height(); ++y)
			{
				QCoreApplication::processEvents();
				for (int x = 0; x < oldImage.width(); ++x)
				{
					QCoreApplication::processEvents();
					pixel = oldImage.pixel(x, y);
					if (pixel == tcolor)
						pixel = qRgba(0, 0, 0, 0);
					newImage.setPixel(x, y, pixel);
				}
			}
			tempString = info.absolutePath() + "/" + info.baseName() + ".png";
			newImage.save(tempString);
			if (info.suffix() == "bmp")
			{
				dir = info.absolutePath();
				dir.remove(info.fileName());
			}
		}
	}
	//Re-arrange all animated-single tiles to a single row
	log->setCurrentTask("Rearranging all animated-single tiles to a single row");
	dir = QDir(projectLocation + "/images/tiles");
	stringList = QStringList() << ".png";
	infoList = dir.entryInfoList(stringList, QDir::AllDirs|QDir::Files|QDir::NoSymLinks|QDir::NoDotAndDotDot|QDir::Readable|QDir::Writable, QDir::Name|QDir::DirsFirst|QDir::IgnoreCase);
	while (!infoList.empty())
	{
		QCoreApplication::processEvents();
		info = infoList.takeFirst();
		if (info.isDir())
		{
			dir = QDir(info.absoluteFilePath());
			infoList.append(dir.entryInfoList(stringList, QDir::AllDirs|QDir::Files|QDir::NoSymLinks|QDir::NoDotAndDotDot|QDir::Readable|QDir::Writable, QDir::Name|QDir::DirsFirst|QDir::IgnoreCase));
		}
		else
		{
			log->addLogMessage(QString("Processing image: %1").arg(info.absoluteFilePath().replace(projectLocation, "")));
			oldImage = QImage(info.absoluteFilePath());
			if (oldImage.width() == 64 && oldImage.height() == 64)
			{
				newImage = QImage(128, 32, QImage::Format_ARGB32);
				newImage.fill(Qt::transparent);
				painter.begin(&newImage);
				painter.drawImage(0, 0, oldImage, 0, 0, 32, 32);
				painter.drawImage(32, 0, oldImage, 32, 0, 32, 32);
				painter.drawImage(64, 0, oldImage, 0, 32, 32, 32);
				painter.drawImage(96, 0, oldImage, 32, 32, 32, 32);
				painter.end();
				newImage.save(info.absoluteFilePath());
			}
		}
	}
	//Separate map sprites into individual images
	log->setCurrentTask("Separating map sprites into individual images");
	dir = QDir(projectLocation);
	dir.mkdir("mapsprites");
	dir = QDir(projectLocation + "/images/sprites");
	stringList = QStringList() << "*.png";
	infoList = dir.entryInfoList(stringList, QDir::AllDirs|QDir::Files|QDir::NoSymLinks|QDir::NoDotAndDotDot|QDir::Readable|QDir::Writable, QDir::Name|QDir::DirsFirst|QDir::IgnoreCase);
	while (!infoList.empty())
	{
		QCoreApplication::processEvents();
		info = infoList.takeFirst();
		if (info.isDir())
		{
			dir = QDir(info.absoluteFilePath());
			infoList.append(dir.entryInfoList(stringList, QDir::AllDirs|QDir::Files|QDir::NoSymLinks|QDir::NoDotAndDotDot|QDir::Readable|QDir::Writable, QDir::Name|QDir::DirsFirst|QDir::IgnoreCase));
		}
		else
		{
			log->addLogMessage(QString("Processing image: %1").arg(info.absoluteFilePath().replace(projectLocation, "")));
			//Separate map sprite's image file
			mapSprites.append(ProjectData::getRelativeResourcePath(info.absoluteFilePath()));
			oldImage = QImage(info.absoluteFilePath());
			dir = QDir(info.absolutePath());
			dir.mkdir(info.baseName());
			oldImage.copy(48, 0, 48, 256).save(info.absolutePath() + "/" + info.baseName() + "/idle.png");
			oldImage.copy(0, 0, 144, 256).save(info.absolutePath() + "/" + info.baseName() + "/walking.png");
			dir.remove(info.fileName());
		}
	}
	//Replace each reference to a mapSprite's original image with an id of 0
	log->setCurrentTask("Replacing map sprite references with an id of 0");
	dir = QDir(projectLocation + "/characters");
	infoList = dir.entryInfoList(QStringList() << "*.xml");
	for (int i = 0; i < infoList.size(); ++i)
	{
		info = infoList[i];
		log->addLogMessage(QString("Updating: %1").arg(info.absoluteFilePath().replace(projectLocation, "")));
		QCoreApplication::processEvents();
		Node1 = XMLNode::openFileHelper(info.absoluteFilePath().toLocal8Bit().data(), "openrpgmaker-character");
		Node2 = Node1.getChildNode("graphics");
		Node2.deleteAttribute("sprite");
		Node2.updateAttribute(Node2.getAttribute("battleSprite"), "sprite", "battleSprite");
		Node1.writeToFile(info.absoluteFilePath().toLocal8Bit().data());
	}
	log->setCurrentTask("Updating classes");
	dir = QDir(projectLocation + "/classes");
	infoList = dir.entryInfoList(QStringList() << "*.xml");
	for (int i = 0; i < infoList.size(); ++i)
	{
		info = infoList[i];
		log->addLogMessage(QString("Updating: %1").arg(info.absoluteFilePath().replace(projectLocation, "")));
		QCoreApplication::processEvents();
		Node1 = XMLNode::openFileHelper(info.absoluteFilePath().toLocal8Bit().data(), "openrpgmaker-class");
		Node1.updateAttribute(Node1.getAttribute("battleSprite"), "sprite", "battleSprite");
		Node1.addAttribute("spriteOpacity", "100");
		Node1.writeToFile(info.absoluteFilePath().toLocal8Bit().data());
	}
	infoList.clear();
	dir = QDir(projectLocation + "/commonevents");
	infoList.append(dir.entryInfoList(QStringList() << "*.xml"));
	dir = QDir(projectLocation + "/layouts");
	infoList.append(dir.entryInfoList(QStringList() << "*.xml"));
	dir = QDir(projectLocation + "/maps");
	infoList.append(dir.entryInfoList(QStringList() << "*.xml"));
	QCoreApplication::processEvents();
	for (int i = 0; i < infoList.size(); ++i)
	{
		QCoreApplication::processEvents();
		info = infoList[i];
		log->addLogMessage(QString("Updating: %1").arg(info.absoluteFilePath().replace(projectLocation, "")));
		if (info.fileName().startsWith("commonevent"))
			Node1 = XMLNode::openFileHelper(info.absoluteFilePath().toLocal8Bit().data(), "openrpgmaker-commonevent");
		else if (info.fileName().startsWith("mainmenulayout"))
			Node1 = XMLNode::openFileHelper(info.absoluteFilePath().toLocal8Bit().data(), "openrpgmaker-mainmenulayout");
		else if (info.fileName().startsWith("popupmenulayout"))
			Node1 = XMLNode::openFileHelper(info.absoluteFilePath().toLocal8Bit().data(), "openrpgmaker-popupmenulayout");
		else if (info.fileName().startsWith("map"))
			Node1 = XMLNode::openFileHelper(info.absoluteFilePath().toLocal8Bit().data(), "openrpgmaker-map");
		for (int j = 0; j < Node1.nChildNode(); ++j)
			nodeList.append(Node1.getChildNode(j));
		while (!nodeList.isEmpty())
		{
			bool deleteNode = false;
			QCoreApplication::processEvents();
			Node2 = nodeList.takeFirst();
			if (info.fileName().startsWith("mainmenulayout") || info.fileName().startsWith("popupmenulayout"))
			{
				if (QString(Node2.getName()) == "object")
				{
					if (QString(Node2.getAttribute("type")) == "image")
					{
						if (QString(Node2.getAttribute("imageType")) == "party member" || QString(Node2.getAttribute("imageType")) == "character")
						{
							if (QString(Node2.getAttribute("graphic")) == "map sprite")
								Node2.addAttribute("pose", "0");
						}
						else if (QString(Node2.getAttribute("imageType")) == "class")
							Node2.addAttribute("graphic", "battle sprite");
						else if (QString(Node2.getAttribute("imageType")) == "vehicle")
							Node2.addAttribute("pose", "0");
					}
				}
				else if (QString(Node2.getName()) == "event")
				{
					if (QString(Node2.getAttribute("type")) == "changeImage")
					{
						if (QString(Node2.getAttribute("imageType")) == "party member" || QString(Node2.getAttribute("imageType")) == "character")
						{
							if (QString(Node2.getAttribute("graphic")) == "map sprite")
								Node2.addAttribute("pose", "0");
						}
						else if (QString(Node2.getAttribute("imageType")) == "class")
							Node2.addAttribute("graphic", "battle sprite");
						else if (QString(Node2.getAttribute("imageType")) == "vehicle")
							Node2.addAttribute("pose", "0");
					}
					else if (QString(Node2.getAttribute("type")) == "changeBattleSprite")
					{
						Node2.updateAttribute(Node2.getAttribute("battleSpriteID"), "spriteID", "battleSpriteID");
						Node2.updateAttribute(Node2.getAttribute("battleSpriteReferencedInVariable"), "spriteReferencedInVariable", "battleSpriteReferencedInVariable");
					}
					else if (QString(Node2.getAttribute("type")) == "changeSpriteGraphic")
						deleteNode = true;
				}
			}
			else if (info.fileName().startsWith("map"))
			{
				if (QString(Node2.getName()) == "page")
				{
					tempString = Node2.getAttribute("spriteLocation");
					if (tempString.startsWith("sprite:"))
						Node2.updateAttribute("sprite:0", NULL, "spriteLocation");
				}
				else if (QString(Node2.getName()) == "event")
				{
					if (QString(Node2.getAttribute("type")) == "changeBattleSprite")
					{
						Node2.updateAttribute(Node2.getAttribute("battleSpriteID"), "spriteID", "battleSpriteID");
						Node2.updateAttribute(Node2.getAttribute("battleSpriteReferencedInVariable"), "spriteReferencedInVariable", "battleSpriteReferencedInVariable");
					}
					else if (QString(Node2.getAttribute("type")) == "changeSpriteGraphic")
						deleteNode = true;
				}
			}
			if (deleteNode)
				Node2.deleteNodeContent();
			else
			{
				for (int j = 0; j < Node2.nAttribute(); ++j)
				{
					QCoreApplication::processEvents();
					tempString = Node2.getAttributeValue(j);
					if (mapSprites.contains(tempString))
						Node2.updateAttribute("0", "sprite", Node2.getAttributeName(i));
				}
				for (int j = 0; j < Node2.nChildNode(); ++j)
				{
					QCoreApplication::processEvents();
					nodeList.append(Node2.getChildNode(j));
				}
			}
		}
		Node1.writeToFile(info.absoluteFilePath().toLocal8Bit().data());
	}
	dir = QDir(projectLocation + "/vehicles");
	infoList = dir.entryInfoList(QStringList() << "*.xml");
	for (int i = 0; i < infoList.size(); ++i)
	{
		info = infoList[i];
		log->addLogMessage(QString("Updating: %1").arg(info.absoluteFilePath().replace(projectLocation, "")));
		QCoreApplication::processEvents();
		Node1 = XMLNode::openFileHelper(info.absoluteFilePath().toLocal8Bit().data(), "openrpgmaker-vehicle");
		tempString = Node1.getAttribute("spritelocation");
		Node1.updateAttribute("0", "sprite", "spritelocation");
		Node1.writeToFile(info.absoluteFilePath().toLocal8Bit().data());
	}
}

void ProjectData::convertProjectFromVersion2to3(ProcessLogDialog *log)
{
	/* Changes made
	system.xml
		remove "defaultColor" attribute from <systemGraphics> node
		remove "fontShadowLocation" attribute from <systemGraphics> node
		add attribute <defaultFontShadowColor="#000000"> to <systemGraphics> node
		add attribute <defaultFontEnabledColor="#F0FAFF-#3899F6"> to <systemGraphics> node
		add attribute <defaultFontDisabledColor="#C9C9CD-#1D1D1E"> to <systemGraphics> node
	for each font images
		upgrade it to 32bit image
		replace magenta(#FF00FF) pixels with transparent pixels
	for each font xml file
		add attribute <characterHeight="32"> to <openrpgmaker-font> node
		add attribute <dropShadowXOffset="2"> to <openrpgmaker-font> node
		add attribute <dropShadowYOffset="2"> to <openrpgmaker-font> node
	for each menu layout xml file
		for each <object> node that has attribute <type="text">
			load the "enabledColor" gradient
			change "enabledColor" attribute to "#R1G1B1-#R2G2B2"
				R1G1B1 is the RGB color of the top-left pixel in the gradient image
				R2G2B2 is the RGB color of the bottom-left pixel in the gradient image
			load the "disabledColor" gradient
			change "disabledColor" attribute to "#R1G1B1-#R2G2B2"
				R1G1B1 is the RGB color of the top-left pixel in the gradient image
				R2G2B2 is the RGB color of the bottom-left pixel in the gradient image
			add attribute <shadowColor="#000000">
	remove folder "/images/gradients" and all files and sub-folders
	*/
	XMLNode Node1, Node2;
	QImage oldImage, newImage;
	QString tempString;
	QStringList stringList;
	QFileInfoList infoList;
	QFileInfo info;
	QList<XMLNode> nodeList;
	QDir dir;
	QRgb pixel, tcolor = qRgb(255, 0, 255);
	//Convert system.xml
	Node1 = XMLNode::openFileHelper(QString("%1/system.xml").arg(projectLocation).toLocal8Bit().data(), "openrpgmaker-system");
	Node2 = Node1.getChildNode("systemGraphics");
	Node2.deleteAttribute("defaultColor");
	Node2.deleteAttribute("fontShadowLocation");
	Node2.addAttribute("defaultFontShadowColor", "#000000");
	Node2.addAttribute("defaultFontEnabledColor", "#F0FAFF-#3899F6");
	Node2.addAttribute("defaultFontDisabledColor", "#C9C9CD-#1D1D1E");
	Node1.writeToFile(QString("%1/system.xml").arg(projectLocation).toLocal8Bit().data());
	//Convert font images
	log->setCurrentTask("Converting font images");
	dir = QDir(projectLocation + "/images/fonts");
	infoList = dir.entryInfoList(QStringList() << "*.png", QDir::AllDirs|QDir::Files|QDir::NoSymLinks|QDir::NoDotAndDotDot|QDir::Readable|QDir::Writable, QDir::Name|QDir::DirsFirst|QDir::IgnoreCase);
	while (!infoList.empty())
	{
		QCoreApplication::processEvents();
		info = infoList.takeFirst();
		if (info.isDir())
		{
			dir = QDir(info.absoluteFilePath());
			infoList.append(dir.entryInfoList(QStringList() << "*.png", QDir::AllDirs|QDir::Files|QDir::NoSymLinks|QDir::NoDotAndDotDot|QDir::Readable|QDir::Writable, QDir::Name|QDir::DirsFirst|QDir::IgnoreCase));
		}
		else
		{
			log->addLogMessage(QString("Processing image: %1").arg(info.absoluteFilePath().replace(projectLocation, "")));
			oldImage = QImage(info.absoluteFilePath());
			newImage = QImage(oldImage.width(), oldImage.height(), QImage::Format_ARGB32);
			for (int y = 0; y < oldImage.height(); ++y)
			{
				QCoreApplication::processEvents();
				for (int x = 0; x < oldImage.width(); ++x)
				{
					QCoreApplication::processEvents();
					pixel = oldImage.pixel(x, y);
					if (pixel == tcolor)
						newImage.setPixel(x, y, qRgba(0, 0, 0, 0));
					else
						newImage.setPixel(x, y, qRgba(0, 0, 0, 255));
				}
			}
			newImage.save(info.absoluteFilePath());
		}
	}
	//Convert font xml files
	log->setCurrentTask("Converting Fonts");
	dir = QDir(projectLocation + "/fonts");
	infoList = dir.entryInfoList(QStringList() << "*.xml");
	for (int i = 0; i < infoList.size(); ++i)
	{
		log->addLogMessage(QString("Updating font: %1").arg(infoList[i].fileName()));
		QCoreApplication::processEvents();
		Node1 = XMLNode::openFileHelper(infoList[i].absoluteFilePath().toLocal8Bit().data(), "openrpgmaker-font");
		Node1.addAttribute("characterHeight", "32");
		Node1.addAttribute("dropShadowXOffset", "2");
		Node1.addAttribute("dropShadowYOffset", "2");
		Node1.writeToFile(infoList[i].absoluteFilePath().toLocal8Bit().data());
	}
	//Convert menu layouts
	log->setCurrentTask("Converting Menu Layouts");
	dir = QDir(projectLocation + "/layouts");
	infoList = dir.entryInfoList(QStringList() << "*.xml");
	for (int i = 0; i < infoList.size(); ++i)
	{
		QCoreApplication::processEvents();
		info = infoList[i];
		log->addLogMessage(QString("Updating layout: %1").arg(info.fileName()));
		if (info.fileName().startsWith("mainmenulayout"))
			Node1 = XMLNode::openFileHelper(info.absoluteFilePath().toLocal8Bit().data(), "openrpgmaker-mainmenulayout");
		else if (info.fileName().startsWith("popupmenulayout"))
			Node1 = XMLNode::openFileHelper(info.absoluteFilePath().toLocal8Bit().data(), "openrpgmaker-popupmenulayout");
		for (int j = 0; j < Node1.nChildNode("object"); ++j)
			nodeList.append(Node1.getChildNode("object", j));
		while (!nodeList.isEmpty())
		{
			QCoreApplication::processEvents();
			Node2 = nodeList.takeFirst();
			if (QString(Node2.getAttribute("type")) == "text" || QString(Node2.getAttribute("type")) == "item list")
			{
				oldImage = QImage(getAbsoluteResourcePath(Node2.getAttribute("enabledColor")));
				pixel = oldImage.pixel(0, 0);
				tempString = QString("#%1%2%3-").arg(qRed(pixel), 2, 16, QChar('0')).arg(qGreen(pixel), 2, 16, QChar('0')).arg(qBlue(pixel), 2, 16, QChar('0'));
				pixel = oldImage.pixel(0, oldImage.height() - 1);
				tempString += QString("#%1%2%3").arg(qRed(pixel), 2, 16, QChar('0')).arg(qGreen(pixel), 2, 16, QChar('0')).arg(qBlue(pixel), 2, 16, QChar('0'));
				Node2.updateAttribute(tempString.toUtf8().data(), NULL, "enabledColor");
				oldImage = QImage(getAbsoluteResourcePath(Node2.getAttribute("disabledColor")));
				pixel = oldImage.pixel(0, 0);
				tempString = QString("#%1%2%3-").arg(qRed(pixel), 2, 16, QChar('0')).arg(qGreen(pixel), 2, 16, QChar('0')).arg(qBlue(pixel), 2, 16, QChar('0'));
				pixel = oldImage.pixel(0, oldImage.height() - 1);
				tempString += QString("#%1%2%3").arg(qRed(pixel), 2, 16, QChar('0')).arg(qGreen(pixel), 2, 16, QChar('0')).arg(qBlue(pixel), 2, 16, QChar('0'));
				Node2.updateAttribute(tempString.toUtf8().data(), NULL, "disabledColor");
			}
			for (int j = 0; j < Node2.nChildNode("object"); ++j)
			{
				QCoreApplication::processEvents();
				nodeList.append(Node2.getChildNode("object", j));
			}
		}
		Node1.writeToFile(info.absoluteFilePath().toLocal8Bit().data());
	}
	//Remove "/images/gradients" folder
	log->setCurrentTask("Deleting /images/gradients Folder");
	deleteDirectory(projectLocation + "/images/gradients");
	QCoreApplication::processEvents();
}

void ProjectData::convertProjectFromVersion3to4(ProcessLogDialog *log)
{
	/* Changes made
	for each map xml file
		for each layer
			iterate over every tile and remove the last eight phase passabilities
			then for each phase calculate the directional passability based on neighboring tiles
			then convert the layer data to base64 encoding instead of hexadecimal
			(map data has gotten too complex to continue to use the more human
			readable hexadecimal format, because it would take too much space).
	for each tileset xml file
		for each tile
			remove the last eight phase passabilities from the "phasePassability" attribute
			add the "phaseDirections" attribute with each phase that is on having all four
			directions set.
	for each vehicle xml file
		if the "passingPhase" attribute is greater than 23 set it to 23
		if the "landingPhase" attribute is greater than 23 set it to 23
	*/
	XMLNode Node1, Node2;
	QDir dir;
	QFileInfoList infoList;
	QStringList tileDataStringList;
	size_t inputLength = 18;
	size_t outputLength;
	int mapWidth, mapHeight;
	unsigned char tileDataBytes[18];
	char tileDataBase64[25];
	bool mapWrapHorizontal, mapWrapVertical;
	//Convert map xml files
	log->setCurrentTask("Converting Maps");
	dir = QDir(projectLocation + "/maps");
	infoList = dir.entryInfoList(QStringList() << "*.xml");
	for (int i = 0; i < infoList.size(); ++i)
	{
		log->addLogMessage(QString("Updating map: %1").arg(infoList[i].fileName()));
		QCoreApplication::processEvents();
		Node1 = XMLNode::openFileHelper(infoList[i].absoluteFilePath().toLocal8Bit().data(), "openrpgmaker-map");
		mapWidth = QString(Node1.getAttribute("width")).toInt();
		mapHeight = QString(Node1.getAttribute("height")).toInt();
		mapWrapHorizontal = (QString(Node1.getAttribute("horizontalMode")) == "wrapAround") ? true:false;
		mapWrapVertical = (QString(Node1.getAttribute("verticalMode")) == "wrapAround") ? true:false;
		for (int j = 0; j < Node1.nChildNode("layer"); ++j)
		{
			Map::LayerData layer;
			Node2 = Node1.getChildNode("layer", j);
			tileDataStringList = QString(Node2.getText()).split(QRegExp("\\s"), QString::SkipEmptyParts);
			Node2.deleteText();
			for (int y = 0; y < mapHeight; ++y)
			{
				for (int x = 0; x < mapWidth; ++x)
				{
					Map::TileData tileData;
					unsigned long phasePassability = tileDataStringList[y * mapWidth + x].mid(0, 8).toULong(NULL, 16);
					unsigned long tileInfo = tileDataStringList[y * mapWidth + x].mid(8, 6).toULong(NULL, 16);
					for (int k = 0; k < NUM_PHASES; ++k)
					{
						tileData.phaseDirections[k] = 0x0;
						tileData.phasePassability[k] = ((phasePassability >> k) % 2 == 0) ? false:true;
					}
					tileData.tilesetID = (unsigned char)((tileInfo >> 16) % 256);
					tileData.tileID = (unsigned char)((tileInfo >> 8) % 256);
					tileData.tileFlags = (unsigned char)(tileInfo % 256);
					layer.tileData.append(tileData);
				}
			}
			for (int y = 0; y < mapHeight; ++y)
			{
				QString layerData = "";
				for (int x = 0; x < mapWidth; ++x)
				{
					Map::TileData *tileData = &(layer.tileData[y * mapWidth + x]);
					unsigned long phasePassability = 0x0;
					for (int k = 0; k < NUM_PHASES; ++k)
					{
						phasePassability += ((tileData->phasePassability[k]) ? 1ul:0ul) << k;
						if (tileData->phasePassability[k])
						{
							int x2, y2;
							unsigned char phaseDirections = 0x0;
							x2 = x;
							y2 = y - 1;
							if (x2 < 0 && mapWrapHorizontal)
								x2 += mapWidth;
							if (x2 == mapWidth && mapWrapHorizontal)
								x2 -= mapWidth;
							if (y2 < 0 && mapWrapVertical)
								y2 += mapHeight;
							if (y2 == mapHeight && mapWrapVertical)
								y2 -= mapHeight;
							if (x2 >= 0 && x2 < mapWidth && y2 >= 0 && y2 < mapHeight)
							{
								if (layer.tileData[y2 * mapWidth + x2].phasePassability[k])
									phaseDirections += Map::PASS_UP;
							}
							x2 = x + 1;
							y2 = y;
							if (x2 < 0 && mapWrapHorizontal)
								x2 += mapWidth;
							if (x2 == mapWidth && mapWrapHorizontal)
								x2 -= mapWidth;
							if (y2 < 0 && mapWrapVertical)
								y2 += mapHeight;
							if (y2 == mapHeight && mapWrapVertical)
								y2 -= mapHeight;
							if (x2 >= 0 && x2 < mapWidth && y2 >= 0 && y2 < mapHeight)
							{
								if (layer.tileData[y2 * mapWidth + x2].phasePassability[k])
									phaseDirections += Map::PASS_RIGHT;
							}
							x2 = x;
							y2 = y + 1;
							if (x2 < 0 && mapWrapHorizontal)
								x2 += mapWidth;
							if (x2 == mapWidth && mapWrapHorizontal)
								x2 -= mapWidth;
							if (y2 < 0 && mapWrapVertical)
								y2 += mapHeight;
							if (y2 == mapHeight && mapWrapVertical)
								y2 -= mapHeight;
							if (x2 >= 0 && x2 < mapWidth && y2 >= 0 && y2 < mapHeight)
							{
								if (layer.tileData[y2 * mapWidth + x2].phasePassability[k])
									phaseDirections += Map::PASS_DOWN;
							}
							x2 = x - 1;
							y2 = y;
							if (x2 < 0 && mapWrapHorizontal)
								x2 += mapWidth;
							if (x2 == mapWidth && mapWrapHorizontal)
								x2 -= mapWidth;
							if (y2 < 0 && mapWrapVertical)
								y2 += mapHeight;
							if (y2 == mapHeight && mapWrapVertical)
								y2 -= mapHeight;
							if (x2 >= 0 && x2 < mapWidth && y2 >= 0 && y2 < mapHeight)
							{
								if (layer.tileData[y2 * mapWidth + x2].phasePassability[k])
									phaseDirections += Map::PASS_LEFT;
							}
							tileData->phaseDirections[k] = phaseDirections;
						}
					}
					tileDataBytes[0] = (unsigned char)((phasePassability >> 16) % 256);
					tileDataBytes[1] = (unsigned char)((phasePassability >> 8) % 256);
					tileDataBytes[2] = (unsigned char)(phasePassability % 256);
					for (int k = 0; k < NUM_PHASES; k += 2)
						tileDataBytes[k / 2 + 3] = (tileData->phaseDirections[k] << 4) + tileData->phaseDirections[k + 1];
					tileDataBytes[15] = tileData->tilesetID;
					tileDataBytes[16] = tileData->tileID;
					tileDataBytes[17] = tileData->tileFlags;
					XMLTools::base64Encode(tileDataBytes, inputLength, tileDataBase64, outputLength);
					tileDataBase64[outputLength] = '\0';
					layerData += tileDataBase64;
				}
				Node2.addText(layerData.toUtf8().data());
			}
		}
		Node1.writeToFile(infoList[i].absoluteFilePath().toLocal8Bit().data());
	}
	//Convert tileset xml files
	log->setCurrentTask("Converting Tilesets");
	dir = QDir(projectLocation + "/tilesets");
	infoList = dir.entryInfoList(QStringList() << "*.xml");
	for (int i = 0; i < infoList.size(); ++i)
	{
		log->addLogMessage(QString("Updating tileset: %1").arg(infoList[i].fileName()));
		QCoreApplication::processEvents();
		Node1 = XMLNode::openFileHelper(infoList[i].absoluteFilePath().toLocal8Bit().data(), "openrpgmaker-tileset");
		for (int j = 0; j < Node1.nChildNode("tile"); ++j)
		{
			QString directionsText = "";
			QString passabilityText = "";
			unsigned long phasePassability = 0ul;
			Node2 = Node1.getChildNode("tile", j);
			phasePassability = QString(Node2.getAttribute("phasePassability")).toULong(0, 16);
			phasePassability &= 0xFFFFFF;
			passabilityText = QString("%1").arg(phasePassability, 6, 16, QChar('0'));
			for (int k = 0; k < NUM_PHASES; ++k)
				directionsText += ((phasePassability >> k) % 2 == 1) ? "F":"0";
			Node2.addAttribute("phaseDirections", directionsText.toUtf8().data());
			Node2.updateAttribute(passabilityText.toUtf8().data(), NULL, "phasePassability");
		}
		Node1.writeToFile(infoList[i].absoluteFilePath().toLocal8Bit().data());
	}
	//Convert vehicle xml files
	log->setCurrentTask("Converting Vehicles");
	dir = QDir(projectLocation + "/vehicles");
	infoList = dir.entryInfoList(QStringList() << "*.xml");
	for (int i = 0; i < infoList.size(); ++i)
	{
		int phase;
		log->addLogMessage(QString("Updating vehicle: %1").arg(infoList[i].fileName()));
		QCoreApplication::processEvents();
		Node1 = XMLNode::openFileHelper(infoList[i].absoluteFilePath().toLocal8Bit().data(), "openrpgmaker-vehicle");
		phase = QString(Node1.getAttribute("passingPhase")).toInt();
		if (phase > 23)
			Node1.updateAttribute("23", NULL, "passingPhase");
		phase = QString(Node1.getAttribute("landingPhase")).toInt();
		if (phase > 23)
			Node1.updateAttribute("23", NULL, "landingPhase");
		Node1.writeToFile(infoList[i].absoluteFilePath().toLocal8Bit().data());
	}
}
#endif // COMPILING_EDITOR
