/*
 * (C) Copyright 2004-2009 Diomidis Spinellis.  All rights reserved.
 *
 * You may only use this code if you agree to the terms of the CScout
 * Source Code License agreement (see License.txt).
 * If you do not agree to the terms, do not use the code.
 *
 * The CScout logo access functions
 *
 * $Id: logo.h,v 1.3 2009/01/15 14:32:57 dds Exp $
 */

#ifndef LOGO_
#define LOGO_

#include <cstdio>

using namespace std;

class Logo {
private:
	static const char logo_bytes[];
	static int len;
public:
	static void logo(FILE *fo) {
		fwrite(logo_bytes, len, 1, fo);
	}
};

#endif // LOGO_
