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

#include <QComboBox>
#include <QDialogButtonBox>
#include "configurebattlecommands.h"
#include "projectdata.h"
#include "setbattlecommands.h"

SetBattleCommands::SetBattleCommands(QWidget *parent) : QDialog(parent)
{
	ignoreEvents = true;
	setupUi(this);
	command1List.append(-1);
	command2List.append(-1);
	command3List.append(-1);
	command4List.append(-1);
	command5List.append(-1);
	command6List.append(-1);
	updateCommand1List();
	updateCommand2List();
	updateCommand3List();
	updateCommand4List();
	updateCommand5List();
	updateCommand6List();
	resize(sizeHint());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
	ignoreEvents = false;
}

SetBattleCommands::~SetBattleCommands()
{
}

int SetBattleCommands::getCommand1()
{
	return command1List[cboxCommand1->currentIndex()];
}

void SetBattleCommands::setCommand1(int value)
{
	ignoreEvents = true;
	cboxCommand1->setCurrentIndex(command1List.indexOf(value));
	updateCommand2List();
	updateCommand3List();
	updateCommand4List();
	updateCommand5List();
	updateCommand6List();
	if (value == 0)
		updateCommand1List();
	ignoreEvents = false;
}

int SetBattleCommands::getCommand2()
{
	return command2List[cboxCommand2->currentIndex()];
}

void SetBattleCommands::setCommand2(int value)
{
	ignoreEvents = true;
	cboxCommand2->setCurrentIndex(command2List.indexOf(value));
	updateCommand1List();
	updateCommand3List();
	updateCommand4List();
	updateCommand5List();
	updateCommand6List();
	if (value == 0)
	{
		updateCommand2List();
		updateCommand1List();
	}
	ignoreEvents = false;
}

int SetBattleCommands::getCommand3()
{
	return command3List[cboxCommand3->currentIndex()];
}

void SetBattleCommands::setCommand3(int value)
{
	ignoreEvents = true;
	cboxCommand3->setCurrentIndex(command3List.indexOf(value));
	updateCommand1List();
	updateCommand2List();
	updateCommand4List();
	updateCommand5List();
	updateCommand6List();
	if (value == 0)
	{
		updateCommand3List();
		updateCommand2List();
		updateCommand1List();
	}
	ignoreEvents = false;
}

int SetBattleCommands::getCommand4()
{
	return command4List[cboxCommand4->currentIndex()];
}

void SetBattleCommands::setCommand4(int value)
{
	ignoreEvents = true;
	cboxCommand4->setCurrentIndex(command4List.indexOf(value));
	updateCommand1List();
	updateCommand2List();
	updateCommand3List();
	updateCommand5List();
	updateCommand6List();
	if (value == 0)
	{
		updateCommand4List();
		updateCommand3List();
		updateCommand2List();
		updateCommand1List();
	}
	ignoreEvents = false;
}

int SetBattleCommands::getCommand5()
{
	return command5List[cboxCommand5->currentIndex()];
}

void SetBattleCommands::setCommand5(int value)
{
	ignoreEvents = true;
	cboxCommand5->setCurrentIndex(command5List.indexOf(value));
	updateCommand1List();
	updateCommand2List();
	updateCommand3List();
	updateCommand4List();
	updateCommand6List();
	if (value == 0)
	{
		updateCommand5List();
		updateCommand4List();
		updateCommand3List();
		updateCommand2List();
		updateCommand1List();
	}
	ignoreEvents = false;
}

int SetBattleCommands::getCommand6()
{
	return command6List[cboxCommand6->currentIndex()];
}

void SetBattleCommands::setCommand6(int value)
{
	ignoreEvents = true;
	cboxCommand6->setCurrentIndex(command6List.indexOf(value));
	updateCommand1List();
	updateCommand2List();
	updateCommand3List();
	updateCommand4List();
	updateCommand5List();
	if (value == 0)
	{
		updateCommand6List();
		updateCommand5List();
		updateCommand4List();
		updateCommand3List();
		updateCommand2List();
		updateCommand1List();
	}
	ignoreEvents = false;
}

void SetBattleCommands::on_cboxCommand1_currentIndexChanged(int index)
{
	if (!ignoreEvents)
	{
		ignoreEvents = true;
		updateCommand2List();
		updateCommand3List();
		updateCommand4List();
		updateCommand5List();
		updateCommand6List();
		if (index == 0)
			updateCommand1List();
		ignoreEvents = false;
	}
}

