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

#include <QString>
#include <QList>
#include <QColor>
#include "audiomanager.h"
#include "projectdata.h"
#include "projectdata_introscreen.h"
#include "storagefile.h"
#include "xmlParser.h"

IntroScreen::IntroScreen()
{
	name = "Intro Screen";
	bgColorTop = QColor(255, 255, 255);
	bgColorBottom = QColor(255, 255, 255);
	showTransition = ProjectData::SHOW_FADEIN;
	eraseTransition = ProjectData::ERASE_FADEOUT;
	skippable = false;
}

IntroScreen::IntroScreen(IntroScreen &other)
{
	name = other.name;
	bgColorTop = other.bgColorTop;
	bgColorBottom = other.bgColorBottom;
	showTransition = other.showTransition;
	eraseTransition = other.eraseTransition;
	skippable = other.skippable;
	for (int i = 0; i < other.effects.size(); ++i)
	{
		Effect *effect = new Effect;
		Effect *otherEffect = other.effects[i];
		effect->type = otherEffect->type;
		if (effect->type == EFFECT_SHOWBACKGROUNDIMAGE)
		{
			effect->data.showBackgroundImage = new ShowBackgroundImage;
			effect->data.showBackgroundImage->imageLocation = otherEffect->data.showBackgroundImage->imageLocation;
			effect->data.showBackgroundImage->transition = otherEffect->data.showBackgroundImage->transition;
		}
		else if (effect->type == EFFECT_HIDEBACKGROUNDIMAGE)
		{
			effect->data.hideBackgroundImage = new HideBackgroundImage;
			effect->data.hideBackgroundImage->transition = otherEffect->data.hideBackgroundImage->transition;
		}
		else if (effect->type == EFFECT_CHANGEBACKGROUNDIMAGE)
		{
			effect->data.changeBackgroundImage = new ChangeBackgroundImage;
			effect->data.changeBackgroundImage->imageLocation = otherEffect->data.changeBackgroundImage->imageLocation;
			effect->data.changeBackgroundImage->eraseTransition = otherEffect->data.changeBackgroundImage->eraseTransition;
			effect->data.changeBackgroundImage->showTransition = otherEffect->data.changeBackgroundImage->showTransition;
		}
		else if (effect->type == EFFECT_PLAYGLOBALANIMATION)
		{
			effect->data.playGlobalAnimation = new PlayGlobalAnimation;
			effect->data.playGlobalAnimation->animationID = otherEffect->data.playGlobalAnimation->animationID;
		}
		else if (effect->type == EFFECT_PLAYSOUNDEFFECT)
		{
			effect->data.playSoundEffect = new PlaySoundEffect;
			effect->data.playSoundEffect->info.location = otherEffect->data.playSoundEffect->info.location;
			effect->data.playSoundEffect->info.volume = otherEffect->data.playSoundEffect->info.volume;
			effect->data.playSoundEffect->info.panning = otherEffect->data.playSoundEffect->info.panning;
			effect->data.playSoundEffect->info.speed = otherEffect->data.playSoundEffect->info.speed;
			effect->data.playSoundEffect->info.numLoops = otherEffect->data.playSoundEffect->info.numLoops;
			effect->data.playSoundEffect->info.sfxID = otherEffect->data.playSoundEffect->info.sfxID;
			effect->data.playSoundEffect->info.retainPitch = otherEffect->data.playSoundEffect->info.retainPitch;
		}
		else if (effect->type == EFFECT_PLAYMOVIE)
		{
			effect->data.playMovie = new PlayMovie;
			effect->data.playMovie->movieLocation = otherEffect->data.playMovie->movieLocation;
		}
		else if (effect->type == EFFECT_PLAYBACKGROUNDMUSIC)
		{
			effect->data.playBackgroundMusic = new PlayBackgroundMusic;
			effect->data.playBackgroundMusic->info.location = otherEffect->data.playBackgroundMusic->info.location;
			effect->data.playBackgroundMusic->info.volume = otherEffect->data.playBackgroundMusic->info.volume;
			effect->data.playBackgroundMusic->info.panning = otherEffect->data.playBackgroundMusic->info.panning;
			effect->data.playBackgroundMusic->info.speed = otherEffect->data.playBackgroundMusic->info.speed;
			effect->data.playBackgroundMusic->info.fadeInTime = otherEffect->data.playBackgroundMusic->info.fadeInTime;
			effect->data.playBackgroundMusic->info.retainPitch = otherEffect->data.playBackgroundMusic->info.retainPitch;
		}
		else if (effect->type == EFFECT_FADEBACKGROUNDMUSIC)
		{
			effect->data.fadeBackgroundMusic = new FadeBackgroundMusic;
			effect->data.fadeBackgroundMusic->fadeOutTime = otherEffect->data.fadeBackgroundMusic->fadeOutTime;
		}
		else if (effect->type == EFFECT_TINTSCREEN)
		{
			effect->data.tintScreen = new TintScreen;
			effect->data.tintScreen->color = otherEffect->data.tintScreen->color;
			effect->data.tintScreen->opacity = otherEffect->data.tintScreen->opacity;
			effect->data.tintScreen->transitionTime = otherEffect->data.tintScreen->transitionTime;
			effect->data.tintScreen->haltOtherProcesses = otherEffect->data.tintScreen->haltOtherProcesses;
		}
		else if (effect->type == EFFECT_FLASHSCREEN)
		{
			effect->data.flashScreen = new FlashScreen;
			effect->data.flashScreen->color = otherEffect->data.flashScreen->color;
			effect->data.flashScreen->type = otherEffect->data.flashScreen->type;
			effect->data.flashScreen->opacity = otherEffect->data.flashScreen->opacity;
			effect->data.flashScreen->duration = otherEffect->data.flashScreen->duration;
			effect->data.flashScreen->interval = otherEffect->data.flashScreen->interval;
			effect->data.flashScreen->haltOtherProcesses = otherEffect->data.flashScreen->haltOtherProcesses;
		}
		else if (effect->type == EFFECT_SHAKESCREEN)
		{
			effect->data.shakeScreen = new ShakeScreen;
			effect->data.shakeScreen->type = otherEffect->data.shakeScreen->type;
			effect->data.shakeScreen->time = otherEffect->data.shakeScreen->time;
			effect->data.shakeScreen->strength = otherEffect->data.shakeScreen->strength;
			effect->data.shakeScreen->speed = otherEffect->data.shakeScreen->speed;
			effect->data.shakeScreen->haltOtherProcesses = otherEffect->data.shakeScreen->haltOtherProcesses;
		}
		else if (effect->type == EFFECT_WAIT)
		{
			effect->data.wait = new Wait;
			effect->data.wait->time = otherEffect->data.wait->time;
		}
		effects.append(effect);
	}
}

