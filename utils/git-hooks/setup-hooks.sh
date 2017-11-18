#!/bin/bash

set -e

ROOT=$(git rev-parse --show-toplevel)

cp $ROOT/utils/git-hooks/pre-commit-clang-format $ROOT/.git/hooks/pre-commit-clang-format
chmod +x $ROOT/.git/hooks/pre-commit-clang-format

cp $ROOT/utils/git-hooks/pre-commit $ROOT/.git/hooks/pre-commit
chmod +x $ROOT/.git/hooks/pre-commit
