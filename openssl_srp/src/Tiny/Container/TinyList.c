/**
 *
 * Copyright (C) 2007-2012 IGRSLab
 *
 * @author ouyangcf@igrslab.com
 * @date   2010-5-25
 *
 * @file   TinyList.c
 *
 * @version 2010.5.25

 * @remark
 *      set tabstop=4
 *      set shiftwidth=4
 *      set expandtab
 */

#include "TinyList.h"
#include "tiny_memory.h"
#include "tiny_log.h"

/*-----------------------------------------------------------------------------
 *
 * Private Declare
 *
 *-----------------------------------------------------------------------------*/
#define TAG     "TinyList"

struct _TinyNode
{
    TinyNode * prev;
    TinyNode * next;
    void    * data;
};

static TinyNode * TinyNode_New(TinyList *thiz);
static void TinyNode_Delete(TinyList *thiz, TinyNode *node);

/*-----------------------------------------------------------------------------
 *
 * Public Implements
 *
 *-----------------------------------------------------------------------------*/
TinyList * TinyList_New(void)
{
    TinyList *thiz = NULL;

    do
    {
        TinyRet ret = TINY_RET_OK;

        thiz = (TinyList *)tiny_malloc(sizeof(TinyList));
        if (thiz == NULL)
        {
            break;
        }

        ret = TinyList_Construct(thiz);
        if (RET_FAILED(ret))
        {
            TinyList_Delete(thiz);
            thiz = NULL;
            break;
        }

    } while (false);

    return thiz;
}

TinyRet TinyList_Construct(TinyList *thiz)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_ARG_NULL);

    memset(thiz, 0, sizeof(TinyList));

    return TINY_RET_OK;
}

TinyRet TinyList_Dispose(TinyList *thiz)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_ARG_NULL);

    TinyList_RemoveAll(thiz);

    return TINY_RET_OK;
}

void TinyList_Delete(TinyList *thiz)
{
    RETURN_IF_FAIL(thiz);

    TinyList_Dispose(thiz);
    tiny_free(thiz);
}

TinyRet TinyList_AddRef(TinyList *thiz)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_ARG_NULL);

    LOG_W(TAG, "TinyList_AddRef => Not Implemented");
    return TINY_RET_E_NOT_IMPLEMENTED;
}

TinyRet TinyList_Release(TinyList *thiz)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_ARG_NULL);

    LOG_W(TAG, "TinyList_Release => Not Implemented");
    return TINY_RET_E_NOT_IMPLEMENTED;
}

TinyList * TinyList_Clone(TinyList *other)
{
    RETURN_VAL_IF_FAIL(other, NULL);

    LOG_W(TAG, "TinyList_Clone => Not Implemented");
    return NULL;
}

int TinyList_Foreach(TinyList * thiz, TinyContainerItemVisit visit, void * ctx)
{
    int i = 0;
    TinyNode * iter = NULL;

    RETURN_VAL_IF_FAIL(thiz, -1);

    iter = thiz->first;
    while (1)
    {
        if (iter == NULL)
        {
            i = -1;
            break;
        }

        if (!visit(iter->data, ctx))
        {
            break;
        }

        iter = iter->next;
        i++;
    }

    return i;
}

void TinyList_SetDeleteListener(TinyList * thiz, TinyContainerItemDeleteListener listener, void *ctx)
{
    RETURN_IF_FAIL(thiz);
    RETURN_IF_FAIL(listener);

    thiz->data_delete_listener = listener;
    thiz->data_delete_listener_ctx = ctx;
}

void * TinyList_GetHead(TinyList * thiz)
{
    void *data = NULL;

    RETURN_VAL_IF_FAIL(thiz, NULL);

    do
    {
        TinyNode * head = thiz->first;
        if (head != NULL)
        {
            data = head->data;
            break;
        }
    }
    while (false);

    return data;
}

void * TinyList_GetTail(TinyList * thiz)
{
    void *data = NULL;

    RETURN_VAL_IF_FAIL(thiz, NULL);

    do
    {
        TinyNode* tail = thiz->first;
        if (tail == NULL)
        {
            break;
        }

        while (tail->next)
        {
            tail = tail->next;
        }

        data = tail->data;
    }
    while (false);

    return data;
}

