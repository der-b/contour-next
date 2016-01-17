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
#include "utils/list.h"

void state(List *list)
{
  //struct list_array *larr = (struct list_array *)list->data;
  LIter iter;
  int *i;
  printf("<<<<---->>>>\n");
  //printf("larr->count:     %3lu\n", larr->count);
  printf("length:     %3lu\n", L_LENGTH(list));
  //printf("larr->max_count: %3lu\n", larr->max_count);
  printf("content:");
  L_FOREACH(list, &iter, i) {
    printf(" %d", *i);
  }
  /*
  for( i = 0; i < larr->count; ++i ) {
    printf(" %d", *((int *)(larr->data + list->size * i)));
    if( larr->data + list->size * i != L_GET(list, i) ) {
      printf(" ERR: index ");
    }
  }
  */
  printf("\n\n");
}

int main( int argc, char **argv) {
  List *list = NULL;

  int i, *tmp, t;

  list = L_NEW(LIST_TYPE_ARRAY, sizeof(int));
  if( !list ) {
    fprintf(stderr, "ERR: L_NEW returned NULL pointer\n");
    exit(1);
  }

  state(list);

  for( i = 0; i < 4; i++ ) {
    if( LIST_ERR == L_PUSHBACK(list, &i) ) {
      fprintf(stderr, "ERR: L_PUSHBACK returned LIST_ERR pointer\n");
      fflush(NULL);
      exit(1);
    }
  }

  state(list);

  for( ; i < 20; i++ ) {
    L_PUSHBACK(list, &i);
  }
  
  state(list);

  for( ; i < 40; i++ ) {
    L_PUSHBACK(list, &i);
  }

  tmp = (int *)L_LAST(list);
  if( tmp ) {
    printf("last: %d\n", *tmp);
  } else {
    printf("last: invalid pointer\n");
  }

  state(list);

  t = 31337;
  L_PUSHFRONT(list, &t);

  tmp = (int *)L_FIRST(list);
  if( tmp ) {
    printf("first: %d\n", *tmp);
  } else {
    printf("first: invalid pointer\n");
  }

  state(list);

  L_POPFRONT(list);

  state(list);
  
  for( ; i < 60; i++ ) {
    L_INSERT(list, i-20, &i);
  }

  state(list);

  for( ; i < 70; i++ ) {
    L_INSERT(list, 20, &i);
  }

  state(list);

  for( ; i > 60; i-- ) {
    L_REMOVE(list, 20);
  }

  state(list);

  for( ; i > 20; i-- ) {
    L_POPBACK(list);
  }

  state(list);

  for( ; i > 13; i-- ) {
    L_POPBACK(list);
  }

  state(list);

  for( ; i > 4; i-- ) {
    L_POPBACK(list);
  }

  state(list);

  printf("free ret: %d\n", L_FREE(list));
  
  return 0;
}
