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

#include <QDateTime>
#include <QMessageBox>
#include <cstdio>
#include <cstdlib>
#include "mapview.h"
#include "projectdata.h"
#include "randommapgenerator.h"
#include "xmlParser.h"
#include "xmlTools.h"

RandomMapGenerator::RandomMapGenerator(QWidget *parent) : QDialog(parent)
{
	XMLNode ConfigurationNode = XMLNode::openFileHelper((ProjectData::homePath + "/storage/randommapgenerators.xml").toLocal8Bit().data(), "randommapgenerators");
	setupUi(this);
	tbGenerateMaps->setEnabled(false);
	frmMapPreview->setEnabled(false);
	buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	buildingGenerator = new RandomMapGenerator_Building();
	buildingGenerator->setupGenerator(ConfigurationNode.getChildNode("buildinggenerator"));
	cavernGenerator = new RandomMapGenerator_CavernStructure();
	cavernGenerator->setupGenerator(ConfigurationNode.getChildNode("caverngenerator"));
	dungeonGenerator = new RandomMapGenerator_DungeonStructure();
	dungeonGenerator->setupGenerator(ConfigurationNode.getChildNode("dungeongenerator"));
	mazeGenerator = new RandomMapGenerator_Maze();
	mazeGenerator->setupGenerator(ConfigurationNode.getChildNode("mazegenerator"));
	TODO("Uncomment the following two lines when the Mountain Generator is ready");
	//mountainGenerator = new RandomMapGenerator_MountainStructure();
	//mountainGenerator->setupGenerator(ConfigurationNode.getChildNode("mountaingenerator"));
	TODO("Uncomment the following two lines when the Town Generator is ready");
	//townGenerator = new RandomMapGenerator_Town();
	//townGenerator->setupGenerator(ConfigurationNode.getChildNode("towngenerator"));
	worldMapGenerator = new RandomMapGenerator_WorldMap();
	worldMapGenerator->setupGenerator(ConfigurationNode.getChildNode("worldMapGenerator"));
	numberGenerator.seedGenerator(QDateTime::currentDateTime().toTime_t());
	randomSeedValidator = new QRegExpValidator(QRegExp("([1-4]\\d{9})|(\\d{1,9})"), 0);
	leRandomSeed->setValidator(randomSeedValidator);
	leRandomSeed->setText(QString::number(numberGenerator.getRandomNumber()));
	gvMapPreview->changeZoom(MapView::ZOOM_1TO1);
	mapViewSetup = false;
	ignoreEvents = false;
}

RandomMapGenerator::~RandomMapGenerator()
{
	XMLNode ConfigurationNode = XMLNode::createXMLTopNode("randommapgenerators");
	ConfigurationNode.addChild(buildingGenerator->getConfigurationNode());
	ConfigurationNode.addChild(cavernGenerator->getConfigurationNode());
	ConfigurationNode.addChild(dungeonGenerator->getConfigurationNode());
	ConfigurationNode.addChild(mazeGenerator->getConfigurationNode());
	ConfigurationNode.addChild("mountaingenerator");
	ConfigurationNode.addChild("towngenerator");
	ConfigurationNode.addChild(worldMapGenerator->getConfigurationNode());
	ConfigurationNode.writeToFile((ProjectData::homePath + "/storage/randommapgenerators.xml").toLocal8Bit().data());
	delete buildingGenerator;
	delete cavernGenerator;
	delete dungeonGenerator;
	delete mazeGenerator;
	TODO("Uncomment the following line when the Mountain Generator is ready");
	//delete mountainGenerator;
	TODO("Uncomment the following line when the Town Generator is ready");
	//delete townGenerator;
	delete worldMapGenerator;
	leRandomSeed->setValidator(0);
	delete randomSeedValidator;
	if (mapViewSetup)
	{
		gvMapPreview->shutdownView();
		mapViewSetup = false;
	}
}

int RandomMapGenerator::numGeneratedMaps()
{
	return cboxSelectMap->count();
}

