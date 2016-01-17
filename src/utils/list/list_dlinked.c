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
#include "utils/list/_list.h"
#include <stdlib.h>
#include <string.h>


/*
 * list_dlinked_pushfront()
 */
void *list_dlinked_pushfront(List *list, void *data) {
  struct list_dlinked *ldl;
  struct list_dlinked_item *i;
#if _LIST_ASUME_NOT_USING_MACROS
  if( !list ) {
    return NULL;
  }

  if( LIST_TYPE_DLINKED != list->type ) {
    return NULL;
  }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE 
  if( !list->data ) {
    return NULL;
  }
#endif
  ldl = (struct list_dlinked *)list->data;
 
  i = malloc(sizeof(struct list_dlinked_item));
  if( !i ) {
    return NULL;
  }
  memset(i, 0, sizeof(struct list_dlinked_item));

  i->data = malloc(L_SIZE(list));
  if( !i->data ) {
    free(i);
    return NULL;
  }

  if( data ) {
    memcpy(i->data, data, L_SIZE(list));
  } else {
    memset(i->data, 0, L_SIZE(list));
  }

  if( !ldl->first ) {
#if _LIST_INVALID_STRUCT_POSSIBLE 
    if( ldl->last ) {
      free(i);
      return NULL;
    }
#endif
    ldl->first = i;
    ldl->last = i;
  }else{
    i->next = ldl->first;
    ldl->first->prev = i;
    ldl->first = i;
  }

  ++ldl->count;

  return i->data;
}


/*
 * list_dlinked_pushback()
 */
void *list_dlinked_pushback(List *list, void *data) {
  struct list_dlinked *ldl;
  struct list_dlinked_item *i;
#if _LIST_ASUME_NOT_USING_MACROS
  if( !list ) {
    return NULL;
  }

  if( LIST_TYPE_DLINKED != list->type ) {
    return NULL;
  }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE 
  if( !list->data ) {
    return NULL;
  }
#endif
  ldl = (struct list_dlinked *)list->data;
 
  i = malloc(sizeof(struct list_dlinked_item));
  if( !i ) {
    return NULL;
  }
  memset(i, 0, sizeof(struct list_dlinked_item));

  i->data = malloc(L_SIZE(list));
  if( !i->data ) {
    free(i);
    return NULL;
  }

  if( data ) {
    memcpy(i->data, data, L_SIZE(list));
  } else {
    memset(i->data, 0, L_SIZE(list));
  }

  if( !ldl->last ) {
#if _LIST_INVALID_STRUCT_POSSIBLE 
    if( ldl->first ) {
      free(i);
      return NULL;
    }
#endif
    ldl->first = i;
    ldl->last = i;
  }else{
    i->prev = ldl->last;
    ldl->last->next = i;
    ldl->last = i;
  }

  ++ldl->count;

  return i->data;
}


/*
 * list_dlinked_popfront()
 */
LRet list_dlinked_popfront(List *list, void *data) {
  struct list_dlinked *ldl;
  struct list_dlinked_item *tmp;
#if _LIST_ASUME_NOT_USING_MACROS
  if( !list ) {
    return LIST_ERR;
  }

  if( LIST_TYPE_DLINKED != list->type ) {
    return LIST_ERR;
  }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE 
  if( !list->data ) {
    return LIST_ERR;
  }
#endif

  ldl = (struct list_dlinked *)list->data;

  if( !ldl->first ) {
    return LIST_ERR;
  }

  tmp = ldl->first;

  ldl->first = tmp->next;
  
  if( ldl->first ) {
    ldl->first->prev = NULL;
  } else {
    ldl->last = NULL;
  }

  --ldl->count;
  if( data ) {
    memcpy(data, tmp->data, L_SIZE(list));
  }
  free(tmp->data);
  free(tmp);

  return LIST_OK;
}


/*
 * list_dlinked_popback()
 */
LRet list_dlinked_popback(List *list, void *data) {
  struct list_dlinked *ldl;
  struct list_dlinked_item *tmp;
#if _LIST_ASUME_NOT_USING_MACROS
  if( !list ) {
    return LIST_ERR;
  }

  if( LIST_TYPE_DLINKED != list->type ) {
    return LIST_ERR;
  }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE 
  if( !list->data ) {
    return LIST_ERR;
  }
#endif

  ldl = (struct list_dlinked *)list->data;

  if( !ldl->last ) {
    return LIST_ERR;
  }

  tmp = ldl->last;

  if( !tmp ) {
    return LIST_ERR;
  }
  
  ldl->last = tmp->prev;

  if( ldl->last ) {
    ldl->last->next = NULL;
  }

  --ldl->count;
  if( data ) {
    memcpy(data, tmp->data, L_SIZE(list));
  }
  free(tmp->data);
  free(tmp);

  return LIST_OK;
  
}


/*
 * list_dlinked_length()
 */
LIndex list_dlinked_length(List *list) {
  struct list_dlinked *ldl;
#if _LIST_ASUME_NOT_USING_MACROS
  if( !list ) {
    return LIST_ERR;
  }

  if( LIST_TYPE_DLINKED != list->type ) {
    return LIST_ERR;
  }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE 
  if( !list->data ) {
    return LIST_ERR;
  }
#endif

  ldl = (struct list_dlinked *)list->data;

  return ldl->count;
}


