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

/* File: pkgparse.h
A collection functions and data structures relating to the retrieval of
PKGBUILD metadata. The main data structure is <pkgbuild_t>, which
encapsulates all PKGBUILD information. This data structure is populated by
parsing a PKGBUILD using the <pkgbuild_parse()> function.

Example:
	(start code)
	FILE *fp;
	pkgbuild_t *pkgbuild;

	// Open the PKGBUILD for reading.
	fp = fopen("PKGBUILD", "r");
	if(fp == NULL) {
		fprintf(stderr, "Unable to open PKGBUILD\n");
		exit(1);
	}

	// Attempt to parse the PKGBUILD.
	pkgbuild = pkgbuild_parse(fp);
	if(pkgbuild == NULL) {
		fprintf(stderr, "Unable to parse PKGBUILD\n");
	}

	// Print package name, version and release.
	printf("Parsed: %s %s-%.1f\n", pkgbuild_names(pkgbuild)[0],
		pkgbuild_version(pkgbuild),
		pkgbuild_release(pkgbuild));
	pkgbuild_release(pkgbuild);
	(end)
*/

#include <stdio.h>

/* Type: pkgbuild_t
An opaque data type encapsulating PKGBUILD metadata.
*/
typedef struct _pkgbuild_t pkgbuild_t;

/* Function: pkgbuild_parse
Initialize and return a pkgbuild_t structure by parsing a PKGBUILD file.

Parameters:
	fp - A file pointer to the PKGBUILD. The file must be opened in read
       mode, and closed, by the caller.

Returns:
	An initialized pkgbuild_t structure containing metadata found in the
       PKGBUILD. This object must be deallocated using <pkgbuild_release()>.
*/
pkgbuild_t *pkgbuild_parse(FILE *fp);

/* Function: pkgbuild_release
Decrement the pkgbuild's reference count.

The object is deallocated automatically when its reference count reaches 0.
The associated value, if set, will also be deallocated.

Parameters:
	pkgbuild - A reference to the symbol to be released.

See Also:
	<pkgbuild_retain()>
*/
void pkgbuild_release(pkgbuild_t *pkgbuild);

/* Function: pkgbuild_retain
Increment the pkgbuild's reference count. This should be used whenever you
want to prevent it from being deallocated without your express permission.

The pkgbuild is deallocated automatically when its reference count reaches 0.
Calling <pkgbuild_retain()> increments the reference count, and calling
<pkgbuild_release()> decrements it.

Example:
	(start code)
	void use_pkgbuild(pkgbuild_t *pkgbuild)
	{
	    pkgbuild_retain(pkgbuild);
	    // Do something with pkgbuild
	    pkgbuild_release(pkgbuild);
	}
	(end)

Parameters:
	pkgbuild - A reference to the symbol to be retained.

Returns:
	A reference to the pkgbuild.

See Also:
	<pkgbuild_release()>
*/
pkgbuild_t *pkgbuild_retain(pkgbuild_t *pkgbuild);

/* Function: pkgbuild_names
Retrieve all names of this package. PKGBUILDs producing split packages will
have multiple elements, while monolithic PKGBUILDs will have a single
element.

Parameters:
	pkgbuild - The pkgbuild to query.

Returns:
	An array of package names, or NULL on error.
*/
char **pkgbuild_names(pkgbuild_t *pkgbuild);

/* Function: pkgbuild_basename
Retrieve the basename of a split package. If not a split package, the
returned value will be NULL.

Parameters:
	pkgbuild - The pkgbuild to query.

Returns:
	A string representing the basename, or NULL if not a split package,
       or on error.
*/
char *pkgbuild_basename(pkgbuild_t *pkgbuild);

/* Function: pkgbuild_version
Retrieve the version of a package.

Parameters:
	pkgbuild - The pkgbuild to query.

Returns:
	A string representing the version, or NULL on error.
*/
char *pkgbuild_version(pkgbuild_t *pkgbuild);

/* Function: pkgbuild_release
Retrieve the release of a package.

Parameters:
	pkgbuild - The pkgbuild to query.

Returns:
	A float representing the release, or NULL on error.
*/
float pkgbuild_rel(pkgbuild_t *pkgbuild);

/* Function: pkgbuild_desc
Retrieve the description of a package.

Parameters:
	pkgbuild - The pkgbuild to query.

Returns:
	A string representing the description, or NULL on error.
*/
char *pkgbuild_desc(pkgbuild_t *pkgbuild);

/* Function: pkgbuild_url
Retrieve the url of a package.

Parameters:
	pkgbuild - The pkgbuild to query.

Returns:
	A string representing the url, or NULL on error.
*/
char *pkgbuild_url(pkgbuild_t *pkgbuild);

/* Function: pkgbuild_licenses
Retrieve the licenses of a package.

Parameters:
	pkgbuild - The pkgbuild to query.

Returns:
	An array of strings, each representing a license, or NULL on error.
*/
char **pkgbuild_licenses(pkgbuild_t *pkgbuild);

/* Function: pkgbuild_install
Retrieve the relative path of the install scriptlet.

Parameters:
	pkgbuild - The pkgbuild to query.

Returns:
	A string representing the path to the install scriptlet, or NULL on
	error.
*/
char *pkgbuild_install(pkgbuild_t *pkgbuild);

/* Function: pkgbuild_sources
Retrieve the sources of a package.

Parameters:
	pkgbuild - The pkgbuild to query.

Returns:
	An array of strings, each representing a source, or NULL on error.
*/
char **pkgbuild_sources(pkgbuild_t *pkgbuild);

