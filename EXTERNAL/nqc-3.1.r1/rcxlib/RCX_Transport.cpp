/*
 * The contents of this file are subject to the Mozilla Public License
 * Version 1.0 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
 * License for the specific language governing rights and limitations
 * under the License.
 *
 * The Initial Developer of this code is David Baum.
 * Portions created by David Baum are Copyright (C) 1998 David Baum.
 * All Rights Reserved.
 *
 * Portions created by John Hansen are Copyright (C) 2005 John Hansen.
 * All Rights Reserved.
 *
 */

#include <cstdio>
#include "RCX_Transport.h"
#include "RCX_Target.h"

using std::printf;
using std::putchar;

void RCX_Transport::DumpData(const UByte *ptr, int length)
{
	int i;

	for(i=0; i<length; i++)
	{
		printf("%02x ", *ptr++);
//		if ((i%16)==15) putchar('\n');
	}

	putchar('\n');
}
