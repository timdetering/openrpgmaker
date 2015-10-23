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

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include "clipboard.h"
#include "database_strings.h"
#include "projectdata.h"
#include "xmlParser.h"

Database_Strings::Database_Strings(QWidget *parent) : QWidget(parent)
{
	setupUi(this);
}

Database_Strings::~Database_Strings()
{
}

void Database_Strings::setupStrings()
{
	leStartBattle->setText(ProjectData::strings[ProjectData::STRING_BATTLEMSG_BEGIN]);
	leBattleInitiative->setText(ProjectData::strings[ProjectData::STRING_BATTLEMSG_INITIATIVE]);
	leAttackMiss->setText(ProjectData::strings[ProjectData::STRING_BATTLEMSG_MISS]);
	leEscapeFailure->setText(ProjectData::strings[ProjectData::STRING_BATTLEMSG_FAILESCAPE]);
	leVictory->setText(ProjectData::strings[ProjectData::STRING_BATTLEMSG_VICTORY]);
	leDefeat->setText(ProjectData::strings[ProjectData::STRING_BATTLEMSG_DEFEAT]);
	leExperienceGain->setText(ProjectData::strings[ProjectData::STRING_BATTLEMSG_EXPERIENCEGAIN]);
	leMoneyAquisition->setText(ProjectData::strings[ProjectData::STRING_BATTLEMSG_MONEYGAIN]);
	leItemDiscovery->setText(ProjectData::strings[ProjectData::STRING_BATTLEMSG_ITEMGAIN]);
	leLevelUp->setText(ProjectData::strings[ProjectData::STRING_LEVELUP]);
	leSkillLearned->setText(ProjectData::strings[ProjectData::STRING_SKILLLEARNED]);
	leShopGreeting->setText(ProjectData::strings[ProjectData::STRING_SHOPMSG_GREETING]);
	leShopAfterFirstTransaction->setText(ProjectData::strings[ProjectData::STRING_SHOPMSG_AFTERFIRSTTRANSACTION]);
	leShopBuy->setText(ProjectData::strings[ProjectData::STRING_SHOPMSG_BUY]);
	leShopSell->setText(ProjectData::strings[ProjectData::STRING_SHOPMSG_SELL]);
	leShopDone->setText(ProjectData::strings[ProjectData::STRING_SHOPMSG_DONE]);
	leShopBuyItem->setText(ProjectData::strings[ProjectData::STRING_SHOPMSG_BUYITEM]);
	leShopBuyQuantity->setText(ProjectData::strings[ProjectData::STRING_SHOPMSG_BUYHOWMANY]);
	leShopBuyComplete->setText(ProjectData::strings[ProjectData::STRING_SHOPMSG_BUYCOMPLETE]);
	leShopSellItem->setText(ProjectData::strings[ProjectData::STRING_SHOPMSG_SELLITEM]);
	leShopSellQuantity->setText(ProjectData::strings[ProjectData::STRING_SHOPMSG_SELLHOWMANY]);
	leShopSellComplete->setText(ProjectData::strings[ProjectData::STRING_SHOPMSG_SELLCOMPLETE]);
	leInnPriceIntro->setText(ProjectData::strings[ProjectData::STRING_INNMSG_PRICEINTRO]);
	leInnAcceptInquiry->setText(ProjectData::strings[ProjectData::STRING_INNMSG_ACCEPTINQUIRY]);
	leInnAccept->setText(ProjectData::strings[ProjectData::STRING_INNMSG_ACCEPT]);
	leInnDecline->setText(ProjectData::strings[ProjectData::STRING_INNMSG_DECLINE]);
	leOtherItemsPossessed->setText(ProjectData::strings[ProjectData::STRING_OTHER_ITEMSPOSSESSED]);
	leOtherItemsEquipped->setText(ProjectData::strings[ProjectData::STRING_OTHER_ITEMSEQUIPPED]);
	leOtherCurrency->setText(ProjectData::strings[ProjectData::STRING_OTHER_CURRENCY]);
	leStatLevel->setText(ProjectData::strings[ProjectData::STRING_STATSANDEQUIP_LEVEL]);
	leStatLevelShortForm->setText(ProjectData::strings[ProjectData::STRING_STATSANDEQUIP_LEVELSHORT]);
	leStatHP->setText(ProjectData::strings[ProjectData::STRING_STATSANDEQUIP_HP]);
	leStatHPShortForm->setText(ProjectData::strings[ProjectData::STRING_STATSANDEQUIP_HPSHORT]);
	leStatMP->setText(ProjectData::strings[ProjectData::STRING_STATSANDEQUIP_MP]);
	leStatMPShortForm->setText(ProjectData::strings[ProjectData::STRING_STATSANDEQUIP_MPSHORT]);
	leStatNormalStatus->setText(ProjectData::strings[ProjectData::STRING_STATSANDEQUIP_NORMALSTATUS]);
	leStatExperience->setText(ProjectData::strings[ProjectData::STRING_STATSANDEQUIP_EXPERIENCE]);
	leStatMPConsumed->setText(ProjectData::strings[ProjectData::STRING_STATSANDEQUIP_MPUSED]);
	leEquipWeapon->setText(ProjectData::strings[ProjectData::STRING_STATSANDEQUIP_WEAPON]);
	leEquipShield->setText(ProjectData::strings[ProjectData::STRING_STATSANDEQUIP_SHIELD]);
	leEquipArmor->setText(ProjectData::strings[ProjectData::STRING_STATSANDEQUIP_ARMOR]);
	leEquipHelmet->setText(ProjectData::strings[ProjectData::STRING_STATSANDEQUIP_HELMET]);
	leEquipAccessory->setText(ProjectData::strings[ProjectData::STRING_STATSANDEQUIP_ACCESSORY]);
	leStatAttack->setText(ProjectData::strings[ProjectData::STRING_STATSANDEQUIP_ATTACK]);
	leStatDefense->setText(ProjectData::strings[ProjectData::STRING_STATSANDEQUIP_DEFENSE]);
	leStatWisdom->setText(ProjectData::strings[ProjectData::STRING_STATSANDEQUIP_WISDOM]);
	leStatAgility->setText(ProjectData::strings[ProjectData::STRING_STATSANDEQUIP_AGILITY]);
	leCmdFight->setText(ProjectData::strings[ProjectData::STRING_COMMAND_FIGHT]);
	leCmdAutoFight->setText(ProjectData::strings[ProjectData::STRING_COMMAND_AUTOFIGHT]);
	leCmdEscape->setText(ProjectData::strings[ProjectData::STRING_COMMAND_ESCAPE]);
	leCmdItem->setText(ProjectData::strings[ProjectData::STRING_COMMAND_ITEM]);
	leCmdSkill->setText(ProjectData::strings[ProjectData::STRING_COMMAND_SKILL]);
	leCmdEquip->setText(ProjectData::strings[ProjectData::STRING_COMMAND_EQUIP]);
	leCmdSave->setText(ProjectData::strings[ProjectData::STRING_COMMAND_SAVE]);
	leCmdQuit->setText(ProjectData::strings[ProjectData::STRING_COMMAND_QUIT]);
	leCmdStatus->setText(ProjectData::strings[ProjectData::STRING_COMMAND_STATUS]);
	leCmdRow->setText(ProjectData::strings[ProjectData::STRING_COMMAND_ROW]);
	leCmdOrder->setText(ProjectData::strings[ProjectData::STRING_COMMAND_ORDER]);
	leCmdNewGame->setText(ProjectData::strings[ProjectData::STRING_COMMAND_NEWGAME]);
	leCmdLoadGame->setText(ProjectData::strings[ProjectData::STRING_COMMAND_LOADGAME]);
	leCmdEndGame->setText(ProjectData::strings[ProjectData::STRING_COMMAND_ENDGAME]);
	leCmdWaitON->setText(ProjectData::strings[ProjectData::STRING_COMMAND_WAITON]);
	leCmdWaitOFF->setText(ProjectData::strings[ProjectData::STRING_COMMAND_WAITOFF]);
	leSaveQuitChooseSaveSlot->setText(ProjectData::strings[ProjectData::STRING_SAVEQUITSCREENMSG_SAVESLOT]);
	leSaveQuitChooseLoadSlot->setText(ProjectData::strings[ProjectData::STRING_SAVEQUITSCREENMSG_LOADSLOT]);
	leSaveQuitConfirmQuit->setText(ProjectData::strings[ProjectData::STRING_SAVEQUITSCREENMSG_CONFIRMQUIT]);
	leSaveQuitYes->setText(ProjectData::strings[ProjectData::STRING_SAVEQUITSCREENMSG_YES]);
	leSaveQuitNo->setText(ProjectData::strings[ProjectData::STRING_SAVEQUITSCREENMSG_NO]);
}

