#include "framework.h"

/// cryptopp 라이브러리를 이용해서 암호화 알고리즘을 테스트하는 코드입니다.

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cryptopp/aes.h>
#include <cryptopp/filters.h>
#include <cryptopp/modes.h>
#include <cryptopp/osrng.h>

class PlayerData {
public:
	PlayerData(int gold, int wood, int iron, float level, int day, float playTime)
		: m_gold(gold), m_wood(wood), m_iron(iron), m_level(level), m_day(day), m_playTime(playTime) {}

	void Serialize(const std::string& filename);
	void Deserialize(const std::string& filename);

	void PrintData() {
		std::cout << "Gold: " << m_gold << ", Wood: " << m_wood << ", Iron: " << m_iron
			<< ", Level: " << m_level << ", Day: " << m_day << ", PlayTime: " << m_playTime << std::endl;
	}

private:
	int m_gold;
	int m_wood;
	int m_iron;
	float m_level;
	int m_day;
	float m_playTime;

	std::string Encrypt(const std::string& plainText, const std::string& keyPath, const std::string& ivPath);
	std::string Decrypt(const std::string& cipherText, const std::string& keyPath, std::string& ivPath);
	void SaveKeyAndIV(const CryptoPP::byte* key, const CryptoPP::byte* iv, const std::string& keyPath, const std::string& ivPath);
	void LoadKeyAndIV(CryptoPP::byte* key, CryptoPP::byte* iv, const std::string& keyPath, std::string& ivPath);
};

class WorldData
{
public:
	WorldData(bool _window, int _width, int _height)
		:m_window(_window), m_width(_width), m_height(_height) {}

	void Serialize(const std::string& filename) {}
	void Deserialize(const std::string& filename) {}

private:
	bool m_window;
	int m_width;
	int m_height;

	std::string Encrypt(const std::string& plainText, const std::string& keyPath, const std::string& ivPath){}
	std::string Decrypt(const std::string& cipherText, const std::string& keyPath, std::string& ivPath){}
	void SaveKeyAndIV(const CryptoPP::byte* key, const CryptoPP::byte* iv, const std::string& keyPath, const std::string& ivPath){}
	void LoadKeyAndIV(CryptoPP::byte* key, CryptoPP::byte* iv, const std::string& keyPath, std::string& ivPath) {}

};

void PlayerData::Serialize(const std::string& filename) {
	std::string serializedString = std::to_string(m_gold) + " " + std::to_string(m_wood) + " " +
		std::to_string(m_iron) + " " + std::to_string(m_level) + " " +
		std::to_string(m_day) + " " + std::to_string(m_playTime);

	std::string keyPath = "key.bin";
	std::string ivPath = "iv.bin";
	std::string encryptedData = Encrypt(serializedString, keyPath, ivPath);

	std::ofstream outFile(filename, std::ios::binary);
	outFile.write(encryptedData.c_str(), encryptedData.size());
	outFile.close();
}

void PlayerData::Deserialize(const std::string& filename) {
	std::ifstream inFile(filename, std::ios::binary);
	std::string encryptedData((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
	inFile.close();

	std::string keyPath = "key.bin";
	std::string ivPath = "iv.bin";
	std::string decryptedData = Decrypt(encryptedData, keyPath, ivPath);

	std::istringstream iss(decryptedData);
	iss >> m_gold >> m_wood >> m_iron >> m_level >> m_day >> m_playTime;
}

std::string PlayerData::Encrypt(const std::string& plainText, const std::string& keyPath, const std::string& ivPath) {
	CryptoPP::AutoSeededRandomPool prng;
	CryptoPP::byte key[CryptoPP::AES::DEFAULT_KEYLENGTH], iv[CryptoPP::AES::BLOCKSIZE];
	prng.GenerateBlock(key, sizeof(key));
	prng.GenerateBlock(iv, sizeof(iv));

	SaveKeyAndIV(key, iv, keyPath, ivPath);

	std::string cipherText;
	CryptoPP::AES::Encryption aesEncryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
	CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption(aesEncryption, iv);

	CryptoPP::StreamTransformationFilter stfEncryptor(cbcEncryption, new CryptoPP::StringSink(cipherText));
	stfEncryptor.Put(reinterpret_cast<const unsigned char*>(plainText.c_str()), plainText.length());
	stfEncryptor.MessageEnd();

	return cipherText;
}

std::string PlayerData::Decrypt(const std::string& cipherText, const std::string& keyPath, std::string& ivPath) {
	CryptoPP::byte key[CryptoPP::AES::DEFAULT_KEYLENGTH], iv[CryptoPP::AES::BLOCKSIZE];
	LoadKeyAndIV(key, iv, keyPath, ivPath);

	std::string decryptedText;
	CryptoPP::AES::Decryption aesDecryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
	CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption(aesDecryption, iv);

	CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, new CryptoPP::StringSink(decryptedText));
	stfDecryptor.Put(reinterpret_cast<const unsigned char*>(cipherText.c_str()), cipherText.size());
	stfDecryptor.MessageEnd();

	return decryptedText;
}

void PlayerData::SaveKeyAndIV(const CryptoPP::byte* key, const CryptoPP::byte* iv, const std::string& keyPath, const std::string& ivPath) {
	std::ofstream keyFile(keyPath, std::ios::binary);
	keyFile.write((char*)key, CryptoPP::AES::DEFAULT_KEYLENGTH);
	keyFile.close();

	std::ofstream ivFile(ivPath, std::ios::binary);
	ivFile.write((char*)iv, CryptoPP::AES::BLOCKSIZE);
	ivFile.close();
}

void PlayerData::LoadKeyAndIV(CryptoPP::byte* key, CryptoPP::byte* iv, const std::string& keyPath, std::string& ivPath) {
	std::ifstream keyFile(keyPath, std::ios::binary);
	keyFile.read((char*)key, CryptoPP::AES::DEFAULT_KEYLENGTH);
	keyFile.close();

	std::ifstream ivFile(ivPath, std::ios::binary);
	ivFile.read((char*)iv, CryptoPP::AES::BLOCKSIZE);
	ivFile.close();
}

int main() {
	PlayerData player(100, 50, 30, 5.0, 1, 2.5);
	player.Serialize("encrypted_player_data.bin");
	player.PrintData();

	PlayerData newPlayer(0, 0, 0, 0.0, 0, 0.0);
	newPlayer.PrintData();
	newPlayer.Deserialize("encrypted_player_data.bin");
	newPlayer.PrintData();

	return 0;
}