void SetBattleCommands::on_cboxCommand2_currentIndexChanged(int index)
{
	if (!ignoreEvents)
	{
		ignoreEvents = true;
		updateCommand1List();
		updateCommand3List();
		updateCommand4List();
		updateCommand5List();
		updateCommand6List();
		if (index == 0)
		{
			updateCommand2List();
			updateCommand1List();
		}
		ignoreEvents = false;
	}
}

void SetBattleCommands::on_cboxCommand3_currentIndexChanged(int index)
{
	if (!ignoreEvents)
	{
		ignoreEvents = true;
		updateCommand1List();
		updateCommand2List();
		updateCommand4List();
		updateCommand5List();
		updateCommand6List();
		if (index == 0)
		{
			updateCommand3List();
			updateCommand2List();
			updateCommand1List();
		}
		ignoreEvents = false;
	}
}

void SetBattleCommands::on_cboxCommand4_currentIndexChanged(int index)
{
	if (!ignoreEvents)
	{
		ignoreEvents = true;
		updateCommand1List();
		updateCommand2List();
		updateCommand3List();
		updateCommand5List();
		updateCommand6List();
		if (index == 0)
		{
			updateCommand4List();
			updateCommand3List();
			updateCommand2List();
			updateCommand1List();
		}
		ignoreEvents = false;
	}
}

void SetBattleCommands::on_cboxCommand5_currentIndexChanged(int index)
{
	if (!ignoreEvents)
	{
		ignoreEvents = true;
		updateCommand1List();
		updateCommand2List();
		updateCommand3List();
		updateCommand4List();
		updateCommand6List();
		if (index == 0)
		{
			updateCommand5List();
			updateCommand4List();
			updateCommand3List();
			updateCommand2List();
			updateCommand1List();
		}
		ignoreEvents = false;
	}
}

void SetBattleCommands::on_cboxCommand6_currentIndexChanged(int index)
{
	if (!ignoreEvents)
	{
		ignoreEvents = true;
		updateCommand1List();
		updateCommand2List();
		updateCommand3List();
		updateCommand4List();
		updateCommand5List();
		if (index == 0)
		{
			updateCommand6List();
			updateCommand5List();
			updateCommand4List();
			updateCommand3List();
			updateCommand2List();
			updateCommand1List();
		}
		ignoreEvents = false;
	}
}

void SetBattleCommands::on_bConfigureCommands_clicked()
{
	ConfigureBattleCommands *configureCommands = new ConfigureBattleCommands;
	if (configureCommands->exec())
	{
		ignoreEvents = true;
		updateCommand1List();
		updateCommand2List();
		updateCommand3List();
		updateCommand4List();
		updateCommand5List();
		updateCommand6List();
		ignoreEvents = false;
	}
	delete configureCommands;
}

void SetBattleCommands::updateCommand1List()
{
	int currentCMD = command1List[cboxCommand1->currentIndex()];
	command1List.clear();
	cboxCommand1->clear();
	command1List.append(-1);
	cboxCommand1->addItem("(NONE)");
	for (int i = 0; i < ProjectData::battleCommands.size(); ++i)
	{
		if (command2List[cboxCommand2->currentIndex()] != i && command3List[cboxCommand3->currentIndex()] != i && command4List[cboxCommand4->currentIndex()] != i && command5List[cboxCommand5->currentIndex()] != i && command6List[cboxCommand6->currentIndex()] != i)
		{
			command1List.append(i);
			cboxCommand1->addItem(ProjectData::battleCommands[i]->name);
		}
	}
	cboxCommand1->setCurrentIndex(command1List.indexOf(currentCMD));
}

void SetBattleCommands::updateCommand2List()
{
	int currentCMD = command2List[cboxCommand2->currentIndex()];
	command2List.clear();
	cboxCommand2->clear();
	command2List.append(-1);
	cboxCommand2->addItem("(NONE)");
	for (int i = 0; i < ProjectData::battleCommands.size(); ++i)
	{
		if (command1List[cboxCommand1->currentIndex()] != i && command3List[cboxCommand3->currentIndex()] != i && command4List[cboxCommand4->currentIndex()] != i && command5List[cboxCommand5->currentIndex()] != i && command6List[cboxCommand6->currentIndex()] != i)
		{
			command2List.append(i);
			cboxCommand2->addItem(ProjectData::battleCommands[i]->name);
		}
	}
	if (cboxCommand1->currentIndex() == 0)
	{
		currentCMD = -1;
		cboxCommand2->setEnabled(false);
	}
	else
		cboxCommand2->setEnabled(true);
	cboxCommand2->setCurrentIndex(command2List.indexOf(currentCMD));
}

