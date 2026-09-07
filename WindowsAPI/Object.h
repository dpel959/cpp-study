#pragma once

enum class ObjectType
{
	None,
	Player,
	Monster,
	Projectile,
};

class Object
{
public:
	Object(ObjectType type);
	virtual ~Object();
	
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Render(HDC hdc) = 0;

	ObjectType GetObjectType() { return _type; }
	void SetPos(Pos pos) { _pos = pos; }
	Pos GetPos() { return _pos; }
	void SetDir(Dir dir) { _dir = dir; }
	Dir GetDir() const { return _dir; }

	float GetRadius() { return _radius; }
	void SetRadius(float radius) { _radius = radius; }
protected:
	ObjectType _type = ObjectType::None;
	Stat _stat = {};
	Pos _pos = {};
	Dir _dir = Dir::Left;
	float _radius = 50.f;
};
