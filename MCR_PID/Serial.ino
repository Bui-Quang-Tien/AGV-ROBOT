#include "Define.h"
unsigned char byteReceived1;
unsigned char byteReceived2;
unsigned char byteReceived3;
unsigned char serialData_Promini[3];
unsigned char index1 = 0;
unsigned char serialData_Microbit[20]; /////////9
unsigned char index2 = 0;
unsigned char serialData_RFID[3];
unsigned char index3 = 0;
volatile unsigned char mode = 1; ////////////////////////mode = 100
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
int API  = 0;
int isAPI1 = 0;
int isAPI2 = 0;
int isAPI3 = 0;
int isAPI4 = 0;
int isAPI5 = 0;
int isAPI6 = 0;
int isAPI7 = 0;
int numberOfLine = 0;
int distanceToStop = 0;
int distanceToRun = 0;
int speedRun = 0;
int overDistance = 0;
String CMD = "";

void serial0Process()
{
    if (Serial.available() > 0)
    {
        CMD = Serial.readStringUntil("!");
       // Serial.println(CMD);
        if (CMD.indexOf('!') > 0)
        {
            Serial.println("#OK!");

            int line_index = CMD.indexOf("LINE");
            int run_index = CMD.indexOf("RUN");
            int left_index = CMD.indexOf("LEFT");
            int right_index = CMD.indexOf("RIGHT");
            int back_index = CMD.indexOf("BACK");
            int stop_index = CMD.indexOf("STOP");
            int bward_index = CMD.indexOf("BWARD");
            
            if (line_index >= 0)
            {
                int speed_index = CMD.indexOf("SPEED");
                int over_index = CMD.indexOf("OVER");
                String line_sub = CMD.substring(line_index + 4, CMD.indexOf('S'));
                numberOfLine = line_sub.toInt();
                String speed_sub = CMD.substring(speed_index + 5, CMD.indexOf('O'));
                speedRun = speed_sub.toInt();
                String over_sub = CMD.substring(over_index + 4, CMD.indexOf('!'));
                overDistance = over_sub.toInt();
                isAPI7 = 1;
                API = 7;
            }

            
            else if (run_index >= 0)
            {
                int speed2_index = CMD.indexOf("SPEED");
                String run_sub = CMD.substring(run_index + 3, CMD.indexOf('S'));
                distanceToRun= run_sub.toInt();
                String speed2_sub = CMD.substring(speed2_index + 5, CMD.indexOf('!'));
                speedRun= speed2_sub.toInt();
                isAPI1 = 1;
                API = 1;
            }

            
            else if (left_index >= 0)
            {
                String left_sub = CMD.substring(left_index + 4, CMD.indexOf('!'));
                speedRun = left_sub.toInt();
                isAPI2 = 1;
                API = 2;
            }


            else if (right_index >= 0)
            {
                String right_sub = CMD.substring(right_index + 5, CMD.indexOf('!'));
                speedRun = right_sub.toInt();
                isAPI3 = 1;
                API = 3;
            }

            
            else if (back_index >= 0)
            {
                String back_sub = CMD.substring(back_index + 4, CMD.indexOf('!'));
                speedRun = back_sub.toInt();
                isAPI4 = 1;
                API = 4;
            }

            
            else if (stop_index >= 0)
            {
                String stop_sub = CMD.substring(stop_index + 4, CMD.indexOf('!'));
                distanceToStop= stop_sub.toInt();
                isAPI1 = 0;
                isAPI7 = 0;
                isAPI6 = 0;
                isAPI5 = 1;
                API = 5;
            }

            
            else if (bward_index >= 0)
            {
                int speed3_index = CMD.indexOf("SPEED");
                String bward_sub = CMD.substring(bward_index + 5, CMD.indexOf('S'));
                distanceToRun= bward_sub.toInt();
                String speed3_sub = CMD.substring(speed3_index + 5, CMD.indexOf('!'));
                speedRun= speed3_sub.toInt();
                isAPI6 = 1;
                API = 6;
            }
            
            Serial.println(CMD);
           // CMD = "";
            Serial.print("NumberOfLine: ");
            Serial.println(numberOfLine);
            Serial.print("speed: ");
            Serial.println(speedRun);
            Serial.print("distanceToRun: ");
            Serial.println(distanceToRun);
            Serial.print("distanceToStop: ");
            Serial.println(distanceToStop);
            Serial.print("API: ");
            Serial.println(API);

        }
    }
}

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
            mode = 1;
            toGo = serialData_Microbit[13] - 48;
            index2 = 0;
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
        if ((index3 == 3) && (isPosData == 0) && (isLine == 0))
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