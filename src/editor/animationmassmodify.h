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

#ifndef ANIMATIONMASSMODIFY_H
#define ANIMATIONMASSMODIFY_H

#include "ui_animationmassmodify.h"

class AnimationMassModify : public QDialog, public Ui::AnimationMassModify
{
	Q_OBJECT
	public:
		AnimationMassModify(int numFrames, int numImages, QWidget *parent=0);
		~AnimationMassModify();
		QList<bool> getAffectedFrames();
		QList<bool> getAffectedCells();
		bool modifyImageID();
		int getImageID();
		bool modifyScale();
		int getScale();
		bool modifyRotation();
		int getRotation();
		bool modifyOpacity();
		int getOpacity();
		bool modifyTintColor();
		QColor getTintColor();
		bool modifyTintAmount();
		int getTintAmount();
	private:
		QList<QCheckBox*> frameCheckBoxes;
		QList<QCheckBox*> cellCheckBoxes;
};

#endif // ANIMATIONMASSMODIFY_H
