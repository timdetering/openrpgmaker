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

#include <QColor>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "projectdata.h"
#include "projectdata_globalanimation.h"
#include "xmlParser.h"
#include "xmlTools.h"

GlobalAnimation::GlobalAnimation()
{
	frames.append(new AnimationFrame);
	name = "Untitled";
	frameRate = 12;
}

GlobalAnimation::GlobalAnimation(GlobalAnimation &other)
{
	for (int i = 0; i < other.frames.size(); ++i)
	{
		AnimationFrame *oldFrame = other.frames[i];
		AnimationFrame *newFrame = new AnimationFrame;
		for (int j = 0; j < oldFrame->imageList.size(); ++j)
		{
			ImageLocation *oldImageLocation = oldFrame->imageList[j];
			ImageLocation *newImageLocation = new ImageLocation;
			newImageLocation->tintColor = oldImageLocation->tintColor;
			newImageLocation->rotation = oldImageLocation->rotation;
			newImageLocation->scale = oldImageLocation->scale;
			newImageLocation->opacity = oldImageLocation->opacity;
			newImageLocation->imageID = oldImageLocation->imageID;
			newImageLocation->tintAmount = oldImageLocation->tintAmount;
			newImageLocation->x = oldImageLocation->x;
			newImageLocation->y = oldImageLocation->y;
			newFrame->imageList.append(newImageLocation);
		}
		for (int j = 0; j < oldFrame->animationEffects.size(); ++j)
		{
			AnimationEffect *oldEffect = oldFrame->animationEffects[j];
			AnimationEffect *newEffect = new AnimationEffect;
			if (oldEffect->soundEffect != NULL)
			{
				newEffect->soundEffect = new AudioManager::SoundEffectInfo;
				newEffect->soundEffect->location = oldEffect->soundEffect->location;
				newEffect->soundEffect->volume = oldEffect->soundEffect->volume;
				newEffect->soundEffect->panning = oldEffect->soundEffect->panning;
				newEffect->soundEffect->speed = oldEffect->soundEffect->speed;
				newEffect->soundEffect->numLoops = oldEffect->soundEffect->numLoops;
				newEffect->soundEffect->retainPitch = oldEffect->soundEffect->retainPitch;
			}
			else
				newEffect->soundEffect = NULL;
			newEffect->flashColor = oldEffect->flashColor;
			newEffect->flashScope = oldEffect->flashScope;
			newEffect->flashOpacity = oldEffect->flashOpacity;
			newEffect->shakeScope = oldEffect->shakeScope;
			newEffect->shakeMagnitude = oldEffect->shakeMagnitude;
			newFrame->animationEffects.append(newEffect);
		}
	}
	for (int i = 0; i < other.images.size(); ++i)
		images.append(other.images[i]);
	name = other.name;
	frameRate = other.frameRate;
}

GlobalAnimation::GlobalAnimation(GlobalAnimation *other)
{
	for (int i = 0; i < other->frames.size(); ++i)
	{
		AnimationFrame *oldFrame = other->frames[i];
		AnimationFrame *newFrame = new AnimationFrame;
		for (int j = 0; j < oldFrame->imageList.size(); ++j)
		{
			ImageLocation *oldImageLocation = oldFrame->imageList[j];
			ImageLocation *newImageLocation = new ImageLocation;
			newImageLocation->tintColor = oldImageLocation->tintColor;
			newImageLocation->rotation = oldImageLocation->rotation;
			newImageLocation->scale = oldImageLocation->scale;
			newImageLocation->opacity = oldImageLocation->opacity;
			newImageLocation->imageID = oldImageLocation->imageID;
			newImageLocation->tintAmount = oldImageLocation->tintAmount;
			newImageLocation->x = oldImageLocation->x;
			newImageLocation->y = oldImageLocation->y;
			newFrame->imageList.append(newImageLocation);
		}
		for (int j = 0; j < oldFrame->animationEffects.size(); ++j)
		{
			AnimationEffect *oldEffect = oldFrame->animationEffects[j];
			AnimationEffect *newEffect = new AnimationEffect;
			if (oldEffect->soundEffect != NULL)
			{
				newEffect->soundEffect = new AudioManager::SoundEffectInfo;
				newEffect->soundEffect->location = oldEffect->soundEffect->location;
				newEffect->soundEffect->volume = oldEffect->soundEffect->volume;
				newEffect->soundEffect->panning = oldEffect->soundEffect->panning;
				newEffect->soundEffect->speed = oldEffect->soundEffect->speed;
				newEffect->soundEffect->numLoops = oldEffect->soundEffect->numLoops;
				newEffect->soundEffect->retainPitch = oldEffect->soundEffect->retainPitch;
			}
			else
				newEffect->soundEffect = NULL;
			newEffect->flashColor = oldEffect->flashColor;
			newEffect->flashScope = oldEffect->flashScope;
			newEffect->flashOpacity = oldEffect->flashOpacity;
			newEffect->shakeScope = oldEffect->shakeScope;
			newEffect->shakeMagnitude = oldEffect->shakeMagnitude;
			newFrame->animationEffects.append(newEffect);
		}
	}
	for (int i = 0; i < other->images.size(); ++i)
		images.append(other->images[i]);
	name = other->name;
	frameRate = other->frameRate;
}

