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
#include "audiomanager.h"
#include "projectdata.h"
#include "projectdata_terrain.h"
#include "xmlParser.h"
#include "xmlTools.h"

Terrain::GridDepthValues Terrain::gridDepth_ShallowValues = {Terrain::DEPTH_SHALLOW, 224, 314, 128, 96, 50, 512, 544, 50};
Terrain::GridDepthValues Terrain::gridDepth_DeepValues = {Terrain::DEPTH_DEEP, 264, 310, 142, 98, 50, 498, 542, 50};

Terrain::Terrain()
{
	name = "";
	bgImageLocation = "";
	fgImageLocation = "";
	soundEffect.location = "";
	soundEffect.volume = 1.0;
	soundEffect.panning = 0.0;
	soundEffect.speed = 1.0;
	soundEffect.numLoops = 0;
	soundEffect.retainPitch = 0;
	damage = 0;
	encMultiplier = 100;
	gridDepth.depth = DEPTH_SHALLOW;
	gridDepth.hTopLineLocation = gridDepth_ShallowValues.hTopLineLocation;
	gridDepth.hBottomLineLocation = gridDepth_ShallowValues.hBottomLineLocation;
	gridDepth.vMonsterLine_TopLocation = gridDepth_ShallowValues.vMonsterLine_TopLocation;
	gridDepth.vMonsterLine_BottomLocation = gridDepth_ShallowValues.vMonsterLine_BottomLocation;
	gridDepth.vMonsterLine_Spacing = gridDepth_ShallowValues.vMonsterLine_Spacing;
	gridDepth.vCharacterLine_TopLocation = gridDepth_ShallowValues.vCharacterLine_TopLocation;
	gridDepth.vCharacterLine_BottomLocation = gridDepth_ShallowValues.vCharacterLine_BottomLocation;
	gridDepth.vCharacterLine_Spacing = gridDepth_ShallowValues.vCharacterLine_Spacing;
	spriteDisplay = DISPLAY_NORMAL;
	bgHScrollSpeed = 0;
	bgVScrollSpeed = 0;
	fgHScrollSpeed = 0;
	fgVScrollSpeed = 0;
	initPartyMod = 0;
	backAttackEnemyMod = 0;
	surroundAttackPartyMod = 0;
	surroundAttackEnemyMod = 0;
	globalAnimID = -1;
	globalAnimType = TYPE_BACKGROUND;
	soundOnlyIfDamaged = false;
}

Terrain::Terrain(Terrain &other)
{
	name = other.name;
	bgImageLocation = other.bgImageLocation;
	fgImageLocation = other.fgImageLocation;
	soundEffect.location = other.soundEffect.location;
	soundEffect.volume = other.soundEffect.volume;
	soundEffect.panning = other.soundEffect.panning;
	soundEffect.speed = other.soundEffect.speed;
	soundEffect.numLoops = other.soundEffect.numLoops;
	soundEffect.retainPitch = other.soundEffect.retainPitch;
	damage = other.damage;
	encMultiplier = other.encMultiplier;
	gridDepth.hTopLineLocation = other.gridDepth.hTopLineLocation;
	gridDepth.hBottomLineLocation = other.gridDepth.hBottomLineLocation;
	gridDepth.vMonsterLine_TopLocation = other.gridDepth.vMonsterLine_TopLocation;
	gridDepth.vMonsterLine_BottomLocation = other.gridDepth.vMonsterLine_BottomLocation;
	gridDepth.vMonsterLine_Spacing = other.gridDepth.vMonsterLine_Spacing;
	gridDepth.vCharacterLine_TopLocation = other.gridDepth.vCharacterLine_TopLocation;
	gridDepth.vCharacterLine_BottomLocation = other.gridDepth.vCharacterLine_BottomLocation;
	gridDepth.vCharacterLine_Spacing = other.gridDepth.vCharacterLine_Spacing;
	gridDepth.depth = other.gridDepth.depth;
	spriteDisplay = other.spriteDisplay;
	bgHScrollSpeed = other.bgHScrollSpeed;
	bgVScrollSpeed = other.bgVScrollSpeed;
	fgHScrollSpeed = other.fgHScrollSpeed;
	fgVScrollSpeed = other.fgVScrollSpeed;
	initPartyMod = other.initPartyMod;
	backAttackEnemyMod = other.backAttackEnemyMod;
	surroundAttackPartyMod = other.surroundAttackPartyMod;
	surroundAttackEnemyMod = other.surroundAttackEnemyMod;
	globalAnimID = other.globalAnimID;
	globalAnimType = other.globalAnimType;
	soundOnlyIfDamaged = other.soundOnlyIfDamaged;
}

