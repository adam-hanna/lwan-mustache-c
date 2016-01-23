#include <stdio.h>
#include "strbuf.h"
#include "lwan-template.h"

/* slightly modified from http://tia.mat.br/posts/2012/11/11/mustache_templates_in_c.html */
struct hello_t {
  char *name;
  int age;
};

int
main(int argc, char **argv)
{
	/*
	 * The TPL_VAR_??? macros provides some convenience to declare each
	 * descriptor. These expand to a declaration containing the name of
	 * the variable as a string (used to validate the template during
	 * compile time), the field offset, and pointers to functions that
	 * convert the values to string and check if they're empty.
	 *
	 * The SENTINEL type is there so the template compiler knows when to
	 * stop looking for descriptors, since of course you can have as
	 many
	 * fields as necessary.
	 */
	lwan_var_descriptor_t hello_descriptor[] = {
	  TPL_VAR_STR(struct hello_t, name),
	  TPL_VAR_INT(struct hello_t, age),
	  TPL_VAR_SENTINEL
	};
	lwan_tpl_t *hello;
	strbuf_t *rendered;

	/*
	 * ``hello'' would usually be compiled once and kept around for
	 * the whole duration of the program.
	 */
	hello = lwan_tpl_compile_file("templates/hello.tpl", hello_descriptor);

	/*
	 * Rendering the template then would be just the matter of calling
	 * this function, which will output a ``strbuf_t''. The template
	 * compiler estimates the starting size of this string buffer, so
	 * rendering will incur in very few expensive reallocations, if
	 * there are reallocations at all.
	 */
	rendered = lwan_tpl_apply(hello, (struct hello_t[]) {{
	  .name = "World",
	  .age = 42
	}});

	printf("%s\n", strbuf_get_buffer(rendered));

	return 0;
};