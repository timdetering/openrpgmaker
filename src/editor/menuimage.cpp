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
 *  Copyright (C) 2011, Justin Davis <tuxdavis@gmail.com>                   *
 ****************************************************************************/

#include <QPixmap>
#include <QPainter>
#include <QBitmap>
#include "menuimage.h"
#include "menuobject.h"
#include "menuscrollarea.h"
#include "projectdata.h"
#include "projectdata_character.h"
#include "projectdata_class.h"
#include "projectdata_monsteranimation.h"
#include "projectdata_monster.h"
#include "projectdata_tileset.h"
#include "projectdata_tile.h"
#include "projectdata_vehicle.h"
#include "storagefile.h"
#include "xmlParser.h"
#include "xmlTools.h"

MenuImage::MenuImage(MenuObject *parent) : MenuObject(parent)
{
	objectName = "image";
	imageType = TYPE_FILE;
	imageLocation.fileLocation = new QString("");
	type = MenuObject::TYPE_IMAGE;
	x = 0;
	y = 0;
	width = 64;
	height = 64;
	numFrames = 0;
	scaledToWidth = 0;
	scaledToHeight = 0;
	scaled = false;
	saveSlotID = 0;
}

MenuImage::MenuImage(MainMenuLayout *layout, XMLNode ObjectNode, MenuObject *parent) : MenuObject(layout, ObjectNode, parent)
{
	type = MenuObject::TYPE_IMAGE;
	loadXMLData(ObjectNode);
}

MenuImage::MenuImage(PopupMenuLayout *layout, XMLNode ObjectNode, MenuObject *parent) : MenuObject(layout, ObjectNode, parent)
{
	type = MenuObject::TYPE_IMAGE;
	loadXMLData(ObjectNode);
}

MenuImage::MenuImage(TitleMenuLayout *layout, XMLNode ObjectNode, MenuObject *parent) : MenuObject(layout, ObjectNode, parent)
{
	type = MenuObject::TYPE_IMAGE;
	loadXMLData(ObjectNode);
}

MenuImage::MenuImage(MainMenuLayout *layout, StorageFile &storageFile, MenuObject *parent) : MenuObject(layout, storageFile, parent)
{
	type = MenuObject::TYPE_IMAGE;
	loadStorageData(storageFile);
}

MenuImage::MenuImage(PopupMenuLayout *layout, StorageFile &storageFile, MenuObject *parent) : MenuObject(layout, storageFile, parent)
{
	type = MenuObject::TYPE_IMAGE;
	loadStorageData(storageFile);
}

MenuImage::MenuImage(TitleMenuLayout *layout, StorageFile &storageFile, MenuObject *parent) : MenuObject(layout, storageFile, parent)
{
	type = MenuObject::TYPE_IMAGE;
	loadStorageData(storageFile);
}

MenuImage::~MenuImage()
{
	if (imageType == TYPE_PARTYMEMBER)
		delete imageLocation.partyMemberImage;
	if (imageType == TYPE_CHARACTER)
		delete imageLocation.characterImage;
	if (imageType == TYPE_CLASS)
		delete imageLocation.classImage;
	if (imageType == TYPE_MONSTER)
		delete imageLocation.monsterImage;
	if (imageType == TYPE_VEHICLE)
		delete imageLocation.vehicleImage;
	if (imageType == TYPE_TILE)
		delete imageLocation.tileImage;
	if (imageType == TYPE_FILE)
		delete imageLocation.fileLocation;
}

void MenuImage::getCopy(MenuObject *object)
{
	MenuImage *image = (MenuImage*)object;
	MenuObject::getCopy(object);
	image->setImageType(imageType);
	if (imageType == TYPE_PARTYMEMBER)
		image->setImageLocation(imageLocation.partyMemberImage);
	else if (imageType == TYPE_CHARACTER)
		image->setImageLocation(imageLocation.characterImage);
	else if (imageType == TYPE_CLASS)
		image->setImageLocation(imageLocation.classImage);
	else if (imageType == TYPE_MONSTER)
		image->setImageLocation(imageLocation.monsterImage);
	else if (imageType == TYPE_VEHICLE)
		image->setImageLocation(imageLocation.vehicleImage);
	else if (imageType == TYPE_TILE)
		image->setImageLocation(imageLocation.tileImage);
	else if (imageType == TYPE_FILE)
		image->setImageLocation(*(imageLocation.fileLocation));
	image->setScaledToWidth(scaledToWidth);
	image->setScaledToHeight(scaledToHeight);
	image->setSaveSlotID(saveSlotID);
	image->setScaled(scaled);
}

