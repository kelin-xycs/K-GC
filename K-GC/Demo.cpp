// Demo.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

#include "K_GC_Lib.h"
#include "Teacher.h"


size_t _teacher_objectFields[] = { offsetof(Teacher, student1), offsetof(Teacher, student2) };
MetaData _teacher_metadata = { "Teacher", _teacher_objectFields, 2 };

void InitObj(Object* obj)
{
	obj->isMarked = false;
	obj->rootRefCount = 0;
	obj->refByWhomList = 0;
	obj->refByWhomListTail = 0;
}
Teacher* NewTeacher(const char* name, int age)
{
	Teacher* t = (Teacher*)malloc(sizeof(Teacher));

	InitObj((Object*)t);
	t->student1.obj = 0;
	t->student2.obj = 0;

	t->metaData = _teacher_metadata;

	t->name = name;
	t->age = age;

	return t;
}

size_t _student_objectFields[] = { offsetof(Student, teacher) };
MetaData _student_metadata = { "Student", _student_objectFields, 1 };

Student* NewStudent(const char* name, int age)
{
	Student* s = (Student*)malloc(sizeof(Student));

	InitObj((Object*)s);
	s->teacher.obj = 0;

	s->metaData = _student_metadata;

	s->name = name;
	s->age = age;

	return s;
}

void GiveSomeStudents(Teacher* t_para)
{
	Object* obj;
	Ref* ref;


	// 参数直接使用 Teacher* t_para ， 而不用 Teacher_Ref t ，因为外层函数会保持对 t_para 的引用，可以保证 t_para 不被回收。
	// 如果是把外层函数的 Teacher_Ref t 按值传进来， t.self_in_RefByWhomList 还是外层函数的局部变量的 Teacher_Ref t 的 t.self_in_RefByWhomList ，
	// 在这里要再声明一个 Teacher_Ref t ， 再用下面的代码包装一次，这就没必要了 。
	// Teacher_Ref t;    // 这个 t 对应参数 t_para
	// ref = (Ref*)& t;
	// 根引用指向对象(ref, (Object*)t_para);


	// Student s1 = new Student("小明", 22);
	obj = (Object*)NewStudent("小明", 22);
	AddToObjectList(obj);
	Student_Ref s1;
	ref = (Ref*) & s1;
	根引用指向对象(ref, obj);

	// Student s2 = new Student("小红", 20);
	obj = (Object*)NewStudent("小红", 20);
	AddToObjectList(obj);
	Student_Ref s2;
	ref = (Ref*) & s2;
	根引用指向对象(ref, obj);

	// t.student1 = s1;
	ref = (Ref*) & t_para->student1;
	obj = (Object*)s1.obj;
	字段引用指向对象(ref, obj);

	// t.student2 = s2;
	ref = (Ref*) & t_para->student2;
	obj = (Object*)s2.obj;
	字段引用指向对象(ref, obj);

	// s1.teacher = t;
	ref = (Ref*)& s1.obj->teacher;
	obj = (Object*)t_para;
	字段引用指向对象(ref, obj);

	// s2.teacher = t;
	ref = (Ref*)& s2.obj->teacher;
	obj = (Object*)t_para;
	字段引用指向对象(ref, obj);


	//  局部变量 超出作用域
	ref = (Ref*)& s1;
	根引用取消指向对象(ref);
	ref = (Ref*)& s2;
	根引用取消指向对象(ref);
}

// 返回值 示例
Student* GiveAStudent()
{
	Object* obj;
	Ref* ref;

	obj = (Object*)NewStudent("小白", 24);
	AddToObjectList(obj);
	Student_Ref s;
	ref = (Ref*)& s;
	根引用指向对象(ref, obj);

	
	//  ……

	
	// 准备返回， 把 返回值 放到 obj
	obj = (Object*)s.obj;

	//  局部变量 超出作用域
	ref = (Ref*)& s;
	根引用取消指向对象(ref);

	return (Student*)obj;
}

void DoSomeThing()
{
	Object* obj;
	Ref* ref;

	// Teacher t = new Teacher("小刚", 30);
	obj = (Object*)NewTeacher("小刚", 30);
	AddToObjectList(obj);
	Teacher_Ref t;
	ref = (Ref*)&t;
	根引用指向对象(ref, obj);


	GiveSomeStudents(t.obj);


	// Student s = GiveAStudent();
	obj = (Object*)GiveAStudent();
	Student_Ref s;
	ref = (Ref*)&s;
	根引用指向对象(ref, obj);


	GC_Work();


	//  局部变量 超出作用域
	ref = (Ref*)& t;
	根引用取消指向对象(ref);
	ref = (Ref*)& s;
	根引用取消指向对象(ref);
}



int main()
{

	//std::cout << "Hello World!\n";


	DoSomeThing();

	GC_Work();

}



// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
