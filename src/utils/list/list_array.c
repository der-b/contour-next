/* Copyright 2015 Bernd Lehmann (der-b@der-b.com)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <stdlib.h>
#include <string.h>
#include "utils/list/_list.h"
#include "utils/list/list_array.h"

/*
 * _list_array_realloc()
 */
inline LRet _list_array_realloc(List *list) {
  struct list_array *larray = (struct list_array *)list->data;
  void *tmp;

  // ensure minimal size
  if( _LIST_ARRAY_MINSIZE > larray->max_count ) {

    tmp = realloc(larray->data, _LIST_ARRAY_MINSIZE * L_SIZE(list));
    if( !tmp ) {
      return LIST_ERR;
    }
    larray->data = tmp;
    larray->max_count = _LIST_ARRAY_MINSIZE;
    return LIST_OK;

  // increase size
  } else if( larray->count >= larray->max_count ) {

    tmp = realloc(larray->data, larray->max_count * 2 * L_SIZE(list));
    if( !tmp ) {
      return LIST_ERR;
    }
    larray->data = tmp;
    larray->max_count *= 2;
    return LIST_OK;

  // decrese size but not smaler then _LIST_ARRAY_MINSIZE
  } else if( larray->count < larray->max_count / 4 ) {

    if( _LIST_ARRAY_MINSIZE == larray->max_count ) {

      return LIST_OK;

    } else if( larray->max_count / 2 < _LIST_ARRAY_MINSIZE ) {

      tmp = realloc(larray->data, _LIST_ARRAY_MINSIZE * L_SIZE(list));

    } else {
      
      tmp = realloc(larray->data, larray->max_count / 2 * L_SIZE(list));

    }

    if( !tmp ) {
      return LIST_ERR;
    }

    larray->data = tmp;
    larray->max_count = larray->max_count / 2;
    return LIST_OK;

  }

  return LIST_OK;
}


/*
 * list_array_free()
 */
LRet list_array_free(List *list) {
  struct list_array *larray;

#if _LIST_ASUME_NOT_USING_MACROS
  if( !list ) {
    return LIST_ERR;
  }

  if( LIST_TYPE_ARRAY != list->type ) {
    return LIST_ERR;
  }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE 
  if( !list->data ) {
    return LIST_ERR;
  }
#endif

  larray = (struct list_array *)list->data;
  
  free(larray->data);
  free(larray);
  free(list);

  return LIST_OK;
}


/*
 * list_array_length
 */
LIndex list_array_length(List *list) {
  struct list_array *larray;

#if _LIST_ASUME_NOT_USING_MACROS
  if( !list ) {
    return LIST_ERR;
  }

  if( LIST_TYPE_ARRAY != list->type ) {
    return LIST_ERR;
  }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE 
  if( !list->data ) {
    return LIST_ERR;
  }
#endif

  larray = (struct list_array *)list->data;

  return larray->count;
}


/*
 * list_array_pushback()
 */
void *list_array_pushback(List *list, void *data) {
  struct list_array *larray;

#if _LIST_ASUME_NOT_USING_MACROS
  if( !list ) {
    return NULL;
  }

  if( LIST_TYPE_ARRAY != list->type ) {
    return NULL;
  }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE 
  if( !list->data ) {
    return NULL;
  }
#endif

  larray = (struct list_array *)list->data;

  if( larray->count >= larray->max_count ) {
    if( _list_array_realloc(list) ) {
      return NULL;
    }
  }
  
  if( data ) {
    memcpy(((uint8_t *)larray->data) + larray->count * L_SIZE(list), data, L_SIZE(list));
  } else {
    memset(((uint8_t *)larray->data) + larray->count * L_SIZE(list), 0, L_SIZE(list));
  }

  ++larray->count;
  
  return ((uint8_t *)larray->data) + (larray->count - 1) * L_SIZE(list);
}


/*
 * list_array_popback()
 */
LRet list_array_popback(List *list, void *data) {
  struct list_array *larray;

#if _LIST_ASUME_NOT_USING_MACROS
  if( !list ) {
    return LIST_ERR;
  }

  if( LIST_TYPE_ARRAY != list->type ) {
    return LIST_ERR;
  }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE 
  if( !list->data ) {
    return LIST_ERR;
  }
#endif

  larray = (struct list_array *)list->data;
  
  if( 0 >= larray->count ) {
    return LIST_ERR;
  }

  if( data ) {
    memcpy(data, (((uint8_t *)larray->data) + (larray->count-1) * L_SIZE(list)), L_SIZE(list));
  }

  --larray->count;

  _list_array_realloc(list);

  return LIST_OK;
}


