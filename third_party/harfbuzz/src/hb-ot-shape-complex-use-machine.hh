
#line 1 "hb-ot-shape-complex-use-machine.rl"
/*
 * Copyright © 2015  Mozilla Foundation.
 * Copyright © 2015  Google, Inc.
 *
 *  This is part of HarfBuzz, a text shaping library.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that the
 * above copyright notice and the following two paragraphs appear in
 * all copies of this software.
 *
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 * ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN
 * IF THE COPYRIGHT HOLDER HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * THE COPYRIGHT HOLDER SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE COPYRIGHT HOLDER HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 * Mozilla Author(s): Jonathan Kew
 * Google Author(s): Behdad Esfahbod
 */

#ifndef HB_OT_SHAPE_COMPLEX_USE_MACHINE_HH
#define HB_OT_SHAPE_COMPLEX_USE_MACHINE_HH

#include "hb-private.hh"


#line 38 "hb-ot-shape-complex-use-machine.hh"
static const char _use_syllable_machine_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 4, 1, 5, 1, 6, 1, 
	7, 1, 8, 1, 9, 1, 10, 1, 
	11, 1, 12, 1, 13, 1, 14, 1, 
	15
};

static const short _use_syllable_machine_key_offsets[] = {
	0, 1, 30, 31, 52, 73, 74, 94, 
	106, 114, 118, 121, 123, 130, 136, 141, 
	152, 162, 171, 185, 198, 213, 232, 252, 
	254, 255, 256, 277, 297, 309, 317, 321, 
	324, 326, 333, 339, 344, 355, 365, 374, 
	388, 401, 416, 435, 436, 458, 478, 481, 
	483
};

