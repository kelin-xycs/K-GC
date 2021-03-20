
#include <iostream>

#include "K_GC_Lib.h"




Object* _rootRefList = 0;
Object* _rootRefListTail = 0;
Object* _objectList = 0;
Object* _objectListTail = 0;


void AddToObjectList(Object* obj)
{
	LinkedList_Add(_objectList, _objectListTail, obj, objectListBefore, objectListNext)
}

void RemoveFromObjectList(Object* obj)
{
	LinkedList_Remove(_objectList, _objectListTail, obj, objectListBefore, objectListNext)
}

RefByWhom* NewRefByWhom(void* who)
{
	RefByWhom* r = (RefByWhom*)malloc(sizeof(RefByWhom));

	r->who = who;

	return r;
}

inline void AddRefByWhom(Ref* ref, Object* obj)
{
	RefByWhom* refByWhom = NewRefByWhom(&ref->obj);

	LinkedList_Add(obj->refByWhomList, obj->refByWhomListTail, refByWhom, before, next)

	ref->self_in_RefByWhomList = refByWhom;
}

inline void RemoveRefByWhom(Ref* ref, Object* obj)
{
	RefByWhom* refByWhom = ref->self_in_RefByWhomList;

	LinkedList_Remove(obj->refByWhomList, obj->refByWhomListTail, refByWhom, before, next)

	free(refByWhom);
}

void 根引用取消指向对象(Ref* ref)
{
	Object* obj = ref->obj;

	if (obj == 0)
		return;

	ref->obj = 0;

	obj->rootRefCount--;

	if (obj->rootRefCount == 0)
	{
		RemoveFromRootRefList(obj);
	}

	RemoveRefByWhom(ref, obj);
}

//  根引用 就是 rootRef ， 包括 局部变量引用 和 静态变量(全局变量)引用
void 根引用指向对象(Ref* ref, Object* obj)
{

	根引用取消指向对象(ref);

	if (obj == 0)
		return;

	ref->obj = obj;

	obj->rootRefCount++;

	if (obj->rootRefCount == 1)
	{
		AddToRootRefList(obj)
	}

	AddRefByWhom(ref, obj);
}

void 字段引用取消指向对象(Ref* ref)
{
	Object* obj = ref->obj;

	if (obj == 0)
		return;

	ref->obj = 0;

	RemoveRefByWhom(ref, obj);
}

void 字段引用指向对象(Ref* ref, Object* obj)
{
	字段引用取消指向对象(ref);

	if (obj == 0)
		return;

	ref->obj = obj;

	AddRefByWhom(ref, obj);
}

void Scan(Object* obj)
{
	obj->isMarked = true;

	Object* child;
	ObjectFieldsArray objectFieldsArray = obj->metaData.objectFieldsArray;

	for (int i = 0; i < objectFieldsArray.length; i++)
	{
		child = ((Ref*)((size_t)obj + objectFieldsArray.arr[i]))->obj;

		if (child == 0 || child->isMarked == true)
			continue;

		Scan(child);
	}
}

void FreeRefByWhomList(Object* obj)
{
	RefByWhom* refByWhom = obj->refByWhomList;

	while (true)
	{
		if (obj->refByWhomList == 0)
			break;

		bool isTail = (refByWhom == obj->refByWhomListTail);

		RefByWhom* refByWhomNext = refByWhom->next;

		std::cout << "回收 RefByWhom 。\n";

		free(refByWhom);

		if (isTail)
			break;

		refByWhom = refByWhomNext;
	}
}

void GC_Work()
{
	std::cout << "GC_Work ...\n\n";

	Object* obj;
	Object* child;

	obj = _rootRefList;

	while (true)
	{
		if (_rootRefList == 0)
			break;

		Scan(obj);

		if (obj == _rootRefListTail)
			break;

		obj = obj->rootRefListNext;
	}

	obj = _objectList;

	while (true)
	{
		if (_objectList == 0)
			break;

		bool isTail = (obj == _objectListTail);

		Object* objNext = obj->objectListNext;

		if (obj->isMarked == false)
		{
			FreeRefByWhomList(obj);

			std::cout << "回收 "<< obj->metaData.typeName <<" 。\n\n";

			RemoveFromObjectList(obj);

			free(obj);
		}
		else
		{
			obj->isMarked = false;
		}

		if (isTail == true)
			break;

		obj = objNext;
	}

	std::cout << "GC_Work End .\n-------------------------------------\n\n";
}






