#include "Define.h"
unsigned char byteReceived1;
unsigned char byteReceived2;
unsigned char byteReceived3;
unsigned char serialData_Promini[3];
unsigned char index1 = 0;
unsigned char serialData_Microbit[20];/////////9
unsigned char index2 = 0;
unsigned char serialData_RFID[3];
unsigned char index3 = 0;
volatile unsigned char mode = 1;////////////////////////mode = 100
volatile unsigned char RFIDpos = 0;
volatile int speed_l = 0;
volatile int speed_r = 0;
volatile unsigned char dir_l = 0;
volatile unsigned char dir_r = 0;
volatile unsigned char music = 0;
volatile unsigned char toGo = 0;
volatile unsigned char isPosData = 0;
extern volatile int realSpeed;
extern unsigned char isRunning;
extern unsigned char DIREC;
extern unsigned char NPS;
extern unsigned char way;
extern unsigned char isLine;
unsigned char STA;

void serial1Process()
{
    if (Serial1.available())
    {
        byteReceived1 = Serial1.read();
        serialData_Promini[index1] = byteReceived1;
        index1++;
        if (serialData_Promini[0] != 0xff)
            index1 = 0;
        if (index1 == 3)
        {
            index1 = 0;
            for (int i = 0; i < 3; i++)
            {
                Serial.print(serialData_Promini[i]);
                Serial.print("  ");
            }
        }
    }
}
// void serial2Process()
// {
//     if (Serial2.available())
//     {
//         byteReceived2 = Serial2.read();
//         serialData_Microbit[index2] = byteReceived2;
//         index2++;
//         if (serialData_Microbit[0] != 0xff)
//             index2 = 0;
//         if (index2 == 9)
//         {
//             if (serialData_Microbit[8] != 0xef)
//             {
//                 index2 = 0;
//                 return;
//             }
//             //   speed_l = 0;
//             //   speed_r = 0;
//             mode = serialData_Microbit[2];
//             toGo = serialData_Microbit[1];
//             if ((mode == 0) || (mode == 1))
//             {
//                 speed_l = serialData_Microbit[3];
//                 dir_l = serialData_Microbit[4];
//                 speed_r = serialData_Microbit[5];
//                 dir_r = serialData_Microbit[6];
//                 music = serialData_Microbit[7];
//                 if (dir_l == 0)
//                     speed_l = -speed_l;
//                 if (dir_r == 0)
//                     speed_r = -speed_r;
//             }
//             index2 = 0;
//             /*  for (int i = 0; i < 9; i++)
//             {
//                 Serial.write(serialData_Microbit[i]);
//             }*/
//             // Serial.println(toGo);
//         }
//     }
// }
////////////////////////////////////////////////////////////////////////////////
void serial2Process()
{
    if (Serial2.available())
    {
        byteReceived2 = Serial2.read();
        serialData_Microbit[index2] = byteReceived2;
        index2++;
     //   Serial.write(byteReceived2);
        if (serialData_Microbit[0] != '!')
        {
            index2 = 0;
            return;
        }
        if (index2 >= 15)
        {
            if (serialData_Microbit[14] != '#')
            {
                Serial2.println("miss");
                Serial.println("miss2");
                index2 = 0;
                return;
            }
            //   speed_l = 0;
            //   speed_r = 0;
            mode = 1;
            toGo = serialData_Microbit[13] - 48;
            index2 = 0;
            /*  for (int i = 0; i < 9; i++)
            {
                Serial.write(serialData_Microbit[i]);
            }*/
        //    Serial.println(toGo);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////
void serial3Process()
{
    if (Serial3.available())
    {
        byteReceived3 = Serial3.read();
        serialData_RFID[index3] = byteReceived3;
        index3++;
        if (serialData_RFID[0] != 0xff)
            index3 = 0;
        if ((index3 == 3) && (isPosData == 0)&&(isLine == 0))
        {
            isPosData = 0;
            RFIDpos = serialData_RFID[1];
            STA = isRunning;
            switch (way)
            {
            case 1:
                switch (RFIDpos)
                {
                case 0:
                    DIREC = 0;
                    NPS = 2;
                    break;
                case 2:
                    DIREC = 0;
                    NPS = 1;
                    break;
                case 1:
                    DIREC = 1;
                    NPS = 4;
                    break;
                case 4:
                    DIREC = 0;
                    NPS = 7;
                    break;
                case 7:
                    DIREC = 0;
                    NPS = 7;
                    break;
                default:
                    break;
                }
                break;
            case 2:
                switch (RFIDpos)
                {
                case 0:
                    DIREC = 0;
                    NPS = 2;
                    break;
                case 2:
                    DIREC = 0;
                    NPS = 3;
                    break;
                case 3:
                    DIREC = 2;
                    NPS = 6;
                    break;
                case 6:
                    DIREC = 0;
                    NPS = 9;
                    break;
                case 9:
                    DIREC = 0;
                    NPS = 9;
                    break;
                default:
                    break;
                }
                break;
            case 3:
                switch (RFIDpos)
                {
                case 7:
                    DIREC = 0;
                    NPS = 4;
                    STA = 0;
                    break;
                case 4:
                    DIREC = 3;
                    NPS = 5;
                    break;
                case 5:
                    DIREC = 2;
                    NPS = 2;
                    break;
                case 2:
                    DIREC = 3;
                    NPS = 0;
                    break;
                case 0:
                 //   DIREC = 3;
                    NPS = 0;
                    break;
                default:
                    break;
                }
                break;
            case 4:
                switch (RFIDpos)
                {
                case 9:
                    DIREC = 0;
                    NPS = 6;
                    STA = 0;
                    break;
                case 6:
                    DIREC = 3;
                    NPS = 5;
                    break;
                case 5:
                    DIREC = 1;
                    NPS = 2;
                    break;
                case 2:
                    DIREC = 3;
                    NPS = 0;
                    break;
                case 0:
                    //   DIREC = 3;
                    NPS = 0;
                    break;
                default:
                    break;
                }
                break;
            case 5:
                switch (RFIDpos)
                {
                case 7:
                    DIREC = 3;
                    NPS = 4;
                    break;
                case 4:
                    DIREC = 2;
                    NPS = 5;
                    break;
                case 5:
                    DIREC = 2;
                    NPS = 6;
                    break;
                case 6:
                    DIREC = 0;
                    NPS = 9;
                    break;
                case 9:

                    break;
                default:
                    break;
                }
                break;
            case 6:
                switch (RFIDpos)
                {
                case 9:
                    DIREC = 3;
                    NPS = 6;
                    break;
                case 6:
                    DIREC = 1;
                    NPS = 5;
                    break;
                case 5:
                    DIREC = 1;
                    NPS = 4;
                    break;
                case 4:
                    DIREC = 0;
                    NPS = 7;
                    break;
                case 7:

                    break;
                default:
                    break;
                }
                break;

            default:
                break;
            }
            //  Serial2.println("!UPT;1;POS:" + (String)RFIDpos + "-" + (String)RFIDpos + "#");
            //  Serial2.println("!UPT;1;CPS:" + (String)RFIDpos + "#");
            //  Serial2.println("!UPT;1;NPS:" + (String)NPS + "#");

            //  Serial2.println("!UPT;1;DIR:" + (String)DIREC + "#");
            Serial2.println("!UPT;1;CND:" + (String)RFIDpos + (String)NPS + (String)DIREC + "#");
            Serial2.println("!UPT;1;SPE:" + (String)realSpeed + "#");
            Serial2.println("!UPT;1;STA:" + (String)isRunning + "#");
            //   Serial.println(RFIDpos);
            index3 = 0;
        }
    }
    if (isPosData == 1)
    {
        STA = isRunning;
        switch (way)
        {
        case 1:
            switch (RFIDpos)
            {
            case 0:
                DIREC = 0;
                NPS = 2;
                break;
            case 2:
                DIREC = 0;
                NPS = 1;
                break;
            case 1:
                DIREC = 1;
                NPS = 4;
                break;
            case 4:
                DIREC = 0;
                NPS = 7;
                break;
            case 7:
                DIREC = 0;
                NPS = 7;
                break;
            default:
                break;
            }
            break;
        case 2:
            switch (RFIDpos)
            {
            case 0:
                DIREC = 0;
                NPS = 2;
                break;
            case 2:
                DIREC = 0;
                NPS = 3;
                break;
            case 3:
                DIREC = 2;
                NPS = 6;
                break;
            case 6:
                DIREC = 0;
                NPS = 9;
                break;
            case 9:
                DIREC = 0;
                NPS = 9;
                break;
            default:
                break;
            }
            break;
        case 3:
            switch (RFIDpos)
            {
            case 7:
                DIREC = 0;
                NPS = 4;
                STA = 0;
                break;
            case 4:
                DIREC = 3;
                NPS = 5;
                break;
            case 5:
                DIREC = 2;
                NPS = 2;
                break;
            case 2:
                DIREC = 3;
                NPS = 0;
                break;
            case 0:
              //  DIREC = 0;
                NPS = 0;
                break;
            default:
                break;
            }
            break;
        case 4:
            switch (RFIDpos)
            {
            case 9:
                DIREC = 0;
                NPS = 6;
                STA = 0;
                break;
            case 6:
                DIREC = 3;
                NPS = 5;
                break;
            case 5:
                DIREC = 1;
                NPS = 2;
                break;
            case 2:
                DIREC = 3;
                NPS = 0;
                break;
            case 0:
                //   DIREC = 3;
                NPS = 0;
                break;
            default:
                break;
            }
            break;
        case 5:
            switch (RFIDpos)
            {
            case 7:
                DIREC = 3;
                NPS = 4;
                break;
            case 4:
                DIREC = 2;
                NPS = 5;
                break;
            case 5:
                DIREC = 2;
                NPS = 6;
                break;
            case 6:
                DIREC = 0;
                NPS = 9;
                break;
            case 9:

                break;
            default:
                break;
            }
            break;
        case 6:
            switch (RFIDpos)
            {
            case 9:
                DIREC = 3;
                NPS = 6;
                break;
            case 6:
                DIREC = 1;
                NPS = 5;
                break;
            case 5:
                DIREC = 1;
                NPS = 4;
                break;
            case 4:
                DIREC = 0;
                NPS = 7;
                break;
            case 7:

                break;
            default:
                break;
            }
            break;

        default:
            break;
        }
        //  Serial2.println("!UPT;1;POS:" + (String)RFIDpos + "-" + (String)RFIDpos + "#");
        //   Serial2.println("!UPT;1;CPS:" + (String)RFIDpos + "#");
        // Serial2.println("!UPT;1;NPS:" + (String)NPS + "#");
        // Serial2.println("!UPT;1;DIR:" + (String)DIREC + "#");
        Serial2.println("!UPT;1;CND:" + (String)RFIDpos + (String)NPS + (String)DIREC + "#");
        Serial2.println("!UPT;1;SPE:" + (String)realSpeed + "#");
        Serial2.println("!UPT;1;STA:" + (String)STA + "#");
        //   Serial.println(RFIDpos);

        index3 = 0;
        isPosData = 0;
    }
}