IntroScreen::IntroScreen(const IntroScreen &other)
{
	name = other.name;
	bgColorTop = other.bgColorTop;
	bgColorBottom = other.bgColorBottom;
	showTransition = other.showTransition;
	eraseTransition = other.eraseTransition;
	skippable = other.skippable;
	for (int i = 0; i < other.effects.size(); ++i)
	{
		Effect *effect = new Effect;
		Effect *otherEffect = other.effects[i];
		effect->type = otherEffect->type;
		if (effect->type == EFFECT_SHOWBACKGROUNDIMAGE)
		{
			effect->data.showBackgroundImage = new ShowBackgroundImage;
			effect->data.showBackgroundImage->imageLocation = otherEffect->data.showBackgroundImage->imageLocation;
			effect->data.showBackgroundImage->transition = otherEffect->data.showBackgroundImage->transition;
		}
		else if (effect->type == EFFECT_HIDEBACKGROUNDIMAGE)
		{
			effect->data.hideBackgroundImage = new HideBackgroundImage;
			effect->data.hideBackgroundImage->transition = otherEffect->data.hideBackgroundImage->transition;
		}
		else if (effect->type == EFFECT_CHANGEBACKGROUNDIMAGE)
		{
			effect->data.changeBackgroundImage = new ChangeBackgroundImage;
			effect->data.changeBackgroundImage->imageLocation = otherEffect->data.changeBackgroundImage->imageLocation;
			effect->data.changeBackgroundImage->eraseTransition = otherEffect->data.changeBackgroundImage->eraseTransition;
			effect->data.changeBackgroundImage->showTransition = otherEffect->data.changeBackgroundImage->showTransition;
		}
		else if (effect->type == EFFECT_PLAYGLOBALANIMATION)
		{
			effect->data.playGlobalAnimation = new PlayGlobalAnimation;
			effect->data.playGlobalAnimation->animationID = otherEffect->data.playGlobalAnimation->animationID;
		}
		else if (effect->type == EFFECT_PLAYSOUNDEFFECT)
		{
			effect->data.playSoundEffect = new PlaySoundEffect;
			effect->data.playSoundEffect->info.location = otherEffect->data.playSoundEffect->info.location;
			effect->data.playSoundEffect->info.volume = otherEffect->data.playSoundEffect->info.volume;
			effect->data.playSoundEffect->info.panning = otherEffect->data.playSoundEffect->info.panning;
			effect->data.playSoundEffect->info.speed = otherEffect->data.playSoundEffect->info.speed;
			effect->data.playSoundEffect->info.numLoops = otherEffect->data.playSoundEffect->info.numLoops;
			effect->data.playSoundEffect->info.sfxID = otherEffect->data.playSoundEffect->info.sfxID;
			effect->data.playSoundEffect->info.retainPitch = otherEffect->data.playSoundEffect->info.retainPitch;
		}
		else if (effect->type == EFFECT_PLAYMOVIE)
		{
			effect->data.playMovie = new PlayMovie;
			effect->data.playMovie->movieLocation = otherEffect->data.playMovie->movieLocation;
		}
		else if (effect->type == EFFECT_PLAYBACKGROUNDMUSIC)
		{
			effect->data.playBackgroundMusic = new PlayBackgroundMusic;
			effect->data.playBackgroundMusic->info.location = otherEffect->data.playBackgroundMusic->info.location;
			effect->data.playBackgroundMusic->info.volume = otherEffect->data.playBackgroundMusic->info.volume;
			effect->data.playBackgroundMusic->info.panning = otherEffect->data.playBackgroundMusic->info.panning;
			effect->data.playBackgroundMusic->info.speed = otherEffect->data.playBackgroundMusic->info.speed;
			effect->data.playBackgroundMusic->info.fadeInTime = otherEffect->data.playBackgroundMusic->info.fadeInTime;
			effect->data.playBackgroundMusic->info.retainPitch = otherEffect->data.playBackgroundMusic->info.retainPitch;
		}
		else if (effect->type == EFFECT_FADEBACKGROUNDMUSIC)
		{
			effect->data.fadeBackgroundMusic = new FadeBackgroundMusic;
			effect->data.fadeBackgroundMusic->fadeOutTime = otherEffect->data.fadeBackgroundMusic->fadeOutTime;
		}
		else if (effect->type == EFFECT_TINTSCREEN)
		{
			effect->data.tintScreen = new TintScreen;
			effect->data.tintScreen->color = otherEffect->data.tintScreen->color;
			effect->data.tintScreen->opacity = otherEffect->data.tintScreen->opacity;
			effect->data.tintScreen->transitionTime = otherEffect->data.tintScreen->transitionTime;
			effect->data.tintScreen->haltOtherProcesses = otherEffect->data.tintScreen->haltOtherProcesses;
		}
		else if (effect->type == EFFECT_FLASHSCREEN)
		{
			effect->data.flashScreen = new FlashScreen;
			effect->data.flashScreen->color = otherEffect->data.flashScreen->color;
			effect->data.flashScreen->type = otherEffect->data.flashScreen->type;
			effect->data.flashScreen->opacity = otherEffect->data.flashScreen->opacity;
			effect->data.flashScreen->duration = otherEffect->data.flashScreen->duration;
			effect->data.flashScreen->interval = otherEffect->data.flashScreen->interval;
			effect->data.flashScreen->haltOtherProcesses = otherEffect->data.flashScreen->haltOtherProcesses;
		}
		else if (effect->type == EFFECT_SHAKESCREEN)
		{
			effect->data.shakeScreen = new ShakeScreen;
			effect->data.shakeScreen->type = otherEffect->data.shakeScreen->type;
			effect->data.shakeScreen->time = otherEffect->data.shakeScreen->time;
			effect->data.shakeScreen->strength = otherEffect->data.shakeScreen->strength;
			effect->data.shakeScreen->speed = otherEffect->data.shakeScreen->speed;
			effect->data.shakeScreen->haltOtherProcesses = otherEffect->data.shakeScreen->haltOtherProcesses;
		}
		else if (effect->type == EFFECT_WAIT)
		{
			effect->data.wait = new Wait;
			effect->data.wait->time = otherEffect->data.wait->time;
		}
		effects.append(effect);
	}
}