/* Function: pkgbuild_noextract
Retrieve an array of sources not to be extracted.

Parameters:
	pkgbuild - The pkgbuild to query.

Returns:
	An array of strings, each representing a source not to be extracted, or
	NULL on error. These values should match those in sources.

See Also:
	<pkgbuild_sources()>
*/
char **pkgbuild_noextract(pkgbuild_t *pkgbuild);

/* Function: pkgbuild_md5sums
Retrieve the md5 checksums of a package.

Parameters:
	pkgbuild - The pkgbuild to query.

Returns:
	An array of strings, each representing an md5 checksum, or NULL on error.
*/
char **pkgbuild_md5sums(pkgbuild_t *pkgbuild);

/* Function: pkgbuild_sha1sums
Retrieve the sha1 checksums of a package.

Parameters:
	pkgbuild - The pkgbuild to query.

Returns:
	An array of strings, each representing an sha1 checksum, or NULL on error.
*/
char **pkgbuild_sha1sums(pkgbuild_t *pkgbuild);

/* Function: pkgbuild_sha256sums
Retrieve the sha256 checksums of a package.

Parameters:
	pkgbuild - The pkgbuild to query.

Returns:
	An array of strings, each representing an sha256 checksum, or NULL on error.
*/
char **pkgbuild_sha256sums(pkgbuild_t *pkgbuild);

/* Function: pkgbuild_sha384sums
Retrieve the sha384sums checksums of a package.

Parameters:
	pkgbuild - The pkgbuild to query.

Returns:
	An array of strings, each representing an sha384 checksum, or NULL on error.
*/
char **pkgbuild_sha384sums(pkgbuild_t *pkgbuild);

/* Function: pkgbuild_sha512sums
Retrieve the sha512sums checksums of a package.

Parameters:
	pkgbuild - The pkgbuild to query.

Returns:
	An array of strings, each representing an sha512 checksum, or NULL on error.
*/
char **pkgbuild_sha512sums(pkgbuild_t *pkgbuild);

/* Function: pkgbuild_groups
Retrieve the groups a package belongs to.

Parameters:
	pkgbuild - The pkgbuild to query.

Returns:
	An array of strings, each representing a group, or NULL on error.
*/
char **pkgbuild_groups(pkgbuild_t *pkgbuild);

/* Function: pkgbuild_architectures
Retrieve the architectures the package can be built for.

Parameters:
	pkgbuild - The pkgbuild to query.

Returns:
	An array of strings, each representing an architecture, or NULL on error.
*/
char **pkgbuild_architectures(pkgbuild_t *pkgbuild);

/* Function: pkgbuild_backup
Retrieve the paths to be backed up when installing/updating the package.

Parameters:
	pkgbuild - The pkgbuild to query.

Returns:
	An array of strings, each representing a backup path, or NULL on error.
*/
char **pkgbuild_backup(pkgbuild_t *pkgbuild);

/* Function: pkgbuild_depends
Retrieve the dependencies of a package.

Parameters:
	pkgbuild - The pkgbuild to query.

Returns:
	An array of strings, each representing a dependency, or NULL on error. These
	dependencies should be other package names, or provisions.
*/
char **pkgbuild_depends(pkgbuild_t *pkgbuild);

/* Function: pkgbuild_makedepends
Retrieve the make-time dependencies of a package.

Parameters:
	pkgbuild - The pkgbuild to query.

Returns:
	An array of strings, each representing a dependency, or NULL on error.

See Also:
	<pkgbuild_depends()>
*/
char **pkgbuild_makedepends(pkgbuild_t *pkgbuild);

/* Function: pkgbuild_optdepends
Retrieve the optional dependencies of a package.

Parameters:
	pkgbuild - The pkgbuild to query.

Returns:
	An array of strings, each representing an optional dependency, or NULL on
	error.

See Also:
	PKGBUILD(5) for information about the format of optional dependencies.
*/
char **pkgbuild_optdepends(pkgbuild_t *pkgbuild);

/* Function: pkgbuild_conflicts
Retrieve the packages a package conflicts with.

Parameters:
	pkgbuild - The pkgbuild to query.

Returns:
	An array of strings, each representing a conflicting package, or NULL on
	error.
*/
char **pkgbuild_conflicts(pkgbuild_t *pkgbuild);

/* Function: pkgbuild_provides
Retrieve the provisions of a package.

Parameters:
	pkgbuild - The pkgbuild to query.

Returns:
	An array of strings, each representing a provision, or NULL on error.

See Also:
	PKGBUILD(5) for information on what a provision is.
*/
char **pkgbuild_provides(pkgbuild_t *pkgbuild);

/* Function: pkgbuild_replaces
Retrieve the packages a package replaces.

Parameters:
	pkgbuild - The pkgbuild to query.

Returns:
	An array of strings, each representing a replaced package, or NULL on
	error.
*/
char **pkgbuild_replaces(pkgbuild_t *pkgbuild);

/* Function: pkgbuild_options
Retrieve the build options of a package.

Parameters:
	pkgbuild - The pkgbuild to query.

Returns:
	An array of strings, each representing an option, or NULL on error.

See Also:
	PKGBUILD(5) for possible options.
*/
char **pkgbuild_options(pkgbuild_t *pkgbuild);

/* Function: pkgbuild_splitpkgs
Retrieve the split packages of a package.

Parameters:
	pkgbuild - The pkgbuild to query.

Returns:
	An array of package objects, each representing a split package, or NULL on
	error.
*/
pkgbuild_t **pkgbuild_splitpkgs(pkgbuild_t *pkgbuild);

#endif
