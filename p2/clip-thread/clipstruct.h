#ifndef CLIPSTRUCT_H
#define CLIPSTRUCT_H

#include <linux/list.h>

/* Definicion de struct clipstruct */
struct clipstruct {
    unsigned int id;
    unsigned int num_elem;
    char* buffer;
    struct list_head lista;
};
 
#endif /* CLIPSTRUCT_H */

