#include <stdio.h>
#include "../source/strbuf.h"
#include "../source/lwan-template.h"

/* slightly modified from http://tia.mat.br/posts/2012/11/11/mustache_templates_in_c.html */
/* single struct example */
struct hello_t {
  char *name;
  int age;
};

/* sequence example */
// an individual toDo struct
struct toDo_t
{
	char *note;
};

// and array to hold all of our toDo items
struct toDo_t toDos[3];

struct toDos_list {
	struct {
		lwan_tpl_list_generator_t generator;
		char *note;
	} toDos_list;
};

int toDos_list_generator(coro_t *coro)
{
	struct toDos_list *c = coro_get_data(coro);

	for ( int i = 0; i < 3; i++ ) {
		c->toDos_list.note = toDos[i].note;
		coro_yield(coro, 1);
	}

	return 0;
}

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

	/* single example */
	lwan_var_descriptor_t hello_descriptor[] = {
	  TPL_VAR_STR(struct hello_t, name),
	  TPL_VAR_INT(struct hello_t, age),
	  TPL_VAR_SENTINEL
	};
	lwan_tpl_t *hello;
	strbuf_t *rendered;

	/* sequence example*/
	lwan_var_descriptor_t toDo_descriptor[] = {
		TPL_VAR_INT(struct toDos_list, toDos_list.note),
		TPL_VAR_SENTINEL
	};
	lwan_var_descriptor_t toDos_descriptor[] = {
		TPL_VAR_SEQUENCE(struct toDos_list, toDos_list, toDos_list_generator, toDo_descriptor),
		TPL_VAR_SENTINEL
	};

	lwan_tpl_t *toDo_templ;
	strbuf_t *rendered_Sequence;



	/*
	 * ``hello'' would usually be compiled once and kept around for
	 * the whole duration of the program.
	 */
	
	/* single example */
	hello = lwan_tpl_compile_file("templates/hello.tpl", hello_descriptor);
	
	/* sequence example */
	/*toDo_templ = lwan_tpl_compile_file("templates/sequence.tpl", toDos_descriptor);*/
	toDo_templ = lwan_tpl_compile_string("Things to do: {{#toDos_list}}\t{{toDos_list.note}}\t{{/toDos_list}}", toDos_descriptor);



	/*
	 * Rendering the template then would be just the matter of calling
	 * this function, which will output a ``strbuf_t''. The template
	 * compiler estimates the starting size of this string buffer, so
	 * rendering will incur in very few expensive reallocations, if
	 * there are reallocations at all.
	 */
	/* Single example */
	rendered = lwan_tpl_apply(hello, (struct hello_t[]) {{
	  .name = "World",
	  .age = 42
	}});

	printf("%s\n", strbuf_get_buffer(rendered));

	/* sequence example */
	toDos[0].note = "Stoofs0";
	toDos[1].note = "Stoofs1";
	toDos[2].note = "Stoofs2";
	
	rendered_Sequence = lwan_tpl_apply(toDo_templ, &toDos);

	printf("%s\n", strbuf_get_buffer(rendered_Sequence));


	lwan_tpl_free(hello);
    strbuf_free(rendered);

    lwan_tpl_free(toDo_templ);
    strbuf_free(rendered_Sequence);

	return 0;
};