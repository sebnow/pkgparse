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
#include "pkgbuild_private.h"

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

static void _free_splitpkgs(pkgbuild_t **splitpkgs)
{
	int i;
	if(splitpkgs != NULL) {
		for(i = 0; splitpkgs[i] != NULL; i++) {
			pkgbuild_release(splitpkgs[i]);
		}
		free(splitpkgs);
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
	free(pkgbuild->basename);
	_free_array(pkgbuild->names);
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
	_free_splitpkgs(pkgbuild->splitpkgs);
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

void pkgbuild_set_rel(struct _pkgbuild_t *pkgbuild, float rel)
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

void pkgbuild_set_basename(struct _pkgbuild_t *pkgbuild, char *basename)
{
	if(pkgbuild != NULL) {
		pkgbuild->basename = strdup(basename);
	}
}

char *pkgbuild_basename(pkgbuild_t *pkgbuild) {
	char *basename = NULL;
	if(pkgbuild != NULL) {
		if(pkgbuild->basename != NULL) {
			basename = pkgbuild->basename;
		} else {
			basename = pkgbuild->names[0];
		}
	}
	return basename;
}

pkgbuild_t **pkgbuild_splitpkgs(pkgbuild_t *pkgbuild)
{
	pkgbuild_t **splitpkgs = NULL;
	if(pkgbuild != NULL) {
		splitpkgs = pkgbuild->splitpkgs;
	}
	return splitpkgs;
}

void pkgbuild_set_splitpkgs(struct _pkgbuild_t *pkgbuild, pkgbuild_t **splitpkgs)
{
	if(pkgbuild != NULL) {
		pkgbuild->splitpkgs = splitpkgs;
	}
}

/* Some preprocessing magic to get rid of code duplication. The macros below
 * will define setter and getter functions for fields in a structure. */

#define MK_STRING_GETTER(object, field) \
char *object ## _ ## field(object ## _t *object) \
{ \
	char *field; \
	if(object != NULL) { \
		field = object->field; \
	} \
	return field; \
}

#define MK_STRING_SETTER(object, field) \
void object ## _set_ ## field(struct _ ## object ## _t *object, char *field) \
{ \
	if(object == NULL) { \
		return; \
	} \
	free(object->field); \
	object->field = strdup(field); \
}

#define MK_ARRAY_GETTER(object, field) \
char **object ## _ ## field(object ## _t *object) \
{ \
	char **field = NULL; \
	if(object != NULL) { \
		field = object->field; \
	} \
	return field; \
}

#define MK_ARRAY_SETTER(object, field) \
void object ## _set_ ## field(struct _ ## object ## _t *object, char **field) \
{ \
	int i; \
	if(object != NULL) { \
		if(field == NULL) { \
			object->field = NULL; \
		} else { \
			/* Count elements in field */ \
			for(i = 0; field[i] != NULL; i++); \
			object->field = malloc((i + 1) * sizeof(*field)); \
			object->field[i] = NULL; \
			for(i = 0; field[i] != NULL; i++) { \
				object->field[i] = strdup(field[i]); \
			} \
		} \
	} \
}

/* While we're at it, why not make them "properties"? */
#define MK_STRING_PROPERTY(object, field) \
	MK_STRING_SETTER(object, field) \
	MK_STRING_GETTER(object, field)
#define MK_ARRAY_PROPERTY(object, field) \
	MK_ARRAY_SETTER(object, field) \
	MK_ARRAY_GETTER(object, field)


MK_ARRAY_PROPERTY(pkgbuild, names)
MK_STRING_PROPERTY(pkgbuild, version)
MK_STRING_PROPERTY(pkgbuild, desc)
MK_STRING_PROPERTY(pkgbuild, url)
MK_ARRAY_PROPERTY(pkgbuild, licenses)
MK_STRING_PROPERTY(pkgbuild, install)
MK_ARRAY_PROPERTY(pkgbuild, sources)
MK_ARRAY_PROPERTY(pkgbuild, noextract)
MK_ARRAY_PROPERTY(pkgbuild, md5sums)
MK_ARRAY_PROPERTY(pkgbuild, sha1sums)
MK_ARRAY_PROPERTY(pkgbuild, sha256sums)
MK_ARRAY_PROPERTY(pkgbuild, sha384sums)
MK_ARRAY_PROPERTY(pkgbuild, sha512sums)
MK_ARRAY_PROPERTY(pkgbuild, groups)
MK_ARRAY_PROPERTY(pkgbuild, architectures)
MK_ARRAY_PROPERTY(pkgbuild, backup)
MK_ARRAY_PROPERTY(pkgbuild, depends)
MK_ARRAY_PROPERTY(pkgbuild, makedepends)
MK_ARRAY_PROPERTY(pkgbuild, optdepends)
MK_ARRAY_PROPERTY(pkgbuild, conflicts)
MK_ARRAY_PROPERTY(pkgbuild, provides)
MK_ARRAY_PROPERTY(pkgbuild, replaces)
MK_ARRAY_PROPERTY(pkgbuild, options)
