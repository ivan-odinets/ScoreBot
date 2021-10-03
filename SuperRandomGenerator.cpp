/*
 **********************************************************************************************************************
 *
 * MIT License
 * Copyright (c) 2021 Ivan Odinets
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include "SuperRandomGenerator.h"

#include <cstdlib>
#include <QDebug>

int SuperRandomGenerator::generateScoreChange(int currentLength)
{
    //Absolute score change
    int change = 1 + std::rand()/((RAND_MAX + 1u)/10);

    //if current length < 12 - only increase
    if (currentLength < 12)
        return change;

    //Else - choose randomly
    int signSelector = std::rand()/((RAND_MAX + 1u)/10);
    qDebug() << __FILE__<< ":"<<__LINE__<<" signSelector="<<signSelector<<"; change="<<change<<";";

    if (!(signSelector % 2) || !(signSelector % 3) ) {
        return change;
    } else {
        return -change;
    }
}