// Demo.cpp : ���ļ����� "main" ����������ִ�н��ڴ˴���ʼ��������
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


	// ����ֱ��ʹ�� Teacher* t_para �� ������ Teacher_Ref t ����Ϊ��㺯���ᱣ�ֶ� t_para �����ã����Ա�֤ t_para �������ա�
	// ����ǰ���㺯���� Teacher_Ref t ��ֵ�������� t.self_in_RefByWhomList ������㺯���ľֲ������� Teacher_Ref t �� t.self_in_RefByWhomList ��
	// ������Ҫ������һ�� Teacher_Ref t �� ��������Ĵ����װһ�Σ����û��Ҫ�� ��
	// Teacher_Ref t;    // ��� t ��Ӧ���� t_para
	// ref = (Ref*)& t;
	// ������ָ�����(ref, (Object*)t_para);


	// Student s1 = new Student("С��", 22);
	obj = (Object*)NewStudent("С��", 22);
	AddToObjectList(obj);
	Student_Ref s1;
	ref = (Ref*) & s1;
	������ָ�����(ref, obj);

	// Student s2 = new Student("С��", 20);
	obj = (Object*)NewStudent("С��", 20);
	AddToObjectList(obj);
	Student_Ref s2;
	ref = (Ref*) & s2;
	������ָ�����(ref, obj);

	// t.student1 = s1;
	ref = (Ref*) & t_para->student1;
	obj = (Object*)s1.obj;
	�ֶ�����ָ�����(ref, obj);

	// t.student2 = s2;
	ref = (Ref*) & t_para->student2;
	obj = (Object*)s2.obj;
	�ֶ�����ָ�����(ref, obj);

	// s1.teacher = t;
	ref = (Ref*)& s1.obj->teacher;
	obj = (Object*)t_para;
	�ֶ�����ָ�����(ref, obj);

	// s2.teacher = t;
	ref = (Ref*)& s2.obj->teacher;
	obj = (Object*)t_para;
	�ֶ�����ָ�����(ref, obj);


	//  �ֲ����� ����������
	ref = (Ref*)& s1;
	������ȡ��ָ�����(ref);
	ref = (Ref*)& s2;
	������ȡ��ָ�����(ref);
}

// ����ֵ ʾ��
Student* GiveAStudent()
{
	Object* obj;
	Ref* ref;

	obj = (Object*)NewStudent("С��", 24);
	AddToObjectList(obj);
	Student_Ref s;
	ref = (Ref*)& s;
	������ָ�����(ref, obj);

	
	//  ����

	
	// ׼�����أ� �� ����ֵ �ŵ� obj
	obj = (Object*)s.obj;

	//  �ֲ����� ����������
	ref = (Ref*)& s;
	������ȡ��ָ�����(ref);

	return (Student*)obj;
}

void DoSomeThing()
{
	Object* obj;
	Ref* ref;

	// Teacher t = new Teacher("С��", 30);
	obj = (Object*)NewTeacher("С��", 30);
	AddToObjectList(obj);
	Teacher_Ref t;
	ref = (Ref*)&t;
	������ָ�����(ref, obj);


	GiveSomeStudents(t.obj);


	// Student s = GiveAStudent();
	obj = (Object*)GiveAStudent();
	Student_Ref s;
	ref = (Ref*)&s;
	������ָ�����(ref, obj);


	GC_Work();


	//  �ֲ����� ����������
	ref = (Ref*)& t;
	������ȡ��ָ�����(ref);
	ref = (Ref*)& s;
	������ȡ��ָ�����(ref);
}



int main()
{

	//std::cout << "Hello World!\n";


	DoSomeThing();

	GC_Work();

}



// ���г���: Ctrl + F5 ����� >����ʼִ��(������)���˵�
// ���Գ���: F5 ����� >����ʼ���ԡ��˵�

// ����ʹ�ü���: 
//   1. ʹ�ý��������Դ�������������/�����ļ�
//   2. ʹ���Ŷ���Դ�������������ӵ�Դ�������
//   3. ʹ��������ڲ鿴���������������Ϣ
//   4. ʹ�ô����б��ڲ鿴����
//   5. ת������Ŀ��>���������Դ����µĴ����ļ�����ת������Ŀ��>�����������Խ����д����ļ���ӵ���Ŀ
//   6. ��������Ҫ�ٴδ򿪴���Ŀ����ת�����ļ���>���򿪡�>����Ŀ����ѡ�� .sln �ļ�
