#pragma once


#include "K_GC_Lib.h"
#include "Teacher.h.h"


//#define offsetof( type, field )(size_t)&(((type*)0)->field)


// C C++ 的结构体如果按值嵌套的话，比如
//
//  struct A
//	{
//		B b;
//	}
//
//  B 也是结构体，这要求 B 定义在 A 之前， 即使在头文件中预先声明了 A 、B 也是如此 。
//  
//  如果是指针则没有这个限制，结构体可以通过指针互相引用，比如
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
//  我们这里原来的设计是 
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
//  但泛型（模板）也存在上面的问题，甚至更严格，比如，B 结构体的 Ref<typename A> a; 字段是值类型的，泛型参数类型是 A，
//  A 定义在 B 之前， 但也不行， 编译器认为 Ref<typename A> 是 无效的类型，大概是这个意思 。
//  
//  如果改成指针，
//
//	struct B
//  {
//		Ref<typename A>* a;
//  }
//
//  就没有问题了， 可以任意使用， 也不受结构体定义顺序的限制 。
//
//  但如果是泛型参数类型改成指针，则仍然不行，
//
//	struct B
//  {
//		Ref<typename A*> a;
//  }
//
//  这里 Ref<typename A*> a; 是值， A* 是指针， 但不行，编译器认为 Ref<typename A*> 是 无效的类型，大概是这个意思 。
//
//  但其实不一定和泛型本身有关， 可能是因为 A 、B 、Ref<T> 三个结构体互相引用而发生的问题， 基本的问题原因同上 。
//  所以可以考虑把 A 、B 、Ref<T> 放到一个头文件里一起声明一下来解决这个问题，但时间有限，这个就没试了 。
//  但从上面看到， 即使在头文件里声明了 A 、B， A 按值引用 B 时仍然要求 B 定义在 A 之前，
//  所以这个方法不见得有效 。
//
//  InnerC 和 C# 一样，不区分 结构体/类 定义的顺序， InnerC 会检查按值嵌套的结构体会不会发生循环引用，
//  总之， 在 InnerC 里， 上面这些问题不存在 。 在 InnerC 里不用考虑结构体和函数定义的顺序，可以任意使用和互相引用，
//  也不用在头文件里声明， 也不用 include 头文件， InnerC 没有头文件，
//	InnerC 和 C# 一样， 指定一个或一些源文件， 让编译器编译， 就这么简单 。
//
//  未来， InnerC 要支持一级泛型（模板），且语法比 C C++ 简单，和 C# 一样，
//
//  struct<T> A
//	{
//		T t;
//	}
//
//  但高级语言的泛型仍然要完全自己实现， 不能使用 InnerC 的泛型， 以免出现问题 。
//  为了避免高级语言误用 InnerC 的泛型， InnerC 的泛型只支持一级泛型，不支持嵌套泛型，嵌套泛型就是泛型里面套泛型，比如 A<B<T>> 。
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





