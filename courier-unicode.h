#ifndef	courier_unicode_h
#define	courier_unicode_h

/*
** Copyright 2000-2018 Double Precision, Inc.
** See COPYING for distribution information.
**
*/

#ifdef	__cplusplus

#include <string>
#include <vector>
#include <list>

extern "C" {
#endif

#if 0
}
#endif

#include	<stdlib.h>
#include	<stdio.h>
#include	<stdint.h>
#include	<sys/types.h>
#include	<locale.h>

#ifdef __cplusplus

#else

#if 1

#include	<uchar.h>

#else

typedef uint32_t char32_t;
#endif
#endif

#define COURIER_UNICODE_VERSION 210

/*
** The system default character set, from the locale.
*/

extern const char *unicode_default_chset();

/*
** The current locale character set.
*/

extern const char *unicode_locale_chset();

#if 1
extern const char *unicode_locale_chset_l(locale_t l);
#endif

/* Unicode upper/lower/title case conversion functions */

extern char32_t unicode_uc(char32_t);
extern char32_t unicode_lc(char32_t);
extern char32_t unicode_tc(char32_t);

/*
** Look up HTML 4.0/XHTML entity.
**
** n="amp", etc...
**
** Returns the unicode entity value, or 0 if no such entity is defined.
*/

char32_t unicode_html40ent_lookup(const char *n);

/*
**
** Return "width" of unicode character.
**
** This is defined as follows: for characters having the F or W property in
** tr11 (EastAsianWidth), unicode_wcwidth() returns 2.
**
** Otherwise, characters having the BK, CR, LF, CM, NL, WJ, and ZW line
** breaking property as per tr14, unicode_wcwdith() returns 0. For all other
** cases, 1.
**
** This provides a rough estimate of the "width" of the character if its
** shown on a text console.
*/

extern int unicode_wcwidth(char32_t c);
extern size_t unicode_wcwidth_str(const char32_t *c);

/* Internal unicode table lookup functions */

extern uint8_t unicode_tab_lookup(char32_t ch,
				  const size_t *unicode_indextab,
				  size_t unicode_indextab_sizeof,
				  const uint8_t (*unicode_rangetab)[2],
				  const uint8_t *unicode_classtab,
				  uint8_t uclass);

extern uint32_t unicode_tab32_lookup(char32_t ch,
				     const size_t *unicode_indextab,
				     size_t unicode_indextab_sizeof,
				     const uint8_t (*unicode_rangetab)[2],
				     const uint32_t *unicode_classtab,
				     uint32_t uclass);

/*
** Look up unicode categorization, see http://unicode.org/notes/tn36/
**
** Returns a 32 bit value with four unicode categories encoded in the
** bits defined by UNICODE_CATEGORY_1..4
*/

#define UNICODE_CATEGORY_1   0xFF000000
#define UNICODE_CATEGORY_2   0x00FF0000
#define UNICODE_CATEGORY_3   0x0000FF00
#define UNICODE_CATEGORY_4   0x000000FF

#include <courier-unicode-categories-tab.h>

uint32_t unicode_category_lookup(char32_t);

/*
** Return non-0 for TAB, and all UNICODE_CATEGORY_2_SPACE.
*/

extern int unicode_isblank(char32_t ch);

/*
** The unicode-ish isspace(). In addition to return non-0 for
** unicode_isblank(), this also returns non-0 for unicode characters
** with linebreaking properties of BK, CR, LF, NL, and SP.
*/
extern int unicode_isspace(char32_t ch);

/*
** Return non-0 for all UNICODE_CATEGORY_1_LETTER
*/

extern int unicode_isalpha(char32_t ch);

/*
** Return non-0 for all UNICODE_CATEGORY_1_NUMBER | UNICODE_CATEGORY_2_DIGIT,
** only (no third categories).
*/
extern int unicode_isdigit(char32_t ch);

/*
** Return non-0 for all unicode_isalpha() or unicode_isdigit().
*/

extern int unicode_isalnum(char32_t ch);

/*
** Returns non-0 for all codepoints above SPACE which are not
** unicode_isspace().
*/

extern int unicode_isgraph(char32_t ch);

/*
** Return non-0 for all UNICODE_CATEGORY_1_PUNCTUATION.
*/

extern int unicode_ispunct(char32_t ch);

/*
** Return non-0 for all unicode_isalpha() for which the character is
** equal to unicode_lc() of itself.
*/
extern int unicode_islower(char32_t ch);

/*
** Return non-0 for all unicode_isalpha() for which the character is
** equal to unicode_uc() of itself.
*/
extern int unicode_isupper(char32_t ch);

/*
** Implementation of grapheme cluster boundary rules, as per
** http://www.unicode.org/reports/tr29/tr29-27.html
** including  GB9a and GB9b.
**
** Returns non-zero if there's a grapheme break between the two referenced
** characters.
*/

int unicode_grapheme_break(char32_t a, char32_t b);

typedef enum {

#include <courier-unicode-script-tab.h>

} unicode_script_t;

/*
** Look up the unicode script property, as per
** http://www.unicode.org/reports/tr24/tr24-24.html
*/

unicode_script_t unicode_script(char32_t a);