Map *RandomMapGenerator::getGeneratedMap(int mapNum)
{
	int mapType = cboxMapType->currentIndex();
	TODO("Uncomment the following if-block when the Building Generator is ready");
	/*if (mapType == TYPE_BUILDING && buildingGenerator->isConfigured())
	{
	}*/
	if (mapType == TYPE_CAVERNSTRUCTURE && cavernGenerator->isConfigured())
		return cavernGenerator->getGeneratedMap(mapNum);
	else if (mapType == TYPE_DUNGEONSTRUCTURE && dungeonGenerator->isConfigured())
		return dungeonGenerator->getGeneratedMap(mapNum);
	else if (mapType == TYPE_MAZE && mazeGenerator->isConfigured())
		return mazeGenerator->getGeneratedMap(mapNum);
	TODO("Uncomment the following if-block when the Mountain Generator is ready");
	/*else if (mapType == TYPE_MOUNTAINSTRUCTURE && mountainGenerator->isConfigured())
	{
	}*/
	TODO("Uncomment the following if-block when the Town Generator is ready.");
	/*else if (mapType == TYPE_TOWN && townGenerator->isConfigured())
	{
	}*/
	if (mapType == TYPE_WORLDMAP && worldMapGenerator->isConfigured())
		return worldMapGenerator->getGeneratedMap(mapNum);
	return NULL;
}

void RandomMapGenerator::on_cboxMapType_currentIndexChanged(int index)
{
	tbGenerateMaps->setEnabled(false);
	if (mapViewSetup)
	{
		gvMapPreview->shutdownView();
		mapViewSetup = false;
	}
	TODO("Uncomment the following if-block when the Building Generator is ready");
	/*if (index == TYPE_BUILDING && buildingGenerator->isConfigured())
	{
	}*/
	if (index == TYPE_CAVERNSTRUCTURE && cavernGenerator->isConfigured())
		tbGenerateMaps->setEnabled(true);
	if (index == TYPE_DUNGEONSTRUCTURE && dungeonGenerator->isConfigured())
		tbGenerateMaps->setEnabled(true);
	if (index == TYPE_MAZE && mazeGenerator->isConfigured())
		tbGenerateMaps->setEnabled(true);
	TODO("Uncomment the following if-block when the Mountain Generator is ready");
	/*if (index == TYPE_MOUNTAINSTRUCTURE && mountainGenerator->isConfigured())
	{
}*/
	TODO("Uncomment the following if-block when the Town Generator is ready.");
	/*if (index == TYPE_TOWN && townGenerator->isConfigured())
	{
	}*/
	if (index == TYPE_WORLDMAP && worldMapGenerator->isConfigured())
		tbGenerateMaps->setEnabled(true);
}

void RandomMapGenerator::on_tbConfigureGenerator_clicked()
{
	int mapType = cboxMapType->currentIndex();
	if (mapType == TYPE_BUILDING)
	{
		QMessageBox::information(NULL, "Not Implemented", "This generator is not yet finished.\nPlease be patient and look for it in future releases.");
		TODO("Uncomment the following lines when the Building Generator is ready");
		//buildingGenerator->exec();
		//if (buildingGenerator->isConfigured())
		//	tbGenerateMaps->setEnabled(true);
	}
	else if (mapType == TYPE_CAVERNSTRUCTURE)
	{
		cavernGenerator->exec();
		if (cavernGenerator->isConfigured())
			tbGenerateMaps->setEnabled(true);
	}
	else if (mapType == TYPE_DUNGEONSTRUCTURE)
	{
		dungeonGenerator->exec();
		if (dungeonGenerator->isConfigured())
			tbGenerateMaps->setEnabled(true);
	}
	else if (mapType == TYPE_MAZE)
	{
		mazeGenerator->exec();
		if (mazeGenerator->isConfigured())
			tbGenerateMaps->setEnabled(true);
	}
	else if (mapType == TYPE_MOUNTAINSTRUCTURE)
	{
		QMessageBox::information(NULL, "Not Implemented", "This generator is not yet finished.\nPlease be patient and look for it in future releases.");
		TODO("Uncomment the following lines when the Mountain Generator is ready");
		//mountainGenerator->exec();
		//if (mountainGenerator->isConfigured())
		//	tbGenerateMaps->setEnabled(true);
	}
	else if (mapType == TYPE_TOWN)
	{
		QMessageBox::information(NULL, "Not Implemented", "This generator is not yet finished.\nPlease be patient and look for it in future releases.");
		TODO("Uncomment the following lines when the Town Generator is ready");
		//townGenerator->exec();
		//if (townGenerator->isConfigured())
		//	tbGenerateMaps->setEnabled(true);
	}
	else if (mapType == TYPE_WORLDMAP)
	{
		worldMapGenerator->exec();
		if (worldMapGenerator->isConfigured())
			tbGenerateMaps->setEnabled(true);
	}
}

