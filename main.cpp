#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include<cmath>
#pragma warning(disable:4996)

/*
MPEG-TS packet:
`        3                   2                   1                   0  `
`      1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0  `
`     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ `
`   0 |                             Header                            | `
`     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ `
`   4 |                  Adaptation field + Payload                   | `
`     |                                                               | `
` 184 |                                                               | `
`     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ `


MPEG-TS packet header:
`        3                   2                   1                   0  `
`      1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0  `
`     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ `
`   0 |       SB      |E|S|T|           PID           |TSC|AFC|   CC  | `
`     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ `

Sync byte                    (SB ) :  8 bits
Transport error indicator    (E  ) :  1 bit
Payload unit start indicator (S  ) :  1 bit
Transport priority           (T  ) :  1 bit
Packet Identifier            (PID) : 13 bits
Transport scrambling control (TSC) :  2 bits
Adaptation field control     (AFC) :  2 bits
Continuity counter           (CC ) :  4 bits
*/


//=============================================================================================================================================================================

class xTS
{
public:
    static constexpr uint32_t TS_PacketLength = 188;
    static constexpr uint32_t TS_HeaderLength = 4;

    static constexpr uint32_t PES_HeaderLength = 6;

    static constexpr uint32_t BaseClockFrequency_Hz = 90000; //Hz
    static constexpr uint32_t ExtendedClockFrequency_Hz = 27000000; //Hz
    static constexpr uint32_t BaseClockFrequency_kHz = 90; //kHz
    static constexpr uint32_t ExtendedClockFrequency_kHz = 27000; //kHz
    static constexpr uint32_t BaseToExtendedClockMultiplier = 300;
};

//=============================================================================================================================================================================

class xTS_PacketHeader
{
public:
    enum class ePID : uint16_t
    {
        PAT = 0x0000,
        CAT = 0x0001,
        TSDT = 0x0002,
        IPMT = 0x0003,
        NIT = 0x0010, //DVB specific PID
        SDT = 0x0011, //DVB specific PID
        NuLL = 0x1FFF,
    };

protected:
    //TODO - header fields
    uint8_t SB = 0;
    uint8_t E = 0;
    uint8_t S = 0;
    uint8_t T = 0;
    uint16_t PID = 0;
    uint8_t TSC = 0;
    uint8_t AFC = 0;
    uint8_t CC = 0;

public:
    void     Reset() {

        setSB(0);
        setE(0);
        setS(0);
        setT(0);
        setPID(0);
        setTSC(0);
        setAFC(0);
        setCC(0);

    }
    void     Parse(const uint8_t* Input) {

        uint32_t newIn = 0;
        newIn <<= 8;
        newIn = newIn | Input[0];
        newIn <<= 8;
        newIn = newIn | Input[1];
        newIn <<= 8;
        newIn = newIn | Input[2];
        newIn <<= 8;
        newIn = newIn | Input[3];


        uint8_t SB = getSB();
        uint8_t E = getE();
        uint8_t S = getS();
        uint8_t T = getT();
        uint16_t PID = getPID();
        uint8_t TSC = getTSC();
        uint8_t AFC = getAFC();
        uint8_t CC = getCC();


        uint32_t SBmask = 0xff000000;
        uint32_t Emask = 0x00800000;
        uint32_t Smask = 0x00400000;
        uint32_t Tmask = 0x00200000;
        uint32_t PIDmask = 0x001fff00;
        uint32_t TSCmask = 0x000000c0;
        uint32_t AFCmask = 0x00000030;
        uint32_t CCmask = 0x0000000f;



        SB = ((newIn & SBmask) >> 24);
        E = ((newIn & Emask) >> 23);
        S = ((newIn & Smask) >> 22);
        T = ((newIn & Tmask) >> 21);
        PID = ((newIn & PIDmask) >> 8);
        TSC = ((newIn & TSCmask) >> 6);
        AFC = ((newIn & AFCmask) >> 4);
        CC = ((newIn & CCmask));

        setSB(SB);
        setE(E);
        setS(S);
        setT(T);
        setPID(PID);
        setTSC(TSC);
        setAFC(AFC);
        setCC(CC);
        ///////
    }
    void     Print() const {

        uint8_t SB = getSB();
        uint8_t E = getE();
        uint8_t S = getS();
        uint8_t T = getT();
        uint16_t PID = getPID();
        uint8_t TSC = getTSC();
        uint8_t AFC = getAFC();
        uint8_t CC = getCC();

        printf("TS : ");
        printf(" SB = %d ", SB);
        printf(" E = %d ", E);
        printf(" S = %d ", S);
        printf(" T = %d ", T);
        printf(" PID = %d ", PID);
        printf(" TSC = %d ", TSC);
        printf(" AFC = %d ", AFC);
        printf(" CC = %d ", CC);
    }

public:
    //TODO - direct acces to header values
    const uint8_t getSB()       const { return SB; };
    const uint8_t getE()        const { return E; };
    const uint8_t getS()        const { return S; };
    const uint8_t getT()        const { return T; };
    const uint16_t getPID()     const { return PID; };
    const uint8_t getTSC()      const { return TSC; };
    const uint8_t getAFC()      const { return AFC; };
    const uint8_t getCC()       const { return CC; };


