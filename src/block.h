#pragma once
enum BlockType{
    Block_I,
    Block_T,
    Block_O,
    Block_J,
    Block_L,
    Block_S,
    Block_Z,
    // Block shadow
    Block_SD,
    // Block garbage
    Block_GB,
    // Block background
    Block_BG,
    Block_BG_DARK,

    // Block light
    Block_I_L,
    Block_T_L,
    Block_O_L,
    Block_J_L,
    Block_L_L,
    Block_S_L,
    Block_Z_L,

    // Block top
    Block_I_Top,
    Block_T_Top,
    Block_O_Top,
    Block_J_Top,
    Block_L_Top,
    Block_S_Top,
    Block_Z_Top,

    // border blocks with background color
    Block_BG_L,
    Block_BG_R,
    Block_BG_T,
    Block_BG_B,
    Block_BG_RT,
    Block_BG_RB,
    Block_BG_LB,
    Block_BG_LT
};

enum BlockProtoType{
    Block_Block,
    Block_Block_Light,
    Block_Block_Top,
    Block_Line
};