/*
** Implementation of line break rules, as per
** http://www.unicode.org/reports/tr14/tr14-35.html
**
** Invoke unicode_lb_init() to initialize the linebreaking algorithm. The
** first parameter is a callback function that gets invoked with two
** arguments: UNICODE_LB_{MANDATORY|NONE|ALLOWED}, and a passthrough argument.
** The second parameter to unicode_lb_init() is the opaque passthrough
** pointer, that is passed as the second argument to the callback function
** with no further interpretation.
**
** unicode_lb_init() returns an opaque handle. Invoke unicode_lb_next(),
** passing the handle and one unicode character. Repeatedly invoke
** unicode_lb_next() to specify the input string for the linebreaking
** algorithm, then invoke unicode_lb_end() to finish calculating the
** linebreaking algorithm, and deallocate the opaque linebreaking handle.
**
** The callback function gets invoked once for each invocation of
** unicode_lb_next(). The contract is that before unicode_lb_end() returns,
** the callback function will get invoked the exact number of times that
** unicode_lb_next(), as long as each invocation of the callback function
** returned 0; nothing more, nothing less. The first parameter to the callback
** function will be one of the following values:
**
** UNICODE_LB_MANDATORY - a linebreak is MANDATORY before the corresponding
** character.
** UNICODE_LB_NONE - a linebreak is PROHIBITED before the corresponding
** character.
** UNICODE_LB_ALLOWED - a linebreak is OPTIONAL before the corresponding
** character (the preceding character is a space, or an equivalent).
**
** The callback function should return 0. A non-zero value indicates an
** error, which gets propagated up to the caller. The contract that the
** callback function gets invoked the same number of times that
** unicode_lb_next() gets invoked is now broken.
*/

#define UNICODE_LB_MANDATORY	-1
#define UNICODE_LB_NONE		0
#define UNICODE_LB_ALLOWED	1

struct unicode_lb_info;

typedef struct unicode_lb_info *unicode_lb_info_t;

/*
** Allocate a linebreaking handle.
*/
extern unicode_lb_info_t unicode_lb_init(int (*cb_func)(int, void *),
					 void *cb_arg);

/*
** Feed the next character through the linebreaking algorithm.
** A non-zero return code indicates that the callback function was invoked
** and it returned a non-zero return code (which is propagated as a return
** value). unicode_lb_end() must still be invoked, in this case.
**
** A zero return code indicates that if the callback function was invoked,
** it returned 0.
*/

extern int unicode_lb_next(unicode_lb_info_t i, char32_t ch);

/*
** Convenience function that invokes unicode_lb_next() with a list of
** unicode chars. Returns 0 if all invocations of unicode_lb_next() returned
** 0, or the first non-zero return value from unicode_lb_next().
*/

extern int unicode_lb_next_cnt(unicode_lb_info_t i,
			       const char32_t *chars,
			       size_t cnt);

/*
** Finish the linebreaking algorithm.
**
** A non-zero return code indicates that the callback function was invoked
** and it returned a non-zero return code (which is propagated as a return
** value).
**
** A zero return code indicates that if the callback function was invoked,
** it returned 0, and that the callback function was invoked exactly the same
** number of times that unicode_lb_next() was invoked.
**
** In all case, the linebreak handle will no longer be valid when this
** function returns.
*/

extern int unicode_lb_end(unicode_lb_info_t i);

/*
** An alternative linebreak API where the callback function receives the
** original unicode character in addition to its linebreak value.
**
** User unicode_lbc_init(), unicode_lbc_next(), and unicode_lbc_end(), whose
** semantics are the same as their _lb_ counterparts.
*/

struct unicode_lbc_info;

typedef struct unicode_lbc_info *unicode_lbc_info_t;

extern unicode_lbc_info_t unicode_lbc_init(int (*cb_func)(int, char32_t,
							  void *),
					   void *cb_arg);
extern int unicode_lbc_next(unicode_lbc_info_t i, char32_t ch);
extern int unicode_lbc_next_cnt(unicode_lbc_info_t i,
				const char32_t *chars,
				size_t cnt);
extern int unicode_lbc_end(unicode_lbc_info_t i);

/*
** Set linebreaking options.
**
** OPTIONS SUBJECT TO CHANGE.
*/

extern void unicode_lb_set_opts(unicode_lb_info_t i, int opts);

extern void unicode_lbc_set_opts(unicode_lbc_info_t i, int opts);

/*
** Tailorization of LB24: Prevent pluses, as in "C++", from breaking.
**
** Adds the following to LB24:
**
**            PR x PR
**
**            AL x PR
**
**            ID x PR
**/
#define UNICODE_LB_OPT_PRBREAK 0x0001


/*
** Tailored "/" breaking rules. This prevents breaking after the "/"
** character. And provides an exception to the "x SY" rule in LB13.
**
** Adds the following rule to LB13:
**
**            SY x EX
**
**            SY x AL
**
**            SY x ID
**
**            SP ÷ SY, which takes precedence over "x SY".
*/
#define UNICODE_LB_OPT_SYBREAK 0x0002

/*
** Tailored / breaking rules.
**
** This reclassifies U+2013 and U+2014 as class WJ, prohibiting breaks before
** and after mdash and ndash.
*/
#define UNICODE_LB_OPT_DASHWJ 0x0004

/*
** Implemention of word break rules, as per
** http://www.unicode.org/reports/tr29/tr29-27.html
**
** Invoke unicode_wb_init() to initialize the wordbreaking algorithm. The
** first parameter is a callback function that gets invoked with two
** arguments: an int flag, and a passthrough argument. The second parameter to
** unicode_wb_init() is the opaque passthrough pointer, that is passed as the
** second argument to the callback function with no further interpretation.
**
** unicode_wb_init() returns an opaque handle. Invoke unicode_wb_next(),
** passing the handle and one unicode character. Repeatedly invoke
** unicode_wb_next() to specify the input string for the wordbreaking
** algorithm, then invoke unicode_wb_end() to finish calculating the
** wordbreaking algorithm, and deallocate the opaque wordbreaking handle.
**
** The callback function gets invoked once for each invocation of
** unicode_wb_next(). The contract is that before unicode_wb_end() returns,
** the callback function will get invoked the exact number of times that
** unicode_wb_next(), as long as each invocation of the callback function
** returned 0; nothing more, nothing less. The first parameter to the callback
** function will be an int. A non-zero value indicates that there is a word
** break between this character and the preceding one.
**
** The callback function should return 0. A non-zero value indicates an
** error, which gets propagated up to the caller. The contract that the
** callback function gets invoked the same number of times that
** unicode_lb_next() gets invoked is now broken.
*/

struct unicode_wb_info;

