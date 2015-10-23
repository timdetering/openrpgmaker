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

#ifndef PROJECTDATA_INTROSCREEN_H
#define PROJECTDATA_INTROSCREEN_H

#include <QString>
#include <QList>
#include <QColor>
#include "audiomanager.h"
#include "storagefile.h"
#include "xmlParser.h"

class IntroScreen
{
	public:
		enum EffectType
		{
			EFFECT_SHOWBACKGROUNDIMAGE=0,
			EFFECT_HIDEBACKGROUNDIMAGE,
			EFFECT_CHANGEBACKGROUNDIMAGE,
			EFFECT_PLAYGLOBALANIMATION,
			EFFECT_PLAYSOUNDEFFECT,
			EFFECT_PLAYMOVIE,
			EFFECT_PLAYBACKGROUNDMUSIC,
			EFFECT_FADEBACKGROUNDMUSIC,
			EFFECT_TINTSCREEN,
			EFFECT_FLASHSCREEN,
			EFFECT_SHAKESCREEN,
			EFFECT_WAIT
		};
		enum FlashType {FLASH_ONCE=0, FLASH_BEGIN, FLASH_STOP};
		enum ShakeType {SHAKE_ONCE=0, SHAKE_BEGIN, SHAKE_STOP};
		struct ShowBackgroundImage
		{
			QString imageLocation;
			int transition;
		};
		struct HideBackgroundImage
		{
			int transition;
		};
		struct ChangeBackgroundImage
		{
			QString imageLocation;
			int eraseTransition;
			int showTransition;
		};
		struct PlayGlobalAnimation
		{
			int animationID;
		};
		struct PlaySoundEffect
		{
			AudioManager::SoundEffectInfo info;
		};
		struct PlayMovie
		{
			QString movieLocation;
		};
		struct PlayBackgroundMusic
		{
			AudioManager::MusicInfo info;
		};
		struct FadeBackgroundMusic
		{
			int fadeOutTime;
		};
		struct TintScreen
		{
			QColor color;
			int opacity;
			int transitionTime;
			bool haltOtherProcesses;
		};
		struct FlashScreen
		{
			QColor color;
			int type;
			int opacity;
			int duration;
			int interval;
			bool haltOtherProcesses;
		};
		struct ShakeScreen
		{
			int type;
			int time;
			int strength;
			int speed;
			bool haltOtherProcesses;
		};
		struct Wait
		{
			int time;
		};
		struct Effect
		{
			int type;
			union EffectData
			{
				ShowBackgroundImage *showBackgroundImage;
				HideBackgroundImage *hideBackgroundImage;
				ChangeBackgroundImage *changeBackgroundImage;
				PlayGlobalAnimation *playGlobalAnimation;
				PlaySoundEffect *playSoundEffect;
				PlayMovie *playMovie;
				PlayBackgroundMusic *playBackgroundMusic;
				FadeBackgroundMusic *fadeBackgroundMusic;
				TintScreen *tintScreen;
				FlashScreen *flashScreen;
				ShakeScreen *shakeScreen;
				Wait *wait;
			} data;
		};
		IntroScreen();
		IntroScreen(IntroScreen &other);
		IntroScreen(const IntroScreen &other);
		IntroScreen(XMLNode IntroScreenNode);
		IntroScreen(StorageFile &storageFile);
		~IntroScreen();
		void saveToStorageFile(StorageFile &storageFile);
		QStringList getResources();
		XMLNode getIntroScreenNode();
		QString getName();
		void setName(QString value);
		QColor getBGColorTop();
		QColor getBGColorBottom();
		void setBGColor(QColor top, QColor bottom);
		int getShowTransition();
		void setShowTransition(int value);
		int getEraseTransition();
		void setEraseTransition(int value);
		bool isSkippable();
		void setSkippable(bool value);
		Effect *getEffect(int effect);
		void addEffect(Effect *effect);
		void deleteEffect(int effect);
		QStringList getEffectStrings();
		static bool tryLoad(XMLNode IntroScreenNode);
	private:
		QList<Effect*> effects;
		QString name;
		QColor bgColorTop;
		QColor bgColorBottom;
		int showTransition;
		int eraseTransition;
		bool skippable;
};

#endif //PROJECTDATA_INTROSCREEN_H
