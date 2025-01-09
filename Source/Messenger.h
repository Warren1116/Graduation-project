#pragma once

#include <functional>
#include <string>
#include <map>


class Messenger
{
public:
	// ƒƒbƒZ[ƒWóMŠÖ”
	using Function = void(void*);
	using Receiver = std::function<Function>;

	static constexpr const uint64_t InvalidKey = 0xFFFFFFFFFFFFFFFF;

private:
	Messenger() {}
	~Messenger() {}

public:
	// —Bˆê‚ÌƒCƒ“ƒXƒ^ƒ“ƒXæ“¾
	static Messenger& Instance()
	{
		static Messenger instance;
		return instance;
	}

	// óMŠÖ”“o˜^‰ğœ
	void Clear();

	// ƒf[ƒ^‘—M
	void SendData(const std::string& identifier, void* data);

	// ŠÖ”“o˜^(“¯ˆêŠÖ”‚ğ•¡”“o˜^‚Å‚«‚é‚Ì‚Å’ˆÓ)
	uint64_t AddReceiver(const std::string& identifier, Receiver receiver);

	// ŠÖ”“o˜^‰ğœ(“o˜^‚ÌƒL[‚ª•K—v)
	void RemoveReceiver(uint64_t key);

private:
	struct Data
	{
		uint64_t	key;
		Receiver	func;
		Data(uint64_t key, Receiver func) : key(key), func(func) {}
		bool operator==(Data& r)
		{
			return key == r.key;
		}
	};
	std::multimap<std::string, Data> receivers;
	uint64_t incrementKey = 0;
};