/*
 * list_array_last()
 */
void *list_array_last(List *list) {
  struct list_array *larray;

#if _LIST_ASUME_NOT_USING_MACROS
  if( !list ) {
    return NULL;
  }

  if( LIST_TYPE_ARRAY != list->type ) {
    return NULL;
  }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE 
  if( !list->data ) {
    return NULL;
  }
#endif

  larray = (struct list_array *)list->data;

  if( 0 >= larray->count ) {
    return NULL;
  }

  return (((uint8_t *)larray->data) + (larray->count - 1) * L_SIZE(list));
}


/*
 * list_array_get()
 */
void *list_array_get(List *list, LIndex pos) {
  struct list_array *larray;

#if _LIST_ASUME_NOT_USING_MACROS
  if( !list ) {
    return NULL;
  }

  if( LIST_TYPE_ARRAY != list->type ) {
    return NULL;
  }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE 
  if( !list->data ) {
    return NULL;
  }
#endif

  larray = (struct list_array *)list->data;

  if( 0 >= larray->count || larray->count <= pos) {
    return NULL;
  }

  return (((uint8_t *)larray->data) + pos * L_SIZE(list));
}


/*
 * list_array_insert()
 */
void *list_array_insert(List *list, LIndex pos, void *data) {
  struct list_array *larray;
  uint8_t *dptr, *stop;

#if _LIST_ASUME_NOT_USING_MACROS
  if( !list ) {
    return NULL;
  }

  if( LIST_TYPE_ARRAY != list->type ) {
    return NULL;
  }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE 
  if( !list->data ) {
    return NULL;
  }
#endif
  
  larray = (struct list_array *)list->data;

  if( pos > larray->count ) {
    return NULL;
  }

  if( larray->count >= larray->max_count ) {
    if( _list_array_realloc(list) ) {
      return NULL;
    }
  }

  stop = ((uint8_t *)larray->data) + pos * L_SIZE(list);
  for( dptr = ((uint8_t *)larray->data) + larray->count * L_SIZE(list); dptr > stop; dptr -= L_SIZE(list) ) {
    memcpy(dptr, dptr - L_SIZE(list), L_SIZE(list));
  }

  if( data ) {
    memcpy(dptr, data, L_SIZE(list));
  } else {
    memset(dptr, 0, L_SIZE(list));
  }

  ++larray->count;

  return dptr;
}


/*
 * list_array_remove()
 */
LRet list_array_remove(List *list, LIndex pos, void *data) {
  struct list_array *larray;
  uint8_t *dptr, *end;

#if _LIST_ASUME_NOT_USING_MACROS
  if( !list ) {
    return LIST_ERR;
  }

  if( LIST_TYPE_ARRAY != list->type ) {
    return LIST_ERR;
  }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE 
  if( !list->data ) {
    return LIST_ERR;
  }
#endif
  
  larray = (struct list_array *)list->data;
  
  if( pos >= larray->count || !larray->count ) {
    return LIST_ERR;
  }

  if( data ) {
    memcpy(data, (((uint8_t *)larray->data) + L_SIZE(list) * pos), L_SIZE(list));
  }

  end = ((uint8_t *)larray->data) + L_SIZE(list) * (larray->count - 1);
  for( dptr = ((uint8_t *)larray->data) + L_SIZE(list) * pos; dptr < end; dptr += L_SIZE(list) ) {
    memcpy(dptr, dptr + L_SIZE(list), L_SIZE(list));
  }

  --larray->count;

  _list_array_realloc(list);
  
  return LIST_OK;
}


/*
 * list_array_remove()
 */
