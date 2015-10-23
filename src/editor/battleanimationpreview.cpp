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
#include "battleanimationpreview.h"
#include "projectdata_battleanimation.h"

BattleAnimationPreview::BattleAnimationPreview(QWidget *parent) : QWidget(parent)
{
	setupUi(this);
	timer = new QTimer(this);
	QObject::connect(timer, SIGNAL(timeout()), this, SLOT(advanceFrame()));
	animationScene = new QGraphicsScene(0, 0, 640, 480);
	gvAnimation->setScene(animationScene);
	gvAnimation->setBackgroundBrush(QBrush(QColor(0, 0, 0)));
	targetTintEffect = new MyGraphicsTintEffect();
	targetItem = new QGraphicsPixmapItem;
	targetItem->setPos(0, 0);
	targetItem->setZValue(0);
	targetItem->setGraphicsEffect(targetTintEffect);
	animationScene->addItem(targetItem);
	screenFlashItem = new QGraphicsRectItem(0, 0, 640, 480);
	screenFlashItem->setPos(0, 0);
	screenFlashItem->setZValue(100);
	screenFlashItem->setVisible(false);
	animationScene->addItem(screenFlashItem);
	for (int i = 0; i < 16; ++i)
	{
		imageTintEffects[i] = new MyGraphicsTintEffect();
		imageItems[i] = new QGraphicsPixmapItem;
		imageItems[i]->setPos(0, 0);
		imageItems[i]->setZValue(i + 2);
		imageItems[i]->setTransformOriginPoint(96, 96);
		imageItems[i]->setVisible(false);
		imageItems[i]->setGraphicsEffect(imageTintEffects[i]);
		animationScene->addItem(imageItems[i]);
	}
	shakeAmount = 0;
	animRef = NULL;
}

BattleAnimationPreview::~BattleAnimationPreview()
{
	if (animRef != NULL)
		delete animRef;
	animRef = NULL;
}

void BattleAnimationPreview::playAnimation(int animID, QPixmap target, bool soundOn)
{
	QPixmap pixmap;
	int xAxisLine, yAxisLine;
	int x, y;
	if (animRef != NULL)
	{
		delete animRef;
		animRef = NULL;
	}
	animRef = new ProjectData::DataReference<BattleAnimation>(animID, __FILE__, __LINE__);
	xAxisLine = (*animRef)->getXAxisLine();
	yAxisLine = (*animRef)->getYAxisLine();
	for (int i = 0; i < (*animRef)->numFrames(); ++i)
	{
		BattleAnimation::AnimationFrame *frame = (*animRef)->getFrame(i);
		for (int j = 0; j < frame->animationEffects.size(); ++j)
		{
			BattleAnimation::AnimationEffect *effect = frame->animationEffects[j];
			if (effect->soundEffect != NULL)
				effect->soundEffect->sfxID = AudioManager::loadSoundEffect(effect->soundEffect->location);
		}
	}
	targetPixmap = target;
	if (target.isNull())
	{
		hasTarget = false;
		targetItem->setPos(0, 0);
		targetItem->setVisible(false);
	}
	else
	{
		hasTarget = true;
		if (xAxisLine == BattleAnimation::POS_LEFT)
			x = 320;
		else if (xAxisLine == BattleAnimation::POS_CENTER)
			x = 320 - (targetPixmap.width() / 2);
		else
			x = 320 - targetPixmap.width();
		if (yAxisLine == BattleAnimation::POS_TOP)
			y = 240;
		else if (yAxisLine == BattleAnimation::POS_MIDDLE)
			y = 240 - (targetPixmap.height() / 2);
		else
			y = 240 - targetPixmap.height();
		targetItem->setPixmap(targetPixmap);
		targetItem->setPos(x, y);
	}
	currentFrame = 0;
	playSounds = soundOn;
	cyclingAnimation = false;
	timer->start(1000 / (*animRef)->getFrameRate());
	done = false;
	playFrame();
}

void BattleAnimationPreview::cycleAnimation(int animID, QPixmap target, bool soundOn)
{
	QPixmap pixmap;
	int xAxisLine, yAxisLine;
	int x, y;
	if (animRef != NULL)
	{
		delete animRef;
		animRef = NULL;
	}
	animRef = new ProjectData::DataReference<BattleAnimation>(animID, __FILE__, __LINE__);
	xAxisLine = (*animRef)->getXAxisLine();
	yAxisLine = (*animRef)->getYAxisLine();
	for (int i = 0; i < (*animRef)->numFrames(); ++i)
	{
		BattleAnimation::AnimationFrame *frame = (*animRef)->getFrame(i);
		for (int j = 0; j < frame->animationEffects.size(); ++j)
		{
			BattleAnimation::AnimationEffect *effect = frame->animationEffects[j];
			if (effect->soundEffect != NULL)
				effect->soundEffect->sfxID = AudioManager::loadSoundEffect(effect->soundEffect->location);
		}
	}
	targetPixmap = target;
	if (target.isNull())
	{
		hasTarget = false;
		targetItem->setPos(0, 0);
		targetItem->setVisible(false);
	}
	else
	{
		if (xAxisLine == BattleAnimation::POS_LEFT)
			x = 320;
		else if (xAxisLine == BattleAnimation::POS_CENTER)
			x = 320 - (targetPixmap.width() / 2);
		else
			x = 320 - targetPixmap.width();
		if (yAxisLine == BattleAnimation::POS_TOP)
			y = 240;
		else if (yAxisLine == BattleAnimation::POS_MIDDLE)
			y = 240 - (targetPixmap.height() / 2);
		else
			y = 240 - targetPixmap.height();
		targetItem->setPixmap(targetPixmap);
		targetItem->setPos(x, y);
	}
	currentFrame = 0;
	playSounds = soundOn;
	cyclingAnimation = true;
	timer->start(1000 / (*animRef)->getFrameRate());
	done = false;
	playFrame();
}

