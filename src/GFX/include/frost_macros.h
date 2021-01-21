/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*             Macros header              */
/*                                        */
/*                                        */

/* Warning : This is a precompiled header.
*            Modifying it will trigger a
*            full rebuild !
*/

#ifndef FROST_MACROS_H
#define FROST_MACROS_H

// Allows compilation assertions (must be placed inside a dummy function)
#define COMPILE_TIME_ASSERT(condition) switch(0) {case 0: case (condition): ;}

#endif
