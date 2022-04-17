#ifndef ENCODER_H
#define ENCODER_H

#include <vector>
#include <string>

//We will use base64 along with a salt to encode data (base64 has no-loss of data when sent through email)

namespace Encoder
{

	//This is a prototype of a fuction which will encode our string to b64

	std::string b64Encode(const std::string &stringToEncode);

	//Here is the codes for base64 encoding

	const std::string &BASE64_CODES = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	//These are salts which will randomize our base64 encoded string

	const std::string &saltOne = "LM::TB::BB";
	const std::string &saltTwo = "_:/_77";
	const std::string &saltThree = "line=wowC++";

	//Below is the function which will actually scramble our base64 string so that it is undecodable using the salts above

	std::string saltify(std::string encodedString)
	{
		return saltTwo + encodedString + saltOne + saltThree;
	}

	//Here is the implementation for the base64 encoding function prototyped at the start 
	//I have no idea how this works, I just copied it since we don't need to reinvent the wheel

	std::string b64Encode(const std::string &stringToEncode)
	{
		std::string encodedString;
		int index = 0;
		int bits = -6;
		const unsigned int b63 = 0x3F;

		for (const auto &letter : stringToEncode)
		{
			index = (index << 8) + letter;
			bits += 8;
			while (bits >= 0)
			{
				encodedString.push_back(BASE64_CODES[(index >> bits) & b63]);
				bits -= 6;
			}
		}

		if (bits > -6)
		{
			encodedString.push_back(BASE64_CODES[((index << 8) >> (bits + 8)) & b63]);
		}

		while (encodedString.size() % 4)
		{
			encodedString.push_back('=');
		}

		return encodedString;
	}




}

#endif // !ENCODER_H