typedef struct unicode_wb_info *unicode_wb_info_t;

/*
** Allocate a wordbreaking handle.
*/
extern unicode_wb_info_t unicode_wb_init(int (*cb_func)(int, void *),
					 void *cb_arg);

/*
** Feed the next character through the wordbreaking algorithm.
** A non-zero return code indicates that the callback function was invoked
** and it returned a non-zero return code (which is propagated as a return
** value). unicode_wb_end() must still be invoked, in this case.
**
** A zero return code indicates that if the callback function was invoked,
** it returned 0.
*/

extern int unicode_wb_next(unicode_wb_info_t i, char32_t ch);

/*
** Convenience function that invokes unicode_wb_next() with a list of
** unicode chars. Returns 0 if all invocations of unicode_wb_next() returned
** 0, or the first non-zero return value from unicode_wb_next().
*/

extern int unicode_wb_next_cnt(unicode_wb_info_t i,
			       const char32_t *chars,
			       size_t cnt);

/*
** Finish the wordbreaking algorithm.
**
** A non-zero return code indicates that the callback function was invoked
** and it returned a non-zero return code (which is propagated as a return
** value).
**
** A zero return code indicates that if the callback function was invoked,
** it returned 0, and that the callback function was invoked exactly the same
** number of times that unicode_wb_next() was invoked.
**
** In all case, the wordbreak handle will no longer be valid when this
** function returns.
*/

extern int unicode_wb_end(unicode_wb_info_t i);

/*
** Search for a word boundary.
**
** Obtain a handle by calling unicode_wbscan_init(), then invoke
** unicode_wbscan_next() to provide a unicode stream, then invoke
** unicode_wbscan_end(). unicode_wbscan_end() returns the number of unicode
** characters from the beginning of the stream until the first word boundary.
**
** You may prematurely stop calling unicode_wbscan_next() once it returns a
** non-0 value, which means that there is sufficient context to compute the
** first word boundary, and all further calls to unicode_wbscan_next() will
** be internal no-ops.
*/

struct unicode_wbscan_info;

typedef struct unicode_wbscan_info *unicode_wbscan_info_t;

unicode_wbscan_info_t unicode_wbscan_init();

int unicode_wbscan_next(unicode_wbscan_info_t i, char32_t ch);

size_t unicode_wbscan_end(unicode_wbscan_info_t i);

/*
** A buffer that holds unicode characters, and dynamically grows as needed.
*/

struct unicode_buf {
	char32_t *ptr;	/* The unicode characters */
	size_t size,		/* Buffer size */
		len,		/* How many characters in ptr are initialized */
		max;		/* Maximum size the buffer can grow to */
};

/*
** Initialize a buffer. Constructor.
*/

void unicode_buf_init(/* Initialize this structure. ptr, size, len cleared */
		      struct unicode_buf *p,

		      /*
		      ** Maximum size the buffer can grow to. (size_t)-1
		      ** means unlimited.
		      */
		      size_t max);
/*
** Like unicode_buf_init, and initialize the new buffer with the contents of
** another buffer. The maximum size of the initialized buffer is exactly the
** number of characters in the existing buffer. This copies a buffer using
** the minimum amount of heap space.
*/

#define unicode_buf_init_copy(a,b)				\
	do {							\
		unicode_buf_init((a), unicode_buf_len(b));	\
		unicode_buf_append_buf((a),(b));		\
	} while (0)

/*
** Deinitialize the buffer. Destructor. Frees memory.
*/

void unicode_buf_deinit(struct unicode_buf *p);

/*
** Official way to access the characters in the unicode buffer.
*/
#define unicode_buf_ptr(p) ((p)->ptr)

/*
** Official way of obtaining the number of characters in the unicode buffer.
*/
#define unicode_buf_len(p) ((p)->len)

/*
** Remove all existing characters from an initialized buffer. Sets len to 0.
*/

#define unicode_buf_clear(p) ((p)->len=0)

/*
** Append characters to the existing characters in the unicode buffer.
** The buffer grows, if needed. If the buffer would exceed its maximum size,
** the extra characters get truncated.
**
** Returns 0 if the characters were appended. -1 for a malloc failure.
*/

int unicode_buf_append(struct unicode_buf *p,	/* The buffer */
		       const char32_t *uc,	/* Characters to append */
		       size_t l);		/* How many of them */

/*
** Convert an iso-8859-1 char string and invoke unicode_buf_append().
*/

void unicode_buf_append_char(struct unicode_buf *dst,
			     const char *str,
			     size_t cnt);

/*
** Remove some portion of the unicode buffer
*/

void unicode_buf_remove(struct unicode_buf *p, /* The buffer */
			size_t pos, /* Offset in buffer */
			size_t cnt); /* How many to remove */

/*
** Append the contents of an existing buffer to another one.
*/

#define unicode_buf_append_buf(a,b)					\
	unicode_buf_append((a), unicode_buf_ptr(b), unicode_buf_len(b))


/*
** The equivalent of strcmp() for unicode buffers.
*/

int unicode_buf_cmp(const struct unicode_buf *a,
		    const struct unicode_buf *b);

/*
** The equivalent of unicode_buf_cmp, except that the second buffer is an
** iso-8859-1 string.
*/

int unicode_buf_cmp_str(const struct unicode_buf *p,
			const char *c,	/* iso-8859-1 string */
			size_t cl);	/* Number of chars in c */

/*
** A wrapper for iconv(3). This wrapper provides a different API for iconv(3).
** A handle gets created by unicode_convert_init().
** unicode_convert_init() receives a pointer to the output function
** which receives converted character text.
**
** The output function receives a pointer to the converted character text, and
** the number of characters in the converted text.
**
** The character text to convert gets passed, repeatedly, to
** unicode_convert(). Each call to unicode_convert() results in
** the output function being invoked, zero or more times, with the converted
** text. Finally, unicode_convert_deinit() stops the conversion and
** deallocates the conversion handle.
**
** Internal buffering takes place. unicode_convert_deinit() may result
** in the output function being called one or more times, to receive the final
** part of the converted character stream.
**
** The output function should return 0. A non-0 value causes
** unicode_convert() and/or unicode_convert_deinit() returning
** non-0.
*/

