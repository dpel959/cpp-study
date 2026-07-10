#include <iostream>
#include "Random.h"
#include "item.h"
#include "Inventory.h"

Item* DropItem();

int main()
{
    for (int i = 0; i < 100; ++i)
    {
        Item* item = DropItem();

        if (!Inventory::Getinstance().AddItem(item))
        {
            std::cout << "Failed add item to Inventory\n";
            delete item;
        }
    }

    // 내가 아이템을 잃을 때 랜덤으로 일부 아이템을 드랍함.
    if (Inventory::Getinstance().RandomRemoveItem())
    {
        std::cout << "Succeed to Random Remove Items\n";
    }
    else
    {
        std::cout << "Failed to Random Remove Items\n";
    }
}

Item* DropItem()
{
    int randNum = Random::GetRandomInt(1, 2);
    if (randNum == 1)
    {
        Weapon* weapon = new Weapon();
        weapon->SetDamage(Random::GetRandomInt(1, 100));
        return weapon;
    }
    else
    {
        Armor* armor = new Armor();
        armor->SetDefence(Random::GetRandomInt(1, 100));
        return armor;
    }
}