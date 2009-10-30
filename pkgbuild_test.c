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

#include "cmockery.h"
#include <stdlib.h>
#include <stdio.h>

#include "pkgparse.h"

void test_parse_pkgbuild_minimal(void **state)
{
	FILE *fp;
	pkgbuild_t *pkgbuild;

	fp = tmpfile();
	fprintf(fp,
		"pkgname=foobar\n"
		"pkgver=1.0\n"
		"pkgrel=1\n"
		"pkgdesc=\"dummy package\"\n");
	fseek(fp, 0, SEEK_SET);
	pkgbuild = pkgbuild_parse(fp);
	fclose(fp);

	assert_string_equal(pkgbuild_names(pkgbuild)[0], "foobar");
	assert_string_equal(pkgbuild_version(pkgbuild), "1.0");
	assert_true(pkgbuild_rel(pkgbuild) == 1);
	assert_string_equal(pkgbuild_desc(pkgbuild), "dummy package");
	pkgbuild_release(pkgbuild);
}

void test_parse_pkgbuild_arrays(void **state)
{
	FILE *fp;
	pkgbuild_t *pkgbuild;
	char **array;

	fp = tmpfile();
	fprintf(fp,
		"license=('MIT')\n"
		"arch=('i686' 'x86_64')\n");
	fseek(fp, 0, SEEK_SET);
	pkgbuild = pkgbuild_parse(fp);
	fclose(fp);

	array = pkgbuild_licenses(pkgbuild);
	assert_true(array != NULL);
	assert_string_equal(array[0], "MIT");
	array = pkgbuild_architectures(pkgbuild);
	assert_true(array != NULL);
	assert_string_equal(array[0], "i686");
	assert_string_equal(array[1], "x86_64");
	pkgbuild_release(pkgbuild);
}

void test_parse_pkgbuild_simple(void **state)
{
	FILE *fp;
	pkgbuild_t *pkgbuild;
	char **array;

	fp = tmpfile();
	fprintf(fp,
		"pkgname=patch\n"
		"pkgver=2.5.4\n"
		"pkgrel=3\n"
		"pkgdesc=\"A utility to apply patch files to original sources\"\n"
		"arch=('i686' 'x86_64')\n"
		"url=\"http://www.gnu.org/software/$pkgname/$pkgname.html\"\n"
		"license=('GPL')\n"
		"groups=('base-devel')\n"
		"depends=('glibc' 'ed')\n"
		"source=(ftp://ftp.gnu.org/gnu/$pkgname/$pkgname-$pkgver.tar.gz)\n"
		"md5sums=('ee5ae84d115f051d87fcaaef3b4ae782')\n");
	fseek(fp, 0, SEEK_SET);
	pkgbuild = pkgbuild_parse(fp);
	fclose(fp);

	assert_string_equal(pkgbuild_names(pkgbuild)[0], "patch");
	assert_string_equal(pkgbuild_version(pkgbuild), "2.5.4");
	assert_true(pkgbuild_rel(pkgbuild) == 3.0f);
	array = pkgbuild_architectures(pkgbuild);
	assert_string_equal(array[0], "i686");
	assert_string_equal(array[1], "x86_64");
	assert_string_equal(pkgbuild_url(pkgbuild),
		"http://www.gnu.org/software/patch/patch.html");
	array = pkgbuild_licenses(pkgbuild);
	assert_string_equal(array[0], "GPL");
	array = pkgbuild_sources(pkgbuild);
	assert_string_equal(array[0],
		"ftp://ftp.gnu.org/gnu/patch/patch-2.5.4.tar.gz");

	pkgbuild_release(pkgbuild);
}

void test_parse_pkgbuild_splitpkg(void **state)
{
	FILE *fp;
	pkgbuild_t *pkgbuild;
	pkgbuild_t **splitpkgs;
	char **names;
	int i;

	fp = tmpfile();
	fprintf(fp,
		"pkgbase=foo\n"
		"pkgname=(foo bar)\n"
		"pkgver=1\n"
		"pkgdesc=\"A foo\"\n"
		"arch=('i686' 'x86_64')\n"
		"license=('GPL')\n"
		"package_foo() {\n"
		"    pkgdesc=\"some foo\"\n"
		"}\n"
		"package_bar() {\n"
		"    pkgver=0.7\n"
		"    pkgdesc=\"a bar\"\n"
		"}\n");
	fseek(fp, 0, SEEK_SET);
	pkgbuild = pkgbuild_parse(fp);
	fclose(fp);

	splitpkgs = pkgbuild_splitpkgs(pkgbuild);

	assert_true(splitpkgs != NULL);
	assert_true(splitpkgs[0] != NULL);
	assert_true(splitpkgs[1] != NULL);
	assert_string_equal(pkgbuild_basename(pkgbuild), "foo");
	assert_string_equal(pkgbuild_names(pkgbuild)[0], "foo");
	assert_string_equal(pkgbuild_names(pkgbuild)[1], "bar");
	assert_string_equal(pkgbuild_desc(splitpkgs[0]), "some foo");
	assert_string_equal(pkgbuild_desc(splitpkgs[1]), "a bar");

	pkgbuild_release(pkgbuild);
}
