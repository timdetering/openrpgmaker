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

#include <QString>
#include <QMessageBox>
#include <QTimer>
#include <cstdio>
#include <cstdlib>
#include "audiomanager.h"
#include "fmod.h"
#include "fmod_errors.h"
#include "fmod_dsp.h"
#include "projectdata.h"

QMap<unsigned int, FMOD_SOUND*> AudioManager::sfx = QMap<unsigned int, FMOD_SOUND*>();
QMap<unsigned int, FMOD_SOUND*>::iterator AudioManager::sfxIter = AudioManager::sfx.end();
QMap<unsigned int, FMOD_CHANNEL*> AudioManager::sfxChannels = QMap<unsigned int, FMOD_CHANNEL*>();
QMap<unsigned int, FMOD_CHANNEL*>::iterator AudioManager::sfxChannelsIter = AudioManager::sfxChannels.end();
QMap<unsigned int, FMOD_DSP*> AudioManager::sfxDSPs = QMap<unsigned int, FMOD_DSP*>();
QMap<unsigned int, FMOD_DSP*>::iterator AudioManager::sfxDSPsIter = AudioManager::sfxDSPs.end();
QMap<unsigned int, bool> AudioManager::freeSFX = QMap<unsigned int, bool>();
QMap<unsigned int, bool>::iterator AudioManager::freeSFXIter = AudioManager::freeSFX.end();
QTimer *AudioManager::timer;
FMOD_SOUND *AudioManager::musicFile;
FMOD_CHANNEL *AudioManager::musicChannel;
FMOD_DSP *AudioManager::musicDSP;
FMOD_SYSTEM *AudioManager::fmodSystem;
FMOD_RESULT AudioManager::fmodResult = FMOD_OK;
float AudioManager::musicMaxVolume = 0;
float AudioManager::musicPlaySpeed = 1.0;
int AudioManager::musicFadeInTime = 0;
bool AudioManager::freeMusic = false;
bool AudioManager::audioEnabled = false;

AudioManager::AudioManager() : QObject(0)
{
}

AudioManager::~AudioManager()
{
}

void AudioManager::initializeAudioManager()
{
	fmodResult = FMOD_System_Create(&fmodSystem);
	if (!checkError("create fmod system. Audio will be disabled"))
	{
		audioEnabled = false;
		return;
	}
	fmodResult = FMOD_System_SetOutput(fmodSystem, FMOD_OUTPUTTYPE_AUTODETECT);
	if (!checkError("set output type to FMOD_OUTPUTTYPE_AUTODETECT. Audio will be disabled."))
	{
		audioEnabled = false;
		FMOD_System_Release(fmodSystem);
		return;
	}
	fmodResult = FMOD_System_Init(fmodSystem, 32, FMOD_INIT_NORMAL, NULL);
	if (!checkError("initialize system with 32 channels in normal mode. Audio will be disabled."))
	{
		audioEnabled = false;
		FMOD_System_Release(fmodSystem);
		return;
	}
	audioEnabled = true;
	timer = new QTimer(ProjectData::audioManager);
	QObject::connect(timer, SIGNAL(timeout()), ProjectData::audioManager, SLOT(updateSystem()));
	timer->start(100);
}