XMLNode MenuImage::getObjectNode()
{
	XMLNode ObjectNode = XMLNode::createXMLTopNode("object");
	ObjectNode.addAttribute("type", "image");
	ObjectNode.addAttribute("objectName", objectName.toUtf8().data());
	ObjectNode.addAttribute("x", QString::number(x).toUtf8().data());
	ObjectNode.addAttribute("y", QString::number(y).toUtf8().data());
	ObjectNode.addAttribute("scaledToWidth", QString::number(scaledToWidth).toUtf8().data());
	ObjectNode.addAttribute("scaledToHeight", QString::number(scaledToHeight).toUtf8().data());
	ObjectNode.addAttribute("saveSlotID", QString::number(saveSlotID).toUtf8().data());
	ObjectNode.addAttribute("scaled", (scaled) ? "true":"false");
	if (imageType == TYPE_PARTYMEMBER)
	{
		ObjectNode.addAttribute("imageType", "party member");
		ObjectNode.addAttribute("memberID", QString::number(imageLocation.partyMemberImage->memberID).toUtf8().data());
		if (imageLocation.partyMemberImage->graphic == GRAPHIC_FACE)
			ObjectNode.addAttribute("graphic", "face");
		else if (imageLocation.partyMemberImage->graphic == GRAPHIC_MAPSPRITE)
		{
			ObjectNode.addAttribute("graphic", "map sprite");
			ObjectNode.addAttribute("direction", QString::number(imageLocation.partyMemberImage->direction).toUtf8().data());
			ObjectNode.addAttribute("pose", QString::number(imageLocation.partyMemberImage->pose).toUtf8().data());
		}
		else if (imageLocation.partyMemberImage->graphic == GRAPHIC_BATTLESPRITE)
		{
			ObjectNode.addAttribute("graphic", "battle sprite");
			ObjectNode.addAttribute("pose", QString::number(imageLocation.partyMemberImage->pose).toUtf8().data());
		}
	}
	else if (imageType == TYPE_CHARACTER)
	{
		ObjectNode.addAttribute("imageType", "character");
		ObjectNode.addAttribute("characterID", QString::number(imageLocation.characterImage->characterID).toUtf8().data());
		if (imageLocation.characterImage->graphic == GRAPHIC_FACE)
			ObjectNode.addAttribute("graphic", "face");
		else if (imageLocation.characterImage->graphic == GRAPHIC_MAPSPRITE)
		{
			ObjectNode.addAttribute("graphic", "map sprite");
			ObjectNode.addAttribute("direction", QString::number(imageLocation.characterImage->direction).toUtf8().data());
			ObjectNode.addAttribute("pose", QString::number(imageLocation.characterImage->pose).toUtf8().data());
		}
		else if (imageLocation.characterImage->graphic == GRAPHIC_BATTLESPRITE)
		{
			ObjectNode.addAttribute("graphic", "battle sprite");
			ObjectNode.addAttribute("pose", QString::number(imageLocation.characterImage->pose).toUtf8().data());
		}
		ObjectNode.addAttribute("idIsReferencedInVariable", (imageLocation.characterImage->idIsReferencedInVariable) ? "true":"false");
	}
	else if (imageType == TYPE_CLASS)
	{
		ObjectNode.addAttribute("imageType", "class");
		ObjectNode.addAttribute("classID", QString::number(imageLocation.classImage->classID).toUtf8().data());
		if (imageLocation.classImage->graphic == GRAPHIC_MAPSPRITE)
		{
			ObjectNode.addAttribute("graphic", "map sprite");
			ObjectNode.addAttribute("direction", QString::number(imageLocation.classImage->direction).toUtf8().data());
			ObjectNode.addAttribute("pose", QString::number(imageLocation.classImage->pose).toUtf8().data());
		}
		else if (imageLocation.classImage->graphic == GRAPHIC_BATTLESPRITE)
		{
			ObjectNode.addAttribute("graphic", "battle sprite");
			ObjectNode.addAttribute("pose", QString::number(imageLocation.classImage->pose).toUtf8().data());
		}
		ObjectNode.addAttribute("idIsReferencedInVariable", (imageLocation.classImage->idIsReferencedInVariable) ? "true":"false");
	}
	else if (imageType == TYPE_MONSTER)
	{
		ObjectNode.addAttribute("imageType", "monster");
		ObjectNode.addAttribute("monsterID", QString::number(imageLocation.monsterImage->monsterID).toUtf8().data());
		ObjectNode.addAttribute("pose", QString::number(imageLocation.monsterImage->pose).toUtf8().data());
		ObjectNode.addAttribute("idIsReferencedInVariable", (imageLocation.monsterImage->idIsReferencedInVariable) ? "true":"false");
	}
	else if (imageType == TYPE_VEHICLE)
	{
		ObjectNode.addAttribute("imageType", "vehicle");
		ObjectNode.addAttribute("vehicleID", QString::number(imageLocation.vehicleImage->vehicleID).toUtf8().data());
		ObjectNode.addAttribute("direction", QString::number(imageLocation.vehicleImage->direction).toUtf8().data());
		ObjectNode.addAttribute("pose", QString::number(imageLocation.vehicleImage->pose).toUtf8().data());
		ObjectNode.addAttribute("idIsReferencedInVariable", (imageLocation.vehicleImage->idIsReferencedInVariable) ? "true":"false");
	}
	else if (imageType == TYPE_TILE)
	{
		ObjectNode.addAttribute("imageType", "tile");
		ObjectNode.addAttribute("tilesetID", QString::number(imageLocation.tileImage->tilesetID).toUtf8().data());
		ObjectNode.addAttribute("tileID", QString::number(imageLocation.tileImage->tileID).toUtf8().data());
		ObjectNode.addAttribute("idsReferencedInVariables", (imageLocation.tileImage->idsReferencedInVariables) ? "true":"false");
	}
	else if (imageType == TYPE_FILE)
	{
		ObjectNode.addAttribute("imageType", "file");
		ObjectNode.addAttribute("fileLocation", ProjectData::getRelativeResourcePath(*(imageLocation.fileLocation)).toUtf8().data());
	}
	return ObjectNode;
}

