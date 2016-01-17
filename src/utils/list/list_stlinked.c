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
#include <utils/list.h>
#include <utils/list/list_stlinked.h>
#include <string.h>

/*
 * list_stlinked_last()
 */
void *list_stlinked_last(List *list) {
  struct list_stlinked *lstl;

#if _LIST_ASUME_NOT_USING_MACROS
  if( !list ) {
    return NULL;
  }

  if( LIST_TYPE_STLINKED != list->type ) {
    return NULL;
  }
#endif 

#if _LIST_INVALID_STRUCT_POSSIBLE
  if( !list->data ) {
    return NULL;
  }
#endif

  lstl = (struct list_stlinked *)list->data;

  if( !lstl->last ) {
    return NULL;
  }

  return lstl->last->data;
}


/*
 * list_stlinked_first()
 */
void *list_stlinked_first(List *list) {
  struct list_stlinked *lstl;

#if _LIST_ASUME_NOT_USING_MACROS
  if( !list ) {
    return NULL;
  }

  if( LIST_TYPE_STLINKED != list->type ) {
    return NULL;
  }
#endif 

#if _LIST_INVALID_STRUCT_POSSIBLE
  if( !list->data ) {
    return NULL;
  }
#endif

  lstl = (struct list_stlinked *)list->data;

  if( !lstl->first ) {
    return NULL;
  }

  return lstl->first->data;
}


/*
 * list_stlinked_pushfront()
 */
void *list_stlinked_pushfront(List *list, void *data) {
  struct list_stlinked *lstl;
  struct list_stlinked_item *item;

#if _LIST_ASUME_NOT_USING_MACROS
  if( !list ) {
    return NULL;
  }

  if( LIST_TYPE_STLINKED != list->type ) {
    return NULL;
  }
#endif 

#if _LIST_INVALID_STRUCT_POSSIBLE
  if( !list->data ) {
    return NULL;
  }
#endif

  lstl = (struct list_stlinked *)list->data;

  item = malloc(sizeof(struct list_stlinked_item));
  memset(item, 0, sizeof(struct list_stlinked_item));

  item->data = malloc(sizeof(L_SIZE(list)));
  if( !item->data ) {
    free(item);
    return NULL;
  }

  if( data ) {
    memcpy(item->data, data, L_SIZE(list));
  } else {
    memset(item->data, 0, L_SIZE(list));
  }

  if( NULL == lstl->last ) {
#if _LIST_INVALID_STRUCT_POSSIBLE
    if( lstl->first ) {
      free(item->data);
      free(item);
      return NULL;
    }
#endif

    lstl->first = item;
    lstl->last = item;
  } else {
    lstl->first->prev = item;
    lstl->first = item;
  }

  ++lstl->count;

  return item->data;
}


/*
 * list_stlinked_pushback()
 */
void *list_stlinked_pushback(List *list, void *data) {
  struct list_stlinked *lstl;
  struct list_stlinked_item *item;

#if _LIST_ASUME_NOT_USING_MACROS
  if( !list ) {
    return NULL;
  }

  if( LIST_TYPE_STLINKED != list->type ) {
    return NULL;
  }
#endif 

#if _LIST_INVALID_STRUCT_POSSIBLE
  if( !list->data ) {
    return NULL;
  }
#endif

  lstl = (struct list_stlinked *)list->data;

  item = malloc(sizeof(struct list_stlinked_item));
  memset(item, 0, sizeof(struct list_stlinked_item));

  item->data = malloc(sizeof(L_SIZE(list)));
  if( !item->data ) {
    free(item);
    return NULL;
  }

  if( data ) {
    memcpy(item->data, data, L_SIZE(list));
  } else {
    memset(item->data, 0, L_SIZE(list));
  }

  if( NULL == lstl->last ) {
#if _LIST_INVALID_STRUCT_POSSIBLE
    if( lstl->first ) {
      free(item->data);
      free(item);
      return NULL;
    }
#endif

    lstl->first = item;
    lstl->last = item;
  } else {
    item->prev = lstl->last;
    lstl->last = item;
  }

  ++lstl->count;

  return item->data;
}


