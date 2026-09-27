#include<iostream>
#include<fstream>
#include<string>
#include<windows.h>
#include<optional>
#include<vector>
#include<sstream>
#include<cmath>
#include<cctype>
#include<ctime>
#include<filesystem>
#include <iomanip>
#include<system_error>
#include <sys/stat.h>

using namespace std;
namespace fs = std::filesystem;

string a; 
int Num;//总数 
struct Goods{
	double pr;
	string name;
};
Goods goods[2000];
int T[2000];

void Record(ostream& out){
	int Rp=0;
	for(int i=1;i<=Num;i++)
		if(T[i]>0)
			Rp++;
	out<<Rp<<"件"<<endl;
	out<<"    "<<"No.    Items    Total"<<endl;
	for(int i=1;i<=Num;i++){
		if(T[i]>0)
			out<<"    "<<setfill('0')<<setw(3)<<i;
			out<<setfill(' ')<<"    "<<left<<setw(9)<<goods[i].name;
			out<<T[i]<<endl;}
	for(int i=1;i<=Num;i++)
		T[i]=0;}
//完成 
bool logCurrentTime(const fs::path& logFolder, bool useSeconds = false) {
	// ---------- 1. 取当前日期与时分 ----------
	time_t now = time(nullptr);
	tm* local = localtime(&now);

	char dateBuf[32];
	strftime(dateBuf, sizeof(dateBuf), "%Y-%m-%d", local);

	char timeBuf[16];
	strftime(timeBuf, sizeof(timeBuf),
			 useSeconds ? "%H:%M:%S" : "%H:%M", local);

	string dateName = string(dateBuf) + ".txt";
	fs::path filePath = logFolder / dateName;   // logs/2026-09-25.txt

	// ---------- 2. 确保 logs 子文件夹存在 ----------
	error_code ec;
	if (!fs::exists(logFolder, ec)) {
		if (!fs::create_directories(logFolder, ec)) {
			cerr << "创建子文件夹失败: " << logFolder << "\n";
			return false;}
	}
	// ---------- 3. 在子文件夹里找"今天"的文件 ----------
	if (!fs::exists(filePath, ec)){
		// 没找到 → 新建
		ofstream out(filePath);
		if (!out) {
			cerr << "创建文件失败: " << filePath << "\n";
			return false;}
		out <<dateBuf << "\n";
		out<<"  ";
		Record(out);
		out << "日志创建于 " << dateBuf << "\n";
		out << "记录时间: " << timeBuf << "\n";
		Record(out);
		cout << "[新建] " << filePath << "\n";}
	 else{
		// 找到 → 追加
		ofstream out(filePath, ios::app);
		if (!out){
			cerr << "打开文件失败: " << filePath << "\n";
			return false;}
		out << "记录时间: " << timeBuf << "\n";
		Record(out);
		cout << "[追加] " << filePath << " -> " << timeBuf << "\n";}
	return true;}
//完成 
int check(string c){
	if(c=="quit"||c=="exit") return 0;
	else if(c=="prices") return 1;
	else if(c[0]>='0'&&c[0]<='9') return 3;
	else if(c[0]=='s') return 4;
	else if(c=="Acc") return 5;
	else return 6;}
//完成 
void Price(){
	cout<<"Item	 No. Pri. "<<endl;
	cout<<"----------------- "<<endl;
	for(int i=1;i<=Num;i++){
		cout<<goods[i].name;
		for(int j=1;j<=9-goods[i].name.size();j++)
			cout<<" ";
		printf("%03d",i);
		cout<<" ";
		printf("%.3lf\n",goods[i].pr);}
	}
//完成 
bool convertToDouble(const string& a1, double& result){
	if(a1.empty())
		return false;
	try{
		size_t pos = 0;
		result=stod(a1,&pos);
		if(pos!=a1.size())
			return false;   // ← 拦住 "1a2b"、"3c"
		if(isinf(result)||isnan(result))
			return false;
		return true;} 
	catch(...){
		return false;}
}
//完成 
void put(){
	ofstream fout_clear("test.txt");
	freopen("test.txt","w",stdout);
	for(int i=1;i<Num;i++)
		cout<<goods[i].pr<<" "<<goods[i].name<<endl;
	cout<<goods[Num].pr<<" "<<goods[Num].name<<endl;
	}
