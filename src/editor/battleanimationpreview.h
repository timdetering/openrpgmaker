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

#ifndef BATTLEANIMATIONPREVIEW_H
#define BATTLEANIMATIONPREVIEW_H

class BattleAnimation;

#include "ui_battleanimationpreview.h"
#include "mygraphicstinteffect.h"
#include "projectdata.h"

class BattleAnimationPreview : public QWidget, public Ui::BattleAnimationPreview
{
	Q_OBJECT
	public:
		BattleAnimationPreview(QWidget *parent=0);
		~BattleAnimationPreview();
		void playAnimation(int animID, QPixmap target, bool soundOn=true);
		void cycleAnimation(int animID, QPixmap target, bool soundOn=true);
		void stopAnimation();
		void setScale(float scale);
		bool done;
	protected slots:
		void advanceFrame();
	private:
		void playFrame();
		QGraphicsScene *animationScene;
		QGraphicsPixmapItem *imageItems[16];
		MyGraphicsTintEffect *imageTintEffects[16];
		QGraphicsPixmapItem *targetItem;
		MyGraphicsTintEffect *targetTintEffect;
		QGraphicsRectItem *screenFlashItem;
		QTimer *timer;
		QPixmap targetPixmap;
		ProjectData::DataReference<BattleAnimation> *animRef;
		int currentFrame;
		int shakeScope;
		int shakeAmount;
		bool playSounds;
		bool cyclingAnimation;
		bool hasTarget;
};

#endif // BATTLEANIMATIONPREVIEW_H