void MenuImage::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(type);
	storageFile.putString(objectName.toUtf8().data());
	storageFile.putSignedInt(x);
	storageFile.putSignedInt(y);
	storageFile.putSignedInt(scaledToWidth);
	storageFile.putSignedInt(scaledToHeight);
	storageFile.putSignedInt(saveSlotID);
	storageFile.putBool(scaled);
	storageFile.putSignedInt(imageType);
	if (imageType == TYPE_PARTYMEMBER)
	{
		storageFile.putSignedInt(imageLocation.partyMemberImage->memberID);
		storageFile.putSignedInt(imageLocation.partyMemberImage->graphic);
		storageFile.putSignedInt(imageLocation.partyMemberImage->direction);
		storageFile.putSignedInt(imageLocation.partyMemberImage->pose);
	}
	else if (imageType == TYPE_CHARACTER)
	{
		storageFile.putSignedInt(imageLocation.characterImage->characterID);
		storageFile.putSignedInt(imageLocation.characterImage->graphic);
		storageFile.putSignedInt(imageLocation.characterImage->direction);
		storageFile.putSignedInt(imageLocation.characterImage->pose);
		storageFile.putBool(imageLocation.characterImage->idIsReferencedInVariable);
	}
	else if (imageType == TYPE_CLASS)
	{
		storageFile.putSignedInt(imageLocation.classImage->classID);
		storageFile.putSignedInt(imageLocation.classImage->graphic);
		storageFile.putSignedInt(imageLocation.classImage->direction);
		storageFile.putSignedInt(imageLocation.classImage->pose);
		storageFile.putBool(imageLocation.classImage->idIsReferencedInVariable);
	}
	else if (imageType == TYPE_MONSTER)
	{
		storageFile.putSignedInt(imageLocation.monsterImage->monsterID);
		storageFile.putSignedInt(imageLocation.monsterImage->pose);
		storageFile.putBool(imageLocation.monsterImage->idIsReferencedInVariable);
	}
	else if (imageType == TYPE_VEHICLE)
	{
		storageFile.putSignedInt(imageLocation.vehicleImage->vehicleID);
		storageFile.putSignedInt(imageLocation.vehicleImage->direction);
		storageFile.putSignedInt(imageLocation.vehicleImage->pose);
		storageFile.putBool(imageLocation.vehicleImage->idIsReferencedInVariable);
	}
	else if (imageType == TYPE_TILE)
	{
		storageFile.putSignedInt(imageLocation.tileImage->tilesetID);
		storageFile.putSignedInt(imageLocation.tileImage->tileID);
		storageFile.putBool(imageLocation.tileImage->idsReferencedInVariables);
	}
	else if (imageType == TYPE_FILE)
		storageFile.putString(imageLocation.fileLocation->toUtf8().data());
}

QStringList MenuImage::getResources()
{
	if (imageType == TYPE_FILE)
		return QStringList() << *(imageLocation.fileLocation);
	return QStringList();
}

int MenuImage::getImageType()
{
	return imageType;
}

void MenuImage::setImageType(int value)
{
	if (imageType != value)
	{
		if (imageType == TYPE_PARTYMEMBER)
			delete imageLocation.partyMemberImage;
		if (imageType == TYPE_CHARACTER)
			delete imageLocation.characterImage;
		if (imageType == TYPE_CLASS)
			delete imageLocation.classImage;
		if (imageType == TYPE_MONSTER)
			delete imageLocation.monsterImage;
		if (imageType == TYPE_VEHICLE)
			delete imageLocation.vehicleImage;
		if (imageType == TYPE_TILE)
			delete imageLocation.tileImage;
		if (imageType == TYPE_FILE)
			delete imageLocation.fileLocation;
		imageType = value;
		if (imageType == TYPE_PARTYMEMBER)
			imageLocation.partyMemberImage = new PartyMemberImage;
		if (imageType == TYPE_CHARACTER)
			imageLocation.characterImage = new CharacterImage;
		if (imageType == TYPE_CLASS)
			imageLocation.classImage = new ClassImage;
		if (imageType == TYPE_MONSTER)
			imageLocation.monsterImage = new MonsterImage;
		if (imageType == TYPE_VEHICLE)
			imageLocation.vehicleImage = new VehicleImage;
		if (imageType == TYPE_TILE)
			imageLocation.tileImage = new TileImage;
		if (imageType == TYPE_FILE)
			imageLocation.fileLocation = new QString("");
	}
}

void MenuImage::getImageLocation(PartyMemberImage *image)
{
	image->memberID = imageLocation.partyMemberImage->memberID;
	image->graphic = imageLocation.partyMemberImage->graphic;
	image->direction = imageLocation.partyMemberImage->direction;
	image->pose = imageLocation.partyMemberImage->pose;
}

void MenuImage::getImageLocation(CharacterImage *image)
{
	image->characterID = imageLocation.characterImage->characterID;
	image->graphic = imageLocation.characterImage->graphic;
	image->direction = imageLocation.characterImage->direction;
	image->pose = imageLocation.characterImage->pose;
	image->idIsReferencedInVariable = imageLocation.characterImage->idIsReferencedInVariable;
}

void MenuImage::getImageLocation(ClassImage *image)
{
	image->classID = imageLocation.classImage->classID;
	image->graphic = imageLocation.classImage->graphic;
	image->direction = imageLocation.classImage->direction;
	image->pose = imageLocation.classImage->pose;
	image->idIsReferencedInVariable = imageLocation.classImage->idIsReferencedInVariable;
}

void MenuImage::getImageLocation(MonsterImage *image)
{
	image->monsterID = imageLocation.monsterImage->monsterID;
	image->pose = imageLocation.monsterImage->pose;
	image->idIsReferencedInVariable = imageLocation.monsterImage->idIsReferencedInVariable;
}

void MenuImage::getImageLocation(VehicleImage *image)
{
	image->vehicleID = imageLocation.vehicleImage->vehicleID;
	image->direction = imageLocation.vehicleImage->direction;
	image->pose = imageLocation.vehicleImage->pose;
	image->idIsReferencedInVariable = imageLocation.vehicleImage->idIsReferencedInVariable;
}

void MenuImage::getImageLocation(TileImage *image)
{
	image->tilesetID = imageLocation.tileImage->tilesetID;
	image->tileID = imageLocation.tileImage->tileID;
	image->idsReferencedInVariables = imageLocation.tileImage->idsReferencedInVariables;
}

void MenuImage::getImageLocation(QString *fileLocation)
{
	fileLocation->clear();
	fileLocation->append(*(imageLocation.fileLocation));
}

void MenuImage::setImageLocation(PartyMemberImage *image)
{
	imageLocation.partyMemberImage->memberID = image->memberID;
	imageLocation.partyMemberImage->graphic = image->graphic;
	imageLocation.partyMemberImage->direction = image->direction;
	imageLocation.partyMemberImage->pose = image->pose;
	updatePixmap();
}

