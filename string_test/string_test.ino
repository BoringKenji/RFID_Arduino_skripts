// seperate_RFID_TAG.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

char first_scan[] = "bb,02,22,00,0d,c8,24,00,00,00,00,00,00,00,11,1d,79,20,e4,7e bb,02,22,00,0d,c0,24,00,00,00,00,00,00,00,11,1e,49,43,d0,7e bb,02,22,00,0d,ca,24,00,00,00,00,00,00,00,11,23,ae,bd,be,7e ";
char second_scan[] = "bb,2,22,0,d,c0,24,0,0,0,0,0,0,0,11,1e,49,43,d0,7e ,bb,2,22,0,d,ca,24,0,0,0,0,0,0,0,11,23,ae,bd,be,7e";
char third_scan[] = "bb,2,22,0,d,c5,24,0,0,0,0,0,0,0,11,20,49,43,d0,7e ,bb,2,22,0,d,c4,24,0,0,0,0,0,0,0,11,25,ae,bd,be,7e";
char rfid[6][100] = { 0 };
char id[6][2] = { 0 };
char signal[2] = { 0 };

//1. seperate the different tags
// String id[0] = bb,2,22,0,d,c8,24,0,0,0,0,0,0,0,11,1d,79,20,e4,7e
void separate(char scan[], char store[][100], int lenght)
{
  int k = 0;
  for (int j = 0; j < 6; j++) {
    int i = 0;
    while (scan[i] != ' ')
    {
      store[j][i] = scan[k];
      i++; k++;
      if (k > lenght) {
        return;
      }
    }
    k++;
    
  }
  return;
}

// get id[5] eg c8 && get the 15th entry eg 1d
void getid(int idx, char rfid[][100], char id[][2])
{
  for(int a=0 ; a<6 ; a++)
  {
    if (rfid[a][0] != 'b')
    {
      return;
    }
    int i = 0, j = 0, k = 0;
    while (i < idx + 1)
    {

      if (i == idx)
      {
        id[a][k] = rfid[a][j];
        k++;
      }
      if (rfid[a][j] == ',')
      {
        i++;
      }
      j++;
    }
    for (int i = 0; i < 2; i++)
    {
      Serial.print(id[a][i]);
    }
    Serial.println();
  }
  return;
}


void setup()
{
  Serial.begin(9600);

  int scan_length = sizeof(first_scan) / sizeof(char);
  Serial.println((String)"signal length" + scan_length);
  separate(first_scan, rfid, scan_length);


  for (int j = 0; j < 6; j++) {
    for (int i = 0; i < 100; i++) {
      Serial.print(rfid[j][i]);
    }

    Serial.println();
  }
  /////////////////////////////////////////////////////////////////////////////////////////////////////
  char signal[6][2] = { 0 };
  Serial.println("Signal Strength");
  getid(5, rfid, signal);
  
  Serial.println("RFID Tag");
  char id0[6][2] = { 0 }; char id1[6][2] = { 0 }; char id2[6][2] = { 0 }; char id3[6][2] = { 0 };
  
  getid(12, rfid, id0); getid(13, rfid, id1); getid(14, rfid, id2); getid(15, rfid, id3);


  Serial.println("send data");
  Serial.println((String)id0[0][0] + id0[0][1] + "," + id1[0][0] + id1[0][1] + "," + id2[0][0] + id2[0][1] + "," + id3[0][0] + id3[0][1]);

 
  
}

void loop(){


}
