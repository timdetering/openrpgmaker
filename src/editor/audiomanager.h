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

#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <QObject>
#include <QMap>
#include "fmod.h"

class QString;
class QTimer;

class AudioManager : public QObject
{
	Q_OBJECT
	public:
		struct SoundEffectInfo
		{
			QString location;
			float volume;
			float panning;
			float speed;
			int numLoops;
			int sfxID;
			bool retainPitch;
		};
		struct MusicInfo
		{
			QString location;
			float volume;
			float panning;
			float speed;
			int fadeInTime;
			bool retainPitch;
		};
		AudioManager();
		~AudioManager();
		static void initializeAudioManager();
		static void shutdownAudioManager();
		static unsigned int loadSoundEffect(QString location);
		static void freeSoundEffect(unsigned int sfxID);
		static void playSoundEffect(unsigned int sfxID, float volume, float panning, float speed, bool retainPitch, int numLoops, bool freeWhenFinished=false);
		static void stopSoundEffect(unsigned int sfxID);
		static void loadMusicFile(QString location);
		static void freeMusicFile();
		static void playMusic(int fadeInTime, float volume, float panning, float speed, bool retainPitch, bool freeWhenFinished=false);
		static void stopMusic();
		static void stopAllSoundEffects();
		static bool isSoundEffectPlaying(unsigned int sfxID);
		static bool isSoundEffectPlaying();
		static bool isMusicPlaying();
		static float getMusicPlaySpeed();
		static unsigned int getMusicPosition();
		static unsigned int getSoundEffectLength(unsigned int sfxID);
		static unsigned int getMusicLength();
		static bool isAudioEnabled();
	signals:
		void soundEffectPositionUpdated(unsigned int sfxID, unsigned int currentPosition);
		void soundEffectStopped(unsigned int sfxID);
		void musicPositionUpdated(unsigned int currentPosition);
		void musicStopped();
	private slots:
		void updateSystem();
	private:
		static bool checkError(QString errorMessage);
		static QTimer *timer;
		static QMap<unsigned int, FMOD_SOUND*> sfx;
		static QMap<unsigned int, FMOD_SOUND*>::iterator sfxIter;
		static QMap<unsigned int, FMOD_CHANNEL*> sfxChannels;
		static QMap<unsigned int, FMOD_CHANNEL*>::iterator sfxChannelsIter;
		static QMap<unsigned int, FMOD_DSP*> sfxDSPs;
		static QMap<unsigned int, FMOD_DSP*>::iterator sfxDSPsIter;
		static QMap<unsigned int, bool> freeSFX;
		static QMap<unsigned int, bool>::iterator freeSFXIter;
		static FMOD_SOUND *musicFile;
		static FMOD_CHANNEL *musicChannel;
		static FMOD_DSP *musicDSP;
		static FMOD_SYSTEM *fmodSystem;
		static FMOD_RESULT fmodResult;
		static float musicMaxVolume;
		static float musicPlaySpeed;
		static int musicFadeInTime;
		static bool freeMusic;
		static bool audioEnabled;
};

#endif // AUDIOMANAGER_H