Terrain::Terrain(const Terrain &other)
{
	name = other.name;
	bgImageLocation = other.bgImageLocation;
	fgImageLocation = other.fgImageLocation;
	soundEffect.location = other.soundEffect.location;
	soundEffect.volume = other.soundEffect.volume;
	soundEffect.panning = other.soundEffect.panning;
	soundEffect.speed = other.soundEffect.speed;
	soundEffect.numLoops = other.soundEffect.numLoops;
	soundEffect.retainPitch = other.soundEffect.retainPitch;
	damage = other.damage;
	encMultiplier = other.encMultiplier;
	gridDepth.hTopLineLocation = other.gridDepth.hTopLineLocation;
	gridDepth.hBottomLineLocation = other.gridDepth.hBottomLineLocation;
	gridDepth.vMonsterLine_TopLocation = other.gridDepth.vMonsterLine_TopLocation;
	gridDepth.vMonsterLine_BottomLocation = other.gridDepth.vMonsterLine_BottomLocation;
	gridDepth.vMonsterLine_Spacing = other.gridDepth.vMonsterLine_Spacing;
	gridDepth.vCharacterLine_TopLocation = other.gridDepth.vCharacterLine_TopLocation;
	gridDepth.vCharacterLine_BottomLocation = other.gridDepth.vCharacterLine_BottomLocation;
	gridDepth.vCharacterLine_Spacing = other.gridDepth.vCharacterLine_Spacing;
	gridDepth.depth = other.gridDepth.depth;
	spriteDisplay = other.spriteDisplay;
	bgHScrollSpeed = other.bgHScrollSpeed;
	bgVScrollSpeed = other.bgVScrollSpeed;
	fgHScrollSpeed = other.fgHScrollSpeed;
	fgVScrollSpeed = other.fgVScrollSpeed;
	initPartyMod = other.initPartyMod;
	backAttackEnemyMod = other.backAttackEnemyMod;
	surroundAttackPartyMod = other.surroundAttackPartyMod;
	surroundAttackEnemyMod = other.surroundAttackEnemyMod;
	globalAnimID = other.globalAnimID;
	globalAnimType = other.globalAnimType;
	soundOnlyIfDamaged = other.soundOnlyIfDamaged;
}