GlobalAnimation::GlobalAnimation(XMLNode AnimationNode)
{
	XMLNode TempNode1, TempNode2, TempNode3;
	name = AnimationNode.getAttribute("name");
	frameRate = QString(AnimationNode.getAttribute("frameRate")).toInt();
	for (int i = 0, count = AnimationNode.nChildNode("frame"); i < count; ++i)
	{
		AnimationFrame *frame = new AnimationFrame;
		TempNode1 = AnimationNode.getChildNode("frame", i);
		for (int j = 0, count = TempNode1.nChildNode("image"); j < count; ++j)
		{
			ImageLocation *image = new ImageLocation;
			TempNode2 = TempNode1.getChildNode("image", j);
			image->tintColor = QColor(QString(TempNode2.getAttribute("tintColorRed")).toInt(), QString(TempNode2.getAttribute("tintColorGreen")).toInt(), QString(TempNode2.getAttribute("tintColorBlue")).toInt());
			image->rotation = QString(TempNode2.getAttribute("rotation")).toFloat();
			image->scale = QString(TempNode2.getAttribute("scale")).toFloat();
			image->opacity = QString(TempNode2.getAttribute("opacity")).toFloat();
			image->imageID = QString(TempNode2.getAttribute("imageID")).toInt();
			image->tintAmount = QString(TempNode2.getAttribute("tintAmount")).toInt();
			image->x = QString(TempNode2.getAttribute("x")).toInt();
			image->y = QString(TempNode2.getAttribute("y")).toInt();
			frame->imageList.append(image);
		}
		for (int j = 0, count = TempNode1.nChildNode("effect"); j < count; ++j)
		{
			AnimationEffect *effect = new AnimationEffect;
			TempNode2 = TempNode1.getChildNode("effect", j);
			if (TempNode2.nChildNode("soundEffect") == 1)
			{
				TempNode3 = TempNode2.getChildNode("soundEffect");
				effect->soundEffect = new AudioManager::SoundEffectInfo;
				effect->soundEffect->location = ProjectData::getAbsoluteResourcePath(TempNode3.getAttribute("location"));
				effect->soundEffect->volume = QString(TempNode3.getAttribute("volume")).toFloat();
				effect->soundEffect->panning = QString(TempNode3.getAttribute("panning")).toFloat();
				effect->soundEffect->speed = QString(TempNode3.getAttribute("speed")).toFloat();
				effect->soundEffect->numLoops = QString(TempNode3.getAttribute("numLoops")).toInt();
				effect->soundEffect->retainPitch = (QString(TempNode3.getAttribute("retainPitch")) == "true") ? true:false;
			}
			else
				effect->soundEffect = NULL;
			effect->flashColor = QColor(QString(TempNode2.getAttribute("flashColorRed")).toInt(), QString(TempNode2.getAttribute("flashColorGreen")).toInt(), QString(TempNode2.getAttribute("flashColorBlue")).toInt());
			effect->flashScope = (QString(TempNode2.getAttribute("flashScope")) == "none") ? SCOPE_NONE:SCOPE_SCREEN;
			effect->flashOpacity = QString(TempNode2.getAttribute("flashOpacity")).toInt();
			effect->shakeScope = (QString(TempNode2.getAttribute("shakeScope")) == "none") ? SCOPE_NONE:SCOPE_SCREEN;
			effect->shakeMagnitude = QString(TempNode2.getAttribute("shakeMagnitude")).toInt();
			frame->animationEffects.append(effect);
		}
		frames.append(frame);
	}
	for (int i = 0, count = AnimationNode.nChildNode("image"); i < count; ++i)
		images.append(ProjectData::getAbsoluteResourcePath(AnimationNode.getChildNode("image", i).getAttribute("location")));
}

