#pragma once

class Refcounted
{
public:
	Refcounted() : usings(0) {}
	virtual ~Refcounted() {}

	void addRef() {usings++;}
	void release() {if(--usings == 0) delete this;}
	int getUsings()	const {return usings;}

private:
	int usings;

};

template<typename T>
class Reference
{
public:
	Reference(T * _ptr = nullptr)
	{
		ptr = _ptr;
		if(_ptr)
			ptr->addRef();
	}

	Reference(const Reference & ref)
	{
		if(ptr)
			ptr->release();

		ptr = ref.ptr;

		if(ptr)
			ptr->addRef();
	}

	~Reference()
	{
		if(ptr)
			ptr->release();
	}

	Reference & operator = (const Reference & ref)
	{
		if(ptr)
			ptr->release();

		ptr = ref.ptr;

		if(ptr)
			ptr->addRef();

		return *this;
	}

	bool operator == (const Reference & ref) const
	{
		return (ref.getPtr() == this->ptr);
	}

	bool operator == (T * ptr) const
	{
		return this->ptr == ptr;
	}

	T * operator -> () const
	{
		return ptr;
	}

	T * getPtr() const
	{
		return ptr;
	}

	void setPtr(Refcounted * _ptr = nullptr)
	{
		if(this->ptr)
			ptr->release();
		this->ptr = _ptr;
	}

	void take()
	{
		if(this->ptr)
			ptr->addRef();
	}

	void release()
	{
		if(this->ptr)
			ptr->release();
	}

private:
	T * ptr;
};