Terrain::Terrain(XMLNode TerrainNode)
{
	XMLNode TempNode1, TempNode2;
	char temp_string[512];
	name = TerrainNode.getAttribute("name");
	damage = atoi(TerrainNode.getAttribute("damage"));
	encMultiplier = atoi(TerrainNode.getAttribute("encmultiplier"));
	sprintf(temp_string, "%s", TerrainNode.getAttribute("spritedisplay"));
	if (strcmp(temp_string, "normal") == 0)
		spriteDisplay = DISPLAY_NORMAL;
	else if (strcmp(temp_string, "obscure-bottom-half") == 0)
		spriteDisplay = DISPLAY_OBSBOTHALF;
	else if (strcmp(temp_string, "obscure-bottom-third") == 0)
		spriteDisplay = DISPLAY_OBSBOTTHIRD;
	else if (strcmp(temp_string, "semi-transparent") == 0)
		spriteDisplay = DISPLAY_SEMITRANSPARENT;
	else
	{
		printf("Warning: invalid sprite display \"%s\" detected.\nDefaulting to normal display.\n", temp_string);
		spriteDisplay = DISPLAY_NORMAL;
	}
	TempNode1 = TerrainNode.getChildNode("soundeffect");
	soundOnlyIfDamaged = (strcmp(TempNode1.getAttribute("onlyifdamaged"), "true") == 0);
	soundEffect.location = ProjectData::getAbsoluteResourcePath(TempNode1.getAttribute("location"));
	soundEffect.volume = QString(TempNode1.getAttribute("volume")).toFloat();
	soundEffect.panning = QString(TempNode1.getAttribute("panning")).toFloat();
	soundEffect.speed = QString(TempNode1.getAttribute("speed")).toFloat();
	soundEffect.numLoops = QString(TempNode1.getAttribute("numLoops")).toInt();
	soundEffect.retainPitch = (QString(TempNode1.getAttribute("retainPitch")) == "true") ? true:false;
	TempNode1 = TerrainNode.getChildNode("background");
	backgroundAssociation = (strcmp(TempNode1.getAttribute("association"), "background") == 0) ? BG_BACKGROUND:BG_FRAME;
	TempNode2 = TempNode1.getChildNode("globalanimation");
	globalAnimID = atoi(TempNode2.getAttribute("id"));
	globalAnimType = (strcmp(TempNode2.getAttribute("type"), "background") == 0) ? TYPE_BACKGROUND:TYPE_FOREGROUND;
	TempNode2 = TempNode1.getChildNode("bgimage");
	bgImageLocation = ProjectData::getAbsoluteResourcePath(TempNode2.getAttribute("location"));
	bgHScrollSpeed = atoi(TempNode2.getAttribute("hspeed"));
	bgVScrollSpeed = atoi(TempNode2.getAttribute("vspeed"));
	if (backgroundAssociation == BG_FRAME)
	{
		TempNode2 = TempNode1.getChildNode("fgimage");
		fgImageLocation = ProjectData::getAbsoluteResourcePath(TempNode2.getAttribute("location"));
		fgHScrollSpeed = atoi(TempNode2.getAttribute("hspeed"));
		fgVScrollSpeed = atoi(TempNode2.getAttribute("vspeed"));
	}
	else
	{
		fgImageLocation = "";
		fgHScrollSpeed = 0;
		fgVScrollSpeed = 0;
	}
	TempNode1 = TerrainNode.getChildNode("battletypes");
	initPartyMod = atoi(TempNode1.getAttribute("partyinitiative"));
	backAttackEnemyMod = atoi(TempNode1.getAttribute("enemybackattack"));
	surroundAttackPartyMod = atoi(TempNode1.getAttribute("partysurroundattack"));
	surroundAttackEnemyMod = atoi(TempNode1.getAttribute("enemysurroundattack"));
	TempNode1 = TerrainNode.getChildNode("griddepth");
	sprintf(temp_string, "%s", TempNode1.getAttribute("depth"));
	if (strcmp(temp_string, "shallow") == 0)
	{
		gridDepth.depth = DEPTH_SHALLOW;
		gridDepth.hTopLineLocation = gridDepth_ShallowValues.hTopLineLocation;
		gridDepth.hBottomLineLocation = gridDepth_ShallowValues.hBottomLineLocation;
		gridDepth.vMonsterLine_TopLocation = gridDepth_ShallowValues.vMonsterLine_TopLocation;
		gridDepth.vMonsterLine_BottomLocation = gridDepth_ShallowValues.vMonsterLine_BottomLocation;
		gridDepth.vMonsterLine_Spacing = gridDepth_ShallowValues.vMonsterLine_Spacing;
		gridDepth.vCharacterLine_TopLocation = gridDepth_ShallowValues.vCharacterLine_TopLocation;
		gridDepth.vCharacterLine_BottomLocation = gridDepth_ShallowValues.vCharacterLine_BottomLocation;
		gridDepth.vCharacterLine_Spacing = gridDepth_ShallowValues.vCharacterLine_Spacing;
	}
	else if (strcmp(temp_string, "deep") == 0)
	{
		gridDepth.depth = DEPTH_DEEP;
		gridDepth.hTopLineLocation = gridDepth_DeepValues.hTopLineLocation;
		gridDepth.hBottomLineLocation = gridDepth_DeepValues.hBottomLineLocation;
		gridDepth.vMonsterLine_TopLocation = gridDepth_DeepValues.vMonsterLine_TopLocation;
		gridDepth.vMonsterLine_BottomLocation = gridDepth_DeepValues.vMonsterLine_BottomLocation;
		gridDepth.vMonsterLine_Spacing = gridDepth_DeepValues.vMonsterLine_Spacing;
		gridDepth.vCharacterLine_TopLocation = gridDepth_DeepValues.vCharacterLine_TopLocation;
		gridDepth.vCharacterLine_BottomLocation = gridDepth_DeepValues.vCharacterLine_BottomLocation;
		gridDepth.vCharacterLine_Spacing = gridDepth_DeepValues.vCharacterLine_Spacing;
	}
	else
	{
		gridDepth.depth = DEPTH_CUSTOM;
		gridDepth.hTopLineLocation = atoi(TempNode1.getAttribute("hTopLineLocation"));
		gridDepth.hBottomLineLocation = atoi(TempNode1.getAttribute("hBottomLineLocation"));
		gridDepth.vMonsterLine_TopLocation = atoi(TempNode1.getAttribute("vMonsterLine_TopLocation"));
		gridDepth.vMonsterLine_BottomLocation = atoi(TempNode1.getAttribute("vMonsterLine_BottomLocation"));
		gridDepth.vMonsterLine_Spacing = atoi(TempNode1.getAttribute("vMonsterLine_Spacing"));
		gridDepth.vCharacterLine_TopLocation = atoi(TempNode1.getAttribute("vCharacterLine_TopLocation"));
		gridDepth.vCharacterLine_BottomLocation = atoi(TempNode1.getAttribute("vCharacterLine_BottomLocation"));
		gridDepth.vCharacterLine_Spacing = atoi(TempNode1.getAttribute("vCharacterLine_Spacing"));
	}
}

