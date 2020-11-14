#ifndef __STATS_H
#define __STATS_H

#include <stddef.h>
#include <stdint.h>

#define STATS_LOWER (0)
#define STATS_UPPER (1)

double stats_mean(size_t n, const double x[n]);
double stats_var(size_t n, const double x[n]);
double stats_sd(size_t n, const double x[n]);
double stats_diff_mean(size_t n, const double x[n], const double y[n]);
double stats_diff_var(size_t n, const double x[n], const double y[n]);
double stats_diff_sd(size_t n, const double x[n], const double y[n]);
double stats_cov(size_t n, const double x[n], const double y[n]);
void stats_shuffle(void *vec, size_t nmemb, size_t size);
int stats_pearson_corr(size_t n, const double x[n], const double y[n],
		       double *rho, double *pvalue);
void stats_mat_cov(size_t m, size_t n, double cov[m][m], const double x[m][n]);

double stats_unif_std_rand();
double stats_unif_rand(double min, double max);
double stats_unif_std_F(double x);
double stats_unif_F(double x, double min, double max);

size_t stats_categorical_rand(size_t n, double cumul_p[n]);

double stats_norm_std_rand();
double stats_norm_rand(double mu, double sig);
double stats_norm_std_F(double x);
double stats_norm_F(double x, double mu, double sig);

double stats_student_rand(double df);
double stats_student_F(double x, double df);

double stats_gamma_rand(double alpha, double beta);
double stats_gamma_rand_k_theta(double k, double theta);
double stats_gamma_F(double x, double alpha, double beta);
void stats_gamma_fit_mm(size_t n, const double x[n], double *alpha, double *beta);

double stats_beta_rand(double alpha, double beta);
double stats_beta_F(double x, double alpha, double beta);
void stats_beta_fit_mm(size_t n, const double x[n], double *alpha, double *beta);
void stats_beta_fit(size_t n, const double x[n], double *alpha, double *beta);

double stats_binom_f(long k, long n, double p);
double stats_binom_F(long k, long n, double p);

double stats_poisson_f(long k, double lambda);
double stats_poisson_F(long k, double lambda);
unsigned long stats_poisson_rand_its(double lambda);
unsigned long stats_poisson_rand_pa(double lambda);
unsigned long stats_poisson_rand(double lambda);

double stats_hyper_f(long k, long K, long n, long N);
double stats_hyper_tail(long k, long K, long n, long N, int upper);
double stats_hyper_F(long k, long K, long n, long N);

long double stats_hyper_fl(long k, long K, long n, long N);
long double stats_hyper_taill(long k, long K, long n, long N, int upper);
long double stats_hyper_Fl(long k, long K, long n, long N);


struct stats_t_test {
  double pvalue, df, t;
};
double stats_t_test(size_t n, const double x[n], double mu, int H0,
		    struct stats_t_test *data);
double stats_t_test_paired(size_t n, const double x[n], const double y[n],
			   double mu, int H0,
			   struct stats_t_test *data);
double stats_t_test_welch(size_t nx, const double x[nx], size_t ny, const double y[ny],
			  double mu, int H0,
			  struct stats_t_test *data);

double stats_rss(size_t m, size_t n, const double y[m], const double x[m][n],
		 double (*predict)(const double x[n], void *cls), void *cls);
int stats_rsquared(size_t m, size_t n, const double y[m], const double x[m][n],
		   double (*predict)(const double x[n], void *cls), void *cls,
		   double *rsquared);


int stats_p_adjust_fdr_bh(size_t n,  const double p[n], double padj[n]);
int stats_p_adjust_fwer_bonf(size_t n,  const double p[n], double padj[n]);

int stats_cross_validation();


#endif