//待完成 
void Checkout(){
	double sum=0;
	cout<<"coutReceipt"<<endl;
	cout<<"Item	 Pri. Qty Amount"<<endl;
	cout<<"-----------------------"<<endl;
	for(int i=1;i<=Num;i++){
		if(T[i]>0){
		cout<<goods[i].name;
		for(int j=1;j<=9-goods[i].name.size();j++)
			cout<<" ";
		printf("%.2f ",goods[i].pr);
		printf("X%d",T[i]);
		sum+=goods[i].pr*T[i];
		cout<<" ";
		if(T[i]<=10)
			cout<<" ";
		printf("%.3lf\n",goods[i].pr*T[i]);}
		}
	cout<<"-----------------------"<<endl;
	cout<<"Total ="<<sum<<endl;
}
//完成 
void Acc(){ 
	system("cls");
	printf("开始结账\n");
	printf("break退出当前界面\n");
	printf("输入条码，记录对应商品+1，显示价格\n");
	printf("在输入前添加一个 - 减号，记录对应商品-1\n");
	printf("print 打印现在的小票，包含商品种类、数量与价格，以及小计价格\n");
	printf("drop 清空记录，重新开始\n");
	printf("checkout 结账。打印小票并清空记录。\n");
	printf("请输入\n");
	while(1){
		int p=0;
		double g_pr=0;
		getline(cin,a);
		vector<string> words;	// 存这一行的所有单词
		stringstream ss(a);
		string word;
		double value[1000];
		while(ss>>word){		// 按空格/制表符拆分
			words.push_back(word);
			if(words[p]=="checkout"){
				Checkout();
				logCurrentTime("logs");
				system("pause");
				system("cls");
				printf("欢迎使用七一一便利店POS系统\n");
				printf("Acc记账\n");
				printf("quit和exit退出当前界面\n");
				printf("prices查看全部价格\n");
				printf("admin进入管理员系统界面\n");
				printf("系统升级了，如果在记账界面外输入编号，则视为查看价格\n");
				return;}
			else if(words[p]=="print"){
				Checkout();}
			else if(words[p]=="break"){
				system("cls");
				printf("欢迎使用七一一便利店POS系统\n");
				printf("Acc记账\n");
				printf("quit和exit退出当前界面\n");
				printf("prices查看全部价格\n");
				//printf("admin进入管理员系统界面\n");
				printf("系统升级了，如果在记账界面外输入编号，则视为查看价格\n");
				return;}
			else if(words[p]=="drop"){
				printf("正在清空,等待2s\n");
				Sleep(1000);
				printf("正在清空,等待1s\n");
				Sleep(1000);
				system("cls");
				printf("重新开始结账\n");
				printf("quit和exit退出当前界面\n");
				printf("请输入编号\n");
				continue;}
			else if(convertToDouble(words[p],value[p])){
				if(value[p]>0)
					T[int(value[p])]++;
				else
					T[-int(value[p])]--;
				g_pr+=goods[int(value[p])].pr;
				p++;}
			else{
				cout<<"滚，别瞎搞";
				Sleep(20000);
				cout<<"自动退出";
				system("cls");
				printf("欢迎使用七一一便利店POS系统\n");
				printf("Acc记账\n");
				printf("quit和exit退出当前界面\n");
				printf("prices查看全部价格\n");
				//printf("admin进入管理员系统界面\n");
				printf("系统升级了，如果在记账界面外输入编号，则视为查看价格\n");
				return;}
			}
		cout<<"记录成功"<<endl;
		cout<<"共"<<p<<"件"<<g_pr<<"元"<<endl;}
}
//完成 
void read1(){
	ifstream fin("data.txt");
	if(!fin){
		cout<<"错误！！！文件打开失败"<<endl;
		exit(0);};
	int p=1; 
	string line;
	while (getline(fin,line)){   // 读不到就退出循环
		int pp=1;
		vector<string> words;	// 存这一行的所有单词
		stringstream ss(line);
		string word;
		while(ss>>word){		 // 按空格/制表符拆分
			words.push_back(word);
			if(pp==1){
				goods[p].pr=stod(word);
				pp++;}
			else{
				goods[p].name=word;
				break;}
			}
			p++;}
	Num=p-1;}