IntroScreen::IntroScreen(XMLNode IntroScreenNode)
{
	XMLNode EffectNode;
	QString tempString;
	int red, green, blue;
	name = IntroScreenNode.getAttribute("name");
	tempString = IntroScreenNode.getAttribute("bgColor");
	red = tempString.mid(1, 2).toInt(0, 16);
	green = tempString.mid(3, 2).toInt(0, 16);
	blue = tempString.mid(5, 2).toInt(0, 16);
	bgColorTop = QColor(red, green, blue);
	red = tempString.mid(9, 2).toInt(0, 16);
	green = tempString.mid(11, 2).toInt(0, 16);
	blue = tempString.mid(13, 2).toInt(0, 16);
	bgColorBottom = QColor(red, green, blue);
	showTransition = QString(IntroScreenNode.getAttribute("showTransition")).toInt();
	eraseTransition = QString(IntroScreenNode.getAttribute("eraseTransition")).toInt();
	skippable = (QString(IntroScreenNode.getAttribute("skippable")) == "true") ? true:false;
	for (int i = 0, count = IntroScreenNode.nChildNode("effect"); i < count; ++i)
	{
		Effect *effect = new Effect;
		EffectNode = IntroScreenNode.getChildNode("effect", i);
		tempString = EffectNode.getAttribute("type");
		if (tempString == "showBackgroundImage")
		{
			effect->type = EFFECT_SHOWBACKGROUNDIMAGE;
			effect->data.showBackgroundImage = new ShowBackgroundImage;
			effect->data.showBackgroundImage->imageLocation = ProjectData::getAbsoluteResourcePath(EffectNode.getAttribute("imageLocation"));
			effect->data.showBackgroundImage->transition = QString(EffectNode.getAttribute("transition")).toInt();
		}
		else if (tempString == "hideBackgroundImage")
		{
			effect->type = EFFECT_HIDEBACKGROUNDIMAGE;
			effect->data.hideBackgroundImage = new HideBackgroundImage;
			effect->data.hideBackgroundImage->transition = QString(EffectNode.getAttribute("transition")).toInt();
		}
		else if (tempString == "changeBackgroundImage")
		{
			effect->type = EFFECT_CHANGEBACKGROUNDIMAGE;
			effect->data.changeBackgroundImage = new ChangeBackgroundImage;
			effect->data.changeBackgroundImage->imageLocation = ProjectData::getAbsoluteResourcePath(EffectNode.getAttribute("imageLocation"));
			effect->data.changeBackgroundImage->eraseTransition = QString(EffectNode.getAttribute("eraseTransition")).toInt();
			effect->data.changeBackgroundImage->showTransition = QString(EffectNode.getAttribute("showTransition")).toInt();
		}
		else if (tempString == "playGlobalAnimation")
		{
			effect->type = EFFECT_PLAYGLOBALANIMATION;
			effect->data.playGlobalAnimation = new PlayGlobalAnimation;
			effect->data.playGlobalAnimation->animationID = QString(EffectNode.getAttribute("animationID")).toInt();
		}
		else if (tempString == "playSoundeffect")
		{
			effect->type = EFFECT_PLAYSOUNDEFFECT;
			effect->data.playSoundEffect = new PlaySoundEffect;
			effect->data.playSoundEffect->info.location = ProjectData::getAbsoluteResourcePath(EffectNode.getAttribute("location"));
			effect->data.playSoundEffect->info.volume = QString(EffectNode.getAttribute("volume")).toFloat();
			effect->data.playSoundEffect->info.panning = QString(EffectNode.getAttribute("panning")).toFloat();
			effect->data.playSoundEffect->info.speed = QString(EffectNode.getAttribute("speed")).toFloat();
			effect->data.playSoundEffect->info.numLoops = QString(EffectNode.getAttribute("numLoops")).toInt();
			effect->data.playSoundEffect->info.retainPitch = (QString(EffectNode.getAttribute("retainPitch")) == "true") ? true:false;
		}
		else if (tempString == "playMovie")
		{
			effect->type = EFFECT_PLAYMOVIE;
			effect->data.playMovie = new PlayMovie;
			effect->data.playMovie->movieLocation = ProjectData::getAbsoluteResourcePath(EffectNode.getAttribute("movieLocation"));
		}
		else if (tempString == "playBackgroundMusic")
		{
			effect->type = EFFECT_PLAYBACKGROUNDMUSIC;
			effect->data.playBackgroundMusic = new PlayBackgroundMusic;
			effect->data.playBackgroundMusic->info.location = ProjectData::getAbsoluteResourcePath(EffectNode.getAttribute("location"));
			effect->data.playBackgroundMusic->info.volume = QString(EffectNode.getAttribute("volume")).toFloat();
			effect->data.playBackgroundMusic->info.panning = QString(EffectNode.getAttribute("panning")).toFloat();
			effect->data.playBackgroundMusic->info.speed = QString(EffectNode.getAttribute("speed")).toFloat();
			effect->data.playBackgroundMusic->info.fadeInTime = QString(EffectNode.getAttribute("fadeInTime")).toInt();
			effect->data.playBackgroundMusic->info.retainPitch = (QString(EffectNode.getAttribute("retainPitch")) == "true") ? true:false;
		}
		else if (tempString == "fadeBackgroundMusic")
		{
			effect->type = EFFECT_FADEBACKGROUNDMUSIC;
			effect->data.fadeBackgroundMusic = new FadeBackgroundMusic;
			effect->data.fadeBackgroundMusic->fadeOutTime = QString(EffectNode.getAttribute("fadeOutTime")).toInt();
		}
		else if (tempString == "tintScreen")
		{
			effect->type = EFFECT_TINTSCREEN;
			effect->data.tintScreen = new TintScreen;
			tempString = EffectNode.getAttribute("color");
			red = tempString.mid(1, 2).toInt();
			green = tempString.mid(3, 2).toInt();
			blue = tempString.mid(5, 2).toInt();
			effect->data.tintScreen->color = QColor(red, green, blue);
			effect->data.tintScreen->opacity = QString(EffectNode.getAttribute("opacity")).toInt();
			effect->data.tintScreen->transitionTime = QString(EffectNode.getAttribute("transitionTime")).toInt();
			effect->data.tintScreen->haltOtherProcesses = (QString(EffectNode.getAttribute("haltOtherProcesses")) == "true") ? true:false;
		}
		else if (tempString == "flashScreen")
		{
			effect->type = EFFECT_FLASHSCREEN;
			effect->data.flashScreen = new FlashScreen;
			tempString = EffectNode.getAttribute("color");
			red = tempString.mid(1, 2).toInt();
			green = tempString.mid(3, 2).toInt();
			blue = tempString.mid(5, 2).toInt();
			effect->data.flashScreen->color = QColor(red, green, blue);
			tempString = EffectNode.getAttribute("flashType");
			if (tempString == "once")
				effect->data.flashScreen->type = FLASH_ONCE;
			else if (tempString == "begin")
				effect->data.flashScreen->type = FLASH_BEGIN;
			else
				effect->data.flashScreen->type = FLASH_STOP;
			effect->data.flashScreen->opacity = QString(EffectNode.getAttribute("opacity")).toInt();
			effect->data.flashScreen->duration = QString(EffectNode.getAttribute("duration")).toInt();
			effect->data.flashScreen->interval = QString(EffectNode.getAttribute("interval")).toInt();
			effect->data.flashScreen->haltOtherProcesses = (QString(EffectNode.getAttribute("haltOtherProcesses")) == "true") ? true:false;
		}
		else if (tempString == "shakeScreen")
		{
			effect->type = EFFECT_SHAKESCREEN;
			effect->data.shakeScreen = new ShakeScreen;
			tempString = EffectNode.getAttribute("shakeType");
			if (tempString == "once")
				effect->data.shakeScreen->type = SHAKE_ONCE;
			else if (tempString == "begin")
				effect->data.shakeScreen->type = SHAKE_BEGIN;
			else
				effect->data.shakeScreen->type = SHAKE_STOP;
			effect->data.shakeScreen->time = QString(EffectNode.getAttribute("time")).toInt();
			effect->data.shakeScreen->strength = QString(EffectNode.getAttribute("strength")).toInt();
			effect->data.shakeScreen->speed = QString(EffectNode.getAttribute("speed")).toInt();
			effect->data.shakeScreen->haltOtherProcesses = (QString(EffectNode.getAttribute("haltOtherProcesses")) == "true") ? true:false;
		}
		else if (tempString == "wait")
		{
			effect->type = EFFECT_WAIT;
			effect->data.wait = new Wait;
			effect->data.wait->time = QString(EffectNode.getAttribute("time")).toInt();
		}
		effects.append(effect);
	}
}

