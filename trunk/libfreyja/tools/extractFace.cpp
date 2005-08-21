
#include <stdio.h>
#include <stdlib.h>

#ifdef UNIX
#   include <unistd.h>
#endif

#define MAX(max, s) max = (s > max) ? s : max
#define SKIP if (dump != 1)

typedef unsigned char byte;

byte gCommonBytes1[] = {0x00, 0x09, 0x01, 0x00};
byte gCommonBytes2[] = {0x00, 0x06, 0x01, 0x00};

// FIXME: Make it adaptive to match mat2 patterns later
char isKnownMat2(unsigned char mat2)
{
	return (mat2 == 0x9 || mat2 == 0x6 || mat2 == 0x8);
}


// Old school UT Index reader, see my UTPackage for a newer algorithm
int read_index(FILE *f, unsigned int &bytes)
{
	int val;
	char b0, b1, b2, b3, b4;

	bytes = 0;                                                         
	val = 0;
   
	fread(&b0, 1, 1, f);
	++bytes;
   
	if (b0 & 0x40)
	{
		fread(&b1, 1, 1, f);
 		++bytes;

		if (b1 & 0x80)
		{
			fread(&b2, 1, 1, f);
	 		++bytes;
 
			if (b2 & 0x80)
			{
				fread(&b3, 1, 1, f);
		 		++bytes;
 
				if (b3 & 0x80)
				{
					fread(&b4, 1, 1, f);
			 		++bytes;
					val = b4;
				}
				
				val = (val << 7) + (b3 & 0x7f);
			}
			
			val = (val << 7) + (b2 & 0x7f);
		}
		
		val = (val << 7) + (b1 & 0x7f);
	}
	
	val = (val << 6) + (b0 & 0x3f);
   
	if (b0 & 0x80)
		val = -val;

	return val;
}


int linageII2obj(FILE *in,
				 unsigned int vertOffset, unsigned int vertCount,
				 unsigned int wedgeOffset, unsigned int wedgeCount,
				 unsigned int faceOffset, unsigned int faceCount)
{
	unsigned int i, u;
	unsigned short s;
	char c;
	float uv[2];

	if (!in)
	{
		return -1;
	}

	printf("# Generated by lineageII2obj\n");
	printf("# %i %i %i %i %i %i\n", 
           vertOffset, vertCount, wedgeOffset, wedgeCount, faceOffset, faceCount);


	float vertices[vertCount][3];

	fseek(in, vertOffset, SEEK_SET);

	for (i = 0; i < vertCount; ++i)
	{
		fread(vertices[i]+0, 4, 1, in);
		fread(vertices[i]+1, 4, 1, in);
		fread(vertices[i]+2, 4, 1, in);
	}


	fseek(in, wedgeOffset, SEEK_SET);

	for (i = 0; i < wedgeCount; ++i)
	{
		fread(&s, 2, 1, in);
		fread(uv+0, 4, 1, in);
		fread(uv+1, 4, 1, in);

		printf("v %f %f %f\n", vertices[s][0],vertices[s][2],vertices[s][1]);
		printf("vt %f %f\n", uv[0], uv[1]);
	}


	fseek(in, faceOffset, SEEK_SET);

	for (i = 0; i < faceCount; ++i)
	{
		printf("f ");
		fread(&s, 2, 1, in);
		printf("%i/%i ", s+1, s+1);
		fread(&s, 2, 1, in);
		printf("%i/%i ", s+1, s+1);
		fread(&s, 2, 1, in);
		printf("%i/%i ", s+1, s+1);
		fread(&c, 1, 1, in); // mat
		fread(&c, 1, 1, in); // mat2
		fread(&u, 4, 1, in); // smoothing group
		printf("\n");
	}

	return 0;
}


// FIXME move extractDriver into this file
// extractDriver looks for face offset and passes it's best guess to extractFace
// * Checks range of each type against each other to ensure good guesses
// * Cheats using known common values!  =)

