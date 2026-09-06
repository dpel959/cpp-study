#include <iostream>
#include <memory>

#include "Inventory.h"
#include "Item.h"
#include "Random.h"

std::unique_ptr<Item> DropItem();

int main()
{
    for (int i = 0; i < 100; ++i)
    {
        if (!Inventory::Getinstance().AddItem(DropItem()).has_value())
        {
            std::cout << "Failed add item to Inventory\n";
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

std::unique_ptr<Item> DropItem()
{
    int randNum = Random::GetRandomInt(1, 2);
    if (randNum == 1)
    {
        auto weapon = std::make_unique<Weapon>();
        weapon->SetDamage(Random::GetRandomInt(1, 100));
        return weapon;
    }

    auto armor = std::make_unique<Armor>();
    armor->SetDefence(Random::GetRandomInt(1, 100));
    return armor;
}