Terrain::Terrain(StorageFile &storageFile)
{
	char *temp_string;
	temp_string = storageFile.getString();
	name = temp_string;
	delete[] temp_string;
	damage = storageFile.getSignedInt();
	encMultiplier = storageFile.getSignedInt();
	spriteDisplay = storageFile.getSignedInt();
	soundOnlyIfDamaged = storageFile.getBool();
	temp_string = storageFile.getString();
	soundEffect.location = temp_string;
	delete[] temp_string;
	soundEffect.volume = storageFile.getFloat();
	soundEffect.panning = storageFile.getFloat();
	soundEffect.speed = storageFile.getFloat();
	soundEffect.numLoops = storageFile.getSignedInt();
	soundEffect.retainPitch = storageFile.getBool();
	backgroundAssociation = storageFile.getSignedInt();
	globalAnimID = storageFile.getSignedInt();
	globalAnimType = storageFile.getSignedInt();
	temp_string = storageFile.getString();
	bgImageLocation = temp_string;
	delete[] temp_string;
	bgHScrollSpeed = storageFile.getSignedInt();
	bgVScrollSpeed = storageFile.getSignedInt();
	if (backgroundAssociation == BG_FRAME)
	{
		temp_string = storageFile.getString();
		fgImageLocation = temp_string;
		delete temp_string;
		fgHScrollSpeed = storageFile.getSignedInt();
		fgVScrollSpeed = storageFile.getSignedInt();
	}
	else
	{
		fgImageLocation = "";
		fgHScrollSpeed = 0;
		fgVScrollSpeed = 0;
	}
	initPartyMod = storageFile.getSignedInt();
	backAttackEnemyMod = storageFile.getSignedInt();
	surroundAttackPartyMod = storageFile.getSignedInt();
	surroundAttackEnemyMod = storageFile.getSignedInt();
	gridDepth.depth = storageFile.getSignedInt();
	if (gridDepth.depth == DEPTH_SHALLOW)
	{
		gridDepth.hTopLineLocation = gridDepth_ShallowValues.hTopLineLocation;
		gridDepth.hBottomLineLocation = gridDepth_ShallowValues.hBottomLineLocation;
		gridDepth.vMonsterLine_TopLocation = gridDepth_ShallowValues.vMonsterLine_TopLocation;
		gridDepth.vMonsterLine_BottomLocation = gridDepth_ShallowValues.vMonsterLine_BottomLocation;
		gridDepth.vMonsterLine_Spacing = gridDepth_ShallowValues.vMonsterLine_Spacing;
		gridDepth.vCharacterLine_TopLocation = gridDepth_ShallowValues.vCharacterLine_TopLocation;
		gridDepth.vCharacterLine_BottomLocation = gridDepth_ShallowValues.vCharacterLine_BottomLocation;
		gridDepth.vCharacterLine_Spacing = gridDepth_ShallowValues.vCharacterLine_Spacing;
	}
	else if (gridDepth.depth == DEPTH_DEEP)
	{
		gridDepth.hTopLineLocation = gridDepth_DeepValues.hTopLineLocation;
		gridDepth.hBottomLineLocation = gridDepth_DeepValues.hBottomLineLocation;
		gridDepth.vMonsterLine_TopLocation = gridDepth_DeepValues.vMonsterLine_TopLocation;
		gridDepth.vMonsterLine_BottomLocation = gridDepth_DeepValues.vMonsterLine_BottomLocation;
		gridDepth.vMonsterLine_Spacing = gridDepth_DeepValues.vMonsterLine_Spacing;
		gridDepth.vCharacterLine_TopLocation = gridDepth_DeepValues.vCharacterLine_TopLocation;
		gridDepth.vCharacterLine_BottomLocation = gridDepth_DeepValues.vCharacterLine_BottomLocation;
		gridDepth.vCharacterLine_Spacing = gridDepth_DeepValues.vCharacterLine_Spacing;
	}
	else
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
}

Terrain::~Terrain()
{
}

void Terrain::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putString(name.toUtf8().data());
	storageFile.putSignedInt(damage);
	storageFile.putSignedInt(encMultiplier);
	storageFile.putSignedInt(spriteDisplay);
	storageFile.putBool(soundOnlyIfDamaged);
	storageFile.putString(soundEffect.location.toUtf8().data());
	storageFile.putFloat(soundEffect.volume);
	storageFile.putFloat(soundEffect.panning);
	storageFile.putFloat(soundEffect.speed);
	storageFile.putSignedInt(soundEffect.numLoops);
	storageFile.putBool(soundEffect.retainPitch);
	storageFile.putSignedInt(backgroundAssociation);
	storageFile.putSignedInt(globalAnimID);
	storageFile.putSignedInt(globalAnimType);
	storageFile.putString(bgImageLocation.toUtf8().data());
	storageFile.putSignedInt(bgHScrollSpeed);
	storageFile.putSignedInt(bgVScrollSpeed);
	if (backgroundAssociation == BG_FRAME)
	{
		storageFile.putString(fgImageLocation.toUtf8().data());
		storageFile.putSignedInt(fgHScrollSpeed);
		storageFile.putSignedInt(fgVScrollSpeed);
	}
	storageFile.putSignedInt(initPartyMod);
	storageFile.putSignedInt(backAttackEnemyMod);
	storageFile.putSignedInt(surroundAttackPartyMod);
	storageFile.putSignedInt(surroundAttackEnemyMod);
	storageFile.putSignedInt(gridDepth.depth);
	if (gridDepth.depth == DEPTH_CUSTOM)
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
}

