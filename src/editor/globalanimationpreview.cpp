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

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QImage>
#include <QBitmap>
#include <QPixmap>
#include <QPainter>
#include <QTimer>
#include <cstdio>
#include "audiomanager.h"
#include "globalanimationpreview.h"
#include "projectdata_globalanimation.h"

GlobalAnimationPreview::GlobalAnimationPreview(QWidget *parent) : QWidget(parent)
{
	setupUi(this);
	timer = new QTimer(this);
	QObject::connect(timer, SIGNAL(timeout()), this, SLOT(advanceFrame()));
	animationScene = new QGraphicsScene(0, 0, 640, 480);
	gvAnimation->setScene(animationScene);
	gvAnimation->setBackgroundBrush(QBrush(QColor(0, 0, 0)));
	backgroundItem = new QGraphicsPixmapItem;
	backgroundItem->setPos(0, 0);
	backgroundItem->setZValue(0);
	animationScene->addItem(backgroundItem);
	flashItem = new QGraphicsRectItem(0, 0, 640, 480);
	flashItem->setPos(0, 0);
	flashItem->setZValue(100);
	flashItem->setVisible(false);
	animationScene->addItem(flashItem);
	for (int i = 0; i < 16; ++i)
	{
		imageTintEffects[i] = new MyGraphicsTintEffect();
		imageItems[i] = new QGraphicsPixmapItem;
		imageItems[i]->setPos(0, 0);
		imageItems[i]->setZValue(i + 1);
		imageItems[i]->setTransformOriginPoint(96, 96);
		imageItems[i]->setVisible(false);
		imageItems[i]->setGraphicsEffect(imageTintEffects[i]);
		animationScene->addItem(imageItems[i]);
	}
	shakeAmount = 0;
	animRef = NULL;
}

GlobalAnimationPreview::~GlobalAnimationPreview()
{
	if (animRef != NULL)
		delete animRef;
	animRef = NULL;
}

void GlobalAnimationPreview::playAnimation(int animID, QPixmap backgroundPixmap)
{
	animRef = new ProjectData::DataReference<GlobalAnimation>(animID, __FILE__, __LINE__);
	for (int i = 0; i < (*animRef)->numFrames(); ++i)
	{
		GlobalAnimation::AnimationFrame *frame = (*animRef)->getFrame(i);
		for (int j = 0; j < frame->animationEffects.size(); ++j)
		{
			GlobalAnimation::AnimationEffect *effect = frame->animationEffects[j];
			if (effect->soundEffect != NULL)
				effect->soundEffect->sfxID = AudioManager::loadSoundEffect(effect->soundEffect->location);
		}
	}
	backgroundItem->setPixmap(backgroundPixmap);
	currentFrame = 0;
	playSounds = true;
	cyclingAnimation = false;
	timer->start(1000 / (*animRef)->getFrameRate());
	done = false;
	playFrame();
}

void GlobalAnimationPreview::cycleAnimation(int animID)
{
	animRef = new ProjectData::DataReference<GlobalAnimation>(animID, __FILE__, __LINE__);
	for (int i = 0; i < (*animRef)->numFrames(); ++i)
	{
		GlobalAnimation::AnimationFrame *frame = (*animRef)->getFrame(i);
		for (int j = 0; j < frame->animationEffects.size(); ++j)
		{
			GlobalAnimation::AnimationEffect *effect = frame->animationEffects[j];
			if (effect->soundEffect != NULL)
				effect->soundEffect->sfxID = AudioManager::loadSoundEffect(effect->soundEffect->location);
		}
	}
	backgroundItem->setVisible(false);
	currentFrame = 0;
	playSounds = false;
	cyclingAnimation = true;
	timer->start(1000 / (*animRef)->getFrameRate());
	done = false;
	playFrame();
}

void GlobalAnimationPreview::stopAnimation()
{
	if (animRef != NULL)
	{
		done = true;
		for (int i = 0; i < (*animRef)->numFrames(); ++i)
		{
			GlobalAnimation::AnimationFrame *frame = (*animRef)->getFrame(i);
			for (int j = 0; j < frame->animationEffects.size(); ++j)
			{
				GlobalAnimation::AnimationEffect *effect = frame->animationEffects[j];
				if (effect->soundEffect != NULL)
					AudioManager::freeSoundEffect(effect->soundEffect->sfxID);
			}
		}
		timer->stop();
		delete animRef;
		animRef = NULL;
	}
}

void GlobalAnimationPreview::setScale(float scale)
{
	int scaledWidth = (int)(640 * scale);
	int scaledHeight = (int)(480 * scale);
	gvAnimation->resetMatrix();
	gvAnimation->setMinimumSize(scaledWidth, scaledHeight);
	gvAnimation->setMaximumSize(scaledWidth, scaledHeight);
	gvAnimation->scale(scale, scale);
}

void GlobalAnimationPreview::advanceFrame()
{
	currentFrame += 1;
	if (currentFrame == (*animRef)->numFrames())
	{
		if (!cyclingAnimation)
		{
			done = true;
			for (int i = 0; i < (*animRef)->numFrames(); ++i)
			{
				GlobalAnimation::AnimationFrame *frame = (*animRef)->getFrame(i);
				for (int j = 0; j < frame->animationEffects.size(); ++j)
				{
					GlobalAnimation::AnimationEffect *effect = frame->animationEffects[j];
					if (effect->soundEffect != NULL)
						AudioManager::freeSoundEffect(effect->soundEffect->sfxID);
				}
			}
			timer->stop();
		}
		else
		{
			currentFrame = 0;
			playFrame();
		}
	}
	else
		playFrame();
}

void GlobalAnimationPreview::playFrame()
{
	GlobalAnimation::AnimationFrame *frame = (*animRef)->getFrame(currentFrame);
	GlobalAnimation::ImageLocation *cell;
	GlobalAnimation::AnimationEffect *effect;
	int imageID;
	for (int i = 0; i < 16; ++i)
	{
		imageTintEffects[i]->setColor(0, 0, 0, 0);
		imageItems[i]->setVisible(false);
	}
	flashItem->setVisible(false);
	for (int i = 0; i < frame->imageList.size(); ++i)
	{
		cell = frame->imageList[i];
		imageID = cell->imageID;
		imageTintEffects[i]->setColor(cell->tintColor.red(), cell->tintColor.green(), cell->tintColor.blue(), cell->tintAmount);
		imageItems[i]->setPixmap(QPixmap((*animRef)->getImageLocation(imageID)));
		imageItems[i]->setPos(cell->x - 96, cell->y - 96);
		imageItems[i]->setScale(cell->scale);
		imageItems[i]->setRotation(cell->rotation);
		imageItems[i]->setOpacity(cell->opacity);
		imageItems[i]->setVisible(true);
	}
	for (int i = 0; i < frame->animationEffects.size(); ++i)
	{
		effect = frame->animationEffects[i];
		if (effect->flashScope == GlobalAnimation::SCOPE_SCREEN)
		{
			flashItem->setBrush(QBrush(effect->flashColor));
			flashItem->setVisible(true);
			flashItem->setOpacity(effect->flashOpacity / 100.0);
		}
		if (effect->shakeScope == GlobalAnimation::SCOPE_NONE)
			shakeAmount = 0;
		else
			shakeAmount = effect->shakeMagnitude;
		if (effect->soundEffect != NULL && playSounds)
		{
			AudioManager::SoundEffectInfo *sound = effect->soundEffect;
			AudioManager::playSoundEffect(sound->sfxID, sound->volume, sound->panning, sound->speed, sound->retainPitch, sound->numLoops);
		}
	}
	animationScene->update();
}
