#include <stdio.h>
#include <math.h>

#define F_2_PI ((float)2.*M_PI)

#define BIGSIGNALFILEBIN	(char*)"bigsignal.bin"
#define BIGSIGNALFILEASCII	(char*)"bigsignal.txt"
#define CSVPLOTFILE		(char*)"plot.csv"

#define NUMELEMENTS	(1*1024*1024)

// only consider this many periods (this is enough to uncover the secret sine waves):

#define MAXPERIODS	100

// which file type to read, BINARY or ASCII (BINARY is much faster to read):

//#define ASCII
#define BINARY

float * BigSignal;
float * BigSums;

int main(int argc, char* argv[]) {
    BigSignal = new float [NUMELEMENTS];		// to hold the entire signal

	FILE *fp = fopen( BIGSIGNALFILEBIN, "rb" );
	if( fp == NULL )
	{
		fprintf( stdout, "Cannot open signal file '%s'\n", BIGSIGNALFILEBIN );
		return -1;
	}

	fread( BigSignal, sizeof(float), NUMELEMENTS, fp );

	BigSums[0] = 0.;
    for (int p = 1; p < MAXPERIODS; p++) {
        BigSums[p] = 0.;
        float omega = F_2_PI / (float)p;
        for (int t = 0; t < NUMELEMENTS; t++) {
            BigSums[p] += BigSignal[t] * sinf(omega*(float)t);
        }
    }

    fp = fopen( CSVPLOTFILE, "w" );
		if( fp == NULL )
		{
			fprintf( stdout, "Cannot write to plot file '%s'\n", CSVPLOTFILE );
		}
		else
		{
			for( int p = 0; p < MAXPERIODS; p++ )
			{
				fprintf( fp, "%6d , %10.2f\n", p, BigSums[p] );
			}
			fclose( fp );
		}
    return 0;
}
