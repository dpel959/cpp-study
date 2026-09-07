#include "pch.h"
#include "ObjectManager.h"
#include "Object.h"
#include <algorithm>

ObjectManager::~ObjectManager()
{
	Clear();
}

void ObjectManager::Update()
{
	_isUpdating = true;

	for (const std::unique_ptr<Object>& object : _objects)
	{
		if (IsPendingRemove(object.get()))
		{
			continue;
		}

		object->Update();
	}

	_isUpdating = false;
	FlushPendingObjects();
}

void ObjectManager::Render(HDC hdc)
{
	for (const std::unique_ptr<Object>& object : _objects)
	{
		object->Render(hdc);
	}
}

void ObjectManager::Remove(Object* object)
{
	if (object == nullptr || IsPendingRemove(object))
	{
		return;
	}

	const bool isActive = std::any_of(
		_objects.begin(),
		_objects.end(),
		[object](const std::unique_ptr<Object>& managedObject)
		{
			return managedObject.get() == object;
		});

	const bool isPendingAdd = std::any_of(
		_pendingAddQ.begin(),
		_pendingAddQ.end(),
		[object](const std::unique_ptr<Object>& managedObject)
		{
			return managedObject.get() == object;
		});

	if (isActive || isPendingAdd)
	{
		_pendingRemoveQ.push_back(object);
	}

	if (_isUpdating == false)
	{
		FlushPendingObjects();
	}
}

void ObjectManager::Clear()
{
	_pendingRemoveQ.clear();
	_pendingAddQ.clear();
	_objects.clear();
}

std::vector<Object*> ObjectManager::GetObjects() const
{
	std::vector<Object*> objects;
	objects.reserve(_objects.size());

	for (const std::unique_ptr<Object>& object : _objects)
	{
		if (IsPendingRemove(object.get()) == false)
		{
			objects.push_back(object.get());
		}
	}

	return objects;
}

bool ObjectManager::IsAlive(const Object* object) const
{
	if (object == nullptr || IsPendingRemove(object))
	{
		return false;
	}

	return std::any_of(
		_objects.begin(),
		_objects.end(),
		[object](const std::unique_ptr<Object>& managedObject)
		{
			return managedObject.get() == object;
		});
}

void ObjectManager::FlushPendingObjects()
{
	for (std::unique_ptr<Object>& object : _pendingAddQ)
	{
		_objects.push_back(std::move(object));
	}
	_pendingAddQ.clear();

	for (Object* removeTarget : _pendingRemoveQ)
	{
		auto findItr = std::find_if(
			_objects.begin(),
			_objects.end(),
			[removeTarget](const std::unique_ptr<Object>& managedObject)
			{
				return managedObject.get() == removeTarget;
			});

		if (findItr == _objects.end())
		{
			continue;
		}

		if (findItr != _objects.end() - 1)
		{
			*findItr = std::move(_objects.back());
		}

		_objects.pop_back();
	}

	_pendingRemoveQ.clear();
}

bool ObjectManager::IsPendingRemove(const Object* object) const
{
	return std::find(_pendingRemoveQ.begin(), _pendingRemoveQ.end(), object)
		!= _pendingRemoveQ.end();
}
