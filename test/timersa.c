#include <pbc.h>
#include "fp.h"
#include "random.h"
#include "get_time.h"

int main(void)
{
    mpz_t p, q, N, d;
    mpz_t dmp1, dmq1;
    mpz_t ipmq, iqmp;
    mpz_t adq, adp;

    field_t f;
    element_t a, b;
    double t0, t1, tnaive = 0, tcrt=0;
    int i, n;

    mpz_init(p);
    mpz_init(q);
    mpz_init(N);
    mpz_init(d);
    mpz_init(dmp1);
    mpz_init(dmq1);
    mpz_init(ipmq);
    mpz_init(iqmp);
    mpz_init(adp);
    mpz_init(adq);
    mpz_setbit(N, 513);
    mpz_setbit(N, 513);
    pbc_mpz_random(p, N);
    pbc_mpz_random(q, N);
    mpz_nextprime(p, p);
    mpz_nextprime(q, q);
    mpz_mul(N, p, q);
    mpz_invert(ipmq, p, q);
    mpz_invert(iqmp, q, p);

    field_init_fp(f, N);
    element_init(a, f);
    element_init(b, f);
    //field_print_info(stdout, f);
    n = 10;
    for (i=0; i<n; i++) {
	pbc_mpz_random(d, N);
	element_random(a);
	t0 = get_time();
	element_pow_mpz(b, a, d);
	t1 = get_time();
	tnaive += t1 - t0;

	mpz_sub_ui(p, p, 1);
	mpz_sub_ui(q, q, 1);

	mpz_mod(dmp1, d, p);
	mpz_mod(dmq1, d, q);

	mpz_add_ui(p, p, 1);
	mpz_add_ui(q, q, 1);

	element_to_mpz(adq, a);
	element_to_mpz(adp, a);

	t0 = get_time();
	mpz_powm(adp, adp, d, p);
	mpz_powm(adq, adq, d, q);
	mpz_mul(adp, adp, q);
	mpz_mul(adp, adp, iqmp);
	mpz_mul(adq, adq, p);
	mpz_mul(adq, adq, ipmq);
	mpz_add(adp, adp, adq);
	t1 = get_time();
	tcrt += t1 - t0;
	element_set_mpz(b, adp);
    }
    printf("average RSA exp time = %lf\n", tnaive / n);
    printf("average RSA exp time (CRT) = %lf\n", tcrt / n);
    return 0;
}
