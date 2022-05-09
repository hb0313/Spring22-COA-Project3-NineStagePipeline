/*
Name - Harshad Bhandwaldar
University - SUNY Binghamton
Course - Computer Organization and Architecture
*/
#include <stdio.h>

typedef enum { false, true } boolean;
#define REG_COUNT 16
#define INVALID_REG 17

/*====================================
  ******** GLOBAL VARIBALES ********
=====================================*/
int tempIndex;
int Ry = 0, Rz = 0, Rx = 0, destReg = 0, i = 3, request_done = 0, regs[16], execution_cycles ;
boolean EXEC_FLAG = false;
char buffer[4];
size_t BUFFER_SIZE;
char INSTRUCTION_REG[4], OPCODE_OP; // 0 : OPCODE 1 : Destination Address, 2 : Left Operand, 3 : Right Operand
int DECODER_QUEUE[3];

//NEW
int data_hazard_count = 0;
int control_hazard_count = 0;
int completed_instructions = 0;

/*====================================
  ******** DECODE(ID) STAGE ********
=====================================*/
int ID_INSTRUCTION()
{
    // cout<<"\nindide ID_INST";
    DECODER_QUEUE[0] = INSTRUCTION_REG[1] & 0xff; // Convert destination to integer
    DECODER_QUEUE[1] = INSTRUCTION_REG[2] & 0xff; // Convert Left Opernad to integer
    DECODER_QUEUE[2] = INSTRUCTION_REG[3] & 0xff; // Convert Right Operand to integer
    OPCODE_OP = INSTRUCTION_REG[0];               // Assign HE value of OPCODE to comparision variable
    return 0;
}

/*====================================
  ******** FECTH(IF) STAGE ********
=====================================*/
int IF_FETCH()
{

    tempIndex = i;
    INSTRUCTION_REG[0] = buffer[tempIndex];
    INSTRUCTION_REG[1] = buffer[--tempIndex];
    INSTRUCTION_REG[2] = buffer[--tempIndex];
    INSTRUCTION_REG[3] = buffer[--tempIndex];
    i = i + 4;
    return 0;
}


/*=================================================
  ******** INSTRUCTION ANALYZE(IA) STAGE ********
==================================================*/
int IA_ANALYZE()
{
    
}



/*======================================================================
  ******** EXECUTION(EX) STAGE - EX:1 - Move values to Registers ********
========================================================================*/
void EX_ALU(int *Rx, int *Ry, int *Rz)
{
    *Rx = DECODER_QUEUE[0]; // Assign decoded value of destination register
    *Ry = DECODER_QUEUE[1]; // Assign decoder value of left operand to/or register
    *Rz = DECODER_QUEUE[2]; // Assign decoder value of right operand to/or register

    //**********If not fetch change Execution flag to TRUE and process the FETHCH_COUNTER***********
    for (int FETCH_COUNTER = 0; FETCH_COUNTER < 3 && i >= BUFFER_SIZE; FETCH_COUNTER++, execution_cycles++)
    {
        EXEC_FLAG = true;
    }
}

/*====================================
  ******** EXECUTE(EF) STAGE ********
=====================================*/
void EX_EXECUTE()
{
    request_done++;

    // *** Compare OPCODE is SET ***//
    if (OPCODE_OP == '\x00')
    {
        OPCODE_x00(Rx, Ry);
    }
    // *** Compare OPCODE is ADD ***//
    else if (OPCODE_OP == '\x10')
    {
        OPCODE_x10(Rx, Ry, Rz);
    }
    // *** Compare OPCODE is ADD with Immediate value ***//
    else if (OPCODE_OP == '\x11')
    {
        OPCODE_x11(Rx, Ry, Rz);
    }
    // *** Compare OPCODE is SUB ***//
    else if (OPCODE_OP == '\x20')
    {
        OPCODE_x20(Rx, Ry, Rz);
    }
    // *** Compare OPCODE is SUB with Immediate value ***//
    else if (OPCODE_OP == '\x21')
    {
        OPCODE_x21(Rx, Ry, Rz);
    }
    // *** Compare OPCODE is MUL ***//
    else if (OPCODE_OP == '\x30')
    {
        OPCODE_x30(Rx, Ry, Rz);
    }
    // *** Compare OPCODE is MUL with Immediate value ***//
    else if (OPCODE_OP == '\x31')
    {
        OPCODE_x31(Rx, Ry, Rz);
    }
    // *** Compare OPCODE is DIV ***//
    else if (OPCODE_OP == '\x40')
    {
        OPCODE_x40(Rx, Ry, Rz);
    }
    // *** Compare OPCODE is DIV with Immediate value ***//
    else if (OPCODE_OP == '\x41')
    {
        OPCODE_x41(Rx, Ry, Rz);
    }
}


/*====================================================
  ******** EXECUTION(EX) STAGE - EX:1.1 - SET ********
=====================================================*/
int OPCODE_x00(int Rx, int Ry)
{
    int Rz = 0; // Dummy value
    EX_ALU(&Rx, &Ry, &Rz);
    regs[Rx] = Ry;
    return 0;
}

/*====================================================
  ******** EXECUTION(EX) STAGE - EX:1.2 - ADD ********
=====================================================*/
int OPCODE_x10(int Rx, int Ry, int Rz) // OPCODE ADD
{
    EX_ALU(&Rx, &Ry, &Rz);
    regs[Rx] = regs[Ry] + regs[Rz];
    return 0;
}