void SetBattleCommands::updateCommand3List()
{
	int currentCMD = command3List[cboxCommand3->currentIndex()];
	command3List.clear();
	cboxCommand3->clear();
	command3List.append(-1);
	cboxCommand3->addItem("(NONE)");
	for (int i = 0; i < ProjectData::battleCommands.size(); ++i)
	{
		if (command1List[cboxCommand1->currentIndex()] != i && command2List[cboxCommand2->currentIndex()] != i && command4List[cboxCommand4->currentIndex()] != i && command5List[cboxCommand5->currentIndex()] != i && command6List[cboxCommand6->currentIndex()] != i)
		{
			command3List.append(i);
			cboxCommand3->addItem(ProjectData::battleCommands[i]->name);
		}
	}
	if (cboxCommand2->currentIndex() == 0)
	{
		currentCMD = -1;
		cboxCommand3->setEnabled(false);
	}
	else
		cboxCommand3->setEnabled(true);
	cboxCommand3->setCurrentIndex(command3List.indexOf(currentCMD));
}

void SetBattleCommands::updateCommand4List()
{
	int currentCMD = command4List[cboxCommand4->currentIndex()];
	command4List.clear();
	cboxCommand4->clear();
	command4List.append(-1);
	cboxCommand4->addItem("(NONE)");
	for (int i = 0; i < ProjectData::battleCommands.size(); ++i)
	{
		if (command1List[cboxCommand1->currentIndex()] != i && command2List[cboxCommand2->currentIndex()] != i && command3List[cboxCommand3->currentIndex()] != i && command5List[cboxCommand5->currentIndex()] != i && command6List[cboxCommand6->currentIndex()] != i)
		{
			command4List.append(i);
			cboxCommand4->addItem(ProjectData::battleCommands[i]->name);
		}
	}
	if (cboxCommand3->currentIndex() == 0)
	{
		currentCMD = -1;
		cboxCommand4->setEnabled(false);
	}
	else
		cboxCommand4->setEnabled(true);
	cboxCommand4->setCurrentIndex(command4List.indexOf(currentCMD));
}

void SetBattleCommands::updateCommand5List()
{
	int currentCMD = command5List[cboxCommand5->currentIndex()];
	command5List.clear();
	cboxCommand5->clear();
	command5List.append(-1);
	cboxCommand5->addItem("(NONE)");
	for (int i = 0; i < ProjectData::battleCommands.size(); ++i)
	{
		if (command1List[cboxCommand1->currentIndex()] != i && command2List[cboxCommand2->currentIndex()] != i && command3List[cboxCommand3->currentIndex()] != i && command4List[cboxCommand4->currentIndex()] != i && command6List[cboxCommand6->currentIndex()] != i)
		{
			command5List.append(i);
			cboxCommand5->addItem(ProjectData::battleCommands[i]->name);
		}
	}
	if (cboxCommand4->currentIndex() == 0)
	{
		currentCMD = -1;
		cboxCommand5->setEnabled(false);
	}
	else
		cboxCommand5->setEnabled(true);
	cboxCommand5->setCurrentIndex(command5List.indexOf(currentCMD));
}

void SetBattleCommands::updateCommand6List()
{
	int currentCMD = command6List[cboxCommand6->currentIndex()];
	command6List.clear();
	cboxCommand6->clear();
	command6List.append(-1);
	cboxCommand6->addItem("(NONE)");
	for (int i = 0; i < ProjectData::battleCommands.size(); ++i)
	{
		if (command1List[cboxCommand1->currentIndex()] != i && command2List[cboxCommand2->currentIndex()] != i && command3List[cboxCommand3->currentIndex()] != i && command4List[cboxCommand4->currentIndex()] != i && command5List[cboxCommand5->currentIndex()] != i)
		{
			command6List.append(i);
			cboxCommand6->addItem(ProjectData::battleCommands[i]->name);
		}
	}
	if (cboxCommand5->currentIndex() == 0)
	{
		currentCMD = -1;
		cboxCommand6->setEnabled(false);
	}
	else
		cboxCommand6->setEnabled(true);
	cboxCommand6->setCurrentIndex(command6List.indexOf(currentCMD));
}
