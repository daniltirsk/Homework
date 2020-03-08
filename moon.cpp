#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cctype>>
#include <stdio.h>
#include <math.h>
#include <ctime>

using namespace std;


int  IsLeapYear(int year) // провнрка високосного года
{
    return (((year % 4 == 0) &&
             (year % 100 != 0)) ||
            (year % 400 == 0));
}

struct Point
{
 double x, y;
};


// проверка даты
int validDate(int day,int month,int year){

    int months[12]={31,28,31,30,31,30,31,31,30,31,30,31};

   if (IsLeapYear(year)){
        months[1]++;
    }

    if (month > 12 || month<1 ){
        return false;
    } else if (day > months[month-1] || day<1 ){
        return false;
    }

    return true;

}

// вывод в нужном виде
void getResult(double mset,double mrise,double peak,string date,int date2){
    string ndate;
    ndate=to_string(date2);
    if (isnan(peak)){
        cout<<"Not found. No such date in the file"<<endl;
        return;
    }

    printf("This day - rise: %02d:%02d:%02d \n",int(mrise),int(mrise*60-int(mrise)*60),int(((mrise*60-int(mrise)*60)-int(mrise*60-int(mrise)*60))*60));
    printf("This day - peak: %02d:%02d:%02d \n",int(peak),int(peak*60-int(peak)*60),int(((peak*60-int(peak)*60)-int(peak*60-int(peak)*60))*60));

    if (date!=ndate){
        cout<<"Next day - ";
    } else {
        cout<<"This day - ";
    }

    printf("set: %02d:%02d:%02d \n",int(mset),int(mset*60-int(mset)*60),int(((mset*60-int(mset)*60)-int(mset*60-int(mset)*60))*60));
}

int main()
{
    double b=0,k=0,x=0,kr=0,xr=0,br=0;
    string date,date2="",line,year,day,month;
    char datebuf[8];

    cout<<"Enter the date. DD/MM/YYYY"<<endl;
    //считывание и проверка даты и файла
    while (true){
        year="";
        day="";
        month="";
        cin>>date;

        for (int i=0,j=0,k=0;i<date.length();i++) {
            if (date[i]-48>=0 && date[i]-48<=9){
                if(k==0){
                    day+=date[i];
                } else if(k==1){
                    month+=date[i];
                } else {
                    year+=date[i];
                }
                j++;
            } else {
                k++;
                j=0;
            }
        }

        ifstream in("moon" + year + ".dat");

        if (year.empty() || day.empty() || month.empty()){
            cout<<"Incorrect input. Enter the date again"<<endl;
        }
        else if (!validDate(stoi(day),stoi(month),stoi(year))){
            cout<<"Incorrect date. Enter the date again"<<endl;
        } else if (!in.is_open()){
            cout<<"File not found. Enter the date again"<<endl;
        } else {
            break;
        }
        in.close();
    }

    sprintf(datebuf,"%04d%02d%02d",stoi(year),stoi(month),stoi(day));
    date2 = datebuf;


    bool found,found1=false,found2=false,found3=false,start=false,rfound,sfound=false;
    double a=0,mset=0,mrise=0,peak=0;
    int cl1, cl2;
    double cl3=0,cl4=0,cl5=0,pcl5=0,pcl3=0,pcl2=0,ppcl2=0,ppcl3=0,ppcl5=0;
    Point p1,p2,p3;
    p1.x=-1;
    p1.y=0;
    p2.x=-1;
    p2.y=0;
    p3.x=-1;
    p3.y=0;

    ifstream in("moon" + year + ".dat");

    if (in.is_open()){
        while (getline(in, line)){
                size_t found = line.find(date2);
                if (found!=string::npos){
                    start=true;
                }
                if (start && !sfound){

                    istringstream iss(line);

                    iss >> cl1>>cl2>>cl3>>cl4>>cl5;
                    // вычисление восхода и заката
                    if (cl5>=0 && pcl5<0){
                        br=pcl5;
                        kr=(cl5-pcl5)/(cl3-pcl3);
                        xr=-br/kr;
                        mrise=pcl3+xr;
                        rfound=true;
                    } else if (cl5<0 && pcl5>=0 && rfound){
                        br=pcl5;
                        kr=(cl5-pcl5)/(cl3-pcl3);
                        xr=-br/kr;
                        mset=pcl3+xr;
                        sfound=true;
                    }

                    //вычисление точек для нахождеия пика
                    if (p1.x != cl3 && !found1){
                        p1.x=cl3;
                        p1.y=cl5;
                        found1=true;
                    } else if (p1.y < cl5 && !found2){
                        p2.x=cl3;
                        p2.y=cl5;
                        found2=true;
                    } else if (p2.y < cl5 && !found3 && found1 && found2) {
                        p1.x=p2.x;
                        p1.y=p2.y;
                        p2.x=cl3;
                        p2.y=cl5;
                    } else if (p2.y > cl5 && !found3 && found1 && found2){
                        p3.x=cl3;
                        p3.y=cl5;
                        found3=true;
                    }

                    ppcl2=pcl2;
                    ppcl3=pcl3;
                    ppcl5=pcl5;
                    pcl5=cl5;
                    pcl3=cl3;
                    pcl2=cl2;

                }


        }
        //вычисление пика
        a= (p3.y-(p3.x*(p2.y-p1.y)+p2.x*p1.y-p1.x*p2.y)/(p2.x-p1.x))/(p3.x*(p3.x-p1.x-p2.x)+p1.x*p2.x);
        b= (p2.y-p1.y)/(p2.x-p1.x) - a*(p1.x+p2.x);
        x=-b/(2*a);
        peak=x;

        getResult(mset,mrise,peak,date2,cl1);

        in.close();
    }

    return 0;
}