/*====================================================
  ******** EXECUTION(EX) STAGE - EX:1.3 - ADD ********
=====================================================*/
int OPCODE_x11(int Rx, int Ry, int Rz) // OPCODE ADD with Immediate value
{
    EX_ALU(&Rx, &Ry, &Rz);
    regs[Rx] = regs[Ry] + Rz;
    return 0;
}

/*====================================================
  ******** EXECUTION(EX) STAGE - EX:1.4 - SUB ********
=====================================================*/
int OPCODE_x20(int Rx, int Ry, int Rz) // OPCODE SUB
{
    EX_ALU(&Rx, &Ry, &Rz);
    regs[Rx] = regs[Ry] - regs[Rz];
    return 0;
}

/*====================================================
  ******** EXECUTION(EX) STAGE - EX:1.5 - SUB ********
=====================================================*/
int OPCODE_x21(int Rx, int Ry, int Rz) // OPCODE SUB with immediate value
{
    EX_ALU(&Rx, &Ry, &Rz);
    regs[Rx] = regs[Ry] - Rz;
    return 0;
}

/*====================================================
  ******** EXECUTION(EX) STAGE - EX:1.6 - MUL ********
=====================================================*/
int OPCODE_x30(int Rx, int Ry, int Rz) // OPCODE MUL
{
    execution_cycles = execution_cycles + 1;
    EX_ALU(&Rx, &Ry, &Rz);
    regs[Rx] = regs[Ry] * regs[Rz];
    return 0;
}

/*====================================================
  ******** EXECUTION(EX) STAGE - EX:1.7 - MUL ********
=====================================================*/
int OPCODE_x31(int Rx, int Ry, int Rz) // OPCODE MUL with Immediate value
{
    execution_cycles = execution_cycles + 1;
    EX_ALU(&Rx, &Ry, &Rz);
    regs[Rx] = regs[Ry] * Rz;
    return 0;
}

/*====================================================
  ******** EXECUTION(EX) STAGE - EX:1.8 - DIV ********
=====================================================*/
int OPCODE_x40(int Rx, int Ry, int Rz) // OPCODE DIV
{
    execution_cycles = execution_cycles + 3;
    EX_ALU(&Rx, &Ry, &Rz);
    regs[Rx] = regs[Ry] / regs[Rz];

    return 0;
}

/*====================================================
  ******** EXECUTION(EX) STAGE - EX:1.9 - DIV ********
=====================================================*/
int OPCODE_x41(int Rx, int Ry, int Rz) // OPCODE DIV with Immediate value
{
    execution_cycles = execution_cycles + 3;
    EX_ALU(&Rx, &Ry, &Rz);
    regs[Rx] = regs[Ry] / Rz;
    return 0;
}


/*================================
  ******** INIT FUNCTION  ********
=================================*/
int main(int argc, char *argv[])
{
    for (int i = 0; i < REG_COUNT; i++)
    {
        regs[i] = 0;
    }

    // **** Open file usinf stream object **** // 
    FILE *file_PTR;
    //ifstream infile(argv[1], ios::in | ios::binary);
    file_PTR = fopen(argv[1],"rb");
    
    boolean OPCODE_SET = false;

    // **** Get the length of file to read ****//
    fread(buffer, sizeof(buffer),1,file_PTR);
    
    // infile.seekg(0, infile.end);
    // size_t length = infile.tellg();
    // infile.seekg(0, infile.beg);

    // **** Read file the file as per the length **** //
    // if (length > 0)
    // {
    //     buffer.resize(length);
    //     infile.read(&buffer[0], length);
    // }

    // **** Get the size of file in varuable BUFFER_SIZE **** //
    // BUFFER_SIZE = buffer.size();
    execution_cycles = -1;
    while (EXEC_FLAG == false)
    {
        ++execution_cycles;
        //=====================================//
        // ****** STAGE 1 ----- FETCH ******* //
        //=====================================//
        IF_FETCH();
        //=====================================//
        // ****** STAGE 2 ----- DECODE ******* //
        //=====================================//
        ID_INSTRUCTION();
        //=====================================//
        // ****** STAGE 3 ----- EXECUTE ****** //
        //=====================================//
        EX_EXECUTE();
    };

    // **** Close the file after completion **** //
    close(file_PTR);

    // **** Print final status **** //
    print_regs();
    printf("Stalled cycles due to data hazard: %d \n", data_hazard_count);
    printf("Stalled cycles due to control hazard: %d \n", control_hazard_count);
    printf("\n");
    printf("Total stalls: %d \n", data_hazard_count+control_hazard_count);
    printf("Total execution cycles: %d\n", execution_cycles);
    printf("Total instruction simulated: %d\n", completed_instructions);
    printf("IPC: %f\n", ((double)completed_instructions/execution_cycles));

    return 0;
}


int print_regs () 
{

    printf("================================\n");
    printf("--------------------------------\n");
    for (int reg=0; reg<16; reg++) {
        printf("REG[%2d]   |   Value=%d  \n",reg, regs[reg]);
        printf("--------------------------------\n");
    }
    printf("================================\n\n");

    return 0;
}
