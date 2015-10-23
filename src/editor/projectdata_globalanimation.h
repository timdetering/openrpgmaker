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

#ifndef PROJECTDATA_GLOBALANIMATION_H
#define PROJECTDATA_GLOBALANIMATION_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "audiomanager.h"
#include "storagefile.h"
#include "xmlParser.h"

class GlobalAnimation
{
	public:
		friend class ProjectData;
		enum Scope{SCOPE_NONE, SCOPE_SCREEN};
		struct ImageLocation
		{
			QColor tintColor;
			float rotation;
			float scale;
			float opacity;
			int imageID;
			int tintAmount;
			int x;
			int y;
		};
		struct AnimationEffect
		{
			AudioManager::SoundEffectInfo *soundEffect;
			QColor flashColor;
			int flashScope;
			int flashOpacity;
			int shakeScope;
			int shakeMagnitude;
		};
		struct AnimationFrame
		{
			QList<ImageLocation*> imageList;
			QList<AnimationEffect*> animationEffects;
		};
		GlobalAnimation();
		GlobalAnimation(GlobalAnimation &other);
		GlobalAnimation(GlobalAnimation *other);
		GlobalAnimation(XMLNode AnimationNode);
		GlobalAnimation(StorageFile &storageFile);
		~GlobalAnimation();
		void saveToStorageFile(StorageFile &storageFile);
		QStringList getResources();
		XMLNode getAnimationNode();
		AnimationFrame *getFrame(int frameNumber);
		int numFrames();
		void setNumFrames(int numFrames);
		QString getImageLocation(int imageID);
		QStringList getImageLocations();
		int numImages();
		void addImage(QString imageLocation);
		void removeImage(int imageID);
		QString getName();
		void setName(QString value);
		int getFrameRate();
		void setFrameRate(int rate);
		static bool tryLoad(XMLNode AnimationNode);
	private:
		QList<AnimationFrame*> frames;
		QStringList images;
		QString name;
		int frameRate;
};

#endif // PROJECTDATA_GLOBALANIMATION_H