IntroScreen::IntroScreen(StorageFile &storageFile)
{
	int red, green, blue, numEffects;
	char *temp_string;
	temp_string = storageFile.getString();
	name = QString(temp_string);
	delete[] temp_string;
	red = storageFile.getSignedInt();
	green = storageFile.getSignedInt();
	blue = storageFile.getSignedInt();
	bgColorTop = QColor(red, green, blue);
	red = storageFile.getSignedInt();
	green = storageFile.getSignedInt();
	blue = storageFile.getSignedInt();
	bgColorBottom = QColor(red, green, blue);
	showTransition = storageFile.getSignedInt();
	eraseTransition = storageFile.getSignedInt();
	skippable = storageFile.getBool();
	numEffects = storageFile.getSignedInt();
	for (int i = 0; i < numEffects; ++i)
	{
		Effect *effect = new Effect;
		effect->type = storageFile.getSignedInt();
		if (effect->type == EFFECT_SHOWBACKGROUNDIMAGE)
		{
			effect->data.showBackgroundImage = new ShowBackgroundImage;
			temp_string = storageFile.getString();
			effect->data.showBackgroundImage->imageLocation = QString(temp_string);
			delete[] temp_string;
			effect->data.showBackgroundImage->transition = storageFile.getSignedInt();
		}
		else if (effect->type == EFFECT_HIDEBACKGROUNDIMAGE)
		{
			effect->data.hideBackgroundImage = new HideBackgroundImage;
			effect->data.hideBackgroundImage->transition = storageFile.getSignedInt();
		}
		else if (effect->type == EFFECT_CHANGEBACKGROUNDIMAGE)
		{
			effect->data.changeBackgroundImage = new ChangeBackgroundImage;
			temp_string = storageFile.getString();
			effect->data.changeBackgroundImage->imageLocation = QString(temp_string);
			delete[] temp_string;
			effect->data.changeBackgroundImage->eraseTransition = storageFile.getSignedInt();
			effect->data.changeBackgroundImage->showTransition = storageFile.getSignedInt();
		}
		else if (effect->type == EFFECT_PLAYGLOBALANIMATION)
		{
			effect->data.playGlobalAnimation = new PlayGlobalAnimation;
			effect->data.playGlobalAnimation->animationID = storageFile.getSignedInt();
		}
		else if (effect->type == EFFECT_PLAYSOUNDEFFECT)
		{
			effect->data.playSoundEffect = new PlaySoundEffect;
			temp_string = storageFile.getString();
			effect->data.playSoundEffect->info.location = QString(temp_string);
			delete[] temp_string;
			effect->data.playSoundEffect->info.volume = storageFile.getFloat();
			effect->data.playSoundEffect->info.panning = storageFile.getFloat();
			effect->data.playSoundEffect->info.speed = storageFile.getFloat();
			effect->data.playSoundEffect->info.numLoops = storageFile.getSignedInt();
			effect->data.playSoundEffect->info.retainPitch = storageFile.getBool();
		}
		else if (effect->type == EFFECT_PLAYMOVIE)
		{
			effect->data.playMovie = new PlayMovie;
			temp_string = storageFile.getString();
			effect->data.playMovie->movieLocation = QString(temp_string);
			delete[] temp_string;
		}
		else if (effect->type == EFFECT_PLAYBACKGROUNDMUSIC)
		{
			effect->data.playBackgroundMusic = new PlayBackgroundMusic;
			temp_string = storageFile.getString();
			effect->data.playBackgroundMusic->info.location = QString(temp_string);
			delete[] temp_string;
			effect->data.playBackgroundMusic->info.volume = storageFile.getFloat();
			effect->data.playBackgroundMusic->info.panning = storageFile.getFloat();
			effect->data.playBackgroundMusic->info.speed = storageFile.getFloat();
			effect->data.playBackgroundMusic->info.fadeInTime = storageFile.getSignedInt();
			effect->data.playBackgroundMusic->info.retainPitch = storageFile.getBool();
		}
		else if (effect->type == EFFECT_FADEBACKGROUNDMUSIC)
		{
			effect->data.fadeBackgroundMusic = new FadeBackgroundMusic;
			effect->data.fadeBackgroundMusic->fadeOutTime = storageFile.getSignedInt();
		}
		else if (effect->type == EFFECT_TINTSCREEN)
		{
			effect->data.tintScreen = new TintScreen;
			red = storageFile.getSignedInt();
			green = storageFile.getSignedInt();
			blue = storageFile.getSignedInt();
			effect->data.tintScreen->color = QColor(red, green, blue);
			effect->data.tintScreen->opacity = storageFile.getSignedInt();
			effect->data.tintScreen->transitionTime = storageFile.getSignedInt();
			effect->data.tintScreen->haltOtherProcesses = storageFile.getBool();
		}
		else if (effect->type == EFFECT_FLASHSCREEN)
		{
			effect->data.flashScreen = new FlashScreen;
			red = storageFile.getSignedInt();
			green = storageFile.getSignedInt();
			blue = storageFile.getSignedInt();
			effect->data.flashScreen->color = QColor(red, green, blue);
			effect->data.flashScreen->type = storageFile.getSignedInt();
			effect->data.flashScreen->opacity = storageFile.getSignedInt();
			effect->data.flashScreen->duration = storageFile.getSignedInt();
			effect->data.flashScreen->interval = storageFile.getSignedInt();
			effect->data.flashScreen->haltOtherProcesses = storageFile.getBool();
		}
		else if (effect->type == EFFECT_SHAKESCREEN)
		{
			effect->data.shakeScreen = new ShakeScreen;
			effect->data.shakeScreen->type = storageFile.getSignedInt();
			effect->data.shakeScreen->time = storageFile.getSignedInt();
			effect->data.shakeScreen->strength = storageFile.getSignedInt();
			effect->data.shakeScreen->speed = storageFile.getSignedInt();
			effect->data.shakeScreen->haltOtherProcesses = storageFile.getBool();
		}
		else if (effect->type == EFFECT_WAIT)
		{
			effect->data.wait = new Wait;
			effect->data.wait->time = storageFile.getSignedInt();
		}
		effects.append(effect);
	}
}

