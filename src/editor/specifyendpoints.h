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

#ifndef SPECIFYENDPOINTS_H
#define SPECIFYENDPOINTS_H

#include "ui_specifyendpoints.h"

class SpecifyEndpoints : public QDialog, public Ui::SpecifyEndpoints
{
	Q_OBJECT
	public:
		SpecifyEndpoints(int level1Endpoint, int level99Endpoint, int maxValue, QWidget *parent=0);
		~SpecifyEndpoints();
		int getLevel1Endpoint();
		int getLevel99Endpoint();
		int getAccelerateGrowth();
};

#endif // SPECIFYENDPOINTS_H
