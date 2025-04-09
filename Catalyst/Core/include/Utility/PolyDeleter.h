#pragma once

class DeleterBase
{
public:
	virtual ~DeleterBase() = default;

	virtual void Destroy(void* obj) = 0;
};

template<typename T>
class PolyDeleter : public DeleterBase
{
public:
	void Destroy(void* obj) override
	{
		delete static_cast<T*>(obj);
	}
};