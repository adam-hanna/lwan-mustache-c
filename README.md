lwan - Mustache Templating
===============

This is the mustache templating system as written for the lwan http server.

You can learn more about lwan at:
The [web site](http://lwan.ws).
The [github repo](https://github.com/lpereira/lwan).

You can learn more about lwan's mustache implementation from the link below (note, the code in his blog is a little out-dated. Please see the example folder for an up-to-date example.):
[Example from lwan blog](http://tia.mat.br/posts/2012/11/11/mustache_templates_in_c.html)

The license is [GNU GPL v2](https://www.gnu.org/licenses/gpl-2.0.txt)


##Usage
I've included both the source and compiled library files in their respective folders


###Building from source

```
$ cd source
$ gcc -Wall -c *.c
$ ar -cvq liblwanmustache.a *.o
```

###Using the Libraries

```
$ gcc -I'/location-of-the-cloned-repo/source' -o lwan-mustache lwan-mustache.c '/location-of-the-cloned-repo/compiled/liblwanmustache.a'
$ ./lwan-mustache
```

To try for yourself, navigate to the example folder and enter the above commands into the command line.

Thanks!