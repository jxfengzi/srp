/**
 *
 * Copyright (C) 2007-2012 jxfengzi
 *
 * @author jxfengzi@gmail.com
 * @date   2010-5-25
 *
 * @file   TinyList.h
 *
 * @version 2010.5.25

 * @remark
 *      set tabstop=4
 *      set shiftwidth=4
 *      set expandtab
 */

#ifndef __TINY_LIST_H__
#define __TINY_LIST_H__

#include "tiny_base.h"
#include "TinyContainerListener.h"

TINY_BEGIN_DECLS

struct _TinyNode;
typedef struct _TinyNode TinyNode;

typedef struct _TinyList
{
    void                                * additionalData;
    uint32_t                              size;
    TinyNode                            * first;
    TinyContainerItemDeleteListener       data_delete_listener;
    void                                * data_delete_listener_ctx;
} TinyList;

TinyList * TinyList_New(void);
TinyRet TinyList_Construct(TinyList *thiz);
TinyRet TinyList_Dispose(TinyList *thiz);
void TinyList_Delete(TinyList *thiz);
TinyRet TinyList_AddRef(TinyList *thiz);
TinyRet TinyList_Release(TinyList *thiz);
TinyList * TinyList_Clone(TinyList *other);

int TinyList_Foreach(TinyList * thiz, TinyContainerItemVisit visit, void * ctx);
void TinyList_SetDeleteListener(TinyList * thiz, TinyContainerItemDeleteListener listener, void *ctx);

void * TinyList_GetHead(TinyList * thiz);
void * TinyList_GetTail(TinyList * thiz);
TinyRet TinyList_AddHead(TinyList * thiz, void *data);
TinyRet TinyList_AddTail(TinyList * thiz, void *data);
TinyRet TinyList_RemoveAll(TinyList * thiz);
TinyRet TinyList_RemoveHead(TinyList * thiz);
TinyRet TinyList_RemoveTail(TinyList * thiz);

int TinyList_GetHeadPosition(TinyList * thiz);
int TinyList_GetTailPosition(TinyList * thiz);
void * TinyList_GetNext(TinyList * thiz, int pos);
void * TinyList_GetPrev(TinyList * thiz, int pos);
void * TinyList_GetAt(TinyList * thiz, int pos);
TinyRet TinyList_RemoveAt(TinyList * thiz, int pos);
TinyRet TinyList_SetAt(TinyList * thiz, int pos, void * data);

TinyRet TinyList_InsertAfter(TinyList * thiz, int pos, void * data);
TinyRet TinyList_InsertBefore(TinyList * thiz, int pos, void * data);

void * TinyList_FindIndex(TinyList * thiz, int index);

int TinyList_GetCount(TinyList * thiz);
int TinyList_GetSize(TinyList * thiz);
bool TinyList_IsEmpty(TinyList * thiz);


TINY_END_DECLS


#endif /* __TINY_LIST_H__ */
