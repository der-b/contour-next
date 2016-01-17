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
#ifndef __LIST_STLINKED_H__
#define __LIST_STLINKED_H__

#include "utils/list.h"

struct list_stlinked {
  struct list_stlinked_item *first;
  struct list_stlinked_item *last;
  LIndex count;
};

struct list_stlinked_item {
  struct list_stlinked_item *prev;
  void *data;
};

struct list_stlinked_iter {
  List *list;
  LIndex i;
};

List *list_stlinked_new(LOpts *opts);

#endif

