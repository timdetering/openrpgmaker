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
#include "projectdata.h"
#include "randommapgenerator_building.h"
#include "randomnumbergenerator.h"
#include "tilebutton.h"
#include "tilechooser.h"
#include "xmlParser.h"

RandomMapGenerator_Building::RandomMapGenerator_Building(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	leBaseName->setText("Untitled");
	tbInteriorLowerWallTile->setEnabled(false);
	tbInteriorUpperWallTile->setEnabled(false);
	tbExteriorLowerWallTile->setEnabled(false);
	tbExteriorUpperWallTile->setEnabled(false);
	tbFloorBTile->setEnabled(false);
	tbFloorCTile->setEnabled(false);
	bWidthSpinButton->setSpinBox(sbWidth);
	bHeightSpinButton->setSpinBox(sbHeight);
	bNumEntrancesSpinButton->setSpinBox(sbNumEntrances);
	bNumFloorsSpinButton->setSpinBox(sbNumFloors);
	configured = false;
	resize(sizeHint());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
}

RandomMapGenerator_Building::~RandomMapGenerator_Building()
{
}

void RandomMapGenerator_Building::setupGenerator(XMLNode ConfigurationNode)
{
	XMLNode TempNode;
	QString temp_string;
	sbWidth->setValue(atoi(ConfigurationNode.getAttribute("width")));
	sbHeight->setValue(atoi(ConfigurationNode.getAttribute("height")));
	temp_string = ConfigurationNode.getAttribute("numRooms");
	if (temp_string == "few")
		rbNumRooms_Few->setChecked(true);
	else if (temp_string == "normal")
		rbNumRooms_Normal->setChecked(true);
	else
		rbNumRooms_Lots->setChecked(true);
	temp_string = ConfigurationNode.getAttribute("roomShape");
	if (temp_string == "rectangular")
		rbRoomShape_Rectangular->setChecked(true);
	else if (temp_string == "square")
		rbRoomShape_Square->setChecked(true);
	else
		rbRoomShape_Irregular->setChecked(true);
	sbNumEntrances->setValue(atoi(ConfigurationNode.getAttribute("numEntrances")));
	sbNumFloors->setValue(atoi(ConfigurationNode.getAttribute("numFloors")));
	tbBlankTile->setTile(QString(ConfigurationNode.getAttribute("blankTile")).toUShort(0, 16));
	TempNode = ConfigurationNode.getChildNode("interiorWallTiles");
	tbInteriorCeilingTile->setTile(QString(ConfigurationNode.getAttribute("ceiling")).toUShort(0, 16));
	tbInteriorLowerWallTile->setTile(QString(ConfigurationNode.getAttribute("lowerWall")).toUShort(0, 16));
	tbInteriorUpperWallTile->setTile(QString(ConfigurationNode.getAttribute("upperWall")).toUShort(0, 16));
	if (QString(ConfigurationNode.getAttribute("useLowerWall")) == "true")
		cbUseInteriorLowerWall->setChecked(true);
	else
		cbUseInteriorLowerWall->setChecked(false);
	if (QString(ConfigurationNode.getAttribute("useUpperWall")) == "true")
		cbUseInteriorUpperWall->setChecked(true);
	else
		cbUseInteriorUpperWall->setChecked(false);
	TempNode = ConfigurationNode.getChildNode("exteriorWallTiles");
	tbExteriorCeilingTile->setTile(QString(ConfigurationNode.getAttribute("ceiling")).toUShort(0, 16));
	tbExteriorLowerWallTile->setTile(QString(ConfigurationNode.getAttribute("lowerWall")).toUShort(0, 16));
	tbExteriorUpperWallTile->setTile(QString(ConfigurationNode.getAttribute("upperWall")).toUShort(0, 16));
	if (QString(ConfigurationNode.getAttribute("useLowerWall")) == "true")
		cbUseExteriorLowerWall->setChecked(true);
	else
		cbUseExteriorLowerWall->setChecked(false);
	if (QString(ConfigurationNode.getAttribute("useUpperWall")) == "true")
		cbUseExteriorUpperWall->setChecked(true);
	else
		cbUseExteriorUpperWall->setChecked(false);
	TempNode = ConfigurationNode.getChildNode("floorTiles");
	tbFloorATile->setTile(QString(ConfigurationNode.getAttribute("floorA")).toUShort(0, 16));
	tbFloorBTile->setTile(QString(ConfigurationNode.getAttribute("floorB")).toUShort(0, 16));
	tbFloorCTile->setTile(QString(ConfigurationNode.getAttribute("floorC")).toUShort(0, 16));
	if (QString(ConfigurationNode.getAttribute("useFloorB")) == "true")
		cbUseFloorB->setChecked(true);
	else
		cbUseFloorB->setChecked(false);
	if (QString(ConfigurationNode.getAttribute("useFloorC")) == "true")
		cbUseFloorC->setChecked(true);
	else
		cbUseFloorC->setChecked(false);
	TempNode = ConfigurationNode.getChildNode("miscTiles");
	tbGroundTile->setTile(QString(ConfigurationNode.getAttribute("ground")).toUShort(0, 16));
	tbStairsUpTile->setTile(QString(ConfigurationNode.getAttribute("stairsUp")).toUShort(0, 16));
	tbStairsDownTile->setTile(QString(ConfigurationNode.getAttribute("stairsDown")).toUShort(0, 16));
}

