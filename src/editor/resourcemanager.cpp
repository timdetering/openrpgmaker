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
 *  Copyright (C) 2013, Justin Davis <tuxdavis@gmail.com>                   *
 ****************************************************************************/

#include <QTreeWidgetItem>
#include <QDir>
#include <QFileInfo>
#include <QFileDialog>
#include <QMessageBox>
#include <QCursor>
#include <QMenu>
#include <QAction>
#include <QCoreApplication>
#include "importer.h"
#include "loading.h"
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
#include "projectdata_monster.h"
#include "projectdata_monsteranimation.h"
#include "projectdata_monstergroup.h"
#include "projectdata_popupmenulayout.h"
#include "projectdata_skill.h"
#include "projectdata_terrain.h"
#include "projectdata_tileset.h"
#include "projectdata_vehicle.h"
#include "resourcemanager.h"
#include "xmlParser.h"
#include "xmlTools.h"

ResourceManager::ResourceManager(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	buildResourceTree();
}

ResourceManager::~ResourceManager()
{
}

void ResourceManager::on_bExport_clicked()
{
	QString fileLocation = QFileDialog::getSaveFileName(0, "Save Location", QDir::homePath(), "Open RPG Maker Resources (*.orpgmresource)");
	if (!fileLocation.isEmpty())
	{
		Loading *loading = new Loading("Importing Resources", this);
		XMLNode ResourceNode = XMLNode::createXMLTopNode("openrpgmaker-resource");
		XMLNode FilesNode = ResourceNode.addChild("files");
		XMLNode FileNode;
		QStringList resourceFiles;
		QString xmlData;
		loading->show();
		if (!fileLocation.endsWith(".orpgmresource"))
			fileLocation += ".orpgmresource";
		for (int i = 0; i < attributeItems.size(); ++i)
		{
			QCoreApplication::processEvents();
			if (attributeItems[i]->checkState(0) == Qt::Checked)
			{
				ProjectData::DataReference<Attribute> *dataRef = new ProjectData::DataReference<Attribute>(i, __FILE__, __LINE__);
				ResourceNode.addChild((*dataRef)->getAttributeNode());
				resourceFiles += (*dataRef)->getResources();
				delete dataRef;
			}
		}
		for (int i = 0; i < battleAnimationItems.size(); ++i)
		{
			QCoreApplication::processEvents();
			if (battleAnimationItems[i]->checkState(0) == Qt::Checked)
			{
				ProjectData::DataReference<BattleAnimation> *dataRef = new ProjectData::DataReference<BattleAnimation>(i, __FILE__, __LINE__);
				ResourceNode.addChild((*dataRef)->getAnimationNode());
				resourceFiles += (*dataRef)->getResources();
				delete dataRef;
			}
		}
		for (int i = 0; i < characterItems.size(); ++i)
		{
			QCoreApplication::processEvents();
			if (characterItems[i]->checkState(0) == Qt::Checked)
			{
				ProjectData::DataReference<Character> *dataRef = new ProjectData::DataReference<Character>(i, __FILE__, __LINE__);
				ResourceNode.addChild((*dataRef)->getCharacterNode());
				resourceFiles += (*dataRef)->getResources();
				delete dataRef;
			}
		}
		for (int i = 0; i < characterSpriteItems.size(); ++i)
		{
			QCoreApplication::processEvents();
			if (characterSpriteItems[i]->checkState(0) == Qt::Checked)
			{
				ProjectData::DataReference<CharacterSprite> *dataRef = new ProjectData::DataReference<CharacterSprite>(i, __FILE__, __LINE__);
				ResourceNode.addChild((*dataRef)->getCharacterSpriteNode());
				resourceFiles += (*dataRef)->getResources();
				delete dataRef;
			}
		}
		for (int i = 0; i < classItems.size(); ++i)
		{
			QCoreApplication::processEvents();
			if (classItems[i]->checkState(0) == Qt::Checked)
			{
				ProjectData::DataReference<Class> *dataRef = new ProjectData::DataReference<Class>(i, __FILE__, __LINE__);
				ResourceNode.addChild((*dataRef)->getClassNode());
				resourceFiles += (*dataRef)->getResources();
				delete dataRef;
			}
		}
		for (int i = 0; i < commonEventItems.size(); ++i)
		{
			QCoreApplication::processEvents();
			if (commonEventItems[i]->checkState(0) == Qt::Checked)
			{
				ProjectData::DataReference<CommonEvent> *dataRef = new ProjectData::DataReference<CommonEvent>(i, __FILE__, __LINE__);
				ResourceNode.addChild((*dataRef)->getCommonEventNode());
				//resourceFiles += (*dataRef)->getResources();
				delete dataRef;
			}
		}
		for (int i = 0; i < conditionItems.size(); ++i)
		{
			QCoreApplication::processEvents();
			if (conditionItems[i]->checkState(0) == Qt::Checked)
			{
				ProjectData::DataReference<Condition> *dataRef = new ProjectData::DataReference<Condition>(i, __FILE__, __LINE__);
				ResourceNode.addChild((*dataRef)->getConditionNode());
				resourceFiles += (*dataRef)->getResources();
				delete dataRef;
			}
		}
		for (int i = 0; i < fontItems.size(); ++i)
		{
			QCoreApplication::processEvents();
			if (fontItems[i]->checkState(0) == Qt::Checked)
			{
				ProjectData::DataReference<Font> *dataRef = new ProjectData::DataReference<Font>(i, __FILE__, __LINE__);
				ResourceNode.addChild((*dataRef)->getFontNode());
				resourceFiles += (*dataRef)->getResources();
				delete dataRef;
			}
		}
		for (int i = 0; i < globalAnimationItems.size(); ++i)
		{
			QCoreApplication::processEvents();
			if (globalAnimationItems[i]->checkState(0) == Qt::Checked)
			{
				ProjectData::DataReference<GlobalAnimation> *dataRef = new ProjectData::DataReference<GlobalAnimation>(i, __FILE__, __LINE__);
				ResourceNode.addChild((*dataRef)->getAnimationNode());
				resourceFiles += (*dataRef)->getResources();
				delete dataRef;
			}
		}
		for (int i = 0; i < itemItems.size(); ++i)
		{
			QCoreApplication::processEvents();
			if (itemItems[i]->checkState(0) == Qt::Checked)
			{
				ProjectData::DataReference<Item> *dataRef = new ProjectData::DataReference<Item>(i, __FILE__, __LINE__);
				ResourceNode.addChild((*dataRef)->getItemNode());
				resourceFiles += (*dataRef)->getResources();
				delete dataRef;
			}
		}
		for (int i = 0; i < mainMenuItems.size(); ++i)
		{
			QCoreApplication::processEvents();
			if (mainMenuItems[i]->checkState(0) == Qt::Checked)
			{
				ProjectData::DataReference<MainMenuLayout> *dataRef = new ProjectData::DataReference<MainMenuLayout>(mainMenuIDs[i], __FILE__, __LINE__);
				ResourceNode.addChild((*dataRef)->getMenuNode());
				resourceFiles += (*dataRef)->getResources();
				delete dataRef;
			}
		}
		for (int i = 0; i < monsterAnimationItems.size(); ++i)
		{
			QCoreApplication::processEvents();
			if (monsterAnimationItems[i]->checkState(0) == Qt::Checked)
			{
				ProjectData::DataReference<MonsterAnimation> *dataRef = new ProjectData::DataReference<MonsterAnimation>(i, __FILE__, __LINE__);
				ResourceNode.addChild((*dataRef)->getMonsterAnimationNode());
				resourceFiles += (*dataRef)->getResources();
				delete dataRef;
			}
		}
		for (int i = 0; i < monsterItems.size(); ++i)
		{
			QCoreApplication::processEvents();
			if (monsterItems[i]->checkState(0) == Qt::Checked)
			{
				ProjectData::DataReference<Monster> *dataRef = new ProjectData::DataReference<Monster>(i, __FILE__, __LINE__);
				ResourceNode.addChild((*dataRef)->getMonsterNode());
				resourceFiles += (*dataRef)->getResources();
				delete dataRef;
			}
		}
		for (int i = 0; i < monsterGroupItems.size(); ++i)
		{
			QCoreApplication::processEvents();
			if (monsterGroupItems[i]->checkState(0) == Qt::Checked)
			{
				ProjectData::DataReference<MonsterGroup> *dataRef = new ProjectData::DataReference<MonsterGroup>(i, __FILE__, __LINE__);
				ResourceNode.addChild((*dataRef)->getMonsterGroupNode());
				resourceFiles += (*dataRef)->getResources();
				delete dataRef;
			}
		}
		for (int i = 0; i < popupMenuItems.size(); ++i)
		{
			QCoreApplication::processEvents();
			if (popupMenuItems[i]->checkState(0) == Qt::Checked)
			{
				ProjectData::DataReference<PopupMenuLayout> *dataRef = new ProjectData::DataReference<PopupMenuLayout>(popupMenuIDs[i], __FILE__, __LINE__);
				ResourceNode.addChild((*dataRef)->getMenuNode());
				resourceFiles += (*dataRef)->getResources();
				delete dataRef;
			}
		}
		for (int i = 0; i < skillItems.size(); ++i)
		{
			QCoreApplication::processEvents();
			if (skillItems[i]->checkState(0) == Qt::Checked)
			{
				ProjectData::DataReference<Skill> *dataRef = new ProjectData::DataReference<Skill>(i, __FILE__, __LINE__);
				ResourceNode.addChild((*dataRef)->getSkillNode());
				resourceFiles += (*dataRef)->getResources();
				delete dataRef;
			}
		}
		for (int i = 0; i < terrainItems.size(); ++i)
		{
			QCoreApplication::processEvents();
			if (terrainItems[i]->checkState(0) == Qt::Checked)
			{
				ProjectData::DataReference<Terrain> *dataRef = new ProjectData::DataReference<Terrain>(i, __FILE__, __LINE__);
				ResourceNode.addChild((*dataRef)->getTerrainNode());
				resourceFiles += (*dataRef)->getResources();
				delete dataRef;
			}
		}
		for (int i = 0; i < tilesetItems.size(); ++i)
		{
			QCoreApplication::processEvents();
			if (tilesetItems[i]->checkState(0) == Qt::Checked)
			{
				ProjectData::DataReference<Tileset> *dataRef = new ProjectData::DataReference<Tileset>(i, __FILE__, __LINE__);
				ResourceNode.addChild((*dataRef)->getTilesetNode());
				resourceFiles += (*dataRef)->getResources();
				delete dataRef;
			}
		}
		for (int i = 0; i < vehicleItems.size(); ++i)
		{
			QCoreApplication::processEvents();
			if (vehicleItems[i]->checkState(0) == Qt::Checked)
			{
				ProjectData::DataReference<Vehicle> *dataRef = new ProjectData::DataReference<Vehicle>(i, __FILE__, __LINE__);
				ResourceNode.addChild((*dataRef)->getVehicleNode());
				resourceFiles += (*dataRef)->getResources();
				delete dataRef;
			}
		}
		for (int i = 0; i < fileItems.size(); ++i)
		{
			QCoreApplication::processEvents();
			if (fileItems[i]->checkState(0) == Qt::Checked)
				resourceFiles += fileLocations[i];
		}
		QCoreApplication::processEvents();
		resourceFiles.removeDuplicates();
		resourceFiles.removeAll(QString());
		resourceFiles.removeAll(QString(""));
		for (int i = 0; i < resourceFiles.size(); ++i)
		{
			QCoreApplication::processEvents();
			if (!resourceFiles[i].isEmpty())
			{
				FileNode = FilesNode.addChild("file");
				FileNode.addAttribute("location", ProjectData::getRelativeResourcePath(resourceFiles[i], true).toUtf8().data());
				XMLTools::embedResource(FileNode, resourceFiles[i]);
			}
		}
		QCoreApplication::processEvents();
		xmlData = ResourceNode.createXMLString(1);
		xmlData.replace("global::", "local::");
		writeXMLData(xmlData, fileLocation);
		loading->doneLoading();
		QCoreApplication::processEvents();
	}
}

