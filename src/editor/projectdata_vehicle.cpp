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

#include <QStringList>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "projectdata.h"
#include "projectdata_vehicle.h"
#include "xmlParser.h"
#include "xmlTools.h"

Vehicle::Vehicle()
{
	name = "Untitled";
	bgmInfo.location = "";
	bgmInfo.volume = 1.0;
	bgmInfo.panning = 0.0;
	bgmInfo.speed = 1.0;
	bgmInfo.fadeInTime = 0;
	bgmInfo.retainPitch = true;
	spriteID = 0;
	passingPhase = 0;
	landingPhase = 0;
	flying = false;
}

Vehicle::Vehicle(Vehicle &other)
{
	name = other.name;
	bgmInfo.location = other.bgmInfo.location;
	bgmInfo.volume = other.bgmInfo.volume;
	bgmInfo.panning = other.bgmInfo.panning;
	bgmInfo.speed = other.bgmInfo.speed;
	bgmInfo.fadeInTime = other.bgmInfo.fadeInTime;
	bgmInfo.retainPitch = other.bgmInfo.retainPitch;
	spriteID = other.spriteID;
	passingPhase = other.passingPhase;
	landingPhase = other.landingPhase;
	flying = other.flying;
}

Vehicle::Vehicle(const Vehicle &other)
{
	name = other.name;
	bgmInfo.location = other.bgmInfo.location;
	bgmInfo.volume = other.bgmInfo.volume;
	bgmInfo.panning = other.bgmInfo.panning;
	bgmInfo.speed = other.bgmInfo.speed;
	bgmInfo.fadeInTime = other.bgmInfo.fadeInTime;
	bgmInfo.retainPitch = other.bgmInfo.retainPitch;
	spriteID = other.spriteID;
	passingPhase = other.passingPhase;
	landingPhase = other.landingPhase;
	flying = other.flying;
}

Vehicle::Vehicle(XMLNode VehicleNode)
{
	XMLNode TempNode;
	name = VehicleNode.getAttribute("name");
	spriteID = QString(VehicleNode.getAttribute("sprite")).toInt();
	passingPhase = QString(VehicleNode.getAttribute("passingPhase")).toInt();
	landingPhase = QString(VehicleNode.getAttribute("landingPhase")).toInt();
	flying = (QString(VehicleNode.getAttribute("flying")) == "true") ? true:false;
	TempNode = VehicleNode.getChildNode("bgm");
	bgmInfo.location = ProjectData::getAbsoluteResourcePath(TempNode.getAttribute("location"));
	bgmInfo.volume = QString(TempNode.getAttribute("volume")).toFloat();
	bgmInfo.panning = QString(TempNode.getAttribute("panning")).toFloat();
	bgmInfo.speed = QString(TempNode.getAttribute("speed")).toFloat();
	bgmInfo.fadeInTime = QString(TempNode.getAttribute("fadeInTime")).toInt();
	bgmInfo.retainPitch = (QString(TempNode.getAttribute("retainPitch")) == "true") ? true:false;
}

Vehicle::Vehicle(StorageFile &storageFile)
{
	char *temp_string;
	temp_string = storageFile.getString();
	name = temp_string;
	delete[] temp_string;
	temp_string = storageFile.getString();
	bgmInfo.location = temp_string;
	delete[] temp_string;
	bgmInfo.volume = storageFile.getFloat();
	bgmInfo.panning = storageFile.getFloat();
	bgmInfo.speed = storageFile.getFloat();
	bgmInfo.fadeInTime = storageFile.getSignedInt();
	bgmInfo.retainPitch = storageFile.getBool();
	spriteID = storageFile.getSignedInt();
	passingPhase = storageFile.getSignedInt();
	landingPhase = storageFile.getSignedInt();
	flying = storageFile.getBool();
}


Vehicle::~Vehicle()
{
}

void Vehicle::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putString(name.toUtf8().data());
	storageFile.putString(bgmInfo.location.toUtf8().data());
	storageFile.putFloat(bgmInfo.volume);
	storageFile.putFloat(bgmInfo.panning);
	storageFile.putFloat(bgmInfo.speed);
	storageFile.putSignedInt(bgmInfo.fadeInTime);
	storageFile.putBool(bgmInfo.retainPitch);
	storageFile.putSignedInt(spriteID);
	storageFile.putSignedInt(passingPhase);
	storageFile.putSignedInt(landingPhase);
	storageFile.putBool(flying);
}

QStringList Vehicle::getResources()
{
	QStringList resources;
	resources.append(bgmInfo.location);
	return resources;
}

