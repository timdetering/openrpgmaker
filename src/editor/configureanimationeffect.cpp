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

#include <QDialog>
#include <QFileInfo>
#include "audiomanager.h"
#include "configureanimationeffect.h"
#include "soundeffectbrowser.h"

ConfigureAnimationEffect::ConfigureAnimationEffect(int animType, int numFrames, QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	if (animType == ANIM_GLOBAL)
	{
		rbFlashScopeTarget->setVisible(false);
		rbShakeScopeTarget->setVisible(false);
	}
	bFrameSideButton->setSpinBox(sbFrame);
	bOpacitySideButton->setSpinBox(sbOpacity);
	bShakeMagnitudeSideButton->setSpinBox(sbShakeMagnitude);
	sbFrame->setRange(1, numFrames);
}

ConfigureAnimationEffect::~ConfigureAnimationEffect()
{
}

void ConfigureAnimationEffect::getAnimationEffect(GlobalAnimation::AnimationEffect *effect, int *frame)
{
	if (!soundEffect.location.isEmpty())
	{
		effect->soundEffect = new AudioManager::SoundEffectInfo;
		effect->soundEffect->location = soundEffect.location;
		effect->soundEffect->volume = soundEffect.volume;
		effect->soundEffect->panning = soundEffect.panning;
		effect->soundEffect->speed = soundEffect.speed;
		effect->soundEffect->numLoops = soundEffect.numLoops;
		effect->soundEffect->retainPitch = soundEffect.retainPitch;
	}
	else
		effect->soundEffect = NULL;
	effect->flashColor = bSetFlashColor->getColor();
	effect->flashScope = (rbFlashScopeNone->isChecked()) ? GlobalAnimation::SCOPE_NONE:GlobalAnimation::SCOPE_SCREEN;
	effect->flashOpacity = sbOpacity->value();
	effect->shakeScope = (rbShakeScopeNone->isChecked()) ? GlobalAnimation::SCOPE_NONE:GlobalAnimation::SCOPE_SCREEN;
	effect->shakeMagnitude = sbShakeMagnitude->value();
	(*frame) = sbFrame->value();
}

void ConfigureAnimationEffect::setAnimationEffect(GlobalAnimation::AnimationEffect *effect, int frame)
{
	if (effect->soundEffect != NULL)
	{
		soundEffect.location = effect->soundEffect->location;
		soundEffect.volume = effect->soundEffect->volume;
		soundEffect.panning = effect->soundEffect->panning;
		soundEffect.speed = effect->soundEffect->speed;
		soundEffect.numLoops = effect->soundEffect->numLoops;
		soundEffect.retainPitch = effect->soundEffect->retainPitch;
	}
	else
	{
		soundEffect.location = "";
		soundEffect.volume = 1.0;
		soundEffect.panning = 0.0;
		soundEffect.speed = 1.0;
		soundEffect.numLoops = 0;
		soundEffect.retainPitch = true;
	}
	bSetFlashColor->setColor(effect->flashColor);
	if (effect->flashScope == GlobalAnimation::SCOPE_NONE)
		rbFlashScopeNone->setChecked(true);
	else
		rbFlashScopeScreen->setChecked(true);
	sbOpacity->setValue(effect->flashOpacity);
	if (effect->shakeScope == GlobalAnimation::SCOPE_NONE)
		rbShakeScopeNone->setChecked(true);
	else
		rbShakeScopeScreen->setChecked(true);
	sbShakeMagnitude->setValue(effect->shakeMagnitude);
	sbFrame->setValue(frame);
}

void ConfigureAnimationEffect::getAnimationEffect(BattleAnimation::AnimationEffect *effect, int *frame)
{
	if (!soundEffect.location.isEmpty())
	{
		effect->soundEffect = new AudioManager::SoundEffectInfo;
		effect->soundEffect->location = soundEffect.location;
		effect->soundEffect->volume = soundEffect.volume;
		effect->soundEffect->panning = soundEffect.panning;
		effect->soundEffect->speed = soundEffect.speed;
		effect->soundEffect->numLoops = soundEffect.numLoops;
		effect->soundEffect->retainPitch = soundEffect.retainPitch;
	}
	else
		effect->soundEffect = NULL;
	effect->flashColor = bSetFlashColor->getColor();
	if (rbFlashScopeNone->isChecked())
		effect->flashScope = BattleAnimation::SCOPE_NONE;
	else if (rbFlashScopeTarget->isChecked())
		effect->flashScope = BattleAnimation::SCOPE_TARGET;
	else
		effect->flashScope = BattleAnimation::SCOPE_SCREEN;
	effect->flashOpacity = sbOpacity->value();
	if (rbShakeScopeNone->isChecked())
		effect->shakeScope = BattleAnimation::SCOPE_NONE;
	else if (rbShakeScopeTarget->isChecked())
		effect->shakeScope = BattleAnimation::SCOPE_TARGET;
	else
		effect->shakeScope = BattleAnimation::SCOPE_SCREEN;
	effect->shakeMagnitude = sbShakeMagnitude->value();
	(*frame) = sbFrame->value();
}

void ConfigureAnimationEffect::setAnimationEffect(BattleAnimation::AnimationEffect *effect, int frame)
{
	if (effect->soundEffect != NULL)
	{
		soundEffect.location = effect->soundEffect->location;
		soundEffect.volume = effect->soundEffect->volume;
		soundEffect.panning = effect->soundEffect->panning;
		soundEffect.speed = effect->soundEffect->speed;
		soundEffect.numLoops = effect->soundEffect->numLoops;
		soundEffect.retainPitch = effect->soundEffect->retainPitch;
	}
	else
	{
		soundEffect.location = "";
		soundEffect.volume = 1.0;
		soundEffect.panning = 0.0;
		soundEffect.speed = 1.0;
		soundEffect.numLoops = 0;
		soundEffect.retainPitch = true;
	}
	bSetFlashColor->setColor(effect->flashColor);
	if (effect->flashScope == BattleAnimation::SCOPE_NONE)
		rbFlashScopeNone->setChecked(true);
	else if (effect->flashScope == BattleAnimation::SCOPE_TARGET)
		rbFlashScopeTarget->setChecked(true);
	else
		rbFlashScopeScreen->setChecked(true);
	sbOpacity->setValue(effect->flashOpacity);
	if (effect->shakeScope == BattleAnimation::SCOPE_NONE)
		rbShakeScopeNone->setChecked(true);
	else if (effect->shakeScope == BattleAnimation::SCOPE_TARGET)
		rbShakeScopeTarget->setChecked(true);
	else
		rbShakeScopeScreen->setChecked(true);
	sbShakeMagnitude->setValue(effect->shakeMagnitude);
	sbFrame->setValue(frame);
}

void ConfigureAnimationEffect::on_bBrowseSFX_clicked()
{
	SoundEffectBrowser *soundBrowser = new SoundEffectBrowser;
	soundBrowser->setupBrowser();
	soundBrowser->setSoundEffectInfo(soundEffect);
	if (soundBrowser->exec())
	{
		soundBrowser->getSoundEffectInfo(&soundEffect);
		leSoundEffect->setText(soundBrowser->getSoundEffectName());
	}
	delete soundBrowser;
}
