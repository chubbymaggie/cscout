/*
 * (C) Copyright 2001-2009 Diomidis Spinellis.  All rights reserved.
 *
 * You may only use this code if you agree to the terms of the CScout
 * Source Code License agreement (see License.txt).
 * If you do not agree to the terms, do not use the code.
 *
 * For documentation read the corresponding .h file
 *
 * $Id: tchar.cpp,v 1.16 2009/01/15 14:32:57 dds Exp $
 */

#include <iostream>
#include <map>
#include <string>
#include <deque>
#include <fstream>
#include <list>
#include <vector>
#include <stack>
#include <set>

#include "cpp.h"
#include "error.h"
#include "attr.h"
#include "metrics.h"
#include "fileid.h"
#include "tokid.h"
#include "token.h"
#include "ytab.h"
#include "ptoken.h"
#include "fchar.h"
#include "tchar.h"

stackTchar Tchar::ps;			// Putback Tchars (from putback())
dequePtoken Tchar::iq;		// Input queue
dequePtoken::const_iterator Tchar::qi;
dequeTpart::const_iterator Tchar::pi;
int Tchar::part_idx;
string::size_type Tchar::val_idx;

void
Tchar::putback(Tchar c)
{
	ps.push(c);
}


void
Tchar::getnext()
{
	if (!ps.empty()) {
		*this = ps.top();
		ps.pop();
		return;
	}
	if (qi == iq.end()) {
		val = EOF;
		return;
	}
	const Ptoken& pt = (*qi);
	val = pt.get_val()[val_idx++];
	if (pt.get_parts_begin() == pt.get_parts_end()) {
		// This is a simple token that does not consist of parts
		// Tokid is unknown, advance pointers
		if (val_idx == pt.get_val().length()) {
			if (++qi == iq.end())
				return;
			pi = (*qi).get_parts_begin();
			val_idx = part_idx = 0;
		}
	} else {
		// Identifier or identifier to be token (PP_NUMBER) consisting
		// of parts
		const Tpart& tp = *pi;
		ti = tp.get_tokid() + part_idx;
		// Advance pointers
		if (++part_idx == tp.get_len()) {
			if (++pi == pt.get_parts_end()) {
				if (++qi == iq.end())
					return;
				pi = (*qi).get_parts_begin();
				val_idx = 0;
			}
			part_idx = 0;
		}
	}
}

void
Tchar::rewind_input()
{
	qi = iq.begin();
	pi = (*qi).get_parts_begin();
	part_idx = val_idx = 0;
}

void
Tchar::clear()
{
	iq.clear();
	while (!ps.empty())
		ps.pop();
}

void
Tchar::push_input(const Ptoken& p)
{
	switch (p.get_code()) {
	case STRING_LITERAL:
		iq.push_back(Ptoken(STRING_LITERAL, "\"" + p.get_val() + "\""));
		break;
	case CHAR_LITERAL:
		iq.push_back(Ptoken(CHAR_LITERAL, "\'" + p.get_val() + "\'"));
		break;
	default:
		iq.push_back(p);
		break;
	}
}