IntroScreen::~IntroScreen()
{
	for (int i = 0; i < effects.size(); ++i)
	{
		if (effects[i]->type == EFFECT_SHOWBACKGROUNDIMAGE)
			delete (effects[i]->data.showBackgroundImage);
		else if (effects[i]->type == EFFECT_HIDEBACKGROUNDIMAGE)
			delete (effects[i]->data.hideBackgroundImage);
		else if (effects[i]->type == EFFECT_CHANGEBACKGROUNDIMAGE)
			delete (effects[i]->data.changeBackgroundImage);
		else if (effects[i]->type == EFFECT_PLAYGLOBALANIMATION)
			delete (effects[i]->data.playGlobalAnimation);
		else if (effects[i]->type == EFFECT_PLAYSOUNDEFFECT)
			delete (effects[i]->data.playSoundEffect);
		else if (effects[i]->type == EFFECT_PLAYMOVIE)
			delete (effects[i]->data.playMovie);
		else if (effects[i]->type == EFFECT_PLAYBACKGROUNDMUSIC)
			delete (effects[i]->data.playBackgroundMusic);
		else if (effects[i]->type == EFFECT_FADEBACKGROUNDMUSIC)
			delete (effects[i]->data.fadeBackgroundMusic);
		else if (effects[i]->type == EFFECT_TINTSCREEN)
			delete (effects[i]->data.tintScreen);
		else if (effects[i]->type == EFFECT_FLASHSCREEN)
			delete (effects[i]->data.flashScreen);
		else if (effects[i]->type == EFFECT_SHAKESCREEN)
			delete (effects[i]->data.shakeScreen);
		else if (effects[i]->type == EFFECT_WAIT)
			delete (effects[i]->data.wait);
		delete (effects[i]);
	}
	effects.clear();
}