void ResourceManager::on_bImportExternal_clicked()
{
	Importer *importer = new Importer;
	importer->exec();
	delete importer;
	buildResourceTree();
}

void ResourceManager::on_bImportORPGM_clicked()
{
	QString fileLocation = QFileDialog::getOpenFileName(0, "File Location", QDir::homePath(), "Open RPG Maker Resources (*.orpgmresource)");
	if (!fileLocation.isEmpty())
	{
		XMLNode ResourceNode, TempNode, FilesNode, FileNode;
		XMLResults results;
		QString xmlData = getXMLData(fileLocation);
		QString resourceType;
		ResourceNode = XMLNode::parseString(xmlData.toUtf8().data(), "openrpgmaker-resource", &results);
		if (ResourceNode.isEmpty() == TRUE)
			QMessageBox::critical(0, "XML Error", QString("Error parsing resource file \"%1\"\nXML Error Message: %2").arg(fileLocation).arg(XMLNode::getError(results.error)));
		else
		{
			Loading *loading = new Loading("Importing Resources", this);
			loading->show();
			FilesNode = ResourceNode.getChildNode("files");
			for (int i = 0; i < FilesNode.nChildNode("file"); ++i)
			{
				QCoreApplication::processEvents();
				FileNode = FilesNode.getChildNode("file", i);
				XMLTools::extractResource(FileNode, ProjectData::getAbsoluteResourcePath(FileNode.getAttribute("location")));
			}
			for (int i = 1; i < ResourceNode.nChildNode(); ++i)
			{
				QCoreApplication::processEvents();
				TempNode = ResourceNode.getChildNode(i);
				resourceType = TempNode.getName();
				if (resourceType == "attribute")
				{
					Attribute *data = new Attribute(TempNode);
					ProjectData::addAttribute(data);
					delete data;
				}
				else if (resourceType == "battleanimation")
				{
					BattleAnimation *data = new BattleAnimation(TempNode);
					ProjectData::addBattleAnimation(data);
					delete data;
				}
				else if (resourceType == "character")
				{
					Character *data = new Character(TempNode);
					ProjectData::addCharacter(data);
					delete data;
				}
				else if (resourceType == "charactersprite")
				{
					CharacterSprite *data = new CharacterSprite(TempNode);
					ProjectData::addCharacterSprite(data);
					delete data;
				}
				else if (resourceType == "class")
				{
					Class *data = new Class(TempNode);
					ProjectData::addClass(data);
					delete data;
				}
				else if (resourceType == "commonevent")
				{
					CommonEvent *data = new CommonEvent(TempNode);
					ProjectData::addCommonEvent(data);
					delete data;
				}
				else if (resourceType == "condition")
				{
					Condition *data = new Condition(TempNode);
					ProjectData::addCondition(data);
					delete data;
				}
				else if (resourceType == "font")
				{
					Font *data = new Font(TempNode);
					ProjectData::addFont(data);
					delete data;
				}
				else if (resourceType == "globalanimation")
				{
					GlobalAnimation *data = new GlobalAnimation(TempNode);
					ProjectData::addGlobalAnimation(data);
					delete data;
				}
				else if (resourceType == "item")
				{
					Item *data = new Item(TempNode);
					ProjectData::addItem(data);
					delete data;
				}
				else if (resourceType == "mainmenulayout")
				{
					MainMenuLayout *data = new MainMenuLayout(TempNode);
					ProjectData::addMainMenuLayout(data);
					delete data;
				}
				else if (resourceType == "monsteranimation")
				{
					MonsterAnimation *data = new MonsterAnimation(TempNode);
					ProjectData::addMonsterAnimation(data);
					delete data;
				}
				else if (resourceType == "monster")
				{
					Monster *data = new Monster(TempNode);
					ProjectData::addMonster(data);
					delete data;
				}
				else if (resourceType == "monstergroup")
				{
					MonsterGroup *data = new MonsterGroup(TempNode);
					ProjectData::addMonsterGroup(data);
					delete data;
				}
				else if (resourceType == "popupmenulayout")
				{
					PopupMenuLayout *data = new PopupMenuLayout(TempNode);
					ProjectData::addPopupMenuLayout(data);
					delete data;
				}
				else if (resourceType == "skill")
				{
					Skill *data = new Skill(TempNode);
					ProjectData::addSkill(data);
					delete data;
				}
				else if (resourceType == "terrain")
				{
					Terrain *data = new Terrain(TempNode);
					ProjectData::addTerrain(data);
					delete data;
				}
				else if (resourceType == "tileset")
				{
					Tileset *data = new Tileset(TempNode);
					ProjectData::addTileset(data);
					delete data;
				}
				else if (resourceType == "vehicle")
				{
					Vehicle *data = new Vehicle(TempNode);
					ProjectData::addVehicle(data);
					delete data;
				}
			}
			QCoreApplication::processEvents();
			buildResourceTree();
			loading->doneLoading();
		}
	}
}

