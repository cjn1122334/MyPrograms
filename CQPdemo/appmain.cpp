/*
* CoolQ Demo for VC++ 
* Api Version 9
* Written by Coxxs & Thanks for the help of orzFly
*/

#include "stdafx.h"
#include "string"
#include "cqp.h"
#include "appmain.h" //Ӧ��AppID����Ϣ������ȷ��д�������Q�����޷�����
#include <ctime>
#include<vector>

using namespace std;

#define CMDNUM 2

int ac = -1; //AuthCode ���ÿ�Q�ķ���ʱ��Ҫ�õ�
bool enabled = false;
int lasttime = time(0);
char sym[5] = { 'd','/','*','-','+' };

int roll_dice(int face);
int request_reg(int *reg, bool *reg_use, int reg_num);
bool ini_cal(char *a0, int num0, int& res_f,vector<char>& outString);
bool calculate(char *a0, int num0, int reg_num, int *reg, bool *reg_use, int& res_f);

/* 
* ����Ӧ�õ�ApiVer��Appid������󽫲������
*/
CQEVENT(const char*, AppInfo, 0)() {
	return CQAPPINFO;
}


/* 
* ����Ӧ��AuthCode����Q��ȡӦ����Ϣ��������ܸ�Ӧ�ã���������������������AuthCode��
* ��Ҫ�ڱ��������������κδ��룬���ⷢ���쳣���������ִ�г�ʼ����������Startup�¼���ִ�У�Type=1001����
*/
CQEVENT(int32_t, Initialize, 4)(int32_t AuthCode) {
	ac = AuthCode;
	return 0;
}


/*
* Type=1001 ��Q����
* ���۱�Ӧ���Ƿ����ã������������ڿ�Q������ִ��һ�Σ���������ִ��Ӧ�ó�ʼ�����롣
* ��Ǳ�Ҫ����������������ش��ڡ���������Ӳ˵������û��ֶ��򿪴��ڣ�
*/
CQEVENT(int32_t, __eventStartup, 0)() {
	int ttt = time(NULL);
	srand(ttt);
	return 0;
}


/*
* Type=1002 ��Q�˳�
* ���۱�Ӧ���Ƿ����ã������������ڿ�Q�˳�ǰִ��һ�Σ���������ִ�в���رմ��롣
* ������������Ϻ󣬿�Q���ܿ�رգ��벻Ҫ��ͨ���̵߳ȷ�ʽִ���������롣
*/
CQEVENT(int32_t, __eventExit, 0)() {

	return 0;
}

/*
* Type=1003 Ӧ���ѱ�����
* ��Ӧ�ñ����ú󣬽��յ����¼���
* �����Q����ʱӦ���ѱ����ã�����_eventStartup(Type=1001,��Q����)�����ú󣬱�����Ҳ��������һ�Ρ�
* ��Ǳ�Ҫ����������������ش��ڡ���������Ӳ˵������û��ֶ��򿪴��ڣ�
*/
CQEVENT(int32_t, __eventEnable, 0)() {
	enabled = true;
	return 0;
}


/*
* Type=1004 Ӧ�ý���ͣ��
* ��Ӧ�ñ�ͣ��ǰ�����յ����¼���
* �����Q����ʱӦ���ѱ�ͣ�ã��򱾺���*����*�����á�
* ���۱�Ӧ���Ƿ����ã���Q�ر�ǰ��������*����*�����á�
*/
CQEVENT(int32_t, __eventDisable, 0)() {
	enabled = false;
	return 0;
}