void MenuImage::setImageLocation(CharacterImage *image)
{
	imageLocation.characterImage->characterID = image->characterID;
	imageLocation.characterImage->graphic = image->graphic;
	imageLocation.characterImage->direction = image->direction;
	imageLocation.characterImage->pose = image->pose;
	imageLocation.characterImage->idIsReferencedInVariable = image->idIsReferencedInVariable;
	updatePixmap();
}

void MenuImage::setImageLocation(ClassImage *image)
{
	imageLocation.classImage->classID = image->classID;
	imageLocation.classImage->graphic = image->graphic;
	imageLocation.classImage->direction = image->direction;
	imageLocation.classImage->pose = image->pose;
	imageLocation.classImage->idIsReferencedInVariable = image->idIsReferencedInVariable;
	updatePixmap();
}

void MenuImage::setImageLocation(MonsterImage *image)
{
	imageLocation.monsterImage->monsterID = image->monsterID;
	imageLocation.monsterImage->pose = image->pose;
	imageLocation.monsterImage->idIsReferencedInVariable = image->idIsReferencedInVariable;
	updatePixmap();
}

void MenuImage::setImageLocation(VehicleImage *image)
{
	imageLocation.vehicleImage->vehicleID = image->vehicleID;
	imageLocation.vehicleImage->direction = image->direction;
	imageLocation.vehicleImage->pose = image->pose;
	imageLocation.vehicleImage->idIsReferencedInVariable = image->idIsReferencedInVariable;
	updatePixmap();
}

void MenuImage::setImageLocation(TileImage *image)
{
	imageLocation.tileImage->tilesetID = image->tilesetID;
	imageLocation.tileImage->tileID = image->tileID;
	imageLocation.tileImage->idsReferencedInVariables = image->idsReferencedInVariables;
	updatePixmap();
}

void MenuImage::setImageLocation(QString fileLocation)
{
	imageLocation.fileLocation->clear();
	imageLocation.fileLocation->append(fileLocation);
	updatePixmap();
}

int MenuImage::getScaledToWidth()
{
	return scaledToWidth;
}

void MenuImage::setScaledToWidth(int value)
{
	scaledToWidth = value;
	if (scaled)
		updatePixmap();
}

int MenuImage::getScaledToHeight()
{
	return scaledToHeight;
}

void MenuImage::setScaledToHeight(int value)
{
	scaledToHeight = value;
	if (scaled)
		updatePixmap();
}

int MenuImage::getSaveSlotID()
{
	return saveSlotID;
}

void MenuImage::setSaveSlotID(int value)
{
	saveSlotID = value;
}

bool MenuImage::isScaled()
{
	return scaled;
}

void MenuImage::setScaled(bool value)
{
	scaled = value;
	updatePixmap();
}

bool MenuImage::tryLoad(XMLNode ObjectNode)
{
	QStringList trueFalseList;
	trueFalseList << "true" << "false";
	if (!XMLTools::attributeExists(ObjectNode, "x"))
		return false;
	if (!XMLTools::attributeExists(ObjectNode, "y"))
		return false;
	if (!XMLTools::attributeExists(ObjectNode, "scaledToWidth"))
		return false;
	if (!XMLTools::attributeExists(ObjectNode, "scaledToHeight"))
		return false;
	if (!XMLTools::attributeExists(ObjectNode, "saveSlotID"))
		return false;
	if (!XMLTools::attributeIntRangeValid(ObjectNode, "saveSlotID", 0, 16))
		return false;
	if (!XMLTools::attributeExists(ObjectNode, "scaled"))
		return false;
	if (!XMLTools::attributeStringValid(ObjectNode, "scaled", trueFalseList, 1))
		return false;
	return MenuImage::tryLoadImageLocation(ObjectNode);
}

