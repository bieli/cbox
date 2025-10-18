#ifndef CBOX_DEBUG_H
#define CBOX_DEBUG_H

#include "cbox.h"
#include <stdio.h>

#define CBOX_DEBUG_INFO(box) \
    printf("[CBox] type: %d, size: %zu, data: %p\n", (box)->type, (box)->size, (box)->data)

#define CBOX_DEBUG_PRINT(box) \
    do { \
        if ((box) && (box)->trait && (box)->trait->print) \
            (box)->trait->print((box)->data); \
        else \
            printf("[CBox] brak funkcji print\n"); \
    } while (0)

#define CBOX_DEBUG_JSON(box) \
    do { \
        char __json_buf[512]; \
        if ((box) && (box)->trait && (box)->trait->serialize_json) { \
            (box)->trait->serialize_json((box)->data, __json_buf, sizeof(__json_buf)); \
            printf("[CBox JSON] %s\n", __json_buf); \
        } else { \
            printf("[CBox JSON] brak funkcji serialize_json\n"); \
        } \
    } while (0)

#define CBOX_DEBUG_ALL(box) \
    do { \
        CBOX_DEBUG_INFO(box); \
        CBOX_DEBUG_PRINT(box); \
        CBOX_DEBUG_JSON(box); \
    } while (0)

#endif // CBOX_DEBUG_H

