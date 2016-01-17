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
#ifndef __list_dlinked_h__
#define __list_dlinked_h__

#include "utils/list.h"

struct list_dlinked {
  struct list_dlinked_item *first;
  struct list_dlinked_item *last;
  LIndex count;
};

struct list_dlinked_item {
  struct list_dlinked_item *next;
  struct list_dlinked_item *prev;
  void *data;
};

List *list_dlinked_new(LOpts *opts);

#endif /* __list_dlinked_h__ */