GlobalAnimation::GlobalAnimation(StorageFile &storageFile)
{
	int count1, count2;
	int r, g, b;
	char *temp_string;
	temp_string = storageFile.getString();
	name = temp_string;
	delete[] temp_string;
	frameRate = storageFile.getSignedInt();
	count1 = storageFile.getSignedInt();
	for (int i = 0; i < count1; ++i)
	{
		AnimationFrame *frame = new AnimationFrame;
		count2 = storageFile.getSignedInt();
		for (int j = 0; j < count2; ++j)
		{
			ImageLocation *image = new ImageLocation;
			r = storageFile.getSignedInt();
			g = storageFile.getSignedInt();
			b = storageFile.getSignedInt();
			image->tintColor = QColor(r, g, b);
			image->rotation = storageFile.getFloat();
			image->scale = storageFile.getFloat();
			image->opacity = storageFile.getFloat();
			image->imageID = storageFile.getSignedInt();
			image->tintAmount = storageFile.getSignedInt();
			image->x = storageFile.getSignedInt();
			image->y = storageFile.getSignedInt();
			frame->imageList.append(image);
		}
		count2 = storageFile.getSignedInt();
		for (int j = 0; j < count2; ++j)
		{
			AnimationEffect *effect = new AnimationEffect;
			if (storageFile.getBool())
			{
				effect->soundEffect = new AudioManager::SoundEffectInfo;
				temp_string = storageFile.getString();
				effect->soundEffect->location = temp_string;
				delete temp_string;
				effect->soundEffect->volume = storageFile.getFloat();
				effect->soundEffect->panning = storageFile.getFloat();
				effect->soundEffect->speed = storageFile.getFloat();
				effect->soundEffect->numLoops = storageFile.getSignedInt();
				effect->soundEffect->retainPitch = storageFile.getBool();
			}
			else
				effect->soundEffect = NULL;
			r = storageFile.getSignedInt();
			g = storageFile.getSignedInt();
			b = storageFile.getSignedInt();
			effect->flashColor = QColor(r, g, b);
			effect->flashScope = storageFile.getSignedInt();
			effect->flashOpacity = storageFile.getSignedInt();
			effect->shakeScope = storageFile.getSignedInt();
			effect->shakeMagnitude = storageFile.getSignedInt();
			frame->animationEffects.append(effect);
		}
		frames.append(frame);
	}
	count1 = storageFile.getSignedInt();
	for (int i = 0; i < count1; ++i)
	{
		temp_string = storageFile.getString();
		images.append(temp_string);
		delete[] temp_string;
	}
}

GlobalAnimation::~GlobalAnimation()
{
	for (int i = 0; i < frames.size(); ++i)
	{
		for (int j = 0; j < frames[i]->imageList.size(); ++j)
		{
			delete (frames[i]->imageList[j]);
			frames[i]->imageList[j] = NULL;
		}
		for (int j = 0; j < frames[i]->animationEffects.size(); ++j)
		{
			if (frames[i]->animationEffects[j]->soundEffect != NULL)
			{
				delete (frames[i]->animationEffects[j]->soundEffect);
				frames[i]->animationEffects[j]->soundEffect = NULL;
			}
			delete (frames[i]->animationEffects[j]);
			frames[i]->animationEffects[j] = NULL;
		}
		frames[i]->imageList.clear();
		frames[i]->animationEffects.clear();
		delete (frames[i]);
		frames[i] = NULL;
	}
	frames.clear();
	images.clear();
}

