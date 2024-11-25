#!/bin/bash

find ./library/ -not -name unordered_dense.h -iname *.h -o -iname *.cpp -o -iname *.cxx | xargs clang-format -i
find ./plugins/ -iname *.h -o -iname *.cpp -o -iname *.cxx | xargs clang-format -i
find ./software/ -iname *.h -o -iname *.cpp -o -iname *.cxx | xargs clang-format -i
find ./tests/ -iname *.h -o -iname *.cpp -o -iname *.cxx | xargs clang-format -i