void RandomMapGenerator::on_tbGenerateMaps_clicked()
{
	int mapType = cboxMapType->currentIndex();
	ignoreEvents = true;
	if (mapType == TYPE_BUILDING)
	{
		TODO("Uncomment the following lines when the Building Generator is ready");
		//buildingGenerator->generateBuilding(leRandomSeed->text().toULong());
		//mapNames = buildingGenerator->getGeneratedMapNames();
	}
	else if (mapType == TYPE_CAVERNSTRUCTURE)
	{
		cavernGenerator->generateCavernStructure(leRandomSeed->text().toULong());
		cboxSelectMap->clear();
		buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
		frmMapPreview->setEnabled(true);
		cboxSelectMap->addItems(cavernGenerator->getGeneratedMapNames());
		gvMapPreview->changeMap(cavernGenerator->getGeneratedMap(0));
	}
	else if (mapType == TYPE_DUNGEONSTRUCTURE)
	{
		dungeonGenerator->generateDungeonStructure(leRandomSeed->text().toULong());
		cboxSelectMap->clear();
		buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
		frmMapPreview->setEnabled(true);
		cboxSelectMap->addItems(dungeonGenerator->getGeneratedMapNames());
		gvMapPreview->changeMap(dungeonGenerator->getGeneratedMap(0));
	}
	else if (mapType == TYPE_MAZE)
	{
		mazeGenerator->generateMaze(leRandomSeed->text().toULong());
		cboxSelectMap->clear();
		buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
		frmMapPreview->setEnabled(true);
		cboxSelectMap->addItems(mazeGenerator->getGeneratedMapNames());
		gvMapPreview->changeMap(mazeGenerator->getGeneratedMap(0));
	}
	else if (mapType == TYPE_MOUNTAINSTRUCTURE)
	{
		TODO("Uncomment the following lines when the Mountain Generator is ready");
		//mountainGenerator->generateMountainStructure(leRandomSeed->text().toULong());
		//mapNames = mountainGenerator->getGeneratedMapNames();
	}
	else if (mapType == TYPE_TOWN)
	{
		TODO("Uncomment the following lines when the Town Generator is ready");
		//townGenerator->generateTown(leRandomSeed->text().toULong());
		//mapNames = townGenerator->getGeneratedMapNames();
	}
	else if (mapType == TYPE_WORLDMAP)
	{
		worldMapGenerator->generateWorldMap(leRandomSeed->text().toULong());
		cboxSelectMap->clear();
		if (!(worldMapGenerator->canceled()))
		{
			buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
			frmMapPreview->setEnabled(true);
			cboxSelectMap->addItems(worldMapGenerator->getGeneratedMapNames());
			gvMapPreview->changeMap(worldMapGenerator->getGeneratedMap(0));
		}
		else
		{
			buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
			frmMapPreview->setEnabled(false);
		}
	}
	if (!mapViewSetup)
	{
		gvMapPreview->setupView();
		mapViewSetup = true;
	}
	ignoreEvents = false;
}