QStringList Terrain::getResources()
{
	QStringList resources;
	resources.append(bgImageLocation);
	resources.append(fgImageLocation);
	resources.append(soundEffect.location);
	return resources;
}

XMLNode Terrain::getTerrainNode()
{
	XMLNode TempNode1, TempNode2, TerrainNode = XMLNode::createXMLTopNode("terrain");
	QString tempString;
	char temp_string[512];
	TerrainNode.addAttribute("name", name.toUtf8().data());
	sprintf(temp_string, "%d", damage);
	TerrainNode.addAttribute("damage", temp_string);
	sprintf(temp_string, "%d", encMultiplier);
	TerrainNode.addAttribute("encmultiplier", temp_string);
	if (spriteDisplay == DISPLAY_NORMAL)
		TerrainNode.addAttribute("spritedisplay", "normal");
	else if (spriteDisplay == DISPLAY_OBSBOTHALF)
		TerrainNode.addAttribute("spritedisplay", "obscure-bottom-half");
	else if (spriteDisplay == DISPLAY_OBSBOTTHIRD)
		TerrainNode.addAttribute("spritedisplay", "obscure-bottom-third");
	else
		TerrainNode.addAttribute("spritedisplay", "semi-transparent");
	TempNode1 = TerrainNode.addChild("soundeffect");
	TempNode1.addAttribute("onlyifdamaged", ((soundOnlyIfDamaged) ? "true":"false"));
	TempNode1.addAttribute("location", ProjectData::getRelativeResourcePath(soundEffect.location).toUtf8().data());
	TempNode1.addAttribute("volume", QString::number(soundEffect.volume).toUtf8().data());
	TempNode1.addAttribute("panning", QString::number(soundEffect.panning).toUtf8().data());
	TempNode1.addAttribute("speed", QString::number(soundEffect.speed).toUtf8().data());
	TempNode1.addAttribute("numLoops", QString::number(soundEffect.numLoops).toUtf8().data());
	TempNode1.addAttribute("retainPitch", (soundEffect.retainPitch) ? "true":"false");
	TempNode1 = TerrainNode.addChild("background");
	TempNode1.addAttribute("association", ((backgroundAssociation == BG_BACKGROUND) ? "background":"frame"));
	TempNode2 = TempNode1.addChild("globalanimation");
	sprintf(temp_string, "%d", globalAnimID);
	TempNode2.addAttribute("id", temp_string);
	TempNode2.addAttribute("type", ((globalAnimType == TYPE_BACKGROUND) ? "background":"foreground"));
	TempNode2 = TempNode1.addChild("bgimage");
	sprintf(temp_string, "%d", bgHScrollSpeed);
	TempNode2.addAttribute("hspeed", temp_string);
	sprintf(temp_string, "%d", bgVScrollSpeed);
	TempNode2.addAttribute("vspeed", temp_string);
	TempNode2.addAttribute("location", ProjectData::getRelativeResourcePath(bgImageLocation).toUtf8().data());
	if (backgroundAssociation == BG_FRAME)
	{
		TempNode2 = TempNode1.addChild("fgimage");
		sprintf(temp_string, "%d", fgHScrollSpeed);
		TempNode2.addAttribute("hspeed", temp_string);
		sprintf(temp_string, "%d", fgVScrollSpeed);
		TempNode2.addAttribute("vspeed", temp_string);
		TempNode2.addAttribute("location", ProjectData::getRelativeResourcePath(fgImageLocation).toUtf8().data());
	}
	TempNode1 = TerrainNode.addChild("battletypes");
	sprintf(temp_string, "%d", initPartyMod);
	TempNode1.addAttribute("partyinitiative", temp_string);
	sprintf(temp_string, "%d", backAttackEnemyMod);
	TempNode1.addAttribute("enemybackattack", temp_string);
	sprintf(temp_string, "%d", surroundAttackPartyMod);
	TempNode1.addAttribute("partysurroundattack", temp_string);
	sprintf(temp_string, "%d", surroundAttackEnemyMod);
	TempNode1.addAttribute("enemysurroundattack", temp_string);
	TempNode1 = TerrainNode.addChild("griddepth");
	if (gridDepth.depth == DEPTH_SHALLOW)
		TempNode1.addAttribute("depth", "shallow");
	else if (gridDepth.depth == DEPTH_DEEP)
		TempNode1.addAttribute("depth", "deep");
	else
	{
		TempNode1.addAttribute("depth", "custom");
		sprintf(temp_string, "%d", gridDepth.hTopLineLocation);
		TempNode1.addAttribute("hTopLineLocation", temp_string);
		sprintf(temp_string, "%d", gridDepth.hBottomLineLocation);
		TempNode1.addAttribute("hBottomLineLocation", temp_string);
		sprintf(temp_string, "%d", gridDepth.vMonsterLine_TopLocation);
		TempNode1.addAttribute("vMonsterLine_TopLocation", temp_string);
		sprintf(temp_string, "%d", gridDepth.vMonsterLine_BottomLocation);
		TempNode1.addAttribute("vMonsterLine_BottomLocation", temp_string);
		sprintf(temp_string, "%d", gridDepth.vMonsterLine_Spacing);
		TempNode1.addAttribute("vMonsterLine_Spacing", temp_string);
		sprintf(temp_string, "%d", gridDepth.vCharacterLine_TopLocation);
		TempNode1.addAttribute("vCharacterLine_TopLocation", temp_string);
		sprintf(temp_string, "%d", gridDepth.vCharacterLine_BottomLocation);
		TempNode1.addAttribute("vCharacterLine_BottomLocation", temp_string);
		sprintf(temp_string, "%d", gridDepth.vCharacterLine_Spacing);
		TempNode1.addAttribute("vCharacterLine_Spacing", temp_string);
	}
	return TerrainNode;
}