XMLNode RandomMapGenerator_Building::getConfigurationNode()
{
	XMLNode TempNode, ConfigurationNode = XMLNode::createXMLTopNode("buildinggenerator");
	ConfigurationNode.addAttribute("width", sbWidth->cleanText().toUtf8().data());
	ConfigurationNode.addAttribute("height", sbHeight->cleanText().toUtf8().data());
	if (rbNumRooms_Few->isChecked())
		ConfigurationNode.addAttribute("numRooms", "few");
	else if (rbNumRooms_Normal->isChecked())
		ConfigurationNode.addAttribute("numRooms", "normal");
	else
		ConfigurationNode.addAttribute("numRooms", "lots");
	if (rbRoomShape_Rectangular->isChecked())
		ConfigurationNode.addAttribute("roomShape", "rectangular");
	else if (rbRoomShape_Square->isChecked())
		ConfigurationNode.addAttribute("roomShape", "square");
	else
		ConfigurationNode.addAttribute("roomShape", "irregular");
	ConfigurationNode.addAttribute("numEntrances", sbNumEntrances->cleanText().toUtf8().data());
	ConfigurationNode.addAttribute("numFloors", sbNumFloors->cleanText().toUtf8().data());
	ConfigurationNode.addAttribute("blankTile", QString("%1").arg(tbBlankTile->getTile(), 4, 16, QChar('0')).toUtf8().data());
	TempNode = ConfigurationNode.addChild("interiorWallTiles");
	TempNode.addAttribute("ceiling", QString("%1").arg(tbInteriorCeilingTile->getTile(), 4, 16, QChar('0')).toUtf8().data());
	TempNode.addAttribute("lowerWall", QString("%1").arg(tbInteriorLowerWallTile->getTile(), 4, 16, QChar('0')).toUtf8().data());
	TempNode.addAttribute("upperWall", QString("%1").arg(tbInteriorUpperWallTile->getTile(), 4, 16, QChar('0')).toUtf8().data());
	TempNode.addAttribute("useLowerWall", (cbUseInteriorLowerWall->isChecked()) ? "true":"false");
	TempNode.addAttribute("useUpperWall", (cbUseInteriorUpperWall->isChecked()) ? "true":"false");
	TempNode = ConfigurationNode.addChild("exteriorWallTiles");
	TempNode.addAttribute("ceiling", QString("%1").arg(tbExteriorCeilingTile->getTile(), 4, 16, QChar('0')).toUtf8().data());
	TempNode.addAttribute("lowerWall", QString("%1").arg(tbExteriorLowerWallTile->getTile(), 4, 16, QChar('0')).toUtf8().data());
	TempNode.addAttribute("upperWall", QString("%1").arg(tbExteriorUpperWallTile->getTile(), 4, 16, QChar('0')).toUtf8().data());
	TempNode.addAttribute("useLowerWall", (cbUseExteriorLowerWall->isChecked()) ? "true":"false");
	TempNode.addAttribute("useUpperWall", (cbUseExteriorUpperWall->isChecked()) ? "true":"false");
	TempNode = ConfigurationNode.addChild("floorTiles");
	TempNode.addAttribute("floorA", QString("%1").arg(tbFloorATile->getTile(), 4, 16, QChar('0')).toUtf8().data());
	TempNode.addAttribute("floorB", QString("%1").arg(tbFloorBTile->getTile(), 4, 16, QChar('0')).toUtf8().data());
	TempNode.addAttribute("floorC", QString("%1").arg(tbFloorCTile->getTile(), 4, 16, QChar('0')).toUtf8().data());
	TempNode.addAttribute("useFloorB", (cbUseFloorB->isChecked()) ? "true":"false");
	TempNode.addAttribute("useFloorC", (cbUseFloorC->isChecked()) ? "true":"false");
	TempNode = ConfigurationNode.addChild("miscTiles");
	TempNode.addAttribute("ground", QString("%1").arg(tbGroundTile->getTile(), 4, 16, QChar('0')).toUtf8().data());
	TempNode.addAttribute("stairsUp", QString("%1").arg(tbStairsUpTile->getTile(), 4, 16, QChar('0')).toUtf8().data());
	TempNode.addAttribute("stairsDown", QString("%1").arg(tbStairsDownTile->getTile(), 4, 16, QChar('0')).toUtf8().data());
	return ConfigurationNode;
}

