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
/*
 * TODO:
 * - functions for copying lists
 * - functions for converting lists from type to type
 * - reverse iterators
 */
#ifndef __list_h__
#define __list_h__

#include <stdint.h>
#include <stdlib.h>
#include <utils/log.h>

#define LIST_OK                ( 0)
#define LIST_ERR               (-1)
#define LIST_ELEMENT_NOT_FOUND (-2)

struct list_iter;
struct list_options;
struct list;

typedef int LRet;
typedef uint64_t LIndex;
typedef uint32_t LFlags;
typedef struct list List;
typedef struct list_options LOpts;
typedef struct list_iter LIter;
typedef size_t LSize;

enum list_type {
  LIST_TYPE_NONE = 0,

  /*
   * LIST_TYPE_DLINKED -- Double linked list implementation.
   *
   * Each element is stored together with two pointers. On points to the successor element and
   * one points to the predecessor element.
   *
   * This implementation has the complexity of O(1) for inserting, removing and accessing an element 
   * at the head or at the tail. Inserting, removing or accessing a element at a arbitrary position 
   * has a complexity of O(n/4). Inserting or removing an element will not cause a reallocation of
   * memory and therefore no unecpected runtime peaks whill occure.
   *
   * Itteration through the array will not profit from caching mechanisms of the CPU, because the 
   * elements are stored as a block in the memory. In fact this implementation tends to fraction the
   * memory.
   */
  LIST_TYPE_DLINKED,
  
  /*
   * LIST_TYPE_STLINKED -- Single linked list (Tail insert, Tail remove)
   * TODO: Documentation
   */
  LIST_TYPE_STLINKED,

  /*
   * LIST_TYPE_ARRAY -- List implementation using an array.
   *
   *
   * Efficient for LIFOs. Usually push_back and pop_back have a computional complexity of O(1).
   * In the case, that the array is to small, then a memory reallocation will occour. The
   * Reallocation will double or halve the current array size. That means the greater the array
   * the more seldom a reallocation will occur. In case of adding an element the reallocation may
   * have a superior complexity since the "reaclloc" call may have to copy the array depending of
   * the memory fraction.
   *
   * This implementation is inefficient while inserting or deleting elements which are not at the 
   * end of the list. The complexity for removing or inserting a element at the beginning is
   * O(n). When the array has to be expanded the complexity may rise due to the copying actions
   * of realloc().
   *
   * Accessing an element has always the complexity O(1) since the position can be calculated.
   *
   * The implementation is also efficient while itterating through the list since all data is lays
   * conteneously in the memory and chaching mechanism of the CPU can be applayed.
   *
   * In terms of memory usage the implementation is efficient. Only a counter and a pointer to the 
   * first element is used by the implementation in theory. Due to the memory  management the actual
   * memory useage can be twice as high as really needed.
   *
   * Please note, that the user of listimplementation have to decide whether he use the list as
   * pointer array or he insert whole elements.
   */
  LIST_TYPE_ARRAY
};

struct list_iter {
  enum list_type type;
  void *data;
  void *(*next)(LIter *iter);
  LRet (*free)(LIter *iter);
};

struct list_options {
  LFlags flags;
  void *data;
  LSize size;
};

struct list {
  enum list_type type;
  LOpts opts;
  void *data;
  LRet (*free)(List *list);
  LIndex (*length)(List *list);
  void *(*insert)(List *list, LIndex pos, void *data);
  LRet (*remove)(List *list, LIndex pos, void *data);
  void *(*get)(List *list, LIndex pos);
  void *(*last)(List *list);
  void *(*pushback)(List *list, void *data);
  LRet (*popback)(List *list, void *data);
  void *(*first)(List *list);
  void *(*pushfront)(List *list, void *data);
  LRet (*popfront)(List *list, void *data);
  void *(*iter)(List *list, LIter *iter);
  LRet (*clear)(List *list);