    void setSB(uint8_t tmp) { SB = tmp; }
    void setE(uint8_t tmp) { E = tmp; }
    void setS(uint8_t tmp) { S = tmp; }
    void setT(uint8_t tmp) { T = tmp; }
    void setPID(uint8_t tmp) { PID = tmp; }
    void setTSC(uint8_t tmp) { TSC = tmp; }
    void setAFC(uint8_t tmp) { AFC = tmp; }
    void setCC(uint8_t tmp) { CC = tmp; }

public:
    //TODO
    bool     hasAdaptationField() const {
        uint8_t AFC = getAFC();
        if (AFC == 2 || AFC == 3) {
            return true;
        }
        else {
            return false;
        }
    }
    bool     hasPayload() const { /*TODO*/
        uint8_t AFC = getAFC();
        if (AFC == 3) {
            return true;
        }
        else {
            return false;
        }
    }
};



//=============================================================================================================================================================================
class xTS_AdaptationField
{
protected:
    uint8_t AFL = 0;
    uint8_t DC = 0;
    uint8_t RA = 0;
    uint8_t SP = 0;
    uint8_t PR = 0;
    uint8_t OR = 0;
    uint8_t SP2 = 0;
    uint8_t TP = 0;
    uint8_t EX = 0;

public:
    void Reset() {
        setAFL(0);
        setDC(0);
        setRA(0);
        setSP(0);
        setPR(0);
        setOR(0);
        setSP2(0);
        setTP(0);
        setEX(0);
    }
    void Parse(const uint8_t* Input, uint8_t AdaptationFieldControl) {

        uint8_t AFL = getAFL();
        uint8_t DC = getDC();
        uint8_t RA = getRA();
        uint8_t SP = getSP();
        uint8_t PR = getPR();
        uint8_t OR = getOR();
        uint8_t SP2 = getSP2();
        uint8_t TP = getTP();
        uint8_t EX = getEX();

        uint32_t newIn = 0;
        newIn = Input[5];

        uint32_t newInDC = newIn >> 7;
        uint32_t newInRA = newIn >> 6;
        uint32_t newInSP = newIn >> 5;
        uint32_t newInPR = newIn >> 4;
        uint32_t newInOR = newIn >> 3;
        uint32_t newInSP2 = newIn >> 2;
        uint32_t newInTP = newIn >> 1;
        uint32_t newInEX = newIn;



        uint32_t mask = 0x00000001;

        AFL = Input[4];

        if (newInDC & mask) {
            DC = (newInDC & mask);
        }
        if (newInRA & mask) {
            RA = (newInRA & mask);
        }
        if (newInSP & mask) {
            SP = (newInSP & mask);
        }
        if (newInPR & mask) {
            PR = (newInPR & mask);
        }
        if (newInOR & mask) {
            OR = (newInOR & mask);
        }
        if (newInSP2 & mask) {
            SP2 = (newInSP2 & mask);
        }
        if (newInTP & mask) {
            TP = (newInTP & mask);
        }
        if (newInEX & mask) {
            EX = (newInEX & mask);
        }


        setAFL(AFL);
        setDC(DC);
        setRA(RA);
        setSP(SP);
        setPR(PR);
        setOR(OR);
        setSP2(SP2);
        setTP(TP);
        setEX(EX);
    }
    void Print() const {

        uint8_t AFL = getAFL();
        uint8_t DC = getDC();
        uint8_t RA = getRA();
        uint8_t SP = getSP();
        uint8_t PR = getPR();
        uint8_t OR = getOR();
        uint8_t SP2 = getSP2();
        uint8_t TP = getTP();
        uint8_t EX = getEX();

        printf("AF : ");
        printf(" L = %d ", getNumBytes());
        printf(" DC = %d ", DC);
        printf(" RA = %d ", RA);
        printf(" SP = %d ", SP);
        printf(" PR = %d ", PR);
        printf(" OR = %d ", OR);
        printf(" SP = %d ", SP2);
        printf(" TP = %d ", TP);
        printf(" EX = %d ", EX);
    }