struct unicode_convert_hdr;

typedef struct unicode_convert_hdr *unicode_convert_handle_t;

/*
** unicode_convert_init() returns a non-NULL handle for the requested
** conversion, or NULL if the requested conversion is not available.
*/

unicode_convert_handle_t
unicode_convert_init(/* Convert from this chset */
		       const char *src_chset,

		       /* Convert to this chset */
		       const char *dst_chset,

		       /* The output function */

		       int (*output_func)(const char *, size_t, void *),

		       /* Passthrough arg */
		       void *convert_arg);

/*
** Repeatedly pass the character text to convert to unicode_convert().
**
** Returns non-0 if the output function returned non-0, or 0 if all invocations
** of the output function returned 0.
*/

int unicode_convert(/* The conversion handle */
		      unicode_convert_handle_t handle,

		      /* Text to convert */
		      const char *text,

		      /* Number of bytes to convert */
		      size_t cnt);

/*
** Finish character set conversion. The handle gets deallocated.
**
** May still result in one or more invocations of the output function.
** Returns non-zero if any previous invocation of the output function returned
** non-zero (this includes any invocations of the output function resulting
** from this call, or prior unicode_convert() calls), or 0 if all
** invocations of the output function returned 0.
**
** If the errptr is not NULL, *errptr is set to non-zero if there were any
** conversion errors -- if there was any text that could not be converted to
** the destination character text.
*/

int unicode_convert_deinit(unicode_convert_handle_t handle,
			     int *errptr);


/*
** Specialization: save converted character text in a buffer.
**
** Implementation: call unicode_convert_tocbuf_init() instead of
** unicode_convert_init(), then call unicode_convert() and
** unicode_convert_deinit(), as usual.
**
** If unicode_convert_deinit() returns 0, *cbufptr_ret gets initialized to a
** malloc()ed buffer, and the number of converted characters, the size of the
** malloc()ed buffer, are placed into *csize_ret arguments, that were passed
** to unicode_convert_tou_init().
**
** Note: if the converted string is an empty string, *cbufsize_ret is set to 0,
** but *cbufptr_ptr still gets initialized (to a dummy malloced buffer).
**
** The optional nullterminate places a trailing \0 character after the
** converted string (this is included in *cbufsize_ret).
*/

unicode_convert_handle_t
unicode_convert_tocbuf_init(/* Convert from this chset */
			      const char *src_chset,

			      /* Convert to this chset */
			      const char *dst_chset,

			      /* malloced buffer */
			      char **cbufptr_ret,

			      /* size of the malloced buffer */
			      size_t *cbufsize_ret,

			      /* null terminate the resulting string */
			      int nullterminate
			      );


/*
** Specialization: convert some character text to a char32_t array.
**
** This is like unicode_convert_tocbuf_init(), but converts to a char32_t
** array.
**
** The returned *ucsize_ret is initialized with the number of char32_ts,
** rather than the byte count.
**
** In all other ways, this function behaves identically to
** unicode_convert_tocbuf_init().
*/

unicode_convert_handle_t
unicode_convert_tou_init(/* Convert from this chset */
			   const char *src_chset,

			   /* malloc()ed buffer pointer, on exit. */
			   char32_t **ucptr_ret,

			   /* size of the malloc()ed buffer, upon exit */
			   size_t *ucsize_ret,

			   /* If true, terminate with U+0x0000, for convenience */
			   int nullterminate
			   );

/*
** Specialization: convert a char32_t array to some character text.
**
** This is the opposite of unicode_convert_tou_init(). Call this to
** initialize the conversion handle, then use unicode_convert_uc()
** instead of unicode_convert.
*/

unicode_convert_handle_t
unicode_convert_fromu_init(/* Convert to this chset */
			     const char *dst_chset,

			     /* malloc()ed buffer pointer, on exit. */
			     char **cbufptr_ret,

			     /* size of the malloc()ed buffer, upon exit */
			     size_t *cbufsize_ret,

			     /* If true, terminate with U+0x0000, for convenience */
			     int nullterminate
			     );

int unicode_convert_uc(/* The conversion handle */
			 unicode_convert_handle_t handle,

			 /* Text to convert */
			 const char32_t *text,

			 /* Number of bytes to convert */
			 size_t cnt);

/*
** Initialize conversion to UTF-8.
**
** This is a wrapper for unicode_convert_tocbuf_init() that specifies the
** destination charset as UTF-8.
*/

unicode_convert_handle_t
unicode_convert_tocbuf_toutf8_init(const char *src_chset,
				     char **cbufptr_ret,
				     size_t *cbufsize_ret,
				     int nullterminate);

/*
** Initialize conversion from UTF-8.
**
** This is a wrapper for unicode_convert_tocbuf_init() that specifies the
** source charset as UTF-8.
*/

unicode_convert_handle_t
unicode_convert_tocbuf_fromutf8_init(const char *dst_chset,
				       char **cbufptr_ret,
				       size_t *cbufsize_ret,
				       int nullterminate);

/*
** Convert a character string to UTF-8.
**
** Returns a malloc-ed buffer holding the UTF-8 string, or NULL if an
** error occured.
*/
char *unicode_convert_toutf8(/* Text to convert to UTF-8 */
			       const char *text,

			       /* Character set to convert to UTF-8 */
			       const char *charset,

			       /*
			       ** If non-NULL, and a non-NULL pointer is
			       ** returned, *error is set to non-zero if
			       ** a character conversion error has occured.
			       */
			       int *error);

/*
** Convert UTF-8 text to another character set.
**
** Returns a malloc-ed buffer holding the string converted to the specified
** character set, or NULL if an error occured.
*/