//完成 
void Find(){
	vector<string> words;	// 存这一行的所有单词
		stringstream ss(a);
		string word;
		double value[1000];
		int p=0;
		while(ss>>word){		 // 按空格/制表符拆分
			words.push_back(word);
			if(convertToDouble(words[p],value[p]))
				p++;
			else{
				cout<<"滚，别瞎搞";
				return;}
		}
	cout<<"Item	 No. Pri. "<<endl;
	cout<<"----------------- "<<endl;
	for(int i=0;i<p;i++){
		if(goods[int(value[i])].pr==0){
			cout<<"滚，没这个,后面的都别看了"<<endl;
			return;}
		cout<<goods[int(value[i])].name;
		for(int j=1;j<=9-goods[int(value[i])].name.size();j++)
			cout<<" ";
		printf("%03d",int(value[i]));
		cout<<" ";
		printf("%.3lf\n",goods[int(value[i])].pr);}
	}
//完成 
bool pathExists(const string& path) {
    struct stat info;
    return stat(path.c_str(), &info) == 0;
}
//完成 
int countRecordsByDate(const string& logFolder, const string& date) {
    // 拼出目标文件路径：logs/2026-09-25.txt
    string filePath = logFolder + "/" + date + ".txt";

    // 文件不存在
    if (!pathExists(filePath)) {
        cout << "没找到该日期的日志文件: " << filePath << endl;
        return 0;
    }

    // 打开文件
    ifstream in(filePath.c_str());
    if (!in) {
        cerr << "打开文件失败: " << filePath << endl;
        return -1;
    }

    // 逐行统计
    int count = 0;
    string line;
    const string keyword = "记录时间:";

    while (getline(in, line)) {
        if (line.find(keyword) == 0) {   // 以"记录时间:"开头
            count++;
        }
    }

    in.close();
    return count;
}
//完成 
bool printLogFile(const string& logFolder, const string& date) {
    string filePath = logFolder + "/" + date + ".txt";

    if (!pathExists(filePath)) {
        cout << "没找到该日期的日志文件: " << filePath << endl;
        return false;
    }
    ifstream in(filePath.c_str());
    if (!in) {
        cerr << "打开文件失败: " << filePath << endl;
        return false;
    }

    cout << "========== " << date << " 的日志内容 ==========" << endl;

    string line;
    while (getline(in, line)) {
        cout << line << endl;   // 逐行打印
    }

    cout << "===========================================" << endl;

    in.close();
    return true;
}
//完成 
void Sales(){
	string date;
	cout << "请输入要查询的日期（格式 YYYY-MM-DD，如 2026-09-25）: ";
	cin >> date;
	int n = countRecordsByDate("logs", date);
	if (n >= 0)
        cout << date << " 共记录了 " << n << " 条信息" << endl<<endl;
	printLogFile("logs", date);
	cin.ignore();}
//完成 
int main(){
	read1();
	printf("欢迎使用七一一便利店POS系统\n");
	printf("Acc记账\n");
	printf("sales查看当日所有销售记录及总营业额\n");
	printf("quit和exit退出当前界面\n");
	printf("prices查看全部价格\n");
	//printf("admin进入管理员系统界面\n");
	printf("系统升级了，如果在记账界面外输入编号，则视为查看价格\n");
	while(1){
	getline(cin,a);
	switch(check(a)){
		case 0:put();exit(0);//完成 
		case 1:Price();break;//完成
		case 3:Find();break;//完成 
		case 4:Sales();break;
		case 5:Acc();break;//完成 
		case 6:printf("错误信息:");printf("%s\n",a);break;//完成 
		}
	}
	return 0;}