static const unsigned char _use_syllable_machine_trans_keys[] = {
	1u, 0u, 1u, 3u, 4u, 5u, 8u, 11u, 
	12u, 18u, 19u, 23u, 24u, 25u, 26u, 27u, 
	28u, 30u, 31u, 32u, 33u, 34u, 35u, 37u, 
	38u, 39u, 16u, 17u, 22u, 29u, 21u, 8u, 
	11u, 12u, 22u, 23u, 24u, 25u, 26u, 27u, 
	28u, 29u, 30u, 32u, 33u, 34u, 35u, 37u, 
	38u, 39u, 21u, 31u, 8u, 11u, 12u, 21u, 
	23u, 24u, 25u, 26u, 27u, 28u, 30u, 33u, 
	34u, 35u, 37u, 38u, 39u, 22u, 29u, 31u, 
	32u, 1u, 8u, 11u, 12u, 23u, 24u, 25u, 
	26u, 27u, 28u, 30u, 33u, 34u, 35u, 37u, 
	38u, 39u, 22u, 29u, 31u, 32u, 8u, 22u, 
	23u, 24u, 25u, 26u, 33u, 34u, 35u, 37u, 
	38u, 39u, 8u, 23u, 24u, 25u, 26u, 37u, 
	38u, 39u, 8u, 24u, 25u, 26u, 8u, 25u, 
	26u, 8u, 26u, 8u, 24u, 25u, 26u, 37u, 
	38u, 39u, 8u, 24u, 25u, 26u, 38u, 39u, 
	8u, 24u, 25u, 26u, 39u, 8u, 23u, 24u, 
	25u, 26u, 33u, 34u, 35u, 37u, 38u, 39u, 
	8u, 23u, 24u, 25u, 26u, 34u, 35u, 37u, 
	38u, 39u, 8u, 23u, 24u, 25u, 26u, 35u, 
	37u, 38u, 39u, 8u, 22u, 23u, 24u, 25u, 
	26u, 28u, 29u, 33u, 34u, 35u, 37u, 38u, 
	39u, 8u, 22u, 23u, 24u, 25u, 26u, 29u, 
	33u, 34u, 35u, 37u, 38u, 39u, 8u, 23u, 
	24u, 25u, 26u, 27u, 28u, 33u, 34u, 35u, 
	37u, 38u, 39u, 22u, 29u, 8u, 11u, 12u, 
	23u, 24u, 25u, 26u, 27u, 28u, 30u, 32u, 
	33u, 34u, 35u, 37u, 38u, 39u, 22u, 29u, 
	8u, 11u, 12u, 23u, 24u, 25u, 26u, 27u, 
	28u, 30u, 31u, 32u, 33u, 34u, 35u, 37u, 
	38u, 39u, 22u, 29u, 13u, 21u, 4u, 13u, 
	8u, 11u, 12u, 21u, 23u, 24u, 25u, 26u, 
	27u, 28u, 30u, 33u, 34u, 35u, 37u, 38u, 
	39u, 22u, 29u, 31u, 32u, 8u, 11u, 12u, 
	23u, 24u, 25u, 26u, 27u, 28u, 30u, 33u, 
	34u, 35u, 37u, 38u, 39u, 22u, 29u, 31u, 
	32u, 8u, 22u, 23u, 24u, 25u, 26u, 33u, 
	34u, 35u, 37u, 38u, 39u, 8u, 23u, 24u, 
	25u, 26u, 37u, 38u, 39u, 8u, 24u, 25u, 
	26u, 8u, 25u, 26u, 8u, 26u, 8u, 24u, 
	25u, 26u, 37u, 38u, 39u, 8u, 24u, 25u, 
	26u, 38u, 39u, 8u, 24u, 25u, 26u, 39u, 
	8u, 23u, 24u, 25u, 26u, 33u, 34u, 35u, 
	37u, 38u, 39u, 8u, 23u, 24u, 25u, 26u, 
	34u, 35u, 37u, 38u, 39u, 8u, 23u, 24u, 
	25u, 26u, 35u, 37u, 38u, 39u, 8u, 22u, 
	23u, 24u, 25u, 26u, 28u, 29u, 33u, 34u, 
	35u, 37u, 38u, 39u, 8u, 22u, 23u, 24u, 
	25u, 26u, 29u, 33u, 34u, 35u, 37u, 38u, 
	39u, 8u, 23u, 24u, 25u, 26u, 27u, 28u, 
	33u, 34u, 35u, 37u, 38u, 39u, 22u, 29u, 
	8u, 11u, 12u, 23u, 24u, 25u, 26u, 27u, 
	28u, 30u, 32u, 33u, 34u, 35u, 37u, 38u, 
	39u, 22u, 29u, 1u, 1u, 5u, 8u, 11u, 
	12u, 23u, 24u, 25u, 26u, 27u, 28u, 30u, 
	31u, 32u, 33u, 34u, 35u, 37u, 38u, 39u, 
	22u, 29u, 8u, 11u, 12u, 23u, 24u, 25u, 
	26u, 27u, 28u, 30u, 31u, 32u, 33u, 34u, 
	35u, 37u, 38u, 39u, 22u, 29u, 21u, 41u, 
	42u, 41u, 42u, 42u, 0
};

static const char _use_syllable_machine_single_lengths[] = {
	1, 25, 1, 19, 17, 1, 16, 12, 
	8, 4, 3, 2, 7, 6, 5, 11, 
	10, 9, 14, 13, 13, 17, 18, 2, 
	1, 1, 17, 16, 12, 8, 4, 3, 
	2, 7, 6, 5, 11, 10, 9, 14, 
	13, 13, 17, 1, 20, 18, 3, 2, 
	1
};

static const char _use_syllable_machine_range_lengths[] = {
	0, 2, 0, 1, 2, 0, 2, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 1, 1, 1, 0, 
	0, 0, 2, 2, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 1, 1, 0, 1, 1, 0, 0, 
	0
};

static const short _use_syllable_machine_index_offsets[] = {
	0, 2, 30, 32, 53, 73, 75, 94, 
	107, 116, 121, 125, 128, 136, 143, 149, 
	161, 172, 182, 197, 211, 226, 245, 265, 
	268, 270, 272, 292, 311, 324, 333, 338, 
	342, 345, 353, 360, 366, 378, 389, 399, 
	414, 428, 443, 462, 464, 486, 506, 510, 
	513
};