TinyRet TinyList_AddHead(TinyList * thiz, void *data)
{
    TinyRet ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_ARG_NULL);

    do
    {
        TinyNode *node = NULL;
        TinyNode *head = NULL;

        node = TinyNode_New(thiz);
        if (node == NULL)
        {
            ret = TINY_RET_E_NEW;
            break;
        }

        node->data = data;

        head = thiz->first;
        if (head)
        {
            node->next = head;
            head->prev = node;
        }

        thiz->first = node;
        node->prev = NULL;

        ret = TINY_RET_OK;
        break;
    }
    while (false);

    return ret;
}

TinyRet TinyList_AddTail(TinyList * thiz, void *data)
{
    TinyRet ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_ARG_NULL);
    RETURN_VAL_IF_FAIL(data, TINY_RET_E_ARG_NULL);

    do
    {
        TinyNode *node = NULL;
        TinyNode *tail = NULL;

        node = TinyNode_New(thiz);
        if (node == NULL)
        {
            ret = TINY_RET_E_NEW;
            break;
        }

        node->data = data;

        /* empty list */
        if (thiz->first == NULL)
        {
            thiz->first = node;
            ret = TINY_RET_OK;
            break;
        }

        tail = thiz->first;
        while (tail->next)
        {
            tail = tail->next;
        }

        tail->next = node;
        node->prev = tail;
        ret = TINY_RET_OK;
    }
    while (false);

    return ret;
}

TinyRet TinyList_RemoveAll(TinyList * thiz)
{
    TinyRet ret = TINY_RET_OK;
    TinyNode * iter = NULL;
    TinyNode * remove = NULL;

    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_ARG_NULL);

    do
    {
        if (thiz->first == NULL)
        {
            ret = TINY_RET_OK;
            break;
        }

        iter = thiz->first;
        thiz->first = NULL;

        while (iter)
        {
            remove = iter;
            iter = iter->next;
            TinyNode_Delete(thiz, remove);
        }

        ret = TINY_RET_OK;
    }
    while (false);

    return ret;
}

TinyRet TinyList_RemoveHead(TinyList * thiz)
{
    TinyRet ret = TINY_RET_OK;
    TinyNode * head = NULL;

    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_ARG_NULL);

    do
    {
        if (thiz->first == NULL)
        {
            ret = TINY_RET_OK;
            break;
        }

        head = thiz->first;
        if (head)
        {
            thiz->first = head->next;
            if (head->next)
            {
                head->next->prev = NULL;
            }

            TinyNode_Delete(thiz, head);
        }

        ret = TINY_RET_OK;
    }
    while (false);

    return ret;
}

TinyRet TinyList_RemoveTail(TinyList * thiz)
{
    TinyRet ret = TINY_RET_OK;
    TinyNode * tail = NULL;

    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_ARG_NULL);

    do
    {

        if (thiz->first == NULL)
        {
            ret = TINY_RET_OK;
            break;
        }

        tail = thiz->first;
        while (tail->next)
        {
            tail = tail->next;
        }

        if (tail->prev != NULL)
        {
            tail->prev->next = NULL;
        }
        else
        {
            thiz->first = NULL;
        }

        TinyNode_Delete(thiz, tail);

        ret = TINY_RET_OK;
    }
    while (false);

    return ret;
}

int TinyList_GetHeadPosition(TinyList * thiz)
{
    RETURN_VAL_IF_FAIL(thiz, -1);

    return 0;
}

int TinyList_GetTailPosition(TinyList * thiz)
{
    int i = 0;

    RETURN_VAL_IF_FAIL(thiz, -1);

    do
    {
        TinyNode * tail = thiz->first;
        if (tail == NULL)
        {
            i = -1;
            break;
        }

        while (tail->next)
        {
            tail = tail->next;
            i++;
        }
    }
    while (false);

    return i;
}

void * TinyList_GetNext(TinyList * thiz, int pos)
{
    return TinyList_GetAt(thiz, pos + 1);
}

void * TinyList_GetPrev(TinyList * thiz, int pos)
{
    return TinyList_GetAt(thiz, pos - 1);
}

