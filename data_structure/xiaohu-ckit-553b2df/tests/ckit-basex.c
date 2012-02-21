/* CKit: Data structure and arithmetic implement
 * ckit-basex: base64/32/16 encode and decode tool
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "ckit/ckit.h"
#include <getopt.h>

static bool_t opt_base16 = false;
static bool_t opt_base32 = false;
static bool_t opt_base64 = true;
static bool_t opt_encode = true;
static bool_t opt_withsize = false;

static struct option longopts[] =
{
    { "16", no_argument,  NULL, '1' },
    { "32", no_argument,  NULL, '3' },
    { "64", no_argument,  NULL, '6' },
    { "deocde", no_argument,  NULL, 'd' },
    { "encode", no_argument,  NULL, 'e' },
    { "size", no_argument,  NULL, 's' },
    { "help", no_argument,  NULL, 'h' },
    { "version", no_argument,  NULL, 'v' },
};


static void
usage (void)
{
  printf ("Usage : ckit-basex [options] [file]\n");
  printf ("Options:\n");
  printf ("  -3,-32:       uses base32\n");
  printf ("  -1,-16:       uses base16\n");
  printf ("  -6,-64:       uses base64, this is default\n");
  printf ("  -e,--encode:  encode, this is default\n");
  printf ("  -d,--decode:  decode\n");
  printf ("  -s,--size:    print result size\n");
  printf ("  -h,--help:    show help information\n");
  printf ("  -v,--version: show version information\n");
  printf ("\nFILE is optional, if missing, will be read from STDIN\n");
}


static void
parse_opts (int argc, char *argv[])
{
  int ch;

  while ((ch = getopt_long (argc, argv, "136deshv", longopts, NULL)) != -1)
    {
      switch (ch)
	{
	case 'd':
	  opt_encode = false;
	  break;
	case 'e':
	  opt_encode = true;
	  break;
	case '1':
	  opt_base16 = true;
	  opt_base32 = false;
	  opt_base64 = false;
	  break;
	case '3':
	  opt_base16 = false;
	  opt_base32 = true;
	  opt_base64 = false;
	  break;
	case '6':
	  opt_base16 = false;
	  opt_base32 = false;
	  opt_base64 = true;
	  break;
	case 's':
	  opt_withsize = true;
	  break;
	case 'h':
	  usage ();
	  exit (0);
	case 'v':
	  printf ("%s\n", PACKAGE_STRING);
	  exit (0);
	default:
	  ckit_info ("type `ckit-basex --help' for help.");
	  exit (1);
	}
    }
}


static inline size_t
encode (const char *buff, size_t size)
{
  char *result;
  size_t result_size;

  if (opt_base16)
    result = ckit_base16_encode (buff, size);
  else if (opt_base32)
    result = ckit_base32_encode (buff, size);
  else
    result = ckit_base64_encode (buff, size, CKIT_BASE64_CHARSET_DEFAULT);

  if (!result)
    return 0;

  result_size = strlen (result);
  fwrite (result, 1, result_size, stdout);
  free (result);

  return result_size;
}


static inline size_t
decode (const char *buff, size_t size)
{
  uchar_t *result;
  size_t result_size;

  if (opt_base16)
    result = ckit_base16_decode (buff, size, &result_size);
  else if (opt_base32)
    result = ckit_base32_decode (buff, size, &result_size);
  else
    result = ckit_base64_decode (buff, size,
				 CKIT_BASE64_CHARSET_DEFAULT,
				 &result_size);

  if (!result)
    return 0;

  fwrite (result, 1, result_size, stdout);
  free (result);

  return result_size;
}


int
main (int argc, char *argv[])
{
  FILE *infp = stdin;
  bool_t final = false;
  char buff[30 * 1024];
  size_t total_size = 0;

  parse_opts (argc, argv);

  argc -= optind;
  argv += optind;

  if (argc > 0)
    {
      infp = fopen (argv[0], "r");
      if (!infp)
	{
	  ckit_warn ("fopen(\"%s\"): %s", argv[0], strerror (errno));
	  return 1;
	}
    }
  while (!final)
    {
      size_t nread = fread (buff, 1, sizeof (buff), infp);
      if (nread < sizeof (buff))
	{
	  if (ferror (infp))
	    {
	      ckit_warn ("fread(): %s", strerror (errno));
	      break;
	    }
	  final = true;
	}
      if (opt_encode)
	total_size += encode (buff, nread);
      else
	total_size += decode (buff, nread);
    }
  if (infp != stdin)
    fclose (infp);

  if (opt_withsize)
    printf ("%zd\n", total_size);

  return 0;
}
