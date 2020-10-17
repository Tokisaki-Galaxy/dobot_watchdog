// dobot_watchdog.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <fstream>
#include <sstream>
#include <iostream>
#include <Windows.h>
#include <corecrt_io.h>

#pragma comment(linker, "/subsystem:windows /entry:mainCRTStartup" )

using namespace std;

string runmode,keyname,pertime,foodpath;

void readTxt(string file)
{
	ifstream infile;
	infile.open(file.data());   //将文件流对象与文件连接起来 
	if (!infile.is_open()) {
		MessageBox(NULL, "无法打开setting.ini文件，程序退出", "ERROR", MB_OK);
		ofstream fout;
		fout.open("setting.ini", ios::app);
		fout << "运行模式：0是找到报警，1是没找到报警\n关键词（字）：要找的字符串\n循环时间：多少毫秒循环检查一次\n目标文件路径" << endl;
		exit(1);
	}
	getline(infile, runmode);	//0是找到报警，1是没找到报警
	getline(infile, keyname);	//要找的字符串
	getline(infile, pertime);	//多少毫秒循环一次
	getline(infile, foodpath);	//目标文件路径
	infile.close();
	return;
}

int main()
{
	if (!_access("killdog.bat", 0)) {
		ofstream fout;
		fout.open("setting.ini", ios::app);
		fout << "taskkill /im dobot_watchdog.exe /t /f" << endl;
	}

	readTxt("setting.ini");

	string tmp,sourceData;
	ifstream infile;

	while (true) {
		infile.open(foodpath.data());   //将文件流对象与文件连接起来 
		if (!infile.is_open()) {
			MessageBox(NULL, "无法打开目标文件，程序退出", "ERROR", MB_OK);
			exit(1);
		}
		while (getline(infile, tmp)) {
			sourceData.append(tmp);
		}
		infile.close();

		if (sourceData.find(keyname) != sourceData.npos)  //如果没找到，返回一个特别的标志c++中用npos表示，我这里npos取值是4294967295，
		{
			if (stoi(runmode) == 0) {
				tmp = "已经找到字符串：\"" + keyname + "\"";
				MessageBox(NULL, tmp.c_str(), "ERROR", MB_OK);
				//cout << tmp.c_str() << endl;
			}
		}
		else
		{
			if (stoi(runmode) == 1) {
				tmp = "没有找到字符串：\"" + keyname + "\"";
				MessageBox(NULL, tmp.c_str(), "ERROR", MB_OK);
				//cout << tmp.c_str() << endl;
			}
		}
		sourceData.clear();
		Sleep(stoi(pertime));
	}
	return 0;
}