/*
 * list_stlinked_length(LIst *llist)
 */
LIndex list_stlinked_length(List *list) {
  struct list_stlinked *lstl;

#if _LIST_ASUME_NOT_USING_MACROS
  if( !list ) {
    return LIST_ERR;
  }

  if( LIST_TYPE_STLINKED != list->type ) {
    return LIST_ERR;
  }
#endif 

#if _LIST_INVALID_STRUCT_POSSIBLE
  if( !list->data ) {
    return LIST_ERR;
  }
#endif

  lstl = (struct list_stlinked *)list->data;
  return lstl->count;
}


/*
 * list_stlinked_insert()
 */
void *list_stlinked_insert(List *list, LIndex pos, void *data) {
  struct list_stlinked *lst;
  struct list_stlinked_item *item;
  struct list_stlinked_item *new_item;
  LIndex i;
#if _LIST_ASUME_NOT_USING_MACROS
  if( !list ) {
    return NULL;
  }

  if( LIST_TYPE_STLINKED != list->type ) {
    return NULL;
  }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE 
  if( !list->data ) {
    return NULL;
  }
#endif

  lst = (struct list_stlinked *)list->data;

  if( 0 > pos || lst->count < pos) {
    return NULL;
  }

  if( 0 == pos ) {
    return list_stlinked_pushfront(list, data);
  } else if( lst->count == pos ) {
    return list_stlinked_pushback(list, data);
  } else {
  /* 
    if( 0 >= lst->count || lst->count <= pos) {
      return NULL;
    }
  */

    for( i = lst->count - 1, item = lst->last; 
         item->prev && pos < i; 
         --i, item = item->prev );

#if _LIST_INVALID_STRUCT_POSSIBLE 
    if( !item ) {
      return NULL;
    }
#endif

    new_item = malloc(sizeof(struct list_stlinked_item));
    if( !new_item) {
      return NULL;
    }
    memset(new_item, 0, sizeof(struct list_stlinked_item));

    new_item->data = malloc(L_SIZE(list));
    if( !new_item->data ) {
      free(new_item);
      return NULL;
    }
    if( data ) {
      memcpy(new_item->data, data, L_SIZE(list));
    } else {
      memset(new_item->data, 0, L_SIZE(list));
    }

    new_item->prev = item->prev;
    item->prev = new_item;
    ++lst->count;
    return new_item->data;
  }

}


/*
 * list_stlinked_get()
 */
void *list_stlinked_get(List *list, LIndex pos) {
  struct list_stlinked *lstlinked;
  struct list_stlinked_item *item;
  LIndex i;

#if _LIST_ASUME_NOT_USING_MACROS
  if( !list ) {
    return NULL;
  }

  if( LIST_TYPE_STLINKED != list->type ) {
    return NULL;
  }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE 
  if( !list->data ) {
    return NULL;
  }
#endif

  lstlinked = (struct list_stlinked *)list->data;

  if( 0 >= lstlinked->count || lstlinked->count <= pos) {
    return NULL;
  }

  for( i = lstlinked->count - 1, item = lstlinked->last; 
       item->prev && pos < i; 
       --i, item = item->prev );

  return item->data;
}


/*
 * list_stlinked_iter_next()
 */
void *list_stlinked_iter_next(LIter *iter) {
  struct list_stlinked_iter *it;
#if _LIST_ASUME_NOT_USING_MACROS
  if( !iter ) {
    return NULL;
  }

  if( LIST_TYPE_STLINKED != iter->type ) {
    return NULL;
  }
#endif

  if( !iter->data ) {
    return NULL;
  }

  it = (struct list_stlinked_iter *)iter->data;

  it->i++;

  return list_stlinked_get(it->list, it->i);
}


/*
 * list_stlinked_iter_free()
 */
