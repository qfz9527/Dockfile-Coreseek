/* Test of isnand() substitute.
   Copyright (C) 2007-2008 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* Written by Bruno Haible <bruno@clisp.org>, 2007.  */

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "nan.h"

#define ASSERT(expr) \
  do									     \
    {									     \
      if (!(expr))							     \
        {								     \
          fprintf (stderr, "%s:%d: assertion failed\n", __FILE__, __LINE__); \
          fflush (stderr);						     \
          abort ();							     \
        }								     \
    }									     \
  while (0)

/* HP cc on HP-UX 10.20 has a bug with the constant expression -0.0.
   So we use -zero instead.  */
double zero = 0.0;

int
main ()
{
  /* Finite values.  */
  ASSERT (!isnand (3.141));
  ASSERT (!isnand (3.141e30));
  ASSERT (!isnand (3.141e-30));
  ASSERT (!isnand (-2.718));
  ASSERT (!isnand (-2.718e30));
  ASSERT (!isnand (-2.718e-30));
  ASSERT (!isnand (0.0));
  ASSERT (!isnand (-zero));
  /* Infinite values.  */
  ASSERT (!isnand (1.0 / 0.0));
  ASSERT (!isnand (-1.0 / 0.0));
  /* Quiet NaN.  */
  ASSERT (isnand (NaNd ()));
#if defined DBL_EXPBIT0_WORD && defined DBL_EXPBIT0_BIT
  /* Signalling NaN.  */
  {
    #define NWORDS \
      ((sizeof (double) + sizeof (unsigned int) - 1) / sizeof (unsigned int))
    typedef union { double value; unsigned int word[NWORDS]; } memory_double;
    memory_double m;
    m.value = NaNd ();
# if DBL_EXPBIT0_BIT > 0
    m.word[DBL_EXPBIT0_WORD] ^= (unsigned int) 1 << (DBL_EXPBIT0_BIT - 1);
# else
    m.word[DBL_EXPBIT0_WORD + (DBL_EXPBIT0_WORD < NWORDS / 2 ? 1 : - 1)]
      ^= (unsigned int) 1 << (sizeof (unsigned int) * CHAR_BIT - 1);
# endif
    m.word[DBL_EXPBIT0_WORD + (DBL_EXPBIT0_WORD < NWORDS / 2 ? 1 : - 1)]
      |= (unsigned int) 1 << DBL_EXPBIT0_BIT;
    ASSERT (isnand (m.value));
  }
#endif
  return 0;
}