XMLNode Vehicle::getVehicleNode()
{
	XMLNode TempNode, VehicleNode = XMLNode::createXMLTopNode("vehicle");
	VehicleNode.addAttribute("name", name.toUtf8().data());
	VehicleNode.addAttribute("sprite", QString::number(spriteID).toUtf8().data());
	VehicleNode.addAttribute("passingPhase", QString::number(passingPhase).toUtf8().data());
	VehicleNode.addAttribute("landingPhase", QString::number(landingPhase).toUtf8().data());
	VehicleNode.addAttribute("flying", (flying) ? "true":"false");
	TempNode = VehicleNode.addChild("bgm");
	TempNode.addAttribute("location", ProjectData::getRelativeResourcePath(bgmInfo.location).toUtf8().data());
	TempNode.addAttribute("volume", QString::number(bgmInfo.volume).toUtf8().data());
	TempNode.addAttribute("panning", QString::number(bgmInfo.panning).toUtf8().data());
	TempNode.addAttribute("speed", QString::number(bgmInfo.speed).toUtf8().data());
	TempNode.addAttribute("fadeInTime", QString::number(bgmInfo.fadeInTime).toUtf8().data());
	TempNode.addAttribute("retainPitch", (bgmInfo.retainPitch) ? "true":"false");
	return VehicleNode;
}

QString Vehicle::getName()
{
	return name;
}

void Vehicle::setName(QString value)
{
	name = value;
}

int Vehicle::getSpriteID()
{
	return spriteID;
}

void Vehicle::setSpriteID(int value)
{
	spriteID = value;
}

void Vehicle::getBGMInfo(AudioManager::MusicInfo *info)
{
	info->location = bgmInfo.location;
	info->volume = bgmInfo.volume;
	info->panning = bgmInfo.panning;
	info->speed = bgmInfo.speed;
	info->fadeInTime = bgmInfo.fadeInTime;
	info->retainPitch = bgmInfo.retainPitch;
}

void Vehicle::setBGMInfo(AudioManager::MusicInfo info)
{
	bgmInfo.location = info.location;
	bgmInfo.volume = info.volume;
	bgmInfo.panning = info.panning;
	bgmInfo.speed = info.speed;
	bgmInfo.fadeInTime = info.fadeInTime;
	bgmInfo.retainPitch = info.retainPitch;
}

int Vehicle::getPassingPhase()
{
	return passingPhase;
}

void Vehicle::setPassingPhase(int phase)
{
	passingPhase = phase;
}

int Vehicle::getLandingPhase()
{
	return landingPhase;
}

void Vehicle::setLandingPhase(int phase)
{
	landingPhase = phase;
}

bool Vehicle::isFlying()
{
	return flying;
}

void Vehicle::setIsFlying(bool value)
{
	flying = value;
}

bool Vehicle::tryLoad(XMLNode VehicleNode)
{
	XMLNode TempNode;
	if (!XMLTools::attributeExists(VehicleNode, "name"))
		return false;
	if (!XMLTools::attributeExists(VehicleNode, "sprite"))
		return false;
	if (!XMLTools::attributeExists(VehicleNode, "passingPhase"))
		return false;
	if (!XMLTools::attributeIntRangeValid(VehicleNode, "passingPhase", 0, 23))
		return false;
	if (!XMLTools::attributeExists(VehicleNode, "landingPhase"))
		return false;
	if (!XMLTools::attributeIntRangeValid(VehicleNode, "landingPhase", 0, 23))
		return false;
	if (!XMLTools::attributeExists(VehicleNode, "flying"))
		return false;
	if (!XMLTools::attributeStringValid(VehicleNode, "flying", QStringList() << "true" << "false", 1))
		return false;
	if (!XMLTools::nodeExists(VehicleNode, "bgm"))
		return false;
	TempNode = VehicleNode.getChildNode("bgm");
	if (!XMLTools::attributeExists(TempNode, "location"))
		return false;
	if (!XMLTools::attributeExists(TempNode, "volume"))
		return false;
	if (!XMLTools::attributeFloatRangeValid(TempNode, "volume", 0.0, 1.0))
		return false;
	if (!XMLTools::attributeExists(TempNode, "panning"))
		return false;
	if (!XMLTools::attributeFloatRangeValid(TempNode, "panning", -1.0, 1.0))
		return false;
	if (!XMLTools::attributeExists(TempNode, "speed"))
		return false;
	if (!XMLTools::attributeFloatRangeValid(TempNode, "speed", 0.5, 2.0))
		return false;
	if (!XMLTools::attributeExists(TempNode, "fadeInTime"))
		return false;
	if (!XMLTools::attributeIntRangeValid(TempNode, "fadeInTime", 0, 10))
		return false;
	if (!XMLTools::attributeExists(TempNode, "retainPitch"))
		return false;
	if (!XMLTools::attributeStringValid(TempNode, "retainPitch", QStringList() << "true" << "false", 1))
		return false;
	return true;
}
