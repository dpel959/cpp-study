#include "pch.h"
#include "ResourceManager.h"
#include "LineMesh.h"

ResourceManager::~ResourceManager()
{
	Clear();
}

void ResourceManager::Init()
{
	Clear();

	auto missileTank = std::make_unique<LineMesh>();
	missileTank->Load(L"MissileTank.txt");
	_lineMeshes.emplace(L"MissileTank", std::move(missileTank));

	auto canonTank = std::make_unique<LineMesh>();
	canonTank->Load(L"CanonTank.txt");
	_lineMeshes.emplace(L"CanonTank", std::move(canonTank));
}

void ResourceManager::Clear()
{
	_lineMeshes.clear();
}

const LineMesh* ResourceManager::GetLineMesh(const std::wstring& key) const
{
	auto findItr = _lineMeshes.find(key);
	if (findItr != _lineMeshes.end())
	{
		return findItr->second.get();
	}
	else
	{
		return nullptr;
	}
}