LRet list_stlinked_iter_free(LIter *iter) {
#if _LIST_ASUME_NOT_USING_MACROS
  if( !iter ) {
    return LIST_ERR;
  }

  if( LIST_TYPE_STLINKED != iter->type ) {
    return LIST_ERR;
  }
#endif

  free(iter->data);
  iter->data = NULL;

  return LIST_OK;
}


/*
 * list_stlinked_iter()
 */
void *list_stlinked_iter(List *list, LIter *iter) {

  if( !iter ) {
    return NULL;
  }

#if _LIST_ASUME_NOT_USING_MACROS
  if( !list ) {
    return NULL;
  }

  if( LIST_TYPE_STLINKED != list->type ) {
    return NULL;
  }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE 
  if( !list->data ) {
    return NULL;
  }
#endif

  memset(iter, 0, sizeof(LIter));
  
  iter->data = malloc(sizeof(struct list_stlinked_iter));
  if( !iter->data ) {
    return NULL;
  }

  ((struct list_stlinked_iter *)iter->data)->i = 0;
  ((struct list_stlinked_iter *)iter->data)->list = list;

  iter->type = LIST_TYPE_STLINKED;
  iter->next = list_stlinked_iter_next;
  iter->free = list_stlinked_iter_free;

  return list_stlinked_get(list, 0);
}


/*
 * list_stlinked_popfront()
 */
LRet list_stlinked_popfront(List *list, void *data) {
  struct list_stlinked *lstl;
  struct list_stlinked_item *item;

#if _LIST_ASUME_NOT_USING_MACROS
  if( !list ) {
    return LIST_ERR;
  }

  if( LIST_TYPE_STLINKED != list->type ) {
    return LIST_ERR;
  }
#endif 

#if _LIST_INVALID_STRUCT_POSSIBLE
  if( !list->data ) {
    return LIST_ERR;
  }
#endif

  lstl = (struct list_stlinked *)list->data;

  for( item = lstl->last; item->prev && item->prev->prev; item = item->prev );

  if( data ) {
    memcpy(data, item->prev->data, L_SIZE(list));
  }
  free(item->prev->data);
  free(item->prev);
  item->prev = NULL;
  lstl->first = item;
  lstl->count--;

  return LIST_OK;
}


/*
 * list_stlinked_popback()
 */
LRet list_stlinked_popback(List *list, void *data) {
  struct list_stlinked *lstl;
  struct list_stlinked_item *item;

#if _LIST_ASUME_NOT_USING_MACROS
  if( !list ) {
    return LIST_ERR;
  }

  if( LIST_TYPE_STLINKED != list->type ) {
    return LIST_ERR;
  }
#endif 

#if _LIST_INVALID_STRUCT_POSSIBLE
  if( !list->data ) {
    return LIST_ERR;
  }
#endif

  lstl = (struct list_stlinked *)list->data;

  item = lstl->last;
  lstl->last = item->prev;

  if( data ) {
    memcpy(data, item->data, L_SIZE(list));
  }
  free(item->data);
  free(item);
  lstl->count--;

  return LIST_OK;
}


/*
 * list_stlinked_remove()
 */
LRet list_stlinked_remove(List *list, LIndex pos, void *data) {
 struct list_stlinked *lstl;
 struct list_stlinked_item *item, *tmp;
 LIndex i;

#if _LIST_ASUME_NOT_USING_MACROS
  if( !list ) {
    return LIST_ERR;
  }

  if( LIST_TYPE_STLINKED != list->type ) {
    return LIST_ERR;
  }
#endif 

#if _LIST_INVALID_STRUCT_POSSIBLE
  if( !list->data ) {
    return LIST_ERR;
  }
#endif

  lstl = (struct list_stlinked *)list->data;

  if( 0 == pos ) {
    return list_stlinked_popfront(list, data);
  } else if( lstl->count-1 == pos ) {
    return list_stlinked_popback(list, data);
  } else {
    for( i = lstl->count - 1, item = lstl->last ; lstl && pos + 1 < i; item = item->prev, i--);
    
    tmp = item->prev;
    item->prev = item->prev->prev;

    if( data ) {
      memcpy(data, tmp->data, L_SIZE(list));
    }
    free(tmp->data);
    free(tmp);
    lstl->count--;
  }

  return LIST_OK;
}