void AudioManager::shutdownAudioManager()
{
	int isPlaying;
	if (audioEnabled)
	{
		timer->stop();
		QObject::disconnect(timer, SIGNAL(timeout()), ProjectData::audioManager, SLOT(updateSystem()));
		delete timer;
		for (sfxDSPsIter = sfxDSPs.begin(); sfxDSPsIter != sfxDSPs.end(); ++sfxDSPsIter)
		{
			if (sfxDSPsIter.value())
			{
				fmodResult = FMOD_DSP_Remove(sfxDSPsIter.value());
				if (!checkError(QString("remove sound effect %1's dsp.").arg(sfxDSPsIter.key())))
					return;
				fmodResult = FMOD_DSP_Release(sfxDSPsIter.value());
				if (!checkError(QString("release sound effect %1's dsp.").arg(sfxDSPsIter.key())))
					return;
				sfxDSPsIter.value() = NULL;
			}
		}
		for (sfxChannelsIter = sfxChannels.begin(); sfxChannelsIter != sfxChannels.end(); ++sfxChannelsIter)
		{
			if (sfxChannelsIter.value())
			{
				fmodResult = FMOD_Channel_IsPlaying(sfxChannelsIter.value(), &isPlaying);
				if (fmodResult == FMOD_ERR_INVALID_HANDLE)
				{
					sfxChannelsIter.value() = NULL;
					isPlaying = false;
				}
				else if (!checkError(QString("check if sound effect %1 is still playing.").arg(sfxChannelsIter.key())))
					return;
				if (isPlaying)
				{
					fmodResult = FMOD_Channel_Stop(sfxChannelsIter.value());
					if (!checkError(QString("stop sound effect %1.").arg(sfxChannelsIter.key())))
						return;
				}
				sfxChannelsIter.value() = NULL;
			}
		}
		for (sfxIter = sfx.begin(); sfxIter != sfx.end(); ++sfxIter)
		{
			if (sfxIter.value())
			{
				fmodResult = FMOD_Sound_Release(sfxIter.value());
				if (!checkError(QString("release sound effect %1").arg(sfxIter.key())))
					return;
				sfxIter.value() = NULL;
			}
		}
		sfx.clear();
		sfxChannels.clear();
		sfxDSPs.clear();
		freeSFX.clear();
		if (musicDSP)
		{
			fmodResult = FMOD_DSP_Remove(musicDSP);
			if (!checkError("remove music's dsp."))
				return;
			fmodResult = FMOD_DSP_Release(musicDSP);
			if (!checkError("release music's dsp."))
				return;
			musicDSP = NULL;
		}
		if (musicChannel)
		{
			fmodResult = FMOD_Channel_IsPlaying(sfxChannelsIter.value(), &isPlaying);
			if (fmodResult == FMOD_ERR_INVALID_HANDLE)
			{
				musicChannel = NULL;
				isPlaying = false;
			}
			else if (!checkError("check if music is still playing."))
				return;
			if (isPlaying)
			{
				fmodResult = FMOD_Channel_Stop(musicChannel);
				if (!checkError("stop music."))
					return;
			}
			musicChannel = NULL;
		}
		if (musicFile)
		{
			fmodResult = FMOD_Sound_Release(musicFile);
			if (!checkError("release music file"))
				return;
			musicFile = NULL;
		}
		if (fmodSystem)
		{
			fmodResult = FMOD_System_Close(fmodSystem);
			if (!checkError("close fmod system."))
				return;
			fmodResult = FMOD_System_Release(fmodSystem);
			if (!checkError("release fmod system."))
				return;
		}
	}
}

unsigned int AudioManager::loadSoundEffect(QString location)
{
	FMOD_SOUND *sound;
	FMOD_DSP *dsp;
	unsigned int sfxID;
	bool foundOne = false;
	if (audioEnabled)
	{
		for (int i = 0; i < sfx.size() && !foundOne; ++i)
		{
			if (!sfx.contains(i))
			{
				sfxID = (unsigned int)i;
				foundOne = true;
			}
		}
		if (!foundOne)
			sfxID = sfx.size();
		fmodResult = FMOD_System_CreateSound(fmodSystem, location.toUtf8().data(), FMOD_LOOP_NORMAL | FMOD_2D | FMOD_SOFTWARE, 0, &sound);
		if (!checkError(QString("load sound effect \"%1\"").arg(location)))
			return 0;
		fmodResult = FMOD_System_CreateDSPByType(fmodSystem, FMOD_DSP_TYPE_PITCHSHIFT, &dsp);
		if (!checkError("create sound effect's dsp."))
			return 0;
		fmodResult = FMOD_DSP_SetActive(dsp, true);
		if (!checkError("activate sound effect's dsp"))
			return 0;
		sfx.insert(sfxID, sound);
		sfxDSPs.insert(sfxID, dsp);
		return sfxID;
	}
	return 0;
}