void IntroScreen::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putString(name.toUtf8().data());
	storageFile.putSignedInt(bgColorTop.red());
	storageFile.putSignedInt(bgColorTop.green());
	storageFile.putSignedInt(bgColorTop.blue());
	storageFile.putSignedInt(bgColorBottom.red());
	storageFile.putSignedInt(bgColorBottom.green());
	storageFile.putSignedInt(bgColorBottom.blue());
	storageFile.putSignedInt(showTransition);
	storageFile.putSignedInt(eraseTransition);
	storageFile.putBool(skippable);
	storageFile.putSignedInt(effects.size());
	for (int i = 0; i < effects.size(); ++i)
	{
		storageFile.putSignedInt(effects[i]->type);
		if (effects[i]->type == EFFECT_SHOWBACKGROUNDIMAGE)
		{
			storageFile.putString(effects[i]->data.showBackgroundImage->imageLocation.toUtf8().data());
			storageFile.putSignedInt(effects[i]->data.showBackgroundImage->transition);
		}
		else if (effects[i]->type == EFFECT_HIDEBACKGROUNDIMAGE)
		{
			storageFile.putSignedInt(effects[i]->data.hideBackgroundImage->transition);
		}
		else if (effects[i]->type == EFFECT_CHANGEBACKGROUNDIMAGE)
		{
			storageFile.putString(effects[i]->data.changeBackgroundImage->imageLocation.toUtf8().data());
			storageFile.putSignedInt(effects[i]->data.changeBackgroundImage->eraseTransition);
			storageFile.putSignedInt(effects[i]->data.changeBackgroundImage->showTransition);
		}
		else if (effects[i]->type == EFFECT_PLAYGLOBALANIMATION)
		{
			storageFile.putSignedInt(effects[i]->data.playGlobalAnimation->animationID);
		}
		else if (effects[i]->type == EFFECT_PLAYSOUNDEFFECT)
		{
			storageFile.putString(effects[i]->data.playSoundEffect->info.location.toUtf8().data());
			storageFile.putFloat(effects[i]->data.playSoundEffect->info.volume);
			storageFile.putFloat(effects[i]->data.playSoundEffect->info.panning);
			storageFile.putFloat(effects[i]->data.playSoundEffect->info.speed);
			storageFile.putSignedInt(effects[i]->data.playSoundEffect->info.numLoops);
			storageFile.putBool(effects[i]->data.playSoundEffect->info.retainPitch);
		}
		else if (effects[i]->type == EFFECT_PLAYMOVIE)
		{
			storageFile.putString(effects[i]->data.playMovie->movieLocation.toUtf8().data());
		}
		else if (effects[i]->type == EFFECT_PLAYBACKGROUNDMUSIC)
		{
			storageFile.putString(effects[i]->data.playBackgroundMusic->info.location.toUtf8().data());
			storageFile.putFloat(effects[i]->data.playBackgroundMusic->info.volume);
			storageFile.putFloat(effects[i]->data.playBackgroundMusic->info.panning);
			storageFile.putFloat(effects[i]->data.playBackgroundMusic->info.speed);
			storageFile.putSignedInt(effects[i]->data.playBackgroundMusic->info.fadeInTime);
			storageFile.putBool(effects[i]->data.playBackgroundMusic->info.retainPitch);
		}
		else if (effects[i]->type == EFFECT_FADEBACKGROUNDMUSIC)
		{
			storageFile.putSignedInt(effects[i]->data.fadeBackgroundMusic->fadeOutTime);
		}
		else if (effects[i]->type == EFFECT_TINTSCREEN)
		{
			storageFile.putSignedInt(effects[i]->data.tintScreen->color.red());
			storageFile.putSignedInt(effects[i]->data.tintScreen->color.green());
			storageFile.putSignedInt(effects[i]->data.tintScreen->color.blue());
			storageFile.putSignedInt(effects[i]->data.tintScreen->opacity);
			storageFile.putSignedInt(effects[i]->data.tintScreen->transitionTime);
			storageFile.putBool(effects[i]->data.tintScreen->haltOtherProcesses);
		}
		else if (effects[i]->type == EFFECT_FLASHSCREEN)
		{
			storageFile.putSignedInt(effects[i]->data.flashScreen->color.red());
			storageFile.putSignedInt(effects[i]->data.flashScreen->color.green());
			storageFile.putSignedInt(effects[i]->data.flashScreen->color.blue());
			storageFile.putSignedInt(effects[i]->data.flashScreen->type);
			storageFile.putSignedInt(effects[i]->data.flashScreen->opacity);
			storageFile.putSignedInt(effects[i]->data.flashScreen->duration);
			storageFile.putSignedInt(effects[i]->data.flashScreen->interval);
			storageFile.putBool(effects[i]->data.flashScreen->haltOtherProcesses);
		}
		else if (effects[i]->type == EFFECT_SHAKESCREEN)
		{
			storageFile.putSignedInt(effects[i]->data.shakeScreen->type);
			storageFile.putSignedInt(effects[i]->data.shakeScreen->time);
			storageFile.putSignedInt(effects[i]->data.shakeScreen->strength);
			storageFile.putSignedInt(effects[i]->data.shakeScreen->speed);
			storageFile.putBool(effects[i]->data.shakeScreen->haltOtherProcesses);
		}
		else if (effects[i]->type == EFFECT_WAIT)
		{
			storageFile.putSignedInt(effects[i]->data.wait->time);
		}
	}
}