bool RandomMapGenerator_Building::isConfigured()
{
	return configured;
}

QStringList RandomMapGenerator_Building::getGeneratedMapNames()
{
	QStringList names;
	QString baseName = leBaseName->text();
	int numFloors = sbNumFloors->value();
	if (numFloors == 1)
		names.append(baseName);
	else
	{
		for (int i = 0; i < numFloors; ++i)
			names.append(QString("%1 (F%2)").arg(baseName).arg(i));
	}
	return names;
}

Map *RandomMapGenerator_Building::getGeneratedMap(int mapNum)
{
	return NULL;
}

void RandomMapGenerator_Building::generateBuilding(unsigned long seed)
{
	numberGenerator.seedGenerator(seed);
}

void RandomMapGenerator_Building::on_cbUseInteriorLowerWall_toggled(bool on)
{
	tbInteriorLowerWallTile->setEnabled(on);
}

void RandomMapGenerator_Building::on_cbUseInteriorUpperWall_toggled(bool on)
{
	tbInteriorUpperWallTile->setEnabled(on);
}

void RandomMapGenerator_Building::on_cbUseExteriorLowerWall_toggled(bool on)
{
	tbExteriorLowerWallTile->setEnabled(on);
}

void RandomMapGenerator_Building::on_cbUseExteriorUpperWall_toggled(bool on)
{
	tbExteriorUpperWallTile->setEnabled(on);
}

void RandomMapGenerator_Building::on_cbUseFloorB_toggled(bool on)
{
	tbFloorBTile->setEnabled(on);
}

void RandomMapGenerator_Building::on_cbUseFloorC_toggled(bool on)
{
	tbFloorCTile->setEnabled(on);
}

void RandomMapGenerator_Building::on_buttonBox_helpRequested()
{
	ProjectData::helpManual->loadIndex("Building Generator");
	if (!ProjectData::helpManual->isVisible())
		ProjectData::helpManual->show();
	else
		ProjectData::helpManual->raise();
	ProjectData::helpManual->activateWindow();
}

void RandomMapGenerator_Building::on_buttonBox_accepted()
{
	configured = true;
}
