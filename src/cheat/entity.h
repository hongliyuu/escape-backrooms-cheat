#pragma once
#include "_sdk.h"
#include <string>

class entity
{
public:
	static entity* get();

	void init();
	void main();
	void kill_all();
	void kill(const std::string& name);
	void spawn(SDK::TSubclassOf<SDK::AActor> cls);
	void poss(SDK::APawn* pawn);
	void unposs();
	void freeze_all();
	void unfreeze_all();
	void freeze(SDK::ACharacter* character);
	void unfreeze(SDK::ACharacter* character);
	bool is_frozen(SDK::ACharacter* character);

private:
	entity() = default;
	void domain();
};