/*
 * list_stlinked_removelast()
 */
LRet list_stlinked_removelast(List *list, void *del, void *data) {
 struct list_stlinked *lstl;
 struct list_stlinked_item *item, *tmp;

#if _LIST_ASUME_NOT_USING_MACROS
  if( !list ) {
    return LIST_ERR;
  }

  if( LIST_TYPE_STLINKED != list->type ) {
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

  lstl = (struct list_stlinked *)list->data;
  item = lstl->last;
  tmp = NULL;

  while( item && !list->equal(item->data, del) ) {
    tmp = item;
    item = item->prev;
  }

  if( item ) { 
    if( tmp ) {
      tmp->prev = item->prev;
    } else {
      lstl->last = item->prev;
    }
    if( data ) {
      memcpy(data, item->data, L_SIZE(list));
    }
    free(item->data);
    free(item);
    lstl->count--;
  } else {
    return LIST_ELEMENT_NOT_FOUND;
  }

  return LIST_OK;
}


/*
 * list_stlinked_free()
 */
LRet list_stlinked_clear(List *list) {
  struct list_stlinked *lstl;
  struct list_stlinked_item *item, *tmp;

#if _LIST_ASUME_NOT_USING_MACROS
  if( !list ) {
    return LIST_ERR;
  }

  if( LIST_TYPE_STLINKED != list->type ) {
    return LIST_ERR;
  }
#endif 

#if _LIST_INVALID_STRUCT_POSSIBLE
  if( !list->data ) {
    return LIST_ERR;
  }
#endif

  lstl = (struct list_stlinked *)list->data;

  item = lstl->last;
  while( item ) {
    tmp = item;
    item = item->prev;
    free(tmp->data);
    free(tmp);
  }

  lstl->first = NULL;
  lstl->last = NULL;
  lstl->count = 0;

  return LIST_OK;
}


/*
 * list_stlinked_free()
 */
LRet list_stlinked_free(List *list) {

#if _LIST_ASUME_NOT_USING_MACROS
  if( !list ) {
    return LIST_ERR;
  }

  if( LIST_TYPE_STLINKED != list->type ) {
    return LIST_ERR;
  }
#endif 

#if _LIST_INVALID_STRUCT_POSSIBLE
  if( !list->data ) {
    return LIST_ERR;
  }
#endif
  if( list_stlinked_clear(list) ) {
    return LIST_ERR;
  }

  free(list->data);
  free(list);

  return LIST_OK;
}


/*
 * list_stlinked_new();
 */
List *list_stlinked_new(LOpts *opts) {
  List *list;
  struct list_stlinked *lstl;

  if( !opts ) {
    return NULL;
  }
  
  if( 0 >= opts->size ) {
    return NULL;
  }

  lstl = malloc(sizeof(struct list_stlinked));
  if( !lstl ) {
    return NULL;
  }

  memset(lstl, 0, sizeof(struct list_stlinked));

  list = malloc(sizeof(List));
  if( !list ) {
    free(lstl);
    return NULL;
  }

  memset(list, 0, sizeof(List));
  memcpy(&list->opts, opts, sizeof(LOpts));

  list->type = LIST_TYPE_STLINKED;
  list->data = lstl;

  list->free       = list_stlinked_free;
  list->length     = list_stlinked_length;
  list->insert     = list_stlinked_insert;
  list->remove     = list_stlinked_remove;
  list->get        = list_stlinked_get;
  list->last       = list_stlinked_last;
  list->pushback   = list_stlinked_pushback;
  list->popback    = list_stlinked_popback;
  list->first      = list_stlinked_first;
  list->pushfront  = list_stlinked_pushfront;
  list->popfront   = list_stlinked_popfront;
  list->iter       = list_stlinked_iter;
  list->removelast = list_stlinked_removelast;
  list->clear      = list_stlinked_clear;

  return list;
}