char *unicode_convert_fromutf8(/* A UTF-8 string */
				 const char *text,

				 /*
				 ** Convert the UTF-8 string to this character
				 ** set.
				 */

				 const char *charset,

				 /*
				 ** If non-NULL, and a non-NULL pointer is
				 ** returned, *error is set to non-zero if
				 ** a character conversion error has occured.
				 */
				 int *error);

/*
** Convert one charset to another charset, placing the result in a malloc-ed
** buffer.
**
** Returns a malloc-ed buffer holding the string converted to the specified
** character set, or NULL if an error occured.
*/

char *unicode_convert_tobuf(/* A string to convert */
			      const char *text,

			      /*
			      ** String's charset.
			      */

			      const char *charset,

			      /*
			      ** Destination charset
			      */
			      const char *dstcharset,

			      /*
			      ** If non-NULL, and a non-NULL pointer is
			      ** returned, *error is set to non-zero if
			      ** a character conversion error has occured.
			      */
			      int *error);

/*
** Convenience function: call unicode_convert_tou_init(), feed the
** character string through unicode_convert(), then call
** unicode_convert_deinit().
**
** If this function returns 0, *uc and *ucsize is set to a malloced buffer+size
** holding the unicode char array.
*/

int unicode_convert_tou_tobuf(/* Character text to convert */
				const char *text,

				/* Number of characters */
				size_t text_l,

				/* text's charset */
				const char *charset,

				/*
				** If this function returns 0, this gets
				** initialized
				*/
				char32_t **uc,

				/*
				** Size of the allocated buffer
				*/
				size_t *ucsize,

				/*
				** If not null and this function returns 0,
				** this is set to non-0 if there
				** was a conversion error (but the output
				** buffer gets still allocated and
				** initialized)
				*/
				int *err);

/*
** Convenience function: call unicode_convert_fromu_init(), feed the
** unicode_array through unicode_convert_uc(), then call
** unicode_convert_deinit().
**
** If this function returns 0, *uc and *ucsize is set to a malloced buffer+size
** holding the converted character string
*/

int unicode_convert_fromu_tobuf(/* Unicode array to convert to a char str */
				  const char32_t *utext,

				  /*
				  ** Size of the unicode array.
				  ** If this is (size_t)-1, utext is a
				  ** 0-terminated array.
				  */
				  size_t utext_l,

				  /*
				  ** Convert the unicode array to this charset.
				  */
				  const char *charset,

				  /*
				  ** If unicode_convert_fromu_tobuf()
				  ** returns 0, this is initialized to a
				  ** malloced buffer with a 0-terminated
				  ** string is kept.
				  */
				  char **c,

				  /*
				  ** Size of the initialized array, including
				  ** the 0-terminator.
				  */
				  size_t *csize,

				  /*
				  ** If unicode_convert_fromu_tobuf()
				  ** returns 0 and this is not NULL,
				  ** *err is set to non-0 if there was a
				  ** conversion error to the requested
				  ** character set.
				  */
				  int *err);

/*
** Convenience function: convert a string in a given character set
** to/from uppercase, lowercase, or something else.
**
** This is done by calling unicode_convert_tou_tobuf() first,
** applying the title_func and char_func, then using
** unicode_convert_fromu_tobuf().
**
** A NULL return indicates that the requested conversion cannot be performed.
*/

char *unicode_convert_tocase( /* String to convert */
			       const char *str,

			       /* String's character set */

			       const char *charset,

			       /*
			       ** Conversion of the first character in
			       ** str: unicode_uc, unicode_lc, or unicode_tc:
			       */

			       char32_t (*first_char_func)(char32_t),

			       /*
			       ** Conversion of the second and the remaining
			       ** character in str. If NULL, same as
			       ** first_char_func.
			       */
			       char32_t (*char_func)(char32_t));



/* Either UCS-4BE or UCS-4LE, matching the native char32_t endianness */

extern const char unicode_u_ucs4_native[];

/* Either UCS-2BE or UCS-2LE, matching the native char32_t endianness */

extern const char unicode_u_ucs2_native[];

/*
** Modified-UTF7 encoding used for IMAP folder names. Pass it for a charset
** parameter.
**
** This can be followed by a " " and up to 15 characters to be escaped in
** addition to unicode chars.
*/

#define unicode_x_imap_modutf7 "x-imap-modutf7"

/*
** EAI-capable Courier-IMAP does not use modified-UTF7, and uses UTF-8.
**
** However, to support SMAP we will still need to encode/decode some
** special characters.
**
** The characters U+0000-U+001F, and ./~:\
**
** They are encoded as a backslash followed by three octal digits.
*/

#define unicode_x_smap_modutf8 "x-smap-modutf8"

#if 0
{
#endif

#ifdef	__cplusplus
}

extern size_t unicode_wcwidth(const std::u32string &uc);

namespace unicode {

#if 0
};
#endif

/*
** Various character sets
*/
extern const char ucs_4[], ucs_2[], utf_8[], iso_8859_1[];

/*
** Interface to iconv.
**
** Subclass converted(). Invoke begin(), then operator(), repeatedly,
** then end().
**
** converted() receives the converted text.
*/

class iconvert {

	unicode_convert_handle_t handle;

 public:
	iconvert();
	virtual ~iconvert();

	/* Start conversion.
	** Returns false if the requested conversion cannot be done.
	**/

	bool begin(/* Convert from */
		   const std::string &src_chset,

		   /* Convert to */
		   const std::string &dst_chset);

	/* Feed iconv(3). Returns false if the conversion was aborted.
	 */

	bool operator()(const char *, size_t);

	bool operator()(const char32_t *, size_t);

	/*
	** Get the results here. If the subclass returns a non-0
	** value, the conversion is aborted.
	*/

	virtual int converted(const char *, size_t);

	/*
	** End of conversion.
	**
	** Returns true if all calls to converted() returned 0,
	** false if the conversion was aborted.
	**
	** errflag is set to true if there was a character that could
	** not be converted, and passed to converted().
	*/