/*
 * list_dlinked_insert()
 */
void *list_dlinked_insert(List *list, LIndex pos, void *data) {
  struct list_dlinked *ldl;
  struct list_dlinked_item *item;
  struct list_dlinked_item *new_item;
  int i;
#if _LIST_ASUME_NOT_USING_MACROS
  if( !list ) {
    return NULL;
  }

  if( LIST_TYPE_DLINKED != list->type ) {
    return NULL;
  }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE 
  if( !list->data ) {
    return NULL;
  }
#endif

  ldl = (struct list_dlinked *)list->data;

  if( 0 > pos || ldl->count < pos) {
    return NULL;
  }

  if( 0 == pos ) {
    return list_dlinked_pushfront(list, data);
  } else if( ldl->count == pos ) {
    return list_dlinked_pushback(list, data);
  } else {
    for( i = 0, item = ldl->first; i < pos; i++ ) {
      item = item->next;
    }

    new_item = malloc(sizeof(struct list_dlinked_item));
    if( !new_item) {
      return NULL;
    }
    memset(new_item, 0, sizeof(struct list_dlinked_item));

    new_item->data = malloc(L_SIZE(list));
    if( !new_item->data ) {
      free(new_item);
      return NULL;
    }
    memcpy(new_item->data, data, L_SIZE(list));

    new_item->next = item;
    new_item->prev = item->prev;
    item->prev->next = new_item;
    item->prev = new_item;
    ++ldl->count;
    return new_item->data;
  }

}


/*
 * list_dlinked_remove()
 */
LRet list_dlinked_remove(List *list, LIndex pos, void *data) {
  struct list_dlinked *ldl;
  struct list_dlinked_item *item;
  int i;
#if _LIST_ASUME_NOT_USING_MACROS
  if( !list ) {
    return LIST_ERR;
  }

  if( LIST_TYPE_DLINKED != list->type ) {
    return LIST_ERR;
  }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE 
  if( !list->data ) {
    return LIST_ERR;
  }
#endif

  ldl = (struct list_dlinked *)list->data;

  if( 0 > pos || ldl->count < pos) {
    return LIST_ERR;
  }

  if( 0 == pos ) {
    return list_dlinked_popfront(list, data);
  } else if( ldl->count-1 == pos ) {
    return list_dlinked_popback(list, data);
  } else {
    for( i = 0, item = ldl->first; i < pos; i++ ) {
      item = item->next;
    }
    
    item->next->prev = item->prev;
    item->prev->next = item->next;
    if( data ) {
      memcpy(data, item->data, L_SIZE(list));
    }
    free(item->data);
    free(item);
    --ldl->count;
    return LIST_OK;
  }
}


/*
 * list_dlinked_removelast()
 */
