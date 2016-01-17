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
#ifndef __list_array_h__
#define __list_array_h__

#include "utils/list.h"

#define _LIST_ARRAY_MINSIZE 16

struct list_array_iter {
  LIndex pos;
  List *list;
};

struct list_array {
  LIndex count;
  LIndex max_count;
  void *data;
};

List *list_array_new(LOpts *opts);

#endif