void ResourceManager::on_buttonBox_helpRequested()
{
	ProjectData::helpManual->loadIndex("ResourceManager");
	if (!ProjectData::helpManual->isVisible())
		ProjectData::helpManual->show();
	else
		ProjectData::helpManual->raise();
	ProjectData::helpManual->activateWindow();
}

void ResourceManager::on_twResources_customContextMenuRequested(const QPoint &pos)
{
	QTreeWidgetItem *item = twResources->itemAt(pos);
	if (item != NULL)
	{
		if (item->childCount() > 0)
		{
			QMenu *popupMenu = new QMenu(this);
			QAction *action;
			popupMenu->addAction(aCheckAll);
			popupMenu->addAction(aCheckAllRecursively);
			popupMenu->addAction(aUncheckAll);
			popupMenu->addAction(aUncheckAllRecursively);
			action = popupMenu->exec(QCursor::pos());
			if (action == aCheckAll)
				setCheckState(item, Qt::Checked);
			else if (action == aCheckAllRecursively)
				setCheckStateRecursively(item, Qt::Checked);
			else if (action == aUncheckAll)
				setCheckState(item, Qt::Unchecked);
			else if (action == aUncheckAllRecursively)
				setCheckStateRecursively(item, Qt::Unchecked);
		}
	}
}

