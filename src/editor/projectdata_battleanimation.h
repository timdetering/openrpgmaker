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

#ifndef PROJECTDATA_BATTLEANIMATION_H
#define PROJECTDATA_BATTLEANIMATION_H

#include <QString>
#include <QList>
#include <QStringList>
#include <QColor>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "audiomanager.h"
#include "storagefile.h"
#include "xmlParser.h"

class BattleAnimation
{
	public:
		friend class ProjectData;
		enum Scope {SCOPE_NONE, SCOPE_TARGET, SCOPE_SCREEN};
		enum XPosition {POS_LEFT, POS_CENTER, POS_RIGHT};
		enum YPosition {POS_TOP, POS_MIDDLE, POS_BOTTOM};
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
		BattleAnimation();
		BattleAnimation(BattleAnimation &other);
		BattleAnimation(BattleAnimation *other);
		BattleAnimation(XMLNode AnimationNode);
		BattleAnimation(StorageFile &storageFile);
		~BattleAnimation();
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
		int getXAxisLine();
		void setXAxisLine(int pos);
		int getYAxisLine();
		void setYAxisLine(int pos);
		int getFrameRate();
		void setFrameRate(int rate);
		void setName(QString value);
		static bool tryLoad(XMLNode AnimationNode);
	private:
		QList<AnimationFrame*> frames;
		QStringList images;
		QString name;
		int xAxisLine;
		int yAxisLine;
		int frameRate;
};

#endif // PROJECTDATA_BATTLEANIMATION_H
