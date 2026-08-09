#include "pch.h"
#include "Monster.h"
#include "Utils.h"

Monster::Monster() : Object(ObjectType::Monster)
{

}

Monster::~Monster()
{
}

void Monster::Init()
{
	_stat = { 100,100,10 };
}

void Monster::Update()
{
}

void Monster::Render(HDC hdc)
{
	Utils::DrawRect(hdc, _pos, 50, 50);
}