static const char _use_syllable_machine_indicies[] = {
	1, 0, 2, 3, 2, 5, 3, 6, 
	1, 7, 8, 9, 11, 12, 13, 14, 
	15, 16, 17, 18, 19, 20, 21, 22, 
	23, 24, 25, 2, 10, 4, 27, 26, 
	29, 30, 31, 33, 34, 35, 36, 37, 
	38, 39, 33, 40, 41, 42, 43, 44, 
	45, 46, 47, 32, 28, 29, 30, 31, 
	48, 34, 35, 36, 37, 38, 39, 40, 
	42, 43, 44, 45, 46, 47, 33, 41, 
	28, 30, 49, 29, 30, 31, 34, 35, 
	36, 37, 38, 39, 40, 42, 43, 44, 
	45, 46, 47, 33, 41, 28, 29, 33, 
	34, 35, 36, 37, 42, 43, 44, 45, 
	46, 47, 28, 29, 34, 35, 36, 37, 
	45, 46, 47, 28, 29, 35, 36, 37, 
	28, 29, 36, 37, 28, 29, 37, 28, 
	29, 35, 36, 37, 45, 46, 47, 28, 
	29, 35, 36, 37, 46, 47, 28, 29, 
	35, 36, 37, 47, 28, 29, 34, 35, 
	36, 37, 42, 43, 44, 45, 46, 47, 
	28, 29, 34, 35, 36, 37, 43, 44, 
	45, 46, 47, 28, 29, 34, 35, 36, 
	37, 44, 45, 46, 47, 28, 29, 33, 
	34, 35, 36, 37, 39, 33, 42, 43, 
	44, 45, 46, 47, 28, 29, 33, 34, 
	35, 36, 37, 33, 42, 43, 44, 45, 
	46, 47, 28, 29, 34, 35, 36, 37, 
	38, 39, 42, 43, 44, 45, 46, 47, 
	33, 28, 29, 30, 31, 34, 35, 36, 
	37, 38, 39, 40, 41, 42, 43, 44, 
	45, 46, 47, 33, 28, 29, 30, 31, 
	34, 35, 36, 37, 38, 39, 40, 32, 
	41, 42, 43, 44, 45, 46, 47, 33, 
	28, 51, 52, 50, 5, 53, 51, 50, 
	6, 1, 55, 56, 11, 12, 13, 14, 
	15, 16, 17, 20, 21, 22, 23, 24, 
	25, 10, 19, 54, 6, 1, 55, 11, 
	12, 13, 14, 15, 16, 17, 20, 21, 
	22, 23, 24, 25, 10, 19, 54, 6, 
	10, 11, 12, 13, 14, 20, 21, 22, 
	23, 24, 25, 54, 6, 11, 12, 13, 
	14, 23, 24, 25, 54, 6, 12, 13, 
	14, 54, 6, 13, 14, 54, 6, 14, 
	54, 6, 12, 13, 14, 23, 24, 25, 
	54, 6, 12, 13, 14, 24, 25, 54, 
	6, 12, 13, 14, 25, 54, 6, 11, 
	12, 13, 14, 20, 21, 22, 23, 24, 
	25, 54, 6, 11, 12, 13, 14, 21, 
	22, 23, 24, 25, 54, 6, 11, 12, 
	13, 14, 22, 23, 24, 25, 54, 6, 
	10, 11, 12, 13, 14, 16, 10, 20, 
	21, 22, 23, 24, 25, 54, 6, 10, 
	11, 12, 13, 14, 10, 20, 21, 22, 
	23, 24, 25, 54, 6, 11, 12, 13, 
	14, 15, 16, 20, 21, 22, 23, 24, 
	25, 10, 54, 6, 1, 55, 11, 12, 
	13, 14, 15, 16, 17, 19, 20, 21, 
	22, 23, 24, 25, 10, 54, 1, 57, 
	3, 3, 6, 1, 55, 11, 12, 13, 
	14, 15, 16, 17, 18, 19, 20, 21, 
	22, 23, 24, 25, 10, 54, 6, 1, 
	55, 11, 12, 13, 14, 15, 16, 17, 
	18, 19, 20, 21, 22, 23, 24, 25, 
	10, 54, 59, 59, 60, 58, 59, 60, 
	58, 60, 58, 0
};