void GlobalAnimation::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putString(name.toUtf8().data());
	storageFile.putSignedInt(frameRate);
	storageFile.putSignedInt(frames.size());
	for (int i = 0; i < frames.size(); ++i)
	{
		AnimationFrame *frame = frames[i];
		storageFile.putSignedInt(frames[i]->imageList.size());
		for (int j = 0; j < frame->imageList.size(); ++j)
		{
			ImageLocation *image = frame->imageList[j];
			storageFile.putSignedInt(image->tintColor.red());
			storageFile.putSignedInt(image->tintColor.green());
			storageFile.putSignedInt(image->tintColor.blue());
			storageFile.putFloat(image->rotation);
			storageFile.putFloat(image->scale);
			storageFile.putFloat(image->opacity);
			storageFile.putSignedInt(image->imageID);
			storageFile.putSignedInt(image->tintAmount);
			storageFile.putSignedInt(image->x);
			storageFile.putSignedInt(image->y);
		}
		storageFile.putSignedInt(frame->animationEffects.size());
		for (int j = 0; j < frame->animationEffects.size(); ++j)
		{
			AnimationEffect *effect = frame->animationEffects[j];
			if (effect->soundEffect == NULL)
				storageFile.putBool(false);
			else
			{
				storageFile.putBool(true);
				storageFile.putString(effect->soundEffect->location.toUtf8().data());
				storageFile.putFloat(effect->soundEffect->volume);
				storageFile.putFloat(effect->soundEffect->panning);
				storageFile.putFloat(effect->soundEffect->speed);
				storageFile.putSignedInt(effect->soundEffect->numLoops);
				storageFile.putBool(effect->soundEffect->retainPitch);
			}
			storageFile.putSignedInt(effect->flashColor.red());
			storageFile.putSignedInt(effect->flashColor.green());
			storageFile.putSignedInt(effect->flashColor.blue());
			storageFile.putSignedInt(effect->flashScope);
			storageFile.putSignedInt(effect->flashOpacity);
			storageFile.putSignedInt(effect->shakeScope);
			storageFile.putSignedInt(effect->shakeMagnitude);
		}
	}
	storageFile.putSignedInt(images.size());
	for (int i = 0; i < images.size(); ++i)
		storageFile.putString(images[i].toUtf8().data());
}

QStringList GlobalAnimation::getResources()
{
	QStringList resources;
	for (int i = 0; i < images.size(); ++i)
		resources.append(images);
	for (int i = 0; i < frames.size(); ++i)
	{
		AnimationFrame* frame = frames[i];
		for (int j = 0; j < frame->animationEffects.size(); ++j)
		{
			AnimationEffect *effect = frame->animationEffects[j];
			if (effect->soundEffect != NULL)
				resources.append(effect->soundEffect->location);
		}
	}
	return resources;
}

XMLNode GlobalAnimation::getAnimationNode()
{
	XMLNode TempNode1, TempNode2, TempNode3, AnimationNode = XMLNode::createXMLTopNode("globalanimation");
	AnimationNode.addAttribute("name", name.toUtf8().data());
	AnimationNode.addAttribute("frameRate", QString::number(frameRate).toUtf8().data());
	for (int i = 0; i < frames.size(); ++i)
	{
		AnimationFrame *frame = frames[i];
		TempNode1 = AnimationNode.addChild("frame");
		for (int j = 0; j < frame->imageList.size(); ++j)
		{
			ImageLocation *image = frame->imageList[j];
			TempNode2 = TempNode1.addChild("image");
			TempNode2.addAttribute("tintColorRed", QString::number(image->tintColor.red()).toUtf8().data());
			TempNode2.addAttribute("tintColorGreen", QString::number(image->tintColor.green()).toUtf8().data());
			TempNode2.addAttribute("tintColorBlue", QString::number(image->tintColor.blue()).toUtf8().data());
			TempNode2.addAttribute("rotation", QString::number(image->rotation).toUtf8().data());
			TempNode2.addAttribute("scale", QString::number(image->scale).toUtf8().data());
			TempNode2.addAttribute("opacity", QString::number(image->opacity).toUtf8().data());
			TempNode2.addAttribute("imageID", QString::number(image->imageID).toUtf8().data());
			TempNode2.addAttribute("tintAmount", QString::number(image->tintAmount).toUtf8().data());
			TempNode2.addAttribute("x", QString::number(image->x).toUtf8().data());
			TempNode2.addAttribute("y", QString::number(image->y).toUtf8().data());
		}
		for (int j = 0; j < frame->animationEffects.size(); ++j)
		{
			AnimationEffect *effect = frame->animationEffects[j];
			TempNode2 = TempNode1.addChild("effect");
			if (effect->soundEffect != NULL)
			{
				AudioManager::SoundEffectInfo *soundEffect = effect->soundEffect;
				TempNode3 = TempNode2.addChild("soundEffect");
				TempNode3.addAttribute("location", ProjectData::getRelativeResourcePath(soundEffect->location).toUtf8().data());
				TempNode3.addAttribute("volume", QString::number(soundEffect->volume).toUtf8().data());
				TempNode3.addAttribute("panning", QString::number(soundEffect->panning).toUtf8().data());
				TempNode3.addAttribute("speed", QString::number(soundEffect->speed).toUtf8().data());
				TempNode3.addAttribute("numLoops", QString::number(soundEffect->numLoops).toUtf8().data());
				TempNode3.addAttribute("retainPitch", (soundEffect->retainPitch) ? "true":"false");
			}
			TempNode2.addAttribute("flashColorRed", QString::number(effect->flashColor.red()).toUtf8().data());
			TempNode2.addAttribute("flashColorGreen", QString::number(effect->flashColor.green()).toUtf8().data());
			TempNode2.addAttribute("flashColorBlue", QString::number(effect->flashColor.blue()).toUtf8().data());
			TempNode2.addAttribute("flashScope", (effect->flashScope == SCOPE_NONE) ? "none":"screen");
			TempNode2.addAttribute("flashOpacity", QString::number(effect->flashOpacity).toUtf8().data());
			TempNode2.addAttribute("shakeScope", (effect->shakeScope == SCOPE_NONE) ? "none":"screen");
			TempNode2.addAttribute("shakeMagnitude", QString::number(effect->shakeMagnitude).toUtf8().data());
		}
	}
	for (int i = 0; i < images.size(); ++i)
	{
		TempNode1 = AnimationNode.addChild("image");
		TempNode1.addAttribute("location", ProjectData::getRelativeResourcePath(images[i]).toUtf8().data());
	}
	return AnimationNode;
}

