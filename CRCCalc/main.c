
#include <stdio.h>


unsigned char generator = 0x00;
unsigned char crcStart = 0x00;

unsigned char Compute_CRC8_Simple(unsigned char * bytes, unsigned char cnt)
{
    unsigned char crc = crcStart;

    for (int n = 0; n < cnt; ++n)
    {
        crc ^= bytes[n];

        for (int i = 0; i < 8; i++)
        {
            if ((crc & 0x80) != 0)
            {
                crc = (unsigned char)((crc << 1) ^ generator);
            }
            else
            {
                crc <<= 1;
            }
        }
    }

    return crc;
}

void main(void)
{
	unsigned int crcCalculations = 0;

	unsigned char frames = 10;
	unsigned char crc;
	unsigned char frame[][9] =
	{
		{ 0x55, 0x60, 0x33, 0x00, 0x00, 0x00, 0x00, 0x9A, 0x54 },
		{ 0x55, 0x10, 0x5F, 0x20, 0x05, 0x90, 0x8D, 0x8E, 0x21 },
		{ 0x55, 0x10, 0x8F, 0x10, 0x44, 0x48, 0xD1, 0xD0, 0x51 },
		{ 0x55, 0x10, 0xD7, 0xD0, 0xB4, 0x98, 0x2E, 0x2E, 0x94 },
		{ 0x55, 0x90, 0x80, 0x20, 0x25, 0x90, 0x83, 0x4E, 0xE6 },
		{ 0x55, 0x10, 0x77, 0x10, 0xF8, 0x48, 0x31, 0x30, 0x71 },
		{ 0x55, 0x10, 0xCF, 0xD0, 0x6C, 0x98, 0x61, 0xAE, 0xA0 },
		{ 0x55, 0x90, 0xA0, 0x20, 0x65, 0x90, 0xC3, 0xCE, 0x7A },
		{ 0x55, 0x10, 0xF7, 0x10, 0x04, 0x48, 0x31, 0xB0, 0xD1 },
		{ 0x55, 0x10, 0x4F, 0xD0, 0xAC, 0x98, 0xDE, 0x6E, 0x88 }
	};

	unsigned char found[65535][2];
	unsigned int foundIndex = 0;


	for(int m = 0; m < frames; ++m)
	{
		for(int n = 0; n < 256; ++n)
		{
			for(int i = 0; i < 256; ++i)
			{
				crc = Compute_CRC8_Simple(&(frame[m][0]), 8);
				if(frame[m][8] == crc)
				{
					found[foundIndex][0] = generator;
					found[foundIndex][1] = crcStart;

					foundIndex++;
				}

				generator++;

				crcCalculations++;
			}

			crcStart++;
		}
	}

	printf("CRC calculations: %i\n", crcCalculations);
	printf("Total matches found: %i\n\n", foundIndex);

	for(int i = 0; i <= 1024; ++i)
	{
		unsigned cnt = 0;

		unsigned char p = found[i][0];
		unsigned char s = found[i][1];

		for(int n = 0; n <= 1024; ++n)
		{
			if(p == found[n][0] && s == found[n][1])
			{
				cnt++;
			}
		}

		if(cnt == frames)
		{
			printf("%X, %X\n", p, s);
		}
	}

	printf("\n");
}