	bool end(bool &errflag)
	{
		return end(&errflag);
	}

	bool end()
	{
		return end(NULL);
	}

	/* Convert between two different charsets */

	static std::string convert(const std::string &text,
				   const std::string &charset,
				   const std::string &dstcharset,
				   bool &errflag);

	/* Convert between two different charsets */

	static std::string convert(const std::string &text,
				   const std::string &charset,
				   const std::string &dstcharset)
	{
		bool dummy;

		return convert(text, charset, dstcharset, dummy);
	}

	/* Convert from unicode to a charset */

	static std::string convert(const std::u32string &uc,
				   const std::string &dstcharset,
				   bool &errflag);

	/* Convert from unicode to a charset */

	static std::string convert(const std::u32string &uc,
				   const std::string &dstcharset)
	{
		bool dummy;

		return convert(uc, dstcharset, dummy);
	}

	/* Convert charset to unicode */

	static bool convert(const std::string &text,
			    const std::string &charset,
			    std::u32string &uc);


	/* Convert to upper/lower/title case */

	static std::string
		convert_tocase(/* Text string */
			       const std::string &text,

			       /* Its charset */
			       const std::string &charset,

			       /* First character: unicode_uc, unicode_lc, or unicode_tc */
			       char32_t (*first_char_func)(char32_t),

			       /* If not NULL, second and subsequent chars */
			       char32_t (*char_func)(char32_t)
			       =NULL)
	{
		bool dummy;

		return convert_tocase(text, charset, dummy,
				      first_char_func,
				      char_func);
	}

	/* Convert to upper/lower/title case */

	static std::string
		convert_tocase(/* Text string */
			       const std::string &text,

			       /* Its charset */
			       const std::string &charset,

			       /* Set if there's a conversion error */
			       bool &err,

			       /* First character: unicode_uc, unicode_lc, or unicode_tc */
			       char32_t (*first_char_func)(char32_t),

			       /* If not NULL, second and subsequent chars */
			       char32_t (*char_func)(char32_t)
			       =NULL);
 private:
	bool end(bool *);

 public:
	class tou;
	class fromu;
};

/* Convert output of iconvert to char32_ts. */

class iconvert::tou : public iconvert {

 public:
	bool begin(const std::string &chset);

	virtual int converted(const char32_t *, size_t);

	using iconvert::operator();
 private:
	int converted(const char *ptr, size_t cnt);

 public:
	template<typename iter_t> class to_iter_class;

	template<typename input_iter_t,
		typename output_iter_t>
		static output_iter_t convert(input_iter_t from_iter,
					     input_iter_t to_iter,
					     const std::string &chset,
					     bool &flag,
					     output_iter_t out_iter);

	template<typename input_iter_t>
		static bool convert(input_iter_t from_iter,
				    input_iter_t to_iter,
				    const std::string &chset,
				    std::u32string &out_buf)
	{
		bool flag;

		out_buf.clear();
		std::back_insert_iterator<std::u32string >
			insert_iter(out_buf);

		convert(from_iter, to_iter, chset, flag, insert_iter);

		return flag;
	}

	static std::pair<std::u32string, bool>
		convert(const std::string &str,
			const std::string &chset);
};

/* Helper class that saves unicode output into an output iterator */

template<typename iter_t>
class iconvert::tou::to_iter_class : public iconvert::tou {

	iter_t iter;
 public:

 to_iter_class(iter_t iterValue)
	 : iter(iterValue) {}

	using tou::operator();

	operator iter_t() const { return iter; }

 private:
	int converted(const char32_t *ptr, size_t cnt)
	{
		while (cnt)
		{
			*iter=*ptr;

			++iter;
			++ptr;
			--cnt;
		}
		return 0;
	}
};

template<typename input_iter_t,
	typename output_iter_t>
	output_iter_t iconvert::tou::convert(input_iter_t from_iter,
					     input_iter_t to_iter,
					     const std::string &chset,
					     bool &flag,
					     output_iter_t out_iter)
{
	class to_iter_class<output_iter_t> out(out_iter);

	if (!out.begin(chset))
		return out;

	std::vector<char> string;

	while (from_iter != to_iter)
	{
		string.push_back(*from_iter++);

		if (string.size() > 31)
		{
			out(&string[0], string.size());
			string.clear();
		}
	}

	if (string.size() > 0)
		out(&string[0], string.size());

	out.end(flag);
	return out;
}

/* Convert output of iconvert from char32_ts. */

class iconvert::fromu : public iconvert {

 public:
	bool begin(const std::string &chset);

	using iconvert::operator();

	template<typename iter_t> class to_iter_class;

	template<typename input_iter_t,
		typename output_iter_t>
		static output_iter_t convert(input_iter_t from_iter,
					     input_iter_t to_iter,
					     const std::string &chset,
					     output_iter_t out_iter,
					     bool &errflag);

	template<typename input_iter_t>
		static void convert(input_iter_t from_iter,
				    input_iter_t to_iter,
				    const std::string &chset,
				    std::string &out_buf,
				    bool &errflag)
	{
		out_buf="";
		std::back_insert_iterator<std::string>
			insert_iter(out_buf);

		convert(from_iter, to_iter, chset, insert_iter,
			errflag);
	}

	static std::pair<std::string, bool>
		convert(const std::u32string &ubuf,
			const std::string &chset);
};

/* Helper class that saves unicode output into an output iterator */

template<typename iter_t>
class iconvert::fromu::to_iter_class : public iconvert::fromu {

	iter_t iter;
 public:

 to_iter_class(iter_t iterValue)
	 : iter(iterValue) {}

	using fromu::operator();

	operator iter_t() const { return iter; }

 private:
	int converted(const char *ptr, size_t cnt)
	{
		while (cnt)
		{
			*iter=*ptr;

			++iter;
			++ptr;
			--cnt;
		}
		return 0;
	}
};

