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
#ifndef __unittest_h__
#define __unittest_h__

#define UT_SUCCESS (0)
#define UT_FAIL    (1)
#define UT_SKIP    (77)
#define UT_HARD    (99)

void ut_state(List *list);

int ut_generic(enum list_type type);
int ut_equal(enum list_type type);

#endif