QStringList IntroScreen::getResources()
{
	QStringList resources;
	for (int i = 0; i < effects.size(); ++i)
	{
		if (effects[i]->type == EFFECT_SHOWBACKGROUNDIMAGE)
			resources.append(effects[i]->data.showBackgroundImage->imageLocation);
		else if (effects[i]->type == EFFECT_CHANGEBACKGROUNDIMAGE)
			resources.append(effects[i]->data.changeBackgroundImage->imageLocation);
		else if (effects[i]->type == EFFECT_PLAYSOUNDEFFECT)
			resources.append(effects[i]->data.playSoundEffect->info.location);
		else if (effects[i]->type == EFFECT_PLAYMOVIE)
			resources.append(effects[i]->data.playMovie->movieLocation);
		else if (effects[i]->type == EFFECT_PLAYBACKGROUNDMUSIC)
			resources.append(effects[i]->data.playBackgroundMusic->info.location);
	}
	return resources;
}

XMLNode IntroScreen::getIntroScreenNode()
{
	XMLNode IntroScreenNode = XMLNode::createXMLTopNode("introscreen");
	TODO("Populate IntroScreenNode.");
	return IntroScreenNode;
}

QString IntroScreen::getName()
{
	return name;
}

void IntroScreen::setName(QString value)
{
	name = value;
}