/*
* Type=21 ˽����Ϣ
* subType �����ͣ�11/���Ժ��� 1/��������״̬ 2/����Ⱥ 3/����������
*/
CQEVENT(int32_t, __eventPrivateMsg, 24)(int32_t subType, int32_t msgId, int64_t fromQQ, const char *msg, int32_t font) {

	char cmd[CMDNUM][10] = {
		"d",
		"help"
		};
	char helpDoc[] = "ָ���б�\n.d \n.help";
	int CMDMAXLENTH = 4;
	srand(lasttime);
	if (msg[0] == '.')
	{
		char *wholeCmd = new char[strlen(msg)+1];
		strcpy(wholeCmd, msg);
		char *a = wholeCmd, *b = new char[CMDMAXLENTH];
		bool isAvlCmd = false;
		int i = 1, cmdNum = -1;
		while (i <= CMDMAXLENTH && wholeCmd[i] != ' ')
		{
			b[i - 1] = wholeCmd[i];
			i++;
		}
		b[i - 1] = 0;
		for (int i0 = 0; i0 < CMDNUM; i0++)
		{
			if (strcmp(b, cmd[i0]) == 0) { isAvlCmd = true; cmdNum = i0; break; }
		}
		if (isAvlCmd)
		{
			switch (cmdNum)
			{
			case 0://.d
			{
				char *par1 = wholeCmd + 2, *p = new char[40], p0[10];
				if (*par1 == 0)
				{
					int a = roll_dice(100);
					strcpy(p, "1d100 = ");
					itoa(a, p0, 10);
					strcat(p, p0);
					int mesid = CQ_sendPrivateMsg(ac, fromQQ, p);
					lasttime = a + mesid;
				}
				else
				{
					par1++;
					char *par2 = par1;
					while (*par2 != ' '&&*par2 != '\0')
					{
						par2++;
					}
					int res;
					vector<char> outString;
					bool isValid = ini_cal(par1, par2 - par1, res, outString);
					outString.push_back('\0');
					if (!isValid)
					{
						int t = outString.size() + 20;
						char *outmes = new char[t];
						copy(outString.begin(), outString.end(), outmes);
						itoa(res, p, 10);
						strcat(outmes, "=");
						strcat(outmes, p);
						int mesid=CQ_sendPrivateMsg(ac, fromQQ, outmes);
						lasttime = res + mesid;
						delete[]outmes;
					}
				}
				delete[]p;
				break;
			}
			case 1://.help
				CQ_sendPrivateMsg(ac, fromQQ, helpDoc);
				break;
			}
		}
		delete[]wholeCmd;
		delete[]b;
		
	}
	//CQ_sendPrivateMsg(ac,fromQQ, msg);
	//���Ҫ�ظ���Ϣ������ÿ�Q�������ͣ��������� return EVENT_BLOCK - �ضϱ�����Ϣ�����ټ�������  ע�⣺Ӧ�����ȼ�����Ϊ"���"(10000)ʱ������ʹ�ñ�����ֵ
	//������ظ���Ϣ������֮���Ӧ��/�������������� return EVENT_IGNORE - ���Ա�����Ϣ
	return EVENT_BLOCK;
}