    const uint8_t getAFL()  const { return AFL; };
    const uint8_t getDC()   const { return DC; };
    const uint8_t getRA()   const { return RA; };
    const uint8_t getSP()   const { return SP; };
    const uint8_t getPR()   const { return PR; };
    const uint8_t getOR()   const { return OR; };
    const uint8_t getSP2()  const { return SP2; };
    const uint8_t getTP()   const { return TP; };
    const uint8_t getEX()   const { return EX; };

    void setAFL(uint8_t tmp) { AFL = tmp; };
    void setDC(uint8_t tmp) { DC = tmp; };
    void setRA(uint8_t tmp) { RA = tmp; };
    void setSP(uint8_t tmp) { SP = tmp; };
    void setPR(uint8_t tmp) { PR = tmp; };
    void setOR(uint8_t tmp) { OR = tmp; };
    void setSP2(uint8_t tmp) { SP2 = tmp; };
    void setTP(uint8_t tmp) { TP = tmp; };
    void setEX(uint8_t tmp) { EX = tmp; };


    uint32_t getNumBytes() const {
        uint8_t AFL = getAFL();
        return AFL;
    }
};

//=============================================================================================================================================================================

class xPES_PacketHeader
{
public:
    enum eStreamId : uint8_t
    {
        eStreamId_program_stream_map = 0xBC,
        eStreamId_padding_stream = 0xBE,
        eStreamId_private_stream_2 = 0xBF,
        eStreamId_ECM = 0xF0,
        eStreamId_EMM = 0xF1,
        eStreamId_program_stream_directory = 0xFF,
        eStreamId_DSMCC_stream = 0xF2,
        eStreamId_ITUT_H222_1_type_E = 0xF8,
    };

protected:
    uint32_t m_PacketStartCodePrefix;
    uint8_t m_StreamId;
    uint16_t m_PacketLength;

    //nie odczytuje ich, dlatego brak funkcji dostêpu 

    uint8_t PESScramblingControl;
    uint8_t PESPriority;
    uint8_t dataAlignmentIndicator;
    uint8_t copyright;
    uint8_t originalOrCopy;
    uint8_t PTSDTSFlags;
    uint8_t ESCRFlag;
    uint8_t ESRateFlag;
    uint8_t DSMTrickModeFlag;
    uint8_t additionalCopyInfoFlag;
    uint8_t PESCRCFlag;
    uint8_t PESExtensionFlag;

    //koniec

