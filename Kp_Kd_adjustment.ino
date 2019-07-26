int val_l[4];//middle to extreme
int val_r[4];//middle to extreme
//int fore;
int weights[]={2,4,6,8};
float weight=0;//current weight
int r_motor=255, l_motor=255;//motor speeds
float D=25.0, p_error=0, pw=0, Kp=17.0;
float factor=0.45;//factors
#define m1a 8
#define m2a 9
#define m1b 12
#define m2b 13
#define ena 10
#define enb 11
#define led A5
int dir=-1;
int counter=0; int error_c=0;

int cf=0;

void input()//detects black line currently
{
  //fore=digitalRead(A5);
  for(int i=0;i<4;i++)
  {
      int v=digitalRead((3-i));
      if(v==0)//change for colour of line
        val_l[i]=-1*weights[i];
      else
        val_l[i]=0;
      v=digitalRead(i+4);
      if(v==0)//change for colour of line
        val_r[i]=weights[i];
      else
        val_r[i]=0;
  }
}
void orientation()
{
  /*for(int i=0;i<4;i++)//instead of adding whole just start from non-zero extremes
    weight+=val_l[i]+val_r[i];
    */
    int z=0, s=0;
    for(int i=3;i>=0;i--)
    {
      if(val_r[i]!=0)
      {
        s+=val_r[i];
        z++;
      }
      if(val_l[i]!=0)
      {
        s+=val_l[i];
        z++;
      }
    }
    if(z!=0)
      weight=s/z;
   //hard turns to be added
  /*if(fore==1 && (weight<-30 || weight>30))
  {
    l_motor=255;
    r_motor=255;
  }
  else{*/
  /*if(weight>=100||weight<=-100)
  {
    if(weight<0)
    {
      steer();
      digitalWrite(m1a,LOW);
      digitalWrite(m2a,HIGH);
      analogWrite(ena, 100);
      digitalWrite(m1b,HIGH);
      digitalWrite(m2b,LOW);
      analogWrite(enb, 100);
      
    }
    else
    {
      steer();
      digitalWrite(m1a,HIGH);
      digitalWrite(m2a,LOW);
      analogWrite(ena, 100);//tune
      digitalWrite(m1b,LOW);
      digitalWrite(m2b,HIGH);
      analogWrite(enb,100);//tune
      
    }
    return;
  }*/
  if(weight<0)
  {
    weight=-weight;
    l_motor=255-(Kp*weight)-D*(pw);
    weight=-weight;
  }
  else if(weight>0)
  {
    r_motor=255-(Kp*weight)+D*(pw);
  }
  if(val_l[3]!=0)
    dir=-1;
  else if(val_r[3]!=0)
    dir=1;
  //}
  l_motor*=factor;
  r_motor*=factor;
  if(p_error!=weight){
    pw=p_error-weight;
    error_c=0;
  }
  if(p_error==weight && weight==0)
  {
    error_c++;
    if(error_c>=10){
      pw=0;
      error_c=0;
    }
  }
  p_error=weight;
  //Serial.print(pw);
  //Serial.print("\n");
  /*Serial.print(dir);
  for(int i=0;i<=1;i++)
  {
    Serial.print(val_l[i]);
    Serial.print("  ");
  }
  
  for(int i=0;i<=1;i++)
  {
    Serial.print(val_r[i]);
    Serial.print("  ");
  }
  Serial.print("\n");
  */
  /*
  Serial.print(l_motor);
  Serial.print("  ");
  Serial.print(r_motor);
  Serial.print("\n");
  */
  steer();
}
void steer()
{
  if(weight==0 && val_l[3]==0 && val_r[3]==0 && val_l[2]==0 && val_r[2]==0 && val_l[1]==0 && val_r[1]==0 && val_l[0]==0 && val_r[0]==0)
  {

    
    /*{
      digitalWrite(m1a,LOW);
      digitalWrite(m2a,HIGH);
      analogWrite(ena, 0);//tune
      digitalWrite(m1b,HIGH);
      digitalWrite(m2b,LOW);
      analogWrite(enb, 0);//tune
      delay(30);
      }
      */


      
    if(dir==-1)
    {
      //while(val_l[3]==0 || val_l[2]==0 || val_l[1]==0 || val_l[0]==0){
      while(val_l[0]==0){//change turning precision
      digitalWrite(m1a,LOW);
      digitalWrite(m2a,HIGH);
      analogWrite(ena, 70);//tune
      digitalWrite(m1b,HIGH);
      digitalWrite(m2b,LOW);
      analogWrite(enb, 70);//tune
      input();
      }
    }
    if(dir==1)
    {
      //while(val_r[3]==0 && val_r[2]==0 && val_r[1]==0 && val_r[0]==0){
      while(val_r[0]==0){//change turning precision
      digitalWrite(m1a,HIGH);
      digitalWrite(m2a,LOW);
      analogWrite(ena, 70);//tune
      digitalWrite(m1b,LOW);
      digitalWrite(m2b,HIGH);
      analogWrite(enb,70);//tune
      input();
      }
    }
    return;    
  }
  else{
  digitalWrite(m1a,HIGH);
  digitalWrite(m2a,LOW);
  analogWrite(ena,l_motor);
  digitalWrite(m1b,HIGH);
  digitalWrite(m2b,LOW);
  analogWrite(enb,r_motor);
  }
}
void checkpoint()
{
  for(int i=0;i<3;i++)
  {
    if(val_l[i]==0 || val_r[i]==0)
    {
      digitalWrite(led,LOW);
      return; 
    }
  }
  digitalWrite(led,HIGH);
}
boolean halt()
{
  for(int i=0;i<4;i++)
  {
    if(val_l[i]==0 || val_r[i]==0)
    {
      counter=0;
      return false; 
    }
  }
  counter++;
  if(counter>500)
    return true;
   return false;    
}
void setup() {
  //Serial.begin(9600);
  // put your setup code here, to run once:
  for(int i=0;i<8;i++)
    pinMode(i,INPUT);
  pinMode(m1a,OUTPUT);
  pinMode(m2a,OUTPUT);
  pinMode(m1b,OUTPUT);
  pinMode(m2b,OUTPUT);
  pinMode(ena,OUTPUT);
  pinMode(enb,OUTPUT);
  pinMode(led,OUTPUT);
  //pinMode(A5,INPUT);
}
void loop() {
  // put your main code here, to run repeatedly:
  
  weight=0;
  l_motor=255;
  r_motor=255;
  input();
  /*if(fore==1)
    factor=0.25;
  else{
  if(weight<0)
    weight=-weight;*/
  //}
  orientation();
  checkpoint();
  while(halt())
  {
   digitalWrite(m1a,LOW);
      digitalWrite(m2a,HIGH);
      analogWrite(ena, 0);//tune
      digitalWrite(m1b,HIGH);
      digitalWrite(m2b,LOW);
      analogWrite(enb, 0);//tune //??
  }
}
