#!/bin/bash

find ./demos/ -iname *.h -o -iname *.cpp -o -iname *.cxx | xargs clang-format -i -style='{BreakStringLiterals: false}'
find ./library/ -iname *.h -o -iname *.cpp -o -iname *.cxx | xargs clang-format -i -style='{BreakStringLiterals: false}'
find ./plugins/ -iname *.h -o -iname *.cpp -o -iname *.cxx | xargs clang-format -i -style='{BreakStringLiterals: false}'
find ./software/ -iname *.h -o -iname *.cpp -o -iname *.cxx | xargs clang-format -i -style='{BreakStringLiterals: false}'
find ./tests/ -iname *.h -o -iname *.cpp -o -iname *.cxx | xargs clang-format -i -style='{BreakStringLiterals: false}'