    uint8_t m_HeaderDataLength;

public:
    void Reset() {

        setPacketStartCodePrefix(0);
        setStreamId(0);
        setPacketLenght(0);
    }
    int32_t Parse(const uint8_t* Input) {
        uint32_t newIn = 0;
        newIn <<= 8;
        newIn = newIn | Input[0];
        newIn <<= 8;
        newIn = newIn | Input[1];
        newIn <<= 8;
        newIn = newIn | Input[2];

        setPacketStartCodePrefix(newIn);

        newIn = 0;
        newIn = Input[3];

        setStreamId(newIn);

        newIn = 0;
        newIn = Input[4];
        newIn <<= 8;
        newIn = newIn | Input[5];

        setPacketLenght(newIn);

        if (m_StreamId != eStreamId::eStreamId_program_stream_map
            && m_StreamId != eStreamId::eStreamId_padding_stream
            && m_StreamId != eStreamId::eStreamId_private_stream_2
            && m_StreamId != eStreamId::eStreamId_ECM
            && m_StreamId != eStreamId::eStreamId_EMM
            && m_StreamId != eStreamId::eStreamId_program_stream_directory
            && m_StreamId != eStreamId::eStreamId_DSMCC_stream
            && m_StreamId != eStreamId::eStreamId_ITUT_H222_1_type_E) {

            setHeaderDataLength(Input[8]);
        }
        return 0;
    }
    void Print() const {

        uint32_t m_PacketStartCodePrefix = getPacketStartCodePrefix();
        uint8_t m_StreamId = getStreamId();
        uint16_t m_PacketLength = getPacketLength();

        printf("PES: ");
        printf("PSCP=%d ", m_PacketStartCodePrefix);
        printf("SID=%d ", m_StreamId);
        printf("L=%d ", m_PacketLength);
    }
public:
    uint32_t getPacketStartCodePrefix()     const { return m_PacketStartCodePrefix; }
    uint8_t getStreamId()                   const { return m_StreamId; }
    uint16_t getPacketLength()              const { return m_PacketLength; }

    uint8_t getHeaderDataLength()           const { return m_HeaderDataLength; }

    void setPacketStartCodePrefix(uint32_t tmp) { m_PacketStartCodePrefix = tmp; }
    void setStreamId(uint8_t tmp) { m_StreamId = tmp; }
    void setPacketLenght(uint16_t tmp) { m_PacketLength = tmp; }

    void setHeaderDataLength(uint8_t tmp) { m_HeaderDataLength = tmp; }
};

//=============================================================================================================================================================================

class xPES_Assembler
{
public:
    enum class eResult : int32_t
    {
        UnexpectedPID = 1,
        StreamPacketLost,
        AssemblingStarted,
        AssemblingContinue,
        AssemblingFinished,
    };

protected:
    int32_t m_PID;
    uint8_t* m_Buffer;
    uint32_t m_BufferSize;
    uint32_t m_DataOffset;
    int8_t m_LastContinuityCounter;
    bool m_Started;
    xPES_PacketHeader m_PESH;

    FILE* m_File;

public:
    xPES_Assembler() {
        m_PID = 0;
        m_Buffer = 0;
        m_BufferSize = 0;
        m_DataOffset = 0;
        m_LastContinuityCounter = -1;
        m_Started = false;
        m_File = NULL;
    }
    ~xPES_Assembler() {
        fclose(m_File);
    }

    void Init(int32_t PID) {
        m_PID = PID;
        m_File = fopen("PID136.mp2", "wb");
        m_LastContinuityCounter = -1;
    }
    eResult AbsorbPacket(const uint8_t* TransportStreamPacket, const xTS_PacketHeader* PacketHeader, const xTS_AdaptationField* AdaptationField) {

        uint8_t CC = (*PacketHeader).getCC();
        bool AF = (*PacketHeader).hasAdaptationField();
        uint8_t AFL = (*AdaptationField).getNumBytes();
       

        m_Started = (*PacketHeader).getS();

        uint32_t sizePacket = xTS::TS_PacketLength;
        uint32_t sizeHeader = xTS::TS_HeaderLength;
        uint32_t size = 188;
        uint32_t beginSize = 4;


        if (m_LastContinuityCounter != -1 && 
            (PacketHeader->getCC() - m_LastContinuityCounter != 1 && 
                PacketHeader->getCC() - m_LastContinuityCounter != -15))
        {
            xBufferReset();
            return eResult::StreamPacketLost;
        }


        if (AF) {

            beginSize = sizeHeader + AFL + 1;
        }
        size = size - beginSize;
        
        m_LastContinuityCounter = CC;
        
        if (m_Started) {   
            m_PESH.Reset();
            m_PESH.Parse(TransportStreamPacket + beginSize);
            xBufferAppend(TransportStreamPacket + beginSize, size);
            return eResult::AssemblingStarted;
        }
        if ((CC + 1) % 16 == 0) {
            xBufferAppend(TransportStreamPacket + beginSize, size);
            m_Started = false;
            return eResult::AssemblingFinished;
        }else {
            xBufferAppend(TransportStreamPacket + beginSize, size);
            return eResult::AssemblingContinue;
        }
        
    }

