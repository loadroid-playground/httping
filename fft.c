#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <fftw3.h>

#include "error.h"

double *pin = NULL;
fftw_complex *pout = NULL;
fftw_plan plan;
int sample_rate = 44100;

void fft_init(int sample_rate_in)
{
	sample_rate = sample_rate_in;

	pin  = (double *)malloc(sizeof(double) * sample_rate_in);
	if (!pin)
		error_exit("failed allocating memory for fft (1)");

	pout = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * sample_rate_in + 1);
	if (!pout)
		error_exit("failed allocating memory for fft (2)");

	/* init fftw */
	plan = fftw_plan_dft_r2c_1d(sample_rate_in, pin, pout, FFTW_ESTIMATE);
	if (!plan)
		error_exit("failed calculating plan for fft");
}

void fft_free(void)
{
	if (pin)
	{
		fftw_free(pout);
		free(pin);
	}
}

void fft_do(double *in, double *output_mag)
{
	int loop = 0;

	memcpy(pin, in, sizeof(double) * sample_rate);

	/* calc fft */
	fftw_execute(plan);

	for(loop=0; loop<(sample_rate / 2) + 1; loop++)
	{
		double real = pout[loop][0];
		double img = pout[loop][1];
		double mag = sqrt(pow(real, 2.0) + pow(img, 2.0));

		output_mag[loop] = mag;
	}
}