bool MenuImage::tryLoadImageLocation(XMLNode ImageLocationNode)
{
	QStringList tempStringList;
	QStringList trueFalseList;
	QString temp_string;
	trueFalseList << "true" << "false";
	tempStringList << "party member";
	tempStringList << "character";
	tempStringList << "class";
	tempStringList << "monster";
	tempStringList << "vehicle";
	tempStringList << "tile";
	tempStringList << "file";
	if (!XMLTools::attributeExists(ImageLocationNode, "imageType"))
		return false;
	if (!XMLTools::attributeStringValid(ImageLocationNode, "imageType", tempStringList))
		return false;
	temp_string = ImageLocationNode.getAttribute("imageType");
	if (temp_string == "party member")
	{
		tempStringList.clear();
		tempStringList << "face";
		tempStringList << "map sprite";
		tempStringList << "battle sprite";
		if (!XMLTools::attributeExists(ImageLocationNode, "memberID"))
			return false;
		if (!XMLTools::attributeExists(ImageLocationNode, "graphic"))
			return false;
		if (!XMLTools::attributeStringValid(ImageLocationNode, "graphic", tempStringList))
			return false;
		temp_string = ImageLocationNode.getAttribute("graphic");
		if (temp_string == "map sprite")
		{
			if (!XMLTools::attributeExists(ImageLocationNode, "direction"))
				return false;
			if (!XMLTools::attributeExists(ImageLocationNode, "pose"))
				return false;
		}
		if (temp_string == "battle sprite" && !XMLTools::attributeExists(ImageLocationNode, "pose"))
			return false;
	}
	else if (temp_string == "character")
	{
		tempStringList.clear();
		tempStringList << "face";
		tempStringList << "map sprite";
		tempStringList << "battle sprite";
		if (!XMLTools::attributeExists(ImageLocationNode, "characterID"))
			return false;
		if (!XMLTools::attributeExists(ImageLocationNode, "graphic"))
			return false;
		if (!XMLTools::attributeStringValid(ImageLocationNode, "graphic", tempStringList))
			return false;
		temp_string = ImageLocationNode.getAttribute("graphic");
		if (temp_string == "map sprite")
		{
			if (!XMLTools::attributeExists(ImageLocationNode, "direction"))
				return false;
			if (!XMLTools::attributeExists(ImageLocationNode, "pose"))
				return false;
		}
		if (temp_string == "battle sprite" && !XMLTools::attributeExists(ImageLocationNode, "pose"))
			return false;
		if (!XMLTools::attributeExists(ImageLocationNode, "idIsReferencedInVariable"))
			return false;
		if (!XMLTools::attributeStringValid(ImageLocationNode, "idIsReferencedInVariable", trueFalseList, 1))
			return false;
	}
	else if (temp_string == "class")
	{
		if (!XMLTools::attributeExists(ImageLocationNode, "classID"))
			return false;
		tempStringList.clear();
		tempStringList << "map sprite";
		tempStringList << "battle sprite";
		if (!XMLTools::attributeExists(ImageLocationNode, "graphic"))
			return false;
		if (!XMLTools::attributeStringValid(ImageLocationNode, "graphic", tempStringList))
			return false;
		temp_string = ImageLocationNode.getAttribute("graphic");
		if (temp_string == "map sprite")
		{
			if (!XMLTools::attributeExists(ImageLocationNode, "direction"))
				return false;
			if (!XMLTools::attributeExists(ImageLocationNode, "pose"))
				return false;
		}
		if (temp_string == "battle sprite" && !XMLTools::attributeExists(ImageLocationNode, "pose"))
			return false;
		if (!XMLTools::attributeExists(ImageLocationNode, "idIsReferencedInVariable"))
			return false;
		if (!XMLTools::attributeStringValid(ImageLocationNode, "idIsReferencedInVariable", trueFalseList, 1))
			return false;
	}
	else if (temp_string == "monster")
	{
		if (!XMLTools::attributeExists(ImageLocationNode, "monsterID"))
			return false;
		if (!XMLTools::attributeExists(ImageLocationNode, "pose"))
			return false;
		if (!XMLTools::attributeExists(ImageLocationNode, "idIsReferencedInVariable"))
			return false;
		if (!XMLTools::attributeStringValid(ImageLocationNode, "idIsReferencedInVariable", trueFalseList, 1))
			return false;
	}
	else if (temp_string == "vehicle")
	{
		if (!XMLTools::attributeExists(ImageLocationNode, "monsterID"))
			return false;
		if (!XMLTools::attributeExists(ImageLocationNode, "direction"))
			return false;
		if (!XMLTools::attributeExists(ImageLocationNode, "pose"))
			return false;
		if (!XMLTools::attributeExists(ImageLocationNode, "idIsReferencedInVariable"))
			return false;
		if (!XMLTools::attributeStringValid(ImageLocationNode, "idIsReferencedInVariable", trueFalseList, 1))
			return false;
	}
	else if (temp_string == "tile")
	{
		if (!XMLTools::attributeExists(ImageLocationNode, "tilesetID"))
			return false;
		if (!XMLTools::attributeExists(ImageLocationNode, "tileID"))
			return false;
		if (!XMLTools::attributeExists(ImageLocationNode, "idsReferencedInVariables"))
			return false;
		if (!XMLTools::attributeStringValid(ImageLocationNode, "idsReferencedInVariables", trueFalseList, 1))
			return false;
		temp_string = ImageLocationNode.getAttribute("idsReferencedInVariables");
		if (temp_string == "false")
		{
			if (!XMLTools::attributeIntRangeValid(ImageLocationNode, "tilesetID", 0, 255))
				return false;
			if (!XMLTools::attributeIntRangeValid(ImageLocationNode, "tileID", 0, 255))
				return false;
		}
	}
	else if (temp_string == "file")
	{
		if (!XMLTools::attributeExists(ImageLocationNode, "fileLocation"))
			return false;
	}
	return true;
}

