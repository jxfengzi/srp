/**
 *
 * Copyright (C) 2007-2012 coding.tom
 *
 * @author jxfengzi@gmail.com
 * @date   2013-5-25
 *
 * @file   TinyQueue.h
 *
 * @version 2013.8.6

 * @remark
 *      set tabstop=4
 *      set shiftwidth=4
 *      set expandtab
 */

#ifndef __TINY_QUEUE_H__
#define __TINY_QUEUE_H__

#include "tiny_base.h"
#include "TinyContainerListener.h"
#include "TinyList.h"

TINY_BEGIN_DECLS


typedef TinyList TinyQueue;

TinyQueue * TinyQueue_New(void);
TinyRet TinyQueue_Construct(TinyQueue *thiz);
TinyRet TinyQueue_Dispose(TinyQueue *thiz);
void TinyQueue_Delete(TinyQueue *thiz);

void TinyQueue_SetDeleteListener(TinyList * thiz, TinyContainerItemDeleteListener listener, void *ctx);

void * TinyQueue_Head(TinyQueue *thiz);
void TinyQueue_Push(TinyQueue *thiz, void *data);
void TinyQueue_Pop(TinyQueue *thiz);
uint32_t TinyQueue_GetSize(TinyQueue *thiz);


TINY_END_DECLS

#endif /* __TINY_QUEUE_H__ */