/*
* Type=2 Ⱥ��Ϣ
*/
CQEVENT(int32_t, __eventGroupMsg, 36)(int32_t subType, int32_t msgId, int64_t fromGroup, int64_t fromQQ, const char *fromAnonymous, const char *msg, int32_t font) {
	char cmd[CMDNUM][10] = {
		"d",
		"help"
	};
	char helpDoc[] = "ָ���б�\n.d \n.help";
	int CMDMAXLENTH = 4;
	srand(lasttime);
	if (msg[0] == '.')
	{
		char *wholeCmd = new char[strlen(msg) + 1];
		strcpy(wholeCmd, msg);
		char *a = wholeCmd, *b = new char[CMDMAXLENTH];
		bool isAvlCmd = false;
		int i = 1, cmdNum = -1;
		while (i <= CMDMAXLENTH && wholeCmd[i] != ' ')
		{
			b[i - 1] = wholeCmd[i];
			i++;
		}
		b[i - 1] = 0;
		for (int i0 = 0; i0 < CMDNUM; i0++)
		{
			if (strcmp(b, cmd[i0]) == 0) { isAvlCmd = true; cmdNum = i0; break; }
		}
		if (isAvlCmd)
		{
			switch (cmdNum)
			{
			case 0://.d
			{
				char *par1 = wholeCmd + 2, *p = new char[strlen(msg) + 20], p0[3];
				if (*par1 == 0)
				{
					int a = roll_dice(100);
					strcpy(p, "1d100 = ");
					itoa(a, p0, 10);
					strcat(p, p0);
					CQ_sendGroupMsg(ac, fromGroup, p);
				}
				else
				{
					par1++;
					char *par2 = par1;
					while (*par2 != ' '&&*par2 != '\0')
					{
						par2++;
					}
					int res;
					vector<char> outString;
					bool isValid = ini_cal(par1, par2 - par1, res, outString);
					outString.push_back('\0');
					if (!isValid)
					{
						int t = outString.size() + 50;
						char *outmes = new char[t];
						copy(outString.begin(), outString.end(), outmes);
						itoa(res, p, 10);
						strcat(outmes, "=");
						strcat(outmes, p);
						strcat(outmes, "\n");
						char *atqq = new char[15];
						strcat(outmes, "[CQ:at,qq=");
						itoa(fromQQ, atqq, 10);
						strcat(outmes, atqq);
						strcat(outmes, "]");
						//strcat(outmes, CQ_getGroupMemberInfoV2(ac, fromGroup, fromQQ, true));
						int mesid=CQ_sendGroupMsg(ac, fromGroup, outmes);
						lasttime = mesid + res;
						delete[]outmes;
					}
				}
				delete[]p;
				break;
			}
			case 1://.help
				CQ_sendGroupMsg(ac, fromGroup, helpDoc);
				break;
			}
		}
		delete[]wholeCmd;
		delete[]b;

	}
	return EVENT_BLOCK; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=4 ��������Ϣ
*/
CQEVENT(int32_t, __eventDiscussMsg, 32)(int32_t subType, int32_t msgId, int64_t fromDiscuss, int64_t fromQQ, const char *msg, int32_t font) {

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=101 Ⱥ�¼�-����Ա�䶯
* subType �����ͣ�1/��ȡ������Ա 2/�����ù���Ա
*/
CQEVENT(int32_t, __eventSystem_GroupAdmin, 24)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t beingOperateQQ) {

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=102 Ⱥ�¼�-Ⱥ��Ա����
* subType �����ͣ�1/ȺԱ�뿪 2/ȺԱ���� 3/�Լ�(����¼��)����
* fromQQ ������QQ(��subTypeΪ2��3ʱ����)
* beingOperateQQ ������QQ
*/
CQEVENT(int32_t, __eventSystem_GroupMemberDecrease, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, int64_t beingOperateQQ) {

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=103 Ⱥ�¼�-Ⱥ��Ա����
* subType �����ͣ�1/����Ա��ͬ�� 2/����Ա����
* fromQQ ������QQ(������ԱQQ)
* beingOperateQQ ������QQ(����Ⱥ��QQ)
*/
CQEVENT(int32_t, __eventSystem_GroupMemberIncrease, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, int64_t beingOperateQQ) {

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=201 �����¼�-���������
*/
CQEVENT(int32_t, __eventFriend_Add, 16)(int32_t subType, int32_t sendTime, int64_t fromQQ) {

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=301 ����-�������
* msg ����
* responseFlag ������ʶ(����������)
*/
CQEVENT(int32_t, __eventRequest_AddFriend, 24)(int32_t subType, int32_t sendTime, int64_t fromQQ, const char *msg, const char *responseFlag) {

	//CQ_setFriendAddRequest(ac, responseFlag, REQUEST_ALLOW, "");

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=302 ����-Ⱥ���
* subType �����ͣ�1/����������Ⱥ 2/�Լ�(����¼��)������Ⱥ
* msg ����
* responseFlag ������ʶ(����������)
*/
CQEVENT(int32_t, __eventRequest_AddGroup, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, const char *msg, const char *responseFlag) {

	//if (subType == 1) {
	//	CQ_setGroupAddRequestV2(ac, responseFlag, REQUEST_GROUPADD, REQUEST_ALLOW, "");
	//} else if (subType == 2) {
	//	CQ_setGroupAddRequestV2(ac, responseFlag, REQUEST_GROUPINVITE, REQUEST_ALLOW, "");
	//}

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}

/*
* �˵������� .json �ļ������ò˵���Ŀ��������
* �����ʹ�ò˵������� .json ���˴�ɾ�����ò˵�
*/
CQEVENT(int32_t, __menuA, 0)() {
	MessageBoxA(NULL, "����menuA�����������봰�ڣ����߽�������������", "" ,0);
	return 0;
}

CQEVENT(int32_t, __menuB, 0)() {
	MessageBoxA(NULL, "����menuB�����������봰�ڣ����߽�������������", "" ,0);
	return 0;
}

int roll_dice(int face)
{
	if (face < 1000)
	{
		return rand() % face + 1;
	}
	double sum = 0.0;
	for (int i = 0; i < log10(face) + 1; i++)
	{
		sum += rand() % 10;
		sum /= 10;
	}
	return (int)(sum*face) + 1;
}


int request_reg(int *reg, bool *reg_use,int reg_num)
{
	int i;
	for (i = 0; i < reg_num; i++)
	{
		if (reg_use[i] == 0)
		{
			reg_use[i] = 1;
			return i;
		}
	}
	return -1;
}

bool ini_cal(char *a0, int num0, int& res_f, vector<char>& outString)
{
	int reg_num = num0 / 3 + 1;
	int *reg = new int[reg_num] {0};
	bool *reg_use = new bool[reg_num] {0};
	char *curr = a0, *last = a0;
	vector<char> wholeString;
	while (curr - a0 < num0)
	{
		while (*curr != 'd'&&curr - a0 < num0) { curr++; }
		if (curr - a0 >= num0)
		{
			wholeString.insert(wholeString.end(), last, curr);
			break;
		}
		char *prev = curr - 1, *next = curr + 1;
		int sum1 = 0, sum2 = 0, sum = 0;
		while (*prev >= '0'&&*prev <= '9')
		{
			sum1 += (*prev - '0')*pow(10, curr - 1 - prev);
			prev--;
		}
		if (curr - prev == 1) { sum1 = 1; }
		while (*next >= '0'&&*next <= '9')
		{
			sum2 *= 10;
			sum2 += (*next - '0');
			next++;
		}
		int j = request_reg(reg, reg_use, reg_num);
		if (j < 0) { break; }
		vector<char> insertString;
		insertString.push_back('(');
		for (int i = 0; i < sum1; i++)
		{
			int t = roll_dice(sum2);
			sum += t;
			char *p = new char[log10(t) + 2];
			itoa(t, p, 10);
			char *q = p;
			while (*q != '\0') { q++; }
			insertString.insert(insertString.end(), p, q);
			if (i == sum1 - 1) { continue; }
			insertString.push_back(',');
		}
		insertString.push_back(')');
		char* clear = prev + 1;
		while (next - clear >= 1)
		{
			*clear = j + 'e';
			clear++;
		}
		reg_use[j] = 1;
		reg[j] = sum;
		if (last <= prev)
		{
			wholeString.insert(wholeString.end(), last, prev + 1);
		}
		wholeString.insert(wholeString.end(), insertString.begin(), insertString.end());
		last = next;
		curr = next;
	}
	bool returnValue = calculate(a0, num0, reg_num, reg, reg_use, res_f);
	delete[]reg;
	delete[]reg_use;
	outString = wholeString;
	return returnValue;
}

bool calculate(char *a0, int num0,int reg_num,int *reg,bool *reg_use,int& res_f)//integer version
{
	char *p = a0, *q = a0, *r = a0, *s = a0;
	int step = 0, i = 0, j = 0, k = 0;
	int res = 0;
	while (q - a0 < num0)
	{
		if (*q == '(')
		{
			i = 0;
			r = q + 1;
			while ((i != 0 || *r != ')'))
			{
				if (r - a0 >= num0)
					return 1;
				if (*r == '(')
					i++;
				else if (*r == ')')
					i--;
				r++;
			}
			j = request_reg(reg, reg_use, reg_num);
			if (j < 0)
				return 1;
			calculate(q + 1, r - q - 1, reg_num, reg, reg_use, res_f);
			reg[j] = res_f;
			res_f = 0;
			p = q;
			while (p <= r)
			{
				*p = 'e' + j;
				p++;
			}
		}
		q++;
	}
	q = a0;
	p = a0;
	r = a0;
	while (true)
	{
		if (*q<'0' || *q>'9')
		{
			bool flag = true;
			for (int i = 0; i < 5; i++)
			{
				if (*q == sym[i] || (*q - 'e' < reg_num&&*q - 'e' >= 0)) { flag = false; break; }
			}
			if (flag) { return 1; }
		}
		if (*q == sym[step])
		{
			r = q;
			s = q;
			while (r - a0 < num0)
			{
				r++;
				k = 0;
				for (i = step; i < 5; i++)
				{
					if (*r == sym[i])
						k = 1;
				}
				if (k == 1)
					break;
			}
			while (s > a0)
			{
				s--;
				k = 0;
				for (i = step; i < 5; i++)
				{
					if (*s == sym[i])
					{
						k = 1;
						s++;
					}
				}
				if (k == 1)
					break;
			}
			if (r - q <= 1)
				return 1;
			j = request_reg(reg, reg_use, reg_num);
			if (j < 0)
				return 1;
			reg[j] = 0;
			if (*s >= '0'&&*s <= '9')
			{
				p = q;
				//dot = q;
				//dotFlag = 0;
				do
				{
					p--;
					/*if (*p == '.'&&dotFlag == 0)
					{
						dot = p;
						reg[j] *= my_pow(10, -(q - dot - 1));
						dotFlag = 1;
					}
					else if (*p == '.'&&dotFlag == 1)
						return 1;
					else*/
					reg[j] += (*p - '0')*pow(10, q - p - 1);
				} while (p != s);
			}
			else if (s == q)
			{
				reg[j] = 1;
			}
			else
			{
				reg[j] = reg[*s - 'e'];
				reg_use[*s - 'e'] = 0;
			}
			if (*(r - 1) >= '0'&&*(r - 1) <= '9')
			{
				p = r;
				//dot = r;
				//dotFlag = 0;
				res = 0;
				do
				{
					p--;
					/*if (*p == '.'&&dotFlag == 0)
					{
						dot = p;
						res *= my_pow(10, -(r - dot - 1));
						dotFlag = 1;
					}
					else if (*p == '.'&&dotFlag == 1)
						return 1;
					else*/
					res += (*p - '0')*pow(10, r - p - 1);
				} while (p != q + 1);
			}
			else
			{
				res = reg[*(r - 1) - 'e'];
				reg_use[*(r - 1) - 'e'] = 0;
			}
			int i0 = reg[j];
			switch (step)
			{
			case 0:
				reg[j] = 0;
				for (int i = 0; i < i0; i++)
				{
					reg[j] += roll_dice(res);
				}
				break;
			case 1:
				if (res == 0)
					return 1;
				reg[j] /= res;
				break;
			case 2:
				reg[j] *= res;
				break;
			case 3:
				reg[j] -= res;
				break;
			case 4:
				reg[j] += res;
				break;
			}
			p = s;
			while (p != r)
			{
				*p = j + 'e';
				p++;
			}
		}
		q++;
		if (q - a0 == num0)
		{
			step++;
			q = a0;
		}
		if (step == 5)
			break;
	}
	if (*q >= '0'&&*q <= '9')
	{
		res_f = 0;
		while (q - a0 != num0)
		{
			res_f *= 10;
			res_f += *q - '0';
			q++;
		}
		return 0;
	}
	res_f = reg[*q - 'e'];
	reg_use[*q - 'e'] = 0;
	return 0;
}
