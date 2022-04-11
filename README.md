# Tetris

[简体中文](README.md) | [English](README-en.md)

在自制 RISC-V CPU 和 x86 上运行的现代俄罗斯方块。

- 超级旋转系统
- bag 7 
- hold 机制
- next preview
- T spin
- 对战！

![video](assets/output.mp4)

## 构建

```bash
CC=clang LD=lld cmake -B build
make
```