void Database_Strings::shutdownStrings()
{
	ProjectData::strings[ProjectData::STRING_BATTLEMSG_BEGIN] = leStartBattle->text();
	ProjectData::strings[ProjectData::STRING_BATTLEMSG_INITIATIVE] = leBattleInitiative->text();
	ProjectData::strings[ProjectData::STRING_BATTLEMSG_MISS] = leAttackMiss->text();
	ProjectData::strings[ProjectData::STRING_BATTLEMSG_FAILESCAPE] = leEscapeFailure->text();
	ProjectData::strings[ProjectData::STRING_BATTLEMSG_VICTORY] = leVictory->text();
	ProjectData::strings[ProjectData::STRING_BATTLEMSG_DEFEAT] = leDefeat->text();
	ProjectData::strings[ProjectData::STRING_BATTLEMSG_EXPERIENCEGAIN] = leExperienceGain->text();
	ProjectData::strings[ProjectData::STRING_BATTLEMSG_MONEYGAIN] = leMoneyAquisition->text();
	ProjectData::strings[ProjectData::STRING_BATTLEMSG_ITEMGAIN] = leItemDiscovery->text();
	ProjectData::strings[ProjectData::STRING_LEVELUP] = leLevelUp->text();
	ProjectData::strings[ProjectData::STRING_SKILLLEARNED] = leSkillLearned->text();
	ProjectData::strings[ProjectData::STRING_SHOPMSG_GREETING] = leShopGreeting->text();
	ProjectData::strings[ProjectData::STRING_SHOPMSG_AFTERFIRSTTRANSACTION] = leShopAfterFirstTransaction->text();
	ProjectData::strings[ProjectData::STRING_SHOPMSG_BUY] = leShopBuy->text();
	ProjectData::strings[ProjectData::STRING_SHOPMSG_SELL] = leShopSell->text();
	ProjectData::strings[ProjectData::STRING_SHOPMSG_DONE] = leShopDone->text();
	ProjectData::strings[ProjectData::STRING_SHOPMSG_BUYITEM] = leShopBuyItem->text();
	ProjectData::strings[ProjectData::STRING_SHOPMSG_BUYHOWMANY] = leShopBuyQuantity->text();
	ProjectData::strings[ProjectData::STRING_SHOPMSG_BUYCOMPLETE] = leShopBuyComplete->text();
	ProjectData::strings[ProjectData::STRING_SHOPMSG_SELLITEM] = leShopSellItem->text();
	ProjectData::strings[ProjectData::STRING_SHOPMSG_SELLHOWMANY] = leShopSellQuantity->text();
	ProjectData::strings[ProjectData::STRING_SHOPMSG_SELLCOMPLETE] = leShopSellComplete->text();
	ProjectData::strings[ProjectData::STRING_INNMSG_PRICEINTRO] = leInnPriceIntro->text();
	ProjectData::strings[ProjectData::STRING_INNMSG_ACCEPTINQUIRY] = leInnAcceptInquiry->text();
	ProjectData::strings[ProjectData::STRING_INNMSG_ACCEPT] = leInnAccept->text();
	ProjectData::strings[ProjectData::STRING_INNMSG_DECLINE] = leInnDecline->text();
	ProjectData::strings[ProjectData::STRING_OTHER_ITEMSPOSSESSED] = leOtherItemsPossessed->text();
	ProjectData::strings[ProjectData::STRING_OTHER_ITEMSEQUIPPED] = leOtherItemsEquipped->text();
	ProjectData::strings[ProjectData::STRING_OTHER_CURRENCY] = leOtherCurrency->text();
	ProjectData::strings[ProjectData::STRING_STATSANDEQUIP_LEVEL] = leStatLevel->text();
	ProjectData::strings[ProjectData::STRING_STATSANDEQUIP_LEVELSHORT] = leStatLevelShortForm->text();
	ProjectData::strings[ProjectData::STRING_STATSANDEQUIP_HP] = leStatHP->text();
	ProjectData::strings[ProjectData::STRING_STATSANDEQUIP_HPSHORT] = leStatHPShortForm->text();
	ProjectData::strings[ProjectData::STRING_STATSANDEQUIP_MP] = leStatMP->text();
	ProjectData::strings[ProjectData::STRING_STATSANDEQUIP_MPSHORT] = leStatMPShortForm->text();
	ProjectData::strings[ProjectData::STRING_STATSANDEQUIP_NORMALSTATUS] = leStatNormalStatus->text();
	ProjectData::strings[ProjectData::STRING_STATSANDEQUIP_EXPERIENCE] = leStatExperience->text();
	ProjectData::strings[ProjectData::STRING_STATSANDEQUIP_MPUSED] = leStatMPConsumed->text();
	ProjectData::strings[ProjectData::STRING_STATSANDEQUIP_WEAPON] = leEquipWeapon->text();
	ProjectData::strings[ProjectData::STRING_STATSANDEQUIP_SHIELD] = leEquipShield->text();
	ProjectData::strings[ProjectData::STRING_STATSANDEQUIP_ARMOR] = leEquipArmor->text();
	ProjectData::strings[ProjectData::STRING_STATSANDEQUIP_HELMET] = leEquipHelmet->text();
	ProjectData::strings[ProjectData::STRING_STATSANDEQUIP_ACCESSORY] = leEquipAccessory->text();
	ProjectData::strings[ProjectData::STRING_STATSANDEQUIP_ATTACK] = leStatAttack->text();
	ProjectData::strings[ProjectData::STRING_STATSANDEQUIP_DEFENSE] = leStatDefense->text();
	ProjectData::strings[ProjectData::STRING_STATSANDEQUIP_WISDOM] = leStatWisdom->text();
	ProjectData::strings[ProjectData::STRING_STATSANDEQUIP_AGILITY] = leStatAgility->text();
	ProjectData::strings[ProjectData::STRING_COMMAND_FIGHT] = leCmdFight->text();
	ProjectData::strings[ProjectData::STRING_COMMAND_AUTOFIGHT] = leCmdAutoFight->text();
	ProjectData::strings[ProjectData::STRING_COMMAND_ESCAPE] = leCmdEscape->text();
	ProjectData::strings[ProjectData::STRING_COMMAND_ITEM] = leCmdItem->text();
	ProjectData::strings[ProjectData::STRING_COMMAND_SKILL] = leCmdSkill->text();
	ProjectData::strings[ProjectData::STRING_COMMAND_EQUIP] = leCmdEquip->text();
	ProjectData::strings[ProjectData::STRING_COMMAND_SAVE] = leCmdSave->text();
	ProjectData::strings[ProjectData::STRING_COMMAND_QUIT] = leCmdQuit->text();
	ProjectData::strings[ProjectData::STRING_COMMAND_STATUS] = leCmdStatus->text();
	ProjectData::strings[ProjectData::STRING_COMMAND_ROW] = leCmdRow->text();
	ProjectData::strings[ProjectData::STRING_COMMAND_ORDER] = leCmdOrder->text();
	ProjectData::strings[ProjectData::STRING_COMMAND_WAITON] = leCmdWaitON->text();
	ProjectData::strings[ProjectData::STRING_COMMAND_NEWGAME] = leCmdNewGame->text();
	ProjectData::strings[ProjectData::STRING_COMMAND_LOADGAME] = leCmdLoadGame->text();
	ProjectData::strings[ProjectData::STRING_COMMAND_ENDGAME] = leCmdEndGame->text();
	ProjectData::strings[ProjectData::STRING_COMMAND_WAITOFF] = leCmdWaitOFF->text();
	ProjectData::strings[ProjectData::STRING_SAVEQUITSCREENMSG_SAVESLOT] = leSaveQuitChooseSaveSlot->text();
	ProjectData::strings[ProjectData::STRING_SAVEQUITSCREENMSG_LOADSLOT] = leSaveQuitChooseLoadSlot->text();
	ProjectData::strings[ProjectData::STRING_SAVEQUITSCREENMSG_CONFIRMQUIT] = leSaveQuitConfirmQuit->text();
	ProjectData::strings[ProjectData::STRING_SAVEQUITSCREENMSG_YES] = leSaveQuitYes->text();
	ProjectData::strings[ProjectData::STRING_SAVEQUITSCREENMSG_NO] = leSaveQuitNo->text();
}
