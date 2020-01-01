#ifndef PCHFIX
#define PCHFIX

/*
FIXME: can not get clangd to work with cmake precompiled headers directive
    build is fine, but auto completion etc. does not recognize anything
    that is inside stdafx.hxx (#include gets prepended to all sources by cmake)
*/
#include "stdafx.hxx"

#endif