void ResourceManager::buildResourceTree()
{
	QTreeWidgetItem *topLevelItem;
	QTreeWidgetItem *resourceItem;
	attributeItems.clear();
	battleAnimationItems.clear();
	characterItems.clear();
	characterSpriteItems.clear();
	classItems.clear();
	commonEventItems.clear();
	conditionItems.clear();
	fontItems.clear();
	globalAnimationItems.clear();
	itemItems.clear();
	mainMenuItems.clear();
	monsterAnimationItems.clear();
	monsterItems.clear();
	monsterGroupItems.clear();
	popupMenuItems.clear();
	skillItems.clear();
	terrainItems.clear();
	tilesetItems.clear();
	vehicleItems.clear();
	fileItems.clear();
	mainMenuIDs.clear();
	popupMenuIDs.clear();
	fileLocations.clear();
	twResources->clear();
	topLevelItem = new QTreeWidgetItem(QStringList() << "Attributes");
	topLevelItem->setFlags(Qt::ItemIsEnabled);
	twResources->addTopLevelItem(topLevelItem);
	for (int i = 0; i < ProjectData::attributeNames.size(); ++i)
	{
		resourceItem = new QTreeWidgetItem(QStringList() << ProjectData::attributeNames[i]);
		resourceItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
		resourceItem->setCheckState(0, Qt::Unchecked);
		topLevelItem->addChild(resourceItem);
		attributeItems.append(resourceItem);
	}
	topLevelItem = new QTreeWidgetItem(QStringList() << "Battle Animations");
	topLevelItem->setFlags(Qt::ItemIsEnabled);
	twResources->addTopLevelItem(topLevelItem);
	for (int i = 0; i < ProjectData::battleAnimationNames.size(); ++i)
	{
		resourceItem = new QTreeWidgetItem(QStringList() << ProjectData::battleAnimationNames[i]);
		resourceItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
		resourceItem->setCheckState(0, Qt::Unchecked);
		topLevelItem->addChild(resourceItem);
		battleAnimationItems.append(resourceItem);
	}
	topLevelItem = new QTreeWidgetItem(QStringList() << "Characters");
	topLevelItem->setFlags(Qt::ItemIsEnabled);
	twResources->addTopLevelItem(topLevelItem);
	for (int i = 0; i < ProjectData::characterNames.size(); ++i)
	{
		resourceItem = new QTreeWidgetItem(QStringList() << ProjectData::characterNames[i]);
		resourceItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
		resourceItem->setCheckState(0, Qt::Unchecked);
		topLevelItem->addChild(resourceItem);
		characterItems.append(resourceItem);
	}
	topLevelItem = new QTreeWidgetItem(QStringList() << "Character Sprites");
	topLevelItem->setFlags(Qt::ItemIsEnabled);
	twResources->addTopLevelItem(topLevelItem);
	for (int i = 0; i < ProjectData::characterSpriteNames.size(); ++i)
	{
		resourceItem = new QTreeWidgetItem(QStringList() << ProjectData::characterSpriteNames[i]);
		resourceItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
		resourceItem->setCheckState(0, Qt::Unchecked);
		topLevelItem->addChild(resourceItem);
		characterSpriteItems.append(resourceItem);
	}
	topLevelItem = new QTreeWidgetItem(QStringList() << "Classes");
	topLevelItem->setFlags(Qt::ItemIsEnabled);
	twResources->addTopLevelItem(topLevelItem);
	for (int i = 0; i < ProjectData::classNames.size(); ++i)
	{
		resourceItem = new QTreeWidgetItem(QStringList() << ProjectData::classNames[i]);
		resourceItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
		resourceItem->setCheckState(0, Qt::Unchecked);
		topLevelItem->addChild(resourceItem);
		classItems.append(resourceItem);
	}
	topLevelItem = new QTreeWidgetItem(QStringList() << "Common Events");
	topLevelItem->setFlags(Qt::ItemIsEnabled);
	twResources->addTopLevelItem(topLevelItem);
	for (int i = 0; i < ProjectData::commonEventNames.size(); ++i)
	{
		resourceItem = new QTreeWidgetItem(QStringList() << ProjectData::commonEventNames[i]);
		resourceItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
		resourceItem->setCheckState(0, Qt::Unchecked);
		topLevelItem->addChild(resourceItem);
		commonEventItems.append(resourceItem);
	}
	topLevelItem = new QTreeWidgetItem(QStringList() << "Conditions");
	topLevelItem->setFlags(Qt::ItemIsEnabled);
	twResources->addTopLevelItem(topLevelItem);
	for (int i = 0; i < ProjectData::conditionNames.size(); ++i)
	{
		resourceItem = new QTreeWidgetItem(QStringList() << ProjectData::conditionNames[i]);
		resourceItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
		resourceItem->setCheckState(0, Qt::Unchecked);
		topLevelItem->addChild(resourceItem);
		conditionItems.append(resourceItem);
	}
	topLevelItem = new QTreeWidgetItem(QStringList() << "Fonts");
	topLevelItem->setFlags(Qt::ItemIsEnabled);
	twResources->addTopLevelItem(topLevelItem);
	for (int i = 0; i < ProjectData::fontNames.size(); ++i)
	{
		resourceItem = new QTreeWidgetItem(QStringList() << ProjectData::fontNames[i]);
		resourceItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
		resourceItem->setCheckState(0, Qt::Unchecked);
		topLevelItem->addChild(resourceItem);
		fontItems.append(resourceItem);
	}
	topLevelItem = new QTreeWidgetItem(QStringList() << "Global Animations");
	topLevelItem->setFlags(Qt::ItemIsEnabled);
	twResources->addTopLevelItem(topLevelItem);
	for (int i = 0; i < ProjectData::globalAnimationNames.size(); ++i)
	{
		resourceItem = new QTreeWidgetItem(QStringList() << ProjectData::globalAnimationNames[i]);
		resourceItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
		resourceItem->setCheckState(0, Qt::Unchecked);
		topLevelItem->addChild(resourceItem);
		globalAnimationItems.append(resourceItem);
	}
	topLevelItem = new QTreeWidgetItem(QStringList() << "Items");
	topLevelItem->setFlags(Qt::ItemIsEnabled);
	twResources->addTopLevelItem(topLevelItem);
	for (int i = 0; i < ProjectData::itemNames.size(); ++i)
	{
		resourceItem = new QTreeWidgetItem(QStringList() << ProjectData::itemNames[i]);
		resourceItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
		resourceItem->setCheckState(0, Qt::Unchecked);
		topLevelItem->addChild(resourceItem);
		itemItems.append(resourceItem);
	}
	topLevelItem = new QTreeWidgetItem(QStringList() << "Monster Animations");
	topLevelItem->setFlags(Qt::ItemIsEnabled);
	twResources->addTopLevelItem(topLevelItem);
	for (int i = 0; i < ProjectData::monsterAnimationNames.size(); ++i)
	{
		resourceItem = new QTreeWidgetItem(QStringList() << ProjectData::monsterAnimationNames[i]);
		resourceItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
		resourceItem->setCheckState(0, Qt::Unchecked);
		topLevelItem->addChild(resourceItem);
		monsterAnimationItems.append(resourceItem);
	}
	topLevelItem = new QTreeWidgetItem(QStringList() << "Monsters");
	topLevelItem->setFlags(Qt::ItemIsEnabled);
	twResources->addTopLevelItem(topLevelItem);
	for (int i = 0; i < ProjectData::monsterNames.size(); ++i)
	{
		resourceItem = new QTreeWidgetItem(QStringList() << ProjectData::monsterNames[i]);
		resourceItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
		resourceItem->setCheckState(0, Qt::Unchecked);
		topLevelItem->addChild(resourceItem);
		monsterItems.append(resourceItem);
	}
	topLevelItem = new QTreeWidgetItem(QStringList() << "Monster Groups");
	topLevelItem->setFlags(Qt::ItemIsEnabled);
	twResources->addTopLevelItem(topLevelItem);
	for (int i = 0; i < ProjectData::monsterGroupNames.size(); ++i)
	{
		resourceItem = new QTreeWidgetItem(QStringList() << ProjectData::monsterGroupNames[i]);
		resourceItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
		resourceItem->setCheckState(0, Qt::Unchecked);
		topLevelItem->addChild(resourceItem);
		monsterGroupItems.append(resourceItem);
	}
	topLevelItem = new QTreeWidgetItem(QStringList() << "Skills");
	topLevelItem->setFlags(Qt::ItemIsEnabled);
	twResources->addTopLevelItem(topLevelItem);
	for (int i = 0; i < ProjectData::skillNames.size(); ++i)
	{
		resourceItem = new QTreeWidgetItem(QStringList() << ProjectData::skillNames[i]);
		resourceItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
		resourceItem->setCheckState(0, Qt::Unchecked);
		topLevelItem->addChild(resourceItem);
		skillItems.append(resourceItem);
	}
	topLevelItem = new QTreeWidgetItem(QStringList() << "Terrain");
	topLevelItem->setFlags(Qt::ItemIsEnabled);
	twResources->addTopLevelItem(topLevelItem);
	for (int i = 0; i < ProjectData::terrainNames.size(); ++i)
	{
		resourceItem = new QTreeWidgetItem(QStringList() << ProjectData::terrainNames[i]);
		resourceItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
		resourceItem->setCheckState(0, Qt::Unchecked);
		topLevelItem->addChild(resourceItem);
		terrainItems.append(resourceItem);
	}
	topLevelItem = new QTreeWidgetItem(QStringList() << "Tilesets");
	topLevelItem->setFlags(Qt::ItemIsEnabled);
	twResources->addTopLevelItem(topLevelItem);
	for (int i = 0; i < ProjectData::tilesetNames.size(); ++i)
	{
		resourceItem = new QTreeWidgetItem(QStringList() << ProjectData::tilesetNames[i]);
		resourceItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
		resourceItem->setCheckState(0, Qt::Unchecked);
		topLevelItem->addChild(resourceItem);
		tilesetItems.append(resourceItem);
	}
	topLevelItem = new QTreeWidgetItem(QStringList() << "Vehicles");
	topLevelItem->setFlags(Qt::ItemIsEnabled);
	twResources->addTopLevelItem(topLevelItem);
	for (int i = 0; i < ProjectData::vehicleNames.size(); ++i)
	{
		resourceItem = new QTreeWidgetItem(QStringList() << ProjectData::vehicleNames[i]);
		resourceItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
		resourceItem->setCheckState(0, Qt::Unchecked);
		topLevelItem->addChild(resourceItem);
		vehicleItems.append(resourceItem);
	}
	topLevelItem = new QTreeWidgetItem(QStringList() << "Main Menu Layouts");
	topLevelItem->setFlags(Qt::ItemIsEnabled);
	twResources->addTopLevelItem(topLevelItem);
	for (ProjectData::mainMenuLayoutNamesIter = ProjectData::mainMenuLayoutNames.begin(); ProjectData::mainMenuLayoutNamesIter != ProjectData::mainMenuLayoutNames.end(); ++ProjectData::mainMenuLayoutNamesIter)
	{
		resourceItem = new QTreeWidgetItem(QStringList() << ProjectData::mainMenuLayoutNamesIter.value());
		resourceItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
		resourceItem->setCheckState(0, Qt::Unchecked);
		topLevelItem->addChild(resourceItem);
		mainMenuItems.append(resourceItem);
		mainMenuIDs.append(ProjectData::mainMenuLayoutNamesIter.key());
	}
	topLevelItem = new QTreeWidgetItem(QStringList() << "Popup Menu Layouts");
	topLevelItem->setFlags(Qt::ItemIsEnabled);
	twResources->addTopLevelItem(topLevelItem);
	for (ProjectData::popupMenuLayoutNamesIter = ProjectData::popupMenuLayoutNames.begin(); ProjectData::popupMenuLayoutNamesIter != ProjectData::popupMenuLayoutNames.end(); ++ProjectData::popupMenuLayoutNamesIter)
	{
		resourceItem = new QTreeWidgetItem(QStringList() << ProjectData::popupMenuLayoutNamesIter.value());
		resourceItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
		resourceItem->setCheckState(0, Qt::Unchecked);
		topLevelItem->addChild(resourceItem);
		popupMenuItems.append(resourceItem);
		popupMenuIDs.append(ProjectData::popupMenuLayoutNamesIter.key());
	}
	topLevelItem = new QTreeWidgetItem(QStringList() << "Resource Files");
	topLevelItem->setFlags(Qt::ItemIsEnabled);
	twResources->addTopLevelItem(topLevelItem);
	resourceItem = getFolderTree(ProjectData::projectLocation + "/images");
	resourceItem->setText(0, "Images");
	topLevelItem->addChild(resourceItem);
	resourceItem = getFolderTree(ProjectData::projectLocation + "/music");
	resourceItem->setText(0, "Music");
	topLevelItem->addChild(resourceItem);
	resourceItem = getFolderTree(ProjectData::projectLocation + "/sounds");
	resourceItem->setText(0, "Sound Effects");
	topLevelItem->addChild(resourceItem);
}