void AudioManager::freeSoundEffect(unsigned int sfxID)
{
	int isPlaying;
	if (audioEnabled)
	{
		sfxDSPsIter = sfxDSPs.find(sfxID);
		if (sfxDSPsIter != sfxDSPs.end())
		{
			if (sfxDSPsIter.value())
			{
				fmodResult = FMOD_DSP_Remove(sfxDSPsIter.value());
				if (!checkError(QString("remove sound effect %1's dsp.").arg(sfxID)))
					return;
				fmodResult = FMOD_DSP_Release(sfxDSPsIter.value());
				if (!checkError(QString("release sound effect %1's dsp.").arg(sfxID)))
					return;
				sfxDSPsIter.value() = NULL;
			}
			sfxDSPs.erase(sfxDSPsIter);
		}
		sfxChannelsIter = sfxChannels.find(sfxID);
		if (sfxChannelsIter != sfxChannels.end())
		{
			if (sfxChannelsIter.value())
			{
				fmodResult = FMOD_Channel_IsPlaying(sfxChannelsIter.value(), &isPlaying);
				if (fmodResult == FMOD_ERR_INVALID_HANDLE)
				{
					sfxChannelsIter.value() = NULL;
					isPlaying = false;
				}
				else if (!checkError(QString("check if sound effect %1 is still playing.").arg(sfxID)))
					isPlaying = false;
				if (isPlaying)
				{
					fmodResult = FMOD_Channel_Stop(sfxChannelsIter.value());
					if (!checkError(QString("stop sound effect %1.").arg(sfxID)))
						return;
				}
				sfxChannelsIter.value() = NULL;
			}
			sfxChannels.erase(sfxChannelsIter);
		}
		sfxIter = sfx.find(sfxID);
		if (sfxIter != sfx.end())
		{
			if (sfxIter.value())
			{
				fmodResult = FMOD_Sound_Release(sfxIter.value());
				if (!checkError(QString("release sound effect %1").arg(sfxID)))
					return;
				sfxIter.value() = NULL;
			}
			sfx.erase(sfxIter);
		}
		freeSFXIter = freeSFX.find(sfxID);
		if (freeSFXIter != freeSFX.end())
			freeSFX.erase(freeSFXIter);
	}
}

void AudioManager::playSoundEffect(unsigned int sfxID, float volume, float panning, float speed, bool retainPitch, int numLoops, bool freeWhenFinished)
{
	FMOD_CHANNEL *channel;
	FMOD_DSPCONNECTION *dspConnection;
	float frequency;
	if (audioEnabled)
	{
		sfxIter = sfx.find(sfxID);
		sfxDSPsIter = sfxDSPs.find(sfxID);
		if (sfxIter != sfx.end() && sfxDSPsIter != sfxDSPs.end())
		{
			if (retainPitch)
			{
				fmodResult = FMOD_DSP_SetParameter(sfxDSPsIter.value(), FMOD_DSP_PITCHSHIFT_PITCH, 1.0 / speed);
				if (!checkError("set sound effect %1's pitch shift amount."))
					return;
			}
			fmodResult = FMOD_System_PlaySound(fmodSystem, FMOD_CHANNEL_FREE, sfxIter.value(), true, &channel);
			if (!checkError(QString("initialize sound effect %1.").arg(sfxID)))
				return;
			fmodResult = FMOD_Channel_AddDSP(channel, sfxDSPsIter.value(), &dspConnection);
			if (!checkError(QString("add sound effect %1's dsp to it's channel.").arg(sfxID)))
				return;
			fmodResult = FMOD_Channel_GetFrequency(channel, &frequency);
			if (!checkError(QString("get sound effect %1's frequency.").arg(sfxID)))
				return;
			fmodResult = FMOD_Channel_SetFrequency(channel, frequency * speed);
			if (!checkError(QString("set sound effect %1's adjusted frequency.").arg(sfxID)))
				return;
			fmodResult = FMOD_Channel_SetVolume(channel, volume);
			if (!checkError(QString("set sound effect %1's volume.").arg(sfxID)))
				return;
			fmodResult = FMOD_Channel_SetLoopCount(channel, numLoops);
			if (!checkError(QString("set sound effect %1's loop count.").arg(sfxID)))
				return;
			fmodResult = FMOD_Channel_SetPan(channel, panning);
			if (!checkError(QString("set sound effect %1's panning.").arg(sfxID)))
				return;
			fmodResult = FMOD_Channel_SetPaused(channel, false);
			if (!checkError(QString("play sound effect %1").arg(sfxID)))
				return;
			sfxChannels.insert(sfxID, channel);
			freeSFX.insert(sfxID, freeWhenFinished);
		}
	}
}

