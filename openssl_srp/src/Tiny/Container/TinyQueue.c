/**
 *
 * Copyright (C) 2007-2012 coding.tom
 *
 * @author jxfengzi@gmail.com
 * @date   2013-5-25
 *
 * @file   queue.c
 *
 * @version 2013.8.6

 * @remark
 *      set tabstop=4
 *      set shiftwidth=4
 *      set expandtab
 */

#include "TinyQueue.h"

TinyQueue * TinyQueue_New(void)
{
    return TinyList_New();
}

TinyRet TinyQueue_Construct(TinyQueue *thiz)
{
    return TinyList_Construct(thiz);
}

TinyRet TinyQueue_Dispose(TinyQueue *thiz)
{
    return TinyList_Dispose(thiz);
}

void TinyQueue_Delete(TinyQueue *thiz)
{
    TinyList_Delete(thiz);
}

void TinyQueue_SetDeleteListener(TinyList * thiz, TinyContainerItemDeleteListener listener, void *ctx)
{
    TinyList_SetDeleteListener(thiz, listener, ctx);
}

void * TinyQueue_Head(TinyQueue *thiz)
{
    return TinyList_GetHead(thiz);
}

void TinyQueue_Push(TinyQueue *thiz, void *data)
{
    TinyList_AddTail(thiz, data);
}

void TinyQueue_Pop(TinyQueue *thiz)
{
    TinyList_RemoveHead(thiz);
}

uint32_t TinyQueue_GetSize(TinyQueue *thiz)
{
    return TinyList_GetCount(thiz);
}