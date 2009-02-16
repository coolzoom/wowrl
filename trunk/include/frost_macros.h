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

// Returns 'true' if the key is found in the map
#define MAPFIND(key, map) ((map).find(key) != (map).end())

// Returns 'true' if the key is found in the vector/list/deque
#define VECTORFIND(key, vector) (find((vector).begin(), (vector).end(), key) != (vector).end())

// Iterates through an stl container
#define foreach(iter, container) for ((iter) = (container).begin(); (iter) != (container).end(); (iter)++)

// Iterates through an XML::Block sub-blocks by name
#define foreach_named_block(block, name, parent) for ((block) = (parent)->First(name); (block); (block) = (parent)->Next())

// Iterates through an XML::Block sub-blocks
#define foreach_block(block, parent) for ((block) = (parent)->First(); (block); (block) = (parent)->Next())

// Iterates through a Directory's sub directories
#define foreach_subdir(subdir, dir) for ((subdir) = (dir).GetNextSubDirectory(); (subdir); (subdir) = (dir).GetNextSubDirectory())

#endif
