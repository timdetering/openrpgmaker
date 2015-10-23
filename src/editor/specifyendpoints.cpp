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

#include <QSpinBox>
#include <QSlider>
#include "specifyendpoints.h"

SpecifyEndpoints::SpecifyEndpoints(int level1Endpoint, int level99Endpoint, int maxValue, QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	sbLevel1Endpoint->setMaximum(maxValue);
	sbLevel99Endpoint->setMaximum(maxValue);
	sbLevel1Endpoint->setValue(level1Endpoint);
	sbLevel99Endpoint->setValue(level99Endpoint);
	bLevel1EndpointSpinButton->setSpinBox(sbLevel1Endpoint);
	bLevel99EndpointSpinButton->setSpinBox(sbLevel99Endpoint);
	resize(sizeHint());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
}

SpecifyEndpoints::~SpecifyEndpoints()
{
}

int SpecifyEndpoints::getLevel1Endpoint()
{
	return sbLevel1Endpoint->value();
}

int SpecifyEndpoints::getLevel99Endpoint()
{
	return sbLevel99Endpoint->value();
}

int SpecifyEndpoints::getAccelerateGrowth()
{
	return sldrAccelerateGrowth->value();
}
