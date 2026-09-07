#pragma once
#include "LineMesh.h"
#include <memory>
#include <unordered_map>

class ResourceManager
{
public:
	DECLARE_SINGLE(ResourceManager);

	~ResourceManager();

public:
	void Init();
	void Clear();

	const LineMesh* GetLineMesh(const std::wstring& key) const;
private:
	// 강의와 차별점 : ResourceManager만 리소스를 소유하고, 사용자는 const 포인터로 관찰만 한다.
	std::unordered_map<std::wstring, std::unique_ptr<LineMesh>> _lineMeshes;
};