void AudioManager::stopSoundEffect(unsigned int sfxID)
{
	int isPlaying;
	if (audioEnabled)
	{
		sfxChannelsIter = sfxChannels.find(sfxID);
		if (sfxChannelsIter != sfxChannels.end())
		{
			if (sfxChannelsIter.value())
			{
				fmodResult = FMOD_Channel_IsPlaying(sfxChannelsIter.value(), &isPlaying);
				if (fmodResult == FMOD_ERR_INVALID_HANDLE)
					isPlaying = false;
				else if (!checkError(QString("check if sound effect %1 is playing.").arg(sfxID)))
					isPlaying = false;
				if (isPlaying)
				{
					fmodResult = FMOD_Channel_Stop(sfxChannelsIter.value());
					if (!checkError(QString("stop sound effect %1").arg(sfxID)))
						return;
					emit ProjectData::audioManager->soundEffectStopped(sfxID);
					sfxChannelsIter.value() = NULL;
					sfxChannels.erase(sfxChannelsIter);
				}
			}
			if (freeSFX[sfxID])
				freeSoundEffect(sfxID);
		}
	}
}

void AudioManager::loadMusicFile(QString location)
{
	if (audioEnabled)
	{
		fmodResult = FMOD_System_CreateStream(fmodSystem, location.toUtf8().data(), FMOD_LOOP_NORMAL | FMOD_2D | FMOD_SOFTWARE | FMOD_UNIQUE, 0, &musicFile);
		if (!checkError(QString("load music file \"%1\"").arg(location)))
			return;
		fmodResult = FMOD_System_CreateDSPByType(fmodSystem, FMOD_DSP_TYPE_PITCHSHIFT, &musicDSP);
		if (!checkError("create music's dsp."))
			return;
		fmodResult = FMOD_DSP_SetActive(musicDSP, true);
		if (!checkError("activate music's dsp."))
			return;
	}
}

void AudioManager::freeMusicFile()
{
	int isPlaying;
	if (audioEnabled)
	{
		if (musicDSP)
		{
			fmodResult = FMOD_DSP_Remove(musicDSP);
			if (!checkError("remove music's dsp."))
				return;
			fmodResult = FMOD_DSP_Release(musicDSP);
			if (!checkError("release music's dsp."))
				return;
		}
		if (musicChannel)
		{
			fmodResult = FMOD_Channel_IsPlaying(musicChannel, &isPlaying);
			if (fmodResult == FMOD_ERR_INVALID_HANDLE)
				isPlaying = false;
			else if (!checkError("check if music is still playing."))
				isPlaying = false;
			if (isPlaying)
			{
				fmodResult = FMOD_Channel_Stop(musicChannel);
				if (!checkError("stop music."))
					return;
			}
		}
		if (musicFile)
		{
			fmodResult = FMOD_Sound_Release(musicFile);
			if (!checkError("release music file."))
				return;
		}
		musicDSP = NULL;
		musicChannel = NULL;
		musicFile = NULL;
		freeMusic = false;
	}
}

void AudioManager::playMusic(int fadeInTime, float volume, float panning, float speed, bool retainPitch, bool freeWhenFinished)
{
	FMOD_DSPCONNECTION *dspConnection;
	float frequency;
	if (audioEnabled)
	{
		if (musicFile && musicDSP)
		{
			if (retainPitch)
			{
				fmodResult = FMOD_DSP_SetParameter(musicDSP, FMOD_DSP_PITCHSHIFT_PITCH, 1.0 / speed);
				if (!checkError("set music file's pitch shift amount."))
					return;
			}
			fmodResult = FMOD_System_PlaySound(fmodSystem, FMOD_CHANNEL_FREE, musicFile, true, &musicChannel);
			if (!checkError("initialize music."))
				return;
			fmodResult = FMOD_Channel_AddDSP(musicChannel, musicDSP, &dspConnection);
			if (!checkError("add music's dsp to it's musicChannel."))
				return;
			fmodResult = FMOD_Channel_GetFrequency(musicChannel, &frequency);
			if (!checkError("get music's frequency."))
				return;
			fmodResult = FMOD_Channel_SetFrequency(musicChannel, frequency * speed);
			if (!checkError("set music's adjusted frequency."))
				return;
			if (fadeInTime != 0)
			{
				fmodResult = FMOD_Channel_SetVolume(musicChannel, 0.0);
				if (!checkError("set music's volume."))
					return;
			}
			else
			{
				fmodResult = FMOD_Channel_SetVolume(musicChannel, volume);
				if (!checkError("set music's volume."))
					return;
			}
			musicMaxVolume = volume;
			fmodResult = FMOD_Channel_SetLoopCount(musicChannel, 0);
			if (!checkError("set music's loop count to 0."))
				return;
			fmodResult = FMOD_Channel_SetPan(musicChannel, panning);
			if (!checkError("set music's panning."))
				return;
			fmodResult = FMOD_Channel_SetPaused(musicChannel, false);
			if (!checkError("play music"))
				return;
			musicPlaySpeed = speed;
			musicFadeInTime = fadeInTime;
			freeMusic = freeWhenFinished;
		}
	}
}