GlobalAnimation::AnimationFrame *GlobalAnimation::getFrame(int frameNumber)
{
	return frames[frameNumber];
}

int GlobalAnimation::numFrames()
{
	return frames.size();
}

void GlobalAnimation::setNumFrames(int numFrames)
{
	if (numFrames > frames.size())
	{
		for (int i = frames.size(); i < numFrames; ++i)
			frames.append(new AnimationFrame);
	}
	else
	{
		while (frames.size() > numFrames)
		{
			AnimationFrame *frame = frames.last();
			for (int i = 0; i < frame->imageList.size(); ++i)
			{
				delete (frame->imageList[i]);
				frame->imageList[i] = NULL;
			}
			for (int i = 0; i < frame->animationEffects.size(); ++i)
			{
				if (frame->animationEffects[i]->soundEffect != NULL)
				{
					delete (frame->animationEffects[i]->soundEffect);
					frame->animationEffects[i]->soundEffect = NULL;
				}
				delete (frame->animationEffects[i]);
				frame->animationEffects[i] = NULL;
			}
			frame->imageList.clear();
			frame->animationEffects.clear();
			frames.removeLast();
		}
	}
}

QString GlobalAnimation::getImageLocation(int imageID)
{
	return images[imageID];
}

QStringList GlobalAnimation::getImageLocations()
{
	return images;
}

int GlobalAnimation::numImages()
{
	return images.size();
}

void GlobalAnimation::addImage(QString imageLocation)
{
	images.append(imageLocation);
}

void GlobalAnimation::removeImage(int imageID)
{
	for (int i = 0; i < frames.size(); ++i)
	{
		int j = 0;
		while (j != frames[i]->imageList.size())
		{
			if (frames[i]->imageList[j]->imageID > imageID)
			{
				frames[i]->imageList[j]->imageID -= 1;
				++j;
			}
			else if (frames[i]->imageList[j]->imageID == imageID)
			{
				delete (frames[i]->imageList[j]);
				frames[i]->imageList.removeAt(j);
			}
			else
				++j;
		}
	}
	images.removeAt(imageID);
}

QString GlobalAnimation::getName()
{
	return name;
}

void GlobalAnimation::setName(QString value)
{
	name = value;
}

int GlobalAnimation::getFrameRate()
{
	return frameRate;
}

void GlobalAnimation::setFrameRate(int rate)
{
	frameRate = rate;
}