static const char _use_syllable_machine_trans_targs[] = {
	1, 26, 2, 3, 1, 23, 1, 43, 
	44, 46, 28, 29, 30, 31, 32, 39, 
	40, 41, 45, 42, 36, 37, 38, 33, 
	34, 35, 1, 1, 1, 1, 4, 5, 
	22, 7, 8, 9, 10, 11, 18, 19, 
	20, 21, 15, 16, 17, 12, 13, 14, 
	6, 1, 1, 24, 25, 1, 1, 0, 
	27, 1, 1, 47, 48
};

static const char _use_syllable_machine_trans_actions[] = {
	31, 5, 0, 0, 13, 0, 11, 0, 
	5, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 5, 5, 0, 0, 0, 0, 
	0, 0, 15, 7, 19, 9, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 17, 23, 0, 0, 21, 27, 0, 
	5, 29, 25, 0, 0
};

static const char _use_syllable_machine_to_state_actions[] = {
	0, 1, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0
};

static const char _use_syllable_machine_from_state_actions[] = {
	0, 3, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0
};

static const short _use_syllable_machine_eof_trans[] = {
	1, 0, 27, 29, 29, 50, 29, 29, 
	29, 29, 29, 29, 29, 29, 29, 29, 
	29, 29, 29, 29, 29, 29, 29, 51, 
	54, 51, 55, 55, 55, 55, 55, 55, 
	55, 55, 55, 55, 55, 55, 55, 55, 
	55, 55, 55, 58, 55, 55, 59, 59, 
	59
};

static const int use_syllable_machine_start = 1;
static const int use_syllable_machine_first_final = 1;
static const int use_syllable_machine_error = -1;

static const int use_syllable_machine_en_main = 1;


#line 38 "hb-ot-shape-complex-use-machine.rl"



#line 138 "hb-ot-shape-complex-use-machine.rl"


