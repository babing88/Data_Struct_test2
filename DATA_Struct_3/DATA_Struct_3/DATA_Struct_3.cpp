#include "pch.h"
#include <iostream>
#include<time.h>
using namespace std;
#define MAXSIZE 3
#define FEE 3//每秒3元
struct Car {
	int IDcard;
	clock_t start;
	Car* next;
};
//停车栈
struct ParkStack {
	Car ** base;//使用二级指针
	int front;
};
//临时栈
struct TempStack {
	Car ** base;
	int front;
};
//便道队列
struct SideWalkQueue
{
	Car * front;
	Car * rear;
};
//停车场栈初始化
bool initParkStack(ParkStack & p) {
	p.base = new Car*[MAXSIZE];
	p.front = 0;
	if (p.base)return true;//空间分配成功
	return false;
}
//临时栈初始化
bool initTempStack(TempStack & t) {
	t.base = new Car*[MAXSIZE - 1];
	t.front = 0;
	if (t.base)return true;
	return false;
}
//便道队列初始化
bool initSideWalkQueue(SideWalkQueue & s) {
	s.front = s.rear = new Car;
	s.front->next = NULL;
	return true;
}
//判断停车场是否已满
bool ParkFull(ParkStack p) {
	if (p.front == MAXSIZE)return true;
	return false;
}
//判断停车场是否为空
bool ParkEmpty(ParkStack p) {
	if (p.front)return false;
	return true;
}
//停车栈入栈
void ParkPush(ParkStack & p,Car *car) {
	p.base[p.front++] = car;
}
//停车栈出栈
Car * ParkPop(ParkStack & p) {
	Car *car = p.base[--p.front];
	return car;
}
//临时栈入栈
void TempPush(TempStack & t, Car * car) {
	t.base[t.front++] = car;
}
//临时栈出栈
Car * TempPop(TempStack & t) {
	Car * car = t.base[--t.front];
	return car;
}
//临时栈是否为空
bool TempEmpty(TempStack t) {
	if (t.front == 0)	return true;
	return false;
}
//便道队列入队
void enSideQueue(SideWalkQueue &s,Car * car) {
	s.rear->next = car;
	car->next = NULL;
	s.rear = car;
}
//便道队列出队
Car * outSideQueue(SideWalkQueue &s) {
	Car * car=s.front->next;
	if (car == s.rear) {
		s.rear = s.front;
	}
	s.front->next = car->next;
	return car;
}
//便道队列是否为空
bool SideEmpty(SideWalkQueue  s) {
	if (s.front == s.rear)return true;
	return false;
}
//车辆准备进入停车场
void enPark(ParkStack & p,SideWalkQueue &s, int IdCard) {
	Car * car = new Car;
	car->IDcard = IdCard;
	car->next = NULL;
	if (!ParkFull(p)) {//如果停车栈未满，将该车辆放入栈尾
		car->start = clock();
		ParkPush(p, car);
	}
	else {
		s.rear->next = car;
		s.rear = s.rear->next;
	}
}
//车辆退出停车场
void outPark(ParkStack &p, TempStack &t, SideWalkQueue &s,int IdCard) {
	Car * car;
	for (int i = 0; i < p.front; i++)
	{
		car = ParkPop(p);
		if (car->IDcard == IdCard) {//该车辆为所要出停车场的车辆
			clock_t finish = clock();//获取该车辆驶出停车场时的时间
			double fee = ((finish - car->start) / CLOCKS_PER_SEC) * FEE;
			cout << car->IDcard << "退出停车场" << "需缴费" << fee << "元";
			delete car;
		}
		else {
			TempPush(t, car);//如果栈顶车辆不是所要出停车场的车辆，将其压入临时栈道当中
		}
	}
	while (!TempEmpty(t))//当临时栈道不为空时，依次将栈顶元素压入停车栈中
	{
		ParkPush(p, TempPop(t));
	}
	if (!SideEmpty(s)) {//如果便道当中有等待的车辆，将其压入停车栈中
		ParkPush(p, outSideQueue(s));
	}
}
//释放停车栈中元素所占用的内存空间
void releasePark(ParkStack & p) {
	Car *car;
	cout << "停车场下班，所有车辆清空" << endl;
	while (p.front)
	{
		car = ParkPop(p);
		cout << car->IDcard << "驶出停车场,需缴纳" << ((clock() - car->start) / CLOCKS_PER_SEC)*FEE << "元"<<endl;
		delete car;
	}
}
//释放便道队列中元素所占用的空间
void releaseSide(SideWalkQueue &s) {
	Car *car;
	cout << "便道清空" << endl;
	while (s.front->next)
	{
		car=outSideQueue(s);
		cout << car->IDcard << "驶出便道" << endl;
		delete car;
	}
}
void printSideCar(SideWalkQueue s) {
	Car* car = s.front->next;
	cout << "便道中的车辆" << endl;
	while (car)
	{
		cout << car->IDcard << endl;
		car = car->next;
	}
}
void printParkCar(ParkStack p) {
	cout << "停车场中的车辆"<<endl;
	for (int i = 0; i < p.front; i++)
	{
		cout << p.base[i]->IDcard << endl;
	}
}
void toString(ParkStack p, SideWalkQueue s) {
	printParkCar(p);
	printSideCar(s);
}
int main()
{
	ParkStack p;
	TempStack t;
	SideWalkQueue s;
	initParkStack(p);
	initTempStack(t);
	initSideWalkQueue(s);
	int i = 1;
	int IdCard;
	while (i)
	{
		cout << "请选择所要使用的操作：1(车辆进入停车场)，2(车辆退出停车场)，3(打印目前停车场中信息),0(退出)" << endl;
		cin >> i;
		switch (i)
		{
		case 1:
			cout << "请输入车辆的车牌号:";
			cin >> IdCard;
			enPark(p, s, IdCard);
			break;
		case 2:
			cout << "请输入车辆的车牌号:";
			cin >> IdCard;
			outPark(p, t, s,IdCard);
			break;
		case 3:
			toString(p, s);
			break;
		default:
			break;
		}
	}
	releasePark(p);
	releaseSide(s);
	delete p.base;
	delete s.front;
}