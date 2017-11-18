# git-hooks

Some usefull git hooks that should be used by Tulip contributors.

Currently, there is only a pre-commit hook that enables to run [clang-format](https://clang.llvm.org/docs/ClangFormat.html)
on any Tulip C/C++ source files to commit in order to get a consistent format in the whole codebase.

To install that hook, issue the following command:
```
$ bash setup-hooks.sh
```
