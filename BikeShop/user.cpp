// bikeshop
// customer
#include<stdio.h>
#include<unordered_map>
#include<queue>
#include<string>
#include<algorithm>
using namespace std;

int n, duraTime, cCnt;
struct BikeShop {
	priority_queue<int, vector<int>, greater<>> pq; //bike's operation time 
	int dTime; //delivery Time
	queue<int> newBike; //new bike Time
} bs[103];
struct Customer {
	int bikeTime, rentTime, eTime; // ticket end time
}cust[10003];
unordered_map<string, int> hmap;

#define MAXN		100
#define MAXL		8

void init(int N, int durableTime, int deliveryTimes[MAXN])
{
	for (int i = 0; i < cCnt; ++i)cust[i] = {};
	n = N; duraTime = durableTime; cCnt = 0;
	for (int i = 0; i < N; ++i) {
		while (bs[i].pq.size())bs[i].pq.pop();
		while (bs[i].newBike.size())bs[i].newBike.pop();
		bs[i].dTime = deliveryTimes[i];
	}
	hmap.clear();
}

void addBicycle(int cTimestamp, int pID, int bicycleNum)
{
	for (int i = 0; i < bicycleNum; ++i) {
		bs[pID].pq.push(0);
	}
}

// id 할당 & 반환
int getId(char str[]) {
	if (!hmap.count(str)) hmap[str] = cCnt++;
	return hmap[str];
}

void buyTicket(int cTimestamp, char uName[MAXL], int validTime)
{
	int id = getId(uName);

	if (cust[id].eTime <= cTimestamp) {
		cust[id].eTime = cTimestamp + validTime;
	}
	else {
		cust[id].eTime += validTime;
	}
}

void updateNewBike(int cTimestamp, int pID) {
	while (bs[pID].newBike.size()
		&& bs[pID].newBike.front() <= cTimestamp)
	{
		bs[pID].newBike.pop();
		bs[pID].pq.push(0);
	}
}

int rentBicycle(int cTimestamp, char uName[MAXL], int pID)
{
	updateNewBike(cTimestamp, pID);

	int id = getId(uName);

	if (cust[id].rentTime != 0)return -1;
	if (!bs[pID].pq.size())return -1;
	if (cust[id].eTime <= cTimestamp)return -1;

	//bs update
	int minTime = bs[pID].pq.top(); bs[pID].pq.pop();
	//cust update
	cust[id].bikeTime = minTime;
	cust[id].rentTime = cTimestamp;

	return minTime;
}

int returnBicycle(int cTimestamp, char uName[MAXL], int pID)
{
	int id = getId(uName);
	int ret = 0;
	Customer cc = cust[id];

	if (!cc.rentTime)ret = -1;
	else if (cc.eTime > cTimestamp)ret = 0;
	else ret = cTimestamp - cc.eTime + 1;

	if (ret != -1)
	{//cust update
		cust[id].bikeTime = 0;
		cust[id].rentTime = 0;

		int operTime = cc.bikeTime + cTimestamp - cc.rentTime;
		if (operTime > duraTime) {
			//자전거 폐기 & 새자전거 주문
			bs[pID].newBike.push(cTimestamp + bs[pID].dTime);
		}
		else {
			bs[pID].pq.push(operTime);
		}
	}
	return ret;
}