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
#include "unittest.h"

int equal(void *data1, void *data2) {
  int *int1 = data1;
  int *int2 = data2;

  return *int1 == *int2;
}

int ut_equal(enum list_type type) {
  List *list = NULL;
  LOpts opts;
  int i;
  int del;
  int data;

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
  for( i = 0; i < 10; ++i ) {
    if( !L_PUSHBACK(list, &i) ) {
      fprintf(stderr, "ERR: L_PUSHBACK returned LIST_ERR\n");
      return UT_FAIL;
    }
  }

  ut_state(list);

  /*** pushback 10 elements ***/
  printf("L_PUSHBACK 10 elements\n");
  for( i = 0; i < 10; ++i ) {
    if( !L_PUSHBACK(list, &i) ) {
      fprintf(stderr, "ERR: L_PUSHBACK returned LIST_ERR\n");
      return UT_FAIL;
    }
  }

  ut_state(list);

  /*** pushback 10 elements ***/
  printf("L_PUSHBACK 10 elements\n");
  for( i = 0; i < 10; ++i ) {
    if( !L_PUSHBACK(list, &i) ) {
      fprintf(stderr, "ERR: L_PUSHBACK returned LIST_ERR\n");
      return UT_FAIL;
    }
  }

  ut_state(list);

  printf("L_SETEQUAL\n");
  L_SETEQUAL(list, equal);

  printf("L_REMOVELAST 5\n");
  del = 5;
  data = 0;
  if( 0 > L_REMOVELAST(list, &del, &data) ) {
    fprintf(stderr, "ERR: L_REMOVELAST returned LIST_ERR\n");
    return UT_FAIL;
  }
  printf("data = %d\n", data);

  ut_state(list);

  printf("L_REMOVELAST 5\n");
  del = 5;
  data = 0;
  if( 0 > L_REMOVELAST(list, &del, &data) ) {
    fprintf(stderr, "ERR: L_REMOVELAST returned LIST_ERR\n");
    return UT_FAIL;
  }
  printf("data = %d\n", data);

  ut_state(list);

  printf("L_REMOVELAST 3\n");
  del = 3;
  data = 0;
  if( 0 > L_REMOVELAST(list, &del, &data) ) {
    fprintf(stderr, "ERR: L_REMOVELAST returned LIST_ERR\n");
    return UT_FAIL;
  }
  printf("data = %d\n", data);

  ut_state(list);

  printf("L_REMOVELAST 5\n");
  del = 5;
  data = 0;
  if( 0 > L_REMOVELAST(list, &del, &data) ) {
    fprintf(stderr, "ERR: L_REMOVELAST returned LIST_ERR\n");
    return UT_FAIL;
  }
  printf("data = %d\n", data);

  ut_state(list);

  printf("L_REMOVELAST 5\n");
  del = 5;
  data = 0;
  if( LIST_ELEMENT_NOT_FOUND != L_REMOVELAST(list, &del, &data) ) {
    fprintf(stderr, "ERR: L_REMOVELAST with other status than LIST_ELEMENT_NOT_FOUND\n");
    return UT_FAIL;
  }

  ut_state(list);

  if( L_FREE(list) ) {
    return UT_FAIL;
  }
  
  return UT_SUCCESS;
}

