/*
 * test-mono-handle: tests for MonoHandle and MonoHandleArena
 *
 * Authors:
 *   Aleksey Kliger <aleksey@xamarin.com>
 *
 * Copyright 2015 Xamarin, Inc. (www.xamarin.com)
 * Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */

#include <config.h>
#include <glib.h>
#include <mono/metadata/handle.h>

static void
test2_arena_push_pop ()
{
	HandleStack *h = mono_handle_stack_alloc ();
	mono_handle_stack_free (h);
}



int
main (int argc, const char* argv[])
{
	test2_arena_push_pop ();

	return 0;
}
