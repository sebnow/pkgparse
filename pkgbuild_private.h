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

#ifndef PKGBUILD_PRIVATE_H
#define PKGBUILD_PRIVATE_H

struct _pkgbuild_t {
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
};

pkgbuild_t *pkgbuild_new();

void pkgbuild_set_name(struct _pkgbuild_t *pkgbuild, char *name);
void pkgbuild_set_version(struct _pkgbuild_t *pkgbuild, char *version);
void pkgbuild_set_rel(struct _pkgbuild_t *pkgbuild, float rel);
void pkgbuild_set_desc(struct _pkgbuild_t *pkgbuild, char *desc);
void pkgbuild_set_url(struct _pkgbuild_t *pkgbuild, char *url);
void pkgbuild_set_licenses(pkgbuild_t *pkgbuild, char **licenses);
void pkgbuild_set_install(struct _pkgbuild_t *pkgbuild, char *install);
void pkgbuild_set_sources(pkgbuild_t *pkgbuild, char **sources);
void pkgbuild_set_noextract(pkgbuild_t *pkgbuild, char **noextract);
void pkgbuild_set_md5sums(pkgbuild_t *pkgbuild, char **md5sums);
void pkgbuild_set_sha1sums(pkgbuild_t *pkgbuild, char **sha1sums);
void pkgbuild_set_sha256sums(pkgbuild_t *pkgbuild, char **sha256sums);
void pkgbuild_set_sha384sums(pkgbuild_t *pkgbuild, char **sha384sums);
void pkgbuild_set_sha512sums(pkgbuild_t *pkgbuild, char **sha512sums);
void pkgbuild_set_groups(pkgbuild_t *pkgbuild, char **groups);
void pkgbuild_set_architectures(pkgbuild_t *pkgbuild, char **architectures);
void pkgbuild_set_backup(pkgbuild_t *pkgbuild, char **backup);
void pkgbuild_set_makedepends(pkgbuild_t *pkgbuild, char **makedepends);
void pkgbuild_set_optdepends(pkgbuild_t *pkgbuild, char **optdepends);
void pkgbuild_set_conflicts(pkgbuild_t *pkgbuild, char **conflicts);
void pkgbuild_set_provides(pkgbuild_t *pkgbuild, char **provides);
void pkgbuild_set_replaces(pkgbuild_t *pkgbuild, char **replaces);
void pkgbuild_set_options(pkgbuild_t *pkgbuild, char **options);

#endif
