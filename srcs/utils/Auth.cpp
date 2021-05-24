#include "Auth.hpp"

static int arrayToDecodeBase64[256] = {
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* 00-0F */
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* 10-1F */
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,62,-1,-1,-1,63,  /* 20-2F */
	52,53,54,55,56,57,58,59,60,61,-1,-1,-1,-1,-1,-1,  /* 30-3F */
	-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,  /* 40-4F */
	15,16,17,18,19,20,21,22,23,24,25,-1,-1,-1,-1,-1,  /* 50-5F */
	-1,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,  /* 60-6F */
	41,42,43,44,45,46,47,48,49,50,51,-1,-1,-1,-1,-1,  /* 70-7F */
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* 80-8F */
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* 90-9F */
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* A0-AF */
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* B0-BF */
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* C0-CF */
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* D0-DF */
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* E0-EF */
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1   /* F0-FF */
};

static int getEncodedTextLength(std::string target)
{
	int encodedTextLength = 0;

	while (arrayToDecodeBase64[(int)target[encodedTextLength]] != -1)
	{
		encodedTextLength++;
	}

	return (encodedTextLength);
}

std::string web::base64Decoder(std::string const &target)
{
	int idx = 0;
	int encodedTextLength = 0;
	std::string encodedText = "";
	std::string decodeResult = "";
	
	if (target.empty())
		return ("");

	encodedTextLength = getEncodedTextLength(target);
	encodedText = target.substr(0, encodedTextLength);

	while (idx + 4 < encodedTextLength)
	{
		decodeResult += (char)(arrayToDecodeBase64[(int)encodedText[idx]] << 2 | arrayToDecodeBase64[(int)encodedText[idx+1]] >> 4);
		decodeResult += (char)(arrayToDecodeBase64[(int)encodedText[idx + 1]] << 4 | arrayToDecodeBase64[(int)encodedText[idx + 2]] >> 2);
		decodeResult += (char)(arrayToDecodeBase64[(int)encodedText[idx + 2]] << 6 | arrayToDecodeBase64[(int)encodedText[idx + 3]]);
		idx += 4;
	}

	if (encodedTextLength - idx > 1)
	{
		decodeResult += (char)(arrayToDecodeBase64[(int)encodedText[idx]] << 2 | arrayToDecodeBase64[(int)encodedText[idx + 1]] >> 4);
	}

	if (encodedTextLength - idx > 2)
	{
		decodeResult += (char)(arrayToDecodeBase64[(int)encodedText[idx + 1]] << 4 | arrayToDecodeBase64[(int)encodedText[idx + 2]] >> 2);
	}

	if (encodedTextLength - idx > 3)
	{
		decodeResult += (char)(arrayToDecodeBase64[(int)encodedText[idx + 2]] << 6 | arrayToDecodeBase64[(int)encodedText[idx + 3]]);
	}

	return (decodeResult);
}