void MenuImage::updatePixmap()
{
	QPixmap pixmap;
	QPainter painter;
	QString fileLocation = "";
	if (imageType == TYPE_PARTYMEMBER)
	{
		ProjectData::DataReference<Character> *charRef;
		int charID;
		if (imageLocation.partyMemberImage->memberID >= 0 && imageLocation.partyMemberImage->memberID <= 3)
			charID = ProjectData::initialParty[imageLocation.partyMemberImage->memberID];
		else
			charID = ProjectData::initialParty[0];
		if (charID >= 0 && charID < ProjectData::characterNames.size())
		{
			charRef = new ProjectData::DataReference<Character>(charID, __FILE__, __LINE__);
			if (imageLocation.partyMemberImage->graphic == GRAPHIC_FACE)
			{
				fileLocation = (*charRef)->getFaceGraphicLocation();
				if (fileLocation.isEmpty())
				{
					pixmap = QPixmap(96, 96);
					pixmap.fill(QColor(255, 0, 255));
				}
				else
					pixmap = QPixmap(fileLocation);
				numFrames = 1;
			}
			else if (imageLocation.partyMemberImage->graphic == GRAPHIC_MAPSPRITE)
			{
				TODO("Get character sprite's map sprite image.");
				pixmap = QPixmap(48, 32);
				pixmap.fill(QColor(255, 0, 255));
				numFrames = 1;
			}
			else
			{
				TODO("Get character sprite's battle sprite image.");
				pixmap = QPixmap(32, 32);
				pixmap.fill(QColor(255, 0, 255));
			}
			delete charRef;
		}
		else
		{
			if (imageLocation.partyMemberImage->graphic == GRAPHIC_FACE)
			{
				pixmap = QPixmap(96, 96);
				pixmap.fill(QColor(255, 0, 255));
			}
			else if (imageLocation.partyMemberImage->graphic == GRAPHIC_MAPSPRITE)
			{
				pixmap = QPixmap(48, 32);
				pixmap.fill(QColor(255, 0, 255));
			}
			else
			{
				pixmap = QPixmap(32, 32);
				pixmap.fill(QColor(255, 0, 255));
			}
		}
	}
	else if (imageType == TYPE_CHARACTER)
	{
		ProjectData::DataReference<Character> *charRef;
		int charID;
		if (imageLocation.characterImage->idIsReferencedInVariable)
			charID = 0;
		else
			charID = imageLocation.characterImage->characterID;
		if (charID >= 0 && charID < ProjectData::characterNames.size())
		{
			charRef = new ProjectData::DataReference<Character>(charID, __FILE__, __LINE__);
			if (imageLocation.characterImage->graphic == GRAPHIC_FACE)
			{
				fileLocation = (*charRef)->getFaceGraphicLocation();
				if (fileLocation.isEmpty())
				{
					pixmap = QPixmap(96, 96);
					pixmap.fill(QColor(255, 0, 255));
				}
				else
					pixmap = QPixmap(fileLocation);
				numFrames = 1;
			}
			else if (imageLocation.characterImage->graphic == GRAPHIC_MAPSPRITE)
			{
				TODO("Get character sprite's map sprite image.");
				pixmap = QPixmap(48, 32);
				pixmap.fill(QColor(255, 0, 255));
				numFrames = 1;
			}
			else
			{
				TODO("Get character sprite's battle sprite image.");
				pixmap = QPixmap(32, 32);
				pixmap.fill(QColor(255, 0, 255));
			}
			delete charRef;
		}
		else
		{
			if (imageLocation.characterImage->graphic == GRAPHIC_FACE)
			{
				pixmap = QPixmap(96, 96);
				pixmap.fill(QColor(255, 0, 255));
			}
			else if (imageLocation.characterImage->graphic == GRAPHIC_MAPSPRITE)
			{
				pixmap = QPixmap(48, 32);
				pixmap.fill(QColor(255, 0, 255));
			}
			else
			{
				pixmap = QPixmap(32, 32);
				pixmap.fill(QColor(255, 0, 255));
			}
		}
	}
	else if (imageType == TYPE_CLASS)
	{
		int id;
		if (imageLocation.classImage->idIsReferencedInVariable)
			id = 0;
		else
			id = imageLocation.classImage->classID;
		if (id >= 0 && id < ProjectData::classNames.size())
		{
			ProjectData::DataReference<Class> *classRef = new ProjectData::DataReference<Class>(id, __FILE__, __LINE__);
			if (imageLocation.classImage->graphic == GRAPHIC_MAPSPRITE)
			{
				TODO("Get character sprite's map sprite image.");
				pixmap = QPixmap(48, 32);
				pixmap.fill(QColor(255, 0, 255));
				numFrames = 1;
			}
			else
			{
				TODO("Get character sprite's battle sprite image.");
				pixmap = QPixmap(32, 32);
				pixmap.fill(QColor(255, 0, 255));
			}
			delete classRef;
		}
		else
		{
			if (imageLocation.classImage->graphic == GRAPHIC_MAPSPRITE)
			{
				pixmap = QPixmap(48, 32);
				pixmap.fill(QColor(255, 0, 255));
			}
			else
			{
				pixmap = QPixmap(32, 32);
				pixmap.fill(QColor(255, 0, 255));
			}
		}
	}
	else if (imageType == TYPE_MONSTER)
	{
		int id;
		if (imageLocation.monsterImage->idIsReferencedInVariable)
			id = 0;
		else
			id = imageLocation.monsterImage->monsterID;
		if (id >= 0 && id < ProjectData::monsterNames.size())
		{
			ProjectData::DataReference<Monster> *monsterRef;
			ProjectData::DataReference<MonsterAnimation> *animRef;
			QImage monsterImage;
			QColor pixelRGB, pixelHSV;
			QRgb pixel, tcolor = qRgb(255, 0, 255);
			int animID, hueShift;
			monsterRef = new ProjectData::DataReference<Monster>(id, __FILE__, __LINE__);
			animID = (*monsterRef)->getMonsterAnimationID();
			hueShift = (*monsterRef)->getHueShift();
			delete monsterRef;
			animRef = new ProjectData::DataReference<MonsterAnimation>(animID, __FILE__, __LINE__);
			numFrames = (*animRef)->getPoseNumFrames(imageLocation.monsterImage->pose);
			monsterImage = QImage((*animRef)->getPoseLocation(imageLocation.monsterImage->pose));
			monsterImage = monsterImage.copy(0, 0, monsterImage.width() / numFrames, monsterImage.height());
			for (int y = 0; y < monsterImage.height(); ++y)
			{
				for (int x = 0; x < monsterImage.width(); ++x)
				{
					pixel = monsterImage.pixel(x, y);
					if (pixel != tcolor)
					{
						pixelRGB = QColor::fromRgba(pixel);
						pixelHSV = pixelRGB.toHsv();
						pixelHSV.setHsv((pixelHSV.hue() + hueShift) % 360, pixelHSV.saturation(), pixelHSV.value(), 255);
						pixelRGB = pixelHSV.toRgb();
						monsterImage.setPixel(x, y, pixelRGB.rgb());
					}
				}
			}
			delete animRef;
		}
		else
		{
			pixmap = QPixmap(32, 32);
			pixmap.fill(QColor(255, 0, 255));
		}
	}
	else if (imageType == TYPE_VEHICLE)
	{
		int vehicleID;
		if (imageLocation.vehicleImage->idIsReferencedInVariable)
			vehicleID = 0;
		else
			vehicleID = imageLocation.vehicleImage->vehicleID;
		if (vehicleID >= 0 && vehicleID < ProjectData::vehicleNames.size())
		{
			ProjectData::DataReference<Vehicle> *vehicleRef = new ProjectData::DataReference<Vehicle>(vehicleID, __FILE__, __LINE__);
			TODO("Get character sprite's map sprite image.");
			pixmap = QPixmap(48, 32);
			pixmap.fill(QColor(255, 0, 255));
			numFrames = 1;
			delete vehicleRef;
		}
	}
	else if (imageType == TYPE_TILE)
	{
		int tilesetID;
		int tileID;
		if (imageLocation.tileImage->idsReferencedInVariables)
		{
			tilesetID = 0;
			tileID = 0;
		}
		else
		{
			tilesetID = imageLocation.tileImage->tilesetID;
			tileID = imageLocation.tileImage->tileID;
		}
		if (tilesetID >= 0 && tilesetID < ProjectData::tilesetNames.size())
		{
			ProjectData::DataReference<Tileset> *tilesetRef = new ProjectData::DataReference<Tileset>(tilesetID, __FILE__, __LINE__);
			if (tileID >= 0 && tileID < (*tilesetRef)->numTiles())
			{
				pixmap = QPixmap::fromImage((*tilesetRef)->getTile(tileID)->getTile());
				numFrames = 1;
			}
			else
			{
				pixmap = QPixmap(32, 32);
				pixmap.fill(QColor(255, 0, 255));
				numFrames = 1;
			}
			delete tilesetRef;
		}
		else
		{
			pixmap = QPixmap(32, 32);
			pixmap.fill(QColor(255, 0, 255));
			numFrames = 1;
		}
	}
	else if (imageType == TYPE_FILE)
	{
		fileLocation = *(imageLocation.fileLocation);
		if (fileLocation.isEmpty())
		{
			pixmap = QPixmap(64, 64);
			pixmap.fill(QColor(255, 0, 255));
		}
		else
			pixmap = QPixmap(fileLocation);
		numFrames = 1;
	}
	if (scaled)
	{
		renderedPixmap = QPixmap(scaledToWidth, scaledToHeight);
		renderedPixmap.fill(QColor(255, 0, 255));
		pixmap = pixmap.scaled(scaledToWidth, scaledToHeight, Qt::KeepAspectRatio);
		painter.begin(&renderedPixmap);
		painter.drawPixmap((scaledToWidth - pixmap.width()) / 2, (scaledToHeight - pixmap.height()) / 2, pixmap);
		painter.end();
	}
	else
		renderedPixmap = pixmap;
	pixmapItem->setPixmap(renderedPixmap);
	if (parentObject == NULL)
		pixmapItem->setPos(x, y);
	else if (parentObject->getType() == MenuObject::TYPE_SCROLLAREA)
	{
		MenuScrollArea *scrollArea = (MenuScrollArea*)parentObject;
		pixmapItem->setPos(x - scrollArea->getCurrentScrollX(), y - scrollArea->getCurrentScrollY());
	}
	else
		pixmapItem->setPos(x, y);
	width = renderedPixmap.width();
	height = renderedPixmap.height();
}

