#include "pch.h"
#include "ResourceManager.h"
#include "LineMesh.h"

ResourceManager::~ResourceManager()
{
	Clear();
}

void ResourceManager::Init()
{
	{
		LineMesh* mesh = new LineMesh();
		mesh->Load(L"UI.txt");
		_lineMeshes[L"UI"] = mesh;
	}
	{
		LineMesh* mesh = new LineMesh();
		mesh->Load(L"Menu.txt");
		_lineMeshes[L"Menu"] = mesh;
	}
	{
		LineMesh* mesh = new LineMesh();
		mesh->Load(L"MissileTank.txt");
		_lineMeshes[L"MissileTank"] = mesh;
	}
	{
		LineMesh* mesh = new LineMesh();
		mesh->Load(L"CanonTank.txt");
		_lineMeshes[L"CanonTank"] = mesh;
	}
}

void ResourceManager::Clear()
{
	for (auto mesh : _lineMeshes)
	{
		SAFE_DELETE(mesh.second);
	}

	_lineMeshes.clear();
}

const LineMesh* ResourceManager::GetLineMesh(std::wstring key)
{
	auto findItr = _lineMeshes.find(key);
	if (findItr != _lineMeshes.end())
	{
		return findItr->second;
	}
	else
	{
		return nullptr;
	}
}
