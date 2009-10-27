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

#include "pkgbuild.h"

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

	assert_string_equal(pkgbuild_name(pkgbuild), "foobar");
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