template<typename input_iter_t,
	typename output_iter_t>
	output_iter_t iconvert::fromu::convert(input_iter_t from_iter,
					       input_iter_t to_iter,
					       const std::string &chset,
					       output_iter_t out_iter,
					       bool &errflag)
{
	errflag=true;

	class to_iter_class<output_iter_t> out(out_iter);

	if (!out.begin(chset))
		return out;

	std::u32string string;

	while (from_iter != to_iter)
	{
		string.push_back(*from_iter++);

		if (string.size() > 31)
		{
			out(&string[0], string.size());
			string.clear();
		}
	}

	if (string.size() > 0)
		out(&string[0], string.size());

	out.end(errflag);
	return out;
}

/*
** Unicode linebreaking algorithm, tr14.
*/

extern "C" int linebreak_trampoline(int value, void *ptr);
extern "C" int linebreakc_trampoline(int value, char32_t ch,
				     void *ptr);

/*
** Subclass linebreak_callback_base, implement operator()(int).
**
** Use operator<< or operator()(iterator, iterator) to feed
** char32_ts into the linebreaking algorithm. The subclass receives
** UNICODE_LB values, as they become available.
*/

class linebreak_callback_base {

	unicode_lb_info_t handle;

	int opts;

#if __cplusplus >= 201103L
 public:
	linebreak_callback_base(const linebreak_callback_base &)=delete;
	linebreak_callback_base &operator=(const
					   linebreak_callback_base &)=delete;
 private:
#else
	linebreak_callback_base(const linebreak_callback_base &);
	/* NOT IMPLEMENTED */

	linebreak_callback_base &operator=(const
					   linebreak_callback_base &);
	/* NOT IMPLEMENTED */
#endif
 public:
	linebreak_callback_base();
	virtual ~linebreak_callback_base();

	void finish();

	void set_opts(int opts);

	friend int linebreak_trampoline(int, void *);

	linebreak_callback_base &operator<<(char32_t uc);

	template<typename iter_type>
		linebreak_callback_base &operator()(iter_type beg_iter,
						    iter_type end_iter)
	{
		while (beg_iter != end_iter)
			operator<<(*beg_iter++);
		return *this;
	}

	template<typename container_type>
		linebreak_callback_base &operator()(const container_type &vec)
	{
		return operator()(vec.begin(), vec.end());
	}
 private:
	virtual int callback(int);
};

class linebreak_callback_save_buf : public linebreak_callback_base {

 public:
	std::list<int> lb_buf;

	linebreak_callback_save_buf();
	~linebreak_callback_save_buf();

	using linebreak_callback_base::operator<<;
	using linebreak_callback_base::operator();
 private:
	int callback(int value);
};

/*
** Convert an input iterator sequence over char32_ts into
** an input iterator sequence over linebreak values.
*/

template<typename input_t> class linebreak_iter
: public std::iterator<std::input_iterator_tag, int, void>
{
	mutable input_t iter_value, end_iter_value;

	mutable linebreak_callback_save_buf *buf;

	void fill() const
	{
		if (buf == NULL)
			return;

		while (buf->lb_buf.empty())
		{
			if (iter_value == end_iter_value)
			{
				buf->finish();
				if (buf->lb_buf.empty())
				{
					delete buf;
					buf=NULL;
				}
				break;
			}

			buf->operator<<(*iter_value++);
		}
	}

	mutable value_type bufvalue;

 public:
 linebreak_iter(const input_t &iter_valueArg,
		const input_t &iter_endvalueArg)
	 : iter_value(iter_valueArg),
		end_iter_value(iter_endvalueArg),
		buf(new linebreak_callback_save_buf)
		{
		}

 linebreak_iter() : buf(NULL)
	{
	}

	void set_opts(int opts)
	{
		if (buf)
			buf->set_opts(opts);
	}

	~linebreak_iter()
	{
		if (buf)
			delete buf;
	}

 linebreak_iter(const linebreak_iter<input_t> &v)
	 : buf(NULL)
	{
		operator=(v);
	}

	linebreak_iter<input_t> &operator=(const
					   linebreak_iter<input_t> &v)
		{
			if (buf)
				delete buf;
			buf=v.buf;
			iter_value=v.iter_value;
			end_iter_value=v.end_iter_value;
			v.buf=NULL;
			return *this;
		}

	bool operator==(const linebreak_iter<input_t> &v) const
	{
		fill();
		v.fill();

		return buf == NULL && v.buf == NULL;
	}

	bool operator!=(const linebreak_iter<input_t> &v) const
	{
		return !operator==(v);
	}

	value_type operator*() const
	{
		fill();
		return buf == NULL ? UNICODE_LB_MANDATORY:
			buf->lb_buf.front();
	}

	linebreak_iter<input_t> &operator++()
	{
		bufvalue=operator*();

		if (buf)
			buf->lb_buf.pop_front();
		return *this;
	}

	const value_type *operator++(int)
	{
		operator++();
		return &bufvalue;
	}
};

/*
** Like linebreak_callback_base, except the subclass receives both
** the linebreaking value, and the unicode character.
*/

class linebreakc_callback_base {

	unicode_lbc_info_t handle;

	int opts;

#if __cplusplus >= 201103L
 public:
	linebreakc_callback_base(const linebreakc_callback_base &)
		=delete;

	linebreakc_callback_base &operator=(const
					    linebreakc_callback_base
					    &)=delete;
 private:
#else
	linebreakc_callback_base(const linebreakc_callback_base &);
	/* NOT IMPLEMENTED */

	linebreakc_callback_base &operator=(const
					    linebreakc_callback_base
					    &);
	/* NOT IMPLEMENTED */
#endif

 public:
	linebreakc_callback_base();
	virtual ~linebreakc_callback_base();

	void finish();

	void set_opts(int opts);

	friend int linebreakc_trampoline(int, char32_t, void *);

	linebreakc_callback_base &operator<<(char32_t uc);

