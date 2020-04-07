#include "ale/matrix.h"
#include "ale/stringutils.h"


int
matrix_init(struct matrix *mat)
{
  return matrix_init_size(mat, MATRIX_DEFAULT_SIZE);
}

int
matrix_init_size(struct matrix *mat, size_t size)
{
  mat->m = mat->n = 0;

  if (size > 0)
    {
      mat->data = malloc(size * sizeof(double));
      ERROR_UNDEF_FATAL(NULL == mat->data,
                        "matrix_init_size() unable to allocate memory!\n");
    }

  mat->alloc_size_double = size;
  return 0;
}

void
matrix_destroy(struct matrix *mat)
{
  free(mat->data);
}

int
matrix_read_full(struct matrix *mat, FILE *file, struct matrix_parameters *params)
{
  struct string str;
  size_t nrows = 0, ncols = 0, nlines = 0;
 
  string_init(&str);
  
  for ( ; 0 != feof(file) ; nlines++ )
    {
      int ret = string_readline(&str, file);
      ERROR_GOTO(-1 == ret, ERROR);
      char *current = str.str;
      size_t count = 0;
    
      string_chomp(&str);
      
      if (0 == str.len)
	{
	  ERROR_CUSTOM_GOTO(0 == (MATRIX_FSKIPEMPTY & params->flags), MATRIX_EEMPTY, ERROR);
	  continue;
	}
      
      if (params->comment == str.str[0])
	continue;

      if ( 0 != (MATRIX_FHEADER & params->flags) && 0 == ncols )
	{
	  ncols = 1;
	  
	  for ( ; *current ; current++ )
	    if ( params->sep == *current )
	      ncols++;

	  if ( 0 != (MATRIX_FROWNAMES & params->flags) &&
	       0 ==  (MATRIX_FRHEADER & params->flags) )
	    ncols--;

	  continue;
	}

      if ( 0 != (MATRIX_FROWNAMES & params->flags) )
	{
	  for ( ; *current && params->sep != *current ; current++ ) ;

	  if (*current)
	    current++;
	}

      for ( count = 0 ; *current ; current++, count++ )
	{
	  errno = 0;
	  double val = strtod(current, &current);
	  ERROR_ERRNO_GOTO(0 != errno, ERROR);
	  ERROR_CUSTOM_GOTO(params->sep != *current &&
			    '\0' != *current, MATRIX_ECHAR, ERROR);

	  matrix_resize(mat, mat->alloc_size_double+1);
	  mat->data[nrows * ncols + count] = val;
	}

      if ( 0 == count )
	ncols = count;

      if ( count != ncols )
	{
	  ERROR_CUSTOM_MSG_FMT(1, MATRIX_ECOLS,
			       "Invalid number of columns line %zu: %zu != %zu\n",
			       nlines, count, ncols);
	  ERROR_CUSTOM_GOTO(1, MATRIX_ECOLS, ERROR);
	}

      nrows++;
    }

  mat->m = nrows;
  mat->n = ncols;
  
  string_destroy(&str);

  return 0;

 ERROR:
  string_destroy(&str);
  return -1;
}