void MenuImage::loadXMLData(XMLNode ObjectNode)
{
	QString temp_string;
	x = QString(ObjectNode.getAttribute("x")).toInt();
	y = QString(ObjectNode.getAttribute("y")).toInt();
	scaledToWidth = QString(ObjectNode.getAttribute("scaledToWidth")).toInt();
	scaledToHeight = QString(ObjectNode.getAttribute("scaledToHeight")).toInt();
	saveSlotID = QString(ObjectNode.getAttribute("saveSlotID")).toInt();
	scaled = (QString(ObjectNode.getAttribute("scaled")) == "true") ? true:false;
	temp_string = ObjectNode.getAttribute("imageType");
	if (temp_string == "party member")
	{
		imageType = TYPE_PARTYMEMBER;
		imageLocation.partyMemberImage = new PartyMemberImage;
		imageLocation.partyMemberImage->memberID = QString(ObjectNode.getAttribute("memberID")).toInt();
		temp_string = ObjectNode.getAttribute("graphic");
		if (temp_string == "face")
		{
			imageLocation.partyMemberImage->graphic = GRAPHIC_FACE;
			imageLocation.partyMemberImage->direction = 0;
			imageLocation.partyMemberImage->pose = 0;
		}
		else if (temp_string == "map sprite")
		{
			imageLocation.partyMemberImage->graphic = GRAPHIC_MAPSPRITE;
			imageLocation.partyMemberImage->direction = QString(ObjectNode.getAttribute("direction")).toInt();
			imageLocation.partyMemberImage->pose = QString(ObjectNode.getAttribute("pose")).toInt();
		}
		else
		{
			imageLocation.partyMemberImage->graphic = GRAPHIC_BATTLESPRITE;
			imageLocation.partyMemberImage->direction = 0;
			imageLocation.partyMemberImage->pose = QString(ObjectNode.getAttribute("pose")).toInt();
		}
	}
	else if (temp_string == "character")
	{
		imageType = TYPE_CHARACTER;
		imageLocation.characterImage = new CharacterImage;
		imageLocation.characterImage->characterID = QString(ObjectNode.getAttribute("characterID")).toInt();
		temp_string = ObjectNode.getAttribute("graphic");
		if (temp_string == "face")
		{
			imageLocation.characterImage->graphic = GRAPHIC_FACE;
			imageLocation.characterImage->direction = 0;
			imageLocation.characterImage->pose = 0;
		}
		else if (temp_string == "map sprite")
		{
			imageLocation.characterImage->graphic = GRAPHIC_MAPSPRITE;
			imageLocation.characterImage->direction = QString(ObjectNode.getAttribute("direction")).toInt();
			imageLocation.characterImage->pose = QString(ObjectNode.getAttribute("pose")).toInt();
		}
		else
		{
			imageLocation.characterImage->graphic = GRAPHIC_BATTLESPRITE;
			imageLocation.characterImage->direction = 0;
			imageLocation.characterImage->pose = QString(ObjectNode.getAttribute("pose")).toInt();
		}
		imageLocation.characterImage->idIsReferencedInVariable = (QString(ObjectNode.getAttribute("idIsReferencedInVariable")) == "true") ? true:false;
	}
	else if (temp_string == "class")
	{
		imageType = TYPE_CLASS;
		imageLocation.classImage = new ClassImage;
		imageLocation.classImage->classID = QString(ObjectNode.getAttribute("classID")).toInt();
		temp_string = ObjectNode.getAttribute("graphic");
		if (temp_string == "map sprite")
		{
			imageLocation.classImage->graphic = GRAPHIC_MAPSPRITE;
			imageLocation.classImage->direction = QString(ObjectNode.getAttribute("direction")).toInt();
			imageLocation.classImage->pose = QString(ObjectNode.getAttribute("pose")).toInt();
		}
		else
		{
			imageLocation.classImage->graphic = GRAPHIC_BATTLESPRITE;
			imageLocation.classImage->direction = 0;
			imageLocation.classImage->pose = QString(ObjectNode.getAttribute("pose")).toInt();
		}
		imageLocation.classImage->idIsReferencedInVariable = (QString(ObjectNode.getAttribute("idIsReferencedInVariable")) == "true") ? true:false;
	}
	else if (temp_string == "monster")
	{
		imageType = TYPE_MONSTER;
		imageLocation.monsterImage = new MonsterImage;
		imageLocation.monsterImage->monsterID = QString(ObjectNode.getAttribute("monsterID")).toInt();
		imageLocation.monsterImage->pose = QString(ObjectNode.getAttribute("pose")).toInt();
		imageLocation.monsterImage->idIsReferencedInVariable = (QString(ObjectNode.getAttribute("idIsReferencedInVariable")) == "true") ? true:false;
	}
	else if (temp_string == "vehicle")
	{
		imageType = TYPE_VEHICLE;
		imageLocation.vehicleImage = new VehicleImage;
		imageLocation.vehicleImage->vehicleID = QString(ObjectNode.getAttribute("vehicleID")).toInt();
		imageLocation.vehicleImage->direction = QString(ObjectNode.getAttribute("direction")).toInt();
		imageLocation.vehicleImage->pose = QString(ObjectNode.getAttribute("pose")).toInt();
		imageLocation.vehicleImage->idIsReferencedInVariable = (QString(ObjectNode.getAttribute("idIsReferencedInVariable")) == "true") ? true:false;
	}
	else if (temp_string == "tile")
	{
		imageType = TYPE_TILE;
		imageLocation.tileImage = new TileImage;
		imageLocation.tileImage->tilesetID = QString(ObjectNode.getAttribute("tilesetID")).toInt();
		imageLocation.tileImage->tileID = QString(ObjectNode.getAttribute("tileID")).toInt();
		imageLocation.tileImage->idsReferencedInVariables = (QString(ObjectNode.getAttribute("idsReferencedInVariables")) == "true") ? true:false;
	}
	else if (temp_string == "file")
	{
		imageType = TYPE_FILE;
		imageLocation.fileLocation = new QString(ProjectData::getAbsoluteResourcePath(ObjectNode.getAttribute("fileLocation")));
	}
}

