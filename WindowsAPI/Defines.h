#pragma once
#define DECLARE_SINGLE(classname)			\
public:										\
	static classname& GetInstance()			\
	{										\
		static classname instance;			\
											\
		return instance;					\
	}										\
											\
private:									\
	classname() {};							\

#define GET_SINGLE(classname)	classname::GetInstance()
//이건 싱글톤을 쓰는 측이 편하게 쓸 수 있도록.