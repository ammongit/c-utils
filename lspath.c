#define _XOPEN_SOURCE	500

#include <sys/types.h>
#include <dirent.h>
#include <limits.h>
#include <regex.h>
#include <unistd.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *argv0;
static int ret;

static struct {
	regex_t *array;
	size_t len;
} patterns;

static struct {
	char *str;
	const char **array;
	size_t len;
} paths;

/* Utilities */
static void cleanup(void)
{
	size_t i;

	for (i = 0; i < patterns.len; i++)
		regfree(&patterns.array[i]);
	free(patterns.array);
	free(paths.array);
	exit(ret);
}

static void warn(const char *message)
{
	if (errno) {
		fprintf(stderr,
			"%s: %s: %s\n",
			argv0,
			message,
			strerror(errno));
	} else {
		fprintf(stderr,
			"%s: %s\n",
			argv0,
			message);
	}
	ret = 1;
}

static void error(const char *message)
{
	warn(message);
	cleanup();
}

/* Functions */
static void split_path(void)
{
	const char *var, *ptr;
	size_t i, j;

	/* Get buffer */
	var = getenv("PATH");
	if (!var)
		cleanup();

	paths.str = strdup(var);
	if (!paths.str)
		error("strdup() failed");

	/* Count items */
	paths.len = 0;
	for (i = 0; paths.str[i]; i++) {
		if (paths.str[i] == ':')
			paths.len++;
	}

	/* Prepare paths struct */
	paths.array = malloc(paths.len * sizeof(const char *));
	if (!paths.array)
		error("malloc() failed");

	/* Split path variable */
	ptr = paths.str, j = 0;
	for (i = 0; paths.str[i]; i++) {
		if (paths.str[i] == ':') {
			paths.str[i] = '\0';
			paths.array[j++] = ptr;
			ptr = paths.str + i + 1;
		}
	}
}

static int is_dotdot(const char *name)
{
	if (name[0] == '.') {
		switch (name[1]) {
		case '\0':
			return 1;
		case '.':
			return name[2] == '\0';
		}
	}
	return 0;
}

static int path_matches(const char *name)
{
	return 1;
}

static void iterate_paths(void)
{
	size_t i;

	for (i = 0; i < paths.len; i++) {
		DIR *dh;
		const struct dirent *dirent;
		char file[PATH_MAX];
		size_t off;

		dh = opendir(paths.array[i]);
		if (!dh) {
			warn(paths.array[i]);
			continue;
		}

		off = strlen(paths.array[i]);
		memcpy(file, paths.array[i], off);
		file[off++] = '/';

		while ((dirent = readdir(dh)) != NULL) {
			if (is_dotdot(dirent->d_name))
				continue;

			strncpy(file + off,
				dirent->d_name,
				sizeof(file) - off);
			if (access(file, X_OK)) {
				warn(file);
				continue;
			}
			if (!path_matches(dirent->d_name))
				continue;
			puts(file);
		}
		closedir(dh);
	}
}

int main(int argc, char *argv[])
{
	struct {
		int regexflags;
	} opt;
	int i, ch;

	/* Parse options */
	argv0 = argv[0];
	opt.regexflags = REG_NOSUB | REG_NEWLINE;
	while ((ch = getopt(argc, argv, ":Ei")) != -1) {
		switch (ch) {
		case 'E':
			opt.regexflags |= REG_EXTENDED;
			break;
		case 'i':
			opt.regexflags |= REG_ICASE;
			break;
		case '?':
			return 1;
		default:
			abort();
		}
	}

	/* Prepare regular expressions */
	patterns.len = (size_t)(argc - optind);
	patterns.array = malloc(patterns.len * sizeof(regex_t));
	if (!patterns.array)
		error("malloc() failed");
	for (i = optind; i < argc; i++) {
		regex_t *re;
		int err;

		re = &patterns.array[i];
		err = regcomp(re, argv[i], opt.regexflags);
		if (err) {
			char errbuf[4096];

			regerror(err, re, errbuf, sizeof(errbuf));
			error(errbuf);
		}
	}

	/* Main execution */
	split_path();
	iterate_paths();
	return ret;
}