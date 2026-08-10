#pragma once
#include<unordered_map>

class ResourceBase;
class LineMesh;

class ResourceManager
{
public:
	DECLARE_SINGLE(ResourceManager);

	~ResourceManager();

public:
	void Init();
	void Clear();

	const LineMesh* GetLineMesh(std::wstring key);
private:
	std::unordered_map<std::wstring, LineMesh*> _lineMeshes;
};