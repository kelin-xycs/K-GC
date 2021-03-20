#pragma once



#define Object_M MetaData metaData; bool isMarked = false; int rootRefCount = 0; Object* rootRefListBefore; Object* rootRefListNext; Object* objectListBefore; Object* objectListNext; RefByWhom* refByWhomList = 0; RefByWhom* refByWhomListTail = 0

#define LinkedList_Add( head, tail, node, beforeField, nextField ) if (head == 0) { head = node; tail = node; } else { tail->nextField = node; node->beforeField = tail; tail = node; }
#define LinkedList_Remove( head, tail, node, beforeField, nextField ) if (node == head) { if (node == tail) { head = 0; tail = 0; } else {	head = node->nextField; } } else { if (node == tail) { tail = node->beforeField; } else	{ node->beforeField->nextField = node->nextField; node->nextField->beforeField = node->beforeField;	} }

#define AddToRootRefList( obj )  LinkedList_Add(_rootRefList, _rootRefListTail, obj, rootRefListBefore, rootRefListNext)
#define RemoveFromRootRefList( obj ) LinkedList_Remove(_rootRefList, _rootRefListTail, obj, rootRefListBefore, rootRefListNext)


struct ObjectFieldsArray
{
	size_t* arr;
	int length;
};

struct MetaData
{
	const char* typeName;
	ObjectFieldsArray objectFieldsArray;  // 对象类型字段的偏移量，供 GC 扫描
};

struct RefByWhom
{
	void* who;
	RefByWhom* before;
	RefByWhom* next;
};

struct Object
{
	Object_M;
};

struct Ref
{
	Object* obj = 0;
	RefByWhom* self_in_RefByWhomList;
};


void AddToObjectList(Object* obj);
void 根引用取消指向对象(Ref* ref);
void 根引用指向对象(Ref* ref, Object* obj);
void 字段引用指向对象(Ref* ref, Object* obj);

void GC_Work();