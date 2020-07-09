#include "head.h"
#define maxInstruction 100

using namespace std;

string  qua[100][4];        
int  maxnum = 0;
string Register[4] = { "", "", "", "" };        


string Instructions[maxInstruction];    

int instruct_index = 0;

int isVoid()        
{
    int back = -1;
    for( int i = 0; i < 4; i++){
        if( Register[i] == ""){
            back = i;
            return back;
        }
    }
    return back;
}

int isSecond(int a)       
{
    int back = -1;
    for( int i = 0; i < 4; i++){
        if( qua[a][1] == Register[i] ){
                back = i;
                return back;
        }
    }
    
    return back;
}

int isThird(int a)       
{
    int back = -1;
    for( int i = 0; i < 4; i++){
        if( qua[a][2] == Register[i] ){
                back = i;
                return back;
        }
    }
    
    return back;
}
int isFourth(int a)          
{
    int back = -1;
    for( int i = 0; i < 4; i++){
        if( qua[a][3] == Register[i] ){
               back = i;
                return back;
        }
    }
    
    return back;
}
string BASE_SEM[100][3];

bool status_[100][3];  

void active_syss()
{
    for(int i = maxnum-1; i >= 0; i--)
    {
        for(int j = 0; j < maxnum ;j++)
        {
            if(qua[i][1] == BASE_SEM[j][0])
            {
                if(BASE_SEM[j][1] != "c")
                {
                    if(BASE_SEM[j][2] == "y")
                        status_[i][0] = true;
                    else
                        status_[i][0] = false;
                    BASE_SEM[j][2] = "y";
                }
                else
                    status_[i][0]= false;
            }
            else if(qua[i][2] == BASE_SEM[j][0])
            {
                if(BASE_SEM[j][1] != "c")
                {
                    if(BASE_SEM[j][2] == "y")
                        status_[i][1]= true;
                    else
                        status_[i][1] = false;
                    BASE_SEM[j][2] = "y";
                }
                else
                        status_[i][1]= false;
            }
            else if(qua[i][3]== BASE_SEM[j][0])
            {
                if(BASE_SEM[j][1] != "c")
                {
                    if(BASE_SEM[j][2] == "y")
                        status_[i][2] = true;
                    else
                        status_[i][2]  = false;
                    BASE_SEM[j][2] = "n";
                }
                else
                        status_[i][2] = false;
            }
        }
    }

    for(int i = 0; i < maxnum; i++)
    {
        for(int j = 0; j < 3; j++)
            BASE_SEM[i][j] = "";
    }

}
void Registers (int a)
{
    for( int i = 0; i < 4; i++){
        
        if( qua[a][1]!= "" && qua[a][1]== Register[i] && !status_[i][0]){
            Instructions[instruct_index++] = "ST   R" + to_string(i) + "," + Register[i];
            
            Register[i] = "";
        }
        else if( qua[i][2] != "" && qua[i][2] == Register[i] && !status_[i][1] ){
            Instructions[instruct_index++] = "ST   R" + to_string(i) + "," + Register[i];
            
            Register[i] = "";
        }
        else if(qua[i][3] != "" && qua[i][3] == Register[i] && !status_[i][2] ){
            Instructions[instruct_index++] = "ST   R" + to_string(i) + "," + Register[i];
            
            Register[i] = "";
        }
    }
}

void setRegister(int a,int b)
{
    for( int i = 0; i < 4; i++){
        if( Register[i] != "" ){
            Instructions[instruct_index++] = "ST    R" + to_string(i) + "," + Register[i];
            
            Register[i] = "";
        }
    }
}

