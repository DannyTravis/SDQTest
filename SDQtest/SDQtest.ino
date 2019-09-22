
#include <SD.h>
#include <SPI.h>



struct Queue{
  long index;
  long lastPlace;
  int strLength; 
  char limiter;
};


File myFile;
struct Queue myQueue{0,0,0,'$'};
String sent1 = "$ Ocean Man";
String sent2 = "$ Take me by the hand ";
String sent3 = "$ Lead me to the land that you understand";//84
String sent4 = "$ Ocean Man";
String sent5 = "$ the voyage to the corner of the globe";
String sent6 = "$ is a real trip";

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("OK chaps, here we go!");
  Serial.println("Lets initialize the SD card");
  Serial.print("Initializing SD card...");
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
// Test SD card
  if(SD.exists("test.txt")){
   Serial.println("Test file exists"); 
  }
  else{
  Serial.println("creating test file.....");
  myFile = SD.open("test.txt", FILE_WRITE);
  myFile.close();
  }

  delay(2000);
  Serial.println("Pushing to Queue....");
  PushToQueue("test.txt",sent1,&myQueue);
  Serial.print("Index: ");
  Serial.print(myQueue.index);
  Serial.print(" Last Place: ");
  Serial.print(myQueue.lastPlace);
  Serial.print(" String Length: ");
  Serial.print(myQueue.strLength);
  delay(2000);
  Serial.println("\n\rAdding other lines");
  PushToQueue("test.txt",sent2,&myQueue);
  PushToQueue("test.txt",sent3,&myQueue);
  PushToQueue("test.txt",sent4,&myQueue);
  PushToQueue("test.txt",sent5,&myQueue);
  PushToQueue("test.txt",sent6,&myQueue);
  delay(2000);
  Serial.print("Index: ");
  Serial.print(myQueue.index);
  Serial.print(" Last Place: ");
  Serial.print(myQueue.lastPlace);
  Serial.print(" String Length: ");
  Serial.println(myQueue.strLength);
  delay(2000);
  Serial.print("Last Line: ");
  String myStr = ReadLastLine("test.txt",&myQueue);
  Serial.println(myStr);
  PopLastLine("test.txt",&myQueue);
  Serial.print("New Last Line: ");
  myStr = "";
  myStr = ReadLastLine("test.txt",&myQueue);
  Serial.println(myStr);
  PopLastLine("test.txt",&myQueue);
  Serial.print("New Last Line: ");
  myStr = "";
  myStr = ReadLastLine("test.txt",&myQueue);
  Serial.println(myStr);
  PopLastLine("test.txt",&myQueue);
  Serial.print("New Last Line: ");
  myStr = "";
  myStr = ReadLastLine("test.txt",&myQueue);
  Serial.println(myStr);
  PopLastLine("test.txt",&myQueue);
  Serial.print("New Last Line: ");
  myStr = "";
  myStr = ReadLastLine("test.txt",&myQueue);
  Serial.println(myStr);
  Serial.println("This concludes the second test");
  DeleteQueue("test.txt",&myQueue);

}

void loop() {
  // put your main code here, to run repeatedly:

}

/******* Add to File *****/
void PushToQueue(char* filename, String myStr, struct Queue *myQueue )
{
  char aux_str[256];
  int addToIndex = 2;
  //Serial.println(filename);
  File myFile = SD.open(filename, FILE_WRITE);
  if (myFile)
  {
    Serial.print("Adding to File: ");
    Serial.println(myStr);
    myFile.println(myStr);
    }
    //Serial.println();
    myFile.close();
    myStr.toCharArray(aux_str, 256);
    for (int i = 0; aux_str[i] != '\0'; i++)
    {
    addToIndex++;
    }
    long tmp = myQueue->lastPlace+(long)addToIndex;
    myQueue->lastPlace = tmp;
    myQueue->index++;
    myQueue->strLength = addToIndex;
}

/****** Whole shebang ******/
void DeleteQueue(char * filename,struct Queue *myQueue )
{
  if(SD.exists(filename)){
    SD.remove(filename);
    Serial.println("Queue has been distroyed");
  }
  myQueue->index = 0;
  myQueue->strLength = 0;
  myQueue->lastPlace = 0;
}


void PopLastLine(char* filename, struct Queue *myQueue){
  int newStrLength = 0;
  char myLimit = myQueue->limiter;
  int offset = myQueue->strLength;
  long tmp = myQueue->lastPlace - (long)offset;
  File myFile = SD.open(filename, FILE_READ);
  if(myFile){
    myFile.seek(tmp);
    while(true){
      tmp--;
      newStrLength++;
      myFile.seek(tmp);
      char cr = myFile.read();
      if(cr == myLimit){
        break;
      }    
    } 
    myFile.close();
  }
  myQueue->index--;
  long newLastPlace = myQueue->lastPlace - (long)offset; 
  myQueue->strLength = newStrLength;
  myQueue->lastPlace = newLastPlace; 
  return;
}

/****** Read Last Line **********/  
String ReadLastLine(char* filename, struct Queue *myQueue)
{ 
  char aux_str[125];
  char cr;
  int tmpStrL = myQueue->strLength;
  unsigned long index = (myQueue->lastPlace) - (long)tmpStrL;
  File myFile = SD.open(filename, FILE_READ);
   if(myFile){
    myFile.seek(index);
    int i = 0;
    while(true){
      cr = myFile.read();
      aux_str[i] = cr;
      if(cr == '\n'){
        aux_str[i] = '\0';
        break;     
      }
      i++;
    }
    myFile.close();
   }
  String finished(aux_str);
  return finished;
}
