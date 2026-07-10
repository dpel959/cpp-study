#include "Item.h"
#include "Random.h"
#include <iostream>
using std::cout;

// *************************
//			Item
// *************************

Item::Item(EItemType itemType)
	: _itemType(itemType)
{
	int randNum = Random::GetRandomInt(1, 100);
	if (randNum < 50)
		_rarity = EItemRarity::Normal;
	else if (randNum < 80)
		_rarity = EItemRarity::Rare;
	else
		_rarity = EItemRarity::Unique;
}

Item::~Item()
{
}

void Item::PrintInfo()
{
	cout << "Item Rarity : ";
	switch (_rarity)
	{
	case EItemRarity::Normal:
		cout << "Normal\n";
		break;
	case EItemRarity::Rare:
		cout << "Rare\n";
		break;
	case EItemRarity::Unique:
		cout << "Unique\n";
		break;
	}
}

EItemRarity Item::GetRarity()
{
	return _rarity;
}

EItemType Item::GetItemType()
{
	return _itemType;
}

// *************************
//			Weapon
// *************************

Weapon::Weapon()
	: Item(EItemType::Weapon)
{
}

Weapon::~Weapon()
{
}

void Weapon::SetDamage(int damage)
{
	_damage = damage >= 0 ? damage : 0;
}

int Weapon::GetDamage()
{
	return _damage;
}

void Weapon::PrintInfo()
{
	cout << "***********************\n";
	Item::PrintInfo();
	cout << "Item Type : Weapon\n";
	cout << "Attack : " << _damage << '\n';
	cout << "***********************\n";
}

// *************************
//			Armor
// *************************

Armor::Armor()
	: Item(EItemType::Armor)
{
}

Armor::~Armor()
{
}

void Armor::SetDefence(int defence)
{
	_defence = defence >= 0 ? defence : 0;
}

int Armor::GetDefence()
{
	return _defence;
}

void Armor::PrintInfo()
{
	cout << "***********************\n";
	Item::PrintInfo();
	cout << "Item Type : Armor\n";
	cout << "Defence : " << _defence << '\n';
	cout << "***********************\n";
}