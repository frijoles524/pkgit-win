/*

  pkgit - package it!

  Copyright (C) 2026 dacctal
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "fetch_git.h"

#include "lua_state.h"
#include "vars.h"

int fetch_git(Pkg pkg) {
	pid_t pid = fork();
	if (pid == 0) {
		if (!is_verbose) {
			int nullfd = open("/dev/null", O_WRONLY);
			if (nullfd >= 0) {
				dup2(nullfd, STDOUT_FILENO);
				dup2(nullfd, STDERR_FILENO);
				close(nullfd);
			}
		}
		const char *argv[12];
		int i = 0;
		argv[i++] = "git";
		argv[i++] = "-c";
		argv[i++] = "advice.detachedHead=false";
		argv[i++] = "clone";
		if (strcmp(pkg.ver, "HEAD") != 0) {
			argv[i++] = "--branch";
			argv[i++] = pkg.ver;
		}
		argv[i++] = "--recursive";
		argv[i++] = "--depth";
		argv[i++] = "1";
		argv[i++] = pkg.url;
		argv[i++] = pkg.src;
		argv[i] = NULL;
		execvp("git", (char *const *)argv);
		_exit(127);
	}

	int status;
	waitpid(pid, &status, 0);
	int result = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
	if (result != 0) {
		pid_t pid = fork();
		if (pid == 0) {
			if (!is_verbose) {
				int nullfd = open("/dev/null", O_WRONLY);
				if (nullfd >= 0) {
					dup2(nullfd, STDOUT_FILENO);
					dup2(nullfd, STDERR_FILENO);
					close(nullfd);
				}
			}
			const char *argv[12];
			int i = 0;
			argv[i++] = "git";
			argv[i++] = "-c";
			argv[i++] = "advice.detachedHead=false";
			argv[i++] = "clone";
			if (strcmp(pkg.ver, "HEAD") != 0) {
				argv[i++] = "--branch";
				argv[i++] = pkg.ver;
			}
			argv[i++] = "--recursive";
			argv[i++] = pkg.url;
			argv[i++] = pkg.src;
			argv[i] = NULL;
			execvp("git", (char *const *)argv);
			_exit(127);
		}

		int status;
		waitpid(pid, &status, 0);
		result = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
		if (result != 0) printf(
			"%s git clone failed: %d\n",
			print_warning, result
		);
	}

	return result;
}