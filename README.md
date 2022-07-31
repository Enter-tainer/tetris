# Tetris

[简体中文](README.md) | [English](README-en.md)

在自制 RISC-V CPU 和 x86 上运行的现代俄罗斯方块。 [完整视频](https://www.bilibili.com/video/BV1wY4y1e7dU)

- 超级旋转系统
- bag 7 
- hold 机制
- next preview
- T spin
- 对战！

https://user-images.githubusercontent.com/25521218/162770476-865898fe-13d6-405b-ad8f-f09d6bf8c8ab.mp4

## 构建

```bash
CC=clang LD=lld cmake -B build
make
```