void mubiaodaima()
{
    ifstream ifile;				//打开文件
    ifile.open("中间代码_优化后.txt");
    while (1)					//读入四元式
    {
        ifile >> qua[maxnum][0] >> qua[maxnum][1] >> qua[maxnum][2] >> qua[maxnum][3];
        if (ifile.eof() != 0)break;						//文件结束时跳出
        maxnum++;
    }
    ifile.close();
    for( int i = 0; i < maxInstruction; i++ ){
        Instructions[i] = "";
    }
    for( int i = 0; i < maxnum; i++ ){      
       
            if( qua[i][0] ==  "+" ){   
                int touseregister = isSecond(i);    
                if( touseregister == -1 ){  
                    touseregister = isThird(i );
                    if( touseregister == -1 ){  
                        touseregister = isVoid();
                        if( touseregister == -1 ){  
                            Instructions[instruct_index++] = "ST    R0," + Register[0];
                            
                            Instructions[instruct_index++] = "LD    R0," + qua[i][1];
                                                        
                            Instructions[instruct_index++] = "ADD    R0," + qua[i][2];
                            
                            Register[0] = qua[i][3];
                        }
                        else{   
                            Instructions[instruct_index++] = "LD    R" + to_string(touseregister) + "," + qua[i][1];
                            
                           Instructions[instruct_index++] = "ADD    R" + to_string(touseregister) + "," +qua[i][2];
                           
                           Register[touseregister] = qua[i][3];
                        }
                    }
                    else{   
                        int tempB = touseregister;
                        if( status_[i][1]){    
                            touseregister = isVoid();
                            if( touseregister == -1 ){  
                                Instructions[instruct_index++] = "ST    R" + to_string(tempB) + "," + Register[tempB];
                                
                                Instructions[instruct_index++] = "ADD    R" + to_string(tempB) + "," + qua[i][1];
                             
                               Register[tempB] = qua[i][3];
                            }
                            else{   
                                Instructions[instruct_index++] = "MOV    R" + to_string(touseregister) + ",R" + to_string(tempB);
                                
                                Instructions[instruct_index++] = "ADD    R" + to_string(touseregister) + "," + qua[i][1];
                                
                               Register[touseregister] = qua[i][3];
                            }
                        }
                        else{   
                            Instructions[instruct_index++] = "ST    R" + to_string(tempB) + "," +Register[tempB];
                            
                            Instructions[instruct_index++] = "ADD    R" + to_string(tempB) + "," + qua[i][1];
                          
                           Register[tempB] = qua[i][3];
                        }
                    }
                }
                else{           
                    int temp = touseregister;   
                    if(status_[i][0] ){       
                        touseregister = isThird(i );
                        if( touseregister == -1 ){  
                            touseregister = isVoid();
                            if( touseregister == -1 ){  
                                Instructions[instruct_index++] = "ST    R" + to_string(temp) + "," + Register[temp];
                                
                                Instructions[instruct_index++] = "ADD    R" + to_string(temp) + "," + qua[i][2];
                            
                               Register[temp] = qua[i][3];
                            }
                            else{   
                                Instructions[instruct_index++] = "LD    R" + to_string(touseregister) + "," + qua[i][2];
                                
                               Instructions[instruct_index++] = "ADD    R" + to_string(touseregister) + ",R" + to_string(temp);
                                
                                Register[touseregister] =qua[i][3];
                            }
                        }
                        else{       
                            int tempB = touseregister;
                            if( status_[i][1] ){    
                                touseregister = isVoid();
                                if( touseregister == -1 ){  
                                    Instructions[instruct_index++] = "ST    R" + to_string(tempB) + "," + Register[tempB];
                                    
                                    Instructions[instruct_index++] = "ADD    R" + to_string(tempB) + ",R" + to_string(temp);
                                    
                                    Register[tempB] = qua[i][3];
                                }
                                else{       
                                    Instructions[instruct_index++] = "MOV    R" + to_string(touseregister) + ",R" + to_string(temp);
                                    
                                    Instructions[instruct_index++] = "ADD    R" + to_string(touseregister) + ",R" + to_string(tempB);
                                    
                                    Register[touseregister] = qua[i][3];
                                }
                            }
                            else{   
                                Instructions[instruct_index++] = "ADD    R" + to_string(tempB) + ",R" + to_string(temp);
                                Register[tempB] = qua[i][3];
                            }
                        }
                    }
                    else{   
                        touseregister = isThird(i );
                        if( touseregister == -1 ){  
                            Instructions[instruct_index++] = "ADD    R" + to_string(temp) + "," + qua[i][2];
                           Register[temp] = qua[i][3];
                        }
                        else{   
                            Instructions[instruct_index++] = "ADD    R" + to_string(temp) + ",R" + to_string(touseregister);
                            Register[temp] = qua[i][3];
                        }
                    }
                } 
            }
            else if( qua[i][0]== "-" ){
                int touseregister = isSecond(i);
                if( touseregister == -1 ){  
                    touseregister = isThird(i);
                    if( touseregister == -1 ){  
                        touseregister = isVoid();
                        if( touseregister == -1 ){  
                            Instructions[instruct_index++] = "ST    R0," + Register[0];
                            Instructions[instruct_index++] = "LD    R0," + qua[i][1];
                           Instructions[instruct_index++] = "SUB    R0," + qua[i][2];
                           Register[0] = qua[i][3];
                        }
                        else {     
                            Instructions[instruct_index++] = "LD    R" + to_string(touseregister) + "," + qua[i][1];
                           Instructions[instruct_index++] = "SUB    R" + to_string(touseregister) + "," + qua[i][2];
                           Register[touseregister] = qua[i][3];
                        }
                    }
                    else{       
                        int tempB = touseregister;
                        touseregister = isVoid();
                        if( touseregister == -1 ){  
                            Instructions[instruct_index++] = "ST    R" + to_string(tempB) + "," + Register[tempB];
                            Instructions[instruct_index++] = "LD    R" + to_string(tempB) + "," + qua[i][1];
                           Instructions[instruct_index++] = "SUB    R" + to_string(tempB) + "," + qua[i][2];
                           Register[tempB] = qua[i][3];
                        }
                        else{       
                            Instructions[instruct_index++] = "LD    R" + to_string(touseregister) + "," +qua[i][1];
                           Instructions[instruct_index++] = "SUB    R" + to_string(touseregister) + ",R" + to_string(tempB);
                            Register[touseregister] = qua[i][3];
                        }
                    }
                }
                else{       
                    int tempA = touseregister;
                    if( status_[i][0] ){   
                        touseregister = isThird(i);
                        if( touseregister == -1 ){  
                            touseregister = isVoid();
                            if( touseregister == -1 ){  
                                Instructions[instruct_index++] = "ST    R" + to_string(tempA) + "," + Register[tempA];
                                Instructions[instruct_index++] = "SUB    R" + to_string(tempA) + "," +qua[i][2];
                               Register[tempA] = qua[i][3];
                            }
                            else{   
                                Instructions[instruct_index++] = "MOV    R" + to_string(touseregister) + ",R" + to_string(tempA);
                                Instructions[instruct_index++] = "SUB    R" + to_string(touseregister) + "," +qua[i][2];
                               Register[touseregister] = qua[i][3];
                            }
                        }
                        else{   
                            int tempB = touseregister;
                            touseregister = isVoid();
                            if( touseregister == -1 ){  
                                Instructions[instruct_index++] = "ST    R" + to_string(tempA) + "," + Register[tempA];
                                Instructions[instruct_index++] = "SUB    R" + to_string(tempA) + ",R" + to_string(tempB);
                                Register[tempA] = qua[i][3];
                            }
                            else{   
                                Instructions[instruct_index++] = "MOV    R" + to_string(touseregister) + ",R" + to_string(tempA);
                                Instructions[instruct_index++] = "SUB    R" + to_string(touseregister) + ",R" + to_string(tempB);
                                Register[touseregister] = qua[i][3];
                            }
                        }
                    }
                    else{       
                        touseregister = isThird(i);
                        if( touseregister == -1 ){  
                            Instructions[instruct_index++] = "SUB    R" + to_string(tempA) + qua[i][2];
                           Register[tempA] = qua[i][3];
                        }
                        else{   
                            Instructions[instruct_index++] = "SUB    R" + to_string(tempA) + ",R" + to_string(touseregister);
                            Register[tempA] = qua[i][3];
                        }
                    }
                } 
            }
            else if( qua[i][0]== "*" ){
                int touseregister = isThird(i);
                if( touseregister == -1 ){  
                    if( Register[0] == "" ){        
                        Instructions[instruct_index++] = "LD    R0," + qua[i][2];
                    }
                    else{
                        Instructions[instruct_index++] = "ST    R0," + Register[0];
                        Instructions[instruct_index++] = "LD    R0," + qua[i][2];
                    }
                    int tofinda = isSecond(i);
                    if( tofinda == -1 ){        
                        Instructions[instruct_index++] = "MUL    R0," + qua[i][1];
                    }
                    else{
                        Instructions[instruct_index++] = "MUL    R0,R" + to_string(tofinda);
                    }
                    Register[0] = qua[i][3];
                }
                else{   
                    int tofinda = isSecond(i);
                    if( tofinda == -1 ){
                        Instructions[instruct_index++] = "MUL    R" + to_string(touseregister) + "," + qua[i][1];
                    }
                    else
                        Instructions[instruct_index++] = "MUL    R" + to_string(touseregister) + ",R" + to_string(tofinda);
                    Register[touseregister] = qua[i][3];
                }
            }
            else if( qua[i][0]== "/" ){
                int touseregister = isSecond(i);
                if( touseregister == -1 ){  
                    if( Register[0] == "" ){        
                        Instructions[instruct_index++] = "LD    R0," + qua[i][1];
                    }
                    else{
                        Instructions[instruct_index++] = "ST    R0," + Register[0];
                        Instructions[instruct_index++] = "LD    R0," + qua[i][1];
                    }
                    Instructions[instruct_index++] = "DIV    R0," + qua[i][2];
                    Register[0] = qua[i][3];
                }
                else{       
                    Instructions[instruct_index++] = "DIV    R" + to_string(touseregister) + "," + qua[i][2];
                    Register[touseregister] = qua[i][3];
                }
            }
            else if( qua[i][0]== "=" ){
                int tousegegister = isFourth(i );
                if( tousegegister == -1 ){      
                    tousegegister = isSecond(i );
                    if( tousegegister == -1 ){      
                        tousegegister = isVoid();
                        if( tousegegister == -1 ){  
                            Instructions[instruct_index++] = "ST    R0," + Register[0];
                            Instructions[instruct_index++] = "LD    R0," + qua[i][1];
                           Instructions[instruct_index++] = "ST    R0," + qua[i][3];
                            Register[0] = qua[i][1];
                        }
                        else{   
                            Instructions[instruct_index++] = "LD    R" + to_string(tousegegister) + "," + qua[i][1];
                           Instructions[instruct_index++] = "ST    R" + to_string(tousegegister) + "," + qua[i][3] ;
                            Register[tousegegister] = qua[i][1];
                        }
                    }
                    else{   
                        Instructions[instruct_index++] = "ST    R" + to_string(tousegegister) + "," + qua[i][3] ;
                    }
                }
                else{       
                    int tempC = tousegegister;
                    tousegegister = isSecond(i );
                    if( tousegegister == -1 ){  
                        Instructions[instruct_index++] = "ST    R" + to_string(tempC) + "," + qua[i][3] ;
                    }
                    else{       
                        Instructions[instruct_index++] = "ST    R" + to_string(tempC) + ",R" + to_string(tousegegister);
                    }
                }
            } 
    }
    setRegister(0,0);
    ofstream ofile;
    ofile.open("目标代码.txt");
    for (int i = 0; i < instruct_index; i++) {
        ofile << Instructions[i] << endl;
    }
    ofile.close();
}