QString Terrain::getName()
{
	return name;
}

void Terrain::setName(QString value)
{
	name = value;
}

QString Terrain::getBGImageLocation()
{
	return bgImageLocation;
}

void Terrain::setBGImageLocation(QString location)
{
	bgImageLocation = location;
}

QString Terrain::getFGImageLocation()
{
	return fgImageLocation;
}

void Terrain::setFGImageLocation(QString location)
{
	fgImageLocation = location;
}

void Terrain::getSoundEffectInfo(AudioManager::SoundEffectInfo *info)
{
	info->location = soundEffect.location;
	info->volume = soundEffect.volume;
	info->panning = soundEffect.panning;
	info->speed = soundEffect.speed;
	info->numLoops = soundEffect.numLoops;
	info->retainPitch = soundEffect.retainPitch;
}

void Terrain::setSoundEffectInfo(AudioManager::SoundEffectInfo info)
{
	soundEffect.location = info.location;
	soundEffect.volume = info.volume;
	soundEffect.panning = info.panning;
	soundEffect.speed = info.speed;
	soundEffect.numLoops = info.numLoops;
	soundEffect.retainPitch = info.retainPitch;
}

int Terrain::getDamage()
{
	return damage;
}

void Terrain::setDamage(int value)
{
	damage = value;
}

int Terrain::getEncMultiplier()
{
	return encMultiplier;
}

void Terrain::setEncMultiplier(int value)
{
	encMultiplier = value;
}

int Terrain::getGridDepth()
{
	return gridDepth.depth;
}

void Terrain::setGridDepth(int value)
{
	gridDepth.depth = value;
	if (value == DEPTH_SHALLOW)
	{
		gridDepth.hTopLineLocation = gridDepth_ShallowValues.hTopLineLocation;
		gridDepth.hBottomLineLocation = gridDepth_ShallowValues.hBottomLineLocation;
		gridDepth.vMonsterLine_TopLocation = gridDepth_ShallowValues.vMonsterLine_TopLocation;
		gridDepth.vMonsterLine_BottomLocation = gridDepth_ShallowValues.vMonsterLine_BottomLocation;
		gridDepth.vMonsterLine_Spacing = gridDepth_ShallowValues.vMonsterLine_Spacing;
		gridDepth.vCharacterLine_TopLocation = gridDepth_ShallowValues.vCharacterLine_TopLocation;
		gridDepth.vCharacterLine_BottomLocation = gridDepth_ShallowValues.vCharacterLine_BottomLocation;
		gridDepth.vCharacterLine_Spacing = gridDepth_ShallowValues.vCharacterLine_Spacing;
	}
	else if (value == DEPTH_DEEP)
	{
		gridDepth.hTopLineLocation = gridDepth_DeepValues.hTopLineLocation;
		gridDepth.hBottomLineLocation = gridDepth_DeepValues.hBottomLineLocation;
		gridDepth.vMonsterLine_TopLocation = gridDepth_DeepValues.vMonsterLine_TopLocation;
		gridDepth.vMonsterLine_BottomLocation = gridDepth_DeepValues.vMonsterLine_BottomLocation;
		gridDepth.vMonsterLine_Spacing = gridDepth_DeepValues.vMonsterLine_Spacing;
		gridDepth.vCharacterLine_TopLocation = gridDepth_DeepValues.vCharacterLine_TopLocation;
		gridDepth.vCharacterLine_BottomLocation = gridDepth_DeepValues.vCharacterLine_BottomLocation;
		gridDepth.vCharacterLine_Spacing = gridDepth_DeepValues.vCharacterLine_Spacing;
	}
}

