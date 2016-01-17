#!/bin/sh

cppcheck --enable=warning,performance,portability --error-exitcode=1 --exitcode-suppressions=cppcheck.suppressions.list.txt -I ../../include ../../
