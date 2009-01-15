/*
 * (C) Copyright 2001-2009 Diomidis Spinellis.  All rights reserved.
 *
 * You may only use this code if you agree to the terms of the CScout
 * Source Code License agreement (see License.txt).
 * If you do not agree to the terms, do not use the code.
 *
 * A character coming from a token.
 *
 * $Id: tchar.h,v 1.8 2009/01/15 14:32:57 dds Exp $
 */

#ifndef TCHAR_
#define TCHAR_

#include <fstream>
#include <stack>
#include <deque>
#include <map>

using namespace std;

#include "fileid.h"
#include "cpp.h"
#include "tokid.h"
#include "tchar.h"
#include "ptoken.h"

class Tchar;

// For pushback; two token stack needed to parse the "..."
typedef stack <Tchar> stackTchar;

typedef deque <Ptoken> dequePtoken;

class Tchar {
private:
	static stackTchar ps;		// Putback stack
	static dequePtoken iq;		// Input queue
	// The following always point to the next character to return
	// If any points to end() it means there is nothing more to return
	// Token from iq use for getnext
	static dequePtoken::const_iterator qi;
	// Token part from *qi to use for getnext
	static dequeTpart::const_iterator pi;
	// Index to character of token part to use for getnext
	static int part_idx;
	// Index to character from token val to use for getnext
	static string::size_type val_idx;

	int val;
	Tokid ti;			// (pos_type from tellg(), fi)

public:
	// Clear queues before pushing new tokens
	static void clear();
	// Call it before the first getnext()
	static void rewind_input();
	// Will read characters from tokens p
	static void push_input(const Ptoken& p);
	// Getnext will return c
	static void putback(Tchar c);

	// Construct an unititialized one
	Tchar() {};
	// Construct it with a given value and tokid
	Tchar(int v, Tokid t) : val(v), ti(t) {}
	Tchar(int v) : val(v) {}
	// Read next from stream
	void getnext();
	// Return the character value (or EOF)
	inline int get_char() const { return (val); }
	// Return the character's Tokid
	inline Tokid get_tokid() const { return (ti); }
	// Return true if the class's source is a file
	static bool is_file_source() { return false; }
};

#endif /* TCHAR_ */