int Terrain::getGridDepth_hTopLineLocation()
{
	return gridDepth.hTopLineLocation;
}

void Terrain::setGridDepth_hTopLineLocation(int value)
{
	gridDepth.hTopLineLocation = value;
}

int Terrain::getGridDepth_hBottomLineLocation()
{
	return gridDepth.hBottomLineLocation;
}

void Terrain::setGridDepth_hBottomLineLocation(int value)
{
	gridDepth.hBottomLineLocation = value;
}

int Terrain::getGridDepth_vMonsterLine_TopLocation()
{
	return gridDepth.vMonsterLine_TopLocation;
}

void Terrain::setGridDepth_vMonsterLine_TopLocation(int value)
{
	gridDepth.vMonsterLine_TopLocation = value;
}

int Terrain::getGridDepth_vMonsterLine_BottomLocation()
{
	return gridDepth.vMonsterLine_BottomLocation;
}

void Terrain::setGridDepth_vMonsterLine_BottomLocation(int value)
{
	gridDepth.vMonsterLine_BottomLocation = value;
}

int Terrain::getGridDepth_vMonsterLine_Spacing()
{
	return gridDepth.vMonsterLine_Spacing;
}

void Terrain::setGridDepth_vMonsterLine_Spacing(int value)
{
	gridDepth.vMonsterLine_Spacing = value;
}

int Terrain::getGridDepth_vCharacterLine_TopLocation()
{
	return gridDepth.vCharacterLine_TopLocation;
}

void Terrain::setGridDepth_vCharacterLine_TopLocation(int value)
{
	gridDepth.vCharacterLine_TopLocation = value;
}

int Terrain::getGridDepth_vCharacterLine_BottomLocation()
{
	return gridDepth.vCharacterLine_BottomLocation;
}

void Terrain::setGridDepth_vCharacterLine_BottomLocation(int value)
{
	gridDepth.vCharacterLine_BottomLocation = value;
}

int Terrain::getGridDepth_vCharacterLine_Spacing()
{
	return gridDepth.vCharacterLine_Spacing;
}

void Terrain::setGridDepth_vCharacterLine_Spacing(int value)
{
	gridDepth.vCharacterLine_Spacing = value;
}

Terrain::GridDepthValues *Terrain::getGridDepthValues()
{
	return &gridDepth;
}

int Terrain::getSpriteDisplay()
{
	return spriteDisplay;
}

void Terrain::setSpriteDisplay(int value)
{
	spriteDisplay = value;
}

int Terrain::getBackgroundAssociation()
{
	return backgroundAssociation;
}

void Terrain::setBackgroundAssociation(int value)
{
	backgroundAssociation = value;
}

int Terrain::getBGHScrollSpeed()
{
	return bgHScrollSpeed;
}

void Terrain::setBGHScrollSpeed(int value)
{
	bgHScrollSpeed = value;
}

int Terrain::getBGVScrollSpeed()
{
	return bgVScrollSpeed;
}

void Terrain::setBGVScrollSpeed(int value)
{
	bgVScrollSpeed = value;
}

int Terrain::getFGHScrollSpeed()
{
	return fgHScrollSpeed;
}

void Terrain::setFGHScrollSpeed(int value)
{
	fgHScrollSpeed = value;
}

int Terrain::getFGVScrollSpeed()
{
	return fgVScrollSpeed;
}

void Terrain::setFGVScrollSpeed(int value)
{
	fgVScrollSpeed = value;
}

int Terrain::getInitPartyMod()
{
	return initPartyMod;
}

void Terrain::setInitPartyMod(int value)
{
	initPartyMod = value;
}

int Terrain::getBackAttackEnemyMod()
{
	return backAttackEnemyMod;
}

void Terrain::setBackAttackEnemyMode(int value)
{
	backAttackEnemyMod = value;
}

int Terrain::getSurroundAttackPartyMod()
{
	return surroundAttackPartyMod;
}

void Terrain::setSurroundAttackPartyMod(int value)
{
	surroundAttackPartyMod = value;
}

int Terrain::getSurroundAttackEnemyMod()
{
	return surroundAttackEnemyMod;
}

void Terrain::setSurroundAttackEnemyMod(int value)
{
	surroundAttackEnemyMod = value;
}

int Terrain::getGlobalAnimID()
{
	return globalAnimID;
}

void Terrain::setGlobalAnimID(int value)
{
	globalAnimID = value;
}

int Terrain::getGlobalAnimType()
{
	return globalAnimType;
}

void Terrain::setGlobalAnimType(int value)
{
	globalAnimType = value;
}

