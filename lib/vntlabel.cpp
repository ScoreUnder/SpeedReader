/*
    Copyright 2014 Vanniktech - Niklas Baudy

    This file is part of SpeedReader.

    SpeedReader is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    SpeedReader is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SpeedReader. If not, see <http://www.gnu.org/licenses/>.
*/

#include "vntlabel.h"

VNTLabel::VNTLabel(QWidget* parent) : QLabel(parent) {

}

VNTLabel::VNTLabel(const QString& text, QWidget* parent) : QLabel(text, parent) {
    connect(this, SIGNAL(clicked()), this, SLOT(slotClicked()));
}

void VNTLabel::mouseReleaseEvent(QMouseEvent* mouseEvent) {
    Q_UNUSED(mouseEvent);

    emit clicked();
}
