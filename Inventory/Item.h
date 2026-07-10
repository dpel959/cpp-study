#pragma once
#include "Enums.h"

// *************************
//			Item
// *************************

class Item
{
public:
	virtual void PrintInfo();
	EItemRarity GetRarity();
	EItemType GetItemType();
	virtual ~Item();

protected:
	Item(EItemType itemType);
	int _itemId = 0;
	int _itemCount = 0;
	EItemRarity _rarity;
	EItemType _itemType;
};

// *************************
//			Weapon
// *************************

class Weapon : public Item
{
public:
	Weapon();
	virtual ~Weapon();

	void SetDamage(int damage);
	int GetDamage();
	virtual void PrintInfo() override;

private:
	int _damage = 0;
};

// *************************
//			Armor
// *************************

class Armor : public Item
{
public:
	Armor();
	virtual ~Armor();

	void SetDefence(int def);
	int GetDefence();
	virtual void PrintInfo() override;

private:
	int _defence = 0;
};