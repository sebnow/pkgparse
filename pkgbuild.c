/* Copyright (c) 2009 Sebastian Nowicki <sebnow@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdlib.h>
#include <string.h>

#include "pkgbuild.h"

static void _free_array(char **array)
{
	char **ptr = array;
	if(ptr != NULL) {
		while(*ptr != NULL) {
			free(*ptr);
			ptr++;
		}
		free(array);
	}
}

/**
* @brief Free a pkgbuild_t structure
*
* \c pkgbuild must not be NULL.
*
* @param pkgbuild the pkgbuild_t object to be freed
*/
static void _pkgbuild_free(pkgbuild_t *pkgbuild)
{
	free(pkgbuild->name);
	free(pkgbuild->version);
	free(pkgbuild->desc);
	free(pkgbuild->url);
	_free_array(pkgbuild->licenses);
	free(pkgbuild->install);
	_free_array(pkgbuild->sources);
	_free_array(pkgbuild->noextract);
	_free_array(pkgbuild->md5sums);
	_free_array(pkgbuild->sha1sums);
	_free_array(pkgbuild->sha256sums);
	_free_array(pkgbuild->sha384sums);
	_free_array(pkgbuild->sha512sums);
	_free_array(pkgbuild->groups);
	_free_array(pkgbuild->architectures);
	_free_array(pkgbuild->backup);
	_free_array(pkgbuild->depends);
	_free_array(pkgbuild->makedepends);
	_free_array(pkgbuild->optdepends);
	_free_array(pkgbuild->conflicts);
	_free_array(pkgbuild->provides);
	_free_array(pkgbuild->replaces);
	_free_array(pkgbuild->options);
	free(pkgbuild);
}

pkgbuild_t *pkgbuild_new()
{
	pkgbuild_t *pkgbuild;
	pkgbuild = malloc(sizeof(*pkgbuild));
	pkgbuild = memset(pkgbuild, 0, sizeof(*pkgbuild));
	return pkgbuild_retain(pkgbuild);
}

void pkgbuild_release(pkgbuild_t *pkgbuild)
{
	if(pkgbuild != NULL) {
		pkgbuild->refcount--;
		if(pkgbuild->refcount == 0) {
			_pkgbuild_free(pkgbuild);
		}
	}
}

pkgbuild_t *pkgbuild_retain(pkgbuild_t *pkgbuild)
{
	if(pkgbuild != NULL) {
		pkgbuild->refcount++;
	}
	return pkgbuild;
}

void pkgbuild_set_name(pkgbuild_t *pkgbuild, char *name)
{
	if(pkgbuild == NULL) {
		return;
	}
	free(pkgbuild->name);
	pkgbuild->name = strdup(name);
}

char *pkgbuild_name(pkgbuild_t *pkgbuild) {
	char *name;
	if(pkgbuild != NULL) {
		name = pkgbuild->name;
	}
	return name;
}

void pkgbuild_set_version(pkgbuild_t *pkgbuild, char *version)
{
	if(pkgbuild != NULL) {
		free(pkgbuild->version);
		pkgbuild->version = strdup(version);
	}
}

char *pkgbuild_version(pkgbuild_t *pkgbuild) {
	char *version;
	if(pkgbuild != NULL) {
		version = pkgbuild->version;
	}
	return version;
}

void pkgbuild_set_rel(pkgbuild_t *pkgbuild, float rel)
{
	if(pkgbuild != NULL) {
		pkgbuild->rel = rel;
	}
}

float pkgbuild_rel(pkgbuild_t *pkgbuild) {
	float rel = 0;
	if(pkgbuild != NULL) {
		rel = pkgbuild->rel;
	}
	return rel;
}

void pkgbuild_set_desc(pkgbuild_t *pkgbuild, char *desc)
{
	if(pkgbuild != NULL) {
		free(pkgbuild->desc);
		pkgbuild->desc = strdup(desc);
	}
}

char *pkgbuild_desc(pkgbuild_t *pkgbuild) {
	char *desc;
	if(pkgbuild != NULL) {
		desc = pkgbuild->desc;
	}
	return desc;
}

void pkgbuild_set_url(pkgbuild_t *pkgbuild, char *url)
{
	if(pkgbuild != NULL) {
		free(pkgbuild->url);
		pkgbuild->url = strdup(url);
	}
}

char *pkgbuild_url(pkgbuild_t *pkgbuild) {
	char *url;
	if(pkgbuild != NULL) {
		url = pkgbuild->url;
	}
	return url;
}