bool GlobalAnimation::tryLoad(XMLNode AnimationNode)
{
	XMLNode TempNode1, TempNode2, TempNode3;
	if (!XMLTools::attributeExists(AnimationNode, "name"))
		return false;
	if (!XMLTools::attributeExists(AnimationNode, "frameRate"))
		return false;
	if (!XMLTools::attributeIntRangeValid(AnimationNode, "frameRate", 1, 24))
		return false;
	for (int i = 0, count = AnimationNode.nChildNode("frame"); i < count; ++i)
	{
		TempNode1 = AnimationNode.getChildNode("frame", i);
		for (int j = 0, count = TempNode1.nChildNode("image"); j < count; ++j)
		{
			TempNode2 = TempNode1.getChildNode("image", j);
			if (!XMLTools::attributeExists(TempNode2, "tintColorRed"))
				return false;
			if (!XMLTools::attributeIntRangeValid(TempNode2, "tintColorRed", 0, 255))
				return false;
			if (!XMLTools::attributeExists(TempNode2, "tintColorGreen"))
				return false;
			if (!XMLTools::attributeIntRangeValid(TempNode2, "tintColorGreen", 0, 255))
				return false;
			if (!XMLTools::attributeExists(TempNode2, "tintColorBlue"))
				return false;
			if (!XMLTools::attributeIntRangeValid(TempNode2, "tintColorBlue", 0, 255))
				return false;
			if (!XMLTools::attributeExists(TempNode2, "rotation"))
				return false;
			if (!XMLTools::attributeExists(TempNode2, "scale"))
				return false;
			if (!XMLTools::attributeExists(TempNode2, "opacity"))
				return false;
			if (!XMLTools::attributeExists(TempNode2, "imageID"))
				return false;
			if (!XMLTools::attributeExists(TempNode2, "tintAmount"))
				return false;
			if (!XMLTools::attributeIntRangeValid(TempNode2, "tintAmount", 0, 100))
				return false;
			if (!XMLTools::attributeExists(TempNode2, "x"))
				return false;
			if (!XMLTools::attributeExists(TempNode2, "y"))
				return false;
		}
		for (int j = 0, count = TempNode1.nChildNode("effect"); j < count; ++j)
		{
			TempNode2 = TempNode1.getChildNode("effect", j);
			if (TempNode2.nChildNode("soundEffect") == 1)
			{
				TempNode3 = TempNode2.getChildNode("soundEffect");
				if (!XMLTools::attributeExists(TempNode3, "location"))
					return false;
				if (!XMLTools::attributeExists(TempNode3, "volume"))
					return false;
				if (!XMLTools::attributeExists(TempNode3, "panning"))
					return false;
				if (!XMLTools::attributeExists(TempNode3, "speed"))
					return false;
				if (!XMLTools::attributeExists(TempNode3, "numLoops"))
					return false;
				if (!XMLTools::attributeExists(TempNode3, "retainPitch"))
					return false;
				if (!XMLTools::attributeStringValid(TempNode3, "retainPitch", QStringList() << "true" << "false", 1))
					return false;
			}
			if (!XMLTools::attributeExists(TempNode2, "flashColorRed"))
				return false;
			if (!XMLTools::attributeIntRangeValid(TempNode2, "flashColorRed", 0, 255))
				return false;
			if (!XMLTools::attributeExists(TempNode2, "flashColorGreen"))
				return false;
			if (!XMLTools::attributeIntRangeValid(TempNode2, "flashColorGreen", 0, 255))
				return false;
			if (!XMLTools::attributeExists(TempNode2, "flashColorBlue"))
				return false;
			if (!XMLTools::attributeIntRangeValid(TempNode2, "flashColorBlue", 0, 255))
				return false;
			if (!XMLTools::attributeExists(TempNode2, "flashScope"))
				return false;
			if (!XMLTools::attributeStringValid(TempNode2, "flashScope", QStringList() << "none" << "screen", 1))
				return false;
			if (!XMLTools::attributeExists(TempNode2, "flashOpacity"))
				return false;
			if (!XMLTools::attributeIntRangeValid(TempNode2, "flashOpacity", 0, 255))
				return false;
			if (!XMLTools::attributeExists(TempNode2, "shakeScope"))
				return false;
			if (!XMLTools::attributeStringValid(TempNode2, "shakeScope", QStringList() << "none" << "screen", 1))
				return false;
			if (!XMLTools::attributeExists(TempNode2, "shakeMagnitude"))
				return false;
			if (!XMLTools::attributeIntRangeValid(TempNode2, "shakeMagnitude", 0, 16))
				return false;
		}
	}
	for (int i = 0, count = AnimationNode.nChildNode("image"); i < count; ++i)
	{
		TempNode1 = AnimationNode.getChildNode("image");
		if (!XMLTools::attributeExists(TempNode1, "location"))
			return false;
	}
	return true;
}
