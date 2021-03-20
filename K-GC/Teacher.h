#pragma once


#include "K_GC_Lib.h"
#include "Teacher.h.h"


//#define offsetof( type, field )(size_t)&(((type*)0)->field)


// C C++ �Ľṹ�������ֵǶ�׵Ļ�������
//
//  struct A
//	{
//		B b;
//	}
//
//  B Ҳ�ǽṹ�壬��Ҫ�� B ������ A ֮ǰ�� ��ʹ��ͷ�ļ���Ԥ�������� A ��B Ҳ����� ��
//  
//  �����ָ����û��������ƣ��ṹ�����ͨ��ָ�뻥�����ã�����
//
//  struct A
//  {
//		B* b;
//  }
//  struct B
//  {
//		A* a;
//  }
//
//  ��������ԭ��������� 
//
//  template struct<typename T> Ref
//	{
//		T* obj;
//		RefByWhom* self_in_RefByWhomList;
//	}
//
//  struct A
//  {
//		Ref<typename B> b;
//  }
//  struct B
//  {
//		Ref<typename A> a;
//  }
//  
//  �����ͣ�ģ�壩Ҳ������������⣬�������ϸ񣬱��磬B �ṹ��� Ref<typename A> a; �ֶ���ֵ���͵ģ����Ͳ��������� A��
//  A ������ B ֮ǰ�� ��Ҳ���У� ��������Ϊ Ref<typename A> �� ��Ч�����ͣ�����������˼ ��
//  
//  ����ĳ�ָ�룬
//
//	struct B
//  {
//		Ref<typename A>* a;
//  }
//
//  ��û�������ˣ� ��������ʹ�ã� Ҳ���ܽṹ�嶨��˳������� ��
//
//  ������Ƿ��Ͳ������͸ĳ�ָ�룬����Ȼ���У�
//
//	struct B
//  {
//		Ref<typename A*> a;
//  }
//
//  ���� Ref<typename A*> a; ��ֵ�� A* ��ָ�룬 �����У���������Ϊ Ref<typename A*> �� ��Ч�����ͣ�����������˼ ��
//
//  ����ʵ��һ���ͷ��ͱ����йأ� ��������Ϊ A ��B ��Ref<T> �����ṹ�廥�����ö����������⣬ ����������ԭ��ͬ�� ��
//  ���Կ��Կ��ǰ� A ��B ��Ref<T> �ŵ�һ��ͷ�ļ���һ������һ�������������⣬��ʱ�����ޣ������û���� ��
//  �������濴���� ��ʹ��ͷ�ļ��������� A ��B�� A ��ֵ���� B ʱ��ȻҪ�� B ������ A ֮ǰ��
//  �������������������Ч ��
//
//  InnerC �� C# һ���������� �ṹ��/�� �����˳�� InnerC ���鰴ֵǶ�׵Ľṹ��᲻�ᷢ��ѭ�����ã�
//  ��֮�� �� InnerC � ������Щ���ⲻ���� �� �� InnerC �ﲻ�ÿ��ǽṹ��ͺ��������˳�򣬿�������ʹ�úͻ������ã�
//  Ҳ������ͷ�ļ��������� Ҳ���� include ͷ�ļ��� InnerC û��ͷ�ļ���
//	InnerC �� C# һ���� ָ��һ����һЩԴ�ļ��� �ñ��������룬 ����ô�� ��
//
//  δ���� InnerC Ҫ֧��һ�����ͣ�ģ�壩�����﷨�� C C++ �򵥣��� C# һ����
//
//  struct<T> A
//	{
//		T t;
//	}
//
//  ���߼����Եķ�����ȻҪ��ȫ�Լ�ʵ�֣� ����ʹ�� InnerC �ķ��ͣ� ����������� ��
//  Ϊ�˱���߼��������� InnerC �ķ��ͣ� InnerC �ķ���ֻ֧��һ�����ͣ���֧��Ƕ�׷��ͣ�Ƕ�׷��;��Ƿ��������׷��ͣ����� A<B<T>> ��
//

struct Teacher_Ref
{
	Teacher* obj = 0;
	RefByWhom* self_in_RefByWhomList;
};
struct Student_Ref
{
	Student* obj = 0;
	RefByWhom* self_in_RefByWhomList;
};

struct Teacher
{
	Object_M;
	
	const char* name;
	int age;

	Student_Ref student1;
	Student_Ref student2;
};

struct Student
{
	Object_M;

	const char* name;
	int age;
	
	Teacher_Ref teacher;
};





