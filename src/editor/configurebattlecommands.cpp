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

#include <QMessageBox>
#include "arraysize.h"
#include "configurebattlecommands.h"
#include "projectdata.h"
#include "storagefile.h"

ConfigureBattleCommands::ConfigureBattleCommands(QWidget *parent) : QDialog(parent)
{
	ignoreEvents = true;
	setupUi(this);
	for (int i = 0; i < ProjectData::battleCommands.size(); ++i)
	{
		ProjectData::BattleCommand *command = new ProjectData::BattleCommand;
		command->name = ProjectData::battleCommands[i]->name;
		command->type = ProjectData::battleCommands[i]->type;
		battleCommands.append(command);
	}
	updateCommandList();
	resize(sizeHint());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
	ignoreEvents = false;
}

ConfigureBattleCommands::~ConfigureBattleCommands()
{
}

void ConfigureBattleCommands::on_lwCommandList_itemSelectionChanged()
{
	int currentRow = lwCommandList->currentRow();
	if (currentRow >= 0)
	{
		ignoreEvents = true;
		leName->setText(battleCommands[currentRow]->name);
		cboxType->setCurrentIndex(battleCommands[currentRow]->type);
		ignoreEvents = false;
	}
}

void ConfigureBattleCommands::on_bArraySize_clicked()
{
	int numBattleCommands = battleCommands.size();
	ArraySize *arraysize = new ArraySize(numBattleCommands, 1, 99);
	if (arraysize->exec())
	{
		int newSize = arraysize->getValue();
		if (newSize < numBattleCommands)
		{
			int confirmdelete = QMessageBox::warning(0, "Delete Data?", QString("If you decrease the array size then\nany unsaved changes will be perminately lost.\nIs it ok to delete data?"), QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
			if (confirmdelete == QMessageBox::Yes)
			{
				for (int i = numBattleCommands - 1; i >= newSize; --i)
				{
					if (battleCommands[i] != NULL)
					{
						delete (battleCommands[i]);
						battleCommands[i] = NULL;
					}
					battleCommands.erase(battleCommands.begin() + i);
				}
			}
		}
		else
		{
			for (int i = numBattleCommands; i < newSize; ++i)
			{
				ProjectData::BattleCommand *command = new ProjectData::BattleCommand;
				command->name = QString("Cmd%1").arg(i, 2, 10, QChar('0'));
				command->type = ProjectData::COMMAND_ATTACK;
				battleCommands.append(command);
			}
		}
		ignoreEvents = true;
		updateCommandList();
		leName->setText(battleCommands[lwCommandList->currentRow()]->name);
		cboxType->setCurrentIndex(battleCommands[lwCommandList->currentRow()]->type);
		ignoreEvents = false;
	}
	delete arraysize;
}

void ConfigureBattleCommands::on_leName_textChanged(QString text)
{
	if (!ignoreEvents)
	{
		int currentRow = lwCommandList->currentRow();
		battleCommands[currentRow]->name = text;
		lwCommandList->item(currentRow)->setText(QString("%1: %2").arg(currentRow, 2, 10, QChar('0')).arg(text));
	}
}

void ConfigureBattleCommands::on_cboxType_currentIndexChanged(int index)
{
	if (!ignoreEvents)
		battleCommands[lwCommandList->currentRow()]->type = index;
}

void ConfigureBattleCommands::on_buttonBox_clicked(QAbstractButton *button)
{
	int buttonClicked = buttonBox->standardButton(button);
	if (buttonClicked == QDialogButtonBox::Ok)
	{
		for (int i = 0; i < ProjectData::battleCommands.size(); ++i)
		{
			if (ProjectData::battleCommands[i] != NULL)
			{
				delete (ProjectData::battleCommands[i]);
				ProjectData::battleCommands[i] = NULL;
			}
		}
		ProjectData::battleCommands.clear();
		for (int i = 0; i < battleCommands.size(); ++i)
		{
			ProjectData::battleCommands.append(battleCommands[i]);
			battleCommands[i] = NULL;
		}
		battleCommands.clear();
		ProjectData::saveToStorage();
		ProjectData::saved = false;
		accept();
	}
	else if (buttonClicked == QDialogButtonBox::Cancel)
	{
		for (int i = 0; i < battleCommands.size(); ++i)
		{
			if (battleCommands[i] != NULL)
			{
				delete (battleCommands[i]);
				battleCommands[i] = NULL;
			}
		}
		battleCommands.clear();
		reject();
	}
	else if (buttonClicked == QDialogButtonBox::Help)
	{
		ProjectData::helpManual->loadIndex("Configure Battle Commands");
		if (!ProjectData::helpManual->isVisible())
			ProjectData::helpManual->show();
		else
			ProjectData::helpManual->raise();
		ProjectData::helpManual->activateWindow();
	}
}

void ConfigureBattleCommands::updateCommandList()
{
	int currentRow = lwCommandList->currentRow();
	lwCommandList->clear();
	for (int i = 0; i < battleCommands.size(); ++i)
		lwCommandList->addItem(QString("%1: %2").arg(i, 2, 10, QChar('0')).arg(battleCommands[i]->name));
	if (currentRow >= battleCommands.size())
		currentRow = battleCommands.size() - 1;
	else if (currentRow < 0)
		currentRow = 0;
	if (battleCommands.size() > 0)
	{
		lwCommandList->setCurrentRow(currentRow);
		lwCommandList->item(currentRow)->setSelected(true);
	}
}
