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
#include <stdio.h>
#include <stdlib.h>
#include <utils/list.h>
#include "unittest.h"


/*
 * ut_generic()
 */
int ut_generic(enum list_type type) {
  List *list = NULL;
  LOpts opts;
  int i, cnt=0;
  int data = 13117;

  memset(&opts, 0, sizeof(opts));
  opts.size = sizeof(int);

  /*** init list ***/
  printf("L_NEW\n");
  list = L_NEW(type, &opts);
  if( !list ) {
    fprintf(stderr, "ERR: L_NEW returned NULL pointer\n");
    return UT_FAIL;
  }

  ut_state(list);

  /*** pushback 10 elements ***/
  printf("L_PUSHBACK 10 elements\n");
  for( i = 0; i < 10; ++i, ++cnt ) {
    if( !L_PUSHBACK(list, &cnt) ) {
      fprintf(stderr, "ERR: L_PUSHBACK returned LIST_ERR\n");
      return UT_FAIL;
    }
  }

  ut_state(list);

  /*** pushfront 10 elements ***/
  printf("L_PUSHFRONT 10 elements\n");
  for( i = 0; i < 10; ++i, ++cnt ) {
    if( !L_PUSHFRONT(list, &cnt) ) {
      fprintf(stderr, "ERR: L_PUSHFRONT returned LIST_ERR\n");
      return UT_FAIL;
    }
  }

  ut_state(list);

  /*** insert 10 elements ***/
  printf("L_INSERT 10 elements\n");
  for( i = 0; i < 10; ++i, ++cnt ) {
    if( !L_INSERT(list, 5 + i, &cnt) ) {
      fprintf(stderr, "ERR: L_INSERT returned LIST_ERR\n");
      return UT_FAIL;
    }
  }

  ut_state(list);

  /*** remove 5 elements ***/
  printf("L_REMOVE 5 elements\n");
  for( i = 0; i < 5; ++i, ++cnt ) {
    if( L_REMOVE(list, 12, &data) ) {
      fprintf(stderr, "ERR: L_REMOVE returned LIST_ERR\n");
      return UT_FAIL;
    }
    printf("got back %d\n", data);
  }

  ut_state(list);

  /*** popfront 3 elements ***/
  printf("L_POPFRONT  elements\n");
  for( i = 0; i < 3; ++i, ++cnt ) {
    if( L_POPFRONT(list, &data) ) {
      fprintf(stderr, "ERR: L_POPFRONT returned LIST_ERR\n");
      return UT_FAIL;
    }
    printf("got back %d\n", data);
  }

  ut_state(list);

  /*** popback 3 elements ***/
  printf("L_POPBACK  elements\n");
  for( i = 0; i < 3; ++i, ++cnt ) {
    if( L_POPBACK(list, &data) ) {
      fprintf(stderr, "ERR: L_POPBACK returned LIST_ERR\n");
      return UT_FAIL;
    }
    printf("got back %d\n", data);
  }

  ut_state(list);
  
  if( L_FREE(list) ) {
    return UT_FAIL;
  }


  return UT_SUCCESS;
}
