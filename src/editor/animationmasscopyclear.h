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

#ifndef ANIMATIONMASSCOPYCLEAR_H
#define ANIMATIONMASSCOPYCLEAR_H

#include "ui_animationmasscopyclear.h"

class AnimationMassCopyClear : public QDialog, public Ui::AnimationMassCopyClear
{
	Q_OBJECT
	public:
		AnimationMassCopyClear(int numFrames, QWidget *parent=0);
		~AnimationMassCopyClear();
		int getStartFrame();
		int getEndFrame();
		int getSourceFrame();
		bool massCopy();
	protected slots:
		void on_sbStartFrame_valueChanged(int value);
		void on_sbEndFrame_valueChanged(int value);
};

#endif // ANIMATIONMASSCOPYCLEAR_H
