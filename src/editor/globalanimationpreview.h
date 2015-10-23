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

#ifndef GLOBALANIMATIONPREVIEW_H
#define GLOBALANIMATIONPREVIEW_H

class GlobalAnimation;

#include "ui_globalanimationpreview.h"
#include "mygraphicstinteffect.h"
#include "projectdata.h"

class GlobalAnimationPreview : public QWidget, public Ui::GlobalAnimationPreview
{
	Q_OBJECT
	public:
		GlobalAnimationPreview(QWidget *parent=0);
		~GlobalAnimationPreview();
		void playAnimation(int animID, QPixmap backgroundPixmap);
		void cycleAnimation(int animID);
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
		QGraphicsPixmapItem *backgroundItem;
		QGraphicsRectItem *flashItem;
		QTimer *timer;
		ProjectData::DataReference<GlobalAnimation> *animRef;
		int currentFrame;
		int shakeAmount;
		bool playSounds;
		bool cyclingAnimation;
};

#endif // GLOBALANIMATIONPREVIEW_H
