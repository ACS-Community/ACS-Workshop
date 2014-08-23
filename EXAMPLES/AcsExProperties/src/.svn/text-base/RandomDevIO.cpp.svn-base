/* @(#) $Id: MonitoringTesterImpl.cpp,v 1.1 2007/06/06 23:04:30 rhiriart Exp $
 *
 * Copyright (C) 2001
 * Associated Universities, Inc. Washington DC, USA.
 *
 * Produced for the ALMA project
 *
 * This library is free software; you can redistribute it and/or modify it it
 * under the terms of the GNU Library General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This library is distributed in the hope that it will be useful but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; if not, write to the Free Software Foundation,
 * Inc., 675 Massachusetts Ave, Cambridge, MA, 02139, USA.
 *
 * Correspondence concerning ALMA should be addressed as follows:
 * Internet email: alma-sw-admin@nrao.edu
 */

#include <RandomDevIO.h>
#include <stdlib.h>

RandomDevIO::RandomDevIO() { }

RandomDevIO::~RandomDevIO() { }

double RandomDevIO::read(ACS::Time& timestamp)
{
	timestamp = getTimeStamp();
	double value = static_cast<double>( rand() ) / RAND_MAX;
	return value;
}

void RandomDevIO::write(const double& value, ACS::Time& timestamp)
{
	timestamp = getTimeStamp();
}