unsigned int guess_face_offset(FILE *f, unsigned long size)
{
	// { s s s 0x00 0x09 0x01 0x00 0x00 0x00 } is the most common face form
	unsigned char b;
	unsigned int offset, state, read, fix = 12;


	offset = ftell(f);

	printf("# guess_face_offset @ %u / %lu\n", offset, size);

	state = 0;

	while (offset < size && state < 5)
	{
		read = fread(&b, 1, 1, f); 	// Slow, but very turing tape of us
		++offset;

#ifdef UNIX
		if ((offset % 1000) == 0)		// Don't be greedy
			usleep(1);
#endif

		switch (state)
		{
		case 0:
			if (b == 0x00)
			{
				state = 1;
			}
			else
			{
				state = 0;
			}
			break;

		case 1:
			if (isKnownMat2(b))
			{
				++state;
				//printf("@ %i, state = %i\n", offset, state);
			}
			else if (b == 0x00)
			{
				//printf("@ %i '0x%x != 0x9' rewind to %i\n", offset, b, offset - state);
				fseek(f, -1, SEEK_CUR);
				--offset;
				state = 0;
			}
			else
			{
				state = 0;
			}
			break;

		case 2:
			if (b == 0x01)
			{
				state = 3;
				//printf("@ %i, state = %i\n", offset, state);
			}
			else if (b == 0x00)
			{
				fseek(f, -1, SEEK_CUR);
				--offset;
				state = 0;
			}
			else
			{
				state = 0;
			}
			break;

		case 3:
		case 4:
		case 5:
			if (b == 0x00)
			{
				++state;

				if (state == 5)
				{
					fix = 11;
					state = 6; // partial match will be fine today
				}

				//printf("@ %i, state = %i\n", offset, state);
			}
			else
			{
				//printf("@ %i '0x%x != 0x0' rewind to %i\n", offset, b, offset - state);
				fseek(f, -3/*state*/, SEEK_CUR);
				state = 0;
			}
			break;

		case 6:		// Trap state
			break;

		default:
			;
		}
	}

	if (state == 6)
		return offset - fix; // account for 's s s' too

	return offset;
}


#define RANGE_CHECK

#define UV_OUT_OF_RANGE -4
#define XYZ_OUT_OF_RANGE -5
#define FOUND 311

unsigned int wedgeOffset, wedgeCount, faceOffset, faceCount, vertexOffset, vertexCount;

int find_mesh(FILE *in, unsigned int offset, unsigned int count)
{
	const unsigned int failsafeLimit = 5;
	unsigned int i, u, total = 0, avg;
	unsigned short x, y, z, s, max = 0, oldmax;
	float f;
	char mat, mat2, highmatch = 0;
	unsigned int bytes = 0, guess = 1, failsafe = 0, range = 12;
	int val;
	char dump = 0;


	for (i = 0; i < count; ++i)
	{
		fread(&x, 2, 1, in);
		MAX(max, x);
		total += x;

		fread(&y, 2, 1, in);
		MAX(max, y);
		total += y;

		fread(&z, 2, 1, in);
		MAX(max, z);
		total += z;

		fread(&mat, 1, 1, in);
		fread(&mat2, 1, 1, in);
		fread(&u, 4, 1, in);

		avg = total/((i+1)*3);

		if (x > avg*4 || y > avg*4 || z > avg*4)
		{
			if (i == 0) i = 1;

			SKIP printf("# Indices outside safety range of avg\n");
			SKIP printf("#    (%i, %i, %i) vs %i\n", x, y, z, avg);
			SKIP printf("#    faceCount %i <= %i\n", count, i - 1);
			SKIP printf("#    wedgeMax %i <= %i\n", max, oldmax);
			count = i - 1;
			max = oldmax;
		}
		else if (mat != 0x0 || !isKnownMat2(mat2) || u != 1)
		{
			SKIP printf("# Material ids not expected\n");
			SKIP printf("#    f%i %i %i %i %i %i %i\n", i, x, y, z, mat, mat2, u);
			SKIP printf("#    faceCount %i <= %i\n", count, i - 1);
			SKIP printf("#    wedgeMax %i <= %i\n", max, oldmax);
			count = i - 1;
			max = oldmax;
		}

		oldmax = max;
	}

	wedgeCount = max + 1;
	wedgeOffset = 0;

	while (offset > 6 && ++failsafe < failsafeLimit)
	{
		fseek(in, offset-guess, SEEK_SET);
        val = read_index(in, bytes);

		SKIP printf("# %i bytes? -> %i\n", guess, val);

		if (val == (int)count)
		{
			SKIP printf("# *** Face index fit ***\n");
			SKIP printf("# Faces likely @ %i x %i\n", offset, count);

			// FIXME this is really an error for non interactive mode
			break;
		}

		if (val < 0 || 
		    val < (int)(count - range) ||
		    val > (int)(count + range))
		{
			++guess;
			continue;
		}

		break;
	}

	faceOffset = offset;
	faceCount = count;
	offset = wedgeOffset = offset - ((max+1) * 10) - 4 - bytes;
	count = wedgeCount = max + 1;
	max = 0;
	guess = 1;
	failsafe = 0;

	while (offset > 6 && ++failsafe < failsafeLimit) 
	{
		fseek(in, offset-guess, SEEK_SET);
        val = read_index(in, bytes);

		SKIP printf("# %i bytes? -> %i\n", guess, val);

		if (val == (int)count)
		{
			printf("# *** Wedge index fit ***\n");
			printf("# Wedges @ %i x %i\n", wedgeOffset, wedgeCount);
			printf("# Faces @ %i x %i\n", faceOffset, faceCount);
			highmatch = 1;
			break;
		}

		if (val < 0 || 
		    val < (int)(count - range) ||
		    val > (int)(count + range))
		{
			++guess;
			continue;
		}

		break;
	}

	if (highmatch)
	{
		max = 0;
		fseek(in, offset, SEEK_SET);

		for (i = 0; i < count; ++i)
		{
			fread(&s, 2, 1, in);
			MAX(max, s);
			fread(&f, 4, 1, in);
#ifdef RANGE_CHECK
			if (f > 1.2f || f < -0.34f)
			{
				printf("#Range check fail on uv[%i][0] = %f\n", i, f);
				return UV_OUT_OF_RANGE;
			}
#endif

			fread(&f, 4, 1, in);
#ifdef RANGE_CHECK
			if (f > 1.2f || f < -0.34f)
			{
				printf("#Range check fail on uv[%i][1] = %f\n", i, f);
				return UV_OUT_OF_RANGE;
			}
#endif
		}

		++max;
		offset -= ((max+1) * 12) + bytes - 8;

#ifdef RANGE_CHECK
		fseek(in, offset, SEEK_SET);
#endif

		for (i = 0; i < max && i < 12; ++i)
		{
			fread(&f, 4, 1, in);
#ifdef RANGE_CHECK
			if (f > 1000000.0f || f < -1000000.0f)
			{
				printf("#Range check fail on xyz[%i][0] = %f\n", i, f);
				return XYZ_OUT_OF_RANGE;
			}
#endif
			fread(&f, 4, 1, in);
#ifdef RANGE_CHECK
			if (f > 1000000.0f || f < -1000000.0f)
			{
				printf("#Range check fail on xyz[%i][1] = %f\n", i, f);
				return XYZ_OUT_OF_RANGE;
			}
#endif

			fread(&f, 4, 1, in);
#ifdef RANGE_CHECK
			if (f > 1000000.0f || f < -1000000.0f)
			{
				printf("#Range check fail on xyz[%i][2] = %f\n", i, f);
				return XYZ_OUT_OF_RANGE;
			}
#endif
		}


		if (offset > 0 && max > 0 && max <= wedgeCount)
		{
			vertexOffset = offset;
			vertexCount = max;
			printf("# Vertices @ %i x %i\n", offset, max);

			return FOUND;
		}
	}

	printf("# Wedges @ %i x %i ???\n", wedgeOffset, wedgeCount);
	printf("# Faces @ %i x %i  ???\n", faceOffset, faceCount);

	return -1;
}