bool Terrain::playSoundOnlyIfDamaged()
{
	return soundOnlyIfDamaged;
}

void Terrain::setPlaySoundOnlyIfDamaged(bool value)
{
	soundOnlyIfDamaged = value;
}

bool Terrain::tryLoad(XMLNode TerrainNode)
{
	XMLNode TempNode1, TempNode2;
	QString temp_string;
	if (!XMLTools::attributeExists(TerrainNode, "name"))
		return false;
	if (!XMLTools::attributeExists(TerrainNode, "damage"))
		return false;
	if (!XMLTools::attributeExists(TerrainNode, "encmultiplier"))
		return false;
	if (!XMLTools::attributeExists(TerrainNode, "spritedisplay"))
		return false;
	XMLTools::attributeStringValid(TerrainNode, "spritedisplay", QStringList() << "normal" << "obscure-bottom-half" << "obscure-bottom-third" << "semi-transparent", 0);
	if (!XMLTools::nodeExists(TerrainNode, "soundeffect"))
		return false;
	TempNode1 = TerrainNode.getChildNode("soundeffect");
	if (!XMLTools::attributeExists(TempNode1, "onlyifdamaged"))
		return false;
	if (!XMLTools::attributeExists(TempNode1, "location"))
		return false;
	if (!XMLTools::attributeExists(TempNode1, "volume"))
		return false;
	if (!XMLTools::attributeFloatRangeValid(TempNode1, "volume", 0.0, 1.0))
		return false;
	if (!XMLTools::attributeExists(TempNode1, "panning"))
		return false;
	if (!XMLTools::attributeFloatRangeValid(TempNode1, "panning", -1.0, 1.0))
		return false;
	if (!XMLTools::attributeExists(TempNode1, "speed"))
		return false;
	if (!XMLTools::attributeFloatRangeValid(TempNode1, "speed", 0.5, 2.0))
		return false;
	if (!XMLTools::attributeExists(TempNode1, "numLoops"))
		return false;
	if (!XMLTools::attributeExists(TempNode1, "retainPitch"))
		return false;
	if (!XMLTools::attributeStringValid(TempNode1, "retainPitch", QStringList() << "true" << "false", 1))
		return false;
	if (!XMLTools::nodeExists(TerrainNode, "background"))
		return false;
	TempNode1 = TerrainNode.getChildNode("background");
	if (!XMLTools::attributeExists(TempNode1, "association"))
		return false;
	XMLTools::attributeStringValid(TempNode1, "association", QStringList() << "background" << "frame", 0);
	if (!XMLTools::nodeExists(TempNode1, "globalanimation"))
		return false;
	TempNode2 = TempNode1.getChildNode("globalanimation");
	if (!XMLTools::attributeExists(TempNode2, "id"))
		return false;
	if (!XMLTools::attributeExists(TempNode2, "type"))
		return false;
	XMLTools::attributeStringValid(TempNode2, "type", QStringList() << "background" << "foreground", 0);
	if (!XMLTools::nodeExists(TempNode1, "bgimage"))
		return false;
	TempNode2 = TempNode1.getChildNode("bgimage");
	if (!XMLTools::attributeExists(TempNode2, "location"))
		return false;
	if (!XMLTools::attributeExists(TempNode2, "hspeed"))
		return false;
	if (!XMLTools::attributeExists(TempNode2, "vspeed"))
		return false;
	if (QString(TempNode1.getAttribute("association")) == "frame")
	{
		if (!XMLTools::nodeExists(TempNode1, "fgimage"))
			return false;
		TempNode2 = TempNode1.getChildNode("fgimage");
		if (!XMLTools::attributeExists(TempNode2, "location"))
			return false;
		if (!XMLTools::attributeExists(TempNode2, "hspeed"))
			return false;
		if (!XMLTools::attributeExists(TempNode2, "vspeed"))
			return false;
	}
	if (!XMLTools::nodeExists(TerrainNode, "battletypes"))
		return false;
	TempNode1 = TerrainNode.getChildNode("battletypes");
	if (!XMLTools::attributeExists(TempNode1, "partyinitiative"))
		return false;
	if (!XMLTools::attributeExists(TempNode1, "enemybackattack"))
		return false;
	if (!XMLTools::attributeExists(TempNode1, "partysurroundattack"))
		return false;
	if (!XMLTools::attributeExists(TempNode1, "enemysurroundattack"))
		return false;
	if (!XMLTools::nodeExists(TerrainNode, "griddepth"))
		return false;
	TempNode1 = TerrainNode.getChildNode("griddepth");
	if (!XMLTools::attributeExists(TempNode1, "depth"))
		return false;
	XMLTools::attributeStringValid(TempNode1, "depth", QStringList() << "shallow" << "deep" << "custom", 2);
	if (QString(TempNode1.getAttribute("depth")) == "custom")
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
	return true;
}