#define found_syllable(syllable_type) \
  HB_STMT_START { \
    if (0) fprintf (stderr, "syllable %d..%d %s\n", last, p+1, #syllable_type); \
    for (unsigned int i = last; i < p+1; i++) \
      info[i].syllable() = (syllable_serial << 4) | syllable_type; \
    last = p+1; \
    syllable_serial++; \
    if (unlikely (syllable_serial == 16)) syllable_serial = 1; \
  } HB_STMT_END

static void
find_syllables (hb_buffer_t *buffer)
{
  unsigned int p, pe, eof, ts HB_UNUSED, te HB_UNUSED, act HB_UNUSED;
  int cs;
  hb_glyph_info_t *info = buffer->info;
  
#line 302 "hb-ot-shape-complex-use-machine.hh"
	{
	cs = use_syllable_machine_start;
	ts = 0;
	te = 0;
	act = 0;
	}

#line 159 "hb-ot-shape-complex-use-machine.rl"


  p = 0;
  pe = eof = buffer->len;

  unsigned int last = 0;
  unsigned int syllable_serial = 1;
  
#line 319 "hb-ot-shape-complex-use-machine.hh"
	{
	int _klen;
	unsigned int _trans;
	const char *_acts;
	unsigned int _nacts;
	const unsigned char *_keys;

	if ( p == pe )
		goto _test_eof;
_resume:
	_acts = _use_syllable_machine_actions + _use_syllable_machine_from_state_actions[cs];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 ) {
		switch ( *_acts++ ) {
	case 1:
#line 1 "NONE"
	{ts = p;}
	break;
#line 338 "hb-ot-shape-complex-use-machine.hh"
		}
	}

	_keys = _use_syllable_machine_trans_keys + _use_syllable_machine_key_offsets[cs];
	_trans = _use_syllable_machine_index_offsets[cs];

	_klen = _use_syllable_machine_single_lengths[cs];
	if ( _klen > 0 ) {
		const unsigned char *_lower = _keys;
		const unsigned char *_mid;
		const unsigned char *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( ( info[p].use_category()) < *_mid )
				_upper = _mid - 1;
			else if ( ( info[p].use_category()) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (unsigned int)(_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _use_syllable_machine_range_lengths[cs];
	if ( _klen > 0 ) {
		const unsigned char *_lower = _keys;
		const unsigned char *_mid;
		const unsigned char *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( ( info[p].use_category()) < _mid[0] )
				_upper = _mid - 2;
			else if ( ( info[p].use_category()) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += (unsigned int)((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
	_trans = _use_syllable_machine_indicies[_trans];
_eof_trans:
	cs = _use_syllable_machine_trans_targs[_trans];

	if ( _use_syllable_machine_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _use_syllable_machine_actions + _use_syllable_machine_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 2:
#line 1 "NONE"
	{te = p+1;}
	break;
	case 3:
#line 127 "hb-ot-shape-complex-use-machine.rl"
	{te = p+1;{ found_syllable (independent_cluster); }}
	break;
	case 4:
#line 129 "hb-ot-shape-complex-use-machine.rl"
	{te = p+1;{ found_syllable (standard_cluster); }}
	break;
	case 5:
#line 133 "hb-ot-shape-complex-use-machine.rl"
	{te = p+1;{ found_syllable (broken_cluster); }}
	break;
	case 6:
#line 134 "hb-ot-shape-complex-use-machine.rl"
	{te = p+1;{ found_syllable (non_cluster); }}
	break;
	case 7:
#line 127 "hb-ot-shape-complex-use-machine.rl"
	{te = p;p--;{ found_syllable (independent_cluster); }}
	break;
	case 8:
#line 128 "hb-ot-shape-complex-use-machine.rl"
	{te = p;p--;{ found_syllable (virama_terminated_cluster); }}
	break;
	case 9:
#line 129 "hb-ot-shape-complex-use-machine.rl"
	{te = p;p--;{ found_syllable (standard_cluster); }}
	break;
	case 10:
#line 130 "hb-ot-shape-complex-use-machine.rl"
	{te = p;p--;{ found_syllable (number_joiner_terminated_cluster); }}
	break;
	case 11:
#line 131 "hb-ot-shape-complex-use-machine.rl"
	{te = p;p--;{ found_syllable (numeral_cluster); }}
	break;
	case 12:
#line 132 "hb-ot-shape-complex-use-machine.rl"
	{te = p;p--;{ found_syllable (symbol_cluster); }}
	break;
	case 13:
#line 133 "hb-ot-shape-complex-use-machine.rl"
	{te = p;p--;{ found_syllable (broken_cluster); }}
	break;
	case 14:
#line 134 "hb-ot-shape-complex-use-machine.rl"
	{te = p;p--;{ found_syllable (non_cluster); }}
	break;
	case 15:
#line 133 "hb-ot-shape-complex-use-machine.rl"
	{{p = ((te))-1;}{ found_syllable (broken_cluster); }}
	break;
#line 460 "hb-ot-shape-complex-use-machine.hh"
		}
	}

_again:
	_acts = _use_syllable_machine_actions + _use_syllable_machine_to_state_actions[cs];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 ) {
		switch ( *_acts++ ) {
	case 0:
#line 1 "NONE"
	{ts = 0;}
	break;
#line 473 "hb-ot-shape-complex-use-machine.hh"
		}
	}

	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	if ( p == eof )
	{
	if ( _use_syllable_machine_eof_trans[cs] > 0 ) {
		_trans = _use_syllable_machine_eof_trans[cs] - 1;
		goto _eof_trans;
	}
	}

	}

#line 168 "hb-ot-shape-complex-use-machine.rl"

}

#undef found_syllable

#endif /* HB_OT_SHAPE_COMPLEX_USE_MACHINE_HH */