  int (*equal)(void *data1, void *data2);
  int (*cmp)(void *data1, void *data2);
  LRet (*removefirst)(List *list, void *del, void *data);
  LRet (*removelast)(List *list, void *del, void *data);
//  LRet (*removeall)(List *list, void *del); /* maybe dangerous */
};

#include "utils/list/list_dlinked.h"
#include "utils/list/list_stlinked.h"
#include "utils/list/list_array.h"

#define L_NEW(_type, _opts)			(\
                                        	  ((_type) == LIST_TYPE_DLINKED) ? (list_dlinked_new(_opts)) : (\
						  ((_type) == LIST_TYPE_STLINKED) ? (list_stlinked_new(_opts)) : (\
                                        	  ((_type) == LIST_TYPE_ARRAY) ? (list_array_new(_opts)) : (\
						  NULL\
						  )))\
						)

#define L_SIZE(_list)				((_list) ? ((_list)->opts.size) : (0))

#define L_FREE(_list)				(((_list) && (_list)->free) ? ((_list)->free(_list)) : (LIST_ERR))
#define L_LENGTH(_list)				(((_list) && (_list)->length) ? ((_list)->length(_list)) : (LIST_ERR))
#define L_INSERT(_list, _pos, _data)		(((_list) && (_list)->insert) ? ((_list)->insert((_list), (_pos), (_data))) : (NULL))
#define L_REMOVE(_list, _pos, _data)		(((_list) && (_list)->remove) ? ((_list)->remove((_list), (_pos), (_data))) : (LIST_ERR))
#define L_GET(_list, _pos)			(((_list) && (_list)->get) ? ((_list)->get((_list), (_pos))) : (NULL))

#define L_LAST(_list)				(((_list) && (_list)->last) ? ((_list)->last((_list))) : (NULL))
#define L_PUSHBACK(_list, _data)		(((_list) && (_list)->pushback) ? ((_list)->pushback((_list), (_data))) : (NULL))
#define L_POPBACK(_list, _data)			(((_list) && (_list)->popback) ? ((_list)->popback((_list), (_data))) : (LIST_ERR))

#define L_FIRST(_list) 				(((_list) && (_list)->first) ? ((_list)->first(_list)) : (NULL))
#define L_PUSHFRONT(_list, _data)		(((_list) && (_list)->pushfront) ? ((_list)->pushfront((_list), (_data))) : (NULL))
#define L_POPFRONT(_list, _data)		(((_list) && (_list)->popfront) ? ((_list)->popfront((_list), (_data))) : (LIST_ERR))

#define L_FOREACH(_list, _iter, _item)		if( !(_list) || !(_list)->iter ) \
                                        	   INFO("Iterators not implementated.");\
						else \
                                        	for( (_item) = (_list)->iter(_list, _iter); \
						     (_item) || (((_iter)->free) && (_iter)->free(_iter)); \
						     (_item) = (_iter)->next(_iter) )

#define L_ITERFREE(_iter)			(((_iter) && ((_iter)->free) ) ? ((_iter)->free(_iter)) : (LIST_ERR))

#define L_CLEAR(_list)				(((_list) && (_list)->clear) ? ((_list)->clear(_list)) : (LIST_ERR))

#define L_SETEQUAL(_list, _equal)		(((_list) && (((_list)->equal = _equal) || 1)) ? (LIST_OK) : (LIST_ERR))
#define L_REMOVEFIRST(_list, _del, _data)	(((_list) && (_list)->removefirst) ? ((_list)->removefirst(_list, _del, _data)) : (LIST_ERR))
#define L_REMOVELAST(_list, _del, _data)	(((_list) && (_list)->removelast) ? ((_list)->removelast(_list, _del, _data)) : (LIST_ERR))
//#define L_REMOVEALL(_list, _del)		(((_list) && (_list)->removeall) ? ((_list)->removeall(_list, _del)) : (LIST_ERR))

#endif /* __list_h__ */
