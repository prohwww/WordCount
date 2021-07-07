#include<iostream>
#include<string>
#include<fstream>
#include<cstring>
#include<algorithm>

using namespace std;

class Word{
	private :
		string name;
		int cnt;
	public :
		Word(){
			this -> name = "";
			this -> cnt = 0;
		}
		Word(string wordName){
			this -> name = wordName;
			this -> cnt = 1;
		}
		void addCnt(){
			cnt++;
		}
		string getName(){
			return name;
		}
		int getCnt(){
			return cnt;
		}
};

static int arrCnt; //배열 size

Word * readFile(string fileName); //파일 읽고 단어 수 계산
string removeChar(string str); //단어에서 특문 제거
void printWord(string c, Word * wArr); //출력
Word * sortWORD(Word * wArr,int num); //1 ASC 2 DESC
Word * sortCNT(Word * wArr, int num); //1 MAX 2 MIN

Word * sortCNT(Word * wArr,int num){
	int i,j;
	Word temp;

	for(i=arrCnt-1; i>0; i--) {
		for(j=0; j<arrCnt-1 ; j++){
			int n1,n2;

			if(num == 1){ //MAX
				n1 = wArr[j+1].getCnt();
				n2  = wArr[j].getCnt();
			} else if(num == 2){ //MIN
				n1 = wArr[j].getCnt();
				n2  = wArr[j+1].getCnt();
			}

			if(n1 > n2){
				temp = wArr[j];
				wArr[j] = wArr[j+1];
				wArr[j+1] = temp;
			}
		}//for
	}//for

	return wArr;

}//sortMAX()

Word * sortWORD(Word * wArr, int num){

	int i,j,k;
	Word temp;
	
	for (j = 0; j < arrCnt; j++) {
		for (i=0; i<j; i++) {
			const char * name1 = wArr[i].getName().c_str();
			const char * name2 = wArr[i+1].getName().c_str();

			if(num == 1 && strcmp(name1, name2) > 0){//ASC name1 > name2
				temp = wArr[i];
				wArr[i] = wArr[i+1];
				wArr[i+1] = temp;
			}else if(num == 2 && strcmp(name1, name2) < 0){//DESC name1 < name2
				temp = wArr[i];
				wArr[i] = wArr[i+1];
				wArr[i+1] = temp;
			}
		}//for_i
	}

	return wArr;

}//sortASC


void printWord(string c, Word * wArr){
	
	int i, wordCnt = 0;

	//sort
	if(c.compare("A") == 0){ //ASC
		wArr = sortWORD(wArr,1);
	}else if(c.compare("D") == 0){//DESC
		wArr = sortWORD(wArr,2);
	}else if(c.compare("M") == 0){//MAX
		wArr = sortCNT(wArr,1);
	}else if(c.compare("I") == 0){//MIN
		wArr = sortCNT(wArr,2);
	}else {
		cout << "잘못된 정렬방식입니다." << endl;
		cout << "A:ASC D:DESC M:NAX I:MIN" <<endl;
		return;
	}

	//cout << "!!!!!" << endl;
	//cout << arrCnt << endl;

	//print
	for(i=0; i<arrCnt; i++){
		cout.width(16);
		cout.setf(ios::left);
		cout << wArr[i].getName(); 

		cout.width(3);
		cout << " : ";

		cout.width(2);
		cout << wArr[i].getCnt() << endl;

		wordCnt += wArr[i].getCnt();
	}
	
	cout << "------------------------------------------" << endl;
	cout << "distinct word cnt : " << arrCnt << endl;
	cout << "total word cnt : " << wordCnt << endl;
}//printWord()

string removeChar(string str){
	int i;
	string temp = "";
	transform(str.begin(), str.end(), str.begin(), (int(*)(int)) tolower);

	for(i=0; i<str.length(); i++){
		if(str.at(i) >= 'a' && str.at(i) <= 'z'){
			temp += str.at(i);
		}
	}

	if(temp.length() == 0)
		return "";
	else
		return temp;
}

Word * readFile(string fileName){
	
	ifstream fin;
	fin.open(fileName.c_str());

	string str;
	
	arrCnt = 50; //배열 size
	int index = 0; //배열 객체의 개수
	Word * wArr = new Word[arrCnt];
	
	while(fin){
		if(fin.is_open()){
			fin >> str;
			int i;
			bool flag = false;
			
			str = removeChar(str);

			if(str.compare("") == 0) continue;

			for(i=0; i<index; i++){
				if(wArr[i].getName().compare(str) == 0){ //단어 중복되는지 검사
					wArr[i].addCnt();
					flag = true;
					break;
				}
			}
			
			if(flag) continue;

			if(index == arrCnt){ //배열 넘칠 경우
				arrCnt *= 2;
				Word * tempArr = new Word[arrCnt];
				
				for(i=0; i<arrCnt/2; i++){
					tempArr[i] = wArr[i];
				}
				
				wArr = tempArr;
			}

			wArr[index] = Word(str);
			index++;
			
		} else {
			cout << "파일을 찾을 수 없습니다!" << endl;
		}
	}

	arrCnt = index;
	fin.close();

	return wArr;
}

int main(int argc, char * argv[]){

	Word * wArr = readFile(argv[1]);
	
	printWord(argv[2], wArr);
	
	return 0;
}

