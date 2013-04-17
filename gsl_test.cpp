#include <stdio.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_eigen.h>

void eigen(Matrix & eigen_vectors, Matrix & eigen_values)
{
   /*
   double data[] = { 1.0, 2.0, 1.0,
      6.0, -1.0, 0.0,
      -1.0, -2.0, -1.0 };
      */
   if(!(eigen_vectors.NumCols() == _rows && eigen_vectors.NumRows() == _rows))
    {
       cout << "param eigen_vectors has incorrect dimensions" << endl;
       return;
    }
    if(!(eigen_values.NumRows() == _rows && eigen_values.NumCols() == 1))
    {
       cout << "param eigen_values has incorrect dimensions" << endl;
       return;
    }

   gsl_matrix_view m 
      = gsl_matrix_view_array (_data, _rows, _rows);

   gsl_vector_complex *eval = gsl_vector_complex_alloc (_rows);
   gsl_matrix_complex *evec = gsl_matrix_complex_alloc (_rows, _rows);

   gsl_eigen_nonsymmv_workspace * w = 
      gsl_eigen_nonsymmv_alloc (_rows);

   gsl_eigen_nonsymmv (&m.matrix, eval, evec, w);

   gsl_eigen_nonsymmv_free (w);

   gsl_eigen_nonsymmv_sort (eval, evec, 
         GSL_EIGEN_SORT_ABS_DESC);

   {
      int i, j;

      for (i = 0; i < _rows; i++)
      {
         gsl_complex eval_i 
            = gsl_vector_complex_get (eval, i);
         gsl_vector_complex_view evec_i 
            = gsl_matrix_complex_column (evec, i);

         printf ("eigenvalue = %g + %gi\n",
               GSL_REAL(eval_i), GSL_IMAG(eval_i));
         printf ("eigenvector = \n");
         for (j = 0; j < _rows; ++j)
         {
            gsl_complex z = 
               gsl_vector_complex_get(&evec_i.vector, j);
            printf("%g + %gi\n", GSL_REAL(z), GSL_IMAG(z));
         }
      }
   }

   gsl_vector_complex_free(eval);
   gsl_matrix_complex_free(evec);

   return 0;
}