    void PrintPESH()            const { m_PESH.Print(); }
    uint8_t* getPacket() { return m_Buffer; }
    int32_t getNumPacketBytes() const { return m_DataOffset; }
    xPES_PacketHeader getm_PESH() { return m_PESH; }


protected:
    void xBufferReset() {
        m_BufferSize = 0;
        m_DataOffset = 0;
    }
    void xBufferAppend(const uint8_t* Data, int32_t Size) {
        int i = 0;
        if (m_Started) {
            i = 5;
            xBufferReset();
            m_BufferSize = m_PESH.getPacketLength() - (getm_PESH().getHeaderDataLength() - 6);
            if (!m_Buffer) {
                m_Buffer = (uint8_t*)malloc(sizeof(uint8_t*) * m_BufferSize);
            }
            else {
                uint8_t* tmp = (uint8_t*)realloc(m_Buffer, sizeof(uint8_t*) * m_BufferSize);
                if (tmp != NULL)
                {
                    m_Buffer = tmp;
                }
                else {
                    free(m_Buffer);
                    exit(0);
                }
            }
        }
        for (i ; i < Size; i++) {
            m_Buffer[m_DataOffset++] = Data[i];
        }
        if (m_DataOffset == m_BufferSize) {
            fwrite(m_Buffer, 1, m_BufferSize, m_File);
        }
        
        
    }
};





int main(int argc, char* argv[], char* envp[])
{

    // TODO - open file

    FILE* file;
    file = fopen("example_new.ts", "rb");

    if (!file) {
        printf(" Brak dostepu do pliku");
        return 0;
    }
    uint8_t TS_PacketBuffer[xTS::TS_PacketLength];

    xTS_PacketHeader    TS_PacketHeader;
    xTS_AdaptationField TS_AdaptationField;
    xPES_Assembler PES_Assembler;
    PES_Assembler.Init(136);
    int32_t TS_PacketId = 0;

    while (!feof(file))
    {
        size_t result = fread(TS_PacketBuffer, 1, xTS::TS_PacketLength, file);


        if (result != xTS::TS_PacketLength) {
            fputs("Reading error", stderr);
            break;
        }


        TS_PacketHeader.Reset();
        TS_PacketHeader.Parse(TS_PacketBuffer);
        TS_AdaptationField.Reset();
        if (TS_PacketHeader.getSB() == 'G' && TS_PacketHeader.getPID() == 136) {
            if (TS_PacketHeader.hasAdaptationField()) {
                TS_AdaptationField.Parse(TS_PacketBuffer, TS_PacketHeader.getAFC());
            }
            printf("%010d ", TS_PacketId);
            TS_PacketHeader.Print();
            if (TS_PacketHeader.hasAdaptationField()) {
                TS_AdaptationField.Print();
            }
            xPES_Assembler::eResult Result = PES_Assembler.AbsorbPacket(TS_PacketBuffer, &TS_PacketHeader, &TS_AdaptationField);
            switch (Result)
            {
            case xPES_Assembler::eResult::StreamPacketLost:
                printf("PcktLost ");
                break;
            case xPES_Assembler::eResult::AssemblingStarted:
                printf("Started "); PES_Assembler.PrintPESH();
                break;
            case xPES_Assembler::eResult::AssemblingContinue:
                printf("Continue ");
                break;
            case xPES_Assembler::eResult::AssemblingFinished:
                printf("Finished ");
                printf("PES: Len=%d" , PES_Assembler.getm_PESH().getPacketLength() + 6);
                break;
            default:
                break;
            }
            printf("\n");
        }

        TS_PacketId++;
        //if (TS_PacketId == 20000) break;
    }
    printf("END PROGRAM");
    fclose(file);
    return 0;

}