LRet list_dlinked_removelast(List *list, void *del, void *data) {
  struct list_dlinked *ldl;
  struct list_dlinked_item *item;

#if _LIST_ASUME_NOT_USING_MACROS
  if( !list ) {
    return LIST_ERR;
  }

  if( LIST_TYPE_DLINKED != list->type ) {
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

  ldl = (struct list_dlinked *)list->data;
  item = ldl->last;

  while( item && !list->equal(item->data, del) ) {
    item = item->prev;
  }

  if( item ) { 

    if( item->prev ) {
      item->prev->next = item->next;
    } else {
      ldl->first = item->next;
    }

    if( item->next ) {
      item->next->prev = item->prev;
    } else {
      ldl->last = item->prev;
    }

    if( data ) {
      memcpy(data, item->data, L_SIZE(list));
    }
    free(item->data);
    free(item);
    ldl->count--;
  } else {
    return LIST_ELEMENT_NOT_FOUND;
  }

  return LIST_OK;
}


/*
 * list_dlinked_clear()
 */
LRet list_dlinked_clear(List *list) {
  struct list_dlinked *ldl;
  struct list_dlinked_item *item, *tmp;

#if _LIST_ASUME_NOT_USING_MACROS
  if( !list ) {
    return LIST_ERR;
  }

  if( LIST_TYPE_DLINKED != list->type ) {
    return LIST_ERR;
  }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE 
  if( !list->data ) {
    return LIST_ERR;
  }
#endif

  ldl = (struct list_dlinked *)list->data;
  item = ldl->last;

  while( item ) {
    tmp = item;
    item = item->prev;
    free(tmp->data);
    free(tmp);
  }

  ldl->first = NULL;
  ldl->last = NULL;
  ldl->count = 0;

  return LIST_OK;
}


/*
 * list_dlinked_get()
 */
void *list_dlinked_get(List *list, LIndex pos) {
  struct list_dlinked *ldl;
  struct list_dlinked_item *item;
  int i;
#if _LIST_ASUME_NOT_USING_MACROS
  if( !list ) {
    return NULL;
  }

  if( LIST_TYPE_DLINKED != list->type ) {
    return NULL;
  }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE 
  if( !list->data ) {
    return NULL;
  }
#endif

  ldl = (struct list_dlinked *)list->data;

  if( 0 > pos || ldl->count < pos) {
    return NULL;
  }

  if( 0 == pos ) {
    return ldl->first->data;
  } else if( ldl->count-1 == pos ) {
    return ldl->last->data;
  } else {
    for( i = 0, item = ldl->first; i < pos; i++ ) {
      item = item->next;
    }
    
    return item->data;
  }

}


/*
 * list_dlinked_last()
 */
void *list_dlinked_last(List *list)  {
  struct list_dlinked *ldl;
#if _LIST_ASUME_NOT_USING_MACROS
  if( !list ) {
    return NULL;
  }

  if( LIST_TYPE_DLINKED != list->type ) {
    return NULL;
  }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE 
  if( !list->data ) {
    return NULL;
  }
#endif

  ldl = (struct list_dlinked *)list->data;

  if( !ldl->last ) {
    return NULL;
  }

  return ldl->last->data;
}


/*
 * list_dlinked_first()
 */
void *list_dlinked_first(List *list) {
  struct list_dlinked *ldl;
#if _LIST_ASUME_NOT_USING_MACROS
  if( !list ) {
    return NULL;
  }

  if( LIST_TYPE_DLINKED != list->type ) {
    return NULL;
  }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE 
  if( !list->data ) {
    return NULL;
  }
#endif

  ldl = (struct list_dlinked *)list->data;

  if( !ldl->first ) {
    return NULL;
  }

  return ldl->first->data;
}


/*
 * list_dlinked_iter_next()
 */
void *list_dlinked_iter_next(LIter *iter) {
  struct list_dlinked_item *item;
#if _LIST_ASUME_NOT_USING_MACROS
  if( !iter ) {
    return NULL;
  }

  if( LIST_TYPE_DLINKED != iter->type ) {
    return NULL;
  }
#endif

  if( !iter->data ) {
    return NULL;
  }

  item = (struct list_dlinked_item *)iter->data;

  iter->data = item->next;
  item = item->next;

  if( !item ) {
    return NULL;
  }

  return item->data;
}


/*
 * list_dlinked_iter_free()
 */
LRet list_dlinked_iter_free(LIter *iter) {
  // nothing to do here
  return LIST_OK;
}

/*
 * list_dlinked_iter()
 */
void *list_dlinked_iter(List *list, LIter *iter) {
  struct list_dlinked *ldl;

  if( !iter ) {
    return NULL;
  }

#if _LIST_ASUME_NOT_USING_MACROS
  if( !list ) {
    return NULL;
  }

  if( LIST_TYPE_DLINKED != list->type ) {
    return NULL;
  }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE 
  if( !list->data ) {
    return NULL;
  }
#endif

  ldl = (struct list_dlinked *)list->data;

  memset(iter, 0, sizeof(LIter));

  if( !ldl->first ) {
    return NULL;
  }

  iter->type = LIST_TYPE_DLINKED;
  iter->data = ldl->first;
  iter->next = list_dlinked_iter_next;
  iter->free = list_dlinked_iter_free;

  return ldl->first->data;
}


/* 
 * list_dlinked_free()
 */
LRet list_dlinked_free(List *list) {
#if _LIST_ASUME_NOT_USING_MACROS
  if( !list ) {
    return LIST_ERR;
  }

  if( LIST_TYPE_DLINKED != list->type ) {
    return LIST_ERR;
  }
#endif
  
#if _LIST_INVALID_STRUCT_POSSIBLE 
  if( !list->data ) {
    return LIST_ERR;
  }
#endif

  if( list_dlinked_clear(list) ) {
    return LIST_ERR;
  }
  
  free(list->data);
  free(list);

  return LIST_OK;
}


/*
 * list_dlinked_new()
 */
List *list_dlinked_new(LOpts *opts) {
  struct list_dlinked *ldl;
  List *list;

  if( !opts ) {
    return NULL;
  }

  if( 0 == opts->size ) {
    return NULL;
  }

  /* new dlinked list structure */
  ldl = malloc(sizeof(struct list_dlinked));
  if( !ldl ) {
    return NULL;
  }
  memset(ldl, 0, sizeof(struct list_dlinked));

  /* new list structure */
  list = malloc(sizeof(List));
  if( !list ) {
    free(ldl);
    return NULL;
  }
  memset(list, 0, sizeof(List));

  memcpy(&list->opts, opts, sizeof(LOpts));
  list->data = ldl;
  
  list->type = LIST_TYPE_DLINKED;

  list->free       = list_dlinked_free;
  list->length     = list_dlinked_length;
  list->insert     = list_dlinked_insert;
  list->remove     = list_dlinked_remove;
  list->get        = list_dlinked_get;
  list->last       = list_dlinked_last;
  list->pushback   = list_dlinked_pushback;
  list->popback    = list_dlinked_popback;
  list->first      = list_dlinked_first;
  list->pushfront  = list_dlinked_pushfront;
  list->popfront   = list_dlinked_popfront;
  list->iter       = list_dlinked_iter;
  list->removelast = list_dlinked_removelast;
  list->clear      = list_dlinked_clear;

  return list;
}