void pkgbuild_set_install(pkgbuild_t *pkgbuild, char *install)
{
	if(pkgbuild != NULL) {
		free(pkgbuild->install);
		pkgbuild->install = strdup(install);
	}
}

char *pkgbuild_install(pkgbuild_t *pkgbuild) {
	char *install;
	if(pkgbuild != NULL) {
		install = pkgbuild->install;
	}
	return install;
}

char **pkgbuild_licenses(pkgbuild_t *pkgbuild)
{
	char **licenses = NULL;
	if(pkgbuild != NULL) {
		licenses = pkgbuild->licenses;
	}
	return licenses;
}

char **pkgbuild_sources(pkgbuild_t *pkgbuild)
{
	char **sources = NULL;
	if(pkgbuild != NULL) {
		sources = pkgbuild->sources;
	}
	return sources;
}

char **pkgbuild_noextract(pkgbuild_t *pkgbuild)
{
	char **noextract = NULL;
	if(pkgbuild != NULL) {
		noextract = pkgbuild->noextract;
	}
	return noextract;
}

char **pkgbuild_md5sums(pkgbuild_t *pkgbuild)
{
	char **md5sums = NULL;
	if(pkgbuild != NULL) {
		md5sums = pkgbuild->md5sums;
	}
	return md5sums;
}

char **pkgbuild_sha1sums(pkgbuild_t *pkgbuild)
{
	char **sha1sums = NULL;
	if(pkgbuild != NULL) {
		sha1sums = pkgbuild->sha1sums;
	}
	return sha1sums;
}

char **pkgbuild_sha256sums(pkgbuild_t *pkgbuild)
{
	char **sha256sums = NULL;
	if(pkgbuild != NULL) {
		sha256sums = pkgbuild->sha256sums;
	}
	return sha256sums;
}

char **pkgbuild_sha384sums(pkgbuild_t *pkgbuild)
{
	char **sha384sums = NULL;
	if(pkgbuild != NULL) {
		sha384sums = pkgbuild->sha384sums;
	}
	return sha384sums;
}

char **pkgbuild_sha512sums(pkgbuild_t *pkgbuild)
{
	char **sha512sums = NULL;
	if(pkgbuild != NULL) {
		sha512sums = pkgbuild->sha512sums;
	}
	return sha512sums;
}

char **pkgbuild_groups(pkgbuild_t *pkgbuild)
{
	char **groups = NULL;
	if(pkgbuild != NULL) {
		groups = pkgbuild->groups;
	}
	return groups;
}

char **pkgbuild_architectures(pkgbuild_t *pkgbuild)
{
	char **architectures = NULL;
	if(pkgbuild != NULL) {
		architectures = pkgbuild->architectures;
	}
	return architectures;
}

char **pkgbuild_backup(pkgbuild_t *pkgbuild)
{
	char **backup = NULL;
	if(pkgbuild != NULL) {
		backup = pkgbuild->backup;
	}
	return backup;
}

char **pkgbuild_depends(pkgbuild_t *pkgbuild)
{
	char **depends = NULL;
	if(pkgbuild != NULL) {
		depends = pkgbuild->depends;
	}
	return depends;
}

char **pkgbuild_makedepends(pkgbuild_t *pkgbuild)
{
	char **makedepends = NULL;
	if(pkgbuild != NULL) {
		makedepends = pkgbuild->makedepends;
	}
	return makedepends;
}

char **pkgbuild_optdepends(pkgbuild_t *pkgbuild)
{
	char **optdepends = NULL;
	if(pkgbuild != NULL) {
		optdepends = pkgbuild->optdepends;
	}
	return optdepends;
}

char **pkgbuild_conflicts(pkgbuild_t *pkgbuild)
{
	char **conflicts = NULL;
	if(pkgbuild != NULL) {
		conflicts = pkgbuild->conflicts;
	}
	return conflicts;
}

char **pkgbuild_provides(pkgbuild_t *pkgbuild)
{
	char **provides = NULL;
	if(pkgbuild != NULL) {
		provides = pkgbuild->provides;
	}
	return provides;
}

char **pkgbuild_replaces(pkgbuild_t *pkgbuild)
{
	char **replaces = NULL;
	if(pkgbuild != NULL) {
		replaces = pkgbuild->replaces;
	}
	return replaces;
}

char **pkgbuild_options(pkgbuild_t *pkgbuild)
{
	char **options = NULL;
	if(pkgbuild != NULL) {
		options = pkgbuild->options;
	}
	return options;
}