void BattleAnimationPreview::stopAnimation()
{
	if (animRef != NULL)
	{
		done = true;
		for (int i = 0; i < (*animRef)->numFrames(); ++i)
		{
			BattleAnimation::AnimationFrame *frame = (*animRef)->getFrame(i);
			for (int j = 0; j < frame->animationEffects.size(); ++j)
			{
				BattleAnimation::AnimationEffect *effect = frame->animationEffects[j];
				if (effect->soundEffect != NULL)
					AudioManager::freeSoundEffect(effect->soundEffect->sfxID);
			}
		}
		for (int i = 0; i < 16; ++i)
			imageItems[i]->setVisible(false);
		targetItem->setVisible(false);
		screenFlashItem->setVisible(false);
		timer->stop();
		delete animRef;
		animRef = NULL;
	}
}

void BattleAnimationPreview::setScale(float scale)
{
	int scaledWidth = (int)(640 * scale);
	int scaledHeight = (int)(480 * scale);
	gvAnimation->resetMatrix();
	gvAnimation->setMinimumSize(scaledWidth, scaledHeight);
	gvAnimation->setMaximumSize(scaledWidth, scaledHeight);
	gvAnimation->scale(scale, scale);
}

void BattleAnimationPreview::advanceFrame()
{
	currentFrame += 1;
	if (currentFrame == (*animRef)->numFrames())
	{
		if (!cyclingAnimation)
		{
			done = true;
			for (int i = 0; i < (*animRef)->numFrames(); ++i)
			{
				BattleAnimation::AnimationFrame *frame = (*animRef)->getFrame(i);
				for (int j = 0; j < frame->animationEffects.size(); ++j)
				{
					BattleAnimation::AnimationEffect *effect = frame->animationEffects[j];
					if (effect->soundEffect != NULL)
						AudioManager::freeSoundEffect(effect->soundEffect->sfxID);
				}
			}
			if (animRef != NULL)
			{
				delete animRef;
				animRef = NULL;
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

void BattleAnimationPreview::playFrame()
{
	QPixmap finalPixmap;
	QPainter painter;
	BattleAnimation::AnimationFrame *frame = (*animRef)->getFrame(currentFrame);
	BattleAnimation::ImageLocation *cell;
	BattleAnimation::AnimationEffect *effect;
	int imageID;
	for (int i = 0; i < 16; ++i)
	{
		imageTintEffects[i]->setColor(0, 0, 0, 0);
		imageItems[i]->setVisible(false);
	}
	targetTintEffect->setColor(0, 0, 0, 0);
	screenFlashItem->setVisible(false);
	for (int i = 0; i < frame->imageList.size(); ++i)
	{
		cell = frame->imageList[i];
		imageID = cell->imageID;
		imageTintEffects[i]->setColor(cell->tintColor.red(), cell->tintColor.green(), cell->tintColor.blue(), cell->tintAmount);
		imageItems[i]->setPixmap(QPixmap((*animRef)->getImageLocation(imageID)));
		imageItems[i]->setPos(cell->x - 96 + 320, cell->y - 96 + 240);
		imageItems[i]->setScale(cell->scale);
		imageItems[i]->setRotation(cell->rotation);
		imageItems[i]->setOpacity(cell->opacity);
		imageItems[i]->setVisible(true);
	}
	for (int i = 0; i < frame->animationEffects.size(); ++i)
	{
		effect = frame->animationEffects[i];
		if (effect->flashScope == BattleAnimation::SCOPE_TARGET && hasTarget)
			targetTintEffect->setColor(effect->flashColor.red(), effect->flashColor.green(), effect->flashColor.blue(), effect->flashOpacity);
		else if (effect->flashScope == BattleAnimation::SCOPE_SCREEN)
		{
			screenFlashItem->setBrush(QBrush(effect->flashColor));
			screenFlashItem->setVisible(true);
			screenFlashItem->setOpacity(effect->flashOpacity / 100.0);
		}
		shakeScope = effect->shakeScope;
		shakeAmount = effect->shakeMagnitude;
		if (effect->soundEffect != NULL && playSounds)
		{
			AudioManager::SoundEffectInfo *sound = effect->soundEffect;
			AudioManager::playSoundEffect(sound->sfxID, sound->volume, sound->panning, sound->speed, sound->retainPitch, sound->numLoops);
		}
	}
	animationScene->update();
}
