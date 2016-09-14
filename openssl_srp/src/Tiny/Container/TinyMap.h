/**
 *
 * Copyright (C) 2007-2012 coding.tom
 *
 * @author jxfengzi@gmail.com
 * @date   2013-5-25
 *
 * @file   TinyMap.h
 *
 * @version 2013.8.6

 * @remark
 *      set tabstop=4
 *      set shiftwidth=4
 *      set expandtab
 */

#ifndef __TINY_MAP_H__
#define __TINY_MAP_H__

#include "tiny_base.h"
#include "TinyContainerListener.h"
#include "TinyList.h"

TINY_BEGIN_DECLS


typedef struct _TinyMap
{
    TinyList                            list;
    TinyContainerItemDeleteListener     data_delete_listener;
    void                              * data_delete_listener_ctx;
} TinyMap;

TinyMap * TinyMap_New(void);
TinyRet TinyMap_Construct(TinyMap *thiz);
TinyRet TinyMap_Dispose(TinyMap *thiz);
void TinyMap_Delete(TinyMap *thiz);

int TinyMap_Foreach(TinyMap * thiz, TinyContainerItemVisit visit, void * ctx);
void TinyMap_SetDeleteListener(TinyMap * thiz, TinyContainerItemDeleteListener listener, void *ctx);

uint32_t TinyMap_GetSize(TinyMap *thiz);
uint32_t TinyMap_GetCount(TinyMap *thiz);
void * TinyMap_GetValueAt(TinyMap *thiz, uint32_t index);
void * TinyMap_GetValue(TinyMap *thiz, const char *key);
TinyRet TinyMap_Insert(TinyMap *thiz, const char *key, void *value);
TinyRet TinyMap_Erase(TinyMap *thiz, const char *key);
void TinyMap_Clear(TinyMap *thiz);


TINY_END_DECLS

#endif /* __TINY_MAP_H__ */