void * TinyList_GetAt(TinyList * thiz, int pos)
{
    void *data = NULL;

    RETURN_VAL_IF_FAIL(thiz, NULL);

    do
    {
        int i = 0;
        TinyNode * node = thiz->first;

        while (node)
        {
            if (i == pos)
            {
                data = node->data;
            }

            node = node->next;
            i++;
        }
    }
    while (false);

    return data;
}

TinyRet TinyList_RemoveAt(TinyList * thiz, int pos)
{
    TinyRet ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_ARG_NULL);

    do
    {
        int i = 0;
        TinyNode * node = thiz->first;

        while (node)
        {
            if (i == pos)
            {
                /* remove first node */
                if (node->prev == NULL)
                {
                    thiz->first = node->next;
                    if (node->next)
                    {
                        node->next->prev = NULL;
                    }
                }
                else
                {
                    node->prev->next = node->next;
                    if (node->next)
                    {
                        node->next->prev = node->prev;
                    }
                }

                TinyNode_Delete(thiz, node);
                ret = TINY_RET_OK;
                break;
            }

            node = node->next;
            i++;
        }

        ret = TINY_RET_E_POSITION_INVALID;
    }
    while (false);

    return ret;
}

TinyRet TinyList_SetAt(TinyList * thiz, int pos, void * data)
{
    TinyRet ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_ARG_NULL);

    do
    {
        int i = 0;
        TinyNode * node = thiz->first;

        if (node == NULL)
        {
            ret = TINY_RET_E_POSITION_INVALID;
            break;
        }

        while (node)
        {
            if (i == pos)
            {
                /* delete old data */
                thiz->data_delete_listener(node->data, thiz->data_delete_listener_ctx);

                /* update data */
                node->data = data;
                ret = TINY_RET_OK;
                break;
            }

            node = node->next;
            i++;
        }

        ret = TINY_RET_E_POSITION_INVALID;
    }
    while (false);

    return ret;
}

TinyRet TinyList_InsertAfter(TinyList * thiz, int pos, void * data)
{
    int i = 0;
    TinyNode * node = NULL;
    TinyNode * new_node = NULL;

    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_ARG_NULL);

    if (thiz->first == NULL)
    {
        return TINY_RET_E_POSITION_INVALID;
    }

    new_node = TinyNode_New(thiz);
    if (new_node == NULL)
    {
        return TINY_RET_E_NEW;
    }

    new_node->data = data;

    node = thiz->first;
    while (node)
    {
        if (i == pos)
        {
            new_node->prev = node;
            new_node->next = node->next;
            if (node->next)
            {
                node->next->prev = new_node;
            }
            node->next = new_node;

            return TINY_RET_OK;
        }

        node = node->next;
        i++;
    }

    return TINY_RET_E_POSITION_INVALID;
}

TinyRet TinyList_InsertBefore(TinyList * thiz, int pos, void * data)
{
    if (pos == 0)
    {
        return TinyList_AddHead(thiz, data);
    }

    return TinyList_InsertAfter(thiz, pos - 1, data);
}

void * TinyList_FindIndex(TinyList * thiz, int index)
{
    return TinyList_GetAt(thiz, index);
}

int TinyList_GetCount(TinyList * thiz)
{
    RETURN_VAL_IF_FAIL(thiz, -1);

    return thiz->size;
}

int TinyList_GetSize(TinyList * thiz)
{
    return TinyList_GetCount(thiz);
}

bool TinyList_IsEmpty(TinyList * thiz)
{
    RETURN_VAL_IF_FAIL(thiz, false);

    return (thiz->first == NULL);
}

/*-----------------------------------------------------------------------------
 *
 * Private Implements
 *
 *-----------------------------------------------------------------------------*/
static TinyNode * TinyNode_New(TinyList *thiz)
{
    TinyNode *node = (TinyNode *) tiny_malloc(sizeof(TinyNode));
    if (node != NULL)
    {
        node->prev = NULL;
        node->next = NULL;
        node->data = NULL;

        thiz->size++;
    }

    return node;
}

static void TinyNode_Delete(TinyList *thiz, TinyNode *node)
{
    if (thiz->data_delete_listener != NULL)
    {
        thiz->data_delete_listener(node->data, thiz->data_delete_listener_ctx);
    }

    tiny_free(node);
    thiz->size--;
}
