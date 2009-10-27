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

#ifndef PKGBUILD_H
#define PKGBUILD_H

#include <stdio.h>

typedef struct {
	unsigned int refcount;
	char *name;
	char *version;
	float rel;
	char *desc;
	char *url;
	char **licenses;
	char *install;
	char **sources;
	char **noextract;
	char **md5sums;
	char **sha1sums;
	char **sha256sums;
	char **sha384sums;
	char **sha512sums;
	char **groups;
	char **architectures;
	char **backup;
	char **depends;
	char **makedepends;
	char **optdepends;
	char **conflicts;
	char **provides;
	char **replaces;
	char **options;
} pkgbuild_t;

pkgbuild_t *pkgbuild_new();
void pkgbuild_release(pkgbuild_t *pkgbuild);
pkgbuild_t *pkgbuild_retain(pkgbuild_t *pkgbuild);

void pkgbuild_set_name(pkgbuild_t *pkgbuild, char *name);
char *pkgbuild_name(pkgbuild_t *pkgbuild);
void pkgbuild_set_version(pkgbuild_t *pkgbuild, char *version);
char *pkgbuild_version(pkgbuild_t *pkgbuild);
void pkgbuild_set_rel(pkgbuild_t *pkgbuild, float rel);
float pkgbuild_rel(pkgbuild_t *pkgbuild);
void pkgbuild_set_desc(pkgbuild_t *pkgbuild, char *desc);
char *pkgbuild_desc(pkgbuild_t *pkgbuild);
void pkgbuild_set_url(pkgbuild_t *pkgbuild, char *url);
char *pkgbuild_url(pkgbuild_t *pkgbuild);
char **pkgbuild_licenses(pkgbuild_t *pkgbuild);
void pkgbuild_set_install(pkgbuild_t *pkgbuild, char *install);
char *pkgbuild_install(pkgbuild_t *pkgbuild);
char **pkgbuild_sources(pkgbuild_t *pkgbuild);
char **pkgbuild_noextract(pkgbuild_t *pkgbuild);
char **pkgbuild_md5sums(pkgbuild_t *pkgbuild);
char **pkgbuild_sha1sums(pkgbuild_t *pkgbuild);
char **pkgbuild_sha256sums(pkgbuild_t *pkgbuild);
char **pkgbuild_sha384sums(pkgbuild_t *pkgbuild);
char **pkgbuild_sha512sums(pkgbuild_t *pkgbuild);
char **pkgbuild_groups(pkgbuild_t *pkgbuild);
char **pkgbuild_architectures(pkgbuild_t *pkgbuild);
char **pkgbuild_backup(pkgbuild_t *pkgbuild);
char **pkgbuild_depends(pkgbuild_t *pkgbuild);
char **pkgbuild_makedepends(pkgbuild_t *pkgbuild);
char **pkgbuild_optdepends(pkgbuild_t *pkgbuild);
char **pkgbuild_conflicts(pkgbuild_t *pkgbuild);
char **pkgbuild_provides(pkgbuild_t *pkgbuild);
char **pkgbuild_replaces(pkgbuild_t *pkgbuild);
char **pkgbuild_options(pkgbuild_t *pkgbuild);

#endif
