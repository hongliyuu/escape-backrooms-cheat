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
	void destroy(SDK::ACharacter* character);
	void spawn(SDK::TSubclassOf<SDK::AActor> cls);
	bool can_control(SDK::ACharacter* character);
	bool poss(SDK::APawn* pawn);
	void unposs();
	void freeze_all();
	void unfreeze_all();
	void freeze(SDK::ACharacter* character);
	void unfreeze(SDK::ACharacter* character);
	bool is_frozen(SDK::ACharacter* character);
	bool has_frozen();
	bool is_valid(SDK::UObject* object);

private:
	entity() = default;
	void domain();
};