void AudioManager::stopMusic()
{
	int isPlaying;
	if (audioEnabled)
	{
		if (musicChannel)
		{
			fmodResult = FMOD_Channel_IsPlaying(musicChannel, &isPlaying);
			if (fmodResult == FMOD_ERR_INVALID_HANDLE)
				isPlaying = false;
			else if (!checkError("check if music is playing."))
				isPlaying = false;
			if (isPlaying)
			{
				fmodResult = FMOD_Channel_Stop(musicChannel);
				if (!checkError("stop music"))
					return;
				emit ProjectData::audioManager->musicStopped();
			}
			musicChannel = NULL;
			if (freeMusic)
				freeMusicFile();
		}
	}
}

void AudioManager::stopAllSoundEffects()
{
	QList<unsigned int> sfxIDs;
	for (sfxIter = sfx.begin(); sfxIter != sfx.end(); ++sfxIter)
		sfxIDs.append(sfxIter.key());
	for (int i = 0; i < sfxIDs.size(); ++i)
		stopSoundEffect(sfxIDs[i]);
	sfxIDs.clear();
}

bool AudioManager::isSoundEffectPlaying(unsigned int sfxID)
{
	int isPlaying = false;
	sfxChannelsIter = sfxChannels.find(sfxID);
	if (sfxChannelsIter != sfxChannels.end())
	{
		if (sfxChannelsIter.value())
		{
			fmodResult = FMOD_Channel_IsPlaying(sfxChannelsIter.value(), &isPlaying);
			if (fmodResult == FMOD_ERR_INVALID_HANDLE)
				isPlaying = false;
			else if (!checkError("check if music is playing."))
				isPlaying = false;
			if (isPlaying)
				return true;
		}
	}
	return false;
}

bool AudioManager::isSoundEffectPlaying()
{
	int isPlaying = false;
	for (sfxChannelsIter = sfxChannels.begin(); sfxChannelsIter != sfxChannels.end(); ++sfxChannelsIter)
	{
		if (sfxChannelsIter.value())
		{
			fmodResult = FMOD_Channel_IsPlaying(sfxChannelsIter.value(), &isPlaying);
			if (fmodResult == FMOD_ERR_INVALID_HANDLE)
				isPlaying = false;
			else if (!checkError("check if music is playing."))
				isPlaying = false;
			if (isPlaying)
				return true;
		}
	}
	return false;
}

bool AudioManager::isMusicPlaying()
{
	int isPlaying = false;
	if (musicChannel)
	{
		fmodResult = FMOD_Channel_IsPlaying(musicChannel, &isPlaying);
		if (fmodResult == FMOD_ERR_INVALID_HANDLE)
			isPlaying = false;
		else if (!checkError("check if music is playing."))
			isPlaying = false;
	}
	return isPlaying;
}

float AudioManager::getMusicPlaySpeed()
{
	return musicPlaySpeed;
}

unsigned int AudioManager::getMusicPosition()
{
	unsigned int position = 0;
	if (AudioManager::isMusicPlaying())
	{
		fmodResult = FMOD_Channel_GetPosition(musicChannel, &position, FMOD_TIMEUNIT_MS);
		if (!checkError("get music's current position."))
			position = 0;
	}
	return position;
}

