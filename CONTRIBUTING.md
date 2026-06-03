# Basics
Do not submit any AI-generated code.

To contribute to pkgit, you should go to the xmpp muc `pkgit@muc.symlinx.net` and drop your patch file which *should* contain the changes you made.

Before submitting a patch, you MUST test the funcitonality of said patch, along with the rest of pkgit's features:
```
pkgit add [url]
pkgit build [path] # should be the root dir of a project with a valid build system
pkgit declare
pkgit find [pkgs] # should find any package from repos in your config
pkgit install [url]
pkgit install [pkgs]
pkgit remove [pkgs]
pkgit list # should list only installed packages
pkgit update # should update only installed packages
```
Also test this functionality with --quiet to make sure it doesn't print garbage to stdout.
Only after this functionality has been confirmed to work on your system should it be submitted.

If your patch makes pkgit exclusive to any system (i.e. depends on glibc, systemd, debian, nixos, etc.) it will not be accepted.

All code must be able to compile using the existing Makefile.
Please write your code to be as extensible as reasonably possible.

# Code Styling
Please follow the following styling guidelines:
- Try not to make your lines longer than 80 characters.
- All indentation should be 2 spaces long.
- Comments should only be used to explain your thought process for writing the code you wrote. Don't explain what it does.
- Variable/Function names should concisely describe what they are/do. All of which are styled in snake_case.

# How big can the patches be?
You should only make one effective change per patch.
Multiple files can be changed per patch, but only as needed to apply the one change.
Do not change two separated things in one patch.
If one change is not related to another, make two patches.
Unless the patch fundamentally changes how the user interacts with pkgit,
you should not change `README.md` in any way.

# How do I generate a patch?
To generate a patch, run the following command in the root directory of the project:
```
git format-patch HEAD~1
```