	template<typename iter_type>
		linebreakc_callback_base &operator()(iter_type beg_iter,
						     iter_type end_iter)
	{
		while (beg_iter != end_iter)
			operator<<(*beg_iter++);
		return *this;
	}

	linebreakc_callback_base &operator<<(const
					     std::u32string
					     &vec)
	{
		return operator()(vec.begin(), vec.end());
	}
 private:
	virtual int callback(int, char32_t);
};

class linebreakc_callback_save_buf : public linebreakc_callback_base {

 public:
	std::list<std::pair<int, char32_t> > lb_buf;

	linebreakc_callback_save_buf();
	~linebreakc_callback_save_buf();

	using linebreakc_callback_base::operator<<;
	using linebreakc_callback_base::operator();
 private:
	int callback(int, char32_t);
};


/*
** Convert an input iterator sequence over char32_ts into
** an input iterator sequence over std::pair<int, char32_t>,
** the original unicode character, and the linebreaking value before
** the character.
*/

template<typename input_t> class linebreakc_iter
: public std::iterator<std::input_iterator_tag,
	std::pair<int, char32_t>, void>
{
	mutable input_t iter_value, end_iter_value;

	mutable linebreakc_callback_save_buf *buf;

	void fill() const
	{
		if (buf == NULL)
			return;

		while (buf->lb_buf.empty())
		{
			if (iter_value == end_iter_value)
			{
				buf->finish();
				if (buf->lb_buf.empty())
				{
					delete buf;
					buf=NULL;
				}
				break;
			}

			buf->operator<<(*iter_value);
			++iter_value;
		}
	}

	mutable value_type bufvalue;

 public:
 linebreakc_iter(const input_t &iter_valueArg,
		 const input_t &iter_endvalueArg)
	 : iter_value(iter_valueArg),
		end_iter_value(iter_endvalueArg),
		buf(new linebreakc_callback_save_buf)
		{
		}

 linebreakc_iter() : buf(NULL)
	{
	}

	~linebreakc_iter()
	{
		if (buf)
			delete buf;
	}

 linebreakc_iter(const linebreakc_iter<input_t> &v)
	 : buf(NULL)
	{
		operator=(v);
	}

	linebreakc_iter<input_t> &operator=(const
					    linebreakc_iter<input_t> &v)
		{
			if (buf)
				delete buf;
			buf=v.buf;
			iter_value=v.iter_value;
			end_iter_value=v.end_iter_value;
			v.buf=NULL;
			return *this;
		}

	bool operator==(const linebreakc_iter<input_t> &v) const
	{
		fill();
		v.fill();

		return buf == NULL && v.buf == NULL;
	}

	bool operator!=(const linebreakc_iter<input_t> &v) const
	{
		return !operator==(v);
	}

	value_type operator*() const
	{
		fill();
		return buf == NULL ?
			std::make_pair(UNICODE_LB_MANDATORY,
				       (char32_t)0):
			buf->lb_buf.front();
	}

	linebreakc_iter<input_t> &operator++()
	{
		bufvalue=operator*();

		if (buf)
			buf->lb_buf.pop_front();
		return *this;
	}

	const value_type *operator++(int)
	{
		operator++();
		return &bufvalue;
	}
};


/*
** Subclass wordbreak_callback_base, implement operator()(int).
**
** Use operator<< or operator()(iterator, iterator) to feed
** char32_ts into the wordbreaking algorithm. The subclass receives
** word flags, as they become available.
*/

extern "C" int wordbreak_trampoline(int value, void *ptr);

class wordbreak_callback_base {

	unicode_wb_info_t handle;

#if __cplusplus >= 201103L
 public:
	wordbreak_callback_base(const wordbreak_callback_base &)=delete;

	wordbreak_callback_base &operator=(const
					   wordbreak_callback_base &)
		=delete;
 private:
#else
	wordbreak_callback_base(const wordbreak_callback_base &);
	/* NOT IMPLEMENTED */

	wordbreak_callback_base &operator=(const
					   wordbreak_callback_base &);
	/* NOT IMPLEMENTED */
#endif
 public:
	wordbreak_callback_base();
	virtual ~wordbreak_callback_base();

	void finish();

	friend int wordbreak_trampoline(int, void *);

	wordbreak_callback_base &operator<<(char32_t uc);

	template<typename iter_type>
		wordbreak_callback_base &operator()(iter_type beg_iter,
						    iter_type end_iter)
	{
		while (beg_iter != end_iter)
			operator<<(*beg_iter++);
		return *this;
	}

	wordbreak_callback_base &operator<<(const
					    std::u32string
					    &vec)
	{
		return operator()(vec.begin(), vec.end());
	}
 private:
	virtual int callback(bool);
};

/*
** A C++ wrapper for unicode_wbscan.
*/

class wordbreakscan {

	unicode_wbscan_info_t handle;

#if __cplusplus >= 201103L
 public:
	wordbreakscan(const wordbreakscan &)=delete;
	wordbreakscan &operator=(const wordbreakscan &)=delete;
 private:
#else
	wordbreakscan(const wordbreakscan &);
	/* NOT IMPLEMENTED */

	wordbreakscan &operator=(const wordbreakscan &);
	/* NOT IMPLEMENTED */
#endif
 public:

	wordbreakscan();
	~wordbreakscan();

	bool operator<<(char32_t uc);

	size_t finish();
};

//! Convert string in unicode_default_chset() to lowercase

std::string tolower(const std::string &string);

//! Convert string in unicode_default_chset() to uppercase

std::string toupper(const std::string &string);

//! Convert string in the given character set to lowercase

std::string tolower(const std::string &string,
		    const std::string &charset);

//! Convert string in the given character set to uppercase

std::string toupper(const std::string &string,
		    const std::string &charset);

//! Convert unicode to lowercase

std::u32string tolower(const std::u32string &u);

//! Convert unicode to uppercase

std::u32string toupper(const std::u32string &u);

#if 0
{
#endif
}
#endif

#endif
