/*
 * Copyright (c) 2005-2019 Libor Pecháček.
 *
 * This file is part of CoVe 
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __THINNER_H__
#define __THINNER_H__

#include <QImage>

namespace cove {
class ProgressObserver;

class Morphology
{
protected:
	static unsigned int masks[];
	static bool todelete[];
	static bool isDeletable[];
	static bool isInsertable[];
	static bool isPrunable[];
	QImage image, thinnedImage;
	bool runMorpholo(bool* table, bool insert,
					 ProgressObserver* progressObserver = 0);
	int modifyImage(bool* table, bool insert,
					ProgressObserver* progressObserver = 0);

public:
	Morphology(const QImage& img);
	bool rosenfeld(ProgressObserver* progressObserver = 0);
	bool erosion(ProgressObserver* progressObserver = 0);
	bool dilation(ProgressObserver* progressObserver = 0);
	bool pruning(ProgressObserver* progressObserver = 0);
	QImage getImage() const;
};
} // cove

#endif