unsigned int AudioManager::getSoundEffectLength(unsigned int sfxID)
{
	unsigned int length = 0;
	if (audioEnabled)
	{
		sfxIter = sfx.find(sfxID);
		if (sfxIter != sfx.end())
		{
			if (sfxIter.value())
			{
				fmodResult = FMOD_Sound_GetLength(sfxIter.value(), &length, FMOD_TIMEUNIT_MS);
				if (!checkError(QString("get sound effect %1's lenght.").arg(sfxID)))
					return 0;
			}
		}
	}
	return length;
}

unsigned int AudioManager::getMusicLength()
{
	unsigned int length = 0;
	if (audioEnabled)
	{
		if (musicFile)
		{
			fmodResult = FMOD_Sound_GetLength(musicFile, &length, FMOD_TIMEUNIT_MS);
			if (!checkError("get music's length."))
				return 0;
		}
	}
	return length;
}

bool AudioManager::isAudioEnabled()
{
	return audioEnabled;
}

void AudioManager::updateSystem()
{
	QList<unsigned int> sfxIDs;
	float volume, volumeAdjustment;
	unsigned int position;
	int isPlaying;
	if (audioEnabled && fmodSystem)
	{
		fmodResult = FMOD_System_Update(fmodSystem);
		checkError("update fmod system.");
		for (sfxChannelsIter = sfxChannels.begin(); sfxChannelsIter != sfxChannels.end(); ++sfxChannelsIter)
		{
			if (sfxChannelsIter.value() && freeSFX[sfxChannelsIter.key()])
			{
				fmodResult = FMOD_Channel_IsPlaying(sfxChannelsIter.value(), &isPlaying);
				if (fmodResult == FMOD_ERR_INVALID_HANDLE)
					isPlaying = false;
				else if (!checkError(QString("check if sound effect %1 is playing.").arg(sfxChannelsIter.key())))
					isPlaying = false;
				if (!isPlaying)
				{
					emit soundEffectStopped(sfxChannelsIter.key());
					sfxIDs.append(sfxChannelsIter.key());
				}
				else
				{
					fmodResult = FMOD_Channel_GetPosition(sfxChannelsIter.value(), &position, FMOD_TIMEUNIT_MS);
					if (checkError(QString("get sound effect %1's current position.").arg(sfxChannelsIter.key())))
						emit soundEffectPositionUpdated(sfxChannelsIter.key(), position);
				}
			}
			else if (freeSFX[sfxChannelsIter.key()])
				sfxIDs.append(sfxChannelsIter.key());
		}
		for (int i = 0; i < sfxIDs.size(); ++i)
			freeSoundEffect(sfxIDs[i]);
		sfxIDs.clear();
		if (musicChannel)
		{
			fmodResult = FMOD_Channel_IsPlaying(musicChannel, &isPlaying);
			if (fmodResult == FMOD_ERR_INVALID_HANDLE)
				isPlaying = false;
			else if (!checkError("check if music is playing."))
				isPlaying = false;
			if (isPlaying)
			{
				fmodResult = FMOD_Channel_GetPosition(musicChannel, &position, FMOD_TIMEUNIT_MS);
				if (checkError("get music's current position."))
					emit musicPositionUpdated(position);
				if (musicFadeInTime != 0)
				{
					volumeAdjustment = (musicMaxVolume / 10.0) / musicFadeInTime;
					fmodResult = FMOD_Channel_GetVolume(musicChannel, &volume);
					if (checkError("get music's current volume."))
					{
						volume += volumeAdjustment;
						if (volume > 1.0)
							volume = 1.0;
						fmodResult = FMOD_Channel_SetVolume(musicChannel, volume);
						checkError("set music's volume.");
					}
				}
			}
			else if (!isPlaying)
			{
				emit musicStopped();
				if (freeMusic)
					freeMusicFile();
			}
		}
		else if (freeMusic)
			freeMusicFile();
	}
}

bool AudioManager::checkError(QString errorMessage)
{
	if (fmodResult != FMOD_OK)
	{
		QMessageBox::critical(0, "Error", QString("Error: failed to %1\nFMOD Error(%2): %3").arg(errorMessage).arg((int)fmodResult).arg(FMOD_ErrorString(fmodResult)));
		return false;
	}
	return true;
}
