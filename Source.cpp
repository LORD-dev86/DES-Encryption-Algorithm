#include <iostream>
#include <string> 
#include <stdio.h>
#include <stdlib.h>
#include "const.h"

using namespace std;

#pragma region __Function__
template<typename Type> void prn(Type y)
{
    for (int i = sizeof(y) * 8; i != 0; --i)
    {
        cout << ((y >> i - 1) & 1);
        if (i % 8 == 1)
            cout << " ";
    }
    cout << "\n";
}

template<typename Type> Type valOFbit64(Type x, int numbOFbit)
{
    unsigned __int64 mask = 0x0000000000000001;
    int sdvig = 64 - numbOFbit;
    x >>= sdvig;

    x &= mask;

    return x;
}

template<typename Type> Type valOFbit56(Type x, int numbOFbit)
{
    unsigned __int64 mask = 0x0000000000000001;
    int sdvig = 56 - numbOFbit;
    x >>= (sdvig);

    x &= mask;

    return x;
}

template<typename Type> Type valOFbit32(Type x, int numbOFbit)
{
    unsigned __int64 mask = 0x0000000000000001;
    int sdvig = 32 - numbOFbit;
    x >>= (sdvig);

    x &= mask;

    return x;
}

template<typename Type> Type low_part(Type x)
{
    unsigned __int64 mask = 0x000000000FFFFFFF;
    x &= mask;

    return x;
}

template<typename Type> Type high_part(Type x)
{
    unsigned __int64 mask = 0x00FFFFFFF0000000;
    x &= mask;
    x >>= 28;

    return x;
}

template<typename Type> Type sdvig(Type x, int shiftval)
{
    unsigned __int32 tmp = 0x00000000;
    unsigned __int32 mask = 0x30000000;
    unsigned __int32 aftermask = 0x00000000;
    aftermask = x & mask;
    if (shiftval == 1)
        aftermask >>= 29;
    else aftermask >>= 28;

    x = x | aftermask;

    return x;
}

int replaceNum(int count, int x, int y)
{
    return S[count][(16 * x) + y];
}

unsigned __int32 hashFun(unsigned __int32 lowPart, unsigned __int64 key)
{
    unsigned __int64 lowPart48 = 0x0;
    for (int i = 0; i < 48; ++i)
    {
        unsigned __int64 tmp = 0;
        tmp = (lowPart >> 32 - E[i]) & 1;
        lowPart48 |= tmp << (47 - i);
    }

    unsigned __int64 lowPart_ = lowPart48 ^ key;
    //unsigned __int64 lowPart_ = 0x965a847dcbd6;

    unsigned __int32* po6 = new unsigned __int32[8];
    unsigned __int64* repl = new unsigned __int64[8];

    for (int i = 0; i < 8; ++i)
    {
        unsigned __int64 tmp = 0;
        tmp = (lowPart_ >> (48 - 6 * (i + 1)) & 0x3f);
        po6[i] = tmp;
    }

    int po2[8][2];
    for (int i = 0; i < 8; ++i) {
        po2[i][0] = (((po6[i] >> 5) & 0x1) << 1) | (po6[i] & 0x1);
        po2[i][1] = (po6[i] >> 1) & 0xf;
    }
    for (int i = 0; i < 8; ++i) {
        repl[i] = replaceNum(i, po2[i][0], po2[i][1]);
    }

    unsigned __int32 result = 0x0;
    unsigned __int32 resultWp = 0x0;
    unsigned __int32 tmp = 0x0;

    for (int i = 0; i < 8; i++)
    {
        result |= (repl[i] << (7 - i) * 4);
    }
    for (int i = 0; i < P_size; i++)
    {
        tmp = valOFbit32(result, P[i]);
        tmp <<= (P_size - i - 1);
        resultWp |= tmp;
    }

    //cout << "res " << hex << result;
    //cout << " resWp " <<  hex << resultWp << endl;
    return resultWp;
}

#pragma endregion

