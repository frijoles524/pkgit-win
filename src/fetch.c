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
#include <sys/wait.h>
#include <unistd.h>

#include "fetch.h"
#include "files.h"
#include "globs.h"
#include "log.h"
#include "str.h"
#include "win.h"

bool fetch(package_t *pkg) {
	if (pkg->url.data == NULL || pkg->src.data == NULL) {
		if (flags.verbose)
			log_warn("invalid pkg: url or src is NULL");
		return false;
	}

	str s = str_dupe(&pkg->src);
	s.len -= pkg->version.len + 1;
	s.data[s.len] = 0;

	if (is_directory(s.data) && flags.force) {
		if (remove_tree(s.data)) {
			log_error("could not remove %.*s", str_fmt(&s));
			return false;
		}
	}

	str_free(&s);

	pid_t pid = fork();
	if (pid < 0) {
		log_error("fork failed");
		return false;
	}

	if (pid == 0) {
		if (!flags.verbose) {
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
		if (pkg->version.data != NULL &&
			!str_equal_cstr(&pkg->version, "HEAD")) {
			argv[i++] = "--branch";
			argv[i++] = pkg->version.data;
		}

		argv[i++] = "--recursive";
		argv[i++] = pkg->url.data;
		#ifdef __CYGWIN__ // this is unreadable as fuck I know
		char *win_path = to_windows_path(pkg->src.data);
		if (win_path == NULL) {
			log_error("path conversion failed");
			_exit(127);
		}
		argv[i++] = win_path;
		#else
		argv[i++] = pkg->src.data;
		#endif
		argv[i] = NULL;
		execvp("git", (char *const *)argv);
		_exit(127);
	}
	int status;
	waitpid(pid, &status, 0);

	bool result = (WEXITSTATUS(status) == 0);
	if (!result) {
		log_error("git clone failed");
	}

	return result;
}