void MenuImage::loadStorageData(StorageFile &storageFile)
{
	char *temp_string;
	x = storageFile.getSignedInt();
	y = storageFile.getSignedInt();
	scaledToWidth = storageFile.getSignedInt();
	scaledToHeight = storageFile.getSignedInt();
	saveSlotID = storageFile.getSignedInt();
	scaled = storageFile.getBool();
	imageType = storageFile.getSignedInt();
	if (imageType == TYPE_PARTYMEMBER)
	{
		imageLocation.partyMemberImage = new PartyMemberImage;
		imageLocation.partyMemberImage->memberID = storageFile.getSignedInt();
		imageLocation.partyMemberImage->graphic = storageFile.getSignedInt();
		imageLocation.partyMemberImage->direction = storageFile.getSignedInt();
		imageLocation.partyMemberImage->pose = storageFile.getSignedInt();
	}
	else if (imageType == TYPE_CHARACTER)
	{
		imageLocation.characterImage = new CharacterImage;
		imageLocation.characterImage->characterID = storageFile.getSignedInt();
		imageLocation.characterImage->graphic = storageFile.getSignedInt();
		imageLocation.characterImage->direction = storageFile.getSignedInt();
		imageLocation.characterImage->pose = storageFile.getSignedInt();
		imageLocation.characterImage->idIsReferencedInVariable = storageFile.getBool();
	}
	else if (imageType == TYPE_CLASS)
	{
		imageLocation.classImage = new ClassImage;
		imageLocation.classImage->classID = storageFile.getSignedInt();
		imageLocation.classImage->graphic = storageFile.getSignedInt();
		imageLocation.classImage->direction = storageFile.getSignedInt();
		imageLocation.classImage->pose = storageFile.getSignedInt();
		imageLocation.classImage->idIsReferencedInVariable = storageFile.getBool();
	}
	else if (imageType == TYPE_MONSTER)
	{
		imageLocation.monsterImage = new MonsterImage;
		imageLocation.monsterImage->monsterID = storageFile.getSignedInt();
		imageLocation.monsterImage->pose = storageFile.getSignedInt();
		imageLocation.monsterImage->idIsReferencedInVariable = storageFile.getBool();
	}
	else if (imageType == TYPE_VEHICLE)
	{
		imageLocation.vehicleImage = new VehicleImage;
		imageLocation.vehicleImage->vehicleID = storageFile.getSignedInt();
		imageLocation.vehicleImage->direction = storageFile.getSignedInt();
		imageLocation.vehicleImage->pose = storageFile.getSignedInt();
		imageLocation.vehicleImage->idIsReferencedInVariable = storageFile.getBool();
	}
	else if (imageType == TYPE_TILE)
	{
		imageLocation.tileImage = new TileImage;
		imageLocation.tileImage->tilesetID = storageFile.getSignedInt();
		imageLocation.tileImage->tileID = storageFile.getSignedInt();
		imageLocation.tileImage->idsReferencedInVariables = storageFile.getBool();
	}
	else if (imageType == TYPE_FILE)
	{
		temp_string = storageFile.getString();
		imageLocation.fileLocation = new QString(temp_string);
		delete[] temp_string;
	}
}
