/*
 **********************************************************************************************************************
 *
 * This file is part of ScoreBot project
 *
 * Copyright (c) 2021-2023 Ivan Odinets <i_odinets@protonmail.com>
 *
 * ScoreBot is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, either version
 * 3 of the License, or (at your option) any later version.
 *
 * ScoreBot is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ScoreBot; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#include "SuperRandomGenerator.h"

#include <cstdlib>
#include <QDebug>

int SuperRandomGenerator::generateScoreChange(int currentScore)
{
    //Absolute score change
    int change = 1 + std::rand()/((RAND_MAX + 1u)/10);

    //if current score < 12 - only increase
    if (currentScore < 12)
        return change;

    //Else - choose randomly
    int signSelector = std::rand()/((RAND_MAX + 1u)/100);

    if ( signSelector < 60 ) {
        return change;
    } else {
        return -change;
    }
}