QTreeWidgetItem *ResourceManager::getFolderTree(QString folderLocation)
{
	QTreeWidgetItem *folderItem = new QTreeWidgetItem(QStringList() << QFileInfo(folderLocation).fileName());
	QTreeWidgetItem *fileItem;
	QStringList subFolders = QDir(folderLocation).entryList(QDir::Dirs|QDir::NoSymLinks|QDir::NoDotAndDotDot, QDir::Name|QDir::IgnoreCase);
	QStringList files = QDir(folderLocation).entryList(QDir::Files|QDir::NoSymLinks, QDir::Name|QDir::IgnoreCase);
	folderItem->setFlags(Qt::ItemIsEnabled);
	for (int i = 0; i < subFolders.size(); ++i)
		folderItem->addChild(getFolderTree(folderLocation + "/" + subFolders[i]));
	for (int i = 0; i < files.size(); ++i)
	{
		fileItem = new QTreeWidgetItem(QStringList() << files[i]);
		fileItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
		fileItem->setCheckState(0, Qt::Unchecked);
		folderItem->addChild(fileItem);
		fileItems.append(fileItem);
		fileLocations.append(folderLocation + "/" + files[i]);
	}
	return folderItem;
}

void ResourceManager::writeXMLData(QString xmlData, QString fileLocation)
{
	QFile file(fileLocation);
	//QFile xmlFile(fileLocation + ".xml");
	QByteArray byteArray = xmlData.toUtf8();
	QByteArray compressedData = qCompress(byteArray);
	//xmlFile.open(QIODevice::WriteOnly);
	//xmlFile.write(byteArray);
	//xmlFile.close();
	file.open(QIODevice::WriteOnly);
	file.write(compressedData);
	file.close();
}

QString ResourceManager::getXMLData(QString fileLocation)
{
	QFile file(fileLocation);
	QByteArray byteArray, uncompressedData;
	file.open(QIODevice::ReadOnly);
	byteArray = file.readAll();
	if (byteArray.startsWith(QString("<openrpgmaker-resource>").toUtf8()))
		uncompressedData = byteArray;
	else
		uncompressedData = qUncompress(byteArray);
	return QString::fromUtf8(uncompressedData.data(), uncompressedData.size());
}

void ResourceManager::setCheckState(QTreeWidgetItem *item, Qt::CheckState state)
{
	QTreeWidgetItem *child;
	for (int i = 0; i < item->childCount(); ++i)
	{
		child = item->child(i);
		if ((child->flags() & Qt::ItemIsUserCheckable) != 0)
			child->setCheckState(0, state);
	}
}

void ResourceManager::setCheckStateRecursively(QTreeWidgetItem *item, Qt::CheckState state)
{
	QTreeWidgetItem *child;
	for (int i = 0; i < item->childCount(); ++i)
	{
		child = item->child(i);
		if ((child->flags() & Qt::ItemIsUserCheckable) != 0)
			child->setCheckState(0, state);
		setCheckStateRecursively(child, state);
	}
}
