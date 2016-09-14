/**
 *
 * Copyright (C) 2007-2012 coding.tom
 *
 * @author jxfengzi@gmail.com
 * @date   2013-5-25
 *
 * @file   TinyMap.c
 *
 * @version 2013.8.6

 * @remark
 *      set tabstop=4
 *      set shiftwidth=4
 *      set expandtab
 */

#include "TinyMap.h"
#include "tiny_memory.h"
#include "tiny_log.h"

/*-----------------------------------------------------------------------------
*
* Private API declare
*
*-----------------------------------------------------------------------------*/
#define TAG     "TinyMap"

static bool item_compare(void *data, void *ctx);
static int TinyList_FindKey(TinyList * thiz, const char *key);
static void on_item_delete_listener(void * data, void *ctx);

typedef struct _Item
{
    #define     MAX_KEY_LEN             256
    char        key[MAX_KEY_LEN];
    void      * value;
} Item;

/*-----------------------------------------------------------------------------
*
* Public API
*
*-----------------------------------------------------------------------------*/

TinyMap * TinyMap_New(void)
{
    TinyMap *thiz = NULL;

    do
    {
        TinyRet ret = TINY_RET_OK;

        thiz = (TinyMap *)tiny_malloc(sizeof(TinyMap));
        if (thiz == NULL)
        {
            break;
        }

        ret = TinyMap_Construct(thiz);
        if (RET_FAILED(ret))
        {
            TinyMap_Delete(thiz);
            thiz = NULL;
            break;
        }

    } while (0);

    return thiz;
}

TinyRet TinyMap_Construct(TinyMap *thiz)
{
    TinyRet ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_ARG_NULL);

    do
    {
        memset(thiz, 0, sizeof(TinyMap));

        ret = TinyList_Construct(&thiz->list);
        if (RET_FAILED(ret))
        {
            break;
        }

        TinyList_SetDeleteListener(&thiz->list, on_item_delete_listener, thiz);
    } while (0);

    return ret;
}

TinyRet TinyMap_Dispose(TinyMap *thiz)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_ARG_NULL);

    TinyList_Dispose(&thiz->list);

    return TINY_RET_OK;
}

void TinyMap_Delete(TinyMap *thiz)
{
    RETURN_IF_FAIL(thiz);

    TinyMap_Dispose(thiz);
    tiny_free(thiz);
}

void TinyMap_SetDeleteListener(TinyMap * thiz, TinyContainerItemDeleteListener listener, void *ctx)
{
    RETURN_IF_FAIL(thiz);
    RETURN_IF_FAIL(listener);

    thiz->data_delete_listener = listener;
    thiz->data_delete_listener_ctx = ctx;
}

int TinyMap_Foreach(TinyMap * thiz, TinyContainerItemVisit visit, void * ctx)
{
    return TinyList_Foreach(&thiz->list, visit, ctx);
}

uint32_t TinyMap_GetSize(TinyMap *thiz)
{
    RETURN_VAL_IF_FAIL(thiz, 0);

    return TinyList_GetSize(&thiz->list);
}

uint32_t TinyMap_GetCount(TinyMap *thiz)
{
    return TinyMap_GetSize(thiz);
}

void * TinyMap_GetValueAt(TinyMap *thiz, uint32_t index)
{
    void *value = NULL;
    Item * item = NULL;
    
    RETURN_VAL_IF_FAIL(thiz, NULL);

    item = (Item *)TinyList_GetAt(&thiz->list, index);
    if (item)
    {
        value = item->value;
    }

    return value;
}

void * TinyMap_GetValue(TinyMap *thiz, const char *key)
{
    int index = 0;

    RETURN_VAL_IF_FAIL(thiz, NULL);
    RETURN_VAL_IF_FAIL(key, NULL);

    index = TinyList_Foreach(&thiz->list, item_compare, (void *)key);
    if (index >= 0)
    {
        Item * item = (Item *)TinyList_GetAt(&thiz->list, index);
        return item->value;
    }

    return NULL;
}

TinyRet TinyMap_Insert(TinyMap *thiz, const char *key, void *value)
{
    int index = 0;

    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_ARG_NULL);
    RETURN_VAL_IF_FAIL(key, TINY_RET_E_ARG_NULL);
    RETURN_VAL_IF_FAIL(value, TINY_RET_E_ARG_NULL);

    index = TinyList_FindKey(&thiz->list, key);
    if (index < 0)
    {
        Item * item = (Item *)tiny_malloc(sizeof(Item));
        if (item == NULL)
        {
            return TINY_RET_E_NEW;
        }

        memset(item, 0, sizeof(Item));
        strncpy(item->key, key, MAX_KEY_LEN);
        item->value = value;

        TinyList_AddTail(&thiz->list, item);
        return TINY_RET_OK;
    }

    return TINY_RET_E_ITEM_EXIST;
}

TinyRet TinyMap_Erase(TinyMap *thiz, const char *key)
{
    int index = 0;

    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_ARG_NULL);
    RETURN_VAL_IF_FAIL(key, TINY_RET_E_ARG_NULL);

    index = TinyList_FindKey(&thiz->list, key);
    if (index < 0)
    {
        return TINY_RET_E_NOT_FOUND;
    }

    TinyList_RemoveAt(&thiz->list, index);
    return TINY_RET_OK;
}

void TinyMap_Clear(TinyMap *thiz)
{
    RETURN_IF_FAIL(thiz);

    TinyList_RemoveAll(&thiz->list);
}

/*-----------------------------------------------------------------------------
*
* Private API
*
*-----------------------------------------------------------------------------*/
static bool item_compare(void *data, void *ctx)
{
    Item * item = (Item *)data;
    const char * key = (const char *)ctx;

    if (strncmp(item->key, key, MAX_KEY_LEN) == 0)
    {
        return false;
    }

    return true;
}

static int TinyList_FindKey(TinyList * thiz, const char *key)
{
    return TinyList_Foreach(thiz, item_compare, (void *)key);
}

static void on_item_delete_listener(void * data, void *ctx)
{
    TinyMap * thiz = (TinyMap *)ctx;
    Item * item = (Item *)data;

    if (thiz->data_delete_listener)
    {
        thiz->data_delete_listener(item->value, thiz->data_delete_listener_ctx);
    }

    tiny_free(item);
}