int main(int argc, char *argv[])
{
	FILE *in;
	unsigned int count, offset, bytes, found = !FOUND;
	char dump = 1;
	unsigned long size;


	in = fopen(argv[1], "rb");

	if (!in)
	{
		printf("extractFace filename offset count [dump]\n");
		return -2;
	}

	fseek(in, 0, SEEK_END);
	size = ftell(in);

	fseek(in, 89, SEEK_SET);
	count = read_index(in, bytes);

	if (count == 0)
	{
		count = read_index(in, bytes);
	}

	printf("# faceCount is likely to be %i\n", count);

	if (argc > 2)
		count = atoi(argv[2]);

	unsigned int guessOffset = ftell(in), guessFailsafe = 0;

	if (argc > 3)
		guessOffset = atoi(argv[3]);

	while (found != FOUND && ++guessFailsafe < 12)
	{
		fseek(in, guessOffset, SEEK_SET);
		offset = guess_face_offset(in, size);

		printf("\n# Pass %i, @ %i x %i\n", guessFailsafe, offset, count);

		guessOffset = offset + 12;
		fseek(in, offset, SEEK_SET);
		found = find_mesh(in, offset, count);
	}

	if (found == FOUND)
	{
		if (dump == 1)
		{
			printf("# Generated from %s\n", argv[1]);
			linageII2obj(in, 
			             vertexOffset, vertexCount,
			             wedgeOffset, wedgeCount,
			             faceOffset, faceCount);
		}
		else
		{
			printf("# ./lineageIIraw2obj %s %i %i %i %i %i %i\n",
			       argv[1],
			       vertexOffset, vertexCount,
			       wedgeOffset, wedgeCount,
			       faceOffset, faceCount);
		}
	}

	fclose(in);

	return -1;
}