QColor IntroScreen::getBGColorTop()
{
	return bgColorTop;
}

QColor IntroScreen::getBGColorBottom()
{
	return bgColorBottom;
}

void IntroScreen::setBGColor(QColor top, QColor bottom)
{
	bgColorTop = top;
	bgColorBottom = bottom;
}

int IntroScreen::getShowTransition()
{
	return showTransition;
}

void IntroScreen::setShowTransition(int value)
{
	showTransition = value;
}

int IntroScreen::getEraseTransition()
{
	return eraseTransition;
}

void IntroScreen::setEraseTransition(int value)
{
	eraseTransition = value;
}

bool IntroScreen::isSkippable()
{
	return skippable;
}

void IntroScreen::setSkippable(bool value)
{
	skippable = value;
}

IntroScreen::Effect *IntroScreen::getEffect(int effect)
{
	return effects[effect];
}

void IntroScreen::addEffect(Effect *effect)
{
	effects.append(effect);
}

void IntroScreen::deleteEffect(int effect)
{
	if (effects[effect]->type == EFFECT_SHOWBACKGROUNDIMAGE)
		delete (effects[effect]->data.showBackgroundImage);
	else if (effects[effect]->type == EFFECT_HIDEBACKGROUNDIMAGE)
		delete (effects[effect]->data.hideBackgroundImage);
	else if (effects[effect]->type == EFFECT_CHANGEBACKGROUNDIMAGE)
		delete (effects[effect]->data.changeBackgroundImage);
	else if (effects[effect]->type == EFFECT_PLAYGLOBALANIMATION)
		delete (effects[effect]->data.playGlobalAnimation);
	else if (effects[effect]->type == EFFECT_PLAYSOUNDEFFECT)
		delete (effects[effect]->data.playSoundEffect);
	else if (effects[effect]->type == EFFECT_PLAYMOVIE)
		delete (effects[effect]->data.playMovie);
	else if (effects[effect]->type == EFFECT_PLAYBACKGROUNDMUSIC)
		delete (effects[effect]->data.playBackgroundMusic);
	else if (effects[effect]->type == EFFECT_FADEBACKGROUNDMUSIC)
		delete (effects[effect]->data.fadeBackgroundMusic);
	else if (effects[effect]->type == EFFECT_TINTSCREEN)
		delete (effects[effect]->data.tintScreen);
	else if (effects[effect]->type == EFFECT_FLASHSCREEN)
		delete (effects[effect]->data.flashScreen);
	else if (effects[effect]->type == EFFECT_SHAKESCREEN)
		delete (effects[effect]->data.shakeScreen);
	else if (effects[effect]->type == EFFECT_WAIT)
		delete (effects[effect]->data.wait);
	delete (effects[effect]);
	effects.removeAt(effect);
}

QStringList IntroScreen::getEffectStrings()
{
	QStringList effectStrings;
	TODO("Populate effectStrings.");
	return effectStrings;
}

bool IntroScreen::tryLoad(XMLNode IntroScreenNode)
{
	TODO("Check IntroScreenNode for errors.");
	return true;
}
