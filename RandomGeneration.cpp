#include <iostream>
#include <math.h>
#include <time.h>
#include <fstream>
#include <stdlib.h>
using namespace std;

#define INT_MAX 32767
extern long seed;
void randomGeneration( FILE* in, int clauseNumber, int valueNumber,
					   int clauseLength )
{
	int i, iClauseLength, j;
	int x;
	bool symbol;
	int shortCount, longCount;

	fprintf( in, "p cnf %d %d\n", valueNumber, clauseNumber );

	shortCount = 0;
	longCount = 0;

	srand( (unsigned)time( NULL ) );
	//srand( seed );

	for( i = 0; i < clauseNumber; i++ )
	{
		iClauseLength = floor( rand()%INT_MAX*1.0/INT_MAX *clauseLength + 0.5 );
		while( iClauseLength == 0
			|| ( ( iClauseLength < 3 ) && ( shortCount > clauseNumber/80 ) )
			|| ( ( iClauseLength < clauseLength/2 ) && ( longCount > clauseNumber/70 ) ) )
			iClauseLength = floor( rand()%INT_MAX*1.0/INT_MAX *clauseLength + 0.5 );

		if( iClauseLength < 3 )
			shortCount++;
		else if( iClauseLength < clauseLength/2 )
			longCount++;

		for( j = 0; j < iClauseLength; j++ )
		{
			symbol = ( rand()%INT_MAX*1.0/INT_MAX > 0.5 ) ? true : false ;
			x = floor( rand()%INT_MAX*1.0/INT_MAX *valueNumber + 0.5 );
			while( x == 0 )
				x = floor( rand()%INT_MAX*1.0/INT_MAX *valueNumber + 0.5 );
			if ( symbol )
				fprintf( in, "%d ", x );
			else
				fprintf( in, "%d ", -x );
		}
		fprintf( in, "0\n" );
	}
	cout<<shortCount<<", "<<longCount<<endl;
}