void main()
{

#pragma region __Generation Keys__

    unsigned __int64 start = 0xAABB09182736CCDD;
    unsigned __int64 start1;
    unsigned __int64 finish = 0x0;
    unsigned __int64 lpart;
    unsigned __int64 hpart;

    cout << "before" << endl;
    prn(start);

    for (int i = 0; i < PC1_size; ++i) {
        start1 = valOFbit64(start, PC1[i]);
        start1 <<= (PC1_size - i - 1);
        finish = finish | start1;
    }
    cout << "after" << endl;
    prn(finish);

    hpart = high_part(finish);
    lpart = low_part(finish);

    /*cout << "\n\tstart\n" << hex << start << endl << "\n\tfinish\n" << hex << finish << endl
        << "\n\n\tHigh part\n" << hex << hpart << "\n\n\tLow part\n" << hex << lpart << endl;*/

    unsigned __int64 fkey = 0x0;
    unsigned __int64 masfkey[16];
    for (int i = 0; i < shift_size; ++i) {
        for (int j = 0; j < shift[i]; ++j) {
            lpart = (lpart << 1 & 0x000000000fffffff) | ((lpart >> ((unsigned __int64)27)));
            hpart = (hpart << 1 & 0x000000000fffffff) | ((hpart >> ((unsigned __int64)27)));
            fkey = (hpart << 28) | lpart;
            masfkey[i] = fkey;
        }
    }

    unsigned __int64 finish1 = 0x0;
    unsigned __int64 ekmas[16];
    unsigned __int64 dkmas[16];
    unsigned __int64 start2;
    //cout << "\n\tekmas" << endl;
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < PC2_size; ++j) {
            start2 = valOFbit56(masfkey[i], PC2[j]);
            start2 <<= (PC2_size - j - 1);
            finish1 = finish1 | start2;
        }
        ekmas[i] = finish1;
        finish1 = 0x0;
        //cout << hex << ekmas[i] << endl;
    }

    //cout << "\n\tdkmas" << endl;

    for (int i = 0; i < 16; ++i) {
        dkmas[i] = ekmas[15 - i];
        //cout << hex << dkmas[i] << endl;
    }
#pragma endregion

#pragma region __Generation Encryption Data__
    unsigned __int64 Data = 0x14A7D67818CA18AD;
    unsigned __int64 Data1;
    unsigned __int64 DataFinish = 0x0;

    cout << "before" << endl << hex << Data << endl;
    prn(Data);

    for (int i = 0; i < IP_size; ++i) {
        Data1 = valOFbit64(Data, IP_1[i]);
        Data1 <<= (IP_size - i - 1);
        DataFinish = DataFinish | Data1;
    }
    cout << "after" << endl << hex << DataFinish << endl;
    prn(DataFinish);

    unsigned __int32 LPartDF;
    unsigned __int32 HPartDF;
    HPartDF = (DataFinish & 0xffffffff00000000) >> 32;
    LPartDF = (DataFinish & 0x00000000ffffffff);

    cout << "\nHigh: " << hex << HPartDF << endl;
    cout << "Low: " << hex << LPartDF << endl;
#pragma endregion

#pragma region __DES Encryption__
    unsigned __int64 num = 0x14a7d67818ca18ad;
    unsigned __int32 resRP[17];
    unsigned __int32 resLP[17];
    unsigned __int32 LPartNM;
    unsigned __int32 RPartNM;

    resRP[0] = (num & 0x00000000ffffffff);
    resLP[0] = (num & 0xffffffff00000000) >> 32;

    for (int i = 1; i <= cycleCount; ++i) {

        if (i != 16) {
            resRP[i] = hashFun(resRP[i - 1], ekmas[i - 1]) ^ resLP[i - 1];
            resLP[i] = resRP[i - 1];
        }
        else {
            resLP[i] = hashFun(resRP[i - 1], ekmas[i - 1]) ^ resLP[i - 1];
            resRP[i] = resRP[i - 1];
        }
    }

    /*for (int i = 0; i <= cycleCount; ++i)
    {
        cout << hex << resLP[i] << "\t\t";
        cout << hex << resRP[i] << endl;
    }*/

    unsigned __int64 resNum = ((unsigned __int64)resLP[16] << 32 | resRP[16]);
    cout << hex << resNum << endl;
    unsigned __int64 tmpNum = 0x0, resNumFinish = 0x0;
    for (int i = 0; i < IP_size; ++i) {
        tmpNum = valOFbit64(resNum, IP_1[i]);
        tmpNum <<= (IP_size - i - 1);
        resNumFinish |= tmpNum;
    }
    cout << "resnumfinish: " << hex << resNumFinish << endl;
#pragma endregion

#pragma region __DES Decryption__

    //TBD...

#pragma endregion

}
