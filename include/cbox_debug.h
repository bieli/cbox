#ifndef CBOX_DEBUG_H
#define CBOX_DEBUG_H

#include "cbox.h"
#include <stdio.h>

#define CBOX_DEBUG_INFO(box) \
    do { \
        printf("[CBox] type: %d, size: %zu, data: %p\n", (box)->type, (box)->size, (box)->data); \
        if ((box)->parent) \
            printf("[CBox] cloned from: %p\n", (box)->parent); \
        if ((box) && (box)->trait && (box)->trait->print) { \
            printf("[CBox Value] "); \
            (box)->trait->print((box)->data); \
        } else { \
            printf("[CBox Value] (no print function available)\n"); \
        } \
    } while (0)


#define CBOX_DEBUG_INFO_VERBOSE(box) \
    do { \
        CBOX_DEBUG_INFO(box); \
        if ((box) && (box)->trait && (box)->trait->serialize_json) { \
            char __json_buf[512]; \
            (box)->trait->serialize_json((box)->data, __json_buf, sizeof(__json_buf)); \
            printf("[CBox JSON] %s\n", __json_buf); \
        } else { \
            printf("[CBox JSON] (no serialize_json function available)\n"); \
        } \
    } while (0)

#define CBOX_DEBUG_PRINT(box) \
    do { \
        if ((box) && (box)->trait && (box)->trait->print) \
            (box)->trait->print((box)->data); \
        else \
            printf("[WARN] [CBox] Function 'print' not exists!\n"); \
    } while (0)

#define CBOX_DEBUG_JSON(box) \
    do { \
        char __json_buf[512]; \
        if ((box) && (box)->trait && (box)->trait->serialize_json) { \
            (box)->trait->serialize_json((box)->data, __json_buf, sizeof(__json_buf)); \
            printf("[CBox JSON] %s\n", __json_buf); \
        } else { \
            printf("[WARN] [CBox JSON] Function 'serialize_json' not exists!\n"); \
        } \
    } while (0)

#define CBOX_DEBUG_ALL(box) \
    do { \
        CBOX_DEBUG_INFO(box); \
        CBOX_DEBUG_PRINT(box); \
        CBOX_DEBUG_JSON(box); \
    } while (0)

#endif // CBOX_DEBUG_H

