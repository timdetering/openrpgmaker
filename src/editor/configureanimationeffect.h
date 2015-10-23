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

#ifndef CONFIGUREANIMATIONEFFECT_H
#define CONFIGUREANIMATIONEFFECT_H

#include <QDialog>
#include "ui_configureanimationeffect.h"
#include "audiomanager.h"
#include "projectdata_battleanimation.h"
#include "projectdata_globalanimation.h"

class ConfigureAnimationEffect : public QDialog, public Ui::ConfigureAnimationEffect
{
	Q_OBJECT
	public:
		enum AnimationType {ANIM_BATTLE, ANIM_GLOBAL};
		ConfigureAnimationEffect(int animType, int numFrames, QWidget *parent=0);
		~ConfigureAnimationEffect();
		void getAnimationEffect(GlobalAnimation::AnimationEffect *effect, int *frame);
		void setAnimationEffect(GlobalAnimation::AnimationEffect *effect, int frame);
		void getAnimationEffect(BattleAnimation::AnimationEffect *effect, int *frame);
		void setAnimationEffect(BattleAnimation::AnimationEffect *effect, int frame);
	protected slots:
		void on_bBrowseSFX_clicked();
	private:
		AudioManager::SoundEffectInfo soundEffect;
};

#endif // CONFIGUREANIMATIONEFFECT_H
