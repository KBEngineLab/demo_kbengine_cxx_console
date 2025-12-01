#pragma once
#include "KBECommon.h"
#include "./blowfish/Blowfish.h"

#if PLATFORM_WINDOWS
#include "Windows/WindowsHWrapper.h"
#include "Windows/AllowWindowsPlatformTypes.h"
#endif

// #define UI UI_ST
// #include "openssl/rand.h"
// #include "openssl/blowfish.h"
// #undef UI

#if PLATFORM_WINDOWS
#include "Windows/HideWindowsPlatformTypes.h"
#endif

namespace KBEngine
{
	class NetworkInterfaceBase;

	class MemoryStream;
class PacketSenderBase;
class MessageReader;

class EncryptionFilter
{
public:

	EncryptionFilter() {}
	virtual ~EncryptionFilter();
	virtual void encrypt(MemoryStream *pMemoryStream) = 0;
	virtual void encrypt(uint8 *buf, MessageLengthEx len) = 0;
	virtual void encrypt(uint8 *buf, MessageLengthEx offset, MessageLengthEx len) = 0;

	virtual void decrypt(MemoryStream *pMemoryStream) = 0;
	virtual void decrypt(uint8 *buf, MessageLengthEx len) = 0;
	virtual void decrypt(uint8 *buf, MessageLengthEx offset, MessageLengthEx len) = 0;

	virtual bool send(NetworkInterfaceBase* pPacketSender, MemoryStream *pPacket) = 0;
	virtual bool recv(MessageReader* pMessageReader, MemoryStream *pPacket) = 0;
};


class BlowfishFilter : public EncryptionFilter
{
public:
	// 每块大小
	static const uint32 BLOCK_SIZE = 64 / 8;
	static const uint32 MIN_PACKET_SIZE = (sizeof(MessageLength) + 1 + BLOCK_SIZE);


	// key的最小和最大大小
	static const int MIN_KEY_SIZE = 32 / 8;
	static const int MAX_KEY_SIZE = 448 / 8;

	// 默认key的大小
	static const int DEFAULT_KEY_SIZE = 128 / 8;

	BlowfishFilter(const KBString & key);
	BlowfishFilter(int keySize = DEFAULT_KEY_SIZE);

	virtual ~BlowfishFilter();

	virtual void encrypt(MemoryStream *pMemoryStream);
	virtual void encrypt(uint8 *buf, MessageLengthEx len);
	virtual void encrypt(uint8 *buf, MessageLengthEx offset, MessageLengthEx len);

	virtual void decrypt(MemoryStream *pMemoryStream);
	virtual void decrypt(uint8 *buf, MessageLengthEx len);
	virtual void decrypt(uint8 *buf, MessageLengthEx offset, MessageLengthEx len);

	virtual bool send(NetworkInterfaceBase *pPacketSender, MemoryStream *pPacket);
	virtual bool recv(MessageReader *pMessageReader, MemoryStream *pPacket);


	Blowfish * pBlowFishKey() { return (Blowfish*)pBlowFishKey_; }

	KBArray<uint8> key()
	{
		KBArray<uint8> keyArray;
		keyArray.SetNum(key_.length());
		memcpy(keyArray.GetData(), TCHARToANSI(key_.c_str()).data(), key_.length());

		return keyArray;
	}

private:
	bool init();

private:
	bool			isGood_;
	MemoryStream*	pPacket_;
	MemoryStream*	pEncryptStream_;
	MessageLength	packetLen_;
	uint8			padSize_;

	KBString key_;
	int keySize_;
	Blowfish * pBlowFishKey_;
};

}