void RandomMapGenerator::on_tbNewSeed_clicked()
{
	leRandomSeed->setText(QString::number(numberGenerator.getRandomNumber()));
}

void RandomMapGenerator::on_cboxSelectMap_currentIndexChanged(int index)
{
	if (!ignoreEvents)
	{
		int mapType = cboxMapType->currentIndex();
		if (mapType == TYPE_BUILDING)
		{
			TODO("Implement \"if (mapType == TYPE_BUILDING)\"");
		}
		else if (mapType == TYPE_CAVERNSTRUCTURE)
			gvMapPreview->changeMap(cavernGenerator->getGeneratedMap(index));
		else if (mapType == TYPE_DUNGEONSTRUCTURE)
			gvMapPreview->changeMap(dungeonGenerator->getGeneratedMap(index));
		else if (mapType == TYPE_MAZE)
			gvMapPreview->changeMap(mazeGenerator->getGeneratedMap(index));
		else if (mapType == TYPE_MOUNTAINSTRUCTURE)
		{
			TODO("Implement \"if (mapType == TYPE_MOUNTAINSTRUCTURE)\"");
		}
		else if (mapType == TYPE_TOWN)
		{
			TODO("Implement \"if (mapType == TYPE_TOWN)\"");
		}
		else if (mapType == TYPE_WORLDMAP)
			gvMapPreview->changeMap(worldMapGenerator->getGeneratedMap(index));
		if (!mapViewSetup)
		{
			gvMapPreview->setupView();
			mapViewSetup = true;
		}
	}
}

void RandomMapGenerator::on_tbZoom1to1_toggled(bool on)
{
	if (on)
	{
		tbZoom1to2->setChecked(false);
		tbZoom1to4->setChecked(false);
		tbZoom1to8->setChecked(false);
		gvMapPreview->changeZoom(MapView::ZOOM_1TO1);
	}
	else if (!tbZoom1to2->isChecked() && !tbZoom1to4->isChecked() && !tbZoom1to8->isChecked())
		tbZoom1to1->setChecked(true);
}

void RandomMapGenerator::on_tbZoom1to2_toggled(bool on)
{
	if (on)
	{
		tbZoom1to1->setChecked(false);
		tbZoom1to4->setChecked(false);
		tbZoom1to8->setChecked(false);
		gvMapPreview->changeZoom(MapView::ZOOM_1TO2);
	}
	else if (!tbZoom1to1->isChecked() && !tbZoom1to4->isChecked() && !tbZoom1to8->isChecked())
		tbZoom1to2->setChecked(true);
}

void RandomMapGenerator::on_tbZoom1to4_toggled(bool on)
{
	if (on)
	{
		tbZoom1to1->setChecked(false);
		tbZoom1to2->setChecked(false);
		tbZoom1to8->setChecked(false);
		gvMapPreview->changeZoom(MapView::ZOOM_1TO4);
	}
	else if (!tbZoom1to1->isChecked() && !tbZoom1to2->isChecked() && !tbZoom1to8->isChecked())
		tbZoom1to4->setChecked(true);
}

void RandomMapGenerator::on_tbZoom1to8_toggled(bool on)
{
	if (on)
	{
		tbZoom1to1->setChecked(false);
		tbZoom1to2->setChecked(false);
		tbZoom1to4->setChecked(false);
		gvMapPreview->changeZoom(MapView::ZOOM_1TO8);
	}
	else if (!tbZoom1to1->isChecked() && !tbZoom1to2->isChecked() && !tbZoom1to4->isChecked())
		tbZoom1to8->setChecked(true);
}

void RandomMapGenerator::on_buttonBox_helpRequested()
{
	ProjectData::helpManual->loadIndex("Random Map Generator");
	if (!ProjectData::helpManual->isVisible())
		ProjectData::helpManual->show();
	else
		ProjectData::helpManual->raise();
	ProjectData::helpManual->activateWindow();
}