LRet list_array_removelast(List *list, void *del, void *data) {
  struct list_array *larray;
  uint8_t *dptr, *end, *pos;

#if _LIST_ASUME_NOT_USING_MACROS
  if( !list ) {
    return LIST_ERR;
  }

  if( LIST_TYPE_ARRAY != list->type ) {
    return LIST_ERR;
  }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE 
  if( !list->data ) {
    return LIST_ERR;
  }
#endif
  
  if( !list->equal ) {
    return LIST_ERR;
  }
  
  larray = (struct list_array *)list->data;
  
  end = ((uint8_t *)larray->data) + L_SIZE(list) * (larray->count - 1);
  pos = end;
  dptr = NULL;

  while( ((uint8_t *)larray->data) < pos ) {
    pos -= L_SIZE(list);
    if( list->equal(pos, del) ) {
      dptr = pos;
      break;
    }
  }

  if( !dptr ) {
    return LIST_ELEMENT_NOT_FOUND;
  }

  if( data ) {
    memcpy(data, dptr, L_SIZE(list));
  }

  for( ; dptr < end; dptr += L_SIZE(list) ) {
    memcpy(dptr, dptr + L_SIZE(list), L_SIZE(list));
  }

  --larray->count;

  _list_array_realloc(list);
  
  return LIST_OK;
}


/*
 * list_array_first()
 */
void *list_array_first(List *list) {
  struct list_array *larray;

#if _LIST_ASUME_NOT_USING_MACROS
  if( !list ) {
    return NULL;
  }

  if( LIST_TYPE_ARRAY != list->type ) {
    return NULL;
  }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE 
  if( !list->data ) {
    return NULL;
  }
#endif

  larray = (struct list_array *)list->data;

  if( 0 >= larray->count ) {
    return NULL;
  }

  return larray->data;
}


/*
 * list_array_pushfront()
 */
void *list_array_pushfront(List *list, void *data) {
  return list_array_insert(list, 0, data);
}


/*
 * list_array_popfront()
 */
LRet list_array_popfront(List *list, void *data) {
  return list_array_remove(list, 0, data);
}


/*
 * list_array_iter_free()
 */
LRet list_array_iter_free(LIter *iter) {

  free(iter->data);

  return LIST_OK;
}


/*
 * list_array_iter_next()
 */
void *list_array_iter_next(LIter *iter) {
  struct list_array_iter *ait;
#if _LIST_ASUME_NOT_USING_MACROS
  if( !iter ) {
    return NULL;
  }

  if( LIST_TYPE_ARRAY != iter->type ) {
    return NULL
  }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE 
  if( !iter->data ) {
    return NULL;
  }
#endif

  ait = (struct list_array_iter *)iter->data;

  return list_array_get(ait->list, ait->pos++);
}


/*
 * list_array_iter()
 */
void *list_array_iter(List *list, LIter *iter) {
  struct list_array_iter *it;

  if( !iter ) {
    return NULL;
  }

#if _LIST_ASUME_NOT_USING_MACROS
  if( !list ) {
    return NULL;
  }

  if( LIST_TYPE_ARRAY != list->type ) {
    return NULL;
  }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE 
  if( !list->data ) {
    return NULL;
  }
#endif

  memset(iter, 0, sizeof(LIter));
  
  iter->data = malloc(sizeof(struct list_array_iter));
  if( !iter->data ) {
    return NULL;
  }
  it = (struct list_array_iter *)iter->data;
  it->pos = 1;
  it->list = list;

  iter->type = LIST_TYPE_ARRAY;
  iter->next = list_array_iter_next;
  iter->free = list_array_iter_free;

  return list_array_get(list, 0);
}


/*
 * list_array_new()
 */
List *list_array_new(LOpts *opts) {
  List *list;
  struct list_array *larray;

  if( !opts ) {
    return NULL;
  }

  if( 0 >= opts->size ) {
    return NULL;
  }

  list = malloc(sizeof(List));
  if( !list ) {
    return NULL;
  }

  larray = malloc(sizeof(struct list_array));
  if( !larray ) {
    free(list);
    return NULL;
  }

  memset(list, 0, sizeof(List));
  memset(larray, 0, sizeof(struct list_array));

  memcpy(&list->opts, opts, sizeof(LOpts));

  list->type = LIST_TYPE_ARRAY;
  list->data = larray;
   
  list->free       = list_array_free;
  list->length     = list_array_length;
  list->insert     = list_array_insert;
  list->remove     = list_array_remove;
  list->get        = list_array_get;
  list->last       = list_array_last;
  list->pushback   = list_array_pushback;
  list->popback    = list_array_popback;
  list->first      = list_array_first;
  list->pushfront  = list_array_pushfront;
  list->popfront   = list_array_popfront;
  list->iter       = list_array_iter;
  list->removelast = list_array_removelast;

  return list;
}

